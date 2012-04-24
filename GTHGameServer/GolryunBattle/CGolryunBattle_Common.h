



#if !defined(AFX_CGOLRYUNBATTLE_COMMON_H__00634699_23E3_4965_A6C4_2E6C12227C4C__INCLUDED_)
#define AFX_CGOLRYUNBATTLE_COMMON_H__00634699_23E3_4965_A6C4_2E6C12227C4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "CGolryunBattle_Data.h"
class CGolryunBattle_Common  
{
	private:
		BOOL m_bOK;

	public:
		BOOL isOK(void) const;

	public:
		static playerCharacter_t* GetArenaPlayer(const int Idx);
		static playerCharacter_t* GetArenaPlayer(const CGolryunBattle_Data::tagFighter& Fighter);
		static playerCharacter_t* GetPlayer(const CGolryunBattle_Data::tagFighter& Fighter);
		static void Change_Inventory_Nak(const __int64 i64ParamNak,playerCharacter_t* pPc);
		static itemTable_t* GetItemTablePointer(const int Idx);

	public:
		CGolryunBattle_Common();
		virtual ~CGolryunBattle_Common();

};

#endif 
