
#ifndef __HMESSAGEQUEUE_H__
#define __HMESSAGEQUEUE_H__

#include "huibase.h"
#include <sys/ipc.h>
#include <sys/msg.h>


namespace HUIBASE {

class HCMessageQueue {
public:
	HCMessageQueue();

	HCMessageQueue(HSYS_T key, HINT flags = IPC_CREAT | 0660);

	~HCMessageQueue();

	HRET Init (HSYS_T key, HINT flags);

	HRET Create ();

	HRET Open ();

	HRET Close ();

	HRET Remove ();

	HRET Control (HINT cmd, struct msgid_ds* mds = nullptr);

	HINT GetId () const { return m_msgid; }

	void SetId (HINT id) { m_msgid = id; }

public:

	HRET Send (const void* ptr, HSIZE size, HINT flags);

	HRET Recv (void* buf, HSIZE& size, HLONG lType, HINT flags);

public:
	void SetKey(HSYS_T msgKey) { m_key = msgKey; }

private:
	static const HSYS_T HMQ_UNINIT_KEY = -1;
	static const HINT HMQ_UNINIT_MSGID = -1;

	HSYS_T m_key;
	
	HINT m_flags;
	
	HINT m_msgid;
	
	HBOOL m_bInit;
};

}



#endif //__HMESSAGEQUEUE_H__


