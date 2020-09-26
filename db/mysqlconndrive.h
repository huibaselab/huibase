/*******************************************************
 *
 * FileName: dbdrive.h
 *
 * Author: Tom Hui, tomhui1009@yahoo.com, 8613760232170
 *
 * Create Date: Sat Jul 28 15:45 2018
 *
 * Brief:
 *   Provides a thin abstraction layer over the underlying
 *   database client library.
 *   This class does as little as possible to adapt between
 *   its public interface and the interface require by
 *   underlying C API. That is, in fact, its only mission.
 *
 *******************************************************/



#ifndef __MYSQLCONNDRIVE_H__
#define __MYSQLCONNDRIVE_H__


#include <huibase.h>
#include <hstr.h>
#include "mysqldata.h"


namespace HUIBASE {

namespace HMYSQL {

typedef struct {
    HSTR m_host;
    HINT m_port;
    HSTR m_user;
    HSTR m_pw;
    HSTR m_db;
}CONN_INFO;

class CMysqlConnDrive {
#define CHECK_DBST HASSERT_RETURN(HIS_TRUE(IsInited()), UN_INIT)
#define CHECK_DB_CONNECTION HASSERT_RETURN(HIS_TRUE(IsInited()) && HIS_TRUE(IsConnected()), UN_INIT)

public:
    typedef struct {
        HUINT nConnectTimeout;
        HUINT nReadTimeout;
        HBOOL bRec;
        HSTR strCharSet;
    } MYSQL_INNER_INFO;

public:
    CMysqlConnDrive();

    /*
        brief: Duplicate a existing connection.
    */  
    CMysqlConnDrive(const CMysqlConnDrive& conn);
   
    CMysqlConnDrive(CMysqlConnDrive&& conn)
        : m_st(conn.m_st) { conn.m_st = nullptr;  }

    virtual ~CMysqlConnDrive ();

    HBOOL IsInited () const { HRET_BOOL(m_st != nullptr); }

    HBOOL IsConnected () const { return m_bConnected; }

    HRET Connect (const CONN_INFO& conn_info, HUINT nConnectTimeout = 2, HUINT nReadTimeout = 2, HBOOL bRec = HTRUE);

    CONN_INFO GetConnInfo (MYSQL_INNER_INFO* pInfo = nullptr) const;

    void Disconnect ();

    HRET Reconnect ();

    HULL GetAffectedRows ();

    HSTR GetClientVersion ();

    HINT GetMysqlErrno ();

    HRET Execute (HCSTRR sql);

    HRET StoreResult (CRes & res);

    HRET UseResult (CRes& res);

    HULL GetNewId ();

    HBOOL Ping ();

    HRET SetReconnect (HBOOL b = HTRUE);

    HRET SetConnectTimeout (HUINT nTimeout = 1);

    HRET SetReadTimeout (HUINT nTimeout = 1);

    HRET SetUtf8 ();

    void saveRes (MYSQL_RES* db_res, CRes& res);

private:
    void init ();

    void uninit ();

    void copy (const CMysqlConnDrive& conn);


private:
    MYSQL * m_st = nullptr;

    HBOOL m_bConnected = HFALSE;
};

}

}

#endif //__MYSQLCONNDRIVE_H__
