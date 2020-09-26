
#include "happ.h"
#include "hlog.h"
#include <getopt.h>
#include "hstr.h"
namespace HUIBASE{

static HCApp* gapp = nullptr;

void stop_handle(int sig) {

    SLOG_WS("get a stop sig");

	(void)sig;
	if (gapp) {
        SLOG_WS("stop main process");
		gapp->Stop();
	}

}


void handle_bug (int sig) {

    SLOG_WS("get a bug sig");

    if(gapp) {

        gapp->Handle_sig(sig);

    }

}


static const HCHAR * s_cmdopts = "a:b:c:d:e:f:g:h:i:j:k:l:m:n:o:p:ABCDEFGHIJKL";
static const struct  option s_opts[] = {
    { "aa", 1, NULL, 'a' },{ "bb", 1, NULL, 'b' },{ "config", 1, NULL, 'c' },{ "dd", 1, NULL, 'd' },
    { "ee", 1, NULL, 'e' },{ "ff", 1, NULL, 'f' },{ "gg", 1, NULL, 'g' },{ "hh", 1, NULL, 'h' },
    { "ii", 1, NULL, 'i' },{ "jj", 1, NULL, 'j' },{ "kk", 1, NULL, 'k' },{ "ll", 1, NULL, 'l' },
    { "mm", 1, NULL, 'm' },{ "nn", 1, NULL, 'n' },{ "oo", 1, NULL, 'o' },{ "proc", 1, NULL, 'p' },
    { "AA", 0, NULL, 'A' },{ "BB", 0, NULL, 'B' },{ "CC", 0, NULL, 'C' },{ "deamon", 0, NULL, 'D' },
    { "EE", 0, NULL, 'E' },{ "FF", 0, NULL, 'F' },{ "GG", 0, NULL, 'G' },{ "help", 0, NULL, 'H' },
    { "II", 0, NULL, 'I' },{ "JJ", 0, NULL, 'J' },{ "KK", 0, NULL, 'K' },{ "LL", 0, NULL, 'L' },
    { NULL, 0, NULL, 0 }
};

HCApp::HCApp()
    : m_bDeamon(HFALSE){

    m_conf.SetValue("conf", "../conf/conf");

}

HCApp::HCApp(int argc,const HCHAR* argv[])
    : m_bDeamon(HFALSE){
    anaOpt(argc, argv);
}


HCApp::~HCApp () {

}


    void HCApp::Handle_sig(int sig) {

        SLOG_ERROR("===========================signal [%d]======================", sig);

    }


void HCApp::anaOpt(HINT argc, const HCHAR * argv[] ){

    const HCHAR * cp = NULL, *program = *argv;
    m_conf.SetValue(EXE_PATH, program);

    cp = strrchr( program, '/');
    if( cp != NULL ){
        program = cp + 1;
    }
    m_conf.SetValue( EXE_NAME, program);

    int cb = 0;
    while ( ( cb = getopt_long( argc, const_cast<char**>(argv), s_cmdopts,s_opts, NULL ) ) != -1 ){
        switch ( cb ){
            case 'c':
                m_conf.SetValue("conf", optarg);
                break;
            case 'l':
                m_conf.SetValue("log", optarg);
                break;
            case 'p':
                m_conf.SetValue("proc_count", optarg);
                break;
            case 'D':
                m_bDeamon = HTRUE;
                break;
            default:               
                break;
        }
    }
}


void HCApp::initLog () {

    HSTR strLogFileName;
	if (HIS_TRUE(m_conf.IsHere("log"))) {
		strLogFileName = GetConfValue("log");	    
	} else {
		m_conf.SetValue("log", HSTR("../log/") + GetConfValue(EXE_NAME));
	}

	if( strLogFileName.empty() ){      
	    strLogFileName = GetConfValue("log");
	}
    
    InitLog( strLogFileName.c_str() , 0, 1 );

}


void HCApp::Init(){

	signal(SIGTERM, stop_handle);
	//signal(SIGINT, stop_handle);
    //signal(SIGSEGV, handle_bug);
	gapp = this;

	if(HIS_FALSE(m_conf.IsHere("conf")) ){
    //if( m_conf.GetValue("conf").empty() ){
        m_conf.SetValue("conf", "../conf/conf");
    }

	if (HNOT_OK(m_conf.LoadConfFile(m_conf.GetValue("conf")))) {
		printf("Load Config File Fialed. Process exit.");
		exit(-1);
	}

	initLog();

    SLOG_NORMAL("conf[%s] log[%s] exe[%s]", GetConfValue("conf").c_str(),
    	GetConfValue("log").c_str(), GetConfValue(EXE_NAME).c_str());

    // show input information
    HCPSR conf_data = m_conf.GetData();
    for (HPS::const_iterator cit = conf_data.begin(); cit != conf_data.end(); ++cit) {
    	SLOG_NORMAL("[%s]:[%s]", cit->first.c_str(), cit->second.c_str());
    }

    HSTR dea_str;
	if (HIS_TRUE(m_conf.IsHere("is_daemon"))) {
		dea_str = m_conf.GetValue("is_daemon");
	   	HCStr::Trim(dea_str);
	    SLOG_NORMAL("whether daemon instance [%s]", dea_str.c_str());
	    if( dea_str == "1" ){
	        SLOG_NS("set as daemon");
	        setDeamonApp();
	    }
	}

	// process lock
	HSTR applock_str;
	if (HIS_TRUE(m_conf.IsHere("app_lock"))) {
		applock_str = m_conf.GetValue("app_lock");
	   	HCStr::Trim(applock_str);
	    SLOG_NORMAL("whether lock instance [%s]", applock_str.c_str());
	    if( applock_str == "1" ){
	        (void)lockApp();
	    }
	}

    init();
}

void HCApp::setDeamonApp(){
    SLOG_NS("setas a deamon app");
        
    if( fork() > 0 ){	
        exit(0);    
    }    

    setsid();    

    if( fork() > 0 )
	
        exit(0);
    
    umask(0);

}

HRET HCApp::lockApp () {

    HSTR str = HSTR(".") + GetConfValue(EXE_NAME);
    
    HIF_NOTOK(m_lock.LockApp(str.c_str())) {
	SLOG_ES("lockapp failed. Admin is running.. Abort");
	
	exit(-1);
    }

    HRETURN_OK;
    
}


HRET HCApp::releaseLock() {

    m_lock.UnlockApp();

    HRETURN_OK;

}


}

