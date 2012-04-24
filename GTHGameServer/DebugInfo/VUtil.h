

#ifndef __VUTIL_H__
#define __VUTIL_H__


#ifdef __cplusplus
extern "C" {            
#endif	


#define		__MYDEBUG
#define		SHOW_LOG_SCREEN
#define		MT_CLIENT
#define		__SHOWLOG_INFO_SCREEN__

   extern void ShowLogInfo(char *pszFmtMsg, ...);

 
	extern __int64 g_qnTotalPC, g_qnFreq, g_qnCurrPC;

	void InitProfile();
	void ShowProfile();
	void StartProfile();
	void EndProfile(char* pMsg);

#ifdef __cplusplus
}

#endif	

#endif 
