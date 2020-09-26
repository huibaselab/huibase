
#include "hshm.h"

#include <sys/types.h>

#include <sys/ipc.h>

#include <sys/shm.h>




namespace HUIBASE{




HCSharaMem::HCSharaMem()
	:  m_key(HSHM_UNINITED_KEY), m_oflag(0), m_shmsize(0), 
	m_shmid(HSHM_UNINITED_MID), m_shm_ptr(nullptr), m_is_init(HFALSE)
{
	
}


HCSharaMem::HCSharaMem(HSYS_T key, HINT oflag, HUINT shmsize)
{

    (void)Init(key, oflag, shmsize);

}



HRET HCSharaMem::Init(HSYS_T key, HINT oflag, HUINT shmsize) {

    m_key = key;

    m_oflag = oflag;

    m_shmsize = shmsize;

    m_shmid = HSHM_UNINITED_MID;

    m_shm_ptr = nullptr;

    m_is_init = HTRUE;

	HRETURN_OK;

}



HCSharaMem::~HCSharaMem() {

    (void)Dettach();

}



HRET HCSharaMem::Create() {

	HASSERT_RETURN(HIS_TRUE(m_is_init), UN_INIT);

    m_shmid = shmget(m_key, m_shmsize, m_oflag | IPC_CREAT | IPC_EXCL);

    if (m_shmid < 0) {

        if (errno != EEXIST) {

            HRETURN(SYS_FAILED);

        }
        else {

            
			HRETURN(RECREATE);
			
        }

    }


    //try to access shm
    m_shm_ptr = (HPSZ) shmat(m_shmid, NULL, 0);
    if (m_shm_ptr == (HPSZ) -1) {      

        (void)Remove();

        HRETURN(SYS_FAILED);

    }

    memset(m_shm_ptr, 0, m_shmsize);

    return Dettach();

}



HRET HCSharaMem::Open_and_attach() {

    HASSERT_RETURN(HIS_TRUE(m_is_init), UN_INIT);

    m_shmid = shmget(m_key, m_shmsize, m_oflag);
    if (m_shmid < 0) {

        HRETURN(SYS_FAILED);

    }


    //try to access shm
    m_shm_ptr = (HPSZ) shmat(m_shmid, nullptr, 0);
    if (m_shm_ptr == (HPSZ) -1) {

        (void)Remove();

        HRETURN(SYS_FAILED);

    }


    HRETURN_OK;

}



HRET HCSharaMem::Attach(void *virtual_addr /*= 0*/ , HINT flag /*= 0*/ ) {

    m_shm_ptr = (HPSZ) shmat(m_shmid, virtual_addr, flag);

    if (m_shm_ptr == (HPSZ) -1) {

        (void)Remove();

        HRETURN(SYS_FAILED);

    }

	HRETURN_OK;

}

 
HRET HCSharaMem::Dettach() {

    if (m_shm_ptr != nullptr) {

        if (shmdt(m_shm_ptr) < 0) {

            HRETURN(SYS_FAILED);

        }

        m_shm_ptr = nullptr;

    }

    HRETURN_OK;

}



HRET HCSharaMem::Remove() {

	HNOTOK_RETURN(Dettach());

    return Control(IPC_RMID);

}



HRET HCSharaMem::Control(HINT cmd, struct shmid_ds *ds) {

    if (shmctl(m_shmid, cmd, (struct shmid_ds *) ds) < 0) {

       	HRETURN(SYS_FAILED);

    }



    HRETURN_OK;

}




}

