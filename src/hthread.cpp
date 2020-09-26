

#include "hthread.h"

#include "hutil.h"

namespace HUIBASE {

HCThreadAttr::HCThreadAttr() {
	pthread_attr_init(&m_attr);
}

HCThreadAttr::~HCThreadAttr() {
	pthread_attr_destroy(&m_attr);
}


HRET HCThreadAttr::SetDetachAttr () {

	auto cb = pthread_attr_setdetachstate(&m_attr, PTHREAD_CREATE_DETACHED);
	HASSERT_THROW(cb == 0, SYS_FAILED);

	HRETURN_OK;

}

HCThread::~HCThread() {
	(void)Terminate();
}

HRET HCThread::Terminate () {

	if (HIS_TRUE(IsRunning()) )  {
		auto cb = pthread_cancel(m_id.GetId());
		HASSERT_RETURN(cb == 0, SYS_FAILED);

 		return Join();
	}

	HRETURN(ERR_STATUS);
}

HRET HCThread::Join (void** rep) {

	auto cb = pthread_join(m_id.GetId(), rep);
 	HASSERT_RETURN(cb == 0, SYS_FAILED);

 	m_id = pid_t{};

	HRETURN_OK;
}

HRET HCThread::JoinWithTime (const HCTimeSpan& timespan, void** rep) {


// TODO: pthread_timedjoin_np cann't in the win.
#ifndef HWINDOWS
	struct timespec _tv = {0,0};
	_tv.tv_sec = timespan.getSeconds();
	_tv.tv_nsec = 0;
	
	auto cb = pthread_timedjoin_np(m_id.GetId(), rep, &_tv);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	m_id = pid_t{};

	HRETURN_OK;

#else 

	(void)timespan;
	auto cb = pthread_join(m_id.GetId(), rep);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	m_id = pid_t{};

	HRETURN_OK;

#endif 


}


HSTR HCThread::GetPidStr () const {
	return pid2s(GetHandle());
}


HSTR HCThread::pid2s (handle_type handle) {

	HNAME buf = {0};
	HCHAR* pos = buf;
	unsigned char* usz = (unsigned char*)(void*)(&handle);

	HSTR ret("0x");
	
	for (size_t i = 0; i < sizeof(handle); ++i) {
		sprintf(pos, "%02x", (unsigned)(usz[i]));
		pos += 2;
	}

	ret.append(buf);
	return ret;

}

HRET HCThread::inCreate (impl* p, const HCThreadAttr& attr) {

	auto cb = pthread_create(m_id.GetIdP(), attr.GetAttrP(), [] (void* arg_p) {
		impl* imp = static_cast<impl*>(arg_p);
		imp->func();
		imp->this_ptr.reset();
		return static_cast<void*>(nullptr);
	}, (void*)p);

	if (cb) {
		p->this_ptr.reset();
		HRETURN(SYS_FAILED);
	}

	HRETURN_OK;
}

}


