
#ifndef		_CASHMALL_ITEM_H_
#define		_CASHMALL_ITEM_H_

#include <vector>



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


void GTH_ProcessMessageRequest_StoneOfSage();
void GTH_ProcessMessageRequest_StoneOfLucky();
void GTH_ProcessMessageRequest_StoneOfEachName();
void GTH_ProcessMessageRequest_ReverseOfMemory();
void GTH_ProcessMessageRequest_OptionItemEdit();
void GTH_ProcessMessageRequest_OptionItemWinClose();
void GTH_ProcessMessageRequest_OptionItemModify();


void GTH_SendMessageReply_StoneOfSage
(playerCharacter_t* pPc, BOOL Reply, int InvenPos, float DestroyRate =0.0f, float SuccecsRate = 0.0f);

void GTH_SendMessageReply_StoneOfLucky
(playerCharacter_t* pPc, BOOL Reply, int InvenPos, float DestroyRate =0.0f, float SuccecsRate = 0.0f);

void GTH_SendMessageReply_StoneOfEachName
(playerCharacter_t* pPc, BOOL Reply, int InvenPos);

void GTH_SendMessageReply_ReverseOfMemory
(playerCharacter_t* pPc, BOOL Reply, int ErrorIdx, 	
int InvenPos, vector<int>* pVSkill = NULL);

void GTH_SendMessageReply_OptionItemEdit
(playerCharacter_t* pPc,  BOOL bError,  
int MouseInvenPos, int EditInevenPos);

void GTH_SendMessageRePly_OptionItemWinClose
(playerCharacter_t* pPc, BOOL bError);


void GTH_SendMessageRePly_OptionItemModify
(playerCharacter_t* pPc, enum CashMallItem_OptionStone::enumUseItemMode, 
 BOOL Reply, item_t* pItem, int ErrorIdx = -1);




void InitCashMallItem(playerCharacter_t* pPc);
void ItemModify_Stone_of_sage();
void ItemModify_Stone_of_lucky();
void ItemModify_Stone_of_each_name();

int IsExistenceItemOption(item_t *pItem, int OptionIdx);
int	SelectOptionGrade(enum CashMallItem_OptionStone::enumUseItemMode mode);





#endif