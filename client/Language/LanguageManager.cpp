



#include "../global_def.h"
#include "..\Terra ICT\CTerraLpack.h"	
	










CLanguageManager::CLanguageManager()
{		
	memset (m_Language, 0, LPACK_TYPE_COUNT);
	m_pFileMng = NULL;
}






CLanguageManager::~CLanguageManager()
{
	Destroy();
}






void CLanguageManager::Destroy()
{
	for (int i = 0 ; i < LPACK_TYPE_COUNT ;i++)
	{
		if (m_Language[i])
		{
			m_Language[i]->Destroy();
			delete m_Language[i];
			m_Language[i] = NULL;
		}
	}
}






void CLanguageManager::Init()
{
	char* lpackFilename[] = 
	{
		"LPACK",
		"QUESTINFO",
		"LPACK_QUEST",
		"HELP",
		"ANSWER",
		"ETC",
		"ABUSEWORD",
		"TIP",
		"TUTORIAL",
		"LPACK2",
		
		"AUTOSCROLL",

		
		"ABUSEWORD2",
		
		
		"SKILL_TABLE",
		"Terra",
		"CHARACTER_LIST",
		
		
		"ITEM_TABLE",

		NULL
	};	
	char expan[20], filename[20][128];
	memset (m_Language, 0, LPACK_TYPE_COUNT);

	
	switch( g_cgv.languageType )
	{
	case GTH_LANGUAGE_TYPE_KOREAN :
		strcpy( expan, ".kr" );
		break;
	case GTH_LANGUAGE_TYPE_ENGLISH :
		strcpy( expan, ".en" );
		break;
	case GTH_LANGUAGE_TYPE_JAPANESE :
		strcpy( expan, ".jp" );
		break;
	case GTH_LANGUAGE_TYPE_SIMPLIFIED_CHINESE :
		strcpy( expan, ".ch" );
		break;
	case GTH_LANGUAGE_TYPE_TRADITIONAL_CHINESE :	
		strcpy( expan, ".tw" );
		break;
	}
	
	CLPack*	pack = NULL;
	for( int i = 0; lpackFilename[i] != NULL; i ++ )
	{
		sprintf( filename[i], "%s%s", lpackFilename[i], expan );
	}

	pack = new CLPack( filename[0], m_pFileMng );				
	InsertLanuage(pack);
	pack = new CLPack_Quest( filename[1], m_pFileMng );			
	InsertLanuage(pack);
	pack = new CLPack( filename[2], m_pFileMng );				
	InsertLanuage(pack);
	pack = new CLPack( filename[3], m_pFileMng );				
	InsertLanuage(pack);
	pack = new CLPack( filename[4], m_pFileMng );				
	InsertLanuage(pack);
	pack = new CLPack_Etc( filename[5], m_pFileMng );			
	InsertLanuage(pack);
	pack = new CLPack( filename[6], m_pFileMng );				
	InsertLanuage(pack);
	pack = new CLPack( filename[7], m_pFileMng );				
	InsertLanuage(pack);
	pack = new CLPack( filename[8], m_pFileMng );				
	InsertLanuage(pack);
	pack = new CLPack( filename[9], m_pFileMng );
	InsertLanuage(pack);
	
	pack = new CLPack( filename[10], m_pFileMng );
	InsertLanuage(pack);


	
	pack = new CLPack( filename[11], m_pFileMng );
	InsertLanuage(pack);
	

	
	pack = new CLPack( filename[12], m_pFileMng );
	InsertLanuage(pack);
	

	pack = new CTerraLpack( filename[LPACK_TYPE_TERRA], m_pFileMng ); 
	InsertLanuage(pack);		

	
	pack = new CLPack( filename[LPACK_TYPE_CHARACTER_NAME], m_pFileMng );
	InsertLanuage(pack);
	
	
	pack = new CLPack( filename[LPACK_TYPE_ITEM], m_pFileMng );
	InsertLanuage(pack);
	
}






void CLanguageManager::InsertLanuage(CLPack* pack)
{
	m_Language[pack->GetLpackType()] = pack;
}






void CLanguageManager::SetFileManager(CFileMng* filemng)
{
	m_pFileMng = filemng;
}






bool CLanguageManager::CheckAbuseWord(char* String)
{
	WCHAR  TEMP[256];
	WCHAR *SerchStr = L" ~!@#$%^&*()_+\|/';:,.<>?=-`.[]";
	WCHAR *CopyStr;
	DXUtil_ConvertAnsiStringToWide(TEMP,String, -1);
	CopyStr = wcstok(TEMP,SerchStr);
	
	
	WCHAR ModifyStr[2048] = {0};
	
	if (SerchStr != NULL )
	{
		while (CopyStr != NULL)
		{			
			
			
			
			
			

			wcscat(ModifyStr, CopyStr);		
			CopyStr = wcstok(NULL,SerchStr);
		}
	}

	if ( m_Language[LPACK_TYPE_ABUSE] != NULL)
	{
		for (int i = 0; i < m_Language[LPACK_TYPE_ABUSE]->GetMessageCount(); i++)
		{			
			WCHAR deststr[200] = {0};
			DXUtil_ConvertAnsiStringToWide(deststr, GetMassage(LPACK_TYPE_ABUSE,i), -1);
			
			
			if ( wcsstr(ModifyStr, deststr) )
			{
				
				return 1;			
			}
		}
	}

	
	if ( m_Language[LPACK_TYPE_ABUSE2] != NULL)
	{
		for (int i = 0; i < m_Language[LPACK_TYPE_ABUSE2]->GetMessageCount(); i++)
		{			
			WCHAR deststr[200] = {0};
			DXUtil_ConvertAnsiStringToWide(deststr, GetMassage(LPACK_TYPE_ABUSE2,i), -1);
			
			
			if ( wcsstr(ModifyStr, deststr) )
			{
				
				return 1;			
			}
		}

#ifdef _DEBUG
		if(m_Language[LPACK_TYPE_ABUSE2]->GetMessageCount() <= 0)
		{
			MessageBox(NULL,"File Not Exit... LPACK_TYPE_ABUSE2 ",NULL,MB_OK);
		}
#endif
	}
	
	return 0;		
}





bool CLanguageManager::CheckCharacterName(char* String)
{
	WCHAR  TEMP[256];



	DXUtil_ConvertAnsiStringToWide(TEMP,String, -1);













	if ( m_Language[LPACK_TYPE_CHARACTER_NAME] != NULL)
	{
		for (int i = 0; i < m_Language[LPACK_TYPE_CHARACTER_NAME]->GetMessageCount(); i++)
		{			
			WCHAR deststr[200] = {0};
			DXUtil_ConvertAnsiStringToWide(deststr, GetMassage(LPACK_TYPE_CHARACTER_NAME,i), -1);			
			
			if ( wcscmp(TEMP, deststr) == 0)						
			
				return 1;			
		}
	}

	return 0;
}