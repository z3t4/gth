



#if !defined(AFX_ITEMEXTENDSYSTEMFUNC_H__F4DAEA80_4AB2_4D38_A3BF_25A52D2CB089__INCLUDED_)
#define AFX_ITEMEXTENDSYSTEMFUNC_H__F4DAEA80_4AB2_4D38_A3BF_25A52D2CB089__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

enum enumItemExtendSystemPacket
{
	
	REQUEST_ITEM_SYNC_TIME	= 0X0000,	
	REPLY_ITEM_SYNC_TIME	= 0X0001,	

	
	REQUEST_ITEM_TIME_SET   = 0X0002,		
	REPLY_ITEM_TIME_SET		= 0X0003,		
	

};



struct sItemUseTimeInfo
{
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
	
	enum enumItemSyncInfo
	{	
		ITEM_TIMESET_REPLACE_ITEMTABLEIDX =	662,	
		ITEM_USETIME_SYNC_TERM			  = 300000,	
		ITEM_DESTROY_NOTICE_5MIN		  = 300,	
		ITEM_DESTROY_NOTICE_1MIN		  = 60,		

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


void	GTH_SendMessageRequestItemTimeSet(const enum sItemUseTimeInfo::enumInventoryType type, int iItemIdx, int iinvenIdx);


void	GTH_SendMessageRequestItemSyncItem();


void	GTH_ProcessMessage_ItemSyncTime();


void	GTH_ProcessItemTimeSet(const int nItemIdx);


void	GTH_ProcessMessageReplyItemTimeSet();


BOOL	GTH_ReplaceItem(const enum sItemUseTimeInfo::enumInventoryType type, int iInvenPos, int NewItemIdx);


enum sItemUseTimeInfo::enumInventoryType	
		GetItemInvenType(const int nItemIdx, int &iInvenToryPos);



BOOL	iSTickPerSec();


void	GTH_ItemUseTimeProcess();


void	ItemDestroyNotice(char* szmsg);


#endif 

