



#if !defined(AFX_ITEMUSEPROC_H__38065D89_2C16_4E42_A76E_DB9D72159A7B__INCLUDED_)
#define AFX_ITEMUSEPROC_H__38065D89_2C16_4E42_A76E_DB9D72159A7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CItemUseProc  
{
public:
	CItemUseProc();
	virtual ~CItemUseProc();

	int			RepairItem(const item_t *_item,const int _Pos);
	int			EpkItem(const item_t *_item,const int _Pos);
	int			WeatherSnowItem(const item_t *_item,const int _Pos);
	int			WeatherRainItem(const item_t *_item,const int _Pos);

	int			UseProc(const item_t *_item,const int _Pos);
	int			EtcItemTypeWorldPortal(const item_t *_item,const int _Pos);

	int			EtcItemTypeSkillSphere(const item_t *_item,const int _Pos);
	int			EtcItemTypePortal(const item_t *_item,const int _Pos);

	int			EtcItemTypeMonsterSphere(const item_t *_item,const int _Pos);
	
	int			QuestScroll(const item_t *_item,const int _Pos);
	int			AutoQuestScroll(const item_t *_item,const int _Pos);
	int			SurgeryItem(const item_t *_item,const int _Pos);
	int			ChatItem(const item_t *_item,const int _Pos);
	int			EtcItemTypeNone(const item_t *_item,const int _Pos);
	int			EtcItemTypeAutoQuestItem(const item_t *_item,const int _Pos);
	int			UseEtcTypeProc(const item_t *_item,const int _Pos);

	int			EpkMuchItem(const item_t *_item,const int _Pos);

	
	int			PcSummonItem(const item_t *_item,const int _Pos,const int _type);
	int			MessageProcessOk(int _msgType);
	int			UseItemMessage(const item_t *_item,const int _Pos);


};

#endif 
