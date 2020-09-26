


#include "des.h"

#include <openssl/des.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#include <hutil.h>

namespace HUIBASE {

namespace CRYPTO{


    HRET HDes3Encrypt (HUPSZ in, HINT inlen, HUPSZ out, HINT* outlen, HCSTRR password) {
        const EVP_CIPHER * cipher = EVP_des_ede3_ecb( );

        HASSERT_THROW(cipher != nullptr, SSL_ERR);

        unsigned char key[EVP_MAX_KEY_LENGTH];
        unsigned char iv[EVP_MAX_KEY_LENGTH];

        EVP_BytesToKey(cipher,EVP_md5(), nullptr, (HCUPSZ)password.c_str(), password.size(), 1, key, iv);

        EVP_CIPHER_CTX ctx;
        EVP_CIPHER_CTX_init(&ctx);

        EVP_CIPHER_CTX_set_padding(&ctx,0);

        int rv = EVP_EncryptInit_ex(&ctx, cipher, nullptr, key, iv);

        if(rv != 1){

            EVP_CIPHER_CTX_cleanup(&ctx);
            HASSERT_THROW(false, SSL_ERR);

        }

        rv = EVP_EncryptUpdate(&ctx,out, outlen, in, inlen);

        if(rv != 1){

            EVP_CIPHER_CTX_cleanup(&ctx);
            HASSERT_THROW(false, SSL_ERR);
        }

        HINT templen = 0;
        rv = EVP_EncryptFinal_ex(&ctx,out + *outlen, &templen);

        if(rv != 1) {

            EVP_CIPHER_CTX_cleanup(&ctx);
            HASSERT_THROW(false, SSL_ERR);

        }


        EVP_CIPHER_CTX_cleanup(&ctx);

        *outlen += templen;

        HRETURN_OK;
    }


    HRET HDes3Decrypt (HUPSZ in, HINT inlen, HUPSZ out, HINT* outlen, HCSTRR password) {

        const EVP_CIPHER * cipher = EVP_des_ede3_ecb( );
        HASSERT_THROW(cipher != nullptr, SSL_ERR);

        unsigned char key[EVP_MAX_KEY_LENGTH];
        unsigned char iv[EVP_MAX_KEY_LENGTH];

        EVP_BytesToKey(cipher, EVP_md5(), nullptr, (HCUPSZ)password.c_str(), password.size(), 1, key, iv);

        EVP_CIPHER_CTX ctx;
        EVP_CIPHER_CTX_init(&ctx);

        int rv = EVP_DecryptInit_ex(&ctx, cipher, nullptr, key, iv);

        if(rv != 1) {

            EVP_CIPHER_CTX_cleanup(&ctx);
            HASSERT_THROW(false, SSL_ERR);

        }

        rv = EVP_DecryptUpdate(&ctx,out, outlen, in, inlen);
        if(rv != 1) {

            EVP_CIPHER_CTX_cleanup(&ctx);
            HASSERT_THROW(false, SSL_ERR);

        }

        HINT templen = 0;
        rv = EVP_DecryptFinal_ex(&ctx,out + *outlen, &templen);
        if(rv != 1) {

            EVP_CIPHER_CTX_cleanup(&ctx);
            HASSERT_THROW(false, SSL_ERR);

        }

        EVP_CIPHER_CTX_cleanup(&ctx);

        *outlen += templen;

        HRETURN_OK;
    }


#define INNER_DES3_EN_MMS(fun) HINT src_len = src.size() * 2; \
        HUPSZ out = new HUCHAR[src_len]; \
        CHECK_NEWPOINT(out); \
        ZERO_MEM(out, src_len); \
        HRET cb = 0; \
        do { \
            cb = fun(&src[0], src.size(), out, &src_len, passwd);\
        } while (0); \
        std::copy(out, out + src_len, std::back_inserter(dst)); \
        HDEL_ARR(out); \
        return cb


    HRET HDes3Encrypt (HMEMR src, HMEMR dst, HCSTRR passwd) {

        INNER_DES3_EN_MMS(HDes3Encrypt);

    }

#define INNER_DES3_EN_SMS(fun) HMEM src_mem; \
        hstr_uvs (src, src_mem); \
        return fun(src_mem, mem, passwd)


    HRET HDes3Encrypt (HCSTRR src, HMEMR mem, HCSTRR passwd) {

        INNER_DES3_EN_SMS(HDes3Encrypt);

    }


#define INNER_DES3_DE_MMS(fun) HINT out_len = src.size() * 2; \
        HUPSZ out = new HUCHAR[out_len]; \
        CHECK_NEWPOINT(out); \
        ZERO_MEM(out, out_len); \
        HRET cb = 0; \
        do { \
            cb = fun(&src[0], src.size(), out, &out_len, passwd); \
        } while (0); \
        std::copy (out, out + out_len, std::back_inserter(dst)); \
        HDEL_ARR(out); \
        return cb

    HRET HDes3Decrypt (HMEMR src, HMEMR dst, HCSTRR passwd) {

        INNER_DES3_DE_MMS(HDes3Decrypt);

    }


    HRET Des3EcbEncrypt (HUPSZ in, HINT inlen, HUPSZ out, HINT* outlen, HCSTRR password) {
        const EVP_CIPHER * cipher = EVP_des_ede3_ecb( );

        HASSERT_THROW(cipher != nullptr, SSL_ERR);

        unsigned char key[EVP_MAX_KEY_LENGTH] = {0};
        unsigned char iv[EVP_MAX_KEY_LENGTH] = {0};

        memcpy(key, password.c_str(), password.length());

        EVP_CIPHER_CTX ctx;
        EVP_CIPHER_CTX_init(&ctx);

        EVP_CIPHER_CTX_set_padding(&ctx,0);

        int rv = EVP_EncryptInit_ex(&ctx, cipher, nullptr, key, iv);

        if(rv != 1){

            EVP_CIPHER_CTX_cleanup(&ctx);
            HASSERT_THROW(false, SSL_ERR);

        }

        rv = EVP_EncryptUpdate(&ctx,out, outlen, in, inlen);

        if(rv != 1){

            EVP_CIPHER_CTX_cleanup(&ctx);
            HASSERT_THROW(false, SSL_ERR);
        }

        HINT templen = 0;
        rv = EVP_EncryptFinal_ex(&ctx,out + *outlen, &templen);

        if(rv != 1) {

            EVP_CIPHER_CTX_cleanup(&ctx);
            HASSERT_THROW(false, SSL_ERR);

        }


        EVP_CIPHER_CTX_cleanup(&ctx);

        *outlen += templen;

        HRETURN_OK;
    }


    HRET Des3EcbDecrypt (HUPSZ in, HINT inlen, HUPSZ out, HINT* outlen, HCSTRR password) {

        const EVP_CIPHER * cipher = EVP_des_ede3_ecb( );
        HASSERT_THROW(cipher != nullptr, SSL_ERR);

        unsigned char key[EVP_MAX_KEY_LENGTH] = {0};
        unsigned char iv[EVP_MAX_KEY_LENGTH] = {0};

        memcpy(key, password.c_str(), password.length());

        EVP_CIPHER_CTX ctx;
        EVP_CIPHER_CTX_init(&ctx);

        int rv = EVP_DecryptInit_ex(&ctx, cipher, nullptr, key, iv);

        if(rv != 1) {

            EVP_CIPHER_CTX_cleanup(&ctx);
            HASSERT_THROW(false, SSL_ERR);

        }

        rv = EVP_DecryptUpdate(&ctx,out, outlen, in, inlen);
        if(rv != 1) {

            EVP_CIPHER_CTX_cleanup(&ctx);
            HASSERT_THROW(false, SSL_ERR);

        }

        HINT templen = 0;
        rv = EVP_DecryptFinal_ex(&ctx,out + *outlen, &templen);
        if(rv != 1) {

            EVP_CIPHER_CTX_cleanup(&ctx);
            HASSERT_THROW(false, SSL_ERR);

        }

        EVP_CIPHER_CTX_cleanup(&ctx);

        *outlen += templen;

        HRETURN_OK;
    }


    HRET Des3EcbEncrypt (HMEMR src, HMEMR dst, HCSTRR passwd) {
        INNER_DES3_EN_MMS(Des3EcbEncrypt);
    }


    HRET Des3EcbEncrypt (HCSTRR src, HMEMR mem, HCSTRR passwd) {
        INNER_DES3_EN_SMS(Des3EcbEncrypt);
    }


    HRET Des3EcbDecrypt (HMEMR src, HMEMR dst, HCSTRR passwd) {
        INNER_DES3_DE_MMS(Des3EcbDecrypt);
    }

}


}
