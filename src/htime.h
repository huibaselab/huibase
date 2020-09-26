/**************************=cpphd=******************************************
 * 
 * @ File Name: htime.h
 * 
 * @ Create Author: Tom Hui
 * 
 * @ Create Date: Tue Dec 27 16:00 2016
 * 
 * @ Brief:
 * 	time class.
 * 
 ****************************************************************************/



#ifndef __HTIME_H__
#define __HTIME_H__

#include "huibase.h"
#include "hstr.h"

namespace HUIBASE{

///////////////////////////////////////////////////////////////////////////////

class HCTimeSpan
{
public:
    HCTimeSpan() : m_timeSpan(0)
    {}

    HCTimeSpan( time_t time ) : m_timeSpan(time)
    {}

    HCTimeSpan( HINT lDays, HINT nHours = 0, HINT nMins = 0, HINT nSecs = 0)
    {
	m_timeSpan = nSecs + 60* (nMins + 60* (nHours + 24 * lDays));
    }

    HINT getDays() const {
	return( (HINT)m_timeSpan / (24*3600) );
    }

    HINT getTotalHours() const {
	return ( (HINT)m_timeSpan / 3600 );
    }

    HINT getHours() const {
	return (getTotalHours() % 24);
    }

    HINT getTotalMinutes() const {
	return ( (HINT)m_timeSpan / 60);
    }

    HINT getMinutes() const {
	return (getTotalMinutes() % 60);
    }

    HINT getTotalSeconds() const {
	return (HINT)m_timeSpan;
    }

    HINT getSeconds() const {
	return ( (HINT)m_timeSpan % 60);
    }

    time_t getTimeSpan() const {
	return m_timeSpan;
    }

    HCTimeSpan operator+( HCTimeSpan span ) const {
	return( HCTimeSpan( m_timeSpan+span.m_timeSpan ) );
    }

    HCTimeSpan operator-( HCTimeSpan span ) const {
	return( HCTimeSpan( m_timeSpan-span.m_timeSpan ) );
    }

    HCTimeSpan& operator+=( HCTimeSpan span ) {
	m_timeSpan += span.m_timeSpan;
	return( *this );
    }

    HCTimeSpan& operator-=( HCTimeSpan span ) {
	m_timeSpan -= span.m_timeSpan;
	return( *this );
    }

    HBOOL operator==( HCTimeSpan span ) const {
	HRET_BOOL(( m_timeSpan == span.m_timeSpan ));
    }

    HBOOL operator!=( HCTimeSpan span ) const {
	HRET_BOOL(( m_timeSpan != span.m_timeSpan ));
    }

    HBOOL operator<( HCTimeSpan span ) const {
	HRET_BOOL(( m_timeSpan < span.m_timeSpan ));
    }

    HBOOL operator>( HCTimeSpan span ) const {
	HRET_BOOL(( m_timeSpan > span.m_timeSpan ));
    }

    HBOOL operator<=( HCTimeSpan span ) const {
	HRET_BOOL(( m_timeSpan <= span.m_timeSpan ));
    }

    HBOOL operator>=( HCTimeSpan span ) const {
	HRET_BOOL(( m_timeSpan >= span.m_timeSpan ));
    }

public:
    //  * the only valid formats:
    //  %D - # of days
    //  %H - hour in 24 hour format
    //  %M - minute (0-59)
    //  %S - seconds (0-59)
    //  %% - percent sign
    HSTR tostr(HCPSZ pszFormat = "%D/%H/%M/%S") const;

private:
    time_t m_timeSpan;
};


class HCTime
{
public:
    static HCTime now() {
	return HCTime(time(NULL));
    }

    // parse time from string, avaliable format: %[w](y|Y|m|d|H|M|S)
    // %Y: long year field (1970-2026), %y: short year field(00-26)
    // %m: month field (1-12), %d: day field (1-31), 
    // %H: hour filed (0-23),  %M: minute filed (0-59), 
    // %S: second filed (0-59), %%: percent sign
    // w: field width
    // perfect programming art (:
    static HCTime parse(HCPSZ szTimeStr, HCPSZ szFormat = "%Y-%m-%d %H:%M:%S");

    HCTime() : m_time(0){}
    HCTime( time_t time ) : m_time(time){}

    HCTime( struct tm time ) {
	m_time = mktime(&time);
    }

    HCTime( HINT nYear, HINT nMonth, HINT nDay, HINT nHour = 0, HINT nMin = 0, HINT nSec = 0, HINT nDST = -1 ) {
	struct tm atm;
	atm.tm_sec = nSec;
	atm.tm_min = nMin;
	atm.tm_hour = nHour;        
	atm.tm_mday = nDay;        
	atm.tm_mon = nMonth - 1;        // tm_mon is 0 based        
	atm.tm_year = nYear - 1900;     // tm_year is 1900 based
	atm.tm_isdst = nDST;
	m_time = mktime(&atm);        
    }

    HCTime(const HCTime& time) {
	m_time = time.m_time;
    }

    HCTime& operator=( time_t time ) {        
	m_time = time;        
	return( *this );
    }

    HCTime& operator= ( const HCTime& tt ) {

        m_time = tt.m_time;
        return (*this);

    }

    HCTime& operator+=( HCTimeSpan span ) {
	m_time += span.getTimeSpan();        
	return( *this );
    }

    HCTime& operator-=( HCTimeSpan span ) {
	m_time -= span.getTimeSpan();
	return (*this);
    }

    HCTimeSpan operator-( HCTime time ) const {
	return( HCTimeSpan( m_time-time.m_time ) );
    }

    HCTime operator-( HCTimeSpan span ) const {
	return( HCTime( m_time-span.getTimeSpan() ) );
    }

    HCTime operator+( HCTimeSpan span ) const {
	return( HCTime( m_time+span.getTimeSpan() ) );
    }

    HBOOL operator==( HCTime time ) const {
	HRET_BOOL(( m_time == time.m_time ));
    }

    HBOOL operator!=( HCTime time ) const {
	HRET_BOOL(( m_time != time.m_time ));
    }

    HBOOL operator<( HCTime time ) const {
	HRET_BOOL(( m_time < time.m_time ));
    }

    HBOOL operator>( HCTime time ) const {
	HRET_BOOL(( m_time > time.m_time ));
    }

    HBOOL operator<=( HCTime time ) const {
	HRET_BOOL(( m_time <= time.m_time ));
    }

    bool operator>=( HCTime time ) const {
	return( m_time >= time.m_time );
    }

	void Touch () {
		m_time = time(nullptr);
	}

    struct tm* getGmtTm( struct tm* ptm = NULL ) const {
	struct tm * ptmTemp;
	ptmTemp = gmtime(&m_time);

	if (ptmTemp == NULL)
	    return NULL;

	if (ptm != NULL) {
		*ptm = *ptmTemp;
		return ptm;
	}
	else {
		return ptmTemp;
	}
    }

    struct tm* getLocalTm( struct tm* ptm = NULL ) const {
	struct tm * ptmTemp;
	ptmTemp = localtime(&m_time);

	if (ptmTemp == NULL)
	    return NULL;

	if (ptm != NULL) {
	    *ptm = *ptmTemp;
	    return ptm;
	}
	else  {
	    return ptmTemp;
	}
    }

    time_t getTime() const {
	return m_time;
    }

    HINT getYear() const {
	struct tm * ptm;
	ptm = getLocalTm();
	return ptm ? (ptm->tm_year + 1900) : 0 ;         
    }

    HINT getMonth() const {
	struct tm * ptm;
	ptm = getLocalTm();
	return ptm ? (ptm->tm_mon + 1) : 0 ; 
    }

    HINT getDay() const {
	struct tm * ptm;
	ptm = getLocalTm();
	return ptm ? ptm->tm_mday : 0 ; 
    }

    HINT getHour() const {
	struct tm * ptm;
	ptm = getLocalTm();
	return ptm ? ptm->tm_hour : -1 ; 
    }

    HINT getMinute() const {
	struct tm * ptm;
	ptm = getLocalTm();
	return ptm ? ptm->tm_min : -1 ; 
    }

    HINT getSecond() const {
	struct tm * ptm;
	ptm = getLocalTm();
	return ptm ? ptm->tm_sec : -1 ;
    }

    HINT getDayOfWeek() const {
	struct tm * ptm;
	ptm = getLocalTm();
	return ptm ? ptm->tm_wday + 1 : 0 ;
    }

    // formatting using "C" strftime
    HSTR tostr( HCPSZ pszFormat="%Y-%m-%d %H:%M:%S") const;
    HSTR togmtstr( HCPSZ pszFormat="%Y-%m-%d %H:%M:%S") const;

    // get time now. thread-safe
    static HRET GetNow(HTIMEBUF buf);

    // get date to buffer. thread-safe
    static HRET GetDate(HTIMEBUF buf);

    static HSTR GetTimeNow ();

    static HSTR GetTimeGroup ();

    static HSTR GetTimeGroup (const HCTime& time);

    static HSTR GetPrevTimeGroup();

private:
    time_t m_time;
};

///////////////////////////////////////////////////////////////////////////////

}

#endif // __HTIME_H__

