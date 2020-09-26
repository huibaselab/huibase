

#include "hcond.h"


namespace HUIBASE {

HCCondAttr::HCCondAttr() {

	auto cb = pthread_condattr_init(&m_attr);
	HASSERT_THROW(cb == 0, SYS_FAILED);

}


HCCondAttr::~HCCondAttr() {

	(void)pthread_condattr_destroy(&m_attr);

}

HCCond::HCCond () { 

	auto cb = pthread_cond_init(m_id.GetIdP(), nullptr);
	HASSERT_THROW(cb == 0, SYS_FAILED);

}


HCCond::HCCond (const HCCondAttr& attr) {

	auto cb = pthread_cond_init(m_id.GetIdP(), attr.GetAttrP());
	HASSERT_THROW(cb == 0, SYS_FAILED);
	
}

HCCond::~HCCond() {
	
	(void)pthread_cond_destroy(m_id.GetIdP());

}


HRET HCCond::Signal () {

	auto cb = pthread_cond_signal(m_id.GetIdP());
	HASSERT_THROW(cb == 0, SYS_FAILED);

	HRETURN_OK;

}

HRET HCCond::Broatcast () {
	auto cb = pthread_cond_broadcast(m_id.GetIdP());
	HASSERT_THROW(cb == 0, SYS_FAILED);

	HRETURN_OK;
}

HRET HCCond::Wait (HCMutex& mutex) {

	auto cb = pthread_cond_wait(m_id.GetIdP(), mutex.m_id.GetIdP());
	HASSERT_THROW(cb == 0, SYS_FAILED);

	HRETURN_OK;

}

}



