



#include "../global_def.h"






bool	CTutorialMng::m_bTutorialMode  = 0;







int CS_InterpretorFunction( ClientScript_t* pCS )
{
	int param[5];								
	int variable = false;						
	int paramAddr;								
	int argCount;

	paramAddr	= CS_GetCharData( pCS );
	
	argCount	= CS_FUNC[paramAddr].argCount;
	if ( argCount > 0 )
		CS_InterpretorParameter( pCS, param, argCount );

	variable	= CS_FUNC[paramAddr].pFunc( pCS, param );
	return( variable );
}






int CS_InterpretorDefine( ClientScript_t* pCS, int paramAddr )
{
	int variable;								

	switch( paramAddr )
	{
	
	case 0 :
		variable = true;
		break;
	
	case 1 :
		variable = false;
		break;	

	
	case 2:
		
		variable = GTH_EV_CHAR_MOUSEWALK;		
		break;

	
	case 3:
		
		variable = GTH_EV_CHAR_MOUSERUN;		
		break;

	
	case 4:
		variable = GTH_EV_CHAR_SIT;
		break;	

	
	case 5:				
		variable = 1;
		break;		

	
	case 6:				
		variable = 2;
		break;

		
	case 7:				
		variable = 3;
		break;

		
	case 8:				
		variable = 4;
		break;
		
	case 9:				
		variable = 5;
		break;		
	}
	return( variable );


}







void CS_LoadScript( ClientScript_t* pCS, char* filename )
{
	FILE *fp;									
	int n, l, type, var, i;						
	char name[128];								

	fp = fopen( filename, "rb" );

	
	fread( &n, sizeof( int ), 1, fp );
	for( i = 0; i < n; i ++ )
	{
		
		fread( &l, sizeof( int ), 1, fp );
		fread( name, l, 1, fp );
		
		fread( &type, sizeof( int ), 1, fp );
		
		fread( &var, sizeof( int ), 1, fp );
		pCS->ScriptVariable[ i ] = var;
	}

	
	fread( &n, sizeof( int ), 1, fp );
	for( i = 0; i < n; i ++ )
	{
		
		fread( &l, sizeof( int ), 1, fp );
		fread( name, l, 1, fp );
		
		fread( &var, sizeof( int ), 1, fp );
		pCS->ScriptProcedureAddr[ i ] = var;
	}

	
	fread( &n, sizeof( int ), 1, fp );

	fread( pCS->ScriptData, n, 1, fp );
	fclose( fp );
	pCS->ScriptAddr = 0;

	
	pCS->callProcedureAddr = 0;
	pCS->isCall = false;

}







void CS_LoadScript( ClientScript_t* pCS, BYTE* FilePointer)
{	
	int n, l, type, var;						
	char name[128];								

	BYTE* pTempFile = FilePointer;
	
	memcpy ( &n , pTempFile, sizeof ( int ) );
	pTempFile += sizeof ( int );
	
	
	for( int i = 0; i < n; i ++ )
	{
		memcpy ( &l, pTempFile, sizeof ( int ) );
		pTempFile+= sizeof ( int );

		memcpy ( &name, pTempFile, l );
		pTempFile += l;

		memcpy ( &type, pTempFile, sizeof ( int ) );
		pTempFile+= sizeof ( int );

		memcpy ( &var, pTempFile, sizeof ( int ) );
		pTempFile+= sizeof ( int );

		pCS->ScriptVariable[ i ] = var;

	}

	
	memcpy ( &n, pTempFile, sizeof ( int ) );
	pTempFile+= sizeof ( int );
	for( i = 0; i < n; i ++ )
	{		
		memcpy ( &l, pTempFile, sizeof ( int ) );
		pTempFile += sizeof ( int );

		memcpy ( &name, pTempFile, l );
		pTempFile+= l;
		memcpy ( &var, pTempFile, sizeof ( int ) );
		pTempFile+= sizeof ( int );
		
		pCS->ScriptProcedureAddr[ i ] = var;
	}

	
	memcpy ( &n, pTempFile, sizeof ( int ) );
		pTempFile += sizeof ( int );

	memcpy ( &pCS->ScriptData, pTempFile,  n);
		pTempFile += n;

	
	pCS->ScriptAddr = 0;

	
	pCS->callProcedureAddr = 0;
	pCS->isCall = false;
}









char CS_GetCharData( ClientScript_t* pCS )
{
	return( pCS->ScriptData [ pCS->ScriptAddr++ ] );
}








int CS_GetIntData( ClientScript_t* pCS )
{
	int n;
	memcpy( &n, &pCS->ScriptData[ pCS->ScriptAddr ], sizeof( int ) );
	pCS->ScriptAddr += sizeof( int );
	return( n );
}







int CS_Interpretor( ClientScript_t* pCS )
{
	char inter;								

	


	
	if( pCS->isCall )
	{
		pCS->isCall = false;
	}
	
	pCS->callProcedureAddr = pCS->ScriptAddr;

	
	while( !pCS->isCall )
	{
		inter = CS_GetCharData( pCS );
		
		if( inter == 'x' ) break;

		switch( inter )
		{
		
		case 'p':
			inter = CS_GetCharData( pCS );
			break;
		
		case 'o':
			CS_InterpretorOperation( pCS );
			break;
		
		case 'i':
			CS_InterpretorIF( pCS );
			break;
		
		case 'c':
			CS_InterpretorCall( pCS );
			break;
		
		case 'f':
			CS_InterpretorFunction( pCS );
		}
	}
	
	pCS->ScriptAddr = pCS->callProcedureAddr;

	return(1);
}









void CS_InterpretorCall( ClientScript_t* pCS )
{	
	int paramAddr;								
	paramAddr = CS_GetCharData( pCS );
	if( pCS->isCall ) return;

	pCS->callProcedureAddr = pCS->ScriptProcedureAddr[paramAddr];
	pCS->isCall = true;
}







void CS_InterpretorOperation( ClientScript_t* pCS )
{
	char inter;									
	int paramAddr1, paramAddr2, operatorParam;	
	int variable;								

	
	inter = CS_GetCharData( pCS );
	
	if( inter == 'v' )
	{
		
		paramAddr1 = CS_GetCharData( pCS );
	}

	
	inter = CS_GetCharData( pCS );
	operatorParam = inter;
	
	
	inter = CS_GetCharData( pCS );
	
	if( inter == 'v' )
	{

		paramAddr2 = CS_GetCharData( pCS );
		
		variable = pCS->ScriptVariable[paramAddr2];
	}
	
	if( inter == 'n' )
	{
		variable = CS_GetIntData( pCS );
	}
	
	if( inter == 'f' )
	{
		
		variable = CS_InterpretorFunction( pCS );
	}
	
	if( inter == 'd' )
	{
		paramAddr2 = CS_GetCharData( pCS );
		variable = CS_InterpretorDefine( pCS, paramAddr2 );
	}

	
	switch( operatorParam )
	{
	case 0:
		pCS->ScriptVariable[paramAddr1] += variable;
		break;
	case 1:
		pCS->ScriptVariable[paramAddr1] -= variable;
		break;
	case 2:
		pCS->ScriptVariable[paramAddr1] *= variable;
		break;
	case 3:
		pCS->ScriptVariable[paramAddr1] /= variable;
		break;
	case 4:
		pCS->ScriptVariable[paramAddr1] = variable;
		break;
	}
}








void CS_InterpretorParameter( ClientScript_t* pCS, int param[], int n )
{
	char inter;									
	int variable, paramAddr;					
	int i;										
	
	for( i = 0; i < n; i ++ )
	{
		inter = CS_GetCharData( pCS );
		
		if( inter == 'n' )
		{
			variable = CS_GetIntData( pCS );
			param[ i ] = variable;
			continue;
		}
		
		if( inter == 'd' )
		{
			paramAddr = CS_GetCharData( pCS );
			variable = CS_InterpretorDefine( pCS, paramAddr );
			param[ i ]= variable;
			continue;
		}
		
		if( inter == 'v' )
		{
			paramAddr = CS_GetCharData( pCS );
			variable = pCS->ScriptVariable[paramAddr];				
			param[ i ] = variable;
			continue;
		}
		
		if( inter == 'f' )
		{
			paramAddr = CS_GetCharData( pCS );
			variable = CS_InterpretorFunction( pCS );
			param[ i ] = variable;
			continue;
		}
	}
}







int CS_InterpretorCase( ClientScript_t* pCS )
{
	char inter;											
	int cases[5], casesNumber = 0;						

	
	int variable1, variable2, paramAddr, operatorAddr;
	
	int i;												

	while( !pCS->isCall )
	{
		
		inter = CS_GetCharData( pCS );
		
		if( inter == 't' ) break;
		
		if( inter == 'a' ) inter = CS_GetCharData( pCS );
		
		if( inter == 'v' )
		{
			paramAddr = CS_GetCharData( pCS );
			variable1 = pCS->ScriptVariable[ paramAddr ];
		}
		
		if( inter == 'n' )
		{
			variable1 = CS_GetIntData( pCS );
		}
		
		if( inter == 'd' )
		{
			paramAddr = CS_GetCharData( pCS );
			variable1 = CS_InterpretorDefine( pCS, paramAddr );
		}
		
		if( inter == 'f' )
		{
			variable1 = CS_InterpretorFunction( pCS );
		}
		
		inter = CS_GetCharData( pCS );
		operatorAddr = inter;

		
		inter = CS_GetCharData( pCS );
		if( inter == 'v' )
		{
			paramAddr = CS_GetCharData( pCS );
			variable2 = pCS->ScriptVariable[ paramAddr ];
		}
		if( inter == 'n' )
		{
			variable2 = CS_GetIntData( pCS );
		}
		if( inter == 'd' )
		{
			paramAddr = CS_GetCharData( pCS );
			variable2 = CS_InterpretorDefine( pCS, paramAddr );
		}
		if( inter == 'f' )
		{
			variable2 = CS_InterpretorFunction( pCS );
		}
		cases[ casesNumber ] = 0;
		switch( operatorAddr )
		{
		
		case 0:
			if( variable1 == variable2 ) cases[ casesNumber ] = 1;
			break;
		
		case 1:
			if( variable1 != variable2 ) cases[ casesNumber ] = 1;
			break;
		
		case 2:
			if( variable1 < variable2 ) cases[ casesNumber ] = 1;
			break;
		
		case 3:
			if( variable1 > variable2 ) cases[ casesNumber ] = 1;
			break;
		
		case 4:
			if( variable1 <= variable2 ) cases[ casesNumber ] = 1;
			break;
		
		case 5:
			if( variable1 >= variable2 ) cases[ casesNumber ] = 1;
			break;
		}
		casesNumber ++;
	}
	
	for( i = 0; i < casesNumber; i ++ ) if( !cases[ i ] ) return( 0 );
	return( 1 );
}







void CS_InterpretorThen( ClientScript_t* pCS, int addrInfo[] )
{

	char inter;										

	
	pCS->ScriptAddr = addrInfo[0];

	while( !pCS->isCall )
	{
		inter = CS_GetCharData( pCS );
		
		if( inter == 'e' || inter == 'q' ) 
		{
			pCS->ScriptAddr = addrInfo[ 2 ];
			break;
		}
		switch( inter )
		{
		
		case 'o':
			CS_InterpretorOperation( pCS );
			break;
		
		case 'i':
			CS_InterpretorIF( pCS );
			break;
		
		case 'c':
			
			CS_InterpretorCall( pCS );
			break;
		
		case 'f':
			CS_InterpretorFunction( pCS );
			break;
		}
	}
}







void CS_InterpretorElse( ClientScript_t* pCS, int addrInfo[] )
{
	char inter;									
	
	
	pCS->ScriptAddr = addrInfo[1];

	while( !pCS->isCall )
	{
		inter = CS_GetCharData( pCS );
		
		if( inter == 'q' ) 
		{
			pCS->ScriptAddr = addrInfo[ 2 ];
			break;
		}
		switch( inter )
		{
		
		case 'o':
			CS_InterpretorOperation( pCS );
			break;
		
		case 'i':
			CS_InterpretorIF( pCS );
			break;
		
		case 'c':
			
			CS_InterpretorCall( pCS );
			break;
		
		case 'f':
			CS_InterpretorFunction( pCS );
			break;
		}
	}
}







void CS_InterpretorIF( ClientScript_t* pCS )
{
	char inter;									
	int addrInfo[3];							
	int cases;									

	

	
	addrInfo[0] = CS_GetIntData( pCS );
	
	addrInfo[1] = CS_GetIntData( pCS );
	
	addrInfo[2] = CS_GetIntData( pCS );

	while( !pCS->isCall )
	{
		inter = CS_GetCharData( pCS );
		if( inter == 'q' ) break;
		if( inter == 'a' ) 
		{
			cases = CS_InterpretorCase( pCS );
			
			if( cases ) CS_InterpretorThen( pCS, addrInfo );
			
			
			else if( addrInfo[1] ) CS_InterpretorElse( pCS, addrInfo );
				else pCS->ScriptAddr = addrInfo[ 2 ];
		}
	}
}



char *TutorialFilenames[]  = 
{
	"Tutorial.ss",
};

char *TipFilenames[]  = 
{
	"Tutorial.ss",
};

#define TutorialFileCount	1
#define TipFileCount		0







CTutorialMng::CTutorialMng()
{
	m_fileMng		= NULL;	
	m_NowTipNum		= -1;
}







void	CTutorialMng::SetCompleteTutorial		(int type)
{
	m_CompleteTutorial |=  1 << type ;
}






BOOL	CTutorialMng::IsCompleteTutorial		(int type)
{	
	int temp = 1 << type;
	return temp & m_CompleteTutorial;
}


int CTutorialMng::FindInCharName(int idx, char *name)
{
	for (int i =0 ; i <m_vTutorialHistory[idx].vBaseInfo.size(); i++)
	{
		if ( !strcmp(m_vTutorialHistory[idx].vBaseInfo[i].Charname, name) )
			return i;
	}
	return -1;
}

int CTutorialMng::FindInLoginID(char *id)
{
	for (int i = 0; i < m_vTutorialHistory.size(); i ++)
	{
		if ( !strcmp (m_vTutorialHistory[i].LoginName, id))
			return i;
	}
	return -1;
}






bool	CTutorialMng::SaveHistory				(char *filename)
{
	
	
	return 0;

	
	FILE *pFile = NULL;
	pFile = fopen(filename,"wb");

	if ( pFile == NULL ) return 0;

	























	int  nameIdx = -1;
	m_LogInIdx = FindInLoginID(g_cgv.clientID);

	
	if ( m_LogInIdx >= 0 )	
	{
		nameIdx = FindInCharName(m_LogInIdx, g_cgv.myCharacterInfo->name);

		
		if (nameIdx >= 0 )
		{
			m_vTutorialHistory[m_LogInIdx].vBaseInfo[nameIdx].TutorialNum		= m_NowTutorialNum;
			m_vTutorialHistory[m_LogInIdx].vBaseInfo[nameIdx].bTutorialMode	= m_bTutorialMode;
		}	
		
		else
		{
			sTutorual_SaveBaseInfo SaveBaseInfo;
			SaveBaseInfo.bTutorialMode		= m_bTutorialMode;
			SaveBaseInfo.CharIdx			= g_cgv.myCharacterInfo->characterID;
			memset(SaveBaseInfo.Charname, 0, 256);
			strcpy( SaveBaseInfo.Charname, g_cgv.myCharacterInfo->name);
			SaveBaseInfo.TutorialNum		= m_NowTutorialNum;

			m_vTutorialHistory[m_LogInIdx].vBaseInfo.push_back(SaveBaseInfo);
		}
	}
	else
	{		
		sTutorial_History	history;
		strcpy(history.LoginName, g_cgv.clientID);
		
		sTutorual_SaveBaseInfo SaveBaseInfo;
		SaveBaseInfo.bTutorialMode		= m_bTutorialMode;
		SaveBaseInfo.CharIdx			= g_cgv.myCharacterInfo->characterID;
		memset(SaveBaseInfo.Charname, 0, 256);
		strcpy( SaveBaseInfo.Charname, g_cgv.myCharacterInfo->name);
		SaveBaseInfo.TutorialNum		= m_NowTutorialNum;
		history.vBaseInfo.push_back(SaveBaseInfo);

		m_vTutorialHistory.push_back(history);		
	}

	int size = m_vTutorialHistory.size();
	fwrite(&size, sizeof(int), 1,  pFile);		

	for (int i = 0; i < m_vTutorialHistory.size(); i ++)
	{		
		
		fwrite(m_vTutorialHistory[i].LoginName, 256, 1,  pFile);	

		
		int size = m_vTutorialHistory[i].vBaseInfo.size();
		fwrite(&size, sizeof(int), 1,  pFile);

		for (int o = 0; o <m_vTutorialHistory[i].vBaseInfo.size(); o++)
		{			
			
			fwrite(&m_vTutorialHistory[i].vBaseInfo[o], sizeof ( sTutorual_SaveBaseInfo), 1,  pFile);			
		}
	}

	
	fclose(pFile);

	return 1;
}






bool	CTutorialMng::LoadHistory				(char *filename)
{
	
	
	return 0;

	FILE *pFile = NULL;
	
	pFile = fopen(filename, "rb");

	if (pFile == NULL ) return 0;


























































	

	int size = m_vTutorialHistory.size();
	fread(&size,  sizeof(int), 1, pFile);		

	for (int i = 0; i < size; i ++)
	{		
		
		sTutorial_History history;
		fread(&history.LoginName, 256, 1, pFile);			
		

		
		int IdCount;
		fread(&IdCount, sizeof(int), 1,  pFile);

		for (int o = 0; o <IdCount; o++)
		{	
			sTutorual_SaveBaseInfo baseinfo;
			fread(&baseinfo, sizeof (sTutorual_SaveBaseInfo), 1,  pFile );
			history.vBaseInfo.push_back(baseinfo);
		}
		m_vTutorialHistory.push_back(history);
	}	


	
	int nameIdx = -1;
	m_LogInIdx = FindInLoginID(g_cgv.clientID);
	if ( m_LogInIdx >= 0 ) 
	{			
		nameIdx = FindInCharName(m_LogInIdx, g_cgv.myCharacterInfo->name);
		if ( nameIdx >= 0 )
		{				
			m_NowTutorialNum = m_vTutorialHistory[m_LogInIdx].vBaseInfo[nameIdx].TutorialNum;
			m_bTutorialMode  = m_vTutorialHistory[m_LogInIdx].vBaseInfo[nameIdx].bTutorialMode;
		}
	}
	fclose(pFile);
	return 1;
}








BOOL CTutorialMng::Initialize()
{	
	
	
	if ( g_cgv.myCharacterInfo->level != 1 || g_cgv.myCharacterInfo->exp != 0)
	{
		m_CompleteTutorial		= 1;
		m_Tutorial_Count		= 0;
		m_Tip_Count				= 0;

		m_bAlwaysRun			= 0;			
		m_bTutorialMode			= 0;			
		QuestNumber				= 0;
		m_NowTutorialNum		= -1;
		m_bTutorialMode = 0;
		return 0;
	}
	
	if (m_bTutorialMode)
	{
		int Idx = FindInLoginID(g_cgv.clientID);
		if ( Idx == m_LogInIdx)		return 1;
		else
			SaveHistory("history.his");			
	}

	m_CompleteTutorial		= 0;
	m_Tutorial_Count		= 0;
	m_Tip_Count				= 0;

	m_bAlwaysRun			= 0;			
	m_bTutorialMode			= 1;			
	QuestNumber				= 0;
	m_NowTutorialNum		= -1;

	memset(ScriptVar, 0, MAX_SCRIPT_VAR);


	
	for (int i = 0; i < TutorialFileCount; i ++)
	{		
		memset (&m_Tutorial_Script[i], 0, sizeof (sTutorial_Script));
		LoadScriptFile(TutorialScript, i, TutorialFilenames[i]);
	}

	
	for (i = 0; i < TipFileCount; i ++)
	{		
		memset (&m_Tip_Script[i], 0, sizeof (sTutorial_Script));
		LoadScriptFile(TipScript, i, TipFilenames[i]);
	}
	
	LoadHistory("history.his");			

	return 1;
}






void CTutorialMng::SettingTutorial(int idx)
{
	if ( m_bAlwaysRun )
	{
		InitScript(TipScript, idx);
		m_NowTipNum = idx;
	}
	else
	{
		if ( !IsCompleteTutorial(idx) )
		{
			InitScript(TipScript, idx);
			m_NowTipNum = idx;
		}
	}

}






BOOL CTutorialMng::InitScript(int type, int idx)
{
	sTutorial_Script* Script;

	if ( type == TutorialScript )	
		Script = m_Tutorial_Script;
	else 
		Script = m_Tip_Script;

	Script[idx].Script.msgFlag				= 0;
	Script[idx].Script.isCall				= false;

	Script[idx].Script.callProcedureAddr	= 0;
	Script[idx].Script.msgWaiting			= 0;
	Script[idx].Script.pcReplyIdx			= -1;

	Script[idx].Script.ScriptEvent			= CS_RUN;	
	Script[idx].Script.saveTime				= 0;
	Script[idx].Script.thinkTime			= 0;	

	Script[idx].Script.msgWaiting			= false;
	Script[idx].Script.msgFlag				= false;

	Script[idx].Script.sendMsgIdx			= 0;
	Script[idx].Script.replyMsgIdxNumber	= 0;
	Script[idx].Script.replyMsgIdx[0]		= 0;
	Script[idx].Script.replyMsgIdx[1]		= 0;
	Script[idx].Script.replyMsgIdx[2]		= 0;
	Script[idx].Script.replyMsgIdx[3]		= 0;

	m_Tip_Script[idx].Script.replyMsgIdx[4]	= 0;

	return 1;
}






BOOL CTutorialMng::Process()
{		
	
	if ( m_bTutorialMode )
	{
		if ( m_Tutorial_Count > -1 && m_NowTutorialNum >= -1)
			CS_Interpretor( &m_Tutorial_Script[0].Script );

		CS_SendMessage_TutorialMessage ( TutorialScript, &m_Tutorial_Script[0].Script );
		return 1;
	}	

	


	return 0;

}







void CTutorialMng::SetReplyMessage(int replyidx)
{
	m_Tutorial_Script[0].Script.pcReplyIdx = replyidx;
}







BOOL	CTutorialMng::LoadScriptFile			(int Type, int idx, char* Filename)
{	
	if ( !m_fileMng  || !Filename) return 0;

	BYTE *pFile					= NULL;
	pFile						= m_fileMng->ReadFileFromPack(Filename);

	sTutorial_Script*	Script = NULL;
	int*				Count	= 0;

	if ( Type == TutorialScript )
	{		
		Script = m_Tutorial_Script;
		Count  = &m_Tutorial_Count;
	}	
	else 
	{
		Script = m_Tip_Script;
		Count  = &m_Tip_Count;
	}

	if ( (*Count) < MAX_CLIENT_SCRIPT_COUNT )
	{	
		Script[idx].TutorialType = 1 << idx;
		CS_LoadScript(&Script[idx].Script, pFile);
		(*Count)++ ;
	}
	else
		return 0;

	return 1;

}






int CS_SendMessage_TutorialMessage( int type, ClientScript_t* pCS )
{	
	if ( !pCS->msgFlag ) return 0;		


	
	if ( type == TipScript )
	{

		return 1;
	}

	
	else
	{
		if( pCS->sendMsgIdx < 0 ) return 0;		
		
		g_ifMng->m_tutorialWin->MakeMessage( pCS->sendMsgIdx );
		for( int i = 0; i < pCS->replyMsgIdxNumber; i ++ )
			g_ifMng->m_tutorialWin->MakeSelectMessage( i, pCS->replyMsgIdx[i] );

		pCS->msgFlag = 0;
		
	}
	return 1;
}






void CTutorialMng::SetTotutirlMode (int active) 
{
	m_bTutorialMode = active;	
};
