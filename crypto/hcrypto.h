

#ifndef __HCRYPTO_COMM_H__
#define __HCRYPTO_COMM_H__

#include <huibase.h>

namespace HUIBASE {

    namespace CRYPTO {

        HRET HEncode (HCSTRR strin, HSTRR strout);


        HRET HDecode (HCSTRR strin, HSTRR strout);


        HRET HDes3Encode (HCSTRR strin, HSTRR strout, HCSTRR strPassword);


        HRET HDes3Decode (HCSTRR strin, HSTRR strout, HCSTRR strPassword);

    }

}


#endif //__HCRYPTO_H__
