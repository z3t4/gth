



#if !defined(AFX_AHNLAB_H__56B2DC40_CF48_4429_8A4F_830574D0063F__INCLUDED_)
#define AFX_AHNLAB_H__56B2DC40_CF48_4429_8A4F_830574D0063F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "AntiCpSvrFunc.h"
#include "AntiCpCnt.h"

class CAhnLab 
{
public:
	enum{
		SEND_DELAY			= 30 * 1000, 
		PROCESSNUMperFRAME	= 40,
		DisConnectTIme		= 15 * 1000,
		AhnLabMessageSendCount	= 8
	};	

	enum enumAhnLabPacket{
		CC_AHNLAB	= 0X0000,
		GSC_AHNLAB  = 0X0001
	};

	
protected:
	BOOL	m_bOK;	
	BYTE	*m_pBasePointer;
	DWORD	m_dmMemSize;
	FLOAT	m_NextSendTime;
	int		m_CheckPcIdx;

public:
	BOOL	IsOK();
	void	UpdateAhnLab();
	void	GTH_SendMessageAhnLabMessage(playerCharacter_t *pPlayer);
	void	GTH_ProcessMessageAhnLabMessage(playerCharacter_t *pPlayer);
	void	GTH_CheckReciveTIme(playerCharacter_t *pPlayer);	

public:
	CAhnLab(const char* filename);
	virtual ~CAhnLab();

};

#endif 
