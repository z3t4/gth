



#include "global.h"
#include "datastruct.h"
#include "CTools.h"




CTools::CTools()
{
	m_bOK=FALSE;

	m_bOK=TRUE;

}





CTools::~CTools()
{

}





BOOL CTools::isOK(void) const
{
	return m_bOK;
}





i3client_t* CTools::GetClientRecord(const int Idx)
{
	if(Idx < 0) return NULL;
	if(Idx > MAX_CLIENTS) return NULL;
	if(!g_clients[Idx].active) return NULL;
	return &g_clients[Idx];
}



characterTable_t* CTools::GetCharactorGenTableRecord(const int GenIdx)
{
	if(GenIdx < 0) return NULL;
	if(GenIdx >= NUM_TABLE_GEN_TYPE) return NULL;

	return &g_characterTable[GenIdx];
}





characterData_t*  CTools::
GetCharactorWithCharacterID(const i3client_t* pClient,const int characterID)
{
	if(characterID < 0) return NULL;
	if(characterID >= MAX_CHARACTER_DATA) return NULL;	

	
	
	
	for(int idx=0; idx < MAX_CHARACTER_DATA; idx++){
		if(g_curClient->data[idx].characterID == characterID)
			return &g_curClient->data[idx];
	}

	return NULL;

}







BOOL CTools::IsItemUseFlag(item_t *pItem, ENUM_ITEM_USE_FLAG flag)
{
	if ( pItem->ItemExtendInfo.UseFlag & flag)	
		return TRUE;

	return FALSE;
}