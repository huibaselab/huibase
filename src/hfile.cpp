

#include <memory>
#include "hfile.h"

HSTR HCFile::OP_MODE(OP_TYPE ot) {

    HSTR res;
    if (ot & OP_RP) {
        res += "r+";
    } else if (ot & OP_R) {
        res += "r";
    }

    if (ot & OP_WP) {
        res += "w+";
    } else if (ot & OP_W) {
        res += "w";
    }

    if (ot & OP_AP) {
        res += "a+";
    } else if (ot & OP_A) {
        res += "a";
    }

    return res;
}


HCFile::HCFile (const HCFileName& fn, OP_TYPE ot) {

    m_pf = fopen(fn.GetName().c_str(), OP_MODE(ot).c_str());
    HASSERT_THROW_MSG(m_pf != nullptr, "fopen failed", SYS_FAILED);

}


HCFile::HCFile (HCFile&& file)
    : m_pf(file.m_pf) {

    file.m_pf = nullptr;

}


HCFile::~HCFile() {

    if (m_pf != nullptr) {

        fclose(m_pf);
        m_pf = nullptr;

    }

}



HRET HCFile::JustSaveFile(HCSTRR strFileName, HCSTRR strContent) {

    return JustSaveFile(HCFileName(strFileName), strContent);

}

#include <hlog.h>


HRET HCFile::JustSaveFile(const HCFileName & fn, HCSTRR strContent) {

    FILE * pf = fopen(fn.GetName().c_str(), "wb");
    HASSERT_THROW_MSG(pf != nullptr, "fopen failed", SYS_FAILED);

    auto cb = fwrite(strContent.c_str(), sizeof(HCHAR), strContent.size(), pf);

    LOG_NORMAL("JustSaveFile: [%d]", cb);

    fclose(pf);
    pf = nullptr;

    HRETURN_OK;
}


HRET HCFile::JustReadFile(const HCFileName &fn, HSTRR strContent) {

    FILE * pf = fopen(fn.GetName().c_str(), "rb");
    HASSERT_THROW_MSG(pf != nullptr, "fopen failed", SYS_FAILED);

    fseek(pf, 0, SEEK_END);
    size_t size = ftell(pf);
    rewind(pf);

    strContent.clear();
    strContent.resize(size);

    auto cb = fread(&strContent[0], sizeof(HCHAR), size, pf);
    HASSERT_THROW_MSG(cb == size, "fread failed", SYS_FAILED);

    LOG_NORMAL("JustReadFile: [%d]", cb);

    fclose(pf);
    HRETURN_OK;
}


HRET HCFile::JustReadFile(HCSTRR strFileName, HSTRR strConntent) {

    return JustReadFile(HCFileName(strFileName), strConntent);

}
