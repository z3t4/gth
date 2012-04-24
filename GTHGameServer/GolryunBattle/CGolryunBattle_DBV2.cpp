



#include "..\global.h"
#include "CGolryunBattle_DBV2.h"
#include <assert.h>





CGolryunBattle_DBV2::CGolryunBattle_DBV2()
{
	m_bOK=FALSE;

	m_nTournamentGUIDminmum=0x7fffffff;
	m_Loading.step=tagLoading::loadingSTEP_START;
	m_Error = ERROR_NOT_READYa;
	m_QueryExeMon.refcount=0;
	
	memset(&m_TemporaryPlayingBattle,0x00,sizeof(struct tagBattle));
	memset(m_Record,0x00,sizeof(struct tagBattle)* MAXNUM_RECORD);

	m_bOK=TRUE;
}






CGolryunBattle_DBV2::~CGolryunBattle_DBV2()
{
	Sleep(0);
}





BOOL CGolryunBattle_DBV2::isOK(void) const
{
	return m_bOK;
}




BOOL CGolryunBattle_DBV2::isReady(void) const
{
	if(ERROR_NONE != GetLastError()) return FALSE;
	if(m_QueryExeMon.refcount > 0) return FALSE;

	return TRUE;
}




int CGolryunBattle_DBV2::GetMinimumTournamentGUID(void) const
{
	return m_nTournamentGUIDminmum;
}




inline void CGolryunBattle_DBV2::Process_loadingSTEP_tournament(void)
{
	m_Loading.step = tagLoading::loadingSTEP_tournament_wait;
	if(FALSE == m_TemporaryPlayingBattle.bActive){ 
		m_Error = ERROR_SQL_LOADING_WHEN_START; 
		return; 
	}
	ODBC_LoadTournament(m_TemporaryPlayingBattle.nGUID);

	struct tagBattle* pBattle=NULL;
	for(int nRecordIdx=0; nRecordIdx < MAXNUM_RECORD; nRecordIdx++){
		pBattle = &m_Record[nRecordIdx];		
		if(FALSE == pBattle->bActive) continue;
		ODBC_LoadTournament(pBattle->nGUID);
	}
}




void CGolryunBattle_DBV2::Process(void)
{
	if(FALSE == GTH_IsLocalServiceWorld(tagGolryunBattle::Golryun_Battle_Map_Index)) return;

	switch(m_Loading.step){
		case tagLoading::loadingSTEP_complete:
			return;

		case tagLoading::loadingSTEP_START:			
			ODBC_LoadLastBattle();
			m_Loading.step = tagLoading::loadingSTEP_WAIT;
			return;

		case tagLoading::loadingSTEP_tournament_wait:
			if(m_QueryExeMon.refcount <= 0){
				m_Loading.step = tagLoading::loadingSTEP_complete;
				SetMinimumTournamentGUID();
				if(ERROR_NOT_READYa == m_Error) m_Error = ERROR_NONE;
			}
			return;

		case tagLoading::loadingSTEP_WAIT:
			return;

		case tagLoading::loadingSTEP_tournament:
			Process_loadingSTEP_tournament();
			return;
	}
	
}




extern HANDLE			hQueryRequestMutex; 
extern CQueryQueue*		queryRequestQueue;
void CGolryunBattle_DBV2::ODBC_LoadLastBattle(void)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(g_DBGameServer->m_query,QUERYSIZE, "EXEC LoadLastBattle");
	g_DBGameServer->m_query[QUERYSIZE]=NULL;

	request.ownerIdx = -1;
	request.ownerID[0]=NULL;
	request.queryType = Query_GolryunBattle_LoadLastBattle;
	strncpy(request.query, g_DBGameServer->m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = g_DBGameServer->GetStmt();
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	m_QueryExeMon.refcount+=1;
}





inline void CGolryunBattle_DBV2::SetMinimumTournamentGUID(void)
{
	m_nTournamentGUIDminmum=0x7fffffff;

	struct tagBattle* pBattle=NULL;
	for(int nBattleIdx=1; nBattleIdx < MAXNUM_RECORD; nBattleIdx++){
		pBattle = &m_Record[nBattleIdx];
		for(int nTournamentIdx=0; nTournamentIdx < tagGolryunBattle::Max_Num_Tournament_Per_Battle; nTournamentIdx++){
			if(FALSE == pBattle->Tournament[nTournamentIdx].bComplete) continue;

			if(pBattle->Tournament[nTournamentIdx].nGUID < m_nTournamentGUIDminmum)
				m_nTournamentGUIDminmum = pBattle->Tournament[nTournamentIdx].nGUID;
		}
	}
}





void CGolryunBattle_DBV2::
ODBC_AddBatle(const enum enumBattleState in_BattleState,const struct tagBattle& in_Battle)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(g_DBGameServer->m_query,QUERYSIZE, 
		"EXEC AddBattle %d,%d,%d,%d,"
				       "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		in_Battle.nGUID,
		in_BattleState,
		in_Battle.nMaxTournamentGUID,
		in_Battle.nMaxEntranceTicketGUID,


		in_Battle.nLotteryGUIDList[0],
		in_Battle.nLotteryGUIDList[1],
		in_Battle.nLotteryGUIDList[2],
		in_Battle.nLotteryGUIDList[3],
		in_Battle.nLotteryGUIDList[4],
		in_Battle.nLotteryGUIDList[5],
		in_Battle.nLotteryGUIDList[6],
		in_Battle.nLotteryGUIDList[7],
		in_Battle.nLotteryGUIDList[8],
		in_Battle.nLotteryGUIDList[9],
		in_Battle.nLotteryGUIDList[10],
		in_Battle.nLotteryGUIDList[11],
		in_Battle.nLotteryGUIDList[12],
		in_Battle.nLotteryGUIDList[13],
		in_Battle.nLotteryGUIDList[14],
		in_Battle.nLotteryGUIDList[15]);

	g_DBGameServer->m_query[QUERYSIZE]=NULL;

	request.ownerIdx = -1;
	request.ownerID[0]=NULL;
	request.queryType = Query_GolryunBattle_AddBattle;
	strncpy(request.query, g_DBGameServer->m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = g_DBGameServer->GetStmt();
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	m_QueryExeMon.refcount+=1;
}




void CGolryunBattle_DBV2::
ODBC_QueryExecuterThread_Query_GolryunBattle_AddBattle(const querySet_t& request,querySet_t& result)
{	
	if(SQL_SUCCESS != result.retCode){
		m_Error = ERROR_SQL_query_AddBattle;	
		return;
	}
}








void CGolryunBattle_DBV2::
ODBC_QueryExecuterThread_Query_GolryunBattle_LoadLastBattle(
	const querySet_t& in_request,
	querySet_t& out_result)
{
	assert(m_Loading.step == tagLoading::loadingSTEP_WAIT);

	RETCODE				rCode=SQL_ERROR;	
	rCode= SQLFetch(in_request.hStmt);
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode)){		
		m_Error = ERROR_SQL_LOADING_WHEN_START;

		out_result.retCode=rCode;
		rCode = SQLCloseCursor( in_request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
			g_DBGameServer->PrintError(in_request);
		}
		return;	
	}
	out_result.retCode = rCode;

	int nBattleRecordIdx=0;
	struct tagBattle Battle;
	memset(&Battle,0x00,sizeof(struct tagBattle));

	long  nLength=0;
	enum enumBattleState nbattleState=enumBattleState::BattleState_Create;

	for(int nIdx=0;rCode == SQL_SUCCESS; rCode = SQLFetch(in_request.hStmt),nIdx++){
		SQLGetData(in_request.hStmt, 1, SQL_INTEGER, &Battle.nGUID,sizeof(int), &nLength);	
		SQLGetData(in_request.hStmt, 2, SQL_INTEGER, &nbattleState,sizeof(int), &nLength);	
		SQLGetData(in_request.hStmt, 3, SQL_INTEGER, &Battle.nMaxTournamentGUID,sizeof(int), &nLength);	
		SQLGetData(in_request.hStmt, 4, SQL_INTEGER, &Battle.nMaxEntranceTicketGUID,sizeof(int), &nLength);

		for(int nLotteryIdx=0; nLotteryIdx < tagGolryunBattle::Max_Num_Prize_List_Per_Battle; nLotteryIdx++)
			SQLGetData(in_request.hStmt, 5 + nLotteryIdx, SQL_INTEGER, &Battle.nLotteryGUIDList[nLotteryIdx],sizeof(int), &nLength);

		if(0 == nIdx){
			m_TemporaryPlayingBattle = Battle;
			m_TemporaryPlayingBattle.bActive=TRUE;
		}else{
			assert(TRUE == m_TemporaryPlayingBattle.bActive);
			m_Record[nBattleRecordIdx]=Battle;
			m_Record[nBattleRecordIdx].bActive=TRUE;

			nBattleRecordIdx+=1;
			if(nBattleRecordIdx >= MAXNUM_RECORD) break;
		}		
	}

	m_Loading.step = tagLoading::loadingSTEP_tournament;
	

	rCode = SQLCloseCursor( in_request.hStmt );
	if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
		g_DBGameServer->PrintError(in_request);
	}
	return;
}










void CGolryunBattle_DBV2::ODBC_LoadTournament(const int nBattleGUID)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(g_DBGameServer->m_query,QUERYSIZE, "EXEC LoadTournament %d",nBattleGUID);
	g_DBGameServer->m_query[QUERYSIZE]=NULL;

	request.ownerIdx = -1;
	request.ownerID[0]=NULL;
	request.queryType = Query_GolryunBattle_LoadTournament;
	strncpy(request.query, g_DBGameServer->m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = g_DBGameServer->GetStmt();
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	m_QueryExeMon.refcount+=1;
}






struct CGolryunBattle_DBV2::tagBattle* CGolryunBattle_DBV2::
GetBattleRecordPointer(const int in_nBattleGUID)
{
	if(m_Loading.step == tagLoading::loadingSTEP_tournament_wait){
		if(m_TemporaryPlayingBattle.nGUID == in_nBattleGUID) return &m_TemporaryPlayingBattle;
	}

	for(int nRecordIdx=0; nRecordIdx < MAXNUM_RECORD; nRecordIdx++){
		if(FALSE == m_Record[nRecordIdx].bActive) continue;
		if(m_Record[nRecordIdx].nGUID == in_nBattleGUID) return &m_Record[nRecordIdx];
	}

	return NULL;
}







struct CGolryunBattle_DBV2::tagBattle*	CGolryunBattle_DBV2::GetTemporaryPlayingBattle(void)
{
	return &m_TemporaryPlayingBattle;
}






enum CGolryunBattle_DBV2::enumError CGolryunBattle_DBV2::GetLastError(void) const
{
	return m_Error;
}




void CGolryunBattle_DBV2::
ODBC_QueryExecuterThread_Query_GolryunBattle_LoadTournament(const querySet_t& request,querySet_t& result)
{
	if(m_Loading.step != tagLoading::loadingSTEP_tournament_wait){
		m_Error = ERROR_SQL_LOADING_WHEN_START;
	}
	RETCODE				rCode=SQL_ERROR;	
	rCode= SQLFetch(request.hStmt);
	if((SQL_NO_DATA == rCode) || (SQL_ERROR == rCode)){		

		if(SQL_ERROR == rCode)	m_Error = ERROR_SQL_LOADING_WHEN_START;
		result.retCode=rCode;
		rCode = SQLCloseCursor( request.hStmt );
		if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
			g_DBGameServer->PrintError(request);
		}
		return;	
	}
	result.retCode = rCode;	
	
	long  lLength=0;	
	int   nBattleGUID=0;
	struct tagBattle* pBattle=NULL;
	struct tagTournament Tournament;

	for(;rCode == SQL_SUCCESS; rCode = SQLFetch(request.hStmt)){
		SQLGetData(request.hStmt, 1, SQL_INTEGER, &nBattleGUID,sizeof(int), &lLength);	
		SQLGetData(request.hStmt, 2, SQL_INTEGER, &Tournament.nGUID,sizeof(int), &lLength);	
		SQLGetData(request.hStmt, 3, SQL_INTEGER, &Tournament.nLevelGroupIdx,sizeof(int), &lLength);	
		SQLGetData(request.hStmt, 4, SQL_INTEGER, &Tournament.bComplete,sizeof(int), &lLength);	

		SQLGetData(request.hStmt, 5, SQL_C_CHAR, Tournament.firstWinner.Name,NAMESTRING, &lLength);
		Tournament.firstWinner.Name[NAMESTRING-1]=NULL;
		SQLGetData(request.hStmt, 6, SQL_INTEGER, &Tournament.firstWinner.Num,sizeof(int), &lLength);	


		SQLGetData(request.hStmt, 7, SQL_C_CHAR, Tournament.secondWinner.Name,NAMESTRING, &lLength);
		Tournament.secondWinner.Name[NAMESTRING-1]=NULL;
		SQLGetData(request.hStmt, 8, SQL_INTEGER, &Tournament.secondWinner.Num,sizeof(int), &lLength);


		SQLGetData(request.hStmt, 9, SQL_INTEGER, &Tournament.TwoGyun.nTotalNak,sizeof(int), &lLength);
		SQLGetData(request.hStmt,10, SQL_C_FLOAT, &Tournament.TwoGyun.Rate,sizeof(float), &lLength);


		pBattle = GetBattleRecordPointer(nBattleGUID);
		if(NULL == pBattle){
			m_Error = ERROR_SQL_LOADING_WHEN_START;
			break;
		}
		assert(Tournament.nLevelGroupIdx >= 0);
		assert(Tournament.nLevelGroupIdx < tagGolryunBattle::Max_Num_Tournament_Per_Battle);
		pBattle->Tournament[Tournament.nLevelGroupIdx] = Tournament;
		pBattle->Tournament[Tournament.nLevelGroupIdx].bComplete=TRUE;
	}
	

	rCode = SQLCloseCursor( request.hStmt );
	if( (rCode == SQL_ERROR) || (rCode == SQL_SUCCESS_WITH_INFO)){
		g_DBGameServer->PrintError(request);
	}
	return;


}












void CGolryunBattle_DBV2::ODBC_SaveBattle(const enum enumBattleState in_BattleState,const struct tagBattle& in_Battle)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(g_DBGameServer->m_query,QUERYSIZE,
		"EXEC SaveBattle %d,%d,%d,%d,"
		"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		in_Battle.nGUID,
		in_BattleState,
		in_Battle.nMaxTournamentGUID,
		in_Battle.nMaxEntranceTicketGUID,
		in_Battle.nLotteryGUIDList[0],
		in_Battle.nLotteryGUIDList[1],
		in_Battle.nLotteryGUIDList[2],
		in_Battle.nLotteryGUIDList[3],
		in_Battle.nLotteryGUIDList[4],
		in_Battle.nLotteryGUIDList[5],
		in_Battle.nLotteryGUIDList[6],
		in_Battle.nLotteryGUIDList[7],
		in_Battle.nLotteryGUIDList[8],
		in_Battle.nLotteryGUIDList[9],
		in_Battle.nLotteryGUIDList[10],
		in_Battle.nLotteryGUIDList[11],
		in_Battle.nLotteryGUIDList[12],
		in_Battle.nLotteryGUIDList[13],
		in_Battle.nLotteryGUIDList[14],
		in_Battle.nLotteryGUIDList[15]);
		
		
	g_DBGameServer->m_query[QUERYSIZE]=NULL;

	request.ownerIdx = -1;
	request.ownerID[0]=NULL;
	request.queryType = Query_GolryunBattle_SaveBattle;
	strncpy(request.query, g_DBGameServer->m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = g_DBGameServer->GetStmt();
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	m_QueryExeMon.refcount+=1;
}




void CGolryunBattle_DBV2::
ODBC_QueryExecuterThread_Query_GolryunBattle_SaveBattle(const querySet_t& in_request,querySet_t& out_result)
{	
	if(SQL_SUCCESS != out_result.retCode){
		m_Error = ERROR_SQL_query_SaveBattle;	
		return;
	}
}






void CGolryunBattle_DBV2::
ODBC_AddTournament(const int in_nBattleGUID,const struct tagTournament& in_Tournament)
{
	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(g_DBGameServer->m_query,QUERYSIZE,
		"EXEC AddTournament "
		"%d,%d,%d,%d,"
		"'%s',%d,"
		"'%s',%d,"
		"%d,%.3f",

		in_nBattleGUID,in_Tournament.nGUID,in_Tournament.nLevelGroupIdx,in_Tournament.bComplete,
		in_Tournament.firstWinner.Name, in_Tournament.firstWinner.Num,
		in_Tournament.secondWinner.Name, in_Tournament.secondWinner.Num,
		in_Tournament.TwoGyun.nTotalNak,in_Tournament.TwoGyun.Rate);
		
		
	g_DBGameServer->m_query[QUERYSIZE]=NULL;

	request.ownerIdx = -1;
	request.ownerID[0]=NULL;
	request.queryType = Query_GolryunBattle_AddTournament;
	strncpy(request.query, g_DBGameServer->m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = g_DBGameServer->GetStmt();
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	m_QueryExeMon.refcount+=1;
}






void CGolryunBattle_DBV2::
ODBC_QueryExecuterThread_Query_GolryunBattle_AddTournament(const querySet_t& in_request,querySet_t& out_result)
{	
	if(SQL_SUCCESS != out_result.retCode){
		m_Error = ERROR_SQL_query_AddTournament;
		return;
	}
}





void CGolryunBattle_DBV2::
ODBC_SaveTournament(const int in_nBattleGUID,const struct tagTournament& in_Tournament)
{

	querySet_t request;
	memset(&request,0x00,sizeof(querySet_t));


	_snprintf(g_DBGameServer->m_query,QUERYSIZE,
		"EXEC SaveTournament "
		"%d,%d,%d,%d,"
		"'%s',%d,"
		"'%s',%d,"
		"%d,%.3f",

		in_nBattleGUID,in_Tournament.nGUID,in_Tournament.nLevelGroupIdx,in_Tournament.bComplete,
		in_Tournament.firstWinner.Name, in_Tournament.firstWinner.Num,
		in_Tournament.secondWinner.Name, in_Tournament.secondWinner.Num,
		in_Tournament.TwoGyun.nTotalNak,in_Tournament.TwoGyun.Rate);
		
		
	g_DBGameServer->m_query[QUERYSIZE]=NULL;

	request.ownerIdx = -1;
	request.ownerID[0]=NULL;
	request.queryType = Query_GolryunBattle_SaveTournament;
	strncpy(request.query, g_DBGameServer->m_query,QUERYSIZE);
	request.query[QUERYSIZE]=NULL;

	request.hStmt = g_DBGameServer->GetStmt();
	request.retCode = -1;
	request.retStructVariable = NULL;
	
	WaitForSingleObject( hQueryRequestMutex, INFINITE );
	queryRequestQueue->Push( &request );
	ReleaseMutex( hQueryRequestMutex );
	m_QueryExeMon.refcount+=1;
}




void CGolryunBattle_DBV2::
ODBC_QueryExecuterThread_Query_GolryunBattle_SaveTournament(const querySet_t& in_request,querySet_t& out_result)
{
	if(SQL_SUCCESS != out_result.retCode){
		m_Error = ERROR_SQL_query_SaveTournament;
		return;
	}

}





char* CGolryunBattle_DBV2::GetLastErrorStr(void) const
{
	static char strMessage[256]="";

	switch(m_Error){
		case ERROR_NONE:
			wsprintf(strMessage,"Success");
			break;

		case ERROR_NOT_READYa:
			wsprintf(strMessage,"db not ready");
			break;

		case ERROR_SQL_LOADING_WHEN_START:
			wsprintf(strMessage,"query AddBattle error");
			break;

		case ERROR_SQL_query_AddBattle:
			wsprintf(strMessage,"query AddBattle error");
			break;

		case ERROR_SQL_query_SaveBattle:
			wsprintf(strMessage,"query SaveBattle error");
			break;
		case ERROR_SQL_query_AddTournament:
			wsprintf(strMessage,"query AddTournament error");
			break;
		case ERROR_SQL_query_SaveTournament:
			wsprintf(strMessage,"query SaverTournament error");
			break;
		default:
			wsprintf(strMessage,"unknown error");
			break;
	}

	return strMessage;
}









void CGolryunBattle_DBV2::
Update(const struct tagBattle& in_Battle)
{	
	ODBC_SaveBattle(BattleState_Playing,in_Battle);	
}




BOOL CGolryunBattle_DBV2::
Update(const int in_nUpdateTornamentGroupIdx,const struct tagBattle& in_Battle)
{
	
	
	if(in_nUpdateTornamentGroupIdx < 0) return FALSE;
	if(in_nUpdateTornamentGroupIdx >= tagGolryunBattle::Max_Num_Tournament_Per_Battle) return FALSE;




	ODBC_AddTournament(in_Battle.nGUID,in_Battle.Tournament[in_nUpdateTornamentGroupIdx]);
	
	ODBC_SaveBattle(BattleState_Playing,in_Battle);
	return TRUE;
}




void CGolryunBattle_DBV2::Add(const struct tagBattle& in_Battle)
{
	ODBC_AddBatle(BattleState_Playing,in_Battle);	

	
}




void CGolryunBattle_DBV2::Save(const struct tagBattle& in_Battle)
{
	ODBC_SaveBattle(BattleState_Playing,in_Battle);	
	
	for(int nRecordIdx = MAXNUM_RECORD-1; nRecordIdx >= 1; nRecordIdx--){
		m_Record[nRecordIdx] = m_Record[nRecordIdx-1];
	}
	m_Record[0] = in_Battle;
	SetMinimumTournamentGUID();
}












void CGolryunBattle_DBV2::ODBC_ProcessQueryResultQueue_Query_GolryunBattle_LoadLastBattle(const querySet_t& )
{
	m_QueryExeMon.refcount-=1;

}



void CGolryunBattle_DBV2::ODBC_ProcessQueryResultQueue_Query_GolryunBattle_AddBattle(const querySet_t& )
{
	m_QueryExeMon.refcount-=1;

}



void CGolryunBattle_DBV2::ODBC_ProcessQueryResultQueue_Query_GolryunBattle_LoadTournament(const querySet_t& )
{
	m_QueryExeMon.refcount-=1;
}



void CGolryunBattle_DBV2::ODBC_ProcessQueryResultQueue_Query_GolryunBattle_SaveBattle(const querySet_t& )
{
	m_QueryExeMon.refcount-=1;
}



void CGolryunBattle_DBV2::ODBC_ProcessQueryResultQueue_Query_GolryunBattle_AddTournament(const querySet_t& )
{
	m_QueryExeMon.refcount-=1;

}




void CGolryunBattle_DBV2::ODBC_ProcessQueryResultQueue_Query_GolryunBattle_SaveTournament(const querySet_t& )
{
	m_QueryExeMon.refcount-=1;
}






























