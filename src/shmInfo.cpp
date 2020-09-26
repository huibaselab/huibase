
#include "shmInfo.h"

namespace HUIBASE {

void CShmInfo::CInfo::ClearUp () {

	m_state = si_free;
	m_index = m_sid = m_classid = m_ctime = m_access = 0;

}


CShmInfo::CShmInfo(HSYS_T key, HUINT count) 
	: m_key(key){

	assert(count < HSI_MAX_COUNT);

	m_control.SetCount(count);
	m_control.SetLen(sizeof(CControlInfo) + sizeof(CInfo) * count);

	m_sem = new HCSemaphore(m_key, IPC_FLAGS, esi_sem);
	assert(m_sem);

	m_shm = new HCSharaMem(m_key, IPC_FLAGS, m_control.GetLen());
	assert(m_shm);

}


CShmInfo::~CShmInfo() {


}


HRET CShmInfo::InitData () {

	CInfo* pInfo = m_pInfo;

	for (HUINT i = 0; i < m_control.GetCount(); ++i) {

		pInfo->SetState(si_free);

		pInfo->SetIndex(i);

		pInfo->SetCId(0);

		pInfo->SetSid(0);

		pInfo->SetCTime(time(nullptr));

		pInfo->SetNowAccess();

		++pInfo;
	
	}

	HRETURN_OK;
}


HUINT CShmInfo::NewPidState (HUINT sid, HUINT cid, HPID_T pid) {

	SEMLOCK(m_sem, esi_this);

	CInfo* pInfo = m_pInfo;

	for (HUINT i = 0; i < m_control.GetCount(); ++i) {

		pInfo = pInfo + i;

		if (pInfo->GetState() == si_free) {

			pInfo->SetSid(sid);

			pInfo->SetCId(cid);

			pInfo->SetPid(pid);

			pInfo->SetNowAccess();

			pInfo->SetState(si_running);

			return i;
		
		}

	}

	HASSERT_THROW(1, POS_END);
	return 0;

}

HRET CShmInfo::UpdateContinue (HUINT index) {

	HASSERT_THROW(index < HSI_MAX_COUNT, INVL_PARA);

	SEMLOCK(m_sem, esi_this);

	CInfo* pInfo = m_pInfo + index;

	pInfo->SetNowAccess();

	HRETURN_OK;
}


HRET CShmInfo::ClearStateConf (HUINT index) {

	HASSERT_THROW(index < HSI_MAX_COUNT, INVL_PARA);

	SEMLOCK(m_sem, esi_this);

	CInfo* pInfo = m_pInfo + index;

	pInfo->ClearUp();

	HRETURN_OK;

}


void CShmInfo::FreeInfo (HUINT index) {

	SEMLOCK(m_sem, esi_this);

    CInfo* pInfo =	m_pInfo + index;

	assert(pInfo != nullptr);

	pInfo->SetFree();

}


HRET CShmInfo::Create () {

	HRET cb = m_sem->Create();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb) || (cb == HERR_NO(RECREATE)), "Create Semphore failed.", cb);

	cb = m_shm->Create();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb) || (cb == HERR_NO(RECREATE)), "Create share memory failed", cb);

	HRETURN_OK;

}

HRET CShmInfo::Open () {

	HRET cb = m_sem->Open();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Semphore Open Failed", cb);

	cb = m_shm->Open_and_attach();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Share Memory Open Failed", cb);

	m_ptr = (HPSZ)m_shm->Get_shm_ptr();
	m_pControlInfo = (CControlInfo*)m_ptr;

	m_pControlInfo->SetCount(m_control.GetCount());
	m_pControlInfo->SetLen(m_control.GetLen());

	m_pInfo = (CInfo*)(m_ptr + sizeof(CControlInfo));

	HRETURN_OK;

}

HRET CShmInfo::Close () {

	HRET cb = m_sem->Close();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Close Semphore failed", cb);

	cb = m_shm->Dettach();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Dettach share memory failed", cb);

	HRETURN_OK;

}

HRET CShmInfo::Remove () {

	HRET cb = m_shm->Remove();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Remove ShareMemory Failed", cb);

	cb = m_sem->Remove();
	HASSERT_MSG_THROW_ERR(HIS_OK(cb), "Remove Semphore Failed", cb);


	HRETURN_OK;

}



}

