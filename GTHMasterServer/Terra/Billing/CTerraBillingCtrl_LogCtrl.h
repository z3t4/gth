



#if !defined(AFX_CTERRABILLINGCTRL_LOGCTRL_H__1DC2AC1C_FE9B_490E_A4F0_2AF95AA14FDB__INCLUDED_)
#define AFX_CTERRABILLINGCTRL_LOGCTRL_H__1DC2AC1C_FE9B_490E_A4F0_2AF95AA14FDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CTerraBillingCtrl_LogCtrl  
{
	private:
		BOOL  m_bOK;		
		char  m_szFileName[_MAX_PATH];
		FILE* m_fp;
		int   m_NextUpdateTime;
		int	  m_generationHour;

	private:
		inline BOOL OpenFile(void);
		inline void CloseFile(void);

	public:
		BOOL isOK(void) const;
		void Update(void);
		void Write(const char* format, ... );
		static char* GetYESNOstring(BOOL bYESorNO);

	
	public:
		CTerraBillingCtrl_LogCtrl();
		virtual ~CTerraBillingCtrl_LogCtrl();

};

#endif 
