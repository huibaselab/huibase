
#include <hlog.h>
#include "pool.h"

namespace HUIBASE {



CPool::CPool (HUINT uChildNum, HCSTRR strLockFile)
    :m_uChildNum(uChildNum), m_fileLock(strLockFile) {

    HASSERT_THROW_MSG(m_uChildNum < MAX_CHILD_COUNT, "CPool struction error, child num is greater than max_child_count", INVL_PARA);

}


void CPool::Init() {

    HASSERT_THROW_MSG(HIS_OK(m_fileLock.WriteLockW(0, m_uChildNum, SEEK_SET)), "CPool run file lock failed", SYS_FAILED);

    HIF_NOTOK(CreateChild(m_uChildNum)) {
        HASSERT_THROW_MSG(HIS_OK(m_fileLock.UnLock(0, m_uChildNum, SEEK_SET)), "CPool run file unlock failed", SYS_FAILED);

        return ;
    }

    HASSERT_THROW_MSG(HIS_OK(m_fileLock.UnLock(0, m_uChildNum, SEEK_SET)), "CPool run file unlock failed", SYS_FAILED);

}


HRET CPool::ManageChild() {
    //HFUN_BEGIN;

    HASSERT_THROW_MSG(HIS_OK(m_fileLock.WriteLockW(0, m_uChildNum, SEEK_SET)), "CPool run file lock failed", SYS_FAILED);

    HUINT uCount = GetChildCount();

    if (uCount != m_uChildNum) {

        LOG_WARNING("Settings-childnum: [%d], nowChildCount: [%d]", m_uChildNum, uCount);

        if (uCount > m_uChildNum) {

            KillChild(uCount - m_uChildNum);

        } else {

            CreateChild(m_uChildNum - uCount);

        }

    }

    HASSERT_THROW_MSG(HIS_OK(m_fileLock.UnLock(0, m_uChildNum, SEEK_SET)), "CPool run file unlock failed", SYS_FAILED);

    sleep(1);

    //HFUN_END;
    HRETURN_OK;
}


CWaitSonLock CThreadPool::s_sonlock;

    //CThreadKey CThreadPool::s_thread_key;

    //CThreadOnce CThreadPool::s_thread_once;

HINT CThreadPool::s_uChildCreated = 0;

    static CThreadPool* ss_pool = nullptr;

CThreadPool::CThreadPool(HUINT uChildNum, HCSTRR strLockFile)
    : CPool(uChildNum, strLockFile) {

    m_lpInfo = new ChildInfo[m_uChildNum];

    for (HUINT i = 0; i < uChildNum; ++i) {

        m_lpInfo[i].m_status = UNUSED;

    }

}


CThreadPool::~CThreadPool () {

    for (HUINT i = 0; i < m_uChildNum; ++i) {

        if (m_lpInfo[i].m_status == UNUSED) {

            continue;

        }

        HIF_NOTOK( m_lpInfo[i].m_thread.Cancel() ) {
            printf("=====================%d thread cancel failed=====================", i);
        }

        HIF_NOTOK( m_lpInfo[i].m_thread.Join(nullptr) )  {
            printf("=====================%d thread join failed=====================", i);
        }

    }

}


HUINT CThreadPool::GetChildCount() {

    HUINT res = 0;

    for (HUINT i = 0; i < m_uChildNum; ++i) {

        if (m_lpInfo[i].m_status != UNUSED) {

            ++ res;

        }

    }

    return res;

}


HRET CThreadPool::CreateChild(HUINT count) {

    SLOG_NORMAL("CThreadPool count: [%d] m_uChildNum[%d]", count, m_uChildNum);

    ChildArg* parr = new ChildArg[count];
    memset(parr, 0, sizeof(ChildArg) * count);
    CThread thread;
    /*
    HUINT uChildIndex = 0;
    for (HUINT i = 0; i < count; ++i) {

        for (; uChildIndex < m_uChildNum; ++uChildIndex) {

            if (m_lpInfo[uChildIndex].m_status == UNUSED) {
                break;
            }

        }

        SLOG_NORMAL("i[%d] index[%d] stat[%d]", i, uChildIndex, m_lpInfo[uChildIndex].m_status);
        if (m_lpInfo[uChildIndex].m_status != UNUSED) {
            break;
        }

*/

    for (HUINT i = 0; i < count; ++i) {

        //SLOG_NORMAL("i[%d] stat[%d]", i, m_lpInfo[i].m_status);
        parr[i].m_pPool = this;
        parr[i].m_uIndex = i;

        HASSERT_THROW_MSG(HIS_OK(thread.Create(ThreadInitRun, &(parr[i]))), "ThreadPool Create Child Thread Failed", SYS_FAILED);

        s_sonlock.WaitSonRun (s_uChildCreated);

        m_lpInfo[i].m_thread = thread;
        m_lpInfo[i].m_status = IDLE;

    }

    delete [] parr;

    HRETURN_OK;

}


HRET CThreadPool::KillChild(HUINT count) {

    HUINT uChildIndex = 0;

    for (HUINT i = 0; i < count; ++i) {

        for (; uChildIndex < m_uChildNum; ++uChildIndex) {

            if (m_lpInfo[uChildIndex].m_status == IDLE) {
                break;
            }

        }

        if (m_lpInfo[uChildIndex].m_status != IDLE) {
            break;
        }

        HASSERT_THROW_MSG(HIS_OK(m_lpInfo[uChildIndex].m_thread.Cancel()), "CThreadPool KillChild cancel pthread failed", SYS_FAILED);

        HASSERT_THROW_MSG(HIS_OK(m_lpInfo[uChildIndex].m_thread.Join(nullptr)), "CThreadPool KillChild join thread failed", SYS_FAILED);

        m_lpInfo[uChildIndex].m_status = UNUSED;

    }

    HRETURN_OK;

}


HRET CThreadPool::SetChildStatus(HUINT uChildIndex, CPool::ChildStatus status) {

    HASSERT_THROW_MSG(HIS_OK(m_fileLock.WriteLockW(0, m_uChildNum, SEEK_SET)), "CPool run file lock failed", SYS_FAILED);

    m_lpInfo[uChildIndex].m_status = status;

    HASSERT_THROW_MSG(HIS_OK(m_fileLock.UnLock(0, m_uChildNum, SEEK_SET)), "CPool run file unlock failed", SYS_FAILED);

    HRETURN_OK;

}


void CThreadPool::ThreadSpecifiOnce() {

    ss_pool->m_thread_key.Create(ThreadSpecifiDestructor);
    //m_thread_key.Create(ThreadSpecifiDestructor);

}


void CThreadPool::ThreadSpecifiDestructor(void *ptr) {

    HPSZ pt = static_cast<HPSZ>(ptr);

    delete [] pt;

}


void* CThreadPool::ThreadInitRun(void *arg) {

    ChildArg* pt = (ChildArg*) arg;

    CThreadPool* pool = dynamic_cast<CThreadPool*>(pt->m_pPool);

    HASSERT_THROW_MSG(pool != nullptr, "CThreadPool ThreadInitRun failed", INVL_PARA);

    s_sonlock.SonRun (s_uChildCreated);

    pool->childRun(pt->m_uIndex);

    return nullptr;
}


HPSZ CThreadPool::CreateThreadSpecific(HUINT len) {

    HPSZ res = nullptr;

    ss_pool = this;

    m_thread_once.Once(ThreadSpecifiOnce);

    if ((res = (HPSZ)m_thread_key.Get()) == nullptr) {

        res = new HCHAR[len];

        HASSERT_THROW_MSG(HIS_OK(m_thread_key.Set(res)), "CThreadPool CreateThreadSpecific key failed", SYS_FAILED);

        memset(res, 0, len);

    }

    return res;

}

}
