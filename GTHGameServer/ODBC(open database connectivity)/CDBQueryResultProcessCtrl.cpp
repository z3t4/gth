



#include "..\global.h"
#include "CDBQueryResultProcessCtrl.h"



extern int			   runQueryExecuterThread;
extern HANDLE          hQueryResultMutex;

CQueryQueue		*queryRequestQueue, *queryResultQueue;





CDBQueryResultProcessCtrl::CDBQueryResultProcessCtrl()
{
	m_bOK=FALSE;

	m_bOK=TRUE;

}





CDBQueryResultProcessCtrl::~CDBQueryResultProcessCtrl()
{

}





BOOL CDBQueryResultProcessCtrl::isOK(void) const
{
	return m_bOK;
}





void DB_ProcessQueryResultQueue_QUERY_LOAD_HELPLIST(const querySet_t result)
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









void CDBQueryResultProcessCtrl::Process(void) const
{
	static querySet_t			result;
	static playerCharacter_t	*pc;
	int					        isExist;
	static int					processedQueryCount;

	if ( !runQueryExecuterThread ) return;

	processedQueryCount = 0;



	while ( processedQueryCount < MAX_QUERY_PER_FRAME ){		
		
		WaitForSingleObject( hQueryResultMutex, INFINITE );
		if(queryResultQueue->m_count > 0)
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
		
		
		if( result.ownerIdx < 0 || result.queryType < 0 || result.queryType >= NUM_QUERY_TYPE || result.ownerIdx >= MAX_PCS ){

			static char *queryTypeStr[] = 
			{
				"QUERY_INSERT_LOGIN",
				"QUERY_DELETE_LOGIN",
				"QUERY_DELETE_LOGIN_HOST",
				"QUERY_LOAD_CHARACTER",
				"QUERY_LOAD_QUESTSTATUS",
				"QUERY_LOAD_QUEST",	
				"QUERY_LOAD_SKILL",
				"QUERY_LOAD_ITEM",
				"QUERY_LOAD_FRIENDS",
				"QUERY_SAVE_GAME",
				"QUERY_SAVE_SKILL",
				"QUERY_SAVE_QUEST",	
				"QUERY_ADD_QUESTSTATUS",
				"QUERY_REMOVE_QUESTSTATUS",
				"QUERY_SAVE_QUESTSTATUS",
				"QUERY_ADD_ITEM",
				"QUERY_REMOVE_ITEM",
				"QUERY_SAVE_ITEM",
				"QUERY_SAVE_FRIENDS",
				"QUERY_ADD_GUILD",
				"QUERY_LOAD_GUILD",
				"QUERY_SAVE_GUILD",
				"QUERY_SAVE_GUILDMARK",
				"QUERY_REMOVE_GUILD",
				"QUERY_BANISHMEMBER_GUILD",
				"QUERY_SEND_MAIL",
				"QUERY_RECV_MAIL",
				"QUERY_GET_MAILSTATE",
				"QUERY_DELETE_MAIL",
				"QUERY_VIEW_MAIL",
				"QUERY_SAVE_MAIL",
				"QUERY_SAVE_CHATBAN",
				"QUERY_LOAD_CHATBAN",
				"QUERY_LOAD_GONRYUNBATTLE",
				"QUERY_SAVE_GONRYUNBATTLE",				
				"QUERY_LOAD_PACKAGELIST",
				"QUERY_SEND_POST",
				"QUERY_DELETE_POST",
				"QUERY_OPEN_POST",	
				"QUERY_DELETE_POSTITEMS",
				"QUERY_SAVECHA_PACKAGE",
				"QUERY_SAVECHA_ITEM_DETAIL",			
				"QUERY_LOAD_PREMIUMINFO",
				"QUERY_SAVE_PREMIUMINFO",
				"QUERY_SAVECHAPACKAGELIST_BY_SYTEM",
				"QUERY_SAVECHA_PACKAGENAK",
				"QUERY_POSTSYSTEM_LOADPACKAGECNT",
				"QUERY_GolryunBattle_SaveBattleInfo",

				
				"QUERY_SAVE_HELPLIST",
				"QUERY_LOAD_HELPLIST",
				"QUERY_SAVE_WEBPOINT",
				
				
				"QUERY_GIFTCARD_LoadGiftCard",
				"QUERY_GIFTCARD_LoadGiftCardItembytype",
				"QUERY_GIFTCARD_SaveGiftCard",
				
				"QUERY_LOADSERVERSTATUS",
				"QUERY_SAVESERVERSTATUS",
				
				"QUERY_GolryunBattle_LoadLastBattle",
				"QUERY_GolryunBattle_AddBattle",
				"QUERY_GolryunBattle_LoadTournament",
				"QUERY_GolryunBattle_SaveBattle",
				"QUERY_GolryunBattle_AddTournament",
				"QUERY_GolryunBattle_SaveTournament",
				
				
				
				"QUERY_INSERT_DANBATTLE",
				"QUERY_DELETE_DANBATTLE",
				"QUERY_DELETE_ALLDANBATTLE",
				"QUERY_SAVE_DANBATTLE_GUILD_ADD_WINPOINT",
				"QUERY_SAVE_DANBATTLE_GUILD_ADD_LOSEPOINT",
				"QUERY_SAVE_DANBATTLE_GUILD_ADD_ACCPOINT",
				"QUERY_LOAD_DANBATTLE_GUILD_INFO",
				"QUERY_SAVE_DANBATTLE_PCPOINT",			

			};

			if( result.queryType >= 0 && result.queryType < NUM_QUERY_TYPE )
			{
				g_logSystem->WriteToError( "QueryError: %s, ownerIdx: %d, query: %s",
					queryTypeStr[result.queryType], 
					result.ownerIdx, 
					result.query );
			}else{
				g_logSystem->WriteToError( "QueryError: Unknown(%d), ownerIdx: %d, query: %s", 
					result.queryType, 
					result.ownerIdx, 
					result.query );
			}
			continue;
		}




		switch ( result.queryType )
		{
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
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_INSERT_LOGIN, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_INSERT_LOGIN, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Duplicated Login Error!! : (ID: %s)", pc->userID);

				break;
			}
			else
			{
				DB_LoadGameData( pc );
			}
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
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_LOAD_CHARACTER, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_LOAD_CHARACTER, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_LOAD_CHARACTER, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
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
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_LOAD_QUEST, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_LOAD_QUEST, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_LOAD_QUEST, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
			break;

		case QUERY_LOAD_QUESTSTATUS :
			if ( result.retCode < 0)
			{
				g_logSystem->Write("Database Error: QUERY_LOAD_QUESTSTATUS, ownerIdx: %d, query: %s", result.ownerIdx, result.query);
				continue;
			}
			pc = &g_pc[result.ownerIdx];
			if ( !pc->active )
			{
				g_logSystem->Write("Not Active PC's Query Executed Error!! : QUERY_LOAD_QUESTSTATUS, ownerIdx: %d)", result.ownerIdx);
				break;
			}
			if ( stricmp(result.ownerID, pc->userID) != 0)
			{
				g_logSystem->Write("Query Owner Changed Error!! : (QUERY_LOAD_QUESTSTATUS, OWNER:%s, PC:%s)", result.ownerID, pc->userID);
				break;
			}
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
		case QUERY_SAVE_CHATBAN:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_SAVE_CHATBAN, ownerIdx: %d, query: %s", result.ownerID, result.query );
			}
			
			break;
		case QUERY_LOAD_CHATBAN:
			if ( result.retCode < 0 )
			{
				g_logSystem->Write ( "Database Error: QUERY_LOAD_CHATBAN, ownerIdx: %d, query: %s", result.ownerID, result.query );
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