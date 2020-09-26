

#include "hfname.h"
#include <sys/stat.h>

HBOOL HCFileNameBase::IsExists (const string_type& st) {

	struct stat buf;
	HRET_BOOL(stat(st.c_str(), &buf) == 0);

}


HRET HCFileNameBase::Remove (const string_type& st) {

	auto cb = remove(st.c_str());

	if (cb != 0) {
		HRETURN(SYS_FAILED);
	}


	HRETURN_OK;
}

#ifndef HWINDOWS
HRET HCFileNameBase::Symlink (const string_type& src, const string_type& dst) {

	auto cb = symlink(src.c_str(), dst.c_str());

	if (cb != 0) {
		HRETURN(SYS_FAILED);
	}


	HRETURN_OK;

}
#else 
HRET HCFileNameBase::Symlink (const string_type& src, const string_type& dst) {

	(void)src;
	(void)dst;

	HRETURN(SYS_SUPP);

}
#endif // HWINDOWS


HBOOL HCDic::GoToParent () {

	IF_TRUE(isInvalidDic()) {
		
		SetCurrnetDict();
		
	}

	IF_TRUE(IsRoot()) {

		return HFALSE;

	}

	// not root.
	auto pos = m_name.find_last_of(SEQ);

	// cannot be /XX/
	if (pos == m_name.length() - 1) {
		pos = m_name.find_last_of (SEQ, m_name.length() - 2);
	}
	
	if (pos == 0) {
		m_name = SEQ;
		return HTRUE;
	}	

	m_name = m_name.substr(0,pos);
	return HTRUE;

}



HCDic HCDic::GetParent () const {

	HCDic ret = *this;

	ret.GoToParent();

	return ret;
	
}

HBOOL HCDic::IsExactExists () const {

	return IsExactExists(*this);

}

HRET HCDic::CreateDic(mode_t mode) {

	return HCDic::CreateDic(GetName(), mode);

}


HRET HCDic::GoToDic(const HCDic& dic) {

	return GoToDic(dic.GetName());

}


HRET HCDic::GoToDic (const string_type& str) {

	auto cb = chdir (str.c_str());

	if (cb != 0) {

		HRETURN(SYS_FAILED);

	}

	HRETURN_OK;

}


HCDic HCDic::GetCurrentDict () {

	HPATH buf = {0};

	HPSZ cb = getcwd(buf, HPATH_LEN);

	HASSERT_THROW (cb != nullptr, SYS_FAILED);

	return HCDic(buf);

}


HBOOL HCDic::IsExactExists (const HCDic& dic) {

	struct stat buf{};

	auto cb = stat (dic.GetName().c_str(), &buf);
	if (cb != 0) {
		return HFALSE;
	}	

	HRET_BOOL(S_ISDIR(buf.st_mode));

}


HRET HCDic::CreateDic (string_type dicName, mode_t mode) {

	auto cb = mkdir(dicName.c_str(), mode);
	if (cb != 0) {
		if (errno != EEXIST) {
			HRETURN(SYS_FAILED);
		}
	}

	HRETURN_OK;
}


HBOOL HCDic::beginWithRoot () const {


	IF_TRUE(isInvalidDic()) {
		return HFALSE;
	}


	HRET_BOOL(m_name[0] == C_SEQ);

}

HBOOL HCDic::isInvalidDic () const {

	if (m_name.empty() || m_name == DOT || m_name == DOTDOT) { return HTRUE; }

	return HFALSE;

}

HBOOL HCFileName ::IsPathName () const {

	auto pos = m_name.find(SEQ);
	HRET_BOOL(pos != string_type::npos);

}


HCDic HCFileName ::GetParent () const {

	HCDic dic(m_name);

	dic.GoToParent();

	return dic;

}


HCFileName::string_type HCFileName::GetExtenName() const {

    string_type str = GetName();

    if (str.empty()) {

	return str;
	
    }
    
    string_type::size_type pos = str.rfind(DOT);

    if (pos == str.length() - 1) {
	return "";
    }

    return str.substr(pos + 1);
    
}


HCFileName::string_type HCFileName::JustFileName () const {
    size_t pos = m_name.rfind(SEQ);
    
    // cann't be /xxx/xxx/
    if (pos == m_name.length() - 1) {
        return "";
    }
    
    // if there are not "/" in name string, we set all string as name.
    if (pos == string_type::npos) {
        pos = 0;
    }
    
    // get the real name without path.
    string_type sf = m_name.substr(pos + 1);
    
    // make true file name is not "." or ".."
    if (sf == DOT || sf == DOTDOT) {
        return "";
    }
    
    // make true file name is not ".XXX", we set file name to "xxx" from ".xxx";
    if (sf[0] == C_DOT) {
        sf = sf.substr(1);
    }
    
    pos = sf.find(DOT);
    
    // if file name have no ".", return filename.
    if (pos == string_type::npos) {
        return sf;
    }
    
    return sf.substr(0, pos);
}

HBOOL HCFileName::IsExactExists (const HCFileName& fn) {

	struct stat buf{};
	
	auto cb = stat (fn.GetName().c_str(), &buf);
	if (cb != 0) {
		return HFALSE;
	}	
	
	HRET_BOOL(S_ISREG(buf.st_mode));

}

