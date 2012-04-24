     #include "../global_def.h"

int GTH_IsLock()
{
	if( g_cgv.waitReply )
	{
		
		if( g_timer.GetAppMilliTime() - g_cgv.lastLockingTime > 5000.0f )
		{
			g_cgv.waitReply = false;
		}			
	}
	return( g_cgv.waitReply );
}

int GTH_LockSending()
{
	if( GTH_IsLock() ) return false;
	g_cgv.waitReply = true;
	g_cgv.lastLockingTime = g_timer.GetAppMilliTime();

	return( true );
}

void GTH_UnlockSending()
{
	g_cgv.waitReply = false;
	g_cgv.lastLockingTime = 0.0f;
}




#ifdef _TERRA
#include "..\Terra ICT\CTerraCtrl.h"
extern CTerraCtrl gcTerraCtrl;
#endif

int GTH_RenameList()
{
	FILE *fp;
	char source[256], target[256];

	fp = fopen( "data/renlist.pack", "r" );
	if( fp == NULL ) return( 0 );

	while( 1 )
	{
		if( feof( fp ) )
		{
			break;
		}

		fscanf( fp, "%s %s", source, target );
		
		while( !CopyFile( source, target, false ) )
		{
			Sleep( 500 );
		}
		remove( source );
	}
	fclose( fp );
	remove( "data/renlist.pack" );

	return( 1 );
}

int GTH_DeleteList()
{
	FILE *fp;
	char temp[255];

	fp = fopen( "data/dellist.pack", "r" );
	if( fp == NULL ) return( 0 );

	while( 1 )
	{
		if( feof( fp ) )
		{
			break;
		}

		fscanf( fp, "%s", temp );
		remove( temp );
	}
	fclose( fp );
	remove( "data/dellist.pack" );

	return( 1 );
}


void GTH_ServerShutdownTimer()
{
	static int oldCountdown = 0;
	int curTime, duration, countdown;
	char msg[256];

	if ( g_cgv.serverShutdownTimer.active ) 
	{
		curTime = g_timer.GetAppTime();
		duration = curTime - g_cgv.serverShutdownTimer.setTime;
		countdown = g_cgv.serverShutdownTimer.countdown - duration;
		if ( countdown != oldCountdown)
		{	
			oldCountdown = countdown;
			if ( (countdown <= 10) || (countdown%10 == 0) )
			{
				
				sprintf(msg, g_LPACK.GetMassage(0,325), countdown);

				g_ifMng->AddNoticeMessage( msg, D3DCOLOR_ARGB(255, 255, 55, 55) );
				g_ifMng->AddSysMessage( msg, D3DCOLOR_ARGB(255, 255, 55, 55) );

				
				g_musicMng->PlaySample( 
					g_soundMng->GetNormalSoundID( GTH_SOUND_ID_WARNING_2 ), 
					GTH_SOUND_PLAY_ONCE );
			}
		}

		if (countdown <= 0)
		{
			GTH_SendMessage_DisconnectGameServer(FALSE);
			g_cgv.serverShutdownTimer.isComplete = true;
			g_cgv.serverShutdownTimer.active = false;
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,323), 1, IF_MESSAGE_EXIT_CLIENT );
		}
	}
}

void GTH_Init_ServerShutdownTimer()
{
	g_cgv.serverShutdownTimer.setTime = 0;
	g_cgv.serverShutdownTimer.active = false;
	g_cgv.serverShutdownTimer.countdown = 0;
	g_cgv.serverShutdownTimer.isComplete = false;
}


int GTH_GetTargetEntityForKeyMode( Fx_CHARACTER_t* pThisChar )
{
	float distance;
	int i, number;
	Fx_CHARACTER_t	 *character;

	character = &g_charManager->m_Characters[0];
	number = g_charManager->m_iNumCharacters;

	for (i=0; i<number; i++, character++)
	{
		if( character->entityType == ENTITY_PC || character->entityType == ENTITY_NPC ) continue;
		if ( character->entityType == ENTITY_MONSTER )
		{
			if( character->stat_alive != MON_ALIVE_STATE_NORMAL ) continue;
		}
		if( character->flag_culled ) continue;
		if( character->entityType == ENTITY_PC && character->idxOnServer == g_cgv.myPCIdxOnServer ) continue;

		distance = GTH_GetDistance( pThisChar->position, character->position);
		if ( distance < g_cgv.myCharacterInfo->calAttackLength )
		{
			g_cgv.pTargetChar = character;
			return( true );
		}
	}
	return( false );
}


int GTH_IsValidTargetAngleForKeyMode( Fx_CHARACTER_t* pThisChar )
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

	if( abs( pThisChar->angles[YAW] - angles[ YAW ] ) < 90.0f ) return( 1 );
	else return( 0 );
}




int GTH_CheckValidCharacterEvent( Fx_CHARACTER_t* character )
{
	if( character->curRA <= 0 )
	{
		if( character->event == GTH_EV_CHAR_ATTACK || character->event == GTH_EV_CHAR_STUN ||
			character->event == GTH_EV_CHAR_PC_STUN ||
			character->event == GTH_EV_CHAR_FREEZE || character->event == GTH_EV_CHAR_PUSHED )
		{
			character->event = GTH_EV_CHAR_DIE;
			return( 0 );
		}
	}

	return( 1 );
}

int GTH_HaveWeapon( Fx_CHARACTER_t* character )
{
	int itemIdx, mainClass;

	if( character->equipNumber <= 0 ) return( 0 );

	itemIdx = character->equipment[0];

	if( itemIdx < 0 ) return( 0 );
	mainClass = g_itemTable[itemIdx].classIdx;
	if( g_itemClassTable[mainClass].attackAtt ) return( 1 );
	return( 0 );
}

void GTH_GetRealMousePosition( vec3_t mousePos, vec3_t position )
{
	vec3_t pos;
	float dist;

	dist = VectorDistance( mousePos, g_pApp->m_myCharacter->position );
	dist -= g_cgv.mouseClickEventDistance;

	if( g_cgv.clickEvent == GTH_CEV_CHAR_IDLE || g_cgv.clickEvent == GTH_CEV_CHAR_NONE ) 
	{
		VectorCopy( position, mousePos );
		return;
	}
	else if( dist < 0.0f )
	{
		VectorCopy( position, g_pApp->m_myCharacter->position );
		return;
	}

	VectorSubtract( pos, mousePos, g_pApp->m_myCharacter->position );
	VectorNormalize( pos );
	VectorScale( pos, pos, dist );
	VectorAdd( position, pos, g_pApp->m_myCharacter->position );
}

void GTH_GetRealPosition( vec3_t position, float dist )
{
	vec3_t pos;
	float realDist;
	
	realDist = VectorDistance( position, g_pApp->m_myCharacter->position );

	if( dist > realDist ) return;

	VectorSubtract( pos, position, g_pApp->m_myCharacter->position );
	VectorNormalize( pos );
	VectorScale( pos, pos, dist );
	VectorAdd( position, pos, g_pApp->m_myCharacter->position );
}

int GTH_IsGameMaster( Fx_CHARACTER_t *character )
{
	//lucky 2012 new GM system
	if(character->gameMaster > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	//end
	return( true );
}

int GTH_RenderGM( Fx_CHARACTER_t* character )
{
	if( GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return( true );
	else
	{
		if( GTH_IsGameMaster( character ) && character->stat_alive ) return( false );
	}
	return( true );
}
