/**************************=cpphd=******************************************
 * 
 * @ File Name: hstr.h
 * 
 * @ Create Author: Tom Hui
 * 
 * @ Create Date: Fri Dec 23 14:17 2016
 * 
 * @ Brief:
 * 	This file declare all functions to process strings.
 * 
 ****************************************************************************/


#ifndef _H_STR_H_
#define _H_STR_H_

#include "huibase.h"

namespace HUIBASE{

/*************************=cldes=**********************************
 @ Class Name:  HCStr
 @ Create Author: Tom Hui
 @ Create Date: Tue Dec 20 10:19 2016
 @ Description:
	manage string function and extend string.
 ******************************************************************/
    class HCStr : public HSTR{
    public:
		typedef HSTR base;
		typedef base::value_type value_type;
		typedef base::size_type size_type;
		typedef base::traits_type traits_type;
		typedef base::allocator_type allocator_type;

    public:
	HCStr()
	{}

    HCStr(const HCStr & str) : base(str)
	{}

	HCStr(const base& str) : base(str)
	{}

	HCStr(const value_type * sz) : base(sz)
	{}

	HCStr(const value_type * c, size_type n, const allocator_type & al = allocator_type())
	    : base(c, n, al)
	{}

	HCStr(const base & b, size_type pos, size_type count = base::npos )
	    : base(b, pos, count)
	{}

	HCStr(size_type c, value_type v)
	    :base(c, v)
	{}
	
    public:
	HCStr& operator= (const value_type * vt) {

	    assign(vt);

	    return *this;
	    
	}

	HCStr& operator= (const HCStr& str) {

	    assign(str);

	    return *this;

	}

    public:
	HINT split(HCSTRR key, HVSTRSR res) const {

	     return Split(*this, key, res);

	}
	
	HCStr & upper() {

	    Upper(*this);

	    return *this;

	}

	HCStr& lower() {

	    Lower(*this);

	    return *this;

	}

	HCStr ltrim(HCPSZ sz = NULL) {

	    Ltrim(*this, sz);

	    return *this;

	}
	

	HCStr& rtrim (HCPSZ sz = NULL) {

	    Rtrim(*this, sz);

	    return *this;

	}

	
	HCStr& trim(HCPSZ sz = NULL) {

	    Trim(*this, sz);

	    return *this;

	}


	HBOOL isDigit() const {

	    return IsDigit(*this);

	}

	HCStr& right(HSIZE len) {

	    Right(*this, len);

	    return *this;

	}

	HCStr& left(HSIZE len) {

	    Left(*this, len);

	    return *this;

	}

	HCStr& replace(HCSTRR srckey, HCSTRR dstKey) {

	    Replace(*this, srckey, dstKey);

	    return *this;

	}

	HCStr& replaces(HCSTRR srcKey, HCSTRR dstKey) {

	    Replaces(*this, srcKey, dstKey);

	    return *this;

	}


	HINT CountTime (HCSTRR key) {

		return CountTime(*this, key);

	}

	
	HCStr& remove(HCSTRR srcKey) {

	    Remove(*this, srcKey);

	    return *this;

	}

	HCStr& removes(HCSTRR srcKey) {

	    Removes(*this, srcKey);

	    return *this;

	}

	HCStr& appendFormat(const value_type* fmt, ... );
	
    public:
	
	/*****************************=memfu=*************************************
	 @ Member Function Name: Split
	 @ Create Author: Tom Hui
	 @ Create Date: Tue Dec 20 11:09 2016
	 @ Parameters:
	       1> str: source string function will operate it;
               2> key: split key;
               3> res: return buffer;
	 @ Return Value:
   	      success 0, otherwide a herror will be signed
	*************************************************************************/ 
	static HINT Split(HCSTRR str, HCSTRR key, HVSTRSR res );

	static HCSTR s_tr_str;
	/*****************************=memfu=*************************************
	 @ Member Function Name: Ltrim
	 @ Create Author: Tom Hui
	 @ Create Date: Tue Dec 20 11:28 2016
	 @ Parameters: 
	      1> str: source string
              2> sz: 
	 @ Return Value:
   	      success 0, otherwide a herror will be signed.
	*************************************************************************/	
	static HINT Ltrim( HSTRR str, HCPSZ sz  = NULL ) ;


	/*****************************=memfu=*************************************
	 @ Member Function Name: Rtrim
	 @ Create Author: Tom Hui
	 @ Create Date: Tue Dec 20 11:42 2016
	 @ Parameters:
	     1> str: source string
	     2> sz: 
	 @ Return Value:
   	      success 0, otherwide a herror will be signed   	
	*************************************************************************/	
	static HINT Rtrim( HSTRR str, HCPSZ sz  = NULL  );

	static HINT Trim( HSTRR str, HCPSZ sz  = NULL  );

	static HINT Upper (HSTRR str);

	static HINT Lower(HSTRR str);

	static HBOOL IsDigit(HCSTRR str);

	static HBOOL IsDigitAndDos (HCSTRR str);

    static HBOOL IsDigitChar (HCSTRR str);

	static HINT Right(HSTRR str, HSIZE len);

	static HINT Left(HSTRR str, HSIZE len);

/*****************************=memfu=*************************************
 * @ Create Author: Tom Hui
 * @ Create Date: Tue Dec 27 10:53 2016
 * @ brief:
 *	replace a string speacified by srcKey in source string with dst string.
 * @ Param:
 *	1> src: source string;
 *	2> srckey: src key;
 *      3> dstKey: dst key;
 * @ Return Value:
 *   	success return OK.
 *************************************************************************/	
	static HINT Replace (HSTRR src, HCSTRR srcKey, HCSTRR dstKey);

/*****************************=memfu=*************************************
 * @ Create Author: Tom Hui
 * @ Create Date: Tue Dec 27 10:56 2016
 * @ brief:
 *	replace all strings in source string with dst string
 * @ Param:
 *	1> src: 
 *	2> srcKey:
 *      3> dstKey:
 * @ Return Value:
 *   	success return OK.
 *************************************************************************/	
	static HINT Replaces (HSTRR src, HCSTRR srcKey, HCSTRR dstkey);

	static HINT CountTime (HCSTRR src, HCSTRR key);

/*****************************=memfu=*************************************
 * @ Create Author: Tom Hui
 * @ Create Date: Tue Dec 27 14:21 2016
 * @ brief:
 *	move a key string specitied by strKey from src.
 * @ Param:
 *	1> src: source string
 *	2> strKey: key string
 * @ Return Value:
 *   	success return OK
 *************************************************************************/	
	static HINT Remove(HSTRR src, HCSTRR strKey);

	static HINT Removes(HSTRR src, HCSTRR strKey);

	static HSTR V2Str (HCVSTRSR strs, HSTR l = "|");

	static HCStr Format(const value_type* fmt, ...);

	static HCStr vFormat(const value_type* fmt, va_list ap);

	static HSTR WebStr (HCSTRR str, HUINT n);

	static HSTR WebStr (HDOUBLE d, HUINT n);

	static HINT stoi (HCSTRR str) {
	    return std::stoi(str);
	}
	
	static HINT stoi (HCPSZ sz) {
	    return std::stoi(HSTR(sz));
	}

    static HLONG stol (HCSTRR str);

	static HDOUBLE stod(HCSTRR str);

	static HN64 ston64(HCSTRR str);

    static HLLINT stoll(HCSTRR str);

    static HSTR lltos (HLLINT ll);

	static HSTR itos (HINT i);

	static HSTR dtos(HDOUBLE d);


    };

}

#endif


