
#ifndef __IPCDATA_H__
#define __IPCDATA_H__

#include "huibase.h"



namespace HUIBASE {

inline HUINT STR_LEN(HCPSZ sz) { return *((HUINT*)sz); }
    
inline void SET_POS_LEN(HPSZ, HINT len);

class HCIpcData {
public:
	static constexpr HUINT IPC_MAX_LEN = HLEN80960_C;

	typedef HPSZ value_type;
	typedef HCPSZ const_value_type;
	
public:
	HCIpcData () noexcept;

	explicit HCIpcData (HUINT);

	HCIpcData (HUINT len, const_value_type data) ;

	HCIpcData (const HCIpcData& data) ;

	HCIpcData (HCIpcData&& data);

	~HCIpcData ();

	HCIpcData& operator= (const HCIpcData& data);

	HCIpcData& operator= (HCIpcData&& data);

	HUINT GetLen () const { if (m_ptr == nullptr) return 0; else  return m_pend - m_ptr; }

	HUINT GetDataLen () const { if (m_ptr == nullptr) return 0; else return m_pos - m_ptr; }

	HBOOL IsFull () const { HRET_BOOL(m_pos == m_pend); }

	HBOOL Empty () const { HRET_BOOL(m_ptr == nullptr || m_pos == m_ptr); }

	void MovePos (HUINT n);

	value_type Begin() { return m_ptr; }

	const_value_type CBegin () const { return m_ptr;}

	value_type End () { return m_pend; }

	const_value_type CEnd () { return m_pend; }

	HSTR GetString () const ;

	static HRET ParseDatas(HCPSZ buf, HUINT len, std::vector<HCIpcData>& datas);
    
	static HPSZ MakeBuf (const std::vector<HSTR>& vs, HUINT& len);


private:
	HPSZ m_ptr {nullptr};
	HPSZ m_pend{nullptr};
	HPSZ m_pos {nullptr};
	
};

}

#endif //__IPCDATA_H__


