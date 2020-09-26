

#include "binbcd.h"


namespace HUIBASE {


    namespace CRYPTO {

        static const char _hexchars[] = "0123456789abcdef";

        HRET Bin2BcdStr (HCUPSZ szByte, HINT byte_len, HUPSZ szBcd, HINT bcd_len) {

            if( bcd_len < byte_len * 2 + 1 ) {

                HRETURN(INVL_PARA);

            }

            for( HINT i = 0 ; i < byte_len; i++ ) {

                    snprintf( (HPSZ)szBcd + 2*i,   2,  "%X", (szByte[i]) >> 4  );
                    snprintf( (HPSZ)szBcd + 2*i + 1, 2, "%X", (szByte[i]& 0x0F) );

            }

            szBcd[ 2 * byte_len ] = 0;

            HRETURN_OK;
        }

        HCHAR IntToBcd (HINT n) {

            return ((n / 10) << 4) | (n % 10);

        }


        HINT BcdToInt (HUINT n) {

            return (n >> 4) * 10 + (n & 0x0f);

        }


        HRET BcdStr2Bin (HPSZ dst, HINT dst_len, HCPSZ src) {

            HINT iLen = strlen( src );

            if( 0 != iLen % 2) {

                HRETURN(INVL_PARA);

            }

            if( dst_len < iLen / 2 ) {

                HRETURN(INVL_PARA);

            }

            HCHAR c = 0;

            for( HINT i =0; i < iLen; i++ ) {

                c = src[i];

                if( (src[i] <= '9') && (src[i] >= '0') ) {

                    c -= '0';

                } else if( (src[i] <= 'F') && (src[i] >= 'A') ) {

                    c -= ('A' - 10);

                } else if( (src[i] <= 'f') && (src[i] >= 'a') ) {

                    c -= ('a' - 10);

                } else {

                    HRETURN(INVL_PARA);

                }

                if( i%2 == 1 ) {

                    dst[i/2] +=  c;

                }else {

                    dst[i/2] = c << 4;

                }
            }

            return iLen/2;

        }



        HINT Str2Bcd (HPSZ sz_bcd, HCPSZ sz_str, HINT max_len) {

            HINT  iBcd = 0, iStr = 0, len;
            HCHAR flag = 0;
            HCHAR val[3] = { 0 };

            len = strlen(sz_str);

            if( len % 2 != 0 ) {

                HRETURN(INVL_PARA);

            }

            do {

                if( !isxdigit( sz_str[iStr] ) ) {

                        continue;

                }


                val[(HINT)flag] = sz_str[iStr];

                if( flag ){

                    sz_bcd[iBcd ++] = strtoul( val, NULL, 16 );

                }

                flag = flag ? 0 : 1;

            }while( iBcd < max_len && ++iStr < len );

            memset( sz_bcd + iBcd, 0x00, max_len - iBcd );

            return iBcd;


        }

        void HexToStr (HCMEMR mem, HSTRR str) {
            str.clear();
            str = Hex2Str(mem);

        }

        constexpr HCHAR sz_hexchars[256] =
            { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,
              -1,0xa,0xb,0xc,0xd,0xe,0xf,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,0xa,0xb,0xc,0xd,0xe,0xf,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, };

        HCHAR HexChar (HCHAR c) {

            return sz_hexchars[(HINT)c];

        }

        HBOOL IsHexStr (HCSTRR str) {

            for (HSTR::const_iterator cit = str.begin(); cit != str.end(); ++cit) {
                if (HexChar(*cit) < 0) {
                    return HFALSE;
                }
            }

            HRET_BOOL(str.size() > 0 && str.size() % 2 == 0);

        }

        void ParseHex(HCSTRR str, HMEMR mem) {

            ParseHex(str.c_str(), mem);

        }

        void ParseHex(HCPSZ psz, HMEMR mem) {

            while (true) {

                while (isspace(*psz))
                    psz++;

                HCHAR c = HexChar(*psz++);
                if (c == (HCHAR)-1)
                    break;

                HCHAR n = (c << 4);
                c = HexChar(*psz++);
                if (c == (HCHAR)-1)
                    break;
                n |= c;
                mem.push_back(n);
            }

        }


        void Bin2Hex (HCMEMR in, HSTRR out) {
            
            Bin2Hex((HUPSZ)in.data(), in.size(), out);

        }


        void Bin2Hex (HCUPSZ in, HSIZE len, HSTRR out) {
            out.clear();
            HCUPSZ p = in;
            while (len--) {
                out.push_back(_hexchars[p[0] >> 4]);
                out.push_back(_hexchars[p[0] & 0xf]);
                ++p;
            }
        }


    }


}
