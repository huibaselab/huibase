

#include "htime.h"


namespace HUIBASE{

    HSTR HCTimeSpan::tostr(HCPSZ pFormat /*= "%D/%H/%M/%S"*/) const
    {
	if (!pFormat)
	    return "";        

	HSTR str;
	HCHAR ch;
	while ((ch = *pFormat++) != '\0')
	    {
		if (ch == '%')   {
		    HCHAR tempBuff[8] = {0};
		    switch (ch = *pFormat++){
		    case '%':
			str += ch;
			break;
		    case 'D':
			sprintf(tempBuff, "%d", getDays());
			str += tempBuff;
			break;
		    case 'H':
			sprintf(tempBuff, "%02d", getHours());
			str += tempBuff;                    
			break;
		    case 'M':
			sprintf(tempBuff, "%02d", getMinutes());
			str += tempBuff;
			break;
		    case 'S':
			sprintf(tempBuff, "%02d", getSeconds());
			str += tempBuff;                    
			break;        
		    }
		}
		else {
		    str += ch;
		    if ((HUCHAR)ch > 0x80)
			{
			    // ∂‡◊÷Ω⁄◊÷∑˚¥ÆµƒºÚµ•¥¶¿Ì
			    str += *pFormat++;
			}
		}
	    }

	return str;
    }

    HSTR HCTime::tostr(HCPSZ pszFormat)const
    {    
	if (m_time == -1)
	    return "invalid time";

	HCHAR szBuffer[128] = {0}; 
	if(pszFormat == NULL)                
	    return szBuffer;    

	struct tm *ptm = getLocalTm();
	if (ptm != NULL)
	    {
		if (!strftime(szBuffer, sizeof(szBuffer), pszFormat, ptm))
		    szBuffer[0] = '\0';
	    }

	return szBuffer;
    }

    HSTR HCTime::togmtstr(HCPSZ pszFormat) const
    {
	if (m_time == -1)
	    return "invalid time";

	HCHAR szBuffer[128] = {0};
	if(pszFormat == NULL)
	    return szBuffer;

	struct tm *ptm = getGmtTm();
	if (ptm != NULL)
	    {
		if (!strftime(szBuffer, sizeof(szBuffer), pszFormat, ptm))
		    szBuffer[0] = '\0';
	    }

	return szBuffer;
    }

    HCTime HCTime::parse(HCPSZ szTimeStr, HCPSZ szFormat)
    {
	struct tm atm;
	memset(&atm, 0, sizeof(atm));

	HCPSZ pFormatTag = szFormat;
	HCPSZ pDataPart = szTimeStr;
	bool bValidData = true;

	if (!szTimeStr || !szFormat)
	    return HCTime(-1);

	for(;*pFormatTag != '\0' && bValidData;)
	    {
		for (;*pFormatTag != '%' && *pFormatTag != '\0';)
		    {            
			HCHAR ch = *pFormatTag;
			HCHAR ch2 = *(pFormatTag + 1);

			++pFormatTag;
			++pDataPart;
			if ((ch == '\t' || ch == '\x20') && (ch2 != '\t' && ch2 != '\x20'))
			    {
				// Ã¯π˝∂‡”‡µƒø’∏Ò
				for (;(*pDataPart == '\x20' || *pDataPart == '\t') && (*pDataPart != '\0'); ++pDataPart);
			    }            
			else if ((HUCHAR)ch > 0x80)
			    {    
				// Ã¯π˝∂‡◊÷Ω⁄◊÷∑˚
				++pFormatTag;
				++pDataPart;
			    }
		    }

		if (*pFormatTag == '\0')
		    break;

		// get a tag, format: %[width](y|m|d|H|M|S|%)
		++pFormatTag;
		HINT width = -1;
		HSTR strTemp;
		for (;*pFormatTag >= '0' && *pFormatTag <= '9'; 
		     strTemp += *pFormatTag, ++pFormatTag) {

		}

		if (!strTemp.empty())
		    width = atoi(strTemp.c_str());

		HCHAR flag = *pFormatTag++;    
		if (flag == '%')
		    {
			bValidData = (*pDataPart++ == '%');            
			continue;
		    }

		strTemp = "";
		for (;*pDataPart >= '0' && *pDataPart <= '9' && (width == -1 || (int)strTemp.length() < width) && *pDataPart != '\0'; 
		     strTemp += *pDataPart, ++pDataPart);

		if (strTemp.empty())
		    {
			bValidData = false;
			continue;
		    }

		HINT val = atoi(strTemp.c_str());
		switch(flag)
		    {
		    case 'y':        
			atm.tm_year = val + 100; // 2000 + val - 1900
			break;

		    case 'Y':
			atm.tm_year = val - 1900;
			break;

		    case 'm':            
			atm.tm_mon = val - 1;
			break;

		    case 'd':
			atm.tm_mday = val;
			break;

		    case 'H':
			atm.tm_hour = val;
			break;

		    case 'M':
			atm.tm_min = val;
			break;

		    case 'S':
			atm.tm_sec = val;
			break;

		    default:
			bValidData = false;
			break;
		    }
	    }    

	time_t t = -1;
	if (bValidData)
	    {
		t = mktime(&atm);
	    }

	return HCTime(t);
    }

    HRET HCTime::GetNow(HTIMEBUF buf) {

	HTIMEBUF_ZERO(buf);

	time_t _tt = time(NULL);

	struct tm* _tm = localtime(&_tt);

	snprintf(buf, HTIME_LEN, "%04d-%02d-%02d %02d:%02d:%02d",
		 _tm->tm_year + 1900, _tm->tm_mon + 1,
		 _tm->tm_mday, _tm->tm_hour, _tm->tm_min,
		 _tm->tm_sec);


	HRETURN_OK;
    }

    HRET HCTime::GetDate(HTIMEBUF buf) {

	HTIMEBUF_ZERO(buf);

	time_t _tt = time(NULL);

	struct tm* _tm = localtime(&_tt);

	snprintf(buf, HTIME_LEN, "%04d-%02d-%02d",
		 _tm->tm_year + 1970, _tm->tm_mon + 1, _tm->tm_mday);


	HRETURN_OK;

    }

    HSTR HCTime::GetTimeNow() {

		HTIMEBUF buf = {0};

		GetNow(buf);

		return HSTR(buf);

    }

	HSTR HCTime::GetTimeGroup () {
		
		HCTime nt = now();

		return GetTimeGroup(nt);

	}

	HSTR HCTime::GetTimeGroup (const HCTime& nt) {

		HSTR strNg = nt.tostr("%Y%m%d%H");

		if (nt.getMinute() > 29) {

			strNg.append("30");

		} else {

			strNg.append("00");

		}

		return strNg;
	}

	HSTR HCTime::GetPrevTimeGroup() {

		HCTime nt = HCTime::now();
		HCTimeSpan ts(0,0,30); // 30 minutes
		nt -= ts;

		return GetTimeGroup(nt);

	}

    
}

















