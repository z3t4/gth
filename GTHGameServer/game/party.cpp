#include "../global.h"

int g_partyTime;

#include "../Tools/CTools.h"
extern CTools* gcpTools;






void GTH_InitPartyInfo()
{
	int i, j;
	for( i = 0; i < MAX_PARTY_NUMBER; i ++ )
	{
		memset( &g_partyInfo[i], 0, sizeof( serverPartyInfo_t ) );
		for( j = 0; j < MAX_NUMBER_OF_PARTY_MEMBER; j ++ )
			g_partyInfo[i].member[j].serverIndex = -1;

		g_partyInfo[i].idx = -1;
		g_partyInfo[i].isHolding = 0;		
	}
	g_partyTime = g_globalTime;
}


void GTH_InitPartyInfo( serverPartyInfo_t *p )
{
	int i;

	memset( p, 0, sizeof( serverPartyInfo_t ) );
	p->idx = -1;

	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		p->member[i].serverIndex = -1;	
		
	}
	p->isHolding = 0;	
}


int GTH_GetEmptyPartyInfo()
{
	int i;
	for( i = 0; i < MAX_PARTY_NUMBER; i ++ )
	{
		if( g_partyInfo[i].memberNumber == 0 ) break;
	}
	if( i == MAX_PARTY_NUMBER ) return -1;

	return i;
}
 

int GTH_GetPartyInfo( char* organizerName, int organizeServer, int organizeTime )
{
	int i;
	for( i = 0; i < MAX_PARTY_NUMBER; i ++ )
	{
		if( g_partyInfo[i].memberNumber == 0 ) continue;
		if( stricmp( organizerName, g_partyInfo[i].organizerName ) ) continue;
		if( organizeServer != g_partyInfo[i].organizeServer ) continue;
		if( organizeTime != g_partyInfo[i].organizeTime ) continue;

		break;
	}
	if( i == MAX_PARTY_NUMBER ) return -1;

	return i;
}

int GTH_GetEmptyMemberIdx( serverPartyInfo_t *p )
{
	int i;
	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( p->member[i].serverIndex == -1 ) break;
	}
	if( i == MAX_NUMBER_OF_PARTY_MEMBER ) return( -1 );
	return( i );
}

int GTH_FindMemberIdx( serverPartyInfo_t *p, char* memberName )
{
	int i;
	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( p->member[i].serverIndex == -1 ) continue;
		if( !stricmp( p->member[i].memberName, memberName ) ) break;
	}
	if( i == MAX_NUMBER_OF_PARTY_MEMBER ) return( -1 );

	return( i );
}

int GTH_FindLeaderIdx( serverPartyInfo_t *p )
{
	int i;
	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( p->member[i].serverIndex == -1 ) continue;
		if( p->member[i].isLeader ) break;
	}
	if( i == MAX_NUMBER_OF_PARTY_MEMBER ) return( -1 );

	return( i );
}



int GTH_FindNextLeader( serverPartyInfo_t *p, int exceptMember )
{
	int i;
	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( p->member[i].serverIndex == -1 ) continue;
		if( i == exceptMember ) continue;
		if( !p->member[i].isActive ) continue;

		break;
	}
	if( i == MAX_NUMBER_OF_PARTY_MEMBER ) return( -1 );

	return( i );
}

int GTH_GetCorrectMember( serverPartyInfo_t *p, playerCharacter_t *pc )
{
	if( stricmp( p->organizerName, pc->organizerName ) ) return( false );
	if( p->organizeServer != pc->organizeServer ) return( false );
	if( p->organizeTime != pc->organizeTime ) return( false );
	if( !pc->active || !pc->ready ) return( false );

	return( true );
}

int GTH_GetActiveMemberNumber( serverPartyInfo_t *p, int exceptMember )
{
	int i;
	int n = 0;
	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( p->member[i].serverIndex == -1 ) continue;
		if( i == exceptMember ) continue;
		if( !p->member[i].isActive ) continue;

		n ++;
	}
	return( n );
}


void GTH_SetServerPartyAckMessage( playerCharacter_t *pc, int errorFlag )
{
	pc->serverAckMsgPartyFlag = true;
	pc->serverAckMsgPartyWaitTimer = g_globalTime + 1000;	
	pc->serverPartyErrorFlag = errorFlag;
}

void GTH_ClearServerPartyAckMessage( playerCharacter_t *pc )
{
	pc->serverAckMsgPartyFlag = false;
	pc->serverAckMsgPartyWaitTimer = 0;
}

int GTH_CheckServerPartyAckMessage( playerCharacter_t *pc )
{
	
	if ( pc->serverAckMsgPartyFlag )
	{
		if ( g_globalTime >= pc->serverAckMsgPartyWaitTimer )
		{
			GTH_ClearServerPartyAckMessage( pc );
			return( false );
		}
	}
	return( true );
}

int GTH_CheckSameParty( playerCharacter_t *pc1, playerCharacter_t *pc2 )
{
	if( stricmp( pc1->organizerName, pc2->organizerName ) ) return( false );
	if( pc1->organizeServer != pc2->organizeServer ) return( false );
	if( pc1->organizeTime != pc2->organizeTime ) return( false );

	return( true );
}






void GTH_PartyInfo_Organize( partyMember_t* l, partyMember_t* m, float organizeTime )
{
	int reply = 1, error = 0, idx;
	serverPartyInfo_t *pi;
	idx = GTH_GetEmptyPartyInfo();
	
	if( idx < 0 )
	{
		reply = 0;
		error = ERROR_PARTY_2;
	}
	else
	{
		pi = &g_partyInfo[idx];
		pi->canJoin = true;
		pi->idx = idx;
		pi->itemDivideType = 0;
		pi->memberNumber = 2;
		strcpy( pi->organizerName, l->memberName );
		pi->organizeServer = l->serverIndex;
		pi->organizeTime = organizeTime;

		memcpy( &pi->member[0], l, sizeof( partyMember_t ) );
		memcpy( &pi->member[1], m, sizeof( partyMember_t ) );

		
		if( pi->member[0].serverIndex == g_config.gameServerNo )
		{
			GTH_PartyInfo_Reply_Organize( l, m, idx, organizeTime, true );
		}
		else
		{
			GTH_SendMessage_PartyInfo_Reply_Organize( pi->member[0].serverIndex, idx, l, m, organizeTime, true );
		}
		
		if( pi->member[1].serverIndex == g_config.gameServerNo )
		{
			GTH_PartyInfo_Reply_Organize( l, m, idx, organizeTime, false );
		}
		else
		{
			GTH_SendMessage_PartyInfo_Reply_Organize( pi->member[1].serverIndex, idx, l, m, organizeTime, false );
		}
	}
}


void GTH_PartyInfo_Request_Organize( playerCharacter_t *leader, playerCharacter_t *firstMember )
{
	partyMember_t l, m;
	int organizeTime;

	memset( &l, 0, sizeof( partyMember_t ) );
	memset( &m, 0, sizeof( partyMember_t ) );

	
	strcpy( l.memberName, leader->name );
	l.idx = 0;
	l.isActive = true;
	l.isLeader = true;
	l.level = leader->level;
	l.gen = leader->pcClass;
	l.serverIndex = g_config.gameServerNo;
	l.idxOnServer = leader->idx;
	l.worldIdx = leader->worldIdx;

	
	strcpy( m.memberName, firstMember->name );
	m.idx = 1;
	m.isActive = true;
	m.isLeader = false;
	m.level = firstMember->level;
	m.gen = firstMember->pcClass;
	m.serverIndex = g_config.gameServerNo;
	m.idxOnServer = firstMember->idx;
	m.worldIdx = firstMember->worldIdx;

	
	organizeTime = g_globalTime;

	GTH_SetServerPartyAckMessage( leader, ERROR_PARTY_2 );
	GTH_SetServerPartyAckMessage( firstMember, ERROR_PARTY_2 );

	
	if ( g_config.isManager )
	{
		
		GTH_PartyInfo_Organize( &l, &m, organizeTime );
	}
	else
	{
		
		GTH_SendMessage_PartyInfo_Request_Organize( &l, &m, organizeTime );
	}
}


void GTH_PartyInfo_Reply_Organize( partyMember_t* l, partyMember_t* m, int partyIdx, float organizeTime, int isLeader )
{
	playerCharacter_t* pc;
	if( isLeader )
	{
		pc = &g_pc[l->idxOnServer];
		strcpy( pc->organizerName, l->memberName );
		pc->organizeServer = l->serverIndex;
		pc->organizeTime = organizeTime;
		pc->memberNumber = 2;
		pc->_partyRandomInfo_t.itemDivideType = 0;
		pc->canJoin = true;
		pc->partyIndex = partyIdx;
		pc->isLeader = true;

		memcpy( &pc->member[0], l, sizeof( partyMember_t ) );
		memcpy( &pc->member[1], m, sizeof( partyMember_t ) );

		GTH_ClearServerPartyAckMessage( pc );
		
		GTH_SendMessage_Party_Admit_Organize( pc, 1, 0, true );
	}
	else
	{
		pc = &g_pc[m->idxOnServer];
		strcpy( pc->organizerName, l->memberName );
		pc->organizeServer = l->serverIndex;
		pc->organizeTime = organizeTime;
		pc->memberNumber = 2;
		pc->_partyRandomInfo_t.itemDivideType = 0;
		pc->canJoin = true;
		pc->partyIndex = partyIdx;
		pc->isLeader = false;

		memcpy( &pc->member[0], l, sizeof( partyMember_t ) );
		memcpy( &pc->member[1], m, sizeof( partyMember_t ) );

		GTH_ClearServerPartyAckMessage( pc );
		
		GTH_SendMessage_Party_Admit_Organize( pc, 1, 0, false );
	}
}






void GTH_PartyInfo_Join( partyMember_t* m, char* organizerName, int organizeServer, int organizeTime )
{
	int reply = 1, error = 0, idx, memberIdx, i;
	serverPartyInfo_t *pi;
	
	idx = GTH_GetPartyInfo( organizerName, organizeServer, organizeTime );
	
	if( idx < 0 )
	{
		reply = 0;
		error = ERROR_PARTY_2;
	}
	else
	{
		pi = &g_partyInfo[idx];

		
		if( GTH_FindMemberIdx( pi, m->memberName ) >= 0 ) return;

		memberIdx = GTH_GetEmptyMemberIdx( pi );
		if( memberIdx < 0 ) return;

		
		pi->memberNumber ++;

 		m->idx = memberIdx;
		memcpy( &pi->member[memberIdx], m, sizeof( partyMember_t ) );

		
		for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
		{
			if( pi->member[i].serverIndex == -1 ) continue;
			
			if( !pi->member[i].isActive ) continue;

			
			if( i == memberIdx ) continue;

			
			if( pi->member[i].serverIndex == g_config.gameServerNo )
			{
				GTH_PartyInfo_Reply_Join( pi, pi->member[i].idxOnServer, false );
			}
			else
			{
				GTH_SendMessage_PartyInfo_Reply_Join( pi, pi->member[i].serverIndex, pi->member[i].idxOnServer, false );
			}
		}
		
		if( pi->member[memberIdx].serverIndex == g_config.gameServerNo )
		{
			GTH_PartyInfo_Reply_Join( pi, pi->member[memberIdx].idxOnServer, true );
		}
		else
		{
			GTH_SendMessage_PartyInfo_Reply_Join( pi, pi->member[memberIdx].serverIndex, pi->member[memberIdx].idxOnServer, true );
		}
	}
}


void GTH_PartyInfo_Request_Join( playerCharacter_t *leader, playerCharacter_t *newMember )
{
	partyMember_t m;

	memset( &m, 0, sizeof( partyMember_t ) );

	
	strcpy( m.memberName, newMember->name );
	m.isActive = true;
	m.isLeader = false;
	m.level = newMember->level;
	m.gen = newMember->pcClass;
	m.serverIndex = g_config.gameServerNo;
	m.idxOnServer = newMember->idx;
	m.worldIdx = newMember->worldIdx;

	GTH_SetServerPartyAckMessage( leader, ERROR_PARTY_2 );
	GTH_SetServerPartyAckMessage( newMember, ERROR_PARTY_2 );

	
	if ( g_config.isManager )
	{
		
		GTH_PartyInfo_Join( &m, leader->organizerName, leader->organizeServer, leader->organizeTime );
	}
	else
	{
		
		GTH_SendMessage_PartyInfo_Request_Join( &m, leader->organizerName, leader->organizeServer, leader->organizeTime );
	}
}


void GTH_PartyInfo_Reply_Join( serverPartyInfo_t* pi, int currentMemberIdx, int isNewMember )
{
	int i;
	playerCharacter_t* pc;

	
	pc = &g_pc[currentMemberIdx];

	
	if( !isNewMember ) 
	{
		if( !GTH_GetCorrectMember( pi, pc ) ) return;
	}
	GTH_ClearServerPartyAckMessage( pc );

	strcpy( pc->organizerName, pi->organizerName );
	pc->organizeServer = pi->organizeServer;
	pc->organizeTime = pi->organizeTime;
	pc->memberNumber = pi->memberNumber;
	pc->_partyRandomInfo_t.itemDivideType = pi->itemDivideType;
	pc->canJoin = pi->canJoin;
	pc->partyIndex = pi->idx;
	
	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( pi->member[i].serverIndex == -1 ) continue;
		memcpy( &pc->member[i], &pi->member[i], sizeof( partyMember_t ) );
	}

	
	if( isNewMember ) GTH_SendMessage_Party_Admit_Join( pc, 1, 0, true );
	else GTH_SendMessage_Party_Admit_Join( pc, 1, 0, false );	
}




void GTH_PartyInfo_Out( int isForced, char* memberName, char* organizerName, int organizeServer, int organizeTime )
{
	serverPartyInfo_t *pi;
	int i;
	int idx, reply = 1, memberIdx;
	int nextLeaderIdx = -1;
	
	idx = GTH_GetPartyInfo( organizerName, organizeServer, organizeTime );

	
	if( idx < 0 ) 
	{
		reply = 0;
		return;
	}
	
	pi = &g_partyInfo[idx];
	
	memberIdx = GTH_FindMemberIdx( pi, memberName );

	if( memberIdx < 0 ) 
	{
		reply = 0;
		return;
	}

	if( pi->member[memberIdx].isLeader )
	{
		nextLeaderIdx = GTH_FindNextLeader( pi, memberIdx );
	}
	else
	{
		nextLeaderIdx = GTH_FindLeaderIdx( pi );
	}
	pi->memberNumber --;
	
	
  
  
	

	
	if( pi->memberNumber < 2 || ( pi->member[memberIdx].isLeader && nextLeaderIdx < 0 ) )
	{
		GTH_PartyInfo_Disorganize( organizerName, organizeServer, organizeTime );
		return;
	}

	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( pi->member[i].serverIndex == -1 ) continue;
		
		if( pi->member[i].idx == memberIdx ) continue;
		if( !pi->member[i].isActive ) continue;

		
		if( pi->member[i].serverIndex == g_config.gameServerNo )
		{
			GTH_PartyInfo_Reply_Out( isForced, &pi->member[i], memberIdx, nextLeaderIdx, false );
		}
		else
		{
			GTH_SendMessage_PartyInfo_Reply_Out( isForced, &pi->member[i], memberIdx, nextLeaderIdx, false );
		}
	}

	
	if( pi->member[memberIdx].serverIndex == g_config.gameServerNo )
	{
		GTH_PartyInfo_Reply_Out( isForced, &pi->member[memberIdx], memberIdx, nextLeaderIdx, true );
	}
	else
	{
		GTH_SendMessage_PartyInfo_Reply_Out( isForced, &pi->member[memberIdx], memberIdx, nextLeaderIdx, true );
	}

	
	if( nextLeaderIdx >= 0 && pi->member[memberIdx].isLeader )
	{
		pi->member[nextLeaderIdx].isLeader = true;
	}

	memset( &pi->member[memberIdx], 0, sizeof( partyMember_t ) );
	pi->member[memberIdx].serverIndex = -1;	
}


void GTH_PartyInfo_Request_Out( playerCharacter_t *pc )
{
	
	if ( g_config.isManager )
	{
		GTH_PartyInfo_Out( false, pc->name, pc->organizerName, pc->organizeServer, pc->organizeTime );
	}
	else
	{
		
		GTH_SendMessage_PartyInfo_Request_Out( false, pc->name, pc->organizerName, pc->organizeServer, pc->organizeTime );
	}
}


void GTH_PartyInfo_Request_ForcedOut( playerCharacter_t* leader, char* memberName )
{
	
	if ( g_config.isManager )
	{
		GTH_PartyInfo_Out( true, memberName, leader->organizerName, leader->organizeServer, leader->organizeTime );
	}
	else
	{
		
		GTH_SendMessage_PartyInfo_Request_Out( true, memberName, leader->organizerName, leader->organizeServer, leader->organizeTime );
	}
}

void GTH_PartyInfo_Reply_Out( int isForced, partyMember_t *m, int reqMemberIdx, int newLeaderIdx, int reqFlag )
{
	playerCharacter_t *pc;
	int idx, i;

	idx = m->idxOnServer;
	if( idx < 0 ) return;

	pc = &g_pc[idx];

	
	if( stricmp( pc->name, m->memberName ) ) return;
	if( !pc->active || !pc->ready ) return;

	
	if( reqFlag )
	{
		memset( pc->organizerName, 0, sizeof( pc->organizerName ) );
		pc->organizeTime = -1;
		pc->organizeServer = -1;
		pc->memberNumber = 0;
		pc->partyIndex = -1;

		for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
		{
			memset( &pc->member[i], 0, sizeof( partyMember_t ) );
			pc->member[i].serverIndex = -1;
		}
		GTH_SendMessage_Party_Out( pc, 1, 0, isForced, reqMemberIdx, newLeaderIdx, reqFlag);
	}
	
	else
	{
		memset( &pc->member[reqMemberIdx], 0, sizeof( partyMember_t ) );
		pc->member[reqMemberIdx].serverIndex = -1;
		pc->memberNumber --;
		if( newLeaderIdx >= 0 )
		{
			pc->member[newLeaderIdx].isLeader = true;
		
			if( !stricmp( pc->name, pc->member[newLeaderIdx].memberName ) )
			{
				pc->isLeader = true;
			}
		}
		GTH_SendMessage_Party_Out( pc, 1, 0, isForced, reqMemberIdx, newLeaderIdx, reqFlag);
	}
}

void GTH_PartyInfo_Disorganize( char *organizerName, int organizeServer, int organizeTime )
{
	serverPartyInfo_t *pi;
	int i;
	int idx;
	
	idx = GTH_GetPartyInfo( organizerName, organizeServer, organizeTime );

	
	if( idx < 0 || idx >= MAX_PARTY_NUMBER ) 
	{
		return;
	}
	
	pi = &g_partyInfo[idx];

	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( pi->member[i].serverIndex == -1 ) continue;
		if( !pi->member[i].isActive ) continue;

		
		if( pi->member[i].serverIndex == g_config.gameServerNo )
		{
			GTH_PartyInfo_Reply_Disorganize( &pi->member[i] );
		}
		else
		{
			GTH_SendMessage_PartyInfo_Reply_Disorganize( &pi->member[i] );
		}
	}

	GTH_InitPartyInfo( pi );
}

void GTH_PartyInfo_Request_Disorganize( playerCharacter_t *pc )
{
	if( !pc->isLeader ) return;	
	
	if( g_config.isManager ) 
	{
		GTH_PartyInfo_Disorganize( pc->organizerName, pc->organizeServer, pc->organizeTime );
	}
	else
	{
		GTH_SendMessage_PartyInfo_Request_Disorganize( pc->organizerName, pc->organizeServer, pc->organizeTime );
	}
}

void GTH_PartyInfo_Reply_Disorganize( partyMember_t *m )
{
	playerCharacter_t *pc;
	int idx, i;

	idx = m->idxOnServer;
	if( idx < 0 ) return;

	pc = &g_pc[idx];

	
	if( stricmp( pc->name, m->memberName ) ) return;
	if( !pc->active || !pc->ready ) return;

	memset( pc->organizerName, 0, sizeof( pc->organizerName ) );
	pc->organizeTime = -1;
	pc->organizeServer = -1;
	pc->memberNumber = 0;
	pc->partyIndex = -1;

	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		memset( &pc->member[i], 0, sizeof( partyMember_t ) );
		pc->member[i].serverIndex = -1;
	}

	GTH_SendMessage_Party_Disorganize( pc );
}






void GTH_PartyInfo_SendAll( int serverIdx, int pcIdx, char* organizerName, int organizeServer, int organizeTime )
{
	serverPartyInfo_t *pi = NULL;
	int idx, reply = 1;
	
	idx = GTH_GetPartyInfo( organizerName, organizeServer, organizeTime );

	
	if( idx < 0 ) 
	{
		reply = 0;
	}
	else
	{
		pi = &g_partyInfo[idx];
	}

	
	if( serverIdx == g_config.gameServerNo )
	{
		GTH_PartyInfo_Reply_SendAll( reply, pcIdx, pi );
	}
	else
	{
		GTH_SendMessage_PartyInfo_Reply_SendAll( reply, serverIdx, pcIdx, pi );
	}
}


void GTH_PartyInfo_Request_SendAll( playerCharacter_t *pc )
{
	
	if ( g_config.isManager )
	{
		
		GTH_PartyInfo_SendAll( g_config.gameServerNo, pc->idx, pc->organizerName, pc->organizeServer, pc->organizeTime );
	}
	else
	{
		
		GTH_SendMessage_PartyInfo_Request_SendAll( pc );
	}
}


void GTH_PartyInfo_Reply_SendAll( int reply, int pcIdx, serverPartyInfo_t *pi )
{
	playerCharacter_t *pc;
	int i;

	pc = &g_pc[pcIdx];

	
	if( reply )
	{
		for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
		{
			if( pi->member[i].serverIndex < 0 ) continue;
			
			if( !stricmp( pi->member[i].memberName, pc->name ) ) break;
		}
		
		
		if( i == MAX_NUMBER_OF_PARTY_MEMBER )
		{
			memset( pc->organizerName, 0, sizeof( pc->organizerName ) );
			pc->organizeServer = -1;
			pc->organizeTime = 0;
			GTH_SendMessage_Party_SendAll( pc, false );
		}
		else
		{
			pc->memberNumber = pi->memberNumber;
			pc->_partyRandomInfo_t.itemDivideType = pi->itemDivideType;
			pc->canJoin = pi->canJoin;
			pc->partyIndex = pi->idx;
			
			for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
			{
				if( pi->member[i].serverIndex == -1 ) continue;
				memcpy( &pc->member[i], &pi->member[i], sizeof( partyMember_t ) );
				if( !stricmp( pc->member[i].memberName, pc->name ) )
				{
					if( pc->member[i].isLeader ) pc->isLeader = true;
				}
			}
			GTH_SendMessage_Party_SendAll( pc, true );
		}
	}
	
	else
	{
		memset( pc->organizerName, 0, sizeof( pc->organizerName ) );
		pc->organizeServer = -1;
		pc->organizeTime = 0;
		GTH_SendMessage_Party_SendAll( pc, false );
	}

	
	if( pc->organizeServer >= 0 && pc->isLeader )
	{
		GTH_PartyInfo_Request_Holding( pc, 0 );
	}
}


void GTH_PartyInfo_Update( char* memberName, int active, int serverIdx, int pcIdx, int level, int worldIdx, char* organizerName, int organizeServer, int organizeTime )
{
	serverPartyInfo_t *pi;
	int i;
	int idx, memberIdx, memberNumber;
	int nextLeaderIdx = 0;
	
	idx = GTH_GetPartyInfo( organizerName, organizeServer, organizeTime );

	
	if( idx < 0 ) 
	{
		return;
	}
	
	pi = &g_partyInfo[idx];
	
	memberIdx = GTH_FindMemberIdx( pi, memberName );

	if( memberIdx < 0 ) 
	{
		return;
	}
	
	if( !active )
	{
		if( pi->member[memberIdx].isLeader )
		{
			pi->member[memberIdx].isLeader = false;

			nextLeaderIdx = GTH_FindNextLeader( pi, memberIdx );
			if( nextLeaderIdx >= 0 ) 
				pi->member[nextLeaderIdx].isLeader = true;
		}
		else
		{
			nextLeaderIdx = GTH_FindLeaderIdx( pi );
			
			if( nextLeaderIdx < 0 ) return;
		}

		memberNumber = GTH_GetActiveMemberNumber( pi, memberIdx );

		if( memberNumber == 0 )
		{
			GTH_PartyInfo_Disorganize( organizerName, organizeServer, organizeTime );
			return;
		}
	}

	pi->member[memberIdx].level = level;
	pi->member[memberIdx].isActive = active;
	pi->member[memberIdx].serverIndex = serverIdx;
	pi->member[memberIdx].idxOnServer = pcIdx;
	pi->member[memberIdx].worldIdx = worldIdx;

	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( pi->member[i].serverIndex == -1 ) continue;
		if( !pi->member[i].isActive ) continue;
		
		if( pi->member[i].idx == memberIdx ) continue;

		
		if( pi->member[i].serverIndex == g_config.gameServerNo )
		{
			GTH_PartyInfo_Reply_Update( &pi->member[i], memberIdx, nextLeaderIdx, active, serverIdx, pcIdx, level, pi->member[memberIdx].worldIdx );
		}
		else
		{
			GTH_SendMessage_PartyInfo_Reply_Update( &pi->member[i], memberIdx, nextLeaderIdx, active, serverIdx, pcIdx, level, pi->member[memberIdx].worldIdx );
		}
	}
}



void GTH_PartyInfo_Request_Update( playerCharacter_t *pc, int active)
{
	if( pc->organizeServer == -1 ) return;

	
	if ( g_config.isManager )
	{
		
		GTH_PartyInfo_Update( pc->name, active, g_config.gameServerNo, pc->idx, pc->level, pc->worldIdx, pc->organizerName, pc->organizeServer, pc->organizeTime );
	}
	else
	{
		
		GTH_SendMessage_PartyInfo_Request_Update( pc->name, active, g_config.gameServerNo, pc->idx, pc->level, pc->worldIdx, pc->organizerName, pc->organizeServer, pc->organizeTime );
	}
}


void GTH_PartyInfo_Reply_Update( 
	partyMember_t *m, 
	int memberIdx, 
	int nextLeaderIdx, 
	int in_nActive, 
	int serverIdx, 
	int pcIdx, 
	int level, 
	int worldIdx )
{
	if(memberIdx < 0) return;
	if(memberIdx >= MAX_NUMBER_OF_PARTY_MEMBER) return;

	if(nextLeaderIdx < 0) return;
	if(nextLeaderIdx >= MAX_NUMBER_OF_PARTY_MEMBER) return;


	playerCharacter_t* pPlayer=NULL;
	pPlayer =  gcpTools->GetPlayerRecordPointer(m->idxOnServer);
	if(NULL == pPlayer) return;
	if(!pPlayer->ready ) return;

	
	if(0 != stricmp( pPlayer->name, m->memberName ) ) return;

	partyMember_t* pPartyMember = &pPlayer->member[memberIdx];
	pPartyMember->isActive    = in_nActive;
	pPartyMember->level		  = level;
	pPartyMember->idxOnServer = pcIdx;
	pPartyMember->serverIndex = serverIdx;
	pPartyMember->worldIdx    = worldIdx;
	if( !in_nActive ){
		pPartyMember->isLeader = false;
	}


	
	pPlayer->member[nextLeaderIdx].isLeader = true;
	if(0 == stricmp( pPlayer->name, pPlayer->member[nextLeaderIdx].memberName ) )
	{
		pPlayer->isLeader = true;
	}


	GTH_SendMessage_Party_Update( pPlayer, memberIdx, nextLeaderIdx, in_nActive, pcIdx, level, worldIdx );
}


void GTH_PartyInfo_Request_ItemDivideType( playerCharacter_t *pc, int type )
{
	if( pc->organizeServer == -1 ) return;

	
	if ( g_config.isManager )
	{
		
		GTH_PartyInfo_ItemDivideType( pc->organizerName, pc->organizeServer, pc->organizeTime, type );
	}
	else
	{
		
		GTH_SendMessage_PartyInfo_Request_ItemDivideType( pc->organizerName, pc->organizeServer, pc->organizeTime, type );
	}
}

void GTH_PartyInfo_ItemDivideType( char* organizerName, int organizeServer, int organizeTime, int type )
{
	serverPartyInfo_t *pi;
	int i;
	int idx;
	int nextLeaderIdx = 0;
	
	idx = GTH_GetPartyInfo( organizerName, organizeServer, organizeTime );

	
	if( idx < 0 ) 
	{
		return;
	}
	
	pi = &g_partyInfo[idx];
	pi->itemDivideType = type;

	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( pi->member[i].serverIndex == -1 ) continue;
		if( !pi->member[i].isActive ) continue;
		
		if( pi->member[i].serverIndex == g_config.gameServerNo )
		{
			GTH_PartyInfo_Reply_ItemDivideType( &pi->member[i], type );
		}
		else
		{
			GTH_SendMessage_PartyInfo_Reply_ItemDivideType( &pi->member[i], type );
		}
	}
}

void GTH_PartyInfo_Reply_ItemDivideType( partyMember_t *m, int type )
{
	playerCharacter_t *pc;
	

	
	int idx;

	idx = m->idxOnServer;
	if( idx < 0 ) return;

	pc = &g_pc[idx];

	
	if( stricmp( pc->name, m->memberName ) ) return;
	if( !pc->active || !pc->ready ) return;


	




	
}


void GTH_PartyInfo_Request_Holding( playerCharacter_t *pc, int holdingFlag )
{	
	
	
	
	

	
	if( g_config.isManager )
	{
		GTH_PartyInfo_Holding( pc->organizerName, pc->organizeServer, pc->organizeTime, holdingFlag );
	}
	else
	{
		GTH_SendMessage_PartyInfo_Request_Holding( pc->organizerName, pc->organizeServer, pc->organizeTime, holdingFlag );
	}
}

void GTH_PartyInfo_Holding( char* organizerName, int organizeServer, int organizeTime, int holdingFlag )
{
	serverPartyInfo_t *pi;
	int i;
	int idx;
	int leaderIdx, nextLeaderIdx = 0, memberNumber;
	int level, pcIdx, serverIdx;

	idx = GTH_GetPartyInfo( organizerName, organizeServer, organizeTime );
	if( idx < 0 ) return;
	pi = &g_partyInfo[idx];

	
	
	
	
	pi->isHolding = holdingFlag;
	pi->holdingCheckTime = g_globalTime;

	if( pi->isHolding != 2 ) return;
	
	pi->isHolding = 0;
	
	leaderIdx = GTH_FindLeaderIdx( pi );
	pi->member[leaderIdx].isLeader = false;
	nextLeaderIdx = GTH_FindNextLeader( pi, leaderIdx );
	if( nextLeaderIdx >= 0 ) 
	{
		pi->member[nextLeaderIdx].isLeader = true;
	}
	memberNumber = GTH_GetActiveMemberNumber( pi, leaderIdx );
	
	if( memberNumber == 0 )
	{
		GTH_PartyInfo_Disorganize( pi->organizerName, pi->organizeServer, pi->organizeTime );
		return;
	}
	level = pi->member[leaderIdx].level;
	pi->member[leaderIdx].isActive = false;
	pcIdx = pi->member[leaderIdx].idxOnServer;
	serverIdx = pi->member[leaderIdx].serverIndex;

	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( pi->member[i].serverIndex == -1 ) continue;
		if( !pi->member[i].isActive ) continue;
		
		if( pi->member[i].idx == leaderIdx ) continue;

		
		if( pi->member[i].serverIndex == g_config.gameServerNo )
		{
			GTH_PartyInfo_Reply_Update( &pi->member[i], leaderIdx, nextLeaderIdx, false, serverIdx, pcIdx, level, pi->member[leaderIdx].worldIdx );
		}
		else
		{
			GTH_SendMessage_PartyInfo_Reply_Update( &pi->member[i], leaderIdx, nextLeaderIdx, false, serverIdx, pcIdx, level, pi->member[leaderIdx].worldIdx );
		}
	}
}

void GTH_PartyInfo_CheckHolding()
{
	int i;

	if( g_globalTime - g_partyTime > PARTY_HOLDING_CHECK_TIME )
	{
		g_partyTime = g_globalTime;
	}
	else 
	{
		return;
	}

	for( i = 0; i < MAX_PARTY_NUMBER; i ++ )
	{
		if( g_partyInfo[i].idx < 0 ) continue;
		if( g_partyInfo[i].isHolding != 1 ) continue;
		
		if( g_globalTime - g_partyInfo[i].holdingCheckTime > PARTY_HOLDING_DELAY_TIME )
		{
			GTH_PartyInfo_Holding( g_partyInfo[i].organizerName, g_partyInfo[i].organizeServer, g_partyInfo[i].organizeTime, 2 );
		}
	}
}


void GTH_PartyInfo_Request_ChatMessage( playerCharacter_t *pc, char* message )
{
	
	if( g_config.isManager )
	{
		GTH_PartyInfo_ChatMessage( pc->name, pc->organizerName, pc->organizeServer, pc->organizeTime, message );
	}
	else
	{
		GTH_SendMessage_PartyInfo_Request_ChatMessage( pc->name, pc->organizerName, pc->organizeServer, pc->organizeTime, message );
	}
}

void GTH_PartyInfo_ChatMessage( char *fromName, char* organizerName, int organizeServer, int organizeTime, char* message )
{
	serverPartyInfo_t *pi;
	int i;
	int idx;
	int nextLeaderIdx = 0;
	
	idx = GTH_GetPartyInfo( organizerName, organizeServer, organizeTime );

	
	if( idx < 0 ) 
	{
		return;
	}
	
	pi = &g_partyInfo[idx];

	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		
		
		if( pi->member[i].serverIndex == -1 ) continue;
		if( !pi->member[i].isActive ) continue;
		if( pi->member[i].idxOnServer < 0 || pi->member[i].idxOnServer >= MAX_PCS ) continue;

		
		if( !stricmp( pi->member[i].memberName, fromName ) ) continue;
		
		if( pi->member[i].serverIndex == g_config.gameServerNo )
		{
			GTH_PartyInfo_Reply_ChatMessage( &pi->member[i], fromName, message );
		}
		else
		{
			GTH_SendMessage_PartyInfo_Reply_ChatMessage( &pi->member[i], fromName, message );
		}
	}
}




void GTH_PartyInfo_Reply_ChatMessage( partyMember_t *m, char* fromName, char* message )
{
	playerCharacter_t *pc;
	int idx;

	idx = m->idxOnServer;
	if( idx < 0 ) return;

	pc = &g_pc[idx];

	
	if( stricmp( pc->name, m->memberName ) ) return;
	if( !pc->active || !pc->ready ) return;

	GTH_SendMessage_Party_ChatMessage( pc, fromName, message );
}





