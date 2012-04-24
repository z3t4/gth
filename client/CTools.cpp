




#include "global_def.h"
#include "CTools.h"


CTools gcTools;




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




void CTools::Replace_singleQUOTATIONmark_by_doubleQUOTATIONmark(char* ntspString)
{
	for(char* pPos=ntspString;NULL != *pPos; pPos++){
		if(*pPos == '\'') *pPos='\"';
	}
}

void CTools::Replace_doubleQUOTATIONmark_by_singleQUOTATIONmark(char* ntspString)
{
	for(char* pPos=ntspString;NULL != *pPos; pPos++){
		if(*pPos == '\"') *pPos='\'';
	}
}



int	 CTools::GetEpkChargeCount(item_t *pItem)							
{
	return pItem->optionValue[item_t::ITEM_T_CHARGE_VALUE];
}




void  CTools::SetEpkChargeCount(item_t *pItem,int count)				
{
	pItem->optionIdx[item_t::ITEM_T_CHARGE_VALUE] = -1;
	pItem->optionValue[item_t::ITEM_T_CHARGE_VALUE] = count;
}     



Fx_CHARACTER_t *  CTools::FindCharacter(const int EntityType,const int IdxOnServer)
{
	Fx_CHARACTER_t *character		= &g_charManager->m_Characters[0];
	int numCharacter				=g_charManager->m_iNumCharacters;

	
	for (int i=0; i < numCharacter; i++, character++)
	{
		if (EntityType == character->entityType && IdxOnServer == character->idxOnServer)
		{
			return character;
		}
	}

	return NULL;
}



bool	CTools::IfMyCharacter(const Fx_CHARACTER_t *character)
{
	if( character->idxOnServer == g_cgv.myPCIdxOnServer && 
		character->entityType == ENTITY_PC ) 
			return true;

	return false;
}



bool CTools::IfSkillCasting_NotItemUse()
{
	if(g_cgv.myCharacterInfo->skillCastingtime > 0)
	{
		if(g_cgv.saveInventoryIdx >= 0 && g_cgv.saveInventoryIdx < MAX_INVENTORY_SIZE)
		{
			int itemIdx = g_cgv.myCharacterInfo->inventory[g_cgv.saveInventoryIdx];
			if(itemIdx >= 0 && itemIdx < MAX_NUMBER_OF_OWNITEM)
			{
				int itemTableIdx;
				item_t *item=NULL;
				item = &g_cgv.myCharacterInfo->item[itemIdx];
				itemTableIdx = item->itemTableIdx;

				if(itemTableIdx >=0 && itemTableIdx < MAX_NUMBER_OF_ITEM_TABLE)
				{
					char szTemp[256];
					sprintf( szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 200 ),g_itemTable[itemTableIdx].name );
					g_ifMng->AddSysMessage( szTemp );
				}
			}
		}				
		return true;
	}
	return false;
}



bool CTools::DeleteInventoryItem(int invenPos)
{
	item_t		*item	= NULL;

	if ( 0 > invenPos || MAX_INVENTORY_SIZE <= invenPos) return false;
	
	int itemIdx		= g_cgv.myCharacterInfo->inventory[invenPos];

	if ( 0 > itemIdx || MAX_NUMBER_OF_OWNITEM <= itemIdx )	return false;
	
	item		= &g_cgv.myCharacterInfo->item[itemIdx];
	if ( item == NULL)	return false;
	
	
	if( item->durability > 0 )	
	{
		item->durability --;	
	}
	
	else	
	{			
		GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
		g_cgv.myCharacterInfo->inventory[invenPos] = -1;
	}
	return true;
}




void CTools::CcToGs(int sysType,int mainType,int subType)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);

		MSG_WriteShort(&netMessage, sysType);
		MSG_WriteShort(&netMessage, mainType);		
		MSG_WriteShort(&netMessage, subType);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CTools::CcToGs(int sysType,int mainType,int subType,int val)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);

		MSG_WriteShort(&netMessage, sysType);     
		MSG_WriteShort(&netMessage, mainType);		
		MSG_WriteShort(&netMessage, subType);

		MSG_WriteLong(&netMessage, val);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void CTools::CcToGs(int sysType,int mainType,int subType,char *TargetPlayerName)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);

		MSG_WriteShort(&netMessage, sysType);     
		MSG_WriteShort(&netMessage, mainType);
		MSG_WriteShort(&netMessage, subType);

		MSG_WriteString(&netMessage,TargetPlayerName);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);	

}


void CTools::CcToGs(int sysType,int mainType,int subType,int val, char *TargetPlayerName)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);

		MSG_WriteShort(&netMessage, sysType);     
		MSG_WriteShort(&netMessage, mainType);
		MSG_WriteShort(&netMessage, subType);

		MSG_WriteLong(&netMessage, val);

		MSG_WriteString(&netMessage,TargetPlayerName);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);	

}

	
void CTools::CcToGs(int sysType,int mainType,int subType, char *RequestPlayerName, char *TargetPlayerName)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);

		MSG_WriteShort(&netMessage, sysType);     
		MSG_WriteShort(&netMessage, mainType);
		MSG_WriteShort(&netMessage, subType);

		MSG_WriteString(&netMessage,RequestPlayerName);
		MSG_WriteString(&netMessage,TargetPlayerName);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);	

}



void CTools::CcToGs(int sysType,int mainType,int subType,int worldIdx,vec3_t position, char *TargetPlayerName)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);

		MSG_WriteShort(&netMessage, sysType);     
		MSG_WriteShort(&netMessage, mainType);
		MSG_WriteShort(&netMessage, subType);

		MSG_WriteByte(&netMessage,		worldIdx);
		MSG_WritePosition(&netMessage,  position);

		MSG_WriteString(&netMessage,TargetPlayerName);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);	

}


	
void CTools::CcToGs(int sysType,int mainType,int subType, int worldIdx, vec3_t position, char *RequestPlayerName, char *TargetPlayerName)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);

		MSG_WriteShort(&netMessage, sysType);     
		MSG_WriteShort(&netMessage, mainType);
		MSG_WriteShort(&netMessage, subType);

		MSG_WriteByte(&netMessage,		worldIdx);
		MSG_WritePosition(&netMessage,  position);

		MSG_WriteString(&netMessage,RequestPlayerName);
		MSG_WriteString(&netMessage,TargetPlayerName);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);	

}