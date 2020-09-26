

#include "hmq.h"
#include "hutil.h"

namespace HUIBASE{

	HCMq::CNodeHeader::CNodeHeader(HSNUM id, HINT len, HSNUM count, state_t state)
		: m_id(id), m_len(len), m_count(count), m_state(state) {
		
	}


	HCMq::CMemBlock::CMemBlock(HSNUM id, HUINT len, HTIME ctime,HTIME mtime) 
		: m_id(id), m_len(len), m_state(ss_end),  m_ctime(ctime), m_mtime(mtime){

		memset(m_data, 0, MAX_BLOCK_LEN);
	
	}


	HSIZE HCMq::CMemBlock::Write (HCPSZ data, HSIZE size) {

		memset(m_data, 0, MAX_BLOCK_LEN);

		HSIZE ret = size > MAX_BLOCK_LEN ? MAX_BLOCK_LEN : size;
		memcpy(m_data, data, ret);


		return ret;
	
	}

	HSIZE HCMq::CMemBlock::Read (HPSZ data, HSIZE size) const{

		memset(data, 0, size);

		HSIZE ret = size > MAX_BLOCK_LEN ? MAX_BLOCK_LEN : size;
		memcpy(data, m_data, ret);

		return ret;
	
	}

void HCMq::CMemBlock::Free () {

	m_state = ss_normal;

	SetNowTime ();

	memset(m_data, 0, MAX_BLOCK_LEN);

}

HCMq::HCMq (HSYS_T ipcKey, HUINT uMaxNodeNum, HUINT uMaxBlockNum,
	const struct SShmControl& control ) 
	: m_iIpcKey(ipcKey), m_uMaxNodeNum(uMaxNodeNum), m_uMaxBlockNum(uMaxBlockNum),
	m_control(control)
{

	assert(uMaxNodeNum > 0 && uMaxNodeNum <= MAX_SHM_ITEM_NUM);
	assert(uMaxBlockNum > 0 && uMaxBlockNum <= MAX_SHM_ITEM_NUM);

	m_info.SetCount(uMaxNodeNum);

	HUINT _len = 0;
	_len += sizeof(CShmHeader);
	_len += sizeof(CNodeHeader) * m_uMaxNodeNum;
	_len += sizeof(CMemBlock) * m_uMaxBlockNum * m_uMaxNodeNum;
	m_info.SetLen(_len);

	m_pMqArrary = new HCMessageQueue[m_uMaxNodeNum];
	assert(m_pMqArrary != NULL);

	for (HUINT i = 0; i < m_uMaxNodeNum; ++i) {

		(void)m_pMqArrary[i].Init(m_iIpcKey + i, IPC_FLAGS);
	
	}

	m_pShm = new HCSharaMem(m_iIpcKey, IPC_FLAGS, m_info.GetLen());
	assert(m_pShm != nullptr);

	m_pSem = new HCSemaphore(m_iIpcKey, IPC_FLAGS, m_uMaxNodeNum);
	assert(m_pSem != nullptr);

}


HCMq::~HCMq() {

	close();

	delete [] m_pMqArrary;
	delete m_pShm;
	delete m_pSem;

}


HRET HCMq::Init () {

	HRET cb = HERR_NO(OK);

	cb = create();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "create ipc Failed", cb);

	cb = open();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "open ipc Failed", cb);

	initMem();
	
	HRETURN_OK;
	
}


HRET HCMq::Clear () {

	HRET cb = HERR_NO(OK);

	cb = close();

	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Close ipc Failed", cb);

	for (HUINT i = 0; i < m_uMaxNodeNum; ++i) {

		HCMessageQueue* pmq = m_pMqArrary + i;

		cb = pmq->Remove();

		HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Remove MessageQueue Failed", cb);
	
	}

	cb = m_pShm->Remove();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Remove ShareMemory Failed", cb);

	cb = m_pSem->Remove();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Remove Semphore Failed", cb);

	HRETURN_OK;
}


void HCMq::initMem() {

	for (HUINT i = 0; i < m_uMaxNodeNum; ++i) {

		CNodeHeader* node = getNodeHeader(i);

		node->SetId(i);
		node->SetLen(sizeof(CNodeHeader) + sizeof(CMemBlock) * m_uMaxBlockNum);
		node->SetCount(m_uMaxBlockNum);
		node->SetState(ss_normal);

		for (HUINT j = 0; j < m_uMaxBlockNum; ++j) {

			CMemBlock* block = getMemHeader(i,j);
			memset(static_cast<void*>(block), 0, sizeof(CMemBlock));
			block->SetId(j);
			block->SetLen(sizeof(CMemBlock));
			block->SetState(ss_normal);
			block->SetCTime(time(nullptr));
			block->SetMTime(time(nullptr));

		}

	}

}


HCMq::CNodeHeader* HCMq::getNodeHeader (HUINT index) {

	if (index > m_uMaxNodeNum) return nullptr;

	char * pos = m_ptr;
	pos += sizeof(CShmHeader);

	pos += (sizeof (CNodeHeader)  + sizeof(CMemBlock) * m_uMaxBlockNum) * index;

	return (CNodeHeader*) pos;

}


HCMq::CMemBlock* HCMq::getMemHeader (HUINT nid, HUINT bid) {

	if (nid > m_uMaxNodeNum || bid > m_uMaxBlockNum) return nullptr;

	char* pos = (char*)getNodeHeader(nid);

	pos += sizeof(CNodeHeader)  + sizeof(CMemBlock) * bid;

	return (CMemBlock*) pos;

}


HCMq::CMemBlock* HCMq::getFreeMem (CMsgControl& control) {

	if (control.GetNid() > m_uMaxNodeNum) return nullptr;

	char* pos = (char*) getNodeHeader(control.GetNid());

	pos += sizeof(CNodeHeader);

	CMemBlock* block = (CMemBlock*)pos;

	for (HUINT i = 0; i < m_uMaxBlockNum; ++i) {

		block += i;

		if (check_ss_free(block->GetState())) {

			SHM_SET_BUSY(block);

			block->SetNowTime ();

			control.SetBid(i);
			
			return block;

		}

	}

	return nullptr;

}


HRET HCMq::resetBusyMem (CMsgControl& control) {

    if (control.GetNid() > m_uMaxNodeNum) {

	HRETURN(INVL_PARA) ;

    }

    CMemBlock* block = getMemHeader(control.GetNid(), control.GetBid());
    assert(block);

    HNOTOK_RETURN(m_pSem->Lock(control.GetNid()));

    block->Free();

    HNOTOK_RETURN(m_pSem->Unlock(control.GetNid()));

    HRETURN_OK;
}



HRET HCMq::create () {

	HRET cb = m_pShm->Create();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb) || (cb == HERR_NO(RECREATE)), "Create share memory failed", cb);

	for (HUINT i = 0; i < m_uMaxNodeNum; ++i) {

		HCMessageQueue* pmq = m_pMqArrary + i;

		for (int ii = 0; ii < 2; ++ii) {

			cb = pmq->Create();

			HASSERT_MSG_THROW_ERR(HIS_OK(cb) || (cb == HERR_NO(RECREATE)) || (errno == EEXIST), "Message Queue Create Failed",  cb);

			if (ii == 0) {
				
				(void)pmq->Remove();

				//HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Remove message queue failed",  cb);

				continue;
			}

			break;
			
		}		
	
	}

	HRET cb_ = m_pSem->Create();

	HASSERT_MSG_THROW_ERR(HIS_OK(cb_) || (cb_ == HERR_NO(RECREATE)), "Create Semphore failed", cb_);

	HRETURN_OK;

}

HRET HCMq::open () {

	HRET cb = m_pSem->Open();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Semphore Open Failed", cb);

	for (HUINT i = 0; i < m_uMaxNodeNum; ++i) {

		HCMessageQueue* pmq = m_pMqArrary + i;

		cb = pmq->Open();

		HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Message Queue Open Failed", cb);

	}

	cb = m_pShm->Open_and_attach();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Share Memory Open Failed", cb);

	m_ptr = (char*)m_pShm->Get_shm_ptr();
	m_pInfoHeader = (CShmHeader*)m_ptr;

	m_pInfoHeader->SetCount(m_info.GetCount());
	m_pInfoHeader->SetLen(m_info.GetLen());
	
	HRETURN_OK;

}

    HRET HCMq::Send (CMsgControl& control, const char* data, HINT iFlags) {

	if (control.GetNid() > m_uMaxNodeNum || control.GetBid() > m_uMaxBlockNum
		|| control.GetLen() > MAX_BLOCK_LEN	) {
		
		HRETURN(INVL_PARA);
		
	}

	HNOTOK_RETURN(m_pSem->Lock(control.GetNid()));

	CMemBlock* block = getFreeMem(control);
	
	block->SetLen(control.GetLen());
	(void)block->Write(data, control.GetLen());

	HNOTOK_RETURN(m_pSem->Unlock(control.GetNid()));

	HCMessageQueue* pmq = m_pMqArrary + control.GetNid();

	do {

		for (HINT i = 0; i < m_control.m_iSendTryTime; ++i) {

			HRET cb_i = pmq->Send((void*)&control, sizeof(control), iFlags | IPC_NOWAIT);

			if (HIS_OK(cb_i)) {

				HRETURN_OK;

			}

			if (EAGAIN == errno) {

				hsleep(0, m_control.m_iSendSleepTimeU);

				continue;
			
			}

			break;

		}

	} while(0);
	
	resetBusyMem(control);
	
	HRETURN(HBASE_FAILED);

}


HRET HCMq::Recv (CMsgControl& control, char* data,
		HINT iTimeOut, HINT iFlags) {

	if (control.GetNid() > m_uMaxNodeNum || control.GetLen() > MAX_BLOCK_LEN || 
		data == nullptr) {
		
		HRETURN(INVL_PARA);	

	}

	signal(SIGALRM, AlarmHander);
	alarm(iTimeOut);

	do {

		HCMessageQueue* pmq = m_pMqArrary + control.GetNid();
		assert(pmq);

		HSIZE slen = sizeof(control);
		HRET _cb = pmq->Recv((void*)&control, slen, control.GetType(), iFlags);

		if (HNOT_OK(_cb)) {

			alarm(0);

			if (errno == EINTR) {
				HRETURN(TIME_OUT);
			}

			return _cb;
	
		}	

	} while(0);

	alarm(0);

		
	CMemBlock* block = getMemHeader(control.GetNid(), control.GetBid());

	HRET ret = HERR_NO(OK);
	do {

		if (not CheckMemBlock(*block, control)) {

			// the content of share memory is invalid.
			ret = HERR_NO(ERR_STATUS);

			break;
		
		}

		// expire time check
		time_t tnow = time(NULL);
		if (tnow - block->GetMTime() > m_control.m_iQueueValidTime) {

			ret = HERR_NO(DATA_TOO_OLD);

			break;
		
		}

		block->Read(data, control.GetLen());
                //memcpy(data, block, control.GetLen());
	

	} while (0);
	
	resetBusyMem(control);

	return ret;

}


HRET HCMq::close () {

	HRET cb = HERR_NO(OK);

	for (HUINT i = 0; i < m_uMaxNodeNum; ++i) {

		HCMessageQueue* pmq = m_pMqArrary + i;

		cb = pmq->Close();

		HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Close MessageQueue Failed", cb);
	
	}

	cb = m_pShm->Dettach();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Close ShareMemory Failed", cb);

	cb = m_pSem->Close();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Close Semphore Failed", cb);


	HRETURN_OK;

}

HRET HCMq::remove () {

	HRET cb = HERR_NO(OK);

	for (HUINT i = 0; i < m_uMaxNodeNum; ++i) {

		HCMessageQueue* pmq = m_pMqArrary + i;

		cb = pmq->Remove();

		HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Remove MessageQueue Failed", cb);

	}

	cb = m_pShm->Remove();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Remove ShareMemory Failed", cb);

	cb = m_pSem->Remove();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Remove Semphore Failed", cb);

	
	HRETURN_OK;

}	

void HCMq::AlarmHander (int sig) {

	(void) sig;

	signal(SIGALRM, AlarmHander);

}


}


