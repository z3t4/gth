



#include "../global.h"
#include "SkillApp.h"

CSkillApp g_SkillApp;





CSkillApp::CSkillApp()
{

}

CSkillApp::~CSkillApp()
{

}
void CSkillApp::InitSkillTable()
{
	
}


bool CSkillApp::LoadSkillTableCheckValue(FILE *fp)
{





	CheckSize(fp,NUM_SKILL_INFOTYPE,"NUM_SKILL_INFOTYPE");
	CheckSize(fp,NUM_SKILL_TARGETTYPE,"NUM_SKILL_TARGETTYPE");
	CheckSize(fp,NUM_SKILL_TARGETINDEX,"NUM_SKILL_TARGETINDEX");
	CheckSize(fp,NUM_SKILL_MESSAGE_TYPE,"NUM_SKILL_MESSAGE_TYPE");
	CheckSize(fp,NUM_SKILL_SUB_ACTION,"NUM_SKILL_SUB_ACTION");
	CheckSize(fp,NUM_SKILL_CHECK,"NUM_SKILL_CHECK");
	CheckSize(fp,NUM_OF_SKILL_CLASS,"NUM_OF_SKILL_CLASS");
	




	CheckSize(fp,NUM_SKILL_ATTACKTYPE,"NUM_SKILL_ATTACKTYPE");
	CheckSize(fp,NUM_SKILL_HITPOWER,"NUM_SKILL_HITPOWER");
	CheckSize(fp,NUM_SKILL_HITRATE,"NUM_SKILL_HITRATE");
	CheckSize(fp,NUM_SKILL_PHYMINDMG,"NUM_SKILL_PHYMINDMG");
	CheckSize(fp,NUM_SKILL_PHYMAXDMG,"NUM_SKILL_PHYMAXDMG");
	CheckSize(fp,NUM_SKILL_POSMINDMG,"NUM_SKILL_POSMINDMG");
	CheckSize(fp,NUM_SKILL_POSMAXDMG,"NUM_SKILL_POSMAXDMG");
	CheckSize(fp,NUM_SKILL_NEGMINDMG,"NUM_SKILL_NEGMINDMG");
	CheckSize(fp,NUM_SKILL_NEGMAXDMG,"NUM_SKILL_NEGMAXDMG");
	CheckSize(fp,NUM_SKILL_CRITICALRATE,"NUM_SKILL_CRITICALRATE");
	CheckSize(fp,NUM_SKILL_CRITICALDAMAGE,"NUM_SKILL_CRITICALDAMAGE");
	CheckSize(fp,NUM_SKILL_DELAYTIME,"NUM_SKILL_DELAYTIME");
	CheckSize(fp,NUM_SKILL_REPEATNUM,"NUM_SKILL_REPEATNUM");
	CheckSize(fp,NUM_SKILL_REPEATDELAYTIME,"NUM_SKILL_REPEATDELAYTIME");
	CheckSize(fp,NUM_SKILL_ETCINFO,"NUM_SKILL_ETCINFO");




	
	CheckSize(fp,NUM_SKILL_AFF_TYPE,"NUM_SKILL_AFF_TYPE");
	CheckSize(fp,NUM_SKILL_AFF_AFFECTYPE,"NUM_SKILL_AFF_AFFECTYPE");
	CheckSize(fp,NUM_SKILL_AFF_STARTTIME,"NUM_SKILL_AFF_STARTTIME");
	CheckSize(fp,NUM_SKILL_AFF_KEEPTIME,"NUM_SKILL_AFF_KEEPTIME");
	CheckSize(fp,NUM_SKILL_AFF_POSMINDMG,"NUM_SKILL_AFF_POSMINDMG");
	CheckSize(fp,NUM_SKILL_AFF_HITRATE,"NUM_SKILL_AFF_HITRATE");
	CheckSize(fp,NUM_SKILL_AFF_SKILLTABLEIDX,"NUM_SKILL_AFF_SKILLTABLEIDX");
	CheckSize(fp,NUM_SKILL_AFF_DELAYTIME,"NUM_SKILL_AFF_DELAYTIME");





	CheckSize(fp,NUM_SKILL_AFF_DATA_COMMON,"NUM_SKILL_AFF_DATA_COMMON");





	CheckSize(fp,NUM_SKILL_AFF_SPE_TYPE,"NUM_SKILL_AFF_SPE_TYPE");
	CheckSize(fp,NUM_SKILL_AFF_SPE_SPECIALIDX,"NUM_SKILL_AFF_SPE_SPECIALIDX");
	CheckSize(fp,NUM_SKILL_AFF_SPE_SPECIALDATA,"NUM_SKILL_AFF_SPE_SPECIALDATA");





	CheckSize(fp,NUM_OF_SKILL_MES_TYPE,"NUM_OF_SKILL_MES_TYPE");
	CheckSize(fp,NUM_OF_SKILL_MES_FROMIDX,"NUM_OF_SKILL_MES_FROMIDX");
	CheckSize(fp,NUM_OF_SKILL_MES_FROMTYPE,"NUM_OF_SKILL_MES_FROMTYPE");
	CheckSize(fp,NUM_OF_SKILL_MES_NAME,"NUM_OF_SKILL_MES_NAME");
	CheckSize(fp,NUM_OF_SKILL_MES_TOTYPE,"NUM_OF_SKILL_MES_TOTYPE");
	CheckSize(fp,NUM_OF_SKILL_MES_TOIDX,"NUM_OF_SKILL_MES_TOIDX");
	CheckSize(fp,NUM_OF_SKILL_MES_WORLDIDX,"NUM_OF_SKILL_MES_WORLDIDX");
	CheckSize(fp,NUM_OF_SKILL_MES_SENDTIME,"NUM_OF_SKILL_MES_SENDTIME");
	CheckSize(fp,NUM_OF_SKILL_MES_RECEIVETIME,"NUM_OF_SKILL_MES_RECEIVETIME");
	CheckSize(fp,NUM_OF_SKILL_MES_DATA,"NUM_OF_SKILL_MES_DATA");





	CheckSize(fp,NUM_OF_SKILL_PARTY_CHECK,"NUM_OF_SKILL_PARTY_CHECK");
	CheckSize(fp,NUM_OF_SKILL_PARTY_MODE,"NUM_OF_SKILL_PARTY_MODE");





	CheckSize(fp,NUM_OF_SKILL_RA,"NUM_OF_SKILL_RA");

	return true;
}

bool CSkillApp::CheckSize(FILE *fp, int Val , LPSTR str)
{
	int size;
	fread( &size, sizeof( size ), 1, fp );
	if(size != Val)
	{
		MessageBox(NULL,str,"Size Error",MB_OK);
		return false;
	}

	return true;
}

bool CSkillApp::LoadSkillTableCheckSize(FILE *fp)
{
	int size;

	

	
	fread( &size, sizeof( size ), 1, fp );
	if(size != sizeof(CSkillDataValue)* MAX_SKILL_VALUE_LIST)
	{
		MessageBox(NULL,"CSkillDataValue Size Error",NULL,MB_OK);
	}

	
	fread( &size, sizeof( size ), 1, fp );
	if(size != sizeof(CSkillDataSubACtion)* MAX_SKILL_SUB_ACTION)
	{
		MessageBox(NULL,"CSkillDataSubACtion Size Error",NULL,MB_OK);
	}

	
	fread( &size, sizeof( size ), 1, fp );
	if(size != sizeof(CSkillDataFormat_Attack))
	{
		MessageBox(NULL,"CSkillDataFormat_Attack Size Error",NULL,MB_OK);
	}

	
	fread( &size, sizeof( size ), 1, fp );
	if(size != sizeof(CSkillDataFormat_Affect))
	{
		MessageBox(NULL,"CSkillDataFormat_Affect Size Error",NULL,MB_OK);
	}

	
	fread( &size, sizeof( size ), 1, fp );
	if(size != sizeof(CSkillData_Message))
	{
		MessageBox(NULL,"CSkillData_Message Size Error",NULL,MB_OK);
	}

	
	fread( &size, sizeof( size ), 1, fp );
	if(size != sizeof(CSkillDataCast))
	{
		MessageBox(NULL,"CSkillDataCast Size Error",NULL,MB_OK);
	}

	fread( &size, sizeof( size ), 1, fp );
	if(size != sizeof(CSkillEffect))
	{
		MessageBox(NULL,"CSkillEffect Size Error",NULL,MB_OK);
	}

	fread( &size, sizeof( size ), 1, fp );
	if(size != sizeof(CSkillRequire))
	{
		MessageBox(NULL,"CSkillRequire Size Error",NULL,MB_OK);
	}

	fread( &size, sizeof( size ), 1, fp );
	if(size != sizeof(CSkillDataFormat))
	{
		MessageBox(NULL,"CSkillDataFormat Size Error",NULL,MB_OK);
	}

	fread( &size, sizeof( size ), 1, fp );
	if(size != sizeof(CSkilTable))
	{
		MessageBox(NULL,"CSkilTable Size Error",NULL,MB_OK);
	}

	return true;
}


int CSkillApp::LoadSkillTable( char *filename,int &skillNumber )
{	
	FILE *fp;

	fp = fopen( filename, "rb" );

	tableHeader_t header;
	memset( &header, 0, sizeof( header ) );
	
	fread( &header, sizeof( header ), 1, fp );
	if( strcmp( header.id, SKILL_TABLE_HEADER ) )
	{
		fclose( fp );
		return( 0 );
	}





	if( header.version < 100 || header.version > 1000)
	{
		fclose( fp );

		MessageBox(NULL,"LoadSkillTable ... Error!!! Version ",NULL,MB_OK);
		return( 0 );
	}

	int i;

	skillNumber = header.recordNumber;

	CSkilTable temp;
	if(header.version == SKILL_TABLE_VERSION_101)
	{
		MessageBox(NULL,"스킬 테이블 버전이 틀립니다 SKILL_TABLE_VERSION_101",NULL,MB_OK);
	}
	else if(header.version == SKILL_TABLE_VERSION_105)
	{
		int size;

		
		LoadSkillTableCheckSize(fp);

		
		LoadSkillTableCheckValue(fp);

		int count=0;
		fread( &count, sizeof( count ), 1, fp );

		if(count >= MAX_SKILL_NUMBER)
		{
			count = MAX_SKILL_NUMBER-1;
			
			MessageBox(NULL,"치명적인 오류.. 스킬 수 틀림","오류",MB_OK);
			return 0;
		}

		for( i = 0; i < count; i ++ )
		{
			fread( &temp, sizeof( CSkilTable ), 1, fp );

			CSkilTable *pSkill = new CSkilTable;
			*pSkill = temp;
 			g_SkillTableDB.InsertDB(temp.m_nId,pSkill);
		}

		
		g_SkillCastDataList.Clear();
		int cast_size = MAX_SKILL_NUMBER;
		fread( &cast_size, sizeof( cast_size ), 1, fp );

		CSkillCastValue temp;
		for(i=0;i<cast_size;i++)
		{
			fread( &size, sizeof( size ), 1, fp );
			for(int k=0;k<size;k++)
			{
				fread( &temp, sizeof( CSkillCastValue ), 1, fp );
				CSkillCastValue *pData = new CSkillCastValue;
				*pData = temp;
				g_SkillCastDataList.Insert(pData->m_nIndex,pData);
			}
		}

		
		int skill_type_size=NUM_SKILL_TYPE;	
		fread( &skill_type_size, sizeof( skill_type_size ), 1, fp );

		if(skill_type_size != NUM_SKILL_TYPE)
		{
			MessageBox(NULL,"스킬 값 테이블의 크기가 지정된 크기와 다릅니다",NULL,MB_OK);

			if(skill_type_size > NUM_SKILL_TYPE)
			{
				MessageBox(NULL,"치명적인 오류.. skill_type_size > NUM_SKILL_TYPE","오류",MB_OK);
				return 0;
			}
		}

		for(i=0;i<skill_type_size;i++)
		{
			size = 0;
			fread( &size, sizeof( size ), 1, fp );

			if(size <=0 ) continue;

			for(int k=0;k<size;k++)
			{
				CSkillTableData tabletemp;

				fread( &tabletemp, sizeof( CSkillTableData ), 1, fp );

				CSkillTableData *pData = new CSkillTableData;

				*pData = tabletemp;
				g_SkillDataTableList.Insert(pData->m_bType,pData);
			}
		}

		
		size = 0;
		fread( &size, sizeof( size ), 1, fp );
		g_SkillItem.m_SkillItemData.Clear();

		for(i=0;i<size;i++)
		{
			CSkillItemSet temp;
			fread( &temp, sizeof( CSkillItemSet ), 1, fp );

			CSkillItemSet *Data = new CSkillItemSet ;
			*Data = temp;

			g_SkillItem.m_SkillItemData.Insert(Data);
		}

		g_SkillItem.SetDataList();

		
		size = 0;
		fread( &size, sizeof( size ), 1, fp );
		m_SkillTransInfo.m_TransList.Clear();

		for(i=0;i<size;i++)
		{			
			for(i=0;i<size;i++)
			{
				CSkillTransformUnit temp;
				fread( &temp, sizeof( CSkillTransformUnit ), 1, fp );

				CSkillTransformUnit *Data = new CSkillTransformUnit ;
				*Data = temp;
				m_SkillTransInfo.m_TransList.Insert(Data);
			}
		}
	}
	else
	{
		MessageBox(NULL,"Error! LoadSkillTable version",NULL,MB_OK);
	}
	fclose( fp );

	return( 1 );
}
