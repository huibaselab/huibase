


#include "hconf.h"
#include "hstr.h"

namespace HUIBASE {

    static const HINT MAX_CONF_LENGTH = 10240;

void HCMapConf::Merge(const HCMapConf& conf  ) {

	for (HPS::const_iterator cit = conf.m_ps.begin(); 
		cit != conf.m_ps.end(); ++cit ) {

		m_ps.insert(HPS::value_type(cit->first, cit->second));

	}

}

    HRET HCMapConf::LoadConfFile(HCSTRR strFileName) {

	if (strFileName.empty() ) {
		HRETURN(UN_INIT);
	}

	HRET ret = HERR_NO(OK);

	HINT cb = 0;

	FILE * pf = NULL;

	do {

	    pf = fopen(strFileName.c_str(), "rb");

	    if (NULL == pf) {

			HRETURN(SYS_FAILED);

	    }

	    HCHAR buf[MAX_CONF_LENGTH] = {0};

	    HSIZE rcb = fread(buf, 1, MAX_CONF_LENGTH, pf);
	    if (rcb == 0) {

		// read nothing, and then we do things.
		HRETURN_OK;
		
	    }
	    
	    HSTR str_src(buf);
	    HVSTRS lines;
	    HCSTR key ("\n");

	    cb = HCStr::Split(str_src, key,  lines);
	    if (!HIS_OK(cb)) {

		ret = cb;
		break;
		
	    }

	    HCSTR skey("=");

	    for (HSIZE i = 0; i < lines.size(); ++i) {

		HCSTRR l = lines.at(i);

		if(noteLine(l)) {
			continue;
		}

		HVSTRS vs;

		cb = HCStr::Split(l, skey, vs);

		if (!HIS_OK(cb))
		    continue;

		if (vs.size() < 2)
		    continue;

		HSTRR ikey = vs.at(0);
		HCStr::Trim(ikey);
		HSTRR ival = vs.at(1);
		HCStr::Trim(ival);

		m_ps[ikey] = ival;

	    }

	    

	} while(0);

	fclose(pf);
	
	return ret;

    }


    HCSTRR HCMapConf::GetValue (HCSTRR key) const {

	HPS::const_iterator cit = m_ps.find(key);

	if (cit == m_ps.end()) {

	    throw HCException(HERR_NO(INDEX_OUT),HTRUE,HFALSE,__LINE__, __FILE__);

	}

	return cit->second;

    }

	HSTR HCMapConf::GetValue (HCSTRR key) {

	HPS::iterator cit = m_ps.find(key);

    HASSERT_THROW_MSG(cit != m_ps.end(), HSTR("ERROR get key: ") + key, INDEX_OUT);

	/*if (cit == m_ps.end()) {

	    throw HCException(HERR_NO(INDEX_OUT),HTRUE,HFALSE,__LINE__, __FILE__);

        }*/

	return cit->second;

    }

	HSTR HCMapConf::GetValue (HCSTRR key, HCSTRR defVal) const {

		HPS::const_iterator cit = m_ps.find(key);

		if (cit == m_ps.end()) {
			return defVal;
		}

		return cit->second;

	}

	HINT HCMapConf::GetIntValue (HCSTRR key) const  {

		HPS::const_iterator cit = m_ps.find(key);

		if (cit == m_ps.end()) {

		    throw HCException(HERR_NO(INDEX_OUT),HTRUE,HFALSE,__LINE__, __FILE__);

		}

		return HCStr::stoi(cit->second);

	}

	HINT HCMapConf::GetIntValue (HCSTRR key, HINT def) const {

		HPS::const_iterator cit = m_ps.find(key);

		if (cit == m_ps.end()) {
			return def;
		}

		return HCStr::stoi(cit->second);
		
	}

    
    void HCMapConf::CopyOut(HPSR ps) {

		ps = m_ps;

    }


    HBOOL HCMapConf::noteLine (HCSTRR l) const {

	if (l.length() < 3) return HTRUE;
	
	if (l.substr(0,2) == ";;")  return HTRUE;

	return HFALSE;
	
    }


}

