



#if !defined(AFX_GOLRYUNBATTLE_H__038E6C79_A7B3_41B2_82D5_44EFC81B077A__INCLUDED_)
#define AFX_GOLRYUNBATTLE_H__038E6C79_A7B3_41B2_82D5_44EFC81B077A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 



#include "CGolryunBattle_Data.h"
#include "CGolryunBattle_Common.h"
#include "CGolryunBattle_Encoder.h"
#include "CGolryunBattle_Decoder.h"
#include "CGolryunBattle_Process.h"

#include "CGolryunBattle_DBV2.h"



class CGolryunBattle  
{
	private:
		CGolryunBattle_Data*    m_cpData;
		CGolryunBattle_Common*  m_cpCommon;
		CGolryunBattle_Encoder* m_cpEncoder;
		CGolryunBattle_Decoder* m_cpDecoder;
		CGolryunBattle_Process* m_cpProcess;
		
		CGolryunBattle_DBV2*    m_cpDBV2;
		BOOL		m_bOK;
	private:
		BOOL Init(void);
		BOOL LoadServerConfigFile(void);
	

	public:
		BOOL isOK(void) const;
		void GTH_ProcessGBBattleSys(void);
		CGolryunBattle_Data*	GetDataClassPointer(void) const;
		CGolryunBattle_Common*  GetCommonClassPointer(void) const;
		CGolryunBattle_Encoder* GetEncoderClassPointer(void) const;
		CGolryunBattle_Process* GetProcessClassPointer(void) const;
		CGolryunBattle_Decoder* GetDecoderClassPointer(void) const;
		
		CGolryunBattle_DBV2*    GetDBV2ClassPointer(void) const;



	public:
		CGolryunBattle();
		virtual ~CGolryunBattle();
};



extern void GTH_SendMessage_CompleteEffect_To_AllPC( playerCharacter_t *pc, int effectIdx );
extern void GTH_PC_ExpUp( int idx, __int64 &exp, __int64 &genExp, int &lootNSE, int &level, int &genLevel );

#endif 
