

#ifndef __HSINGLETON_H__
#define __HSINGLETON_H__


#include "huibase.h"

namespace HUIBASE{


    template<typename T>
    class HCSingleton {
    private:
	HCSingleton() {}

	struct create_obj {
	    create_obj () {

		HCSingleton<T>::Instance();

	    }

	    inline void do_nothing () const {}

	};

	static create_obj _create_obj;

    public:
	typedef T target_type;

	static target_type* Instance () {

	    static target_type target_obj;

	    _create_obj.do_nothing();

            return (&target_obj);

	}

    };

    template<typename T>
    typename HCSingleton<T>::create_obj HCSingleton<T>::_create_obj;

}


#endif // matched __HSINGLETON_H__











