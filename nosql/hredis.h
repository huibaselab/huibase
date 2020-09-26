/*******************************************************
 *
 * FileName: hredis.h
 *
 * Author: Tom Hui, tomhui1009@yahoo.com, 8613760232170
 *
 * Create Date: Sun Mar 11 01:03 2018
 *
 * Brief:
 *   wrapper redis.
 *
 *******************************************************/

#ifndef __HREDIS_H__
#define __HREDIS_H__

#include <huibase.h>
#include "nosql_comm.h"
#include <hiredis.h>

namespace HUIBASE {

    namespace NOSQL {

        typedef redisContext redis_base;

        typedef redisReply redis_res;

        class CRedis {
        public:
            static constexpr HINT REDIS_DEF_EXPIRE = 3;
            
        public:
            CRedis (const NoSqlConnectionInfo& info);

            virtual ~CRedis ();

            HRET Init ();

            HRET Reinit();

            void Release ();

        public:
            HCSTRR GetName() const { return m_info.strName; }

            void SetName(HCSTRR strName) { m_info.strName = strName; }

            // single
            HRET Set (HCSTRR key, HCSTRR val);

            HRET Setex (HCSTRR key, HCSTRR val, HINT exp = REDIS_DEF_EXPIRE);

            HBOOL Setnx(HCSTRR key, HCSTRR val);

            HRET Get (HCSTRR key, HSTRR val);

            HRET Get (HCSTRR key, HSTRR val, HCSTRR defval);

            HRET Expire (HCSTRR key, HINT sec);

            HBOOL Exists (HCSTRR key);

            HRET Del (HCSTRR strKey);

            HRET Incr (HCSTRR key);

            HRET Decr (HCSTRR key);

            // for hash
            HRET HSet (HCSTRR key, HCSTRR field, HCSTRR val);

            HBOOL HSetnx (HCSTRR key, HCSTRR field, HCSTRR val);

            HRET HGet (HCSTRR key, HCSTRR field, HSTRR val);

            HRET HGet (HCSTRR key, HCSTRR field, HSTRR val, HCSTRR defval);

            HBOOL HExists (HCSTRR key, HCSTRR field);

            HRET HDel (HCSTRR key, HCSTRR field);

            HINT HLen (HCSTRR key);

            HRET HIncrby (HCSTRR key, HCSTRR field, HINT by);

            HRET HGetall (HCSTRR key, HPSR kvs);

            HRET HKeys (HCSTRR key, HVSTRSR strs);

            // for list.
            HRET BLPop (HCSTRR key, HSTRR val, HINT timeout = 0);

            HRET BRPop (HCSTRR key, HSTRR val, HINT timeout = 0);

            HRET LRem (HCSTRR key, HCSTRR val, HINT count = 0);

            HRET LIndex (HCSTRR key, HINT index, HSTRR val);

            HINT LLen (HCSTRR key);

            HRET LPop (HCSTRR key, HSTRR val);

            HRET LPush (HCSTRR key, HCSTRR val);

            HRET LRange (HCSTRR key, HINT start, HINT end, HVSTRSR strs);

            HRET LSet (HCSTRR key, HINT index, HCSTRR val);

            HRET RPop (HCSTRR key, HSTRR val);

            HRET RPush (HCSTRR key, HCSTRR val);

            // for set
            HRET SAdd (HCSTRR key, HCSTRR val);
            
            HRET SRem (HCSTRR key, HCSTRR val);

            HINT SCard (HCSTRR key);

            HBOOL SIsMember (HCSTRR key, HCSTRR val);

            HRET SMembers (HCSTRR key, HSSTRSR strs);
            
        private:
            redis_base* m_context = nullptr;

            NoSqlConnectionInfo m_info;
        };

    }

}



#endif //__HREDIS_H__
