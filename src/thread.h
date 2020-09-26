

#ifndef __THREAD_H__
#define __THREAD_H__

#include "huibase.h"
#include "hthread.h"
#include "hthreadid_t.h"
#include <pthread.h>
#include "htime.h"

namespace HUIBASE {

    class CThread {
    public:
        using handle_type = pthread_t;

        using pid_t = cid<handle_type>;

        typedef void* (*thread_fun_t)(void*);

        static HSTR pid2s (handle_type handle);

    public:
        CThread () {}

        CThread (const HCThreadAttr& attr, thread_fun_t fun, void* arg);

        HRET Create (const HCThreadAttr& attr, thread_fun_t fun, void* arg);

        HRET Create (thread_fun_t fun, void* arg);

        HRET Cancel ();

        HRET Join (void** retval);

        HRET JoinTimeout (const HCTimeSpan& ts, void** resp);

        HRET Detach ();

        handle_type GetHandle () const noexcept { return m_id.GetId(); }

        HSTR GetPidStr () const;


    private:
        pid_t m_id{};

    };


    class CThreadKey {
    public:
        using handle_type = pthread_key_t;
        using id_t = cid<handle_type>;

        typedef void (*destr_function)(void*);

    public:
        CThreadKey ();

        ~CThreadKey ();

        HRET Create (destr_function ff);

        handle_type GetHandle () const noexcept { return m_id.GetId(); }

        HRET Set (const void* pointer);


        void* Get ();


    private:
        id_t m_id{};
    };


    class CThreadOnce {
    public:
        using handle_type = pthread_once_t;

        using id_t = cid<pthread_once_t>;

        typedef void (*init_routine) (void);

    public:
        CThreadOnce ();

        void Once (init_routine ff);


    private:
        id_t m_id{};

    };

}


#endif //__THREAD_H__
