
#ifndef __HPROCESS_H__
#define __HPROCESS_H__

#include <huibase.h>

namespace HUIBASE {

class HCProcess {
public:
	using hpid_t = pid_t;
public:



public:
	static HBOOL ProcessExists (const hpid_t& pid);

	static HRET KillProcess (const hpid_t& pid);

	static HRET GetProcName (HSTRR strName);

};

}

#endif // __HPROCESS_H__


