#include "global.h"



int GTH_CheckValidationCharacterID( i3client_t *client, int characterID )
{
	int i;

	if (characterID < 0 || characterID >= MAX_CHARACTER_DATA) return false;
	if (client->dataNumber >= MAX_CHARACTER_DATA) return false;

	for (i=0; i<client->dataNumber; i++)
	{
		if (client->data[i].characterID == characterID) 
			return false;
	}
	

	return true;
}


void GTH_SetCharacterData(characterData_t *character, int characterID, charEditData_t *editData)
{
	int tableIdx;
	int i;
	tableIdx = editData->pcClass;

	
	GTH_InitCharacterData( character );

	character->characterID = characterID;
	strcpy(character->name, editData->name);
	character->pcClass = editData->pcClass;
	character->skinPart[0] = editData->skinPart[0];
	character->skinPart[1] = editData->skinPart[1];
	character->skinPart[2] = editData->skinPart[2];
	character->skinPart[3] = editData->skinPart[3];

	character->pcJob = g_characterTable[tableIdx].pcJob;

	character->age = g_characterTable[tableIdx].age;
	strcpy(character->rank, g_characterTable[tableIdx].rank);

	character->rankPoint = g_characterTable[tableIdx].rankPoint;

	
	character->worldIdx = g_characterTable[tableIdx].worldIdx;
	vec_copy(g_characterTable[tableIdx].position, character->position);
	vec_copy(g_characterTable[tableIdx].angles, character->angles);
	
	
	character->level = g_characterTable[tableIdx].level;
	character->exp = g_characterTable[tableIdx].exp;
	character->increaseExpRate = g_characterTable[tableIdx].increaseExpRate;

	character->selectedGen = g_characterTable[tableIdx].selectedGen;
	character->genLevel = g_characterTable[tableIdx].genLevel;
	character->genExp = g_characterTable[tableIdx].genExp;
	character->increaseGenExpRate = g_characterTable[tableIdx].increaseGenExpRate;
	for (i=0; i<GEN_NUMBER; i++)
	{
		character->genCapability[i] = g_characterTable[tableIdx].genCapability[i];
	}

	
	character->force = editData->force;
	character->soulpower = editData->soulpower;
	character->agility = editData->agility;
	character->vitality = editData->vitality;
	character->bonusStatusPoint = editData->bonusPoint;

	
	character->curRA = g_characterTable[tableIdx].curRA;
	character->curSA = g_characterTable[tableIdx].curSA;
	character->curChargeNSE = g_characterTable[tableIdx].curChargeNSE;
	character->curChargeSE = g_characterTable[tableIdx].curChargeSE;
	character->curChargeRA = g_characterTable[tableIdx].curChargeRA;
	character->curChargeSA = g_characterTable[tableIdx].curChargeSA;

	
	character->bindingIdx = g_characterTable[tableIdx].bindingIdx;				


	
	character->itemNumber = g_characterTable[tableIdx].itemNumber;

	for (i=0; i<MAX_INVENTORY_SIZE; i++)
		character->inventory[i] = g_characterTable[tableIdx].inventory[i];

	for (i=0; i<MAX_DEPOT_SIZE; i++)
		character->depot[i] = g_characterTable[tableIdx].depot[i];

	for (i=0; i<MAX_EQUIPMENT; i++)
		character->equipment[i] = g_characterTable[tableIdx].equipment[i];

	character->mouseInventory = g_characterTable[tableIdx].mouseInventory;

	
	for (i=0; i<g_characterTable[tableIdx].itemNumber; i++)
		memcpy(&character->item[i], &g_characterTable[tableIdx].item[i], sizeof(item_t));

	
	character->bonusSkillPoint = 0;
	

	character->skillNumber = g_characterTable[tableIdx].skillNumber;
	for (i=0; i<g_characterTable[tableIdx].skillNumber; i++)
	{
		 character->skill[i].tableIdx = g_characterTable[tableIdx].skill[i].tableIdx;
		 character->skill[i].level = g_characterTable[tableIdx].skill[i].level;
		 character->hotkeyIdx[i] = g_characterTable[tableIdx].hotkeyIdx[i];
		 character->hotkeyType[i] = g_characterTable[tableIdx].hotkeyType[i];
	}

}


void GTH_InitAllCharacterData(i3client_t *client)
{
	characterData_t *character;
	int i;

	client->dataNumber = 0;

	character = &client->data[0];
	for (i=0; i<MAX_CHARACTER_DATA; i++, character++)
	{
		GTH_InitCharacterData( character );
	}
} 


void GTH_InitCharacterData(characterData_t *character)
{
	int j, k;


	memset(character, 0, sizeof(characterData_t));

	character->characterID = -1;

	
	for (j=0; j<GEN_NUMBER; j++)
		character->genCapability[j] = -1;

	
	for (j=0; j<MAX_INVENTORY_SIZE; j++)
		character->inventory[j] = -1;

	for (j=0; j<MAX_DEPOT_SIZE; j++)
		character->depot[j] = -1;

	for (j=0; j<MAX_EQUIPMENT; j++)
		character->equipment[j] = -1;

	character->mouseInventory = -1;

	
	for (j=0; j<MAX_NUMBER_OF_OWNITEM; j++)
	{
		character->item[j].itemTableIdx = -1; 
		for (k=0; k<ITEM_OPTION_NUMBER; k++)
			character->item[j].optionIdx[k] = -1;
	}
	
	
	for (j=0; j<MAX_NUMBER_OF_SKILL; j++)
		character->skill[j].tableIdx = -1;

	for (j=0; j<MAX_NUMBER_OF_SKILL_VARIABLE; j++)
		character->skillVariable[j] = -1;

	
	for (j=0; j<MAX_NUMBER_OF_HOTKEY; j++)
		character->hotkeyIdx[j] = -1;


	
	for (j=0; j<MAX_COUNT_QUEST_VARIABLE; j++)
	{
		character->questVar[j].iVar = 65280;	
	}

	
	for (j=0; j<MAX_COUNT_QUEST_INVENTORY; j++)
	{
		character->questInventory[j].iItem = 65535; 
	}

	
	for (j=0; j<MAX_COUNT_QUEST_STATUS; j++)
	{
		character->questStatus[j].idx = j;
		character->questStatus[j].questIdx = -1;
	}

	
	for (j=0; j<MAX_PRECOCITY_SIZE; j++)
		character->precocityInventory[j] = -1;
	
	
	
	character->guildIdx = -1;
} 

extern char *curpos, *endpos;
int GTH_LoadCharacterTable()
{
	FILE *fp;
	int size;
	byte *buffer;
	char *token;
	characterTable_t *table;
	int tableIdx, itemIdx;
	int	indexType, idx;
	int i ;
	int skillNumber, skillIdx, skillLevel;
	char fileName[PATHSTRING], errorMsg[256];

	switch ( g_config.languageType )
	{
	case	GTH_LANGUAGE_TYPE_KOREAN:
		strcpy(fileName, "tables/chargenerate_kr.tbl");
		break;
	case	GTH_LANGUAGE_TYPE_ENGLISH:
		strcpy(fileName, "tables/chargenerate_en.tbl");
		break;
	case	GTH_LANGUAGE_TYPE_JAPANESE:
		strcpy(fileName, "tables/chargenerate_jp.tbl");
		break;
	case	GTH_LANGUAGE_TYPE_CHINESE:
		strcpy(fileName, "tables/chargenerate_ch.tbl");
		break;
	default:
		strcpy(fileName, "tables/chargenerate_kr.tbl");
		break;
	}

	fp = fopen( fileName , "rb" );

	if( !fp )   
	{
		sprintf(errorMsg, "Does not exist '%s' file", fileName);
		MessageBox(NULL, errorMsg, "Error!", MB_ICONHAND|MB_OK);
		return false;
	}

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	if(NULL != fp){ fclose(fp); fp=NULL; }

	curpos = (char *)buffer;
	endpos = curpos + size;

	g_numCharacterTable = 0;
	while ((token = NextToken()) != NULL)
    {
		if( !stricmp( token , "TABLE_IDX" ) )
		{
			tableIdx = atoi( NextArg() );
			if (tableIdx < 0 || tableIdx >= GEN_NUMBER) return 0;
			table = &g_characterTable[tableIdx];
			GTH_InitCharacterTable( table );
			g_numCharacterTable ++;
		}
		else if( !stricmp( token , "NAME" ) )
		{
			strcpy( table->name, NextArg() );
		}
		else if( !stricmp( token, "PC_CLASS" ) )
		{
			table->pcClass = atoi( NextArg() );
		}
		else if( !stricmp( token, "PC_JOB" ) )
		{
			table->pcJob = atoi( NextArg() );
		}
		else if( !stricmp( token , "SKIN_PART" ) )
		{
			for (i=0; i<DEFAULT_SKINPART_NUMBER; i++)
				table->skinPart[i] = atoi( NextArg() );
		}
		else if( !stricmp( token, "AGE" ) )
		{
			table->age = atoi( NextArg() );
		}
		else if( !stricmp( token , "RANK" ) )
		{
			strcpy( table->rank, NextArg() );
		}
		else if( !stricmp( token, "RANK_POINT" ) )
		{
			table->rankPoint = atoi( NextArg() );
		}
		else if( !stricmp( token, "WORLD_IDX" ) )
		{
			table->worldIdx = atoi( NextArg() );
		}
		else if( !stricmp( token, "POSITION" ) )
		{
			table->position[0] = (float) atoi( NextArg() );
			table->position[1] = (float) atoi( NextArg() );
			table->position[2] = (float) atoi( NextArg() );
		}
		else if( !stricmp( token, "ANGLE" ) )
		{
			table->angles[0] = 0;
			table->angles[1] = (float) atoi( NextArg() );
			table->angles[2] = 0;
		}
		else if( !stricmp( token, "LEVEL" ) )
		{
			table->level = atoi( NextArg() );
		}
		else if( !stricmp( token, "EXP" ) )
		{
			table->exp = _atoi64( NextArg() );
		}
		else if( !stricmp( token, "INC_EXPRATE" ) )
		{
			table->increaseExpRate = atoi( NextArg() );
		}
		else if( !stricmp( token, "SELECTED_GEN" ) )
		{
			table->selectedGen = atoi( NextArg() );
		}
		else if( !stricmp( token, "GEN_LEVEL" ) )
		{
			table->genLevel = atoi( NextArg() );
		}
		else if( !stricmp( token, "GEN_EXP" ) )
		{
			table->genExp = atoi( NextArg() );
		}
		else if( !stricmp( token, "INC_GENEXPRATE" ) )
		{
			table->increaseGenExpRate = atoi( NextArg() );
		}
		else if( !stricmp( token, "GEN_CAPABILITY" ) )
		{
			for (i=0; i<GEN_NUMBER; i++)
				table->genCapability[i] = atoi( NextArg() );
		}
		else if( !stricmp( token, "FORCE" ) )
		{
			table->force = atoi( NextArg() );
		}
		else if( !stricmp( token, "SOULPOWER" ) )
		{
			table->soulpower = atoi( NextArg() );
		}
		else if( !stricmp( token, "AGILITY" ) )
		{
			table->agility = atoi( NextArg() );
		}
		else if( !stricmp( token, "VITALITY" ) )
		{
			table->vitality = atoi( NextArg() );
		}
		else if( !stricmp( token, "BONUSPOINT" ) )
		{
			table->bonusStatusPoint = atoi( NextArg() );
		}
		else if( !stricmp( token, "RA" ) )
		{
			table->curRA = atoi( NextArg() );
		}
		else if( !stricmp( token, "SA" ) )
		{
			table->curSA = atoi( NextArg() );
		}
		else if( !stricmp( token, "CHARGENSE" ) )
		{
			table->curChargeNSE = atoi( NextArg() );
		}
		else if( !stricmp( token, "CHARGESE" ) )
		{
			table->curChargeSE = atoi( NextArg() );
		}
		else if( !stricmp( token, "CHARGERA" ) )
		{
			table->curChargeRA = atoi( NextArg() );
		}
		else if( !stricmp( token, "CHARGESA" ) )
		{
			table->curChargeSA = atoi( NextArg() );
		}
		else if( !stricmp( token, "BINDINGIDX" ) )
		{
			table->bindingIdx = atoi( NextArg() );
		}
		else if( !stricmp( token, "ITEM_NUMBER" ) )
		{
			table->itemNumber = atoi( NextArg() );
		}
		
		else if( !stricmp( token, "ITEM_IDX_INFO" ) )
		{
			itemIdx = atoi( NextArg() );
			indexType = atoi( NextArg() );
			idx = atoi( NextArg() );
			if (indexType == 0)			
				table->inventory[idx] = itemIdx;
			else if (indexType == 1 )	
				table->equipment[idx] = itemIdx;
		}
		else if( !stricmp( token, "ITEM_TABLEIDX" ) )
		{
			
			table->item[itemIdx].itemTableIdx = atoi( NextArg() );
			
		}
		else if( !stricmp( token , "ITEM_NAME" ) )
		{
			strcpy( table->item[itemIdx].name, NextArg() );
		}
		else if( !stricmp( token, "OPTIONNUMBER" ) )
		{
			table->item[itemIdx].optionNumber = atoi( NextArg() );
		}
		else if( !stricmp( token, "OPTIONIDX" ) )
		{
			for (i=0; i<ITEM_OPTION_NUMBER; i++)
				table->item[itemIdx].optionIdx[i] = atoi( NextArg() );
		}
		else if( !stricmp( token, "OPTIONVALUE" ) )
		{
			for (i=0; i<ITEM_OPTION_NUMBER; i++)
				table->item[itemIdx].optionValue[i] = atoi( NextArg() );
		}
		else if( !stricmp( token, "DURABILITY" ) )
		{
			table->item[itemIdx].durability = atoi( NextArg() );
		}
		else if( !stricmp( token, "REQLEVEL" ) )
		{
			table->item[itemIdx].reqLevel = atoi( NextArg() );
		}
		else if( !stricmp( token, "ITEMEXP" ) )
		{
			table->item[itemIdx].experience = atoi( NextArg() );
		}
		else if( !stricmp( token, "ITEMTIMER" ) )
		{
			table->item[itemIdx].timer = atoi( NextArg() );
		}
		else if( !stricmp( token, "DEFAULT_SKILL" ) )
		{
			skillNumber = atoi( NextArg() );
			skillIdx = atoi( NextArg() );
			skillLevel = atoi( NextArg() );

			if ( skillNumber > 0 )
			{
				table->skillNumber = skillNumber;
				table->skill[0].tableIdx = skillIdx;
				table->skill[0].iLevel = skillLevel;
				table->hotkeyIdx[0] = 0;
				table->hotkeyType[0] = 0;
			}
			else
			{
				table->skillNumber = 0;
				table->skill[0].tableIdx = -1;
				table->skill[0].iLevel = 0;
				table->hotkeyIdx[0] = -1;
				table->hotkeyType[0] = 0;
			}
		}
	}

	if(NULL != buffer){
		delete []buffer;
		buffer=NULL;
	}


	
	for (int iGenIdx = 0; iGenIdx < NUM_TABLE_GEN_TYPE ; iGenIdx++)
	{
		table = &g_characterTable[iGenIdx];
		table->TotalStatusPointForGenType = 0;
		table->TotalStatusPointForGenType += table->force;
		table->TotalStatusPointForGenType += table->soulpower;
		table->TotalStatusPointForGenType += table->agility;
		table->TotalStatusPointForGenType += table->vitality;	
		table->TotalStatusPointForGenType += table->bonusStatusPoint;
	}		
	
	g_characterTable[0].MaxHairNumber = 7;
	g_characterTable[0].MaxFaceNumber = 7;
	
	g_characterTable[1].MaxHairNumber = 6;
	g_characterTable[1].MaxFaceNumber = 6;
	
	g_characterTable[2].MaxHairNumber = 7;
	g_characterTable[2].MaxFaceNumber = 8;
	
	g_characterTable[3].MaxHairNumber = 7;
	g_characterTable[3].MaxFaceNumber = 7;

	

	return 1;
}


void GTH_InitCharacterTable( characterTable_t *characterTable )
{
	int i;

	for (i=0; i<MAX_INVENTORY_SIZE; i++)
		characterTable->inventory[i] = -1;

	for (i=0; i<MAX_DEPOT_SIZE; i++)
		characterTable->depot[i] = -1;

	for (i=0; i<MAX_EQUIPMENT; i++)
		characterTable->equipment[i] = -1;

	characterTable->mouseInventory = -1;

}

void GTH_CheckGameData( i3client_t *client )
{
	int i, j, k;
	characterData_t *data;
	int indexItemNumber, realItemNumber;
	int idx, emptyIdx;
	int found;
	int checkIdx;
	int errorNumber;

	for (i=0; i<client->dataNumber; i++)
	{
		errorNumber = 0;
		data = &client->data[i];

		
		indexItemNumber = 0;
		for (j=0; j<MAX_INVENTORY_SIZE; j++)
		{
			if ( data->inventory[j] >= 0) indexItemNumber++;
		}
		for (j=0; j<MAX_EQUIPMENT; j++)
		{
			if ( data->equipment[j] >= 0) indexItemNumber++;
		}
		for (j=0; j<MAX_DEPOT_SIZE; j++)
		{
			if ( data->depot[j] >= 0) indexItemNumber++;
		}
		if ( data->mouseInventory >= 0) indexItemNumber++;

		for (j=0; j<MAX_PRECOCITY_SIZE; j++)
		{
			if ( data->precocityInventory[j] >= 0) indexItemNumber++;
		}

		
		realItemNumber = 0;
		for (j=0; j<MAX_NUMBER_OF_OWNITEM; j++)
		{
			if ( data->item[j].itemTableIdx >= 0 ) realItemNumber++;
		}		







			
			for (j=0; j<MAX_EQUIPMENT; j++)
			{
				if (data->equipment[j] == -1) continue;

				checkIdx = data->equipment[j];

				for(k=j+1; k<MAX_EQUIPMENT; k++)
				{
					if ( data->equipment[k] == -1 ) continue;
					if ( checkIdx == data->equipment[k] )
					{
						data->equipment[k] = -1;
						g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 착용 %d에서 중복된 아이템 인덱스 %d번이 삭제됨)",
							client->id, data->name, data->characterID, client->serverGroupID, k, checkIdx);
						errorNumber ++;
					}
				}

				for(k=0; k<MAX_PRECOCITY_SIZE; k++)
				{
					if ( data->precocityInventory[k] == -1 ) continue;
					if ( checkIdx == data->precocityInventory[k] )
					{
						data->precocityInventory[k] = -1;
						data->precocityTime[k] = 0;
						g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 숙성 %d에서 중복된 아이템 인덱스 %d번이 삭제됨)",
							client->id, data->name, data->characterID, client->serverGroupID, k, checkIdx);
						errorNumber ++;
					}
				}
				for(k=0; k<MAX_INVENTORY_SIZE; k++) 
				{
					if ( data->inventory[k] == -1 ) continue;
					if ( checkIdx == data->inventory[k] )
					{
						data->inventory[k] = -1;
						g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 인벤토리 %d에서 중복된 아이템 인덱스 %d번이 삭제됨)",
							client->id, data->name, data->characterID, client->serverGroupID, k, checkIdx);
						errorNumber ++;
					}
				}
				for(k=0; k<MAX_DEPOT_SIZE; k++)
				{
					if ( data->depot[k] == -1 ) continue;
					if ( checkIdx == data->depot[k] )
					{
						data->depot[k] = -1;
						g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 창고 %d에서 중복된 아이템 인덱스 %d번이 삭제됨)",
							client->id, data->name, data->characterID, client->serverGroupID, k, checkIdx);
						errorNumber ++;
					}
				}
				if ( checkIdx == data->mouseInventory )
				{
					data->mouseInventory = -1;
					g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 마우스 인벤토리에서 중복된 아이템 인덱스 %d번이 삭제됨)",
						client->id, data->name, data->characterID, client->serverGroupID, checkIdx);
					errorNumber ++;
				}
			}


			for (j=0; j<MAX_PRECOCITY_SIZE; j++)
			{
				if (data->precocityInventory[j] == -1) continue;

				checkIdx = data->precocityInventory[j];

				for(k=j+1; k<MAX_PRECOCITY_SIZE; k++)
				{
					if ( data->precocityInventory[k] == -1 ) continue;
					if ( checkIdx == data->precocityInventory[k] )
					{
						data->precocityInventory[k] = -1;
						data->precocityTime[k] = 0;
						g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 숙성 %d에서 중복된 아이템 인덱스 %d번이 삭제됨)",
							client->id, data->name, data->characterID, client->serverGroupID, k, checkIdx);
						errorNumber ++;
					}
				}
				for(k=j+1; k<MAX_INVENTORY_SIZE; k++)	
				{
					if ( data->inventory[k] == -1 ) continue;
					if ( checkIdx == data->inventory[k] )
					{
						data->inventory[k] = -1;
						g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 인벤토리 %d에서 중복된 아이템 인덱스 %d번이 삭제됨)",
							client->id, data->name, data->characterID, client->serverGroupID, k, checkIdx);
						errorNumber ++;
					}
				}
				for(k=0; k<MAX_DEPOT_SIZE; k++)
				{
					if ( data->depot[k] == -1 ) continue;
					if ( checkIdx == data->depot[k] )
					{
						data->depot[k] = -1;
						g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 창고 %d에서 중복된 아이템 인덱스 %d번이 삭제됨)",
							client->id, data->name, data->characterID, client->serverGroupID, k, checkIdx);
						errorNumber ++;
					}
				}
				if ( checkIdx == data->mouseInventory )
				{
					data->mouseInventory = -1;
					g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 마우스 인벤토리에서 중복된 아이템 인덱스 %d번이 삭제됨)",
						client->id, data->name, data->characterID, client->serverGroupID, checkIdx);
					errorNumber ++;
				}
			}

			for (j=0; j<MAX_INVENTORY_SIZE; j++)
			{
				if (data->inventory[j] == -1) continue;

				checkIdx = data->inventory[j];

				for(k=j+1; k<MAX_INVENTORY_SIZE; k++)	
				{
					if ( data->inventory[k] == -1 ) continue;
					if ( checkIdx == data->inventory[k] )
					{
						data->inventory[k] = -1;
						g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 인벤토리 %d에서 중복된 아이템 인덱스 %d번이 삭제됨)",
							client->id, data->name, data->characterID, client->serverGroupID, k, checkIdx);
						errorNumber ++;
					}
				}
				for(k=0; k<MAX_DEPOT_SIZE; k++)
				{
					if ( data->depot[k] == -1 ) continue;
					if ( checkIdx == data->depot[k] )
					{
						data->depot[k] = -1;
						g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 창고 %d에서 중복된 아이템 인덱스 %d번이 삭제됨)",
							client->id, data->name, data->characterID, client->serverGroupID, k, checkIdx);
						errorNumber ++;
					}
				}
				if ( checkIdx == data->mouseInventory )
				{
					data->mouseInventory = -1;
					g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 마우스 인벤토리에서 중복된 아이템 인덱스 %d번이 삭제됨)",
						client->id, data->name, data->characterID, client->serverGroupID, checkIdx);
					errorNumber ++;
				}
			}

			for (j=0; j<MAX_DEPOT_SIZE; j++)
			{
				if (data->depot[j] == -1) continue;

				checkIdx = data->depot[j];

				for(k=j+1; k<MAX_DEPOT_SIZE; k++)	
				{
					if ( data->depot[k] == -1 ) continue;
					if ( checkIdx == data->depot[k] )
					{
						data->depot[k] = -1;
						g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 창고 %d에서 중복된 아이템 인덱스 %d번이 삭제됨)",
							client->id, data->name, data->characterID, client->serverGroupID, k, checkIdx);
						errorNumber ++;
					}
				}
				if ( checkIdx == data->mouseInventory )
				{
					data->mouseInventory = -1;
					g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 마우스 인벤토리에서 중복된 아이템 인덱스 %d번이 삭제됨)",
						client->id, data->name, data->characterID, client->serverGroupID, checkIdx);
					errorNumber ++;
				}
			}


			
			for (idx=0; idx<MAX_NUMBER_OF_OWNITEM; idx++)
			{
				if ( data->item[idx].itemTableIdx == -1) continue;

				found = false;
				
				for (j=0; j<MAX_INVENTORY_SIZE; j++)
				{
					if ( data->inventory[j] == idx )
					{
						found = true;
						break;
					}
				}
				if ( found )	continue; 

				
				for (j=0; j<MAX_EQUIPMENT; j++)
				{
					if ( data->equipment[j] == idx )
					{
						found = true;
						break;
					}
				}	
				if ( found )	continue; 

				
				for (j=0; j<MAX_DEPOT_SIZE; j++)
				{
					if ( data->depot[j] == idx )
					{
						found = true;
						break;
					}
				}	
				if ( found )	continue; 

				
				
				for (j=0; j<MAX_PRECOCITY_SIZE; j++)
				{
					if ( data->precocityInventory[j] == idx )
					{
						found = true;
						break;
					}
				}	
				if ( found )	continue; 

				
				if ( data->mouseInventory == idx ) 	
				{
					found = true;
				}		
				if ( found )	continue; 

				
				emptyIdx = GTH_FindEmptyInventoryPos( data );
				errorNumber ++;

				if ( emptyIdx >= 0 ) 
				{
					data->inventory[emptyIdx] = idx;
					g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 인벤토리 %d에서 아이템 인덱스 %d번이 추가됨)",
						client->id, data->name, data->characterID, client->serverGroupID, emptyIdx, idx);
				}
				else	
				{
					g_DBGameServer[client->serverGroupID]->DeleteItem(client, data->characterID, idx);

					g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 아이템 삭제 : %d (%s,%d,%d, %d,%d,%d,%d,%d,%d, %d,%d,%d,%d,%d,%d, %d,%d,%d,%d))",
						client->id, data->name, data->characterID, client->serverGroupID, idx, 
						data->item[idx].name, data->item[idx].optionNumber, data->item[idx].itemTableIdx,
						data->item[idx].optionIdx[0], data->item[idx].optionIdx[1], data->item[idx].optionIdx[2], 
						data->item[idx].optionIdx[3], data->item[idx].optionIdx[4], data->item[idx].optionIdx[5],
						data->item[idx].optionValue[0], data->item[idx].optionValue[1], data->item[idx].optionValue[2], 
						data->item[idx].optionValue[3], data->item[idx].optionValue[4], data->item[idx].optionValue[5],
						data->item[idx].durability, data->item[idx].reqLevel, data->item[idx].experience, data->item[idx].timer);

					
					memset(&data->item[idx], 0, sizeof(item_t));
					data->item[idx].itemTableIdx = -1;
					for (j=0; j<ITEM_OPTION_NUMBER; j++)
						data->item[idx].optionIdx[j] = -1;

					--realItemNumber;
				}
			}	

			
			for (j=0; j<MAX_INVENTORY_SIZE; j++)
			{
				if ( data->inventory[j] == -1) continue;
				idx = data->inventory[j];

				if ( data->item[idx].itemTableIdx >= 0 )
					continue;

				data->inventory[j] = -1;
				g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 인벤토리%02d에서 아이템 인덱스 %d번을 삭제함)",
					client->id, data->name, data->characterID, client->serverGroupID, j, idx);
				errorNumber ++;
			}
			for (j=0; j<MAX_EQUIPMENT; j++)
			{
				if ( data->equipment[j] == -1) continue;
				idx = data->equipment[j];

				if ( data->item[idx].itemTableIdx >= 0 )
					continue;

				data->equipment[j] = -1;
				g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 착용%02d에서 아이템 인덱스 %d번을 삭제함)",
					client->id, data->name, data->characterID, client->serverGroupID, j, idx);
				errorNumber ++;
			}
			for (j=0; j<MAX_DEPOT_SIZE; j++)
			{
				if ( data->depot[j] == -1) continue;
				idx = data->depot[j];

				if ( data->item[idx].itemTableIdx >= 0 )
					continue;

				data->depot[j] = -1;
				g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 창고%02d에서 아이템 인덱스 %d번을 삭제함)",
					client->id, data->name, data->characterID, client->serverGroupID, j, idx);
				errorNumber ++;
			}

			for (j=0; j<MAX_PRECOCITY_SIZE; j++)
			{
				if ( data->precocityInventory[j] == -1) continue;
				idx = data->precocityInventory[j];

				if ( data->item[idx].itemTableIdx >= 0 )
					continue;

				data->precocityInventory[j] = -1;
				g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 숙성%02d에서 아이템 인덱스 %d번을 삭제함)",
					client->id, data->name, data->characterID, client->serverGroupID, j, idx);
				errorNumber ++;
			}

			if ( data->mouseInventory >= 0 )
			{
				idx = data->mouseInventory;
				if ( data->item[idx].itemTableIdx == -1 )	
				{
					data->mouseInventory = -1;
					g_logSystem->Write("게임데이터 불일치 오류 수정 내역: (ID: %s, name: %s(%d), serverGroup: %d, 마우스 인벤토리에서 아이템 인덱스 %d번을 삭제함)",
						client->id, data->name, data->characterID, client->serverGroupID, idx);
					errorNumber ++;

					
					
				}
			}

			
			data->itemNumber = realItemNumber;

			
			if ( errorNumber > 0 )
			{
				g_DBGameServer[client->serverGroupID]->UpdateGameOwnItemInfo(client, data);
				g_logSystem->Write("%s님 서버군: %d, 캐릭터:%s(%d) 게임데이터 수정 로딩 완료",
					client->id, client->serverGroupID, data->name, data->characterID);
			}	
			else
			{
				g_logSystem->Write("%s님 서버군: %d, 캐릭터:%s(%d) 게임데이터 정상 로딩 완료",
					client->id, client->serverGroupID, data->name, data->characterID);
			}

	}	
}

int GTH_FindEmptyInventoryPos(characterData_t *data)
{
	int i;

	for( i = 0; i < MAX_INVENTORY_SIZE; i ++ )
	{
		if( data->inventory[i] == -1 )
		{
			return i;
		}
	}
	return -1;
}
