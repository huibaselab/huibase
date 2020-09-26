/*****************************************************************************
 *
 * FileName: mysqldata.h
 *
 * Author: Tom Hui, tomhui1009@yahoo.com, 8613760232170
 *
 * Create Date: Sun Jul 29 17:25 2018
 *
 * Brief:
 *       Wrapper class for MYSQL_RES, MYSQL_ROW.
 *
 *****************************************************************************/

#ifndef __MYSQLDATA_H__
#define __MYSQLDATA_H__

#include <huibase.h>
#include <hdict.h>
#include <mysql/mysql.h>

namespace HUIBASE {

    namespace HMYSQL{

        typedef HVSTRS ROWHEADER;
        typedef HVSTRS ROW;
        typedef std::vector<ROW> TABLE;


        class CRes {
        public:
            CRes ();

            void SetEmpty();

            void SetHeader(HCVSTRSR header) { m_header = header; }

            void AddRow (HCVSTRSR row) { m_table.emplace_back(row); }

            HBOOL IsValid () const { HRET_BOOL(not m_header.empty() and not m_table.empty()); }

            const ROWHEADER& GetHeader () const { return m_header; }

            const TABLE& GetTable () const { return m_table; }

            HCSTRR GetValue(HCSTRR strKey, HUINT _row = 0);

            HCSTRR GetValue (HUINT i, HUINT j);

            HUINT GetCount() const;

        private:
            ROWHEADER m_header;
            TABLE m_table;

        };


    }

}


#endif //__MYSQLDATA_H__
