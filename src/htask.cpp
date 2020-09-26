


#include "htask.h"

namespace HUIBASE{


HCTask::HCTask(){


}


HCTask::~HCTask(){


}

HRET HCTask::ParseInput (HCPSR ps) {

	m_inps = ps;

	return CommitInput();

}


HRET HCTask::DoWork() {

    return	Commit();

}


HRET HCTask::DoTurn() { 

    

    return CommitReturn();
    
}



}

