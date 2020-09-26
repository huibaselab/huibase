
#include "hthreadpool.h"
#include "hipcdata.h"

namespace HUIBASE {

HINT HCThreadPool::ms_threadCreated = 0;


HRET HCThreadPool::WaitChildThread() {

	HFAILED_THROW(ms_mutex.Lock());

	while (ms_threadCreated == 0) {

		HFAILED_THROW(ms_cond.Wait(ms_mutex));

	}

	ms_threadCreated = 0;

	HFAILED_THROW(ms_mutex.UnLock());
	
	
	HRETURN_OK;
	
}


HRET HCThreadPool::ChildReleaseLock () {

	HFAILED_THROW(ms_mutex.Lock());

	ms_threadCreated = 1;

	HFAILED_THROW(ms_cond.Signal());

	HFAILED_THROW(ms_mutex.UnLock());

	HRETURN_OK;

}


void* HCThreadPool::ThreadInitRun (void* argv) {

	stThreadInfo* info = (stThreadInfo*)argv;

	HASSERT_THROW(info != nullptr, ILL_PT);
	
	HASSERT_THROW(info->pPool != nullptr, ILL_PT);

	ChildReleaseLock();

	HCThreadPool* pPool = info->pPool;
	assert(pPool);

	// do work;
	pPool->dispathWork();


	return nullptr;

}


HRET HCThreadPool::CThread::DetachCreate (stThreadInfo* info) {

	HCThreadAttr attr;
	attr.SetDetachAttr();

	auto cb = pthread_create(GetIdp(), attr.GetAttrP(), ThreadInitRun, (void*)info);

	HASSERT_THROW(cb == 0, SYS_FAILED);

	HRETURN_OK;
}


HCThreadPool::HCThreadPool (HSYS_T       msg_key, HUINT num, HUINT buf_len)
	: m_childNum(num), m_pThreads(nullptr), m_msg(msg_key), m_bufLen(buf_len)
		, m_bRunning(false){

	HASSERT_THROW(m_childNum < MAX_THREAD_LEN, INVL_PARA);

}


HCThreadPool::~HCThreadPool() {

	HDEL_ARR(m_pThreads);

	(void) m_msg.Close();

	(void) m_msg.Remove();

}



HRET HCThreadPool::Init () {

	HFAILED_THROW(m_msg.Create());

	HFAILED_THROW(m_msg.Open());

	m_pThreads = new CThread[m_childNum];
	CHECK_NEWPOINT(m_pThreads);

	stThreadInfo info;
	info.pPool = this;
	info.index = 0;

	for (HUINT i = 0; i < m_childNum; ++i) {

		CThread* pt = m_pThreads + i;

		info.index = i;

		HFAILED_THROW(pt->DetachCreate(&info));

		WaitChildThread();

	}

	HRETURN_OK;

}


HRET HCThreadPool::postWork (HCPSZ buf, HUINT len) {

	HFAILED_THROW (m_msg.Send(buf, len, 0));

	HRETURN_OK;

}


HRET HCThreadPool::dispathWork () {
		

	while (m_bRunning) {

		HCIpcData buf(m_bufLen);

		HSIZE len = buf.GetLen();
	
		HRET cb =  m_msg.Recv(buf.Begin(), len, MSGQ_TYPE, 0);

		HFAILED_THROW(cb);

		buf.MovePos(len);

		cb = workTask(buf);

		HFAILED_THROW(cb);

	}

	HRETURN_OK;
}

}


