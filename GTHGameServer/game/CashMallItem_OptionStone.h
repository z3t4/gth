
#ifndef		_CASHMALL_ITEM_OPTION_STONE_H_
#define		_CASHMALL_ITEM_OPTION_STONE_H_



struct CashMallItem_OptionStone
{
	enum	enumCashMall_ItemIdx
	{
			STONE_OF_SAGE_IDX		= 862,
			STONE_OF_LUCKY_IDX		= 863,
			STONE_OF_EACH_NAME_IDX	= 861,
			REVERSE_OF_MEMORY_IDX	= 480,
			MAX_NUMBER_OF_STONE		= 3
	};
	
	enum enumUseItemMode
	{
		STONE_OF_SAGE,
		STONE_OF_LUCKY,
		STONE_OF_EACH_NAME,

		NONE_ITEM,
	};
	
	struct CashMallItemLogic
	{			
		float DestroyRate;		
		float SuccecsRate;
		float OptionGrade[MAX_NUMBER_OF_OPTION_VALUE_GRADE];
	};

	enum enumUseItemMode	m_iUsingItemMode;
	int						m_iEditOptionItemIdx;
	int						m_iUsingItemInvenIdx;
	int						m_iSortOptionIdx[MAX_NUMBER_OF_ITEM_OPTION];
	int						m_iSortOptionValue[MAX_NUMBER_OF_ITEM_OPTION];


	CashMallItem_OptionStone()
	{		
		InitCashMallItem_OptionStone();
	}
	~CashMallItem_OptionStone(){};

	void InitCashMallItem_OptionStone()
	{
		m_iEditOptionItemIdx = -1;
		m_iUsingItemInvenIdx = -1;
		m_iUsingItemMode	 = NONE_ITEM;
		memset( m_iSortOptionIdx, -1, MAX_NUMBER_OF_ITEM_OPTION );
		memset( m_iSortOptionValue, -1, MAX_NUMBER_OF_ITEM_OPTION );
	}
};

#endif
