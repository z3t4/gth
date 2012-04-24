



#if !defined(AFX_CDBQUERYRESULTPROCESSCTRL_H__5A82B753_E5C4_4718_ADC7_4DBE2D65B4E2__INCLUDED_)
#define AFX_CDBQUERYRESULTPROCESSCTRL_H__5A82B753_E5C4_4718_ADC7_4DBE2D65B4E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CDBQueryResultProcessCtrl  
{
	private:
		BOOL m_bOK;

	public:
		BOOL isOK(void) const;
		void Process(void) const;

	public:
		CDBQueryResultProcessCtrl();
		virtual ~CDBQueryResultProcessCtrl();

};

#endif 
