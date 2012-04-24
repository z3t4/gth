




#include "../global_def.h"




CSkillFile g_SkillFile;
CSkillFile::CSkillFile()
{

}

CSkillFile::~CSkillFile()
{

}


bool CSkillFile::GetData(void *p,int size)
{
	memcpy(p, m_Buffer, size);
	m_Buffer += size;

	return true;
}


int CSkillFile::LoadSkillTable( char *filename)
{
	BYTE *buf2;

	buf2 = g_fileMng.ReadFileFromPack( filename );
	m_Buffer = buf2;

	tableHeader_t header;
	memset( &header, 0, sizeof( header ) );
	memcpy( &header, m_Buffer, sizeof( header ) );
	m_Buffer += sizeof( header );
	
	if( strcmp( header.id, SKILL_TABLE_HEADER ) )
	{
		return false;
	}

	int i;

	g_skillNumber = header.recordNumber;

	CSkilTable temp;
	if(header.version == SKILL_TABLE_VERSION_101 || header.version == SKILL_TABLE_VERSION_102)
	{
		MessageBox(NULL,"구 버전 스킬 테이블 입니다",NULL,MB_OK);
	}

	
	else if(header.version == SKILL_TABLE_VERSION_105)
	{
		int size;

		
		LoadSkillTableCheckSize();

		LoadSkillTableCheckValue();

		int count=0;

		GetData(&count,sizeof(count));
		if(count >= MAX_SKILL_NUMBER)
		{
			count = MAX_SKILL_NUMBER-1;
		}

		for( i = 0; i < count; i ++ )
		{
			GetData(&temp,sizeof(CSkilTable));

			CSkilTable *pSkill = new CSkilTable;
			*pSkill = temp;
 			g_SkillTableDB.InsertDB(temp.m_nId,pSkill);
		}

		
		g_SkillCastDataList.Clear();
		int cast_size = MAX_SKILL_NUMBER;
		GetData(&cast_size,sizeof(cast_size));
	

		CSkillCastValue temp;
		for(i=0;i<cast_size;i++)
		{
		
			GetData(&size,sizeof(size));
			for(int k=0;k<size;k++)
			{
				GetData(&temp,sizeof(CSkillCastValue));
				CSkillCastValue *pData = new CSkillCastValue;
				*pData = temp;
				g_SkillCastDataList.Insert(pData->m_nIndex,pData);
			}
		}

		
		int skill_type_size=NUM_SKILL_TYPE;	
		
		GetData(&skill_type_size,sizeof(skill_type_size));

		if(skill_type_size != NUM_SKILL_TYPE)
		{
			MessageBox(NULL,"스킬 값 테이블의 크기가 지정된 크기와 다릅니다",NULL,MB_OK);
			
		}

		for(i=0;i<skill_type_size;i++)
		{
			size = 0;
		
			GetData(&size,sizeof(size));

			if(size <=0 ) continue;

			for(int k=0;k<size;k++)
			{
				CSkillTableData tabletemp;
				GetData(&tabletemp,sizeof(CSkillTableData));

				
				if(i >= NUM_SKILL_TYPE) continue;

				CSkillTableData *pData = new CSkillTableData;

				*pData = tabletemp;
				g_SkillDataTableList.Insert(pData->m_bType,pData);
			}
		}

		
		size = 0;
		GetData(&size,sizeof(size));

		CSkillItemData * pSkillItemList = g_SkillItem.GetSkillItemList();
		pSkillItemList->Clear();

		for(i=0;i<size;i++)
		{
			CSkillItemSet temp;
			GetData(&temp,sizeof(CSkillItemSet));

			CSkillItemSet *Data = new CSkillItemSet;
			*Data = temp;
			pSkillItemList->Insert(Data);
		}
		g_SkillItem.SetDataList();

		
		size = 0;
		GetData(&size,sizeof(size));

		CSkillTransformData * pTransList = g_SkillTransInfo.Get();
		pTransList->Clear();

		for(i=0;i<size;i++)
		{
			CSkillTransformUnit temp;
			GetData(&temp,sizeof(CSkillTransformUnit));

			CSkillTransformUnit *Data = new CSkillTransformUnit;
			*Data = temp;
			pTransList->Insert(Data);
		}

		GTH_SAFE_FREE ( buf2 );
		return true;
	}
	
	GTH_SAFE_FREE ( buf2 );
	return false;
}


void  CSkillFile::DebugOutString(LPCTSTR lpszFormat, ...)
{
#ifdef _DEBUG
	va_list argList;
	va_start(argList, lpszFormat);

	char szOutputMessage[1024];

	vsprintf(szOutputMessage, lpszFormat, argList);

	
	strcat(szOutputMessage,"\n");
	va_end(argList);

	OutputDebugString ( szOutputMessage );
#endif

}


bool CSkillFile::CheckSize(int Val , LPSTR str)
{
	int size;
	GetData(&size,sizeof(size));
	if(size != Val)
	{
		MessageBox(NULL,str,"Size Error",MB_OK);
		return false;
	}

	return true;
}

bool CSkillFile::LoadSkillTableCheckValue()
{





	CheckSize(NUM_SKILL_INFOTYPE,"NUM_SKILL_INFOTYPE");
	CheckSize(NUM_SKILL_TARGETTYPE,"NUM_SKILL_TARGETTYPE");

	CheckSize(NUM_SKILL_TARGETINDEX,"NUM_SKILL_TARGETINDEX");
	CheckSize(NUM_SKILL_MESSAGE_TYPE,"NUM_SKILL_MESSAGE_TYPE");
	CheckSize(NUM_SKILL_SUB_ACTION,"NUM_SKILL_SUB_ACTION");
	CheckSize(NUM_SKILL_CHECK,"NUM_SKILL_CHECK");
	CheckSize(NUM_OF_SKILL_CLASS,"NUM_OF_SKILL_CLASS");
	




	CheckSize(NUM_SKILL_ATTACKTYPE,"NUM_SKILL_ATTACKTYPE");
	CheckSize(NUM_SKILL_HITPOWER,"NUM_SKILL_HITPOWER");
	CheckSize(NUM_SKILL_HITRATE,"NUM_SKILL_HITRATE");
	CheckSize(NUM_SKILL_PHYMINDMG,"NUM_SKILL_PHYMINDMG");
	CheckSize(NUM_SKILL_PHYMAXDMG,"NUM_SKILL_PHYMAXDMG");
	CheckSize(NUM_SKILL_POSMINDMG,"NUM_SKILL_POSMINDMG");
	CheckSize(NUM_SKILL_POSMAXDMG,"NUM_SKILL_POSMAXDMG");
	CheckSize(NUM_SKILL_NEGMINDMG,"NUM_SKILL_NEGMINDMG");
	CheckSize(NUM_SKILL_NEGMAXDMG,"NUM_SKILL_NEGMAXDMG");
	CheckSize(NUM_SKILL_CRITICALRATE,"NUM_SKILL_CRITICALRATE");
	CheckSize(NUM_SKILL_CRITICALDAMAGE,"NUM_SKILL_CRITICALDAMAGE");
	CheckSize(NUM_SKILL_DELAYTIME,"NUM_SKILL_DELAYTIME");
	CheckSize(NUM_SKILL_REPEATNUM,"NUM_SKILL_REPEATNUM");
	CheckSize(NUM_SKILL_REPEATDELAYTIME,"NUM_SKILL_REPEATDELAYTIME");
	CheckSize(NUM_SKILL_ETCINFO,"NUM_SKILL_ETCINFO");




	
	CheckSize(NUM_SKILL_AFF_TYPE,"NUM_SKILL_AFF_TYPE");
	CheckSize(NUM_SKILL_AFF_AFFECTYPE,"NUM_SKILL_AFF_AFFECTYPE");
	CheckSize(NUM_SKILL_AFF_STARTTIME,"NUM_SKILL_AFF_STARTTIME");
	CheckSize(NUM_SKILL_AFF_KEEPTIME,"NUM_SKILL_AFF_KEEPTIME");
	CheckSize(NUM_SKILL_AFF_POSMINDMG,"NUM_SKILL_AFF_POSMINDMG");
	CheckSize(NUM_SKILL_AFF_HITRATE,"NUM_SKILL_AFF_HITRATE");
	CheckSize(NUM_SKILL_AFF_SKILLTABLEIDX,"NUM_SKILL_AFF_SKILLTABLEIDX");
	CheckSize(NUM_SKILL_AFF_DELAYTIME,"NUM_SKILL_AFF_DELAYTIME");





	CheckSize(NUM_SKILL_AFF_DATA_COMMON,"NUM_SKILL_AFF_DATA_COMMON");





	CheckSize(NUM_SKILL_AFF_SPE_TYPE,"NUM_SKILL_AFF_SPE_TYPE");
	CheckSize(NUM_SKILL_AFF_SPE_SPECIALIDX,"NUM_SKILL_AFF_SPE_SPECIALIDX");
	CheckSize(NUM_SKILL_AFF_SPE_SPECIALDATA,"NUM_SKILL_AFF_SPE_SPECIALDATA");





	CheckSize(NUM_OF_SKILL_MES_TYPE,"NUM_OF_SKILL_MES_TYPE");
	CheckSize(NUM_OF_SKILL_MES_FROMIDX,"NUM_OF_SKILL_MES_FROMIDX");
	CheckSize(NUM_OF_SKILL_MES_FROMTYPE,"NUM_OF_SKILL_MES_FROMTYPE");
	CheckSize(NUM_OF_SKILL_MES_NAME,"NUM_OF_SKILL_MES_NAME");
	CheckSize(NUM_OF_SKILL_MES_TOTYPE,"NUM_OF_SKILL_MES_TOTYPE");
	CheckSize(NUM_OF_SKILL_MES_TOIDX,"NUM_OF_SKILL_MES_TOIDX");
	CheckSize(NUM_OF_SKILL_MES_WORLDIDX,"NUM_OF_SKILL_MES_WORLDIDX");
	CheckSize(NUM_OF_SKILL_MES_SENDTIME,"NUM_OF_SKILL_MES_SENDTIME");
	CheckSize(NUM_OF_SKILL_MES_RECEIVETIME,"NUM_OF_SKILL_MES_RECEIVETIME");
	CheckSize(NUM_OF_SKILL_MES_DATA,"NUM_OF_SKILL_MES_DATA");





	CheckSize(NUM_OF_SKILL_PARTY_CHECK,"NUM_OF_SKILL_PARTY_CHECK");
	CheckSize(NUM_OF_SKILL_PARTY_MODE,"NUM_OF_SKILL_PARTY_MODE");





	CheckSize(NUM_OF_SKILL_RA,"NUM_OF_SKILL_RA");


	return true;
}


bool CSkillFile::LoadSkillTableCheckSize()
{
	int size;
	
	GetData(&size,sizeof(size));
	if(size != sizeof(CSkillDataValue)* MAX_SKILL_VALUE_LIST)
	{
		MessageBox(NULL,"SkillDataValue Size Error",NULL,MB_OK);
		return false;
	}

	
	GetData(&size,sizeof(size));
	if(size != sizeof(CSkillDataSubACtion)* MAX_SKILL_SUB_ACTION)
	{
		MessageBox(NULL,"SkillDataSubACtion Size Error",NULL,MB_OK);
		return false;
	}

	
	GetData(&size,sizeof(size));
	if(size != sizeof(CSkillDataFormat_Attack))
	{
		MessageBox(NULL,"SkillDataFormat_Attack Size Error",NULL,MB_OK);
		return false;
	}

	
	GetData(&size,sizeof(size));
	if(size != sizeof(CSkillDataFormat_Affect))
	{
		MessageBox(NULL,"SkillDataFormat_Affect Size Error",NULL,MB_OK);
		return false;
	}

	
	GetData(&size,sizeof(size));
	if(size != sizeof(CSkillData_Message))
	{
		MessageBox(NULL,"SkillData_Message Size Error",NULL,MB_OK);
		return false;
	}

	
	GetData(&size,sizeof(size));
	if(size != sizeof(CSkillDataCast))
	{
		MessageBox(NULL,"SkillDataCast Size Error",NULL,MB_OK);
		return false;
	}

	GetData(&size,sizeof(size));
	if(size != sizeof(CSkillEffect))
	{
		MessageBox(NULL,"SkillEffect Size Error",NULL,MB_OK);
		return false;
	}

	GetData(&size,sizeof(size));
	if(size != sizeof(CSkillRequire))
	{
		MessageBox(NULL,"SkillRequire Size Error",NULL,MB_OK);
		return false;
	}

	GetData(&size,sizeof(size));
	if(size != sizeof(CSkillDataFormat))
	{
		MessageBox(NULL,"SkillDataFormat Size Error",NULL,MB_OK);
		return false;
	}

	GetData(&size,sizeof(size));
	if(size != sizeof(CSkilTable))
	{
		MessageBox(NULL,"SkilTable Size Error",NULL,MB_OK);
		return false;
	}

	return true;
}

