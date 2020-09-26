/**************************************************
*	Create Author: TomHui
*	Create Date: 2016-02-16
*	Description: this file share memory class
*					for base library.
**************************************************/


#ifndef _H_SHM_H_
#define _H_SHM_H_

#include "huibase.h"

#include <sys/types.h>

#include <sys/ipc.h>

#include <sys/shm.h>

namespace HUIBASE{

class HCSharaMem

{

public:

    HCSharaMem();

    HCSharaMem(HSYS_T key, HINT flag, HUINT shmsize);
	
    virtual ~HCSharaMem();

    HRET Init(HSYS_T key, HINT oflag, HUINT shmsize);

public:

    HINT Create();



    HRET Open_and_attach();

    

    HRET Attach(void *virtual_addr = 0, HINT flag = 0);



    HRET Dettach();

    

    void * Get_shm_ptr() const {return (void*)m_shm_ptr;}



    HUINT Get_shm_size() const {return m_shmsize;}



public:

    HRET Remove();



    HRET Control(HINT cmd, struct shmid_ds * ds = 0);



    HINT Get_id() const {return m_shmid;}



    void Set_id(HINT shmid){ m_shmid = shmid;}




private:

	static const HSYS_T HSHM_UNINITED_KEY = -1;

	static const HINT HSHM_UNINITED_MID = -1;

    HSYS_T m_key = HSHM_UNINITED_KEY;

    HINT m_oflag = 0;

    HUINT m_shmsize = 0;

    HINT m_shmid = HSHM_UNINITED_MID;

    HPSZ m_shm_ptr = nullptr;

    HBOOL m_is_init = HFALSE;

};

}

#endif

