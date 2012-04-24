




#include "../global_def.h"
#include "../game/GolryunBattle/GolryunBattle_Command.h"
#include "../game/GolryunBattle/GolryunBattle_Encoder.h"
								
CGolryunBattle_Command g_GolryunBattle_Command;							

CGolryunBattle_Command::CGolryunBattle_Command()
{
	
	
}

CGolryunBattle_Command::~CGolryunBattle_Command()
{
	
}

void CGolryunBattle_Command::ProcessCommandToGM(char message[], char command[], chatMessage param[])
{
	CommandToGMBattleNotice(message, command, param);
	CommandToGMBattleStart(message, command, param);
	CommandToGMJoinStart(message, command, param);
	CommandToGMRecallStart(message, command, param);
	CommandToGMJoinClose(message, command, param);
	CommandToGMBettingStart(message, command, param);
	CommandToGMReadyTournament(message, command, param);
	CommandToGMStartTournament(message, command, param);
	CommandToGMPauseTournament(message, command, param);
	CommandToGMReStartTournament(message, command, param);
	CommandToGMCloseBattle(message, command, param);
	CommandToGMTransportGolryun(message, command, param);
	CommandToGMTransportWaitingRoom(message, command, param);
	CommandToGMTransportBattleField(message, command, param);
	CommandToGMWalkOverMatch(message, command, param);
	CommandToGMCheckJoinNumber(message, command, param);
	CommandToGMStartDrawPrize(message, command, param);
	CommandToGMCancelTournament(message, command, param);
	CommandToGMTaltalNakBetting(message, command, param);
	CommandToGMForceJoinList(message, command, param);
	CommandToGMAssignFighterNumber(message, command, param);
	CommandToGMForceEject(message, command, param);
	CommandToGMCheckStatusPvPMode(message, command, param);
	
}


void CGolryunBattle_Command::CommandToGMBattleNotice(char message[], char command[], chatMessage param[])
{
	
	if ( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 376)) )
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		if(NULL == param[0][0]) return;
		g_GolryunBattle_Encoder.SendNoticeBattle(param[0]);
	}
}


void CGolryunBattle_Command::CommandToGMBattleStart(char message[], char command[], chatMessage param[])
{

	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 201)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;

		int intparam = atoi( param[0] );

		switch(intparam)
		{
		case CGolryunBattle::eManor::normal:
			break;
		case CGolryunBattle::eManor::week:
			break;
		case CGolryunBattle::eManor::month:
			break;
		case CGolryunBattle::eManor::ramification:
			break;
		case CGolryunBattle::eManor::year:
			break;
		default:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 379));
			return;
		}

		char szTemp[255];
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,205));
		g_GolryunBattle_Encoder.SendRequestStartBattle(intparam);
		GTH_SendMessage_SystemAdimin_Notice( szTemp );

		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
	}
}

void CGolryunBattle_Command::CommandToGMJoinStart(char message[], char command[], chatMessage param[])
{


	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 206)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		int intparam = atoi( param[0] );

		switch(intparam)
		{
		case 30:
			g_GolryunBattle.SetTournamentIdx(0);
			break;
		case 40:
			g_GolryunBattle.SetTournamentIdx(1);
			break;
		case 50:
			g_GolryunBattle.SetTournamentIdx(2);
			break;
		case 60:
			g_GolryunBattle.SetTournamentIdx(3);
			break;
		case 70:
			g_GolryunBattle.SetTournamentIdx(4);
			break;
		case 80:
			g_GolryunBattle.SetTournamentIdx(5);
			break;
		case 90:
			g_GolryunBattle.SetTournamentIdx(6);
			break;
		default:
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 379) );
			return;
		}

		char szTemp[255];
		int cnt = g_GolryunBattle.GetTournamentIdx();
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,143), g_golryunManagerTable[cnt].levelMin);
		g_GolryunBattle_Encoder.SendRequestStartJoin(szTemp);
		g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_START_JOIN);


		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
	}
}

void CGolryunBattle_Command::CommandToGMRecallStart(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 463)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		int intparam = atoi( param[0] );

		switch(intparam)
		{
		case 30:
			g_GolryunBattle.SetTournamentIdx(0);
			break;
		case 40:
			g_GolryunBattle.SetTournamentIdx(1);
			break;
		case 50:
			g_GolryunBattle.SetTournamentIdx(2);
			break;
		case 60:
			g_GolryunBattle.SetTournamentIdx(3);
			break;
		case 70:
			g_GolryunBattle.SetTournamentIdx(4);
			break;
		case 80:
			g_GolryunBattle.SetTournamentIdx(5);
			break;
		case 90:
			g_GolryunBattle.SetTournamentIdx(6);
			break;
		default:
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 379) );
			return;
		}

		char szTemp[255];
		int cnt = g_GolryunBattle.GetTournamentIdx();
		
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,464), g_golryunManagerTable[cnt].levelMin);
		g_GolryunBattle_Encoder.SendRequestStartReCallFighter(szTemp);


		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
	}
}

void CGolryunBattle_Command::CommandToGMJoinClose(char message[], char command[], chatMessage param[])
{


	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 422)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendRequestCloseJoin();
		g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_CLOSE_JOIN);
	}
}

void CGolryunBattle_Command::CommandToGMBettingStart(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 202)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendRequestStartBetting();
		g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_START_BETTING);
	}
}


void CGolryunBattle_Command::CommandToGMReadyTournament(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 203)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		int intparam = atoi( param[0] );

		switch(intparam)
		{
		case CGolryunBattle::eRoundStep::round_16:	
			break;
		case CGolryunBattle::eRoundStep::round_8:		
			break;
		case CGolryunBattle::eRoundStep::round_4:		
			break;
		case CGolryunBattle::eRoundStep::round_final:		
			break;
		default:
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 379) );
			return;
		}
		
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendRequestReadyTournament(intparam);
		g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_READY_TOURNAMENT);
	}

}


void CGolryunBattle_Command::CommandToGMStartTournament(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 204)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendRequestStartCount();

	}

}

void CGolryunBattle_Command::CommandToGMPauseTournament(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 207)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendRequestPauseMatch();
	}
}

void CGolryunBattle_Command::CommandToGMReStartTournament(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 467)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendRequestReStartMatch();
	}
}

void CGolryunBattle_Command::CommandToGMCloseBattle(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 208)))
	{

		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;

		char szTemp[255];
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,169));
		g_GolryunBattle_Encoder.SendRequestCloseBattle();
		GTH_SendMessage_SystemAdimin_Notice( szTemp );
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
	}
}

void CGolryunBattle_Command::CommandToGMTransportGolryun(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 176)))
	{

		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;

	
		if(g_pApp->m_myCharacter->worldIdx == CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX )
			g_GolryunBattle_Encoder.SendTransWatchingRoom();
		else
			GTH_SendMessage_TransportWorld( CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX, 0 );
	}
	
}

void CGolryunBattle_Command::CommandToGMTransportWaitingRoom(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 377)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
	
		if(g_pApp->m_myCharacter->worldIdx == CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX )
			g_GolryunBattle_Encoder.SendTransWaitingRoom();
		else
			GTH_SendMessage_TransportWorld( CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX, 1 );
	}
	
}

void CGolryunBattle_Command::CommandToGMTransportBattleField(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 378)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
	
		if(g_pApp->m_myCharacter->worldIdx == CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX )
			g_GolryunBattle_Encoder.SendTransBattleStadium();
		else
			return;
	
	}
}

void CGolryunBattle_Command::CommandToGMWalkOverMatch(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 381)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		int intparam = atoi( param[0] );

		if(intparam<0 || intparam > CGolryunBattle::MAX_NUM_FIGHTER_PER_TOURNAMENT)
		{
			
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 379) );
			return;
		}

		g_GolryunBattle_Encoder.SendWalkOverMatch(intparam);
	}
}


void CGolryunBattle_Command::CommandToGMCheckJoinNumber(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 382)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendCheckJoinerNum();
	}

}

void CGolryunBattle_Command::CommandToGMStartDrawPrize(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 383)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		char szTemp[255];
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,172));
		g_GolryunBattle_Encoder.SendRequestStartPrize();
		GTH_SendMessage_SystemAdimin_Notice( szTemp );
	}
}

void CGolryunBattle_Command::CommandToGMCancelTournament(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 384)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendRequestCancelTournament();
	}
}

void CGolryunBattle_Command::CommandToGMTaltalNakBetting(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 421)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendTwoGyunTotalNak();
	}

}


void CGolryunBattle_Command::CommandToGMForceJoinList(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 465)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		if( param[0][0] == 0 ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendInsertJoinList(param[0]);
	}

}

void CGolryunBattle_Command::CommandToGMAssignFighterNumber(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 466)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		if( param[0][0] == 0 ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendAssignFighterNumber( param[0], atoi(param[1]) );
	}

}

void CGolryunBattle_Command::CommandToGMForceEject(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 539)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		if( param[0][0] == 0 ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendForceEject( param[0]);
	}

}

void CGolryunBattle_Command::CommandToGMCheckStatusPvPMode(char message[], char command[], chatMessage param[])
{
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 540)))
	{
		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle_Encoder.SendCheckStatusPvPMode();
	}

}






