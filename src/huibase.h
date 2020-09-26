/****************************************************************************
 *
 * File Name: huibase.h
 *
 * Create Author: Tom Hui, tomhui1009@yahoo.com, 8613760232170
 *
 * Create Date: Thu Dec 08 16:16 2016
 *
 * Description:
 *	base type and macro.
 *
 ****************************************************************************/

#ifndef _H_HUIBASE_H_
#define _H_HUIBASE_H_

// c header files.
#include <errno.h>
#include <ctype.h>
#include <memory.h>
#include <assert.h>
#include <limits.h>


// c++ standard header files.
#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <functional>
#include <utility>


// linux system header files.
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>

#include <time.h>
#include <pthread.h>
#include <sys/stat.h>

#include <signal.h>

// c++ stl header files.
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm> 
#include <sstream>


using namespace std;

namespace HUIBASE{

    typedef char HCHAR;
    typedef unsigned char HUCHAR;
    typedef char* HPSZ;
    typedef const char* HCPSZ;

    typedef unsigned char* HUPSZ;
    typedef const unsigned char* HCUPSZ;

    typedef char HBUP[1];

    typedef string HSTR;
    typedef const string HCSTR;
    typedef string& HSTRR;
    typedef const string& HCSTRR;

    typedef map<HSTR, HSTR> HPS;
    typedef const map<HSTR, HSTR>  HCPS;
    typedef map<HSTR, HSTR>& HPSR;
    typedef const map<HSTR, HSTR>& HCPSR;

    typedef vector<HSTR> HVSTRS;
    typedef const vector<HSTR> HCVSTRS;
    typedef vector<HSTR>& HVSTRSR;
    typedef const vector<HSTR>& HCVSTRSR;

    typedef set<HSTR> HSSTRS;
    typedef const set<HSTR> HCSSTRS;
    typedef set<HSTR>& HSSTRSR;
    typedef const set<HSTR>& HCSSTRSR;

    typedef vector<HCHAR> HBUF;
    typedef vector<HCHAR>& HBUFR;
    typedef const vector<HCHAR> HCBUF;
    typedef const vector<HCHAR>& HCBUFR;

    typedef vector<HUCHAR> HMEM;
    typedef vector<HUCHAR>& HMEMR;
    typedef const vector<HUCHAR> HCMEM;
    typedef const vector<HUCHAR>& HCMEMR;

    typedef std::pair<HSTR, HSTR> HKV;
    typedef std::pair<HSTR, HSTR>& HKVR;
    typedef const std::pair<HSTR, HSTR> HCKV;
    typedef const std::pair<HSTR, HSTR>& HCKVR;

    typedef std::vector<HKV> HKVS;
    typedef std::vector<HKV>& HKVSR;
    typedef const std::vector<HKV> HCKVS;
    typedef const std::vector<HKV>& HCKVSR;

    typedef int HINT;
    typedef unsigned int HUINT;
    typedef long HLONG;
    typedef short HSINT;
    typedef short HSHORT;
    typedef unsigned short HUSHORT;
    typedef long long HLLINT;
    typedef unsigned long long HULL;
    typedef size_t HSIZE;
    typedef ssize_t HSSIZE;

    typedef time_t HTIME;

    typedef double HDOUBLE;
	typedef HUINT HSNUM;

    // special types.
    typedef HINT HFD_T;
    typedef HINT HRET;
    typedef int HSYS_T; // system type.
    typedef int HSYS_RET; // syscall return type.(int)

    typedef int64_t HN64;
    typedef uint64_t HUN64;

    // define common constant
    #define HLEN_C (64)
    #define HLEN2_C (HLEN_C*2)
	#define HLEN4_C (HLEN_C*4)
	#define HLEN8_C (HLEN_C*8)
	#define HLEN16_C (HLEN_C*16)
    #define HLEN32_C (HLEN_C*32)
	#define HLEN64_C (HLEN_C*64)
	#define HLEN128_C (HLEN_C*128)
	#define HLEN256_C (HLEN_C*256)
	#define HLEN512_C (HLEN_C*512)
	#define HLEN1024_C (HLEN_C*1024)
	#define HLEN2048_C (HLEN_C*2048)
	#define HLEN8096_C (HLEN_C*8096)
	#define HLEN80960_C (HLEN_C*80960)

	#define HMAX_INT INT_MAX
	#define HMIN_INT INT_MIN
	#define HMAX_UINT UINT_MAX

    #define HINVA_FD -1
    #define HNULL_FD 0

	
	using HTID_T = pthread_t;

#if 0    
    typedef HINT HBOOL;
#   define HTRUE 1
#   define HFALSE 0    
#else
    enum HBOOL {
		HFALSE = 0,
		HTRUE = 1
    };	

    inline bool HBool (HBOOL b) { return b == HTRUE; }
    inline HBOOL  SETHBOOL(bool b) { return b ? HTRUE : HFALSE; }

#endif

#ifndef HIS_TRUE
#define _HIS_TRUE(bol) ((bol) != HFALSE)
#define HIS_TRUE(bol) _HIS_TRUE(bol)   
#define _HIS_FALSE(bol) ((bol)==HFALSE)
#define HIS_FALSE(bol) _HIS_FALSE(bol)

#define _IF_TRUE(bol) if(HIS_TRUE(bol))
#define IF_TRUE(bol) _IF_TRUE(bol)

#define _IF_FALSE(bol) if(HIS_FALSE(bol))
#define IF_FALSE(bol) _IF_FALSE(bol) 
#endif    

#ifndef HRET_BOOL
#define _HRET_BOOL(bol) return ((bol)) ? HTRUE : HFALSE
#define HRET_BOOL(bol) _HRET_BOOL(bol)    
#endif   

    // HBASE's type
    #define HNAME_LEN HLEN2_C
	#define HPATH_LEN HLEN4_C
	#define HTIME_LEN HLEN2_C
	
    typedef HCHAR HNAME[HNAME_LEN];
    typedef HCHAR HPATH[HPATH_LEN];
    typedef HCHAR HTIMEBUF[HTIME_LEN];

#define HNAME_ZERO(buf) memset(buf, 0, HNAME_LEN)
#define HPATH_ZERO(buf) memset(buf, 0, HPATH_LEN)
#define HTIMEBUF_ZERO(buf) memset(buf, 0, HTIME_LEN)

#define HCLOSE_FD(fd) do {\
	if((fd) > 0) {\
	    close((fd)); \
	    (fd) = HINVA_FD; \
	}}while(0)


#define HDELP(p) do {\
	if((p) != nullptr) {\
	delete (p); \
	(p) = nullptr;\
	}} while(0)

#define HDEL_ARR(p) do { \
	if ((p) != nullptr) { \
	    delete [] (p); \
	    (p) = nullptr; \
	} \
} while(0) 

#define HFREE(p) do { \
	if ((p) != nullptr) { \
		free((p)); \
		(p) = nullptr; \
	} \
} while(0)
    

    // error map for hbase library
#define HBASE_ERR_MAP(XX) \
    XX(OK, "success")	\
    XX(INVL_PARA, "invaild parameter") \
    XX(ILL_PT, "uninitialize pointer") \
    XX(UN_INIT, "target has not inited") \
    XX(ERR_STATUS,"invalid status") \
    XX(MISS_FILE,"miss file") \
    XX(SRC_FAIL, "initilize source failed") \
    XX(INVAIL_DE, "null pointer or invaild descriptor") \
    XX(RECREATE, "the decriptor is exists. It's an error to recreate it") \
    XX(INDEX_OUT, "index overflow") \
    XX(POS_END, "pos is end, not find") \
    XX(SYS_FAILED, "syscall failed")	\
    XX(MEMORY_OUT, "memory out") \
    XX(TIME_OUT, "time out") \
    XX(DATA_TOO_OLD, "expire data") \
    XX(RE_INITED, "object is inited") \
    XX(HBASE_FAILED, "call base's function failed or return value is invalid.") \
    XX(HTTP_FAILED,"http client or server call failed") \
    XX(INVL_RES, "invaild result or return.") \
    XX(LOGIC_ERR, "error in task or command excution. LOGIC ERROR") \
    XX(EX_GET, "get a exception in logic work") \
    XX(NET_REQ, "get request from network failed") \
    XX(NET_RES, "sent response to network failed") \
    XX(IO_ERR, "I/O failed") \
    XX(QRY_ERR, "db query failed") \
    XX(DB_ERR, "error in db") \
    XX(DB_DISCONN, "db is not connected") \
    XX(RDS_ERR, "redis error") \
    XX(MEM_ERR, "memcached error") \
    XX(SSL_ERR, "openssl error") \
    XX(DEP_ERROR, "dep source rise error") \
    XX(NO_DONE, "nothing done") \
    XX(SYS_SUPP, "operation unsupport") \
    XX(UNKOWN_ERR, "unknown error number.") \
	\
    XX(ERR_END, "err-end") 
    
#ifndef _HERR_NO
#define _HERR_NO(err) BASE_ERR_##err 
#define HERR_NO(err) _HERR_NO(err)
#endif


#ifndef _HIS_OK
#define _HIS_OK(cb) ((cb) == HERR_NO(OK))
#define HIS_OK(cb) _HIS_OK(cb)

#define _HNOT_OK(cb) ((cb) != HERR_NO(OK))
#define HNOT_OK(cb) _HNOT_OK(cb)

#define _HIF_OK(cb) if (HIS_OK(cb))
#define HIF_OK(cb) _HIF_OK(cb)

#define _HIF_NOTOK(cb) if(HNOT_OK(cb))
#define HIF_NOTOK(cb) _HIF_NOTOK(cb)
#endif


#ifndef HRETURN_OK
#define HRETURN_OK return HERR_NO(OK)
#endif    

#ifndef _HRETURN
#define _HRETURN(ret) if (!(HIS_OK(ret))){	\
	    g_hErr = ret;		\
	} \
    return ret

#define _HRETURN_(ret) _HRETURN(ret)
#define HRETURN(err_code) _HRETURN_(HERR_NO(err_code))
#endif

#ifndef HNULLSTR
#define HNULLSTR ""
#endif    

    extern HINT g_hErr;

	#define ERR_GEN(n,s) BASE_ERR_##n,
    enum base_errs_enum{
	HBASE_ERR_MAP(ERR_GEN)
    };
    #undef ERR_GEN
    
    HCPSZ hbase_err (HINT err);

	HINT hlast_err () ;


#define HDEL_BIT(a,b) HINT del_bit##b = b; a = a & (~ del_bit##b)

#define _HASSERT_RETURN(aa,ee) do {\
	if (!(aa)) {\
		HRETURN(ee);\
	}\
} while(0)
#define HASSERT_RETURN(aa,ss) _HASSERT_RETURN(aa,ss)

#ifndef HNOTOK_RETURN
#define _HNOTOK_RETURN(seg) do { HRET cb = (seg); if (HNOT_OK(cb)){ return cb; } } while(0)
#define HNOTOK_RETURN(seg) _HNOTOK_RETURN(seg)
#endif 


#ifndef _ZERO_MEM
#define _ZERO_MEM(buf, len) memset((buf), 0, (len))
#define ZERO_MEM(buf, len) _ZERO_MEM(buf, len)    
#endif    

HINT inline HMAX(HINT a, HINT b) { return (a > b ? a : b); }

#ifndef HMAC_STR
#define _HMAC_STR(mac) #mac
#define HMAC_STR(mac) _HMAC_STR(mac)
#endif 
    
} // end for HUIBASE

#include "hexception.h"


#ifndef CHECK_NEWPOINT
#define _CHECK_NEWPOINT(p) HASSERT_THROW((p)!=nullptr, MEMORY_OUT)
#define CHECK_NEWPOINT(p) _CHECK_NEWPOINT(p)
#endif //CHECK_NEWPOINT

#ifndef HLOG_DEBUG
#define HLOG_DEBUG do { \
	FILE* pf = fopen("debug", "aw"); \
	fprintf(pf, "Errno: %d\n", errno); \
	fprintf(pf, "Base_msg: %s\n", hbase_err(hlast_err())); \
	fprintf(pf, "LineNo: %d\n", __LINE__); \
	fprintf(pf, "File: %s\n", __FILE__); \
	fclose(pf); \
} while(0)
#endif 



#endif //_H_HBASE_H_


