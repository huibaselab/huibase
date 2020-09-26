


#include "hconnection.h"

namespace HUIBASE {


    namespace HMYSQL {

        HRET CMyConnection::Query(HCSTRR sql, CRes& res) throw () {

            HASSERT_THROW_MSG(HIS_TRUE(IsInited()), "mysql db is not initialized", UN_INIT);

            HASSERT_THROW_MSG(HIS_TRUE(IsConnected()), "mysql db is not connected", UN_INIT);

            HFAILED_MSG(Execute(sql), "query mysql failed");

            HFAILED_MSG(StoreResult(res), "store query result failed");

            HRETURN_OK;
        }


        HRET CMyConnection::Exec(HCSTRR sql) throw () {

            HASSERT_THROW_MSG(HIS_TRUE(IsInited()), "mysql db is not initialized", UN_INIT);

            HASSERT_THROW_MSG(HIS_TRUE(IsConnected()), "mysql db is not connected", UN_INIT);

            HFAILED_MSG(Execute(sql), "query mysql failed");

            HRETURN_OK;
        }


        HRET CMyConnection::BeginTransaction () throw () {

            return Exec("start transaction");

        }

        HRET CMyConnection::CommitTransaction () throw () {

            return Exec("commit");

        }


        HRET CMyConnection::CMyConnection::RollBack () throw () {
            
            return Exec("rollback");

        }


        HRET CMyConnection::MultiExec (HCVSTRSR sqlss) throw () {

            HNOTOK_RETURN(BeginTransaction());

            for (size_t i = 0; i < sqlss.size(); ++i) {

                HCSTRR str = sqlss[i];

                HIF_NOTOK(Exec(str)) {
                    
                    RollBack();

                    HRETURN(QRY_ERR);

                }

            }

            return CommitTransaction();

        }


        HRET CMyConnection::MultiInsert (HCSTRR strTable, const CRes& res) {

            HASSERT_RETURN(not strTable.empty() and HIS_TRUE(res.IsValid()), INVL_PARA);

            HSTR filds("( ");
            const ROWHEADER& _header = res.GetHeader();
            for (HSIZE i = 0; i < _header.size(); ++i) {
                const HCSTRR _name = _header[i];
                filds += _name + ",";
            }
            filds = filds.substr(0, filds.length() - 1);
            filds += ")";

            HSTR values(" ");
            const TABLE& tab = res.GetTable();
            for (HSIZE i = 0; i < tab.size(); ++i) {
                const ROW& _row = tab[i];

                HSTR _row_val ("( ");

                for (HSIZE j = 0; j < _row.size(); ++j) {

                    HCSTRR col = _row[j];
                    _row_val += col + ",";

                }

                _row_val = _row_val.substr(0, _row_val.length() - 1);
                _row_val += ")";
                values += _row_val + ",";
            }
            values = values.substr(0, values.length() - 1);

            HASSERT_RETURN(values.length() < SQL_MAX, INVL_PARA);

            HSTR sql = HCStr::Format("INSERT INTO `%s` %s %s", strTable.c_str(), filds.c_str(), values.c_str());

            return Exec(sql);

        }

    }

}
