

#ifndef __SHMINFO_H__
#define __SHMINFO_H__

#include "huibase.h"
#include "hshm.h"
#include "hsem.hpp"
#include "hprocess.h"

namespace HUIBASE {

class CShmInfo {
public:
	using HPID_T = HCProcess::hpid_t;
public:
	enum si_state {
		si_free,
	
		si_stop,

		si_running,

		si_end,
	};

	enum esi_sem{
		esi_this,

		esi_sem,
	};

	class CControlInfo {
	public:
		CControlInfo(HUINT len = 0, HUINT count = 0) 
			: m_len(len), m_count(count) 
		{}

		HUINT GetLen () const { return m_len; }
		void SetLen (HUINT len) { m_len = len; }

		HUINT GetCount () const { return m_count; }
		void SetCount (HUINT count) { m_count = count; }
		
	private:
		HUINT m_len {0};
		HUINT m_count {0};
	};

	class CInfo {
	public:
		CInfo (HUINT index = 0, si_state state = si_stop, HTIME ctime = 0,
			HTIME last_access = 0) 
			:  m_state(state), m_index(index), m_ctime(ctime), m_access(last_access)
		{}

		void SetFree() { m_state = si_free; }

		HUINT GetIndex() const { return m_index; }
		void SetIndex (HUINT index) { m_index = index; }

		si_state GetState () const { return m_state; }
		void SetState (si_state state) { m_state = state; }

		HTIME GetCTime () const { return m_ctime; }
		void SetCTime (HTIME ctime ) { m_ctime = ctime; }

		HTIME GetAccess () const { return m_access; }
		void SetAccess (HTIME last_access) { m_access = last_access; }
		void SetNowAccess () { m_access = time(nullptr); }

		HPID_T GetPid () const { return m_pid; }
		void SetPid (HPID_T pid) { m_pid = pid; }

		HTID_T GetTid () const { return m_tid; }
		void SetTid (HTID_T tid) { m_tid = tid; }

		void SetCId (HUINT cid) { m_classid = cid; }
		HUINT GetCid () const { return m_classid; }

		void SetSid (HUINT sid) { m_sid = sid; }
		HUINT GetSid () const { return m_sid; }

		void ClearUp ();

	private:
		si_state m_state = si_free;
		HUINT m_index{0};			// config number
		HUINT m_sid{0};				// server number
		HUINT m_classid{0};			// server type number
		HTIME m_ctime{0};			// create time
		HTIME m_access{0};			// last modify time
		HPID_T m_pid{0};			// process number
		HTID_T m_tid{0};			// thread number
	};

public:
	static const HUINT HSI_MAX_COUNT = 1024;
	static const HSYS_T IPC_FLAGS = 0777;

	CShmInfo (HSYS_T key, HUINT count = HSI_MAX_COUNT);

	~CShmInfo ();

	HRET InitData () ;

	HUINT NewPidState (HUINT sid, HUINT cid, HPID_T pid) ;

	HRET UpdateContinue (HUINT index) ;

	HRET ClearStateConf (HUINT index);

public:
	HUINT GetLen () const { return m_control.GetLen(); }
	
	HUINT GetCount () const { return m_control.GetCount(); }

	const CInfo* GetInfo (HUINT index) const { return m_pInfo + index; }

	void FreeInfo (HUINT index);

public:

	HRET Create () ;

	HRET Open () ;

	HRET Close () ;

	HRET Remove () ;

private:

	HSYS_T m_key{0};

	CControlInfo m_control;

	HCSharaMem* m_shm = nullptr;
	HCSemaphore* m_sem = nullptr;

	HPSZ m_ptr = nullptr;
	CControlInfo* m_pControlInfo = nullptr;
	CInfo* m_pInfo = nullptr;
	
};

}


#endif //__SHMINFO_H__


