/*******************************************************
 *
 * FileName: pool.h
 *
 * Author: Tom Hui, tomhui1009@yahoo.com, 8613760232170
 *
 * Create Date: Wed May 30 15:45 2018
 *
 * Brief:
 *
 *
 *******************************************************/


#ifndef __MIDDLE_POOL_H__
#define __MIDDLE_POOL_H__

#include <huibase.h>
#include <thread.h>
#include <hfilelock.h>
#include <hconlock.h>

#include <sys/types.h>


namespace HUIBASE{

class CPool {
 public:
    static constexpr HUINT MAX_CHILD_COUNT = 1024;
    static constexpr HUINT MAIN_SLEEP_TIME = 1;
    static constexpr HINT KILL_SIGNAL = SIGTERM;

    enum ChildStatus { UNUSED = 0, IDLE = 1, BUSY = 2};

 protected:
    HUINT m_uChildNum = 0;

    HCFileLock m_fileLock;

 public:
    virtual ~ CPool () { }

    virtual void Init ();

    virtual HRET ManageChild ();

    virtual HRET SetChildStatus(HUINT chindIndex, ChildStatus status) = 0;

 protected:
    CPool (HUINT uChildNum, HCSTRR strLockFileName);

    virtual HUINT GetChildCount () = 0;

    virtual HRET CreateChild (HUINT count) = 0;

    virtual HRET KillChild (HUINT num) = 0;
};


class CThreadPool : public CPool {
 public:
    typedef struct _ChildInfo {
        CThread m_thread;
        ChildStatus m_status;
        _ChildInfo () : m_status(UNUSED) {
        }
    } ChildInfo;

    typedef struct _ChildArg {
        CThreadPool* m_pPool;
        HUINT m_uIndex;
    } ChildArg;

 public:
    virtual ~CThreadPool ();

 protected:
    CThreadPool (HUINT uChildNum, HCSTRR strLockFile);

 protected:
    static CWaitSonLock s_sonlock;

    CThreadOnce m_thread_once;

    static HINT s_uChildCreated;

    static void ThreadSpecifiOnce ();
    static void ThreadSpecifiDestructor (void* ptr);

    static void* ThreadInitRun (void* arg);

 public:
    CThreadKey m_thread_key;

 protected:
    virtual HUINT GetChildCount () override;

    virtual HRET CreateChild (HUINT count) override;

    virtual HRET KillChild (HUINT count)  override;

    virtual HRET SetChildStatus (HUINT uChildIndex, ChildStatus status) override;

    virtual HPSZ CreateThreadSpecific (HUINT len);

    virtual void childRun (HUINT uChildIndex) = 0;

 protected:
    ChildInfo * m_lpInfo = nullptr;
};

}
#endif //__MIDDLE_POOL_H__
