#include "../global_def.h"

CSoundMng::CSoundMng()
{
}

CSoundMng::~CSoundMng()
{
}

void CSoundMng::Initialize()
{
	int i, j;

	memset( &m_soundResource, 0, sizeof( gthSoundResource_t ) );	
	for( i = 0; i < 128; i ++ )
	{
		m_soundResource.normalID[i].idx = -1;
		m_soundResource.itemID[i].idx = -1;
		
		for( j = 0; j < MAX_ANIMCOUNT_TYPE; j ++ )
		{
			m_soundResource.eventID[j][i].idx = -1;
		}
	}
	

	m_nummdlSoundID	=	0;
	for( i = 0; i < MAX_COUNT_GTH_MDL_SOUND_ID; i ++ )
	{
		m_mdlSoundID[i].idx = -1;
		memset( m_mdlSoundID[i].mdlname, 0, sizeof( m_mdlSoundID [i].mdlname ) );
		memset( m_mdlSoundID[i].soundname, 0, sizeof( m_mdlSoundID [i].soundname ) );
	}

	for( i = 0; i < MAX_COUNT_GTH_MONSTER_SOUND; i ++ )
	{
		m_monSound[i].idx = -1;
		
		for( j = 0; j < MAX_ANIMCOUNT_TYPE; j ++ )
		{
			m_monSound[i].soundID[j] = -1;
			m_monSound[i].frequency[j] = 1;
			m_monSound[i].playTime[j] = 0.0f;
			m_monSound[i].playCount[j] = 1;
		}
		m_monSound[i].playCount[ANIM_ITEM_RUN] = 2;
		m_monSound[i].playCount[ANIM_ITEM_WALK] = 2;
	}
	for( i = 0; i < MAX_COUNT_GTH_PC_SOUND; i ++ )
	{
		m_pcSound[i].idx = -1;
		for( j = 0; j < MAX_ANIMCOUNT_TYPE; j ++ )
		{
			m_pcSound[i].soundID[j] = -1;
			m_pcSound[i].frequency[j] = 1;
			m_pcSound[i].playTime[j] = 0.0f;
			m_pcSound[i].playCount[j] = 1;
		}
		m_pcSound[i].playCount[ANIM_ITEM_RUN] = 2;
		m_pcSound[i].playCount[ANIM_ITEM_WALK] = 2;
	}
	for( i = 0; i < MAX_COUNT_GTH_ITEM_SOUND; i ++ )
	{
		m_itemSound[i].idx = -1;
		m_itemSound[i].soundID = -1;
	}
}


void CSoundMng::LoadMDLSoundConfig( char *in_filename )
{
	if( !g_script.Load( in_filename ) ) return;

	char *token;

	g_script.BeginParse();

	m_nummdlSoundID	=	0;

	while( 1 )
	{
		token = g_script.GetNextToken( true );
		if( !token[0] ) break;

		m_mdlSoundID[m_nummdlSoundID].idx	=	m_maxIdx  +  m_nummdlSoundID;
		
		strcpy ( m_mdlSoundID [m_nummdlSoundID].mdlname, token );
		token	=	g_script .GetNextToken ( true );
		strcpy ( m_mdlSoundID [m_nummdlSoundID].soundname, token );

		++m_nummdlSoundID;
	}
}

void CSoundMng::LoadSoundConfig( char* filename )
{
	char* token;
	char soundName[128];
	int idx, is3D, dup, resType = 0, type, eventType, soundID;
	float fraction, freq;

	Initialize();

	g_script.Load( filename );
	g_script.BeginParse();

	m_maxIdx	=	0;

	while( 1 )
	{
		token = g_script.GetNextToken( true );
		if( token[0] == 0 ) break;

		if( !stricmp( token, "RESOURCE" ) ) resType = 0;
		if( !stricmp( token, "MATCHING" ) ) resType = 1;
		else if( token[0] == '{' )
		{
			while( 1 )
			{
				token = g_script.GetNextToken( true );
				if( token[0] == '}' ) break;

				
				if( !resType )
				{
					if( !stricmp( token, "NORMAL" ) ) type = 0;
					else if( !stricmp( token, "ITEM" ) ) type = 1;
					else if( !stricmp( token, "EVENT_IDLE" ) ) { type = 2; eventType = ANIM_ITEM_IDLE; }
					else if( !stricmp( token, "EVENT_DAMAGE" ) ) { type = 2; eventType = ANIM_ITEM_DEFENSE; }
					else if( !stricmp( token, "EVENT_DIE" ) ) { type = 2; eventType = ANIM_ITEM_DYING_0; }
					else if( !stricmp( token, "EVENT_WALK" ) ) { type = 2; eventType = ANIM_ITEM_WALK; }
					else if( !stricmp( token, "EVENT_RUN" ) ) { type = 2; eventType = ANIM_ITEM_RUN; }
					else if( !stricmp( token, "EVENT_ATTACK" ) ) { type = 2; eventType = ANIM_ITEM_ATTACK_0; }
					else if( !stricmp( token, "EVENT_SHOCK" ) ) { type = 2; eventType = ANIM_ITEM_ATTACKED_0; }
					else if( token[0] == '{' )
					{
						while( 1 )
						{
							token = g_script.GetNextToken( true );
							if( token[0] == '}' ) break;

							
							idx = atoi( token );
							
							token = g_script.GetNextToken( true );
							strcpy( soundName, token );
							
							token = g_script.GetNextToken( true );
							is3D = atoi( token );
							
							token = g_script.GetNextToken( true );
							dup = atoi( token );
							if( type == 0 )
							{
								m_soundResource.normalID[idx].idx = idx;
								strcpy( m_soundResource.normalID[idx].filename, soundName );
								m_soundResource.normalID[idx].is3D = is3D;
								m_soundResource.normalID[idx].numDup = dup;
								m_soundResource.numNoramlID ++;

								m_maxIdx ++;
							}
							else if( type == 1 )
							{
								m_soundResource.itemID[idx].idx = idx;
								strcpy( m_soundResource.itemID[idx].filename, soundName );
								m_soundResource.itemID[idx].is3D = is3D;
								m_soundResource.itemID[idx].numDup = dup;
								m_soundResource.numItemID ++;

								m_maxIdx ++;
							}
							else if( type == 2 )
							{
								m_soundResource.eventID[eventType][idx].idx = idx;
								strcpy( m_soundResource.eventID[eventType][idx].filename, soundName );
								m_soundResource.eventID[eventType][idx].is3D = is3D;
								m_soundResource.eventID[eventType][idx].numDup = dup;
								m_soundResource.numEventID[eventType] ++;

								m_maxIdx ++;
							}
						}
					}
				}
				else
				{
					if( !stricmp( token, "ITEM" ) ) type = 0;
					else if( !stricmp( token, "PC" ) ) type = 1;
					else if( !stricmp( token, "MONSTER" ) ) type = 2;
					else if( token[0] == '{' )
					{
						while( 1 )
						{
							token = g_script.GetNextToken( true );
							if( token[0] == '}' ) break;
							else if( type == 0 )
							{
								idx = atoi( token );
								
								token = g_script.GetNextToken( true );
								soundID = atoi( token );
								
								token = g_script.GetNextToken( true );
								fraction = atof( token );

								m_itemSound[idx].idx = idx;
								m_itemSound[idx].soundID = soundID;
								m_itemSound[idx].playTime = fraction;
							}
							else if( type == 1 )
							{
								if( !stricmp( token, "IDX" ) ) eventType = -1;
								else if( !stricmp( token, "IDLE" ) ) eventType = ANIM_ITEM_IDLE;
								else if( !stricmp( token, "WALK" ) ) eventType = ANIM_ITEM_WALK;
								else if( !stricmp( token, "RUN" ) ) eventType = ANIM_ITEM_RUN;
								else if( !stricmp( token, "DAMAGE" ) ) eventType = ANIM_ITEM_DEFENSE;
								else if( !stricmp( token, "ATTACK" ) ) eventType = ANIM_ITEM_ATTACK_0;
								else if( !stricmp( token, "SHOCK" ) ) eventType = ANIM_ITEM_ATTACKED_0;
								else if( !stricmp( token, "DIE" ) ) eventType = ANIM_ITEM_DYING_0;
								else if( eventType == -1 )
								{
									idx = atoi( token );
								}
								else
								{
									soundID = atoi( token );
								
									token = g_script.GetNextToken( true );
									fraction = atof( token );

									token = g_script.GetNextToken( true );
									freq = atoi( token );

									m_pcSound[idx].idx = idx;
									m_pcSound[idx].soundID[eventType] = soundID;
									m_pcSound[idx].playTime[eventType] = fraction;
									m_pcSound[idx].frequency[eventType] = freq;
								}
							}
							else if( type == 2 )
							{
								if( !stricmp( token, "IDX" ) ) eventType = -1;
								else if( !stricmp( token, "IDLE" ) ) eventType = ANIM_ITEM_IDLE;
								else if( !stricmp( token, "WALK" ) ) eventType = ANIM_ITEM_WALK;
								else if( !stricmp( token, "RUN" ) ) eventType = ANIM_ITEM_RUN;
								else if( !stricmp( token, "DAMAGE" ) ) eventType = ANIM_ITEM_DEFENSE;
								else if( !stricmp( token, "ATTACK" ) ) eventType = ANIM_ITEM_ATTACK_0;
								else if( !stricmp( token, "SHOCK" ) ) eventType = ANIM_ITEM_ATTACKED_0;
								else if( !stricmp( token, "DIE" ) ) eventType = ANIM_ITEM_DYING_0;
								else if( eventType == -1 )
								{
									idx = atoi( token );
								}
								else
								{
									soundID = atoi( token );
								
									token = g_script.GetNextToken( true );
									fraction = atof( token );

									token = g_script.GetNextToken( true );
									freq = atoi( token );

									m_monSound[idx].idx = idx;
									m_monSound[idx].soundID[eventType] = soundID;
									m_monSound[idx].playTime[eventType] = fraction;
									m_monSound[idx].frequency[eventType] = freq;
								}
							}
						}
					}
				}
			}
		}
	}
}

void CSoundMng::PlayCharSound( Fx_CHARACTER_t* character )
{
	int itemIdx, mainClass, soundID, tableIdx;

	float fraction;

	
	
	if ( character->isTransform )
	{
		PlayMonsterSound(character);
		return ;
	}
	
	

	
	fraction = g_charManager->GetAnimFraction( character );
	tableIdx = character->tableIdx;

	
	
	











					















	{
		switch( character->ani_curSetID )
		{
		case ANIM_ITEM_ATTACK_0 : case ANIM_ITEM_ATTACK_1 : case ANIM_ITEM_ATTACK_2 :
		case ANIM_ITEM_ATTACK_3 : case ANIM_ITEM_ATTACK_4 :
			{
				return;

				if ( character->equipNumber > 0 )
				{
					itemIdx = character->equipment[0];
					mainClass = g_itemTable[itemIdx].classIdx;
					
					if ( g_itemClassTable[mainClass].attackAtt )
					{
						
						if( m_itemSound[itemIdx].soundID < 0 ) return;
	
	
	
	
	
	
						{
							g_musicMng->PlaySample3D( GetItemSoundID( m_itemSound[itemIdx].soundID ), GTH_SOUND_PLAY_ONCE , character->position );
							
							character->stat_sound = CHAR_SOUND_DISABLE;
						}
					}
				}
			}
			break;

		
		case ANIM_ITEM_RUN :
			{
				if( m_pcSound[tableIdx].soundID[ANIM_ITEM_RUN] < 0 ) return;
				if( m_pcSound[tableIdx].playTime[ANIM_ITEM_RUN] > fraction ) return;
				if( fraction < 0.8 && m_pcSound[tableIdx].playCount[ANIM_ITEM_RUN] == 1 ) return;
				
				m_pcSound[tableIdx].playCount[ANIM_ITEM_RUN] --;
				
				soundID = GetEventSoundID( ANIM_ITEM_RUN, m_pcSound[tableIdx].soundID[ANIM_ITEM_RUN] );
				
				g_musicMng->PlaySample3D( soundID, GTH_SOUND_PLAY_ONCE , character->position );
				if( m_pcSound[tableIdx].playCount[ANIM_ITEM_RUN] <= 0 ) character->stat_sound = CHAR_SOUND_DISABLE;
			}
			break;
		case ANIM_ITEM_WALK :
			{
				if( m_pcSound[tableIdx].soundID[ANIM_ITEM_WALK] < 0 ) return;
				if( m_pcSound[tableIdx].playTime[ANIM_ITEM_WALK] > fraction ) return;
				if( fraction < 0.8 && m_pcSound[tableIdx].playCount[ANIM_ITEM_WALK] == 1 ) return;
				
				m_pcSound[tableIdx].playCount[ANIM_ITEM_WALK] --;
				
				soundID = GetEventSoundID( ANIM_ITEM_WALK, m_pcSound[tableIdx].soundID[ANIM_ITEM_WALK] );

				g_musicMng->PlaySample3D( soundID, GTH_SOUND_PLAY_ONCE , character->position );
				if( m_pcSound[tableIdx].playCount[ANIM_ITEM_WALK] <= 0 ) character->stat_sound = CHAR_SOUND_DISABLE;
			}
			break;
		case ANIM_ITEM_DYING_0 :
			{
				if( m_pcSound[tableIdx].soundID[ANIM_ITEM_DYING_0] < 0 ) return;
				if( m_pcSound[tableIdx].playTime[ANIM_ITEM_DYING_0] > fraction ) return;

				soundID = GetEventSoundID( ANIM_ITEM_DYING_0, m_pcSound[tableIdx].soundID[ANIM_ITEM_DYING_0] );

				g_musicMng->PlaySample3D( soundID, GTH_SOUND_PLAY_ONCE , character->position );
				character->stat_sound = CHAR_SOUND_DISABLE;
			}
			break;
		case ANIM_ITEM_ATTACKED_0 :
			{
				if( m_pcSound[tableIdx].soundID[ANIM_ITEM_ATTACKED_0] < 0 ) return;
				if( m_pcSound[tableIdx].playTime[ANIM_ITEM_ATTACKED_0] > fraction ) return;

				soundID = GetEventSoundID( ANIM_ITEM_ATTACKED_0, m_pcSound[tableIdx].soundID[ANIM_ITEM_ATTACKED_0] );

				g_musicMng->PlaySample3D( soundID, GTH_SOUND_PLAY_ONCE , character->position );
				character->stat_sound = CHAR_SOUND_DISABLE;
			}
			break;
		}
	}
}

void CSoundMng::PlayMonsterSound( Fx_CHARACTER_t* character )
{
	int soundID, tableIdx;
	float soundTime;
	float fraction;

	
	fraction = g_charManager->GetAnimFraction( character );

	
	
	if ( character->isTransform)
		tableIdx = character->transformEntityIdx;
	else
		tableIdx = character->tableIdx;
	
	

	switch( character->ani_curSetID )
	{
	case ANIM_ITEM_ATTACK_0 : case ANIM_ITEM_ATTACK_1 : case ANIM_ITEM_ATTACK_2 :
	case ANIM_ITEM_ATTACK_3 : case ANIM_ITEM_ATTACK_4 :
		{
			if( m_monSound[tableIdx].playTime[ANIM_ITEM_ATTACK_0] > fraction ) return;
			if( m_monSound[tableIdx].soundID[ANIM_ITEM_ATTACK_0] < 0 ) return;

			soundID = GetEventSoundID( ANIM_ITEM_ATTACK_0, m_monSound[tableIdx].soundID[ANIM_ITEM_ATTACK_0] );
			
			g_musicMng->PlaySample3D( soundID, GTH_SOUND_PLAY_ONCE , character->position );
			character->stat_sound = CHAR_SOUND_DISABLE;
		}
		break;
	case ANIM_ITEM_IDLE :
		{
			if( m_monSound[tableIdx].soundID[ANIM_ITEM_IDLE] < 0 ) return;
			soundTime = g_timer.GetAppMilliTime() - character->soundTime;
			if( soundTime > 60000 / m_monSound[tableIdx].frequency[ANIM_ITEM_IDLE] )
			{
				character->soundTime = g_timer.GetAppMilliTime();
				if( rand() % 2 == 0 ) return;

				soundID = GetEventSoundID( ANIM_ITEM_IDLE, m_monSound[tableIdx].soundID[ANIM_ITEM_IDLE] );

				g_musicMng->PlaySample3D( soundID, GTH_SOUND_PLAY_ONCE , character->position );
				character->stat_sound = CHAR_SOUND_DISABLE;
			}
		}
		break;
	
	case ANIM_ITEM_RUN :
		{
			if( m_monSound[tableIdx].soundID[ANIM_ITEM_RUN] < 0 ) return;
			if( m_monSound[tableIdx].playTime[ANIM_ITEM_RUN] > fraction ) return;
			if( fraction < 0.8 && m_monSound[tableIdx].playCount[ANIM_ITEM_RUN] == 1 ) return;
			
			m_monSound[tableIdx].playCount[ANIM_ITEM_RUN] --;

			soundID = GetEventSoundID( ANIM_ITEM_RUN, m_monSound[tableIdx].soundID[ANIM_ITEM_RUN] );
			
			g_musicMng->PlaySample3D( soundID, GTH_SOUND_PLAY_ONCE , character->position );
			if( m_monSound[tableIdx].playCount[ANIM_ITEM_RUN] <= 0 ) character->stat_sound = CHAR_SOUND_DISABLE;
		}
		break;
	case ANIM_ITEM_WALK :
		{

		}
		break;
	case ANIM_ITEM_DYING_0 :
		{
			if( m_monSound[tableIdx].soundID[ANIM_ITEM_DYING_0] < 0 ) return;
			if( m_monSound[tableIdx].playTime[ANIM_ITEM_DYING_0] > fraction ) return;

			soundID = GetEventSoundID( ANIM_ITEM_DYING_0, m_monSound[tableIdx].soundID[ANIM_ITEM_DYING_0] );

			g_musicMng->PlaySample3D( soundID, GTH_SOUND_PLAY_ONCE , character->position );
			character->stat_sound = CHAR_SOUND_DISABLE;
		}
		break;
	case ANIM_ITEM_ATTACKED_0 :
		{
			if( m_monSound[tableIdx].soundID[ANIM_ITEM_ATTACKED_0] < 0 ) return;
			if( m_monSound[tableIdx].playTime[ANIM_ITEM_ATTACKED_0] > fraction ) return;

			soundID = GetEventSoundID( ANIM_ITEM_ATTACKED_0, m_monSound[tableIdx].soundID[ANIM_ITEM_ATTACKED_0] );
			
			g_musicMng->PlaySample3D( soundID, GTH_SOUND_PLAY_ONCE , character->position );
			character->stat_sound = CHAR_SOUND_DISABLE;
		}
		break;
	}
}

void CSoundMng::Play( Fx_CHARACTER_t* character )
{
	if( character->stat_sound == CHAR_SOUND_DISABLE ) return;
	
	if( character->entityType == ENTITY_PC ) PlayCharSound( character );
	else if( character->entityType == ENTITY_MONSTER || character->entityType == ENTITY_NPC ) PlayMonsterSound( character );
}

void CSoundMng::PlayDamageSound( Fx_CHARACTER_t* character )
{
	int tableIdx, soundID;

	if( character->entityType == ENTITY_MONSTER || character->entityType == ENTITY_PC )
	{
		tableIdx = character->tableIdx;
		if( m_monSound[tableIdx].soundID[ANIM_ITEM_DEFENSE] < 0 ) return;

		soundID = GetEventSoundID( ANIM_ITEM_DEFENSE, m_monSound[tableIdx].soundID[ANIM_ITEM_DEFENSE] );

		g_musicMng->PlaySample3D( soundID, GTH_SOUND_PLAY_ONCE , character->position );
	}
	else
	{
		tableIdx = character->tableIdx;
		if( m_pcSound[tableIdx].soundID[ANIM_ITEM_DEFENSE] < 0 ) return;

		soundID = GetEventSoundID( ANIM_ITEM_DEFENSE, m_pcSound[tableIdx].soundID[ANIM_ITEM_DEFENSE] );

		g_musicMng->PlaySample3D( soundID, GTH_SOUND_PLAY_ONCE , character->position );
	}
}


int CSoundMng::GetNormalSoundID( int idx )
{
	return( m_soundResource.normalID[idx].soundID );
}

int CSoundMng::GetItemSoundID( int idx )
{
	return( m_soundResource.itemID[idx].soundID );
}

int CSoundMng::GetEventSoundID( int event, int idx )
{
	return( m_soundResource.eventID[event][idx].soundID );
}