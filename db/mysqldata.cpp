

#include "mysqldata.h"



namespace HUIBASE {

    namespace HMYSQL {

        CRes::CRes() {

        }

        void CRes::SetEmpty() {
            m_header.clear();
            m_table.clear();
        }


        HCSTRR CRes::GetValue(HCSTRR strKey, HUINT _row) {
            
            HASSERT_THROW_MSG(_row < m_table.size(), "CRes::GetValue row number overflow", INVL_PARA);

            ROWHEADER::const_iterator fit = std::find(m_header.cbegin(), m_header.cend(), strKey);
            HASSERT_THROW_MSG(fit != m_header.cend(), "CRes::GetValue key column is not found", INVL_PARA);

            ROWHEADER::difference_type j = std::distance(m_header.cbegin(), fit);

            const ROW& _r = m_table[_row];
            return _r[j];
        }


        HCSTRR CRes::GetValue (HUINT i, HUINT j) {

            HASSERT_THROW_MSG(i < m_table.size(), "CRes::GetValue row number overflow", INVL_PARA);

            HASSERT_THROW_MSG(j < m_header.size(), "CRes::GetValue column number overflow", INVL_PARA);

            return m_table[i][j];

        }


        HUINT CRes::GetCount() const {

            return m_table.size();

        }


    }

}
