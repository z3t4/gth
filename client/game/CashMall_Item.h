
#ifndef		_CASHMALL_ITEM_H_
#define		_CASHMALL_ITEM_H_




enum enumPacketCashMallItem
{
	
	REQUEST_USE_STONE_OF_SAGE		= 0x0000,
	REPLY_USE_STONE_OF_SAGE			= 0x0001,

	
	REQUEST_USE_STONE_OF_LUCKY		= 0x0002,
	REPLY_USE_STONE_OF_LUCKY		= 0x0003,

	
	REQUEST_USE_STONE_OF_EACH_NAME	= 0x0004,
	REPLY_USE_STONE_OF_EACH_NAME	= 0x0005,

	
	REQUEST_USE_REVERSE_OF_MEMORY	= 0x0006,
	REPLY_USE_REVERSE_OF_MEMORY		= 0x0007,

	
	REQUEST_OPTION_ITEM_EDIT		= 0x0008,
	REPLY_OPTION_ITEM_EDIT			= 0x0009,

	
	REQUEST_OPTION_ITEM_WINCLOSE	= 0x000a,
	REPLY_OPTION_ITEM_WINCLOSE		= 0x000b,	

	
	REQUEST_OPTION_ITEM_MODIFY		= 0x000c,
	REPLY_OPTION_ITEM_MODIFY		= 0x000d,	
	
};

enum enumCashMallItem_Error
{
	CASHMALL_ITEM_ERROR_0,
	CASHMALL_ITEM_ERROR_1,
	CASHMALL_ITEM_ERROR_2,
	CASHMALL_ITEM_ERROR_3,
	CASHMALL_ITEM_ERROR_4,
	CASHMALL_ITEM_ERROR_5,
	CASHMALL_ITEM_ERROR_6,		
	CASHMALL_ITEM_ERROR_7,		
};


void GTH_DecodeCashMallItem();

void GTH_ProcessMessageReply_StoneOfSage();
void GTH_ProcessMessageReply_StoneOfLucky();
void GTH_ProcessMessageReply_StoneOfEachName();
void GTH_ProcessMessageReply_ReverseOfMemory();
void GTH_ProcessMessageReply_OptionItemEdit();
void GTH_ProcessMessageReply_OptionItemWinClose();
void GTH_ProcessMessageReply_ItemModify();


void GTH_SendMessageRequest_StoneOfSage(const int InvenPos);
void GTH_SendMessageRequest_StoneOfLucky(const int InvenPos);
void GTH_SendMessageRequest_StoneOfEachName(const int InvenPos);
void GTH_SendMessageRequest_ReverseOfMemory(const int InvenPos);
void GTH_SendMessageRequest_OptionItemEdit();
void GTH_SendMessageRequest_OptionItemWinClose();

void GTH_SendMessageRequest_ItemModify (CashMallItem_OptionStone::enumUseItemMode mode, 
										int SelectOptionIdx = -1);
void GTH_ItemModify_DestroyItem();


void GTH_ItemModify_FailedItem(void);


void GTH_ProcessMessageReply_ItemModify_Stone_Of_Sage();
void GTH_ProcessMessageReply_ItemModify_Stone_Of_lucky();
void GTH_ProcessMessageReply_ItemModify_Stone_Of_EachName();





#endif