



#if !defined(AFX_DAYS_H__7EFDED9F_F832_404E_A17B_4963F280A015__INCLUDED_)
#define AFX_DAYS_H__7EFDED9F_F832_404E_A17B_4963F280A015__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


class CDays  
{
public:
	CDays();
	virtual ~CDays();

	void		Init();
	
	int         GetDays(tm tmDate);
	int			GetDays(int yy,int mm,int dd);
	int			IfYunYear(int year);
	int			GetDays(int fyy,int fmm,int fdd, int tyy,int tmm,int tdd);


public:
	int MonthTot[12];
	int YunMonthTot[12];
};


extern CDays gDays;

#endif 
