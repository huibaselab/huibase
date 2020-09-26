


#include "urlencode.h"


namespace HUIBASE {


namespace CRYPTO {


    static inline unsigned char ToHex(unsigned char x) {

        return  x > 9 ? x + 55 : x + 48;

    }


    unsigned char FromHex(unsigned char x) {

        unsigned char y;

        if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
        else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
        else if (x >= '0' && x <= '9') y = x - '0';
        else assert(0);
        return y;

    }


    HSTR UrlEncode(HCSTRR str){


        HSTR strTemp = "";
        size_t length = str.length();

        for (size_t i = 0; i < length; i++) {

            if (isalnum((unsigned char)str[i]) ||

                (str[i] == '-') ||
                (str[i] == '_') ||
                (str[i] == '.') ||
                (str[i] == '~'))
                strTemp += str[i];

            else if (str[i] == ' ')

                strTemp += "+";

            else {

                strTemp += '%';
                strTemp += ToHex((unsigned char)str[i] >> 4);
                strTemp += ToHex((unsigned char)str[i] % 16);

            }
        }
        return strTemp;
    }


    HSTR UrlDecode(HCSTRR str) {

        HSTR strTemp = "";

        size_t length = str.length();

        for (size_t i = 0; i < length; i++) {

            if (str[i] == '+') strTemp += ' ';

            else if (str[i] == '%') {

                assert(i + 2 < length);

                unsigned char high = FromHex((unsigned char)str[++i]);

                unsigned char low = FromHex((unsigned char)str[++i]);

                strTemp += high*16 + low;

            }

            else strTemp += str[i];

        }

        return strTemp;

    }


    HBOOL IsUrlEncode (HCSTRR str) {

        auto good_char = [] (HCHAR c) -> bool {
            if (c >= 'A' && c <= 'Z') return true;
            else if (c >= '0' && c <= '9') return true;
            else return false;
            return false;
        };

        HINT len = str.length();
        if (str[len - 2] == '%' or str[len - 1] == '%') {
            return HFALSE;
        }

        for (size_t i = 0; i < str.length(); i++) {

            if (str[i] == '%') {
                
                if (not good_char(str[i+1]) or not good_char(str[i+2])) {

                    return HFALSE;

                }

            }

        }

        return HTRUE;

    }


}


}
