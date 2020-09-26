

#include "hstr.h"

namespace HUIBASE {

    HCStr& HCStr::appendFormat(const value_type *fmt, ...) {

	HCStr newstr;

	va_list ap;

	va_start(ap, fmt);

	newstr =  vFormat(fmt, ap);

	va_end(ap);

	append(newstr);

	return *this;

    }

    HCSTR HCStr::s_tr_str = " \n\r\t";
    
   
    HINT HCStr::Split(HCSTRR str, HCSTRR key, HVSTRSR res ) {

	if (str.empty() || key.empty()) {
	    HRETURN(UN_INIT);
	}

	HSIZE beg = 0, pos = 0, len = key.length();
	HSTR strKey, strVal;
	while ( ( pos = str.find( key, beg ) ) != HSTR::npos ){
	    res.push_back( str.substr(beg, pos - beg ));
	    beg = pos + len;
	}

	if( pos != str.length() - 1 ){
	    res.push_back( str.substr(beg) );
	}

	HRETURN_OK;

    }    


    HINT HCStr::Ltrim( HSTRR str, HCPSZ sz ) {

	if (str.empty()) {
	    HRETURN(UN_INIT);
	}

	string not_find_str(s_tr_str);

	if (NULL != sz) {
	    not_find_str.append(sz);
	}

	//str = str.substr(str.find_first_not_of(not_find_str));
	size_type pos = str.find_first_not_of(not_find_str);
	if (pos == HSTR::npos) {

		str.clear();
	
	} else {
	
		str = str.substr(pos);

	}

	HRETURN_OK;

    }

    HINT HCStr::Rtrim( HSTRR str, HCPSZ sz ) {

	if (str.empty()) {
	    HRETURN(UN_INIT);
	}

	string not_find_str(s_tr_str);

	if (NULL != sz) {
	    not_find_str.append(sz);
	}

	str = str.substr(0, str.find_last_not_of(not_find_str) + 1);

	HRETURN_OK;

    }

    HINT HCStr::Trim( HSTRR str, HCPSZ sz ) {
	    
	Ltrim(str, sz);
	Rtrim(str, sz);

	HRETURN_OK;

    }


    HINT HCStr::Upper (HSTRR str) {

	std::transform(str.begin(), str.end(), str.begin(), ::toupper);

	HRETURN_OK;

    }

    HINT HCStr::Lower(HSTRR str) {

	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	HRETURN_OK;

    }

	
    HBOOL HCStr::IsDigit(HCSTRR str) {

	return str.find_first_not_of("1234567890") == HSTR::npos ?
	    HTRUE : HFALSE;

		
    }

    HBOOL HCStr::IsDigitAndDos (HCSTRR str) {

	return str.find_first_not_of("1234567890.") == HSTR::npos ?
	    HTRUE : HFALSE;

    }


    HBOOL HCStr::IsDigitChar (HCSTRR str) {

        HCSTR cstr = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        return str.find_first_not_of(cstr) == HSTR::npos ? HTRUE : HFALSE;

    }



    HINT HCStr::Right(HSTRR str, HSIZE len) {

	HSIZE size = str.length();
	if (len >= size) {

	    // return, do nothins.
	    HRETURN_OK; 	
		
	} else {

	    str = str.substr(size - len);

	}

	HRETURN_OK;
    }

    HINT HCStr::Left(HSTRR str, HSIZE len) {

	str = str.substr(0, len);
	    
	HRETURN_OK;
	    
    }

    HINT HCStr::Replace (HSTRR src, HCSTRR srcKey, HCSTRR dstKey) {

	if (src.empty() || srcKey.empty()) {

	    // do nothing and return OK.
	    HRETURN(UN_INIT);
	    
	}

	// get the position of the first match
	HSIZE pos = src.find(srcKey);

	if (pos == HSTR::npos) {

	    // cann't find, we do nothing too.
	    HRETURN(POS_END);

	}

	HSIZE len = srcKey.length();

	src.replace(pos, len, dstKey);

	HRETURN_OK;

    }


    HINT HCStr::Replaces (HSTRR src, HCSTRR srcKey, HCSTRR dstkey) {

	HINT cb = HERR_NO(OK);

	while (HIS_OK((cb = Replace(src, srcKey, dstkey)))) ;

	HRETURN_OK;
    }


	HINT HCStr::CountTime (HCSTRR src, HCSTRR key) {

		HINT res = 0;
		HSIZE pos = 0;

		while ((pos = src.find(key, pos)) != HSTR::npos and pos < src.length()) {

			++ res;
			++ pos;

		}

		return res;

	}


    HINT HCStr::Remove(HSTRR src, HCSTRR strKey) {

	if (src.empty() || strKey.empty()) {

	    HRETURN(UN_INIT);

	}

	HSIZE pos = src.find(strKey);
	if (pos == HSTR::npos) {
	    HRETURN(POS_END);
	}

	HSIZE len = strKey.length();

	src.erase(pos, len);

	HRETURN_OK;
    }


    HINT HCStr::Removes(HSTRR src, HCSTRR strKey) {

	HINT cb = HERR_NO(OK);

	while(HIS_OK((cb = Remove(src, strKey)))) ;
	
	HRETURN_OK;
    }


	HSTR HCStr::V2Str (HCVSTRSR strs, HSTR l) {
		
		HSTR res;

		for (size_t i = 0; i < strs.size(); ++i) {

			res.append(strs[i]);
			res.append(l);

		}

		if (not l.empty()) {
			res = res.substr(0, res.length()-1);
		}

		return res;
	}
    

    HCStr HCStr::Format(const value_type* fmt, ...) {

	HCStr res;

	va_list ap;

	va_start(ap, fmt);

	res = vFormat(fmt, ap);

	va_end(ap);

	return res;

    }

    HCStr HCStr::vFormat(const value_type* fmt, va_list ap) {

	HCStr res;

	int cb = 0;
#ifdef _GUN_SOURCE
	char* buf = NULL;
	cb = vasprintf(&buf, fmt, ap);

	if ( cb >= 0 && buf != NULL) {
	    // vasprintf success!
	    
	    res = buf;
	    free(buf);

	}

#else

	HCHAR buf[HLEN128_C] = {0};

	cb = vsprintf(buf, fmt, ap);

	if (cb >= 0) {

	    // vsprintf success
	    res = buf;

	}

#endif

	return res;

    }


	HSTR HCStr::WebStr (HCSTRR str, HUINT n) {
		return WebStr(stod(str), n);
	}


	HSTR HCStr::WebStr (HDOUBLE d, HUINT n) {
		HCHAR buf[HLEN2_C] = {0};
		HCHAR forbuf[HLEN_C] = {0};
		sprintf(forbuf, "%%.0%df", n);
		sprintf(buf, forbuf, d);
		return buf;
	}


    HLONG HCStr::stol (HCSTRR str) {
        return std::stol(str);
    }


    HDOUBLE HCStr::stod(HCSTRR str) {

	return std::stod(str);

    }

    HN64 HCStr::ston64(HCSTRR str) {

	return std::stoll(str);

    }

    HLLINT HCStr::stoll(HCSTRR str) {

        return std::stoll(str);

    }

    HSTR HCStr::lltos (HLLINT ll) {

        return to_string(ll);

    }

    HSTR HCStr::itos (HINT i) {

	return std::to_string(i);

    }

    HSTR HCStr::dtos(HDOUBLE d) {

	return std::to_string(d);
	
    }

}












