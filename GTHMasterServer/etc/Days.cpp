





#include <time.h>
#include "../global.h"


int MonthEndDay[12]={	31,	28,	31,	30,	31,	30,	31,	31,	30,	31,	30,	31};
int YunMonthEndDay[12]={	31,	29,	31,	30,	31,	30,	31,	31,	30,	31,	30,	31};

CDays gDays;





CDays::CDays()
{
	Init();
}

CDays::~CDays()
{

}
void	CDays::Init()
{
	int tot=0,ytot=0;
	for(int i=0;i<12;i++)
	{
		MonthTot[i] = tot;
		YunMonthTot[i] = ytot;

		tot+= MonthEndDay[i];
		ytot+= MonthEndDay[i];

		if(i == 1) ytot ++;
	}
}


int CDays::IfYunYear(int year)
{
	int leap;

	leap=(year%400==0) || (year%100!=0 && year%4==0);

	if(leap) return true;
	return false;
}


int CDays::GetDays(tm tmDate)
{
	struct tm *today=0;
	time_t ltime;

	time(&ltime);

	today=localtime(&ltime);

	return GetDays(today->tm_year+1900,today->tm_mon, today->tm_mday,
		tmDate.tm_year+1900,tmDate.tm_mon, tmDate.tm_mday);
}


int CDays::GetDays(int yy,int mm,int dd)
{
	struct tm *today=0;
	time_t ltime;

	time(&ltime);

	today=localtime(&ltime);

	return GetDays(today->tm_year+1900,today->tm_mon, today->tm_mday, yy,mm,dd);
}

int	CDays::GetDays(int fyy,int fmm,int fdd, int tyy,int tmm,int tdd)
{
	int today_tot_date=0;
	int dist_tot_date=0;

	if(IfYunYear(fyy))
	{
		today_tot_date = YunMonthTot[fmm-1] + fdd;
	}
	else
	{
		today_tot_date = MonthTot[fmm-1] + fdd;
	}


	int diff_year=0;
	
	
	if(fyy  > tyy)
	{
		dist_tot_date=0;
		int startmm = tmm;

		if(IfYunYear(tyy)) 
		{
			dist_tot_date = YunMonthEndDay[tmm-1]-tdd;
		}
		else
		{
			dist_tot_date = MonthEndDay[tmm-1]-tdd;
		}

		for(int i=tyy ; i <= fyy ;i++)
		{
			for(int j=startmm; j <12;j++ )
			{
				if(i == fyy && j == fmm-1) break;
				
				if(IfYunYear(i)) 
				{
					dist_tot_date += YunMonthEndDay[j];
				}
				else
				{
					dist_tot_date += MonthEndDay[j];
				}
				
			}

			if(startmm != 0) startmm = 0;
		}

		dist_tot_date += fdd;

		return dist_tot_date;
	}

	
	else
	{
		for(int i=fyy ; i < tyy ;i++)
		{
			if(IfYunYear(i)) diff_year += 366;
			else diff_year += 365;
		}		

		if(IfYunYear(tyy))
		{
			dist_tot_date  = diff_year+ YunMonthTot[tmm-1] + tdd;
		}
		else
		{
			dist_tot_date  = diff_year + MonthTot[tmm-1] + tdd;
		}

		return (dist_tot_date - today_tot_date);
	}

	return 0;
}
