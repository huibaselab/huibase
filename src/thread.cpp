

#include "thread.h"


namespace HUIBASE {


    HSTR CThread::pid2s (handle_type handle) {

        HNAME buf = {0};
        HCHAR* pos = buf;
        unsigned char* usz = (unsigned char*)(void*)(&handle);

        HSTR ret("0x");

        for (size_t i = 0; i < sizeof(handle); ++i) {
            sprintf(pos, "%02x", (unsigned)(usz[i]));
            pos += 2;
        }

        ret.append(buf);
        return ret;

    }


    CThread::CThread(const HCThreadAttr& attr, thread_fun_t fun, void* arg) {

        auto cb = pthread_create(m_id.GetIdP(), attr.GetAttrP(), fun, arg);

        HASSERT_THROW_MSG(cb == 0, "pthread_create failed", SYS_FAILED);

    }



    HRET CThread::Create(const HCThreadAttr &attr, thread_fun_t fun, void *arg) {

        auto cb = pthread_create(m_id.GetIdP(), attr.GetAttrP(), fun, arg);

        HASSERT_RETURN(cb == 0, SYS_FAILED);

        HRETURN_OK;

    }


    HRET CThread::Create(thread_fun_t fun, void *arg) {

        auto cb = pthread_create(m_id.GetIdP(), nullptr, fun, arg);

        HASSERT_RETURN(cb == 0, SYS_FAILED);

        HRETURN_OK;

    }



    HRET CThread::Cancel() {

        auto cb = pthread_cancel(m_id.GetId());

        HASSERT_RETURN(cb == 0, SYS_FAILED);

        HRETURN_OK;

    }


    HRET CThread::Join(void** retval) {

        auto cb = pthread_join(m_id.GetId(), retval);

        HASSERT_RETURN(cb == 0, SYS_FAILED);

        HRETURN_OK;

    }


    HRET CThread::JoinTimeout (const HCTimeSpan& ts, void** resp) {

        struct timespec _tv = {0,0};
        _tv.tv_sec = ts.getSeconds();
        _tv.tv_nsec = 0;

        auto cb = pthread_timedjoin_np(m_id.GetId(), resp, &_tv);
        HASSERT_RETURN(cb == 0, SYS_FAILED);

        m_id = pid_t{};

        HRETURN_OK;

    }


    HRET CThread::Detach() {

        auto cb = pthread_detach(m_id.GetId());

        HASSERT_RETURN(cb == 0, SYS_FAILED);

        HRETURN_OK;

    }


    HSTR CThread::GetPidStr() const {

        return pid2s(GetHandle());

    }


    CThreadKey::CThreadKey () {

    }


    HRET CThreadKey::Create (destr_function ff) {

        HASSERT_THROW_MSG(pthread_key_create(m_id.GetIdP(), ff) == 0, "key create failed", SYS_FAILED);

        HRETURN_OK;

    }


    CThreadKey::~CThreadKey () {

        pthread_key_delete(m_id.GetId());

    }


    HRET CThreadKey::Set (const void* pointer) {

        HASSERT_RETURN(pthread_setspecific(m_id.GetId(), pointer) == 0, SYS_FAILED);

        HRETURN_OK;

    }



    void* CThreadKey::Get() {

        void* res = pthread_getspecific(m_id.GetId());

        //HASSERT_THROW_MSG((res = pthread_getspecific(m_id.GetId())) != nullptr, "pthread_getspecific failed", SYS_FAILED);

        return res;

    }


    CThreadOnce::CThreadOnce () {

        m_id.GetId () = PTHREAD_ONCE_INIT;

    }


    void CThreadOnce::Once(init_routine ff) {

        (void) pthread_once(m_id.GetIdP(), ff);

    }


}
