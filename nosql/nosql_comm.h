

#ifndef __H_NOSQL_COMM_H__
#define __H_NOSQL_COMM_H__

#include <huibase.h>


namespace HUIBASE {

    namespace NOSQL {

        typedef struct _NoSqlConnectionInfo {
            HSTR strIp;
            HUINT nPort = 6379;
            HSTR strPw;
            HSTR strName;
        } NoSqlConnectionInfo;

    }


}


#endif //__H_NOSQL_COMM_H__
