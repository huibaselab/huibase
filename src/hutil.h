

#ifndef __HADAPTATION_H__
#define __HADAPTATION_H__


#include "huibase.h"

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include "hdict.h"



namespace HUIBASE {

    HRET hsleep(HUINT sec, HUINT nMinSec = 0);

    HCSTRR hcmap_get (HCPSR ps, HCSTRR strKey);

    HUINT hget_rand (HUINT n = 0);

    inline void hstr_uvs (HCSTRR str, HMEMR mem) {
        std::copy(str.begin(), str.end(), std::back_inserter(mem));
    }


    inline HSTR huvs_str(HCMEMR mem) {

        HSTR str;

        std::copy (mem.begin(), mem.end(), std::back_inserter(str));

        return str;
    }


    inline void hstr_vs (HCSTRR str, HBUFR buf) {

        std::copy (str.begin(), str.end(), std::back_inserter(buf));

    }

    HDOUBLE GetRang (HUINT n);

    void hvs2map (HCKVSR ckvs, HCParam& ps);
}

#endif
