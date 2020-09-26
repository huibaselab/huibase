/*****************************************************************************
 *
 * FileName: hfile.h
 *
 * Author: Tom Hui, tomhui1009@yahoo.com, 8613760232170
 *
 * Create Date: Thu Sep 27 15:28 2018
 *
 * Brief:
 *
 *
 *****************************************************************************/


#ifndef __HFILE_H__
#define __HFILE_H__

#include <huibase.h>
#include <hfname.h>


using namespace HUIBASE;


class HCFile {
 public:
    typedef enum {
        OP_R = 1 << 0,
        OP_RP = 1<<1,
        OP_W = 1<<2,
        OP_WP = 1<<3,
        OP_A = 1<<4,
        OP_AP = 1<<5,

        OP_END = 0
    } OP_TYPE;

    static HSTR OP_MODE(OP_TYPE ot);

 public:
    HCFile () noexcept { }

    HCFile (const HCFileName& fn, OP_TYPE ot = OP_R);

    HCFile (const HCFile& file) = delete;

    HCFile (HCFile&& file);

    virtual ~ HCFile();

    static HRET JustSaveFile (HCSTRR strFileName, HCSTRR strContent);

    static HRET JustSaveFile (const HCFileName& fn, HCSTRR strContent);

    static HRET JustReadFile (const HCFileName& fn, HSTRR strContent);

    static HRET JustReadFile (HCSTRR strFileName, HSTRR strConntent);


 private:
    FILE* m_pf = nullptr;
};



#endif //__HFILE_H__
