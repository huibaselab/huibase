

#include "huibase.h" 


namespace HUIBASE {

    // hbase's global error variable.
    HINT g_hErr = HERR_NO(OK);


#define BASE_ERR_STR_GEN(n,s) s,

    static HCPSZ s_base_err_map[] = {

	HBASE_ERR_MAP(BASE_ERR_STR_GEN)

    };

#undef BASE_ERR_STR_GEN


    HCPSZ hbase_err (int err) {

	if (err < 0 || err >=  HERR_NO(ERR_END)){

	    g_hErr = HERR_NO(UNKOWN_ERR);
	    return s_base_err_map[g_hErr];
	    
	}

	g_hErr = err;

	assert(g_hErr >= 0 && g_hErr < HERR_NO(ERR_END));

	return s_base_err_map[g_hErr];

    }

	HINT hlast_err ()  {

		return g_hErr;
	
	}

}









