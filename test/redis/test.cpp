

#include <huibase.h>
#include <hredis.h>

using namespace HUIBASE;
using namespace HUIBASE::NOSQL;

int main() {

    NoSqlConnectionInfo info;
    info.strIp = "127.0.0.1";
    info.nPort = 6379;
    info.strName = "test";

    CRedis rds(info);
    rds.Init();

    rds.Set("test", "number_oneaaaa");
    HSTR val;
    rds.Get("test", val);

    cout << val << endl;

    rds.Set("test", "AABBCCaabbcc");

    rds.Get("test", val);

    cout << val << endl;

	return 0;
}

