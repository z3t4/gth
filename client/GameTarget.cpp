



#include "global_def.h"

CGameTarget g_GameTarget;





CGameTarget::CGameTarget()
{

}

CGameTarget::~CGameTarget()
{

}


float CGameTarget::GTH_GetTargetDistance( Fx_CHARACTER_t *pThisChar )
{
	Fx_CHARACTER_t* character;
	int i;
	float dist;
	if( pThisChar->targetType < 0 || pThisChar->targetIdx < 0 ) return( -1.0f );

	character = &g_charManager->m_Characters[0];

	for( i = 0; i < g_charManager->m_iNumCharacters; i ++, character ++ )
	{
		if( character->entityType == pThisChar->targetType &&	\
			character->idxOnServer == pThisChar->targetIdx ) break;
	}
	if( i == g_charManager->m_iNumCharacters ) return( -1.0f);
	dist = GTH_GetDistance2D( pThisChar->position, character->position );

	if(dist <0.0f)
	{
		DXUtil_Trace("- dist %2.f \n",dist);
	}

	return( dist );
}


int CGameTarget::GTH_GetTargetPosition( int targetType, int targetIdx, vec3_t position )
{
	Fx_CHARACTER_t* character;
	int i;

	character = &g_charManager->m_Characters[0];

	for( i = 0; i < g_charManager->m_iNumCharacters; i ++, character ++ )
	{
		if( character->entityType == targetType &&	\
			character->idxOnServer == targetIdx ) break;
	}
	if( i == g_charManager->m_iNumCharacters ) 
	{
		return( 0 );
	}
	VectorCopy( position, character->position );

	
	position[2] += 50.0f;
	return( 1 );
}


int CGameTarget::GTH_GetTargetPosition( Fx_CHARACTER_t* pThisChar, vec3_t pos )
{
	Fx_CHARACTER_t* character;
	int i;

	if( pThisChar->targetType < 0 || pThisChar->targetIdx < 0 ) return( 0 );

	character = &g_charManager->m_Characters[0];

	for( i = 0; i < g_charManager->m_iNumCharacters; i ++, character ++ )
	{
		if( character->entityType == pThisChar->targetType &&	\
			character->idxOnServer == pThisChar->targetIdx ) break;
	}
	if( i == g_charManager->m_iNumCharacters ) return( 0 );
	
	VectorCopy( pos, character->position );

	return( 1 );
}


int CGameTarget::GTH_GetTargetAlive( Fx_CHARACTER_t* pThisChar )
{
	Fx_CHARACTER_t* character;
	int i;

	if( pThisChar->targetType < 0 || pThisChar->targetIdx < 0 ) return( 0 );

	character = &g_charManager->m_Characters[0];

	for( i = 0; i < g_charManager->m_iNumCharacters; i ++, character ++ )
	{
		if( character->entityType == pThisChar->targetType &&	\
			character->idxOnServer == pThisChar->targetIdx ) break;
	}
	if( i == g_charManager->m_iNumCharacters ) 
	{
		return( 0 );
	}
	
	if( character->curRA <= 0 ) 
	{
		return( 0 );
	}
	return( 1 );
}


int CGameTarget::GTH_GetTargetClientID( Fx_CHARACTER_t* pThisChar )
{
	Fx_CHARACTER_t* character;
	int i;

	if( pThisChar->targetType < 0 || pThisChar->targetIdx < 0 ) return( -1 );

	character = &g_charManager->m_Characters[0];

	for( i = 0; i < g_charManager->m_iNumCharacters; i ++, character ++ )
	{
		if( character->entityType == pThisChar->targetType &&	\
			character->idxOnServer == pThisChar->targetIdx ) break;
	}
	if( i == g_charManager->m_iNumCharacters ) 
	{
		return( -1 );
	}
	return( character->ID );
}


int CGameTarget::GTH_SetAngleToTarget( Fx_CHARACTER_t* pThisChar )
{
	Fx_CHARACTER_t* character;
	int i;

	vec3_t dir, angles;

	if( pThisChar->targetType < 0 || pThisChar->targetIdx < 0 ) return( 0 );

	character = &g_charManager->m_Characters[0];

	for( i = 0; i < g_charManager->m_iNumCharacters; i ++, character ++ )
	{
		if( character->entityType == pThisChar->targetType &&	\
			character->idxOnServer == pThisChar->targetIdx ) break;
	}
	if( i == g_charManager->m_iNumCharacters ) 
	{
		return( 0 );
	}

	VectorSubtract( dir, character->position, pThisChar->position );
	dir[2] = 0.0f;
	VectorNormalize( dir );
	VectorToAngles( dir , angles );

	pThisChar->angles[ YAW ] = angles[ YAW ];

	return( 1 );
}


Fx_CHARACTER_t*	CGameTarget::GTH_GetTargetPointer( Fx_CHARACTER_t* pThisChar )
{
	Fx_CHARACTER_t* character;
	int i;

	if( pThisChar->targetType < 0 || pThisChar->targetIdx < 0 ) return( NULL );

	character = &g_charManager->m_Characters[0];

	for( i = 0; i < g_charManager->m_iNumCharacters; i ++, character ++ )
	{
		if( character->entityType == pThisChar->targetType &&	\
			character->idxOnServer == pThisChar->targetIdx ) break;
	}
	if( i == g_charManager->m_iNumCharacters ) 
	{
		return( NULL );
	}

	return( character );
}


Fx_CHARACTER_t*	CGameTarget::GTH_GetTargetPointer( int targetType, int targetIdx )
{
	Fx_CHARACTER_t* character;
	int i;

	if( targetType < 0 || targetIdx < 0 ) return( NULL );

	character = &g_charManager->m_Characters[0];

	for( i = 0; i < g_charManager->m_iNumCharacters; i ++, character ++ )
	{
		if( character->entityType == targetType &&	\
			character->idxOnServer == targetIdx ) break;
	}
	if( i == g_charManager->m_iNumCharacters ) 
	{
		return( NULL );
	}

	return( character );
}

void CGameTarget::GTH_ChangeTarget( Fx_CHARACTER_t *character )
{
	
	g_pApp->m_myCharacter->atk_playAnim = false;

	g_pApp->m_myCharacter->targetType = character->entityType;
	g_pApp->m_myCharacter->targetIdx = character->idxOnServer;
	g_cgv.myCharacterInfo->targetType = character->entityType;
	g_cgv.myCharacterInfo->targetIdx = character->idxOnServer;
	g_cgv.pTargetChar = character;
	strcpy( g_cgv.targetName, character->name );

	
	g_cgv.resendTargeting = false;
	g_cgv.resendTime = g_timer.GetAppMilliTime() - TARGETING_RESEND_DELAY;
	g_cgv.resendAttack = false;

	VectorClear( g_cgv.oldTargetPosition );	

	g_cgv.saveEvent = -1;
	g_cgv.clickEvent = GTH_CEV_CHAR_IDLE;
	


}


int CGameTarget::GTH_GetTargetName( int targetType, int targetIdx, char *name )
{
	Fx_CHARACTER_t* character;
	int i;

	if( targetType < 0 || targetIdx < 0 ) return( false );

	character = &g_charManager->m_Characters[0];

	for( i = 0; i < g_charManager->m_iNumCharacters; i ++, character ++ )
	{
		if( character->entityType == targetType &&	\
			character->idxOnServer == targetIdx ) break;
	}
	if( i == g_charManager->m_iNumCharacters ) 
	{
		return( false );
	}

	strcpy( name, character->name );
	return( true );
}

int CGameTarget::GTH_GetTargetServerIdxFromName( char* name )
{
	Fx_CHARACTER_t* character;
	int i;

	character = &g_charManager->m_Characters[0];

	for( i = 0; i < g_charManager->m_iNumCharacters; i ++, character ++ )
	{
		if( character->entityType == ENTITY_PC )
		{
			if( !stricmp( character->name, name ) ) break;
		}
	}
	if( i == g_charManager->m_iNumCharacters ) 
	{
		return( -1 );
	}

	return( character->idxOnServer );
}
