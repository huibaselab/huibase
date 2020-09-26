/**************************=cpphd=******************************************
 * 
 * @ File Name: hconf.h
 * 
 * @ Create Author: Tom Hui
 * 
 * @ Create Date: Tue Dec 27 17:30 2016
 * 
 * @ Brief:
 * 	defines all config classes for hbase.
 * 
 ****************************************************************************/

#ifndef __HCONF_H__
#define __HCONF_H__

#include "huibase.h"
#include "hstr.h"


namespace HUIBASE{
    

    class HIConf {
    public:
		
	virtual ~HIConf() {} 
	
	virtual HRET LoadConfFile (HCSTRR strFileName) = 0;

	virtual HCSTRR GetValue (HCSTRR key) const = 0;

	virtual HSTR GetValue (HCSTRR key) = 0;

	virtual HSTR GetValue (HCSTRR key, HCSTRR defVal) const  = 0;

	virtual void SetValue (HCSTRR strKey, HCSTRR value) = 0;

	virtual HBOOL IsHere(HCSTRR key) const = 0;

    };


    class HCMapConf : public HIConf {
    public:
	HCMapConf() {}

	virtual ~HCMapConf() {}

	HCMapConf(HCSTRR strFileName) {

	    LoadConfFile(strFileName);

	}

	virtual HCSTRR GetValue (HCSTRR key) const ;
	
	virtual HSTR GetValue (HCSTRR key) ;

	virtual HSTR GetValue (HCSTRR key, HCSTRR defVal) const;

	HINT GetIntValue (HCSTRR key) const;

	HINT GetIntValue (HCSTRR key, HINT def) const;

	virtual void SetValue (HCSTRR strKey, HCSTRR value) {
		m_ps[strKey] = value;
	}

	void Merge(const HCMapConf& conf  );

	HRET LoadConfFile (HCSTRR strFileName);

	HBOOL IsHere(HCSTRR key) const {
		HRET_BOOL(m_ps.count(key) != 0);
	}

	void CopyOut (HPSR ps) ;

	HCPSR GetData () const { return m_ps;}


    protected:
	
	virtual HBOOL noteLine (HCSTRR l) const ;
	
    private:
	HPS m_ps;
	
    };

}    


#endif // __HCONF_H__









