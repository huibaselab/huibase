



#include "md5.h"

#include <openssl/md5.h>

#include "binbcd.h"

namespace HUIBASE {


    namespace CRYPTO {

        HSTR Md5 (HCSTRR str) {

            unsigned char md[16];
            ::MD5((const unsigned char*)str.c_str(), str.size(), md);

            HCHAR buf[1024] = { 0 };
            int len = sizeof(buf) - 1;
            HASSERT_THROW(HIS_OK(Bin2BcdStr((HCUPSZ) md, sizeof(md), (HUPSZ)buf, len)), INVL_RES);

            HSTR result(buf);
            std::transform(result.begin(), result.end(), result.begin(), ::tolower);

            return result;

        }


    }


}
