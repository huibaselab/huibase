


#ifndef __BINBCD_H__
#define __BINBCD_H__

#include <huibase.h>

namespace HUIBASE {


    namespace CRYPTO {

        HRET Bin2BcdStr (HCUPSZ cusz, HINT len, HUPSZ usz, HINT rlen);


        HCHAR IntToBcd (HINT n);


        HINT BcdToInt (HUINT n);


        HINT BcdStr2Bin (HPSZ dst, HINT dst_len, HCPSZ src);


        HINT Str2Bcd (HPSZ sz_bcd, HCPSZ sz_str, HINT max_len);

        template<typename T>
        std::string Hex2Str(const T itbegin, const T itend, bool fSpaces=false) {
            std::string rv;
            static constexpr char hexmap[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                                             '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
            rv.reserve((itend-itbegin)*3);
            for(T it = itbegin; it < itend; ++it)
                {
                    unsigned char val = (unsigned char)(*it);
                    if(fSpaces && it != itbegin)
                        rv.push_back(' ');
                    rv.push_back(hexmap[val>>4]);
                    rv.push_back(hexmap[val&15]);
                }

            return rv;
        }

        template<typename T>
        inline std::string Hex2Str(const T& vch, bool fSpaces=false) {
            return Hex2Str(vch.begin(), vch.end(), fSpaces);
        }

        void HexToStr (HCMEMR mem, HSTRR str);

        HCHAR HexChar(HCHAR c);

        HBOOL IsHexStr (HCSTRR str);

        void ParseHex(HCSTRR str, HMEMR mem);

        void ParseHex(HCPSZ psz, HMEMR mem);

        void Bin2Hex (HCMEMR in, HSTRR out);

        void Bin2Hex (HCUPSZ in, HSIZE len, HSTRR out);

    }

}

#endif //__BIN2BCD_H__
