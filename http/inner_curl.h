
#ifndef __INNER_CURL_H__
#define __INNER_CURL_H__

#include <huibase.h>
#include <curl/curl.h>


namespace HUIBASE {

    namespace HTTP {

        namespace INNER_CURL {

            struct CURL_IMPL{
                CURL* handle;
            };

        }

    }

}



#endif //__INNER_CURL_H__