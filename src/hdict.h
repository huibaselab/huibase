/**************************=cpphd=******************************************
 * 
 * @ File Name: hidict.h
 * 
 * @ Create Author: Tom Hui
 * 
 * @ Create Date: Wed Dec 28 15:12 2016
 * 
 * @ Brief:
 * 	defines common interface for dictionary classes.
 * 
 ****************************************************************************/



#ifndef __HIDICT_H__
#define __HIDICT_H__

#include "huibase.h"
#include "hstr.h"

namespace HUIBASE{

class HIDict{
public:
    virtual ~HIDict() {}

    virtual HCSTRR at(HCSTRR key) const = 0;

    virtual HSTRR at(HCSTRR key) = 0;

    virtual HBOOL hasKey(HCSTRR key) const = 0;

    virtual void set(HCSTRR key, HCSTRR val) = 0;

};

class HCParam : public HPS {
public:
    typedef HPS base;
    typedef base::iterator iterator;
    typedef base::const_iterator const_iterator;

    HRET SetParam (HCSTRR str, HCSTRR key, HCSTRR sonKey);

    HSTR ParamToString () const;

    HBOOL isDigit (HCSTRR strKey) const;

    HCPSZ getSz (HCSTRR strKey) const;
    
    HSTR getStr (HCSTRR strKey, HCSTRR def_val = HNULLSTR) const;

#define INLINE_RET_INT_IMP return atoi(getSz(strKey));

    HINT getInt (HCSTRR strKey) const {

	INLINE_RET_INT_IMP

    }

    HINT getInt (HCSTRR strKey, HINT def_val) const;

    HSINT getShort (HCSTRR strKey) {

	INLINE_RET_INT_IMP

    }

#undef INLINE_RET_INT_IMP    

    HSINT getShort (HCSTRR strKey, HSINT def_val) const;

    HLLINT getLlint (HCSTRR strKey) const {

	return atoll(getSz(strKey));

    }

    HLLINT getLlint (HCSTRR strKey, HLLINT def_val) const;

    HCParam& merge (const HCParam& ps);

    HCParam& operator+ (const HCParam& ps);

    HCParam& operator+= (const HCParam& ps);

    void setValue (HCSTRR key, HCSTRR val);

    void setValue (HCSTRR key, HLLINT llval);

    HCSTRR GetValue (HCSTRR key) const ;

    void setValueFormat (HCSTRR key, HCPSZ fmt, ...);

    static HBOOL HasKey (HCPSR ps, HCSTRR key) {

	HRET_BOOL(ps.find(key) != ps.end());

    }

    HBOOL hasKey (HCSTRR key) const {

	return HasKey (*this, key);

    }

    static HSTR ToJson (const HCParam& ps);

    HSTR ToJson () const { return ToJson(*this); }
};
    
}

#endif //__HIDICT_H__
