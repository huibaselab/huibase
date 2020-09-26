


#ifndef __HDIC_H__
#define __HDIC_H__

#include <huibase.h>
#include <hstr.h>

using namespace HUIBASE;

class HCFileNameBase {
public:
	enum name_type {
		nt_base,
		nt_dic,
		nt_file,

		nt_end,
	};
public:
	typedef HCHAR value_type;
	typedef HCStr string_type;

	#define SEQ "/"

	static constexpr value_type C_SEQ = '/';

	#define DOT "."

	#define DOTDOT ".."

	static constexpr value_type C_DOT = '.';

	HCFileNameBase () noexcept {}

	HCFileNameBase (string_type str) : m_name(str) {}

	string_type GetName () const { return m_name; }

	void SetName (HCSTRR name) { m_name = name; }
	

	HBOOL Compare (const HCFileNameBase& dic) const { HRET_BOOL(m_name == dic.m_name); }

	HBOOL Empty () const { HRET_BOOL(m_name.empty()); }

	virtual name_type GetType() const { return nt_base; }	

	static HBOOL IsExists (const string_type& st);
	
	static HRET Remove (const string_type& st);

	static HRET Symlink (const string_type& src, const string_type& dst);
	

protected:
	string_type m_name;

};

class HCDic : public HCFileNameBase {
public:
	HCDic () noexcept{}

	HCDic (const HCDic& dic) : HCFileNameBase(dic.m_name) {}

	HCDic(const string_type& s) : HCFileNameBase(s) {}

	HCDic (HCDic&& dic) noexcept { m_name = std::move(dic.m_name);}

	HCDic& operator= (const HCDic& dic) {
		m_name = dic.m_name;
		return *this;
	}
	
	HCDic& operator= (const string_type& str) { m_name = str; return *this; }

	HCDic& operator= (HCDic&& dic)noexcept { m_name = std::move(dic.m_name);  return *this; }

	virtual name_type GetType() const { return nt_dic; } 

	HBOOL DicIsDot () const { HRET_BOOL(m_name == DOT); }

	HBOOL DicIsDotDot () const  { HRET_BOOL(m_name == DOTDOT); }

	HBOOL IsRoot () const { HRET_BOOL (m_name == SEQ); }

	void Append (string_type strName) { m_name =  m_name + SEQ + strName; }

	HBOOL GoToParent ();

	HCDic GetParent () const;	

	HBOOL IsExactExists () const;

	HBOOL IsAbsoluteDic () const { return beginWithRoot(); }

	HBOOL IsRelativeDic () const { HRET_BOOL(HIS_FALSE(beginWithRoot())); }

	void SetCurrnetDict () { *this = GetCurrentDict(); }

	HRET CreateDic(mode_t mode = 0755); 

	static HRET GoToDic(const HCDic& dic);

	static HRET GoToDic (const string_type& str);

	static HCDic GetCurrentDict ();

	static HBOOL IsExactExists (const HCDic& dic);

	static HRET CreateDic (string_type dicName, mode_t mode = 0755);

private:
	HBOOL beginWithRoot () const;

	HBOOL isInvalidDic () const;

};

class HCFileName : public HCFileNameBase {
public:
	HCFileName () noexcept{}
	
	HCFileName (const HCFileName& fn) : HCFileNameBase(fn.m_name) {}
	
	HCFileName(const string_type& s) : HCFileNameBase(s) {}
	
	HCFileName (HCFileName&& fn) noexcept { m_name = std::move(fn.m_name);}
	
	HCFileName& operator= (const HCFileName& fn) {
		m_name = fn.m_name;
		return *this;
	}
		
	HCFileName& operator= (const string_type& str) { m_name = str; return *this; }

	HCFileName& operator= (HCFileName&& fn)noexcept { m_name = std::move(fn.m_name);  return *this; }

	HBOOL IsPathName () const;

	HCDic GetParent () const;

	string_type GetExtenName () const;
    
    string_type JustFileName () const;
	
	virtual name_type GetType() const { return nt_file; } 

	static HBOOL IsExactExists (const HCFileName& fn);

	static HRET CopyFile (const string_type & src, const string_type& dst);

};


#endif 

