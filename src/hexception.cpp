


#include "hexception.h"
#include "hstr.h"

namespace HUIBASE {

    HCException::HCException(HINT err, HBOOL bComm, HBOOL bError,
			     HINT iLineNo, HCPSZ szFile)
	: m_err(err), m_bComm(bComm), m_bError(bError), m_iLineno(iLineNo), m_strFile(szFile) {

	m_str.clear();
	
	if (HIS_TRUE(m_bComm)) {
		m_str.append(" Base error information: {");
	    m_str.append(hbase_err(m_err));
		m_str.append("}");
	}

	if (HIS_TRUE(m_bError)) {
		m_str.append(" System error information: {");
	    m_str.append(strerror(errno));
		m_str.append("}");
	}

	if (m_str.empty()) {

	    m_str = "Unkunown exception!";

	}

		appendLineNum();

    }

    HCException::HCException(HCSTRR strErrMsg, HINT err, HBOOL bComm ,
			     HBOOL bError,  HINT iLine, HCPSZ szFile)
	: m_err(err), m_bComm(bComm), m_bError(bError), m_iLineno(iLine),
	  m_strFile(szFile), m_str(strErrMsg){

	if (m_str.empty()) {

	    m_str = "Unkunown exception!";

	}

	if (m_bComm) {
	    m_str.append(" Base error information: {");
	    m_str.append(hbase_err(m_err));
	    m_str.append("}");
	}

	if (m_bError) {

	    m_str.append(" System error information: {");
	    m_str.append(strerror(errno));
	    m_str.append("}");

	}

		appendLineNum();
	
    }

    HCException::~HCException() throw (){


    }

	void HCException::appendLineNum () {

		m_str.append(HCStr::Format("[%s:%d]", m_strFile.c_str(), m_iLineno));

	}

}


