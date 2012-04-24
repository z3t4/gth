// CTerraCtrl.h: interface for the CTerraCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTERRACTRL_H__64C0883E_1487_4603_BD88_B78568152D24__INCLUDED_)
#define AFX_CTERRACTRL_H__64C0883E_1487_4603_BD88_B78568152D24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include <comutil.h>
#include <stdio.h>
#import "EGameEncrypt.dll" no_namespace


class CTerraCtrl  
{

	private:
		BOOL m_bOK;
		IEGameEncryptPtr m_pEGameEncrypto;

	public:
		BOOL isOK(void) const;
		BOOL Encrypt(const unsigned char* pTIDStr,unsigned char* pEncryptStr,const int EncryptStrLength) const;



	public:
		CTerraCtrl();
		virtual ~CTerraCtrl();

};

#endif // !defined(AFX_CTERRACTRL_H__64C0883E_1487_4603_BD88_B78568152D24__INCLUDED_)
