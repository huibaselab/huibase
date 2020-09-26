

#include "hconlock.h"

namespace HUIBASE {

HRET CConLock::Wait() {

    return HCCond::Wait(*this);

}


    void CWaitSonLock::WaitSonRun(HINT& i) {

        HFAILED_MSG(Lock(), "WaitSonRun lock failed");

        while (i == 0) {

            HFAILED_MSG(Wait(), "WaitSonRun wait failed");

        }

        i = 0;

        HFAILED_MSG(UnLock(), "WaitSonRun unlock failed");

    }


    void CWaitSonLock::SonRun(HINT &i) {

        HFAILED_MSG(Lock(), "SonRun Lock failed");

        i = 1;

        HFAILED_MSG(Signal(), "SonRun signal failed");

        HFAILED_MSG(UnLock(), "SonRun unlock failed");

    }

}
