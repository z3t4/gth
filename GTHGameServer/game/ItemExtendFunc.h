

#ifndef _ITEMEXTEND_FUNC_H
#define _ITEMEXTEND_FUNC_H



enum enumItemExtendSystemPacket
{
	
	REQUEST_ITEM_SYNC_TIME	= 0X0000,	
	REPLY_ITEM_SYNC_TIME	= 0X0001,	

	
	REQUEST_ITEM_TIME_SET   = 0X0002,		
	REPLY_ITEM_TIME_SET		= 0X0003,		
	

};



struct sItemUseTimeInfo
{
	enum enumItemSyncInfo
	{
		
		ITEM_TIMESET_REPLACE_ITEMTABLEIDX =	662,
		ITEM_USETIME_SYNC_TERM	= 300000 
			
	};
	
	
	enum enumInventoryType
	{
		ITEM_INVEN			= 0,
		ITEM_UPGRADE,			
		ITEM_EQUIP,				
		ITEM_GB_BATTING,		
		ITEM_PRECOCITY,			
		ITEM_CRAFT,				
		ITEM_DEPOT,				
		ITEM_BOOTHINVENTORY,	
		ITEM_MOUSEIN,			
		ITEM_UNKNOW,			
		
		MAX_NUM_OF_INVENTYPE,
	};	
	enum enumItemExptendError
	{
		ITEM_EXTEND_ERROR1,
		ITEM_EXTEND_ERROR2,
		ITEM_EXTEND_ERROR3,
		ITEM_EXTEND_ERROR4,
		ITEM_EXTEND_ERROR5,
		ITEM_EXTEND_ERROR6,
		ITEM_EXTEND_ERROR7,
	};	
};


void	GTH_ItemExtendSystemDecode();


void	GTH_ProcessMessageRequestItemTimeSet();


void	GTH_ProcessMessageRequestItemSyncTIme();


void	GTH_SendMessage_ItemSyncTime(playerCharacter_t* pPc);


void	GTH_SendMessage_ItemTimeSet(playerCharacter_t* pPc, int Reply, int iItemIdx = -1, int ErrorIdx = -1,
									item_t *pReplaceItem =NULL, 
									enum sItemUseTimeInfo::enumInventoryType type =
									sItemUseTimeInfo::enumInventoryType::ITEM_INVEN, 
									int InvenIdx = 0);



void	GTH_ItemUseTimeProcess(playerCharacter_t* pPc);


BOOL	IsInvalideItem_FromInvenType(playerCharacter_t* pPc, int InvenType, int iInvenPos, int ItemIdx);


void	GTH_ProcessItemSync(playerCharacter_t *pPc);


void	GTH_CollectItemItem(playerCharacter_t *pPc);


void	GTH_DBSaveItemForTimeType(playerCharacter_t *pPc);


BOOL	GTH_ReplaceItem(playerCharacter_t *pPc, enum sItemUseTimeInfo::enumInventoryType type, int iInvenPos, int NewItemIdx = -1);

#endif


