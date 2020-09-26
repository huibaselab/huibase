/*****************************************************************************
 *
 * FileName: simpledb.h
 *
 * Author: Tom Hui, tomhui1009@yahoo.com, 8613760232170
 *
 * Create Date: Sat Aug 04 13:21 2018
 *
 * Brief:
 *   Provides a simple mysql connection class base the mysql connection drive.
 *   This connection class only use to short connection application.
 *
 *****************************************************************************/


#ifndef __HCONNECTION_H__
#define __HCONNECTION_H__

#include "mysqlconndrive.h"


namespace HUIBASE {

    namespace HMYSQL {

        class CMyConnection : public CMysqlConnDrive {
        public:
            CMyConnection () { }

        public:
            HRET Query (HCSTRR sql, CRes& res) throw ();

            HRET Exec (HCSTRR sql) throw ();

            HRET BeginTransaction () throw ();

            HRET CommitTransaction () throw ();

            HRET RollBack () throw ();

            HRET MultiExec (HCVSTRSR sqlss) throw ();

            HRET MultiInsert (HCSTRR strTable, const CRes& res);


        private:
            static constexpr HUINT SQL_MAX = 128 * 1024 * 1024;

        };

    }


}



#endif //__HCONNECTION_H__
