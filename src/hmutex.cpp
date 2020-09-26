

#include "hmutex.h"

namespace HUIBASE {

HCMutexAttr::HCMutexAttr() {

	auto cb = pthread_mutexattr_init(&m_attr);
	HASSERT_THROW(cb == 0, SYS_FAILED);

}


HCMutexAttr::~HCMutexAttr() {

	(void)pthread_mutexattr_destroy(&m_attr);

}

HCMutex::HCMutex () {

	auto cb = pthread_mutex_init(m_id.GetIdP(), nullptr);
	HASSERT_THROW(cb == 0, SYS_FAILED);

}

HCMutex::HCMutex (const HCMutexAttr& attr) {

	auto cb = pthread_mutex_init(m_id.GetIdP(), attr.GetAttrP());
	HASSERT_THROW(cb == 0, SYS_FAILED);

}


HCMutex::~HCMutex() {

	(void)pthread_mutex_destroy(m_id.GetIdP());

}


HRET HCMutex::Lock () { 

	auto cb = pthread_mutex_lock(m_id.GetIdP());
	HASSERT_THROW(cb == 0, SYS_FAILED);

	HRETURN_OK;

}

HRET HCMutex::UnLock () {
	auto cb = pthread_mutex_unlock(m_id.GetIdP());
	HASSERT_THROW(cb == 0, SYS_FAILED);

	HRETURN_OK;
}

CRwlockAttr::CRwlockAttr() {

	auto cb = pthread_rwlockattr_init(&m_attr);

	HASSERT_THROW( cb == 0, SYS_FAILED );

}


CRwlockAttr::~CRwlockAttr() {

	(void)pthread_rwlockattr_destroy(&m_attr);

}


  CRwlock::CRwlock() {

    auto cb = pthread_rwlock_init(m_id.GetIdP(), nullptr);
    HASSERT_THROW(cb == 0, SYS_FAILED);

  }


  CRwlock::CRwlock (const CRwlockAttr& attr ) {

    auto cb = pthread_rwlock_init(m_id.GetIdP(), attr.GetAttrP());
    HASSERT_THROW(cb == 0, SYS_FAILED);

  }


  CRwlock::~CRwlock () {

    (void) pthread_rwlock_destroy(m_id.GetIdP());

  }


  HRET CRwlock::RLock() {

    auto cb = pthread_rwlock_rdlock(m_id.GetIdP());
    HASSERT_THROW(cb == 0, SYS_FAILED );

    HRETURN_OK;

  }


  HRET CRwlock::WLock() {

    auto cb = pthread_rwlock_wrlock(m_id.GetIdP());
    HASSERT_THROW(cb == 0, SYS_FAILED);

    HRETURN_OK;

  }


  HRET CRwlock::Unlock() {

    auto cb = pthread_rwlock_unlock(m_id.GetIdP());
    HASSERT_THROW(cb == 0, SYS_FAILED);

    HRETURN_OK;
  }


}


