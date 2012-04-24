#include "../global_def.h"
#include "IFCommon.h"

int CIFLoginWin::ChangeLoginStep( int step )
{
	if( step == m_loginStep ) return 0;

	
	
	int i;
	int oldLoginStep = m_loginStep;

	
	int stepFlag[6] = { 0, 0, 0, 0, 0 , 0};

	stepFlag[step] = 1;

	


#if defined (_GTH_TERRA_VERSION) || (_GTH_MALAYSIA_CHN_VERSION)	
	stepFlag[0] = 0;
#endif

	m_login->Enable( stepFlag[0] );
	m_loginTitleBar->Enable( stepFlag[0] );
	m_loginTitleBottom->Enable( stepFlag[0] );
	m_loginBar->Enable( stepFlag[0] );
	m_loginLeftBar->Enable( stepFlag[0] );
	m_loginRightBar->Enable( stepFlag[0] );
	m_loginIDBar->Enable( stepFlag[0] );
	m_loginPasswordBar->Enable( stepFlag[0] );
	m_loginOkBtn->Enable( stepFlag[0] );
	m_loginCancelBtn->Enable( stepFlag[0] );

#ifdef _GTH_ENABLE_GOODNOTICE	
	
	m_loginNotice->Enable(stepFlag[LOGIN_STATE_NOTICE]);
	m_cNoticeOkBtn->Enable(stepFlag[LOGIN_STATE_NOTICE]);
#endif
	
	
	for( i = 0; i < MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN; i ++ )
		m_serverBtn[i]->Enable( false );
	



	m_serverOkBtn->Enable( false );
	m_serverCancelBtn->Enable( false );

	

	m_cselInfo->Enable( false );
	
	m_cselInfoTitleBar->Enable( stepFlag[2] );
	m_cselInfoTitleBottom->Enable( stepFlag[2] );
	m_cselInfoBar->Enable( stepFlag[2] );
	m_cselInfoGenBar->Enable( stepFlag[2] );
	m_cselInfoGenIcon0->Enable( stepFlag[2] );
	m_cselInfoGenIcon1->Enable( stepFlag[2] );
	m_cselInfoGenIcon2->Enable( stepFlag[2] );
	m_cselInfoGenIcon3->Enable( stepFlag[2] );
	m_cselExitBtn->Enable( false );
	m_cselOkBtn->Enable( false );
	m_cselBackBtn->Enable( false );
	m_cselDeleteBtn->Enable( false );
	m_cselMakeBtn->Enable( false );
	
	m_DifferentCharBtn->Enable(false);

#ifdef _GTH_FLAG_RELOGIN
	
	m_cReLoginBtn->Enable(false);
#endif

	
	m_cmakeInfo->Enable( false );
	m_cmakeInfoTitleBar->Enable( stepFlag[3] );
	m_cmakeInfoTitleBottom->Enable( stepFlag[3] );
	m_cmakeInfoBar->Enable( stepFlag[3] );
	m_cmake->Enable( stepFlag[3] );
	m_cmakeTitleBar->Enable( stepFlag[3] );
	m_cmakeTitleBottom->Enable( stepFlag[3] );
	m_cmakeBar->Enable( stepFlag[3] );
	m_cmakeNameIcon->Enable( stepFlag[3] );
	m_cmakeNameBar->Enable( stepFlag[3] );
	m_cmakeFaceIcon->Enable( stepFlag[3] );
	m_cmakeFaceBar->Enable( stepFlag[3] );
	m_cmakeHairIcon->Enable( stepFlag[3] );
	m_cmakeHairBar->Enable( stepFlag[3] );
	m_cmakeStatBar->Enable( stepFlag[3] );
	m_cmakeStatLine->Enable( stepFlag[3] );
	m_cmakeForceIcon->Enable( stepFlag[3] );
	m_cmakeForceBar->Enable( stepFlag[3] );
	m_cmakeSoulIcon->Enable( stepFlag[3] );
	m_cmakeSoulBar->Enable( stepFlag[3] );
	m_cmakeAgiIcon->Enable( stepFlag[3] );
	m_cmakeAgiBar->Enable( stepFlag[3] );
	m_cmakeVitIcon->Enable( stepFlag[3] );
	m_cmakeVitBar->Enable( stepFlag[3] );
	m_cmakeFacePrevBtn->Enable( stepFlag[3] );
	m_cmakeFaceNextBtn->Enable( stepFlag[3] );
	m_cmakeHairPrevBtn->Enable( stepFlag[3] );
	m_cmakeHairNextBtn->Enable( stepFlag[3] );
	m_cmakeForceUpBtn->Enable( stepFlag[3] );
	m_cmakeForceDnBtn->Enable( stepFlag[3] );
	m_cmakeSoulUpBtn->Enable( stepFlag[3] );
	m_cmakeSoulDnBtn->Enable( stepFlag[3] );
	m_cmakeAgiUpBtn->Enable( stepFlag[3] );
	m_cmakeAgiDnBtn->Enable( stepFlag[3] );
	m_cmakeVitUpBtn->Enable( stepFlag[3] );;
	m_cmakeVitDnBtn->Enable( stepFlag[3] );
	m_cmakeOkBtn->Enable( stepFlag[3] );
	m_cmakeCancelBtn->Enable( stepFlag[3] );
	m_cmakePrevBtn->Enable( false );
	m_cmakeNextBtn->Enable( false );

	
	switch( m_loginStep )
	{
	case LOGIN_STATE_IDPW :
		{
			g_chatManager->InitInputStr( 0 );
			g_chatManager->InitInputStr( 1 );
			g_chatManager->SetInputState( 0, -1 );
			g_chatManager->SetInputState( 1, -1 );
			m_selectedCharacter = NULL;


			
			if ( g_cgv.state == CLIENT_LOGIN_PROCESS )
				g_ifMng->m_ServerListWin->Enable(TRUE);		

		}
		break;
	case LOGIN_STATE_SELECT_SERVER :
		{
		}
		break;
	case LOGIN_STATE_SELECT_CHARACTER :
		{
		}
		break;
	case LOGIN_STATE_CREATE_CHARACTER :
		{
			g_chatManager->SetInputState( 0, -1 );
		}
		break;
	}

	m_loginStep = step;

	
	switch( m_loginStep )
	{
	case LOGIN_STATE_IDPW :
		{
			if( stepFlag[0] )
			{
				int cx, cy;
				cx = m_textCtl[IT_LOGIN_WIN_ID_U]->GetXPos();
				cy = m_textCtl[IT_LOGIN_WIN_ID_U]->GetYPos();
				g_chatManager->SetInput( cx, cy, 12, 12, 0, 1, 90 );
		
				cx = m_textCtl[IT_LOGIN_WIN_PASSWORD_U]->GetXPos();
				cy = m_textCtl[IT_LOGIN_WIN_PASSWORD_U]->GetYPos();
				g_chatManager->SetInput( cx, cy, 12, 12, 1, 2, 90 );

				g_cgv.enableChat = true;
				g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
			}
		}
		break;
	case LOGIN_STATE_SELECT_SERVER :
		{
		}
		break;
	case LOGIN_STATE_SELECT_CHARACTER :
		{
		}
		break;
	case LOGIN_STATE_CREATE_CHARACTER :
		{
			int cx, cy;
			cx = m_textCtl[IT_CHARMAKE_WIN_NAME_U]->GetXPos();
			cy = m_textCtl[IT_CHARMAKE_WIN_NAME_U]->GetYPos();
			g_chatManager->SetInput( cx, cy, NAMESTRING - 2, NAMESTRING - 2, INPUT_INDEX_DIALOGBOX, 1 );
			g_cgv.enableChat = true;
			g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
		}
	}

	return( oldLoginStep );
}


#ifdef _TERRA
#include "..\Terra ICT\CTerraCtrl.h"
extern CTerraCtrl gcTerraCtrl;
#endif

void CIFLoginWin::RequestLogin()
{


				
	
	if ( g_cgv.restartState == RESTART_NONE )
	{
		strcpy( g_cgv.clientID, g_chatManager->GetInputStr( 0 ) );
		strcpy( g_cgv.clientPassword, g_chatManager->GetInputStr( 1 ) );
	}
	
	

	#if defined(_GTH_ONLINE_VERSION)
		#if defined(_TERRA)	
		gcTerraCtrl.GTH_SendMessage_RequestLogin();
		#else
		GTH_SendMessage_RequestLogin( g_cgv.clientID, g_cgv.clientPassword );
		#endif
	#endif




}

void CIFLoginWin::MakeServerList()
{
	DWORD color;
	char szTemp[256], serverName[256];
	int x, y;

	
	
	if( g_cgv.restartState == RESTART_CHARACTER )
	{
		m_cameraStep = 0; 
		m_selectedCharacter = NULL;
		RequestGameData();
		
		return;
	}
	else if( g_cgv.restartState == RESTART_SERVER )
	{
		m_cameraStep = 0; 
		m_selectedCharacter = NULL;
	}
	
	
	
	
#if defined (_GTH_CHINESE_VERSION) || (_GTH_MALAYSIA_CHN_VERSION )
	x = g_ifMng->m_clientWidth / 2 - m_serverBtn[0]->m_info[0].sizeX;
	y = g_ifMng->m_clientHeight / 2 - ( m_serverBtn[0]->m_info[0].sizeY * ( MAX_SERVER_GROUP / 2 ) ) / 2;
#else
	x = g_ifMng->m_clientWidth / 2 - m_serverBtn[0]->m_info[0].sizeX / 2;
	y = g_ifMng->m_clientHeight / 2 - ( m_serverBtn[0]->m_info[0].sizeY * ( MAX_SERVER_GROUP ) ) / 2;
#endif

	for( int i = 0; i < MAX_SERVER_GROUP; i ++ )
	{
		strcpy( serverName, g_cgv.serverGroup[i].name );
		
		if ( g_cgv.serverGroup[i].active )
		{
			if( g_cgv.serverGroup[i].access )
			{
				

				

				





				

				if( g_cgv.serverGroup[i].clientNumber < 100 ) strcpy( szTemp, "(Good)" );
				else if( g_cgv.serverGroup[i].clientNumber < 200 ) strcpy( szTemp, "(Normal)" );
				else if( g_cgv.serverGroup[i].clientNumber < 500 ) strcpy( szTemp, "(Busy)" );
				else strcpy( szTemp, "(Very Busy)" );



				
				color = 0xffffffff;
				m_serverBtn[i]->SetButtonState( IF_BTN_TYPE_ENABLE );
			}
			else
			{
				sprintf( szTemp, "(%s)", g_LPACK.GetMassage(0,346) );
				color = 0xff808080;
				m_serverBtn[i]->SetButtonState( IF_BTN_TYPE_DISABLE );
			}





		}
		else 
		{
			sprintf( szTemp, "(%s)", g_LPACK.GetMassage(0,346) );
			color = 0xff808080;
			m_serverBtn[i]->SetButtonState( IF_BTN_TYPE_DISABLE );
		}
		strcat( serverName, szTemp );
		m_serverBtn[i]->SetButtonText( serverName, color );
		
#if defined (_GTH_CHINESE_VERSION) || (_GTH_MALAYSIA_CHN_VERSION )
		m_serverBtn[i]->UpdateClientPosition( x + ( i % 2 ) * m_serverBtn[0]->m_info[0].sizeX, y + ( i / 2 ) * m_serverBtn[0]->m_info[0].sizeY );
#else
		m_serverBtn[i]->UpdateClientPosition( x, y + i * m_serverBtn[0]->m_info[0].sizeY );
#endif
		
		m_serverBtn[i]->Enable( true );
	}
}


void CIFLoginWin::MakeMasterServerGroupList()
{
	DWORD color = 0xffffffff;
	char serverName[256];
	int x, y;
	
	x = g_ifMng->m_clientWidth / 2 - m_serverBtn[0]->m_info[0].sizeX / 2;
	y = g_ifMng->m_clientHeight / 2 - ( m_serverBtn[0]->m_info[0].sizeY * ( g_cgv.masterServerGroupNumber ) ) / 2;
	
	for( int i = 0; i < g_cgv.masterServerGroupNumber; i ++ )
	{
		strcpy( serverName, g_cgv.masterServerGroupInfo[i].masterServerGroupName );
		m_serverBtn[i]->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_serverBtn[i]->SetButtonText( serverName, color );
		m_serverBtn[i]->UpdateClientPosition( x, y + i * m_serverBtn[0]->m_info[0].sizeY );		
		m_serverBtn[i]->Enable( true );
	}
}

void CIFLoginWin::RequestGameData()
{
	
	
#ifdef _GTH_ONLINE_VERSION
	GTH_SendMessage_RequestGameData();
#endif
}

void CIFLoginWin::RequestMakeCharacter()
{
	
	m_makeFlag = true;
	m_cameraStep = 1;
	m_curSelectedMakeCharacter = 0;
	m_cameraStepFlag = true;
}

void CIFLoginWin::RequestCheckCharacterName()
{
	charEditData_t *pChar = &m_makeCharData[m_curSelectedMakeCharacter];
	char substr[2];

	strcpy( pChar->name, g_chatManager->GetInputStr( 0 ) );
	if( pChar->name[0] == 0 )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,95), 1, IF_MESSAGE_NONE );			
		return;
	}
	
	if ( strstr( pChar->name, g_LPACK.GetMassage(0,96)) != NULL )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,97), 1, IF_MESSAGE_NONE );
		return;
	}
	
	if ( strstr( pChar->name, g_LPACK.GetMassage(0,97)) != NULL )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,98), 1, IF_MESSAGE_NONE );
		return;
	}
	
	if ( strstr( pChar->name, g_LPACK.GetMassage(0,99)) != NULL )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,100), 1, IF_MESSAGE_NONE );
		return;
	}
	
	if ( strstr( pChar->name, "gM") != NULL )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,343), 1, IF_MESSAGE_NONE );
		return;
	}
	
	if ( strstr( pChar->name, "Gm") != NULL )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,398), 1, IF_MESSAGE_NONE );
		return;
	}
	
	if ( !g_chatManager->IsVaildString( pChar->name ) )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( 0, 554 ), 1, IF_MESSAGE_NONE );
		return;
	}
	
	for ( int i = 0; i < 128; i++)
	{
		if ( i == 0 ) continue;		
		if (i > 47 && i < 58) continue;	
		if (i > 64 && i < 91) continue;	
		if (i > 96 && i < 123) continue;
		
		sprintf(substr, "%c", i);
		if ( strstr( pChar->name, substr) != NULL )
		{
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,399), 1, IF_MESSAGE_NONE );
			return;
		}
	}

	
		
	if (g_LPACK.CheckAbuseWord(pChar->name) || g_LPACK.CheckCharacterName(pChar->name))
	{
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL,117), 	1, IF_MESSAGE_NONE );
		return;
	}	
	GTH_SendMessage_RequestCheckCharacterName( pChar->name );
}

void CIFLoginWin::RequestServerAddress()
{
	if( m_selectedCharacter == NULL ) return;
	if( m_selectedCharacter->entityType == ENTITY_NPC ) return;

	
	g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,113), MSG_BUTTONTYPE_NONE );
#ifdef _GTH_ONLINE_VERSION
	GTH_SendMessage_RequestGameServerAddr();
#else
	GTH_ProcessMessage_ReplyServerAddress();
#endif
}

void CIFLoginWin::SetCharacterInfo()
{
	extern Fx_CHARACTER_t *g_mouseSelectedCharacter;
	myCharacterInfo_t *selectedChar;
	int idx;
	char szTemp[256];

	if( g_mouseSelectedCharacter == NULL && m_selectedCharacter == NULL ) 
	{
		m_cselInfo->Enable( false );
		return;
	}
	else if( g_mouseSelectedCharacter != NULL && m_selectedCharacter == NULL ) 
	{
		
		
		idx = g_mouseSelectedCharacter->ID;
		if ( g_cgv.characterSlotMode == globalVariable_t::CharacterSlotMode::BACK)
			idx += 3;

		if( idx >= MAX_SAVED_DATA ) 
			return;
		

		g_ifMng->m_iPos[IF_CHARINFO_WIN].clientX = g_ifMng->m_clientWidth / 2 - g_ifMng->m_iPos[IF_CHARINFO_WIN].sizeX / 2;
		g_ifMng->m_iPos[IF_CHARINFO_WIN].clientY = 0;
		UpdateControl();
	} 
	else if( m_selectedCharacter != NULL ) 
	{
		idx = m_selectedCharacter->ID;
		
		if ( g_cgv.characterSlotMode == globalVariable_t::CharacterSlotMode::BACK)
			idx += 3;
		
		if( idx >= MAX_SAVED_DATA ) 
			return;
		

		g_ifMng->m_iPos[IF_CHARINFO_WIN].clientX = 0;
		g_ifMng->m_iPos[IF_CHARINFO_WIN].clientY = 0;
		UpdateControl();
	}

	selectedChar = &g_cgv.savedData[idx];
	if( selectedChar->characterID < 0 )
	{
		m_cselInfo->Enable( false );
		return;
	}

	m_cselInfo->Enable( true );
		
	m_textCtl[IT_CHARINFO_WIN_CHARINFO]->SetText( g_LPACK.GetMassage( 0, 506 ) );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 555 ), selectedChar->name );
	m_textCtl[IT_CHARINFO_WIN_NAME_U]->SetText( szTemp );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 556 ), selectedChar->level, selectedChar->genLevel );
	m_textCtl[IT_CHARINFO_WIN_LEVEL_U]->SetText( szTemp );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 557 ), selectedChar->rank );
	m_textCtl[IT_CHARINFO_WIN_RANK_U]->SetText( szTemp );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 558 ), 
		g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_GEN_ATTRIBUTE, selectedChar->pcClass ) );
	m_textCtl[IT_CHARINFO_WIN_CLASS_U]->SetText( szTemp );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 559 ), g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, selectedChar->pcJob ) );
	m_textCtl[IT_CHARINFO_WIN_GEN_BAR]->SetText( szTemp );

	m_textCtl[IT_CHARINFO_WIN_GEN_ICON_0_U]->SetText( g_LPACK.GetMassage( 0, 464 ) );
	m_textCtl[IT_CHARINFO_WIN_GEN_ICON_1_U]->SetText( g_LPACK.GetMassage( 0, 464 ) );
	m_textCtl[IT_CHARINFO_WIN_GEN_ICON_2_U]->SetText( g_LPACK.GetMassage( 0, 464 ) );
	m_textCtl[IT_CHARINFO_WIN_GEN_ICON_3_U]->SetText( g_LPACK.GetMassage( 0, 464 ) );
	

	if( CheckAvailableGenCapability( selectedChar, J_MENTALITY ) )
	{
		strcpy( szTemp, g_ifMng->GetNumberString( selectedChar->genCapability[0] ) );
		m_cselInfoGenIcon0->ChangeControlTexInfo( IF_CHAR_WIN_PGEN_MARK_0_0 );
	}
	else
	{
		strcpy( szTemp, g_LPACK.GetMassage( 0, 465 ) );
		m_cselInfoGenIcon0->ChangeControlTexInfo( IF_CHAR_WIN_PGEN_MARK_0_0 + 1 );
	}
	m_textCtl[IT_CHARINFO_WIN_GENCAP_0_U]->SetText( szTemp );

	if( CheckAvailableGenCapability( selectedChar, J_WIND ) )
	{
		strcpy( szTemp, g_ifMng->GetNumberString( selectedChar->genCapability[1] ) );
		m_cselInfoGenIcon1->ChangeControlTexInfo( IF_CHAR_WIN_PGEN_MARK_1_0 );
	}
	else
	{
		strcpy( szTemp, g_LPACK.GetMassage( 0, 465 ) );
		m_cselInfoGenIcon1->ChangeControlTexInfo( IF_CHAR_WIN_PGEN_MARK_1_0 + 1 );
	}
	m_textCtl[IT_CHARINFO_WIN_GENCAP_1_U]->SetText( szTemp );

	if( CheckAvailableGenCapability( selectedChar, J_WATER ) )
	{
		strcpy( szTemp, g_ifMng->GetNumberString( selectedChar->genCapability[2] ) );
		m_cselInfoGenIcon2->ChangeControlTexInfo( IF_CHAR_WIN_PGEN_MARK_2_0 );
	}
	else
	{
		strcpy( szTemp, g_LPACK.GetMassage( 0, 465 ) );
		m_cselInfoGenIcon2->ChangeControlTexInfo( IF_CHAR_WIN_PGEN_MARK_2_0 + 1 );
	}
	m_textCtl[IT_CHARINFO_WIN_GENCAP_2_U]->SetText( szTemp );

	if( CheckAvailableGenCapability( selectedChar, J_EARTH ) )
	{
		strcpy( szTemp, g_ifMng->GetNumberString( selectedChar->genCapability[3] ) );
		m_cselInfoGenIcon3->ChangeControlTexInfo( IF_CHAR_WIN_PGEN_MARK_3_0 );
	}
	else
	{
		strcpy( szTemp, g_LPACK.GetMassage( 0, 465 ) );
		m_cselInfoGenIcon3->ChangeControlTexInfo( IF_CHAR_WIN_PGEN_MARK_3_0 + 1 );
	}
	m_textCtl[IT_CHARINFO_WIN_GENCAP_3_U]->SetText( szTemp );
}