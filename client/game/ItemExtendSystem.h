



#if !defined(AFX_ITEMEXTENDSYSTEM_H__3B20E8DC_0D76_4FA7_A15B_B4F85F20C419__INCLUDED_)
#define AFX_ITEMEXTENDSYSTEM_H__3B20E8DC_0D76_4FA7_A15B_B4F85F20C419__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 






void	SetItemSpawnType(item_t *pItem, ITEM_SPAWN_INFO::enumItemSpawnType type);

ITEM_SPAWN_INFO::enumItemSpawnType	
		GetItemSpawnType(const item_t *pItem);

void	SetItemUseFlag(item_t *pItem, ENUM_ITEM_USE_FLAG flag);
BOOL	IsItemUseFlag(item_t *pItem, ENUM_ITEM_USE_FLAG flag);




#endif 
