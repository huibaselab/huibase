/*
 * @Author: Tom Hui
 * @Date: 2019-12-30 10:53:05
 * @Description: 
 */




#ifndef __HTTPHELPER_H__
#define __HTTPHELPER_H__


#include <huibase.h>
#include <memory>


namespace HUIBASE {

namespace HTTP {

	namespace INNER_CURL {
		struct CURL_IMPL;
	}

	class HCurl {
	public:
		typedef struct {
			HINT CONNECTTIMEOUT = 3;
			HINT TIMEOUT = 10;
		} CURL_INFO;
	public:
		HCurl ();
		HCurl (const HCurl& curl);
		HCurl (HCurl&& curl);		

		~ HCurl();

	public:		
		HRET Get (HCSTRR strUrl, HSTRR strRes);

		HRET Post (HCSTRR strUrl, HCSTRR strData, HSTRR strRes);

		void SetConnectTimeout (HINT CONNECTTIMEOUT = 3) { m_info.CONNECTTIMEOUT = CONNECTTIMEOUT; }

		void SetTimeout (HINT TIMEOUT = 10) { m_info.TIMEOUT = TIMEOUT; }

	private:
		// Re-initializes a CURL handle to the default values.
		HRET reset ();

		void init_exec();

	private:
		INNER_CURL::CURL_IMPL* m_impl = nullptr;

		CURL_INFO m_info;
	};

}

}



#endif //__HTTPHELPER_H__


