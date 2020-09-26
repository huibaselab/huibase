

#ifndef _H_TASK_H_
#define _H_TASK_H_

#include "huibase.h"

namespace HUIBASE{

class HCTask{
public:
	HCTask();
	virtual ~ HCTask();

public:
	virtual HRET ParseInput(HCPSR ps) ;

	virtual HRET DoWork() ;

	virtual HRET DoTurn() ;

protected:
	virtual HRET CommitInput() {HRETURN_OK;}

	virtual HRET Commit() = 0 ;

	virtual HRET CommitReturn() {HRETURN_OK;}

protected:
	HPS m_inps;
	
};


}
#endif 

