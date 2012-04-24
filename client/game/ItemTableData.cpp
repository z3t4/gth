


#include "../global_def.h"


CItemTableData gItemTableData;





CItemTableData::CItemTableData()
{

}

CItemTableData::~CItemTableData()
{

}

float	CItemTableData::GetData(const int _ItemTableIndex ,const int _Type)
{
	if(_ItemTableIndex < 0 && _ItemTableIndex >= MAX_NUMBER_OF_ITEM_TABLE)
	{
		return 0.0f;
	}

	itemTable_t * pItemTable = &g_itemTable[_ItemTableIndex];

	for(int i=0;i<itemTable_t::MAX_ITEM_TABLE_FLOAT_DATA;i++)
	{
		if(0 == pItemTable->iFluxDataIndex[i]) break;
		if(pItemTable->iFluxDataIndex[i] == _Type)
		{
			return pItemTable->fFluxData[i];
		}
	}

	return 0.0f;
}


float	CItemTableData::GetData(const itemTable_t *_pItemTable,const int _Type)
{
	for(int i=0;i<itemTable_t::MAX_ITEM_TABLE_FLOAT_DATA;i++)
	{
		if(0 == _pItemTable->iFluxDataIndex[i]) break;
		if(_pItemTable->iFluxDataIndex[i] == _Type)
		{
			return _pItemTable->fFluxData[i];
		}
	}

	return 0.0f;
}

bool	CItemTableData::IsType(const itemTable_t *_pItemTable,const int _Type)
{
	for(int i=0;i<itemTable_t::MAX_ITEM_TABLE_FLOAT_DATA;i++)
	{
		if(0 == _pItemTable->iFluxDataIndex[i]) break;

		if(_pItemTable->iFluxDataIndex[i] == _Type)
		{
			return true;
		}
	}

	return false;
}


int		CItemTableData::GetUseItemType(const itemTable_t *_pItemTable)
{
	for(int i=0;i<itemTable_t::MAX_ITEM_TABLE_FLOAT_DATA;i++)
	{
		if(0 == _pItemTable->iFluxDataIndex[i]) break;

		if(0 < _pItemTable->iFluxDataIndex[i] && 100 > _pItemTable->iFluxDataIndex[i])
			return _pItemTable->iFluxDataIndex[i];
	}

	return -1;
}
