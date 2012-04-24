



#if !defined(AFX_CHECKSUM_H__25AE8BA5_981D_4BF5_B9FA_CB7B5259ECE3__INCLUDED_)
#define AFX_CHECKSUM_H__25AE8BA5_981D_4BF5_B9FA_CB7B5259ECE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <windows.h>
class checksum {
public:
    checksum() { clear(); }
    void clear() { sum = 0; r = 55665; c1 = 52845; c2 = 22719;}
    void add(DWORD w);
    void add(BOOL w) { add((DWORD)w); }
    void add(UINT w) { add((DWORD)w); }
    void add(WORD w);
    
    void add(LPBYTE b, UINT length);
    void add(BYTE b);
    DWORD get() { return sum; }
protected:
    WORD r;
    WORD c1;
    WORD c2;
    DWORD sum;
};

#endif 
