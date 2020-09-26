

#include <hutil.h>
#include "base64.h"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>


namespace HUIBASE {

namespace CRYPTO {

	HRET Base64Encode (HCSTRR strIn, HSTRR strOut) {
	
		BIO *bio = NULL, *b64 = NULL;
		BUF_MEM *bufferPtr;

		b64 = BIO_new(BIO_f_base64());
		bio = BIO_new(BIO_s_mem());
		bio = BIO_push(b64, bio);

		BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line

		BIO_write(bio, (const unsigned char*)strIn.c_str(), strIn.length());
		(void)BIO_flush(bio);		
		BIO_get_mem_ptr(bio, &bufferPtr);

		char* outbuf = (char*)malloc(bufferPtr->length + 1);
		memset(outbuf, 0, bufferPtr->length + 1);
		memcpy(outbuf, bufferPtr->data, bufferPtr->length);

		strOut = outbuf;
		free(outbuf);

		//BIO_set_close(bio, BIO_NOCLOSE);
		BIO_free_all(bio);

		HRETURN_OK;

	}

    HRET Base64Encode (HCUPSZ cusz, HUINT len, HSTRR strOut) {
        BIO *bio = NULL, *b64 = NULL;
		BUF_MEM *bufferPtr;

		b64 = BIO_new(BIO_f_base64());
		bio = BIO_new(BIO_s_mem());
		bio = BIO_push(b64, bio);

		BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line

		BIO_write(bio, cusz, len);
		(void)BIO_flush(bio);
		BIO_get_mem_ptr(bio, &bufferPtr);

		char* outbuf = (char*)malloc(bufferPtr->length + 1);
		memset(outbuf, 0, bufferPtr->length + 1);
		memcpy(outbuf, bufferPtr->data, bufferPtr->length);

		strOut = outbuf;
		free(outbuf);

		//BIO_set_close(bio, BIO_NOCLOSE);
		BIO_free_all(bio);

		HRETURN_OK;
    }

    /*
	HRET Base64Decode (HCSTRR strIn, HSTRR strOut) {

		auto __f = [] (HCSTRR str) ->HSIZE {

			HSIZE len = str.length(), padding = 0;

			if (str[len - 1] == '=' && str[len-2] == '=') { //last two chars are =
				padding = 2;
			}
			else if (str[len-1] == '=') {//last char is =
				padding = 1;
			}
			
			return (len*3)/4 - padding;
		};

		BIO *bio = NULL, *b64 = NULL;

		HSIZE _len = __f(strIn);
		unsigned char* buf = (unsigned char*)malloc(_len + 1);
		memset(buf, 0, _len+1);

		bio = BIO_new_mem_buf(const_cast<char*>(strIn.c_str()), -1);
		b64 = BIO_new(BIO_f_base64());
		bio = BIO_push(b64, bio);

		BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
		BIO_read(bio, buf, _len);

		strOut = HSTR(reinterpret_cast<char*>(buf));
		free(buf);
		
		BIO_free_all(bio);	

		HRETURN_OK;

        }*/

    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";


    static inline bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

    HRET Base64Decode (HCSTRR strIn, HSTRR ret) {

        int in_len = strIn.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];

        while (in_len-- && ( strIn[in_] != '=') && is_base64(strIn[in_])) {

            char_array_4[i++] = strIn[in_]; in_++;

            if (i ==4) {
                for (i = 0; i <4; i++)
                    char_array_4[i] = base64_chars.find(char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    ret += char_array_3[i];

                i = 0;
            }
        }

        if (i) {
            for (j = i; j <4; j++)
                char_array_4[j] = 0;

            for (j = 0; j <4; j++)
                char_array_4[j] = base64_chars.find(char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
        }

        HRETURN_OK;

    }


    HBOOL IsBase64Str (HCSTRR str) {

        HSIZE ss = str.size(), e1 = ss - 1, e2 = ss - 2; 
        for (size_t i = 0; i < str.size(); ++i) {

            HCHAR c = str[i];

            if (not is_base64(c)) {

                if ((i == e1 or i == e2) and c == '=') {
                    
                    continue;

                }

                return HFALSE;

            }

        }

        return HTRUE;

    }


    HRET CBase64::EncodeFile(HCSTRR strInFilePath, HCSTRR strOutFilePath) {

        FILE* infp = fopen(strInFilePath.c_str(), "rb");

        HASSERT_THROW(infp != nullptr, SRC_FAIL);

        FILE* outfp = fopen(strOutFilePath.c_str(),"w");

        HASSERT_THROW(outfp != nullptr, SRC_FAIL);

        EVP_ENCODE_CTX ctx;
        EVP_EncodeInit(&ctx);

        unsigned char in[1024] = {0};
        int inl = 0;
        char out[2048]={0};
        int outl = 0;

        while(1) {

            inl = fread(in,1,1024,infp);

            if(inl <= 0)
                break;

            EVP_EncodeUpdate(&ctx,(unsigned char *)out,&outl,(unsigned char *)in,inl);

            fwrite(out,1,outl,outfp);

        }

        EVP_EncodeFinal(&ctx,(unsigned char *)out,&outl);

        fwrite(out,1,outl,outfp);

        fclose(infp);
        fclose(outfp);

        HRETURN_OK;

    }


    HRET CBase64::DecodeFile(HCSTRR strInFilePath, HCSTRR strOutFilePath) {

        FILE* infp = fopen(strInFilePath.c_str(),"r");
        HASSERT_THROW(infp != nullptr, SRC_FAIL);

        FILE* outfp = fopen(strOutFilePath.c_str(),"wb");
        HASSERT_THROW(outfp != nullptr, SRC_FAIL);

        EVP_ENCODE_CTX ctx;
        EVP_DecodeInit(&ctx);

        char in[1024] = {0};
        int inl = 0;
        unsigned char out[1024] = {0};
        int outl = 0;
        int ret = 0;
        while(1) {

            inl = fread(in,1,1024,infp);

            if(inl <= 0)
                break;

            ret = EVP_DecodeUpdate(&ctx,out,&outl,(unsigned char *)in,inl);

            if (ret < 0)
                break;

            fwrite(out,1,outl,outfp);

        }

        EVP_DecodeFinal(&ctx,out,&outl);
        fwrite(out,1,outl,outfp);
        fclose(infp);
        fclose(outfp);

        HASSERT_THROW(ret >= 0, INVL_RES);

        HRETURN_OK;
    }


    HRET CBase64::EncodeBlock(HCSTRR strIn, HSTRR strOut) {

        char result[32*1024] = {0};

        int out_len = 0;

        out_len = EVP_EncodeBlock((unsigned char *) result, (const unsigned char *) strIn.c_str(), strIn.length());

        if (out_len > 0 && result[out_len - 1] == '\n') {

            result[out_len - 1] = '\0';
            out_len--;

        }

        strOut = HSTR(result, out_len);

        HRETURN_OK;
    }


    HRET CBase64::DecodeBlock (HCSTRR strIn, HSTRR strOut) {

        char result[32*1024] = {0};
        int out_len = EVP_DecodeBlock((unsigned char*) result, (const unsigned char *) strIn.c_str(), strIn.length());

        HASSERT_THROW(out_len >= 0, INVL_RES);

        HINT len = strIn.length();
        if(len>=2&& strIn[len -1 ]=='=' && strIn[len-2]=='=' && out_len>=2 && result[out_len-1]=='\0' && result[out_len-2]=='\0') {

            out_len-=2;

        }
        else if(len>=1 && strIn[len-1]=='=' && out_len>=1 && result[out_len-1]=='\0'){
            out_len-=1;
        }

        strOut = HSTR(result, out_len);

        HRETURN_OK;
    }


    HRET CBase64::OpensslBase64Encode(HUPSZ outData, HINT* outLen, HUPSZ inData, HINT inLen) {

        HINT tmp=0;
        EVP_ENCODE_CTX base64;
        EVP_EncodeInit(&base64);
        EVP_EncodeUpdate(&base64,
                         outData,
                         outLen,
                         inData,
                         inLen
                         );
        tmp = *outLen;
        EVP_EncodeFinal(&base64,outData + *outLen, outLen);
        *outLen += tmp;
        outData[*outLen]=0;

        HRETURN_OK;
    }


    HRET CBase64::OpensslBase64Decode(HUPSZ outData, HINT* outLen, HUPSZ inData, HINT inLen) {

        HINT tmp=0,i=0;
        EVP_ENCODE_CTX base64;
        EVP_DecodeInit(&base64);

        i=EVP_DecodeUpdate(&base64,
                           outData,
                           outLen,
                           inData,
                           inLen);

        HASSERT_THROW(i >= 0, INVL_RES);
        tmp+=*outLen;

        EVP_DecodeFinal(&base64,outData+tmp,outLen);
        *outLen+=tmp;

        outData[*outLen]=0;

        HRETURN_OK;
    }


    HRET CBase64::Encode(HUPSZ inData, HINT inLen, HUPSZ outData, HINT* outLen) {

        return OpensslBase64Encode(outData, outLen, inData, inLen);

    }


    HRET CBase64::Decode (HUPSZ inData, HINT inLen, HUPSZ outData, HINT* outLen) {

        return OpensslBase64Decode (outData, outLen, inData, inLen);

    }


    HRET CBase64::Encode (HMEMR inMem, HMEMR outMem) {

        HINT len = inMem.size() * 4;

        HUPSZ mem = new HUCHAR[len];
        CHECK_NEWPOINT(mem);

        ZERO_MEM(mem, len);

        do {

            (void) Encode(&inMem[0], inMem.size(), mem, &len);

            std::copy(mem, mem + len, std::back_inserter(outMem));

        } while (0);

        HDEL_ARR(mem);

        HRETURN_OK;

    }



    HRET CBase64::Decode (HMEMR inMem, HMEMR outMem) {

        HINT len = inMem.size();

        HUPSZ mem = new HUCHAR[len];
        CHECK_NEWPOINT(mem);

        ZERO_MEM(mem, len);

        do {

            (void) Decode(&inMem[0], inMem.size(), mem, &len);

            std::copy(mem, mem + len, std::back_inserter(outMem));

        } while (0);

        HDEL_ARR(mem);

        HRETURN_OK;

    }


    HRET CBase64::Encode (HCSTRR str, HMEMR outMem) {

        HMEM mem;
        hstr_uvs (str, mem);

        return Encode (mem, outMem);

    }

}

}
