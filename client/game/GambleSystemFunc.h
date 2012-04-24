
#ifndef _GAMBLE_SYSTEM_H
#define _GAMBLE_SYSTEM_H


enum enumGambleSystem
{
	ACTIVE_SYSTEM_REQUEST	= 0X0000,	
	ACTIVE_SYSTEM_REPLY		= 0X0001,
	BUY_GAMBLE_ITEM_REQUEST	= 0X0002,
	BUY_GAMBLE_ITEM_REPLY	= 0X0003,
};


void	GambleSystemDecode();

void	GTH_SendMessage_GambleSystem_Active(BOOL bActive);


void	GTH_SendMessageRequest_BuyGambleItem(int SlotIdx, int Num = 1);
void	GTH_ReplytMessage_BuyGambleItem();

#endif