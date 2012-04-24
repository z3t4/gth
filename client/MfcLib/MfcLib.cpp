
#include "stdafx.h"

int GetDays(tm ToDay)
{
	CTime from;
	from =CTime::GetCurrentTime();

	CTime to(ToDay.tm_year,ToDay.tm_mon,ToDay.tm_mday,0,0,0);

	CTimeSpan elapsedTime = to - from;

	return elapsedTime.GetDays();
}

int GetElapsedDays (int yy,int mm,int dd)
{
	CTime curr;
	curr =CTime::GetCurrentTime();
	
	CTime from(curr.GetYear(),curr.GetMonth(),curr.GetDay(),24,0,0);
	CTime to(yy,mm,dd,24,0,0);
	CTimeSpan elapsedTime = to - from;
	
	return elapsedTime.GetDays();
}



