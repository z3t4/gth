
#include "../global.h"
#include <assert.h>








int	LoadItemClassTable( char* filename, itemClassTable_t itemClassTable[], int &itemClassTableNumber )
{
	FILE* fp=NULL;
	

	tableHeader_t header;
	memset( &header, 0, sizeof( tableHeader_t ) );

	fp = fopen( filename, "rb" );
	if (!fp)
		return 0;
	int i;

	fread( &header, sizeof( header ), 1, fp );
	if( strcmp( header.id, ITEM_CLASS_TABLE_HEADER ) )
	{
		fclose( fp );
		return( 0 );
	}

	if( header.version != ITEM_CLASS_TABLE_VERSION_103 )
	{
		fclose( fp );
		return 0;
	}

	itemClassTableNumber = header.recordNumber;	

	if(itemClassTableNumber >MAX_NUMBER_OF_ITEM_CLASS_TABLE)
	{
		fclose( fp );
		return 0;
	}
	
	for( i = 0; i < itemClassTableNumber; i ++ )
	{
		fread( &itemClassTable[i], sizeof( itemClassTable_t ), 1, fp );
	}
	
	fclose( fp );
	
	return( 1 );
}



void ConvertItemTable103To104( char *filename, itemTable_t itemTable[], int &itemTableNumber )
{
	FILE	*fp;
	tableHeader_t	header;
	oldItemTable103_t	oldTable;
	memset ( &header,	0,	sizeof (tableHeader_t) );

	fp	=	fopen ( filename,	"rb" );
	fread (	&header,	sizeof (header),	1,	fp );
	itemTableNumber = header.recordNumber;

	for( int i = 0; i <	itemTableNumber; i ++ )
	{
		fread( &oldTable, sizeof( oldItemTable103_t ), 1, fp );
		itemTable[i].idx = oldTable.idx;
		itemTable[i].atkDelay = oldTable.attackDelay;
		itemTable[i].atkHanded = oldTable.handed;
		itemTable[i].atkHitPower = oldTable.hitPower;
		itemTable[i].atkLength = oldTable.attackLength;
		itemTable[i].atkPhyMaxDmg = oldTable.attackMaxDmg;
		itemTable[i].atkPhyMinDmg = oldTable.attackMinDmg;
		itemTable[i].atkCriticalRate = oldTable.onceRACost;
		itemTable[i].atkCriticalDmg = oldTable.onceSACost;
		itemTable[i].atkBowSACost = oldTable.onceSECost;
		itemTable[i].atkRange = oldTable.attackRange;
		memcpy( &itemTable[i].attackAni, &oldTable.attackAni, sizeof( NUM_TABLE_GEN_TYPE ) );
		itemTable[i].classIdx = oldTable.mainClass;
		itemTable[i].defPower = oldTable.defensePower;
		strcpy( itemTable[i].document, oldTable.document );
		itemTable[i].durability = oldTable.durabilty;
		itemTable[i].durYn = oldTable.durYn;
		itemTable[i].epkMaxChargeRA = oldTable.maxChargeRA;
		itemTable[i].epkMaxChargeSA = oldTable.maxChargeSA;
		itemTable[i].epkRecoveryRA = oldTable.onceRecoveryRA;
		itemTable[i].epkRecoverySA = oldTable.onceRecoverySA;
		itemTable[i].gambleCost = oldTable.gambleCost;
		itemTable[i].iconItemID = oldTable.iconItemID;
		itemTable[i].level = oldTable.level;
		itemTable[i].mdlItemID = oldTable.mdlItemID;
		itemTable[i].mdlItemType = oldTable.mdlItemType;
		strcpy( itemTable[i].name, oldTable.name );
		itemTable[i].onceRepairDecDur = oldTable.onceRepairDecDur;
		itemTable[i].rarity = oldTable.rarity;
		itemTable[i].repairYn = oldTable.repairYn;
		itemTable[i].reqAgility = oldTable.reqAgility;
		itemTable[i].reqForce = oldTable.reqForce;
		itemTable[i].reqNumber = oldTable.reqNumber;
		itemTable[i].reqSoulpower = oldTable.reqSoulpower;
		itemTable[i].reqVitality = oldTable.reqVitality;
		itemTable[i].sellCost = oldTable.sellCost;
		itemTable[i].sellYn = oldTable.sellYn;
		itemTable[i].size = oldTable.size;
		memcpy( &itemTable[i].skinModelID, &oldTable.skinModelID, sizeof( NUM_TABLE_GEN_TYPE ) );
		itemTable[i].skinningPosition = oldTable.skinningPosition;
		itemTable[i].skinningYn = oldTable.skinningYn;
		itemTable[i].supplyRA = oldTable.supplyRA;
		itemTable[i].supplySA = oldTable.supplySA;
		itemTable[i].tagID = oldTable.tagID;
		memcpy( &itemTable[i].tagModelID, &oldTable.tagModelID, sizeof( MAX_NUMBER_OF_ITEM_PART ) );
		memcpy( &itemTable[i].tagMoveID, &oldTable.tagMoveID, sizeof( MAX_NUMBER_OF_ITEM_PART ) );
		itemTable[i].type = oldTable.type;
		itemTable[i].upgBasicEfficiency = oldTable.basicEfficiency;
		itemTable[i].upgNumber = oldTable.gradeNumber;
		memcpy( &itemTable[i].upgReqExp, &oldTable.upgradeExp, sizeof( oldTable.upgradeExp ) );
		itemTable[i].wearPosition = oldTable.wearPosition;
		itemTable[i].weight = oldTable.weight;
	}

	fclose ( fp );
}

void ConvertItemTable104To105( char *filename, itemTable_t itemTable[], int &itemTableNumber )
{
	FILE	*fp;
	tableHeader_t		header;
	oldItemTable104_t	*oldTable;
	memset ( &header,	0,	sizeof (tableHeader_t) );

	fp	=	fopen ( filename,	"rb" );
	fread (	&header,	sizeof (header),	1,	fp );
	itemTableNumber = header.recordNumber;

	oldTable = ( oldItemTable104_t *)malloc( sizeof( oldItemTable104_t ) * MAX_NUMBER_OF_ITEM_TABLE );

	for( int i = 0; i <	itemTableNumber; i ++ )
	{
		fread( &oldTable[i], sizeof( oldItemTable104_t ), 1, fp );
	}

	fclose ( fp );

	fp	=	fopen ( filename,	"rb" );
	fread (	&header,	sizeof (header),	1,	fp );
	itemTableNumber = header.recordNumber;

	for( i = 0; i <	itemTableNumber; i ++ )
	{
		fread( &itemTable[i], sizeof( itemTable_t ), 1, fp );

		memset( itemTable[i].name, 0, sizeof( itemTable[i].name ) );
		strcpy( itemTable[i].name, oldTable[i].name );
		memset( itemTable[i].document, 0, sizeof( itemTable[i].document ) );
		strcpy( itemTable[i].document, oldTable[i].document );
		
		
	}

	fclose ( fp );

	free( oldTable );
}




int LoadItemTable( char* filename, itemTable_t itemTable[], int &itemTableNumber )
{
	FILE* fp=NULL;
	

	tableHeader_t header;

	memset( &header, 0, sizeof( tableHeader_t ) );

	fp = fopen( filename, "rb" );
	int i;

	fread( &header, sizeof( header ), 1, fp );
	if( strcmp( header.id, ITEM_TABLE_HEADER ) )
	{
		fclose( fp );
		return( 0 );
	}

	int size; 
	fread( &size, sizeof( size ), 1, fp );
	if(size !=  sizeof(itemTable_t))
	{
		return( 0 );
	}

	int TypeSize; 
	fread( &TypeSize, sizeof( TypeSize ), 1, fp );

	itemTableNumber = header.recordNumber;
	
	if(itemTableNumber > MAX_NUMBER_OF_ITEM_TABLE)
	{
		fclose( fp );
		return 0;
	}

	for( i = 0; i < itemTableNumber; i ++ )
	{
		fread( &itemTable[i], sizeof( itemTable_t ), 1, fp );

		
		lstrcpyn(itemTable[i].name, g_TxtApp.GetName(itemTable[i].NameIdx),itemTable_t::MAXBYTES_NAME);
		lstrcpyn(itemTable[i].document , g_TxtApp.GetDesc(itemTable[i].DocumentIdx),itemTable_t::MAXBYTES_DOCUMENT);

	}	
	fclose( fp );
	
	return( 1 );
}


int SaveItemTable( char* filename, itemTable_t itemTable[], int itemTableNumber )
{
	FILE *fp;
	tableHeader_t header;

	fp = fopen( filename, "wb" );
	int i;

	memset( &header, 0, sizeof( tableHeader_t ) );
	memcpy( header.id, ITEM_TABLE_HEADER, sizeof( ITEM_TABLE_HEADER ) );
	header.version = ITEM_TABLE_VERSION;

	
	header.recordNumber = itemTableNumber;

	fwrite( &header, sizeof( header ), 1, fp );

	for( i = 0; i < itemTableNumber; i ++ )
	{
		fwrite( &itemTable[i], sizeof( itemTable_t ), 1, fp );
	}
	fclose( fp );
	
	return( 1 );
}


int LoadItemOptionTable( char* filename, itemOptionTable_t itemOptionTable[], int &itemOptionTableNumber )
{
	FILE *fp;
	tableHeader_t header;

	fp = fopen( filename, "rb" );

	fread( &header, sizeof( header ), 1, fp );
	if( strcmp( header.id, ITEM_OPTION_TABLE_HEADER ) )
	{
		fclose( fp );
		return( 0 );
	}
	if( header.version != ITEM_OPTION_TABLE_VERSION )
	{
		fclose( fp );
		return( 0 );
	}

	itemOptionTableNumber = header.recordNumber;

	int i;
	for( i = 0; i < itemOptionTableNumber; i ++ )
	{
		fread( &itemOptionTable[i], sizeof( itemOptionTable_t ), 1, fp );
	}
	fclose( fp );

	return( 1 );
}

int SaveItemOptionTable( char* filename, itemOptionTable_t itemOptionTable[], int itemOptionTableNumber )
{
	FILE *fp;
	tableHeader_t header;

	fp = fopen( filename, "wb" );

	memset( &header, 0, sizeof( header ) );
	memcpy( header.id, ITEM_OPTION_TABLE_HEADER, sizeof( ITEM_OPTION_TABLE_HEADER ) );
	header.version = ITEM_OPTION_TABLE_VERSION;

	
	header.recordNumber = itemOptionTableNumber;

	fwrite( &header, sizeof( header ), 1, fp );

	int i;
	for( i = 0; i < itemOptionTableNumber; i ++ )
	{
		fwrite( &itemOptionTable[i], sizeof( itemOptionTable_t ), 1, fp );
	}
	fclose( fp );

	return( 1 );
}


int LoadItemGenerateTable( char* filename, itemGenerateTable_t itemGenerateTable[], int &itemGenerateTableNumber )
{
	FILE *fp;
	tableHeader_t header;

	fp = fopen( filename, "rb" );

	memset( &header, 0, sizeof( header ) );

	fread( &header, sizeof( header ), 1, fp );

	if( strcmp( header.id, ITEM_GENERATE_TABLE_HEADER ) )
	{
		fclose( fp );
		return( 0 );
	}
	if( header.version != ITEM_GENERATE_TABLE_VERSION )
	{
		fclose( fp );
		return( 0 );
	}

	
	itemGenerateTableNumber = header.recordNumber;

	int i;
	for( i = 0; i < itemGenerateTableNumber; i ++ )
	{
		fread( &itemGenerateTable[i], sizeof( itemGenerateTable_t ), 1, fp );
	}
	fclose( fp );

	return( 1 );
}


int SaveItemGenerateTable( char* filename, itemGenerateTable_t itemGenerateTable[], int itemGenerateTableNumber )
{
	FILE *fp;
	tableHeader_t header;

	fp = fopen( filename, "wb" );

	memset( &header, 0, sizeof( header ) );

	memcpy( header.id, ITEM_GENERATE_TABLE_HEADER, sizeof( ITEM_GENERATE_TABLE_HEADER ) );
	header.version = ITEM_GENERATE_TABLE_VERSION;

	
	header.recordNumber = itemGenerateTableNumber;

	fwrite( &header, sizeof( header ), 1, fp );

	int i;
	for( i = 0; i < itemGenerateTableNumber; i ++ )
	{
		fwrite( &itemGenerateTable[i], sizeof( itemGenerateTable_t ), 1, fp );
	}
	fclose( fp );

	return( 1 );
}

void ConvertMonsterTable102To103( char* filename, monsterBasicTable_t monsterTable[], int &monsterTableNumber )
{
	FILE *fp;
	oldMonsterBasicTable_t oldTable;

	fp = fopen( filename, "rb" );

	tableHeader_t header;
	memset( &header, 0, sizeof( header ) );	
	fread( &header, sizeof( header ), 1, fp );

	int i;

	monsterTableNumber = header.recordNumber;

	for( i = 0; i < monsterTableNumber; i ++ )
	{
		fread( &oldTable, sizeof( oldMonsterBasicTable_t ), 1, fp );
		monsterTable[i].idx = oldTable.idx;
		monsterTable[i].aniSetType = oldTable.aniSetType;
		monsterTable[i].atkDelay = oldTable.attackDelay[0];
		monsterTable[i].atkHitPower = oldTable.hitPower[0];
		monsterTable[i].atkLength = oldTable.attackLength[0];
		monsterTable[i].atkPhyMinDmg = oldTable.minDmg[0];
		monsterTable[i].atkPhyMaxDmg = oldTable.maxDmg[0];
		monsterTable[i].atkRange = oldTable.attackRange[0];
		monsterTable[i].attackAni = oldTable.attackAni[0];
		monsterTable[i].defPower = oldTable.defensePower;
		strcpy( monsterTable[i].document, oldTable.document );
		monsterTable[i].exp = oldTable.exp;
		monsterTable[i].gen = oldTable.gen;
		monsterTable[i].genExp = oldTable.genExp;
		monsterTable[i].level = oldTable.level;
		memcpy( &monsterTable[i].lootItem, &oldTable.lootItem, sizeof( lootItem_t ) );
		monsterTable[i].lootSE = oldTable.lootNSE;
		monsterTable[i].maxRA = oldTable.maxRA;
		monsterTable[i].maxSA = oldTable.maxSA;
		monsterTable[i].modelID = oldTable.modelID;
		strcpy( monsterTable[i].name, oldTable.name );
		monsterTable[i].pathFindingSize = oldTable.pathFindingSize;
		monsterTable[i].recoveryRARate = oldTable.recoveryRARate;
		monsterTable[i].recoverySARate = oldTable.recoverySARate;
		monsterTable[i].size = oldTable.size;
		monsterTable[i].stunTime = oldTable.stunTime;
		memcpy( &monsterTable[i].tagModelID, &oldTable.tagModelID, sizeof( oldTable.tagModelID ) );
		memcpy( &monsterTable[i].tagMoveID, &oldTable.tagMoveID, sizeof( oldTable.tagMoveID ) );
		monsterTable[i].type = oldTable.type;
		monsterTable[i].velocity = (float)oldTable.velocity / 10.0f;
	}
	fclose( fp );
}



BOOL LoadMonsterTable(const char* filename, monsterBasicTable_t monsterTable[], int &monsterTableNumber )
{
	FILE* fp=NULL;
	fp = fopen( filename, "rb" );
	if(NULL == fp) return FALSE;


	tableHeader_t header;
	memset( &header, 0, sizeof( header ) );
	
	fread( &header, sizeof( header ), 1, fp );
	if(0 != strcmp( header.id, MONSTER_TABLE_HEADER ) )	{
		if(NULL != fp){ fclose( fp ); fp=NULL; }
		return FALSE;
	}



	if( header.version != MONSTER_TABLE_VERSION )
	{
		
		return FALSE;
	}


	monsterTableNumber = header.recordNumber;
	for(int idx = 0; idx < monsterTableNumber; idx ++ )
	{
		fread( &monsterTable[idx], sizeof( monsterBasicTable_t ), 1, fp );
		monsterTable[idx].name[monsterBasicTable_t::MAXBYTES_NAME]=NULL;
		monsterTable[idx].document[monsterBasicTable_t::MAXBYTES_DOCUMENT]=NULL;
	}
	if(NULL != fp){ fclose( fp ); fp=NULL; }
	return TRUE;
}










int SaveMonsterTable( char* filename, monsterBasicTable_t monsterTable[], int monsterTableNumber )
{
	FILE *fp;
	tableHeader_t header;

	fp = fopen( filename, "wb" );

	memset( &header, 0, sizeof( header ) );

	memcpy( header.id, MONSTER_TABLE_HEADER, sizeof( MONSTER_TABLE_HEADER ) );
	header.version = MONSTER_TABLE_VERSION;
	header.recordNumber = monsterTableNumber;


	fwrite( &header, sizeof( header ), 1, fp );

	int i;
	for( i = 0; i < monsterTableNumber; i ++ )
	{
		fwrite( &monsterTable[i], sizeof( monsterBasicTable_t ), 1, fp );
	}
	fclose( fp );

	return( 1 );
}
void InitSkillTable( skillTable_t skill[] )
{
	int i;
	for( i = 0; i < MAX_SKILL_NUMBER; i ++ )
	{
		::ZeroMemory( &skill[i], sizeof( skillTable_t ) );
		
		for( int j = 0; j < 8; j ++ )
			skill[i].reqPartyGen[j] = -1;

		skill[i].reqItemType = -1;
		skill[i].reqItem = -1;

		skill[i].id = -1;
		skill[i].iconID = -1;
		skill[i].effect.zpos = 50;

		skill[i].numValue = NUM_OF_SKILL_VALUE;
		for( j = 0; j < NUM_OF_SKILL_VALUE; j ++ )
		{
			strcpy( skill[i].valueName[j], "없음" );
		}
		strcpy( skill[i].valueName[0], "SA소모량" );
		strcpy( skill[i].valueName[1], "캐스팅타임" );
		strcpy( skill[i].valueName[2], "쿨타임" );
	}
}

void DeleteSkillTable( skillTable_t *skill_one )
{
	::ZeroMemory( skill_one, sizeof( skillTable_t ) );
	skill_one->id = -1;
	skill_one->iconID = -1;
	skill_one->reqItemType = -1;
	skill_one->reqItem = -1;

	for( int j = 0; j < 8; j ++ )
		skill_one->reqPartyGen[j] = -1;

	skill_one->numValue = NUM_OF_SKILL_VALUE;

	for( j = 0; j < NUM_OF_SKILL_VALUE; j ++ )
	{
		strcpy( skill_one->valueName[j], "없음" );
	}

	strcpy( skill_one->valueName[0], "SA소모량" );
	strcpy( skill_one->valueName[1], "캐스팅타임" );
	strcpy( skill_one->valueName[2], "쿨타임" );

	skill_one->effect.zpos = 50;
}

int LoadSkillTable( char *filename, skillTable_t skill[], int &skillNumber )
{	


	return( 1 );
}


int SaveSkillTable( char *filename, skillTable_t skill[], int skillNumber )
{
	FILE *fp;
	tableHeader_t header;

	fp = fopen( filename, "wb" );

	memset( &header, 0, sizeof( header ) );

	memcpy( header.id, SKILL_TABLE_HEADER, sizeof( SKILL_TABLE_HEADER ) );
	header.version = SKILL_TABLE_VERSION_104;
	header.recordNumber = skillNumber;

	fwrite( &header, sizeof( header ), 1, fp );

	int i;
	for( i = 0; i < MAX_SKILL_NUMBER; i ++ )
	{
		fwrite( &skill[i], sizeof( skillTable_t ), 1, fp );
	}
	fclose( fp );

	return( 1 );

}

int LoadSkillTable( BYTE *buffer, skillTable_t skill[], int &skillNumber )
{
	return 1;
}







void InitQIGTable( questItemGenerate_t qigTable[] )
{
	for( int i = 0; i < MAX_NUMBER_OF_MONSTER_TABLE; i ++ )
	{
		memset( &qigTable[i], 0, sizeof( questItemGenerate_t ) );
		qigTable[i].monsterTableIdx = i;
		
		for( int j = 0; j < MAX_NUMBER_OF_QUEST_ITEM_GENERATE; j ++ )
		{
			qigTable[i].questIdx[j] = -1;
			qigTable[i].itemTableIdx[j] = -1;
		}
	}
}

void ConvertQIGTable100To101( char *filename, questItemGenerate_t qigTable[] )
{
	FILE *fp;
	oldQuestItemGenerate_t old;

	fp = fopen( filename, "rb" );

	tableHeader_t header;
	memset( &header, 0, sizeof( header ) );
	
	fread( &header, sizeof( header ), 1, fp );
	
	int i;

	for( i = 0; i < MAX_NUMBER_OF_MONSTER_TABLE; i ++ )
	{
		fread( &old, sizeof( oldQuestItemGenerate_t ), 1, fp );
		qigTable[i].monsterTableIdx = old.monsterTableIdx;
		qigTable[i].questIdx[0] = old.questIdx;
		qigTable[i].step[0] = old.step;
		qigTable[i].itemNumber = old.itemNumber;
		for( int j = 0; j < MAX_NUMBER_OF_QUEST_ITEM_GENERATE; j ++ )
		{
			qigTable[i].itemTableIdx[j] = old.itemTableIdx[j];
			qigTable[i].maxItemCount[j] = old.maxItemCount[j];
			qigTable[i].generateProbaility[j] = old.generateProbaility[j];
		}
	}
	fclose( fp );
}

int SaveQIGTable( char *filename, questItemGenerate_t qigTable[] )
{
	FILE *fp;
	tableHeader_t header;

	fp = fopen( filename, "wb" );

	memset( &header, 0, sizeof( header ) );

	memcpy( header.id, QIG_TABLE_HEADER, sizeof( QIG_TABLE_HEADER ) );
	header.version = QIG_TABLE_VERSION;

	header.recordNumber = MAX_NUMBER_OF_MONSTER_TABLE;

	fwrite( &header, sizeof( header ), 1, fp );

	int i;
	for( i = 0; i < MAX_NUMBER_OF_MONSTER_TABLE; i ++ )
	{
		fwrite( &qigTable[i], sizeof( questItemGenerate_t ), 1, fp );
	}
	fclose( fp );

	return( 1 );
}

int LoadQIGTable( char* filename, questItemGenerate_t qigTable[] )
{
	FILE *fp;

	fp = fopen( filename, "rb" );
	if(NULL == fp) return 0;
	

	tableHeader_t header;
	memset( &header, 0, sizeof( header ) );
	
	fread( &header, sizeof( header ), 1, fp );
	if( strcmp( header.id, QIG_TABLE_HEADER ) )
	{
		fclose( fp );
		return( 0 );
	}
	if( header.version != QIG_TABLE_VERSION )
	{
		fclose( fp );
		ConvertQIGTable100To101( filename, qigTable );
		return( 0 );
	}

	int i;

	for( i = 0; i < MAX_NUMBER_OF_MONSTER_TABLE; i ++ )
	{
		fread( &qigTable[i], sizeof( questItemGenerate_t ), 1, fp );
	}
	fclose( fp );

	return( 1 );
}


int LoadAutoQuestScrollTable( char *filename, autoQuestScrollTable_t autoqsTable[], int &autoTableNumber)
{
	FILE *fp;
	int size;
	byte *buffer;
	char token[512];

	if ( ( fp = fopen( filename, "rb" ) ) == NULL )
		return 0;

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	curpos = (char *)buffer;
	endpos = curpos + size;

	int ret, idx = 0;

	for( int i = 0; i < MAX_NUMBER_OF_AUTO_QUESTSCROLL_TABLE; i ++ )
	{
		memset( &g_autoQuestscrollTable[i], 0, sizeof( autoQuestScrollTable_t ) );
		g_autoQuestscrollTable[i].Index = -1;
	}

	g_autoQuestscrollTableNumber = 0;

	while( 1 )
	{
		ret  = NextCSVToken( token );

		if ( ret == 1 )	break;

		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;

			while (1)
			{
				ret  = NextCSVToken( token );
				if ( ret == 1 )	
				{
					
					if(buffer!=NULL)
					{
						delete [] buffer;
					}
				
					return 1;
				}
				
				if ( ret == 2 )	break;
			}
			continue;
		}

		
		g_autoQuestscrollTable[idx].Index = atoi( token );

		ret  = NextCSVToken( token );
		strcpy( g_autoQuestscrollTable[idx].name, token );

		if ( ret == 1 )	
		{
			g_autoQuestscrollTableNumber++;
			break;
		}
		if ( ret == 2 )	
		{
			idx++;
			g_autoQuestscrollTableNumber++;
			continue;
		}

		for (int j=0; j < ( sizeof( AUTOQSDATAFIELD ) / 4 ); j++)
		{
			ret  = NextCSVToken( token );
			g_autoQuestscrollTable[idx].dataField[j] = atoi( token );

			if ( ret == 1 )	
			{
				g_autoQuestscrollTableNumber++;
						
				
				if(buffer!=NULL)
				{
					delete [] buffer;
				}
				return 1;
			}
			if ( ret == 2 )	break;
		}

		idx++;
		g_autoQuestscrollTableNumber++;

	}

	if(buffer!=NULL)
	{
		delete [] buffer;
	}
	return 1;
}


int LoadGolryunManagerTable( char *filename)
{
	FILE *fp;
	int size;
	byte *buffer;
	char token[512];

	if ( ( fp = fopen( filename, "rb" ) ) == NULL )
		return 0;

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	curpos = (char *)buffer;
	endpos = curpos + size;

	int ret, idx = 0;

	for( int i = 0; i < MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE; i ++ )
	{
		memset( &g_golryunManagerTable[i], 0, sizeof( golryunManagerTable_t ) );
		g_golryunManagerTable[i].index = -1;
	}

	while( 1 )
	{
		ret  = NextCSVToken( token );

		if ( ret == 1 )	break;

		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;

			while (1)
			{
				ret  = NextCSVToken( token );
				if ( ret == 1 )	return 1;
				if ( ret == 2 )	break;
			}
			continue;
		}

		
		g_golryunManagerTable[idx].index = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunManagerTable[idx].bcontrol = atoi(token);

		ret  = NextCSVToken( token );
		g_golryunManagerTable[idx].bgamble = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunManagerTable[idx].levelMin = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunManagerTable[idx].levelMax = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunManagerTable[idx].joinNum = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunManagerTable[idx].bdeposit = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunManagerTable[idx].deposit = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunManagerTable[idx].progressIdx = atoi( token );

		idx++;
	}

	return 1;
}

int LoadGolryunProgressTable( char *filename)
{
	FILE *fp;
	int size;
	byte *buffer;
	char token[512];

	if ( ( fp = fopen( filename, "rb" ) ) == NULL )
		return 0;

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	curpos = (char *)buffer;
	endpos = curpos + size;

	int ret, idx = 0;

	for( int i = 0; i < MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE; i ++ )
	{
		for( int j = 0; j < 7; j++)
		{
			g_golryunProgressTable[i].statusTime[j] = 0;
		}
	
		g_golryunProgressTable[i].index = -1;
	}

	while( 1 )
	{
		ret  = NextCSVToken( token );

		if ( ret == 1 )	break;

		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;

			while (1)
			{
				ret  = NextCSVToken( token );
				if ( ret == 1 )	return 1;
				if ( ret == 2 )	break;
			}
			continue;
		}

		
		g_golryunProgressTable[idx].index = atoi( token );

		for(i=0; i<7; i++)
		{
			ret  = NextCSVToken( token );
			g_golryunProgressTable[idx].statusTime[i] = atoi( token );
		}	

		ret  = NextCSVToken( token );
		g_golryunProgressTable[idx].rewardIdx = atoi( token );

		idx++;
	}

	return 1;
}

int LoadGolryunRewardManagerTable( char *filename)
{
	FILE *fp;
	int size;
	byte *buffer;
	char token[512];

	if ( ( fp = fopen( filename, "rb" ) ) == NULL )
		return 0;

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	curpos = (char *)buffer;
	endpos = curpos + size;

	int ret, idx = 0;

	for( int i = 0; i < MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE; i ++ )
	{
		memset(&g_golryunRewardManagerTable[i], 0, sizeof(golryunRewardManagerTable_t));
		g_golryunRewardManagerTable[i].index = -1;		
	}

	while( 1 )
	{
		ret  = NextCSVToken( token );

		if ( ret == 1 )	break;

		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;

			while (1)
			{
				ret  = NextCSVToken( token );
				if ( ret == 1 )	return 1;
				if ( ret == 2 )	break;
			}
			continue;
		}

		
		g_golryunRewardManagerTable[idx].index = atoi( token );
		
		ret  = NextCSVToken( token );
		g_golryunRewardManagerTable[idx].rewardType = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunRewardManagerTable[idx].bsetWaiting = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunRewardManagerTable[idx].waitingExp = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunRewardManagerTable[idx].bsetDefeat = atoi( token );
		
		ret  = NextCSVToken( token );
		g_golryunRewardManagerTable[idx].defeatExp = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunRewardManagerTable[idx].setReward = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunRewardManagerTable[idx].rewardItemRate = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunRewardManagerTable[idx].rewardGroupIdx[0] = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunRewardManagerTable[idx].rewardGroupIdx[1] = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunRewardManagerTable[idx].rewardGroupIdx[2] = atoi( token );

		ret  = NextCSVToken( token );
		
		idx++;

		if ( ret == 1 )	
		{
			return 1;
		}
		if ( ret == 2 )	continue;	
	}

	return 1;
}

int LoadGolryunItemGroupTable( char *filename)
{
	FILE* fp;
	int size;
	byte *buffer;
	char token[512];

	if ( ( fp = fopen( filename, "rb" ) ) == NULL )
		return 0;

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	curpos = (char *)buffer;
	endpos = curpos + size;

	int ret, idx = 0;

	for( int i = 0; i < MAX_NUMBER_OF_GOLRYUN_ITEM_GROUP_TABLE; i ++ )
	{
		for( int j=0; j<3; j++)
		{
			memset(&g_golryunItemGroupTable[i].rewardField[j], 0, sizeof(golryunRewardField));
		}
		g_golryunItemGroupTable[i].id = -1;	
		g_golryunItemGroupTable[i].groupIdx = -1;
	}

	while( 1 )
	{
		ret  = NextCSVToken( token );

		if ( ret == 1 )	break;

		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;

			while (1)
			{
				ret  = NextCSVToken( token );
				if ( ret == 1 )	return 1;
				if ( ret == 2 )	break;
			}
			continue;
		}

		
		g_golryunItemGroupTable[idx].id = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunItemGroupTable[idx].groupIdx = atoi( token );

		int groupId = g_golryunItemGroupTable[idx].groupIdx;

		ret  = NextCSVToken( token );
		g_golryunItemGroupTable[idx].rewardField[groupId].chooseRate = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunItemGroupTable[idx].rewardField[groupId].expRate = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunItemGroupTable[idx].rewardField[groupId].rewardExp = atoi( token );
		
		ret  = NextCSVToken( token );
		g_golryunItemGroupTable[idx].rewardField[groupId].rewardItem = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunItemGroupTable[idx].rewardField[groupId].provisionNum = atoi( token );

		ret  = NextCSVToken( token );
		g_golryunItemGroupTable[idx].rewardField[groupId].setOption = atoi( token );

		ret  = NextCSVToken( token );
		
		if(groupId>=2)	idx++;

		if ( ret == 1 )	
		{
			return 1;
		}
		if ( ret == 2 )	continue;	
	}

	return 1;
}




extern char *curpos, *endpos;
int LoadQuestScrollTable( char *filename, questscrollTable_t qsTable[], int &qsTableNumber )
{

	FILE *fp;
	int size;
	byte *buffer;
	char token[512];

	if ( ( fp = fopen( filename, "rb" ) ) == NULL )
		return 0;

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	curpos = (char *)buffer;
	endpos = curpos + size;

	int ret, idx = 0;

	for( int i = 0; i < MAX_NUMBER_OF_QUESTSCROLL_TABLE; i ++ )
	{
		memset( &g_questscrollTable[i], 0, sizeof( questscrollTable_t ) );
		g_questscrollTable[i].Index = -1;
	}

	g_questscrollTableNumber = 0;

	while( 1 )
	{
		ret  = NextCSVToken( token );

		if ( ret == 1 )	break;

		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;

			while (1)
			{
				ret  = NextCSVToken( token );
				if ( ret == 1 )
				{
					
					if(buffer!=NULL)
					{
						delete [] buffer;
					}
					return 1;
				}
					
				if ( ret == 2 )	break;
			}
			continue;
		}

		
		g_questscrollTable[idx].Index = atoi( token );

		ret  = NextCSVToken( token );
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
			ret  = NextCSVToken( token );
			g_questscrollTable[idx].dataField[j] = atoi( token );

			if ( ret == 1 )	
			{
				g_questscrollTableNumber++;
				
				if(buffer!=NULL)
				{
					delete [] buffer;
				}
				return 1;
			}
			if ( ret == 2 )	break;
		}

		ret = NextCSVToken( token );
		strcpy( g_questscrollTable[idx].description, token );
	
		idx++;
		g_questscrollTableNumber++;

	}

	
	int bError;
	for( i=0; i<g_questscrollTableNumber; i++ )
	{
		if ( !strcmp( g_questscrollTable[i].name, "0" ) )
			continue;

		bError = 1;
		for( int j=0; j<QS_REWARD_NUMBER; j++ )
		{
			if ( g_questscrollTable[i].data.rewardField[j] > 0 )
				bError = 0;
		}

		if ( bError )
		{
			
			if(buffer!=NULL)
			{
				delete [] buffer;
			}
			return 0;
		}
			
	}
	
	if(buffer!=NULL)
	{
		delete [] buffer;
	}
	return 1;
}



int LoadOptionGroupTable( char* filename, optionGroupTable_t groupTable[] )
{
	char				token[128];
	int					ret;
	optionGroupTable_t	*pGroup;

	if( !LoadTokenBuffer( filename ) ) return false;

	for( int i = 0; i < MAX_COUNT_OPTION_GROUP_TABLE; i ++ )
	{
		memset( &groupTable[i], 0, sizeof( optionGroupTable_t ) );
		groupTable[i].idx = -1;
	}

	
	while( 1 )
	{
		ret = NextCSVToken( token );
		if( ret == CSV_EndOfLine ) break;
	}
	while( 1 )
	{
		
		ret						=	NextCSVToken( token );
		pGroup					=	&groupTable[ atoi( token ) ];
		pGroup->idx				=	atoi( token );
		
		ret						=	NextCSVToken( token );
		strcpy(	pGroup->name,	token );
		
		for( i = 0; i < MAX_NUMBER_OF_ITEM_OPTION; i ++ )
		{
			ret							=	NextCSVToken( token );
			pGroup->optionIdx[i]		=	atoi( token );
			ret							=	NextCSVToken( token );
			pGroup->optionGrade[i]		=	atoi( token );
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

	if( !LoadTokenBuffer( optionTableFilename ) ) return false;

	for( int i = 0; i < MAX_COUNT_OPTION_TABLE; i ++ )
	{
		memset( &optionTable[i], 0, sizeof( optionTable_t ) );
		optionTable[i].idx = -1;
	}

	
	while( 1 )
	{
		ret = NextCSVToken( token );
		if( ret == CSV_EndOfLine ) break;
	}
	while( 1 )
	{
		
		ret						=	NextCSVToken( token );
		pOpt					=	&optionTable[ atoi( token ) ];
		pOpt->idx				=	atoi( token );
		
		ret						=	NextCSVToken( token );
		strcpy(	pOpt->name,	token );
		
		ret						=	NextCSVToken( token );
		strcpy(	pOpt->printing,	token );
		
		ret						=	NextCSVToken( token );
		pOpt->dropYN			=	atoi( token );
		
		ret						=	NextCSVToken( token );
		pOpt->fixationPro		=	atoi( token );
		
		ret						=	NextCSVToken( token );
		pOpt->optionType		=	atoi( token );
		
		for( i = 0; i < MAX_OPTION_ITEM_TYPE; i ++ )
		{
			ret						=	NextCSVToken( token );
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
	
	if( !LoadTokenBuffer( valueTableFilename ) ) return false;

	
	char		valueName[64];
	int			optionStoneGrade;
	int			itemType;
	float		value[MAX_OPTION_GRADE];	

	while( 1 )
	{
		ret = NextCSVToken( token );
		if( ret == CSV_EndOfLine ) break;
	}
	while( 1 )
	{
		
		ret						=	NextCSVToken( token );
		
		ret						=	NextCSVToken( token );
		strcpy(	valueName,	token );
		
		ret						=	NextCSVToken( token );
		pOpt					=	&optionTable[ atoi( token ) ];
		
		ret						=	NextCSVToken( token );
		optionStoneGrade		=	atoi( token );
		
		ret						=	NextCSVToken( token );
		itemType				=	atoi( token );
		
		for( i = 0; i < MAX_OPTION_GRADE; i ++ )
		{
			ret						=	NextCSVToken( token );
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

	if( !LoadTokenBuffer( filename ) ) return false;

	for( int i = 0; i < MAX_NUMBER_OF_SUMMON_MONSTER_TABLE; i ++ )
	{
		memset( &summonTable[i], 0, sizeof( SummonMonster_t ) );		
		summonTable[i].idx				= -1;
		summonTable[i].SummonWorldIdx	= -1;
	}

	
	while( 1 )
	{
		ret = NextCSVToken( token );
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

		
		ret						= NextCSVToken( token );

		
		ret						= NextCSVToken( token );
		pTable->idx				= atoi( token );

		
		ret						= NextCSVToken( token );		

		strcpy (pTable->doc, token);
		
		ret						= NextCSVToken( token );		
		strcpy (pTable->name, token);		

		
		ret						= NextCSVToken( token );		
		pTable->SummonTime = atof(token);

		
		ret						= NextCSVToken( token );		
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

int LoadConsiderationTable(char *filename, Consideration_t considerationTable[], int &considerationTableNumber)
{
	char				token[128];
	int					ret;
	
	int count = 0;		

	if( !LoadTokenBuffer( filename ) ) return false;

	for( int i = 0; i < MAX_NUMBER_OF_CONSIDERATION_TABLE; i ++ )
	{
		memset( &considerationTable[i], 0, sizeof( Consideration_t ) );		
		considerationTable[i].Idx				= -1;		
	}

	
	while( 1 )
	{
		ret = NextCSVToken( token );
		if( ret == CSV_EndOfLine ) break;
	}
	while( 1 )
	{		
		Consideration_t* pTable	= NULL;		

		if ( count > MAX_NUMBER_OF_CONSIDERATION_TABLE)
			return false;

		pTable					= &considerationTable[count];
		if ( pTable == NULL )	
			return false;

		
		ret						= NextCSVToken( token );
		pTable->Idx				= atoi(token);
		
		
		ret						= NextCSVToken( token );				
		strcpy (pTable->Name, g_itemTable[pTable->Idx].name);

		
		ret						= NextCSVToken( token );
		pTable->Quantity		= atoi(token);	

		
		ret						= NextCSVToken( token );
		pTable->Exp				= atoi(token);	

		
		ret						= NextCSVToken( token );
		pTable->GenExp			= atoi(token);	

		
		ret						= NextCSVToken( token );
		pTable->SelectRate		= atof(token);

		count++;
		if( ret == CSV_EndOfFile ) break;
		if( ret == CSV_EndOfLine ) continue;

		return( false );
	}

	
	considerationTableNumber = count;
	
	return true;
	
	return 1;
}



