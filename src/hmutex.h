/****************************************************************************
 *
 * File Name: hmutex.h
 *
 * Create Author: Tom Hui, tomhui1009@yahoo.com, 8613760232170
 *
 * Create Date: Thu Dec 08 16:16 2016
 *
 * Description:
 *	pthread mutex lock.
 *
 ****************************************************************************/

#ifndef __HMUTEX_H__
#define __HMUTEX_H__

#include "huibase.h"
#include "hthreadid_t.h"
#include <pthread.h>
#include "hcond.h"

namespace HUIBASE {


class HCMutexAttr{
public:
	using attr_type = pthread_mutex_t;
	using attr_type_ptr = pthread_mutex_t*;

	HCMutexAttr ();

	HCMutexAttr (const HCMutexAttr& attr) = delete;

	~ HCMutexAttr();

	const pthread_mutexattr_t* GetAttrP ()const { return &m_attr; }

private:
	pthread_mutexattr_t m_attr;
};

class HCMutex {
public:
	friend class HCCond;
	using handle_type = pthread_mutex_t;

	using mid_t = cid<handle_type>;

	HCMutex () ;

	HCMutex (const HCMutexAttr& attr) ;

	HCMutex (const HCMutex& ) = delete;

	HCMutex (HCMutex&& other)   noexcept: m_id(other.m_id)  {
		other.m_id = mid_t{};
	}

	virtual ~ HCMutex();

    //mid_t GetMid () const { return m_id; }

	HRET Lock () ;

	HRET UnLock () ;

private:
	mid_t m_id;

};


class CRwlockAttr {
public:
	using attr_type = pthread_rwlock_t;
	using attr_type_ptr = pthread_rwlock_t*;

	CRwlockAttr ();

	CRwlockAttr (const CRwlockAttr& attr ) = delete;

	~ CRwlockAttr ();

    const pthread_rwlockattr_t* GetAttrP () const { return &m_attr; }

private:
	pthread_rwlockattr_t m_attr;


};


 class CRwlock {
 public:
	 using handle_type = pthread_rwlock_t;
	 using id_t = cid<handle_type>;

	 CRwlock ();

	 CRwlock (const CRwlockAttr& attr);

	 CRwlock (const CRwlock& rwlock ) = delete;

	 CRwlock (CRwlock&& rw ) noexcept : m_id(rw.m_id) {

		 rw.m_id = id_t {};

	 }

	 virtual ~ CRwlock ();

	 HRET RLock ();

	 HRET WLock ();

	 HRET Unlock ();


 private:
	 id_t m_id;
 };

 template <class T>
 class CMutexHolder {
 public:
     typedef T value_type;
     typedef T& ref_type;

 public:
 CMutexHolder(ref_type ins) : m_ref (ins) {
         m_ref.Lock();
     }

     ~CMutexHolder () { m_ref.UnLock(); }

 private:
     ref_type m_ref;
 };

 typedef CMutexHolder<HCMutex> MUTEXHOLDER;

template <class T>
class READLOCK {
public:
    typedef T value_type;
    typedef T& ref_type;
    READLOCK (ref_type ins): m_ref(ins) {
        m_ref.RLock ();
    }

    ~ READLOCK () {
        m_ref.Unlock ();
    }

private:
    ref_type m_ref;
};

template <class T>
class WRITELOCK{
public:
typedef T value_type;
 typedef T& ref_type;
 WRITELOCK (ref_type ins) : m_ref(ins) {
    m_ref.WLock ();
}

 ~ WRITELOCK () {
    m_ref.Unlock ();
}

private:
 ref_type m_ref;
};
}

#endif //__HMUTEX_H__



