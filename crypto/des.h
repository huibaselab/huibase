


#ifndef __HUIBASE_DES_H__
#define __HUIBASE_DES_H__


#include <huibase.h>


namespace HUIBASE {


namespace CRYPTO {


    HRET HDes3Encrypt (HUPSZ in, HINT inlen, HUPSZ out, HINT* outlen, HCSTRR password);


    HRET HDes3Decrypt (HUPSZ in, HINT inlen, HUPSZ out, HINT* outlen, HCSTRR password);


    HRET HDes3Encrypt (HMEMR src, HMEMR dst, HCSTRR passwd);


    HRET HDes3Encrypt (HCSTRR src, HMEMR dst, HCSTRR passwd);


    HRET HDes3Decrypt (HMEMR src, HMEMR dst, HCSTRR passwd);

    // pkcs5padding.
    HRET Des3EcbEncrypt (HUPSZ in, HINT inlen, HUPSZ out, HINT* outlen, HCSTRR password);


    HRET Des3EcbDecrypt (HUPSZ in, HINT inlen, HUPSZ out, HINT* outlen, HCSTRR password);


    HRET Des3EcbEncrypt (HMEMR src, HMEMR dst, HCSTRR passwd);


    HRET Des3EcbEncrypt (HCSTRR src, HMEMR dst, HCSTRR passwd);


    HRET Des3EcbDecrypt (HMEMR src, HMEMR dst, HCSTRR passwd);

}


}


#endif //__HUIBASE_DES_H__
