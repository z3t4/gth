

#ifndef __VEMER_COMMON_H__
#define __VEMER_COMMON_H__

#include <windows.h>

#pragma once







#define chSTR2(x)   #x
#define chSTR(x)    chSTR2(x)
#define chMSG(desc) message(__FILE__ "(" chSTR(__LINE__) ") : message : " #desc)






#define chINRANGE(low, Num, High) (((low) <= (Num)) && ((Num) <= (High)))






#define chDIMOF(Array) (sizeof(Array) / sizeof(Array[0]))






#define chSIZEOFSTRING(psz)   ((lstrlen(psz) + 1) * sizeof(TCHAR))






template <class TV, class TM>
inline TV chROUNDDOWN(TV Value, TM Multiple) {
   return((Value / Multiple) * Multiple);
}



template <class TV, class TM>
inline TV chROUNDUP(TV Value, TM Multiple) {
   return(chROUNDDOWN(Value, Multiple) + 
      (((Value % Multiple) > 0) ? Multiple : 0));
}







#ifdef _X86_
#define DebugBreak()    _asm { int 3 }
#endif






#define MAKESOFTWAREEXCEPTION(Severity, Facility, Exception) \
   ((DWORD) ( \
     (Severity       ) |     \
     (1         << 29) |     \
     (0         << 28) |     \
     (Facility  << 16) |     \
     (Exception <<  0)))





inline void chMB(PCSTR s) {
   char szTMP[256];
   GetModuleFileNameA(NULL, szTMP, chDIMOF(szTMP));
   HWND hwnd = GetActiveWindow();


}





inline void chMBANDDEBUG(PSTR szMsg) {
   chMB(szMsg);
   DebugBreak();
}



inline void chASSERTFAIL(LPCSTR file, int line, PCSTR expr) {
   char sz[256];
   wsprintfA(sz, "File %s, line %d : %s", file, line, expr);
   chMBANDDEBUG(sz);
}



#ifdef _DEBUG
#define chASSERT(x) if (!(x)) chASSERTFAIL(__FILE__, __LINE__, #x)
#else
#define chASSERT(x)
#endif



#ifdef _DEBUG
#define chFAIL() chASSERTFAIL(__FILE__, __LINE__, "")
#else
#define chFAIL()
#endif



#ifdef _DEBUG
#define chVERIFY(x) chASSERT(x)
#else
#define chVERIFY(x) (x)
#endif








#define chHANDLE_DLGMSG(hwnd, message, fn)                 \
   case (message): return (SetDlgMsgResult(hwnd, uMsg,     \
      HANDLE_##message((hwnd), (wParam), (lParam), (fn))))






inline void chSETDLGICONS(HWND hwnd, int idi) {
   SendMessage(hwnd, WM_SETICON, TRUE,  (LPARAM) 
      LoadIcon((HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), 
         MAKEINTRESOURCE(idi)));
   SendMessage(hwnd, WM_SETICON, FALSE, (LPARAM) 
      LoadIcon((HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), 
      MAKEINTRESOURCE(idi)));
}




   



   
   extern bool InitLogWnd(HWND hParentWnd);



#endif 