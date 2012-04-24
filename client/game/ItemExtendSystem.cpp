




#include "../global_def.h"









void	SetItemSpawnType(item_t *pItem, ITEM_SPAWN_INFO::enumItemSpawnType type)
{
	
	pItem->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType = type;
}








ITEM_SPAWN_INFO::enumItemSpawnType	GetItemSpawnType(const item_t *pItem)
{	
	return pItem->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType;	
}








void	SetItemUseFlag(item_t *pItem, ENUM_ITEM_USE_FLAG flag)
{
	pItem->ItemExtendInfo.UseFlag |= flag;
}







BOOL	IsItemUseFlag(item_t *pItem, ENUM_ITEM_USE_FLAG flag)
{
	if ( pItem->ItemExtendInfo.UseFlag & flag)	
		return TRUE;

	return FALSE;
}






