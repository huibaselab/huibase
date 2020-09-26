/**************************=cpphd=******************************************
 * 
 * @ File Name: hlog.h
 * 
 * @ Create Author: Tom Hui
 * 
 * @ Create Date: Sun Jan 08 21:46 2017
 * 
 * @ Brief:
 * 	declare huibase's log class.
 * 
 ****************************************************************************/

#ifndef __HLOG_H__
#define __HLOG_H__

#include "huibase.h"
#include "hholder.hpp"

namespace HUIBASE
{

#define LOG_MAX_LEN (HLEN8096_C)

class HLog {
public:
	HLog(HUINT nShiftSize = ms_nShiftSize);

	HLog(HCSTRR strFileName, HUINT nShiftSize = ms_nShiftSize);

	virtual ~HLog();

	void SetFileLogName(HCSTRR strFileName) {

		m_strFileLogName = strFileName;

	}

	HCSTRR GetFileLogName() const { return m_strFileLogName; }

public:
	HRET Normal(HCPSZ ff, HINT ll, HCPSZ fmt, ...);

	HRET Warning(HCPSZ ff, HINT ll, HCPSZ fmt, ...);

	HRET Error(HCPSZ ff, HINT ll, HCPSZ fmt, ...);

	HRET PushLog (HCPSZ ff, HINT ll, HCPSZ fmt, ...);

	HRET WriteLog(HCPSZ ff, HINT ll, HCPSZ stype, HCPSZ slog);

	HRET WriteLog(HCPSZ ff, HINT ll, HCPSZ slog);

	HRET __write_log(HCPSZ ff, HINT ll, HCPSZ slog, HCPSZ stype = nullptr, bool with_flag = false);

protected:
	HSTR newLogName();

	HRET shiftLog();

private:
	void init();

	void uninit();

private:
	HSTR m_strFileLogName;

	HUINT m_len = 0;

	HUINT m_todayIndex = 0;

	// Declare the shift size variable, make it can be configed.
	const HUINT m_cnShiftSize;	

	HSYS_T m_fd = HINVA_FD;

private:
	static time_t ms_logTime;
	static HBOOL more_day();

	static const HUINT ms_nShiftSize = 1024 * 1024 * 100;		// 100M
};

class HLogManager {
public:
	typedef enum {
		lfs_normal,
		lfs_warn,
		lfs_error,

		lfb_normal,
		lfb_warn,
		lfb_error,

		lf_end
	} hlog_flags;

public:
	HLogManager (HUINT nShiftSize = 1024 * 1024 * 100);

	HLogManager (HCSTRR strName, HINT isMulti = 0, HINT isWithSystem = 0, HUINT nShiftSize = 1024 * 1024 * 100);

	~ HLogManager ();

	HLog* GetLog (hlog_flags lf);

	void Setup (HCSTRR strName, HINT isMulti = 0, HINT isWithSystem = 0);

	HINT IsMultiLog () const { return m_isMultiLog; }

	HINT IsWithSystem () const { return m_isWithSystem; }

public:

	static void log_down(hlog_flags fl, HCPSZ ff, HINT ll, HCPSZ fmt, ...);

	HSTR GetFlagsStr (hlog_flags lf);

protected:
	void init ();

	void uninit();


private:
	HINT m_isMultiLog = 0;

	HINT m_isWithSystem = 0;

	HUINT m_nShiftSize = 1024 * 1024 * 100;		// 100M
	
	HLog* m_logs[lf_end] = {0};

	HSTR m_strName;

public:
	static HLogManager gs_log_manger;
	static HLogManager *gs_manager;
};

HRET InitLog(HCSTRR strFilename, HINT isMultiLog = 0, HINT isSystem = 1);


#ifndef _JUST_TEST_
#define LOG_NORMAL(fmt, args...) HLogManager::log_down(HLogManager::lfb_normal, __FILE__, __LINE__, fmt, ##args)

#define LOG_WARNING(fmt, args...) HLogManager::log_down(HLogManager::lfb_warn, __FILE__, __LINE__, fmt, ##args)

#define LOG_ERROR(fmt, args...) HLogManager::log_down(HLogManager::lfb_error, __FILE__, __LINE__, fmt, ##args)

#define LOG_NS(ss) HLogManager::log_down(HLogManager::lfb_normal, __FILE__, __LINE__, ss)
#define LOG_WS(ss) HLogManager::log_down(HLogManager::lfb_warn, __FILE__, __LINE__, ss)
#define LOG_ES(ss) HLogManager::log_down(HLogManager::lfb_error, __FILE__, __LINE__, ss)

#define HFUN_BEGIN LOG_NORMAL("[%s]: begin...", __FUNCTION__)
#define HFUN_END LOG_NORMAL("[%s]: end...", __FUNCTION__)

#define SLOG_NORMAL(fmt, args...) HLogManager::log_down(HLogManager::lfs_normal, __FILE__, __LINE__, fmt, ##args)

#define SLOG_WARNING(fmt, args...) HLogManager::log_down(HLogManager::lfs_warn, __FILE__, __LINE__, fmt, ##args)

#define SLOG_ERROR(fmt, args...) HLogManager::log_down(HLogManager::lfs_error, __FILE__, __LINE__, fmt, ##args)

#define SLOG_NS(ss) HLogManager::log_down(HLogManager::lfs_normal, __FILE__, __LINE__, ss)
#define SLOG_WS(ss) HLogManager::log_down(HLogManager::lfs_warn, __FILE__, __LINE__, ss)
#define SLOG_ES(ss) HLogManager::log_down(HLogManager::lfs_error, __FILE__, __LINE__, ss)

#else

#define HFUN_END
#define HFUN_BEGIN
#define LOG_NORMAL
#define LOG_WARNING
#define LOG_ERROR

#define LOG_NS
#define LOG_WS
#define LOG_ES

#define SLOG_NORMAL
#define SLOG_WARNING
#define SLOG_ERROR

#define SLOG_NS
#define SLOG_WS
#define SLOG_ES

#endif

} // namespace HUIBASE

#endif //__HLOG_H__
