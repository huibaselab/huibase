/*******************************************************
 *
 * FileName: hredis.cpp
 *
 * Author: Tom Hui, tomhui1009@yahoo.com, 8613760232170
 *
 * Create Date: Sun Mar 11 01:03 2018
 *
 * Brief:
 *   Implementation for hredis.
 *
 *******************************************************/

#include <hstr.h>
#include "hredis.h"

namespace HUIBASE {

    namespace NOSQL {

        CRedis::CRedis(const NoSqlConnectionInfo& info)
            :m_info(info) { }


        CRedis::~CRedis () {

            Release ();

        }


        HRET CRedis::Init () {

            m_context = redisConnect(m_info.strIp.c_str(), m_info.nPort);
            CHECK_NEWPOINT(m_context);
            HASSERT_THROW_MSG(m_context->err == 0, "connect to redis failed", RDS_ERR);

            HRETURN_OK;
        }


        HRET CRedis::Reinit() {

            Release();

            return Init();

        }


        void CRedis::Release() {

            if (m_context != nullptr) {

                redisFree(m_context);
                m_context = nullptr;

            }

        }

#define CHECK_REDIS_INS do { if (m_context == nullptr) { Reinit(); } HASSERT_THROW(m_context != nullptr, UN_INIT); } while(0) 

#define SET_CMD do { redis_res* r = static_cast<redis_res*>(redisCommand(m_context, cmd.c_str())); \
            if (r == nullptr) { \
                Reinit(); \
                HRETURN(RDS_ERR); \
            } \
            if (m_context->err or r->type != REDIS_REPLY_STATUS) { \
                freeReplyObject(r); \
                Reinit(); \
                HRETURN(RDS_ERR); \
            } \
            if (HSTR(r->str) != "OK") { \
                freeReplyObject(r); \
                Reinit(); \
                HRETURN(RDS_ERR); \
            } \
            freeReplyObject(r); \
        } while(0)

        HRET CRedis::Set (HCSTRR key, HCSTRR val) {

            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("SET %s %s", key.c_str(), val.c_str());

            SET_CMD;

            HRETURN_OK;
        }


        HRET CRedis::Setex (HCSTRR key, HCSTRR val, HINT exp) {
            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("SETEX %s %d %s", key.c_str(), exp, val.c_str());

            SET_CMD;

            HRETURN_OK;                
        }


#define RET_BOOL_CMD redis_res* r = static_cast<redis_res*>(redisCommand(m_context, cmd.c_str())); \
            if (r == nullptr) { \
                Reinit(); \
                HASSERT_THROW(0, RDS_ERR); \
                return HFALSE; \
            } \
            if (m_context->err or r->type != REDIS_REPLY_INTEGER) { \
                freeReplyObject(r); \
                Reinit(); \
                HASSERT_THROW(0, RDS_ERR); \
                return HFALSE; \
            } \
            HINT cb = r->integer; \
            freeReplyObject(r); \
            HRET_BOOL(cb)
        
        HBOOL CRedis::Setnx(HCSTRR key, HCSTRR val) {

            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("SETNX %s %s", key.c_str(), val.c_str());

            RET_BOOL_CMD;

        }

#define GET_CMD do { redis_res* r = static_cast<redis_res*>(redisCommand(m_context, cmd.c_str())); \
                    if (r == nullptr) { \
                        Reinit(); \
                        HRETURN(RDS_ERR); \
                    } \
                    if (m_context->err) { \
                        freeReplyObject(r); \
                        Reinit(); \
                        HRETURN(RDS_ERR); \
                    } \
                    if (r->type == REDIS_REPLY_NIL) { \
                        freeReplyObject(r); \
                        HRETURN_OK; \
                    } \
                    if (r->type != REDIS_REPLY_STRING) { \
                        freeReplyObject(r); \
                        Reinit(); \
                        HRETURN(RDS_ERR); \
                    } \
                    val = r->str; \
                    freeReplyObject(r); \
                } while(0)


        HRET CRedis::Get(HCSTRR key, HSTRR val) {

            CHECK_REDIS_INS;

            val.clear();

            HSTR cmd = HCStr::Format("GET %s", key.c_str()); 

            GET_CMD;

            HRETURN_OK;

        }


        HRET CRedis::Get (HCSTRR key, HSTRR val, HCSTRR defval) {
            
            CHECK_REDIS_INS;

            val.clear();
            val = defval;

            HSTR cmd = HCStr::Format("GET %s", key.c_str()); 

            GET_CMD;

            HRETURN_OK;

        }

#define RET_INT_CMD do { redis_res* r = static_cast<redis_res*>(redisCommand(m_context, cmd.c_str())); \
            if (r == nullptr) { \
                Reinit(); \
                HRETURN(RDS_ERR); \
            } \
            if (m_context->err or r->type != REDIS_REPLY_INTEGER) { \
                freeReplyObject(r); \
                Reinit(); \
                HRETURN(RDS_ERR); \
            } \
            freeReplyObject(r); \
        } while(0)


        HRET CRedis::Expire(HCSTRR key, HINT sec) {

            CHECK_REDIS_INS;

            HASSERT_RETURN(sec >= 0, INVL_PARA);

            HSTR cmd = HCStr::Format("EXPIRE %s %d", key.c_str(), sec);

            RET_INT_CMD;

            HRETURN_OK;

        }


        HBOOL CRedis::Exists(HCSTRR key) {

            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("EXISTS %s", key.c_str());

            RET_BOOL_CMD;

        }


        HRET CRedis::Del(HCSTRR key) {

            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("DEL %s", key.c_str());

            RET_INT_CMD;

            HRETURN_OK;
        }


        HRET CRedis::Incr (HCSTRR key) {

            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("INCR %s", key.c_str());

            RET_INT_CMD;

            HRETURN_OK;

        }


        HRET CRedis::Decr (HCSTRR key) {

            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("DECR %s", key.c_str());

            RET_INT_CMD;

            HRETURN_OK;

        }

//================================HASH====================================================        
        HRET CRedis::HSet (HCSTRR key, HCSTRR field, HCSTRR val) {

            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("HSET %s %s %s", key.c_str(), field.c_str(), val.c_str());

            RET_INT_CMD;

            HRETURN_OK;

        }


        HBOOL CRedis::HSetnx (HCSTRR key, HCSTRR field, HCSTRR val) {
            
            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("HSETNX %s %s %s", key.c_str(), field.c_str(), val.c_str());

            RET_BOOL_CMD;

        }


        HRET CRedis::HGet (HCSTRR key, HCSTRR field, HSTRR val) {

            CHECK_REDIS_INS;

            val.clear();

            HSTR cmd = HCStr::Format("HGet %s %s", key.c_str(), field.c_str()); 

            GET_CMD;

            HRETURN_OK;

        }


        HRET CRedis::HGet (HCSTRR key, HCSTRR field, HSTRR val, HCSTRR defval) {

            CHECK_REDIS_INS;

            val.clear();
            val = defval;

            HSTR cmd = HCStr::Format("HGet %s %s", key.c_str(), field.c_str()); 

            GET_CMD;

            HRETURN_OK;

        }


        HBOOL CRedis::HExists (HCSTRR key, HCSTRR field) {
            
            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("HEXISTS %s %s", key.c_str(), field.c_str());

            RET_BOOL_CMD;

        }


        HRET CRedis::HDel (HCSTRR key, HCSTRR field) {

            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("HDEL %s %s", key.c_str(), field.c_str());

            RET_INT_CMD;

            HRETURN_OK;

        }

#define INT_RET_CMD do { redis_res* r = static_cast<redis_res*>(redisCommand(m_context, cmd.c_str())); \
            if (r == nullptr) { \
                Reinit(); \
                HRETURN(RDS_ERR); \
            } \
            if (m_context->err or r->type != REDIS_REPLY_INTEGER) { \
                freeReplyObject(r); \
                Reinit(); \
                HRETURN(RDS_ERR); \
            } \
            ret = r->integer; \
            freeReplyObject(r); \
        } while(0)

        HINT CRedis::HLen (HCSTRR key) {
            
            CHECK_REDIS_INS;

            HINT ret = 0;

            HSTR cmd = HCStr::Format("HLEN %s", key.c_str());
            
            INT_RET_CMD;

            return ret;

        }


        HRET CRedis::HIncrby (HCSTRR key, HCSTRR field, HINT by) {
            
            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("HINCRBY %s %s %d", key.c_str(), field.c_str(), by);

            RET_INT_CMD;

            HRETURN_OK;

        }

#define CMD_EXCEPS(ty) redis_res* r = static_cast<redis_res*>(redisCommand(m_context, cmd.c_str())); do { \
    if (r == nullptr) { \
        Reinit(); \
        HRETURN(RDS_ERR); \
    } \
    if (m_context->err) { \
        freeReplyObject(r); \
        Reinit(); \
        HRETURN(RDS_ERR); \
    } \
    if (r->type == REDIS_REPLY_NIL) { \
        freeReplyObject(r); \
        HRETURN_OK; \
    } \
    if (r->type != ty) { \
        freeReplyObject(r); \
        Reinit(); \
        HRETURN(RDS_ERR); \
    } \
} while(0)

        HRET CRedis::HGetall (HCSTRR key, HPSR kvs) {

            CHECK_REDIS_INS;

            kvs.clear();

            HSTR cmd = HCStr::Format("HGETALL %s", key.c_str());
            
            CMD_EXCEPS(REDIS_REPLY_ARRAY);
            
            for (HUINT i = 0; i < r->elements; i = i + 2) {
                kvs.insert(HPS::value_type(HSTR(r->element[i]->str), HSTR(r->element[i+1]->str)));
            }

            freeReplyObject(r);

            HRETURN_OK;
        }

        HRET CRedis::HKeys (HCSTRR key, HVSTRSR strs) {

            CHECK_REDIS_INS;

            strs.clear();

            HSTR cmd = HCStr::Format("HKEYS %s", key.c_str());
            
            CMD_EXCEPS(REDIS_REPLY_ARRAY);
            
            for (HUINT i = 0; i < r->elements; ++i) {
                strs.push_back(r->element[i]->str);
            }

            freeReplyObject(r);

            HRETURN_OK;
        }


//================================LIST==================================================== 
        HRET CRedis::BLPop (HCSTRR key, HSTRR val, HINT timeout) {

            CHECK_REDIS_INS;

            val.clear();

            HSTR cmd = HCStr::Format("BLPOP %s %d", key.c_str(), timeout);

            CMD_EXCEPS(REDIS_REPLY_ARRAY);
            
            if (r->elements < 2) {
                freeReplyObject(r);
                Reinit();
                HRETURN(RDS_ERR);
            } 

            val = r->element[1]->str;

            freeReplyObject(r);
            HRETURN_OK;
        }


        HRET CRedis::BRPop (HCSTRR key, HSTRR val, HINT timeout) {

            CHECK_REDIS_INS;

            val.clear();

            HSTR cmd = HCStr::Format("BRPOP %s %d", key.c_str(), timeout);

            CMD_EXCEPS(REDIS_REPLY_ARRAY);
            
            if (r->elements < 2) {
                freeReplyObject(r);
                Reinit();
                HRETURN(RDS_ERR);
            } 

            val = r->element[1]->str;

            freeReplyObject(r);
            HRETURN_OK;

        }


        HRET CRedis::LRem (HCSTRR key, HCSTRR val, HINT count) {
            
            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("LREM %s %d %s", key.c_str(), count, val.c_str());

            RET_INT_CMD;

            HRETURN_OK;

        }


        HRET CRedis::LIndex(HCSTRR key, HINT index, HSTRR val) {
            
            CHECK_REDIS_INS;

            val.clear();

            HSTR cmd = HCStr::Format("LINDEX %s %d", key.c_str(), index); 

            GET_CMD;

            HRETURN_OK;

        }


        HINT CRedis::LLen (HCSTRR key) {

            CHECK_REDIS_INS;

            HINT ret = 0;

            HSTR cmd = HCStr::Format("LLEN %s", key.c_str());
            
            INT_RET_CMD;

            return ret;

        }


        HRET CRedis::LPop (HCSTRR key, HSTRR val) {

            CHECK_REDIS_INS;

            val.clear();

            HSTR cmd = HCStr::Format("LPOP %s", key.c_str()); 

            GET_CMD;

            HRETURN_OK;

        }


        HRET CRedis::LPush (HCSTRR key, HCSTRR val) {

            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("LPUSH %s %s", key.c_str(), val.c_str());

            RET_INT_CMD;

            HRETURN_OK;

        }


        HRET CRedis::LRange (HCSTRR key, HINT start, HINT end, HVSTRSR strs) {
            
            CHECK_REDIS_INS;

            strs.clear();

            HSTR cmd = HCStr::Format("LRANGE %s %d %d", key.c_str(), start, end);
            
            CMD_EXCEPS(REDIS_REPLY_ARRAY);
            
            for (HUINT i = 0; i < r->elements; ++i) {
                strs.push_back(r->element[i]->str);
            }

            freeReplyObject(r);

            HRETURN_OK;
        }


        HRET CRedis::LSet (HCSTRR key, HINT index, HCSTRR val) {

            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("LSET %s %d %s", key.c_str(), index, val.c_str());

            SET_CMD;

            HRETURN_OK;

        }


        HRET CRedis::RPop (HCSTRR key, HSTRR val) {

            CHECK_REDIS_INS;

            val.clear();

            HSTR cmd = HCStr::Format("RPOP %s", key.c_str()); 

            GET_CMD;

            HRETURN_OK;
        }


        HRET CRedis::RPush (HCSTRR key, HCSTRR val) {

            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("RPUSH %s %s", key.c_str(), val.c_str());

            RET_INT_CMD;

            HRETURN_OK;
        }


//================================SET==================================================== 
        HRET CRedis::SAdd (HCSTRR key, HCSTRR val) {
            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("SADD %s %s", key.c_str(), val.c_str());

            RET_INT_CMD;

            HRETURN_OK;
        }


        HRET CRedis::SRem (HCSTRR key, HCSTRR val) {

            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("SREM %s %s", key.c_str(), val.c_str());

            RET_INT_CMD;

            HRETURN_OK;
        }



        HINT CRedis::SCard (HCSTRR key) {

            CHECK_REDIS_INS;

            HINT ret = 0;

            HSTR cmd = HCStr::Format("SCARD %s", key.c_str());
            
            INT_RET_CMD;

            return ret;

        }


        HBOOL CRedis::SIsMember (HCSTRR key, HCSTRR val) {
            
            CHECK_REDIS_INS;

            HSTR cmd = HCStr::Format("SISMEMBER %s %s", key.c_str(), val.c_str());

            RET_BOOL_CMD;

        }


        HRET CRedis::SMembers (HCSTRR key, HSSTRSR strs) {
            CHECK_REDIS_INS;

            strs.clear();

            HSTR cmd = HCStr::Format("SMEMBERS %s", key.c_str());
            
            CMD_EXCEPS(REDIS_REPLY_ARRAY);
            
            for (HUINT i = 0; i < r->elements; ++i) {
                strs.insert(r->element[i]->str);
            }

            freeReplyObject(r);

            HRETURN_OK;
        }


        
    }

}
