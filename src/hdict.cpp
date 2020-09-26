


#include "hdict.h"
#include "hstr.h"

#include <sstream>

namespace HUIBASE{

    HRET HCParam::SetParam (HCSTRR str, HCSTRR key, HCSTRR sonKey) {

        HSTR _s = str;
        HCStr::Trim(_s);

        HVSTRS strs;
        HNOTOK_RETURN(HCStr::Split(str, key, strs));

        for (size_t i = 0; i < strs.size(); ++i) {

            HCSTRR si = strs[i];

            HVSTRS ssi;

            HIF_NOTOK(HCStr::Split(si, sonKey, ssi)) {

                continue;

            }

            if (ssi.size() != 2) {
                continue;
            }

            HCSTRR sk = ssi[0];
            HCSTRR sv = ssi[1];

            setValue(sk, sv);

        }

        HRETURN_OK;

    }


    HSTR HCParam::ParamToString() const {

        std::stringstream ss;

        for (const_iterator it = begin(); it != end(); ++it) {

            ss << it->first << "=" << it->second << "&";

        }

        HSTR res = ss.str();

        res = res.substr(0, res.length() - 1);

        return res;

    }



    HBOOL HCParam::isDigit(HCSTRR strKey) const {

	if (strKey.empty()) return HFALSE;

	const_iterator cit = find(strKey);

	if (cit == end()) return HFALSE;

	HCStr ss = cit->second;

	HRET_BOOL(!ss.empty() && HIS_TRUE( ss.isDigit()));
    
    }

    HCPSZ HCParam::getSz(HCSTRR strKey) const {

	if (strKey.empty()) return HNULLSTR;

	const_iterator cit = find(strKey);

	if (cit == end()) return HNULLSTR;

	return cit->second.c_str();

    }

    HSTR HCParam::getStr(HCSTRR strKey, HCSTRR def_val) const {

	HSTR res = ((def_val == HNULLSTR) ?  HNULLSTR : def_val);

	if (strKey.empty()) return res;

	const_iterator cit = find(strKey);

	if (cit == end()) return res;

	return cit->second;

    }

#define RET_INT_IMP if (strKey.empty()) return def_val;\
	const_iterator cit = find(strKey); \
	if (cit == end()) return def_val; \
	return atoi(cit->second.c_str()); 

    HINT HCParam::getInt(HCSTRR strKey, HINT def_val) const {

	RET_INT_IMP

    }

    HSINT HCParam::getShort(HCSTRR strKey, HSINT def_val)const {

	RET_INT_IMP

    }

#undef RET_INT_IMP    

    HLLINT HCParam::getLlint (HCSTRR strKey, HLLINT def_val) const {

	if (strKey.empty()) return def_val;
	
	const_iterator cit = find(strKey);
	
	if (cit == end()) return def_val;
	
	return atoll(cit->second.c_str()); 

    }

    HCParam& HCParam::merge(const HCParam &ps) {

	for ( const_iterator cit = ps.begin();
	      cit != ps.end(); cit++ ) {

	    insert( value_type(cit->first, cit->second) );

	}

	return *this;

    }

    HCParam& HCParam::operator+ (const HCParam& ps) {

	return merge(ps);

    }

    HCParam& HCParam::operator+= (const HCParam& ps) {

	return merge(ps);

    }

    void HCParam::setValue (HCSTRR key, HCSTRR val) {

	iterator it = find(key);

	if(it != end()) {

	    it->second = val;

	} else {

	    insert(value_type(key, val));

	}

    }


    HCSTRR HCParam::GetValue (HCSTRR key) const  {
        
        const_iterator cit = find(key);
        
        HASSERT_THROW(cit != cend(), POS_END);

        return cit->second;

    }


    void HCParam::setValue(HCSTRR key, HLLINT llval) {
	
	char buf[64] = {0};

	sprintf(buf, "%lld", llval);

	setValue(key, buf);

    }
    

    void HCParam::setValueFormat(HCSTRR key, HCPSZ fmt, ...) {

	va_list ap;

	va_start(ap, fmt);

	setValue(key, HCStr::vFormat(fmt, ap));

	va_end(ap);

    }


    HSTR HCParam::ToJson (const HCParam& ps) {
        std::stringstream ss;
        ss << "[ ";

        for (const_iterator cit = ps.cbegin(); cit != ps.cend(); ++cit) {
            ss << "{\"name\":\"" << cit->first << "\", \"value\":\"" << cit->second << "\"},";
        }

        HSTR res = ss.str();
        res = res.substr(0, res.length() - 1);
        res += "]";

        return res;
    }


}
