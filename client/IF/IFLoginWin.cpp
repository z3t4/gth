



#include "../global_def.h"
#include "IFCommon.h"







CIFLoginWin::CIFLoginWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	m_login = new CIFControl( this );
	m_loginTitleBar = new CIFControl( this );
	m_loginTitleBottom = new CIFControl( this );
	m_loginBar = new CIFControl( this );
	m_loginLeftBar = new CIFControl( this );
	m_loginRightBar = new CIFControl( this );
	m_loginIDBar = new CIFControl( this );
	m_loginPasswordBar = new CIFControl( this );
	m_loginOkBtn = new CIFButtonCtl( this );
	m_loginCancelBtn = new CIFButtonCtl( this );

#ifdef _GTH_ENABLE_GOODNOTICE
	
	m_loginNotice	= new CIFControl(this);
	m_cNoticeOkBtn = new CIFButtonCtl(this);
#endif
	
	
	for( int i = 0; i < MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN; i ++ )
		m_serverBtn[i] = new CIFButtonCtl( this );
	
	m_serverOkBtn = new CIFButtonCtl( this );
	m_serverCancelBtn = new CIFButtonCtl( this );

	
	m_cselInfo = new CIFControl( this );
	m_cselInfoTitleBar = new CIFControl( this );
	m_cselInfoTitleBottom = new CIFControl( this );
	m_cselInfoBar = new CIFControl( this );
	m_cselInfoGenBar = new CIFControl( this );
	m_cselInfoGenIcon0 = new CIFControl( this );
	m_cselInfoGenIcon1 = new CIFControl( this );
	m_cselInfoGenIcon2 = new CIFControl( this );
	m_cselInfoGenIcon3 = new CIFControl( this );
	
	m_DifferentCharBtn= new CIFButtonCtl( this );
	m_cselExitBtn = new CIFButtonCtl( this );
	m_cselOkBtn = new CIFButtonCtl( this );
	m_cselBackBtn = new CIFButtonCtl( this );
	m_cselDeleteBtn = new CIFButtonCtl( this );
	m_cselMakeBtn = new CIFButtonCtl( this );

#ifdef _GTH_FLAG_RELOGIN
	
	m_cReLoginBtn = new CIFButtonCtl (this);
#endif
	
	
	m_cmakeInfo = new CIFControl( this );
	m_cmakeInfoTitleBar = new CIFControl( this );
	m_cmakeInfoTitleBottom = new CIFControl( this );
	m_cmakeInfoBar = new CIFControl( this );
	m_cmake = new CIFControl( this );
	m_cmakeTitleBar = new CIFControl( this );
	m_cmakeTitleBottom = new CIFControl( this );
	m_cmakeBar = new CIFControl( this );
	m_cmakeNameIcon = new CIFControl( this );
	m_cmakeNameBar = new CIFControl( this );
	m_cmakeFaceIcon = new CIFControl( this );
	m_cmakeFaceBar = new CIFControl( this );
	m_cmakeHairIcon = new CIFControl( this );
	m_cmakeHairBar = new CIFControl( this );
	m_cmakeStatBar = new CIFControl( this );
	m_cmakeStatLine = new CIFControl( this );
	m_cmakeForceIcon = new CIFControl( this );
	m_cmakeForceBar = new CIFControl( this );
	m_cmakeSoulIcon = new CIFControl( this );
	m_cmakeSoulBar = new CIFControl( this );
	m_cmakeAgiIcon = new CIFControl( this );
	m_cmakeAgiBar = new CIFControl( this );
	m_cmakeVitIcon = new CIFControl( this );
	m_cmakeVitBar = new CIFControl( this );
	m_cmakeFacePrevBtn = new CIFButtonCtl( this );
	m_cmakeFaceNextBtn = new CIFButtonCtl( this );
	m_cmakeHairPrevBtn = new CIFButtonCtl( this );
	m_cmakeHairNextBtn = new CIFButtonCtl( this );
	m_cmakeForceUpBtn = new CIFButtonCtl( this );
	m_cmakeForceDnBtn = new CIFButtonCtl( this );
	m_cmakeSoulUpBtn = new CIFButtonCtl( this );
	m_cmakeSoulDnBtn = new CIFButtonCtl( this );
	m_cmakeAgiUpBtn = new CIFButtonCtl( this );
	m_cmakeAgiDnBtn = new CIFButtonCtl( this );
	m_cmakeVitUpBtn = new CIFButtonCtl( this );
	m_cmakeVitDnBtn = new CIFButtonCtl( this );
	m_cmakeOkBtn = new CIFButtonCtl( this );
	m_cmakeCancelBtn = new CIFButtonCtl( this );
	m_cmakePrevBtn = new CIFButtonCtl( this );
	m_cmakeNextBtn = new CIFButtonCtl( this );

	
	for( i = 0; i < NUM_IT_LOGIN_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	m_loginStep = -1;
	m_cameraStep = 0;
	m_faceFlag = 0;
	m_deleteFlag = false;
	m_makeFlag = false;
	m_deleteCharRemainTime = -1.0f;
	m_bDeleteCharFlag = false;

	
	InitCharacterSlot();
	
#ifdef _GTH_ENABLE_GOODNOTICE
	
	SetFirstConnectMasterServer(true);
#endif
	
	Enable( true );
}

CIFLoginWin::~CIFLoginWin()
{

}

void CIFLoginWin::PrepareControl()
{
	
	m_login->PrepareControl( IF_LOGIN_WIN );
	m_loginTitleBar->PrepareControl( IF_LOGIN_WIN_TITLE_BAR );
	m_loginTitleBottom->PrepareControl( IF_LOGIN_WIN_TITLE_BOTTOM );
	m_loginBar->PrepareControl( IF_LOGIN_WIN_LOGIN_BAR );
	m_loginLeftBar->PrepareControl( IF_LOGIN_WIN_LEFT_BAR );
	m_loginRightBar->PrepareControl( IF_LOGIN_WIN_RIGHT_BAR );
	m_loginIDBar->PrepareControl( IF_LOGIN_WIN_ID_BAR );
	m_loginPasswordBar->PrepareControl( IF_LOGIN_WIN_PASSWORD_BAR );
	m_loginOkBtn->PrepareControl( IF_LOGIN_WIN_OK_BTN_0 );
	m_loginCancelBtn->PrepareControl( IF_LOGIN_WIN_CANCEL_BTN_0 );

#ifdef _GTH_ENABLE_GOODNOTICE
	
	m_loginNotice->PrepareControl(IF_LOGIN_WIN_NOTICE);
	m_cNoticeOkBtn->PrepareControl(IF_LOGIN_WIN_NOTICE_OK_BTN_0);
#endif

	
	for( int i = 0; i < MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN; i ++ )
		m_serverBtn[i]->PrepareControl( IF_SERVER_BTN_0 );

	m_serverOkBtn->PrepareControl( IF_SERVER_OK_BTN_0 );
	m_serverCancelBtn->PrepareControl( IF_SERVER_CANCEL_BTN_0 );

	
	m_cselInfo->PrepareControl( IF_CHARINFO_WIN );
	m_cselInfoTitleBar->PrepareControl( IF_CHARINFO_WIN_TITLE_BAR );
	m_cselInfoTitleBottom->PrepareControl( IF_CHARINFO_WIN_TITLE_BOTTOM );
	m_cselInfoBar->PrepareControl( IF_CHARINFO_WIN_CHARINFO_BAR );
	m_cselInfoGenBar->PrepareControl( IF_CHARINFO_WIN_GEN_BAR );
	m_cselInfoGenIcon0->PrepareControl( IF_CHARINFO_WIN_GEN_ICON_0 );
	m_cselInfoGenIcon1->PrepareControl( IF_CHARINFO_WIN_GEN_ICON_1 );
	m_cselInfoGenIcon2->PrepareControl( IF_CHARINFO_WIN_GEN_ICON_2 );
	m_cselInfoGenIcon3->PrepareControl( IF_CHARINFO_WIN_GEN_ICON_3 );
	m_cselExitBtn->PrepareControl( IF_SERVER_BTN_0 );
	m_cselOkBtn->PrepareControl( IF_SERVER_BTN_0 );
	m_cselBackBtn->PrepareControl( IF_SERVER_BTN_0 );
	m_cselDeleteBtn->PrepareControl( IF_SERVER_BTN_0 );
	m_cselMakeBtn->PrepareControl( IF_SERVER_BTN_0 );

	
	m_DifferentCharBtn->PrepareControl( IF_SERVER_BTN_0 );

#ifdef _GTH_FLAG_RELOGIN
	
	m_cReLoginBtn->PrepareControl(IF_SERVER_BTN_0);
#endif

	
	m_cmakeInfo->PrepareControl( IF_CHARINFO2_WIN );
	m_cmakeInfoTitleBar->PrepareControl( IF_CHARINFO2_WIN_TITLE_BAR );
	m_cmakeInfoTitleBottom->PrepareControl( IF_CHARINFO2_WIN_TITLE_BOTTOM );
	m_cmakeInfoBar->PrepareControl( IF_CHARINFO2_WIN_CHARINFO_BAR );
	m_cmake->PrepareControl( IF_CHARMAKE_WIN );
	m_cmakeTitleBar->PrepareControl( IF_CHARMAKE_WIN_TITLE_BAR );
	m_cmakeTitleBottom->PrepareControl( IF_CHARMAKE_WIN_TITLE_BOTTOM );
	m_cmakeBar->PrepareControl( IF_CHARMAKE_WIN_CHARMAKE_BAR );
	m_cmakeNameIcon->PrepareControl( IF_CHARMAKE_WIN_NAME_ICON );
	m_cmakeNameBar->PrepareControl( IF_CHARMAKE_WIN_NAME_BAR );
	m_cmakeFaceIcon->PrepareControl( IF_CHARMAKE_WIN_FACE_ICON );
	m_cmakeFaceBar->PrepareControl( IF_CHARMAKE_WIN_FACE_BAR );
	m_cmakeHairIcon->PrepareControl( IF_CHARMAKE_WIN_HAIR_ICON );
	m_cmakeHairBar->PrepareControl( IF_CHARMAKE_WIN_HAIR_BAR );
	m_cmakeStatBar->PrepareControl( IF_CHARMAKE_WIN_STAT_BAR );
	m_cmakeStatLine->PrepareControl( IF_CHARMAKE_WIN_STAT_LINE );
	m_cmakeForceIcon->PrepareControl( IF_CHARMAKE_WIN_STAT_FORCE_ICON );
	m_cmakeForceBar->PrepareControl( IF_CHARMAKE_WIN_STAT_FORCE_BAR );
	m_cmakeSoulIcon->PrepareControl( IF_CHARMAKE_WIN_STAT_SOUL_ICON );
	m_cmakeSoulBar->PrepareControl( IF_CHARMAKE_WIN_STAT_SOUL_BAR );
	m_cmakeAgiIcon->PrepareControl( IF_CHARMAKE_WIN_STAT_AGI_ICON );
	m_cmakeAgiBar->PrepareControl( IF_CHARMAKE_WIN_STAT_AGI_BAR );
	m_cmakeVitIcon->PrepareControl( IF_CHARMAKE_WIN_STAT_VIT_ICON );
	m_cmakeVitBar->PrepareControl( IF_CHARMAKE_WIN_STAT_VIT_BAR );
	m_cmakeFacePrevBtn->PrepareControl( IF_CHARMAKE_WIN_FACE_PREV_BTN_0 );
	m_cmakeFaceNextBtn->PrepareControl( IF_CHARMAKE_WIN_FACE_NEXT_BTN_0 );
	m_cmakeHairPrevBtn->PrepareControl( IF_CHARMAKE_WIN_HAIR_PREV_BTN_0 );
	m_cmakeHairNextBtn->PrepareControl( IF_CHARMAKE_WIN_HAIR_NEXT_BTN_0 );
	m_cmakeForceUpBtn->PrepareControl( IF_CHARMAKE_WIN_STAT_FORCE_UP_BTN_0 );
	m_cmakeForceDnBtn->PrepareControl( IF_CHARMAKE_WIN_STAT_FORCE_DN_BTN_0 );
	m_cmakeSoulUpBtn->PrepareControl( IF_CHARMAKE_WIN_STAT_SOUL_UP_BTN_0 );
	m_cmakeSoulDnBtn->PrepareControl( IF_CHARMAKE_WIN_STAT_SOUL_DN_BTN_0 );
	m_cmakeAgiUpBtn->PrepareControl( IF_CHARMAKE_WIN_STAT_AGI_UP_BTN_0 );
	m_cmakeAgiDnBtn->PrepareControl( IF_CHARMAKE_WIN_STAT_AGI_DN_BTN_0 );
	m_cmakeVitUpBtn->PrepareControl( IF_CHARMAKE_WIN_STAT_VIT_UP_BTN_0 );
	m_cmakeVitDnBtn->PrepareControl( IF_CHARMAKE_WIN_STAT_VIT_DN_BTN_0 );
	m_cmakeOkBtn->PrepareControl( IF_CHARMAKE_WIN_OK_BTN_0 );
	m_cmakeCancelBtn->PrepareControl( IF_CHARMAKE_WIN_CANCEL_BTN_0 );
	m_cmakePrevBtn->PrepareControl( IF_SERVER_BTN_0 );
	m_cmakeNextBtn->PrepareControl( IF_SERVER_BTN_0 );


	
	m_textCtl[IT_LOGIN_WIN_LOGIN]->PrepareControl( IF_LOGIN_WIN_LOGIN_TEXT );
	m_textCtl[IT_LOGIN_WIN_ID]->PrepareControl( IF_LOGIN_WIN_ID_TEXT );
	m_textCtl[IT_LOGIN_WIN_PASSWORD]->PrepareControl( IF_LOGIN_WIN_PASSWORD_TEXT );
	m_textCtl[IT_LOGIN_WIN_ID_U]->PrepareControl( IF_LOGIN_WIN_ID_TEXT_U );
	m_textCtl[IT_LOGIN_WIN_PASSWORD_U]->PrepareControl( IF_LOGIN_WIN_PASSWORD_TEXT_U );
	

	
	m_textCtl[IT_LOGIN_WIN_VERSION_TEXT_POS]->PrepareControl( IF_LOGIN_WIN_VERSION_TEXT );

	m_textCtl[IT_CHARINFO_WIN_CHARINFO]->PrepareControl( IF_CHARINFO_WIN_CHARINFO_TEXT );
	m_textCtl[IT_CHARINFO_WIN_NAME_U]->PrepareControl( IF_CHARINFO_WIN_NAME_TEXT_U );
	m_textCtl[IT_CHARINFO_WIN_LEVEL_U]->PrepareControl( IF_CHARINFO_WIN_LEVEL_TEXT_U );
	m_textCtl[IT_CHARINFO_WIN_RANK_U]->PrepareControl( IF_CHARINFO_WIN_RANK_TEXT_U );
	m_textCtl[IT_CHARINFO_WIN_CLASS_U]->PrepareControl( IF_CHARINFO_WIN_CLASS_TEXT_U );
	m_textCtl[IT_CHARINFO_WIN_GEN_BAR]->PrepareControl( IF_CHARINFO_WIN_GEN_BAR_TEXT );
	m_textCtl[IT_CHARINFO_WIN_GEN_ICON_0_U]->PrepareControl( IF_CHARINFO_WIN_GEN_ICON_0_TEXT_U );
	m_textCtl[IT_CHARINFO_WIN_GEN_ICON_1_U]->PrepareControl( IF_CHARINFO_WIN_GEN_ICON_1_TEXT_U );
	m_textCtl[IT_CHARINFO_WIN_GEN_ICON_2_U]->PrepareControl( IF_CHARINFO_WIN_GEN_ICON_2_TEXT_U );
	m_textCtl[IT_CHARINFO_WIN_GEN_ICON_3_U]->PrepareControl( IF_CHARINFO_WIN_GEN_ICON_3_TEXT_U );
	m_textCtl[IT_CHARINFO_WIN_GENCAP_0_U]->PrepareControl( IF_CHARINFO_WIN_GENCAP_0_TEXT_U );
	m_textCtl[IT_CHARINFO_WIN_GENCAP_1_U]->PrepareControl( IF_CHARINFO_WIN_GENCAP_1_TEXT_U );
	m_textCtl[IT_CHARINFO_WIN_GENCAP_2_U]->PrepareControl( IF_CHARINFO_WIN_GENCAP_2_TEXT_U );
	m_textCtl[IT_CHARINFO_WIN_GENCAP_3_U]->PrepareControl( IF_CHARINFO_WIN_GENCAP_3_TEXT_U );


	m_textCtl[IT_CHARINFO2_WIN_CHARINFO]->PrepareControl( IF_CHARINFO2_WIN_CHARINFO_TEXT );
	m_textCtl[IT_CHARINFO2_WIN_U]->PrepareControl( IF_CHARINFO2_WIN_TEXT_U );
	m_textCtl[IT_CHARMAKE_WIN_CHARMAKE]->PrepareControl( IF_CHARMAKE_WIN_CHARMAKE_TEXT );
	m_textCtl[IT_CHARMAKE_WIN_NAME]->PrepareControl( IF_CHARMAKE_WIN_NAME_TEXT );
	m_textCtl[IT_CHARMAKE_WIN_NAME_U]->PrepareControl( IF_CHARMAKE_WIN_NAME_TEXT_U );
	m_textCtl[IT_CHARMAKE_WIN_FACE]->PrepareControl( IF_CHARMAKE_WIN_FACE_TEXT );
	m_textCtl[IT_CHARMAKE_WIN_FACE_U]->PrepareControl( IF_CHARMAKE_WIN_FACE_TEXT_U );
	m_textCtl[IT_CHARMAKE_WIN_HAIR]->PrepareControl( IF_CHARMAKE_WIN_HAIR_TEXT );
	m_textCtl[IT_CHARMAKE_WIN_HAIR_U]->PrepareControl( IF_CHARMAKE_WIN_HAIR_TEXT_U );
	m_textCtl[IT_CHARMAKE_WIN_STAT]->PrepareControl( IF_CHARMAKE_WIN_STAT_TEXT );
	m_textCtl[IT_CHARMAKE_WIN_STAT_FORCE]->PrepareControl( IF_CHARMAKE_WIN_STAT_FORCE_TEXT );
	m_textCtl[IT_CHARMAKE_WIN_STAT_FORCE_U]->PrepareControl( IF_CHARMAKE_WIN_STAT_FORCE_TEXT_U );
	m_textCtl[IT_CHARMAKE_WIN_STAT_SOUL]->PrepareControl( IF_CHARMAKE_WIN_STAT_SOUL_TEXT );
	m_textCtl[IT_CHARMAKE_WIN_STAT_SOUL_U]->PrepareControl( IF_CHARMAKE_WIN_STAT_SOUL_TEXT_U );
	m_textCtl[IT_CHARMAKE_WIN_STAT_AGI]->PrepareControl( IF_CHARMAKE_WIN_STAT_AGI_TEXT );
	m_textCtl[IT_CHARMAKE_WIN_STAT_AGI_U]->PrepareControl( IF_CHARMAKE_WIN_STAT_AGI_TEXT_U );
	m_textCtl[IT_CHARMAKE_WIN_STAT_VIT]->PrepareControl( IF_CHARMAKE_WIN_STAT_VIT_TEXT );
	m_textCtl[IT_CHARMAKE_WIN_STAT_VIT_U]->PrepareControl( IF_CHARMAKE_WIN_STAT_VIT_TEXT_U );
	m_textCtl[IT_CHARMAKE_WIN_OK]->PrepareControl( IF_CHARMAKE_WIN_OK_TEXT );
	m_textCtl[IT_CHARMAKE_WIN_CANCEL]->PrepareControl( IF_CHARMAKE_WIN_CANCEL_TEXT );	
}

#include "../CGameVersion.h"
extern CGameVersion* g_GameVersion;
void CIFLoginWin::SettingDrawText()
{
	char szTemp[256];
	
	
	m_textCtl[IT_LOGIN_WIN_LOGIN]->SetText( g_LPACK.GetMassage( 0, 503 ) );
	
	m_textCtl[IT_LOGIN_WIN_ID]->SetText( g_LPACK.GetMassage( 0, 504 ) );
	
	m_textCtl[IT_LOGIN_WIN_PASSWORD]->SetText( g_LPACK.GetMassage( 0, 505 ) );
	
	m_textCtl[IT_CHARINFO_WIN_CHARINFO]->SetText( g_LPACK.GetMassage( 0, 506 ) );
	



	
	
	if ( g_GameVersion != NULL)
	{
		sprintf(szTemp,"ver. %.2f ",g_GameVersion->m_iClientGameVersion);		
		m_textCtl[IT_LOGIN_WIN_VERSION_TEXT_POS]->SetText(szTemp);
	}
	
	
	
	



	m_DifferentCharBtn->SetButtonText( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 595 ) );


	
	m_cselExitBtn->SetButtonText( g_LPACK.GetMassage( 0, 507 ) );
	
	m_cselOkBtn->SetButtonText( g_LPACK.GetMassage( 0, 508 ) );
	
	m_cselBackBtn->SetButtonText( g_LPACK.GetMassage( 0, 509 ) );
	
	m_cselDeleteBtn->SetButtonText( g_LPACK.GetMassage( 0, 510 ) );
	
	m_cselMakeBtn->SetButtonText( g_LPACK.GetMassage( 0, 511 ) );
	
	m_cmakePrevBtn->SetButtonText( g_LPACK.GetMassage( 0, 512 ) );
	
	m_cmakeNextBtn->SetButtonText( g_LPACK.GetMassage( 0, 513 ) );
	
#ifdef _GTH_FLAG_RELOGIN	
	
	m_cReLoginBtn->SetButtonText(g_LPACK.GetMassage( 9, 69 ));
#endif

#ifdef _GTH_ENABLE_GOODNOTICE	
	
	m_cNoticeOkBtn->SetButtonText(g_LPACK.GetMassage( 0, 549 ));
#endif
	
	if( m_loginStep == LOGIN_STATE_CREATE_CHARACTER )
	{
		
		m_textCtl[IT_CHARINFO2_WIN_U]->SetText( g_LPACK.GetMassage( 0, 514 ) );
		
		
		m_textCtl[IT_CHARMAKE_WIN_CHARMAKE]->SetText( g_LPACK.GetMassage( 0, 515 ) );
		
		m_textCtl[IT_CHARMAKE_WIN_NAME]->SetText( g_LPACK.GetMassage( 0, 516 ) );
		
		
		m_textCtl[IT_CHARMAKE_WIN_FACE]->SetText( g_LPACK.GetMassage( 0, 517 ) );
		
		sprintf( szTemp, "%d/%d", m_makeCharData[m_curSelectedMakeCharacter].skinPart[1] + 1, 
			m_makeCharData[m_curSelectedMakeCharacter].maxFaceCount );
		m_textCtl[IT_CHARMAKE_WIN_FACE_U]->SetText( szTemp );
		
		m_textCtl[IT_CHARMAKE_WIN_HAIR]->SetText( g_LPACK.GetMassage( 0, 518 ) );
		
		sprintf( szTemp, "%d/%d", m_makeCharData[m_curSelectedMakeCharacter].skinPart[0] + 1, 
			m_makeCharData[m_curSelectedMakeCharacter].maxHairCount );
		m_textCtl[IT_CHARMAKE_WIN_HAIR_U]->SetText( szTemp );
		
		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 519 ), m_makeCharData[m_curSelectedMakeCharacter].bonusPoint );
		m_textCtl[IT_CHARMAKE_WIN_STAT]->SetText( szTemp );
		
		
		m_textCtl[IT_CHARMAKE_WIN_STAT_FORCE]->SetText( g_LPACK.GetMassage( 0, 479 ) );
		sprintf( szTemp, "%d", m_makeCharData[m_curSelectedMakeCharacter].curForce );
		m_textCtl[IT_CHARMAKE_WIN_STAT_FORCE_U]->SetText( szTemp );
		
		m_textCtl[IT_CHARMAKE_WIN_STAT_SOUL]->SetText( g_LPACK.GetMassage( 0, 481 ) );
		sprintf( szTemp, "%d", m_makeCharData[m_curSelectedMakeCharacter].curSoulpower );
		m_textCtl[IT_CHARMAKE_WIN_STAT_SOUL_U]->SetText( szTemp );
		
		m_textCtl[IT_CHARMAKE_WIN_STAT_AGI]->SetText( g_LPACK.GetMassage( 0, 480 ) );
		sprintf( szTemp, "%d", m_makeCharData[m_curSelectedMakeCharacter].curAgility );
		m_textCtl[IT_CHARMAKE_WIN_STAT_AGI_U]->SetText( szTemp );
		
		m_textCtl[IT_CHARMAKE_WIN_STAT_VIT]->SetText( g_LPACK.GetMassage( 0, 482 ) );
		sprintf( szTemp, "%d", m_makeCharData[m_curSelectedMakeCharacter].curVitality );
		m_textCtl[IT_CHARMAKE_WIN_STAT_VIT_U]->SetText( szTemp );
		
		
		m_textCtl[IT_CHARMAKE_WIN_OK]->SetText( g_LPACK.GetMassage( 0, 520 ) );
		
		m_textCtl[IT_CHARMAKE_WIN_CANCEL]->SetText( g_LPACK.GetMassage( 0, 521 ) );
	}
}




void CIFLoginWin::IdPwRender()
{
	m_login->Render();
	m_loginTitleBar->Render();
	m_loginTitleBottom->Render();
	m_loginBar->Render();
	m_loginLeftBar->Render();
	m_loginRightBar->Render();
	m_loginIDBar->Render();
	m_loginPasswordBar->Render();
	m_loginOkBtn->Render();
	m_loginCancelBtn->Render();
}

void CIFLoginWin::CreateCharacterRender()
{
	m_cmakeInfo->Render();
	m_cmakeInfoTitleBar->Render();
	m_cmakeInfoTitleBottom->Render();
	m_cmakeInfoBar->Render();
	m_cmake->Render();
	m_cmakeTitleBar->Render();
	m_cmakeTitleBottom->Render();
	m_cmakeBar->Render();
	m_cmakeNameIcon->Render();
	m_cmakeNameBar->Render();
	m_cmakeFaceIcon->Render();
	m_cmakeFaceBar->Render();
	m_cmakeHairIcon->Render();
	m_cmakeHairBar->Render();
	m_cmakeStatBar->Render();
	m_cmakeStatLine->Render();
	m_cmakeForceIcon->Render();
	m_cmakeForceBar->Render();
	m_cmakeSoulIcon->Render();
	m_cmakeSoulBar->Render();
	m_cmakeAgiIcon->Render();
	m_cmakeAgiBar->Render();
	m_cmakeVitIcon->Render();
	m_cmakeVitBar->Render();
	m_cmakeFacePrevBtn->Render();
	m_cmakeFaceNextBtn->Render();
	m_cmakeHairPrevBtn->Render();
	m_cmakeHairNextBtn->Render();
	m_cmakeForceUpBtn->Render();
	m_cmakeForceDnBtn->Render();
	m_cmakeSoulUpBtn->Render();
	m_cmakeSoulDnBtn->Render();
	m_cmakeAgiUpBtn->Render();
	m_cmakeAgiDnBtn->Render();
	m_cmakeVitUpBtn->Render();
	m_cmakeVitDnBtn->Render();
	m_cmakeOkBtn->Render();
	m_cmakeCancelBtn->Render();
	m_cmakePrevBtn->RenderTextBtn();
	m_cmakeNextBtn->RenderTextBtn();
}

void CIFLoginWin::TextRender()
{
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{


		
		m_textCtl[IT_LOGIN_WIN_VERSION_TEXT_POS]->Render();				

		
		if( m_loginStep == LOGIN_STATE_IDPW )
		{	
			if( m_login->State() )
			{
				m_textCtl[IT_LOGIN_WIN_LOGIN]->Render();
				m_textCtl[IT_LOGIN_WIN_ID]->Render();
				m_textCtl[IT_LOGIN_WIN_PASSWORD]->Render();
				
			}
		}
		if( m_cselInfo->State() )
		{
			m_textCtl[IT_CHARINFO_WIN_CHARINFO]->Render();
			m_textCtl[IT_CHARINFO_WIN_NAME_U]->Render();
			m_textCtl[IT_CHARINFO_WIN_LEVEL_U]->Render();
			m_textCtl[IT_CHARINFO_WIN_RANK_U]->Render();
			m_textCtl[IT_CHARINFO_WIN_CLASS_U]->Render();
			m_textCtl[IT_CHARINFO_WIN_GEN_BAR]->Render();
			m_textCtl[IT_CHARINFO_WIN_GEN_ICON_0_U]->Render();
			m_textCtl[IT_CHARINFO_WIN_GEN_ICON_1_U]->Render();
			m_textCtl[IT_CHARINFO_WIN_GEN_ICON_2_U]->Render();
			m_textCtl[IT_CHARINFO_WIN_GEN_ICON_3_U]->Render();
			m_textCtl[IT_CHARINFO_WIN_GENCAP_0_U]->Render();
			m_textCtl[IT_CHARINFO_WIN_GENCAP_1_U]->Render();
			m_textCtl[IT_CHARINFO_WIN_GENCAP_2_U]->Render();
			m_textCtl[IT_CHARINFO_WIN_GENCAP_3_U]->Render();
		}
		if( m_cmakeInfo->State() )
		{
			m_textCtl[IT_CHARINFO2_WIN_CHARINFO]->Render();

			
			
			
			int x, y, xs, ys;
			x = m_textCtl[IT_CHARINFO2_WIN_U]->GetXPos();
			y = m_textCtl[IT_CHARINFO2_WIN_U]->GetYPos();
			xs = m_textCtl[IT_CHARINFO2_WIN_U]->GetXSize();
			ys = m_textCtl[IT_CHARINFO2_WIN_U]->GetYSize();
			g_chatManager->SetColor( 0xffffffff );
			g_chatManager->DrawTextRect( x, y, g_LPACK.GetMessage( 0, 718+m_curSelectedMakeCharacter ), xs, ys, 0 );

			m_textCtl[IT_CHARMAKE_WIN_CHARMAKE]->Render();
			m_textCtl[IT_CHARMAKE_WIN_NAME]->Render();
			m_textCtl[IT_CHARMAKE_WIN_FACE]->Render();
			m_textCtl[IT_CHARMAKE_WIN_FACE_U]->Render();
			m_textCtl[IT_CHARMAKE_WIN_HAIR]->Render();
			m_textCtl[IT_CHARMAKE_WIN_HAIR_U]->Render();
			m_textCtl[IT_CHARMAKE_WIN_STAT]->Render();
			m_textCtl[IT_CHARMAKE_WIN_STAT_FORCE]->Render();
			m_textCtl[IT_CHARMAKE_WIN_STAT_FORCE_U]->Render();
			m_textCtl[IT_CHARMAKE_WIN_STAT_SOUL]->Render();
			m_textCtl[IT_CHARMAKE_WIN_STAT_SOUL_U]->Render();
			m_textCtl[IT_CHARMAKE_WIN_STAT_AGI]->Render();
			m_textCtl[IT_CHARMAKE_WIN_STAT_AGI_U]->Render();
			m_textCtl[IT_CHARMAKE_WIN_STAT_VIT]->Render();
			m_textCtl[IT_CHARMAKE_WIN_STAT_VIT_U]->Render();
			m_textCtl[IT_CHARMAKE_WIN_OK]->Render();
			m_textCtl[IT_CHARMAKE_WIN_CANCEL]->Render();
		}
		g_chatManager->RenderInputFrame( 0 );
		g_chatManager->RenderInputFrame( 1 );
	}
	g_chatManager->EndFrame();
}






void CIFLoginWin::NoticeProcess()
{
	SetFirstConnectMasterServer(false);

	if( m_cNoticeOkBtn->ProcessButton() == IF_CTL_SELECT || g_input->KeyDown( DIK_RETURN ) )
	{	
		
		if ( !GTH_StartUpGame() )	return ;

		
		ChangeLoginStep(LOGIN_STATE_SELECT_MASTER_SERVER);	
		MakeMasterServerGroupList();
	}
}

void CIFLoginWin::IdPwProcess()
{
	if( m_loginIDBar->Selection() == IF_LBDN ) g_chatManager->ChangeInputMessage( 0 );
	if( m_loginPasswordBar->Selection() == IF_LBDN ) g_chatManager->ChangeInputMessage( 1 );
	if( m_loginOkBtn->ProcessButton() == IF_CTL_SELECT || g_input->KeyDown( DIK_RETURN ) )
	{
		RequestLogin();
	}
	if( m_loginCancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,320), g_LPACK.GetMassage(0,321), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_EXIT_CLIENT );
	}
}


void CIFLoginWin::SelectServerProcess()
{		

	














	return;
}

void CIFLoginWin::SelectCharacterProcess()
{
	CharacterSelectProcess();

	if( m_cselExitBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,320), g_LPACK.GetMassage(0,321), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_EXIT_CLIENT );
	}
	if( m_cselOkBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		RequestServerAddress();
	}

	
	if ( m_DifferentCharBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if (g_cgv.characterSlotMode == globalVariable_t::CharacterSlotMode::BACK)
			g_cgv.characterSlotMode = globalVariable_t::CharacterSlotMode::FRONT;
		else
			g_cgv.characterSlotMode = globalVariable_t::CharacterSlotMode::BACK;	
		
			
			ChangeCharacterSlot();
			

	}

#ifdef _GTH_FLAG_RELOGIN
	
	if(m_cReLoginBtn->ProcessButton() == IF_CTL_SELECT)
	{
		
		GTH_ShutdownGame();

		
		Sleep(500);
		
		
		if ( !GTH_StartUpGame() )	return ;

		g_charManager->GenerateCharacter( NULL, 0 );
		
		
		ChangeLoginStep(LOGIN_STATE_IDPW);
	}
#endif			

	if( m_cselBackBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		
		if( m_selectedCharacter != NULL ) 
		{
			m_cameraStep = 1;
		}
		
		else
		{
			g_cgv.restartState = RESTART_RESERVER;
			g_ifMng->m_changeStateFlag = true;
			return ;
		}
	}
	if( m_cselDeleteBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,93), g_LPACK.GetMassage(0,94), 0, IF_MESSAGE_DELETE_CHARACTER );
	}
	if( m_cselMakeBtn->ProcessButton() == IF_CTL_SELECT )
	{
#ifdef _GTH_ONLINE_VERSION
		GTH_SendMessage_RequestReadyCreate();
#else
		RequestMakeCharacter();
#endif
	}
}

void CIFLoginWin::CreateCharacterProcess()
{
	CharacterSelectProcess();
	
	if( m_cmakePrevBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		m_nextSelectedMakeCharacter = m_curSelectedMakeCharacter - 1;
		if( m_nextSelectedMakeCharacter < 0 ) m_nextSelectedMakeCharacter = 3;
		m_cameraStep = 3 + m_curSelectedMakeCharacter;
		m_cameraStepFlag = false;
	}
	
	if( m_cmakeNextBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		m_nextSelectedMakeCharacter = m_curSelectedMakeCharacter + 1;
		if( m_nextSelectedMakeCharacter > 3 ) m_nextSelectedMakeCharacter = 0;
		m_cameraStep = 3 + m_curSelectedMakeCharacter;
		m_cameraStepFlag = false;
	}
	
	if( m_cmakeOkBtn->ProcessButton() == IF_CTL_SELECT )
	{
		RequestCheckCharacterName();
	}
	if( m_cmakeCancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		ChangeLoginStep( LOGIN_STATE_SELECT_CHARACTER );
		m_makeFlag = false;
		m_cameraStep = 3 + m_curSelectedMakeCharacter;
		m_cameraStepFlag = false;
	}
	
	if( m_cmakeFacePrevBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_makeCharData[m_curSelectedMakeCharacter].skinPart[1] <= 0 ) return;
		m_makeCharData[m_curSelectedMakeCharacter].skinPart[1] --;
		
		g_charManager->m_Characters[3 + m_curSelectedMakeCharacter].skinPart[1] = 
			m_makeCharData[m_curSelectedMakeCharacter].skinPart[1];

		g_charManager->GenerateOtherCharacter( 3 + m_curSelectedMakeCharacter );
		SetFaceCamera( 0 );
	}
	if( m_cmakeFaceNextBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_makeCharData[m_curSelectedMakeCharacter].skinPart[1] >= 
			m_makeCharData[m_curSelectedMakeCharacter].maxFaceCount - 1 ) return;
		m_makeCharData[m_curSelectedMakeCharacter].skinPart[1] ++;
		
		g_charManager->m_Characters[3 + m_curSelectedMakeCharacter].skinPart[1] = 
			m_makeCharData[m_curSelectedMakeCharacter].skinPart[1];
		
		g_charManager->GenerateOtherCharacter( 3 + m_curSelectedMakeCharacter );
		SetFaceCamera( 0 );
	}
	
	if( m_cmakeHairPrevBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_makeCharData[m_curSelectedMakeCharacter].skinPart[0] <= 0 ) return;
		m_makeCharData[m_curSelectedMakeCharacter].skinPart[0] --;

		g_charManager->m_Characters[3 + m_curSelectedMakeCharacter].skinPart[0] = 
			m_makeCharData[m_curSelectedMakeCharacter].skinPart[0];
		
		g_charManager->GenerateOtherCharacter( 3 + m_curSelectedMakeCharacter );
		SetFaceCamera( 0 );
	}
	if( m_cmakeHairNextBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_makeCharData[m_curSelectedMakeCharacter].skinPart[0] >= 
			m_makeCharData[m_curSelectedMakeCharacter].maxHairCount - 1 ) return;
		m_makeCharData[m_curSelectedMakeCharacter].skinPart[0] ++;

		g_charManager->m_Characters[3 + m_curSelectedMakeCharacter].skinPart[0] = 
			m_makeCharData[m_curSelectedMakeCharacter].skinPart[0];
		
		g_charManager->GenerateOtherCharacter( 3 + m_curSelectedMakeCharacter );
		SetFaceCamera( 0 );
	}
	
	if( m_cmakeForceUpBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_makeCharData[m_curSelectedMakeCharacter].bonusPoint <= 0 ) return;

		m_makeCharData[m_curSelectedMakeCharacter].curForce ++;
		m_makeCharData[m_curSelectedMakeCharacter].bonusPoint --;

		SetFaceCamera( 1 );
	}
	if( m_cmakeForceDnBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_makeCharData[m_curSelectedMakeCharacter].curForce <= 
			m_makeCharData[m_curSelectedMakeCharacter].force ) return;

		m_makeCharData[m_curSelectedMakeCharacter].curForce --;
		m_makeCharData[m_curSelectedMakeCharacter].bonusPoint ++;

		SetFaceCamera( 1 );
	}
	if( m_cmakeSoulUpBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_makeCharData[m_curSelectedMakeCharacter].bonusPoint <= 0 ) return;

		m_makeCharData[m_curSelectedMakeCharacter].curSoulpower ++;
		m_makeCharData[m_curSelectedMakeCharacter].bonusPoint --;

		SetFaceCamera( 1 );
	}
	if( m_cmakeSoulDnBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_makeCharData[m_curSelectedMakeCharacter].curSoulpower <= 
			m_makeCharData[m_curSelectedMakeCharacter].soulpower ) return;

		m_makeCharData[m_curSelectedMakeCharacter].curSoulpower --;
		m_makeCharData[m_curSelectedMakeCharacter].bonusPoint ++;

		SetFaceCamera( 1 );
	}
	if( m_cmakeAgiUpBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_makeCharData[m_curSelectedMakeCharacter].bonusPoint <= 0 ) return;

		m_makeCharData[m_curSelectedMakeCharacter].curAgility ++;
		m_makeCharData[m_curSelectedMakeCharacter].bonusPoint --;

		SetFaceCamera( 1 );
	}
	if( m_cmakeAgiDnBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_makeCharData[m_curSelectedMakeCharacter].curAgility <= 
			m_makeCharData[m_curSelectedMakeCharacter].agility ) return;

		m_makeCharData[m_curSelectedMakeCharacter].curAgility --;
		m_makeCharData[m_curSelectedMakeCharacter].bonusPoint ++;

		SetFaceCamera( 1 );
	}
	if( m_cmakeVitUpBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_makeCharData[m_curSelectedMakeCharacter].bonusPoint <= 0 ) return;

		m_makeCharData[m_curSelectedMakeCharacter].curVitality ++;
		m_makeCharData[m_curSelectedMakeCharacter].bonusPoint --;

		SetFaceCamera( 1 );
	}
	if( m_cmakeVitDnBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_makeCharData[m_curSelectedMakeCharacter].curVitality <= 
			m_makeCharData[m_curSelectedMakeCharacter].vitality ) return;

		m_makeCharData[m_curSelectedMakeCharacter].curVitality --;
		m_makeCharData[m_curSelectedMakeCharacter].bonusPoint ++;

		SetFaceCamera( 1 );
	}
}

void CIFLoginWin::SelectMasterServer()
{
	for( int i = 0; i < g_cgv.masterServerGroupNumber; i ++ )
	{
		if( m_serverBtn[i]->ProcessButton() == IF_CTL_SELECT ) break;
	}
	if( i == g_cgv.masterServerGroupNumber ) return;
	g_cgv.masterServerNumber = g_cgv.masterServerGroupInfo[i].masterServerNumber;
	for( int j = 0; j < g_cgv.masterServerGroupInfo[i].masterServerNumber; j ++ )
	{
		strcpy( g_cgv.listMasterServerIP[j], g_cgv.masterServerGroupInfo[i].listMasterServerIP[j] );
		g_cgv.listMasterServerPort[j] = g_cgv.masterServerGroupInfo[i].listMasterServerPort[j];
	}
	if( !GTH_Network_EstablishMasterServerConnection() )
	{
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,253), 1, IF_MESSAGE_EXIT_CLIENT );
	}
	else
	{
		ChangeLoginStep( LOGIN_STATE_IDPW );
		
		
#ifdef _GTH_MALAYSIA_CHN_VERSION
		RequestLogin();
#endif
	}
			
}





void CIFLoginWin::CharacterSelect_SelectScreenProcess()
{
	if( m_selectedCharacter == NULL )
	{
		
		m_DifferentCharBtn->Enable();

		m_cselExitBtn->Enable();
		m_cselOkBtn->Enable( false );
		
		
		m_cselBackBtn->Enable();
		m_cselDeleteBtn->Enable( false );
		m_cselMakeBtn->Enable( false );

#ifdef _GTH_FLAG_RELOGIN
		
		m_cReLoginBtn->Enable();
#endif	
	}
	else if( m_selectedCharacter->entityType == ENTITY_NPC )
	{
		
		m_DifferentCharBtn->Enable(false);

		m_cselExitBtn->Enable( false );
		m_cselOkBtn->Enable( false );
		m_cselBackBtn->Enable();
		m_cselDeleteBtn->Enable( false );
		m_cselMakeBtn->Enable();

#ifdef _GTH_FLAG_RELOGIN
		
		m_cReLoginBtn->Enable(false);
#endif
	}
	else
	{
		
		m_DifferentCharBtn->Enable(false);
		
		m_cselExitBtn->Enable( false );
		m_cselOkBtn->Enable();
		m_cselBackBtn->Enable();
		m_cselDeleteBtn->Enable();
		m_cselMakeBtn->Enable( false );

#ifdef _GTH_FLAG_RELOGIN
		
		m_cReLoginBtn->Enable(false);
#endif
	}
	int cx, cy, n = 0, i = 0;
	if( m_cselExitBtn->State() ) n ++;
	if( m_cselOkBtn->State() ) n ++;
	if( m_cselBackBtn->State() ) n ++;
	if( m_cselMakeBtn->State() ) n ++;
	
	if ( m_DifferentCharBtn->State() ) n ++;


	
	cx = g_ifMng->m_clientWidth - m_cselExitBtn[0].m_info[0].sizeX - 50;
	cy = g_ifMng->m_clientHeight - ( m_cselExitBtn[0].m_info[0].sizeY * n ) - 50;
	
	if( m_cselExitBtn->State() ) 
	{
		m_cselExitBtn->UpdateClientPosition( cx, cy + m_cselExitBtn[0].m_info[0].sizeY * i );
		i ++;
	}
	if( m_cselOkBtn->State() )
	{
		m_cselOkBtn->UpdateClientPosition( cx, cy + m_cselExitBtn[0].m_info[0].sizeY * i );
		i ++;
	}
	if( m_cselBackBtn->State() )
	{
		m_cselBackBtn->UpdateClientPosition( cx, cy + m_cselExitBtn[0].m_info[0].sizeY * i );
		i ++;
	}
	if( m_cselMakeBtn->State() )
	{
		m_cselMakeBtn->UpdateClientPosition( cx, cy + m_cselExitBtn[0].m_info[0].sizeY * i );
		i ++;
	}

	
	if( m_DifferentCharBtn->State() ) 
	{
		m_DifferentCharBtn->UpdateClientPosition( cx, cy + m_cselExitBtn[0].m_info[0].sizeY * i );
		i ++;
	}	

	
	

	if( m_cselDeleteBtn->State() )
	{
		cx = 50;
		cy = g_ifMng->m_clientHeight - ( m_cselExitBtn[0].m_info[0].sizeY * n ) - 50;
		m_cselDeleteBtn->UpdateClientPosition( cx, cy + m_cselExitBtn[0].m_info[0].sizeY );
	}
#ifdef _GTH_FLAG_RELOGIN
	
	if(m_cReLoginBtn->State())
	{
		cx = 50;
		cy = g_ifMng->m_clientHeight - ( m_cselExitBtn[0].m_info[0].sizeY * n ) - 50;

		m_cReLoginBtn->UpdateClientPosition(cx, cy + m_cselExitBtn[0].m_info[0].sizeY);
	}
#endif
}

void CIFLoginWin::CharacterSelect_CreateScreenProcess()
{
	m_cmakePrevBtn->Enable();
	m_cmakeNextBtn->Enable();
	m_cmakeInfo->Enable();
	int cx, cy;
	cx = g_ifMng->m_clientWidth - m_cselExitBtn[0].m_info[0].sizeX - 50;
	cy = g_ifMng->m_clientHeight - ( m_cselExitBtn[0].m_info[0].sizeY * 2 ) - 50;
	m_cmakePrevBtn->UpdateClientPosition( cx, cy );
	m_cmakeNextBtn->UpdateClientPosition( cx, cy + m_cselExitBtn[0].m_info[0].sizeY * 1 );
}


void CIFLoginWin::Storoge_Action()
{
	
	if(m_nMembershipType < LOGIN_CHAR_PU_NEW)
	{
		for(int i=0;i<3;i++)
		{
			
			if(g_charManager->m_Characters[i].entityType != ENTITY_PC) continue;

			if(i != (m_nMembershipType-LOGIN_CHAR_FREE_LEFT) && i != (m_btTempMembershipType-LOGIN_CHAR_FREE_LEFT))
			{
				GTH_Character_Sit( &g_charManager->m_Characters[i],1000.0f,false);
			}
			else
			{
				GTH_Character_Idle(&g_charManager->m_Characters[i]);
			}
		}
	}
	else
	{
		for(int i=0;i<3;i++)
		{
			
			if(g_charManager->m_Characters[i].entityType != ENTITY_PC) continue;
			GTH_Character_Idle(&g_charManager->m_Characters[i]);
		}
	}

	
	if(m_bCharacterShout)
	{
		int CharIndex = m_nMembershipType-LOGIN_CHAR_FREE_LEFT;
		Fx_CHARACTER_t* character = g_charManager->GetCharacter(CharIndex);
		if(character != NULL)
		{
			
			if( character->entityType == ENTITY_PC && character->ani_curSetID == ANIM_ITEM_IDLE )
			{
				m_bCharacterShout=false;
				GTH_Character_Shout(character);
			}
		}
	}
}

bool CIFLoginWin::Storoge_SelectCharacter(Fx_CHARACTER_t *g_mouseSelectedCharacter)
{
	
	
	if(m_nMembershipType < LOGIN_CHAR_PU_NEW)
	{
		
		if(m_nMembershipType >= LOGIN_CHAR_FREE_LEFT && m_nMembershipType <= LOGIN_CHAR_FREE_RIGHT)
		{
			if(m_nMembershipType-LOGIN_CHAR_FREE_LEFT != g_mouseSelectedCharacter->ID)
			{
				g_ifMng->SetMessage(g_LPACK.GetMassage(0,322),g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,214), MSG_BUTTONTYPE_OK );
				g_mouseSelectedCharacter=NULL;
				m_cameraStep=0;
				return false;
			}
		}
		
		else if(m_btSelectCharStage != CREATE_NONE_CHAR)
		{				
			m_btTempMembershipType = g_mouseSelectedCharacter->ID + LOGIN_CHAR_FREE_LEFT;
			GTH_Character_Stand(g_mouseSelectedCharacter);

			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,216), MSG_BUTTONTYPE_OKCANCEL ,IF_MESSAGE_FREE_USER_SELECT_SLOT);
			GTH_Character_Idle(g_mouseSelectedCharacter);
		}
	}
	

	return true;
}

void CIFLoginWin::CharacterSelect_CameraProcess()
{
	extern Fx_CHARACTER_t *g_mouseSelectedCharacter;

	vec2_t mouse;
	vec2_t viewport;
	
	Vector2Set( mouse , (float)g_input->GetPosX() , (float)g_input->GetPosY() );
	Vector2Set( viewport , (float)g_cursor->m_viewport.Width , (float)g_cursor->m_viewport.Height );

	g_mouseSelectedCharacter = g_charManager->SearchCursorEntityNeo2 ( g_camera.m_position , g_camera.m_angles , g_camera.GetTransformMatrix() , 
		g_itemMng->GetMinSquareDist() , mouse , viewport );

	
	if(gGameConfigFlag.m_bCharacter_Storage)
	{
		
		if(!m_bLoadCharacterSlot) return;
		
		Storoge_Action();	
		
	}

	switch( m_cameraStep )
	{
		
	case -1 :
		if( m_makeFlag ) ChangeLoginStep( LOGIN_STATE_CREATE_CHARACTER );
		else 
		{
			SetCharacterInfo();
			ChangeLoginStep( LOGIN_STATE_SELECT_CHARACTER );
		}
		break;
		
	case 0 :
		{
			SetCharacterInfo();
			
			if( g_input->GetLBDown() && g_mouseSelectedCharacter != NULL )
			{
				
				
				if(gGameConfigFlag.m_bCharacter_Storage)
				{
					if(!Storoge_SelectCharacter(g_mouseSelectedCharacter)) break;
				}

				switch( g_mouseSelectedCharacter->ID)
				{
				case LOGIN_SELECT_CHAR_LEFT :
					{
						m_selectedCharacter = g_mouseSelectedCharacter;
						g_cgv.selectedGameDataIdx = 0;
						
						if ( g_cgv.characterSlotMode == globalVariable_t::CharacterSlotMode::BACK)
							g_cgv.selectedGameDataIdx +=3;
						

						
						g_entityMng->ActivateCameraPath( "cam_start_1" , "cam_select_1" , GTH_ENTITY_PLAY_FORWARD );
						m_cameraStep = -1;
						
						g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_GAMELOGIN_OK ), GTH_SOUND_PLAY_ONCE );
					}
					break;
				case LOGIN_SELECT_CHAR_CENTER :
					{
						m_selectedCharacter = g_mouseSelectedCharacter;
						g_cgv.selectedGameDataIdx = 1;

						
						if ( g_cgv.characterSlotMode == globalVariable_t::CharacterSlotMode::BACK)
							g_cgv.selectedGameDataIdx +=3;

						g_entityMng->ActivateCameraPath( "cam_start_2" , "cam_select_2" , GTH_ENTITY_PLAY_FORWARD );
						m_cameraStep = -1;
						
						g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_GAMELOGIN_OK ), GTH_SOUND_PLAY_ONCE );
					}
					break;
				case LOGIN_SELECT_CHAR_RIGHT :
					{
						m_selectedCharacter = g_mouseSelectedCharacter;
						g_cgv.selectedGameDataIdx = 2;

						
						if ( g_cgv.characterSlotMode == globalVariable_t::CharacterSlotMode::BACK)
							g_cgv.selectedGameDataIdx +=3;

											

						g_entityMng->ActivateCameraPath( "cam_start_3" , "cam_select_3" , GTH_ENTITY_PLAY_FORWARD );
						m_cameraStep = -1;
						
						g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_GAMELOGIN_OK ), GTH_SOUND_PLAY_ONCE );
					}
					break;
				} 
			}
		}
		break;
		
	case 1 :
		{
			
			
			if( g_cgv.selectedGameDataIdx == 0 || g_cgv.selectedGameDataIdx == 3) 
			{
				g_entityMng->ActivateCameraPath( "cam_start_1" , "cam_select_1" , GTH_ENTITY_PLAY_BACKSTEP );
				m_selectedCharacter = NULL;
			}
			

			
			else if( g_cgv.selectedGameDataIdx == 1 || g_cgv.selectedGameDataIdx == 4) 			
			{
				g_entityMng->ActivateCameraPath( "cam_start_2" , "cam_select_2" , GTH_ENTITY_PLAY_BACKSTEP );
				m_selectedCharacter = NULL;
			}
			
			else if( g_cgv.selectedGameDataIdx == 2 || g_cgv.selectedGameDataIdx == 5) 			
			{
				g_entityMng->ActivateCameraPath( "cam_start_3" , "cam_select_3" , GTH_ENTITY_PLAY_BACKSTEP );
				m_selectedCharacter = NULL;
			}

			if( !m_makeFlag ) m_cameraStep = 0;
			else m_cameraStep = 2;
		}
		break;
		
	case 2 :
		{
			g_entityMng->ActivateCameraPath( "cam_create_1" , "cam_create_1_path_2" , GTH_ENTITY_PLAY_FORWARD );
			m_cameraStep = -1;
		}
		break;
		
	case 3 :
		{
			
			if( m_cameraStepFlag )
			{
				g_entityMng->ActivateCameraPath( "cam_create_zero" , "cam_create_1_path_2" , GTH_ENTITY_PLAY_FORWARD );
			}
			
			else
			{
				g_entityMng->ActivateCameraPath( "cam_create_zero" , "cam_create_1_path_2" , GTH_ENTITY_PLAY_BACKSTEP );
			}
			
			if( m_makeFlag )
			{
				if( m_curSelectedMakeCharacter != m_nextSelectedMakeCharacter )
				{
					m_cameraStep = 3 + m_nextSelectedMakeCharacter;
					m_cameraStepFlag = true;
					m_curSelectedMakeCharacter = m_nextSelectedMakeCharacter;
					m_faceFlag = 0;
				}
				else
				{
					m_cameraStep = -1;
				}
			}
			
			else
			{
				m_cameraStep = 20;
			}
		}
		break;
	
	case 4 :
		{
			
			if( m_cameraStepFlag )
			{
				g_entityMng->ActivateCameraPath( "cam_create_zero_2" , "cam_create_2_path_2" , GTH_ENTITY_PLAY_FORWARD );
			}
			
			else
			{
				g_entityMng->ActivateCameraPath( "cam_create_zero_2" , "cam_create_2_path_2" , GTH_ENTITY_PLAY_BACKSTEP );
			}
			
			if( m_makeFlag )
			{
				if( m_curSelectedMakeCharacter != m_nextSelectedMakeCharacter )
				{
					m_cameraStep = 3 + m_nextSelectedMakeCharacter;
					m_cameraStepFlag = true;
					m_curSelectedMakeCharacter = m_nextSelectedMakeCharacter;
					m_faceFlag = 0;
				}
				else
				{
					m_cameraStep = -1;
				}
			}
			
			else
			{
				m_cameraStep = 20;
			}
		}
		break;
	
	case 5 :
		{
			
			if( m_cameraStepFlag )
			{
				g_entityMng->ActivateCameraPath( "cam_create_zero_3" , "cam_create_3_path_2" , GTH_ENTITY_PLAY_FORWARD );
			}
			
			else
			{
				g_entityMng->ActivateCameraPath( "cam_create_zero_3" , "cam_create_3_path_2" , GTH_ENTITY_PLAY_BACKSTEP );
			}
			
			if( m_makeFlag )
			{
				if( m_curSelectedMakeCharacter != m_nextSelectedMakeCharacter )
				{
					m_cameraStep = 3 + m_nextSelectedMakeCharacter;
					m_cameraStepFlag = true;
					m_curSelectedMakeCharacter = m_nextSelectedMakeCharacter;
					m_faceFlag = 0;
				}
				else
				{
					m_cameraStep = -1;
				}
			}
			
			else
			{
				m_cameraStep = 20;
			}
		}
		break;
	
	case 6 :
		{
			
			if( m_cameraStepFlag )
			{
				g_entityMng->ActivateCameraPath( "cam_create_zero_4" , "cam_create_4_path_2" , GTH_ENTITY_PLAY_FORWARD );
			}
			
			else
			{
				g_entityMng->ActivateCameraPath( "cam_create_zero_4" , "cam_create_4_path_2" , GTH_ENTITY_PLAY_BACKSTEP );
			}
			
			if( m_makeFlag )
			{
				if( m_curSelectedMakeCharacter != m_nextSelectedMakeCharacter )
				{
					m_cameraStep = 3 + m_nextSelectedMakeCharacter;
					m_cameraStepFlag = true;
					m_curSelectedMakeCharacter = m_nextSelectedMakeCharacter;
					m_faceFlag = 0;
				}
				else
				{
					m_cameraStep = -1;
				}
			}
			
			else
			{
				m_cameraStep = 20;
			}
		}
		break;
		
	case 20 :
		{
			g_entityMng->ActivateCameraPath( "cam_create_1" , "cam_create_zero" , GTH_ENTITY_PLAY_BACKSTEP );
			m_cameraStep = 0;
		}
		break;
	}
}





void CIFLoginWin::SetFaceCamera( int state )
{
	vec3_t pos;
	VectorCopy( pos, g_camera.m_position );
	
	if( state == 0 )
	{
		if( m_faceFlag ) return;
		if( m_curSelectedMakeCharacter % 2 == 0 ) pos[0] += 60.0f;
		if( m_curSelectedMakeCharacter % 2 == 1 ) pos[0] -= 60.0f;
		m_faceFlag = 1;
	} 
	else
	{
		if( !m_faceFlag ) return;
		if( m_curSelectedMakeCharacter % 2 == 0 ) pos[0] -= 60.0f;
		if( m_curSelectedMakeCharacter % 2 == 1 ) pos[0] += 60.0f;
		m_faceFlag = 0;
	}
	g_camera.SetPosition( pos );
	g_camera.SetupMatrices( true );
}


void CIFLoginWin::SetDeleteCharacter()
{
	char szTemp[256];

	
	if( g_cgv.savedData[g_cgv.selectedGameDataIdx].guildIdx != -1 )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,93), g_LPACK.GetMassage(0,943), MSG_BUTTONTYPE_OK, IF_MESSAGE_NONE );
		m_bDeleteCharFlag = false;
		return;
	}

	
	m_bDeleteCharFlag = true;
	m_deleteCharRemainTime = 10000.0f;
	sprintf( szTemp, g_LPACK.GetMassage(0,734), (int)( m_deleteCharRemainTime * 0.001f ) );
	g_ifMng->SetMessage( g_LPACK.GetMassage(0,93), szTemp, MSG_BUTTONTYPE_CANCEL, IF_MESSAGE_DELETE_CHARACTER_COMPLETE );
}




int CIFLoginWin::WindowFrame()
{
	SettingDrawText();

	
	if( m_bDeleteCharFlag )
	{	
		m_deleteCharRemainTime -= g_timer.GetElapsedMilliTime();
		if( m_deleteCharRemainTime < 0 )
		{
			m_deleteCharRemainTime = 0.0f;
			GTH_SendMessage_RequestDeleteCharacter();
			m_bDeleteCharFlag = false;
			g_ifMng->ExitMessage();
		}
		sprintf( g_ifMng->m_msgStr, g_LPACK.GetMassage(0,734), (int)( m_deleteCharRemainTime * 0.001f ) );
	}
	
	return IF_CTL_NONE;
}


int CIFLoginWin::WindowProcess()
{
	switch( m_loginStep )
	{	
	case LOGIN_STATE_NOTICE:				NoticeProcess();				break;		
	case LOGIN_STATE_IDPW :					IdPwProcess();					break;
	case LOGIN_STATE_SELECT_SERVER :		SelectServerProcess();			break;
	case LOGIN_STATE_SELECT_CHARACTER :		SelectCharacterProcess();		break;
	case LOGIN_STATE_CREATE_CHARACTER :		CreateCharacterProcess();		break;
	case LOGIN_STATE_SELECT_MASTER_SERVER : SelectMasterServer();			break;		
	}
	return IF_CTL_NONE;
}

int CIFLoginWin::WindowSelection()
{
	return IF_CTL_NONE;
}

void CIFLoginWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	
	if( m_login->State() )
	{
		IdPwRender();
	}

#ifdef _GTH_ENABLE_GOODNOTICE
	
	if(m_loginStep == LOGIN_STATE_NOTICE)
	{
		m_loginNotice->Render();
		m_cNoticeOkBtn->RenderTextBtn();
	}
#endif

	
	for( int i = 0; i < MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN; i ++ )
		m_serverBtn[i]->RenderTextBtn();

	m_serverOkBtn->Render();
	m_serverCancelBtn->Render();

	
	if( m_cselInfo->State() )
	{
		m_cselInfo->Render();
		m_cselInfoTitleBar->Render();
		m_cselInfoTitleBottom->Render();
		m_cselInfoBar->Render();
		m_cselInfoGenBar->Render();
		m_cselInfoGenIcon0->Render();
		m_cselInfoGenIcon1->Render();
		m_cselInfoGenIcon2->Render();
		m_cselInfoGenIcon3->Render();
	}
	m_cselExitBtn->RenderTextBtn();
	m_cselOkBtn->RenderTextBtn();
	m_cselBackBtn->RenderTextBtn();
	m_cselDeleteBtn->RenderTextBtn();
	m_cselMakeBtn->RenderTextBtn();
	
	m_DifferentCharBtn->RenderTextBtn();

#ifdef _GTH_FLAG_RELOGIN
	
	m_cReLoginBtn->RenderTextBtn();
#endif

	
	if( m_cmakeInfo->State() )
	{
		CreateCharacterRender();
	}

	TextRender();
}






int CIFLoginWin::CharacterSelectProcess()
{
	
	m_DifferentCharBtn->Enable(false);
	
	m_cselExitBtn->Enable( false );
	m_cselOkBtn->Enable( false );
	m_cselBackBtn->Enable( false );
	m_cselDeleteBtn->Enable( false );
	m_cselMakeBtn->Enable( false );
	m_cmakePrevBtn->Enable( false );
	m_cmakeNextBtn->Enable( false );
	m_cmakeInfo->Enable( false );

#ifdef _GTH_FLAG_RELOGIN
	
	m_cReLoginBtn->Enable(false);
#endif

	
	if( g_entityMng->m_currCameraPathEntity != NULL ) return( IF_CTL_NONE );
	
	CharacterSelect_CameraProcess();
	
	if( g_entityMng->m_currCameraPathEntity != NULL ) return( IF_CTL_NONE );
	
	if( m_loginStep == LOGIN_STATE_SELECT_CHARACTER )
	{
		CharacterSelect_SelectScreenProcess();
	}
	else if( m_loginStep == LOGIN_STATE_CREATE_CHARACTER )
	{
		CharacterSelect_CreateScreenProcess();
	}

	return( IF_CTL_NONE );
}

void CIFLoginWin::SelectSlot(bool flag)
{
	
	if(flag)
	{
		m_nMembershipType = m_btTempMembershipType;

		
		Fx_CHARACTER_t* character = g_charManager->GetCharacter(m_nMembershipType-LOGIN_CHAR_FREE_LEFT);
		if(character == NULL) return;

		
		if( character->entityType == ENTITY_PC && character->ani_curSetID == ANIM_ITEM_IDLE )
		{
			GTH_Character_Shout(character);			
		}
		else
		{
			m_bCharacterShout =true;
		}
		GTH_SendMessage_Request_SaveCharacterSlot(m_nMembershipType);
	}
	
	else
	{
		m_cameraStep=1;
	}

	m_btTempMembershipType=-1;
}


void CIFLoginWin::InitCharacterSlot()
{
	m_nMembershipType=LOGIN_CHAR_FREE_NEW;
	m_nDate=0;
	m_btSelectCharStage=0;
	m_btTempMembershipType=-1;
	m_bCharacterShout=false;
	
	m_bLoadCharacterSlot=true;
	m_bSendLoadCharacterSlot=false;
}


bool CIFLoginWin::IsSafeMembershipType(int nType)
{
	
	if(nType >= LOGIN_CHAR_FREE_NEW && nType <= LOGIN_CHAR_FREE_RIGHT)  return true;

	
	if(nType >= LOGIN_CHAR_PU_NEW && nType <= LOGIN_CHAR_PU)  return true;

	return false;
}


void CIFLoginWin::SettingCharacterSlot()
{
	
	if(!m_bLoadCharacterSlot) return;

	if(!IsSafeMembershipType(m_nMembershipType)) 
	{
		assert(0);
		return;
	}           

	m_btSelectCharStage=CREATE_NONE_CHAR;
	if(g_charManager->m_Characters[0].entityType == ENTITY_PC) m_btSelectCharStage+=CREATE_ONLY_LEFT_CHAR;
	if(g_charManager->m_Characters[1].entityType == ENTITY_PC) m_btSelectCharStage+=CREATE_ONLY_CENTER_CHAR;
	if(g_charManager->m_Characters[2].entityType == ENTITY_PC) m_btSelectCharStage+=CREATE_ONLY_RIGHT_CHAR;
	

	
	if(m_nMembershipType == LOGIN_CHAR_FREE_NEW)
	{
		
		if(m_btSelectCharStage != CREATE_NONE_CHAR)
		{
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322),g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,213), MSG_BUTTONTYPE_OK );
		}
	}
	
	else if(m_nMembershipType == LOGIN_CHAR_PU_NEW)
	{
		g_ifMng->SetMessage(g_LPACK.GetMassage(0,322),g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,211), MSG_BUTTONTYPE_OK );
	
	}
	
	else if(m_nMembershipType == LOGIN_CHAR_PU)
	{
		
		if(m_nDate <= LOGIN_PU_REMAIN_DATE_NOTICE && m_nDate > 0)
		{
			char temp[256];
			wsprintf(temp,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,212),m_nDate,m_nDate);

			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), temp, MSG_BUTTONTYPE_OK);
		}
	
	}
	
	else
	{
	
	}

	
}
