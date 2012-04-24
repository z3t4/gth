


#include "../Global.h"


extern char *g_lTableToken[];

CGameConfigFlag gGameConfigFlag;





void	CGameConfigFlag::SetFlag()
{
	m_nFlag=0;

	
	if(m_bPrecocity_Time)			m_nFlag +=1 << 1;

	
	if(m_bDie_Reduce_panality)		m_nFlag +=1 << 2;

	
	if(m_bMap_Attr_Ratio)			m_nFlag +=1 << 3;

	
	if(m_bItem_Upgrade_Limit)		m_nFlag +=1 << 4;

	
	if(m_bItem_Craft_Limit)			m_nFlag +=1 << 5;

	
	if(m_bWorld_Chat_Limit)			m_nFlag +=1 << 6;

	return ;
}

void CGameConfigFlag::SetFlag(int flag)
{
	m_nFlag = flag;

	int f=0;

	if(flag & (1 << 1))		m_bPrecocity_Time=TRUE;
	if(flag & (1 << 2))		m_bDie_Reduce_panality=TRUE;
	if(flag & (1 << 3))		m_bMap_Attr_Ratio=TRUE;
	if(flag & (1 << 4))		m_bItem_Upgrade_Limit=TRUE;
	if(flag & (1 << 5))		m_bItem_Craft_Limit=TRUE;
	if(flag & (1 << 6))		m_bWorld_Chat_Limit=TRUE;
}

int	CGameConfigFlag::LoadFlagFile()
{
	FILE *fp;
	int size;
	byte *buffer;
	char *token;
	
	char filename[128];

	sprintf( filename, "flag%s.cfg", g_lTableToken[g_config.languageType] );
	fp = fopen(filename, "rb" );

	if( !fp )   
	{
		MessageBox(NULL, "does not exist 'flag.cfg' file", "Error", MB_ICONHAND|MB_OK);
		return false;
	}

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	curpos = (char *)buffer;
	endpos = curpos + size;
	
	while ((token = NextToken()) != NULL)
    {
		if( !stricmp( token, "CHARACTER_STORAGE" ) )
		{
			token = NextArg();
			if(!stricmp( token, "TRUE" ))			gGameConfigFlag.m_bCharacter_Storage = TRUE;
			else if (!stricmp( token, "FALSE" ))	gGameConfigFlag.m_bCharacter_Storage = FALSE;
		}
		else if( !stricmp( token, "PRECOCITY_TIME" ) )
		{
			token = NextArg();
			if(!stricmp( token, "TRUE" ))			gGameConfigFlag.m_bPrecocity_Time = TRUE;
			else if (!stricmp( token, "FALSE" ))	gGameConfigFlag.m_bPrecocity_Time = FALSE;
		}

		else if( !stricmp( token, "DIE_REDUCE_PANALTY" ) )
		{
			token = NextArg();
			if(!stricmp( token, "TRUE" ))			gGameConfigFlag.m_bDie_Reduce_panality = TRUE;
			else if (!stricmp( token, "FALSE" ))	gGameConfigFlag.m_bDie_Reduce_panality = FALSE;
		}

		else if( !stricmp( token, "MAP_ATTR_RATIO" ) )
		{
			token = NextArg();
			if(!stricmp( token, "TRUE" ))			gGameConfigFlag.m_bMap_Attr_Ratio = TRUE;
			else if (!stricmp( token, "FALSE" ))	gGameConfigFlag.m_bMap_Attr_Ratio = FALSE;
		}


		else if( !stricmp( token, "ITEM_UPGRADE_LIMIT" ) )
		{
			token = NextArg();
			if(!stricmp( token, "TRUE" ))			gGameConfigFlag.m_bItem_Upgrade_Limit = TRUE;
			else if (!stricmp( token, "FALSE" ))	gGameConfigFlag.m_bItem_Upgrade_Limit = FALSE;
		}

		else if( !stricmp( token, "ITEM_CRAFT_LIMIT" ) )
		{
			token = NextArg();
			if(!stricmp( token, "TRUE" ))			gGameConfigFlag.m_bItem_Craft_Limit = TRUE;
			else if (!stricmp( token, "FALSE" ))	gGameConfigFlag.m_bItem_Craft_Limit = FALSE;
		}


		else if( !stricmp( token, "WORLD_CHAT_LIMIT" ) )
		{
			token = NextArg();
			if(!stricmp( token, "TRUE" ))			gGameConfigFlag.m_bWorld_Chat_Limit = TRUE;
			else if (!stricmp( token, "FALSE" ))	gGameConfigFlag.m_bWorld_Chat_Limit = FALSE;
		}
	}

	fclose(fp);

	SetFlag();

	
	if(NULL  != buffer){ 
		delete []buffer;
		buffer=NULL; 
	}
	return true;
}


