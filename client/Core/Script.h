



#if !defined(AFX_SCRIPT_H__6A3BE47E_7231_49B1_B9CC_C88DF5B89578__INCLUDED_)
#define AFX_SCRIPT_H__6A3BE47E_7231_49B1_B9CC_C88DF5B89578__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <windows.h>
#include "core_def.h"

class CScript  
{
public:
	char m_name[ GTH_MAX_SIZE_FILENAME ];
	char m_token[ GTH_MAX_SIZE_TOKEN ];

	         CScript();
	virtual ~CScript();

	void  Reset();
	int   MatchToken( char *match );
	void  SkipBracedSection();
	int   GetCurrParseLine();
	void  BeginParse();
	char *GetName();

	void  SkipLine();
	int   SkipToken();
	int   SetOffset( int offset );
	char *GetCurrToken();
	char *GetNextToken();
	char *GetNextToken(int allowNewline);
	int   Load( char *fName );
	int   Load( char *fName , DWORD size , BYTE *data );

	
	
	int	  GetCSVToken( char token[] );

protected:
	BYTE  *m_buffer;
	int    m_lines;
	
	BYTE  *m_prevOffset;
	BYTE  *m_baseOffset;
	BYTE  *m_currOffset;
	DWORD  m_bufferSize;

	int   SkipSpace();
};

extern CScript g_script;

#endif 
