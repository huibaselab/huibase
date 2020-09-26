


#include "httphelper.h"
#include "inner_curl.h"


namespace HUIBASE {

namespace HTTP {

	namespace {

		HSIZE OnWriteData(void* buffer, HSIZE size, HSIZE nmemb, void* lpVoid)  {  
    		HSTR* str = dynamic_cast<HSTR*>((HSTR *)lpVoid);  
			if( NULL == str || NULL == buffer )  
			{  
				return -1;  
			}  
  
			HCHAR* pData = (HCHAR*)buffer;  
			str->append(pData, size * nmemb);  
			return nmemb;  
		}  

	}

	using namespace HUIBASE::HTTP::INNER_CURL;	

	HCurl::HCurl() {

		m_impl = new CURL_IMPL;
		CHECK_NEWPOINT(m_impl);

		m_impl->handle = curl_easy_init();
		CHECK_NEWPOINT(m_impl->handle);

	}


	HCurl::HCurl (const HCurl& curl) {

		m_impl = new CURL_IMPL;
		CHECK_NEWPOINT(m_impl);

		m_impl->handle = curl_easy_duphandle(curl.m_impl->handle);

	}


	HCurl::HCurl (HCurl&& curl)
		: m_impl(curl.m_impl) {

		curl.m_impl = nullptr;

	}


	HCurl::~HCurl () {

		if (nullptr != m_impl) {
			curl_easy_cleanup(m_impl->handle);
			HDELP(m_impl);
		}

	}	


	HRET HCurl::Get (HCSTRR strUrl, HSTRR strRes) {

		reset();

		curl_easy_setopt(m_impl->handle, CURLOPT_URL, strUrl.c_str()); 		
		curl_easy_setopt(m_impl->handle, CURLOPT_WRITEDATA, (void *)&strRes);  
		init_exec();

		CURLcode res = curl_easy_perform(m_impl->handle); 
		return (HINT)res == 0 ? HERR_NO(OK) : HERR_NO(HTTP_FAILED);

	}


	HRET HCurl::Post (HCSTRR strUrl, HCSTRR strData, HSTRR strRes) {

		reset();

		curl_easy_setopt(m_impl->handle, CURLOPT_URL, strUrl.c_str());  
		curl_easy_setopt(m_impl->handle, CURLOPT_POSTFIELDS, strData.c_str());		
		curl_easy_setopt(m_impl->handle, CURLOPT_WRITEDATA, (void *)&strRes);  
		init_exec();

		CURLcode res = curl_easy_perform(m_impl->handle); 
		return (HINT)res == 0 ? HERR_NO(OK) : HERR_NO(HTTP_FAILED);

	}


	HRET HCurl::reset () {

		curl_easy_reset(m_impl->handle);

		HRETURN_OK;
	}


	void HCurl::init_exec() {

		curl_easy_setopt(m_impl->handle, CURLOPT_WRITEFUNCTION, OnWriteData);
    	curl_easy_setopt(m_impl->handle, CURLOPT_READFUNCTION, NULL);     
    	curl_easy_setopt(m_impl->handle, CURLOPT_NOSIGNAL, 1);  
	    curl_easy_setopt(m_impl->handle, CURLOPT_SSL_VERIFYPEER, false);
    	curl_easy_setopt(m_impl->handle, CURLOPT_CONNECTTIMEOUT, m_info.CONNECTTIMEOUT);  
    	curl_easy_setopt(m_impl->handle, CURLOPT_TIMEOUT, m_info.TIMEOUT);

	}


}

}


namespace __HUI_HTTP_INSIDE__ {

class HCurlInit {
public:
	HCurlInit () {
		curl_global_init(CURL_GLOBAL_ALL);
	}

	~ HCurlInit () {
		curl_global_cleanup();
	}
};

}

static const __HUI_HTTP_INSIDE__::HCurlInit ss_init;
