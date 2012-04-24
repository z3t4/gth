


















#if !defined(AFX_IM_H__6887B165_972D_4D17_8A75_FE07930CE59C__INCLUDED_) 
#define AFX_IM_H__6887B165_972D_4D17_8A75_FE07930CE59C__INCLUDED_ 

#if _MSC_VER > 1000 
#pragma once 
#endif 

#define _CIM_MAXINPUTCHARNUMBER 24 
#pragma comment ( lib, "imm32.lib" )
#include <windows.h>


class CIM 
{ 
private: 

LPCANDIDATELIST m_lpCandList; 
LPSTR m_lpszImeInput; 
LPSTR m_lpszCurInputLanguageDesc; 
char m_cCandidateList[255]; 
char m_cInput[64]; 

BOOL CandidateToString( LPCANDIDATELIST lpCandidateList ); 

public: 

CIM(); 
virtual ~CIM(); 

LPSTR GetResultString( void ); 

void UpdateShow( ); 

LPSTR GetCurInputLanguageDesc( ); 
void ClearCandidateList( void ); 
BOOL GetCandidateList( HWND hWnd ); 
BOOL ImeIsOpen( void ); 
void OnChar( TCHAR ch ); 
void OnImeNotify( HWND hWnd,WPARAM wParam ); 
void OnImeComposition( HWND hWnd, LPARAM lParam ); 
}; 

#endif 



