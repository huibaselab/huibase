


#include "hcrypto.h"
#include "urlencode.h"
#include "hutil.h"
#include "base64.h"
#include "des.h"

namespace HUIBASE {


    namespace CRYPTO {


        HRET HEncode (HCSTRR strin, HSTRR strout) {

            HMEM base_mem;
            CBase64::Encode (strin, base_mem);

            HSTR strBase = huvs_str(base_mem);

            strout = UrlEncode (strBase);

            HRETURN_OK;
        }


        HRET HDecode (HCSTRR strIn, HSTRR strOut) {

            HSTR strUnUrl = UrlDecode (strIn);

            HMEM base_mem, src_mem;
            hstr_uvs(strUnUrl, base_mem);

            CBase64::Decode(base_mem, src_mem);

            strOut = huvs_str(src_mem);

            HRETURN_OK;

        }


        HRET HDes3Encode (HCSTRR strin, HSTRR strout, HCSTRR strPassword) {
            
            HMEM des3_dst, base_mem;
            HASSERT_RETURN(HIS_OK(HDes3Encrypt(strin, des3_dst, strPassword)), NO_DONE);

            CBase64::Encode (des3_dst, base_mem);

            HSTR strBase = huvs_str(base_mem);

            strout = UrlEncode (strBase);

            HRETURN_OK;
        }


        HRET HDes3Decode (HCSTRR strin, HSTRR strout, HCSTRR strPassword) {
            
            HSTR strUnUrl = UrlDecode (strin);

            HMEM base_mem, src_mem, dst_mem;
            hstr_uvs(strUnUrl, base_mem);

            CBase64::Decode(base_mem, src_mem);
            
            HASSERT_RETURN(HIS_OK(HDes3Decrypt(src_mem, dst_mem, strPassword)), NO_DONE);

            strout = huvs_str(dst_mem);

            HRETURN_OK;
        }


    }



}
