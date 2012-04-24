

#include "global.h"
//lucky 2011 read odbc info from server.cfg
#include "Ini.h"
//end
HANDLE			hQueryExecuterThread=NULL;
DWORD			queryExecuterThreadID=NULL;
int				runQueryExecuterThread=FALSE;
HANDLE			hQueryRequestMutex=NULL;
HANDLE			hQueryResultMutex=NULL;
CQueryQueue* queryRequestQueue=NULL;
CQueryQueue* queryResultQueue=NULL;




#include "CPremiumPlayerCtrl.h"
extern CPremiumPlayerCtrl* gcpPremiumPlayerCtrl;

#include "Terra\Billing\CTerraBillingCtrl_Encoder.h"
extern CTerraBillingCtrl_Encoder* gcpTerraBillingCtrl_Encder;




void  TRACE(LPCTSTR lpszFormat, ...);

CDBServer::CDBServer()
: m_hDbc ( SQL_NULL_HDBC ), m_hEnv ( SQL_NULL_HENV ), m_hStmt ( SQL_NULL_HSTMT )
{
}

CDBServer::~CDBServer()
{
}

int CDBServer::Initialize(char *DSN, int DSNType)
{
	unsigned char outConnectionStr[OUTCONNECTIONSIZE]="";
	char connection[256]="";
	
    m_rCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv); 
    if (m_rCode != SQL_SUCCESS)
		return false;

	SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);

    m_rCode = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc); 
    if (m_rCode != SQL_SUCCESS )
		return false;
	//lucky 2011 Read the ODBC information from the server.cfg files. to make the server easy to start
	CIni iniConfig;
	char sPath[260] = {0};
	GetCurrentDirectory( 260, sPath );
	strcat( sPath, "\\" );
	strcat( sPath, "server.cfg" );
	iniConfig.SetPathName( sPath );
	char sName[32] = {0};
	char sPsw[32] = {0};
	
	
	if ( DSNType == 1 )	
	// FILE DSN
	{
		iniConfig.GetArray( "DB_PARAM", "Name1", sName, 32 );
		iniConfig.GetArray( "DB_PARAM", "Password1", sPsw, 32 );
		sprintf(connection, "FILEDSN=%s;UID=%s;PWD=%s", DSN, sName, sPsw);
	}
	else
	// SYSTEM DSN
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

int CDBServer::Initialize(char *DSN, int DSNType, char *UID, char *PWD)
{
	unsigned char outConnectionStr[OUTCONNECTIONSIZE]="";
	char connection[256]="";
	
    m_rCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv); 
    if (m_rCode != SQL_SUCCESS)
		return false;

	SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);

    m_rCode = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc); 
    if (m_rCode != SQL_SUCCESS )
		return false;

	
	if ( DSNType == 1 )	
		sprintf(connection, "FILEDSN=%s;UID=%s;PWD=%s", DSN, UID, PWD);
	else				
		sprintf(connection, "DSN=%s;UID=%s;PWD=%s", DSN, UID, PWD);

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

	i = 1;
	while ( (rc = SQLGetDiagRec ( handleType, m_hStmt, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen ) ) != SQL_NO_DATA) 
	{
		g_logSystem->Write ( "%s, %d, %s", SqlState, NativeError, Msg ) ;
		i++;
		if ( i >= 10)
			break;

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








int CDBAccount::CheckAccount(i3client_t *client, char *id, char *password) 
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC CheckAccount '%s', '%s'", id, password);
	m_query[QUERYSIZE]=NULL;


	request.ownerIdx = client->idx;
	strncpy(request.ownerID, "",IDSTRING); request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_CHECK_ACCOUNT;
	strncpy(request.query,m_query,QUERYSIZE); request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

	if( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}
































int CDBAccount::CheckUserID(i3client_t *client)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC CheckUserID '%s'", client->id);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = client->idx;
	strncpy(request.ownerID, "",IDSTRING);
	request.ownerID[IDSTRING]=NULL;


	request.queryType = QUERY_CHECK_USERID;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}

int CDBAccount::InsertLogin(i3client_t *client)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));



	_snprintf(m_query,QUERYSIZE,"EXEC InsertLogin '%s', '%s'", client->id, g_hostName);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = client->idx;
	strncpy(request.ownerID, client->id,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_INSERT_LOGIN;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}

int CDBAccount::DeleteLogin(i3client_t *client) 
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC DeleteLogin '%s'", client->id);
	m_query[QUERYSIZE]=NULL;


	request.ownerIdx = client->idx;
	strncpy(request.ownerID, client->id ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_DELETE_LOGIN;
	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

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

int CDBAccount::InsertServerGroupUsers(int serverGroupID, int currentUsers, int maxUsers)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC usp_insert_group_usercount %d, %d, %d",serverGroupID, currentUsers, maxUsers);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = -1;
	strncpy(request.ownerID, "",IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_INSERT_SERVER_GROUP_USERS;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = NULL;

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


#include "CTools.h"
void CDBAccount::SaveCharactorPostion(const i3client_t* pClient,const int CharacterID)
{
	characterData_t*  pCharactorSlot=NULL;
	pCharactorSlot=CTools::GetCharactorWithCharacterID(pClient,CharacterID);
	if(NULL == pCharactorSlot) return;


	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC SaveCharacterPosition '%s',%d,%d,%d,%f,%f,%f",
		pClient->id,
		pClient->serverGroupID,
		CharacterID,
		pCharactorSlot->worldIdx,
		pCharactorSlot->position[0],
		pCharactorSlot->position[1],
		pCharactorSlot->position[2]);

	m_query[QUERYSIZE]=NULL;
	

	request.SaveCharactorPosition.CharacterID = CharacterID;
	request.ownerIdx = pClient->idx;
	strncpy(request.ownerID, pClient->id,IDSTRING); request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVE_CHARACTOR_POSTION;
	strncpy(request.query, m_query,QUERYSIZE); request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pClient;

	if ( !runQueryExecuterThread ) return;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return;
}



void CDBGame::LoadPremiumInfo(i3client_t* pClient)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC LoadPremiumInfo '%s'", pClient->id);
	m_query[QUERYSIZE]=NULL;
	

	request.ownerIdx = pClient->idx;
	strncpy(request.ownerID, pClient->id,IDSTRING); request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_PREMIUMINFO;
	strncpy(request.query, m_query,QUERYSIZE); request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pClient;

	if ( !runQueryExecuterThread ) return;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return;
}




int CDBGame::CheckCharacterName(i3client_t *client, int serverGroupID, char *charName)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(m_query,QUERYSIZE, "EXEC CheckCharacterName %d, '%s'", serverGroupID, charName);
	m_query[QUERYSIZE]=NULL;


	request.ownerIdx = client->idx;
	strncpy(request.ownerID, client->id,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_CHECK_CHARACTER_NAME;
	strncpy(request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}



int CDBGame::LoadAllGame(i3client_t *client)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC LoadAllGame '%s', %d", client->id, client->serverGroupID);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = client->idx;
	strncpy(request.ownerID, client->id,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_ALL_GAME;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}

int CDBGame::LoadAllItem(i3client_t *client)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(m_query,QUERYSIZE, "EXEC LoadAllItem '%s', %d", client->id, client->serverGroupID);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = client->idx;
	strncpy(request.ownerID, client->id,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_ALL_ITEM;
	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}

int CDBGame::LoadAllQuest(i3client_t *client)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC LoadAllQuest '%s', %d", client->id, client->serverGroupID);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = client->idx;
	strncpy(request.ownerID, client->id, IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_ALL_QUEST;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
	
}

int CDBGame::LoadAllQuestStatus(i3client_t *client)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC LoadAllQuestStatus '%s', %d", client->id, client->serverGroupID);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = client->idx;
	strncpy(request.ownerID, client->id,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_LOAD_ALL_QUESTSTATUS;
	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
	
}


int CDBGame::LoadCharacterSlot(void)
{
	querySet_t	request;
	i3client_t* pClient = g_curClient;

	_snprintf(m_query,QUERYSIZE, "EXEC LoadCharacterSlot '%s',%d",pClient->id,pClient->serverGroupID);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx	= pClient->idx;
	strncpy(request.ownerID, pClient->id,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType	= QUERY_LOAD_CHARACTER_SLOT;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt		= m_hStmt;
	request.retCode		= -1;
	request.retStructVariable = (void *)pClient;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}


int CDBGame::SaveCharacterSlot(i3client_t* pClient,int type)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));
	

	
	if(type < LOGIN_CHAR_FREE_NEW || type > LOGIN_CHAR_PU) return 0;

	pClient->m_nCharacterSlotType = type;
		
	_snprintf(m_query,QUERYSIZE, "EXEC SaveCharacterSlot '%s',%d,%d",
		pClient->id, 
		pClient->serverGroupID,
		pClient->m_nCharacterSlotType);
	m_query[QUERYSIZE]=NULL;


	request.ownerIdx	= pClient->idx;
	strncpy(request.ownerID, pClient->id,IDSTRING);
	request.ownerID[IDSTRING]=NULL;


	request.queryType	= QUERY_SAVE_CHARACTER_SLOT;
	strncpy(request.query, m_query ,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt		= m_hStmt;
	request.retCode		= -1;
	request.retStructVariable = (void *)pClient;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}

int CDBGame::LoadAllSkill(i3client_t *client)
{
	querySet_t	request;

	sprintf(m_query, "EXEC LoadAllSkill '%s', %d", client->id, client->serverGroupID);

	request.ownerIdx = client->idx;
	strcpy(request.ownerID, client->id);
	request.queryType = QUERY_LOAD_ALL_SKILL;
	strcpy(request.query, m_query);
	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}


int CDBGame::UpdateGameOwnItemInfo(i3client_t *client, characterData_t *data)
{
	querySet_t	request;


	sprintf(m_query, "EXEC UpdateGameOwnItemInfo '%s', %d, %d, \
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
						%d, \
						%d, %d, %d, %d, %d, %d",
		client->id, client->serverGroupID, data->characterID,
		data->itemNumber,
		data->inventory[0], data->inventory[1], data->inventory[2], data->inventory[3], data->inventory[4], data->inventory[5],
		data->inventory[6], data->inventory[7], data->inventory[8], data->inventory[9], data->inventory[10], data->inventory[11],
		data->inventory[12], data->inventory[13], data->inventory[14], data->inventory[15], data->inventory[16], data->inventory[17],
		data->inventory[18], data->inventory[19], data->inventory[20], data->inventory[21], data->inventory[22], data->inventory[23],
		data->inventory[24], data->inventory[25], data->inventory[26], data->inventory[27], data->inventory[28], data->inventory[29],
		data->inventory[30], data->inventory[31], data->inventory[32], data->inventory[33], data->inventory[34], data->inventory[35],
		data->inventory[36], data->inventory[37], data->inventory[38], data->inventory[39], data->inventory[40], data->inventory[41],
		data->inventory[42], data->inventory[43], data->inventory[44], data->inventory[45], data->inventory[46], data->inventory[47],
		data->depot[0], data->depot[1], data->depot[2], data->depot[3], data->depot[4], data->depot[5], data->depot[6], data->depot[7], data->depot[8], data->depot[9],
		data->depot[10], data->depot[11], data->depot[12], data->depot[13], data->depot[14], data->depot[15], data->depot[16], data->depot[17], data->depot[18], data->depot[19],
		data->depot[20], data->depot[21], data->depot[22], data->depot[23], data->depot[24], data->depot[25], data->depot[26], data->depot[27], data->depot[28], data->depot[29],
		data->depot[30], data->depot[31], data->depot[32], data->depot[33], data->depot[34], data->depot[35], data->depot[36], data->depot[37], data->depot[38], data->depot[39],
		data->depot[40], data->depot[41], 
		data->equipment[0], data->equipment[1], data->equipment[2], data->equipment[3], data->equipment[4], data->equipment[5], data->equipment[6],
		data->equipment[7], data->equipment[8], data->equipment[9], data->equipment[10], data->equipment[11], data->equipment[12], data->equipment[13],
		data->mouseInventory,
		data->precocityInventory[0], data->precocityInventory[1], data->precocityInventory[2], data->precocityInventory[3], data->precocityInventory[4], data->precocityInventory[5]);


	request.ownerIdx = client->idx;
	strcpy(request.ownerID, client->id);
	request.queryType = QUERY_UPDATE_GAMEOWNITEMINFO;
	strcpy(request.query, m_query);
	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}

int CDBGame::CreateCharacter(i3client_t *client)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	characterData_t* data=NULL;
	data = &client->createData;
	
	

	_snprintf(m_query,QUERYSIZE, "EXEC CreateCharacter "
		"'%s', %d, %d, '%s', %d, %d, %d, %d, %d, %d,"
		"'%s', %d, %d, %d, %f, %f, %f, %f, %d, %I64d, %d,"
		"%d, %d, %I64d, %d,"
		"%d, %d, %d, %d, %d, %d, %d, %d,"
		"%d, %d, %d, %d, %d,"
		"%d, %d, %d, %d, %d, %d,"
		"%d, %d,	%d, %d, %d, %d, %d,"
		"%d, %d,"
		"0, 'default0', '%s', %d, %d,"
		"%d, %d, %d, %d, %d, %d,"
		"%d, %d, %d, %d, %d, %d,"
		"%d, %d, %d,"
		"1, 'default1', '%s', %d, %d,"
		"%d, %d, %d, %d, %d, %d,"
		"%d, %d, %d, %d, %d, %d,"
		"%d, %d, %d,"
		"2, 'default2', '%s', %d, %d,"
		"%d, %d, %d, %d, %d, %d,"
		"%d, %d, %d, %d, %d, %d,"
		"%d, %d, %d",
  
		

		client->id, client->serverGroupID, data->characterID, data->name, data->pcClass, data->pcJob, data->skinPart[0], data->skinPart[1], data->skinPart[2], data->skinPart[3],
		data->rank, data->rankPoint, data->age, data->worldIdx, data->position[0], data->position[1], data->position[2], data->angles[1], data->level, data->exp, data->increaseExpRate,
		data->selectedGen, data->genLevel, data->genExp, data->increaseGenExpRate,
		data->genCapability[0], data->genCapability[1], data->genCapability[2], data->genCapability[3], data->genCapability[4], data->genCapability[5], data->genCapability[6], data->genCapability[7],
		data->force, data->soulpower, data->agility, data->vitality, data->bonusStatusPoint,
		data->curRA, data->curSA, data->curChargeNSE, data->curChargeSE, data->curChargeRA, data->curChargeSA,
		data->bindingIdx, data->itemNumber, data->inventory[0], data->inventory[1], data->inventory[2], data->equipment[13], data->hotkeyIdx[0],
		data->skill[0].tableIdx, data->skill[0].level,
		data->item[0].name, data->item[0].itemTableIdx, data->item[0].optionNumber, 
		data->item[0].optionIdx[0], data->item[0].optionIdx[1], data->item[0].optionIdx[2], data->item[0].optionIdx[3], data->item[0].optionIdx[4], data->item[0].optionIdx[5], 
		data->item[0].optionValue[0], data->item[0].optionValue[1], data->item[0].optionValue[2], data->item[0].optionValue[3], data->item[0].optionValue[4], data->item[0].optionValue[5], 
		data->item[0].durability, data->item[0].reqLevel, data->item[0].experience, 
		data->item[1].name, data->item[1].itemTableIdx, data->item[1].optionNumber, 
		data->item[1].optionIdx[0], data->item[1].optionIdx[1], data->item[1].optionIdx[2], data->item[1].optionIdx[3], data->item[1].optionIdx[4], data->item[1].optionIdx[5], 
		data->item[1].optionValue[0], data->item[1].optionValue[1], data->item[1].optionValue[2], data->item[1].optionValue[3], data->item[1].optionValue[4], data->item[1].optionValue[5], 
		data->item[1].durability, data->item[1].reqLevel, data->item[1].experience,
		data->item[2].name, data->item[2].itemTableIdx, data->item[2].optionNumber, 
		data->item[2].optionIdx[0], data->item[2].optionIdx[1], data->item[2].optionIdx[2], data->item[2].optionIdx[3], data->item[2].optionIdx[4], data->item[2].optionIdx[5], 
		data->item[2].optionValue[0], data->item[2].optionValue[1], data->item[2].optionValue[2], data->item[2].optionValue[3], data->item[2].optionValue[4], data->item[2].optionValue[5], 
		data->item[2].durability, data->item[2].reqLevel, data->item[2].experience

		
		);

	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = client->idx;
	strncpy(request.ownerID, client->id ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_CREATE_CHARACTER;
	strncpy(request.query, m_query , QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );

	return 1;
}

int CDBGame::DeleteCharacter(i3client_t *client, int characterID)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC DeleteCharacter '%s',%d,%d",client->id, client->serverGroupID, characterID);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx	= client->idx;
	strncpy(request.ownerID, client->id ,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType	= QUERY_DELETE_CHARACTER;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt		= m_hStmt;
	request.retCode		= -1;
	request.retStructVariable = (void *)client;

	if ( !runQueryExecuterThread ) return 0;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return 1;
}


int CDBGame::DeleteItem (i3client_t *client, int characterID, int idx)
{
	querySet_t	request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(m_query,QUERYSIZE, "EXEC DeleteItem '%s', %d, %d, %d",
		client->id, client->serverGroupID, characterID, idx);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = client->idx ;
	strncpy(request.ownerID, client->id,IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_DELETE_ITEM;
	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)client;

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
}


bool DB_ReconnectAccount ( querySet_t & request )
{
	g_DBAccountServer->Close () ;

	if ( g_DBAccountServer->Initialize( g_config.accountDatabaseDSN, g_config.odbcDSNType ) )
	{
		request.hStmt = g_DBAccountServer->GetStmt () ;
		return true ;
	}
	else
		return false ;
}


bool DB_ReconnectGame ( querySet_t & request )
{
	int i ;
	for ( i = 0 ; i < MAX_SERVER_GROUP ; ++i )
	{
		if (g_DBGameServer[i]->GetStmt () == request.hStmt )
			break ;
	}

	if ( i < MAX_SERVER_GROUP )
	{
		g_DBGameServer[i]->Close () ;

		if ( g_DBGameServer[i]->Initialize( g_config.gameDatabaseDSN[i], g_config.odbcDSNType ) )
		{
			request.hStmt = g_DBGameServer[i]->GetStmt () ;
			return true ;
		}
	}

	return false ;
}




void DB_QueryExecuterThread_Load_PremiumInfo(const querySet_t& request,querySet_t& result)
{
	RETCODE rCode=SQL_ERROR;
	long length=0;
	
	rCode = SQLFetch(request.hStmt); 
	if((SQL_ERROR == rCode) || (SQL_NO_DATA == rCode)){
		result.retCode = rCode;
		SQLCloseCursor(request.hStmt);		
		return; 
	}


	result.retCode = rCode;	
	SQLGetData(request.hStmt,1, SQL_C_SLONG,&(result.PremiumVar.iMemberShipType),sizeof(int), &length);
	SQLGetData(request.hStmt,2, SQL_C_SLONG,&(result.PremiumVar.iRemainSecond),sizeof(int), &length);
	SQLGetData(request.hStmt,3, SQL_C_SLONG,&(result.PremiumVar.WorldChatting.lDate),sizeof(int), &length);
	SQLGetData(request.hStmt,4, SQL_C_SLONG,&(result.PremiumVar.WorldChatting.iDecreaseCount),sizeof(int), &length);

	SQLCloseCursor(request.hStmt);	
}




void DB_ResultQuery_TerraCheck(const querySet_t& request, querySet_t& result);
DWORD WINAPI DB_QueryExecuterThread(LPVOID param)
{
	querySet_t		request, result;
	RETCODE			rCode;
	int				isExistQuery;

	queryRequestQueue = new CQueryQueue;
	queryResultQueue = new CQueryQueue;

	int count=0;
	runQueryExecuterThread = 1;
	while ( runQueryExecuterThread )
	{
		Sleep(1); //lucky CPU
		WaitForSingleObject( hQueryRequestMutex, INFINITE );
		if (queryRequestQueue->m_count > 0)
		{
			isExistQuery = queryRequestQueue->Pop( &request );
		}
		else
		{
			isExistQuery = 0;
		}
		ReleaseMutex( hQueryRequestMutex );

		count++;
		if(count > 699999)
		{
			TRACE("DB_QueryExecuterThread %d ... queryRequestQueue->m_count %d .. isExistQuery %d \n",timeGetTime(),queryRequestQueue->m_count,isExistQuery);
			count=0;
		}
		

		if ( !isExistQuery ) continue;
		isExistQuery = 0;

		
		rCode = SQLExecDirect(request.hStmt, (unsigned char *)&request.query, SQL_NTS);


		if ( rCode == SQL_ERROR )
		{
			if ( CheckDisconnect ( request.hStmt ) )
			{
				if ( DB_Reconnect ( request ) )
				{
					
					rCode = SQLExecDirect (request.hStmt, (unsigned char *)&request.query, SQL_NTS) ;
				}
			}
		}

		memcpy(&result, &request, sizeof(querySet_t));
		result.retCode = rCode;
		if ( result.retCode < 0){
			g_DBAccountServer->PrintError();


		}


		if ( result.retCode >= 0)
		{
			switch ( request.queryType )
			{
			case QUERY_TERRA_CHECK:
				DB_ResultQuery_TerraCheck(request,result);
				break;

			case QUERY_SAVESERVERSTATUS:
				Sleep(0);
				break;
			case QUERY_SAVE_CHARACTOR_POSTION:
				result.SaveCharactorPosition = request.SaveCharactorPosition;
				break;

			case QUERY_LOAD_PREMIUMINFO:
				DB_QueryExecuterThread_Load_PremiumInfo(request,result);
				break;

			case QUERY_CHECK_ACCOUNT:
			case QUERY_CHECK_ACCOUNT_HICOCOON:
				DB_ResultQuery_CheckAccount(request,result);
				break;
			
	


			case QUERY_CHECK_USERID:
				DB_ResultQuery_CheckUserID(&request, &result);
				break;
			case QUERY_INSERT_LOGIN:
				Sleep(0);
				break;
			case QUERY_DELETE_LOGIN:
				break;
			case QUERY_DELETE_LOGIN_HOST:
				break;

			case QUERY_CHECK_CHARACTER_NAME:
				DB_ResultQuery_CheckCharacterName(&request, &result);
				break;

			case QUERY_LOAD_ALL_GAME:
				DB_ResultQuery_LoadAllGame(&request, &result);
				break;
			case QUERY_LOAD_ALL_ITEM:
				DB_ResultQuery_LoadAllItem(&request, &result);
				break;

			case QUERY_LOAD_ALL_SKILL:
				DB_ResultQuery_LoadAllSkill(&request, &result);
				break;
			case QUERY_LOAD_ALL_QUEST:
				DB_ResultQuery_LoadAllQuest(&request, &result);
				break;
			case QUERY_LOAD_ALL_QUESTSTATUS:
				DB_ResultQuery_LoadAllQuestStatus(&request, &result);
				break;

			case QUERY_CREATE_CHARACTER:
				break;

			case QUERY_DELETE_CHARACTER:
				break;

			case QUERY_UPDATE_GAMEOWNITEMINFO:
				break;

			case QUERY_INSERT_SERVER_GROUP_USERS:
				Sleep(0);
				break;

			
			case QUERY_DELETE_ITEM:
				break ;

			
			case QUERY_LOAD_CHARACTER_SLOT:
				DB_ResultQuery_LoadCharaterSlot(request,result);
				break;

			
			case QUERY_SAVE_CHARACTER_SLOT:
				break;

			default:
				break;
			}
		}



		SQLCloseCursor( request.hStmt );

		
		WaitForSingleObject( hQueryResultMutex, INFINITE );
		queryResultQueue->Push( &result );
		ReleaseMutex( hQueryResultMutex );
	}

	
	if ( queryRequestQueue )    { delete queryRequestQueue;   queryRequestQueue = NULL; }
	if ( queryResultQueue )    { delete queryResultQueue;   queryResultQueue = NULL; }

	return 0;
} 


#include "CTools.h"
#include "tagMCPacket.h"



extern void TransmitePacket_MSC_GAMESERVERADDR(i3client_t* pClient,const int SlotIdx);
void DB_ProcessQueryResultQueue_QUERY_SAVE_CHARACTOR_POSTION(const querySet_t& result)
{
	if(result.retCode < 0){
		Sleep(0);
		return;
	}
	i3client_t* pClient = CTools::GetClientRecord(result.ownerIdx);
	if(NULL == pClient) return;

	pClient->isComplete = true;	
	g_DBAccountServer->DeleteLogin(pClient);
	TransmitePacket_MSC_GAMESERVERADDR(pClient,result.SaveCharactorPosition.CharacterID);
}





void DB_ProcessQueryResultQueue_QUERY_LOAD_CHARACTER_SLOT(const querySet_t& result)
{
	if (result.retCode < 0){
		g_logSystem->Write("Database Error: QUERY_LOAD_CHARACTER_SLOT, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
	}

	i3client_t* pClient=NULL;
	pClient = CTools::GetClientRecord(result.ownerIdx);
	if(NULL == pClient) return;
	if(0 != strcmp(pClient->id,result.ownerID)) return;

	pClient->m_nCharacterSlotType = result.LoadCharaterSlot.iCharacterSlotType;
	pClient->m_tmDate = result.LoadCharaterSlot.tmDate;

	int day = 0;
	if(pClient->m_nCharacterSlotType == LOGIN_CHAR_PU)
	{
		day = gDays.GetDays(pClient->m_tmDate);
		if(day < 0) day=0;
	}

	
	GTH_SendCharacterLoadSlot(pClient,day);	
}














void DB_ProcessQueryResultQueue_QUERY_LOAD_PREMIUMINFO(const querySet_t& result)
{
	i3client_t* pClient = CTools::GetClientRecord(result.ownerIdx);
	if(NULL == pClient) return;

	gcpPremiumPlayerCtrl->SetData(result.PremiumVar.iMemberShipType,pClient);
	pClient->PremiumInfo.iRemainSecond = result.PremiumVar.iRemainSecond;
	pClient->PremiumInfo.WorldChatting.lDate = result.PremiumVar.WorldChatting.lDate;
	pClient->PremiumInfo.WorldChatting.iDecreaseCount = result.PremiumVar.WorldChatting.iDecreaseCount;
}




void DB_ProcessQueryResultQueue_QUERY_CHECK_ACCOUNT(const querySet_t& result)
{
	i3client_t* pClientRecord = CTools::GetClientRecord(result.ownerIdx);
	if(NULL == pClientRecord){
		g_logSystem->Write("활성화되지 않은(Not Active) 클라이언트의 쿼리가 실행됨:QUERY_CHECK_ACCOUNT, ownerIdx: %d)", result.ownerIdx);
		return;
	}

	if(result.retCode < 0){
		g_logSystem->Write("Database Error: QUERY_CHECK_ACCOUNT, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
		GTH_ReplyLoginV2(pClientRecord,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::database_error);
		return;
	}

	pClientRecord->dbCheckAccount = result.CheckAccountVar.dbCheckAccount;
	pClientRecord->Identify       = result.CheckAccountVar.Identity;

	
	if(pClientRecord->isMonitor){	
		switch(pClientRecord->dbCheckAccount){
			case 9:		
				GTH_ReplyLoginV2(pClientRecord,TRUE,tagMCPaket_MSC_REPLYLOGIN::enumError::none);
				pClientRecord->isSuccessLogin = true; 
				g_logSystem->Write("서버 모니터링 클라이언트가 접속했습니다.(ID:%s, IP:%s)", pClientRecord->id, NET_AddrToString((struct sockaddr *)&pClientRecord->sock.addr));
				return;
			case -1:
				g_DBAccountServer->CheckUserID(pClientRecord);
				return;

			default:
				
				GTH_ReplyLoginV2(pClientRecord,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::block_user);				
				return;
		}
		return;
	}

	
	if(0 == pClientRecord->dbCheckAccount){	
		
		GTH_ReplyLoginV2( pClientRecord,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::block_user);		
		return;
	}

	
	
	if(pClientRecord->dbCheckAccount > 0 ){		
		
		
		
		if (g_config.bCheckBilling){
			Billing_SendAuth(pClientRecord->id);
			return;
		}

		if(NULL != gcpTerraBillingCtrl_Encder){
			gcpTerraBillingCtrl_Encder->Transmite_Billing_Authorization(pClientRecord);
			return;
		}

			g_DBAccountServer->InsertLogin(pClientRecord);
			g_logSystem->Write("%s님이 접속(%d)했습니다.(%s)", pClientRecord->id, pClientRecord->dbCheckAccount, NET_AddrToString((struct sockaddr *)&pClientRecord->sock.addr));
		return;		
	}	

	
	g_DBAccountServer->CheckUserID(pClientRecord);

}


void DB_ProcessQueryResultQueue_QUERY_INSERT_LOGIN(const querySet_t& result)
{
	i3client_t* pClient = CTools::GetClientRecord(result.ownerIdx);
	if(NULL == pClient){
		g_logSystem->Write("존재하지 않는 사용자: QUERY_INSERT_LOGIN, ownerIdx: %d)", result.ownerIdx);
		return;
	}	
	
	if(0 != stricmp(result.ownerID,pClient->id))
	{
		g_logSystem->Write("쿼리의 소유자 변경 오류(QUERY_INSERT_LOGIN, OWNER:%s, CLIENT:%s)", result.ownerID, pClient->id);
		
		return;
	}

	if(result.retCode < 0){
		g_logSystem->Write("중복 로그인 오류(ID: %s)", pClient->id);
		
		GTH_ReplyLoginV2( pClient,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::double_login);
		
	}else{


		switch(pClient->Identify.Certification){			
			case i3client_t::tagIdentification::enumCertification::real_name:
			case i3client_t::tagIdentification::enumCertification::developer:
			case i3client_t::tagIdentification::enumCertification::unknown:
				
				GTH_ReplyLoginV2(pClient,TRUE,tagMCPaket_MSC_REPLYLOGIN::enumError::none);
				pClient->isSuccessLogin = true; 
				break;

			default:
			case i3client_t::tagIdentification::enumCertification::nick_name:

				if(TRUE == g_config.bBlockNotRealNameUser){
					
					GTH_ReplyLoginV2(pClient, FALSE, tagMCPaket_MSC_REPLYLOGIN::enumError::not_check_real_name);
						

					
				}else{
					
					GTH_ReplyLoginV2(pClient, TRUE, tagMCPaket_MSC_REPLYLOGIN::enumError::none);
					pClient->isSuccessLogin = true; 
				}
				return;		
		}	

		
	}
}



void DB_ProcessQueryResultQueue_QUERY_TERRA_CHECK(const querySet_t& result);
void DB_ProcessQueryResultQueue()
{
	querySet_t			result;
	int					isExist;
	i3client_t			*client;
	int					processedQueryCount;
	int i;
	char delCharName[NAMESTRING];


	if ( !runQueryExecuterThread ) return;

	processedQueryCount = 0;
	while ( processedQueryCount < MAX_QUERY_PER_FRAME )		
	{
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

		processedQueryCount++;

		switch ( result.queryType )
		{
		case QUERY_TERRA_CHECK:
			DB_ProcessQueryResultQueue_QUERY_TERRA_CHECK(result);
			break;

		case QUERY_SAVESERVERSTATUS:
			Sleep(0);
			break;
		case QUERY_SAVE_CHARACTOR_POSTION:
			DB_ProcessQueryResultQueue_QUERY_SAVE_CHARACTOR_POSTION(result);
			break;


		case QUERY_LOAD_PREMIUMINFO:
			DB_ProcessQueryResultQueue_QUERY_LOAD_PREMIUMINFO(result);
			break;


		case QUERY_CHECK_ACCOUNT:
		case QUERY_CHECK_ACCOUNT_HICOCOON:
			DB_ProcessQueryResultQueue_QUERY_CHECK_ACCOUNT(result);
			break;

			

		case QUERY_CHECK_USERID:
			client = &g_clients[result.ownerIdx];
			if ( !client->active ) 
			{
				g_logSystem->Write("활성화되지 않은(Not Active) 클라이언트의 쿼리가 실행됨:QUERY_CHECK_USERID, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_CHECK_USERID, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				
				GTH_ReplyLoginV2(client, FALSE, tagMCPaket_MSC_REPLYLOGIN::enumError::database_error);
				continue;
			}
			if ( client->dbCheckID > 0)	
				
				GTH_ReplyLoginV2(client, FALSE, tagMCPaket_MSC_REPLYLOGIN::enumError::mismatch_password);
			else
				
				GTH_ReplyLoginV2(client, FALSE, tagMCPaket_MSC_REPLYLOGIN::enumError::unregister_user);
			break;



		case QUERY_LOAD_ALL_GAME:
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_LOAD_ALL_GAME, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			client = &g_clients[result.ownerIdx];
			if ( !client->active ) 
			{
				g_logSystem->Write("활성화되지 않은(Not Active) 클라이언트의 쿼리가 실행됨:QUERY_LOAD_ALL_GAME, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, client->id) != 0 )
			{
				g_logSystem->Write("쿼리의 소유자 변경 오류(QUERY_LOAD_ALL_GAME, OWNER:%s, CLIENT:%s)", result.ownerID, client->id);
				break;
			}
			break;

		case QUERY_LOAD_ALL_SKILL:
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_LOAD_ALL_SKILL, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			client = &g_clients[result.ownerIdx];
			if ( !client->active ) 
			{
				g_logSystem->Write("활성화되지 않은(Not Active) 클라이언트의 쿼리가 실행됨:QUERY_LOAD_ALL_SKILL, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, client->id) != 0 )
			{
				g_logSystem->Write("쿼리의 소유자 변경 오류(QUERY_LOAD_ALL_SKILL, OWNER:%s, CLIENT:%s)", result.ownerID, client->id);
				break;
			}
			break;

		case QUERY_LOAD_ALL_ITEM:
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_LOAD_ALL_ITEM, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			client = &g_clients[result.ownerIdx];
			if ( !client->active ) 
			{
				g_logSystem->Write("활성화되지 않은(Not Active) 클라이언트의 쿼리가 실행됨:QUERY_LOAD_ALL_ITEM, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, client->id) != 0 )
			{
				g_logSystem->Write("쿼리의 소유자 변경 오류(QUERY_LOAD_ALL_ITEM, OWNER:%s, CLIENT:%s)", result.ownerID, client->id);
				break;
			}
			
			GTH_CheckGameData( client );
			
			GTH_SendGameData( client );
			break;

		case QUERY_INSERT_LOGIN:
			DB_ProcessQueryResultQueue_QUERY_INSERT_LOGIN(result);
			break;

		case QUERY_DELETE_LOGIN:
			if ( result.retCode < 0)
			{
				g_DBAccountServer->PrintError();
				g_logSystem->Write("Database Error: QUERY_DELETE_LOGIN, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
			}
			break;

		case QUERY_CHECK_CHARACTER_NAME:
			client = &g_clients[result.ownerIdx];
			if ( !client->active ) 
			{
				g_logSystem->Write("활성화되지 않은(Not Active) 클라이언트의 쿼리가 실행됨:QUERY_CHECK_CHARACTER_NAME, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, client->id) != 0 )
			{
				g_logSystem->Write("쿼리의 소유자 변경 오류(QUERY_CHECK_CHARACTER_NAME, OWNER:%s, CLIENT:%s)", result.ownerID, client->id);
				break;
			}

			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_CHECK_CHARACTER_NAME, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				GTH_ReplyCheckCharacterName( client, 0 ); 
				continue;
			}

			if ( client->dbCheckCharacterName > 0)	
				GTH_ReplyCheckCharacterName( client, 0 );
			else
				GTH_ReplyCheckCharacterName( client, 1 );
			break;


		case QUERY_UPDATE_GAMEOWNITEMINFO:
			client = &g_clients[result.ownerIdx];
			if ( !client->active ) 
			{
				g_logSystem->Write("활성화되지 않은(Not Active) 클라이언트의 쿼리가 실행됨:QUERY_UPDATE_GAMEOWNITEMINFO, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, client->id) != 0 )
			{
				g_logSystem->Write("쿼리의 소유자 변경 오류(QUERY_UPDATE_GAMEOWNITEMINFO, OWNER:%s, CLIENT:%s)", result.ownerID, client->id);
				break;
			}

			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_UPDATE_GAMEOWNITEMINFO, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
			}
			break;

		case QUERY_DELETE_LOGIN_HOST:
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_DELETE_LOGIN_HOST, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
			}
			break;

		case QUERY_LOAD_ALL_QUEST:
			client = &g_clients[result.ownerIdx];
			if ( !client->active ) 
			{
				g_logSystem->Write("활성화되지 않은(Not Active) 클라이언트의 쿼리가 실행됨:QUERY_LOAD_ALL_QUEST, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, client->id) != 0 )
			{
				g_logSystem->Write("쿼리의 소유자 변경 오류(QUERY_LOAD_ALL_QUEST, OWNER:%s, CLIENT:%s)", result.ownerID, client->id);
				break;
			}
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_LOAD_ALL_QUEST, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
			}
			break;

		case QUERY_LOAD_ALL_QUESTSTATUS:
			client = &g_clients[result.ownerIdx];
			if ( !client->active ) 
			{
				g_logSystem->Write("활성화되지 않은(Not Active) 클라이언트의 쿼리가 실행됨:QUERY_LOAD_ALL_QUESTSTATUS, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, client->id) != 0 )
			{
				g_logSystem->Write("쿼리의 소유자 변경 오류(QUERY_LOAD_ALL_QUESTSTATUS, OWNER:%s, CLIENT:%s)", result.ownerID, client->id);
				break;
			}
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_LOAD_ALL_QUESTSTATUS, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
			}
			break;

		case QUERY_DELETE_CHARACTER:
			client = &g_clients[result.ownerIdx];
			if ( !client->active ) 
			{
				g_logSystem->Write("활성화되지 않은(Not Active) 클라이언트의 쿼리가 실행됨:QUERY_DELETE_CHARACTER, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, client->id) != 0 )
			{
				g_logSystem->Write("쿼리의 소유자 변경 오류(QUERY_DELETE_CHARACTER, OWNER:%s, CLIENT:%s)", result.ownerID, client->id);
				break;
			}
			if ( result.retCode < 0)
			{
				GTH_SendMessage_ReplyDeleteCharacter(client, 0);
				g_logSystem->Write("Database Error: QUERY_DELETE_CHARACTER, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}

			GTH_SendMessage_ReplyDeleteCharacter(client, 1);

			
			strcpy(delCharName, "");
			for (i=0; i<client->dataNumber; i++)
			{
				if (client->data[i].characterID == client->deleteCharacterID )
				{
					strcpy(delCharName, client->data[i].name);
					break;
				}
			}
			g_logSystem->Write("게임데이터 삭제 ID: %s, serverGroupID: %d, characterID: %d, %s", 
				client->id, client->serverGroupID, client->deleteCharacterID, delCharName);

#ifdef _TEST_SERVER	
			g_logSystem->WriteToLog("%d;%s;%s;;%d;;;;;;;;;;;;;;;%s;", 
				255, client->id, delCharName, LOG_CLASS_PCDELETE, NET_AddrToIPString((struct sockaddr *)&client->sock.addr) );
#else
			g_logSystem->WriteToLog("%d;%s;%s;;%d;;;;;;;;;;;;;;;%s;", 
				client->serverGroupID, client->id, delCharName, LOG_CLASS_PCDELETE, NET_AddrToIPString((struct sockaddr *)&client->sock.addr) );
#endif
			break;

		case QUERY_CREATE_CHARACTER:
			client = &g_clients[result.ownerIdx];
			if ( !client->active ) 
			{
				g_logSystem->Write("활성화되지 않은(Not Active) 클라이언트의 쿼리가 실행됨:QUERY_CREATE_CHARACTER, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, client->id) != 0 )
			{
				g_logSystem->Write("쿼리의 소유자 변경 오류(QUERY_CREATE_CHARACTER, OWNER:%s, CLIENT:%s)", result.ownerID, client->id);
				break;
			}
			if ( result.retCode < 0)
			{
				GTH_SendMessage_ReplyCreateCharacter(client, 0);
				g_logSystem->Write("Database Error: QUERY_CREATE_CHARACTER, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}

			g_logSystem->Write("게임데이터 생성 ID: %s, serverGroupID: %d, characterID: %d, name: %s", 
				client->id, client->serverGroupID, client->createData.characterID, client->createData.name);
#ifdef _TEST_SERVER	
			g_logSystem->WriteToLog("%d;%s;%s;;%d;;;;;;;;;;%d;;;;;%s;", 
				255, client->id, client->createData.name, LOG_CLASS_PCCREATE,  client->createData.pcClass, NET_AddrToIPString((struct sockaddr *)&client->sock.addr) );
#else
			g_logSystem->WriteToLog("%d;%s;%s;;%d;;;;;;;;;;%d;;;;;%s;", 
				client->serverGroupID, client->id, client->createData.name, LOG_CLASS_PCCREATE,  client->createData.pcClass, NET_AddrToIPString((struct sockaddr *)&client->sock.addr) );
#endif

			GTH_SendMessage_ReplyCreateCharacter(client, 1);
			break;

		case QUERY_INSERT_SERVER_GROUP_USERS:
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_INSERT_SERVER_GROUP_USERS, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
			}
			break;

		
		case QUERY_DELETE_ITEM:
			if (result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_DELETE_ITEM, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
			}
			break ;

		
		case QUERY_LOAD_CHARACTER_SLOT:
			DB_ProcessQueryResultQueue_QUERY_LOAD_CHARACTER_SLOT(result);
			break;
			

		
		case QUERY_SAVE_CHARACTER_SLOT:
			if (result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_SAVE_CHARACTER_SLOT, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
			}
			break ;
		default:
			break;
		}

	} 
}





void DB_ResultQuery_CheckUserID(querySet_t *request, querySet_t *result)
{
	i3client_t	*client;
	RETCODE rCode;
	long	length;


	
	rCode = SQLFetch(request->hStmt);        
	if (rCode == 100) 
	{
		result->retCode = rCode;
		return; 
	}

	result->retCode = rCode;
	client = (i3client_t *)request->retStructVariable;

	SQLGetData(request->hStmt, 1, SQL_C_SLONG, &client->dbCheckID, COLUMNSIZE, &length);

}


void DB_ResultQuery_LoadAllGame(querySet_t *request, querySet_t *result)
{
	i3client_t		*client;
	characterData_t *data;
	RETCODE			rCode;
	long			length;
	int				column;
	int				dataNumber;
	int				i;
	int				hotKey;

	
	rCode = SQLFetch(request->hStmt);        
	if (rCode == 100) 
	{
		result->retCode = rCode;
		return; 
	}

	result->retCode = rCode;
	client = (i3client_t *)request->retStructVariable;

	dataNumber = 0;
	for (; rCode == SQL_SUCCESS; rCode = SQLFetch(request->hStmt))
	{
		
		data = &client->data[dataNumber];
		dataNumber ++;

		column = 1;
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->characterID, COLUMNSIZE, &length); 
		SQLGetData(request->hStmt, column++, SQL_C_CHAR, data->name, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->pcClass, COLUMNSIZE, &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->pcJob, COLUMNSIZE, &length); 

		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->skinPart[0], COLUMNSIZE, &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->skinPart[1], COLUMNSIZE, &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->skinPart[2], COLUMNSIZE, &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->skinPart[3], COLUMNSIZE, &length); 

		SQLGetData(request->hStmt, column++, SQL_C_CHAR, data->rank, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->rankPoint, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->age, COLUMNSIZE, &length);

		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->worldIdx, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_FLOAT, &data->position[0], COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_FLOAT, &data->position[1], COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_FLOAT, &data->position[2], COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_FLOAT, &data->angles[1], COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->level, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SBIGINT, &data->exp, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->increaseExpRate, COLUMNSIZE, &length);

		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->selectedGen, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->genLevel, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SBIGINT, &data->genExp, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->increaseGenExpRate, COLUMNSIZE, &length);
		for (i=0; i<GEN_NUMBER; i++)
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->genCapability[i], COLUMNSIZE, &length);

		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->force, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->soulpower, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->agility, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->vitality, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->bonusStatusPoint, COLUMNSIZE, &length);

		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->curRA, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->curSA, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->curChargeNSE, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->curChargeSE, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->curChargeRA, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->curChargeSA, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->depotSE, COLUMNSIZE, &length);

		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->curWeapon, COLUMNSIZE, &length);

		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->itemNumber, COLUMNSIZE, &length);

		for (i=0; i<MAX_INVENTORY_SIZE; i++)
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->inventory[i], COLUMNSIZE, &length);

		for (i=0; i<MAX_DEPOT_SIZE; i++)
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->depot[i], COLUMNSIZE, &length);

		for (i=0; i<MAX_EQUIPMENT; i++)
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->equipment[i], COLUMNSIZE, &length);

		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->mouseInventory, COLUMNSIZE, &length);

		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->bonusSkillPoint, COLUMNSIZE, &length);

		for (i=0; i<MAX_NUMBER_OF_SKILL_VARIABLE; i++)
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->skillVariable[i], COLUMNSIZE, &length);

		
		for (i=0; i<MAX_NUMBER_OF_HOTKEY; i++)
		{
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &hotKey, COLUMNSIZE, &length);
			if (hotKey >= MASK_HOTKEY_ITEM)
			{
				data->hotkeyType[i] = 2;	
				data->hotkeyIdx[i] = hotKey - MASK_HOTKEY_ITEM;
			}
			else if (hotKey >= MASK_HOTKEY_ACTION)
			{
				data->hotkeyType[i] = 1;	
				data->hotkeyIdx[i] = hotKey - MASK_HOTKEY_ACTION;
			}
			else
			{
				data->hotkeyType[i] = 0;	
				data->hotkeyIdx[i] = hotKey;
			}
		}
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->questPoint, COLUMNSIZE, &length);

		
		
		for (i=0; i<MAX_PRECOCITY_SIZE; i++)
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->precocityInventory[i], COLUMNSIZE, &length);
		for (i=0; i<MAX_PRECOCITY_SIZE; i++)
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->precocityTime[i], COLUMNSIZE, &length);
		
		
		
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->chaosPoint, COLUMNSIZE, &length);
		
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->guildIdx, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->guildJoinTime, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &data->guildSecedeTime, COLUMNSIZE, &length);
	}

	client->dataNumber = dataNumber;   
}


void DB_ResultQuery_LoadAllItem(querySet_t *request, querySet_t *result)
{
	i3client_t		*client;
	characterData_t *data;
	item_t			*item;
	RETCODE			rCode;
	long			length;
	int				column;

	int				itemIdx;
	int				characterID;
	int				i;

	
	rCode = SQLFetch(request->hStmt);        
	if (rCode == 100) 
	{
		result->retCode = rCode;

		return; 
	}

	result->retCode = rCode;
	client = (i3client_t *)request->retStructVariable;

	for (; rCode == SQL_SUCCESS; rCode = SQLFetch(request->hStmt) )
	{
		column = 1;
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &characterID, COLUMNSIZE, &length); 

		data = NULL;
		for (i=0; i<client->dataNumber; i++)			
		{
			if (client->data[i].characterID == characterID)
			{
				data = &client->data[i];
				break;
			}
		}
		if ( data == NULL ) 
		{
			g_logSystem->Write("데이터베이스 무결성 오류: 아이템, %s, %d, %d", client->id, client->serverGroupID, characterID);		
			continue;
		}


		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &itemIdx, COLUMNSIZE, &length);

		item = &data->item[itemIdx];

		SQLGetData(request->hStmt, column++, SQL_C_CHAR,  item->name, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->itemTableIdx, COLUMNSIZE, &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->optionNumber, COLUMNSIZE, &length); 

		for (i=0; i<ITEM_OPTION_NUMBER; i++)
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &item->optionIdx[i], COLUMNSIZE, &length); 

		for (i=0; i<ITEM_OPTION_NUMBER; i++)
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
}

void DB_ResultQuery_LoadAllSkill(querySet_t *request, querySet_t *result)
{
	i3client_t		*client;
	skill_t			*skill;
	RETCODE			rCode;
	long			length;
	int				column;
	int				i;
	int				characterID;
	int				skillNumber;
	characterData_t *data;

	
	rCode = SQLFetch(request->hStmt);        
	if (rCode == 100) 
	{
		result->retCode = rCode;
		return; 
	}

	result->retCode = rCode;
	client = (i3client_t *)request->retStructVariable;

	for (; rCode == SQL_SUCCESS; rCode = SQLFetch(request->hStmt))
	{
		column = 1;
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &characterID, COLUMNSIZE, &length); 

		data = NULL;
		for (i=0; i<client->dataNumber; i++)			
		{
			if (client->data[i].characterID == characterID)
			{
				data = &client->data[i];
				break;
			}
		}

		if ( data == NULL ) 
		{
			g_logSystem->Write("데이터베이스 무결성 오류: 스킬, %s, %d, %d", client->id, client->serverGroupID, characterID);		
			continue;
		}


		skill = &data->skill[0];
		skillNumber = 0;
		for (i = 0; i<MAX_NUMBER_OF_SKILL; i++, skill++)
		{
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &skill->tableIdx, COLUMNSIZE, &length); 
			if ( skill->tableIdx >= 0 ) skillNumber ++;
		}

		skill = &data->skill[0];
		for (i = 0; i<MAX_NUMBER_OF_SKILL; i++, skill++)
		{
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &skill->iLevel, COLUMNSIZE, &length); 
		}
		
		data->skillNumber = skillNumber;
	}
}


void DB_ResultQuery_CheckCharacterName(querySet_t *request, querySet_t *result)
{
	i3client_t	*client;
	RETCODE rCode;
	long	length;


	
	rCode = SQLFetch(request->hStmt);        
	if (rCode == 100) 
	{
		result->retCode = rCode;
		return; 
	}

	result->retCode = rCode;
	client = (i3client_t *)request->retStructVariable;

	SQLGetData(request->hStmt, 1, SQL_C_SLONG, &client->dbCheckCharacterName, COLUMNSIZE, &length);

}



void DB_ResultQuery_LoadAllQuest(querySet_t *request, querySet_t *result)
{
	i3client_t		*client;
	RETCODE			rCode;
	long			length;
	int				column;
	int				i;
	int				characterID;
	characterData_t *data;

	questVariable_t		*questVar;
	questInventory_t	*questInventory;


	
	rCode = SQLFetch(request->hStmt);        
	if (rCode == 100) 
	{
		result->retCode = rCode;
		return; 
	}

	result->retCode = rCode;
	client = (i3client_t *)request->retStructVariable;

	for (; rCode == SQL_SUCCESS; rCode = SQLFetch(request->hStmt))
	{
		column = 1;
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &characterID, COLUMNSIZE, &length); 

		data = NULL;
		for (i=0; i<client->dataNumber; i++)			
		{
			if (client->data[i].characterID == characterID)
			{
				data = &client->data[i];
				break;
			}
		}

		if ( data == NULL ) 
		{
			g_logSystem->Write("데이터베이스 무결성 오류: 퀘스트, %s, %d, %d", client->id, client->serverGroupID, characterID);		
			continue;
		}

		questVar = &data->questVar[0];
		for (i = 0; i<MAX_COUNT_QUEST_VARIABLE; i++, questVar++)
		{
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &questVar->iVar, COLUMNSIZE, &length); 
		}

		questInventory = &data->questInventory[0];
		for (i = 0; i<MAX_COUNT_QUEST_INVENTORY; i++, questInventory++)
		{
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &questInventory->iItem, COLUMNSIZE, &length); 
		}
	}
}

void DB_ResultQuery_LoadAllQuestStatus(querySet_t *request, querySet_t *result)
{
	i3client_t		*client;
	characterData_t *data;
	RETCODE			rCode;
	long			length;
	int				column;

	int				characterID;
	int				i;

	questStatus_t	*questStatus;

	int				idx;

	
	rCode = SQLFetch(request->hStmt);        
	if (rCode == 100) 
	{
		result->retCode = rCode;
		return; 
	}

	result->retCode = rCode;
	client = (i3client_t *)request->retStructVariable;

	for (; rCode == SQL_SUCCESS; rCode = SQLFetch(request->hStmt) )
	{
		column = 1;
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &characterID, COLUMNSIZE, &length); 

		data = NULL;
		for (i=0; i<client->dataNumber; i++)			
		{
			if (client->data[i].characterID == characterID)
			{
				data = &client->data[i];
				break;
			}
		}
		if ( data == NULL ) 
		{
			g_logSystem->Write("데이터베이스 무결성 오류: 퀘스트상태, %s, %d, %d", client->id, client->serverGroupID, characterID);		
			continue;
		}

		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &idx, COLUMNSIZE, &length);

		if ( idx < 0 || idx >= MAX_COUNT_QUEST_STATUS) 
		{
			g_logSystem->Write("퀘스트 진행 데이터 로딩중 인덱스 범위를 초과한 데이터가 있습니다.(%s, %d, %d)",
				client->id, data->characterID, idx);
			continue;
		}

		data->questNumber ++; 
		questStatus = &data->questStatus[idx];	

		SQLGetData(request->hStmt, column++, SQL_C_SLONG,  &questStatus->questIdx, COLUMNSIZE, &length);
		for (i=0; i<MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD; i++)
			SQLGetData(request->hStmt, column++, SQL_C_SLONG, &questStatus->dField[i], COLUMNSIZE, &length);

		SQLGetData(request->hStmt, column++, SQL_C_CHAR,  questStatus->partyOrganizerName, COLUMNSIZE, &length);
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &questStatus->partyOrganizeServer, COLUMNSIZE, &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &questStatus->partyOrganizeTime, COLUMNSIZE, &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &questStatus->clearTime, COLUMNSIZE, &length); 
		SQLGetData(request->hStmt, column++, SQL_C_SLONG, &questStatus->limitTime, COLUMNSIZE, &length); 
	}   
	
}



void DB_ResultQuery_LoadCharaterSlot(querySet_t& request, querySet_t& result)
{
	i3client_t* pClient=NULL;
	RETCODE		rCode=SQL_ERROR;
	long		length=0;	
	const int bytes_date_string=31;
	char date_string[bytes_date_string+1]="1900-01-01-00";

	
	rCode = SQLFetch(request.hStmt);        
	if((rCode == SQL_ERROR) || (rCode == SQL_NO_DATA))
	{
		result.retCode = rCode;
		rCode = SQLCloseCursor( request.hStmt );
		if((rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
			
		}
		return;
	}

	result.retCode = rCode;	
	SQLGetData(request.hStmt, 1, SQL_C_SLONG,&(result.LoadCharaterSlot.iCharacterSlotType),sizeof(int), &length);		
	SQLGetData(request.hStmt, 2, SQL_C_CHAR,date_string,bytes_date_string, &length);
	date_string[bytes_date_string]=NULL;

	memset(&(result.LoadCharaterSlot.tmDate),0x00,sizeof(struct tm));	
	date_string[4]=NULL;  result.LoadCharaterSlot.tmDate.tm_year  = atoi(date_string);
	date_string[7]=NULL;  result.LoadCharaterSlot.tmDate.tm_mon   = atoi(&date_string[5]);
	date_string[10]=NULL; result.LoadCharaterSlot.tmDate.tm_mday  = atoi(&date_string[8]);

	SQLCloseCursor( request.hStmt );	
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
			g_logSystem->Write("데이터베이스 쓰레드에서 모든 쿼리를 실행하지 못했습니다.");
			break;
		}
		remainderQueryNumber = queryRequestQueue->m_count;

		
		DB_ProcessQueryResultQueue();
	} 
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


void  TRACE(LPCTSTR lpszFormat, ...)
{
#ifdef _DEBUG
	va_list argList;
	va_start(argList, lpszFormat);

	char szOutputMessage[1024];

	vsprintf(szOutputMessage, lpszFormat, argList);
	va_end(argList);

	OutputDebugString ( szOutputMessage );
#endif

}











CTerraDBCtrl::CTerraDBCtrl()
{

}




CTerraDBCtrl::~CTerraDBCtrl()
{

}





void CTerraDBCtrl::SelectTmember(i3client_t* pClient,const char* pTID) 
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"select TLoginName from tempuser where UTid = '%s'",pTID);
	m_query[QUERYSIZE]=NULL;		

	request.ownerIdx = pClient->idx;
	strncpy(request.ownerID, "",IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_TERRA_CHECK;
	strncpy(request.query, m_query , QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pClient;

	if ( !runQueryExecuterThread ) return;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );	
	return;
}




void DB_ResultQuery_TerraCheck(const querySet_t& request, querySet_t& result)
{
	RETCODE rCode= SQL_ERROR;
	long	length=0;

	
	rCode = SQLFetch(request.hStmt);        
	if((SQL_ERROR == rCode) || (SQL_NO_DATA == rCode)){
		result.retCode = rCode;	
		g_logSystem->Write("Terra Check SQL_NO_DATA [%s]",request.query);
		
		rCode = SQLCloseCursor( request.hStmt );
		if((rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
			
		}
		return;
	}

	result.retCode = rCode;
	SQLGetData(request.hStmt, 1, SQL_C_CHAR,result.TerraCheck.TLoginName,
		i3client_t::tagTerra::MaxBytesTLoginName,
		&length);
	result.TerraCheck.TLoginName[i3client_t::tagTerra::MaxBytesTLoginName]=NULL;		


	SQLCloseCursor( request.hStmt );
}


void DB_ProcessQueryResultQueue_QUERY_TERRA_CHECK(const querySet_t& result)
{
	i3client_t* pClient=NULL;
	pClient = &g_clients[result.ownerIdx];		


	if(NULL == pClient->id[0]) return;
	if(!pClient->active){
		g_logSystem->Write("활성화되지 않은(Not Active) 클라이언트의 쿼리가 실행됨:QUERY_TERRA_CHECK, ownerIdx: %d)", result.ownerIdx);
		return;
	}	

	if(SQL_NO_DATA == result.retCode){
		pClient->dbCheckAccount = -1;
		GTH_ReplyLoginV2(pClient,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::unregister_user);		
		return;
	}
	if(result.retCode != SQL_SUCCESS){
		pClient->dbCheckAccount = -1;
		GTH_ReplyLoginV2(pClient,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::database_error);
		g_logSystem->Write("Database Error: QUERY_TERRA_CHECK, ownerIdx: %d, query: %s", result.ownerIdx, result.query);	
		return;
	}


	strncpy(pClient->Terra.TLoginName,
		result.TerraCheck.TLoginName,
		i3client_t::tagTerra::MaxBytesTLoginName);
	pClient->Terra.TLoginName[i3client_t::tagTerra::MaxBytesTLoginName]=NULL;

	strncpy(pClient->id,pClient->Terra.TLoginName,IDSTRING);
	pClient->id[IDSTRING]=NULL;


	g_DBAccountServer->MalCheckAccount(pClient);	
}





void CDBAccount::CheckAccountHicocoon(i3client_t *pClient, char *id)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC CheckAccountHicocoon '%s'", id);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pClient->idx;
	strncpy(request.ownerID, "",IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_CHECK_ACCOUNT_HICOCOON;
	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pClient;
	

	if ( !runQueryExecuterThread ) return;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );	
	return;

}

void CDBAccount::MalCheckAccount(i3client_t* pClient) 
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE,"EXEC CheckAccount '%s'",pClient->Terra.TLoginName);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pClient->idx;
	strncpy(request.ownerID, "",IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_CHECK_ACCOUNT;
	strncpy(request.query, m_query, QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pClient;
	

	if ( !runQueryExecuterThread ) return;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );	
	return;
}







bool DB_ReconnectTerra(querySet_t & request )
{
	
	g_TerraDBCtrl->Close() ;

	if ( g_TerraDBCtrl->Initialize( g_config.TerraV2.MemberShipDSN, g_config.odbcDSNType,"TGTH","gth%terra6345"))
	{
		request.hStmt = g_TerraDBCtrl->GetStmt () ;
		return TRUE;
	}
	else
		return FALSE;
}




bool DB_Reconnect ( querySet_t & request )
{
	
	if((NULL != g_TerraDBCtrl) && (g_TerraDBCtrl->GetStmt() == request.hStmt)){
		return DB_ReconnectTerra(request);
	}
	

	
	if ( g_DBAccountServer->GetStmt () == request.hStmt )
	{
		return DB_ReconnectAccount ( request ) ;
	}
	else	
	{
		return DB_ReconnectGame ( request ) ;
	}
}






void DB_ResultQuery_CheckAccount(const querySet_t& request,querySet_t& result)
{
	
	RETCODE rCode=SQL_ERROR;
	long	length=0;

	result.CheckAccountVar.dbCheckAccount=-1;
	result.CheckAccountVar.Identity.Certification = i3client_t::tagIdentification::enumCertification::unknown;
	result.CheckAccountVar.Identity.PayLevel=0;


	
	rCode = SQLFetch(request.hStmt);        
	if ((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode)){
		result.retCode = rCode;	
		
		rCode = SQLCloseCursor( request.hStmt );
		if((rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
			
		}
		return;
		
		
	}	
	result.retCode = rCode;
	SQLGetData(request.hStmt, 1, SQL_C_SLONG, &result.CheckAccountVar.dbCheckAccount,        sizeof(int), &length);
	SQLGetData(request.hStmt, 2, SQL_C_SLONG, &result.CheckAccountVar.Identity.Certification,sizeof(int), &length);
	SQLGetData(request.hStmt, 3, SQL_C_SLONG, &result.CheckAccountVar.Identity.PayLevel,     sizeof(int), &length);


	SQLCloseCursor( request.hStmt );
}


void CDBAccount::
SaveServerStatus(
	const int   iServerGroupIdx,
	const char* szpServerGroupName,
	const int   iAcceptPlayerCnt)	
{
	querySet_t request;
	_snprintf(m_query,
		QUERYSIZE,
		"EXEC SaveServerStatus %d,'%s','%s',%d",
		iServerGroupIdx,
		szpServerGroupName,
		"unknown",
		iAcceptPlayerCnt);
	m_query[QUERYSIZE]=NULL;


	request.ownerIdx = -1;
	strncpy(request.ownerID, "",IDSTRING); 
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_SAVESERVERSTATUS;
	strncpy(request.query,m_query,QUERYSIZE); 
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


void CDBAccount::
CheckAccount_Monitor(i3client_t* pClient,const char* szpID,const char* szpPassword)

{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));

	_snprintf(m_query,QUERYSIZE, "EXEC CheckAccount_Monitor '%s', '%s'", szpID, szpPassword);
	m_query[QUERYSIZE]=NULL;

	request.ownerIdx = pClient->idx;
	strncpy(request.ownerID, "",IDSTRING);
	request.ownerID[IDSTRING]=NULL;

	request.queryType = QUERY_CHECK_ACCOUNT;
	strncpy(request.query, m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = m_hStmt;
	request.retCode = -1;
	request.retStructVariable = (void *)pClient;



	if ( !runQueryExecuterThread ) return;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	return;
}
