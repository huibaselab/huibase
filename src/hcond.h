
#ifndef __HCOND_H__
#define __HCOND_H__

#include "huibase.h"
#include <pthread.h>
#include "hmutex.h"

namespace HUIBASE {

class HCMutex;
class HCCondAttr {
public:
	HCCondAttr () ;

	HCCondAttr (const HCCondAttr& attr) = delete;

	~ HCCondAttr();

	const pthread_condattr_t* GetAttrP ()const { return &m_attr; }


private:
	pthread_condattr_t m_attr;
};


class HCCond {
public:
	using handle_type = pthread_cond_t;

	using cid_t = cid<handle_type> ;

	HCCond () ;

	HCCond (const HCCondAttr& attr) ;

	HCCond (const HCCond& ) = delete;

	HCCond (HCCond&& other) noexcept : m_id(other.m_id) {
		other.m_id = cid_t{};
	}

	virtual ~ HCCond();

	HRET Signal () ;

	HRET Broatcast () ;

	HRET Wait (HCMutex& mutex) ;

private:
	cid_t m_id;

};



}

#endif



