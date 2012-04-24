	  




#include "..\global.h"
#include <windowsx.h>
#include <assert.h>

//lucky 2011 read the odbc info from server.cfg
#include "..\Ini.h"
//end


HANDLE			hQueryExecuterThread;
DWORD			queryExecuterThreadID;
int				runQueryExecuterThread=0;

HANDLE			hQueryRequestMutex= INVALID_HANDLE_VALUE; 
HANDLE          hQueryResultMutex = INVALID_HANDLE_VALUE;

CQueryQueue		*queryRequestQueue, *queryResultQueue;


#include "..\Tools\CTools.h"
extern CTools* gcpTools;

#include "..\Guild/CGuildCtrl.h"
extern CGuildCtrl* gcpGuildCtrl;
#include "..\GonryunPracticeBattle\CGonryunPracticeBattle.h"
#include "..\GonryunPracticeBattle\CGonryunPractice_WaiterCtrl.h"	
extern	CGonryunPracticeBattle	g_GonryunPracticeBattle;									

#include "..\GolryunBattle\CGolryunBattle.h"
extern CGolryunBattle* gcpGolryunBattle;

#include "..\CPremiumPlayerCtrl.h"
extern CPremiumPlayerCtrl* gcpPremiumPlayerCtrl;


#include "..\Terra\Billing\CTerraBillingCtrl.h"
extern CTerraBillingCtrl* gcpTerraBillingCtrl;

#include "..\GolryunBattle\CGolryunBattle_Observer.h"
extern CGolryunBattle_Observer* gcpGolryunBattle_Observer;

#include "..\GM(Game Master)\CGMCtrl.h"
extern CGMCtrl* gcpGMCtrl;



#include "../HelperSystem/HelperManager.h"
extern CHelperManager g_HelperManager;



void DB_ResultQuery_Load_PremiumInfo(querySet_t& request, querySet_t& result);
void DB_ProcessQueryResultQueue_QUERY_LOAD_PREMIUMINFO(const querySet_t& result);



CDBServer::CDBServer()
{
}

CDBServer::~CDBServer()
{
}
 
int CDBServer::Initialize(char *DSN, int DSNType)
{
	unsigned char outConnectionStr[OUTCONNECTIONSIZE];
	char connection[256];
	
    m_rCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv); 
    if (m_rCode != SQL_SUCCESS)
		return false;

	SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);

    m_rCode = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc); 
    if (m_rCode != SQL_SUCCESS )
		return false;

	
	//lucky 2011 Read the ODBC information from server.cfg
	CIni iniConfig;
	char sPath[260] = {0};
	GetCurrentDirectory( 260, sPath );
	strcat( sPath, "\\" );
	strcat( sPath, "server.cfg" );
	iniConfig.SetPathName( sPath );
	char sName[32] = {0};
	char sPsw[32] = {0};
	if ( DSNType == 1 )	// FILE DSN
	{
		iniConfig.GetArray( "DB_PARAM", "Name1", sName, 32 );
		iniConfig.GetArray( "DB_PARAM", "Password1", sPsw, 32 );
		sprintf(connection, "FILEDSN=%s;UID=%s;PWD=%s", DSN, sName, sPsw);
	}
	else				// SYSTEM DSN
	{
		iniConfig.GetArray( "DB_PARAM", "Name0", sName, 32 );
		iniConfig.GetArray( "DB_PARAM", "Password0", sPsw, 32 );
		sprintf(connection, "DSN=%s;UID=%s;PWD=%s", DSN, sName, sPsw);
	}
//end


    m_rCode = SQLDriverConnect(m_hDbc, g_hWnd, (unsigned char *)connection,
				SQL_NTS, outConnectionStr, OUTCONNECTIONSIZE, NULL, SQL_DRIVER_NOPROMPT);
    if (m_rCode != SQL_SUCCESS && m_rCode != SQL_SUCCESS_WITH_INFO)
		return false;

	m_rCode = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt); 
	if (m_rCode != SQL_SUCCESS)
		return false;

	return true;
}


void CDBServer::PrintError ( SQLSMALLINT handleType )
{
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	SQLSMALLINT   i, MsgLen;
	SQLRETURN     rc ;

	int			ErrorLogCount = 0;
	i = 1;
	while ( (rc = SQLGetDiagRec ( handleType, m_hStmt, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen ) ) != SQL_NO_DATA) 
	{
		if ( ErrorLogCount >= 10)
			break;
		
		g_logSystem->Write ( "%s, %d, %s", SqlState, NativeError, Msg ) ;
		

		ErrorLogCount++;
		i++;
	}
}


void CDBServer::PrintError ( HSTMT hstmt )
{
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	SQLSMALLINT   i, MsgLen;
	SQLRETURN     rc ;

	int			ErrorLogCount = 0;
	i = 1;
	while ( (rc = SQLGetDiagRec ( SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen ) ) != SQL_NO_DATA) 
	{
		if ( ErrorLogCount >= 10)
			break;

		g_logSystem->Write ( "%s, %d, %s", SqlState, NativeError, Msg ) ;
		


		i++;
		ErrorLogCount++;
	}
}



void CDBServer::Close()
{
	if ( m_hStmt )
	{
		SQLCloseCursor(m_hStmt);
	}

	if ( m_hStmt )
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
		m_hStmt = SQL_NULL_HSTMT ;
	}

	if ( m_hDbc )
	{
		SQLDisconnect(m_hDbc);
	}

	if ( m_hDbc )
	{
		SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
		m_hDbc = SQL_NULL_HDBC ;
	}

	if ( m_hEnv )
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		m_hEnv = SQL_NULL_HENV ;
	}
}

char *CDBServer::ReturnString ()
{
	switch ( m_rCode )
	{
	case SQL_SUCCESS: return "SQL_SUCCESS";
	case SQL_SUCCESS_WITH_INFO: return "SQL_SUCCESS_WITH_INFO";
	case SQL_NEED_DATA: return "SQL_NEED_DATA";
	case SQL_STILL_EXECUTING: return "SQL_STILL_EXECUTING";
	case SQL_ERROR: return "SQL_ERROR";
	case SQL_NO_DATA: return "SQL_NO_DATA";
	case SQL_INVALID_HANDLE: return "SQL_INVALID_HANDLE";
	default: return "NO ERROR";
	}
}






CDBAccount::CDBAccount()
{
}

CDBAccount::~CDBAccount()
{
}


void CDBAccount::InsertLogin(playerCharacter_t* pPlayer) 
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC InsertLogin '%s','%s'",pPlayer->userID,g_hostName);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pPlayer->idx;
	strncpy(request.ownerID, pPlayer->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_INSERT_LOGIN;
	strncpy(request.query,m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	

	
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	

}


void CDBGame::LoadPremiumInfo(playerCharacter_t* pPlayer)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(m_query,QUERYSIZE, "EXEC LoadPremiumInfo '%s'", pPlayer->userID);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pPlayer->idx;
	strncpy(request.ownerID, pPlayer->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_PREMIUMINFO;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;


	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;


	
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	
}


void CDBGame::SavePremiumInfo(playerCharacter_t* pPlayer)
{
	if(FALSE == pPlayer->PremiumInfo.bLoad) return;

	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC SavePremiumInfo '%s',%d,%d,%d", 
		pPlayer->userID,
		pPlayer->PremiumInfo.iRemainSecond,
		pPlayer->PremiumInfo.WorldChatting.lDate,
		pPlayer->PremiumInfo.WorldChatting.iDecreaseCount);
	m_query[QUERYSIZE]=NULL;


	request.ownerIdx = pPlayer->idx;
	strncpy(request.ownerID, pPlayer->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_PREMIUMINFO;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;


	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pPlayer;


	if ( !runQueryExecuterThread ) return;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return;
}

int CDBAccount::DeleteLogin(playerCharacter_t *pc) 
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC DeleteLogin '%s'", pc->userID);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_DELETE_LOGIN;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pc;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;

}




int CDBAccount::DeleteLoginHost( char *hostName ) 
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC DeleteLoginHost '%s'", hostName);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = -1;
	strncpy(request.ownerID, "",IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_DELETE_LOGIN_HOST;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *) NULL;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}








CDBGame::CDBGame()
{
}

CDBGame::~CDBGame()
{
}


void CDBGame::LoadGame(playerCharacter_t* pPlayer)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	
	_snprintf(m_query,QUERYSIZE, "EXEC LoadGame '%s',%d,%d", 
		pPlayer->userID, 
		pPlayer->serverGroupID, 
		pPlayer->characterID);


	m_query[QUERYSIZE]=NULL;


	request.ownerIdx = pPlayer->idx;
	strncpy(request.ownerID, pPlayer->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_CHARACTER;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pPlayer;

	
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	
}






int CDBGame::LoadItem( playerCharacter_t *pc )
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC LoadItem '%s', %d, %d", pc->userID, pc->serverGroupID, pc->characterID);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_ITEM;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;


	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pc;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}



int CDBGame::LoadSkill( playerCharacter_t *pc )
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC LoadSkill '%s', %d, %d", pc->userID, pc->serverGroupID, pc->characterID);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_SKILL;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pc;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;

}









int CDBGame::LoadFriends ( playerCharacter_t * pc )
{
	querySet_t request ;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf ( m_query,QUERYSIZE, "EXEC LoadFriends '%s','%d','%d'",
			  pc->userID, 
			  pc->serverGroupID, 
			  pc->characterID);

	request.ownerIdx = pc->idx ;
	strncpy ( request.ownerID, pc->userID ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_FRIENDS ;
	strncpy(request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;


	request.hStmt = m_hStmt ;
	request.retCode = -1 ;
	request.retStructVariable = ( void * ) pc ;

	if ( !runQueryExecuterThread ) return 0 ;

	WaitForSingleObject ( hQueryRequestMutex, INFINITE ) ;
	queryRequestQueue->Push ( &request ) ;
	ReleaseMutex ( hQueryRequestMutex ) ;
	
	return 1 ;
}



void CDBGame::LoadHelpList(const playerCharacter_t* pPlayer)
{
	querySet_t request ;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC LoadHelpList '%s','%d','%d'",
			  pPlayer->userID, 
			  pPlayer->serverGroupID, 
			  pPlayer->characterID );

	request.ownerIdx = pPlayer->idx ;
	strncpy( request.ownerID, pPlayer->userID ,IDSTRING) ;
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_HELPLIST;
	strncpy ( request.query, m_query ,QUERYSIZE) ;
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt ;
	request.retCode = -1 ;
	request.retStructVariable = NULL;

	if ( !runQueryExecuterThread ) return;

	WaitForSingleObject ( hQueryRequestMutex, INFINITE ) ;
	queryRequestQueue->Push ( &request ) ;
	ReleaseMutex ( hQueryRequestMutex ) ;
	return;
}


void CDBGame::SaveHelpList(const playerCharacter_t* pPlayer)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));
		
		
	_snprintf ( m_query, QUERYSIZE,
						"EXEC SaveHelpList"
						"'%s',"			
						"%d,"			
						"%d,"			
						"%d,"			
						"%d,"			
						"%d,"			
						"%d,"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s'"			
						,
						pPlayer->userID,							
						pPlayer->serverGroupID,					
						pPlayer->characterID,					
						pPlayer->tHelper.List.mode,				
						pPlayer->tHelper.tPoint.iMyPoint,		
						pPlayer->tHelper.tPoint.iHelpPoint,		
						pPlayer->tHelper.tPoint.iTotalHelpPoint,	
						pPlayer->tHelper.List.Member[0].name,	
						pPlayer->tHelper.List.Member[0].id,		
						pPlayer->tHelper.List.Member[1].name,	
						pPlayer->tHelper.List.Member[1].id,		
						pPlayer->tHelper.List.Member[2].name,	
						pPlayer->tHelper.List.Member[2].id,		
						pPlayer->tHelper.List.Member[3].name,	
						pPlayer->tHelper.List.Member[3].id,		
						pPlayer->tHelper.List.Member[4].name,	
						pPlayer->tHelper.List.Member[4].id,		
						pPlayer->tHelper.List.Member[5].name,	
						pPlayer->tHelper.List.Member[5].id,		
						pPlayer->tHelper.List.Member[6].name,	
						pPlayer->tHelper.List.Member[6].id,		
						pPlayer->tHelper.List.Member[7].name,	
						pPlayer->tHelper.List.Member[7].id,		
						pPlayer->tHelper.List.Member[8].name,	
						pPlayer->tHelper.List.Member[8].id,		
						pPlayer->tHelper.List.Member[9].name,	
						pPlayer->tHelper.List.Member[9].id		
						);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pPlayer->idx;
	strncpy ( request.ownerID, pPlayer->userID ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_HELPLIST;
	strncpy ( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;


	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;

	if ( !runQueryExecuterThread ) return;
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
}






void CDBGame::SaveWepPoint(const playerCharacter_t* pPlayer,const int WepPoint)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf ( m_query, QUERYSIZE,
						"EXEC SaveHelpList"
						"'%s',"			
						"%d"			
						,
						pPlayer->userID,		
						WepPoint		
						);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pPlayer->idx;
	strncpy ( request.ownerID, pPlayer->userID,IDSTRING );
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_WEBPOINT;
	strncpy ( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;

	if ( !runQueryExecuterThread ) return;
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );	
}









BOOL CDBGame::LoadGuild(const int serverGroupIdx)
{
	querySet_t request ;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC LoadDan '%d'",serverGroupIdx);
	m_query[QUERYSIZE]=NULL;

	
	request.ownerIdx = g_config.serverGroupNo;
	strncpy(request.ownerID, g_config.serverGroupName ,IDSTRING) ;
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_GUILD ;
	strncpy(request.query, m_query,QUERYSIZE ) ;
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt ;
	request.retCode = -1 ;
	request.retStructVariable = NULL;

	if ( !runQueryExecuterThread ) return FALSE;

	WaitForSingleObject ( hQueryRequestMutex, INFINITE ) ;
	queryRequestQueue->Push ( &request ) ;
	ReleaseMutex ( hQueryRequestMutex ) ;
	
	return TRUE;
}


void CDBGame::LoadQuest(playerCharacter_t* pPlayer)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(m_query,QUERYSIZE, "EXEC LoadQuest '%s', %d, %d", pPlayer->userID, pPlayer->serverGroupID, pPlayer->characterID);
	m_query[QUERYSIZE]=NULL;
	
	request.ownerIdx = pPlayer->idx;
	strncpy(request.ownerID, pPlayer->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_QUEST;
	request.hStmt = m_hStmt;
	strncpy(request.query,m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.retCode = -1;
	request.retStructVariable = NULL;

	
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	
}




void CDBGame::LoadQuestStatus(playerCharacter_t* pPlayer)	
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC LoadQuestStatus '%s', %d, %d", pPlayer->userID, pPlayer->serverGroupID, pPlayer->characterID);
	m_query[QUERYSIZE]=NULL;
	
	request.ownerIdx = pPlayer->idx;
	strncpy(request.ownerID, pPlayer->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_QUESTSTATUS;
	request.hStmt = m_hStmt;
	strncpy(request.query,m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.retCode = -1;
	request.retStructVariable = NULL;

	
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	
}





BOOL CDBGame::RemoveGuild(const int guildIdx,const int serverGroupIdx)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(m_query,QUERYSIZE, "EXEC DeleteDan %d, %d", guildIdx, serverGroupIdx );
	m_query[QUERYSIZE]=NULL;

	
	request.ownerIdx = guildIdx;
	strncpy ( request.ownerID, g_guild[guildIdx].name,IDSTRING) ;
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_REMOVE_GUILD ;
	strncpy ( request.query, m_query ,QUERYSIZE) ;
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt ;
	request.retCode = -1 ;
	request.retStructVariable = NULL;

	if( !runQueryExecuterThread ) return FALSE;

	WaitForSingleObject ( hQueryRequestMutex, INFINITE ) ;
	queryRequestQueue->Push ( &request ) ;
	ReleaseMutex ( hQueryRequestMutex ) ;

	
	return TRUE;
}




void CDBGame::SaveGame(
	playerCharacter_t *pc,
	enum enumWhySaveGame why,
	enum enumQUERY_SAVE_GAMEparameter parameter)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	int worldIdx=-1;
	vec3_t position;

	
	
	if ( !pc->alive || (int)pc->curRA <= 0 )
	{
		int bindingIdx;
		int rand_i;
		vec3_t pos, rpos;

		pc->curRA = 0.0f;
		pc->curSA = 0.0f;

		bindingIdx = pc->bindingIdx;
		if( bindingIdx < 0 ) bindingIdx = 0;
		vec_copy( g_binding[bindingIdx].pos, pos );
		rand_i = GTH_Randomize( (int)g_binding[bindingIdx].range );

		rpos[0] = (float)( rand_i ) * 2.0f - g_binding[bindingIdx].range;
		rpos[1] = (float)( rand_i ) * 2.0f - g_binding[bindingIdx].range;

		worldIdx = g_binding[bindingIdx].worldIdx;

		pos[0] += rpos[0];
		pos[1] += rpos[1];

		
		vec_copy( pos, position );	
	}
	else		
	{
		
		
		if(pc->worldIdx == tagGolryunBattle::Golryun_Battle_Map_Index)	
		{
			worldIdx = 0;
			vec_copy( g_binding[0].pos, position );
		}
		else
		{
			worldIdx = pc->worldIdx;
			vec_copy(pc->position, position);
		}	
		
	}

	
	
	if( pc->angles[1] < 0.0f || pc->angles[1] > 360.0f ) pc->angles[1] = 0.0f;



	
	
	
    
	_snprintf(m_query,QUERYSIZE,"EXEC SaveGame '%s', %d, %d,"
						"'%d', '%s', %d, %d, %d, %7.2f, %7.2f, %7.2f, %4.1f, %d, %I64d, %d,"
						"%d, %d, %I64d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d,"
						"%7.3f, %7.3f, %d, %d, %7.3f, %7.3f, %d,"
						"%d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d,"
						"%d, %d, %d, %d,"
						"%d, %d,"
						"%d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d,"
						"'%s', %d, %d,"
						"%d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d,"
						"%d,"
						
						"%d, %d, %d,%d,%d,"
						"%d, %d,"
						"%d,"
						"%d, %d",	
						

        pc->userID, pc->serverGroupID, pc->characterID, 
		pc->pcJob,		pc->rank, 		pc->rankPoint, 		pc->age, 		worldIdx, 		position[0], 		position[1], 		position[2], 		pc->angles[1], 		pc->level, 		pc->exp, 		pc->increaseExpRate,
		pc->selectedGen, 		pc->genLevel, 		pc->genExp, 		pc->increaseGenExpRate,
		pc->genCapability[0], 		pc->genCapability[1], 		pc->genCapability[2], 		pc->genCapability[3], 		pc->genCapability[4], 		pc->genCapability[5], 		pc->genCapability[6], 		pc->genCapability[7],

		pc->force, 		pc->soulpower, 		pc->agility, 		pc->vitality, 		pc->bonusStatusPoint,
		
		
		pc->curRA, pc->curSA, pc->curChargeNSE, pc->curChargeSE, 		pc->curChargeRA,		pc->curChargeSA, pc->depotSE,



		pc->curWeapon, pc->bindingIdx, pc->itemNumber,
		pc->inventory[0], pc->inventory[1], pc->inventory[2], pc->inventory[3], pc->inventory[4], pc->inventory[5],
		pc->inventory[6], pc->inventory[7], pc->inventory[8], pc->inventory[9], pc->inventory[10], pc->inventory[11],
		pc->inventory[12], pc->inventory[13], pc->inventory[14], pc->inventory[15], pc->inventory[16], pc->inventory[17],
		pc->inventory[18], pc->inventory[19], pc->inventory[20], pc->inventory[21], pc->inventory[22], pc->inventory[23],
		pc->inventory[24], pc->inventory[25], pc->inventory[26], pc->inventory[27], pc->inventory[28], pc->inventory[29],
		pc->inventory[30], pc->inventory[31], pc->inventory[32], pc->inventory[33], pc->inventory[34], pc->inventory[35],
		pc->inventory[36], pc->inventory[37], pc->inventory[38], pc->inventory[39], pc->inventory[40], pc->inventory[41],
		pc->inventory[42], pc->inventory[43], pc->inventory[44], pc->inventory[45], pc->inventory[46], pc->inventory[47],

		pc->depot[0], pc->depot[1], pc->depot[2], pc->depot[3], pc->depot[4], pc->depot[5], pc->depot[6], pc->depot[7], pc->depot[8], pc->depot[9],
		pc->depot[10], pc->depot[11], pc->depot[12], pc->depot[13], pc->depot[14], pc->depot[15], pc->depot[16], pc->depot[17], pc->depot[18], pc->depot[19],
		pc->depot[20], pc->depot[21], pc->depot[22], pc->depot[23], pc->depot[24], pc->depot[25], pc->depot[26], pc->depot[27], pc->depot[28], pc->depot[29],
		pc->depot[30], pc->depot[31], pc->depot[32], pc->depot[33], pc->depot[34], pc->depot[35], pc->depot[36], pc->depot[37], pc->depot[38], pc->depot[39],
		pc->depot[40], pc->depot[41],

		pc->equipment[0], pc->equipment[1], pc->equipment[2], pc->equipment[3], pc->equipment[4], pc->equipment[5], pc->equipment[6],
		pc->equipment[7], pc->equipment[8], pc->equipment[9], pc->equipment[10], pc->equipment[11], pc->equipment[12], pc->equipment[13],
		pc->mouseInventory, pc->bonusSkillPoint, 
		pc->skillVariable[0], pc->skillVariable[1], pc->skillVariable[2], pc->skillVariable[3], pc->skillVariable[4],
		pc->skillVariable[5], pc->skillVariable[6], pc->skillVariable[7], pc->skillVariable[8], pc->skillVariable[9],
		pc->skillVariable[10], pc->skillVariable[11], pc->skillVariable[12], pc->skillVariable[13], pc->skillVariable[14],
		pc->skillVariable[15], pc->skillVariable[16], pc->skillVariable[17], pc->skillVariable[18], pc->skillVariable[19],

		F_HOTKEY(pc->hotkeyType[0], pc->hotkeyIdx[0]),   
		F_HOTKEY(pc->hotkeyType[1], pc->hotkeyIdx[1]), 
		F_HOTKEY(pc->hotkeyType[2], pc->hotkeyIdx[2]),   
		F_HOTKEY(pc->hotkeyType[3], pc->hotkeyIdx[3]), 
		F_HOTKEY(pc->hotkeyType[4], pc->hotkeyIdx[4]),   
		F_HOTKEY(pc->hotkeyType[5], pc->hotkeyIdx[5]), 
		F_HOTKEY(pc->hotkeyType[6], pc->hotkeyIdx[6]),   
		F_HOTKEY(pc->hotkeyType[7], pc->hotkeyIdx[7]), 
		F_HOTKEY(pc->hotkeyType[8], pc->hotkeyIdx[8]),   
		F_HOTKEY(pc->hotkeyType[9], pc->hotkeyIdx[9]), 
		F_HOTKEY(pc->hotkeyType[10], pc->hotkeyIdx[10]), 
		F_HOTKEY(pc->hotkeyType[11], pc->hotkeyIdx[11]), 
		F_HOTKEY(pc->hotkeyType[12], pc->hotkeyIdx[12]), 
		F_HOTKEY(pc->hotkeyType[13], pc->hotkeyIdx[13]), 
		F_HOTKEY(pc->hotkeyType[14], pc->hotkeyIdx[14]),
		F_HOTKEY(pc->hotkeyType[15], pc->hotkeyIdx[15]), 
		F_HOTKEY(pc->hotkeyType[16], pc->hotkeyIdx[16]), 
		F_HOTKEY(pc->hotkeyType[17], pc->hotkeyIdx[17]), 
		F_HOTKEY(pc->hotkeyType[18], pc->hotkeyIdx[18]), 
		F_HOTKEY(pc->hotkeyType[19], pc->hotkeyIdx[19]), 
		F_HOTKEY(pc->hotkeyType[20], pc->hotkeyIdx[20]), 
		F_HOTKEY(pc->hotkeyType[21], pc->hotkeyIdx[21]), 
		F_HOTKEY(pc->hotkeyType[22], pc->hotkeyIdx[22]), 
		F_HOTKEY(pc->hotkeyType[23], pc->hotkeyIdx[23]), 
		F_HOTKEY(pc->hotkeyType[24], pc->hotkeyIdx[24]), 
		F_HOTKEY(pc->hotkeyType[25], pc->hotkeyIdx[25]), 
		F_HOTKEY(pc->hotkeyType[26], pc->hotkeyIdx[26]), 
		F_HOTKEY(pc->hotkeyType[27], pc->hotkeyIdx[27]), 
		F_HOTKEY(pc->hotkeyType[28], pc->hotkeyIdx[28]), 
		F_HOTKEY(pc->hotkeyType[29], pc->hotkeyIdx[29]), 
		F_HOTKEY(pc->hotkeyType[30], pc->hotkeyIdx[30]), 
		F_HOTKEY(pc->hotkeyType[31], pc->hotkeyIdx[31]), 
		
		pc->organizerName, pc->organizeServer, pc->organizeTime, 
		pc->questPoint, pc->penaltyTime, pc->levelUpTime, pc->genLevelUpTime, pc->flagChangeServer,
		pc->precocityInventory[0], pc->precocityInventory[1], pc->precocityInventory[2], pc->precocityInventory[3], pc->precocityInventory[4], pc->precocityInventory[5], 
		pc->precocityTime[0], pc->precocityTime[1], pc->precocityTime[2], pc->precocityTime[3], pc->precocityTime[4], pc->precocityTime[5],
		
		pc->chaosPoint, 
		
		


		



		
		pc->GonryunBattlePractice.Record.iwin,
		pc->GonryunBattlePractice.Record.idefeat,
		pc->GonryunBattlePractice.Record.idraw,		
		pc->PremiumInfo.GonyounPracticeBattle.lDateV2,		
		pc->PremiumInfo.GonyounPracticeBattle.iDecreseCount,
		pc->PremiumInfo.Status.iDefaultInitCount,			
		pc->m_bHide,			
		pc->GM.Chat.RemainBanTimePerSecond, 
		pc->GuildDeconstructionTime,
		pc->GuildsecedeTime
		);

	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_GAME;
	request.parameter = parameter;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;
	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pc;
	request.SaveGameVar.why = why;


	if ( !runQueryExecuterThread ) return;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );


	

	return;
}













int CDBGame::SaveSkill(playerCharacter_t *pc)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC SaveSkill '%s', %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"
						"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		pc->userID, pc->serverGroupID, pc->characterID,
		pc->skill[0].tableIdx, pc->skill[1].tableIdx, pc->skill[2].tableIdx, pc->skill[3].tableIdx, pc->skill[4].tableIdx, 
		pc->skill[5].tableIdx, pc->skill[6].tableIdx, pc->skill[7].tableIdx, pc->skill[8].tableIdx, pc->skill[9].tableIdx, 
		pc->skill[10].tableIdx, pc->skill[11].tableIdx, pc->skill[12].tableIdx, pc->skill[13].tableIdx, pc->skill[14].tableIdx, 
		pc->skill[15].tableIdx, pc->skill[16].tableIdx, pc->skill[17].tableIdx, pc->skill[18].tableIdx, pc->skill[19].tableIdx, 
		pc->skill[20].tableIdx, pc->skill[21].tableIdx, pc->skill[22].tableIdx, pc->skill[23].tableIdx, pc->skill[24].tableIdx, 
		pc->skill[25].tableIdx, pc->skill[26].tableIdx, pc->skill[27].tableIdx, pc->skill[28].tableIdx, pc->skill[29].tableIdx, 
		pc->skill[30].tableIdx, pc->skill[31].tableIdx, pc->skill[32].tableIdx, pc->skill[33].tableIdx, pc->skill[34].tableIdx, 
		pc->skill[35].tableIdx, pc->skill[36].tableIdx, pc->skill[37].tableIdx, pc->skill[38].tableIdx, pc->skill[39].tableIdx, 
		pc->skill[40].tableIdx, pc->skill[41].tableIdx, pc->skill[42].tableIdx, pc->skill[43].tableIdx, pc->skill[44].tableIdx, 
		pc->skill[45].tableIdx, pc->skill[46].tableIdx, pc->skill[47].tableIdx, pc->skill[48].tableIdx, pc->skill[49].tableIdx, 
		pc->skill[50].tableIdx, pc->skill[51].tableIdx, pc->skill[52].tableIdx, pc->skill[53].tableIdx, pc->skill[54].tableIdx, 
		pc->skill[55].tableIdx, pc->skill[56].tableIdx, pc->skill[57].tableIdx, pc->skill[58].tableIdx, pc->skill[59].tableIdx, 
		pc->skill[60].tableIdx, pc->skill[61].tableIdx, pc->skill[62].tableIdx, pc->skill[63].tableIdx, pc->skill[64].tableIdx, 
		pc->skill[65].tableIdx, pc->skill[66].tableIdx, pc->skill[67].tableIdx, pc->skill[68].tableIdx, pc->skill[69].tableIdx, 
		pc->skill[70].tableIdx, pc->skill[71].tableIdx, pc->skill[72].tableIdx, pc->skill[73].tableIdx, pc->skill[74].tableIdx, 
		pc->skill[75].tableIdx, pc->skill[76].tableIdx, pc->skill[77].tableIdx, pc->skill[78].tableIdx, pc->skill[79].tableIdx, 
		pc->skill[80].tableIdx, pc->skill[81].tableIdx, pc->skill[82].tableIdx, pc->skill[83].tableIdx, pc->skill[84].tableIdx, 
		pc->skill[85].tableIdx, pc->skill[86].tableIdx, pc->skill[87].tableIdx, pc->skill[88].tableIdx, pc->skill[89].tableIdx, 
		pc->skill[90].tableIdx, pc->skill[91].tableIdx, pc->skill[92].tableIdx, pc->skill[93].tableIdx, pc->skill[94].tableIdx, 
		pc->skill[95].tableIdx, pc->skill[96].tableIdx, pc->skill[97].tableIdx, pc->skill[98].tableIdx, pc->skill[99].tableIdx, 
		pc->skill[0].iLevel, pc->skill[1].iLevel, pc->skill[2].iLevel, pc->skill[3].iLevel, pc->skill[4].iLevel,
		pc->skill[5].iLevel, pc->skill[6].iLevel, pc->skill[7].iLevel, pc->skill[8].iLevel, pc->skill[9].iLevel,
		pc->skill[10].iLevel, pc->skill[11].iLevel, pc->skill[12].iLevel, pc->skill[13].iLevel, pc->skill[14].iLevel,
		pc->skill[15].iLevel, pc->skill[16].iLevel, pc->skill[17].iLevel, pc->skill[18].iLevel, pc->skill[19].iLevel,
		pc->skill[20].iLevel, pc->skill[21].iLevel, pc->skill[22].iLevel, pc->skill[23].iLevel, pc->skill[24].iLevel,
		pc->skill[25].iLevel, pc->skill[26].iLevel, pc->skill[27].iLevel, pc->skill[28].iLevel, pc->skill[29].iLevel,
		pc->skill[30].iLevel, pc->skill[31].iLevel, pc->skill[32].iLevel, pc->skill[33].iLevel, pc->skill[34].iLevel,
		pc->skill[35].iLevel, pc->skill[36].iLevel, pc->skill[37].iLevel, pc->skill[38].iLevel, pc->skill[39].iLevel,
		pc->skill[40].iLevel, pc->skill[41].iLevel, pc->skill[42].iLevel, pc->skill[43].iLevel, pc->skill[44].iLevel,
		pc->skill[45].iLevel, pc->skill[46].iLevel, pc->skill[47].iLevel, pc->skill[48].iLevel, pc->skill[49].iLevel,
		pc->skill[50].iLevel, pc->skill[51].iLevel, pc->skill[52].iLevel, pc->skill[53].iLevel, pc->skill[54].iLevel,
		pc->skill[55].iLevel, pc->skill[56].iLevel, pc->skill[57].iLevel, pc->skill[58].iLevel, pc->skill[59].iLevel,
		pc->skill[60].iLevel, pc->skill[61].iLevel, pc->skill[62].iLevel, pc->skill[63].iLevel, pc->skill[64].iLevel,
		pc->skill[65].iLevel, pc->skill[66].iLevel, pc->skill[67].iLevel, pc->skill[68].iLevel, pc->skill[69].iLevel,
		pc->skill[70].iLevel, pc->skill[71].iLevel, pc->skill[72].iLevel, pc->skill[73].iLevel, pc->skill[74].iLevel,
		pc->skill[75].iLevel, pc->skill[76].iLevel, pc->skill[77].iLevel, pc->skill[78].iLevel, pc->skill[79].iLevel,
		pc->skill[80].iLevel, pc->skill[81].iLevel, pc->skill[82].iLevel, pc->skill[83].iLevel, pc->skill[84].iLevel,
		pc->skill[85].iLevel, pc->skill[86].iLevel, pc->skill[87].iLevel, pc->skill[88].iLevel, pc->skill[89].iLevel,
		pc->skill[90].iLevel, pc->skill[91].iLevel, pc->skill[92].iLevel, pc->skill[93].iLevel, pc->skill[94].iLevel,
		pc->skill[95].iLevel, pc->skill[96].iLevel, pc->skill[97].iLevel, pc->skill[98].iLevel, pc->skill[99].iLevel );

	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_SKILL;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pc;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}



int CDBGame::SaveQuest( playerCharacter_t *pc )
{
	querySet_t	request;

	_snprintf(m_query,QUERYSIZE,"EXEC SaveQuest '%s',%d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
						%d, %d, %d, %d",
						pc->userID, pc->serverGroupID, pc->characterID, 
						pc->questVar[0].iVar, pc->questVar[1].iVar, pc->questVar[2].iVar, pc->questVar[3].iVar, pc->questVar[4].iVar, 
						pc->questVar[5].iVar, pc->questVar[6].iVar, pc->questVar[7].iVar, pc->questVar[8].iVar, pc->questVar[9].iVar, 
						pc->questVar[10].iVar, pc->questVar[11].iVar, pc->questVar[12].iVar, pc->questVar[13].iVar, pc->questVar[14].iVar, 
						pc->questVar[15].iVar, pc->questVar[16].iVar, pc->questVar[17].iVar, pc->questVar[18].iVar, pc->questVar[19].iVar, 
						pc->questVar[20].iVar, pc->questVar[21].iVar, pc->questVar[22].iVar, pc->questVar[23].iVar, pc->questVar[24].iVar, 
						pc->questVar[25].iVar, pc->questVar[26].iVar, pc->questVar[27].iVar, pc->questVar[28].iVar, pc->questVar[29].iVar, 
						pc->questVar[30].iVar, pc->questVar[31].iVar, pc->questVar[32].iVar, pc->questVar[33].iVar, pc->questVar[34].iVar, 
						pc->questVar[35].iVar, pc->questVar[36].iVar, pc->questVar[37].iVar, pc->questVar[38].iVar, pc->questVar[39].iVar, 
						pc->questVar[40].iVar, pc->questVar[41].iVar, pc->questVar[42].iVar, pc->questVar[43].iVar, pc->questVar[44].iVar, 
						pc->questVar[45].iVar, pc->questVar[46].iVar, pc->questVar[47].iVar, pc->questVar[48].iVar, pc->questVar[49].iVar, 
						pc->questVar[50].iVar, pc->questVar[51].iVar, pc->questVar[52].iVar, pc->questVar[53].iVar, pc->questVar[54].iVar, 
						pc->questVar[55].iVar, pc->questVar[56].iVar, pc->questVar[57].iVar, pc->questVar[58].iVar, pc->questVar[59].iVar, 
						pc->questVar[60].iVar, pc->questVar[61].iVar, pc->questVar[62].iVar, pc->questVar[63].iVar, pc->questVar[64].iVar, 
						pc->questVar[65].iVar, pc->questVar[66].iVar, pc->questVar[67].iVar, pc->questVar[68].iVar, pc->questVar[69].iVar, 
						pc->questVar[70].iVar, pc->questVar[71].iVar, pc->questVar[72].iVar, pc->questVar[73].iVar, pc->questVar[74].iVar, 
						pc->questVar[75].iVar, pc->questVar[76].iVar, pc->questVar[77].iVar, pc->questVar[78].iVar, pc->questVar[79].iVar, 
						pc->questVar[80].iVar, pc->questVar[81].iVar, pc->questVar[82].iVar, pc->questVar[83].iVar, pc->questVar[84].iVar, 
						pc->questVar[85].iVar, pc->questVar[86].iVar, pc->questVar[87].iVar, pc->questVar[88].iVar, pc->questVar[89].iVar, 
						pc->questVar[90].iVar, pc->questVar[91].iVar, pc->questVar[92].iVar, pc->questVar[93].iVar, pc->questVar[94].iVar, 
						pc->questVar[95].iVar, pc->questVar[96].iVar, pc->questVar[97].iVar, pc->questVar[98].iVar, pc->questVar[99].iVar, 
						
						pc->questVar[100].iVar, pc->questVar[101].iVar, pc->questVar[102].iVar, pc->questVar[103].iVar, pc->questVar[104].iVar, 
						pc->questVar[105].iVar, pc->questVar[106].iVar, pc->questVar[107].iVar, pc->questVar[108].iVar, pc->questVar[109].iVar, 
						pc->questVar[110].iVar, pc->questVar[111].iVar, pc->questVar[112].iVar, pc->questVar[113].iVar, pc->questVar[114].iVar, 
						pc->questVar[115].iVar, pc->questVar[116].iVar, pc->questVar[117].iVar, pc->questVar[118].iVar, pc->questVar[119].iVar, 
						pc->questVar[120].iVar, pc->questVar[121].iVar, pc->questVar[122].iVar, pc->questVar[123].iVar, pc->questVar[124].iVar, 
						pc->questVar[125].iVar, pc->questVar[126].iVar, pc->questVar[127].iVar, pc->questVar[128].iVar, pc->questVar[129].iVar, 
						pc->questVar[130].iVar, pc->questVar[131].iVar, pc->questVar[132].iVar, pc->questVar[133].iVar, pc->questVar[134].iVar, 
						pc->questVar[135].iVar, pc->questVar[136].iVar, pc->questVar[137].iVar, pc->questVar[138].iVar, pc->questVar[139].iVar, 
						pc->questVar[140].iVar, pc->questVar[141].iVar, pc->questVar[142].iVar, pc->questVar[143].iVar, pc->questVar[144].iVar, 
						pc->questVar[145].iVar, pc->questVar[146].iVar, pc->questVar[147].iVar, pc->questVar[148].iVar, pc->questVar[149].iVar, 
						pc->questVar[150].iVar, pc->questVar[151].iVar, pc->questVar[152].iVar, pc->questVar[153].iVar, pc->questVar[154].iVar, 
						pc->questVar[155].iVar, pc->questVar[156].iVar, pc->questVar[157].iVar, pc->questVar[158].iVar, pc->questVar[159].iVar, 
						pc->questVar[160].iVar, pc->questVar[161].iVar, pc->questVar[162].iVar, pc->questVar[163].iVar, pc->questVar[164].iVar, 
						pc->questVar[165].iVar, pc->questVar[166].iVar, pc->questVar[167].iVar, pc->questVar[168].iVar, pc->questVar[169].iVar, 
						pc->questVar[170].iVar, pc->questVar[171].iVar, pc->questVar[172].iVar, pc->questVar[173].iVar, pc->questVar[174].iVar, 
						pc->questVar[175].iVar, pc->questVar[176].iVar, pc->questVar[177].iVar, pc->questVar[178].iVar, pc->questVar[179].iVar, 
						pc->questVar[180].iVar, pc->questVar[181].iVar, pc->questVar[182].iVar, pc->questVar[183].iVar, pc->questVar[184].iVar, 
						pc->questVar[185].iVar, pc->questVar[186].iVar, pc->questVar[187].iVar, pc->questVar[188].iVar, pc->questVar[189].iVar, 
						pc->questVar[190].iVar, pc->questVar[191].iVar, pc->questVar[192].iVar, pc->questVar[193].iVar, pc->questVar[194].iVar, 
						pc->questVar[195].iVar, pc->questVar[196].iVar, pc->questVar[197].iVar, pc->questVar[198].iVar, pc->questVar[199].iVar, 

						pc->questInventory[0].iItem, pc->questInventory[1].iItem, pc->questInventory[2].iItem,
						pc->questInventory[3].iItem, pc->questInventory[4].iItem, pc->questInventory[5].iItem,
						pc->questInventory[6].iItem, pc->questInventory[7].iItem, pc->questInventory[8].iItem,
						pc->questInventory[9].iItem, pc->questInventory[10].iItem, pc->questInventory[11].iItem,
						pc->questInventory[12].iItem, pc->questInventory[13].iItem, pc->questInventory[14].iItem,
						pc->questInventory[15].iItem, pc->questInventory[16].iItem, pc->questInventory[17].iItem,
						pc->questInventory[18].iItem, pc->questInventory[19].iItem, pc->questInventory[20].iItem,
						pc->questInventory[21].iItem, pc->questInventory[22].iItem, pc->questInventory[23].iItem );


	m_query[QUERYSIZE]=NULL;
	strncpy(request.query,m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_QUEST;
	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pc;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}




int CDBGame::AddQuestStatus(playerCharacter_t *pc, questStatus_t *questStatus)
{
	querySet_t	request;

	_snprintf(m_query,QUERYSIZE, "EXEC AddQuestStatus '%s',%d, %d, %d, %d, \
						%d, %d, %d, %d, %d, \
						%d, %d, %d, %d, %d, \
						'%s', %d, %d, %d, %d", 
		pc->userID, pc->serverGroupID, pc->characterID, questStatus->idx, questStatus->questIdx,
		questStatus->dField[0], questStatus->dField[1], questStatus->dField[2], questStatus->dField[3], questStatus->dField[4],
		questStatus->dField[5], questStatus->dField[6], questStatus->dField[7], questStatus->dField[8], questStatus->dField[9],
		questStatus->partyOrganizerName, questStatus->partyOrganizeServer, questStatus->partyOrganizeTime, 
		questStatus->clearTime, questStatus->limitTime);


	m_query[QUERYSIZE]=NULL;
	request.ownerIdx	= pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType	= QUERY_ADD_QUESTSTATUS;
	request.hStmt		= m_hStmt;

	strncpy(request.query,m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.retCode		= -1;
	request.retStructVariable = (void *)pc;


	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}




int CDBGame::RemoveQuestStatus(playerCharacter_t *pc, int idx)
{
	querySet_t	request;
	_snprintf(m_query,QUERYSIZE, "EXEC RemoveQuestStatus '%s', %d, %d, %d", pc->userID, pc->serverGroupID, pc->characterID, idx);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx	= pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType	= QUERY_REMOVE_QUESTSTATUS;
	request.hStmt		= m_hStmt;
	strncpy(request.query,m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.retCode		= -1;
	request.retStructVariable = (void *)pc;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	
	return 1;
}




int CDBGame::SaveQuestStatus(playerCharacter_t *pc, questStatus_t *questStatus)	
{
	querySet_t	request;
	_snprintf(m_query,QUERYSIZE, "EXEC SaveQuestStatus '%s', %d, %d, %d, %d, \
							%d, %d, %d, %d, %d, \
							%d, %d, %d, %d, %d, \
							%d",
							pc->userID, pc->serverGroupID, pc->characterID, questStatus->idx, questStatus->questIdx,
							questStatus->dField[0], questStatus->dField[1], 
							questStatus->dField[2], questStatus->dField[3],
							questStatus->dField[4], questStatus->dField[5],
							questStatus->dField[6], questStatus->dField[7],
							questStatus->dField[8], questStatus->dField[9],
							questStatus->clearTime);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx	= pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType	= QUERY_SAVE_QUESTSTATUS;
	request.hStmt		= m_hStmt;
	strncpy(request.query,m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.retCode		= -1;
	request.retStructVariable = (void *)pc;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	
	return 1;
}



int CDBGame::AddItem(playerCharacter_t *pc, item_t *item, int idx, char *generateID)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC AddItem '%s', %d, %d, %d, '%s', '%s', %d, %d, \
			%d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, \
			%d, %d, %d, \
			%d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, \
			%d, %I64d, %d, %d, %d, '%s'",
		pc->userID, pc->serverGroupID, pc->characterID, idx, generateID, item->name, item->itemTableIdx, item->optionNumber, 
		item->optionIdx[0], item->optionIdx[1], item->optionIdx[2], item->optionIdx[3], item->optionIdx[4], item->optionIdx[5], 
		item->optionValue[0], item->optionValue[1], item->optionValue[2], item->optionValue[3], item->optionValue[4], item->optionValue[5], 
		item->durability, item->reqLevel, item->experience,

		pc->itemNumber,
		pc->inventory[0], pc->inventory[1], pc->inventory[2], pc->inventory[3], pc->inventory[4], pc->inventory[5],
		pc->inventory[6], pc->inventory[7], pc->inventory[8], pc->inventory[9], pc->inventory[10], pc->inventory[11],
		pc->inventory[12], pc->inventory[13], pc->inventory[14], pc->inventory[15], pc->inventory[16], pc->inventory[17],
		pc->inventory[18], pc->inventory[19], pc->inventory[20], pc->inventory[21], pc->inventory[22], pc->inventory[23],
		pc->inventory[24], pc->inventory[25], pc->inventory[26], pc->inventory[27], pc->inventory[28], pc->inventory[29],
		pc->inventory[30], pc->inventory[31], pc->inventory[32], pc->inventory[33], pc->inventory[34], pc->inventory[35],
		pc->inventory[36], pc->inventory[37], pc->inventory[38], pc->inventory[39], pc->inventory[40], pc->inventory[41],
		pc->inventory[42], pc->inventory[43], pc->inventory[44], pc->inventory[45], pc->inventory[46], pc->inventory[47],
		pc->depot[0], pc->depot[1], pc->depot[2], pc->depot[3], pc->depot[4], pc->depot[5], pc->depot[6], pc->depot[7], pc->depot[8], pc->depot[9],
		pc->depot[10], pc->depot[11], pc->depot[12], pc->depot[13], pc->depot[14], pc->depot[15], pc->depot[16], pc->depot[17], pc->depot[18], pc->depot[19],
		pc->depot[20], pc->depot[21], pc->depot[22], pc->depot[23], pc->depot[24], pc->depot[25], pc->depot[26], pc->depot[27], pc->depot[28], pc->depot[29],
		pc->depot[30], pc->depot[31], pc->depot[32], pc->depot[33], pc->depot[34], pc->depot[35], pc->depot[36], pc->depot[37], pc->depot[38], pc->depot[39],
		pc->depot[40], pc->depot[41],
		pc->equipment[0], pc->equipment[1], pc->equipment[2], pc->equipment[3], pc->equipment[4], pc->equipment[5], pc->equipment[6],
		pc->equipment[7], pc->equipment[8], pc->equipment[9], pc->equipment[10], pc->equipment[11], pc->equipment[12], pc->equipment[13],
		pc->mouseInventory,
		item->ItemExtendInfo.Serial,
		item->ItemExtendInfo.UseFlag,
		item->ItemExtendInfo.AllowUseTime,
		item->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType,
		item->ItemExtendInfo.ItemSpawnInfomation.data);


		
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_ADD_ITEM;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pc;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}





int CDBGame::RemoveItem(playerCharacter_t *pc, int idx)
{
	querySet_t	request;

	_snprintf(m_query,QUERYSIZE, "EXEC RemoveItem '%s', %d, %d, %d, \
			%d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, \
			%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, \
			%d",
		pc->userID, pc->serverGroupID, pc->characterID, idx,
		pc->itemNumber,
		pc->inventory[0], pc->inventory[1], pc->inventory[2], pc->inventory[3], pc->inventory[4], pc->inventory[5],
		pc->inventory[6], pc->inventory[7], pc->inventory[8], pc->inventory[9], pc->inventory[10], pc->inventory[11],
		pc->inventory[12], pc->inventory[13], pc->inventory[14], pc->inventory[15], pc->inventory[16], pc->inventory[17],
		pc->inventory[18], pc->inventory[19], pc->inventory[20], pc->inventory[21], pc->inventory[22], pc->inventory[23],
		pc->inventory[24], pc->inventory[25], pc->inventory[26], pc->inventory[27], pc->inventory[28], pc->inventory[29],
		pc->inventory[30], pc->inventory[31], pc->inventory[32], pc->inventory[33], pc->inventory[34], pc->inventory[35],
		pc->inventory[36], pc->inventory[37], pc->inventory[38], pc->inventory[39], pc->inventory[40], pc->inventory[41],
		pc->inventory[42], pc->inventory[43], pc->inventory[44], pc->inventory[45], pc->inventory[46], pc->inventory[47],
		pc->depot[0], pc->depot[1], pc->depot[2], pc->depot[3], pc->depot[4], pc->depot[5], pc->depot[6], pc->depot[7], pc->depot[8], pc->depot[9],
		pc->depot[10], pc->depot[11], pc->depot[12], pc->depot[13], pc->depot[14], pc->depot[15], pc->depot[16], pc->depot[17], pc->depot[18], pc->depot[19],
		pc->depot[20], pc->depot[21], pc->depot[22], pc->depot[23], pc->depot[24], pc->depot[25], pc->depot[26], pc->depot[27], pc->depot[28], pc->depot[29],
		pc->depot[30], pc->depot[31], pc->depot[32], pc->depot[33], pc->depot[34], pc->depot[35], pc->depot[36], pc->depot[37], pc->depot[38], pc->depot[39],
		pc->depot[40], pc->depot[41],
		pc->equipment[0], pc->equipment[1], pc->equipment[2], pc->equipment[3], pc->equipment[4], pc->equipment[5], pc->equipment[6],
		pc->equipment[7], pc->equipment[8], pc->equipment[9], pc->equipment[10], pc->equipment[11], pc->equipment[12], pc->equipment[13],
		pc->mouseInventory);

	m_query[QUERYSIZE]=NULL;
	request.ownerIdx = pc->idx;
	strncpy(request.ownerID, pc->userID, IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_REMOVE_ITEM;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pc;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}





int CDBGame::SaveItem(playerCharacter_t *pc, item_t *item, int idx)
{
	querySet_t	request;

	_snprintf(m_query,QUERYSIZE, "EXEC SaveItem '%s', %d, %d, %d, \
			%d, '%s', %d, \
			%d, %d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d, %d, \
			%d, %d, %d, \
			%I64d, \
			%d,	\
			%d, \
			%d, \
			'%s'",
		pc->userID, pc->serverGroupID, pc->characterID, idx,
		item->itemTableIdx, item->name, item->optionNumber, 
		item->optionIdx[0], item->optionIdx[1], item->optionIdx[2], item->optionIdx[3], item->optionIdx[4], item->optionIdx[5], 
		item->optionValue[0], item->optionValue[1], item->optionValue[2], item->optionValue[3], item->optionValue[4], item->optionValue[5], 
		item->durability, item->reqLevel, item->experience , 
		item->ItemExtendInfo.Serial,
		item->ItemExtendInfo.UseFlag,
		item->ItemExtendInfo.AllowUseTime,
		item->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType,
		item->ItemExtendInfo.ItemSpawnInfomation.data);

	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_ITEM;
	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pc;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}







int CDBGame::AddGuild( short guildIdx )
{
	querySet_t	request;
	
	int querySize;	
	querySize = _snprintf( m_query,QUERYSIZE, "EXEC defaultAddDan %d, %d, '%s', '%s'",
		g_guild[guildIdx].idx, g_guild[guildIdx].serverGroupID, g_guild[guildIdx].name, g_guild[guildIdx].masterName );
	m_query[QUERYSIZE]=NULL;


	request.ownerIdx = guildIdx;
	strncpy(request.ownerID, g_guild[guildIdx].name,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_ADD_GUILD;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)&g_guild[guildIdx];

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}








int CDBGame::SaveGuild( short guildIdx )
{
	querySet_t	request;

	_snprintf(m_query,QUERYSIZE, "EXEC SaveDan %d, %d, '%s',%d,'%s', "
			"%d, %d, %d, %d, %d, "
			"%d, "
			"'%s', "
			"'%s', '%s', '%s', '%s', "
			"'%s', "
			"'%s', "
			"'%s', "
			"'%s', "
			"'%s', "
			"'%s', "
			"'%s', "
			"'%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"%d, %d, %d, %d, %d, "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s', "
			"'%s', '%s', '%s', '%s', '%s'",

		g_guild[guildIdx].idx, 
		g_guild[guildIdx].serverGroupID, 
		g_guild[guildIdx].name, 
		g_guild[guildIdx].markUpdateTime, 
		g_guild[guildIdx].notice, 
		g_guild[guildIdx].famePoint, g_guild[guildIdx].expPoint, g_guild[guildIdx].raidPoint, g_guild[guildIdx].pvpPoint, g_guild[guildIdx].hunterPoint,
		g_guild[guildIdx].level, 
		g_guild[guildIdx].masterName,
		g_guild[guildIdx].subMasterName[0], g_guild[guildIdx].subMasterName[1], g_guild[guildIdx].subMasterName[2], g_guild[guildIdx].subMasterName[3],
		g_guild[guildIdx].factionMasterName[0],
		g_guild[guildIdx].factionMasterName[1],
		g_guild[guildIdx].factionMasterName[2],
		g_guild[guildIdx].factionMasterName[3],
		g_guild[guildIdx].factionMasterName[4],
		g_guild[guildIdx].factionMasterName[5],
		g_guild[guildIdx].factionMasterName[6],
		g_guild[guildIdx].factionMasterName[7],

		g_guild[guildIdx].mateName[0], g_guild[guildIdx].mateName[1], g_guild[guildIdx].mateName[2], g_guild[guildIdx].mateName[3], g_guild[guildIdx].mateName[4],
		g_guild[guildIdx].mateName[5], g_guild[guildIdx].mateName[6], g_guild[guildIdx].mateName[7], g_guild[guildIdx].mateName[8], g_guild[guildIdx].mateName[9],
		g_guild[guildIdx].mateName[10], g_guild[guildIdx].mateName[11], g_guild[guildIdx].mateName[12], g_guild[guildIdx].mateName[13], g_guild[guildIdx].mateName[14],
		g_guild[guildIdx].mateName[15], g_guild[guildIdx].mateName[16], g_guild[guildIdx].mateName[17], g_guild[guildIdx].mateName[18], g_guild[guildIdx].mateName[19],
		g_guild[guildIdx].mateName[20], g_guild[guildIdx].mateName[21], g_guild[guildIdx].mateName[22], g_guild[guildIdx].mateName[23], g_guild[guildIdx].mateName[24],
		g_guild[guildIdx].mateName[25], g_guild[guildIdx].mateName[26], g_guild[guildIdx].mateName[27], g_guild[guildIdx].mateName[28], g_guild[guildIdx].mateName[29],
		g_guild[guildIdx].mateName[30], g_guild[guildIdx].mateName[31], g_guild[guildIdx].mateName[32], g_guild[guildIdx].mateName[33], g_guild[guildIdx].mateName[34],
		g_guild[guildIdx].mateName[35], g_guild[guildIdx].mateName[36], g_guild[guildIdx].mateName[37], g_guild[guildIdx].mateName[38], g_guild[guildIdx].mateName[39],
		g_guild[guildIdx].mateName[40], g_guild[guildIdx].mateName[41], g_guild[guildIdx].mateName[42], g_guild[guildIdx].mateName[43], g_guild[guildIdx].mateName[44],
		g_guild[guildIdx].mateName[45], g_guild[guildIdx].mateName[46], g_guild[guildIdx].mateName[47], g_guild[guildIdx].mateName[48], g_guild[guildIdx].mateName[49],

		g_guild[guildIdx].mateAuthority[0], g_guild[guildIdx].mateAuthority[1], g_guild[guildIdx].mateAuthority[2], g_guild[guildIdx].mateAuthority[3], g_guild[guildIdx].mateAuthority[4],
		g_guild[guildIdx].mateAuthority[5], g_guild[guildIdx].mateAuthority[6], g_guild[guildIdx].mateAuthority[7], g_guild[guildIdx].mateAuthority[8], g_guild[guildIdx].mateAuthority[9],
		g_guild[guildIdx].mateAuthority[10], g_guild[guildIdx].mateAuthority[11], g_guild[guildIdx].mateAuthority[12], g_guild[guildIdx].mateAuthority[13], g_guild[guildIdx].mateAuthority[14],
		g_guild[guildIdx].mateAuthority[15], g_guild[guildIdx].mateAuthority[16], g_guild[guildIdx].mateAuthority[17], g_guild[guildIdx].mateAuthority[18], g_guild[guildIdx].mateAuthority[19],
		g_guild[guildIdx].mateAuthority[20], g_guild[guildIdx].mateAuthority[21], g_guild[guildIdx].mateAuthority[22], g_guild[guildIdx].mateAuthority[23], g_guild[guildIdx].mateAuthority[24],
		g_guild[guildIdx].mateAuthority[25], g_guild[guildIdx].mateAuthority[26], g_guild[guildIdx].mateAuthority[27], g_guild[guildIdx].mateAuthority[28], g_guild[guildIdx].mateAuthority[29],
		g_guild[guildIdx].mateAuthority[30], g_guild[guildIdx].mateAuthority[31], g_guild[guildIdx].mateAuthority[32], g_guild[guildIdx].mateAuthority[33], g_guild[guildIdx].mateAuthority[34],
		g_guild[guildIdx].mateAuthority[35], g_guild[guildIdx].mateAuthority[36], g_guild[guildIdx].mateAuthority[37], g_guild[guildIdx].mateAuthority[38], g_guild[guildIdx].mateAuthority[39],
		g_guild[guildIdx].mateAuthority[40], g_guild[guildIdx].mateAuthority[41], g_guild[guildIdx].mateAuthority[42], g_guild[guildIdx].mateAuthority[43], g_guild[guildIdx].mateAuthority[44],
		g_guild[guildIdx].mateAuthority[45], g_guild[guildIdx].mateAuthority[46], g_guild[guildIdx].mateAuthority[47], g_guild[guildIdx].mateAuthority[48], g_guild[guildIdx].mateAuthority[49],

		g_guild[guildIdx].mateLastConnectTime[0], g_guild[guildIdx].mateLastConnectTime[1], g_guild[guildIdx].mateLastConnectTime[2], g_guild[guildIdx].mateLastConnectTime[3], g_guild[guildIdx].mateLastConnectTime[4],
		g_guild[guildIdx].mateLastConnectTime[5], g_guild[guildIdx].mateLastConnectTime[6], g_guild[guildIdx].mateLastConnectTime[7], g_guild[guildIdx].mateLastConnectTime[8], g_guild[guildIdx].mateLastConnectTime[9],
		g_guild[guildIdx].mateLastConnectTime[10], g_guild[guildIdx].mateLastConnectTime[11], g_guild[guildIdx].mateLastConnectTime[12], g_guild[guildIdx].mateLastConnectTime[13], g_guild[guildIdx].mateLastConnectTime[14],
		g_guild[guildIdx].mateLastConnectTime[15], g_guild[guildIdx].mateLastConnectTime[16], g_guild[guildIdx].mateLastConnectTime[17], g_guild[guildIdx].mateLastConnectTime[18], g_guild[guildIdx].mateLastConnectTime[19],
		g_guild[guildIdx].mateLastConnectTime[20], g_guild[guildIdx].mateLastConnectTime[21], g_guild[guildIdx].mateLastConnectTime[22], g_guild[guildIdx].mateLastConnectTime[23], g_guild[guildIdx].mateLastConnectTime[24],
		g_guild[guildIdx].mateLastConnectTime[25], g_guild[guildIdx].mateLastConnectTime[26], g_guild[guildIdx].mateLastConnectTime[27], g_guild[guildIdx].mateLastConnectTime[28], g_guild[guildIdx].mateLastConnectTime[29],
		g_guild[guildIdx].mateLastConnectTime[30], g_guild[guildIdx].mateLastConnectTime[31], g_guild[guildIdx].mateLastConnectTime[32], g_guild[guildIdx].mateLastConnectTime[33], g_guild[guildIdx].mateLastConnectTime[34],
		g_guild[guildIdx].mateLastConnectTime[35], g_guild[guildIdx].mateLastConnectTime[36], g_guild[guildIdx].mateLastConnectTime[37], g_guild[guildIdx].mateLastConnectTime[38], g_guild[guildIdx].mateLastConnectTime[39],
		g_guild[guildIdx].mateLastConnectTime[40], g_guild[guildIdx].mateLastConnectTime[41], g_guild[guildIdx].mateLastConnectTime[42], g_guild[guildIdx].mateLastConnectTime[43], g_guild[guildIdx].mateLastConnectTime[44],
		g_guild[guildIdx].mateLastConnectTime[45], g_guild[guildIdx].mateLastConnectTime[46], g_guild[guildIdx].mateLastConnectTime[47], g_guild[guildIdx].mateLastConnectTime[48], g_guild[guildIdx].mateLastConnectTime[49],

		g_guild[guildIdx].masterNote[0], g_guild[guildIdx].masterNote[1], g_guild[guildIdx].masterNote[2], g_guild[guildIdx].masterNote[3], g_guild[guildIdx].masterNote[4],
		g_guild[guildIdx].masterNote[5], g_guild[guildIdx].masterNote[6], g_guild[guildIdx].masterNote[7], g_guild[guildIdx].masterNote[8], g_guild[guildIdx].masterNote[9],
		g_guild[guildIdx].masterNote[10], g_guild[guildIdx].masterNote[11], g_guild[guildIdx].masterNote[12], g_guild[guildIdx].masterNote[13], g_guild[guildIdx].masterNote[14],
		g_guild[guildIdx].masterNote[15], g_guild[guildIdx].masterNote[16], g_guild[guildIdx].masterNote[17], g_guild[guildIdx].masterNote[18], g_guild[guildIdx].masterNote[19],
		g_guild[guildIdx].masterNote[20], g_guild[guildIdx].masterNote[21], g_guild[guildIdx].masterNote[22], g_guild[guildIdx].masterNote[23], g_guild[guildIdx].masterNote[24],
		g_guild[guildIdx].masterNote[25], g_guild[guildIdx].masterNote[26], g_guild[guildIdx].masterNote[27], g_guild[guildIdx].masterNote[28], g_guild[guildIdx].masterNote[29],
		g_guild[guildIdx].masterNote[30], g_guild[guildIdx].masterNote[31], g_guild[guildIdx].masterNote[32], g_guild[guildIdx].masterNote[33], g_guild[guildIdx].masterNote[34],
		g_guild[guildIdx].masterNote[35], g_guild[guildIdx].masterNote[36], g_guild[guildIdx].masterNote[37], g_guild[guildIdx].masterNote[38], g_guild[guildIdx].masterNote[39],
		g_guild[guildIdx].masterNote[40], g_guild[guildIdx].masterNote[41], g_guild[guildIdx].masterNote[42], g_guild[guildIdx].masterNote[43], g_guild[guildIdx].masterNote[44],
		g_guild[guildIdx].masterNote[45], g_guild[guildIdx].masterNote[46], g_guild[guildIdx].masterNote[47], g_guild[guildIdx].masterNote[48], g_guild[guildIdx].masterNote[49]
		);


	m_query[QUERYSIZE]=NULL;


	request.ownerIdx = guildIdx;
	strncpy(request.ownerID, g_guild[guildIdx].name ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_GUILD;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)&g_guild[guildIdx];

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}







BOOL CDBGame::SaveGuildMark(const int guildIdx)
{
	querySet_t	request;
	_snprintf( m_query,QUERYSIZE, "EXEC SaveDanMark ?,%d,%d,%d",
		g_guild[guildIdx].idx, 
		g_guild[guildIdx].serverGroupID,
		g_guild[guildIdx].markChangeDate);
	m_query[QUERYSIZE]=NULL;



	request.ownerIdx = guildIdx;
	strncpy(request.ownerID, g_guild[guildIdx].name,IDSTRING );
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_GUILDMARK;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.hDbc = m_hDbc;
	request.retCode = -1;
	request.retStructVariable = (void *)&g_guild[guildIdx];

	if ( !runQueryExecuterThread ) return FALSE;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return TRUE;
}






int CDBGame::BanishMemberGuild(const char* szpName,const int decRankPoint)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf( m_query, QUERYSIZE,
						"EXEC DanForceSecede " 
						"'%s',"			
						"%d"			
						,
						szpName,
						decRankPoint
						);

	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = -1;
	strncpy(request.ownerID,"",IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	strncpy( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.queryType			=	QUERY_BANISHMEMBER_GUILD;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	NULL;

	if ( !runQueryExecuterThread ) return 0;

	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}



int CDBGame::SaveFriends ( playerCharacter_t *pc )
{
	if ( pc->friends.count < 0 )
	{
		pc->friends.count = 0 ;
	}
	if ( pc->friends.count > MAX_FRIENDS )
	{
		pc->friends.count = MAX_FRIENDS ;
	}

	for ( int i = pc->friends.count; i < MAX_FRIENDS; ++i )
	{
		pc->friends.names[i][0] = 0 ;
	}

	querySet_t request;
	_snprintf ( m_query, QUERYSIZE,
						"EXEC SaveFriends "
						"'%s',"			
						"%d,"			
						"%d,"			
						"%d,"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s',"			
						"'%s'"			
						,
						pc->userID,				
						pc->serverGroupID,		
						pc->characterID,		
						pc->friends.count,		
						pc->friends.names[0],	
						pc->friends.names[1],	
						pc->friends.names[2],	
						pc->friends.names[3],	
						pc->friends.names[4],	
						pc->friends.names[5],	
						pc->friends.names[6],	
						pc->friends.names[7],	
						pc->friends.names[8],	
						pc->friends.names[9],	
						pc->friends.names[10],	
						pc->friends.names[11],	
						pc->friends.names[12],	
						pc->friends.names[13],	
						pc->friends.names[14],	
						pc->friends.names[15],	
						pc->friends.names[16],	
						pc->friends.names[17],	
						pc->friends.names[18],	
						pc->friends.names[19]	
						) ;

	m_query[QUERYSIZE]=NULL;
	request.ownerIdx = pc->idx;
	strncpy ( request.ownerID, pc->userID,IDSTRING );
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_FRIENDS;
	strncpy ( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pc;

	if ( !runQueryExecuterThread ) return 0;

	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}






int CDBGame::Send_Save_EventCraftItem( playerCharacter_t *pc, int craftIdx)
{
	querySet_t	request;

	if(craftIdx < 0) return 0;

	itemCraftTable_t *pData = g_EventCraftSystem.Get(craftIdx);
	if(pData == NULL)
	{
		return 0;
	}

	int nPoint = pData->craftCost;
	int nEventIndex = pData->m_lEventIndex;

	_snprintf( m_query, QUERYSIZE,
						"EXEC SaveEvent " 
						"'%s',"					
						"%d,"					
						"%d,"					
						"%d,"					
						"%d"					
						,
						pc->userID,
						pc->serverGroupID,
						pc->characterID,
						nEventIndex,
						nPoint
						);
	m_query[ QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy( request.ownerID, pc->userID ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	strncpy( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.queryType			=	QUERY_SAVE_EVENTCRAFTITEM;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;


	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}

void DB_ResultQuery_Save_EventCraftItem(querySet_t *request, querySet_t *result)
{
	RETCODE				rCode;

	int					column = 1;

	
	playerCharacter_t	*pc = (playerCharacter_t *)request->retStructVariable;

	rCode = SQLFetch(request->hStmt);	

	
	if (rCode == SQL_NO_DATA)				
	{
		g_logSystem->WriteToError ("Save_EventCraftItem Query Error", result->ownerID, result->ownerIdx );

		rCode = SQLCloseCursor( request->hStmt );
		if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
		{
			g_DBGameServer->PrintError( request->hStmt );
		}
		return;
	}
}

int CDBGame::Recv_Load_EventCraftItem( playerCharacter_t *pc,int nEventIndex )
{
	querySet_t	request;

	_snprintf( m_query, QUERYSIZE,
						"EXEC loadEvent " 
						"'%s',"					
						"%d,"					
						"%d,"					
						"%d"					
						,
						pc->userID,
						pc->serverGroupID,
						pc->characterID,
						nEventIndex
						);

	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy( request.ownerID, pc->userID ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	strncpy( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.queryType			=	QUERY_LOAD_EVENTCRAFTITEM;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;

	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}

void DB_ResultQuery_Load_EventCraftItem(querySet_t *request, querySet_t *result)
{
	playerCharacter_t	*pc;
	RETCODE				rCode;
	long				length;
	int					column;

	short				sEventIdx=-1;
	int					n1PointCnt=0;
	int					n3PointCnt=0;
	int					nTotalPoint=0;	


	
	rCode =	SQLFetch(request->hStmt);
	
	pc = (playerCharacter_t *)request->retStructVariable;

	if( rCode == SQL_NO_DATA ) 
	{
		g_logSystem->WriteToError ("Load_EventCraftItem Query Error", result->ownerID, result->ownerIdx );

		rCode = SQLCloseCursor( request->hStmt );
		if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
		{
			g_DBGameServer->PrintError( request->hStmt );
		}

		
		GTH_SendMessage_Reply_Load_EventCraftItem(pc,sEventIdx,n1PointCnt,n3PointCnt,nTotalPoint);
		return; 
	}

	column = 1;
	SQLGetData(request->hStmt, column++, SQL_C_SHORT, &sEventIdx, COLUMNSIZE, &length);
	SQLGetData(request->hStmt, column++, SQL_C_LONG,  &n1PointCnt, COLUMNSIZE, &length);
	SQLGetData(request->hStmt, column++, SQL_C_LONG,  &n3PointCnt, COLUMNSIZE, &length);
	SQLGetData(request->hStmt, column++, SQL_C_LONG,  &nTotalPoint, COLUMNSIZE, &length);

	rCode = SQLCloseCursor( request->hStmt );
	if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
	{
		g_DBGameServer->PrintError( request->hStmt );
	}
	   

	
	GTH_SendMessage_Reply_Load_EventCraftItem(pc,sEventIdx,n1PointCnt,n3PointCnt,nTotalPoint);
}











int CDBGame::SendMail( playerCharacter_t *pc, char* toName, char* mailTitle, char* sendTime, char* mailStr )
{
	querySet_t	request;

	_snprintf( m_query, QUERYSIZE,
						"EXEC SendMail " 
						"%d,"					
						"'%s',"					
						"'%s',"					
						"'%s',"					
						"'%s',"					
						"'%s'"					
						,
						pc->serverGroupID,
						pc->name,
						toName,
						mailTitle,
						sendTime,
						mailStr
						);

	m_query[QUERYSIZE]=NULL;
	request.ownerIdx = pc->idx;
	strncpy( request.ownerID, pc->userID ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	strncpy( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.queryType			=	QUERY_SEND_MAIL;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;

	if ( !runQueryExecuterThread ) return 0;

	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}




int CDBGame::RecvMail( playerCharacter_t *pc )
{
	querySet_t	request;

	_snprintf( m_query, QUERYSIZE,
						"EXEC RecvMail " 
						"'%s',"					
						"%d,"					
						"%d"					
						,
						pc->userID,
						pc->serverGroupID,
						pc->characterID
						);

	m_query[QUERYSIZE]=NULL;
	request.ownerIdx = pc->idx;
	strncpy( request.ownerID, pc->userID ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	strncpy( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;


	request.queryType			=	QUERY_RECV_MAIL;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;

	if ( !runQueryExecuterThread ) return 0;

	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}







int CDBGame::GetMailState( playerCharacter_t *pc )
{
	querySet_t	request;

	_snprintf( m_query, QUERYSIZE,
						"EXEC GetMailState " 
						"'%s',"					
						"%d,"					
						"%d"					
						,
						pc->userID,
						pc->serverGroupID,
						pc->characterID
						);

	m_query[QUERYSIZE]=NULL;
	request.ownerIdx = pc->idx;
	strncpy( request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	strncpy( request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.queryType			=	QUERY_GET_MAILSTATE;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;

	if ( !runQueryExecuterThread ) return 0;

	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}





int CDBGame::DeleteMail( playerCharacter_t *pc, int mailIdx[] )
{
	querySet_t	request;

	_snprintf( m_query, QUERYSIZE,
						"EXEC DeleteMail " 
						"'%s',"					
						"%d,"					
						"%d,"					
						"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
						"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
						"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
						"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
						"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"
						,
						pc->userID,
						pc->serverGroupID,
						pc->characterID,
						mailIdx[0], mailIdx[1], mailIdx[2], mailIdx[3], mailIdx[4], mailIdx[5], mailIdx[6], mailIdx[7], mailIdx[8], mailIdx[9],
						mailIdx[10], mailIdx[11], mailIdx[12], mailIdx[13], mailIdx[14], mailIdx[15], mailIdx[16], mailIdx[17], mailIdx[18], mailIdx[19],
						mailIdx[20], mailIdx[21], mailIdx[22], mailIdx[23], mailIdx[24], mailIdx[25], mailIdx[26], mailIdx[27], mailIdx[28], mailIdx[29],
						mailIdx[30], mailIdx[31], mailIdx[32], mailIdx[33], mailIdx[34], mailIdx[35], mailIdx[36], mailIdx[37], mailIdx[38], mailIdx[39],
						mailIdx[40], mailIdx[41], mailIdx[42], mailIdx[43], mailIdx[44], mailIdx[45], mailIdx[46], mailIdx[47], mailIdx[48], mailIdx[49]
						);

	m_query[QUERYSIZE]=NULL;
	request.ownerIdx = pc->idx;
	strncpy( request.ownerID, pc->userID,IDSTRING );
	request.ownerID[IDSTRING]=NULL;

	strncpy( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.queryType			=	QUERY_DELETE_MAIL;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;

	if ( !runQueryExecuterThread ) return 0;

	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}






int CDBGame::ViewMail( playerCharacter_t *pc, int mailIdx )
{
	querySet_t	request;

	_snprintf( m_query, QUERYSIZE,
						"EXEC ViewMail " 
						"'%s',"					
						"%d,"					
						"%d,"					
						"%d"					
						,
						pc->userID,
						pc->serverGroupID,
						pc->characterID,
						mailIdx
						);

	m_query[QUERYSIZE]=NULL;
	request.ownerIdx = pc->idx;
	strncpy( request.ownerID, pc->userID,IDSTRING );
	request.ownerID[IDSTRING]=NULL;


	strncpy( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.queryType			=	QUERY_VIEW_MAIL;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;

	if ( !runQueryExecuterThread ) return 0;

	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}








int CDBGame::SaveMail( playerCharacter_t *pc, int mailIdx[] )
{
	querySet_t	request;

	_snprintf( m_query, QUERYSIZE,
						"EXEC SaveMail " 
						"'%s',"					
						"%d,"					
						"%d,"					
						"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
						"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
						"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
						"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
						"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"
						,
						pc->userID,
						pc->serverGroupID,
						pc->characterID,
						mailIdx[0], mailIdx[1], mailIdx[2], mailIdx[3], mailIdx[4], mailIdx[5], mailIdx[6], mailIdx[7], mailIdx[8], mailIdx[9],
						mailIdx[10], mailIdx[11], mailIdx[12], mailIdx[13], mailIdx[14], mailIdx[15], mailIdx[16], mailIdx[17], mailIdx[18], mailIdx[19],
						mailIdx[20], mailIdx[21], mailIdx[22], mailIdx[23], mailIdx[24], mailIdx[25], mailIdx[26], mailIdx[27], mailIdx[28], mailIdx[29],
						mailIdx[30], mailIdx[31], mailIdx[32], mailIdx[33], mailIdx[34], mailIdx[35], mailIdx[36], mailIdx[37], mailIdx[38], mailIdx[39],
						mailIdx[40], mailIdx[41], mailIdx[42], mailIdx[43], mailIdx[44], mailIdx[45], mailIdx[46], mailIdx[47], mailIdx[48], mailIdx[49]
						);

	m_query[QUERYSIZE]=NULL;
	request.ownerIdx = pc->idx;
	strncpy( request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	strncpy( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.queryType			=	QUERY_SAVE_MAIL;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;

	if ( !runQueryExecuterThread ) return 0;

	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	

	return 1;
}








CQueryQueue::CQueryQueue()
{
	m_first = 0;
	m_last = -1;
	m_count = 0;
	m_size = MAX_QUERY_NUMBER;
	m_overflow = false;
	m_maxCount = 0;

	memset( m_queue , 0 , sizeof(querySet_t) * MAX_QUERY_NUMBER );
}




CQueryQueue::~CQueryQueue()
{
}





int CQueryQueue::Push (querySet_t *qSet)
{
	if (m_count >= m_size) 
	{
		m_overflow = true;
		return 0;
	}
	if (m_last < m_size - 1)
		m_last ++;
	else 
		m_last = 0;

	memcpy( &m_queue[m_last], qSet, sizeof(querySet_t) );

	m_count ++;
	if (m_count > m_maxCount) 
		m_maxCount = m_count;

	return 1;
}



int CQueryQueue::Pop (querySet_t *qSet)
{
	if (m_count <= 0)
	{
		
		return 0;
	}

	memcpy( qSet, &m_queue[m_first], sizeof(querySet_t) );

	if (m_first < m_size - 1)
		m_first ++;
	else
		m_first = 0;

	m_count --;

	return 1;
}




void CQueryQueue::Clear()
{
	CQueryQueue();
}








int DB_CreateQueryExecuterThread()
{
	
	hQueryRequestMutex = CreateMutex(NULL, FALSE, NULL);
	hQueryResultMutex = CreateMutex(NULL, FALSE, NULL);

	HANDLE			hQueryExecuterThread=NULL;
	DWORD			queryExecuterThreadID;
	


	hQueryExecuterThread = CreateThread(NULL, 0, DB_QueryExecuterThread, NULL, 0, &queryExecuterThreadID);
	if ( !hQueryExecuterThread )
	{
		return 0;
	}	
	
	
 	





	CloseHandle( hQueryExecuterThread );
	return 1;
}




void DB_DestroyQueryExecuterThread()
{
	
	runQueryExecuterThread = 0;
	

	if(INVALID_HANDLE_VALUE != hQueryRequestMutex){
		CloseHandle(hQueryRequestMutex);
		hQueryRequestMutex=INVALID_HANDLE_VALUE;
	}
	

	if(INVALID_HANDLE_VALUE != hQueryResultMutex){
		CloseHandle(hQueryResultMutex);
		hQueryResultMutex=INVALID_HANDLE_VALUE;
	}
	
}


bool DB_ReconnectAccount ( querySet_t & request )
{
	g_DBAccountServer->Close () ;

	if ( g_DBAccountServer->Initialize( g_config.accountDatabase, g_config.odbcDSNType ) )
	{
		request.hStmt = g_DBAccountServer->GetStmt () ;
		return true ;
	}
	else
		return false ;
}


bool DB_ReconnectGame ( querySet_t & request )
{









		g_DBGameServer->Close () ;

		if ( g_DBGameServer->Initialize( g_config.gameDatabase, g_config.odbcDSNType ) )
		{
			request.hStmt = g_DBGameServer->GetStmt () ;
			return true ;
		}


	return false ;
}


bool DB_Reconnect ( querySet_t & request )
{
	
	if ( g_DBAccountServer->GetStmt () == request.hStmt )
	{
		return DB_ReconnectAccount ( request ) ;
	}
	else	
	{
		return DB_ReconnectGame ( request ) ;
	}
}



void DB_ResultQuery_QUERY_SAVECHA_PACKAGE(querySet_t& request, querySet_t& result)
{
	RETCODE rCode;
	long length;	

	
	rCode = SQLFetch(request.hStmt);        
	if((rCode == SQL_NO_DATA) || (rCode == SQL_ERROR)){
		result.retCode = rCode;
		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
			g_DBGameServer->PrintError( request.hStmt );
		}
		return; 
	}

	result.retCode = rCode;	
	SQLGetData(request.hStmt,1, SQL_C_SLONG,&(result.QUERY_SAVECHA_PACKAGE.packageIdx),sizeof(int), &length);	

	rCode = SQLCloseCursor( request.hStmt );
	if((rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
		g_DBGameServer->PrintError( request.hStmt );
	}
}




void DB_ResultQuery_QUERY_SAVECHAPACKAGELIST_BY_SYTEM(querySet_t& request, querySet_t& result)
{
	RETCODE rCode;
	long length;	

	
	rCode = SQLFetch(request.hStmt);        
	if((rCode == SQL_NO_DATA) || (rCode == SQL_ERROR)){
		result.retCode = rCode;
		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
			g_DBGameServer->PrintError( request.hStmt );
		}
		return; 
	}

	result.retCode = rCode;	
	
	SQLGetData(request.hStmt,1, SQL_C_SLONG,&(result.QUERY_SAVECHA_PACKAGE.packageIdx),sizeof(int), &length);	

	rCode = SQLCloseCursor( request.hStmt );
	if((rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
		g_DBGameServer->PrintError( request.hStmt );
	}
}



void DB_ResultQuery_QUERY_POSTSYSTEM_LOADPACKAGECNT(querySet_t& request, querySet_t& result)
{
	if(result.retCode < 0){
		result.PostSystem_LoadPackageCnt.iUnReadedNum=0;
		result.PostSystem_LoadPackageCnt.iReadedNum=0;
		return;
	}


	RETCODE rCode;
	long length;	
	
	
	rCode = SQLFetch(request.hStmt);        
	if((rCode == SQL_NO_DATA) || (rCode == SQL_ERROR)){
		result.retCode = rCode;
		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
			g_DBGameServer->PrintError( request.hStmt );
		}
		return; 
	}
	
	result.retCode = rCode;	
	SQLGetData(request.hStmt,1, SQL_C_SLONG,&(result.PostSystem_LoadPackageCnt.iUnReadedNum),sizeof(int), &length);
	SQLGetData(request.hStmt,2, SQL_C_SLONG,&(result.PostSystem_LoadPackageCnt.iReadedNum) ,sizeof(int), &length);	
	
	
	rCode = SQLCloseCursor( request.hStmt );
	if((rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
		g_DBGameServer->PrintError( request.hStmt );
	}
}




void DB_ProcessQueryResultQueue_QUERY_OPEN_POST(const querySet_t& result)
{
	
	CPostManager::tagDetail* pDetail = 	(CPostManager::tagDetail*)result.pData;
	

	playerCharacter_t* pPlayer = gcpTools->GetPlayerRecordPointer(result);
	if(NULL == pPlayer){
		if(NULL != pDetail){ GlobalFreePtr(pDetail); pDetail=NULL; }
		return;
	}


	switch(result.retCode){
		case SQL_ERROR:
			g_PostFunc.GTH_SendMessage_MailBox_Open(pPlayer,-1,1,-1);
			break;

		case SQL_NO_DATA:
			{
				
				CPostManager::PostPackage_t* pMyPostPackage = pPlayer->m_PostMng.GetOpenPost();	
				if(NULL == pMyPostPackage){
					g_PostFunc.GTH_SendMessage_MailBox_Open(pPlayer,0,1,-1);
					if(NULL != pDetail){ GlobalFreePtr(pDetail); pDetail=NULL; }
					return;
				}				

				g_PostFunc.GTH_SendMessage_MailBox_Open(pPlayer,1,0,pPlayer->m_PostMng.m_OpenPostIdx);
			}
			break;

		case SQL_SUCCESS:
			{
				CPostManager::PostPackage_t* pMyPostPackage = pPlayer->m_PostMng.GetOpenPost();	
				if(NULL == pMyPostPackage){
					g_PostFunc.GTH_SendMessage_MailBox_Open(pPlayer,0,1,-1);
					if(NULL != pDetail){ GlobalFreePtr(pDetail); pDetail=NULL; }
					return;
				}				
				
				pMyPostPackage ->ItemNumber = pDetail->ItemNumber;
				for(int idx=0; idx < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; idx++)
					pMyPostPackage->Items[idx]=pDetail->Items[idx];	
				g_PostFunc.GTH_SendMessage_MailBox_Open(pPlayer,1,0,pPlayer->m_PostMng.m_OpenPostIdx);
			}
			break;
	}

	if(NULL != pDetail){ GlobalFreePtr(pDetail); pDetail=NULL; }
	return;
}





void DB_QueryExecuterThread_OpenPost(const querySet_t& request,querySet_t& result)
{
	RETCODE				rCode=SQL_ERROR;
	int					idx=0;	

	
	rCode= SQLFetch(request.hStmt);	
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode)){		
		result.retCode=rCode;

		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
		{
			g_DBGameServer->PrintError( request.hStmt );
		}
		return;	
	}
	result.retCode = rCode;	
	CPostManager::tagDetail* pDetail = 
		(CPostManager::tagDetail*)GlobalAllocPtr(GMEM_FIXED,sizeof(CPostManager::tagDetail));	
	if(NULL == pDetail){
		result.retCode=-1;
		return;
	}



	result.pData = (void*)pDetail;
	
	
	pDetail->ItemNumber = 0;	
	for(idx =0; idx < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; idx++)
		InitItem(&pDetail->Items[idx]);

	
	long		length =0;
	item_t*		pItem = NULL;
	int			itemIdx = 0;
	int			column = 1;	
	const int	maxbytes_genid=29;
	char		GenId[maxbytes_genid+1]="";

	for(idx = 0; (rCode == SQL_SUCCESS) && (idx < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE); rCode = SQLFetch(request.hStmt), idx++)
	{
		column = 1;	
		
		
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &itemIdx,sizeof(int), &length);
		if(itemIdx < 0) break;
		if(itemIdx > CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE) break;

		pItem = &pDetail->Items[itemIdx];		
		SQLGetData(request.hStmt, column++, SQL_C_CHAR,  GenId, maxbytes_genid, &length);
		GenId[maxbytes_genid]=NULL;


		SQLGetData(request.hStmt, column++, SQL_C_CHAR,  pItem->name, 32, &length);
		pItem->name[32-1]=NULL;


		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pItem->itemTableIdx, sizeof(int), &length); 
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pItem->optionNumber, sizeof(int), &length); 

		for (int i=0; i<MAX_NUMBER_OF_ITEM_OPTION; i++)
			SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pItem->optionIdx[i], sizeof(int), &length); 

		for (i=0; i<MAX_NUMBER_OF_ITEM_OPTION; i++)
			SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pItem->optionValue[i], sizeof(int), &length); 

		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pItem->durability, sizeof(int), &length); 
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pItem->reqLevel, sizeof(int), &length); 
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pItem->experience, sizeof(int), &length); 
		
		SQLGetData(request.hStmt, column++, SQL_C_SBIGINT, &pItem->ItemExtendInfo.Serial, sizeof(__int64), &length); 
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pItem->ItemExtendInfo.UseFlag, sizeof(int), &length); 
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pItem->ItemExtendInfo.AllowUseTime, sizeof(int), &length); 
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pItem->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType, sizeof(int), &length); 		
		SQLGetData(request.hStmt, column++, SQL_C_CHAR, &pItem->ItemExtendInfo.ItemSpawnInfomation.data, 
				item_t::MAX_SIZE_OF_ITEM_SPAWN_DATA, &length);
		
		pDetail->ItemNumber++;
	}
	

	rCode = SQLCloseCursor( request.hStmt );
	if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
	{
		g_DBGameServer->PrintError( request.hStmt );
	}
	return;
}




void DB_QueryExecuterThread_QUERY_GIFTCARD_LoadGiftCard(const querySet_t& request,querySet_t& result);
void DB_QueryExecuterThread_QUERY_GIFTCARD_LoadGiftCardItembytype(const querySet_t& request,querySet_t& result);
void DB_QueryExecuterThread_QUERY_GIFTCARD_SaveGiftCard(const querySet_t& ,querySet_t& );
void DB_QueryExecuterThread_QUERY_LOAD_CHARACTER(const querySet_t& ,querySet_t& );
void DB_QueryExecuterThread_QUERY_LOAD_QUEST(const querySet_t& request,querySet_t& result);
void DB_QueryExecuterThread_QUERY_LOAD_QUESTSTATUS(const querySet_t& request,querySet_t& result);
void DB_QueryExecuterThread_QUERY_LOADSERVERSTATUS(const querySet_t& request,querySet_t& result);
void DB_QueryExecuterThread_QUERY_SAVESERVERSTATUS(const querySet_t& ,querySet_t& );

DWORD WINAPI DB_QueryExecuterThread(LPVOID param)
{	
	queryRequestQueue = new CQueryQueue;
	queryResultQueue = new CQueryQueue;

	querySet_t		request;
	querySet_t		result;
	RETCODE			rCode;
	int				isExistQuery;	


	runQueryExecuterThread = 1;
	while(runQueryExecuterThread){

		
		WaitForSingleObject( hQueryRequestMutex, INFINITE );
		if (queryRequestQueue->m_count > 0)
		{
			isExistQuery = queryRequestQueue->Pop( &request );
		}else{
			isExistQuery = 0;
		}
		ReleaseMutex( hQueryRequestMutex );
		Sleep(1);//lucky CPU



		if ( !isExistQuery ) continue;
		isExistQuery = 0;


		
		
		
		
		

		if( request.queryType == QUERY_SAVE_GUILDMARK )
		{
			SQLINTEGER szTm = SQL_NTS;
			guild_t* pGuild = (guild_t*)request.retStructVariable;

			rCode = SQLPrepare( request.hStmt, (unsigned char *)request.query, SQL_NTS );
			if ( SQL_SUCCESS_WITH_INFO == rCode || SQL_ERROR == rCode )
			{
				g_DBGameServer->PrintError(request) ;
			}
			SQLINTEGER cbMark;
			SQLPOINTER pParamData;

			rCode = SQLBindParameter( request.hStmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY, GUILDMARKSIZE, 0, pGuild->mark, 0, &cbMark ) ;
			if( SQL_SUCCESS_WITH_INFO == rCode || SQL_ERROR == rCode ){
				g_DBGameServer->PrintError (request) ;
			}

			cbMark	= SQL_LEN_DATA_AT_EXEC(0);
			rCode = SQLExecute(request.hStmt) ;
			while(rCode == SQL_NEED_DATA){      
				rCode = SQLParamData(request.hStmt, &pParamData) ;
				if(rCode == SQL_NEED_DATA){
					if(pParamData == pGuild->mark ) 
						SQLPutData(request.hStmt, pGuild->mark , GUILDMARKSIZE );
				}
				else break ;
			}

			rCode = SQLExecute(request.hStmt) ;		
			if(rCode == SQL_NEED_DATA){
				rCode= SQLCancel(request.hStmt) ;
				rCode= SQLTransact(SQL_NULL_HENV, request.hDbc, SQL_COMMIT);				
				if(rCode != SQL_SUCCESS && rCode != SQL_SUCCESS_WITH_INFO){
					g_DBGameServer->PrintError (request) ;
				}
			}else{
				g_DBGameServer->PrintError (request) ;
			}

		}
		else
		{
			
			rCode = SQLExecDirect(request.hStmt, (unsigned char *)request.query, SQL_NTS);
		}


		if ( SQL_ERROR == rCode )
		{
			if ( CheckDisconnect ( request.hStmt ) )
			{
				if ( DB_Reconnect ( request ) )
				{
					
					rCode = SQLExecDirect (request.hStmt, (unsigned char *)request.query, SQL_NTS) ;
				}
			}
			else
			{

				g_DBGameServer->PrintError (request) ;
			}
		}
		else if ( SQL_SUCCESS_WITH_INFO == rCode )
		{

			g_DBGameServer->PrintError (request) ;
		}

		memcpy(&result, &request, sizeof(querySet_t));
		result.retCode = rCode;
		
		switch ( request.queryType )
		{
		
			
		case QUERY_SAVE_DANBATTLE_GUILD_ADD_WINPOINT:
			break;
		case QUERY_SAVE_DANBATTLE_GUILD_ADD_LOSEPOINT:
			break;
		case QUERY_SAVE_DANBATTLE_GUILD_ADD_ACCPOINT:
			break;
		case QUERY_LOAD_DANBATTLE_GUILD_INFO:
			DB_QueryExecuterThread_Query_LOAD_DANBATTLE_GUILD_INFO(request, result);
			break;
		case QUERY_SAVE_DANBATTLE_PCPOINT:
			break;

		case QUERY_INSERT_DANBATTLE:
			DB_QueryExecuterThread_INSERT_DANBATTLE(request,result);
			break;

		case QUERY_DELETE_ALLDANBATTLE:
			
			break;
		case QUERY_DELETE_DANBATTLE:
			
			break;


		case Query_GolryunBattle_LoadLastBattle:
			
			
			assert(NULL != gcpGolryunBattle);
			assert(NULL != (gcpGolryunBattle->GetDBV2ClassPointer()));
			(gcpGolryunBattle->GetDBV2ClassPointer())->ODBC_QueryExecuterThread_Query_GolryunBattle_LoadLastBattle(request,result);
			break;

		case Query_GolryunBattle_AddBattle:
			
			(gcpGolryunBattle->GetDBV2ClassPointer())->ODBC_QueryExecuterThread_Query_GolryunBattle_AddBattle(request,result);
			break;



		case Query_GolryunBattle_LoadTournament:
			
			
			assert(NULL != gcpGolryunBattle);
			assert(NULL != (gcpGolryunBattle->GetDBV2ClassPointer()));
			(gcpGolryunBattle->GetDBV2ClassPointer())->ODBC_QueryExecuterThread_Query_GolryunBattle_LoadTournament(request,result);
			break;

			
		case Query_GolryunBattle_SaveBattle:
			
			if(NULL == gcpGolryunBattle) break;
			if(NULL == gcpGolryunBattle->GetDBV2ClassPointer()) break;
			(gcpGolryunBattle->GetDBV2ClassPointer())->ODBC_QueryExecuterThread_Query_GolryunBattle_SaveBattle(request,result);
			break;
			
		case Query_GolryunBattle_AddTournament:
			
			(gcpGolryunBattle->GetDBV2ClassPointer())->ODBC_QueryExecuterThread_Query_GolryunBattle_AddTournament(request,result);
			break;
			
		case Query_GolryunBattle_SaveTournament:
			
			(gcpGolryunBattle->GetDBV2ClassPointer())->ODBC_QueryExecuterThread_Query_GolryunBattle_SaveTournament(request,result);
			break;
			

		case QUERY_SAVESERVERSTATUS:
			DB_QueryExecuterThread_QUERY_SAVESERVERSTATUS(request,result);
			break;

		case QUERY_LOADSERVERSTATUS:
			DB_QueryExecuterThread_QUERY_LOADSERVERSTATUS(request,result);
			break;


		case QUERY_GIFTCARD_SaveGiftCard:
			DB_QueryExecuterThread_QUERY_GIFTCARD_SaveGiftCard(request,result);
			break;

		case QUERY_GIFTCARD_LoadGiftCardItembytype:
			DB_QueryExecuterThread_QUERY_GIFTCARD_LoadGiftCardItembytype(request,result);
			break;

		case QUERY_GIFTCARD_LoadGiftCard:
			DB_QueryExecuterThread_QUERY_GIFTCARD_LoadGiftCard(request,result);
			break;


		case QUERY_DELETE_POSTITEMS:
			Sleep(0);
			break;

		case QUERY_SAVECHAPACKAGELIST_BY_SYTEM:
			DB_ResultQuery_QUERY_SAVECHAPACKAGELIST_BY_SYTEM(request,result);
			break;

		case QUERY_GolryunBattle_SaveBattleInfo:
			Sleep(0);
			break;


		case QUERY_POSTSYSTEM_LOADPACKAGECNT:
			DB_ResultQuery_QUERY_POSTSYSTEM_LOADPACKAGECNT(request,result);
			break;

		case QUERY_SAVECHA_PACKAGE:
			DB_ResultQuery_QUERY_SAVECHA_PACKAGE(request,result);
			break;

		case QUERY_SAVECHA_ITEM_DETAIL:
			break;

			
		case QUERY_UPDATE_GUILDIDX:
			break;
			

		case QUERY_INSERT_LOGIN:
			break;


		case QUERY_DELETE_LOGIN:
			break;
		case QUERY_DELETE_LOGIN_HOST :
			break;

		case QUERY_SAVE_PREMIUMINFO:
			Sleep(0);
			break;

		case QUERY_LOAD_PREMIUMINFO:
			DB_ResultQuery_Load_PremiumInfo(request,result);
			break;

		case QUERY_LOAD_CHARACTER:
			DB_QueryExecuterThread_QUERY_LOAD_CHARACTER(request,result);
			
			break;


		case QUERY_LOAD_ITEM:
			DB_ResultQuery_LoadItem(&request, &result);
			break;

		case QUERY_LOAD_QUEST :	
			DB_QueryExecuterThread_QUERY_LOAD_QUEST(request,result);
			
			break;

		case QUERY_LOAD_QUESTSTATUS :	
			DB_QueryExecuterThread_QUERY_LOAD_QUESTSTATUS(request,result);
			
			break;

		case QUERY_LOAD_SKILL:
			DB_ResultQuery_LoadSkill(&request, &result);
			break;

		case QUERY_SAVE_GAME:
			break;
		case QUERY_SAVE_QUEST :	
			break;
		case QUERY_SAVE_SKILL:
			break;
			
			
		case QUERY_LOAD_PACKAGELIST:
			DB_ResultQuery_LoadWebPackageList(request,result);
			break;
			
		case QUERY_OPEN_POST:
			DB_QueryExecuterThread_OpenPost(request,result);
			
			break;			

		case QUERY_LOAD_FRIENDS:
			DB_ResultQuery_LoadFriends (&request, &result) ;
			break ;
		
		case QUERY_LOAD_HELPLIST:
			DB_ResultQuery_LoadHelpList(request,result);
			break;
		case QUERY_ADD_QUESTSTATUS :
			break;
		case QUERY_REMOVE_QUESTSTATUS :
			break;
		case QUERY_SAVE_QUESTSTATUS :
			break;

		case QUERY_SAVE_FRIENDS:		
			break ;

		case QUERY_ADD_ITEM:
			break;
		case QUERY_REMOVE_ITEM:
			break;
		case QUERY_SAVE_ITEM:
			break;

		case QUERY_ADD_GUILD:
			break;

		case QUERY_SAVE_GUILD:
			break;

		case QUERY_LOAD_GUILD:
			DB_ResultQuery_LoadGuild(&request, &result);						
			
			break;

		case QUERY_REMOVE_GUILD:
			break;


		case QUERY_BANISHMEMBER_GUILD:			
			
	
			
			
			break;


			
			
		case QUERY_SEND_MAIL:
			DB_ResultQuery_SendMail( &request, &result );
			break;
		case QUERY_RECV_MAIL:
			DB_ResultQuery_RecvMail( &request, &result );
			break;
		case QUERY_GET_MAILSTATE:
			DB_ResultQuery_GetMailState( &request, &result );
			break;
		case QUERY_DELETE_MAIL:
			DB_ResultQuery_DeleteMail( &request, &result );
			break;
		case QUERY_VIEW_MAIL:
			DB_ResultQuery_ViewMail( &request, &result );
			break;
		case QUERY_SAVE_MAIL:
			DB_ResultQuery_SaveMail( &request, &result );
			break;

		
		case QUERY_SAVE_EVENTCRAFTITEM:
			DB_ResultQuery_Save_EventCraftItem( &request, &result );
			break;

		case QUERY_LOAD_EVENTCRAFTITEM:
			DB_ResultQuery_Load_EventCraftItem( &request, &result );
			break;

		

			


			

		
		case QUERY_LOAD_GONRYUNBATTLE:
			DB_ResultQuery_LoadGonryunBattle(&request, &result);

			g_DBGameServer->PrintError (request.hStmt) ;
			break;
		case QUERY_SAVE_GONRYUNBATTLE:
			break;
		
		case QUERY_SAVE_SKIN:
			
			break;
		default:
			break;
		}



		
		WaitForSingleObject( hQueryResultMutex, INFINITE );
		queryResultQueue->Push( &result );
		ReleaseMutex( hQueryResultMutex );

		
	}


	
	if(NULL != queryRequestQueue )    { delete queryRequestQueue;   queryRequestQueue = NULL; }
	if(NULL !=  queryResultQueue )    { delete queryResultQueue;   queryResultQueue = NULL; }	

	return 0;
} 






void DB_ProcessQueryResultQueue_QUERY_SAVE_GAME(const querySet_t& result)
{
	playerCharacter_t	*pc = gcpTools->GetPlayerRecordPointer(result);
	if(NULL == pc) return;






	if ( result.retCode < 0)
	{
		
		if(result.parameter == force){
			if (0 == stricmp(result.ownerID, pc->userID)) GTH_RemovePC( pc );
		}
		g_DBGameServer->PrintError(result.hStmt);
		g_logSystem->Write("Database Error: QUERY_SAVE_GAME, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
		return;
	}

	
	if ( !pc->active )
	{
		g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_SAVE_GAME, ownerIdx: %d)", result.ownerIdx);
		return;
	}
	if ( stricmp(result.ownerID, pc->userID) != 0)
	{
		g_logSystem->Write("Query Owner Changed Error!! : (QUERY_SAVE_GAME, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
		return;
	}

	g_logSystem->Write("User [%s] Data Save Complete(flag: %d)", pc->userID, pc->flagChangeServer);
	if ( pc->flagChangeServer > 0 ) 
	{
		GTH_ChangeServer( pc );
	}

	if ( pc->flagChangeServer >= -1 || !pc->normalNetwork )
	{
		GTH_RemovePC( pc );
	}
	else
	{		
		pc->flagChangeServer = -1;		
	}


	if(result.SaveGameVar.why == packet_communication_timeout){
		g_logSystem->Write("not reay charactor :  name: %s", pc->name);
		GTH_RemovePC( pc );
		return;
	}

}









void DB_ProcessQueryResultQueue_QUERY_POSTSYSTEM_LOADPACKAGECNT(const querySet_t& result)
{
	playerCharacter_t* pPlayerRecord = gcpTools->GetPlayerRecordPointer(result);
	if(NULL == pPlayerRecord) return;


	g_PostFunc.GTH_SendMessage_MailBox_Cnt(
		pPlayerRecord,
		result.PostSystem_LoadPackageCnt.iUnReadedNum,
		result.PostSystem_LoadPackageCnt.iReadedNum);
}




void DB_ProcessQueryResultQueue_QUERY_SAVECHA_PACKAGE_DETAIL(const querySet_t& result)
{
	if(result.retCode < 0){
		g_logSystem->Write("SavePackageDetail Error!! : query=%s", result.query);				
		return;
	}
	
	
	
}

void DB_ProcessQueryResultQueue_QUERY_SAVECHA_PACKAGE(const querySet_t& result)
{	
	playerCharacter_t* pPlayerRecord = NULL;
	pPlayerRecord = gcpTools->GetPlayerRecordPointer(result.ownerIdx);
	if(NULL == pPlayerRecord) return;
	
	
	
	if( result.retCode < 0){
		g_PostFunc.GTH_ProcessMessage_MailBox_UndoInvenItem(pPlayerRecord);
		g_logSystem->Write("SavePackage Error!! : (ID: %s)", pPlayerRecord->userID);				
		g_PostFunc.GTH_SendMessage_PostSend(pPlayerRecord, FALSE,0);
		return;
	}
	

	
	if(result.QUERY_SAVECHA_PACKAGE.packageIdx == -1)
	{
		g_PostFunc.GTH_ProcessMessage_MailBox_UndoInvenItem(pPlayerRecord);
		g_logSystem->Write("Character not found Error!! : (ID: %s)", pPlayerRecord->userID);
		g_PostFunc.GTH_SendMessage_PostSend(pPlayerRecord, FALSE, 3);
		return;
	}

	
	
	
	if ( pPlayerRecord->m_PostMng.GetPostState() == CPostManager::enumPostSystemMode::POSTSYSTEM_NONE){
		g_logSystem->Write("SavePackage Error!! : (ID: %s)", pPlayerRecord->userID);				
		g_PostFunc.GTH_SendMessage_PostSend(pPlayerRecord, FALSE,0);
		return;
	}
	
	
	
	CPostManager::PostPackage_t* pPostPackage = NULL;	
	pPostPackage = pPlayerRecord->m_PostMng.GetOpenPost();
	if(NULL == pPostPackage){
		g_PostFunc.GTH_SendMessage_PostSend(pPlayerRecord, FALSE,1);
		return;	 
	}

	g_PostFunc.GTH_SendMessage_PostSend(pPlayerRecord, TRUE, -1);
	
	
	item_t* pItem	= NULL;
	for(int PostItemIdx = 0; PostItemIdx < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; PostItemIdx++)
	{		
		if(pPostPackage->ItemBox[PostItemIdx] < 0) continue;
		if(pPostPackage->ItemBox[PostItemIdx] >= MAX_NUMBER_OF_OWNITEM) continue;
		
		pItem = &pPlayerRecord->item[pPostPackage->ItemBox[PostItemIdx]];		
		if( (pItem->itemTableIdx < 0) || (pItem->itemTableIdx >= g_itemTableNumber))	continue;
		
		g_DBGameServer->SavePost(
			pPlayerRecord, 
			result.QUERY_SAVECHA_PACKAGE.packageIdx,
			PostItemIdx,	
			"Unknown", 
			pPostPackage->szFromName, 
			pItem);	
		

		g_logSystem->WriteToLog(pPlayerRecord, LOG_CLASS_POSTSYSTEM,
								"%d;%d;%d;;;%s;%s", LOG_CLASS_POSTSYSTEM_SEND_CHA,
								1, pItem->durability, 
								pItem->name,
								pPostPackage->szFromName);


		GTH_DeleteMyItem(pPlayerRecord,pItem);
		g_DBGameServer->RemoveItem(pPlayerRecord, pPostPackage->ItemBox[PostItemIdx]);
		pPostPackage->ItemBox[PostItemIdx]=-1;

	}

	g_logSystem->WriteToLog(pPlayerRecord, LOG_CLASS_POSTSYSTEM,
							"%d;%d;%d;;;%s;%s", LOG_CLASS_POSTSYSTEM_SEND_CHA,
							0, pPostPackage->Nak, 
							"NAK",
							pPostPackage->szFromName);



	pPlayerRecord->curChargeSE -= pPostPackage->Nak;

	g_PostFunc.GTH_SendMessage_PostSendComplete(pPlayerRecord, 1,-1);
	
}





void DB_ResultQuery_Load_PremiumInfo(querySet_t& request, querySet_t& result)
{
	RETCODE rCode=SQL_ERROR;
	long length=0;	

	result.Premium.iMemberShipType = playerCharacter_t::tagPremiumInfo::enumMeberShipType::Pay;
	result.Premium.iRemainSecond = 0x7fffffff;
	result.Premium.WorldChatting.lDate=time(NULL);
	result.Premium.WorldChatting.iDecreaseCount=0x7fffffff;
	


	
	
	rCode = SQLFetch(request.hStmt);        
	if((rCode == SQL_NO_DATA) || (rCode == SQL_ERROR)){
		result.retCode = rCode;
		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
			g_DBGameServer->PrintError( request.hStmt );
		}
		return; 
	}

	result.retCode = rCode;
	
	SQLGetData(request.hStmt,1, SQL_C_SLONG,&(result.Premium.iMemberShipType),sizeof(int), &length);
	SQLGetData(request.hStmt,2, SQL_C_SLONG,&(result.Premium.iRemainSecond),sizeof(int), &length);
	SQLGetData(request.hStmt,3, SQL_C_SLONG,&(result.Premium.WorldChatting.lDate),sizeof(long), &length);
	SQLGetData(request.hStmt,4, SQL_C_SLONG,&(result.Premium.WorldChatting.iDecreaseCount),sizeof(int), &length);

	
	rCode = SQLCloseCursor( request.hStmt );
	if((rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
		g_DBGameServer->PrintError( request.hStmt );
	}
}







void DB_ResultQuery_LoadItem(querySet_t *request, querySet_t *result )
{
	playerCharacter_t	*pc=NULL;
	item_t				*item=NULL;
	RETCODE rCode=SQL_ERROR;
	long length=0;
	int column, i;
	int itemIdx=-1;
	int itemNumber=0;

	
	rCode = SQLFetch(request->hStmt);        
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode))
	{
		result->retCode = rCode;
		rCode = SQLCloseCursor( request->hStmt );
		if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
		{

			g_DBGameServer->PrintError( request->hStmt );
		}
		return; 
	}

	result->retCode = rCode;
	pc = (playerCharacter_t *)request->retStructVariable;

	itemNumber = 0;

	for (; rCode == SQL_SUCCESS; rCode = SQLFetch(request->hStmt), itemNumber++ )
	{
		column = 1;
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &itemIdx, COLUMNSIZE, &length);

		item = &pc->item[itemIdx];

		SQLGetData(request->hStmt, column++, SQL_C_CHAR,  item->name, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->itemTableIdx, COLUMNSIZE, &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->optionNumber, COLUMNSIZE, &length); 

		for (i=0; i<MAX_NUMBER_OF_ITEM_OPTION; i++)
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->optionIdx[i], COLUMNSIZE, &length); 

		for (i=0; i<MAX_NUMBER_OF_ITEM_OPTION; i++)
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->optionValue[i], COLUMNSIZE, &length); 

		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->durability, COLUMNSIZE, &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->reqLevel, COLUMNSIZE, &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->experience, COLUMNSIZE, &length); 

		
		SQLGetData(request->hStmt, column++, SQL_C_SBIGINT, &item->ItemExtendInfo.Serial, sizeof(__int64), &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->ItemExtendInfo.UseFlag, sizeof(int), &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->ItemExtendInfo.AllowUseTime, sizeof(int), &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType, sizeof(int), &length); 		
		SQLGetData(request->hStmt, column++, SQL_C_CHAR, &item->ItemExtendInfo.ItemSpawnInfomation.data, 
			item_t::MAX_SIZE_OF_ITEM_SPAWN_DATA, &length); 
		
		
	}
	    
	pc->itemNumber = itemNumber; 

	rCode = SQLCloseCursor( request->hStmt );
	if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
	{

		g_DBGameServer->PrintError( request->hStmt );
	}
}

void DB_ResultQuery_LoadSkill( querySet_t *request, querySet_t *result )
{
	RETCODE rCode;
	playerCharacter_t *pc;
	skill_t *skill;
	int skillNumber;
	long length;
	int column, i;

	
	rCode = SQLFetch(request->hStmt);        
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode))
	{
		result->retCode = rCode;
		rCode = SQLCloseCursor( request->hStmt );
		if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
		{
			g_DBGameServer->PrintError( request->hStmt );
		}
		return; 
	}

	result->retCode = rCode;
	pc = (playerCharacter_t *)request->retStructVariable;

	for (; rCode == SQL_SUCCESS; rCode = SQLFetch(request->hStmt))
	{
		column = 1;

		skillNumber = 0;
		skill = &pc->skill[0];
		for (i = 0; i<MAX_NUMBER_OF_SKILL; i++, skill++)
		{
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &skill->tableIdx, COLUMNSIZE, &length); 
			if ( skill->tableIdx >= 0 ) skillNumber ++;
		}

		skill = &pc->skill[0];
		for (i = 0; i<MAX_NUMBER_OF_SKILL; i++, skill++)
		{
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &skill->iLevel, COLUMNSIZE, &length); 
		}
		
		pc->skillNumber = skillNumber;
		
		
	}

	


	
	
	for (int HotkeyIdx = 0; HotkeyIdx < MAX_NUMBER_OF_HOTKEY; HotkeyIdx++)
	{
		if ( IsValidHotKey_Skill(pc, HotkeyIdx) == FALSE )
			pc->hotkeyIdx[HotkeyIdx]=-1;
	}








	
	
	rCode = SQLCloseCursor( request->hStmt );
	if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
	{

		g_DBGameServer->PrintError( request->hStmt );
	}
	
	

}



#include "..\HelperSystem\tagHelper.h"
void DB_ResultQuery_LoadHelpList(const querySet_t& request, querySet_t& result)
{
	RETCODE			rCode= SQL_ERROR;
	long			length=0;
	int				column = 1 ;


	


	struct tagHelper* pHelper = (struct tagHelper*)GlobalAllocPtr(GMEM_FIXED,sizeof(struct tagHelper));
	if(NULL == pHelper){
		result.retCode = SQL_ERROR;
		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO ))
		{
			g_DBGameServer->PrintError( request.hStmt );
		}
		return;
	}

	result.pData = (void*)(pHelper);


	memset(pHelper,0x00,sizeof(struct tagHelper));
	pHelper->List.count = 0 ;
	pHelper->bBusy = FALSE;


	rCode = SQLFetch(request.hStmt);	
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode))
	{
		result.retCode = rCode;
		g_logSystem->WriteToError ("DB에 친구목록 없음. userID: %s",request.ownerID);
		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO ))
		{
			g_DBGameServer->PrintError( request.hStmt );
		}
		return;
	}



	
	SQLGetData (request.hStmt, column++, SQL_C_SLONG, &(pHelper->List.mode),sizeof(int),&length);			
	SQLGetData (request.hStmt, column++, SQL_C_SLONG, &(pHelper->tPoint.iMyPoint), sizeof(int), &length) ;
	SQLGetData (request.hStmt, column++, SQL_C_SLONG, &(pHelper->tPoint.iHelpPoint),sizeof(int), &length) ;
	SQLGetData (request.hStmt, column++, SQL_C_SLONG, &(pHelper->tPoint.iTotalHelpPoint), sizeof(int), &length) ;

	
	pHelper->List.count=0;
	for( int idx = 0 ; idx < tagHelper::MAX_MEMBER ; idx++)
	{
		SQLGetData (request.hStmt, column++, SQL_C_CHAR, pHelper->List.Member[idx].name,NAMESTRING, &length) ;
		pHelper->List.Member[idx].name[NAMESTRING]=NULL;

		SQLGetData (request.hStmt, column++, SQL_C_CHAR, pHelper->List.Member[idx].id, IDSTRING, &length) ;
		pHelper->List.Member[idx].id[IDSTRING]=NULL;

		if(NULL != pHelper->List.Member[idx].name[0])
		{
			pHelper->List.count++;
		}
	}	

	rCode = SQLCloseCursor( request.hStmt );
	if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
	{
		g_DBGameServer->PrintError( request.hStmt );
	}

}













void DB_ResultQuery_LoadFriends (querySet_t *request, querySet_t *result)
{
	RETCODE			rCode;
	long			length;
	int				column = 1 ;
	playerCharacter_t *pc = (playerCharacter_t *)request->retStructVariable;

	
	pc->friends.count = 0 ;
	pc->friendsBusy = false ;
	ZeroMemory ( &pc->friends.names, sizeof ( pc->friends.names ) );

	rCode = SQLFetch(request->hStmt);	
	if((rCode == SQL_NO_DATA) || (SQL_ERROR == rCode))			
	{
		result->retCode = rCode;
		g_logSystem->WriteToError ("DB에 친구목록 없음. userID: %s, name: %s",
			pc->userID, pc->name );

		rCode = SQLCloseCursor( request->hStmt );
		if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
		{

			g_DBGameServer->PrintError( request->hStmt );
		}
		return;
	}

	result->retCode = rCode;
	
	SQLGetData (request->hStmt, column++, SQL_C_SLONG, &pc->friends.count, COLUMNSIZE, &length) ;
	if ( pc->friends.count > MAX_FRIENDS)
	{
		pc->friends.count = MAX_FRIENDS ;
	}
	if ( pc->friends.count < 0 )
	{
		pc->friends.count = 0 ;
	}

	
	for ( int i = 0 ; i < pc->friends.count ; ++i, ++column )
	{
		SQLGetData (request->hStmt, column, SQL_C_CHAR, pc->friends.names[i], COLUMNSIZE, &length) ;
	}

	rCode = SQLCloseCursor( request->hStmt );
	if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
	{

		g_DBGameServer->PrintError( request->hStmt );
	}
}



void DB_ResultQuery_LoadGuild(querySet_t *request, querySet_t *result)
{

	RETCODE rCode;
	long length;
	int i, j;
	short index;
	int	column = 1 ;

	rCode = SQLFetch(request->hStmt);	
	if( (rCode == SQL_NO_DATA) || (SQL_ERROR == rCode))			
	{
		result->retCode = rCode;
		g_logSystem->WriteToError ("DB 에 길드목록 없음. 길드이름:%s(%d)", result->ownerID, result->ownerIdx );

		rCode = SQLCloseCursor( request->hStmt );
		if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
		{

			g_DBGameServer->PrintError( request->hStmt );
		}
		return;
	}
	rCode = result->retCode;

	
	for ( i = 0; rCode == SQL_SUCCESS; rCode = SQLFetch(request->hStmt), i++ )
	{
		column = 1;

		SQLGetData(request->hStmt, column++, SQL_C_USHORT, &index, COLUMNSIZE, &length);

		if( index < 0 || index > MAX_GUILDS )
		{
			
			g_logSystem->WriteToError ("서버에 저장할수 있는 길드 인덱스를 넘겼다. 길드이름:%s(%d), idx=%d", result->ownerID, result->ownerIdx, index );

			continue;
		}
		
		g_guild[index].idx = index;

		SQLGetData(request->hStmt, column++, SQL_C_UTINYINT, &g_guild[index].serverGroupID, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_CHAR, g_guild[index].name, COLUMNSIZE, &length);

		


		
		
		
		
		SQLGetData(request->hStmt, column++, SQL_C_BINARY, g_guild[index].mark, GUILDMARKSIZE, &length);

		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &g_guild[index].markUpdateTime, COLUMNSIZE, &length);

		




		SQLGetData(request->hStmt, column++, SQL_C_CHAR, g_guild[index].notice, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &g_guild[index].famePoint, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &g_guild[index].expPoint, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &g_guild[index].raidPoint, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &g_guild[index].pvpPoint, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &g_guild[index].hunterPoint, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &g_guild[index].level, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_CHAR, g_guild[index].masterName, COLUMNSIZE, &length);
		
		for( j=0; j<MAX_GUILD_SUBMASTERS; j++)
			SQLGetData(request->hStmt, column++, SQL_C_CHAR, g_guild[index].subMasterName[j], COLUMNSIZE, &length);

		for( j=0; j<MAX_GUILD_FACTIONMASTERS; j++)
			SQLGetData(request->hStmt, column++, SQL_C_CHAR, g_guild[index].factionMasterName[j], COLUMNSIZE, &length);

		for( j=0; j<MAX_GUILD_MATES; j++)
			SQLGetData(request->hStmt, column++, SQL_C_CHAR, g_guild[index].mateName[j], COLUMNSIZE, &length);

		for( j=0; j<MAX_GUILD_MATES; j++)
			SQLGetData(request->hStmt, column++, SQL_C_UTINYINT, &g_guild[i].mateAuthority[j], COLUMNSIZE, &length);

		for( j=0; j<MAX_GUILD_MATES; j++)
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &g_guild[index].mateLastConnectTime[j], COLUMNSIZE, &length);

		for( j=0; j<MAX_GUILD_MATES; j++)
			SQLGetData(request->hStmt, column++, SQL_C_CHAR, g_guild[index].masterNote[j], COLUMNSIZE, &length);

		
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &(g_guild[index].markChangeDate), COLUMNSIZE, &length);
		
	}



	rCode = SQLCloseCursor( request->hStmt );
	if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
	{

		g_DBGameServer->PrintError( request->hStmt );
	}

	if(g_config.isManager)
		if(NULL != gcpGuildCtrl) gcpGuildCtrl->InitWhenManagerServer();


}





void DB_ResultQuery_SendMail(querySet_t *request, querySet_t *result)
{
	RETCODE				rCode;
	long				length;
	int					column = 1;
	int					errorCode;			
	char				toName[NAMESTRING];
	playerCharacter_t	*pc = (playerCharacter_t *)request->retStructVariable;

	rCode = SQLFetch(request->hStmt);	
	if( (rCode == SQL_NO_DATA) || (SQL_ERROR == rCode))				
	{
		result->retCode = rCode;
		g_logSystem->WriteToError ("SendMail Query Error", result->ownerID, result->ownerIdx );

		rCode = SQLCloseCursor( request->hStmt );
		if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
		{

			g_DBGameServer->PrintError( request->hStmt );
		}
		return;
	}

	result->retCode = rCode;
	memset( toName, 0, sizeof( toName ) );
	errorCode = 0;

	SQLGetData(request->hStmt, column++, SQL_C_CHAR, toName, COLUMNSIZE, &length);
	SQLGetData(request->hStmt, column++, SQL_C_SLONG, &errorCode, COLUMNSIZE, &length);
	
	rCode = SQLCloseCursor( request->hStmt );
	if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
	{

		g_DBGameServer->PrintError( request->hStmt );
	}

	
	GTH_SendMessage_Reply_SendMail( pc, errorCode, toName );
}








void DB_ResultQuery_RecvMail(querySet_t *request, querySet_t *result)
{
	playerCharacter_t	*pc;
	RETCODE				rCode;
	long				length;
	int					column;
	int					mailCount;
	MailEntry			mail[MAX_MAIL_COUNT];
	MailEntry			*pMail;

	
	result->retCode =	SQLFetch(request->hStmt);
	rCode			=	result->retCode;
	pc = (playerCharacter_t *)request->retStructVariable;

	if( rCode == SQL_NO_DATA ) 
	{
		g_logSystem->WriteToError ("RecvMail Query Error", result->ownerID, result->ownerIdx );

		rCode = SQLCloseCursor( request->hStmt );
		if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
		{

			g_DBGameServer->PrintError( request->hStmt );
		}

		GTH_SendMessage_Reply_RecvMail( pc, 0, mail );
		return; 
	}


	mailCount = 0;
	pMail = &mail[0];

	for (; rCode == SQL_SUCCESS; rCode = SQLFetch(request->hStmt), pMail ++, mailCount ++ )
	{
		memset( pMail, 0, sizeof( MailEntry ) );

		column = 1;
		SQLGetData(request->hStmt, column++, SQL_C_UTINYINT, &pMail->mailIdx, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_CHAR,  &pMail->fromName, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_CHAR,  &pMail->mailTitle, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_CHAR,  &pMail->mailTime, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_UTINYINT,  &pMail->mailState, COLUMNSIZE, &length);
	}

	rCode = SQLCloseCursor( request->hStmt );
	if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
	{

		g_DBGameServer->PrintError( request->hStmt );
	}
	    
	GTH_SendMessage_Reply_RecvMail( pc, mailCount, mail );
}
void DB_ResultQuery_GetMailState(querySet_t *request, querySet_t *result)
{
	RETCODE				rCode;
	long				length;
	int					column = 1;
	int					notReadNumber, readNumber, s_notReadNumber, s_readNumber;
	playerCharacter_t	*pc = (playerCharacter_t *)request->retStructVariable;

	rCode = SQLFetch(request->hStmt);	
	if (rCode == SQL_NO_DATA)				
	{
		g_logSystem->WriteToError ("GetMailState Query Error", result->ownerID, result->ownerIdx );

		rCode = SQLCloseCursor( request->hStmt );
		if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
		{

			g_DBGameServer->PrintError( request->hStmt );
		}
		return;
	}

	notReadNumber = readNumber = s_notReadNumber = s_readNumber = 0;

	SQLGetData(request->hStmt, column++, SQL_C_SLONG, &notReadNumber, COLUMNSIZE, &length);
	SQLGetData(request->hStmt, column++, SQL_C_SLONG, &readNumber, COLUMNSIZE, &length);
	SQLGetData(request->hStmt, column++, SQL_C_SLONG, &s_notReadNumber, COLUMNSIZE, &length);
	SQLGetData(request->hStmt, column++, SQL_C_SLONG, &s_readNumber, COLUMNSIZE, &length);

	rCode = SQLCloseCursor( request->hStmt );
	if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
	{

		g_DBGameServer->PrintError( request->hStmt );
	}

	GTH_SendMessage_Reply_GetMailState( pc, notReadNumber, readNumber, s_notReadNumber, s_readNumber );
}
void DB_ResultQuery_DeleteMail(querySet_t *request, querySet_t *result)
{
}
void DB_ResultQuery_ViewMail(querySet_t *request, querySet_t *result)
{
	RETCODE				rCode;
	long				length;
	int					column = 1;
	MailEntry			mail;

	playerCharacter_t	*pc = (playerCharacter_t *)request->retStructVariable;

	rCode = SQLFetch(request->hStmt);	
	if (rCode == SQL_NO_DATA)				
	{
		g_logSystem->WriteToError ("GetMailState Query Error", result->ownerID, result->ownerIdx );

		rCode = SQLCloseCursor( request->hStmt );
		if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
		{

			g_DBGameServer->PrintError( request->hStmt );
		}
		return;
	}

	memset( &mail, 0, sizeof( MailEntry ) );
	
	SQLGetData(request->hStmt, column++, SQL_C_UTINYINT, &mail.mailIdx, COLUMNSIZE, &length);
	SQLGetData(request->hStmt, column++, SQL_C_CHAR, &mail.fromName, COLUMNSIZE, &length);
	SQLGetData(request->hStmt, column++, SQL_C_CHAR, &mail.mailTitle, COLUMNSIZE, &length);
	SQLGetData(request->hStmt, column++, SQL_C_CHAR, &mail.mailTime, COLUMNSIZE, &length);
	SQLGetData(request->hStmt, column++, SQL_C_CHAR, &mail.mailStr, MAIL_STRSIZE, &length);
	SQLGetData(request->hStmt, column++, SQL_C_UTINYINT, &mail.mailState, COLUMNSIZE, &length);

	rCode = SQLCloseCursor( request->hStmt );
	if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
	{

		g_DBGameServer->PrintError( request->hStmt );
	}

	GTH_SendMessage_Reply_ViewMail( pc, &mail );
}
void DB_ResultQuery_SaveMail(querySet_t *request, querySet_t *result)
{
}







void DB_Waiting_ProcessRemainderQuery()
{
	int remainderQueryNumber;
	int	delayCount;

	delayCount = 0;
	remainderQueryNumber = queryRequestQueue->m_count; 
	while ( remainderQueryNumber > 0 ) 
	{
		Sleep( 1000 );	
		delayCount ++;
		if (delayCount > 60) 
		{
			g_logSystem->Write("Can not Execute All Query in DB Thread.");
			break;
		}
		remainderQueryNumber = queryRequestQueue->m_count; 

		
		DB_ProcessQueryResultQueue();
	} 
}












void DB_SaveGameData( 
	playerCharacter_t *pc,
	int quit,
	const enum enumWhySaveGame why,
	const BOOL bForceQuit_fromGM)
{



	pc->nextSaveTime = g_globalTime + PC_GAMEDATA_SAVETIME;

	if ( quit )	
	{
		
		
		if (g_config.bCheckBilling)		
		{
			Billing_SendGameEnd (pc) ;
		}

		
		if(NULL != gcpTerraBillingCtrl){
			gcpTerraBillingCtrl->Player_GameEnd(pc);
		}
		
		
		GTH_DBSaveItemForTimeType(pc);
		
		if(TRUE == pc->GonryunBattle.Observer.bActive){
			if(TRUE == gcpGolryunBattle_Observer->Delete(pc)){
				pc->GonryunBattle.Observer.bActive=FALSE;
			}
		}
		

		
		
		if(pc->worldIdx == tagGolryunBattle::Golryun_Battle_Map_Index)
		{
			(gcpGolryunBattle->GetProcessClassPointer())->CheckQuitUser(pc);
		}
	
		
		
		
		CGonryunPractice_ProcessCtrl* pProcessCtrl = g_GonryunPracticeBattle.GetProcessCtrl();
		pProcessCtrl->IrregularQuitBattle(pc);

		g_DBAccountServer->DeleteLogin( pc );

		
		GTH_FriendsList_Request_Update ( pc, FriendsLogout );

		
		g_HelperManager.MemberList_Request_Update(pc,CHelperManager::Memberstate::HelpersLogout);
		
		

		if( pc->guildIdx != -1 )
		{
			if( g_config.isManager )
			{
				GTH_GuildUpdateUser( pc->guildIdx, pc->name, pc->rankInGuild, pc->rankIdxInGuild, GUILD_USERLOCATION, -1 );
			}
			else
			{
				GTH_SendMessage_Guild_Update_Request_To_ManagerServer( pc->guildIdx, pc->name, pc->rankInGuild, pc->rankIdxInGuild, GUILD_USERLOCATION, -1 );
			}
		}

		
		
		entityPCList_t pl;
		g_entityListMng.MakePCList( pc, &pl );
		g_entityListMng.DeletePCList( &pl );
	}

	if( pc->flagDBLoading )
	{
		g_DBGameServer->SavePremiumInfo(pc);
		g_DBGameServer->SaveQuest( pc );
		g_DBGameServer->SaveSkill( pc );


		
		g_DBGameServer->SaveHelpList(pc);
		
		
		if(TRUE == bForceQuit_fromGM) g_DBGameServer->SaveGame( pc,why,force );
		else g_DBGameServer->SaveGame( pc,why,normal);
	}
	
	else if( quit )
	{
		g_logSystem->Write( "Data Load Error!! : [%s](%s), PC was Closed before Data Loading", pc->name, pc->userID );
		GTH_RemovePC( pc );
	}
}

void DB_LoadGameData( playerCharacter_t *pc )
{
	
	
	
	g_DBGameServer->LoadPremiumInfo(pc);
	g_DBGameServer->LoadGame( pc );			
	g_DBGameServer->LoadQuest( pc );		
	g_DBGameServer->LoadQuestStatus( pc );	
	g_DBGameServer->LoadSkill( pc );		
	g_DBGameServer->LoadFriends ( pc ) ;	
	g_DBGameServer->LoadHelpList( pc);		
	g_DBGameServer->LoadItem( pc );			
	
	
	entityPCList_t pl;
	g_entityListMng.MakePCList( pc, &pl );
	g_entityListMng.AddPCList( &pl );
}


bool CheckDisconnect ( HSTMT hStmt )
{
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	SQLSMALLINT   MsgLen;
	SQLRETURN     rc ;
	int i ;

	i = 1 ;
	while ( (rc = SQLGetDiagRec ( SQL_HANDLE_STMT, hStmt, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen ) ) != SQL_NO_DATA )
	{
		++i ;
	}

	
	return !strcmp ( (char*)SqlState, "08S01" ) ;
}

























int CDBGame::SaveGonryunBattle(playerCharacter_t *pc)
{
	querySet_t	request;
	
	
	

	
    
	
	
	
	


	

	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC SaveWarRecord '%s',%d,%d,%d,%d,%d,%d,%d",
		pc->userID, 
		pc->serverGroupID, 
		pc->characterID,
		pc->GonryunBattlePractice.Record.iwin,
		pc->GonryunBattlePractice.Record.idefeat,		
		pc->GonryunBattlePractice.Record.idraw,
		pc->PremiumInfo.GonyounPracticeBattle.lDateV2,
		pc->PremiumInfo.GonyounPracticeBattle.iDecreseCount);
	m_query[QUERYSIZE]=NULL;


	request.ownerIdx = pc->idx;
	strncpy( request.ownerID, pc->userID ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	strncpy( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.queryType			=	QUERY_SAVE_GONRYUNBATTLE;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;


	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
	
}
int CDBGame::LoadGonryunBattle(playerCharacter_t *pc)
{
	querySet_t	request;

	_snprintf(m_query,QUERYSIZE, "EXEC LoadWarRecord '%s', %d, %d", pc->userID, pc->serverGroupID, pc->characterID);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_GONRYUNBATTLE;
	strncpy(request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pc;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;	
}




void	DB_ResultQuery_LoadGonryunBattle(querySet_t *request, querySet_t *result)
{
	RETCODE rCode;
	playerCharacter_t *pc;
	long length;
	int column;

	
	rCode = SQLFetch(request->hStmt);        
	if (rCode == 100) 
	{
		result->retCode = rCode;
		rCode = SQLCloseCursor( request->hStmt );
		if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
		{

			g_DBGameServer->PrintError( request->hStmt );
		}
		return; 
	}

	result->retCode = rCode;
	pc = (playerCharacter_t *)request->retStructVariable;
	

	for (; rCode == SQL_SUCCESS; rCode = SQLFetch(request->hStmt))
	{
		column = 1;	
		SQLGetData(request->hStmt, column++, SQL_C_LONG, &pc->GonryunBattlePractice.Record.iwin, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_LONG, &pc->GonryunBattlePractice.Record.idefeat, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_LONG, &pc->GonryunBattlePractice.Record.idraw, COLUMNSIZE, &length);		
		SQLGetData(request->hStmt, column++, SQL_C_LONG, &pc->PremiumInfo.GonyounPracticeBattle.lDateV2, COLUMNSIZE, &length);		
		
		
		
		
		
		
		

		SQLGetData(request->hStmt, column++, SQL_C_LONG, &pc->PremiumInfo.GonyounPracticeBattle.iDecreseCount, COLUMNSIZE, &length);	
	}

	
	rCode = SQLCloseCursor( request->hStmt );
	if( rCode == SQL_ERROR || rCode == SQL_SUCCESS_WITH_INFO )
	{
		g_DBGameServer->PrintError( request->hStmt );
	}
}






int CDBGame::SaveSkin(playerCharacter_t *pc)
{
	querySet_t	request;

	_snprintf(m_query,QUERYSIZE, "EXEC saveSkin '%s', %d, %d, %d, %d", 
		pc->userID, 
		pc->serverGroupID, 
		pc->characterID,
		pc->skinPart[0],
		pc->skinPart[1]);


	m_query[QUERYSIZE]=NULL;
	request.ownerIdx = pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_SKIN;
	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pc;
	
	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}







BOOL CDBGame::
UpdateGuildIdx(const char* pUserID,const int ServerGroupIdx,const int CharactorSlotIdx,const int GuildIdx)
{
	querySet_t request ;
	_snprintf(m_query,QUERYSIZE,"EXEC SaveDanIdx '%s',%d,%d,%d",
		pUserID,
		ServerGroupIdx,
		CharactorSlotIdx,
		GuildIdx);
			  

	
	m_query[QUERYSIZE]=NULL;
	request.ownerIdx = 0;
	strncpy(request.ownerID,"",IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_UPDATE_GUILDIDX;
	strncpy ( request.query, m_query,QUERYSIZE ) ;
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt ;
	request.retCode = -1 ;
	request.retStructVariable = NULL;

	if( !runQueryExecuterThread ) return FALSE;
	WaitForSingleObject ( hQueryRequestMutex, INFINITE ) ;
	queryRequestQueue->Push ( &request ) ;
	ReleaseMutex ( hQueryRequestMutex ) ;

	
	return TRUE;
}



int CDBGame::LoadPackageList(playerCharacter_t *pc)
{
	querySet_t	request;
	_snprintf( m_query, QUERYSIZE,
						"EXEC LoadPackageList " 
						"'%s'"					
						,pc->name );


	m_query[QUERYSIZE]=NULL;
	request.ownerIdx = pc->idx;	

	strncpy(request.ownerID, pc->userID, IDSTRING);
	request.ownerID[IDSTRING] = NULL;

	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE] = NULL;


	request.queryType			=	QUERY_LOAD_PACKAGELIST;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;

	if ( !runQueryExecuterThread ) return 0;

	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );	
	

	return 1;
}


int CDBGame::DeletePost( playerCharacter_t *pc,  int PostType, int PostIdx )
{
	querySet_t	request;

	_snprintf( m_query, QUERYSIZE,
						"EXEC DeletePackage " 
						"'%s',"					
						"%d,"					
						"%d"					
						,
						pc->name,						
						PostType,
						PostIdx);


	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy( request.ownerID, pc->userID,IDSTRING );
	request.ownerID[IDSTRING]=NULL;

	strncpy( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.queryType			=	QUERY_DELETE_POST;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;

	if ( !runQueryExecuterThread ) return 0;

	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}

int CDBGame::OpenPost( playerCharacter_t *pc, int PostType, int PostIdx )
{
	querySet_t	request;

	memset(&request,0x00,sizeof(querySet_t));

	_snprintf( m_query, QUERYSIZE,
						"EXEC LoadPackageDetail " 
						"'%s',"	
						"%d,"							
						"%d"	
						,
						pc->name,		
						PostType,
						PostIdx
						);
	m_query[ QUERYSIZE ]=NULL;


	request.ownerIdx = pc->idx;
	strncpy( request.ownerID, pc->userID ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	strncpy( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.queryType			=	QUERY_OPEN_POST;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;

	if ( !runQueryExecuterThread ) return 0;

	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
	
}



void CDBGame::DeletePostItems(const playerCharacter_t* pPlayer,const int PostSendType,const int PostIdx)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf( m_query, QUERYSIZE,
						"EXEC DeletePackageItems " 
						"'%s',"					
						"%d,"
						"%d",
						pPlayer->name,						
						PostSendType,						
						PostIdx);


	request.ownerIdx = pPlayer->idx;
	strncpy( request.ownerID, pPlayer->userID ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	strncpy( request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;


	request.queryType			=	QUERY_DELETE_POSTITEMS;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	NULL;
	request.PostSystem_DeletePackage.PostPackageIdx = PostIdx;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
}









void CDBGame::SavePackage(playerCharacter_t* pPlayer, char* title, char* message, char* toname, int nak)
{
	querySet_t request;

	_snprintf(m_query, QUERYSIZE, "EXEC SaveChaPackageList '%s','%s','%s', '%s',%d", 
		pPlayer->name,
		title,
		message,
		toname,
		nak);

	m_query[QUERYSIZE] = NULL;

	request.ownerIdx = pPlayer->idx;
	strncpy(request.ownerID, pPlayer->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVECHA_PACKAGE;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;
	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pPlayer;


	if ( !runQueryExecuterThread ) return;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );



	ReleaseMutex( hQueryRequestMutex );
	return;
}


void CDBGame::
SavePost( 
	playerCharacter_t *pPlayer, 
	const int packageidx, 
	const int itempos, 
	const char* generateID, 
	const char* toname,
	item_t* pitem )
{
	int reply = 1, error = -1;
	querySet_t request;

	
	_snprintf(m_query, QUERYSIZE, "EXEC SaveChaPackageDetail '%s', %d, %d, '%s', '%s', \
	 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \
	 %I64d, %d, %d, %d, '%s'",
	 toname, packageidx, itempos, generateID, pitem->name,
	 pitem->itemTableIdx,
	 pitem->optionNumber,
	 pitem->optionIdx[0],
	 pitem->optionIdx[1],
	 pitem->optionIdx[2],
	 pitem->optionIdx[3],
	 pitem->optionIdx[4],
	 pitem->optionIdx[5],
	 pitem->optionValue[0],
	 pitem->optionValue[1],
	 pitem->optionValue[2],
	 pitem->optionValue[3],
	 pitem->optionValue[4],
	 pitem->optionValue[5],
	 pitem->durability,
	 pitem->reqLevel,
	 pitem->experience,
	 pitem->ItemExtendInfo.Serial,
	 pitem->ItemExtendInfo.UseFlag,
	 pitem->ItemExtendInfo.AllowUseTime,
	 pitem->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType,
	 pitem->ItemExtendInfo.ItemSpawnInfomation.data);		
		


	m_query[QUERYSIZE] = NULL;

	request.ownerIdx = pPlayer->idx;
	strncpy(request.ownerID, pPlayer->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVECHA_ITEM_DETAIL;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;


	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pPlayer;


	if ( !runQueryExecuterThread ) return;

	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return;
}




void DB_ProcessQueryResultQueue_QUERY_LOAD_PREMIUMINFO(const querySet_t& result)
{
	
	
	if(result.retCode < 0) return;
	if(result.ownerIdx < 0) return;
	if(result.ownerIdx >= MAX_PCS) return;
	if(!g_pc[result.ownerIdx].active) return;
	if(0 != strcmp(result.ownerID,g_pc[result.ownerIdx].userID)) return;

	playerCharacter_t* pPlayerRecord=&g_pc[result.ownerIdx];

	gcpPremiumPlayerCtrl->SetData(result.Premium.iMemberShipType,pPlayerRecord);
	pPlayerRecord->PremiumInfo.iRemainSecond = result.Premium.iRemainSecond;
	pPlayerRecord->PremiumInfo.WorldChatting.lDate = result.Premium.WorldChatting.lDate;
	pPlayerRecord->PremiumInfo.WorldChatting.iDecreaseCount = result.Premium.WorldChatting.iDecreaseCount;
	
	pPlayerRecord->Login.Loading.bPremiumInfo=TRUE;

}


void DB_ProcessQueryResultQueue_QUERY_LOAD_PACKAGELIST(querySet_t& result)
{
	int reply = 1;
	int error = 1;
	playerCharacter_t* pPlayerRecord=gcpTools->GetPlayerRecordPointer(result.ownerIdx);
	if(NULL == pPlayerRecord)
	{
		if(NULL != result.pData){
			GlobalFreePtr(result.pData); 
			result.pData=NULL; 			
		}		
		return;
	}


	CPostManager::PostPackage_t* pPostPackage = NULL;
	pPostPackage = 	(CPostManager::PostPackage_t*)result.pData;

	
	if ( NULL == pPostPackage )
	{
		CPostFunc::GTH_SendMessage_PostList(pPlayerRecord, 
			tagGCPacket_POSTSYSTEM_OPEN::enumCode::success);
		return;
	}	
	
	pPlayerRecord->m_PostMng.InitPostSystem();

	for (int idx = 0; idx < CPostManager::MAX_POSTPACKAGE_COUNT; idx++)
	{
		if ( FALSE == pPostPackage[idx].bActive )
			continue;

		pPlayerRecord->m_PostMng.AddPostPackage(&pPostPackage[idx]);		
	}

	if(NULL != result.pData){ GlobalFreePtr(result.pData); result.pData=NULL; }

	
	pPlayerRecord->m_PostMng.SetPostSystem(CPostManager::enumPostSystemMode::POSTSYSTEM_READMODE);
	CPostFunc::GTH_SendMessage_PostList(pPlayerRecord, tagGCPacket_POSTSYSTEM_OPEN::enumCode::success);

}

void DB_ProcessQueryResultQueue_QUERY_DELETE_POSTITEMS(const querySet_t& result)
{	
	playerCharacter_t* pPlayerRecord=gcpTools->GetPlayerRecordPointer(result);
	if(NULL == pPlayerRecord) return;

	if(result.retCode < 0){
		g_PostFunc.GTH_ProcessMessage_MailBox_AddItemToPlayer(pPlayerRecord,result.PostSystem_DeletePackage.PostPackageIdx,FALSE);		
	}else{
		g_PostFunc.GTH_ProcessMessage_MailBox_AddItemToPlayer(pPlayerRecord,result.PostSystem_DeletePackage.PostPackageIdx,TRUE);		
	}

	
}

void DB_ProcessQueryResultQueue_QUERY_DELETE_POST(querySet_t& result)
{
	playerCharacter_t* pPlayerRecord=gcpTools->GetPlayerRecordPointer(result.ownerIdx);
	if(NULL == pPlayerRecord) return;
	
	int IsDeleteAllow = TRUE;
	int erroridx =0;
	if ( result.retCode < 0)
	{
		g_logSystem->Write("DeletePost Error!! : (ID: %s)", pPlayerRecord->userID);		
		IsDeleteAllow = FALSE;
		erroridx = 3;
	}





	pPlayerRecord->m_PostMng.InitPostSystem();
	pPlayerRecord->m_PostMng.SetPostSystem(CPostManager::enumPostSystemMode::POSTSYSTEM_READMODE);	


	g_DBGameServer->LoadPackageList(pPlayerRecord);
	
}



void DB_ResultQuery_LoadWebPackageList(const querySet_t& request,querySet_t& result)
{
	static RETCODE			rCode;
	static long				length;	
	static unsigned char Type = 0;
	static int Idx;
	static CPostManager::PostPackage_t* pPostPackage=NULL;
	const int MaxCount = CPostManager::MAX_POSTPACKAGE_COUNT;


	result.pData=NULL;

	
	rCode= SQLFetch(request.hStmt);	
	if((rCode == SQL_NO_DATA) || (rCode == SQL_ERROR)){
		result.retCode = rCode;
		goto safe_end;
	}
	
	result.retCode = rCode;
	pPostPackage = (CPostManager::PostPackage_t*)GlobalAllocPtr(GMEM_FIXED,sizeof(CPostManager::PostPackage_t) * CPostManager::MAX_POSTPACKAGE_COUNT);
	if(NULL == pPostPackage) goto safe_end;

	for(Idx=0; Idx < CPostManager::MAX_POSTPACKAGE_COUNT; Idx++) 
		CPostManager::InitPost(&(pPostPackage[Idx]));	

	

	
	for(Idx = 0; (rCode == SQL_SUCCESS) && (Idx < MaxCount); rCode = SQLFetch(request.hStmt), Idx++)
	{	
		CPostManager::PostPackage_t* pTempPostPackage = &(pPostPackage[Idx]);	
		
		
		
		SQLGetData(request.hStmt,1, SQL_INTEGER, &(pTempPostPackage->iPostIdx),sizeof(int), &length);			
		
		
		
		SQLGetData(request.hStmt, 2, SQL_C_BIT,	&Type, sizeof(unsigned char), &length);
		if ( Type == 0)
			pTempPostPackage->PostSendType = CPostManager::enumPostSendType::POST_SENDTYPE_WEB;
		else 
			pTempPostPackage->PostSendType = CPostManager::enumPostSendType::POST_SENDTYPE_CHAR;

		
		
		SQLGetData(request.hStmt, 3, SQL_C_CHAR, pTempPostPackage->szPostTitle, CPostManager::POST_TITLESIZE, &length);
		pTempPostPackage->szPostTitle[CPostManager::POST_TITLESIZE]=NULL;


		
		SQLGetData(request.hStmt, 4, SQL_C_CHAR, pTempPostPackage->szMailStr, 	CPostManager::POST_STRSIZE, &length);
		pTempPostPackage->szMailStr[CPostManager::POST_STRSIZE] = NULL;


		
		SQLGetData(request.hStmt, 5, SQL_C_CHAR, pTempPostPackage->szFromName, NAMESTRING, &length);
		pTempPostPackage->szFromName[NAMESTRING]=NULL;
	
		
		SQLGetData(request.hStmt, 6, SQL_C_BIT, &(pTempPostPackage->MailState),sizeof(int), &length);

		
		SQLGetData(request.hStmt, 7, SQL_INTEGER, &(pTempPostPackage->iRemainDays), sizeof(int), &length);

		
		SQLGetData(request.hStmt, 8, SQL_C_CHAR, pTempPostPackage->szSendPostTime, CPostManager::POST_SENDTIME_INFO_STR_LEN, &length);
		pTempPostPackage->szSendPostTime[CPostManager::POST_SENDTIME_INFO_STR_LEN]=NULL;

		
		SQLGetData(request.hStmt, 9, SQL_INTEGER, &pTempPostPackage->Nak,  sizeof(int), &length);	

		pTempPostPackage->bActive = TRUE;
		

	}	
	result.pData = (void*)pPostPackage;



safe_end:
	rCode = SQLCloseCursor( request.hStmt );	
	if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
	{
		g_DBGameServer->PrintError( request.hStmt );
	}
	return;
}	



















void CDBGame::
SaveChaPackageListBySystem(
	const char* pFromName,
	const char* pToName,
	const char* title, 
	const char* message, 
	const int   nak)
{
	querySet_t request;

	_snprintf(m_query, QUERYSIZE, "EXEC SaveChaPackageList '%s','%s','%s','%s',%d", 
		pFromName,
		title,
		message,
		pToName,
		nak);

	m_query[QUERYSIZE] = NULL;

	request.ownerIdx = 0;
	strncpy(request.ownerID,"",IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVECHAPACKAGELIST_BY_SYTEM;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;


	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;


	if ( !runQueryExecuterThread ) return;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return;
}



int CDBGame::SaveChaPackageNak(playerCharacter_t *pc, CPostManager::PostPackage_t* pPostPackage)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf( m_query, QUERYSIZE,
						"EXEC SaveChaPackageNak " 
						"'%s', %d, %d, %d"					
						,pc->name,
						(int)pPostPackage->PostSendType,
						pPostPackage->iPostIdx,
						pPostPackage->Nak);


	m_query[QUERYSIZE]=NULL;
	
	request.ownerIdx = pc->idx;	

	strncpy(request.ownerID, pc->userID, IDSTRING);
	request.ownerID[IDSTRING] = NULL;

	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE] = NULL;


	request.queryType			=	QUERY_SAVECHA_PACKAGENAK;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pc;

	if ( !runQueryExecuterThread ) return 0;

	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );	
	

	return 1;
}



BOOL CDBGame::PostSystem_LoadPackageCnt(playerCharacter_t* pPc)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));
	_snprintf( m_query, QUERYSIZE,"EXEC LoadPackageCnt '%s'",pPc->name);

	m_query[QUERYSIZE]=NULL;
	request.ownerIdx = pPc->idx;	
	strncpy(request.ownerID, pPc->userID, IDSTRING);
	request.ownerID[IDSTRING] = NULL;
	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE] = NULL;
	request.queryType			=	QUERY_POSTSYSTEM_LOADPACKAGECNT;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pPc;

	if( !runQueryExecuterThread ) return FALSE;
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );		

	return TRUE;
}


void CDBGame::GolryunBattle_SaveBattleInfo(
	playerCharacter_t* pPlayer,
	const int iBattleGUID,
	const int TournamentLevelIdx,		
	enum playerCharacter_t::enumNameColor enumManor)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));
	_snprintf( m_query, QUERYSIZE,"EXEC SaveBattleInfo '%s',%d,%d,%d,%d,%d",
		pPlayer->userID,
		pPlayer->serverGroupID, 
		pPlayer->characterID,
		iBattleGUID,
		TournamentLevelIdx,
		enumManor);

	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pPlayer->idx;	
	strncpy(request.ownerID, pPlayer->userID, IDSTRING);
	request.ownerID[IDSTRING] = NULL;

	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE] = NULL;

	request.queryType			=	QUERY_GolryunBattle_SaveBattleInfo;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	(void *)pPlayer;

	if( !runQueryExecuterThread ) return;
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );		

	return;
}




void DB_ProcessQueryResultQueue_QUERY_INSERT_LOGIN(const querySet_t& result)
{
	
	
	if(result.ownerIdx < 0) return;
	if(result.ownerIdx >= MAX_PCS) return;
	if(0 != strcmp(result.ownerID,g_pc[result.ownerIdx].userID)) return;

	playerCharacter_t* pPlayer=&g_pc[result.ownerIdx];	
	if( !pPlayer->active ){
		g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_INSERT_LOGIN, ownerIdx: %d)", result.ownerIdx);
		return;
	}

	
	if(result.retCode < 0){
		g_logSystem->Write("Duplicated Login Error!! : (ID: %s)", pPlayer->userID);
		return;
	}
	DB_LoadGameData( pPlayer);
}







void DB_ProcessQueryResultQueue_QUERY_LOAD_HELPLIST(const querySet_t& result);
void DB_ProcessQueryResultQueue_QUERY_GIFTCARD_SaveGiftCard(const querySet_t& result);
void DB_ProcessQueryResultQueue_QUERY_GIFTCARD_LoadGiftCard(const querySet_t& result);
void DB_ProcessQueryResultQueue_QUERY_GIFTCARD_LoadGiftCardItembytype(const querySet_t& result);
void DB_ProcessQueryResultQueue_QUERY_LOAD_CHARACTER(const querySet_t& result);
void DB_ProcessQueryResultQueue_QUERY_LOAD_QUEST(const querySet_t& result);
void DB_ProcessQueryResultQueue_QUERY_LOAD_QUESTSTATUS(const querySet_t& result);

void DB_ProcessQueryResultQueue_QUERY_SAVESERVERSTATUS(const querySet_t& result);


void DB_ProcessQueryResultQueue(void)
{
	static querySet_t			result;
	static playerCharacter_t	*pc;
	int					isExist;
	static int					processedQueryCount;

	if ( !runQueryExecuterThread ) return;

	processedQueryCount = 0;



	while ( processedQueryCount < MAX_QUERY_PER_FRAME ){		
		
		WaitForSingleObject( hQueryResultMutex, INFINITE );
		if (queryResultQueue->m_count > 0)
		{
			
			isExist = queryResultQueue->Pop( &result );
		}
		else
		{
			isExist = 0;
		}

		ReleaseMutex( hQueryResultMutex );

		if ( !isExist ) break;

		processedQueryCount ++;
		
		
		if( (result.ownerIdx < 0) || (result.queryType < 0) || (result.queryType >= NUM_QUERY_TYPE) || (result.ownerIdx >= MAX_PCS)){


			switch(result.queryType){
				case QUERY_SAVESERVERSTATUS:
					DB_ProcessQueryResultQueue_QUERY_SAVESERVERSTATUS(result);
					break;

				case QUERY_LOADSERVERSTATUS:
					assert(NULL != gcpGMCtrl);
					assert(NULL != gcpGMCtrl->GetServerStateDataCtrlPointer());

					(gcpGMCtrl->GetServerStateDataCtrlPointer())->ODBC_ProcessQueryResultQueue_QUERY_LOADSERVERSTATUS(result);
					break;


					
					


				case Query_GolryunBattle_LoadLastBattle:
					(gcpGolryunBattle->GetDBV2ClassPointer())->ODBC_ProcessQueryResultQueue_Query_GolryunBattle_LoadLastBattle(result);
					break;

				case Query_GolryunBattle_AddBattle:
					(gcpGolryunBattle->GetDBV2ClassPointer())->ODBC_ProcessQueryResultQueue_Query_GolryunBattle_AddBattle(result);
					break;

				case Query_GolryunBattle_LoadTournament:										
					(gcpGolryunBattle->GetDBV2ClassPointer())->ODBC_ProcessQueryResultQueue_Query_GolryunBattle_LoadTournament(result);
					break;

				case Query_GolryunBattle_SaveBattle:					
					(gcpGolryunBattle->GetDBV2ClassPointer())->ODBC_ProcessQueryResultQueue_Query_GolryunBattle_SaveBattle(result);
					break;

				case Query_GolryunBattle_AddTournament:
					(gcpGolryunBattle->GetDBV2ClassPointer())->ODBC_ProcessQueryResultQueue_Query_GolryunBattle_AddTournament(result);
					break;

				case Query_GolryunBattle_SaveTournament:					
					(gcpGolryunBattle->GetDBV2ClassPointer())->ODBC_ProcessQueryResultQueue_Query_GolryunBattle_SaveTournament(result);
					break;	
					
					





			}
			
			continue;
		}




		switch ( result.queryType )
		{
			
		case QUERY_SAVE_DANBATTLE_GUILD_ADD_WINPOINT:
			if( result.retCode < 0){
				g_logSystem->Write("Database Error: QUERY_SAVE_DANBATTLE_GUILD_ADD_WINPOINT query: %s",result.query);
			}
			break;
		case QUERY_SAVE_DANBATTLE_GUILD_ADD_LOSEPOINT:
			if( result.retCode < 0){
				g_logSystem->Write("Database Error: QUERY_SAVE_DANBATTLE_GUILD_ADD_LOSEPOINT query: %s",result.query);
			}
			break;
		case QUERY_SAVE_DANBATTLE_GUILD_ADD_ACCPOINT:
			if( result.retCode < 0){
				g_logSystem->Write("Database Error: QUERY_SAVE_DANBATTLE_GUILD_ADD_ACCPOINT query: %s",result.query);
			}
			break;
		case QUERY_LOAD_DANBATTLE_GUILD_INFO:
			DB_ProcessQueryResultQueue_LOAD_DANBATTLE_GUILD_INFO(result);			
			break;
		case QUERY_SAVE_DANBATTLE_PCPOINT:
			if( result.retCode < 0){
				g_logSystem->Write("Database Error: QUERY_SAVE_DANBATTLE_PCPOINT query: %s",result.query);
			}
			break;
		

		case QUERY_INSERT_DANBATTLE:
			DB_ProcessQueryResultQueue_QUERY_INSERT_DANBATTLE(result);
			break;

		case QUERY_DELETE_ALLDANBATTLE:
			
			break;

		case QUERY_DELETE_DANBATTLE:
			
			break;

		case QUERY_GIFTCARD_SaveGiftCard:
			DB_ProcessQueryResultQueue_QUERY_GIFTCARD_SaveGiftCard(result);
			break;


		case QUERY_GIFTCARD_LoadGiftCardItembytype:
			DB_ProcessQueryResultQueue_QUERY_GIFTCARD_LoadGiftCardItembytype(result);
			break;

		case QUERY_GIFTCARD_LoadGiftCard:
			DB_ProcessQueryResultQueue_QUERY_GIFTCARD_LoadGiftCard(result);
			break;


		case QUERY_GolryunBattle_SaveBattleInfo:
			Sleep(0);
			break;
		
		case QUERY_OPEN_POST:
			DB_ProcessQueryResultQueue_QUERY_OPEN_POST(result);
			break;

		case QUERY_POSTSYSTEM_LOADPACKAGECNT:
			DB_ProcessQueryResultQueue_QUERY_POSTSYSTEM_LOADPACKAGECNT(result);
			break;

			
		case QUERY_SAVECHA_PACKAGENAK:
			if( result.retCode < 0){
				g_logSystem->Write("Database Error: QUERY_SAVECHA_PACKAGENAK query: %s",result.query);
			}
			break;

		case QUERY_SAVECHAPACKAGELIST_BY_SYTEM:
			if( result.retCode < 0){
				g_logSystem->Write("Database Error: QUERY_SAVECHAPACKAGELIST_BY_SYTEM query: %s",result.query);
			}
			break;

		case QUERY_SAVE_PREMIUMINFO:

			if( result.retCode < 0){
				g_logSystem->Write("Database Error: QUERY_SAVE_PREMIUMINFO, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
			}
			break;

		case QUERY_LOAD_PREMIUMINFO:
			DB_ProcessQueryResultQueue_QUERY_LOAD_PREMIUMINFO(result);
			break;

			
		case QUERY_LOAD_PACKAGELIST:
			DB_ProcessQueryResultQueue_QUERY_LOAD_PACKAGELIST(result);
			break;
			
		case QUERY_DELETE_POSTITEMS:
			DB_ProcessQueryResultQueue_QUERY_DELETE_POSTITEMS(result);
			break;
			
		case QUERY_DELETE_POST:
			DB_ProcessQueryResultQueue_QUERY_DELETE_POST(result);
			break;
			


			
		case QUERY_SAVECHA_PACKAGE:
			DB_ProcessQueryResultQueue_QUERY_SAVECHA_PACKAGE(result);
			break;			

		case QUERY_SAVECHA_ITEM_DETAIL:
			DB_ProcessQueryResultQueue_QUERY_SAVECHA_PACKAGE_DETAIL(result);
			break;
		
			
		case QUERY_INSERT_LOGIN:
			DB_ProcessQueryResultQueue_QUERY_INSERT_LOGIN(result);
			break;
			

		case QUERY_DELETE_LOGIN:
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_DELETE_LOGIN, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_DELETE_LOGIN, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_DELETE_LOGIN, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			break;

		case QUERY_DELETE_LOGIN_HOST :
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_DELETE_LOGIN_HOST, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			break;

		
		case QUERY_LOAD_FRIENDS:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_LOAD_FRIENDS, ownerID: %s, query: %s", result.ownerID, result.query ) ;
			}
			break ;

		case QUERY_LOAD_CHARACTER:
			DB_ProcessQueryResultQueue_QUERY_LOAD_CHARACTER(result);
			break;

			

		case QUERY_LOAD_ITEM:
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_LOAD_ITEM, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_LOAD_ITEM, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_LOAD_ITEM, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			GTH_ReplyConnectGameServer( pc );
			
			
			
			break;

		case QUERY_LOAD_QUEST :	
			DB_ProcessQueryResultQueue_QUERY_LOAD_QUEST(result);
			break;
			

		case QUERY_LOAD_QUESTSTATUS :
			DB_ProcessQueryResultQueue_QUERY_LOAD_QUESTSTATUS(result);
			break;

			

		case QUERY_LOAD_SKILL:
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_LOAD_SKILL, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_LOAD_SKILL, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_LOAD_SKILL, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			break;

		case QUERY_SAVE_GAME:
			DB_ProcessQueryResultQueue_QUERY_SAVE_GAME(result);
			break;			
			

		case QUERY_SAVE_QUEST :
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_SAVE_QUEST, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_SAVE_QUEST, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_SAVE_QUEST, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			break;

		case QUERY_SAVE_SKILL:
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_SAVE_SKILL, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_SAVE_SKILL, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_SAVE_SKILL, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			break;

		case QUERY_SAVE_ITEM:
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_SAVE_ITEM, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_SAVE_ITEM, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_SAVE_ITEM, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			break;

		case QUERY_SAVE_FRIENDS:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_SAVE_FRIENDS, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break ;

		case QUERY_ADD_QUESTSTATUS :	
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_ADD_QUESTSTATUS, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_ADD_QUESTSTATUS, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_ADD_QUESTSTATUS, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			break;

		case QUERY_REMOVE_QUESTSTATUS :	
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_REMOVE_QUESTSTATUS, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_REMOVE_QUESTSTATUS, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_REMOVE_QUESTSTATUS, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			break;

		case QUERY_SAVE_QUESTSTATUS :	
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_SAVE_QUESTSTATUS, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_SAVE_QUESTSTATUS, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_SAVE_QUESTSTATUS, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			break;

		case QUERY_ADD_ITEM:
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_ADD_ITEM, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_ADD_ITEM, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_ADD_ITEM, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			break;

		case QUERY_REMOVE_ITEM:

			if ( result.retCode < 0)
			{

				g_logSystem->Write("Database Error: QUERY_REMOVE_ITEM, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_REMOVE_ITEM, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_REMOVE_ITEM, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			break;

		
		case QUERY_ADD_GUILD:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_ADD_GUILD, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;

		case QUERY_LOAD_GUILD:

			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_LOAD_GUILD, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;

		
		case QUERY_UPDATE_GUILDIDX:
			if(result.retCode < 0 ){
				g_logSystem->Write ( "Database Error: QUERY_UPDATE_GUILDIDX, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;
		


		case QUERY_SAVE_GUILD:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_SAVE_GUILD, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;

		case QUERY_BANISHMEMBER_GUILD:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_BANISHMEMBER_GUILD, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;

		
		
		case QUERY_SEND_MAIL:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_SEND_MAIL, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;
		case QUERY_RECV_MAIL:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_RECV_MAIL, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;
		case QUERY_GET_MAILSTATE:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_GET_MAILSTATE, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;
		case QUERY_DELETE_MAIL:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_DELETE_MAIL, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;
		case QUERY_VIEW_MAIL:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_VIEW_MAIL, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;
		case QUERY_SAVE_MAIL:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_SAVE_MAIL, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;


			


			

		
		case QUERY_SAVE_SKIN :
			{
				if ( result.retCode < 0 ){
					g_logSystem->Write ( "Database Error: QUERY_SAVE_SKIN, ownerIdx: %d, query: %s", result.ownerID, result.query );
				}		
			}
			break;
		

		
		case QUERY_SAVE_EVENTCRAFTITEM:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_SAVE_EVENTCRAFTITEM, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;

		case QUERY_LOAD_EVENTCRAFTITEM:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_LOAD_EVENTCRAFTITEM, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;
		

		
		case QUERY_LOAD_GONRYUNBATTLE:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_LOAD_GONRYUNBATTLE, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;
		case QUERY_SAVE_GONRYUNBATTLE:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_SAVE_GONRYUNBATTLE, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;
		

		
		case QUERY_SAVE_HELPLIST:
			if(result.retCode < 0)
			{
				g_logSystem->Write ( "Database Error: QUERY_SAVE_HELPLIST, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;

		case QUERY_LOAD_HELPLIST:
			DB_ProcessQueryResultQueue_QUERY_LOAD_HELPLIST(result);
			break;
		
		case QUERY_SAVE_WEBPOINT:
			if(result.retCode < 0)
			{
				g_logSystem->Write ( "Database Error: QUERY_SAVE_WEBPOINT, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			break;



			
		default:
			break;
		}

	} 
}



void DB_ProcessQueryResultQueue_QUERY_LOAD_HELPLIST(const querySet_t& result)
{
	struct tagHelper* pHelper = (struct tagHelper*)result.pData;

	if((result.ownerIdx < 0) ||
	   (result.ownerIdx >= MAX_PCS)){

		if(NULL != pHelper){ GlobalFreePtr(pHelper); pHelper=NULL; }
		
		return;
	}

	playerCharacter_t* pPlayer=&g_pc[result.ownerIdx];
	if(FALSE == pPlayer->active) 
	{
		if(NULL != pHelper){ GlobalFreePtr(pHelper); pHelper=NULL; }
		
		return;
	}
	
	if(result.retCode < 0 )
	{
		g_logSystem->Write ( "Database Error: QUERY_LOAD_HELPLIST, ownerID: %s, query: %s", pPlayer->userID,result.query ) ;
		if(NULL != pHelper){ GlobalFreePtr(pHelper); pHelper=NULL; }
		return;		
	}

	pPlayer->tHelper = *pHelper;

	if(31 <= pPlayer->level)
	{
		if(tagHelper::tagMemberList::enumMode::TAKER==pPlayer->tHelper.List.mode)
		{
			if(0==pPlayer->tHelper.List.count)
			{
				pPlayer->tHelper.List.mode=tagHelper::tagMemberList::enumMode::HELPER;
			}

			else{
				pPlayer->tHelper.List.mode=tagHelper::tagMemberList::enumMode::TAKER;
			}
		}
	}

	else if(31 > pPlayer->level)
	{
		pPlayer->tHelper.List.mode=tagHelper::tagMemberList::enumMode::TAKER;
	}

	if(NULL != pHelper){ GlobalFreePtr(pHelper); pHelper=NULL; }
}







void DB_QueryExecuterThread_QUERY_GIFTCARD_LoadGiftCardItembytype(const querySet_t& request,querySet_t& result)
{
	RETCODE				rCode=SQL_ERROR;
	int					idx=0;


	result.GiftCardItemList.pRecord=NULL;
	result.GiftCardItemList.Cnt=0;


	
	rCode= SQLFetch(request.hStmt);	
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode)){		
		result.retCode=rCode;

		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
		{
			g_DBGameServer->PrintError( request.hStmt );
		}
		return;	
	}
	result.retCode = rCode;	

	struct tagQuerySet::tagGiftCardItemList::tagRecord* pRecord = 
		(struct tagQuerySet::tagGiftCardItemList::tagRecord*)GlobalAllocPtr(
			GMEM_FIXED,
			sizeof(struct tagQuerySet::tagGiftCardItemList::tagRecord) * request.GiftCard.Cnt);

	if(NULL == pRecord){
		result.retCode = SQL_ERROR;
		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
		{
			g_DBGameServer->PrintError( request.hStmt );
		}
		return;			
	}



	
	for(idx=0; idx < request.GiftCard.Cnt; idx++){
		pRecord[idx].ItemTableIdx=-1;
		pRecord[idx].Cnt=0;
		pRecord[idx].Rate=0;
	}


	
	struct tagQuerySet::tagGiftCardItemList::tagRecord* pTempListRecord=NULL; 
	short int type=0;
	long length=0;

	for(idx = 0; (rCode == SQL_SUCCESS) && (idx < request.GiftCard.Cnt); rCode = SQLFetch(request.hStmt), idx++)
	{
		pTempListRecord = &pRecord[idx];		
		SQLGetData(request.hStmt, 1, SQL_C_LONG, &(pTempListRecord->ItemTableIdx),sizeof(int), &length);
		SQLGetData(request.hStmt, 2, SQL_C_LONG, &type,sizeof(int), &length);
		SQLGetData(request.hStmt, 3, SQL_C_LONG, &(pTempListRecord->Cnt),sizeof(int), &length);
		SQLGetData(request.hStmt, 4, SQL_C_LONG, &(pTempListRecord->Rate),sizeof(int), &length);
	}

	result.GiftCardItemList.pRecord = pRecord;
	result.GiftCardItemList.Cnt		= request.GiftCard.Cnt;


	

	rCode = SQLCloseCursor( request.hStmt );
	if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
	{
		g_DBGameServer->PrintError( request.hStmt );
	}
	return;
}





void DB_QueryExecuterThread_QUERY_GIFTCARD_LoadGiftCard(const querySet_t& request,querySet_t& result)
{
	RETCODE				rCode=SQL_ERROR;
	long				    length=0;
	

	result.GiftCard.code = CGiftCard::enumCode::unknown;	
	rCode= SQLFetch(request.hStmt);	
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode)){		
		result.retCode=rCode;

		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
		{
			g_DBGameServer->PrintError( request.hStmt );
		}
		return;	
	}
	result.retCode = rCode;		
		
	
	SQLGetData(request.hStmt, 1, SQL_C_SLONG, &rCode,sizeof(int), &length);	
	switch(rCode){
		case 0:
			result.GiftCard.code = CGiftCard::enumCode::success;
			break;
		case 1:
			result.GiftCard.code = CGiftCard::enumCode::can_not_find;
			break;
		case 2:
			result.GiftCard.code = CGiftCard::enumCode::password_not_match;
			break;

		case 3:
			result.GiftCard.code = CGiftCard::enumCode::pre_use;
			break;
	};

	SQLGetData(request.hStmt, 2, SQL_C_SLONG, &(result.GiftCard.type), sizeof(int), &length); 	
	SQLGetData(request.hStmt, 3, SQL_C_SLONG, &(result.GiftCard.Cnt), sizeof(int), &length); 	
	

	rCode = SQLCloseCursor( request.hStmt );
	if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
	{
		g_DBGameServer->PrintError( request.hStmt );
	}
	return;
}


void CDBGame::LoadGiftCard(const playerCharacter_t* pPlayer)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf((char*)m_query, QUERYSIZE,"EXEC LoadGiftCard '%s','%s'",
		pPlayer->GiftCard.serial,
		pPlayer->GiftCard.password);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pPlayer->idx;
	strncpy(request.ownerID, pPlayer->userID, IDSTRING);
	request.ownerID[IDSTRING] = NULL;

	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE] = NULL;

	request.queryType			=	QUERY_GIFTCARD_LoadGiftCard;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	NULL;

	if( !runQueryExecuterThread ) return;
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );	

	return;
}


#include "..\Terra\GiftCard\CGiftCard.h"
extern CGiftCard* gcpGiftCard;






void DB_ProcessQueryResultQueue_QUERY_GIFTCARD_LoadGiftCard(const querySet_t& result)
{
	playerCharacter_t* pPlayer = gcpTools->GetPlayerRecordPointer(result);
	if(NULL == pPlayer) return;

	if(result.GiftCard.code != CGiftCard::enumCode::success){
		gcpGiftCard->TransmitePacket_SC_Notify(pPlayer,result.GiftCard.code);

		pPlayer->GiftCard.step = playerCharacter_t::tagGiftCard::step_none;
		return;
	}

	g_DBGameServer->LoadGiftCardItembytype(pPlayer,result.GiftCard.type,result.GiftCard.Cnt);
}






void DB_ProcessQueryResultQueue_QUERY_GIFTCARD_LoadGiftCardItembytype(const querySet_t& result)
{

	struct tagQuerySet::tagGiftCardItemList::tagRecord* pRecord = 
		(struct tagQuerySet::tagGiftCardItemList::tagRecord*)result.GiftCardItemList.pRecord;




	playerCharacter_t* pPlayer = gcpTools->GetPlayerRecordPointer(result);
	if(NULL == pPlayer){
		if(NULL != pRecord){ GlobalFreePtr(pRecord); pRecord=NULL; }		
		return;
	}


	if(result.retCode != SQL_SUCCESS){
		if(NULL != pRecord){ GlobalFreePtr(pRecord); pRecord=NULL; }		
		gcpGiftCard->TransmitePacket_SC_Notify(pPlayer,CGiftCard::enumCode::unknown);
		pPlayer->GiftCard.step = playerCharacter_t::tagGiftCard::step_none;
		return;
	}


	
	
	float  RandomValue=GTH_Random() * 1000.;
	BOOL   bRandomDropItem=FALSE;
	int    Sum=0;
	item_t item;	
	struct tagQuerySet::tagGiftCardItemList::tagRecord* pTempRecord = NULL;
	const int maxbytes_receiveitemlist=99;
	char      ReceiveItemList[maxbytes_receiveitemlist+1]="";
	const int maxbytes_temp=9;
	char      temp[maxbytes_temp+1];

	
	for(int idx=0; idx < result.GiftCardItemList.Cnt; idx++)
	{
		pTempRecord = &pRecord[idx];

		if(pTempRecord->ItemTableIdx < 0) continue;
		if(pTempRecord->Rate >= 1000){
			if( !SpawnItem(&item,
				           pTempRecord->ItemTableIdx,
				           GTH_RandomizePower( 0, 3 ),
						   GTH_RandomizePower( 1, 2),0)) continue;

			if(TRUE == GTH_IsStackItem(&item)){
				item.durability = pTempRecord->Cnt-1;
				if(item.durability < 0) item.durability=0;

			}
			
			if(TRUE == gcpTools->AddItem(pPlayer,item)){
				gcpGiftCard->TransmitePacket_SC_AddItem(pPlayer,item);				

				if( (maxbytes_receiveitemlist - strlen(ReceiveItemList)) > (maxbytes_temp+1)){
					if(NULL == ReceiveItemList[0]) _snprintf(temp,maxbytes_temp,"%d",item.itemTableIdx);
					else _snprintf(temp,maxbytes_temp,",%d",item.itemTableIdx);
					temp[maxbytes_temp]=NULL;
					strcat(ReceiveItemList,temp);
				}

			}	
			continue;
		}
		if(TRUE == bRandomDropItem)  continue;


		if((RandomValue >= Sum) && (RandomValue <= Sum + pTempRecord->Rate)){
			if( !SpawnItem(&item,
				           pTempRecord->ItemTableIdx,
						   GTH_RandomizePower( 0, 3 ),
						   GTH_RandomizePower( 1, 2),0)) continue;

			if(TRUE == GTH_IsStackItem(&item)){
				item.durability = pTempRecord->Cnt-1;
				if(item.durability < 0) item.durability=0;
			}

			if(TRUE == gcpTools->AddItem(pPlayer,item)){
				gcpGiftCard->TransmitePacket_SC_AddItem(pPlayer,item);				
				bRandomDropItem=TRUE;

				if( (maxbytes_receiveitemlist - strlen(ReceiveItemList)) > (maxbytes_temp+1)){
					if(NULL == ReceiveItemList[0]) _snprintf(temp,maxbytes_temp,"%d",item.itemTableIdx);
					else _snprintf(temp,maxbytes_temp,",%d",item.itemTableIdx);
					temp[maxbytes_temp]=NULL;
					strcat(ReceiveItemList,temp);
				}

			}
		}
		Sum += pTempRecord->Rate;
	}

	gcpGiftCard->TransmitePacket_SC_Close(pPlayer);	
	g_DBGameServer->SaveGiftCard(pPlayer->GiftCard.serial,ReceiveItemList,pPlayer->userID);


	pPlayer->GiftCard.step = playerCharacter_t::tagGiftCard::step_none;


	if(pPlayer->busyState == BUSY_STATE_NPC){
		pPlayer->busyState = BUSY_STATE_NONE;
		monsterCharacter_t* pNPC=gcpTools->GetNPCRecordPointer(pPlayer->npcIdx);
		if(NULL != pNPC){			
			NPC_DeleteNoticePC(pNPC,pPlayer);
		}
	}

	
	if(NULL != pRecord){ GlobalFreePtr(pRecord); pRecord=NULL; }		
}





void CDBGame::LoadGiftCardItembytype(const playerCharacter_t* pPlayer,const int type,const int Cnt)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf((char*)m_query, QUERYSIZE,"EXEC LoadGiftCardItembytype %d",type);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pPlayer->idx;
	strncpy(request.ownerID, pPlayer->userID, IDSTRING);
	request.ownerID[IDSTRING] = NULL;

	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE] = NULL;

	request.queryType			=	QUERY_GIFTCARD_LoadGiftCardItembytype;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	NULL;
	request.GiftCard.Cnt		=   Cnt;

	if( !runQueryExecuterThread ) return;
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );	

	return;
}



void CDBGame::SaveGiftCard(
	const char* szpGiftCardSerial,
	const char* szpGiveItemList,
	const char* szpPlayerAccountID)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf((char*)m_query, QUERYSIZE,"EXEC SaveGiftCard '%s','%s','%s'",
		szpGiftCardSerial,
		szpGiveItemList,
		szpPlayerAccountID);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = -1;
	strncpy(request.ownerID,"", IDSTRING);
	request.ownerID[IDSTRING] = NULL;

	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE] = NULL;

	request.queryType			=	QUERY_GIFTCARD_SaveGiftCard;
	request.hStmt				=	m_hStmt;
	request.retCode				=	-1;
	request.retStructVariable	=	NULL;
	

	if( !runQueryExecuterThread ) return;
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );	

	return;
}


void DB_QueryExecuterThread_QUERY_SAVESERVERSTATUS(const querySet_t& ,querySet_t& )
{
	
}




void DB_QueryExecuterThread_QUERY_LOADSERVERSTATUS(const querySet_t& request,querySet_t& result)
{
	RETCODE rCode=SQL_ERROR;
	
	rCode = SQLFetch(request.hStmt);        
	if((rCode == SQL_NO_DATA) || (SQL_ERROR == rCode)){
		result.retCode = rCode;
		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO )){
			g_DBGameServer->PrintError(request);
		}
		return; 
	}

	long length=0;
	result.retCode = rCode;
	SQLGetData(request.hStmt, 1, SQL_INTEGER, &result.ServerStatus.Value,sizeof(int), &length);


	rCode = SQLCloseCursor( request.hStmt );
	if((rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
		g_DBGameServer->PrintError(request);
	}
}


void DB_QueryExecuterThread_QUERY_GIFTCARD_SaveGiftCard(const querySet_t& request,querySet_t& result)
{
	
	Sleep(0);
}



void DB_ProcessQueryResultQueue_QUERY_SAVESERVERSTATUS(const querySet_t& result)
{
	if(result.retCode < 0){
		g_logSystem->Write("Save ServerStatus Error!!");
		return;
	}

}






void DB_ProcessQueryResultQueue_QUERY_GIFTCARD_SaveGiftCard(const querySet_t& result)
{
	
	Sleep(0);

}


void CDBServer::PrintError(const querySet_t& request)
{
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	SQLSMALLINT   i, MsgLen;
	SQLRETURN     rc ;

	i = 1;
	while ( (rc = SQLGetDiagRec ( SQL_HANDLE_STMT,request.hStmt, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen ) ) != SQL_NO_DATA) 
	{
		g_logSystem->Write ( "%s, %d, %s", SqlState, NativeError, Msg ) ;
		g_logSystem->Write ( "%s",request.query);
		i++;
	}
}












void DB_QueryExecuterThread_QUERY_LOAD_CHARACTER(const querySet_t& request,querySet_t& result )
{
	result.pCharactor=NULL;
	RETCODE rCode=SQL_ERROR;
	
	rCode = SQLFetch(request.hStmt);        
	if((rCode == SQL_NO_DATA) || (SQL_ERROR == rCode)){
		result.retCode = rCode;
		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO )){
			g_DBGameServer->PrintError( request.hStmt );
		}
		return; 
	}

	result.retCode = rCode;
	result.pCharactor = (struct tagQuerySet::tagCharactor*)GlobalAllocPtr(GMEM_FIXED,sizeof(struct tagQuerySet::tagCharactor));
	if(NULL == result.pCharactor) return;

	struct tagQuerySet::tagCharactor* pCharactor = result.pCharactor;
	int column=1;
	long length=0;
	int i;

	SQLGetData(request.hStmt, column++, SQL_C_CHAR,  pCharactor->name, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->pcClass, COLUMNSIZE, &length); 
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->pcJob, COLUMNSIZE, &length); 

	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->skinPart[0], COLUMNSIZE, &length); 
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->skinPart[1], COLUMNSIZE, &length); 
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->skinPart[2], COLUMNSIZE, &length); 
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->skinPart[3], COLUMNSIZE, &length); 

	SQLGetData(request.hStmt, column++, SQL_C_CHAR,  pCharactor->rank, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->rankPoint, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->age, COLUMNSIZE, &length);

	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->worldIdx, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_FLOAT, &pCharactor->position[0], COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_FLOAT, &pCharactor->position[1], COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_FLOAT, &pCharactor->position[2], COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_FLOAT, &pCharactor->angles[1], COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->level, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SBIGINT, &pCharactor->exp, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->increaseExpRate, COLUMNSIZE, &length);

	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->selectedGen, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->genLevel, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SBIGINT, &pCharactor->genExp, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->increaseGenExpRate, COLUMNSIZE, &length);

	for (i=0; i<GEN_NUMBER; i++)
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->genCapability[i], COLUMNSIZE, &length);

	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->force, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->soulpower, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->agility, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->vitality, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->bonusStatusPoint, COLUMNSIZE, &length);
	
	SQLGetData(request.hStmt, column++, SQL_C_FLOAT, &pCharactor->curRA, COLUMNSIZE, &length);	
	SQLGetData(request.hStmt, column++, SQL_C_FLOAT, &pCharactor->curSA, COLUMNSIZE, &length);

	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->curChargeNSE, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->curChargeSE, COLUMNSIZE, &length);
	
	SQLGetData(request.hStmt, column++, SQL_C_FLOAT, &pCharactor->curChargeRA, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_FLOAT, &pCharactor->curChargeSA, COLUMNSIZE, &length);

	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->depotSE, COLUMNSIZE, &length);

	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->curWeapon, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->bindingIdx, COLUMNSIZE, &length);

	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->itemNumber, COLUMNSIZE, &length);

	for (i=0; i<MAX_INVENTORY_SIZE; i++)
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->inventory[i], COLUMNSIZE, &length);

	for (i=0; i<MAX_DEPOT_SIZE; i++)
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->depot[i], COLUMNSIZE, &length);

	for (i=0; i<MAX_EQUIPMENT; i++)
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->equipment[i], COLUMNSIZE, &length);

	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->mouseInventory, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->bonusSkillPoint, COLUMNSIZE, &length);

	for (i=0; i<MAX_NUMBER_OF_SKILL_VARIABLE; i++)
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->skillVariable[i], COLUMNSIZE, &length);


	int hotKey=0;
	for (i=0; i<MAX_NUMBER_OF_HOTKEY; i++){
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &hotKey, COLUMNSIZE, &length);
		if (hotKey >= MASK_HOTKEY_ITEM)
		{
			pCharactor->hotkeyType[i] = 2;	
			pCharactor->hotkeyIdx[i] = hotKey - MASK_HOTKEY_ITEM;
		}
		else if (hotKey >= MASK_HOTKEY_ACTION)
		{									
			pCharactor->hotkeyType[i] = 1;	
			pCharactor->hotkeyIdx[i] = hotKey - MASK_HOTKEY_ACTION;				
		}
		else
		{
			pCharactor->hotkeyType[i] = 0;	
			pCharactor->hotkeyIdx[i] = hotKey;
		}
	}

	SQLGetData(request.hStmt, column++, SQL_C_CHAR, pCharactor->organizerName, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->organizeServer, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->organizeTime, COLUMNSIZE, &length);

	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->questPoint, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->penaltyTime, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->levelUpTime, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->genLevelUpTime, COLUMNSIZE, &length);

	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->gameMaster, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->flagChangeServer, COLUMNSIZE, &length);

	for (i=0; i<MAX_PRECOCITY_SIZE; i++)
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->precocityInventory[i], COLUMNSIZE, &length);
	for (i=0; i<MAX_PRECOCITY_SIZE; i++)
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->precocityTime[i], COLUMNSIZE, &length);
	
	
	
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->chaosPoint, COLUMNSIZE, &length);
	
	
	SQLGetData(request.hStmt, column++, SQL_C_USHORT, &pCharactor->guildIdx, COLUMNSIZE, &length);
	if(pCharactor->guildIdx < 0) pCharactor->guildIdx=-1;
	




	




	
	
	
	
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->GonryunBattle.m_iwin, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->GonryunBattle.m_idefeat, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->GonryunBattle.m_idraw, COLUMNSIZE, &length);
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->GonryunBattle.lDateV2,sizeof(long),&length);
	SQLGetData(request.hStmt, column++, SQL_INTEGER, &pCharactor->m_igonryunUseCount, COLUMNSIZE, &length);

	
	SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pCharactor->iDefaultInitCount, sizeof(int),&length);

	SQLGetData(request.hStmt,column++,SQL_C_SLONG,&pCharactor->GonryunBattle.iBattleGUID, sizeof(int),	&length);
	SQLGetData(request.hStmt,column++,SQL_C_SLONG,&pCharactor->GonryunBattle.iTournamentLevelIdx, sizeof(int),	&length);
	SQLGetData(request.hStmt,column++,SQL_C_SLONG,&pCharactor->GonryunBattle.manor, sizeof(int),	&length);
	SQLGetData(request.hStmt,column++,SQL_INTEGER, &pCharactor->m_bHide, sizeof(BOOL),	&length);

	
	SQLGetData(request.hStmt,column++,SQL_INTEGER, &pCharactor->GM.Chat.RemainBanTimePerSecond,sizeof(int),&length);	
	

	
	SQLGetData(request.hStmt,column++,SQL_INTEGER, &pCharactor->nAccKillPoint, sizeof(int),&length);	

	
	SQLGetData(request.hStmt,column++,SQL_INTEGER, &pCharactor->GuildDeconstructionTime, sizeof(int),&length);
	SQLGetData(request.hStmt,column++,SQL_INTEGER, &pCharactor->GuildsecedeTime, sizeof(int),&length);	
	



	rCode = SQLCloseCursor( request.hStmt );
	if((rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
		g_DBGameServer->PrintError(request);
	}
}



playerCharacter_t* GetNOWLoadingPLAYERPointer(const querySet_t& result)
{
	if(result.ownerIdx < 0) return NULL;
	if(result.ownerIdx >= MAX_PCS) return NULL;
	if(!g_pc[result.ownerIdx].active) return NULL;
	if(0 != strcmp(result.ownerID,g_pc[result.ownerIdx].userID)) return NULL;

	return &g_pc[result.ownerIdx];
}
									



void DB_ProcessQueryResultQueue_QUERY_LOAD_CHARACTER(const querySet_t& result)
{
	struct tagQuerySet::tagCharactor* pCharactor = result.pCharactor;
	if((result.retCode < 0) || (NULL == pCharactor)){ 
		if(NULL != pCharactor){ GlobalFreePtr(pCharactor); pCharactor=NULL;}
		return;
	}

	playerCharacter_t* pPlayer=GetNOWLoadingPLAYERPointer(result);
	if(NULL == pPlayer){
		if(NULL != pCharactor){ GlobalFreePtr(pCharactor); pCharactor=NULL;}
		return;
	}

	int idx;


	strncpy(pPlayer->name,pCharactor->name,NAMESTRING); 
	pPlayer->name[NAMESTRING-1]=NULL;

	pPlayer->pcClass = pCharactor->pcClass;	
	pPlayer->pcJob = pCharactor->pcJob;
	pPlayer->skinPart[0] = pCharactor->skinPart[0];
	pPlayer->skinPart[1] = pCharactor->skinPart[1];
	pPlayer->skinPart[2] = pCharactor->skinPart[2];
	pPlayer->skinPart[3] = pCharactor->skinPart[3];
	strncpy(pPlayer->rank,pCharactor->rank,NAMESTRING); 
	pPlayer->rank[NAMESTRING-1]=NULL;

	pPlayer->rankPoint   = pCharactor->rankPoint;
	pPlayer->age		 = pCharactor->age;
	pPlayer->worldIdx	 = pCharactor->worldIdx;
	pPlayer->position[0] = pCharactor->position[0];
	pPlayer->position[1] = pCharactor->position[1];
	pPlayer->position[2] = pCharactor->position[2];
	pPlayer->angles[1]   = pCharactor->angles[1];
	pPlayer->level = pCharactor->level;
	pPlayer->exp   = pCharactor->exp;
	pPlayer->increaseExpRate = pCharactor->increaseExpRate;
	pPlayer->selectedGen = pCharactor->selectedGen;
	pPlayer->genLevel = pCharactor->genLevel;
	pPlayer->genExp = pCharactor->genExp;
	pPlayer->increaseGenExpRate = pCharactor->increaseGenExpRate;

	
	for(idx=0; idx<GEN_NUMBER; idx++)
		pPlayer->genCapability[idx] = pCharactor->genCapability[idx];

	pPlayer->force = pCharactor->force;
	pPlayer->soulpower = pCharactor->soulpower;
	pPlayer->agility = pCharactor->agility;
	pPlayer->vitality = pCharactor->vitality;
	pPlayer->bonusStatusPoint = pCharactor->bonusStatusPoint;
	pPlayer->curRA = pCharactor->curRA;
	pPlayer->curSA = pCharactor->curSA;
	pPlayer->curChargeNSE = pCharactor->curChargeNSE;
	pPlayer->curChargeSE = pCharactor->curChargeSE;
	pPlayer->curChargeRA = pCharactor->curChargeRA;
	pPlayer->curChargeSA = pCharactor->curChargeSA;
	pPlayer->depotSE = pCharactor->depotSE;
	pPlayer->curWeapon = pCharactor->curWeapon;
	pPlayer->bindingIdx = pCharactor->bindingIdx;


	pPlayer->itemNumber = pCharactor->itemNumber;
	for(idx=0; idx < MAX_INVENTORY_SIZE; idx++)
		pPlayer->inventory[idx] = pCharactor->inventory[idx];

	for(idx=0; idx < MAX_DEPOT_SIZE; idx++)
		pPlayer->depot[idx] = pCharactor->depot[idx];

	for(idx=0; idx < MAX_EQUIPMENT; idx++)
		pPlayer->equipment[idx] = pCharactor->equipment[idx];

	pPlayer->mouseInventory = pCharactor->mouseInventory;
	pPlayer->bonusSkillPoint = pCharactor->bonusSkillPoint;

	for(idx=0; idx < MAX_NUMBER_OF_SKILL_VARIABLE; idx++)
		pPlayer->skillVariable[idx] = pCharactor->skillVariable[idx];

	for(idx=0; idx < MAX_NUMBER_OF_HOTKEY; idx++){
		pPlayer->hotkeyType[idx] = pCharactor->hotkeyType[idx];
		pPlayer->hotkeyIdx[idx]  = pCharactor->hotkeyIdx[idx];
	}

	strncpy(pPlayer->organizerName,pCharactor->organizerName,NAMESTRING); 
	pPlayer->organizerName[NAMESTRING-1]=NULL;	

	pPlayer->organizeServer = pCharactor->organizeServer;
	pPlayer->organizeTime   =pCharactor->organizeTime;

	pPlayer->questPoint = pCharactor->questPoint;
	pPlayer->penaltyTime = pCharactor->penaltyTime;


	pPlayer->levelUpTime = pCharactor->levelUpTime;
	pPlayer->genLevelUpTime = pCharactor->genLevelUpTime;

	pPlayer->gameMaster = pCharactor->gameMaster;
	pPlayer->flagChangeServer = pCharactor->flagChangeServer;

	for(idx=0; idx < MAX_PRECOCITY_SIZE; idx++)
		pPlayer->precocityInventory[idx] = pCharactor->precocityInventory[idx];

	for(idx=0; idx < MAX_PRECOCITY_SIZE; idx++)
		pPlayer->precocityTime[idx] = pCharactor->precocityTime[idx];
		
	pPlayer->chaosPoint = pCharactor->chaosPoint;
	pPlayer->guildIdx = pCharactor->guildIdx;

	


	pPlayer->GonryunBattlePractice.Record.iwin = pCharactor->GonryunBattle.m_iwin;
	pPlayer->GonryunBattlePractice.Record.idefeat = pCharactor->GonryunBattle.m_idefeat;
	pPlayer->GonryunBattlePractice.Record.idraw = pCharactor->GonryunBattle.m_idraw;
	pPlayer->PremiumInfo.GonyounPracticeBattle.lDateV2 = pCharactor->GonryunBattle.lDateV2;


	
		
	pPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount = pCharactor->m_igonryunUseCount;
	pPlayer->PremiumInfo.Status.iDefaultInitCount = pCharactor->iDefaultInitCount;	
		
		


	
	
	
	pPlayer->m_bHide = pCharactor->m_bHide;


	
	pPlayer->enumNameColor = playerCharacter_t::enumNameColor::normal;
	if((pCharactor->GonryunBattle.iBattleGUID >= 1) &&
	   (pCharactor->GonryunBattle.iTournamentLevelIdx >= 1)){
		
		switch(pCharactor->GonryunBattle.manor){			
			case CGolryunBattle_Process::enumManor::week:
				pPlayer->enumNameColor = playerCharacter_t::enumNameColor::week;
				break;

			case CGolryunBattle_Process::enumManor::month:
				pPlayer->enumNameColor = playerCharacter_t::enumNameColor::month;
				break;

			case CGolryunBattle_Process::enumManor::ramification:
				pPlayer->enumNameColor = playerCharacter_t::enumNameColor::ramification;
				break;

			case CGolryunBattle_Process::enumManor::year:
				pPlayer->enumNameColor = playerCharacter_t::enumNameColor::year;
				break;
		}
	}
	



	
	
	pPlayer->GM.Chat.RemainBanTimePerSecond = pCharactor->GM.Chat.RemainBanTimePerSecond;
	

	
	pPlayer->nAccumulationBattleKillPoint = pCharactor->nAccKillPoint;

	
	pPlayer->GuildDeconstructionTime	= pCharactor->GuildDeconstructionTime;
	pPlayer->GuildsecedeTime			= pCharactor->GuildsecedeTime;


	pPlayer->Login.Loading.bCharactor=TRUE;
	if(NULL != pCharactor){ GlobalFreePtr(pCharactor); pCharactor=NULL;}
}








void DB_QueryExecuterThread_QUERY_LOAD_QUEST(const querySet_t& request,querySet_t& result)
{	
	RETCODE	rCode=SQL_ERROR;
	result.pQuest =NULL;

	
	rCode = SQLFetch(request.hStmt);        
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode))
	{
		result.retCode = rCode;
		rCode = SQLCloseCursor( request.hStmt );
		if((rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
			g_DBGameServer->PrintError(request);
		}
		return; 
	}

	result.retCode = rCode;
	struct tagQuerySet::tagQuest* pQuest=(struct tagQuerySet::tagQuest*)GlobalAllocPtr(GMEM_FIXED,sizeof(struct tagQuerySet::tagQuest));
	if(NULL == pQuest) return;

	result.pQuest = pQuest;
	long			length=0;
	int				column=1;
	int				idx;
	
	for(idx = 0; idx < MAX_COUNT_QUEST_VARIABLE; idx++){
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &(pQuest->questVar[idx].iVar), COLUMNSIZE, &length); 
	}

	for(idx = 0; idx < MAX_COUNT_QUEST_INVENTORY; idx++){
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &(pQuest->questInventory[idx].iItem), COLUMNSIZE, &length); 
	}
	
	
	rCode = SQLCloseCursor( request.hStmt );
	if((rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
		g_DBGameServer->PrintError(request);
	}
}


void DB_ProcessQueryResultQueue_QUERY_LOAD_QUEST(const querySet_t& result)
{
	struct tagQuerySet::tagQuest* pQuest = result.pQuest;
	if((result.retCode < 0) || (NULL == pQuest)){ 
		if(NULL != pQuest){ GlobalFreePtr(pQuest); pQuest=NULL;}
		return;
	}

	playerCharacter_t* pPlayer=GetNOWLoadingPLAYERPointer(result);
	if(NULL == pPlayer){
		if(NULL != pQuest){ GlobalFreePtr(pQuest); pQuest=NULL;}
		return;
	}

	int idx;
	for(idx = 0; idx < MAX_COUNT_QUEST_VARIABLE; idx++){
		pPlayer->questVar[idx].iVar = pQuest->questVar[idx].iVar;
	}

	for(idx = 0; idx < MAX_COUNT_QUEST_INVENTORY; idx++){
		pPlayer->questInventory[idx].iItem = pQuest->questInventory[idx].iItem;
	}


	pPlayer->Login.Loading.bQuest=TRUE;
	if(NULL != pQuest){ GlobalFreePtr(pQuest); pQuest=NULL;}	
}




void DB_QueryExecuterThread_QUERY_LOAD_QUESTSTATUS(const querySet_t& request,querySet_t& result)
{
	RETCODE			rCode=SQL_ERROR;
	result.pQuestStatus=NULL;
	
	rCode = SQLFetch(request.hStmt);        
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode)){
		result.retCode = rCode;
		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
			g_DBGameServer->PrintError(request);
		}
		return; 
	}

	result.retCode = rCode;
	struct tagQuerySet::tagQuestStatus* pQuestStatus=
		(struct tagQuerySet::tagQuestStatus*)GlobalAllocPtr(GMEM_FIXED,sizeof(struct tagQuerySet::tagQuestStatus));
	if(NULL == pQuestStatus) return;

	int idx;
	pQuestStatus->questNumber=0;
	for( idx = 0; idx < MAX_COUNT_QUEST_STATUS; idx ++ ){
		pQuestStatus->questStatus[idx].idx = idx;
		pQuestStatus->questStatus[idx].questIdx=-1;
		pQuestStatus->questStatus[idx].limitTime=-1;
		memset(pQuestStatus->questStatus[idx].partyOrganizerName,0,sizeof(char)*NAMESTRING);
		pQuestStatus->questStatus[idx].partyOrganizeServer=-1;
		pQuestStatus->questStatus[idx].partyOrganizeTime=-1;	
		memset(pQuestStatus->questStatus[idx].dField, -1, sizeof(int)*MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD );
	}
	result.pQuestStatus=pQuestStatus;


	int column;	
	long length;
	questStatus_t* pTempQuestStatus=NULL;	

	for (; rCode == SQL_SUCCESS; rCode = SQLFetch(request.hStmt) ){
		column = 1;
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &idx, COLUMNSIZE, &length);
		if( (idx < 0) || (idx >= MAX_COUNT_QUEST_STATUS)){			
			break;
		}

		pQuestStatus->questNumber++;
		pTempQuestStatus = &pQuestStatus->questStatus[idx];	

		SQLGetData(request.hStmt, column++, SQL_C_SLONG,  &pTempQuestStatus->questIdx, COLUMNSIZE, &length);
		for(idx=0; idx < MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD; idx++)
			SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pTempQuestStatus->dField[idx], COLUMNSIZE, &length);

		SQLGetData(request.hStmt, column++, SQL_C_CHAR,  pTempQuestStatus->partyOrganizerName, COLUMNSIZE, &length);
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pTempQuestStatus->partyOrganizeServer, COLUMNSIZE, &length); 
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pTempQuestStatus->partyOrganizeTime, COLUMNSIZE, &length); 
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pTempQuestStatus->clearTime, COLUMNSIZE, &length); 
		SQLGetData(request.hStmt, column++, SQL_C_SLONG, &pTempQuestStatus->limitTime, COLUMNSIZE, &length); 
	}


	rCode = SQLCloseCursor(request.hStmt );
	if((rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
		g_DBGameServer->PrintError(request);
	}	
}


void DB_ProcessQueryResultQueue_QUERY_LOAD_QUESTSTATUS(const querySet_t& result)
{
	struct tagQuerySet::tagQuestStatus* pQuestStatus = result.pQuestStatus;
	if((result.retCode < 0) || (NULL == pQuestStatus)){ 
		if(NULL != pQuestStatus){ GlobalFreePtr(pQuestStatus); pQuestStatus=NULL;}
		return;
	}

	playerCharacter_t* pPlayer=GetNOWLoadingPLAYERPointer(result);
	if(NULL == pPlayer){
		if(NULL != pQuestStatus){ GlobalFreePtr(pQuestStatus); pQuestStatus=NULL;}
		return;
	}


	for(int idx = 0; idx < MAX_COUNT_QUEST_STATUS; idx ++ ){
		pPlayer->questStatus[idx]=pQuestStatus->questStatus[idx];
	}
	pPlayer->questNumber = pQuestStatus->questNumber;


	pPlayer->Login.Loading.bQuestStatus=TRUE;
	if(NULL != pQuestStatus){ GlobalFreePtr(pQuestStatus); pQuestStatus=NULL;}
}








void CDBGame::LoadServerStatus(void)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(m_query,QUERYSIZE, "EXEC LoadServerStatus");
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = -1;
	request.ownerID[0]=NULL;
	request.queryType = QUERY_LOADSERVERSTATUS;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
}

void CDBGame::SaveServerStatus(const BOOL bPK)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(m_query,QUERYSIZE, "EXEC SaveServerStatus %d",bPK);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = -1;
	request.ownerID[0]=NULL;
	request.queryType = QUERY_SAVESERVERSTATUS;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
}











void CDBGame::Delete_Danbattle(int nRequestIdx, int nAcceptIdx)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC deleteDanBattle %d, %d",  nRequestIdx, nAcceptIdx);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = g_config.gameServerNo;
	strncpy(request.ownerID, g_config.serverGroupName ,IDSTRING) ;
	request.ownerID[IDSTRING -1] = NULL;	
	
	request.queryType = QUERY_DELETE_DANBATTLE;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

}

void DB_QueryExecuterThread_DELETE_DANBATTLE(const querySet_t& request,querySet_t& result)
{
	RETCODE				rCode=SQL_ERROR;
	long				length=0;
	
	rCode= SQLFetch(request.hStmt);	
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode)){		
		result.retCode=rCode;

		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
		{
			g_DBGameServer->PrintError( request.hStmt );
		}
		return;	
	}

	result.retCode = rCode;	
	SQLGetData(request.hStmt, 1, SQL_C_SLONG, &result.Danbattle.code, sizeof(int), &length);		

	rCode = SQLCloseCursor( request.hStmt );
	if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
	{
		g_DBGameServer->PrintError( request.hStmt );
	}
	return;
}

void DB_ProcessQueryResultQueue_DELETE_DANBATTLE(querySet_t& result)
{
}
	









void CDBGame::Delete_Alldanbattle(int MemberServerIdx)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC deletealldanbattle %d", MemberServerIdx);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = MemberServerIdx;
	strncpy(request.ownerID, g_config.serverGroupName ,IDSTRING) ;
	request.ownerID[IDSTRING -1] = NULL;

	request.queryType = QUERY_DELETE_ALLDANBATTLE;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
}






void DB_QueryExecuterThread_DELETE_ALLDANBATTLE(const querySet_t& request,querySet_t& result)
{
	RETCODE				rCode=SQL_ERROR;
	long				length=0;
	
	rCode= SQLFetch(request.hStmt);	
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode)){		
		result.retCode=rCode;

		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
		{
			g_DBGameServer->PrintError( request.hStmt );
		}
		return;	
	}
	result.retCode = rCode;

	rCode = SQLCloseCursor( request.hStmt );
	if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
	{
		g_DBGameServer->PrintError( request.hStmt );
	}
	return;
}






void DB_ProcessQueryResultQueue_DELETE_ALLDANBATTLE(querySet_t& result)
{
	if ( result.retCode < 0)
		g_logSystem->Write("Database Error: QUERY_DELETE_ALLDANBATTLE query: %s",result.query);
}






void CDBGame::Save_danbattle_guild_AddWinpoint(const int nGuildIdx)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC SaveDanBattleGuildAddWinPoint %d", nGuildIdx);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = nGuildIdx;
	strncpy(request.ownerID, g_config.serverGroupName ,IDSTRING) ;
	request.ownerID[IDSTRING -1] = NULL;

	request.queryType = QUERY_SAVE_DANBATTLE_GUILD_ADD_WINPOINT;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
}






void CDBGame::Save_danbattle_guild_AddLosepoint(const int nGuildIdx)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC SaveDanBattleGuildAddLosePoint %d", nGuildIdx);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = nGuildIdx;
	strncpy(request.ownerID, g_config.serverGroupName ,IDSTRING) ;
	request.ownerID[IDSTRING -1] = NULL;

	request.queryType = QUERY_SAVE_DANBATTLE_GUILD_ADD_LOSEPOINT;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
}






void CDBGame::Save_danbattle_guild_AddAccpoint(const int nGuildIdx, const int nAccPoint)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC SaveDanBattleGuildAdd_AccumulationPoint %d, %d", nGuildIdx, nAccPoint);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = nGuildIdx;
	strncpy(request.ownerID, g_config.serverGroupName ,IDSTRING) ;
	request.ownerID[IDSTRING -1] = NULL;

	request.queryType = QUERY_SAVE_DANBATTLE_GUILD_ADD_ACCPOINT;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

}






void CDBGame::Load_danbattle_guild_Info(const int nGuildIdx)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC LoadDanBattleGuildInfo %d", nGuildIdx);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = nGuildIdx;
	strncpy(request.ownerID, g_config.serverGroupName ,IDSTRING) ;
	request.ownerID[IDSTRING -1] = NULL;

	request.queryType = QUERY_LOAD_DANBATTLE_GUILD_INFO;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	
}






void CDBGame::Save_danbattle_pcpoint(playerCharacter_t *pc, int nPoint)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC SaveDanBattlePcPoint '%s', %d", pc->name,  nPoint);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pc->idx;
	strncpy(request.ownerID, pc->userID,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_DANBATTLE_PCPOINT;
	strncpy(request.query,m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	
}






void CDBGame::Insert_Danbattle(int nDanBattleIdx, int nRequestIdx, int AcceptIdx)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC InsertDanBattle %d, %d, %d", g_config.gameServerNo, nRequestIdx, AcceptIdx);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = nDanBattleIdx;
	strncpy(request.ownerID, g_config.serverGroupName ,IDSTRING);
	request.ownerID[IDSTRING-1] = NULL;
	request.queryType = QUERY_INSERT_DANBATTLE;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
}








void DB_QueryExecuterThread_INSERT_DANBATTLE(const querySet_t& request,querySet_t& result)
{
	RETCODE				rCode=SQL_ERROR;
	long				length=0;
	
	rCode= SQLFetch(request.hStmt);	
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode)){		
		result.retCode=rCode;

		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
		{
			g_DBGameServer->PrintError( request.hStmt );
		}
		return;	
	}

	result.retCode = rCode;		
	SQLGetData(request.hStmt, 1, SQL_C_SLONG, &result.Danbattle.code, sizeof(int), &length);		

	rCode = SQLCloseCursor( request.hStmt );
	if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
	{
		g_DBGameServer->PrintError( request.hStmt );
	}
	return;
}







void DB_ProcessQueryResultQueue_QUERY_INSERT_DANBATTLE(querySet_t& result)
{	
	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = NULL;

	
	pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo(result.ownerIdx, 
		NewDanBattleSystem::DanBattleInfo::BATTLESTATUS_REGIST);

	
	if ( pDanBattleInfo != NULL)
		GTH_ProcessInsertDanBattle(pDanBattleInfo, result.Danbattle.code);
}













void DB_QueryExecuterThread_Query_LOAD_DANBATTLE_GUILD_INFO(const querySet_t& request,querySet_t& result)
{
	RETCODE				rCode=SQL_ERROR;
	long				length=0;
	
	rCode= SQLFetch(request.hStmt);
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode)){
		result.retCode=rCode;

		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
		{
			g_DBGameServer->PrintError( request.hStmt );
		}
		return;
	}

	result.retCode = rCode;	


	
	SQLGetData(request.hStmt, 1, SQL_C_SLONG, &result.DanBattleGuildInfo.WinPoint	, sizeof(int), &length);
	
	SQLGetData(request.hStmt, 2, SQL_C_SLONG, &result.DanBattleGuildInfo.LosePoint, sizeof(int), &length);
	
	SQLGetData(request.hStmt, 3, SQL_C_SLONG, &result.DanBattleGuildInfo.AccPoint, sizeof(int), &length);	

	rCode = SQLCloseCursor( request.hStmt );
	if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO))
	{
		g_DBGameServer->PrintError( request.hStmt );
	}
	return;

}

void DB_ProcessQueryResultQueue_LOAD_DANBATTLE_GUILD_INFO(querySet_t& result)
{
	if( result.retCode < 0){
		g_logSystem->Write("Database Error: QUERY_LOAD_DANBATTLE_GUILD_INFO query: %s",result.query);
		result;
	}

	int GuildIdx = result.ownerIdx;
	if ( GuildIdx <= 0 || GuildIdx >= MAX_GUILDS)
		return;

	g_guild[GuildIdx].wincount = result.DanBattleGuildInfo.WinPoint;
	g_guild[GuildIdx].Losecount = result.DanBattleGuildInfo.LosePoint;
	g_guild[GuildIdx].nKillPoint = result.DanBattleGuildInfo.AccPoint;

}

void DB_QueryExecuterThread_Query_SAVE_DANBATTLE_PCPOINT(const querySet_t& request,querySet_t& result)
{

}
