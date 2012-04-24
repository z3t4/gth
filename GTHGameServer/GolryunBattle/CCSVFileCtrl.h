



#if !defined(AFX_CCSVFILECTRL_H__73B81FA9_85AD_42A3_994F_6842B352AC71__INCLUDED_)
#define AFX_CCSVFILECTRL_H__73B81FA9_85AD_42A3_994F_6842B352AC71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CCSVFileCtrl  
{
	private:
		enum{
			MaxBytes_Token=500,
		};
		BOOL m_bOK;
		unsigned char* m_pFileBuffer;
		int            m_iFileBytes;
		char           m_szFileName[_MAX_PATH+1];
		unsigned char* m_pCurPos;
		unsigned char* m_pEndPos;
		char		   m_Token[MaxBytes_Token+1];
	private:
		BOOL LoadFile(void);
	public:
		BOOL isOK(void) const;
		const unsigned char* NextToken(void);


	
	public:
		CCSVFileCtrl(const char* szpFileName);
		virtual ~CCSVFileCtrl();

};

#endif 
