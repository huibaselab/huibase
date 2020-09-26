

#ifndef __HHOLDER_H__
#define __HHOLDER_H__

#include "huibase.h"

namespace HUIBASE {

    class HCHolder {
    public:

	virtual ~HCHolder() {}

    };

    class HCHandlerHolder : public HCHolder {
    public:
	HCHandlerHolder (HSYS_T fd) : m_fd(fd) {}

	virtual ~HCHandlerHolder() {

	    ::close(m_fd);

	}
	
    private:
	
	HSYS_T m_fd;
    };

    template<typename T>
    class HCPointerHolder : public HCHolder {
    public:
	HCPointerHolder() {}
	
	HCPointerHolder(T* pt) : m_pt(pt) {}

	virtual ~HCPointerHolder() {

	    delete m_pt;
	    
	}

	void SetPoint(T* _t) { m_pt = _t; }

    private:

	T* m_pt;
   
    };

	


}


#endif //__HHOLDER_H__












