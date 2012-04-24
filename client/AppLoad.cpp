#include <windows.h>
#include <basetsd.h>
#include <tchar.h>

#include "global_def.h"
#include "resource.h"

#include "GTH.h"



int GTH::LoadShaders()
{
	packsearch_t *list , *tmpList;
	int  i , size , ret;
	int   success;
	int   numShaders; 

	success = 0;
	numShaders = 0;
	size = g_fileMng.ReadExtFilesFromPack( "shader" , &list );
	tmpList = list;
	for( i = 0 ; i < size ; i++ , tmpList++ )
		numShaders += g_shaderMng.GetShaderCount( tmpList->name );
	
	g_shaderMng.Init( numShaders );
	for( i = 0 ; i < size ; i++ , list++ )
	{
		ret = g_shaderMng.ReadShaderScript( list->name );
		if( !ret )
		{
			GTH_ERROR( GTH_ERROR_PRINT , "There is an error in loading shader file" );
			continue;
		}

		success++;
	}

	if( !success )  return false;

	success = 0;
	numShaders = 0;
	size = g_fileMng.ReadExtFilesFromPack( "model.shader" , &list );
	tmpList = list;
	for( i = 0 ; i < size ; i++ , tmpList++ )
		numShaders += g_modelShaderMng->GetShaderCount( tmpList->name );

	g_modelShaderMng->Init( numShaders );
	for( i = 0 ; i < size ; i++ , list++ )
	{
		ret = g_modelShaderMng->ReadShaderScript( list->name );
		if( !ret )
		{
			GTH_ERROR( GTH_ERROR_PRINT , "There is an error in loading shader file" );
			continue;
		}

		success++;
	}

	if( !success )  return true;

	success = 0;
	numShaders = 0;
	size = g_fileMng.ReadExtFilesFromPack( "character.shader" , &list );
	tmpList = list;
	for( i = 0 ; i < size ; i++ , tmpList++ )
		numShaders += g_charShaderMng->GetShaderCount( tmpList->name );

	g_charShaderMng->Init( numShaders );
	for( i = 0 ; i < size ; i++ , list++ )
	{
		ret = g_charShaderMng->ReadShaderScript( list->name );
		if( !ret )
		{
			GTH_ERROR( GTH_ERROR_PRINT , "There is an error in loading shader file" );
			continue;
		}

		success++;
	}

	if( !success )  return true;

	return true;
}

int GTH::LoadMap( char *pMap )
{
	CBSP *world;

	world = new CBSP();
	if( !world )
	{
		GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "GTH::LoadMap::world" );
		return false;
	}

	
	
	
	
	
	world->SetID( m_numWorldBSPs );
	

	if( !world->Load( pMap ) )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "could not load world" );
		return false;
	}

	m_worldBSPs[ m_numWorldBSPs++ ] = world;

	return true;
}

int GTH::LoadMDLClasses( mapinfo_t *mapInfo )
{
	CMDLMng   *MDLMng;

	if( m_MDLMng )
		m_MDLMng->Release();
	GTH_SAFE_DELETE( m_MDLMng );

	MDLMng = new CMDLMng;
	if( !MDLMng )
	{
		GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "GTH::LoadMDLClasses::MDLMng" );
		return false;
	}

	MDLMng->SetFileMng( &g_fileMng );
	MDLMng->SetRenderer( &g_d3dRender );
	MDLMng->SetScript( &g_script );
	MDLMng->SetShaderMng( g_modelShaderMng );
	MDLMng->SetTexMng( &g_texMng );
	MDLMng->SetTimer( &g_timer );
	MDLMng->SetDevice( m_pd3dDevice );
	MDLMng->SetMusicMng( g_musicMng );
	
	if( !MDLMng->LoadWorldClass( mapInfo->modelName ) )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "could not load model class" );
		return false;
	}
	if( !MDLMng->LoadItemClass( mapInfo->itemName ) )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "could not load model class" );
		return false;
	}
	if( !MDLMng->LoadEffectClass( mapInfo->effectName ) )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "could not load model class" );
		return false;
	}

	m_MDLMng = MDLMng;

	g_charManager->SetMDLMng( m_MDLMng );

	return true;
}


int GTH::LoadEntity( char *objectfName )
{
	if( !g_entityMng->LoadEntity( objectfName ) )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "could not load model entity object" );
		return false;
	}

	g_entityMng->SetMDLMng( m_MDLMng );
	g_itemMng->SetMDLMng( m_MDLMng );
	g_effectMng->SetMDLMng( m_MDLMng );

	return true;
}


int GTH::LoadConfig( char *fname )
{
	char *token;
	int   numBrace;
	int   spawnId;
	int i;
	int red , green , blue;

	if( !g_script.Load( fname ) )
		return false;

	g_script.BeginParse();

	 
	memset ( m_mapInfos, 0, sizeof(m_mapInfos) );

	m_numMapInfos = 0;
	numBrace = 0;
	while( 1 )
	{
		token = g_script.GetNextToken( true );
		if( !token[0] ) break;

		if( token[0] == '{' )
		{
			numBrace++;
			m_mapInfos[ m_numMapInfos ].worldVisible  = 30000.0f;
			m_mapInfos[ m_numMapInfos ].modelVisible  = 10000.0f;
			m_mapInfos[ m_numMapInfos ].itemVisible   = 5000.0f;
			m_mapInfos[ m_numMapInfos ].effectVisible = 5000.0f;
			m_mapInfos[ m_numMapInfos ].numSpawnPos = 0;
			m_mapInfos[ m_numMapInfos ].numSpawnAngles = 0;
			VectorSet( m_mapInfos[ m_numMapInfos ].sunAngle , 1.0f , 0.5f , -0.5f );
			VectorSet( m_mapInfos[ m_numMapInfos ].sunColor , 1.0f , 1.0f , 1.0f );
			VectorNormalize( m_mapInfos[ m_numMapInfos ].sunAngle );
		}
		else if( !stricmp( token , "world" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			strcpy( m_mapInfos[ m_numMapInfos ].mapName , token );
		}
		else if( !stricmp( token , "model" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			strcpy( m_mapInfos[ m_numMapInfos ].modelName , token );
		}
		else if( !stricmp( token , "item" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			strcpy( m_mapInfos[ m_numMapInfos ].itemName , token );
		}
		else if( !stricmp( token , "effect" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			strcpy( m_mapInfos[ m_numMapInfos ].effectName , token );
		}
		else if( !stricmp( token , "object" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			strcpy( m_mapInfos[ m_numMapInfos ].objectName , token );
		}
		else if( !stricmp( token , "position" ) )
		{
			spawnId = m_mapInfos[ m_numMapInfos ].numSpawnPos;
			if( spawnId >= GTH_MAX_SIZE_MAP_SPAWN_POS ) 
			{
				g_script.SkipLine();
				continue;
			}

			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].spawnPos[ spawnId ][0] = (float)atof( token );
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].spawnPos[ spawnId ][1] = (float)atof( token );
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].spawnPos[ spawnId ][2] = (float)atof( token );
			m_mapInfos[ m_numMapInfos ].numSpawnPos++;
		}
		else if( !stricmp( token , "angle" ) )
		{
			spawnId = m_mapInfos[ m_numMapInfos ].numSpawnAngles;
			if( spawnId >= GTH_MAX_SIZE_MAP_SPAWN_POS ) 
			{
				g_script.SkipLine();
				continue;
			}

			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].spawnAngle[ spawnId ] = (float)atof( token );
			m_mapInfos[ m_numMapInfos ].numSpawnAngles++;
		}
		else if( !stricmp( token , "worldVisible" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].worldVisible = (float)atof( token );
		}
		else if( !stricmp( token , "modelVisible" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].modelVisible = (float)atof( token );
		}
		else if( !stricmp( token , "itemVisible" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].itemVisible = (float)atof( token );
		}
		else if( !stricmp( token , "effectVisible" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].effectVisible = (float)atof( token );
		}
		else if( !stricmp( token , "bgmusic" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			strcpy( m_mapInfos[ m_numMapInfos ].BGMusic , token );
		}
		else if( !stricmp( token , "sunangle" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].sunAngle[ YAW ] = (float)atof( token );
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].sunAngle[ PITCH ] = (float)atof( token );
			m_mapInfos[ m_numMapInfos ].sunAngle[ ROLL ] = 0.0f;

			m_mapInfos[ m_numMapInfos ].sunAngle[ YAW ] += 180.0f;
			if( m_mapInfos[ m_numMapInfos ].sunAngle[ YAW ] > 360.0f )
				m_mapInfos[ m_numMapInfos ].sunAngle[ YAW ] -= 360.0f;
			AngleToVector( m_mapInfos[ m_numMapInfos ].sunAngle , m_mapInfos[ m_numMapInfos ].sunAngle );
		}
		else if( !stricmp( token , "suncolor" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].sunColor[0] = (float)atof( token );
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].sunColor[1] = (float)atof( token );
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].sunColor[2] = (float)atof( token );
		}
		else if( !stricmp( token , "fogcolor" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			red = atoi( token );
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			green = atoi( token );
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			blue = atoi( token );

			m_mapInfos[ m_numMapInfos ].fogColor = MAKE_COLOR_ARGB( 0x00 , red , green , blue );
			
			
			
			m_mapInfos[ m_numMapInfos ].fogColor = 0;
		}
		else if( !stricmp( token , "fogStart" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].fogStart = (float)atof( token );

		}
		else if( !stricmp( token , "fogEnd" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  
				return false;
			m_mapInfos[ m_numMapInfos ].fogEnd = (float)atof( token );

		}
		 
		else if	(	! stricmp ( token, "monsterKind" )	)
		{
			token	=	g_script .GetNextToken (true);
			if	(	! token	)	
				return	false;
			int	monsterKind	=	(int)atoi ( token );
			if	(	monsterKind	<=	0	)	
				return	false;
			m_mapInfos [m_numMapInfos].monsterKind	=	monsterKind;
			for	(	int	index	=	0;	\
						index	<	monsterKind;	\
						++index	)
			{
				token	=	g_script .GetNextToken ( true );
				if	(	! token	)	
					return	false;
				m_mapInfos [m_numMapInfos].monsters [index]	=	atoi ( token );
			}
		


			
			
			CSkillTransformData	*pTransList = g_SkillTransInfo.Get();
			int size = pTransList->m_TransData.size();
			for(int i=0;i<size;i++)
			{
				m_mapInfos [m_numMapInfos].monsters[index + i] = pTransList->m_TransData[i]->m_nTransformEntityIdx;
			}
			m_mapInfos [m_numMapInfos].monsterKind += size;
			
			

		}
			
		 
		else if	(	! stricmp ( token , "weather" )	)
		{
			token	=	g_script .GetNextToken (true);
			if	(	! token	)	
				return	false;
			if	(	! stricmp ( token, "rain" )	)
			{
				m_mapInfos [m_numMapInfos].weather	=	TIME_RAIN;
			}
			else if	(	! stricmp ( token, "snow" )	)
			{
				m_mapInfos [m_numMapInfos].weather	=	TIME_SNOW;
			}
			else
			{
				m_mapInfos [m_numMapInfos].weather	=	TIME_DAY;
			}

		}
		 
		else if	(	! stricmp( token, "bgvolume" )	)
		{
			token	=	g_script.GetNextToken( true );
			if	(	! token	)	
				return	false;
			m_mapInfos[ m_numMapInfos ].bgVolume	=	atoi( token );
		}
		else if	(	! stricmp( token, "effectvolume" )	)
		{
			token	=	g_script.GetNextToken( true );
			if	(	! token	)	
				return	false;
			m_mapInfos[ m_numMapInfos ].effectVolume	=	atoi( token );
		}

		else if( token[0] == '}' )
		{
			if( m_mapInfos[ m_numMapInfos ].numSpawnAngles < m_mapInfos[ m_numMapInfos ].numSpawnPos )
			{
				for( i = m_mapInfos[ m_numMapInfos ].numSpawnAngles ; i < m_mapInfos[ m_numMapInfos ].numSpawnPos ; i++ )
					m_mapInfos[ m_numMapInfos ].spawnAngle[ i ] = 0;
			}

			if( --numBrace )  break;
			m_numMapInfos++;
		}
		else
			g_script.SkipLine();
	}

	if( !m_numMapInfos )
		return false;

	return true;
}


