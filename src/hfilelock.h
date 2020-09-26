

#ifndef __HFILELOCK_H__
#define __HFILELOCK_H__

#include <huibase.h>

namespace HUIBASE {

class HCFileLock {
public:
	HCFileLock (HCSTRR strPathFileName);

	~HCFileLock ();

public:


	HRET WriteLockW (HINT iOffset, HINT iLen = 1, HINT iWhere = SEEK_SET) {
		return Fcntl(F_SETLKW, F_WRLCK, iOffset, iLen, iWhere);
	}

	HRET UnLock (HINT iOffset, HINT iLen = 1, HINT iWhere = SEEK_SET) {
		return Fcntl(F_SETLK, F_UNLCK, iOffset, iLen, iWhere);
	}

private:
	HRET Fcntl (HINT iCmd, HINT iType, HINT offset, HINT iLen, HINT iWhere);

private:
	HSYS_T m_fd{0};
};

class CAppLockException : public HCException{
public:
	CAppLockException(HINT err = HERR_NO(OK), HBOOL bComm = HTRUE, HBOOL bError = HFALSE,
	             HINT iLineNo = 0, HCPSZ szFile = NULL);

};


class CAppLock {
public:
	CAppLock() = default;
	
	CAppLock (HCPSZ szFileName) ;

	HRET LockApp(HCPSZ szFileName = ".lock")  ;

    HRET UnlockApp ();
	
	~ CAppLock();

private:
	int m_fd{0};
};


class CProcessLock {
public:
    CProcessLock () = default;

    HRET LockProcess (HCPSZ szFileName = ".lock") ;

    ~CProcessLock ();

private:
    int m_fd {0};

};

}

#endif //__HFILELOCK_H__









