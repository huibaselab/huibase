

#include "hlog.h"
#include "htime.h"

namespace HUIBASE {

time_t HLog::ms_logTime = time(NULL);

HLog::HLog(HUINT nShiftSize) :
	m_cnShiftSize(nShiftSize) {

	m_fd = HINVA_FD;

}

HLog::HLog(HCSTRR strFileName, HUINT nShiftSize)
	: m_strFileLogName(strFileName), m_cnShiftSize(nShiftSize) {

	m_fd = HINVA_FD;
	
	init();

}

HLog::~HLog() {

	uninit();

}

#define __WRITE_LOG                              \
	HCHAR buf[LOG_MAX_LEN] = {0};                \
	memset(buf, 0, sizeof(HCHAR) * LOG_MAX_LEN); \
	va_list ap;                                  \
	va_start(ap, fmt);                           \
	vsnprintf(buf, LOG_MAX_LEN, fmt, ap);        \
	va_end(ap)

HRET HLog::Normal(HCPSZ ff, HINT ll, HCPSZ fmt, ...)
{

	__WRITE_LOG;

	WriteLog(ff, ll, "NORMAL", buf);

	HRETURN_OK;
}

HRET HLog::Warning(HCPSZ ff, HINT ll, HCPSZ fmt, ...)
{

	__WRITE_LOG;

	WriteLog(ff, ll, "WARNING", buf);

	HRETURN_OK;
}

HRET HLog::Error(HCPSZ ff, HINT ll, HCPSZ fmt, ...)
{

	__WRITE_LOG;

	WriteLog(ff, ll, "ERROR", buf);

	HRETURN_OK;
}


HRET HLog::PushLog (HCPSZ ff, HINT ll, HCPSZ fmt, ...) {

	__WRITE_LOG;

	WriteLog(ff, ll, buf);

	HRETURN_OK;

}



HRET HLog::WriteLog(HCPSZ ff, HINT ll, HCPSZ stype, HCPSZ slog)
{

	return __write_log(ff, ll, slog, stype, true);
}


HRET HLog::WriteLog(HCPSZ ff, HINT ll, HCPSZ slog) {
	
	return __write_log(ff, ll, slog, nullptr, false);

}


HRET HLog::__write_log(HCPSZ ff, HINT ll, HCPSZ slog, HCPSZ stype, bool with_flag) {
	HCHAR buf[LOG_MAX_LEN] = {0};

	if (with_flag) {

#ifdef WIN32
	snprintf(buf, LOG_MAX_LEN, "[%s][%d][%s]: %s.[%s:%d]\n", HCTime::GetTimeNow().c_str(), (int)getpid(), stype, slog, ff, ll);
#else
	snprintf(buf, LOG_MAX_LEN, "[%s][%d][%lu][%s]: %s.[%s:%d]\n", HCTime::GetTimeNow().c_str(), (int)getpid(), (unsigned long)pthread_self(), stype, slog, ff, ll);
#endif

	} else {

#ifdef WIN32
	snprintf(buf, LOG_MAX_LEN, "[%s][%d]: %s.[%s:%d]\n", HCTime::GetTimeNow().c_str(), (int)getpid(), slog, ff, ll);
#else
	snprintf(buf, LOG_MAX_LEN, "[%s][%d][%lu]: %s.[%s:%d]\n", HCTime::GetTimeNow().c_str(), (int)getpid(), (unsigned long)pthread_self(), slog, ff, ll);
#endif

	}

	m_len += strlen(buf);

	shiftLog();

	if (HINVA_FD == m_fd)
	{

		init();

	}

	ssize_t wcb = write(m_fd, buf, strlen(buf));
	if (wcb < 0)
	{
		printf("write failed");
	}

	HRETURN_OK;
}



HSTR HLog::newLogName()
{

	HSTR ret;

	ret = m_strFileLogName + "_" + HCTime::GetTimeNow().substr(0, 10) + "_" +
		  HCStr::itos(m_todayIndex) + ".log";

	return ret;
}

void HLog::init() {

	// before initalize the log file descriptor.
	// We need to make sure it has been uninitialized.
	if (HINVA_FD != m_fd) {

		uninit();

	}

	m_fd = ::open(newLogName().c_str(), O_CREAT | O_WRONLY | O_APPEND, 0664);
	if (HINVA_FD == m_fd) {

		printf("fopen failed\n");
		exit(-1);

	}

	m_len = 0;

}

void HLog::uninit()
{
	if (m_fd > 0) {

		::close(m_fd);

		m_fd = HINVA_FD;

	}

	m_len = 0;

}

HRET HLog::shiftLog()
{

	if (HIS_TRUE(more_day()))
	{

		m_todayIndex = 0;

		m_len = 0;

		init();
	}

	if (m_len > m_cnShiftSize)
	{


		++m_todayIndex;

		init();
	}

	HRETURN_OK;
}

HBOOL HLog::more_day()
{

	HTIME _tt = time(NULL);
	struct tm *p = localtime(&_tt);
	int ny = p->tm_year, nm = p->tm_mon, nd = p->tm_mday;

	p = localtime(&ms_logTime);
	int ly = p->tm_year, lm = p->tm_mon, ld = p->tm_mday;

	if (ny == ly && nm == lm && nd == ld)
	{
		return HFALSE;
	}

	ms_logTime = _tt;

	return HTRUE;
}


HLogManager::HLogManager (HUINT nShiftSize) 
	: m_nShiftSize(nShiftSize) {

	for (HINT i = 0; i < lf_end; ++i) {

		m_logs[i] = nullptr;

	}

}


HLogManager::HLogManager (HCSTRR strName, HINT isMulti, HINT isWithSystem, HUINT nShiftSize)
	: m_isMultiLog (isMulti), m_isWithSystem(isWithSystem), m_nShiftSize(nShiftSize), m_strName(strName) {

	for (HINT i = 0; i < lf_end; ++i) {

		m_logs[i] = nullptr;

	}

	init();

}

HLogManager::~HLogManager () {

	uninit();

}


HLog* HLogManager::GetLog (hlog_flags lf) {

	HASSERT_THROW(lf < lf_end, INDEX_OUT);

	if (m_isMultiLog){

		if (m_isWithSystem) {
			return m_logs[lf];
		} else {
			if (lf >= lfb_normal) {
				lf = (hlog_flags)((int)lf - (int)lfb_normal);
			}

			return m_logs[lf];
		}

	} else {

		if (m_isWithSystem) {

			if (lf < lfb_normal) {

				return m_logs[lfs_normal]; 

			} else {
				
				return m_logs[lfb_normal];

			}

		} else {

			return m_logs[lfs_normal];

		}

	}

	return nullptr;

}


void HLogManager::Setup (HCSTRR strName, HINT isMulti, HINT isWithSystem) {

	m_strName = strName;

	m_isMultiLog = isMulti;

	m_isWithSystem = isWithSystem;

	init();

}


#define INIT_LOG_MS(fmt,index) \
	strName = HCStr::Format(fmt, m_strName.c_str()); \
	m_logs[index] = new HLog(strName, m_nShiftSize); \
	CHECK_NEWPOINT(m_logs[index])

void HLogManager::init () {
	
	HSTR strName;
	if (m_isMultiLog) {
		// multi log instance
		if (m_isWithSystem) {

			INIT_LOG_MS("%s_sn",lfs_normal);

			INIT_LOG_MS("%s_sw",lfs_warn);

			INIT_LOG_MS("%s_se",lfs_error);

			INIT_LOG_MS("%s_bn",lfb_normal);
			
			INIT_LOG_MS("%s_bw",lfb_warn);

			INIT_LOG_MS("%s_be",lfb_error);

		} else {

			INIT_LOG_MS("%s_normal",lfs_normal);

			INIT_LOG_MS("%s_warn",lfs_warn);

			INIT_LOG_MS("%s_error",lfs_error);
			
		}

	} else {

		if (m_isWithSystem) {

			INIT_LOG_MS("%s_system",lfs_normal);

			INIT_LOG_MS("%s",lfb_normal);

		} else {

			INIT_LOG_MS("%s",lfs_normal);

		}

	}
	

}

#undef INIT_LOG_MS


void HLogManager::uninit() {

	for (HINT i = 0; i < lf_end; ++i) {

		HLog* p = m_logs[i];
		HDELP(p);

	}

}


HLogManager HLogManager::gs_log_manger;
HLogManager* HLogManager::gs_manager = nullptr;

HRET InitLog(HCSTRR strFilename, HINT isMultiLog, HINT isSystem)
{

	HLogManager::gs_log_manger.Setup(strFilename, isMultiLog, isSystem);

	HLogManager::gs_manager = &HLogManager::gs_log_manger;

	HRETURN_OK;
}

void HLogManager::log_down(hlog_flags fl, HCPSZ ff, HINT ll, HCPSZ fmt, ...)
{

	HCHAR buf[LOG_MAX_LEN] = {0};
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, LOG_MAX_LEN, fmt, ap);
	va_end(ap);

	HLog* plog = gs_manager->GetLog(fl);

	if (not gs_manager->IsMultiLog() and not gs_manager->IsWithSystem()) {
		plog->WriteLog(ff, ll, gs_manager->GetFlagsStr(fl).c_str(), buf);
		return ;
	} 

	if (gs_manager->IsMultiLog()) {
		plog->WriteLog(ff, ll, buf);
	} else {
		plog->WriteLog(ff, ll, gs_manager->GetFlagsStr(fl).c_str(), buf);
	}
}

#define MAY_SYSTEM(ss) do { if (not IsMultiLog() and not IsWithSystem()) { \
		return "SYSTEM_"#ss; \
	} else { \
		return #ss; \
	} } while (0)



HSTR HLogManager::GetFlagsStr (hlog_flags lf) {

	HASSERT_THROW(lf < lf_end, INDEX_OUT);

	switch (lf)
	{
		case lfs_normal:
			MAY_SYSTEM(Normal);		
			break;

		case lfs_warn:
			MAY_SYSTEM(Warning);		
			break;

		case lfs_error:
			MAY_SYSTEM(Error);		
			break;
		
		case lfb_normal:
			return "Normal";
			break;

		case lfb_warn:
			return "Warning";
			break;

		case lfb_error:
			return "Error";
			break;
	
		default:
			return "SYSTEM_ERROR";
			break;
	}

	return "SYSTEM_ERROR";
}


} // namespace HUIBASE
