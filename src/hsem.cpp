

#include "hsem.hpp"
#include "hutil.h"

namespace HUIBASE{

#define HSEM_CHECK HASSERT_RETURN(HIS_TRUE(m_bInited),UN_INIT)

HCSemaphore::HCSemaphore()
    :m_key(HSEM_UNINITED_KEY), m_semflag(0), m_nsems(0), 
    m_sem_id(HSEM_UNINITED_ID), m_bInited(HFALSE){

}


HCSemaphore::HCSemaphore(HSYS_T key, HINT semflag, HINT nsems){

    (void)Init(key, semflag, nsems);
    
}
    
HCSemaphore::~HCSemaphore(){
    
    (void)Close();
    
}


HRET HCSemaphore::Create(HUSHORT * init_array ){

	HSEM_CHECK;
    
    m_sem_id = semget(m_key, m_nsems, IPC_CREAT | IPC_EXCL | m_semflag);

    if( m_sem_id < 0 ){

        if (errno == EEXIST) {

            HRETURN(RECREATE);

        }

        else {
            
            HRETURN(SYS_FAILED);

        }
        
    }

    HUSHORT *array = nullptr;

    if (init_array != NULL) {

        array = init_array;

    }

    else {

        array = new HUSHORT[m_nsems];

        for (int i = 0; i < m_nsems; ++i) {

            array[i] = 1;

        }

    }

    union SVSemUn arg;

    arg.array = array;

	HRET _cb = Control(0, SETALL, arg);
    if  (HNOT_OK(_cb) ){

        (void)Remove();

        if (init_array == NULL) {

            delete[]array;

        }

        return _cb;

    }


    Lock(0);

    Unlock(0);


    HRETURN_OK;
    
}

HRET HCSemaphore::Init(HSYS_T key, HINT semflag, HINT nsems){

    m_key = key;

    m_semflag = semflag;

    m_nsems = nsems;

    m_sem_id = HSEM_UNINITED_ID;

    m_bInited = HTRUE;

	HRETURN_OK;
    
}

HRET HCSemaphore::Open(){

    HSEM_CHECK;

    const static int MAX_TRIES = 5;

    m_sem_id= semget(m_key, 0, m_semflag);

    if (m_sem_id < 0) {

        HRETURN(SYS_FAILED);

    }


    struct semid_ds seminfo;
    union SVSemUn arg;
    arg.buf = &seminfo;


    bool is_timeout = true;
    for (int i = 0; i < MAX_TRIES; ++i) {

        /* waiting for the sem to be initialized */
		HNOTOK_RETURN(Control(0, IPC_STAT, arg));

        if (arg.buf->sem_otime != 0) {
			
            is_timeout = false;
			
            break;

        }

        /* waiting for 0.1 second */
        hsleep(0, 100000);

    }

    if (is_timeout) {
		
        HRETURN(TIME_OUT);

    }

    
    HRETURN_OK;
}

HRET HCSemaphore::Close(){

    HRETURN_OK;
    
}

HRET HCSemaphore::Lock(HSHORT semnum , HSHORT sem_flg  ){

    struct sembuf sops;

    sops.sem_num = semnum;

    sops.sem_op = -1;

    sops.sem_flg = sem_flg;

    if (semop(m_sem_id, &sops, 1) < 0) {

        HRETURN(SYS_FAILED);

    }

    HRETURN_OK;
    
}

HRET HCSemaphore::Unlock(HSHORT semnum, HSHORT sem_flg  ){

    struct sembuf sops;

    sops.sem_num = semnum;

    sops.sem_op = 1;

    sops.sem_flg = sem_flg;



    if (semop(m_sem_id, &sops, 1) < 0) {

        HRETURN(SYS_FAILED);

    }

    HRETURN_OK;

}


HRET HCSemaphore::Operation(struct sembuf *sops, HUINT nsops){

    if (semop(m_sem_id, sops, nsops) < 0) {

        HRETURN(SYS_FAILED);
    }



    HRETURN_OK;

}


HRET HCSemaphore::Remove(){

    if (m_sem_id >= 0) {

        if (semctl(m_sem_id, 0, IPC_RMID) < 0) {

            HRETURN(SYS_FAILED);

        }

    }

    HRETURN_OK;
    
}


HRET HCSemaphore::Control(HINT semnum, HINT cmd, SVSemUn arg){

    if (semctl(m_sem_id, semnum, cmd, arg) < 0) {

        HRETURN(SYS_FAILED);

    }

    HRETURN_OK;

}

CSemHolder::CSemHolder(HCSemaphore* sem, HSHORT index) 
	: m_sem(sem), m_index(index) {
	
	m_sem->Lock(m_index);
	
}

CSemHolder::~CSemHolder () {

	m_sem->Unlock (m_index);
	
}

}
