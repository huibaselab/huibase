/*
 * @Author: Tom Hui
 * @Date: 2019-12-30 10:53:05
 * @Description: 
 */


#ifndef __URLENCODE_H__
#define __URLENCODE_H__

#include <huibase.h>

namespace HUIBASE {

namespace CRYPTO {

    HSTR UrlEncode (HCSTRR str);



    HSTR UrlDecode (HCSTRR str);


    HBOOL IsUrlEncode (HCSTRR str);

}

}

#endif //__URLENCODE_H__
