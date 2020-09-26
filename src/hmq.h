


#ifndef __HMQ_H__
#define __HMQ_H__

#include "huibase.h"

#include "hmessagequeue.h"
#include "hshm.h"
#include "hsem.hpp"


namespace HUIBASE {

class HCMq {
public:

	static const HINT MAX_NODE_LEN = HLEN80960_C;	// max buffer length for node of share memory.
	static const HINT MAX_BLOCK_LEN = HLEN8096_C;	// max buffer length for block of share memory.
	static const HUINT MAX_ID = HMAX_UINT;
	static const HUINT MAX_LEN = HMAX_UINT;

	static const HSYS_T IPC_FLAGS = 0777;
	static const HUINT MAX_SHM_ITEM_NUM = 129;
	

	enum shm_state {
		ss_normal,
		ss_busy,
		ss_waitting,

		ss_error,

		
		ss_end
	};

	inline bool check_ss_free(shm_state state) { return state == ss_normal; }

	inline bool check_ss_busy (shm_state state) { return state == ss_busy; }

#ifndef SHM_SET_FREE
#define _SHM_SET_FREE_(p) do { (p)->SetState(ss_normal);  } while(0)
#define SHM_SET_FREE(p) _SHM_SET_FREE_(p) 

#define _SHM_SET_BUSY_(p) do { (p)->SetState(ss_busy);  } while(0)
#define SHM_SET_BUSY(p) _SHM_SET_BUSY_(p) 
#endif

	using state_t = shm_state;

	class CShmHeader {
	public:
		CShmHeader(HUINT len = 0, HSNUM count = 0) 
			: m_len (len), m_count(count) {}

		HUINT GetLen () const { return m_len; }
		void SetLen(HUINT len) { m_len = len; }

		HSNUM GetCount () const { return m_count; }
		void SetCount (HSNUM count ) { m_count  = count; }
			
	private:
		HUINT m_len = 0;
		HSNUM m_count = 0;
	};

	class CNodeHeader {
	public:
		CNodeHeader(HSNUM id      = MAX_ID, HINT len = MAX_NODE_LEN, HSNUM count = 0,
			state_t state = ss_normal);

		HSNUM GetId () const { return m_id; }
		void SetId (HSNUM id) { m_id = id; }

		HUINT GetLen () const { return m_len; }
		void SetLen (HUINT len) { m_len = len; }

		HSNUM GetCount () const { return m_count; }
		void SetCount(HSNUM count ) { m_count = count; }

		state_t GetState () const { return m_state; } 
		void SetState (state_t state) { m_state = state; }


	private:
		HSNUM m_id = MAX_ID;
		HUINT m_len = MAX_NODE_LEN;
		HSNUM m_count = 0;
		state_t m_state = ss_normal;		
		
		
	};

	class CMsgControl {
	public:
		CMsgControl (HSNUM nid = 0, HSNUM bid = 0, HUINT len = 0, HTIME ctime = 0)
			: m_nid(nid), m_bid(bid), m_len(len), m_ctime(ctime) { }

		HSNUM GetNid () const { return m_nid; }
		void SetNid (HSNUM nid) { m_nid = nid; }

		HSNUM GetBid () const { return m_bid; }
		void SetBid (HSNUM bid ) { m_bid = bid; };

		HUINT GetLen () const { return m_len; }
		void SetLen (HUINT len) { m_len = len; }

		HTIME GetCTime () const { return m_ctime; }
		void SetCTime (HTIME ctime ) { m_ctime = ctime; }

		HLONG GetType () const { return m_type; }
		void SetType (HLONG nType) { m_type = nType; }

		HLONG GetRType () const { return m_rtype; }
		void SetRType (HLONG lType) { m_rtype = lType; }

		void SetReturn () { m_type = m_rtype; }
		

	private:
		HLONG m_type = 0;
		HSNUM m_nid = MAX_ID;
		HSNUM m_bid = MAX_ID;
		HUINT m_len = MAX_LEN;
		HLONG m_rtype = 0;
		HTIME m_ctime = 0;
	};

	class CMemBlock {
	public:
		CMemBlock(HSNUM id = MAX_ID, HUINT len = MAX_LEN, HTIME ctime = 0,
			HTIME mtime = 0);

		HSNUM GetId () const { return m_id; }
		void SetId (HSNUM id)  { m_id = id; }
		
		HUINT GetLen () const { return m_len; }
		void SetLen (HUINT len ) { m_len = len; }

		HTIME GetCTime () const { return m_ctime; }
		void SetCTime (HTIME ctime) { m_ctime = ctime; }

		HTIME GetMTime () const { return m_mtime; }
		void SetMTime (HTIME mtime) { m_mtime = mtime; }
		void SetNowTime () { m_mtime = time(nullptr); }

		shm_state GetState () const { return m_state; }
		void SetState (shm_state state) { m_state = state; }

		HSIZE Write (HCPSZ data, HSIZE size);

		HSIZE Read (HPSZ data, HSIZE size) const;

		void Free ();

	private:
		HSNUM m_id = MAX_ID;
		HUINT m_len = MAX_LEN;
		shm_state m_state = ss_end;
		HTIME m_ctime = 0;
		HTIME m_mtime = 0;
		HCHAR m_data[MAX_BLOCK_LEN] = {0};
	};

	inline bool CheckMemBlock (const CMemBlock& block, const CMsgControl& control) {

		if (not check_ss_busy(block.GetState())) {

			return false;
		
		}
		
		if (not (block.GetId() == control.GetBid()) ) return false;

		if (not (block.GetLen() == control.GetLen()) ) return false;

		return true;
	
	}

	struct SShmControl { 
		SShmControl (HINT validt = 0, 
			HINT tryt = 0, HINT sleept = 0, HINT sleeput = 0) 
			: m_iQueueValidTime(validt), 
			m_iSendTryTime(tryt),
			m_iSendSleepTime(sleept),
			m_iSendSleepTimeU(sleeput) {} 
	
		HINT m_iQueueValidTime = 0;
		HINT m_iSendTryTime = 0;
		HINT m_iSendSleepTime = 0;
		HINT m_iSendSleepTimeU = 5;
	};

	

public:
	HCMq (HSYS_T ipcKey, HUINT uMaxNodeNum, HUINT uMaxBlockNum,
		const struct SShmControl& control );

	~ HCMq();

public:

	HRET Init () ;

	HRET Clear () ;

	HRET Send (CMsgControl& control, const char* data, HINT iFlags = 0) ;

	HRET Recv (CMsgControl& control, char* data,
		   HINT iTimeOut, HINT iFlags = 0) ;

	HSNUM GetCount () const { return m_info.GetCount(); }

	HUINT GetLength () const { return m_info.GetLen(); } 

private:

	void initMem();

	CNodeHeader* getNodeHeader (HUINT index);

	CMemBlock* getMemHeader (HUINT nid, HUINT bid);

	CMemBlock* getFreeMem (CMsgControl& control);

	HRET resetBusyMem (CMsgControl& control);

public:
	HRET Open () { return open(); }

	HRET Close() { return close(); }
	

protected:

	HRET create () ;

	HRET open () ;

	HRET close () ;

	HRET remove () ;

private:
	static void AlarmHander (int sig);

private:


	HSYS_T m_iIpcKey = IPC_FLAGS;

	HUINT m_uMaxNodeNum = MAX_SHM_ITEM_NUM;
	HUINT m_uMaxBlockNum = MAX_SHM_ITEM_NUM;


	SShmControl m_control;

	CShmHeader m_info = {0};

	HCMessageQueue* m_pMqArrary = nullptr;
	HCSharaMem* m_pShm = nullptr;
	HCSemaphore* m_pSem = nullptr;

	char* m_ptr = nullptr;
	CShmHeader* m_pInfoHeader = nullptr;
	

};

}

#endif // __HMQ_H__




