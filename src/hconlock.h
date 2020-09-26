

#ifndef __HCON_LOCK_H__
#define __HCON_LOCK_H__

#include "hcond.h"
#include "hmutex.h"

namespace HUIBASE {

class CConLock : public HCMutex, public HCCond {
public:
    CConLock () {}

    virtual ~ CConLock () {}

    HRET Wait ();



};


 class CWaitSonLock : public CConLock {
 public:
     CWaitSonLock () { }

     void WaitSonRun (HINT& i);


     void SonRun (HINT& i);

 };

}
#endif //__HCON_LOCK_H__
