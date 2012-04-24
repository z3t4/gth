



#if !defined(AFX_ITEMTABLEDATA_H__F3A4BC8C_A73C_4E43_8CDD_2296573B32B0__INCLUDED_)
#define AFX_ITEMTABLEDATA_H__F3A4BC8C_A73C_4E43_8CDD_2296573B32B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CItemTableData  
{
public:
	

	
	
	enum ItemTable_FluxDataType
	{
		NONE_TYPE=0,		
		USE_ITEM_FOR_EPK_CHARGE=2,							
		USE_ITEM_FOR_EPK_MUCH_CHARGE=3,						
		USE_ITEM_FOR_REPAIR=4,								
		USE_ITEM_FOR_WEATHER_SNOW=5,						
		USE_ITEM_FOR_WEATHER_RAIN=6,						
		USE_ITEM_FOR_QUESTSCROLL=7,							
		USE_ITEM_FOR_AUTOQUESTSCROLL=8,						
		USE_ITEM_FOR_SURGERY=9,								
		USE_ITEM_FOR_CHAT=10,								
		USE_ITEM_FOR_RECYCLE=11,							
		USE_ITEM_FOR_SURGERY_MAXHAIRCOUNT_TYPE=12,			
		
		USE_ITEM_FOR_SUMMONCARD_ITEM		 = 13,			
		USE_ITEM_FOR_OPTION_MANAGE			 = 14,			
		USE_ITEM_FOR_AWAKEN					 = 15,			
		
	

		
		USE_ITEM_FOR_PC_SUMMON = 16,						
		USE_ITEM_FOR_PC_MOVE_SUMMON =17,							

		MAX_ITEM_TABLE_TYPE_DATA,
	};

	
	
	enum ItemTable_FluxDataValue
	{
		EPK_RATIO=101,										
		LPACK_TAB=102,										
		LPACK_INDEX=103,										
		RECYCLE_DURABILITY_MULTIPLY=104,						
		RECYCLE_LEVEL_MULTIPLY=105,							
		RECYCLE_LEVEL_EXP_MULTIPLY=106,
		RECYCLE_LEVEL_GENEXP_MULTIPLY=107,
		RECYCLE_PARAM_ITEM_FUNCTION=108,
		RECYCLE_LEVEL_EXP_AND_GENEXP_MULTIPLY=109,
		EPK_NOT_MORE_CHARGE_IS_DELETE=110,
		RECYCLE_RA_CHARGE=111,		
		RECYCLE_SA_CHARGE=112,		
		RECYCLE_GEN_CAPABLITY=113,	
		RECYCLE_DESTROYITEM=114,	
		RUN_SKILL_ITEM=115,			
		RUN_SKILL_INDEX=116,		
		RUN_SKILL_ADD_LEVEL=117,	
		
		OPTION_MANAGE_TYPE= 118,	
		

		
		EFFECT_ID =119,						

		MAX_ITEM_TABLE_FLOAT_DATA,
	};



	enum Surgery_HairItem_Type
	{
		SURGERY1=0,		
		SURGERY2,		
	};

public:
	float	GetData(const itemTable_t *_pItemTable,const int _Type);
	bool	IsType(const itemTable_t *_pItemTable,const int _Type);
	int		GetUseItemType(const itemTable_t *_pItemTable);
	float	GetData(const int _ItemTableIndex ,const int _Type);

public:
	CItemTableData();
	virtual ~CItemTableData();

};

#endif 
