



#if !defined(AFX_CSERVERLPACKCTRL_H__26EF6DC0_F144_451E_9BC4_060E53FB268A__INCLUDED_)
#define AFX_CSERVERLPACKCTRL_H__26EF6DC0_F144_451E_9BC4_060E53FB268A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CServerLpackCtrl  
{
	private:
		enum {
			MaxLineNum=100,
			MaxBytes_per_Line=100,
		};

		enum enumRet{
			end_of_token=0,
			end_of_file=1,
			end_of_line=2,
		};

		BOOL m_bOK;
		char m_LineBuffer[MaxLineNum][MaxBytes_per_Line+1];
		static const char m_OverflowMessageLine[MaxBytes_per_Line+1];
		
	private:
		BOOL LoadFile(const char* szpFileName);
		BOOL Init(void);
	public:
		BOOL isOK(void) const;
		char* GetLpack(const unsigned int Idx);

	public:
		CServerLpackCtrl();
		virtual ~CServerLpackCtrl();

};

#endif 
