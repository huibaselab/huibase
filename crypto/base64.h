

#ifndef __HCRYPTO_H__
#define __HCRYPTO_H__

#include "huibase.h"

namespace HUIBASE {

namespace CRYPTO {

    HRET Base64Encode (HCSTRR strIn, HSTRR strOut);

    HRET Base64Encode (HCUPSZ cusz, HUINT len, HSTRR strOut);

    HRET Base64Decode (HCSTRR strIn, HSTRR strOut);

    HBOOL IsBase64Str (HCSTRR str);


    class CBase64 {
    public:
        CBase64() = delete;

    public:
        static HRET EncodeFile (HCSTRR strInFilePath, HCSTRR strOutFilePath);


        static HRET DecodeFile (HCSTRR strInFilePath, HCSTRR strOutFilePath);


        static HRET EncodeBlock (HCSTRR strIn, HSTRR strOut);


        static HRET DecodeBlock (HCSTRR strIn, HSTRR strOut);


        static HRET OpensslBase64Encode (HUPSZ outData, HINT* outLen, HUPSZ inData, HINT inLen);


        static HRET OpensslBase64Decode (HUPSZ outData, HINT* outLen, HUPSZ inData, HINT inLen);


        static HRET Encode (HUPSZ inData, HINT inLen, HUPSZ outData, HINT* outLen);



        static HRET Decode (HUPSZ inData, HINT inLen, HUPSZ outData, HINT* outLen);


        static HRET Encode (HMEMR inMem, HMEMR outMem);


        static HRET Decode (HMEMR inMem, HMEMR outMem);


        static HRET Encode (HCSTRR str, HMEMR outMem);

    };

}

}

#endif //__HCRYPTO_H__


