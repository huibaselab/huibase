

#ifndef __HTHREADID_T_H__
#define __HTHREADID_T_H__


namespace HUIBASE {

template<class T>
class cid {
public:
	typedef T handle_type;

	typedef T* handle_type_ptr;

	cid () = default;

	explicit cid(handle_type handle) : m_handle(handle) {}

	const handle_type & GetId () const { return m_handle;}

    handle_type& GetId () { return m_handle; }

	handle_type_ptr GetIdP() { return &m_handle; }

	bool operator== (const cid& id) const noexcept {
		return m_handle == id.m_handle;
	}

	bool operator< (const cid& id) const noexcept {
		return m_handle < id.m_handle;
	}	

private:
	handle_type m_handle{};
};



}

#endif

