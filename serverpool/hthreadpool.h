

#ifndef __HTHREADPOOL_H__
#define __HTHREADPOOL_H__

#include "huibase.h"
#include "hipcdata.h"
#include "hthread.h"

#include "hmessagequeue.h"
#include "hmutex.h"
#include "hcond.h"

namespace HUIBASE {




class HCThreadPool {
public:	
	enum {
		MSGQ_TYPE = 101,
	};

	static HCMutex ms_mutex;
	static HCCond ms_cond;

	static HRET WaitChildThread();

	static HRET ChildReleaseLock ();

	static HINT ms_threadCreated;

	static constexpr HUINT MAX_THREAD_LEN = 1023;

	static constexpr HUINT MAX_THREAD_BUF_LEN = HLEN2048_C;

	typedef struct  {
		HCThreadPool* pPool{nullptr};
		HINT index{0};
	}stThreadInfo;

	static void* ThreadInitRun (void* argv); 

	class CThread : public HCThread{
	public:
		HRET DetachCreate (stThreadInfo* info);
		
	};


public:
	HCThreadPool(HSYS_T      msg_key, HUINT num = 128, HUINT buf_len = MAX_THREAD_BUF_LEN);

	~HCThreadPool();

	HRET Init ();

	HRET Stop () { HRETURN_OK;}


protected:
	HRET postWork (HCPSZ buf, HUINT len);

	HRET dispathWork ();

	virtual HRET workTask (const HCIpcData& data) = 0;

public:
	void SetMsgKey (HSYS_T msgKey) { m_msg.SetKey (msgKey); }

	void SetNum (HUINT num) { m_childNum = num; }

private:
	HUINT m_childNum{0};
	CThread* m_pThreads{nullptr};
	HCMessageQueue m_msg;
	const HUINT m_bufLen{MAX_THREAD_BUF_LEN};

	volatile bool m_bRunning;
	
};

}

#endif //__HTHREADPOOL_H__

