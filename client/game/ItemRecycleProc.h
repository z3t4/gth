



#if !defined(AFX_ITEMRECYCLEPROC_H__636A537B_5ADD_4274_B5E6_5AE6EA03FB32__INCLUDED_)
#define AFX_ITEMRECYCLEPROC_H__636A537B_5ADD_4274_B5E6_5AE6EA03FB32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CItemRecycleProc  
{
public:
	CItemRecycleProc();
	virtual ~CItemRecycleProc();

public:
	int			RecycleProc(item_t *_item);
};

#endif 
