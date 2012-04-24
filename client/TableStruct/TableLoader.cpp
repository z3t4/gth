 #include "../global_def.h"
#include "../ijl.h"



int	LoadItemClassTable( char* filename )
{
	BYTE *buf, *buf2;

	buf2 = g_fileMng.ReadFileFromPack( filename );
	buf = buf2;

	tableHeader_t header;

	memset( &header, 0, sizeof( tableHeader_t ) );

	memcpy( &header, buf, sizeof( header ) );
	buf += sizeof( header );

	if( strcmp( header.id, ITEM_CLASS_TABLE_HEADER ) )
	{
		return( 0 );
	}
	if( header.version != ITEM_CLASS_TABLE_VERSION_103 )
	{
		return( 0 );
	}
	g_nItemClassTable = header.recordNumber;
	
	for( int i = 0; i < g_nItemClassTable; i ++ )
	{
		memcpy( &g_itemClassTable[i], buf, sizeof( itemClassTable_t ) );
		buf += sizeof( itemClassTable_t );
	}

	 

	GTH_SAFE_FREE ( buf2 );
	
	return( 1 );
}



int LoadItemTable( char* filename )
{
	BYTE *buf, *buf2;

	buf2 = g_fileMng.ReadFileFromPack( filename );
	buf = buf2;

	tableHeader_t header;

	memset( &header, 0, sizeof( tableHeader_t ) );

	memcpy( &header, buf, sizeof( header ) );
	buf += sizeof( header );

	if( strcmp( header.id, ITEM_TABLE_HEADER ) )
	{
		return( 0 );
	}
	if( header.version != ITEM_TABLE_VERSION )
	{
		return( 0 );
	}
	g_nItemTable = header.recordNumber;
	
	int size; 
	memcpy( &size, buf, sizeof( size ) );
	buf += sizeof( size );
	if(size !=  sizeof(itemTable_t))
	{
		return( 0 );
	}

	int TypeSize; 
	memcpy( &TypeSize, buf, sizeof( TypeSize ) );
	buf += sizeof( TypeSize );


	char txt[256];
	for( int i = 0; i < g_nItemTable; i ++ )
	{
		memcpy( &g_itemTable[i], buf, sizeof( itemTable_t ) );

		strcpy(g_itemTable[i].name,g_LPACK.Get(LPACK_TYPE_ITEM,g_itemTable[i].NameIdx));
		
		strcpy(txt,g_LPACK.Get(LPACK_TYPE_ITEM,g_itemTable[i].DocumentIdx));
		if(strcmp(txt,"empty"))
		{
			strcpy(g_itemTable[i].document,txt);
		}

		buf += sizeof( itemTable_t );
	}
	
	GTH_SAFE_FREE ( buf2 );
	
	return( 1 );

}


int LoadItemGenerateTable( char* filename )
{
	BYTE *buf, *buf2;

	buf2 = g_fileMng.ReadFileFromPack( filename );
	buf = buf2;

	tableHeader_t header;

	memset( &header, 0, sizeof( tableHeader_t ) );

	memcpy( &header, buf, sizeof( header ) );
	buf += sizeof( header );

	if( strcmp( header.id, ITEM_GENERATE_TABLE_HEADER ) )
	{
		return( 0 );
	}
	if( header.version != ITEM_GENERATE_TABLE_VERSION )
	{
		return( 0 );
	}
	g_nItemGenerateTable = header.recordNumber;
	
	for( int i = 0; i < g_nItemGenerateTable; i ++ )
	{
		memcpy( &g_itemGenerateTable[i], buf, sizeof( itemGenerateTable_t ) );
		buf += sizeof( itemGenerateTable_t );
	}

	
	GTH_SAFE_FREE ( buf2 );
	
	return( 1 );

}

int LoadMonsterTable( char* filename )
{
	BYTE *buf, *buf2;

	buf2 = g_fileMng.ReadFileFromPack( filename );
	buf = buf2;

	tableHeader_t header;

	memset( &header, 0, sizeof( tableHeader_t ) );

	memcpy( &header, buf, sizeof( header ) );
	buf += sizeof( header );

	if( strcmp( header.id, MONSTER_TABLE_HEADER ) )
	{
		return( 0 );
	}
	if( header.version != MONSTER_TABLE_VERSION )
	{
		return( 0 );
	}
	g_nMonsterTable = header.recordNumber;
	
	for( int i = 0; i < g_nMonsterTable; i ++ )
	{
		memcpy( &g_monsterTable[i], buf, sizeof( monsterBasicTable_t ) );
		buf += sizeof( monsterBasicTable_t );
	}
	
	GTH_SAFE_FREE ( buf2 );
	
	return( 1 );
}


int LoadSkillTable( char *filename )
{	


	return( 1 );
}

void LoadExpTableForClient( char* filename )
{
	int level;
	int expLevel=0;
	char* token;

	g_script.Load( filename );
	g_script.BeginParse();

	for( int i = 0; i < MAX_NUMBER_OF_LEVEL; i ++ )
		memset( &g_expTable[i], 0, sizeof( expTable_t ) );

	while( 1 )
	{
		token = g_script.GetNextToken( true );
		if( token[0] == 0 ) break;
		level = atoi( token );
		
		token = g_script.GetNextToken( true );
		g_expTable[expLevel].totalexp = _atoi64( token );
		token = g_script.GetNextToken( true );
		g_expTable[expLevel].exp = _atoi64( token );

		expLevel++;
	}
}

void LoadGenExpTableForClient( char* filename )
{
	int level;
	int genExpLevel=0;
	char* token;

	g_script.Load( filename );
	g_script.BeginParse();

	while( 1 )
	{
		token = g_script.GetNextToken( true );
		if( token[0] == 0 ) break;
		level = atoi( token );
		
		token = g_script.GetNextToken( true );
		g_expTable[genExpLevel].totalGenExp = _atoi64( token );
		token = g_script.GetNextToken( true );
		g_expTable[genExpLevel].genExp = _atoi64( token );

		genExpLevel++;
	}
}

void GetActionTableData(actionTable_t *table,int id,char* token)
{
	table->id = id;

	token = g_script.GetNextToken( true );
	table->iconIdx = atoi( token );

	
	token = g_script.GetNextToken( true );
	table->nDescIdx = atoi( token );

	token = g_script.GetNextToken( true );
	table->nLPackID = atoi( token );

	
	token = g_script.GetNextToken( true );
	table->nFreeDescIdx = atoi( token );

	token = g_script.GetNextToken( true );
	table->nFreeLPackID = atoi( token );
}

void LoadActionTable( char *filename )
{
	char* token;

#define ACTION_TABLE_VERSION 100

	g_script.Load( filename );
	g_script.BeginParse();

	for( int i = 0; i < MAX_NUMBER_OF_ACTION; i ++ )
	{
		memset( &g_actionTable[i], 0, sizeof( actionTable_t ) );
		g_actionTable[i].id = -1;
	}

	int Version =0;
	while( 1 )
	{
		token = g_script.GetNextToken( true );
		if( token[0] == 0 ) break;

		if( !stricmp( token, "VERSION" ) )
		{
			token = g_script.GetNextToken( true );
			Version = atoi( token );
		}		
		else if( !stricmp( token, "ATTACK" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_ATTACK],ACTION_ATTACK,token);
		}
		else if( !stricmp( token, "ASSIST" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_ASSIST],ACTION_ASSIST,token);
		} 
		else if( !stricmp( token, "FOLLOW" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_FOLLOW],ACTION_FOLLOW,token);			
		} 
		else if( !stricmp( token, "PICKUP_ITEM" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_PICKUP_ITEM],ACTION_PICKUP_ITEM,token);
		} 
		else if( !stricmp( token, "EPACK_RA" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_EPACK_RA],ACTION_EPACK_RA,token);
		} 
		else if( !stricmp( token, "EPACK_SA" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_EPACK_SA],ACTION_EPACK_SA,token);
		} 
		else if( !stricmp( token, "SIT" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_SIT],ACTION_SIT,token);
		} 
		else if( !stricmp( token, "TRADE" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_TRADE],ACTION_TRADE,token);
		} 
		else if( !stricmp( token, "PARTY_INVITE" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_PARTY_INVITE],ACTION_PARTY_INVITE,token);
		} 
		else if( !stricmp( token, "PARTY_OUT" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_PARTY_OUT],ACTION_PARTY_OUT,token);
		} 
		else if( !stricmp( token, "PARTY_FORCEDOUT" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_PARTY_FORCEDOUT],ACTION_PARTY_FORCEDOUT,token);
		} 
		else if( !stricmp( token, "PARTY_DISORGANIZE" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_PARTY_DISORGANIZE],ACTION_PARTY_DISORGANIZE,token);
		} 
		
		else if( !stricmp( token, "MAP" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_MAP],ACTION_MAP,token);
		}
		else if( !stricmp( token, "BOOTH" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_BOOTH],ACTION_BOOTH,token);
		}
		
		else if( !stricmp( token, "FRIEND_MAKE" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_FRIEND_MAKE],ACTION_FRIEND_MAKE,token);
		}
		else if( !stricmp( token, "FRIEND_BYE" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_FRIEND_BYE],ACTION_FRIEND_BYE,token);
		}
		
		else if( !stricmp( token, "SHOUT" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_SHOUT],ACTION_SHOUT,token);
		}
		else if( !stricmp( token, "BOW" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_BOW],ACTION_BOW,token);
		}
		
		else if( !stricmp( token, "CHATTING" ) )
		{
			GetActionTableData(&g_actionTable[ACTION_CHATTING],ACTION_CHATTING,token);
		}

		else if( !stricmp(token, "REGISTHELPER") )
		{
			GetActionTableData(&g_actionTable[ACTION_REGIST_HELPER],ACTION_REGIST_HELPER,token);
		}
		else if(!stricmp(token, "GIVEPOINT"))
		{
			GetActionTableData(&g_actionTable[ACTION_GIVE_POINT],ACTION_GIVE_POINT,token);
		}

		
		
	
		
	}

	if(Version != ACTION_TABLE_VERSION)
	{
		MessageBox(NULL,"Action Table Version ... Error! ",NULL,MB_OK);
	}
}


int LoadGenCapabilityTable( char *filename )
{
	char* token;
	int idx, level, genCapability, limitGenLevel, type;

	g_script.Load( filename );
	g_script.BeginParse();

	for( int i = 0; i < 100; i ++ )
		memset( &g_skillGradeClassTable[i], 0, sizeof( skillGradeClass_t ) );

	while( 1 )
	{
		token = g_script.GetNextToken( true );
		if( token[0] == NULL ) break;

		if( !stricmp( token, "SKILL_CLASS" ) )
		{
			token = g_script.GetNextToken( true );
			idx = atoi( token );
			type = 0;
		} 
		else if( !stricmp( token, "MONSTER_GEN_CAPABILITY" ) )
		{
			type = 1;
		}
		else if( token[0] == '{' )
		{
			while ( 1 )
			{
				token = g_script.GetNextToken( true );
				if( token[0] == '}' ) break;

				level = atoi( token );
				token = g_script.GetNextToken( true );
				
				if( type == 0 )
				{
					genCapability = atoi( token );
					token = g_script.GetNextToken( true );
					limitGenLevel = atoi( token );
					g_skillGradeClassTable[idx].genCapability[level] = genCapability;
					g_skillGradeClassTable[idx].limitGenLevel[level] = limitGenLevel;
				}
				else
				{
					g_genCapabilityTable.genCapability[level] = atoi( token );
				}
			}
		}
	}


	return( 1 );
}




int LoadItemCraftTableForServer( char* filename )
{

	char* token;
	int  numSourceItem, numRequireSkill, numResultItem, numOptionIdx;

	g_script.Load( filename );
	g_script.BeginParse();

	for( int i = 0; i < MAX_NUMBER_OF_ITEM_CRAFT_TABLE; i ++ )
		memset( &g_itemCraftTable[i], 0, sizeof( itemCraftTable_t ) );

	g_itemCraftTableNumber = 0;

	itemCraftTable_t		TempItemCraftTable;
	
	while( 1 )
	{
		token = g_script.GetNextToken( true );
		if( token[0] == NULL ) break;

		memset(&TempItemCraftTable,0,sizeof(itemCraftTable_t));

		
		TempItemCraftTable.m_nMembership_type=-1;

		if( token[0] == '{' )
		{
			while ( 1 )
			{
				token = g_script.GetNextToken( true );
				if( token[0] == '}' ) break;
				if( token[0] == NULL ) break;

				if( !stricmp( token, "craft_grade" ) )
				{
					token = g_script.GetNextToken( true );
					TempItemCraftTable.craftGrade = atoi( token );
				}
				else if( !stricmp( token, "success_probability" ) )
				{
					token = g_script.GetNextToken( true );
					TempItemCraftTable.successProbability = atof( token );
				}
				else if( !stricmp( token, "craft_cost" ) )
				{
					token = g_script.GetNextToken( true );
					TempItemCraftTable.craftCost = atoi( token );
				}
				else if( !stricmp( token, "craft_special_type" ) )
				{
					token = g_script.GetNextToken( true );
					TempItemCraftTable.specialType = atoi( token );
				}
				
				
				else if( !stricmp( token, "craft_event_index" ) )
				{
					token = g_script.GetNextToken( true );
					TempItemCraftTable.m_lEventIndex = atoi( token );
				}

				
				else if( !stricmp( token, "event_type" ) )
				{
					token = g_script.GetNextToken( true );
					TempItemCraftTable.m_btEventType = atoi( token );
				}
				
				
				else if( !stricmp( token, "craft_cheat_name" ) )
				{
					token = g_script.GetNextToken( true );

					int len = strlen(token);
					if(len > 20)
					{
						wsprintf(TempItemCraftTable.m_chCheatName,"NULL");
					}
					else
					{
						
						
						wsprintf(TempItemCraftTable.m_chCheatName,"%s",token);
					}
				}
				
				

				else if( !stricmp( token, "korea" ) )
				{
					token = g_script.GetNextToken( true );		
					while( 1 )
					{
						token = g_script.GetNextToken( true );
						if( token[0] == '}' ) break;
						if( token[0] == '{' )
						{
							while( 1 )
							{
								token = g_script.GetNextToken( true );
								if( token[0] == '}' ) break;

								if( !stricmp( token, "membership_type" ) )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.m_nMembership_type = atoi( token );
								}
							}
						}
					}
				}

				
				else if( !stricmp( token, "result_etc" ) )
				{
					token = g_script.GetNextToken( true );		
					while( 1 )
					{
						token = g_script.GetNextToken( true );
						if( token[0] == '}' ) break;
						if( token[0] == '{' )
						{
							while( 1 )
							{
								token = g_script.GetNextToken( true );
								if( token[0] == '}' ) break;

								if( !stricmp( token, "nak" ) )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.m_lResultNak = atoi( token );
								}
								else if( !stricmp( token, "exp" ) )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.m_lResultExp = atoi( token );
								}
								else if( !stricmp( token, "icon" ) )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.m_nResultMark = atoi( token );
								}		
							}
						}
					}
				}
				
				else if( !stricmp( token, "source_item" ) )
				{
					numSourceItem = 0;
					token = g_script.GetNextToken( true );		
				
					while( 1 )
					{
						token = g_script.GetNextToken( true );
						if( token[0] == '}' ) break;
						if( token[0] == '{' )
						{
							numOptionIdx = 0;
							while( 1 )
							{
								token = g_script.GetNextToken( true );
								if( token[0] == '}' ) break;

								if( !stricmp( token, "index" ) )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.sourceItem[numSourceItem].itemIndex = atoi( token );
								}
								else if( !stricmp( token, "number" ) )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.sourceItem[numSourceItem].itemCount = atoi( token );
								}
								else if( !stricmp( token, "option_index" ) )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionIndex[numOptionIdx] = atoi( token );
									numOptionIdx ++;
								}
								else if( !stricmp( token, "above_option_number" ) )
								{
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionCountType = 0;
									token = g_script.GetNextToken( true );
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionCount = atoi( token );
								}
								else if( !stricmp( token, "equal_option_number" ) )
								{
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionCountType = 1;
									token = g_script.GetNextToken( true );
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionCount = atoi( token );
								}
								else if( !stricmp( token, "below_option_number" ) )
								{
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionCountType = 2;
									token = g_script.GetNextToken( true );
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionCount = atoi( token );
								}
							}
							TempItemCraftTable.sourceItem[numSourceItem].numReqOptionIndex = numOptionIdx;
							numSourceItem ++;
						}
					}
					TempItemCraftTable.numSourceItem = numSourceItem;
				}
				
				else if( !stricmp( token, "require_skill" ) )
				{
					numRequireSkill = 0;
					token = g_script.GetNextToken( true );		
					while( 1 )
					{
						token = g_script.GetNextToken( true );
						if( token[0] == '}' ) break;
						if( token[0] == '{' )
						{
							while( 1 )
							{
								token = g_script.GetNextToken( true );
								if( token[0] == '}' ) break;

								if( !stricmp( token, "index" ) )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.requireSkill[numRequireSkill].skillIndex = atoi( token );
								}
								else if( !stricmp( token, "above_level" ) )
								{
									TempItemCraftTable.requireSkill[numRequireSkill].reqSkillLevelType = 0;
									token = g_script.GetNextToken( true );
									TempItemCraftTable.requireSkill[numRequireSkill].reqSkillLevel = atoi( token );
								}
								else if( !stricmp( token, "equal_level" ) )
								{
									TempItemCraftTable.requireSkill[numRequireSkill].reqSkillLevelType = 1;
									token = g_script.GetNextToken( true );
									TempItemCraftTable.requireSkill[numRequireSkill].reqSkillLevel = atoi( token );
								}
								else if( !stricmp( token, "below_level" ) )
								{
									TempItemCraftTable.requireSkill[numRequireSkill].reqSkillLevelType = 2;
									token = g_script.GetNextToken( true );
									TempItemCraftTable.requireSkill[numRequireSkill].reqSkillLevel = atoi( token );
								}
							}
							numRequireSkill ++;
						}
					}
					TempItemCraftTable.numRequireSkill = numRequireSkill;
				}
				
				else if( !stricmp( token, "result_item" ) )
				{
					numResultItem = 0;
					token = g_script.GetNextToken( true );		
					while( 1 )
					{
						token = g_script.GetNextToken( true );
						if( token[0] == '}' ) break;
						if( token[0] == '{' )
						{
							
							
							TempItemCraftTable.resultItem[numResultItem].optionGroupIdx = -1;

							while( 1 )
							{
								token = g_script.GetNextToken( true );
								if( token[0] == '}' ) break;

								if( !stricmp( token, "index" ) )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.resultItem[numResultItem].itemIndex = atoi( token );
								}
								else if( !stricmp( token, "number" ) )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.resultItem[numResultItem].itemCount = atoi( token );
								}
								
								
								else if( !stricmp( token, "option_grade") )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.resultItem[numResultItem].optionGrade = atoi( token );
								}
								else if( !stricmp( token, "rareitem_grade") )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.resultItem[numResultItem].rareItemGrade = atoi( token );
								}
								else if( !stricmp( token, "optiongroup") )
								{
									token = g_script.GetNextToken( true );
									TempItemCraftTable.resultItem[numResultItem].optionGroupIdx = atoi( token );
								}
							}
							numResultItem ++;
						}
					}
					TempItemCraftTable.numResultItem = numResultItem;
				}
			}

			if(TempItemCraftTable.specialType == CRAFT_TYPE_EVENT)
			{
				TempItemCraftTable.idx = g_ifMng->m_EventCraftWin->GetTableSize();

				g_ifMng->m_EventCraftWin->Push(TempItemCraftTable);
			}
			else
			{
				TempItemCraftTable.idx = g_itemCraftTableNumber;

				memcpy(&g_itemCraftTable[g_itemCraftTableNumber],&TempItemCraftTable,sizeof(itemCraftTable_t));
				g_itemCraftTableNumber ++;

				
			}
			
		}
	}
	return 1;
}



int LoadQuestScrollTable( char* filename )
{
	char token[512];
	int ret, idx = 0;

	for( int i = 0; i < MAX_NUMBER_OF_QUESTSCROLL_TABLE; i ++ )
	{
		memset( &g_questscrollTable[i], 0, sizeof( questscrollTable_t ) );
		g_questscrollTable[i].Index = -1;
	}

	g_questscrollTableNumber = 0;

	if ( !g_script.Load( filename ) )	return 0;
	g_script.BeginParse();

	while( 1 )
	{
		ret  = g_script.GetCSVToken( token );

		if ( ret == 1 )	break;

		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;

			while (1)
			{
				ret = g_script.GetCSVToken( token );
				if ( ret == 1 )	return 1;
				if ( ret == 2 )	break;
			}
			continue;
		}

		
		g_questscrollTable[idx].Index = atoi( token );

		ret = g_script.GetCSVToken( token );
		strcpy( g_questscrollTable[idx].name, token );

		if ( ret == 1 )	
		{
			g_questscrollTableNumber++;
			break;
		}
		if ( ret == 2 )	
		{
			idx++;
			g_questscrollTableNumber++;
			continue;
		}

		for (int j=0; j < ( sizeof( QSDATAFIELD ) / 4 ); j++)
		{
			ret = g_script.GetCSVToken( token );
			g_questscrollTable[idx].dataField[j] = atoi( token );

			if ( ret == 1 )	
			{
				g_questscrollTableNumber++;
				return 1;
			}
			if ( ret == 2 )	break;
		}

		ret = g_script.GetCSVToken( token );
		strcpy( g_questscrollTable[idx].description, token );
	
		idx++;
		g_questscrollTableNumber++;

	}

	return 1;
}


int LoadAutoQuestScrollTable( char* filename )
{
	char token[512];
	int ret, idx = 0;

	for( int i = 0; i < MAX_NUMBER_OF_AUTOQUESTSCROLL_TABLE; i ++ )
	{
		memset( &g_autoquestscrollTable[i], 0, sizeof( autoQuestScrollTable_t ) );
		g_autoquestscrollTable[i].Index = -1;
	}

	g_autoquestscrollTableNumber = 0;

	if ( !g_script.Load( filename ) )	return 0;
	g_script.BeginParse();

	while( 1 )
	{
		ret  = g_script.GetCSVToken( token );

		if ( ret == 1 )	break;

		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;

			while (1)
			{
				ret = g_script.GetCSVToken( token );
				if ( ret == 1 )	return 1;
				if ( ret == 2 )	break;
			}
			continue;
		}

		
		g_autoquestscrollTable[idx].Index = atoi( token );

		ret = g_script.GetCSVToken( token );
		strcpy( g_autoquestscrollTable[idx].name, token );

		if ( ret == 1 )	
		{
			g_autoquestscrollTableNumber++;
			break;
		}
		if ( ret == 2 )	
		{
			idx++;
			g_autoquestscrollTableNumber++;
			continue;
		}

		for (int j=0; j < ( sizeof( AUTOQSDATAFIELD ) / 4 ); j++)
		{
			ret = g_script.GetCSVToken( token );
			g_autoquestscrollTable[idx].dataField[j] = atoi( token );

			if ( ret == 1 )	
			{
				g_autoquestscrollTableNumber++;
				return 1;
			}
			if ( ret == 2 )	break;
		}
	
		idx++;
		g_autoquestscrollTableNumber++;

	}

	return 1;
}


int LoadOptionTable( char* filename )
{
	char			token[128];
	int				ret;
	optionTable_t	*pOpt;

	if ( !g_script.Load( filename ) )	return 0;
	g_script.BeginParse();

	for( int i = 0; i < MAX_COUNT_OPTION_TABLE; i ++ )
	{
		memset( &g_optionTable[i], 0, sizeof( optionTable_t ) );
		g_optionTable[i].idx = -1;
	}

	
	while( 1 )
	{
		ret = g_script.GetCSVToken( token );
		if( ret == CSV_EndOfLine ) break;
	}
	while( 1 )
	{
		
		ret						=	g_script.GetCSVToken( token );
		pOpt					=	&g_optionTable[ atoi( token ) ];
		pOpt->idx				=	atoi( token );
		
		ret						=	g_script.GetCSVToken( token );
		strcpy(	pOpt->name,	token );
		
		ret						=	g_script.GetCSVToken( token );
		strcpy(	pOpt->printing,	token );
		
		ret						=	g_script.GetCSVToken( token );

		
		for( i = 0; i < MAX_OPTION_GRADE; i ++ )
		{
			ret						=	g_script.GetCSVToken( token );

		}
		if( ret == CSV_EndOfFile ) break;
		if( ret == CSV_EndOfLine ) continue;

		return( false );
	}
	
	return true;
}


int MakeOptionTable( char* optionTableFilename, char* valueTableFilename, optionTable_t optionTable[] )
{
	char			token[128];
	int				ret;
	optionTable_t	*pOpt;

	if ( !g_script.Load( optionTableFilename ) )	return 0;
	g_script.BeginParse();

	for( int i = 0; i < MAX_COUNT_OPTION_TABLE; i ++ )
	{
		memset( &optionTable[i], 0, sizeof( optionTable_t ) );
		optionTable[i].idx = -1;
	}

	
	while( 1 )
	{
		ret = g_script.GetCSVToken( token );
		if( ret == CSV_EndOfLine ) break;
	}
	while( 1 )
	{
		
		ret						=	g_script.GetCSVToken( token );
		pOpt					=	&optionTable[ atoi( token ) ];
		pOpt->idx				=	atoi( token );
		
		ret						=	g_script.GetCSVToken( token );
		strcpy(	pOpt->name,	token );
		
		ret						=	g_script.GetCSVToken( token );
		strcpy(	pOpt->printing,	token );
		
		ret						=	g_script.GetCSVToken( token );
		pOpt->dropYN			=	atoi( token );
		
		ret						=	g_script.GetCSVToken( token );
		pOpt->fixationPro		=	atoi( token );
		
		ret						=	g_script.GetCSVToken( token );
		pOpt->optionType		=	atoi( token );
		
		for( i = 0; i < MAX_OPTION_ITEM_TYPE; i ++ )
		{
			ret						=	g_script.GetCSVToken( token );
			pOpt->itemType[i]		=	atoi( token );
		}
		
		
		pOpt->availableOn			=	0;

		if( pOpt->optionType == -1 )
		{
			for( int j = 0; j < MAX_OPTION_ITEM_TYPE; j ++ )
			{
				if( pOpt->itemType[j] < 0 ) break;
				if( pOpt->itemType[j] == 0 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_WEAPONS;
				if( pOpt->itemType[j] == 1 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_DAGGER;
				if( pOpt->itemType[j] == 2 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_1HSWORD;
				if( pOpt->itemType[j] == 3 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_2HSWORD;
				if( pOpt->itemType[j] == 4 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_BOW;
				if( pOpt->itemType[j] == 5 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_STAFF;
				if( pOpt->itemType[j] == 6 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_SPIRITOR;
				if( pOpt->itemType[j] == 7 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_CLAW;
				if( pOpt->itemType[j] == 8 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_ARMORS;
				if( pOpt->itemType[j] == 9 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_UPPER;
				if( pOpt->itemType[j] == 10 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_LOWER;
				if( pOpt->itemType[j] == 11 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_AIRPROTECTOR;
				if( pOpt->itemType[j] == 12 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_BODYPROTECTOR;
				if( pOpt->itemType[j] == 13 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_GLOVES;
				if( pOpt->itemType[j] == 14 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_BOOTS;
				if( pOpt->itemType[j] == 15 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_RING;
				if( pOpt->itemType[j] == 16 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_NECKLACE;
				if( pOpt->itemType[j] == 17 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_BRACELET;
				if( pOpt->itemType[j] == 18 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_EARRING;
				if( pOpt->itemType[j] == 19 ) pOpt->availableOn |= OPTION_AVAILABLE_ON_EPACK;
			}
		}
		else if( pOpt->optionType == 0 )
		{
			pOpt->availableOn |= 0xffffffff;
		}
		else if( pOpt->optionType == 1 )
		{
			pOpt->availableOn |= OPTION_AVAILABLE_ON_WEAPONS;
		}
		else if( pOpt->optionType == 2 )
		{
			pOpt->availableOn |= OPTION_AVAILABLE_ON_ARMORS;
		}
		else if( pOpt->optionType == 3 )
		{
			pOpt->availableOn |= OPTION_AVAILABLE_ON_RING;
			pOpt->availableOn |= OPTION_AVAILABLE_ON_NECKLACE;
			pOpt->availableOn |= OPTION_AVAILABLE_ON_BRACELET;
			pOpt->availableOn |= OPTION_AVAILABLE_ON_EARRING;
		}
		else if( pOpt->optionType == 4 )
		{
			pOpt->availableOn |= OPTION_AVAILABLE_ON_EPACK;
		}

		if( ret == CSV_EndOfFile ) break;
		if( ret == CSV_EndOfLine ) continue;

		return( false );
	}
	
	if ( !g_script.Load( valueTableFilename ) )	return false;
	g_script.BeginParse();

	
	char		valueName[64];
	int			optionStoneGrade;
	int			itemType;
	float		value[MAX_OPTION_GRADE];	

	while( 1 )
	{
		ret = g_script.GetCSVToken( token );
		if( ret == CSV_EndOfLine ) break;
	}
	while( 1 )
	{
		
		ret						=	g_script.GetCSVToken( token );
		
		ret						=	g_script.GetCSVToken( token );
		strcpy(	valueName,	token );
		
		ret						=	g_script.GetCSVToken( token );
		pOpt					=	&optionTable[ atoi( token ) ];
		
		ret						=	g_script.GetCSVToken( token );
		optionStoneGrade		=	atoi( token );
		
		ret						=	g_script.GetCSVToken( token );
		itemType				=	atoi( token );
		
		for( i = 0; i < MAX_OPTION_GRADE; i ++ )
		{
			ret						=	g_script.GetCSVToken( token );
			value[i]				=	atof( token );
		}

		
		if( optionStoneGrade == -1 ) 
		{
			for( i = 0; i < MAX_OPTIONSTONE_GRADE; i ++ )
			{
				strcpy( pOpt->optValue[i].value_item[itemType].valueName, valueName );
				if( itemType == 0 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_WEAPONS;
				if( itemType == 1 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_DAGGER;
				if( itemType == 2 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_1HSWORD;
				if( itemType == 3 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_2HSWORD;
				if( itemType == 4 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_BOW;
				if( itemType == 5 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_STAFF;
				if( itemType == 6 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_SPIRITOR;
				if( itemType == 7 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_CLAW;
				if( itemType == 8 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_ARMORS;
				if( itemType == 9 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_UPPER;
				if( itemType == 10 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_LOWER;
				if( itemType == 11 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_AIRPROTECTOR;
				if( itemType == 12 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_BODYPROTECTOR;
				if( itemType == 13 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_GLOVES;
				if( itemType == 14 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_BOOTS;
				if( itemType == 15 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_RING;
				if( itemType == 16 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_NECKLACE;
				if( itemType == 17 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_BRACELET;
				if( itemType == 18 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_EARRING;
				if( itemType == 19 ) pOpt->optValue[i].availableOn |= OPTION_AVAILABLE_ON_EPACK;

				for( int j = 0; j < MAX_OPTION_GRADE; j ++ )
				{
					pOpt->optValue[i].value_item[itemType].value[j]	=	value[j];
				}
			}
		}
		else
		{
			strcpy( pOpt->optValue[optionStoneGrade].value_item[itemType].valueName, valueName );
			if( itemType == 0 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_WEAPONS;
			if( itemType == 1 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_DAGGER;
			if( itemType == 2 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_1HSWORD;
			if( itemType == 3 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_2HSWORD;
			if( itemType == 4 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_BOW;
			if( itemType == 5 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_STAFF;
			if( itemType == 6 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_SPIRITOR;
			if( itemType == 7 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_CLAW;
			if( itemType == 8 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_ARMORS;
			if( itemType == 9 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_UPPER;
			if( itemType == 10 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_LOWER;
			if( itemType == 11 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_AIRPROTECTOR;
			if( itemType == 12 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_BODYPROTECTOR;
			if( itemType == 13 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_GLOVES;
			if( itemType == 14 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_BOOTS;
			if( itemType == 15 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_RING;
			if( itemType == 16 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_NECKLACE;
			if( itemType == 17 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_BRACELET;
			if( itemType == 18 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_EARRING;
			if( itemType == 19 ) pOpt->optValue[optionStoneGrade].availableOn |= OPTION_AVAILABLE_ON_EPACK;

			for( i = 0; i < MAX_OPTION_GRADE; i ++ )
			{
				pOpt->optValue[optionStoneGrade].value_item[itemType].value[i]	=	value[i];
			}
		}
		
		if( ret == CSV_EndOfFile ) break;
		if( ret == CSV_EndOfLine ) continue;

		return( false );
	}

	return true;
}



int LoadSummonMonsterTable( char *filename, SummonMonster_t summonTable[], int &summontablenumber)
{
	char				token[128];
	int					ret;

	int count = 0;		

	
	if ( !g_script.Load( filename ) )	return 0;
	g_script.BeginParse();

	for( int i = 0; i < MAX_NUMBER_OF_SUMMON_MONSTER_TABLE; i ++ )
	{
		memset( &summonTable[i], 0, sizeof( SummonMonster_t ) );		
		summonTable[i].idx				= -1;
		summonTable[i].SummonWorldIdx	= -1;
	}

	
	while( 1 )
	{
		ret = g_script.GetCSVToken( token );
		if( ret == CSV_EndOfLine ) break;
	}
	while( 1 )
	{		
		SummonMonster_t* pTable	= NULL;		

		if ( count > MAX_NUMBER_OF_SUMMON_MONSTER_TABLE)
			return false;

		pTable					= &summonTable[count];
		if ( pTable == NULL )	
			return false;
		
		ret						= g_script.GetCSVToken( token );

		
		ret						= g_script.GetCSVToken( token );
		pTable->idx				= atoi( token );

		
		ret						= g_script.GetCSVToken( token );		

		strcpy (pTable->doc, token);
		
		ret						= g_script.GetCSVToken( token );		
		strcpy (pTable->name, token);		

		
		ret						= g_script.GetCSVToken( token );		
		pTable->SummonTime = atof(token);

		
		ret						= g_script.GetCSVToken( token );		
		pTable->SummonWorldIdx	= atoi( token );		

		count++;

		if( ret == CSV_EndOfFile ) break;
		if( ret == CSV_EndOfLine ) continue;

		return( false );
	}

	
	summontablenumber = count;
	
	return true;
	
	return 1;
}




int LoadGonryunBattlePracticeTableReword(char *filenameReword, BattleRewordField_t *pBattleRewordField)
{


  

	char token[512];
	
	if ( !g_script.Load( filenameReword ) )	return 0;
	g_script.BeginParse();



	int ret, idx = 0;
	
     
	while (1) {
		ret  = g_script.GetCSVToken( token );
		
		if ( ret == 1 )	break;
		
		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;
			
			while (1)
			{
				ret  = g_script.GetCSVToken( token );
				if ( ret == 1 )	return 1;
				if ( ret == 2 )	break;
			}
			continue;
		}
		
		
		pBattleRewordField->m_irewordId=atoi( token );
		ret  =g_script.GetCSVToken( token );
		
		strncpy(pBattleRewordField->m_czname, token,strlen(token)+1);	
		ret  = g_script.GetCSVToken( token );
		pBattleRewordField->m_irewordType=atoi( token );	
		ret  =g_script.GetCSVToken( token );
		pBattleRewordField->m_iexp=atoi( token );	
		ret  =g_script.GetCSVToken( token );
		pBattleRewordField->m_iexpRate=atoi( token );	
		ret  = g_script.GetCSVToken( token );
		pBattleRewordField->m_irewordItemId=atoi( token );	
		ret  =g_script.GetCSVToken( token );
		pBattleRewordField->m_iitemRate=atoi( token );	
   		ret  = g_script.GetCSVToken( token );
		
		if ( ret == 1 )	
		{
			return 1;
		}
		if ( ret == 2 )	break;
	}
	return 1;
}


int LoadGonryunBattlePracticeTableRewordItem(char *filenameRewordItem, BattleRewordItem_t pBattleRewordItem[])
{


 

	char token[512];

	if ( !g_script.Load( filenameRewordItem ) )	return 0;
	g_script.BeginParse();


	



	 int ret=0, idx = 0;
    
  
	
	 
	 while (1) {

		ret  = g_script.GetCSVToken( token );
		
		if ( ret == 1 )	break;
		
		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;
			
			while (1)
			{
				ret  =g_script.GetCSVToken( token );
				if ( ret == 1 )	return 1;
				if ( ret == 2 )	break;
			}
			continue;
		}
	
			
	
	
				pBattleRewordItem[idx].m_irewordItemId=atoi( token );
				ret  = g_script.GetCSVToken( token );
				pBattleRewordItem[idx].m_iitemType= atoi( token );	
				ret  = g_script.GetCSVToken( token );
				pBattleRewordItem[idx].m_iitemId=atoi( token );	
				ret  = g_script.GetCSVToken( token );
				pBattleRewordItem[idx].m_iitemNumber=atoi( token );	
				ret  =g_script.GetCSVToken( token );
				pBattleRewordItem[idx].m_iitemRate=atoi( token );	
				ret  = g_script.GetCSVToken( token );
		
	
			
		
		if ( ret == 1 )	
		{
			idx++;
		
			return 1;
		}
		if ( ret == 2 )	
		{
			idx++;
		
			continue;
		}
	}
	return 1;
}



int LoadGolryunManagerTable( char* filename )
{
	char token[512];
	int ret, idx = 0;

	for( int i = 0; i < MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE; i ++ )
	{
		memset( &g_golryunManagerTable[i], 0, sizeof( golryunManagerTable_t ) );
		g_golryunManagerTable[i].index = -1;
	}

	if ( !g_script.Load( filename ) )	return 0;
	g_script.BeginParse();

	while( 1 )
	{
		ret  = g_script.GetCSVToken( token );

		if ( ret == 1 )	break;

		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;

			while (1)
			{
				ret = g_script.GetCSVToken( token );
				if ( ret == 1 )	return 1;
				if ( ret == 2 )	break;
			}
			continue;
		}

		
		g_golryunManagerTable[idx].index = atoi( token );

		ret = g_script.GetCSVToken( token );
		g_golryunManagerTable[idx].bcontrol = atoi( token );

		ret = g_script.GetCSVToken( token );
		g_golryunManagerTable[idx].bgamble = atoi( token );

		ret = g_script.GetCSVToken( token );
		g_golryunManagerTable[idx].levelMin = atoi( token );

		ret = g_script.GetCSVToken( token );
		g_golryunManagerTable[idx].levelMax = atoi( token );

		ret = g_script.GetCSVToken( token );
		g_golryunManagerTable[idx].joinNum = atoi( token );

		ret = g_script.GetCSVToken( token );
		g_golryunManagerTable[idx].bdeposit = atoi( token );

		ret = g_script.GetCSVToken( token );
		g_golryunManagerTable[idx].deposit = atoi( token );

		ret = g_script.GetCSVToken( token );
		g_golryunManagerTable[idx].progressIdx = atoi( token );

		idx++;
	}

	return 1;
}

int LoadGolryunProgressTable( char* filename )
{
	char token[512];
	int ret, idx = 0;

	for( int i = 0; i < MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE; i ++ )
	{
		for(int j=0; j<7; j++)
		{
			g_golryunProgressTable[i].statusTime[j] = 0;
		}
		g_golryunManagerTable[i].index = -1;
	}

	if ( !g_script.Load( filename ) )	return 0;
	g_script.BeginParse();

	while( 1 )
	{
		ret  = g_script.GetCSVToken( token );

		if ( ret == 1 )	break;

		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;

			while (1)
			{
				ret = g_script.GetCSVToken( token );
				if ( ret == 1 )	return 1;
				if ( ret == 2 )	break;
			}
			continue;
		}

		
		g_golryunProgressTable[idx].index = atoi( token );

		for(i=0; i<7; i++)
		{
			ret = g_script.GetCSVToken( token );
			g_golryunProgressTable[idx].statusTime[i] = atoi( token );
		}
		
		ret = g_script.GetCSVToken( token );
		g_golryunProgressTable[idx].rewardIdx = atoi( token );

		idx++;
	}

	return 1;
}


char *g_lTableToken[] =
{
	"_kr",
	"_en",
	"_jp",
	"_ch",
	"_tw",
};

int LoadAllGameTable()
{
	
	char filename[128], filename2[128];

	sprintf( filename, "open%s.slt", g_lTableToken[g_cgv.languageType] );
	if( !LoadItemClassTable( filename ) )
	{
		MessageBox( NULL, "Failure Item Class Table Loading!", "Error", MB_OK );
		return 0;
	}

	sprintf( filename, "open%s.sit", g_lTableToken[g_cgv.languageType] );
	if( !LoadItemTable( filename ) )
	{
		MessageBox( NULL, "Failure Item Table Loading!", "Error", MB_OK );
		return 0;
	}

	sprintf( filename, "open%s.smt", g_lTableToken[g_cgv.languageType] );
	if( !LoadMonsterTable( filename ) )
	{
		MessageBox( NULL, "Failure Monster Table Loading!", "Error", MB_OK );
		return 0;
	}

	sprintf( filename, "open%s.skt", g_lTableToken[g_cgv.languageType] );
	
	
	if( !g_SkillFile.LoadSkillTable( filename ) )
	{
		MessageBox( NULL, "Failure Skill Table Loading!", "Error", MB_OK );
		return 0;
	}

	
	sprintf( filename, "QSTable%s.csv", g_lTableToken[g_cgv.languageType] );
	if ( !LoadQuestScrollTable( filename ) )
	{
		MessageBox( NULL, "Failure Quest Scroll Table Loading!", "Error", MB_OK );
		return 0;
	}

	
	sprintf( filename, "AqsTable%s.csv", g_lTableToken[g_cgv.languageType] );
	if ( !LoadAutoQuestScrollTable( filename ) )
	{
		MessageBox( NULL, "Failure AutoQuest Scroll Table Loading!", "Error", MB_OK );
		return 0;
	}

	
	
	
	sprintf( filename, "summontable%s.csv", g_lTableToken[g_cgv.languageType]);
	if ( !LoadSummonMonsterTable( filename, g_summonMonstertable, g_summonMonstertableNumber))
	{
		MessageBox( NULL, "Failure Summon MonsterTable Loading!", "Error", MB_OK );
		return 0;
	}	
	
	
	
	sprintf( filename, "GonryunBattlePractice_RewardField%s.csv", g_lTableToken[g_cgv.languageType]);
	if ( !LoadGonryunBattlePracticeTableReword( filename, &g_BattleRewordField) )
	{
		MessageBox( NULL, "LoadGonryunBattlePracticeTableReword!", "Error", MB_OK );
		return 0;
	}	

	sprintf( filename, "GonryunBattlePractice_RewardItem%s.csv", g_lTableToken[g_cgv.languageType]);
	if ( !LoadGonryunBattlePracticeTableRewordItem( filename, g_BattleRewordItem))
	{
		MessageBox( NULL, "LoadGonryunBattlePracticeTableRewordItem!", "Error", MB_OK );
		return 0;
	}	

	

	
	sprintf( filename, "GolryunBattle_Manager.csv" );
	if ( !LoadGolryunManagerTable( filename ) )
	{
		MessageBox( NULL, "Failure Golryun Manager Table Loading!", "Error", MB_OK );
		return 0;
	}

	sprintf( filename, "GolryunBattle_Progress.csv" );
	if ( !LoadGolryunProgressTable( filename ) )
	{
		MessageBox( NULL, "Failure Golryun Progress Table Loading!", "Error", MB_OK );
		return 0;
	}
	
	
	
	
	
	LoadExpTableForClient( "newexptable.tbl" );
	LoadGenExpTableForClient( "newGenExpTable.tbl" ); 
	

	sprintf( filename, "OptTable%s.csv", g_lTableToken[g_cgv.languageType] );
	sprintf( filename2, "ValueTable%s.csv", g_lTableToken[g_cgv.languageType] );
	if ( !MakeOptionTable( filename, filename2, g_optionTable ) )
	{
		MessageBox( NULL, "Failure OptionTable or ValueTable Loading!", "Error", MB_OK );
		return 0;
	}
	LoadActionTable( "action.tbl" );
	LoadGenCapabilityTable( "GenCapability.tbl" );
	
	LoadItemCraftTableForServer( "itemCraft.tbl" );

	return 1;
}

#include <direct.h> 

int	g_screenCount = -1;
int GetScreenShotName()
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	BOOL bResult=TRUE;
	char Path[255];
	int filen = 0;

	strcpy( Path, "../screenshot/*.jpg" );
	if( g_screenCount == -1 )
	{
		hSrch = FindFirstFile(Path, &wfd);
		if( hSrch == INVALID_HANDLE_VALUE ) 
		{

			g_screenCount = 0;
			return( g_screenCount );
		}

		filen ++;
		while (1) 
		{
			bResult = FindNextFile(hSrch, &wfd);
			if( !bResult ) break;
			filen ++;
		}
		FindClose(hSrch);
		g_screenCount = filen;
		return( g_screenCount );
	}
	else g_screenCount ++;
	return( g_screenCount );
}

void MakeScreenShot(IDirect3DDevice8* device, int screenx, int screeny) 
{
	char screenFolder[255];
	char filename[255];
	char temp[255];

	memset( screenFolder, 0, 255 );
	memset( filename, 0, 255 );
	memset( temp, 0, 255 );

	strcpy( screenFolder, "../screenshot/" );
	mkdir( screenFolder );

	int n = GetScreenShotName();

	if( n / 10 == 0 )
	{
		itoa( n, temp, 10 );
		sprintf( filename, "../screenshot/screen00%s.jpg", temp );
	}
	else if( n / 10 > 0 && n / 10 < 10 )
	{
		itoa( n, temp, 10 );
		sprintf( filename, "../screenshot/screen0%s.jpg", temp );
	} 
	else
	{
		itoa( n, temp, 10 );
		sprintf( filename, "../screenshot/screen%s.jpg", temp );
	} 
	TakeScreenShot( device, filename, screenx, screeny );
	
	if( g_cgv.state == CLIENT_GAME_PROCESS )
	{
		sprintf( temp, g_LPACK.GetMassage( 0, 420 ), filename );
		g_ifMng->AddSysMessage( temp );
	}
}
void TakeScreenShot(IDirect3DDevice8* device, char* file_name, int screenx, int screeny)
{
	LPDIRECT3DSURFACE8 frontbuf = NULL;
	device->CreateImageSurface(screenx, screeny, D3DFMT_A8R8G8B8, &frontbuf);
	HRESULT hr = device->GetFrontBuffer(frontbuf);

	D3DLOCKED_RECT d3dlr;
	frontbuf->LockRect( &d3dlr, NULL, 0 );
	BYTE *imageData = (BYTE*)d3dlr.pBits;

	BOOL bres = TRUE;
	JPEG_CORE_PROPERTIES image;
	ZeroMemory( &image, sizeof( JPEG_CORE_PROPERTIES ) );

	BYTE* realImage;
	int i, j;
	long realsize = 0;
	float temppoint;
	realImage = new BYTE[screenx * screeny * 3];

	for( i = 0; i < screeny; i ++ )
		for( j = 0; j < screenx; j ++ )
		{
			temppoint = (float)imageData[ ( i * screenx + j ) * 4 + 0] * 1.3f;
			if( temppoint > 255.0f ) temppoint = 255.0f;
			realImage[realsize] = (int)temppoint;
			realsize ++;
			temppoint = (float)imageData[ ( i * screenx + j ) * 4 + 1] * 1.3f;
			if( temppoint > 255.0f ) temppoint = 255.0f;
			realImage[realsize] = (int)temppoint;
			realsize ++;
			temppoint = (float)imageData[ ( i * screenx + j ) * 4 + 2] * 1.3f;
			if( temppoint > 255.0f ) temppoint = 255.0f;
			realImage[realsize] = (int)temppoint;
			realsize ++;
		}


	if( ijlInit( &image ) != IJL_OK )
	{
		ijlFree( &image );
		frontbuf->UnlockRect();
		frontbuf->Release();
		GTH_SAFE_DELETE ( realImage );
		return;
	}

	image.DIBWidth = screenx;
	image.DIBHeight = screeny;
	image.DIBBytes = realImage;
	image.DIBColor = IJL_BGR;
	
   
	image.JPGFile = const_cast<char*>(file_name);
	image.JPGWidth = screenx;
	image.JPGHeight = screeny;
   
	if( ijlWrite( &image, IJL_JFILE_WRITEWHOLEIMAGE ) != IJL_OK )
	{
		ijlFree( &image );
		frontbuf->UnlockRect();
		frontbuf->Release();
		GTH_SAFE_DELETE ( realImage );
		return;
	}
   
	if( ijlFree( &image ) != IJL_OK )
	{
		ijlFree( &image );
		frontbuf->UnlockRect();
		frontbuf->Release();
		GTH_SAFE_DELETE ( realImage );
		return;
	}

	ijlFree( &image );
	frontbuf->UnlockRect();
	frontbuf->Release();
	GTH_SAFE_DELETE ( realImage );
}