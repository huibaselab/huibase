/**************************************************
*	Create Author: TomHui
*	Create Date: 2016-08-09
*	Description: this file declare semphore for
*                   hbase library.
**************************************************/


#include "huibase.h"
#include <sys/types.h>

#include <sys/ipc.h>

#include <sys/sem.h>


#ifndef _H_SEM_H_
#define _H_SEM_H_

namespace HUIBASE{

union SVSemUn{

    int val;                  /* value for SETVAL */

    struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */

    unsigned short *array;    /* array for GETALL, SETALL */

                              /* Linux specific part: */

    struct seminfo *__buf;    /* buffer for IPC_INFO */

};


class HCSemaphore{
public:
    HCSemaphore();
    HCSemaphore(HSYS_T key, HINT semflag, HINT nsems);

    virtual ~ HCSemaphore();

public:
    HRET Create(HUSHORT * init_array = NULL);
    HRET Init(HSYS_T key, HINT semflag, HINT nsems);    
    HRET Open();
    HRET Close();

public:

    HRET Lock(HSHORT semnum = 0, HSHORT sem_flg = SEM_UNDO);
    HRET Unlock(HSHORT semnum = 0, HSHORT sem_flg = SEM_UNDO);
    HRET Operation(struct sembuf *sops, HUINT nsops);

    HRET Remove();
    HRET Control(HINT semnum, HINT cmd, SVSemUn arg);
    HRET Get_id() const {return m_sem_id;}
    void Set_id(HINT semid){m_sem_id = semid;}

private:
	static const HSYS_T HSEM_UNINITED_KEY = -1;
	static const HINT HSEM_UNINITED_ID = -1;

    HSYS_T m_key = HSEM_UNINITED_KEY;
    HINT m_semflag = 0;
    HINT m_nsems = 0;

    HINT m_sem_id = HSEM_UNINITED_ID;
    HBOOL m_bInited =  HFALSE;   
};

class CSemHolder {
public:
	CSemHolder (HCSemaphore* sem, HSHORT index);

	~CSemHolder ();
private:
	HCSemaphore* m_sem;
	HSHORT m_index;
};

#define SEMLOCK(psem,index) CSemHolder _##sem##semlock(psem,index);


}
#endif

