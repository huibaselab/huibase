/**************************************************
*	Create Author: TomHui
*	Create Date:
*	Description: this file declare some http tools 
* 				 for making HTTP request and parsing 
*				 HTTP response.
**************************************************/




#ifndef _H_APP_H_
#define _H_APP_H_

#include "huibase.h"
#include "hconf.h"
#include "hfilelock.h"

namespace HUIBASE{

#define EXE_PATH "exe_path"
#define EXE_NAME "exe_name"

void stop_handle(int sig);

 void handle_bug (int sig);

class HCApp{
public:
	HCApp();
	HCApp(HINT argc, const HCHAR * argv[] );
	virtual ~ HCApp();

public:
	virtual HBOOL Run() = 0;
	virtual void Init();		
	
	virtual void Stop () { _running = false; }

    void Handle_sig (int sig);

	HBOOL IsRunning () const { HRET_BOOL(_running); }

	void SetConfValue (HCSTRR strKey, HCSTRR strVal) { m_conf.SetValue(strKey, strVal); }

	

protected:
	
	virtual void init() {}

private:
	void anaOpt(int argc, const HCHAR ** argv );

	virtual void initLog () ;
	

protected:
	HCSTR GetConfValue( HCSTRR strKey ){
		return m_conf.GetValue( strKey );
	}

	virtual void setDeamonApp();

	virtual HRET lockApp ();

    HRET releaseLock ();

protected:
	HCMapConf m_conf;
	HBOOL m_bDeamon;

private:
	volatile bool _running = true;
	CAppLock m_lock;
};

}

#endif 

