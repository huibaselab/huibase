
#include <sstream>
#include "mysqlconndrive.h"

namespace HUIBASE {

namespace HMYSQL {


CMysqlConnDrive::CMysqlConnDrive()
    : m_st(nullptr), m_bConnected(HFALSE){

    init();

}


CMysqlConnDrive::CMysqlConnDrive(const CMysqlConnDrive& conn) {

    uninit();    

    init();

    copy(conn);

}


CMysqlConnDrive::~CMysqlConnDrive() {

    uninit();

}


HRET CMysqlConnDrive::Connect(const CONN_INFO& conn_info, HUINT nConnectTimeout, HUINT nReadTimeout, HBOOL bRec){

    CHECK_DBST;

    if (nConnectTimeout != 0) {

        HNOTOK_RETURN(SetConnectTimeout(nConnectTimeout));

    }

    if (nReadTimeout != 0) {

        HNOTOK_RETURN(SetReadTimeout(nReadTimeout));

    }


    HNOTOK_RETURN(SetReconnect(bRec));

    m_st = mysql_real_connect(m_st, conn_info.m_host.c_str(), conn_info.m_user.c_str(), conn_info.m_pw.c_str(), conn_info.m_db.c_str(), conn_info.m_port, nullptr, 0);
    
    if (not m_st) {
        HRETURN(DB_DISCONN);
    }

    m_bConnected = HTRUE;

    HRETURN_OK;
}


CONN_INFO CMysqlConnDrive::GetConnInfo (MYSQL_INNER_INFO* pInfo) const {

    HASSERT_THROW(HIS_TRUE(IsInited()) and HIS_TRUE(IsConnected()), UN_INIT);

    if (pInfo != nullptr) {
        // reconnection, connect-timeout, read-timeout, charset-name.
        bool myval = 0;
        HASSERT_THROW_MSG(mysql_get_option(m_st, MYSQL_OPT_RECONNECT, &myval) == 0, "mysql_get_option get reconnection flag failed", DB_ERR);

        pInfo->bRec = SETHBOOL(myval);

        HASSERT_THROW_MSG(mysql_get_option(m_st, MYSQL_OPT_CONNECT_TIMEOUT, &pInfo->nConnectTimeout) == 0, "mysql_get_option get connect-timeout failed", DB_ERR);

        HASSERT_THROW_MSG(mysql_get_option(m_st, MYSQL_OPT_READ_TIMEOUT, &pInfo->nReadTimeout) == 0, "mysql_get_option get read-timeout failed", DB_ERR);

        char buf[HLEN2_C] = {0};
        HASSERT_THROW_MSG(mysql_get_option(m_st, MYSQL_SET_CHARSET_NAME, buf) == 0, "mysql_get_option get charset name failed", DB_ERR);

        pInfo->strCharSet = buf;

    }

    CONN_INFO info;
    info.m_host = m_st->host;
    info.m_port = m_st->port;
    info.m_user = m_st->user;
    info.m_pw = m_st->passwd;
    info.m_db = m_st->db;

    //LOG_NORMAL("host[%s] port[%d] user[%s] pw[%s] db[%s]", info.m_host.c_str(), info.m_port, info.m_user.c_str(), info.m_pw.c_str(), info.m_db.c_str());

    return info;
}


void CMysqlConnDrive::Disconnect() {

    uninit();

}


HRET CMysqlConnDrive::Reconnect () {

    MYSQL_INNER_INFO inner_info;

    CONN_INFO conn_info = GetConnInfo(&inner_info);

    Disconnect();

    init();

    return Connect(conn_info, inner_info.nConnectTimeout, inner_info.nReadTimeout, inner_info.bRec);
}


HULL CMysqlConnDrive::GetAffectedRows() {

    if (m_st == nullptr || HIS_FALSE(IsConnected())) {
        return 0;
    }

    return mysql_affected_rows(m_st);

}


HSTR CMysqlConnDrive::GetClientVersion() {

    IF_FALSE(IsInited()) {
        return "Error Mysql Client Version";
    }

    return mysql_get_client_info();

}


HINT CMysqlConnDrive::GetMysqlErrno () {
    IF_FALSE(IsInited()) {
        return -1;
    }

    return mysql_errno(m_st);
}


HRET CMysqlConnDrive::Execute(HCSTRR sql) {

    CHECK_DB_CONNECTION;

    //LOG_NORMAL("m_st[%p] sql[%s] sql[%d]", m_st, sql.c_str(), sql.length());

    HASSERT_RETURN(mysql_real_query(m_st, sql.c_str(), sql.length()) == 0, QRY_ERR);

    HRETURN_OK;

}


HRET CMysqlConnDrive::StoreResult(CRes& res) {

    CHECK_DB_CONNECTION;

    res.SetEmpty();

    MYSQL_RES* cb_res = mysql_store_result(m_st);

    if (cb_res == nullptr) {

        HRETURN_OK;

    }

    saveRes (cb_res, res);

    mysql_free_result(cb_res);

    HRETURN_OK;
}


HRET CMysqlConnDrive::UseResult(CRes& res) {

    CHECK_DB_CONNECTION;

    MYSQL_RES* cb_res = mysql_use_result(m_st);

    if (cb_res == nullptr) {

        HRETURN_OK;

    }

    saveRes (cb_res, res);

    mysql_free_result(cb_res);

    HRETURN_OK;
}


HULL CMysqlConnDrive::GetNewId() {

    CHECK_DB_CONNECTION;

    return (HULL) mysql_insert_id(m_st);

}


HBOOL CMysqlConnDrive::Ping() {

    IF_FALSE(IsInited()) {
        return HFALSE;
    }

    HRET_BOOL(mysql_ping(m_st) == 0);

}


HRET CMysqlConnDrive::SetReconnect(HBOOL b) {

    CHECK_DBST;
    
    bool recb = b == HTRUE ? 1 : 0;
    HASSERT_RETURN(mysql_options(m_st, MYSQL_OPT_RECONNECT, &recb) == 0, QRY_ERR);

    HRETURN_OK;
}


HRET CMysqlConnDrive::SetConnectTimeout(HUINT nTimeout) {

    CHECK_DBST;

    HASSERT_RETURN(mysql_options(m_st, MYSQL_OPT_CONNECT_TIMEOUT, &nTimeout) == 0, QRY_ERR);

    HRETURN_OK;
}


HRET CMysqlConnDrive::SetReadTimeout(HUINT nTimeout) {

    CHECK_DBST;

    HASSERT_RETURN(mysql_options(m_st, MYSQL_OPT_READ_TIMEOUT, &nTimeout) == 0, QRY_ERR);

    HRETURN_OK;

}


HRET CMysqlConnDrive::SetUtf8() {

    CHECK_DBST;

    HASSERT_RETURN(mysql_options(m_st, MYSQL_SET_CHARSET_NAME, "utf8") == 0, QRY_ERR);

    HRETURN_OK;

}


void CMysqlConnDrive::saveRes (MYSQL_RES* db_res, CRes& res) {

    HUINT filed_num = mysql_num_fields (db_res);

    MYSQL_FIELD* field = nullptr;
    MYSQL_ROW __row;

    HVSTRS names;

    while ((field = mysql_fetch_field(db_res))) {

        names.push_back(field->name);

    }

    res.SetHeader(names);

    while ((__row = mysql_fetch_row(db_res))) {
        
        HVSTRS row;
        for (HUINT i = 0; i < filed_num; ++i) {

            row.push_back (__row[i] ? __row[i] : "NULL");

        }

        res.AddRow(row);

    }

}


void CMysqlConnDrive::init() {

    m_st = mysql_init (nullptr);
    HASSERT_THROW_MSG(m_st != nullptr, "mysql_init failed", MEMORY_OUT);

}


void CMysqlConnDrive::uninit() {

    IF_TRUE(IsInited()) {

        mysql_close(m_st);

        m_st = nullptr;

        m_bConnected = HFALSE;

    }

}


void CMysqlConnDrive::copy (const CMysqlConnDrive& conn) {

    HASSERT_THROW(HIS_TRUE(IsInited()), UN_INIT);

    IF_FALSE(conn.IsInited()) {
        return ;
    }

    // reconnection, connect-timeout, read-timeout, charset-name.
    bool myval = 0;
    HASSERT_THROW_MSG(mysql_get_option(m_st, MYSQL_OPT_RECONNECT, &myval) == 0, "mysql_get_option get reconnection flag failed", DB_ERR);

    if (myval) {
        SetReconnect();
    }

    HUINT n = 0;
    HASSERT_THROW_MSG(mysql_get_option(m_st, MYSQL_OPT_CONNECT_TIMEOUT, &n) == 0, "mysql_get_option get connect-timeout failed", DB_ERR);

    SetConnectTimeout(n);

    n = 0;
    HASSERT_THROW_MSG(mysql_get_option(m_st, MYSQL_OPT_READ_TIMEOUT, &n) == 0, "mysql_get_option get read-timeout failed", DB_ERR);

    SetReadTimeout(n);

    char buf[HLEN2_C] = {0};
    HASSERT_THROW_MSG(mysql_get_option(m_st, MYSQL_SET_CHARSET_NAME, buf) == 0, "mysql_get_option get charset name failed", DB_ERR);

    HASSERT_THROW(mysql_options(m_st, MYSQL_SET_CHARSET_NAME, buf) == 0, DB_ERR);

    IF_TRUE(conn.IsConnected()) {

        m_st = mysql_real_connect(m_st, conn.m_st->host, conn.m_st->user, conn.m_st->passwd, conn.m_st->db, conn.m_st->port,nullptr, 0);

        HASSERT_THROW_MSG(m_st != nullptr, "copy connection mysql_real_connect failed", DB_ERR);

        m_bConnected = HTRUE;
        
    }

    m_bConnected = conn.m_bConnected;
    
}


class ___init_mysql_class {
public:
    ___init_mysql_class() {
        /*
        * In a nonmultithreaded environment, mysql_init() invokes mysql_library_init() automatically as necessary. However, mysql_library_init() is not thread-safe in a multithreaded environment, and thus neither is mysql_init. Before calling mysql_init(), either call mysql_library_init prior to spawning any threads, or use a mutex to protect the mysql_library_init() call. This should be done prior to any other client library call.
        */
        // Before calling mysql_init(), call mysql_library_init prior.
        (void) mysql_library_init(0, nullptr, nullptr);
    }

    ~ ___init_mysql_class() {
        /*
         * To avoid memory leaks after the application is done using the library, be sure to call mysql_library_end() explicitly. This enables memory managment to be performed to clean up and free resources used by the library.
         */
        (void) mysql_library_end();
    }
};

static const ___init_mysql_class __init_mysql_object;

}

}
