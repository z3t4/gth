



#if !defined(AFX_GAMBLESYSTEMFUNC_H__84C4D938_0B9E_4DF1_AD9D_C73BE44FB82A__INCLUDED_)
#define AFX_GAMBLESYSTEMFUNC_H__84C4D938_0B9E_4DF1_AD9D_C73BE44FB82A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


enum enumGambleSystem
{
	ACTIVE_SYSTEM_REQUEST	= 0X0000,	
	ACTIVE_SYSTEM_REPLY		= 0X0001,
	BUY_GAMBLE_ITEM_REQUEST	= 0X0002,
	BUY_GAMBLE_ITEM_REPLY	= 0X0003,
};


void	GambleSystemDecode();
void	GambleSystem_ActiveSystem_ProcessMessage();
void	BuyGambleItem_ProcessMessage();
void	BuyGambleItem_ReplyMessage(playerCharacter_t *pc, int reply, 
		int error, item_t *item, int storeItemIdx, int curNumber);
	


#endif 

