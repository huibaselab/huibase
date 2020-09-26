


#ifndef __HEXCEPTION_H__
#define __HEXCEPTION_H__

#include "huibase.h"

namespace HUIBASE
{

namespace
{
#define MAX_EX_LEN HLEN8_C
}

class HCException : public std::exception
{
public:
	HCException (HINT err = HERR_NO(OK), HBOOL bComm = HTRUE, HBOOL bError = HFALSE,
	             HINT iLineNo = 0, HCPSZ szFile = NULL);

	HCException (HCSTRR strErrMsg, HINT err = HERR_NO(OK), HBOOL hComm = HFALSE,
	             HBOOL bError = HFALSE, HINT iLineNo = 0, HCPSZ szFile = NULL);

	virtual ~HCException () throw ();

public:

	virtual HCPSZ what() const throw()
	{

		return m_str.c_str();

	}

	virtual HINT GetHErr () const { return m_err;}


protected:
	void appendLineNum ();


private:

	HINT m_err;
	HBOOL m_bComm;
	HBOOL m_bError;
	HINT m_iLineno;
	HSTR m_strFile;
	HSTR m_str;

};

#ifndef HASSERT_EXCEPTION
#define HASSERT_EXCEPTION

namespace {
#define __IN__VAL_THROW(ff,val,berr) do { \
	if (! (ff)) { \
	    throw HCException((val), HTRUE, (berr), __LINE__, __FILE__); } \
    } while(0)

#define __VAL_THROW(ff,val,berr) __IN__VAL_THROW(ff, val, berr)

#define _HASSERT_THROW(ff,val) __VAL_THROW(ff,HERR_NO(val),HFALSE)

#define _HFAILED_THROW(val) do { \
	HRET __ret = (val); \
	if (! HIS_OK(__ret)) { \
	    throw HCException(__ret, HTRUE, HFALSE, __LINE__, __FILE__); \
	} } while(0)

#define _HASSERT_MSG_VAL_THROW(ff,msg,val,berr) do {	\
		if (! (ff)) { \
			throw HCException((msg), (val), HTRUE, (berr), __LINE__, __FILE__); } \
		} while(0)

#define _HFAILED_MSG(val,msg) do { \
        HRET __ret = (val); \
        HIF_NOTOK(__ret) { \
            throw HCException((msg),(__ret),HTRUE,HTRUE,__LINE__,__FILE__); \
        } \
    } while (0)

#define _IN_MSG_THROW(ff,msg,val,berr) _HASSERT_MSG_VAL_THROW(ff,msg,val,berr)

#define _HASSERT_MSG_THROW(ff,msg,ret) _IN_MSG_THROW(ff,msg,ret,HFALSE)

#define _HASSERT_MSG_THROW_ERR(ff,msg,ret) _IN_MSG_THROW(ff,msg,ret,HTRUE)

}

// If base error, throw exception without system error.
#define HASSERT_THROW(ff,val) _HASSERT_THROW(ff,val)

// If base error, throw exception with system error.
#define HASSERT_THROW_ERR(ff,val,err) __VAL_THROW(ff,val,HTRUE)

#define HFAILED_THROW(val) _HFAILED_THROW(val)

#define HASSERT_MSG_THROW(ff,msg,ret) _HASSERT_MSG_THROW(ff,msg,ret)

#define HASSERT_MSG_THROW_ERR(ff,msg,ret) _HASSERT_MSG_THROW_ERR(ff,msg,ret)

#define HASSERT_THROW_MSG(ff,msg,err) _HASSERT_MSG_THROW_ERR(ff,msg,HERR_NO(err))

#define HFAILED_MSG(ff,msg) _HFAILED_MSG(ff,msg)

#endif // matched HASSERT_EXCEPTION
}

#endif
