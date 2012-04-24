// CTerraCtrl.cpp: implementation of the CTerraCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "../global.h"
#include "CTerraCtrl.h"




//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//
CTerraCtrl::CTerraCtrl()
{
	m_bOK=FALSE;	


	
	CoInitialize(NULL);

	HRESULT hr = m_pEGameEncrypto.CreateInstance(__uuidof(EGameEncrypt));

	
	
	//caution hr의 값이 정상적인경우 NULL 이 return 됩니다.
	//if(NULL == hr) return;	


	/*
	//받드시 아래와 같은 방법으로 셋업하여야만 정상적인 실행이 가능합니다. 
	
	  1.EGameEncrypt.dll 파일을 다음의 디렉토리에 복사하여야 합니다.
		c:\windows\system (if windows 95/98/Me)
		c:\winnt\system32 (if windows NT/2000)	
		c:\windows\system32 (if windows XP)
	  2. command prompt 'cd' to the %system% path, and type
	    c:\windows\system32\regsvr32 EGameEncrypt.dll
	  3. dll파일을 정상적으로 사용하시려면 컴퓨터를 리스타트 하여야만 합니다.

	*/





	m_bOK=TRUE;
}


//------------------------------------------------------------------//
//
//------------------------------------------------------------------//
CTerraCtrl::~CTerraCtrl()
{
	m_pEGameEncrypto=NULL;
	CoUninitialize();

}

//------------------------------------------------------------------//
//
//------------------------------------------------------------------//
BOOL CTerraCtrl::Encrypt(const unsigned char* pTIDStr,unsigned char* pEncryptStr,const int EncryptStrLength) const
{
	_bstr_t bstrSend = (char*)pTIDStr;

	WCHAR* cDecoded;

	// It is not being decoded properly
	if(FAILED(m_pEGameEncrypto->Decode(bstrSend,&cDecoded))){// &cDecoded))){
		//MessageBox(HWND_DESKTOP, "Not decoded properly", "Decoding", MB_OK);
		return FALSE;
	}
	_bstr_t hoge_copy(cDecoded, true);

	int len=hoge_copy.length();
	if(len > EncryptStrLength) len=EncryptStrLength;

	memcpy(pEncryptStr,(char*)hoge_copy,len);
	pEncryptStr[len]=NULL;

	return TRUE;

}



//------------------------------------------------------------------//
//
//------------------------------------------------------------------//
BOOL CTerraCtrl::isOK(void) const
{
	return m_bOK; 
}





