
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "hstr.h"

#include "hprocess.h"
#include "hstr.h"



namespace HUIBASE {

HBOOL HCProcess::ProcessExists (const hpid_t& pid) {

	HSTR str = HCStr::Format("/proc/%d", pid);

	struct stat st;
	if (stat(str.c_str(), &st) < 0) {
		return HFALSE;
	}

	return HTRUE;
}


HRET HCProcess::KillProcess (const hpid_t& pid) {

#ifdef HWINDOWS
	HCStr str = HCStr::Format("kill -15 %d", (int)pid);
	system(str.c_str());
	//(void)execl("/usr/bin/kill", "kill", "15", HCStr::itos(pid).c_str(), nullptr);
	//(void)execl("/bin/kill", "kill", "15", HCStr::itos(pid).c_str(), nullptr);
	
#else
	
	auto cb = kill(pid, SIGTERM);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

#endif 
	

	HRETURN_OK;

}


HRET HCProcess::GetProcName(HSTRR strName) {

    HCPSZ csz = "/proc/self/exe";

    size_t linksize = 256;

    HCHAR buf[linksize] = {0};

    if (readlink(csz, buf, linksize) != -1) {

	strName = buf;

	HRETURN_OK;

    }

    HRETURN(SYS_FAILED);

}


}










