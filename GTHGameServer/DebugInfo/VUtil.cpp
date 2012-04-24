



#include <windows.h>
#include "VUtil.h"

__int64 g_qnTotalPC, g_qnFreq, g_qnCurrPC;

void ShowLogInfo(char *pszFmtMsg, ...);

void ShowLogInfo(char *pszFmtMsg, ...)
{
    DWORD       dwTime; 
    va_list     valArg;
    char        szTime[60];
    char        szMsg[8192];
    DWORD       dwThreadID = GetCurrentThreadId();

    dwTime = GetTickCount(); 
    wsprintf(szTime, "[info] thrd=%d ", dwThreadID);
    

    va_start(valArg, pszFmtMsg);
	wvsprintf( szMsg, pszFmtMsg, valArg );
	va_end(valArg);

#ifdef __MYDEBUG
   #ifdef SHOW_LOG_SCREEN
      #ifdef MT_CLIENT

         OutputDebugString(szTime);
         OutputDebugString(szMsg);
         OutputDebugString("\r\n");
      #else
         
         printf(szTime);
         printf(szMsg);
         printf("\r\n");
         
      #endif 
   #endif
#endif

}



void InitProfile()
{
    QueryPerformanceFrequency( (LARGE_INTEGER*) &g_qnFreq); 
	QueryPerformanceCounter((LARGE_INTEGER*) &g_qnCurrPC);
	g_qnTotalPC = 0;

}

void ShowProfile()
{
	double dDiff, dFreq;

	dDiff = (double)g_qnTotalPC;
	dFreq = (double)g_qnFreq;

	dDiff = dDiff/g_qnFreq;
  
   


}

void StartProfile()
{
	QueryPerformanceCounter((LARGE_INTEGER*) &g_qnCurrPC);
}

void EndProfile(char* pMsg)
{
	__int64 qnPC;
	int nDiffPC; 
	float fDiff, fFreq;

	QueryPerformanceCounter((LARGE_INTEGER*)&qnPC);
	nDiffPC = (int)(qnPC - g_qnCurrPC);

	g_qnTotalPC += nDiffPC;

	fDiff = (float)nDiffPC;
	fFreq = (float)g_qnFreq;

	fDiff = fDiff/fFreq;


}