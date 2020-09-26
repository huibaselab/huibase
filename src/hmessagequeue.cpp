

#include "hmessagequeue.h"


namespace HUIBASE {


HCMessageQueue::HCMessageQueue()
	: m_key(HMQ_UNINIT_KEY), m_flags(0), m_msgid(HMQ_UNINIT_MSGID), m_bInit(HFALSE) {

}

HCMessageQueue::HCMessageQueue(HSYS_T key, HINT flags) {

	(void)Init(key, flags);

}


HCMessageQueue::~HCMessageQueue() {

	(void)Close();

}


HRET HCMessageQueue::Init (HSYS_T key, HINT flags) {

	m_key = key;

	m_flags = flags;

	m_msgid = HMQ_UNINIT_MSGID;

	m_bInit = HTRUE;

	HRETURN_OK;

}


HRET HCMessageQueue::Create () {

	if (HIS_FALSE(m_bInit)) {

		HRETURN(UN_INIT);
	
	}

	m_msgid = msgget(m_key, m_flags | IPC_CREAT | IPC_EXCL);
	HASSERT_RETURN(m_msgid >= 0, SYS_FAILED);

	HRETURN_OK;

}


HRET HCMessageQueue::Open () {

	if (HIS_FALSE(m_bInit)) {

		HRETURN(UN_INIT);
	
	}

	m_msgid = msgget(m_key, m_flags);
	HASSERT_RETURN(m_msgid > 0, SYS_FAILED);

	HRETURN_OK;

}


HRET HCMessageQueue::Close () {

	HRETURN_OK;

}


HRET HCMessageQueue::Remove () {

	return Control(IPC_RMID);

}


HRET HCMessageQueue::Control (HINT cmd, struct msgid_ds* mds) {

	auto cb = msgctl(m_msgid, cmd, (struct msqid_ds*)mds);
	
	if (cb < 0) {
		HRETURN(SYS_FAILED);
	}
	
	//HASSERT_RETURN(msgctl(m_msgid, cmd, (struct msqid_ds*)mds) >= 0, SYS_FAILED);

	HRETURN_OK;

}

HRET HCMessageQueue::Send (const void* ptr, HSIZE size, HINT flags) {

	HASSERT_RETURN(msgsnd(m_msgid, ptr, size, flags) >= 0, SYS_FAILED);

	HRETURN_OK;

}


HRET HCMessageQueue::Recv (void* buf, HSIZE& size, HLONG lType, HINT flags) {

	auto cb = msgrcv(m_msgid, buf, size, lType, flags);
	HASSERT_RETURN(cb >= 0, SYS_FAILED);

	size = cb;

	HRETURN_OK;

}


}



