/**************************=cpphd=******************************************
 *
 * @ File Name: hthread.h
 *
 * @ Create Author: Tom Hui
 *
 * @ Create Date: Thu Sep 21 16:41 2017
 *
 * @ Brief:
 * 	define the thread component.
 *
 ****************************************************************************/


#ifndef __HTHREAD_H__
#define __HTHREAD_H__

#include "huibase.h"
#include "hthreadid_t.h"
#include <pthread.h>
#include <htime.h>

namespace HUIBASE {

class HCThreadAttr {
public:
	// default construction
	HCThreadAttr();

	// copy construction
	HCThreadAttr (const HCThreadAttr& attr) = delete;

	~ HCThreadAttr();

	HRET SetDetachAttr ();

	const pthread_attr_t* GetAttrP ()const { return &m_attr; }

private:
	pthread_attr_t m_attr;
};

class HCThread {
public:
	using handle_type = pthread_t;

	using pid_t = cid<handle_type>;

	struct impl{
		std::function<void(void)> func;
		std::shared_ptr<struct impl> this_ptr;
	};

	HCThread () = default;

	HCThread (const HCThread& other) : m_id (other.m_id) {	}

	HCThread (HCThread&& other) noexcept : m_id(other.m_id) {
		other.m_id = pid_t{};
	}

	template<class Func, class... Args>
	explicit HCThread(Func&& ff, Args&&... args) {
	    auto imp = std::make_shared<impl>();

		imp->func = [=] { Scall(ff, args...); };
		imp->this_ptr = imp;

		HCThreadAttr attr;

		if (HNOT_OK(inCreate(imp.get(), attr))) {
			imp->this_ptr.reset();

		}

	}

	virtual ~ HCThread();

	template<class Func, class... Args>
	HRET Create(Func&& ff, Args&&... args) {
		auto imp = std::make_shared<impl>();
		imp->func = [=] { Scall(ff, args...); };
		imp->this_ptr = imp;

		HCThreadAttr attr;
		if (HNOT_OK(inCreate(imp.get(), attr))) {
			imp->this_ptr.reset();
			HRETURN(SYS_FAILED);
		}

		HRETURN_OK;
	}

	HBOOL IsRunning () const { HRET_BOOL( not (m_id == pid_t{}) ); }

    HRET Terminate () ;

    HRET Join (void** rep = nullptr) ;

    HRET JoinWithTime (const HCTimeSpan& timespan, void** rep = nullptr);

    pid_t GetId () const noexcept { return m_id; }

	handle_type* GetIdp () noexcept { return m_id.GetIdP();}

    handle_type GetHandle () const noexcept { return m_id.GetId(); }

    HSTR GetPidStr () const;

    static HSTR pid2s (handle_type handle);

private:
	HRET inCreate (impl* p, const HCThreadAttr& attr);

	template<class Func, class... Args>
	static void Scall (Func&& ff, Args&&... args) {
		ff(args...);
	}

private:
	pid_t m_id{};

};

}


#endif //__HTHREAD_H__

