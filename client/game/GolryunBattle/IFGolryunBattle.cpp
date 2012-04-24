



#include "../global_def.h"
#include "IFGolryunBattle.h"
#include "../game/GolryunBattle/GolryunBattle.h"
#include "../game/GolryunBattle/GolryunBattle_Encoder.h"











CIFBattleTicketWin::CIFBattleTicketWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	int i;

	for( i=0; i<NUM_IT_TICKET; i++ )
		m_textCtl[i]	= new CIFText( this );

	for( i=0; i<TICKET_LIST_MAX_NUM; i++ )
		m_checkBtn[i]	= new CIFCheckBox( this );

	
	m_titleBar			= new CIFControl( this );
	m_titleBarBottom	= new CIFControl( this );

	m_selectBar			= new CIFControl( this );

	
	m_oneWinBtn			= new CIFButtonCtl( this );
	m_twoWinBtn			= new CIFButtonCtl( this );
	m_oneTwoWinBtn		= new CIFButtonCtl( this );

	m_renewBtn			= new CIFButtonCtl( this );
	m_buy100Btn			= new CIFButtonCtl( this );
	m_buy10Btn			= new CIFButtonCtl( this );
	m_cancelBtn			= new CIFButtonCtl( this );
	m_exitBtn			= new CIFButtonCtl( this );

	
	m_scrollBar			= new CIFScrollBar( this );

	m_interfaceType		= MAIL_INTERFACE_TYPE_RECVMAIL;

	m_bDisplay			= false;

	m_winNumber			= 0;		

	m_JoinerCnt			= 0;

	for( i=0; i<2; i++)
	{
		m_selectedIdx[i] = -1;
		m_selectBarSavePos[i] = 0;
	}

	for( i=0; i<TICKET_LIST_MAX_NUM; i++)
	{
		m_ticketList[i].num = i+1;
		m_ticketList[i].job = 0;
		m_ticketList[i].win = 0;
		m_ticketList[i].lose = 0;
		m_ticketList[i].shareRate = 1.1f;
		m_ticketList[i].checkFlag = false;
	}

	m_renewBtn->Enable(false);
	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_BATTLE_TICKET_WINDOW_TYPE );

}

CIFBattleTicketWin::~CIFBattleTicketWin()
{
}

void CIFBattleTicketWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_TICKET_WIN );

	
	m_textCtl[IT_TICKET_TITLE]->PrepareControl( IF_TICKET_TITLE_TEXT );

	m_textCtl[IT_TICKET_LIST_NUMBER]->PrepareControl( IF_TICKET_LIST_NUMBER_TEXT );
	m_textCtl[IT_TICKET_LIST_CLASS]->PrepareControl( IF_TICKET_LIST_CLASS_TEXT );
	m_textCtl[IT_TICKET_LIST_WAR_RECORD]->PrepareControl( IF_TICKET_LIST_WAR_RECORD_TEXT );
	m_textCtl[IT_TICKET_LIST_SHARE_RATE]->PrepareControl( IF_TICKET_LIST_SHARE_RATE_TEXT );

	
	m_titleBar->PrepareControl( IF_TICKET_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_TICKET_TITLE_BAR_BOTTOM );
	m_selectBar->PrepareControl( IF_TICKET_SELECT_BAR );

	
	m_oneWinBtn->PrepareControl( IF_TICKET_ONEWIN_BTN_0 );
	m_oneTwoWinBtn->PrepareControl( IF_TICKET_ONETWOWIN_BTN_0 );
	m_twoWinBtn->PrepareControl( IF_TICKET_TWOWIN_BTN_0 );

	m_renewBtn->PrepareControl( IF_TICKET_RENEW_BTN_0 );
	m_buy100Btn->PrepareControl( IF_TICKET_BUY100_BTN_0 );
	m_buy10Btn->PrepareControl( IF_TICKET_BUY10_BTN_0 );
	m_cancelBtn->PrepareControl( IF_TICKET_CANCEL_BTN_0 );
	m_exitBtn->PrepareControl( IF_TICKET_EXIT_BTN_0 );

	
	m_scrollBar->PrepareControl( IF_TICKET_SCROLL );
	m_scrollBar->SetBarPosition( 0, 0 );

	for( int i=0; i<TICKET_LIST_MAX_NUM; i++ )
	{
		m_checkBtn[i]->PrepareControl( IF_TICKET_CHECKBOX_BTN_0 + i*2, 0.0f, (float)( TICKET_LIST_LINE_HEIGHT * i ) );
		m_checkBtn[i]->SetCheck( false );
	}

	m_selectBarSavePos[0] = g_ifMng->m_iPos[IF_TICKET_SELECT_BAR].clientY;
	m_selectBarSavePos[1] = g_ifMng->m_iPos[IF_TICKET_SELECT_BAR].clientY;
}

int CIFBattleTicketWin::WindowFrame()
{

	int i;
	for( i=0; i<m_JoinerCnt; i++ )
	{
		if( m_checkBtn[i]->GetCheck() ) break;
	}

	m_textCtl[IT_TICKET_TITLE]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,338) );

	m_textCtl[IT_TICKET_LIST_NUMBER]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,339) );
	m_textCtl[IT_TICKET_LIST_CLASS]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,340) );
	m_textCtl[IT_TICKET_LIST_WAR_RECORD]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,341) );
	m_textCtl[IT_TICKET_LIST_SHARE_RATE]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,188) );

	m_oneWinBtn->SetButtonText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,333) );
	m_oneTwoWinBtn->SetButtonText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,334) );
	m_oneTwoWinBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	m_twoWinBtn->SetButtonText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,335) );
	m_twoWinBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	m_renewBtn->SetButtonText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,342) );
	m_buy100Btn->SetButtonText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,343) );
	m_buy10Btn->SetButtonText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,344) );
	m_cancelBtn->SetButtonText( g_LPACK.GetMessage(0,521) );

	return( IF_CTL_NONE );
}

void CIFBattleTicketWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();


	for( int i=0; i<m_JoinerCnt; i++ )
	{

		m_checkBtn[i]->Render();
	}

	
	m_oneWinBtn->RenderTextBtn();
	m_twoWinBtn->RenderTextBtn();
	m_oneTwoWinBtn->RenderTextBtn();

	m_renewBtn->RenderTextBtn();
	m_buy100Btn->RenderTextBtn();
	m_buy10Btn->RenderTextBtn();
	m_cancelBtn->RenderTextBtn();
	m_exitBtn->Render();
	
	
	m_scrollBar->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_TICKET_TITLE]->Render();

		m_textCtl[IT_TICKET_LIST_NUMBER]->Render();
		m_textCtl[IT_TICKET_LIST_CLASS]->Render();
		m_textCtl[IT_TICKET_LIST_WAR_RECORD]->Render();
		m_textCtl[IT_TICKET_LIST_SHARE_RATE]->Render();

		UpdateTicketList();
	}
	g_chatManager->EndFrame();
}

int CIFBattleTicketWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFBattleTicketWin::WindowProcess()
{
	if( m_exitBtn->ProcessButton() == IF_CTL_SELECT )
	{
		for( int i=0; i<m_JoinerCnt; i++ )
			m_checkBtn[i]->SetCheck( false );

		Enable( false );
		return IF_CTL_SELECT;
	}

	if( m_oneWinBtn->ProcessButton() == IF_CTL_SELECT )
	{
	
		return IF_CTL_SELECT;
	}

	if( m_oneTwoWinBtn->ProcessButton() == IF_CTL_SELECT )
	{
	
		return IF_CTL_SELECT;
	}

	if( m_twoWinBtn->ProcessButton() == IF_CTL_SELECT )
	{

		return IF_CTL_SELECT;
	}

	if( m_renewBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_GolryunBattle_Encoder.SendReNewBattleTicket();
		return IF_CTL_SELECT;
	}

	if( m_buy100Btn->ProcessButton() == IF_CTL_SELECT )
	{
		int ticketCnt = GetTicketCount();
		if(ticketCnt>(g_cgv.myCharacterInfo->level/2))
		{
			
			g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,346) );
			return IF_CTL_SELECT;
		}


		g_GolryunBattle_Encoder.SendBuyBattleTicket(CGolryunBattle::TWOGYUN_1000000_ITEM_INDEX, m_winNumber);
		return IF_CTL_SELECT;
	}

	if( m_buy10Btn->ProcessButton() == IF_CTL_SELECT )
	{	

		int ticketCnt = GetTicketCount();
		if(ticketCnt>(g_cgv.myCharacterInfo->level/2))
		{
			
			g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,346) );
			return IF_CTL_SELECT;
		}


		g_GolryunBattle_Encoder.SendBuyBattleTicket(CGolryunBattle::TWOGYUN_100000_ITEM_INDEX, m_winNumber);
		return IF_CTL_SELECT;
	}

	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		for( int i=0; i<m_JoinerCnt; i++ )
			m_checkBtn[i]->SetCheck( false );
		
		Enable( false );

		return IF_CTL_SELECT;
	}

	if( m_scrollBar->ProcessScrollBar() == IF_CTL_SELECT )
	{
		return IF_CTL_SELECT;
	}

	for( int i=0; i<m_JoinerCnt ; i++ )
	{ 
		if( m_checkBtn[i]->ProcessCheckBox() == IF_CTL_SELECT )
		{
			for( int j=0; j<m_JoinerCnt; j++ )
			{
				m_ticketList[j].checkFlag = false;
				m_checkBtn[j]->SetCheck( false );
			}

			m_winNumber = i+1;
			m_checkBtn[i]->SetCheck( true );
			m_ticketList[i].checkFlag = m_checkBtn[i]->GetCheck();
			return IF_CTL_SELECT;
		}
	}

	return IF_CTL_NONE;
}

int CIFBattleTicketWin::GetTicketCount()
{
	CGolryunBattle::tagTournament* pTournament = g_GolryunBattle.GetTournamentPointer();
	int invCnt=0;
	for( int i=0; i<MAX_INVENTORY_SIZE; i++ )
	{
		if( g_cgv.myCharacterInfo->inventory[i] < 0 ) continue;
		int itemIdx = g_cgv.myCharacterInfo->inventory[i];
		int tableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
		int battlIdx = g_cgv.myCharacterInfo->item[itemIdx].optionIdx[0];
		int level = g_cgv.myCharacterInfo->item[itemIdx].optionIdx[1];
		int serial = g_cgv.myCharacterInfo->item[itemIdx].experience;
		int curBattleIdx = g_GolryunBattle.GetBattleIdx();
		int cnt = g_GolryunBattle.GetTournamentIdx();

		if(tableIdx == CGolryunBattle::TWOGYUN_100000_ITEM_INDEX || 
		   tableIdx == CGolryunBattle::TWOGYUN_1000000_ITEM_INDEX)
		{
			if( battlIdx == curBattleIdx && 
				level    == g_golryunManagerTable[cnt].levelMin &&
				serial   == pTournament->m_iTournamentGUID)
			{
				invCnt++;
			}
		}
	}

	int depotCnt=0;
	for( i = 0; i < MAX_DEPOT_SIZE; i ++ )
	{
		if( g_cgv.myCharacterInfo->depot[i] < 0 ) continue;
		int itemIdx = g_cgv.myCharacterInfo->depot[i];
		int tableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
		int battlIdx = g_cgv.myCharacterInfo->item[itemIdx].optionIdx[0];
		int level = g_cgv.myCharacterInfo->item[itemIdx].optionIdx[1];
		int serial = g_cgv.myCharacterInfo->item[itemIdx].experience;
		int curBattleIdx = g_GolryunBattle.GetBattleIdx();
		int cnt = g_GolryunBattle.GetTournamentIdx();

		if(tableIdx == CGolryunBattle::TWOGYUN_100000_ITEM_INDEX || 
		   tableIdx == CGolryunBattle::TWOGYUN_1000000_ITEM_INDEX)
		{
			if( battlIdx == curBattleIdx && 
				level    == g_golryunManagerTable[cnt].levelMin &&
				serial   == pTournament->m_iTournamentGUID)
			{
				depotCnt++;
			}
		}
	}

	return invCnt + depotCnt;
}

void CIFBattleTicketWin::UpdateTicketList()
{




	char buffer[256];

	for( int i=0; i<m_JoinerCnt; i++ )
	{
		sprintf( buffer, "%d", i+1 );

		g_chatManager->DrawText( m_textCtl[IT_TICKET_LIST_NUMBER]->GetXPos(), m_textCtl[IT_TICKET_LIST_NUMBER]->GetYPos()+(i+1)*TICKET_LIST_LINE_HEIGHT+2, buffer, 109, 13, CHAT_ARRANGE_OVERSIZECUT );

		int jobIdx = m_ticketList[i].job;

		strcpy( buffer, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, jobIdx ) );
		g_chatManager->DrawText( m_textCtl[IT_TICKET_LIST_CLASS]->GetXPos(), m_textCtl[IT_TICKET_LIST_CLASS]->GetYPos()+(i+1)*TICKET_LIST_LINE_HEIGHT+2, buffer, 253, 13, CHAT_ARRANGE_OVERSIZECUT );
	
		
		sprintf( buffer, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,347), m_ticketList[i].win, m_ticketList[i].lose );
		g_chatManager->DrawText( m_textCtl[IT_TICKET_LIST_WAR_RECORD]->GetXPos(), m_textCtl[IT_TICKET_LIST_WAR_RECORD]->GetYPos()+(i+1)*TICKET_LIST_LINE_HEIGHT+2, buffer, 121, 13, CHAT_ARRANGE_OVERSIZECUT );


		sprintf( buffer, "%.2f", m_ticketList[i].shareRate );	
		g_chatManager->DrawText( m_textCtl[IT_TICKET_LIST_SHARE_RATE]->GetXPos(), m_textCtl[IT_TICKET_LIST_SHARE_RATE]->GetYPos()+(i+1)*TICKET_LIST_LINE_HEIGHT+2, buffer, 49, 13, CHAT_ARRANGE_OVERSIZECUT );

	}
}

void CIFBattleTicketWin::SetDisplayMode( bool mode )
{
	m_bDisplay = mode;
}

void CIFBattleTicketWin::SetSelectBar( int idx )
{
	m_selectBar->Enable();
	g_ifMng->m_iPos[IF_TICKET_SELECT_BAR].clientY = m_selectBarSavePos[m_interfaceType] + idx * MAIL_WIN_LIST_LINE_HEIGHT;
	m_selectBar->UpdateControl();
	m_selectBar->UpdateBuffer();
	m_selectBar->Render();
}








CIFShareWin::CIFShareWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	int i;

	for( i=0; i<NUM_IT_SHARE; i++ )
		m_textCtl[i]	= new CIFText( this );


	
	m_titleBar			= new CIFControl( this );
	m_titleBarBottom	= new CIFControl( this );

	m_battlePrevBtn      = new CIFButtonCtl( this );
	m_battleNextBtn      = new CIFButtonCtl( this );

	m_levelPrevBtn      = new CIFButtonCtl( this );
	m_levelNextBtn      = new CIFButtonCtl( this );

	
	m_changeBtn			= new CIFButtonCtl( this );
	m_cancelBtn			= new CIFButtonCtl( this );

	m_bDisplay			= false;
	m_iShareNak			= 0;
	m_iBattleCnt		= 1;
	m_iLevelCnt			= 0;
	
	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_SHARE_WINDOW_TYPE );

}

CIFShareWin::~CIFShareWin()
{
}

void CIFShareWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_SHARE_WIN );

	
	m_textCtl[IT_SHARE_TITLE]->PrepareControl( IF_SHARE_WIN_TITLE_TEXT );
	m_textCtl[IT_SHARE_WIN_SERIAL_BAR]->PrepareControl( IF_SHARE_WIN_SERIAL_BAR );
	m_textCtl[IT_SHARE_WIN_GUID_BAR]->PrepareControl( IF_SHARE_WIN_GUID_BAR );
	m_textCtl[IT_SHARE_WIN_RATE_BAR]->PrepareControl( IF_SHARE_WIN_RATE_BAR );
	m_textCtl[IT_SHARE_WIN_LEVEL]->PrepareControl( IF_SHARE_WIN_LEVEL_TEXT );
	m_textCtl[IT_SHARE_WIN_INVEN_POS]->PrepareControl( IF_SHARE_WIN_INVEN_POS );
	m_textCtl[IT_SHARE_WIN_NAK]->PrepareControl( IF_SHARE_WIN_NAK_TEXT );
	m_textCtl[IT_SHARE_WIN_NAK_BAR]->PrepareControl( IF_SHARE_WIN_NAK_BAR );

	
	m_titleBar->PrepareControl( IF_SHARE_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_SHARE_WIN_TITLE_BOTTOM );

	m_battlePrevBtn->PrepareControl( IF_SHARE_WIN_BATTLE_PREV_BTN_0 );
	m_battleNextBtn->PrepareControl( IF_SHARE_WIN_BATTLE_NEXT_BTN_0 );

	m_levelPrevBtn->PrepareControl( IF_SHARE_WIN_LEVEL_PREV_BTN_0 );
	m_levelNextBtn->PrepareControl( IF_SHARE_WIN_LEVEL_NEXT_BTN_0 );

	
	m_changeBtn->PrepareControl( IF_SHARE_WIN_EXCHANGE_BTN_0 );
	m_cancelBtn->PrepareControl( IF_SHARE_WIN_CANCEL_BTN_0 );
}

int CIFShareWin::WindowFrame()
{
	if( !State() ) return IF_CTL_NONE;
	char szTemp[256];
	
	
	m_textCtl[IT_SHARE_TITLE]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,181) );







	
	sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,348), m_iBattleCnt);
	m_textCtl[IT_SHARE_WIN_SERIAL_BAR]->SetText(szTemp);

	int iBattleIdx = g_GolryunBattle.GetBattleIdx();
	if(iBattleIdx == m_iBattleCnt)
	{
		
		CurrentBattleInformation();
	}
	else
	{
		
		RecordBattleInformation(m_iBattleCnt);
	}

	
	m_textCtl[IT_SHARE_WIN_NAK]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,349) );

	strcpy( szTemp, g_ifMng->GetNumberString( m_iShareNak ) );
	m_textCtl[IT_SHARE_WIN_NAK_BAR]->SetText( szTemp );

	
	m_changeBtn->SetButtonText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,191) );
	m_cancelBtn->SetButtonText( g_LPACK.GetMessage(0,521) );

	return( IF_CTL_NONE );
}

void CIFShareWin::CurrentBattleInformation()
{
	char szTemp[256];
	
	CGolryunBattle::tagBattle* pBattle = g_GolryunBattle.GetBattleCurrentPointer();
	if(pBattle == NULL)	return;
	if(pBattle->Tournament[m_iLevelCnt].bComplete)
	{
		if(pBattle->Tournament[m_iLevelCnt].FirstWinner.iFightNum<=0)
		{
			
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,350), g_golryunManagerTable[m_iLevelCnt].levelMin);
			m_textCtl[IT_SHARE_WIN_LEVEL]->SetText( szTemp );

			
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,351), pBattle->Tournament[m_iLevelCnt].m_iTournamentGUID);
			m_textCtl[IT_SHARE_WIN_GUID_BAR]->SetText(szTemp);

			m_textCtl[IT_SHARE_WIN_RATE_BAR]->SetText("");
			return;
		}

		
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,352), g_golryunManagerTable[m_iLevelCnt].levelMin, 
			pBattle->Tournament[m_iLevelCnt].FirstWinner.iFightNum );
		m_textCtl[IT_SHARE_WIN_LEVEL]->SetText( szTemp );

		
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,351), pBattle->Tournament[m_iLevelCnt].m_iTournamentGUID);
		m_textCtl[IT_SHARE_WIN_GUID_BAR]->SetText(szTemp);

		
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,353), pBattle->Tournament[m_iLevelCnt].m_ShareRate );
		m_textCtl[IT_SHARE_WIN_RATE_BAR]->SetText(szTemp);
	}else
	{
		
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,354), g_golryunManagerTable[m_iLevelCnt].levelMin);
		m_textCtl[IT_SHARE_WIN_LEVEL]->SetText( szTemp );
		sprintf( szTemp, "");
		m_textCtl[IT_SHARE_WIN_GUID_BAR]->SetText(szTemp);
		m_textCtl[IT_SHARE_WIN_RATE_BAR]->SetText(szTemp);
	}
}

void CIFShareWin::RecordBattleInformation(int battleCnt)
{
	char szTemp[256];
	
	CGolryunBattle::tagBattle* pRecord = g_GolryunBattle.FindBattleRecordPointer(battleCnt);
	if(pRecord == NULL) return;
	if(pRecord->Tournament[m_iLevelCnt].bComplete)
	{
		if(pRecord->Tournament[m_iLevelCnt].FirstWinner.iFightNum<=0)
		{
			
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,350), g_golryunManagerTable[m_iLevelCnt].levelMin);
			m_textCtl[IT_SHARE_WIN_LEVEL]->SetText( szTemp );

			
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,351), pRecord->Tournament[m_iLevelCnt].m_iTournamentGUID);
			m_textCtl[IT_SHARE_WIN_GUID_BAR]->SetText(szTemp);

			m_textCtl[IT_SHARE_WIN_RATE_BAR]->SetText("");
			return;
		}

		
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,352), g_golryunManagerTable[m_iLevelCnt].levelMin, 
			pRecord->Tournament[m_iLevelCnt].FirstWinner.iFightNum );
		m_textCtl[IT_SHARE_WIN_LEVEL]->SetText( szTemp );

		
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,351), pRecord->Tournament[m_iLevelCnt].m_iTournamentGUID);
		m_textCtl[IT_SHARE_WIN_GUID_BAR]->SetText(szTemp);

		
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,353), pRecord->Tournament[m_iLevelCnt].m_ShareRate );
		m_textCtl[IT_SHARE_WIN_RATE_BAR]->SetText(szTemp);
	}else
	{
		
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,354), g_golryunManagerTable[m_iLevelCnt].levelMin);
		m_textCtl[IT_SHARE_WIN_LEVEL]->SetText( szTemp );
		sprintf( szTemp, "");
		m_textCtl[IT_SHARE_WIN_GUID_BAR]->SetText(szTemp);
		m_textCtl[IT_SHARE_WIN_RATE_BAR]->SetText(szTemp);
	}
}


void CIFShareWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();

	m_battlePrevBtn->Render();
	m_battleNextBtn->Render();

	m_levelPrevBtn->Render();
	m_levelNextBtn->Render();
	
	m_changeBtn->RenderTextBtn();
	m_cancelBtn->RenderTextBtn();

	g_ifMng->m_iconCtl->BeginFrame();
	{
		
		for( int i = 0; i < MAX_SHARE_INVENTORY_SIZE; i ++ )
		{
			int itemIdx, itemTableIdx, cx, cy;
			itemIdx = g_cgv.myCharacterInfo->shareInventory[i];
			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_SHARE_WIN_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
				cy = m_textCtl[IT_SHARE_WIN_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}

	}
	g_ifMng->m_iconCtl->EndFrame();
	
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_SHARE_TITLE]->Render();
		m_textCtl[IT_SHARE_WIN_SERIAL_BAR]->Render();
		m_textCtl[IT_SHARE_WIN_GUID_BAR]->Render();
		m_textCtl[IT_SHARE_WIN_RATE_BAR]->Render();
		m_textCtl[IT_SHARE_WIN_LEVEL]->Render();
		m_textCtl[IT_SHARE_WIN_NAK]->Render();

		m_textCtl[IT_SHARE_WIN_NAK_BAR]->Render();

	}
	g_chatManager->EndFrame();
}

int CIFShareWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFShareWin::WindowProcess()
{
	int itemIdx, mouseItemIdx;
	int x, y, pos;
	int mx, my, temp;

	if( m_battlePrevBtn->ProcessButton() == IF_CTL_SELECT )
	{
		int iBattleIdx = g_GolryunBattle.GetBattleIdx();
		int record_start_idx = iBattleIdx - CGolryunBattle::MAX_NUM_BATTLE_RECORD;
		if(record_start_idx <= 1) record_start_idx = 1;
	
		if( m_iBattleCnt <= record_start_idx )
			m_iBattleCnt = record_start_idx;
		else
			m_iBattleCnt--;
		
	}
	else if( m_battleNextBtn->ProcessButton() == IF_CTL_SELECT )
	{
		int iBattleIdx = g_GolryunBattle.GetBattleIdx();
		if( m_iBattleCnt >= iBattleIdx ) 
			m_iBattleCnt = iBattleIdx;
		else
			m_iBattleCnt++;
	}

	if( m_levelPrevBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_iLevelCnt <= 0 )
			m_iLevelCnt = 0;
		else
			m_iLevelCnt--;
		
	}
	else if( m_levelNextBtn->ProcessButton() == IF_CTL_SELECT )
	{
		int turnCnt = g_GolryunBattle.GetTournamentIdx();
		if( m_iLevelCnt >= CGolryunBattle::MAX_NUM_TOURNAMENT_PER_BATTLE-1 ) 
			m_iLevelCnt = CGolryunBattle::MAX_NUM_TOURNAMENT_PER_BATTLE-1;
		else
			m_iLevelCnt++;
	}

	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
	
		GTH_SendMessage_ItemSystem_ExchangeClose();
		Enable( false );

		return IF_CTL_SELECT;
	}

	if( m_changeBtn->ProcessButton() == IF_CTL_SELECT )
	{		
		GTH_SendMessage_ItemSystem_ExchangePrepare();		
		return IF_CTL_SELECT;
	}

	pos = -1;
	
	mx = g_input->GetPosX() - m_textCtl[IT_SHARE_WIN_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_SHARE_WIN_INVEN_POS]->GetYPos();

	if( mx <= 0 || my <= 0 ) return( IF_CTL_NONE );
	if( mx >= 34 * 6 ||  my >= 33 * 3 ) return( IF_CTL_NONE );

	temp = mx / 34;
	x = temp % 6;
	temp = my / 33;
	y = temp % 3;

	pos = y * 6 + x;

	
	if( pos < 0 || pos >= MAX_SHARE_INVENTORY_SIZE )
	{
		return( IF_CTL_NONE );
	}
		
	itemIdx = g_cgv.myCharacterInfo->shareInventory[pos];
	mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;

	
	if( g_input->GetLBNone() ) 
	{
		
		if( itemIdx < 0 ) return( IF_CTL_NONE );

		g_ifMng->InitRolloverText();
		g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[itemIdx] );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
	}

	
	if( g_input->GetLBUp() ) 
	{
		
		if( g_cgv.myCharacterInfo->curItemWeight > g_cgv.myCharacterInfo->calItemWeight )
		{
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( 0, 589 ) );
			return( IF_CTL_NONE );
		}
		if( mouseItemIdx < 0 && itemIdx < 0 ) return IF_CTL_NONE;

		GTH_SendMessage_ItemSystem_ExchangeItem( m_iBattleCnt, m_iLevelCnt, pos ); 
	}

	return IF_CTL_NONE;
}

void CIFShareWin::InitShareInventory()
{
	for( int i = 0; i < MAX_SHARE_INVENTORY_SIZE; i ++ )
	{
		g_cgv.myCharacterInfo->shareInventory[i] = -1;
	}

	m_iShareNak = 0;

}

void CIFShareWin::UndoShareInventory()
{
	item_t *item;
	int itemIdx, inventoryPos;

	for( int i = 0; i < MAX_SHARE_INVENTORY_SIZE; i ++ )
	{
		itemIdx = g_cgv.myCharacterInfo->shareInventory[i];
		if( itemIdx < 0 ) continue;
		
		item = &g_cgv.myCharacterInfo->item[itemIdx];

		int stackItemIdx = GTH_StackMyItem( item );

		if( stackItemIdx >= 0 )
		{
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += item->durability + 1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
			GTH_DeleteMyItem( item );
		}
		else
		{
			inventoryPos = GTH_FindEmptyInventoryPos();
			if( inventoryPos >= 0 ) 
			{
				g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;
			}
		}
	}
	
	InitShareInventory();
	Enable( false );
}








CIFTournamentWin::CIFTournamentWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	int i;

	for( i=0; i<NUM_IT_TOURNAMENT; i++ )
		m_textCtl[i]	= new CIFText( this );

	for( i=0; i<30; i++ )
		m_loseX[i]		= new CIFControl( this );

	

	m_closeBtn			= new CIFButtonCtl( this );
	
	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_TOURNAMENT_WINDOW_TYPE );

}

CIFTournamentWin::~CIFTournamentWin()
{
}

void CIFTournamentWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_TOURNAMENT_WIN );

	
	m_textCtl[IT_TOURNAMENT_TITLE]->PrepareControl( IF_TOURNAMENT_TITLE_TEXT );
	m_textCtl[IT_TOURNAMENT_STEP0_NUMBER]->PrepareControl( IF_TOURNAMENT_STEP0_NUMBER );
	m_textCtl[IT_TOURNAMENT_STEP1_NUMBER]->PrepareControl( IF_TOURNAMENT_STEP1_NUMBER );
	m_textCtl[IT_TOURNAMENT_STEP2_NUMBER]->PrepareControl( IF_TOURNAMENT_STEP2_NUMBER );
	m_textCtl[IT_TOURNAMENT_STEP3_NUMBER]->PrepareControl( IF_TOURNAMENT_STEP3_NUMBER );
	m_textCtl[IT_TOURNAMENT_STEP4_NUMBER]->PrepareControl( IF_TOURNAMENT_STEP4_NUMBER );
	m_textCtl[IT_TOURNAMENT_WIN_NUMBER]->PrepareControl( IF_TOURNAMENT_WIN_NUMBER );

	for( int i=0; i<30; i++ )
	{
		m_loseX[i]->PrepareControl( IF_TOURNAMENT_LOSE_DISPLAY_0 + i);
	}

	

	m_closeBtn->PrepareControl( IF_TOURNAMENT_CLOSE_BTN_0 );

	
	m_textCtl[IT_TOURNAMENT_TITLE]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,355) );


	m_closeBtn->SetButtonText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,936) );

}

int CIFTournamentWin::WindowFrame()
{
	
	return( IF_CTL_NONE );
}

void CIFTournamentWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();

	int i,j,k=0;
	for(i=0; i<CGolryunBattle::MAX_NUM_MATCH_PER_TOURNAMENT; i++)
	{

		for(j=0; j<2; j++)
		{
			CGolryunBattle::tagMatch* pMatch = g_GolryunBattle.GetMatchPointer(i);
			if((TRUE == pMatch->bComplete) && (FALSE == pMatch->Fighter[j].bWin)){

				m_loseX[k]->Render();
			}			
			k++;
		}
	}

	

	m_closeBtn->RenderTextBtn();
	
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_TOURNAMENT_TITLE]->Render();






		UpdateTournamentList();
	}
	g_chatManager->EndFrame();
}

int CIFTournamentWin::WindowSelection()
{
	int ret;

	
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFTournamentWin::WindowProcess()
{
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_SELECT;
	}

	return IF_CTL_NONE;
}

void CIFTournamentWin::UpdateTournamentList()
{




	int i,j,k;
	char buffer[32];

	CGolryunBattle::tagBattle* pBattle = g_GolryunBattle.GetBattleCurrentPointer();
	
	for(i=0; i<CGolryunBattle::MAX_NUM_TOURNAMENT_PER_BATTLE; i++)
	{
		if(!pBattle->Tournament[i].bComplete) continue;
		if(pBattle->Tournament[i].FirstWinner.iFightNum <= 0) continue;
		
		sprintf( buffer, "%d", pBattle->Tournament[i].FirstWinner.iFightNum );
		g_chatManager->DrawText(m_textCtl[IT_TOURNAMENT_WIN_NUMBER]->GetXPos()+i*48,
			m_textCtl[IT_TOURNAMENT_WIN_NUMBER]->GetYPos(), buffer, 16, 13, CHAT_ARRANGE_CENTER );
	}

	k=0;
	for(i=0; i<8; i++)
	{	
		for(j=0; j<2; j++)
		{
			CGolryunBattle::tagMatch* pMatch = g_GolryunBattle.GetMatchPointer(i);
			if(pMatch->Fighter[j].iFightNum <= 0) continue;

			sprintf( buffer, "%d", pMatch->Fighter[j].iFightNum );
			g_chatManager->DrawText(m_textCtl[IT_TOURNAMENT_STEP0_NUMBER]->GetXPos()+35*j+87*k,
				m_textCtl[IT_TOURNAMENT_STEP0_NUMBER]->GetYPos(), buffer, 16, 13, CHAT_ARRANGE_CENTER );
		}
		k++;
	}

	
	k=0;
	for(i=8; i<12; i++)
	{	
		for(j=0; j<2; j++)
		{
			CGolryunBattle::tagMatch* pMatch = g_GolryunBattle.GetMatchPointer(i);
			if(pMatch->Fighter[j].iFightNum <= 0) continue;

			sprintf( buffer, "%d", pMatch->Fighter[j].iFightNum );
			g_chatManager->DrawText(m_textCtl[IT_TOURNAMENT_STEP1_NUMBER]->GetXPos()+87*j+174*k,
				m_textCtl[IT_TOURNAMENT_STEP1_NUMBER]->GetYPos(), buffer, 16, 13, CHAT_ARRANGE_CENTER );
		}
		k++;
	}

	
	k=0;
	for(i=12; i<14; i++)
	{	
		for(j=0; j<2; j++)
		{
			CGolryunBattle::tagMatch* pMatch = g_GolryunBattle.GetMatchPointer(i);
			if(pMatch->Fighter[j].iFightNum <= 0) continue;

			sprintf( buffer, "%d", pMatch->Fighter[j].iFightNum );
			g_chatManager->DrawText(m_textCtl[IT_TOURNAMENT_STEP2_NUMBER]->GetXPos()+174*j+347*k,
				m_textCtl[IT_TOURNAMENT_STEP2_NUMBER]->GetYPos(), buffer, 16, 13, CHAT_ARRANGE_CENTER );
		}
		k++;
	}

	
	for(j=0; j<2; j++)
	{	
		CGolryunBattle::tagMatch* pMatch = g_GolryunBattle.GetMatchPointer(14);
		if(pMatch->Fighter[j].iFightNum <= 0) continue;

		sprintf( buffer, "%d", pMatch->Fighter[j].iFightNum );
		g_chatManager->DrawText(m_textCtl[IT_TOURNAMENT_STEP3_NUMBER]->GetXPos()+347*j,
			m_textCtl[IT_TOURNAMENT_STEP3_NUMBER]->GetYPos(), buffer, 16, 13, CHAT_ARRANGE_CENTER );
	
	}
	
	CGolryunBattle::tagTournament* pTournament = g_GolryunBattle.GetTournamentPointer();
	if(pTournament->FirstWinner.iFightNum <= 0) return;
	sprintf( buffer, "%d", pTournament->FirstWinner.iFightNum );
	g_chatManager->DrawText(m_textCtl[IT_TOURNAMENT_STEP4_NUMBER]->GetXPos(),
		m_textCtl[IT_TOURNAMENT_STEP4_NUMBER]->GetYPos(), buffer, 16, 13, CHAT_ARRANGE_CENTER );

}

void CIFTournamentWin::SetDisplayMode( bool mode )
{

}

void CIFTournamentWin::SetSelectBar( int idx )
{





}









CIFPrizeWin::CIFPrizeWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i=0; i<NUM_IT_PRIZE; i++ )
		m_textCtl[i]	= new CIFText( this );

	
	m_closeBtn			= new CIFButtonCtl( this );

	m_scrollBar			= new CIFScrollBar( this );



	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_PRIZE_WINDOW_TYPE );

}

CIFPrizeWin::~CIFPrizeWin()
{
}

void CIFPrizeWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_PRIZE_WIN );

	
	m_textCtl[IT_PRIZE_GRADE]->PrepareControl( IF_PRIZE_GRADE_TEXT );
	m_textCtl[IT_PRIZE_ITEM_IMAGE]->PrepareControl( IF_PRIZE_ITEM_IMAGE_TEXT );
	m_textCtl[IT_PRIZE_STACK_NUM]->PrepareControl( IF_PRIZE_STACK_NUM_TEXT );
	m_textCtl[IT_PRIZE_ITEM_NAME]->PrepareControl( IF_PRIZE_ITEM_NAME_TEXT );
	m_textCtl[IT_PRIZE_GRADE_POS]->PrepareControl( IF_PRIZE_GRADE_POS );
	m_textCtl[IT_PRIZE_ITEM_IMAGE_POS]->PrepareControl( IF_PRIZE_ITEM_IMAGE_POS );
	m_textCtl[IT_PRIZE_STACK_NUM_POS]->PrepareControl( IF_PRIZE_STACK_NUM_POS );
	m_textCtl[IT_PRIZE_ITEM_NAME_POS]->PrepareControl( IF_PRIZE_ITEM_NAME_POS );
	m_textCtl[IT_PRIZE_USER_NAME]->PrepareControl( IF_PRIZE_USER_NAME_TEXT );
	m_textCtl[IT_PRIZE_USER_NAME_POS]->PrepareControl( IF_PRIZE_USER_NAME_POS );

	
	m_closeBtn->PrepareControl( IF_PRIZE_CLOSE_BTN_0 );

	
	m_scrollBar->PrepareControl( IF_PRIZE_SCROLL );
	m_scrollBar->SetBarPosition( 0, 0 );

}

int CIFPrizeWin::WindowFrame()
{
	if( !State() ) return IF_CTL_NONE;

	m_textCtl[IT_PRIZE_GRADE]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,182) );
	m_textCtl[IT_PRIZE_ITEM_IMAGE]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,492) );
	m_textCtl[IT_PRIZE_STACK_NUM]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,356) );
	m_textCtl[IT_PRIZE_ITEM_NAME]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,184) );
	m_textCtl[IT_PRIZE_USER_NAME]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,357) );

	m_closeBtn->SetButtonText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,936) );

	if( CGolryunBattle::MAX_NUM_PRIZE_LIST_PER_BATTLE - 8 > 0 && CGolryunBattle::MAX_NUM_PRIZE_LIST_PER_BATTLE - 8 != m_scrollBar->m_max )
	{
		m_scrollBar->SetBarPosition( CGolryunBattle::MAX_NUM_PRIZE_LIST_PER_BATTLE - 8, m_scrollBar->m_current );
	}
	else if( CGolryunBattle::MAX_NUM_PRIZE_LIST_PER_BATTLE - 8 <= 0 )
	{
		m_scrollBar->SetBarPosition( 0, 0 );
	}

	return IF_CTL_NONE;
}

void CIFPrizeWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();

	
	m_closeBtn->RenderTextBtn();

	m_scrollBar->Render();

	int itemTableIdx, cx, cy;
	g_ifMng->m_iconCtl->BeginFrame();
	{
		
		for( int i = 0; i < 3; i ++ )
		{


			{
				cx = m_textCtl[IT_PRIZE_ITEM_IMAGE_POS]->m_info.clientX;
				cy = m_textCtl[IT_PRIZE_ITEM_IMAGE_POS]->m_info.clientY + i*47;
				itemTableIdx = 456+i;
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}

	}
	g_ifMng->m_iconCtl->EndFrame();
	
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_PRIZE_GRADE]->Render();
		m_textCtl[IT_PRIZE_ITEM_IMAGE]->Render();
		m_textCtl[IT_PRIZE_STACK_NUM]->Render();
		m_textCtl[IT_PRIZE_ITEM_NAME]->Render();
		m_textCtl[IT_PRIZE_USER_NAME]->Render();

		UpdatePrizeList();
		UpdatePrizeUserList();
	}
	g_chatManager->EndFrame();
}

int CIFPrizeWin::WindowSelection()
{


	





	return( IF_CTL_NONE );
}

int CIFPrizeWin::WindowProcess()
{
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_SELECT;
	}

	if( m_scrollBar->ProcessScrollBar() == IF_CTL_SELECT )
	{
		return IF_CTL_SELECT;
	}

	return IF_CTL_NONE;
}

void CIFPrizeWin::UpdatePrizeList()
{




	char buffer[256];

	for( int i=0; i<3; i++ )
	{
		
		sprintf( buffer, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,358), i+1 );
		g_chatManager->DrawText( m_textCtl[IT_PRIZE_GRADE_POS]->GetXPos(), m_textCtl[IT_PRIZE_GRADE_POS]->GetYPos()+i*49, buffer, 50, 13, CHAT_ARRANGE_OVERSIZECUT );

		sprintf( buffer, "%d", 10*(i+1));
		g_chatManager->DrawText( m_textCtl[IT_PRIZE_STACK_NUM_POS]->GetXPos(), m_textCtl[IT_PRIZE_STACK_NUM_POS]->GetYPos()+i*49, buffer, 50, 13, CHAT_ARRANGE_OVERSIZECUT );
	
		sprintf( buffer, "%s", g_itemTable[456+i].name );
		g_chatManager->DrawText( m_textCtl[IT_PRIZE_ITEM_NAME_POS]->GetXPos(), m_textCtl[IT_PRIZE_ITEM_NAME_POS]->GetYPos()+i*49, buffer, 121, 13, CHAT_ARRANGE_OVERSIZECUT );

	}
}

void CIFPrizeWin::UpdatePrizeUserList()
{

	char buffer[256];
	int pos = m_scrollBar->m_current;

	CGolryunBattle::tagBattle *pBattle = g_GolryunBattle.GetBattleCurrentPointer();

	for( int i=pos; i<8+pos; i++ )
	{
		if(pBattle->m_PrizeNumber[i] <= 0) continue;

		if(i<1)
			sprintf( buffer, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,359), pBattle->m_PrizeNumber[i]);
		else if(i<6)
			sprintf( buffer, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,360), pBattle->m_PrizeNumber[i]);
		else if(i<16)
			sprintf( buffer, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,380), pBattle->m_PrizeNumber[i]);

		g_chatManager->DrawText( m_textCtl[IT_PRIZE_USER_NAME_POS]->GetXPos(), m_textCtl[IT_PRIZE_USER_NAME_POS]->GetYPos()+18*(i-pos), buffer, 300, 13, CHAT_ARRANGE_OVERSIZECUT );

	}
}

void CIFPrizeWin::SetSelectBar( int idx )
{





}




CIFGolryunInfoWin::CIFGolryunInfoWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	m_textCtl = new CIFText( this );

	m_title				= new CIFControl( this );	

	Enable( false );
	g_ifMng->AddFocus( this );
	SetWindowType( IF_BATTLE_INFO_WINDOW_TYPE );

	m_RemainTime  =0;
	m_bTimeOver = false;

}

CIFGolryunInfoWin::~CIFGolryunInfoWin()
{

}

void CIFGolryunInfoWin::PrepareControl()
{

	CIFControl::PrepareControl( IF_GUILD_BATTLE_INFO_WIN );

	m_textCtl->PrepareControl(IF_GUILD_BATTLE_TIME);

}

void CIFGolryunInfoWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();
	
	m_textCtl->Render();
}

int	CIFGolryunInfoWin::WindowFrame()
{
	if( !State() ) return IF_CTL_NONE;

	char szTemp[256];

	if(m_RemainTime<0)	m_RemainTime=0;

	sprintf(szTemp, "%d(min) %d(sec)", m_RemainTime/60000, (m_RemainTime%60000) / 1000);
	m_textCtl->SetText(szTemp);

	return IF_CTL_NONE;

}

int	CIFGolryunInfoWin::WindowProcess()
{
	
	return IF_CTL_NONE;
}

int	CIFGolryunInfoWin::WindowSelection()
{
	return IF_CTL_NONE;
	
}






CIFObserverMenuWin::CIFObserverMenuWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	m_twogyunBtn = new CIFButtonCtl( this );
	m_shareBtn = new CIFButtonCtl( this );
	m_seedtableBtn = new CIFButtonCtl( this );
	m_closeBtn = new CIFButtonCtl( this );

	Enable( true );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_OBSERVER_MENU_WINDOW_TYPE );
}

CIFObserverMenuWin::~CIFObserverMenuWin()
{

}

void CIFObserverMenuWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_OBSERVER_MENU_WIN );

	m_twogyunBtn->PrepareControl( IF_OBSERVER_MENU_WIN_TWOGYUN_BTN_0 );
	m_shareBtn->PrepareControl( IF_OBSERVER_MENU_WIN_SHARE_BTN_0 );
	m_seedtableBtn->PrepareControl( IF_OBSERVER_MENU_WIN_SEEDTABLE_BTN_0 );
	m_closeBtn->PrepareControl( IF_OBSERVER_MENU_WIN_CLOSE_BTN_0 );

}

int CIFObserverMenuWin::WindowFrame()
{
	if( !State() ) return( IF_CTL_NONE );
	
	return IF_CTL_NONE;
}

void CIFObserverMenuWin::Render()
{
	if( !State() ) return;
	
	g_ifMng->SetRenderState();
	CIFControl::Render();

	m_twogyunBtn->Render();
	m_shareBtn->Render();
	m_seedtableBtn->Render();
	m_closeBtn->Render();

}

int CIFObserverMenuWin::WindowSelection()
{
	return IF_CTL_NONE;
}

int CIFObserverMenuWin::WindowProcess()
{
	if( m_twogyunBtn->ProcessButton() == IF_CTL_SELECT )
	{

		g_GolryunBattle_Encoder.SendOpenBattleTicket();
		return( IF_CTL_SELECT );
	}
	else if( m_twogyunBtn->Selection() == IF_IN_POS )
	{
		g_ifMng->InitRolloverText();
		
		g_ifMng->AddRolloverText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,458) );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
	}


	if( m_shareBtn->ProcessButton() == IF_CTL_SELECT )
	{	

		g_GolryunBattle_Encoder.SendOpenShareWin();
		return( IF_CTL_SELECT );
	}
	else if( m_shareBtn->Selection() == IF_IN_POS )
	{
		g_ifMng->InitRolloverText();
		
		g_ifMng->AddRolloverText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,459) );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
	}

	if( m_seedtableBtn->ProcessButton() == IF_CTL_SELECT )
	{

		g_GolryunBattle_Encoder.SendDisplayTournament();
		return( IF_CTL_SELECT );
	}
	else if( m_seedtableBtn->Selection() == IF_IN_POS )
	{
		g_ifMng->InitRolloverText();
		
		g_ifMng->AddRolloverText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,460) );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
	}


	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{

		g_GolryunBattle.SetObserverMode(CGolryunBattle::eObserverMode::general);
		memcpy(g_pApp->m_myCharacter->position, g_GolryunBattle.m_OldPosition, sizeof(vec3_t));
		g_pApp->m_myCharacter->m_bObserveMode = FALSE;
		g_camera.m_maxCamDist = -350.0f;
		g_camera.m_distance = -350.0f;

		g_ifMng->m_observerInfoWin->Enable(false);
		
		g_GolryunBattle_Encoder.SendObserveEnd();

		
		GTH_SendEventMessage_Idle();
		GTH_UpdateSendedZPos();

		return( IF_CTL_SELECT );
	}
	else if( m_closeBtn->Selection() == IF_IN_POS )
	{
		g_ifMng->InitRolloverText();
		
		g_ifMng->AddRolloverText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,461) );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
	}

	return IF_CTL_NONE;
}







CIFObserverInfoWin::CIFObserverInfoWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{

	for( int i=0; i<NUM_IT_OBSERVER_INFO; i++ )
		m_textCtl[i]	= new CIFText( this );
	
	m_redFighterRaBar = new CIFBarCtl( this );
	m_redFighterSaBar = new CIFBarCtl( this );
	m_blueFighterRaBar = new CIFBarCtl( this );
	m_blueFighterSaBar = new CIFBarCtl( this );

	Enable( false );
	m_redFighterRaBar->Enable( false );
	m_redFighterSaBar->Enable( false );
	m_blueFighterRaBar->Enable( false );
	m_blueFighterSaBar->Enable( false );

	m_RemainTime = 0;

	m_redFighterNumber = 0;
	m_blueFighterNumber = 0;

	m_redFighterCurRa = 0;
	m_redFighterMaxRa = 0;
	m_redFighterCurSa = 0;
	m_redFighterMaxSa = 0;

	m_blueFighterCurRa = 0;
	m_blueFighterMaxRa = 0;
	m_blueFighterCurSa = 0;
	m_blueFighterMaxSa = 0;


	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_OBSERVER_INFO_WINDOW_TYPE );

}

CIFObserverInfoWin::~CIFObserverInfoWin()
{

}

void CIFObserverInfoWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_OBSERVER_INFO_WIN );

	
	m_textCtl[IT_BATTLE_TIME]->PrepareControl( IF_OBSERVER_INFO_WIN_TIME );
	m_textCtl[IT_RED_FIGHTER_NAME]->PrepareControl( IF_OBSERVER_INFO_WIN_RED );
	m_textCtl[IT_BLUE_FIGHTER_NAME]->PrepareControl( IF_OBSERVER_INFO_WIN_BLUE );

	m_redFighterRaBar->PrepareControl( IF_OBSERVER_INFO_WIN_RED_RA_BAR );
	m_redFighterSaBar->PrepareControl( IF_OBSERVER_INFO_WIN_RED_SA_BAR );
	m_blueFighterRaBar->PrepareControl( IF_OBSERVER_INFO_WIN_BLUE_RA_BAR );
	m_blueFighterSaBar->PrepareControl( IF_OBSERVER_INFO_WIN_BLUE_SA_BAR );
	
}

int CIFObserverInfoWin::WindowFrame()
{
	if( !State() ) return IF_CTL_NONE;

	char szTemp[256];

	sprintf(szTemp, "%d : %d", m_RemainTime/60000, (m_RemainTime%60000) / 1000);
	m_textCtl[IT_BATTLE_TIME]->SetColor(D3DCOLOR_ARGB(255, 255, 255, 0));
	m_textCtl[IT_BATTLE_TIME]->SetText(szTemp);

	if(m_redFighterNumber<=0 || m_blueFighterNumber<=0)
	{
		m_textCtl[IT_RED_FIGHTER_NAME]->SetText( "" );
		m_textCtl[IT_BLUE_FIGHTER_NAME]->SetText( "" );

		m_redFighterRaBar->Enable(false);
		m_redFighterSaBar->Enable(false);
		m_blueFighterRaBar->Enable(false);
		m_blueFighterSaBar->Enable(false);

		g_pApp->m_myCharacter->position[0] = 2108;
		g_pApp->m_myCharacter->position[1] = -11771;
		g_pApp->m_myCharacter->position[2] = -300;

		g_GolryunBattle.m_ObserverPos[0] = 2108;
		g_GolryunBattle.m_ObserverPos[1] = -11771;
		g_GolryunBattle.m_ObserverPos[2] = 180;

		return IF_CTL_NONE;
	}

	
	sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,396), m_redFighterNumber );

	m_textCtl[IT_RED_FIGHTER_NAME]->SetText( szTemp );
		
	
	sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,396), m_blueFighterNumber );

	m_textCtl[IT_BLUE_FIGHTER_NAME]->SetText( szTemp );


	int i;
	Fx_CHARACTER_t *character=NULL;
	int numCharacter=0;
	vec3_t redPos, bluePos;
	memset(redPos, 0, sizeof(vec3_t));
	memset(bluePos, 0, sizeof(vec3_t));

	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i<numCharacter; i++, character++)
	{
		
		if(character->entityType != ENTITY_PC)	continue;
		if(character->m_JoinNum<=0)	continue;

		if(character->m_JoinNum == m_redFighterNumber)
		{
			m_redFighterRaBar->Enable();
			m_redFighterRaBar->UpdateBarState_Inverse( character->curRA, character->calMaxRA );

			m_redFighterSaBar->Enable();
			m_redFighterSaBar->UpdateBarState_Inverse( character->curSA, character->calMaxSA );

			redPos[0] = character->position[0];
			redPos[1] = character->position[1];
		}

		if(character->m_JoinNum == m_blueFighterNumber)
		{
			m_blueFighterRaBar->Enable();
			m_blueFighterRaBar->UpdateBarState( character->curRA, character->calMaxRA );

			m_blueFighterSaBar->Enable();
			m_blueFighterSaBar->UpdateBarState( character->curSA, character->calMaxSA );

			bluePos[0] = character->position[0];
			bluePos[1] = character->position[1];
		}

		g_pApp->m_myCharacter->position[0] = (redPos[0]+bluePos[0])/2;
		g_pApp->m_myCharacter->position[1] = (redPos[1]+bluePos[1])/2;
		g_pApp->m_myCharacter->position[2] = -300;

		g_GolryunBattle.m_ObserverPos[0] = (redPos[0]+bluePos[0])/2;
		g_GolryunBattle.m_ObserverPos[1] = (redPos[1]+bluePos[1])/2;
		g_GolryunBattle.m_ObserverPos[2] = 180;


		
		if(g_GolryunBattle.m_ObserverPos[0]>2894.0f || g_GolryunBattle.m_ObserverPos[0]<1331.0f || 
		   g_GolryunBattle.m_ObserverPos[1]> -10992.0f || g_GolryunBattle.m_ObserverPos[1]< -12554.0f)
		{
			g_GolryunBattle.m_ObserverPos[0] = 2108;
			g_GolryunBattle.m_ObserverPos[1] = -11771;
			g_GolryunBattle.m_ObserverPos[2] = 180;
		}
	}

	return( IF_CTL_NONE );
}

void CIFObserverInfoWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();

	m_redFighterRaBar->Render();
	m_redFighterSaBar->Render();
	m_blueFighterRaBar->Render();
	m_blueFighterSaBar->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_BATTLE_TIME]->Render();
		m_textCtl[IT_RED_FIGHTER_NAME]->Render();
		m_textCtl[IT_BLUE_FIGHTER_NAME]->Render();
	}
	g_chatManager->EndFrame();
}


int CIFObserverInfoWin::WindowSelection()
{
	int ret;

	
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFObserverInfoWin::WindowProcess()
{
	if( Selection() == IF_LBUP )
	{
		





		g_GameTarget.GTH_ChangeTarget( g_pApp->m_myCharacter );
	}
	return IF_CTL_NONE;
}






CIFTournamentMiniWin::CIFTournamentMiniWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	int i;
	for( i=0; i<NUM_IT_TOURNAMENT_MINI; i++ )
		m_textCtl[i]	= new CIFText( this );

	for( i=0; i<TOURNAMENT_NUMBER_MAX_NUM; i++ )
		m_textNumCtl[i] = new CIFText( this );

	for( i=0; i<LOSE_DISPLAY_MAX_NUM; i++ )
		m_loseX[i]		= new CIFControl( this );

	
	m_closeBtn			= new CIFButtonCtl( this );
	
	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_TOURNAMENT_MINI_WINDOW_TYPE );

}

CIFTournamentMiniWin::~CIFTournamentMiniWin()
{
}

void CIFTournamentMiniWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_TOURNAMENT_MINI_WIN );

	
	m_textCtl[IT_TOURNAMENT_MINI_TITLE]->PrepareControl( IF_TOURNAMENT_MINI_TITLE_TEXT );
	m_textCtl[IT_TOURNAMENT_MINI_WIN_NUMBER]->PrepareControl( IF_TOURNAMENT_MINI_WIN_NUMBER );

	int i;
	for( i=0; i<TOURNAMENT_NUMBER_MAX_NUM; i++ )
		m_textNumCtl[i]->PrepareControl( IF_TOURNAMENT_MINI_NUMBER_0 + i);

	for( i=0; i<LOSE_DISPLAY_MAX_NUM; i++ )
		m_loseX[i]->PrepareControl( IF_TOURNAMENT_MINI_LOSE_0 + i);

	
	m_closeBtn->PrepareControl( IF_TOURNAMENT_MINI_CLOSE_BTN_0 );

	
	m_textCtl[IT_TOURNAMENT_MINI_TITLE]->SetText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,355) );

	m_closeBtn->SetButtonText( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,936) );

}

int CIFTournamentMiniWin::WindowFrame()
{
	
	return( IF_CTL_NONE );
}

void CIFTournamentMiniWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();

	int i,j,k=0;
	for(i=0; i<CGolryunBattle::MAX_NUM_MATCH_PER_TOURNAMENT; i++)
	{

		for(j=0; j<2; j++)
		{
			CGolryunBattle::tagMatch* pMatch = g_GolryunBattle.GetMatchPointer(i);
			if((TRUE == pMatch->bComplete) &&
				(FALSE == pMatch->Fighter[j].bWin)){

				m_loseX[k]->Render();
			}			
			k++;
		}
	}

	
	m_closeBtn->RenderTextBtn();
	
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_TOURNAMENT_MINI_TITLE]->Render();

		UpdateTournamentList();
	}
	g_chatManager->EndFrame();
}

int CIFTournamentMiniWin::WindowSelection()
{
	int ret;

	
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFTournamentMiniWin::WindowProcess()
{
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_SELECT;
	}
	return IF_CTL_NONE;
}

void CIFTournamentMiniWin::UpdateTournamentList()
{

	int i,j,k;
	char buffer[32];

	CGolryunBattle::tagBattle* pBattle = g_GolryunBattle.GetBattleCurrentPointer();
	if(pBattle == NULL) return;
	
	for(i=0; i<CGolryunBattle::MAX_NUM_TOURNAMENT_PER_BATTLE; i++)
	{
		if(!pBattle->Tournament[i].bComplete) continue;
		if(pBattle->Tournament[i].FirstWinner.iFightNum <= 0) continue;
		
		sprintf( buffer, "%d", pBattle->Tournament[i].FirstWinner.iFightNum );
		g_chatManager->DrawText(m_textCtl[IT_TOURNAMENT_MINI_WIN_NUMBER]->GetXPos()+i*31,
			m_textCtl[IT_TOURNAMENT_MINI_WIN_NUMBER]->GetYPos(), buffer, 12, 14, CHAT_ARRANGE_CENTER );
	}

	k=0;
	for(i=0; i<CGolryunBattle::MAX_NUM_MATCH_PER_TOURNAMENT; i++)
	{
		CGolryunBattle::tagMatch* pMatch = g_GolryunBattle.GetMatchPointer(i);
		if(pMatch == NULL)	continue;
 
		for(j=0; j<2; j++)
		{
			if(pMatch->Fighter[j].iFightNum > 0)
			{
				sprintf( buffer, "%d", pMatch->Fighter[j].iFightNum );
				g_chatManager->DrawText(m_textNumCtl[k]->GetXPos(),
					m_textNumCtl[k]->GetYPos(), buffer, 12, 14, CHAT_ARRANGE_CENTER );
			}

			k++;	
		}	
	}

	
	CGolryunBattle::tagTournament* pTournament = g_GolryunBattle.GetTournamentPointer();
	if(pTournament == NULL)	return;
	if(pTournament->FirstWinner.iFightNum <= 0) return;
	sprintf( buffer, "%d", pTournament->FirstWinner.iFightNum );
	g_chatManager->DrawText(m_textNumCtl[30]->GetXPos(),
		m_textNumCtl[30]->GetYPos(), buffer, 12, 14, CHAT_ARRANGE_CENTER );

}
