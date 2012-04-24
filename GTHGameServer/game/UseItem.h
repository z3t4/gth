



#if !defined(AFX_USEITEM_H__34296A00_0E39_4F37_A5E5_A95C7017A68A__INCLUDED_)
#define AFX_USEITEM_H__34296A00_0E39_4F37_A5E5_A95C7017A68A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CUseItem  
{
public:
	CUseItem();
	virtual ~CUseItem();


	int		RecyleItem(item_t *item,int &raCount,int &saCount,int &reply,int &error,int &itemIdx,int invenPos);
	int		AutoQuestItemType(item_t *item,int &invenPos,int &raCount,int &saCount,int &bIsStack,int &stackNumber,int &reply,int &error,int &itemIdx);
	int		QuestScrollItem(item_t *item,int &invenPos,int &reply,int &error,int &itemIdx);
	int		AutoQuestScrollItem(item_t *item,int &invenPos,int &reply,int &error,int &itemIdx);
	int		MonsterCardItem(item_t *item,int &invenPos,int &reply,int &error,int &itemIdx);
	int		DefaultUsetItem(item_t *item,int &invenPos,int &reply,int &error,int &itemIdx,int &raCount,int &saCount);
};

#endif 
