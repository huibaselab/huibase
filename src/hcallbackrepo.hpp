

#ifndef __HCALLBACK_REPO_H__
#define __HCALLBACK_REPO_H__


#include "huibase.h"


namespace HUIBASE{

    template<typename T>
    class HCallbackRepo {
	public:
	typedef T target_type;
	typedef std::map<HSTR, target_type> repo_map;

	HCallbackRepo() {}

	~ HCallbackRepo () { }

	void SetCallback (HCSTRR name, T callback) {

		m_callbacks.insert(typename repo_map::value_type(name, callback));

	}

	T SetCallback (HCSTRR name) {
		
		if (name.empty()) {
			return nullptr;
		}
		
		typename repo_map::iterator fit = m_callbacks.find(name);
		if (fit == m_callbacks.end()) {
			return nullptr;
		}

		return fit->second;

	}

	private:
		repo_map m_callbacks;
    };

}


#endif // matched __HSINGLETON_H__











