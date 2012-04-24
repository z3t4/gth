



#include <VECTOR>
#include <algorithm>
#include "global.h"







CEntityListMng::CEntityListMng()
{
	ZeroMemory ( m_entityPCList, sizeof ( m_entityPCList ) );

	for( int i = 0; i < MAX_NUMBER_OF_ENTITY_PCLIST; ++i )
	{
		m_entityPCList[i].idx = -1;
	}

	
	m_vEntityPCList.reserve (MAX_NUMBER_OF_ENTITY_PCLIST) ;
}

CEntityListMng::~CEntityListMng()
{
	m_vEntityPCList.clear ();
}

void CEntityListMng::MakePCList( playerCharacter_t *pc, entityPCList_t *pl )
{
	ZeroMemory ( pl, sizeof ( entityPCList_t ) );

	pl->idx = 0;
	pl->serverIdx = g_config.gameServerNo;

	pl->idxOnServer = pc->idx;
	pl->serverGroupID = pc->serverGroupID;
	pl->characterID = pc->characterID;
	
	strncpy( pl->userID, pc->userID, NAMESTRING );
	strncpy( pl->name, pc->name, NAMESTRING );
	
	pl->worldIdx = pc->worldIdx;
	pl->job		 = pc->pcJob;
	pl->level	 = pc->level;


}

int CEntityListMng::FindPCList( entityPCList_t *pl )
{
	if ( !g_config.isManager || pl->userID[0] == 0 ) return -1;

	VECTOR_ENTITYPCLIST_STRUCT::iterator itor;
	for( itor = m_vEntityPCList.begin(); itor != m_vEntityPCList.end(); ++itor )
	{
		if( ( !stricmp( (*itor)->userID, pl->userID ) ) &&
			( (*itor)->serverGroupID == pl->serverGroupID ) &&
			( (*itor)->characterID == pl->characterID ) ) return (*itor)->idx;
	}

	return -1;
}

int CEntityListMng::FindPCList ( char *name, entityPCList_t *pl )
{
	if ( name[0] == 0 ) return -1 ;
	
	VECTOR_ENTITYPCLIST_STRUCT::iterator itor;
	for( itor = m_vEntityPCList.begin(); itor != m_vEntityPCList.end(); ++itor )
	{
		if ( !stricmp ( name, (*itor)->name ) )
		{
			ZeroMemory ( pl, sizeof ( entityPCList_t ) );
			memcpy ( pl, *itor, sizeof ( entityPCList_t ) );
			
			return (*itor)->idx ;
		}
	}

	return -1 ;
}

void CEntityListMng::AddPCList( entityPCList_t *pl )
{
	if( g_config.isManager )
	{
		int emptyIdx = FindEmptyPCList();

		
		if( FindPCList( pl ) >= 0 )
		{
			
			return;
		}
		if( emptyIdx < 0 )
		{
			
			return;
		}
	
		pl->idx = emptyIdx;
		memcpy( &m_entityPCList[emptyIdx], pl, sizeof( entityPCList_t ) );	
		m_vEntityPCList.push_back( &m_entityPCList[emptyIdx] );
	}
	else
	{
		
		GTH_SendMessage_PCList_Request_Add( pl );
	}
}

void CEntityListMng::DeletePCList( entityPCList_t *pl )
{
	
	if( g_config.isManager )
	{	
		int pcIdx = FindPCList( pl );
		if( pcIdx < 0 )
		{
			
			return;
		}

		m_vEntityPCList.erase( std::find( m_vEntityPCList.begin(), m_vEntityPCList.end(), &m_entityPCList[pcIdx] ) );
		m_entityPCList[pcIdx].idx = -1;
	}
	else
	{
		
		GTH_SendMessage_PCList_Request_Delete( pl );
	}
}

void CEntityListMng::UpdatePCList( entityPCList_t *pl )
{
	
	if( g_config.isManager )
	{	
		int pcIdx = FindPCList( pl );
		if( pcIdx < 0 )
		{
			
			return;
		}

		memcpy( &m_entityPCList[pcIdx], pl, sizeof( entityPCList_t ) );
		m_entityPCList[pcIdx].idx = pcIdx;
	}
	else
	{
		
		GTH_SendMessage_PCList_Request_Update( pl );
	}
}
