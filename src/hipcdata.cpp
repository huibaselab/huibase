
#include "hipcdata.h"

namespace HUIBASE {
    
inline void SET_POS_LEN(HPSZ p, HINT len) {
 
    *((HINT*)p) = len;
    
}

HCIpcData::HCIpcData () noexcept 
	: m_ptr(nullptr), m_pend(nullptr), m_pos(nullptr){	

}


HCIpcData::HCIpcData (HUINT len) 
	: m_ptr(nullptr), m_pend(nullptr), m_pos (nullptr){

	m_ptr = (HPSZ)malloc(len);
	CHECK_NEWPOINT(m_ptr);

	m_pend = m_ptr + len;
	m_pos = m_ptr;

}


HCIpcData::HCIpcData (HUINT len, const_value_type data) 
	: m_ptr(nullptr), m_pend(nullptr){

	m_ptr = (HPSZ)malloc(len);
	CHECK_NEWPOINT(m_ptr);
	
	memcpy(m_ptr, data, len);
	m_pos = m_ptr + len;
	m_pend = m_pos;
}


HCIpcData::HCIpcData (const HCIpcData& data) 
	: m_ptr(nullptr), m_pend(nullptr){

	m_ptr = (HPSZ)malloc(data.GetLen());
	CHECK_NEWPOINT(m_ptr);

	memcpy(m_ptr, data.m_ptr, data.GetLen());
	m_pend = m_ptr + data.GetLen();
	m_pos = m_ptr + data.GetDataLen();
}


HCIpcData::HCIpcData (HCIpcData&& data) 
	: m_ptr(data.m_ptr), m_pend(data.m_pend), m_pos (data.m_pos){

	data.m_ptr = nullptr;
	data.m_pend = nullptr;
	data.m_pos = nullptr;

}

HCIpcData::~HCIpcData () {

	HFREE(m_ptr);
	m_pend = nullptr;
	m_pos = nullptr;

}


HCIpcData& HCIpcData::operator= (const HCIpcData& data) {

	HFREE(m_ptr);

	m_ptr = (HPSZ)malloc(data.GetLen());
	CHECK_NEWPOINT(m_ptr);

	m_pend = m_ptr + data.GetLen();
	
	memcpy(m_ptr, data.m_ptr, data.GetLen());

	m_pos = m_ptr + data.GetDataLen ();

	return *this;
}

HCIpcData& HCIpcData::operator= (HCIpcData&& data) {

	HFREE(m_ptr);

	m_ptr = data.m_ptr;
	m_pend = data.m_pend;
	m_pos = data.m_pos;

	data.m_ptr = nullptr;
	data.m_pend = nullptr;
	data.m_pos = nullptr;

	return *this;

}

void HCIpcData::MovePos (HUINT n) {

	HASSERT_THROW(n <= GetLen() - GetDataLen(), INVL_PARA);

	m_pos += n;

}


HSTR HCIpcData::GetString () const {

    HSTR str;
    str.insert (str.end(), m_ptr, m_pos);
    return str;
    
}


HRET HCIpcData::ParseDatas(HCPSZ buf, HUINT len, std::vector<HCIpcData>& datas) {

	HCPSZ pos = buf, pend = buf + len;
	HUINT _l = 0;

	while (pos < pend) {

		_l = STR_LEN(pos);
		pos += sizeof(HUINT);

		datas.push_back(std::move(HCIpcData(_l, pos)));

		pos += _l;

	}

	HRETURN_OK;

}

HPSZ HCIpcData::MakeBuf (const std::vector<HSTR>& vs, HUINT& len) {

    if (vs.empty()) {
    
        return nullptr;
    
    }
    
    HPSZ ret = nullptr;
 
    len = 0;
    for (size_t i = 0; i < vs.size(); ++i) {
        
        HCSTRR str = vs[i];
        len += sizeof(int) + str.length();
        
        
    }
    
    ret = (HPSZ)malloc(len);
    if (ret == nullptr) {
        return nullptr;
    }
    
    memset(ret, 0, len);
    
    HPSZ pos = ret;
    
    for (size_t i = 0; i < vs.size(); ++i) {
        
        HCSTRR str = vs[i];
        SET_POS_LEN(pos, str.length());
        pos += sizeof(HINT);
        
        memcpy(pos, str.c_str(), str.length());
        
        pos += str.length();
        
    }
    
    return ret;
}

}



