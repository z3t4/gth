



#include "../global_def.h"
#include "IFCommon.h"









CIFMsgWin::CIFMsgWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_MESSAGE_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );
	m_line = new CIFControl( this );

	
	m_okBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );
	m_okCenterBtn = new CIFButtonCtl( this );

	m_buttonType = MSG_BUTTONTYPE_NONE;
	m_bInputMode = false;

	

	Enable( false );
}

CIFMsgWin::~CIFMsgWin()
{

}

void CIFMsgWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_MESSAGE_WIN );
	
	m_textCtl[IT_MESSAGE_WIN_TITLE]->PrepareControl( IF_MESSAGE_WIN_TITLE_TEXT );
	m_textCtl[IT_MESSAGE_WIN_U]->PrepareControl( IF_MESSAGE_WIN_TEXT_U );
	
	
	m_titleBar->PrepareControl( IF_MESSAGE_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_MESSAGE_WIN_TITLE_BOTTOM );
	m_line->PrepareControl( IF_MESSAGE_WIN_LINE );

	
	m_okBtn->PrepareControl( IF_MESSAGE_WIN_OK_BTN_0 );
	m_cancelBtn->PrepareControl( IF_MESSAGE_WIN_CANCEL_BTN_0 );
	m_okCenterBtn->PrepareControl( IF_MESSAGE_WIN_OKCENTER_BTN_0 );

	
	
}

int CIFMsgWin::WindowFrame()
{

	
	
	m_textCtl[IT_MESSAGE_WIN_TITLE]->SetText( g_ifMng->m_msgTitleStr );
	m_textCtl[IT_MESSAGE_WIN_U]->SetText( g_ifMng->m_msgStr );

	
	
	switch( m_buttonType )
	{
	case MSG_BUTTONTYPE_OKCANCEL :
		m_okBtn->ChangeControlTexInfo( IF_MESSAGE_WIN_OK_BTN_0, 0 );
		m_okBtn->ChangeControlTexInfo( IF_MESSAGE_WIN_OK_BTN_1, 1 );
		m_okBtn->ChangeControlTexInfo( IF_MESSAGE_WIN_OK_BTN_2, 2 );
		m_cancelBtn->ChangeControlTexInfo( IF_MESSAGE_WIN_CANCEL_BTN_0, 0 );
		m_cancelBtn->ChangeControlTexInfo( IF_MESSAGE_WIN_CANCEL_BTN_1, 1 );
		m_cancelBtn->ChangeControlTexInfo( IF_MESSAGE_WIN_CANCEL_BTN_2, 2 );
		break;
	case MSG_BUTTONTYPE_OK :
		m_okCenterBtn->ChangeControlTexInfo( IF_MESSAGE_WIN_OKCENTER_BTN_0, 0 );
		m_okCenterBtn->ChangeControlTexInfo( IF_MESSAGE_WIN_OKCENTER_BTN_1, 1 );
		m_okCenterBtn->ChangeControlTexInfo( IF_MESSAGE_WIN_OKCENTER_BTN_2, 2 );
		break;
	case MSG_BUTTONTYPE_CANCEL :
		m_okCenterBtn->ChangeControlTexInfo( IF_MESSAGE_WIN_CANCEL_BTN_0, 0 );
		m_okCenterBtn->ChangeControlTexInfo( IF_MESSAGE_WIN_CANCEL_BTN_1, 1 );
		m_okCenterBtn->ChangeControlTexInfo( IF_MESSAGE_WIN_CANCEL_BTN_2, 2 );
		break;
	case MSG_BUTTONTYPE_NONE :
		break;
	}

	return IF_CTL_NONE;
}

void CIFMsgWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	
	m_titleBar->Render();
	m_titleBottom->Render();
	m_line->Render();

	
	switch( m_buttonType )
	{
	case MSG_BUTTONTYPE_OKCANCEL :
		m_okBtn->Render();
		m_cancelBtn->Render();
		break;
	case MSG_BUTTONTYPE_OK :
		m_okCenterBtn->Render();
		break;
	case MSG_BUTTONTYPE_CANCEL :
		m_okCenterBtn->Render();
		break;
	case MSG_BUTTONTYPE_NONE :
		break;
	}

	



	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_MESSAGE_WIN_TITLE]->Render();
		m_textCtl[IT_MESSAGE_WIN_U]->Render( IF_TEXT_OUTPUT_RECT );
	}
	g_chatManager->EndFrame();
}

int CIFMsgWin::WindowSelection()
{
	return IF_CTL_NONE;
}

int CIFMsgWin::WindowProcess()
{
	int timeSelect = false;

	if( !State() ) return( IF_CTL_NONE );


	
	if( g_ifMng->m_msgWaitTime > 0.0f )
	{
		if( g_timer.GetAppMilliTime() - g_ifMng->m_msgTime > g_ifMng->m_msgWaitTime )
		{
			timeSelect = true;
		}
	}

	switch( m_buttonType )
	{
	case MSG_BUTTONTYPE_OKCANCEL :
		if( m_okBtn->ProcessButton() == IF_CTL_SELECT )
		{
			g_ifMng->MessageProcessOk();
			return( IF_CTL_NONE );
		}
		if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT || timeSelect )
		{
			g_ifMng->MessageProcessCancel();
			return( IF_CTL_NONE );
		}
		if( g_input->KeyDown( DIK_RETURN ) ) g_ifMng->MessageProcessOk();
		if( g_input->KeyDown( DIK_ESCAPE ) ) g_ifMng->MessageProcessCancel();
		break;
	case MSG_BUTTONTYPE_OK : 
		if( m_okCenterBtn->ProcessButton() == IF_CTL_SELECT || timeSelect ) 
		{
			g_ifMng->MessageProcessOk();
			return( IF_CTL_NONE );
		}
		if( g_input->KeyDown( DIK_RETURN ) ) g_ifMng->MessageProcessOk();
		if( g_input->KeyDown( DIK_ESCAPE ) ) g_ifMng->MessageProcessOk();
		break;	
	case MSG_BUTTONTYPE_CANCEL :
		if( m_okCenterBtn->ProcessButton() == IF_CTL_SELECT || timeSelect ) 
		{
			g_ifMng->MessageProcessCancel();
			return( IF_CTL_NONE );
		}
		if( g_input->KeyDown( DIK_RETURN ) ) g_ifMng->MessageProcessCancel();
		if( g_input->KeyDown( DIK_ESCAPE ) ) g_ifMng->MessageProcessCancel();
		break;	
	case MSG_BUTTONTYPE_NONE :
		break;
	}
	return IF_CTL_NONE;
}












CIFStackWin::CIFStackWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_STACK_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );
	m_inputNumBar = new CIFControl( this );
	m_inputBar = new CIFControl( this );
	m_numBar = new CIFControl( this );
	m_line = new CIFControl( this );
	
	for( i = 0; i < MAX_COUNT_IF_STACK_WIN_NUM; i ++ )
	{
		m_numUpBtn[i] = new CIFButtonCtl( this );
		m_numDnBtn[i] = new CIFButtonCtl( this );
	}
	m_okBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );
	m_closeBtn = new CIFButtonCtl( this );
	
	m_count = 0;
	m_maxCount = 0;

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_STACK_WINDOW_TYPE );
}

CIFStackWin::~CIFStackWin()
{

}

void CIFStackWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_STACK_WIN );
	
	m_textCtl[IT_STACK_WIN_TITLE]->PrepareControl( IF_STACK_WIN_TITLE_TEXT );
	m_textCtl[IT_STACK_WIN_INPUT]->PrepareControl( IF_STACK_WIN_INPUT_TEXT );
	m_textCtl[IT_STACK_WIN_INPUT_U]->PrepareControl( IF_STACK_WIN_INPUT_TEXT_U );
	m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->PrepareControl( IF_STACK_WIN_INPUT_NUM_TEXT_U );
	m_textCtl[IT_STACK_WIN_NUM_POS_0]->PrepareControl( IF_STACK_WIN_NUM_POS_0 );
	m_textCtl[IT_STACK_WIN_NUM_POS_1]->PrepareControl( IF_STACK_WIN_NUM_POS_1 );
	m_textCtl[IT_STACK_WIN_NUM_POS_2]->PrepareControl( IF_STACK_WIN_NUM_POS_2 );
	m_textCtl[IT_STACK_WIN_NUM_POS_3]->PrepareControl( IF_STACK_WIN_NUM_POS_3 );
	m_textCtl[IT_STACK_WIN_NUM_POS_4]->PrepareControl( IF_STACK_WIN_NUM_POS_4 );
	m_textCtl[IT_STACK_WIN_NUM_POS_5]->PrepareControl( IF_STACK_WIN_NUM_POS_5 );
	m_textCtl[IT_STACK_WIN_NUM_POS_6]->PrepareControl( IF_STACK_WIN_NUM_POS_6 );
	m_textCtl[IT_STACK_WIN_NUM_POS_7]->PrepareControl( IF_STACK_WIN_NUM_POS_7 );
	m_textCtl[IT_STACK_WIN_NUM_POS_8]->PrepareControl( IF_STACK_WIN_NUM_POS_8 );
	m_textCtl[IT_STACK_WIN_NUM_POS_9]->PrepareControl( IF_STACK_WIN_NUM_POS_9 );

	m_titleBar->PrepareControl( IF_STACK_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_STACK_WIN_TITLE_BOTTOM );
	m_inputNumBar->PrepareControl( IF_STACK_WIN_INPUT_NUM_BAR );
	m_inputBar->PrepareControl( IF_STACK_WIN_INPUT_BAR );
	m_numBar->PrepareControl( IF_STACK_WIN_NUM_BAR );
	m_line->PrepareControl( IF_STACK_WIN_LINE );
	
	for( int i = 0; i < MAX_COUNT_IF_STACK_WIN_NUM; i ++ )
	{
		m_numUpBtn[i]->PrepareControl( IF_STACK_WIN_NUM_BTN_P0_0 + i * 3 );
		m_numDnBtn[i]->PrepareControl( IF_STACK_WIN_NUM_BTN_M0_0 + i * 3 );
	}
		
	m_okBtn->PrepareControl( IF_STACK_WIN_OK_BTN_0 );
	m_cancelBtn->PrepareControl( IF_STACK_WIN_CANCEL_BTN_0 );
	m_closeBtn->PrepareControl( IF_STACK_WIN_CLOSE_BTN_0 );
}

int CIFStackWin::WindowFrame()
{
	int i, n, tempValue, price;
	char szTemp[256], numStr[20];
	
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 535 ) );
	m_textCtl[IT_STACK_WIN_TITLE]->SetText( szTemp );

	itoa( m_count, numStr, 10 );
	n = strlen( numStr );
	for( i = 0; i < MAX_COUNT_IF_STACK_WIN_NUM; i ++ )
	{
		if( i < n ) 
		{
			sprintf( szTemp, "%c", numStr[(n - i) - 1] );
			m_textCtl[IT_STACK_WIN_NUM_POS_0 + i]->SetText( szTemp );
		}
		else
		{
			m_textCtl[IT_STACK_WIN_NUM_POS_0 + i]->m_str[0] = 0;
		}
	}

	m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetColor( 0xffffffff );

	switch( m_stackType )
	{
	case IF_STACK_TYPE_INVENTORY : case IF_STACK_TYPE_DEPOT :
		strcpy( szTemp, g_ifMng->GetNumberString( m_maxCount - m_count ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
		break;
	case IF_STACK_TYPE_BUY_ITEM : case IF_STACK_TYPE_BUY_BOOTH_ITEM :
		strcpy( szTemp, g_ifMng->GetNumberString( m_count * m_saveItemPrice ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
		break;
	case IF_STACK_TYPE_EPACK :
		if( m_chargeType == 0 ) 
		{
			
			
			
			int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
			
			if( epackIdx < 0 ) return IF_CTL_NONE;

			int idx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;
			
			price = (int)GetEpackRechargePrice_RA( g_cgv.myCharacterInfo->onceRecoveryRA,
													m_chargeCost,
													(float)g_itemTable[idx].level );

			tempValue = m_count * price;
		}
		else
		{
			
			
			int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
			
			if( epackIdx < 0 ) return IF_CTL_NONE;

			int idx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;
			
			price = (int)GetEpackRechargePrice_SA( g_cgv.myCharacterInfo->onceRecoverySA,
													m_chargeCost,
													(float)g_itemTable[idx].level );

			tempValue = m_count * price;
		}
		strcpy( szTemp, g_ifMng->GetNumberString( tempValue ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
		break;
		
	case IF_STACK_TYPE_BATTING_TRADE: case IF_STACK_TYPE_TRADE_SE : case IF_STACK_TYPE_DEPOT_SAVE_SE : case IF_STACK_TYPE_DEPOT_LOAD_SE : case IF_STACK_TYPE_SELL_BOOTH_PRICE : case IF_STACK_TYPE_MAILBOX_SEND : case IF_STACK_TYPE_HELPERSYSTEM_MYPOINT:
		strcpy( szTemp, g_ifMng->GetNumberString( m_count ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
		break;
	}
		
	return( IF_CTL_NONE );
}

void CIFStackWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	m_titleBar->Render();
	m_titleBottom->Render();
	m_inputNumBar->Render();
	m_inputBar->Render();
	m_numBar->Render();
	m_line->Render();
	
	for( int i = 0; i < MAX_COUNT_IF_STACK_WIN_NUM; i ++ )
	{
		m_numUpBtn[i]->Render();
		m_numDnBtn[i]->Render();
	}
		
	m_okBtn->Render();
	m_cancelBtn->Render();
	m_closeBtn->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_STACK_WIN_TITLE]->Render();
		m_textCtl[IT_STACK_WIN_INPUT]->Render();
		m_textCtl[IT_STACK_WIN_INPUT_U]->Render();
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->Render( IF_TEXT_OUTPUT_RIGHT );
		m_textCtl[IT_STACK_WIN_NUM_POS_0]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_STACK_WIN_NUM_POS_1]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_STACK_WIN_NUM_POS_2]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_STACK_WIN_NUM_POS_3]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_STACK_WIN_NUM_POS_4]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_STACK_WIN_NUM_POS_5]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_STACK_WIN_NUM_POS_6]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_STACK_WIN_NUM_POS_7]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_STACK_WIN_NUM_POS_8]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_STACK_WIN_NUM_POS_9]->Render( IF_TEXT_OUTPUT_CENTER );
	}
	g_chatManager->EndFrame();
}

int CIFStackWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN )
	{
		int iii=0;
		return( IF_CTL_BEGIN_MOVING );
	}	
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFStackWin::WindowProcess()
{
	int tempValue, price;
	char szTemp[256], szTemp2[128], szTemp3[128];
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		m_count = 0;
		Enable( false );
		return( IF_CTL_NONE );
	}
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		m_count = 0;
		Enable( false );
		return( IF_CTL_NONE );
	}
	if( m_okBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		if( m_count <= 0 )
		{
			return( IF_CTL_NONE );
		}
		switch( m_stackType )
		{
		case IF_STACK_TYPE_INVENTORY :
			GTH_SendMessage_RequestGetInventoryItem( m_saveInvenPos, false, m_count );
			break;
			
		case IF_STACK_TYPE_DEPOT :
			GTH_SendMessage_ItemSystem_DepotGetItem( m_saveInvenPos, m_count );
			break;
		case IF_STACK_TYPE_BUY_ITEM :
			
			
			strcpy( szTemp2, g_ifMng->GetNumberString( m_count ) );
			strcpy( szTemp3, g_ifMng->GetNumberString( m_saveItemPrice * m_count ) );
			sprintf( szTemp, g_LPACK.GetMassage( 0, 536 ), g_ifMng->m_storeWin->m_storeItem[m_saveInvenPos].item.name, szTemp2, szTemp3 );
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_STORE_BUY_STACK_ITEM );
			break;
		
		case IF_STACK_TYPE_BUY_BOOTH_ITEM :
			
			
			strcpy( szTemp2, g_ifMng->GetNumberString( m_count ) );
			strcpy( szTemp3, g_ifMng->GetNumberString( m_saveItemPrice * m_count ) );
			sprintf( szTemp, g_LPACK.GetMassage( 0, 536 ), g_ifMng->m_boothWin->m_boothBuyItem[m_saveInvenPos].item.name, szTemp2, szTemp3 );
			g_ifMng->m_boothWin->m_saveItemIdx = m_saveInvenPos;
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_BUY_BOOTH_ITEM );
			break;

		case IF_STACK_TYPE_EPACK :
			{
				
				if( m_chargeType == 0 )
				{
					
					
					int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
					
					if( epackIdx < 0 ) return IF_CTL_NONE;

					int idx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;
					
					
					price = (int)GetEpackRechargePrice_RA( g_cgv.myCharacterInfo->onceRecoveryRA,
															m_chargeCost,
															(float)g_itemTable[idx].level );
					tempValue = price * m_count;
					strcpy( szTemp2, g_ifMng->GetNumberString( m_count ) );
					strcpy( szTemp3, g_ifMng->GetNumberString( tempValue ) );
					sprintf( szTemp, g_LPACK.GetMassage( 0, 537 ), szTemp2, szTemp3 );
					g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_STORE_CHARGE_RA );
				}
				
				else
				{
					
					
					int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
					
					if( epackIdx < 0 ) return IF_CTL_NONE;

					int idx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;
					
					
					price = (int)GetEpackRechargePrice_SA( g_cgv.myCharacterInfo->onceRecoverySA,
															m_chargeCost,
															(float)g_itemTable[idx].level );
					tempValue = price * m_count;
					strcpy( szTemp2, g_ifMng->GetNumberString( m_count ) );
					strcpy( szTemp3, g_ifMng->GetNumberString( tempValue ) );
					sprintf( szTemp, g_LPACK.GetMassage( 0, 538 ), szTemp2, szTemp3 );
					g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_STORE_CHARGE_SA );
				}
			}
			break;
		case IF_STACK_TYPE_TRADE_SE :
			{
				if( !g_ifMng->m_tradeWin->m_readyFlag && !g_ifMng->m_tradeWin->m_otherReadyFlag )
				{
					g_ifMng->m_tradeWin->m_myTradeSE = m_count;
					GTH_SendMessage_PCTrade_SE( g_ifMng->m_tradeWin->m_otherIdxOnServer, g_ifMng->m_tradeWin->m_myTradeSE );
				}
			}
			break;

			
		case IF_STACK_TYPE_BATTING_TRADE :
			{
				if( !g_ifMng->m_pgonryunBattleBattingwindow->m_readyFlag && !g_ifMng->m_pgonryunBattleBattingwindow->m_otherReadyFlag )
				{


					int max_batting_nak=g_cgv.myCharacterInfo->curChargeSE - g_GonryunBattlePractic.m_iroyalty;
					if(max_batting_nak > g_GonryunBattlePractic.m_ibattingRoyalty) max_batting_nak = g_GonryunBattlePractic.m_ibattingRoyalty;


					if( m_count > max_batting_nak) {					
					
						char buffer[255];
						
						wsprintf(buffer,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2,538),
							max_batting_nak,
							g_GonryunBattlePractic.m_ibattingRoyalty,
							g_GonryunBattlePractic.m_iroyalty);							
						g_ifMng->SetMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL,322 ),buffer,MSG_BUTTONTYPE_OK,IF_MESSAGE_NONE);
						return IF_CTL_NONE;
					}

					g_ifMng->m_pgonryunBattleBattingwindow->m_myTradeSE = m_count;
					GTH_SendMessage_PCTrade_SEBatting( g_ifMng->m_pgonryunBattleBattingwindow->m_otherIdxOnServer, g_ifMng->m_pgonryunBattleBattingwindow->m_myTradeSE );
				}
			}
			break;
			

			
		case IF_STACK_TYPE_DEPOT_SAVE_SE :
			{
				GTH_SendMessage_ItemSystem_DepotNak( 0, m_count );
			}
			break;
		case IF_STACK_TYPE_DEPOT_LOAD_SE :
			{
				GTH_SendMessage_ItemSystem_DepotNak( 1, m_count );
			}
			break;
		case IF_STACK_TYPE_SELL_BOOTH_PRICE :
			{
				GTH_SendMessage_ItemSystem_BoothSellItem( m_saveInvenPos, m_count );
			}
			break;
	
		case IF_STACK_TYPE_MAILBOX_SEND :
			{
				g_ifMng->m_mailBoxWin->m_nak = 0;
				g_ifMng->m_mailBoxWin->m_nak = m_count;
			}
			break;
	
		case IF_STACK_TYPE_HELPERSYSTEM_MYPOINT:
			{
				g_HelperSystem.m_iGivePoint = m_count;
				g_HelperSystem.GTH_SendMessage_HelpSystem_GivePointToHelper(g_HelperSystem.m_iGivePoint, g_cgv.pTargetChar->name);
				break;
		}
	}
}







	int i, n = 1;
	for( i = 0; i < MAX_COUNT_IF_STACK_WIN_NUM; i ++ )
	{
		if( m_numUpBtn[i]->ProcessButton() == IF_CTL_SELECT )
		{
			m_count += n;
			break;
		}
		if( m_numDnBtn[i]->ProcessButton() == IF_CTL_SELECT )
		{
			m_count -= n;
			break;
		}
		n *= 10;
	}

	if( m_count < 0 ) m_count = 0;
	if( m_count >= m_maxCount ) m_count = m_maxCount;
	
	return IF_CTL_NONE;
}

void CIFStackWin::InitStack( char *str, item_t* item, int type, int invenPos, float price )
{
	char szTemp[256];
	int tempValue1 = 0, tempValue2 = 0;

	m_textCtl[IT_STACK_WIN_INPUT]->SetText( str );
	m_count = 0;
	m_stackType = type;

	if( m_stackType == IF_STACK_TYPE_INVENTORY )
	{
		m_maxCount = item->durability + 1;
		m_saveInvenPos = invenPos;

		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 609 ) );
		m_textCtl[IT_STACK_WIN_INPUT_U]->SetText( szTemp );
		strcpy( szTemp, g_ifMng->GetNumberString( m_maxCount - m_count ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
	}
	
	else if( m_stackType ==  IF_STACK_TYPE_DEPOT )
	{
		m_maxCount = item->durability + 1;
		m_saveInvenPos = invenPos;

		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 609 ) );
		m_textCtl[IT_STACK_WIN_INPUT_U]->SetText( szTemp );
		strcpy( szTemp, g_ifMng->GetNumberString( m_maxCount - m_count ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
	}
	else if( m_stackType == IF_STACK_TYPE_BUY_ITEM )
	{
		
		if( price <= 0.0f ) price = 1.0f;
		m_maxCount = g_cgv.myCharacterInfo->curChargeSE / (int)price;
		
		m_saveInvenPos = invenPos;
		m_saveItemPrice = (int)price;

		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 610 ) );
		m_textCtl[IT_STACK_WIN_INPUT_U]->SetText( szTemp );
		strcpy( szTemp, g_ifMng->GetNumberString( m_count * m_saveItemPrice ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
	}
	
	else if( m_stackType == IF_STACK_TYPE_BUY_BOOTH_ITEM )
	{
		
		if( price <= 0.0f ) price = 1.0f;
		m_maxCount = g_cgv.myCharacterInfo->curChargeSE / (int)price;
		if( m_maxCount > g_ifMng->m_boothWin->m_boothBuyItem[invenPos].item.durability + 1 )
		{
			m_maxCount = g_ifMng->m_boothWin->m_boothBuyItem[invenPos].item.durability + 1;
		}
		
		m_saveInvenPos = invenPos;
		m_saveItemPrice = (int)price;

		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 610 ) );
		m_textCtl[IT_STACK_WIN_INPUT_U]->SetText( szTemp );
		strcpy( szTemp, g_ifMng->GetNumberString( m_count * m_saveItemPrice ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
	}

	else if( m_stackType == IF_STACK_TYPE_EPACK )
	{
		m_chargeType = invenPos;
		m_chargeCost = price;
		if( m_chargeType == 0 ) 
		{
			m_maxCount = g_cgv.myCharacterInfo->maxChargeRA - g_cgv.myCharacterInfo->curChargeRA;
			
			
			int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
			
			

			int idx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;
			
			
			tempValue1 = (int)GetEpackRechargePrice_RA( g_cgv.myCharacterInfo->onceRecoveryRA,
														m_chargeCost,
														(float)g_itemTable[idx].level );
			
			if( tempValue1 > 0 ) tempValue2 = g_cgv.myCharacterInfo->curChargeSE / tempValue1;
			if( tempValue2 < m_maxCount ) m_maxCount = tempValue2;
		}
		else 
		{
			m_maxCount = g_cgv.myCharacterInfo->maxChargeSA - g_cgv.myCharacterInfo->curChargeSA;

			
			if(m_maxCount <= 0)
			{
				m_count = 0;
				m_maxCount =0;
				g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 401 ));
				return;
			}

			
			
			int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
			
			

			int idx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;
			
			
			tempValue1 = (int)GetEpackRechargePrice_SA( g_cgv.myCharacterInfo->onceRecoverySA,
														m_chargeCost,
														(float)g_itemTable[idx].level );
			if( tempValue1 > 0 ) tempValue2 = g_cgv.myCharacterInfo->curChargeSE / tempValue1;
			if( tempValue2 < m_maxCount ) m_maxCount = tempValue2;
		}

		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 539 ) );
		m_textCtl[IT_STACK_WIN_INPUT_U]->SetText( szTemp );
		
		if( m_chargeType == 0 ) 
		{
			
			
			int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
			
			

			int idx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;
			
			
			tempValue1 = (int)GetEpackRechargePrice_RA( g_cgv.myCharacterInfo->onceRecoveryRA,
														m_chargeCost,
														(float)g_itemTable[idx].level );
		}
		else
		{
			
			
			int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
			
			

			int idx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;
			
			
			tempValue1 = (int)GetEpackRechargePrice_SA( g_cgv.myCharacterInfo->onceRecoverySA,
														m_chargeCost,
														(float)g_itemTable[idx].level );
		}
		strcpy( szTemp, g_ifMng->GetNumberString( tempValue1  ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
	}
	else if( m_stackType == IF_STACK_TYPE_TRADE_SE )
	{
		m_count = invenPos;
		m_maxCount = g_cgv.myCharacterInfo->curChargeSE;

		sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
		m_textCtl[IT_STACK_WIN_INPUT_U]->SetText( szTemp );

		strcpy( szTemp, g_ifMng->GetNumberString( m_count ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
	}
	

	else if( m_stackType == IF_STACK_TYPE_BATTING_TRADE )
	{
		m_count = invenPos;
		m_maxCount = g_cgv.myCharacterInfo->curChargeSE;

		sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
		m_textCtl[IT_STACK_WIN_INPUT_U]->SetText( szTemp );

		strcpy( szTemp, g_ifMng->GetNumberString( m_count ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
	}
	
	
	
	else if( m_stackType == IF_STACK_TYPE_DEPOT_SAVE_SE )
	{
		m_count = 0;
		m_maxCount = g_cgv.myCharacterInfo->curChargeSE;
		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
		m_textCtl[IT_STACK_WIN_INPUT_U]->SetText( szTemp );

		strcpy( szTemp, g_ifMng->GetNumberString( m_count ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
	}
	else if( m_stackType == IF_STACK_TYPE_DEPOT_LOAD_SE )
	{
		m_count = 0;
		m_maxCount = g_cgv.myCharacterInfo->depotSE;
		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
		m_textCtl[IT_STACK_WIN_INPUT_U]->SetText( szTemp );

		strcpy( szTemp, g_ifMng->GetNumberString( m_count ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
	}
	
	else if( m_stackType == IF_STACK_TYPE_SELL_BOOTH_PRICE )
	{
		m_count = 0;
		m_maxCount = 999999999;
		m_saveInvenPos = invenPos;

		sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
		m_textCtl[IT_STACK_WIN_INPUT_U]->SetText( szTemp );
		strcpy( szTemp, g_ifMng->GetNumberString( m_count ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
	}

	else if( m_stackType == IF_STACK_TYPE_MAILBOX_SEND)
	{
		m_count = 0;
		m_maxCount = g_cgv.myCharacterInfo->curChargeSE;
		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
		m_textCtl[IT_STACK_WIN_INPUT_U]->SetText( szTemp );
		
		strcpy( szTemp, g_ifMng->GetNumberString( m_count ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
	}


	
	else if( m_stackType == IF_STACK_TYPE_HELPERSYSTEM_MYPOINT){
		
		m_count = 0;
		m_maxCount = g_HelperSystem.m_MyPointInfo.iMyPoint;
		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
		m_textCtl[IT_STACK_WIN_INPUT_U]->SetText( szTemp );
		
		strcpy( szTemp, g_ifMng->GetNumberString( m_count ) );
		m_textCtl[IT_STACK_WIN_INPUT_NUM_U]->SetText( szTemp );
	}
	Enable();
	g_ifMng->SetFocus( this );
}












CIFMenuWin::CIFMenuWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	m_charBtn = new CIFButtonCtl( this );
	m_itemBtn = new CIFButtonCtl( this );
	m_skillBtn = new CIFButtonCtl( this );
	m_questBtn = new CIFButtonCtl( this );
	m_communityBtn = new CIFButtonCtl( this );
	m_guildBtn = new CIFButtonCtl( this );
	m_optionBtn = new CIFButtonCtl( this );

	Enable( true );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_MENU_WINDOW_TYPE );
}

CIFMenuWin::~CIFMenuWin()
{

}

void CIFMenuWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_SYSTEM_WIN );

	m_charBtn->PrepareControl( IF_SYSTEM_WIN_CHAR_BTN_0 );
	m_itemBtn->PrepareControl( IF_SYSTEM_WIN_ITEM_BTN_0 );
	m_skillBtn->PrepareControl( IF_SYSTEM_WIN_SKILL_BTN_0 );
	m_questBtn->PrepareControl( IF_SYSTEM_WIN_QUEST_BTN_0 );
	m_communityBtn->PrepareControl( IF_SYSTEM_WIN_COMMUNITY_BTN_0 );
	m_guildBtn->PrepareControl( IF_SYSTEM_WIN_GUILD_BTN_0 );
	m_optionBtn->PrepareControl( IF_SYSTEM_WIN_OPTION_BTN_0 );
}

int CIFMenuWin::WindowFrame()
{
	if( !State() ) return( IF_CTL_NONE );
	
	return IF_CTL_NONE;
}

void CIFMenuWin::Render()
{
	if( !State() ) return;
	
	g_ifMng->SetRenderState();
	CIFControl::Render();

	m_charBtn->Render();
	m_itemBtn->Render();
	m_skillBtn->Render();
	m_questBtn->Render();
	m_communityBtn->Render();
	m_guildBtn->Render();
	m_optionBtn->Render();
}

int CIFMenuWin::WindowSelection()
{
	return IF_CTL_NONE;
}

int CIFMenuWin::WindowProcess()
{
	if( m_charBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( g_ifMng->m_charWin->ToggleState() ) g_ifMng->SetFocus( g_ifMng->m_charWin );
		return( IF_CTL_SELECT );
	}
	else if( m_charBtn->Selection() == IF_IN_POS )
	{
		g_ifMng->InitRolloverText();
		g_ifMng->AddRolloverText( g_LPACK.GetMassage( 0, 310 ) );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
	}


	if( m_itemBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		
	
	
		if(g_GonryunBattlePractic.m_ibattleMode != PCB_NONE)
		{
			return IF_CTL_NONE;	
		}
		
	
		
		
		if( g_ifMng->m_itemWin->ToggleState() ) g_ifMng->SetFocus( g_ifMng->m_itemWin );
		return( IF_CTL_SELECT );
	}
	else if( m_itemBtn->Selection() == IF_IN_POS )
	{
		g_ifMng->InitRolloverText();
		g_ifMng->AddRolloverText( g_LPACK.GetMassage( 0, 311 ) );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
	}

	if( m_skillBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( g_ifMng->m_skillWin->ToggleState() ) g_ifMng->SetFocus( g_ifMng->m_skillWin );
		return( IF_CTL_SELECT );
	}
	else if( m_skillBtn->Selection() == IF_IN_POS )
	{
		g_ifMng->InitRolloverText();
		g_ifMng->AddRolloverText( g_LPACK.GetMassage( 0, 312 ) );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
	}


	if( m_questBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( g_ifMng->m_questWin->ToggleState() ) g_ifMng->SetFocus( g_ifMng->m_questWin );
		return( IF_CTL_SELECT );
	}
	else if( m_questBtn->Selection() == IF_IN_POS )
	{
		g_ifMng->InitRolloverText();
		g_ifMng->AddRolloverText( g_LPACK.GetMassage( 0, 313 ) );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
	}
	
	
	if( m_communityBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		if( g_ifMng->m_communityWin->ToggleState() ) g_ifMng->SetFocus( g_ifMng->m_communityWin );

		return( IF_CTL_SELECT );
	}
	else if( m_communityBtn->Selection() == IF_IN_POS )
	{
		g_ifMng->InitRolloverText();
		g_ifMng->AddRolloverText( g_LPACK.GetMessage(0,728) );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
	}


	
	if( m_guildBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( g_ifMng->m_guildWin->ToggleState() ) g_ifMng->SetFocus( g_ifMng->m_guildWin );
		return( IF_CTL_SELECT );
	}
	else if( m_guildBtn->Selection() == IF_IN_POS )
	{
		g_ifMng->InitRolloverText();
		g_ifMng->AddRolloverText( g_LPACK.GetMessage( 0, 845 ) );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
	}


	if( m_optionBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( g_ifMng->m_optionWin->ToggleState() ) g_ifMng->SetFocus( g_ifMng->m_optionWin );
		return( IF_CTL_SELECT );
	}
	else if( m_optionBtn->Selection() == IF_IN_POS )
	{
		g_ifMng->InitRolloverText();
		g_ifMng->AddRolloverText( g_LPACK.GetMassage( 0, 314 ) );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
	}

	return IF_CTL_NONE;
}












CIFMiniMapWin::CIFMiniMapWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_MINI_MAP_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	
	m_titleBar = new CIFControl( this );
	m_closeBtn = new CIFButtonCtl( this );
	m_zoomInBtn = new CIFButtonCtl( this );
	m_zoomOutBtn = new CIFButtonCtl( this );
	m_mapOpenBtn = new CIFButtonCtl( this );

	
	m_myImg = new CIFControl( this );
	for( i = 0; i < MAX_CHAR_A_SCENE; i ++ )
	{
		m_entityImg[i] = new CIFControl( this );
		m_entityImg[i]->Enable( false );
	}

	m_miniMapVexbuf = NULL;

	Enable( true );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_MINI_MAP_WINDOW_TYPE );
	m_realMapSize = 2000.0f;
}

CIFMiniMapWin::~CIFMiniMapWin()
{
	
}
	
void CIFMiniMapWin::PrepareBuffer()
{
	CIFControl::PrepareBuffer();

	
	m_miniMapVexbuf = new CD3DBuffer();
	m_miniMapVexbuf->Create( 6, GTH_FORMAT_XYZRHW |  GTH_FORMAT_DIFFUSE | GTH_FORMAT_TEXVERTEX(1),
		GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY );
}

void CIFMiniMapWin::Release()
{
	CIFControl::Release();

	if(	m_miniMapVexbuf	) m_miniMapVexbuf->Release();
	GTH_SAFE_DELETE( m_miniMapVexbuf );
}

void CIFMiniMapWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_MAP_WIN_MINI );

	m_textCtl[IT_MAP_WIN_MINI_TITLE]->PrepareControl( IF_MAP_WIN_MINI_TITLE_TEXT );
	m_textCtl[IT_MAP_WIN_MINI_MAP_POS]->PrepareControl( IF_MAP_WIN_MINI_MAP_POS );
	m_textCtl[IT_MAP_WIN_POSITION]->PrepareControl( IF_MAP_WIN_POSITION_TEXT );
	
	m_titleBar->PrepareControl( IF_MAP_WIN_MINI_TITLE_BAR );
	m_closeBtn->PrepareControl( IF_MAP_WIN_MINI_CLOSE_BTN_0 );
	m_zoomInBtn->PrepareControl( IF_MAP_WIN_ZOOMIN_BTN_0 );
	m_zoomOutBtn->PrepareControl( IF_MAP_WIN_ZOOMOUT_BTN_0 );
	m_mapOpenBtn->PrepareControl( IF_MAP_WIN_MAP_BTN_0 );

	
	m_myImg->PrepareControl( IF_MAP_WIN_MY_IMAGE );
	for( int i = 0; i < MAX_CHAR_A_SCENE; i ++ )
	{
		m_entityImg[i]->PrepareControl( IF_MAP_WIN_MONSTER_IMAGE );
	}
}

void CIFMiniMapWin::UpdateBuffer()
{
	CIFControl::UpdateBuffer();

	
	float px, py, xsize, ysize;
	IFTEXTUREVERTEX *pVertices = NULL;

	
	float mapwidth	= g_pApp->m_worldBSPs[0]->m_models->bbox[3] - g_pApp->m_worldBSPs[0]->m_models->bbox[0];
	float mapheight	= g_pApp->m_worldBSPs[0]->m_models->bbox[4] - g_pApp->m_worldBSPs[0]->m_models->bbox[1];
	float mx = g_pApp->m_myCharacter->position[0] - g_pApp->m_worldBSPs[0]->m_models->bbox[0];		
	float my = g_pApp->m_myCharacter->position[1] - g_pApp->m_worldBSPs[0]->m_models->bbox[1];
	float centerx = mx * 512.0f / mapwidth;
	float centery = my * 512.0f / mapheight;
	float realMapXSize = ( m_realMapSize / mapwidth ) * 512.0f;
	float realMapYSize = ( m_realMapSize / mapheight ) * 512.0f;
	float x = centerx - realMapXSize * 0.5f;
	float y = centery - realMapYSize * 0.5f;

	y = 512.0f - y - realMapYSize;
	
	float tx1, ty1, tx2, ty2;
	tx1 = x / 512.0f;
	ty1 = y / 512.0f;
	tx2 = ( x + realMapXSize ) / 512.0f;
	ty2 = ( y + realMapYSize ) / 512.0f;

	px = (float)m_textCtl[IT_MAP_WIN_MINI_MAP_POS]->GetXPos();
	py = (float)m_textCtl[IT_MAP_WIN_MINI_MAP_POS]->GetYPos();
	xsize = (float)m_textCtl[IT_MAP_WIN_MINI_MAP_POS]->GetXSize();
	ysize = (float)m_textCtl[IT_MAP_WIN_MINI_MAP_POS]->GetYSize();

	pVertices = NULL;
	pVertices = (IFTEXTUREVERTEX *)m_miniMapVexbuf->Lock();
	
	pVertices[0].position = D3DXVECTOR4( px - 0.5f, py - 0.5f, 0.0f, 1.0f );
	pVertices[0].color = 0xffffffff;
	pVertices[0].tu = tx1;
	pVertices[0].tv = ty1;
				
	pVertices[1].position = D3DXVECTOR4( px + xsize - 0.5f, py - 0.5f, 0.0f, 1.0f );			
	pVertices[1].color = 0xffffffff;
	pVertices[1].tu = tx2;
	pVertices[1].tv = ty1;
				
	pVertices[2].position = D3DXVECTOR4( px + xsize - 0.5f, py + ysize - 0.5f, 0.0f, 1.0f );			
	pVertices[2].color = 0xffffffff;
	pVertices[2].tu = tx2;
	pVertices[2].tv = ty2;
				
	pVertices[3].position = D3DXVECTOR4( px - 0.5f, py + ysize - 0.5f, 0.0f, 1.0f );	
	pVertices[3].color = 0xffffffff;
	pVertices[3].tu = tx1;
	pVertices[3].tv = ty2;

	m_miniMapVexbuf->Unlock();

	
	
	
	Fx_CHARACTER_t *character = &g_charManager->m_Characters[1];
	int n = 0, bPartyMember;
	int numCharacter = g_charManager->m_iNumCharacters;

	m_myImg->ChangeControlTexInfo( IF_MAP_WIN_MY_IMAGE );
	m_myImg->m_info[0].clientX = (int)( px + xsize * 0.5f ) - m_myImg->m_info[0].sizeX / 2;
	m_myImg->m_info[0].clientY = (int)( py + ysize * 0.5f ) - m_myImg->m_info[0].sizeY / 2;
	m_myImg->UpdateRotateBuffer( 90.0f - g_pApp->m_myCharacter->angles[YAW] );

	for( int i = 0; i < MAX_CHAR_A_SCENE; i ++ )
	{
		m_entityImg[i]->Enable( false );
	}
	
	for( i = 1; i < numCharacter; i ++, character ++ )
	{
		bPartyMember = false;

		mx = character->position[0] - g_pApp->m_myCharacter->position[0];
		my = character->position[1] - g_pApp->m_myCharacter->position[1];

		mx = mx * xsize / m_realMapSize;
		my = my * ysize / m_realMapSize;
		my = -my;

		if( character->entityType == ENTITY_MONSTER ) m_entityImg[n]->ChangeControlTexInfo( IF_MAP_WIN_MONSTER_IMAGE );
		else if( character->entityType == ENTITY_NPC ) m_entityImg[n]->ChangeControlTexInfo( IF_MAP_WIN_NPC_IMAGE );
		else if( character->entityType == ENTITY_PC ) 
		{
			
			
			if( !GTH_RenderGM( character ) ) continue;

			m_entityImg[n]->ChangeControlTexInfo( IF_MAP_WIN_OTHER_PC_IMAGE );
			if( g_cgv.myCharacterInfo->organizeServer >= 0 )
			{
				for( int j = 0; j < MAX_NUMBER_OF_PARTY_MEMBER; j ++ )
				{
					if( g_cgv.myCharacterInfo->member[j].serverIndex < 0 ) continue;
					if( !stricmp( character->name, g_cgv.myCharacterInfo->member[j].memberName ) ) 
						m_entityImg[n]->ChangeControlTexInfo( IF_MAP_WIN_PARTY_IMAGE );

					bPartyMember = true;
				}
			}
		}

		m_entityImg[n]->m_info[0].clientX = (int)( px + xsize * 0.5f + mx ) - m_entityImg[n]->m_info[0].sizeX / 2;
		m_entityImg[n]->m_info[0].clientY = (int)( py + ysize * 0.5f + my ) - m_entityImg[n]->m_info[0].sizeY / 2;

		if( m_entityImg[n]->m_info[0].clientX <= (int)px )
		{
			if( bPartyMember ) m_entityImg[n]->m_info[0].clientX = px;
			else continue;
		}
		if( m_entityImg[n]->m_info[0].clientX + m_entityImg[n]->m_info[0].sizeX >= (int)( px + xsize ) ) 
		{
			if( bPartyMember ) m_entityImg[n]->m_info[0].clientX = px + xsize - m_entityImg[n]->m_info[0].sizeX;
			else continue;
		}
		if( m_entityImg[n]->m_info[0].clientY <= (int)py ) 
		{
			if( bPartyMember ) m_entityImg[n]->m_info[0].clientY = py;
			else continue;
		}
		if( m_entityImg[n]->m_info[0].clientY + m_entityImg[n]->m_info[0].sizeY >= (int)( py + ysize ) )
		{
			if( bPartyMember ) m_entityImg[n]->m_info[0].clientY = py + ysize - m_entityImg[n]->m_info[0].sizeY;
			else continue;
		}

		m_entityImg[n]->UpdateBuffer();
		m_entityImg[n]->Enable();

		n ++;
	}
}

int CIFMiniMapWin::WindowFrame()
{
	char szTemp[256];
	int mx, my;
	
	m_textCtl[IT_MAP_WIN_MINI_TITLE]->SetText( g_cgv.worldTable[g_pApp->m_myCharacter->worldIdx].hanName );
	m_textCtl[IT_MAP_WIN_MINI_TITLE]->SetColor( 0xffffffff );
	
	mx = g_pApp->m_myCharacter->position[0] - g_pApp->m_worldBSPs[0]->m_models->bbox[0];
	my = g_pApp->m_myCharacter->position[1] - g_pApp->m_worldBSPs[0]->m_models->bbox[1];

	sprintf( szTemp, "(%d,%d)", mx / 100, my / 100 );
	m_textCtl[IT_MAP_WIN_POSITION]->SetText( szTemp );
	m_textCtl[IT_MAP_WIN_POSITION]->SetColor( 0xffffffff );
	
	
	m_mapOpenBtn->SetButtonText( g_LPACK.GetMassage( 0, 607 ) );

	return IF_CTL_NONE;
}

void CIFMiniMapWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();
	
	m_titleBar->Render();
	m_closeBtn->Render();
	
	if( !MinimizeState() )
	{
		m_zoomInBtn->Render();
		m_zoomOutBtn->Render();
		m_mapOpenBtn->RenderTextBtn();



		m_pd3dDevice->SetTexture( 0, m_texMng->GetD3DTexture( g_ifMng->m_mapWin->m_mapTextureID ) );
		m_pd3dDevice->SetStreamSource( 0, m_miniMapVexbuf->GetD3DVertexBuffer(), sizeof( IFTEXTUREVERTEX ) );
		m_pd3dDevice->SetVertexShader( D3DFVF_IFVERTEX );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );
		
		for( int i = 0; i < MAX_CHAR_A_SCENE; i ++ )
		{
			m_entityImg[i]->Render();
		}
		m_myImg->Render();
	}
	

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		
		m_textCtl[IT_MAP_WIN_MINI_TITLE]->Render();
		m_textCtl[IT_MAP_WIN_POSITION]->Render( IF_TEXT_OUTPUT_RIGHT );
	}
	g_chatManager->EndFrame();
}

int CIFMiniMapWin::WindowSelection()
{
	int ret;
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );
	return( IF_CTL_NONE );
}

int CIFMiniMapWin::WindowProcess()
{
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		ToggleMinimize();
		return IF_CTL_SELECT;
	}
	if( !MinimizeState() )
	{
		if( m_zoomOutBtn->ProcessButton() == IF_CTL_SELECT )
		{
			m_realMapSize += 500.0f;
			if( m_realMapSize >= 4000.0f ) m_realMapSize = 4000.0f;
		}
		if( m_zoomInBtn->ProcessButton() == IF_CTL_SELECT )
		{
			m_realMapSize -= 500.0f;
			if( m_realMapSize <= 2000.0f ) m_realMapSize = 2000.0f;
		}
		if( m_mapOpenBtn->ProcessButton() == IF_CTL_SELECT )
		{
			g_ifMng->m_mapWin->ToggleState();
		}

	}
	return IF_CTL_NONE;
}

int CIFMiniMapWin::Selection()
{
	int x1, y1, x2, y2;
	int mx, my;

	if( MinimizeState() )
	{
		return( m_titleBar->Selection() );
	}
	
	x1 = m_info[m_current].clientX;
	y1 = m_info[m_current].clientY;
	x2 = m_info[m_current].clientX + m_info[m_current].sizeX;
	y2 = m_info[m_current].clientY + m_info[m_current].sizeY;
	
	mx = g_input->GetPosX();
	my = g_input->GetPosY();

	if( x1 <= mx && x2 >= mx && y1 <= my && y2 >= my )
	{
		if( g_input->GetLBDown() ) return( IF_LBDN );
		else if( g_input->GetRBDown() ) return( IF_RBDN );
		else if( g_input->GetLBUp() ) return( IF_LBUP );
		else if( g_input->GetRBUp() ) return( IF_RBUP );
		else if( g_input->GetLBKeep() ) return( IF_LBKEEP );
		else if( g_input->GetRBKeep() ) return( IF_RBKEEP );
		else return( IF_IN_POS );
	}
	
	return( IF_NOT_IN_POS );
}








#define		IF_MAP_XSIZE			512
#define		IF_MAP_YSIZE			512
#define		IF_MINI_MAP_XSIZE		90
#define		IF_MINI_MAP_YSIZE		120

#define		IF_MAP_TEXTURE_SIZE		512

#define		IF_MAP_NPCICON_ZOOM_SIZE	1.5f

CIFMapWin::CIFMapWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_MAP_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	for( i = 0; i < MAX_NUMBER_OF_MAP_NPCICON; i ++ )
	{
		m_npcImg[i] = new CIFControl( this );
		m_npcImg[i]->Enable( false );
		memset( &m_npcInfo[i], 0, sizeof( mapNpcIconInfo_t ) );
	}
	m_numNpcInfo = 0;
	m_npcZoomType = 0;
	m_npcZoomSize = 1.0f;
	
	
	m_titleBar = new CIFControl( this );
	m_closeBtn = new CIFButtonCtl( this );
	m_myImg = new CIFControl( this );

	m_mapVexbuf = NULL;

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_MAP_WINDOW_TYPE );
}

CIFMapWin::~CIFMapWin()
{
	
}

void CIFMapWin::AddTextureToManager()
{
	char tempFileName[MAX_PATH];

	if( g_cgv.myCharacterInfo == NULL )
	{
		sprintf( tempFileName, "%s_minimap.dds", "ora" );	
	}
	else
	{
		sprintf( tempFileName, "%s_minimap.dds", g_cgv.worldTable[g_cgv.syncCharacter[0].worldIdx].name );
	}
	m_texMng->Add( tempFileName, OWNER_TYPE_INTERFACE );
}

void CIFMapWin::UpdateTextureID()
{
	char tempFileName[MAX_PATH];

	if( g_cgv.myCharacterInfo == NULL )
	{
		sprintf( tempFileName, "%s_minimap.dds", "ora" );	
	}
	else
	{
		sprintf( tempFileName, "%s_minimap.dds", g_cgv.worldTable[g_cgv.syncCharacter[0].worldIdx].name );
	}

	m_mapTextureID = m_texMng->Search( tempFileName, OWNER_TYPE_INTERFACE );
}

	
void CIFMapWin::PrepareBuffer()
{
	CIFControl::PrepareBuffer();
	
	
	m_mapVexbuf = new CD3DBuffer();
	m_mapVexbuf->Create( 6, GTH_FORMAT_XYZRHW |  GTH_FORMAT_DIFFUSE | GTH_FORMAT_TEXVERTEX(1),
		GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY );
}

void CIFMapWin::Release()
{
	CIFControl::Release();

	if( m_mapVexbuf ) m_mapVexbuf->Release ();
	GTH_SAFE_DELETE( m_mapVexbuf );
}

void CIFMapWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_MAP_WIN );

	m_textCtl[IT_MAP_WIN_TITLE]->PrepareControl( IF_MAP_WIN_TITLE_TEXT );

	m_titleBar->PrepareControl( IF_MAP_WIN_TITLE_BAR );
	m_closeBtn->PrepareControl( IF_MAP_WIN_CLOSE_BTN_0 );
	m_myImg->PrepareControl( IF_MAP_WIN_MY_IMAGE );
}

void CIFMapWin::UpdateBuffer()
{
	CIFControl::UpdateBuffer();
	
	
	float px, py, xsize, ysize;
	DWORD mapAlpha = 0xb0ffffff;

	px = (float)m_info[0].clientX;
	py = (float)m_info[0].clientY;
	xsize = (float)m_info[0].sizeX;
	ysize = (float)m_info[0].sizeY;

	IFTEXTUREVERTEX *pVertices = NULL;

	pVertices = (IFTEXTUREVERTEX *)m_mapVexbuf->Lock();
	
	pVertices[0].position = D3DXVECTOR4( px - 0.5f, py - 0.5f, 0.0f, 1.0f );
	pVertices[0].color = mapAlpha;
	pVertices[0].tu = 0.0f;
	pVertices[0].tv = 0.0f;
				
	pVertices[1].position = D3DXVECTOR4( px + xsize - 0.5f, py - 0.5f, 0.0f, 1.0f );			
	pVertices[1].color = mapAlpha;
	pVertices[1].tu = 1.0f;
	pVertices[1].tv = 0.0f;
				
	pVertices[2].position = D3DXVECTOR4( px + xsize - 0.5f, py + ysize - 0.5f, 0.0f, 1.0f );			
	pVertices[2].color = mapAlpha;
	pVertices[2].tu = 1.0f;
	pVertices[2].tv = 1.0f;
				
	pVertices[3].position = D3DXVECTOR4( px - 0.5f, py + ysize - 0.5f, 0.0f, 1.0f );	
	pVertices[3].color = mapAlpha;
	pVertices[3].tu = 0.0f;
	pVertices[3].tv = 1.0f;

	m_mapVexbuf->Unlock();

	float mapwidth	= g_pApp->m_worldBSPs[0]->m_models->bbox[3] - g_pApp->m_worldBSPs[0]->m_models->bbox[0];
	float mapheight	= g_pApp->m_worldBSPs[0]->m_models->bbox[4] - g_pApp->m_worldBSPs[0]->m_models->bbox[1];
	float mx = g_pApp->m_myCharacter->position[0] - g_pApp->m_worldBSPs[0]->m_models->bbox[0];		
	float my = g_pApp->m_myCharacter->position[1] - g_pApp->m_worldBSPs[0]->m_models->bbox[1];
	float centerx = mx * xsize / mapwidth;
	float centery = my * ysize / mapheight;
	float rxsize, rysize;

	centery = ysize - centery;

	m_myImg->m_info[0].clientX = (int)( centerx + px ) - m_myImg->m_info[0].sizeX / 2;
	m_myImg->m_info[0].clientY = (int)( centery + py ) - m_myImg->m_info[0].sizeY / 2;

	m_myImg->UpdateRotateBuffer( 90.0f - g_pApp->m_myCharacter->angles[YAW] );

	
	
	for( int i = 0; i < MAX_NUMBER_OF_MAP_NPCICON; i ++ )
	{
		m_npcImg[i]->Enable( false );
	}

	int npcIconType;

	if( !m_npcZoomType ) 
	{
		m_npcZoomSize += ( IF_MAP_NPCICON_ZOOM_SIZE - 1.0f ) * g_timer.GetElapsedMilliTime() * 0.001f;
		if( m_npcZoomSize > IF_MAP_NPCICON_ZOOM_SIZE )
		{
			m_npcZoomSize = IF_MAP_NPCICON_ZOOM_SIZE;
			m_npcZoomType = 1;
		}
	}
	else 
	{
		m_npcZoomSize -= ( IF_MAP_NPCICON_ZOOM_SIZE - 1.0f ) * g_timer.GetElapsedMilliTime() * 0.001f;
		if( m_npcZoomSize < 1.0f )
		{
			m_npcZoomSize = 1.0f;
			m_npcZoomType = 0;
		}
	}

	for( i = 0; i < m_numNpcInfo; i ++ )
	{
		if( m_npcInfo[i].iconType == 0 ) npcIconType = g_monsterTable[m_npcInfo[i].tableIdx].mapIconType;
		else npcIconType = 9;

		m_npcImg[i]->ChangeControlTexInfo( IF_MAP_WIN_NPCICON_NORMAL + npcIconType );

		
		
		if(g_monsterTable[m_npcInfo[i].tableIdx].idx == 222)
			m_npcImg[i]->ChangeControlTexInfo(IF_MAP_WIN_NPCICON_TREE);	
		

		
		mx = m_npcInfo[i].position[0] - g_pApp->m_worldBSPs[0]->m_models->bbox[0];
		my = m_npcInfo[i].position[1] - g_pApp->m_worldBSPs[0]->m_models->bbox[1];
		centerx = mx * xsize / mapwidth;
		centery = my * ysize / mapheight;
		centery = ysize - centery;

		rxsize = (int)( (float)m_npcZoomSize * m_npcImg[i]->m_info[0].sizeX );
		rysize = (int)( (float)m_npcZoomSize * m_npcImg[i]->m_info[0].sizeY );

		m_npcImg[i]->m_info[0].clientX = (int)( centerx + px ) - rxsize / 2;
		m_npcImg[i]->m_info[0].clientY = (int)( centery + py ) - rysize / 2;

		m_npcImg[i]->UpdateBuffer( rxsize, rysize );
		m_npcImg[i]->Enable();
	}
}

int CIFMapWin::WindowFrame()
{
	
	m_textCtl[IT_MAP_WIN_TITLE]->SetText( g_cgv.worldTable[g_pApp->m_myCharacter->worldIdx].hanName );
	m_textCtl[IT_MAP_WIN_TITLE]->SetColor( 0xffffffff );
	
	return IF_CTL_NONE;
}

void CIFMapWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	m_titleBar->Render();
	m_closeBtn->Render();
	
	m_pd3dDevice->SetTexture( 0, m_texMng->GetD3DTexture( m_mapTextureID ) );
	m_pd3dDevice->SetStreamSource( 0, m_mapVexbuf->GetD3DVertexBuffer(), sizeof( IFTEXTUREVERTEX ) );
	m_pd3dDevice->SetVertexShader( D3DFVF_IFVERTEX );
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );

	for( int i = 0; i < MAX_NUMBER_OF_MAP_NPCICON; i ++ )
	{
		m_npcImg[i]->Render();
	}

	m_myImg->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		
		m_textCtl[IT_MAP_WIN_TITLE]->Render();
	}
	g_chatManager->EndFrame();
}

int CIFMapWin::WindowSelection()
{
	int ret;

	if( !State() ) return( IF_CTL_NONE );

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) 
	{
		int iii=0;
		return( IF_CTL_BEGIN_MOVING );
	}
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );
	
	return( IF_CTL_NONE );
}

int CIFMapWin::WindowProcess()
{
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		Enable( false );
		return IF_CTL_SELECT;
	}
	
	for( int i = 0; i < m_numNpcInfo; i ++ )
	{
		if( m_npcImg[i]->Selection() == IF_NOT_IN_POS ) continue;

		if( m_npcInfo[i].iconType == 0 )
		{
			int monsterTableIdx = m_npcInfo[i].tableIdx;
			g_ifMng->InitRolloverText();
			g_ifMng->AddRolloverText( g_monsterTable[monsterTableIdx].name, 5 );
			g_ifMng->AddRolloverText( g_monsterTable[monsterTableIdx].document );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
		}
		else
		{
			int worldIdx = m_npcInfo[i].linkedWorldIdx;

			g_ifMng->InitRolloverText();
			g_ifMng->AddRolloverText( g_LPACK.GetMassage( 0, 681 ), 5 );
			g_ifMng->AddRolloverText( g_cgv.worldTable[worldIdx].hanName );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
		}
	}
	return IF_CTL_NONE;
}








#define MAX_COUNT_GM_WINDOW_MENU		7

enum
{
	GM_MENU_TYPE_WHISPER	=	0,
	GM_MENU_TYPE_MOVE,
	GM_MENU_TYPE_SUMMON,
	GM_MENU_TYPE_FORCED_SUMMON,
	GM_MENU_TYPE_ADD_NAME,
	GM_MENU_TYPE_DEL_NAME,
	GM_MENU_TYPE_FORCED_QUIT,
};

int g_gmWinMenuLPackIndex[] = 
{
	13,				
	10,				
	11,				
	12,				
	568,			
	14,				
	569				
};

#define MAX_COUNT_GM_WINDOW_GOLRYUN		14

int g_gmWinGolryunLPackIndex[] = 
{
	201,				
	206,				
	463,				
	382,				
	422,				
	202,				
	421,				
	203,				
	204,				
	384,				
	208,				
	465,				
	539,				
	540,				
};


CIFGmWin::CIFGmWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_GM_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	m_selectBar = new CIFControl( this );
	m_nameSelectBar = new CIFControl( this );

	
	m_closeBtn = new CIFButtonCtl( this );
	m_prevBtn = new CIFButtonCtl( this );
	m_nextBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_GM_WINDOW_TYPE );

	m_numCaller = 0;
	m_nameSelectedIdx = -1;
	m_selectedIdx = -1;

	m_golryunSelectedIdx = -1;
}

CIFGmWin::~CIFGmWin()
{

}

void CIFGmWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_GM_WIN );

	
	m_textCtl[IT_GM_WIN_TITLE]->PrepareControl( IF_GM_WIN_TITLE_TEXT );
	m_textCtl[IT_GM_WIN_NAME_U]->PrepareControl( IF_GM_WIN_NAME_TEXT_U );
	m_textCtl[IT_GM_WIN_U]->PrepareControl( IF_GM_WIN_TEXT_U );
	m_textCtl[IT_GM_WIN_SELECT_U]->PrepareControl( IF_GM_WIN_SELECT_TEXT_U );
	m_textCtl[IT_GM_WIN_GOLRYUN_U]->PrepareControl( IF_GM_WIN_TEXT_U );
	
	
	m_titleBar->PrepareControl( IF_GM_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_GM_WIN_TITLE_BAR_BOTTOM );
	m_selectBar->PrepareControl( IF_GM_WIN_SELECT_BAR );
	m_nameSelectBar->PrepareControl( IF_GM_WIN_NAME_SELECT_BAR );

	
	m_closeBtn->PrepareControl( IF_GM_WIN_CLOSE_BTN_0 );
	m_prevBtn->PrepareControl( IF_GM_WIN_PREV_BTN_0 );
	m_nextBtn->PrepareControl( IF_GM_WIN_NEXT_BTN_0 );

	m_selectBarSavePos = g_ifMng->m_iPos[IF_GM_WIN_SELECT_BAR].clientY;
	m_nameSelectBarSavePos = g_ifMng->m_iPos[IF_GM_WIN_NAME_SELECT_BAR].clientY;
}

int CIFGmWin::WindowFrame()
{
	char szTemp[256];
	char tempStr[128];

	if( g_pApp->m_myCharacter->stat_alive ) strcpy( tempStr, g_LPACK.GetMassage( 0, 17 ) );
	else strcpy( tempStr, g_LPACK.GetMassage( 0, 16 ) );
	
	sprintf( szTemp, "GM(%s)", tempStr );
	m_textCtl[IT_GM_WIN_TITLE]->SetText( szTemp );

	if( !g_cgv.isGmBusy ) strcpy( tempStr, g_LPACK.GetMassage( 0, 18 ) );
	else strcpy( tempStr, g_LPACK.GetMassage( 0, 19 ) );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 491 ), tempStr );
	m_textCtl[IT_GM_WIN_NAME_U]->SetText( szTemp );

	return( IF_CTL_NONE );
}

void CIFGmWin::Render()
{
	if( !State() ) return;


	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	
	m_titleBar->Render();
	m_titleBarBottom->Render();



	
	m_closeBtn->Render();
	m_prevBtn->Render();
	m_nextBtn->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->SetReverseSize( -1 );
	g_chatManager->BeginFrame();
	{
		int cx, cy, yy;
		m_textCtl[IT_GM_WIN_TITLE]->Render();
		m_textCtl[IT_GM_WIN_NAME_U]->Render();

		
		for( int i = 0; i < MAX_COUNT_GM_WINDOW_MENU; i ++ )
		{
			cx = m_textCtl[IT_GM_WIN_SELECT_U]->GetXPos();
			cy = m_textCtl[IT_GM_WIN_SELECT_U]->GetYPos();

			if( i >= 5 ) 
			{
				cx += m_textCtl[IT_GM_WIN_SELECT_U]->GetXSize() / 2;
				yy = i - 5;
			}
			else
			{
				yy = i;
			}

			g_chatManager->SetColor( 0xffffffff );
			if( m_selectedIdx == i ) g_chatManager->Output( cx, cy + yy * 13, g_LPACK.GetMassage( 0, g_gmWinMenuLPackIndex[i] ), true );
			else g_chatManager->Output( cx, cy + yy * 13, g_LPACK.GetMassage( 0, g_gmWinMenuLPackIndex[i] ), false );
		}

		
		for( i = 0; i < m_numCaller; i ++ )
		{
			cx = m_textCtl[IT_GM_WIN_U]->GetXPos();
			cy = m_textCtl[IT_GM_WIN_U]->GetYPos();
			g_chatManager->SetColor( 0xff00ffff );
			if( m_nameSelectedIdx == i ) g_chatManager->Output( cx, cy + i * 13, g_cgv.callerName[i], true );
			else g_chatManager->Output( cx, cy + i * 13, g_cgv.callerName[i], false );
		}

		
		for( i = 0; i < MAX_COUNT_GM_WINDOW_GOLRYUN; i ++ )
		{
			cx = m_textCtl[IT_GM_WIN_GOLRYUN_U]->GetXPos()+100;
			cy = m_textCtl[IT_GM_WIN_GOLRYUN_U]->GetYPos();
			g_chatManager->SetColor( D3DCOLOR_ARGB( 255, 0, 255, 0 ) );
			if( m_golryunSelectedIdx == i ) g_chatManager->Output( cx, cy + i * 13, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, g_gmWinGolryunLPackIndex[i] ), true );
			else g_chatManager->Output( cx, cy + i * 13, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, g_gmWinGolryunLPackIndex[i] ), false );
		}		
	}
	g_chatManager->EndFrame();
}

int CIFGmWin::WindowSelection()
{
	int ret;
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFGmWin::WindowProcess()
{
	int mx, my;
	int x1, y1, xs, ys;
	int selectIdx;
	char szTemp[256];
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT ) Enable( false );
	
	















	
	mx = g_input->GetPosX();
	my = g_input->GetPosY();

	x1 = m_textCtl[IT_GM_WIN_U]->GetXPos();
	y1 = m_textCtl[IT_GM_WIN_U]->GetYPos();

	xs = m_textCtl[IT_GM_WIN_U]->GetXSize();
	ys = m_numCaller * 13;

	mx = mx - x1;
	my = my - y1;
	if( mx < 0 || my < 0 || mx > xs || my > ys ) 
	{
		selectIdx = -1;
	}
	else
	{
		selectIdx = my / 13;
	}

	if( selectIdx >= 0 && selectIdx < m_numCaller )
	{
		if( g_input->GetLBUp() ) 
		{
			m_nameSelectedIdx = selectIdx;
		}
		else
		{
			if( g_cgv.callerName[selectIdx][0] != 0 )
			{
				g_ifMng->InitRolloverText();
				g_ifMng->AddRolloverText( g_cgv.callerReason[selectIdx] );
				g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
			}
		}
	}

	
	mx = g_input->GetPosX();
	my = g_input->GetPosY();

	x1 = m_textCtl[IT_GM_WIN_SELECT_U]->GetXPos();
	y1 = m_textCtl[IT_GM_WIN_SELECT_U]->GetYPos();

	xs = m_textCtl[IT_GM_WIN_SELECT_U]->GetXSize();
	ys = 5 * 13;

	mx = mx - x1;
	my = my - y1;
	if( mx < 0 || my < 0 || mx > xs || my > ys ) 
	{
		selectIdx = -1;
	}
	else
	{
		selectIdx = my / 13;
		selectIdx += ( mx / ( xs / 2 ) ) * 5;
	}

	if( g_input->GetLBUp() ) 
	{
		if( selectIdx >= 0 && selectIdx < MAX_COUNT_GM_WINDOW_MENU )
		{
			m_selectedIdx = selectIdx;
			switch( m_selectedIdx )
			{
			case GM_MENU_TYPE_WHISPER :
				ProcessGM();
				break;
			case GM_MENU_TYPE_ADD_NAME :
				ProcessGM();
				break;
			default :
				if( m_nameSelectedIdx >= 0 )
				{
				
#if defined _GTH_CHINESE_VERSION || _GTH_TAIWAN_VERSION	|| (_GTH_MALAYSIA_CHN_VERSION ) 
					sprintf( szTemp, g_LPACK.GetMassage( 0, 15 ), g_LPACK.GetMassage( 0, g_gmWinMenuLPackIndex[m_selectedIdx] ), 
						g_cgv.callerName[m_nameSelectedIdx]  );
#else
					sprintf( szTemp, g_LPACK.GetMassage( 0, 15 ), g_cgv.callerName[m_nameSelectedIdx],
						g_LPACK.GetMassage( 0, g_gmWinMenuLPackIndex[m_selectedIdx] ) );
#endif
					g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSGAE_GM_OK );
				}
				break;
			}
		}
	}


	
	mx = g_input->GetPosX();
	my = g_input->GetPosY();

	x1 = m_textCtl[IT_GM_WIN_GOLRYUN_U]->GetXPos()+100;
	y1 = m_textCtl[IT_GM_WIN_GOLRYUN_U]->GetYPos();

	xs = m_textCtl[IT_GM_WIN_GOLRYUN_U]->GetXSize();
	ys = MAX_COUNT_GM_WINDOW_GOLRYUN * 13;

	mx = mx - x1;
	my = my - y1;
	if( mx < 0 || my < 0 || mx > xs || my > ys ) 
	{
		selectIdx = -1;
	}
	else
	{
		selectIdx = my / 13;
	}

	if( selectIdx >= 0 && selectIdx < MAX_COUNT_GM_WINDOW_GOLRYUN )
	{
		if( g_input->GetLBUp() ) 
		{
			m_golryunSelectedIdx = selectIdx;
			ProcessGMGolryunCommand();
		}

	}

	return IF_CTL_NONE;
}

void CIFGmWin::AddCaller( char* name, char* reason )
{
	if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
	int i;
	for( i = 0; i < MAX_NUMBER_OF_GM_CALLER_NAME; i ++ )
	{
		if( !stricmp( name, g_cgv.callerName[i] ) ) return;
	}
	for( i = 0; i < MAX_NUMBER_OF_GM_CALLER_NAME; i ++ )
	{
		if( g_cgv.callerName[i][0] == 0 ) break;
	}
	if( i == MAX_NUMBER_OF_GM_CALLER_NAME ) return;

	strcpy( g_cgv.callerName[i], name );
	strcpy( g_cgv.callerReason[i], reason );

	m_numCaller ++;
}

void CIFGmWin::DelCaller( char* name )
{
	if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
	int i, delIdx;
	for( i = 0; i < MAX_NUMBER_OF_GM_CALLER_NAME; i ++ )
	{
		if( !stricmp( name, g_cgv.callerName[i] ) ) break;
	}
	if( i == MAX_NUMBER_OF_GM_CALLER_NAME ) return;
	
	delIdx = i;	
	for( i = delIdx; i < m_numCaller - 1; i ++ )
	{
		strcpy( g_cgv.callerName[i], g_cgv.callerName[i + 1] );
		strcpy( g_cgv.callerReason[i], g_cgv.callerReason[i + 1] );
	}
	memset( g_cgv.callerName[m_numCaller - 1], 0, sizeof( chatMessage ) );
	memset( g_cgv.callerReason[m_numCaller - 1], 0, sizeof( chatMessage ) );
	m_numCaller --;

	m_nameSelectedIdx = -1;
}

void CIFGmWin::ProcessGM()
{

	
	switch( m_selectedIdx )
	{
		
	case GM_MENU_TYPE_MOVE :
		{
			if( m_nameSelectedIdx >= 0 )
			{
				if( g_cgv.callerName[m_nameSelectedIdx][0] == 0 ) break;
				GTH_SendMessage_SystemAdimin_GoToPlayer( g_cgv.callerName[m_nameSelectedIdx] );
			}
		}
		break;
		
	case GM_MENU_TYPE_SUMMON :
		{
			if( m_nameSelectedIdx >= 0 )
			{
				if( g_cgv.callerName[m_nameSelectedIdx][0] == 0 ) break;
				GTH_SendMessage_RequestSummons( g_cgv.callerName[m_nameSelectedIdx], 0 );
			}
		}
		break;
		
	case GM_MENU_TYPE_FORCED_SUMMON :
		{
			if( m_nameSelectedIdx >= 0 )
			{
				if( g_cgv.callerName[m_nameSelectedIdx][0] == 0 ) break;
				GTH_SendMessage_RequestSummons( g_cgv.callerName[m_nameSelectedIdx], 1 );
			}
		}
		break;
		
	case GM_MENU_TYPE_WHISPER :
		{
			if( m_nameSelectedIdx >= 0 )
			{
				if( g_cgv.callerName[m_nameSelectedIdx][0] == 0 ) break;
				g_ifMng->m_dlgWin->m_curSendType = IF_CHATMSG_TYPE_WHISPER;
				strcpy( g_cgv.whisperName, g_cgv.callerName[m_nameSelectedIdx] );
				g_ifMng->m_dlgWin->SetWhisper( g_cgv.whisperName );
			}
		}
		break;
		
	case GM_MENU_TYPE_DEL_NAME :
		{
			if( m_nameSelectedIdx >= 0 )
			{
				if( g_cgv.callerName[m_nameSelectedIdx][0] == 0 ) break;
				DelCaller( g_cgv.callerName[m_nameSelectedIdx] );
			}
		}
		break;
		
	case GM_MENU_TYPE_ADD_NAME :
		{
			if( g_cgv.pTargetChar != NULL )
			{
				if( g_cgv.pTargetChar->entityType == ENTITY_PC )
				{
					AddCaller( g_cgv.pTargetChar->name, g_LPACK.GetMassage( 0, 571 ) );
				}
			}
		}
		break;
		
	case GM_MENU_TYPE_FORCED_QUIT :
		{
			if( m_nameSelectedIdx >= 0 )
			{
				if( g_cgv.callerName[m_nameSelectedIdx][0] == 0 ) break;
				GTH_SendChatMessage_Whisper( g_cgv.callerName[m_nameSelectedIdx], g_LPACK.GetMassage( 0, 441 ) );
			}
		}
		break;
	}
	m_selectedIdx = -1;
}

void CIFGmWin::ProcessGMGolryunCommand()
{
	char szTemp[255];
	
	switch( m_golryunSelectedIdx )
	{
		
	case GM_GolryunCommand::start_battle :
		{
			sprintf( szTemp, "/%s ", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,201));
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
			g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
		}
		break;
		
	case GM_GolryunCommand::start_join :
		{
			sprintf( szTemp, "/%s ", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,206));
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
			g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
		}
		break;
		
	case GM_GolryunCommand::start_recall_fighter :
		{
			sprintf( szTemp, "/%s ", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,463));
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
			g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
		}
		break;
		
	case GM_GolryunCommand::check_join_num :
		{
			sprintf( szTemp, "/%s ", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,382));
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
			g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
		}
		break;
		
	case GM_GolryunCommand::close_join :
		{
			sprintf( szTemp, "/%s ", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,422));
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
			g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
		}
		break;
		
	case GM_GolryunCommand::start_betting :
		{
			sprintf( szTemp, "/%s ", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,202));
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
			g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
		}
		break;
		
	case GM_GolryunCommand::check_betting_nak :
		{
			sprintf( szTemp, "/%s ", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,421));
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
			g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
		}
		break;
		
	case GM_GolryunCommand::ready_tournament :
		{
			sprintf( szTemp, "/%s ", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,203));
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
			g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
		}
		break;
		
	case GM_GolryunCommand::start_tournament :
		{
			sprintf( szTemp, "/%s ", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,204));
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
			g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
		}
		break;
		
	case GM_GolryunCommand::cancel_tournament :
		{
			sprintf( szTemp, "/%s ", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,384));
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
			g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
		}
		break;
		
	case GM_GolryunCommand::close_battle :
		{
			sprintf( szTemp, "/%s ", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,208));
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
			g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
		}
		break;
		
	case GM_GolryunCommand::force_join_fighter :
		{
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );

			if( m_nameSelectedIdx >= 0 )
			{
				if( g_cgv.callerName[m_nameSelectedIdx][0] == 0 ) break;

				sprintf( szTemp, "/%s %s", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,465), g_cgv.callerName[m_nameSelectedIdx]);
				g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
			}
		}
		break;
	
	case GM_GolryunCommand::force_eject :
		{
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );

			if( m_nameSelectedIdx >= 0 )
			{
				if( g_cgv.callerName[m_nameSelectedIdx][0] == 0 ) break;

				sprintf( szTemp, "/%s %s", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,539), g_cgv.callerName[m_nameSelectedIdx]);
				g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
			}
		}
		break;
		
	case GM_GolryunCommand::check_status_pvpMode :
		{
			sprintf( szTemp, "/%s ", g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,540));
			g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
			g_chatManager->AddInputStr( INPUT_INDEX_DIALOGBOX, szTemp );
		}
		break;
	}

}












CIFHelpWin::CIFHelpWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_HELP_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );

	
	m_closeBtn = new CIFButtonCtl( this );
	m_prevBtn = new CIFButtonCtl( this );
	m_nextBtn = new CIFButtonCtl( this );
	m_homeBtn = new CIFButtonCtl( this );
	m_scrollBar = new CIFScrollBar( this );
	m_textHyperLink = new CIFHyperLink( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_HELP_WINDOW_TYPE );

	m_helpIndex = -1;
	m_prevIndex = -1;
	m_nextIndex = -1;
	
}

CIFHelpWin::~CIFHelpWin()
{

}
	
void CIFHelpWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_HELP_WIN );

	
	m_textCtl[IT_HELP_WIN_TITLE]->PrepareControl( IF_HELP_WIN_TITLE_TEXT );
	
	
	m_titleBar->PrepareControl( IF_HELP_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_HELP_WIN_TITLE_BAR_BOTTOM );

	
	m_closeBtn->PrepareControl( IF_HELP_WIN_CLOSE_BTN_0 );

	m_homeBtn->PrepareControl( IF_HELP_WIN_HOME_BTN_0 );
	m_prevBtn->PrepareControl( IF_HELP_WIN_PREV_BTN_0 );
	m_nextBtn->PrepareControl( IF_HELP_WIN_NEXT_BTN_0 );

	m_scrollBar->PrepareControl( IF_HELP_WIN_SCROLL );

	m_textCtl[IT_HELP_WIN_HOME_BUTTON]->PrepareControl( IF_HELP_WIN_HOME_BUTTON_TEXT );
	m_textCtl[IT_HELP_WIN_PREV_BUTTON]->PrepareControl( IF_HELP_WIN_PREV_BUTTON_TEXT );
	m_textCtl[IT_HELP_WIN_NEXT_BUTTON]->PrepareControl( IF_HELP_WIN_NEXT_BUTTON_TEXT );

	m_textCtl[IT_HELP_WIN_HELP]->PrepareControl( IF_HELP_WIN_HELP_TEXT );

	m_textHyperLink->PrepareControl( IF_HELP_WIN_HELP_TEXT );

	
	m_helpIndex = 0;
	m_textHyperLink->SetText( g_LPACK.GetMassage( LPACK_TYPE_HELP, m_helpIndex ) );

	m_scrollBar->SetBarPosition( m_textHyperLink->m_maxLineCount - m_textHyperLink->m_viewLineCount, 0 );
}

int CIFHelpWin::WindowFrame()
{
	char szTemp[256];


	sprintf( szTemp, g_LPACK.GetMassage( 0, 567 ) );
	m_textCtl[IT_HELP_WIN_TITLE]->SetText( szTemp );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 684 ) );
	m_homeBtn->SetButtonText( szTemp, 0xffffffff );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 685 ) );
	m_prevBtn->SetButtonText( szTemp, 0xffffffff );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 686 ) );
	m_nextBtn->SetButtonText( szTemp, 0xffffffff );

	
	

	
	

	
	

	

	return( IF_CTL_NONE );
}

void CIFHelpWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	
	m_titleBar->Render();
	m_titleBarBottom->Render();

	
	m_closeBtn->Render();
	m_homeBtn->RenderTextBtn();
	m_prevBtn->RenderTextBtn();
	m_nextBtn->RenderTextBtn();

	m_scrollBar->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_HELP_WIN_TITLE]->Render();
		
		
		
		

		if( m_helpIndex >= 0 )
		{
			m_textHyperLink->Render();
			
			
			
			
			
			
			
		}
	}
	g_chatManager->EndFrame();
}

int CIFHelpWin::WindowSelection()
{
	int ret;
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFHelpWin::WindowProcess()
{
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		Enable( false );
		return IF_CTL_NONE;
	}

	if( m_homeBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		m_prevIndex = m_helpIndex;

		m_helpIndex = 0;
		m_textHyperLink->SetText( g_LPACK.GetMassage( LPACK_TYPE_HELP, m_helpIndex ) );

		m_scrollBar->SetBarPosition( m_textHyperLink->m_maxLineCount - m_textHyperLink->m_viewLineCount, 0 );

		return IF_CTL_NONE;
	}
	if( m_prevBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		if( m_prevIndex != -1 )
		{
			m_nextIndex = m_helpIndex;
			m_helpIndex = m_prevIndex;
			m_prevIndex = -1;
			m_textHyperLink->SetText( g_LPACK.GetMassage( LPACK_TYPE_HELP, m_helpIndex ) );

			m_scrollBar->SetBarPosition( m_textHyperLink->m_maxLineCount - m_textHyperLink->m_viewLineCount, 0 );
		}

		return IF_CTL_NONE;
	}
	if( m_nextBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		if( m_nextIndex != -1 )
		{
			m_prevIndex = m_helpIndex;
			m_helpIndex = m_nextIndex;
			m_nextIndex = -1;
			m_textHyperLink->SetText( g_LPACK.GetMassage( LPACK_TYPE_HELP, m_helpIndex ) );

			m_scrollBar->SetBarPosition( m_textHyperLink->m_maxLineCount - m_textHyperLink->m_viewLineCount, 0 );
		}

		return IF_CTL_NONE;
	}

	if( m_scrollBar->ProcessScrollBar() == IF_CTL_SELECT )
	{
		m_textHyperLink->m_startLine = m_scrollBar->m_current;
		return IF_CTL_NONE;
	}

	int linkIndex = m_textHyperLink->PointInLink();
	if( linkIndex != -1 )
	{
		m_prevIndex = m_helpIndex;
		m_helpIndex = linkIndex;
		m_nextIndex = -1;
		m_textHyperLink->SetText( g_LPACK.GetMassage( LPACK_TYPE_HELP, m_helpIndex ) );

		m_scrollBar->SetBarPosition( m_textHyperLink->m_maxLineCount - m_textHyperLink->m_viewLineCount, 0 );
		return IF_CTL_NONE;
	}

	return IF_CTL_NONE;
}















CIFTipWin::CIFTipWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_TIP_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	
	m_titleBar = new CIFControl( this );
	m_closeBtn = new CIFButtonCtl( this );
	m_nextBtn = new CIFButtonCtl( this );

	Enable( true );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_TIP_WINDOW_TYPE );

	
	m_tipIdx = -1;
	m_curTick = 0;
	m_winOriginYPos = 0;
	m_winRelativeYPos = -32; 

	
	m_bTip = true;

	memset( m_tipStr, 0, sizeof(m_tipStr) );
}

CIFTipWin::~CIFTipWin()
{

}

void CIFTipWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_TIP_WIN );

	
	
	m_winOriginYPos = g_ifMng->m_iPos[ m_info[0].ID ].clientY;


	
	m_maxtip = g_LPACK.GetMessageCount(LPACK_TYPE_TIP);

	m_textCtl[IT_TIP_WIN_TITLE]->PrepareControl( IF_TIP_WIN_TITLE_TEXT );
	m_textCtl[IT_TIP_WIN_TIP]->PrepareControl( IF_TIP_WIN_TIP_TEXT );

	m_titleBar->PrepareControl( IF_TIP_WIN_TITLE_BAR );
	m_closeBtn->PrepareControl( IF_TIP_WIN_CLOSE_BTN_0 );
	m_nextBtn->PrepareControl( IF_TIP_WIN_NEXT_BTN_0 );
}

int CIFTipWin::WindowFrame()
{
	
	char szTemp[256];

	sprintf( szTemp, g_LPACK.GetMassage( 0, 683 ) );

	m_textCtl[IT_TIP_WIN_TITLE]->SetText( szTemp );
	m_textCtl[IT_TIP_WIN_TITLE]->SetColor( 0xffffffff );

	
	

	

	if( m_bTip )
	{
		
		float fCurTime = g_timer.GetAbsoluteTime();

		if( m_winRelativeYPos > -32 )
		{
			m_winRelativeYPos = m_info[0].sizeY - ( fCurTime - m_curTick ) * 150 - 32;
		}
		else
		{
			m_winRelativeYPos = -32;
		}

		g_ifMng->m_iPos[ m_info[0].ID ].clientY = m_winOriginYPos + m_winRelativeYPos;

		
		if( m_curTick+30.0f < fCurTime )
		{
			Enable( false );
		}
	}

	return IF_CTL_NONE;
}

void CIFTipWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();
	
	m_titleBar->Render();
	m_closeBtn->Render();
	m_nextBtn->Render();
	
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		
		m_textCtl[IT_TIP_WIN_TITLE]->Render();
		if( m_tipIdx >= 0 )
		{
			int x, y, xs;
			x = m_textCtl[IT_TIP_WIN_TIP]->GetXPos();
			y = m_textCtl[IT_TIP_WIN_TIP]->GetYPos();
			xs = m_textCtl[IT_TIP_WIN_TIP]->GetXSize();
			
			g_chatManager->SetColor( 0xffffffff );
			
			g_chatManager->DrawText( x, y, m_tipStr, xs, 13 );
		}
	}
	g_chatManager->EndFrame();
}

void CIFTipWin::SetTipState(bool bTip)
{
	m_bTip = bTip;

	if( !m_bTip )
		Enable(false);
}

int CIFTipWin::WindowSelection()
{
	int ret;
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_NONE );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_NONE );
	return( IF_CTL_NONE );
}

int CIFTipWin::WindowProcess()
{
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_NONE;
	}

	if( m_nextBtn->ProcessButton() == IF_CTL_SELECT )
	{
		m_curTick = g_timer.GetAbsoluteTime();

		if( m_maxtip > 0 )
		{
			if( m_tipIdx+1 < m_maxtip)
				m_tipIdx++;
			else
				m_tipIdx = 0;
			
			sprintf( m_tipStr, g_LPACK.GetMassage( LPACK_TYPE_TIP, m_tipIdx ) );


			int size = m_textCtl[IT_TIP_WIN_TIP]->GetXSize();

			int templen = g_chatManager->GetChatStrSize( m_tipStr );

			int lineCount = ( templen + 12 ) / size;
			if( templen + 12 >= size )
			{
			}
		}
		else
		{
			m_tipIdx = -1;
		}

		m_winRelativeYPos = -32;
		g_ifMng->m_iPos[ m_info[0].ID ].clientY = m_winOriginYPos + m_winRelativeYPos;

		UpdateControl();

		return IF_CTL_NONE;
	}





















	return IF_CTL_NONE;
}

int CIFTipWin::Selection()
{
	int x1, y1, x2, y2;
	int mx, my;

	x1 = m_info[m_current].clientX;
	y1 = m_info[m_current].clientY;
	x2 = m_info[m_current].clientX + m_info[m_current].sizeX;
	y2 = m_info[m_current].clientY + m_info[m_current].sizeY;
	
	mx = g_input->GetPosX();
	my = g_input->GetPosY();

	if( x1 <= mx && x2 >= mx && y1 <= my && y2 >= my )
	{
		if( g_input->GetLBDown() ) return( IF_LBDN );
		else if( g_input->GetRBDown() ) return( IF_RBDN );
		else if( g_input->GetLBUp() ) return( IF_LBUP );
		else if( g_input->GetRBUp() ) return( IF_RBUP );
		else if( g_input->GetLBKeep() ) return( IF_LBKEEP );
		else if( g_input->GetRBKeep() ) return( IF_RBKEEP );
		else return( IF_IN_POS );
	}
	
	return( IF_NOT_IN_POS );
}

void CIFTipWin::Open()
{
	m_curTick = g_timer.GetAbsoluteTime();

	if( m_maxtip > 0 )
	{
		if( m_tipIdx+1 < m_maxtip)
			m_tipIdx++;
		else
			m_tipIdx = 0;
		
		sprintf( m_tipStr, g_LPACK.GetMassage( LPACK_TYPE_TIP, m_tipIdx ) );


		int size = m_textCtl[IT_TIP_WIN_TIP]->GetXSize();

		int templen = g_chatManager->GetChatStrSize( m_tipStr );

		int lineCount = ( templen + 12 ) / size;
		if( templen + 12 >= size )
		{
		}
	}
	else
	{
		m_tipIdx = -1;
	}

	m_winRelativeYPos = m_info[0].sizeY - 32;
	g_ifMng->m_iPos[ m_info[0].ID ].clientY = m_winOriginYPos + m_winRelativeYPos;

	UpdateControl();
}

bool CIFTipWin::CanOpen()
{
	float fCurTime = g_timer.GetAbsoluteTime();

	if( !m_bTip )
		return false;

	if( m_enable )
		return false;

	
	if( m_curTick+180.0f < fCurTime )
	{
		return true;
	}

	return false;
}









CIFAutoQuestWin::CIFAutoQuestWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{

	for( int i = 0; i < NUM_AUTO_QUEST_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );


	m_wideUp = new CIFControl( this );
	m_wideDown = new CIFControl( this );

	m_startBtn = new CIFButtonCtl( this );



	Enable( false );
	WideEnable( false );
	ScrollEnable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_AUTOQUEST_WINDOW_TYPE );

	m_curTick = 0.0f;
	m_totalTime = 0.0f;
	
	m_arrEffIdx = 0;
	m_questIdx = 0;

	m_fWideUpPosY = 0.0f;
	m_fWideDownPosY = 0.0f;
	m_fAngle = 0.0f;
	m_eType = AUTO_WIDE_START;
	m_eMsgtype = AUTO_WIDE_START;
	
	m_iautoflag=-1;
	m_bComplete = FALSE;

}

CIFAutoQuestWin::~CIFAutoQuestWin()
{

}

void CIFAutoQuestWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_AUTOQUEST_SCROLL_WIN );

	m_textCtl[AUTO_QUEST_SCROLL_TITLE]->PrepareControl( IF_AUTOQUEST_WIN_TITLE_TEXT );
	m_textCtl[AUTO_QUEST_SCROLL_STORY]->PrepareControl( IF_AUTOQUEST_WIN_STORY );
	m_textCtl[AUTO_QUEST_SCROLL_MESSAGE]->PrepareControl( IF_AUTOQUEST_WIN_MESSAGE );

	
	g_ifMng->m_iPos[IF_AUTOQUEST_WIDEUP].sizeX = g_ifMng->m_clientWidth;
	g_ifMng->m_iPos[IF_AUTOQUEST_WIDEDOWN].sizeX = g_ifMng->m_clientWidth;
	g_ifMng->m_iPos[IF_AUTOQUEST_WIDEUP].clientY = -100;
	g_ifMng->m_iPos[IF_AUTOQUEST_WIDEDOWN].clientY = g_ifMng->m_clientHeight;

	m_wideUp->PrepareControl( IF_AUTOQUEST_WIDEUP );
	m_wideDown->PrepareControl( IF_AUTOQUEST_WIDEDOWN );

	m_startBtn->PrepareControl( IF_AUTOQUEST_WIN_START_BTN0 );


}

int CIFAutoQuestWin::WindowFrame()
{
	if( !State() ) return 0;
	

	float fCurTime = g_timer.GetAbsoluteTime();
	float fTime = fCurTime - m_curTick;

	switch( m_eType )
	{
		case AUTO_WIDE_START:		
		{

			g_ifMng->m_iPos[IF_AUTOQUEST_WIDEUP].clientY = g_ifMng->m_iPos[IF_AUTOQUEST_WIDEUP].clientY + fTime;
			g_ifMng->m_iPos[IF_AUTOQUEST_WIDEDOWN].clientY = g_ifMng->m_iPos[IF_AUTOQUEST_WIDEDOWN].clientY - fTime; 

			m_fWideUpPosY = g_ifMng->m_iPos[IF_AUTOQUEST_WIDEUP].clientY;
			m_fWideDownPosY = g_ifMng->m_iPos[IF_AUTOQUEST_WIDEDOWN].clientY;

			g_input->MouseEnable(FALSE);
			g_input->KeyEnable(FALSE);
			g_cgv.enableChat = FALSE;

			WideEnable(true);
			if(m_fWideUpPosY > 0 && m_fWideDownPosY < g_ifMng->m_clientHeight-100)
			{
				m_eType = m_eMsgtype;

				g_ifMng->m_iPos[IF_AUTOQUEST_WIDEUP].clientY = 0;
				g_ifMng->m_iPos[IF_AUTOQUEST_WIDEDOWN].clientY = g_ifMng->m_clientHeight-100;

			}
		}
		break;

		case AUTO_SCROLL_ORDER:		
		{
			
			g_input->MouseEnable(FALSE);
			
			g_cgv.enableChat = FALSE;
			m_arrEffIdx = g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
							50, 
							g_pApp->m_myCharacter->position,
							NULL, 
							true, 
							g_pApp->m_myCharacter->entityType, 
							g_pApp->m_myCharacter->idxOnServer,
							0.0f, 
							0.0f, 
							0.0f, 
							-1.0f,
							false );

			m_eType = AUTO_EFFECT_PROCESS;
			
			if( TRUE != m_bComplete ){
				char szTemp[255];
			
				sprintf(szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 545), g_autoquestscrollTable[m_questIdx].name);
				m_textCtl[AUTO_QUEST_SCROLL_MESSAGE]->SetText( szTemp );
				m_textCtl[AUTO_QUEST_SCROLL_MESSAGE]->m_color = 0xffffffff; 
				
				m_textCtl[AUTO_QUEST_SCROLL_MESSAGE]->Enable();
			}
			
		}
		break;

		case AUTO_SCROLL_START:		
		{	
			ScrollEnable( true );

			m_textCtl[AUTO_QUEST_SCROLL_TITLE]->SetText( g_autoquestscrollTable[m_questIdx].name );
			m_textCtl[AUTO_QUEST_SCROLL_TITLE]->m_color = 0xffffff00; 

			float px = m_textCtl[AUTO_QUEST_SCROLL_STORY]->m_info.clientX;
			float py = m_textCtl[AUTO_QUEST_SCROLL_STORY]->m_info.clientY;	
			
			int lpack = g_autoquestscrollTable[m_questIdx].dataField[8];	

			g_chatManager->SetColor(0xffffffff);
			g_chatManager->DrawText( px, py, g_LPACK.GetMassage( 10, lpack), 400, 13 );	
			
			g_input->MouseEnable(TRUE);	
			g_input->KeyEnable(TRUE);
			
			if( m_startBtn->ProcessButton() == IF_CTL_SELECT ) 
			{
				ScrollEnable(FALSE);
				m_eType = AUTO_WIDE_END;
				GTH_SendMessage_RequestDestroyItem( g_cgv.saveInventoryIdx );
				m_curTick = g_timer.GetAbsoluteTime();
			}

		
			g_input->MouseEnable(FALSE);	
			g_input->KeyEnable(FALSE);
	
		}
		break;

		case AUTO_SCROLL_FAIL:		
		{
			m_eType = AUTO_WIDE_END;
		}
		break;

		
		case AUTO_SCROLL_COMPLETE:		
		{
			m_curTick = g_timer.GetAbsoluteTime();
			m_fAngle = 0.0f;
			m_eType = AUTO_SCROLL_ORDER;
			m_bComplete = TRUE;
		
		}
		
		break;

		case AUTO_EFFECT_PROCESS:		
		{

			Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;	

			if( g_camera.m_angles[ YAW ] <  0.0f )     g_camera.m_angles[ YAW ] += 360.0f;
			if( g_camera.m_angles[ YAW ] >  360.0f )   g_camera.m_angles[ YAW ] -= 360.0f;

			m_fAngle += fTime;

			if( m_fAngle < 90.0f ) 
				g_camera.m_angles[ YAW ] += fTime;	

			if(!g_effectMng->m_entityInUse[ m_arrEffIdx ])
			{
				
				if( TRUE == m_bComplete )
					m_eType = AUTO_SCROLL_ENDMESSAGE;
				
				else
					m_eType = AUTO_WIDE_END;
				
				
				m_curTick = g_timer.GetAbsoluteTime();	
			}
		}
		break;

		
		case AUTO_EFFECT_END:		
		{
		
			m_totalTime++;
			if(m_totalTime > 150.0f)
			{

				m_totalTime = 0.0f;
				m_eType = AUTO_WIDE_END;
				m_curTick = g_timer.GetAbsoluteTime();	
			}
		}
		break;
		

		case AUTO_WIDE_END:		
		{			
		
			

			g_ifMng->m_iPos[IF_AUTOQUEST_WIDEUP].clientY = g_ifMng->m_iPos[IF_AUTOQUEST_WIDEUP].clientY - fTime;
			g_ifMng->m_iPos[IF_AUTOQUEST_WIDEDOWN].clientY = g_ifMng->m_iPos[IF_AUTOQUEST_WIDEDOWN].clientY + fTime; 

			m_fWideUpPosY = g_ifMng->m_iPos[IF_AUTOQUEST_WIDEUP].clientY;
			m_fWideDownPosY = g_ifMng->m_iPos[IF_AUTOQUEST_WIDEDOWN].clientY;
			
			if(m_fWideUpPosY < -100 && m_fWideDownPosY > g_ifMng->m_clientHeight)
			{
				m_textCtl[AUTO_QUEST_SCROLL_MESSAGE]->Enable(false);

				m_eType = AUTO_SCROLL_END;	

				g_ifMng->m_iPos[IF_AUTOQUEST_WIDEUP].clientY = -100;
				g_ifMng->m_iPos[IF_AUTOQUEST_WIDEDOWN].clientY = g_ifMng->m_clientHeight;

			}
		}
		break;

		
		case	AUTO_SCROLL_ENDMESSAGE:
			{
				ScrollEnable( true );
				
				m_textCtl[AUTO_QUEST_SCROLL_TITLE]->SetText( g_autoquestscrollTable[m_questIdx].name );
				m_textCtl[AUTO_QUEST_SCROLL_TITLE]->m_color = 0xffffff00; 
				
				float px = m_textCtl[AUTO_QUEST_SCROLL_STORY]->m_info.clientX;
				float py = m_textCtl[AUTO_QUEST_SCROLL_STORY]->m_info.clientY;	
				
				int lpack = g_autoquestscrollTable[m_questIdx].data.endLpack;
			
				g_chatManager->SetColor(0xffffffff);
				g_chatManager->DrawText( px, py, g_LPACK.GetMassage( 10, lpack), 400, 13 );	
				
				
				g_input->MouseEnable(TRUE);	
				g_input->KeyEnable(TRUE);
				
				if( m_startBtn->ProcessButton() == IF_CTL_SELECT ) 
				{
					ScrollEnable( FALSE );
					m_eType = AUTO_WIDE_END;
					m_curTick = g_timer.GetAbsoluteTime();	
				}
				
				g_input->MouseEnable(FALSE);	
				g_input->KeyEnable(FALSE);
			}
			break;
		
		case AUTO_SCROLL_END:		
		{

			Enable( FALSE );
			WideEnable(FALSE);
			ScrollEnable( FALSE );

			g_input->MouseEnable(TRUE);
			g_input->KeyEnable(TRUE);
			g_cgv.enableChat = TRUE;
			m_eType = AUTO_WIDE_START;

			for( int i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
			{
				if(g_ifMng->m_winControl[i]->BeforeState())
				{
					g_ifMng->m_winControl[i]->SetBeforeState(false);
					g_ifMng->m_winControl[i]->Enable(true);
				}		
			}

			
			m_iautoflag = -1;
			m_bComplete = FALSE;
		
			GTH_SendMessage_AutoQuestScroll_Progress(AUTOQUESTSCROLL_END);
		}
		break;
	}

	return IF_CTL_NONE;
}

int CIFAutoQuestWin::WindowProcess()
{
	
	return IF_CTL_NONE;
}

void CIFAutoQuestWin::WideRender()
{

	if( !WideState() ) return;

	g_ifMng->SetRenderState();

	m_wideUp->Render(); 
	m_wideDown->Render();

}

void CIFAutoQuestWin::ScrollRender()
{

	if( !ScrollState() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();

	m_startBtn->Render();

	
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		m_textCtl[AUTO_QUEST_SCROLL_TITLE]->Render();
		m_textCtl[AUTO_QUEST_SCROLL_STORY]->Render();
	}
	g_chatManager->EndFrame();

}

void CIFAutoQuestWin::MessageRender(void)
{

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		m_textCtl[AUTO_QUEST_SCROLL_MESSAGE]->Render();
	}
	g_chatManager->EndFrame();

}

void CIFAutoQuestWin::Render()
{
	if( !State() ) return;

	WideRender();
	ScrollRender();
	MessageRender();
}













CIFCraftHelpWin::CIFCraftHelpWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_CRAFTHELP_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	for( i = 0; i < 32; i ++ )
		m_searchItemBox[i] = new CIFControl( this );

	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	m_searchText = new CIFControl( this );
	
	
	m_MemberShipBar= new CIFControl( this ); 

	
	m_prevBtn = new CIFButtonCtl( this );
	m_nextBtn = new CIFButtonCtl( this );
	m_closeBtn = new CIFButtonCtl( this );
	m_totalViewBtn = new CIFButtonCtl( this );
	m_searchViewBtn = new CIFButtonCtl( this );
	m_searchBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_CRAFTHELP_WINDOW_TYPE );

	m_totalMaxCraftFormula = 0;
	m_totalCurCraftFormula = 0;
	m_searchMaxCraftFormula = 0;
	m_searchCurCraftFormula = 0;
	m_curType = 0;
	memset( m_searchStr, 0, sizeof( m_searchStr ) );
}

CIFCraftHelpWin::~CIFCraftHelpWin()
{

}	

void CIFCraftHelpWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_CRAFTHELP_WIN );

	m_textCtl[IT_CRAFTHELP_WIN_TITLE]->PrepareControl( IF_CRAFTHELP_WIN_TITLE_TEXT );
	m_textCtl[IT_CRAFTHELP_WIN_RESULT_INVEN_0_POS]->PrepareControl( IF_CRAFTHELP_WIN_RESULT_INVEN_0 );
	m_textCtl[IT_CRAFTHELP_WIN_RESULT_INVEN_1_POS]->PrepareControl( IF_CRAFTHELP_WIN_RESULT_INVEN_1 );
	m_textCtl[IT_CRAFTHELP_WIN_SOURCE_INVEN_POS]->PrepareControl( IF_CRAFTHELP_WIN_SOURCE_INVEN_POS );
	m_textCtl[IT_CRAFTHELP_WIN_SEARCH_POS]->PrepareControl( IF_CRAFTHELP_WIN_SEARCH_TEXT_POS );
	
	
	m_textCtl[IF_CRAFTHELP_WIN_MEMBERSHIP_POS]->PrepareControl( IF_CRAFTHELP_WIN_MEMBERSHIP_TEXT_POS );
	m_MemberShipBar->PrepareControl( IF_CRAFTHELP_WIN_MEMBERSHIP_BAR );
	
	
	for( int i = 0; i < 32; i ++ )
		m_searchItemBox[i]->PrepareControl( IF_CRAFTHELP_WIN_SEARCH_ITEM );

	
	m_titleBar->PrepareControl( IF_CRAFTHELP_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_CRAFTHELP_WIN_TITLE_BAR_BOTTOM );
	m_searchText->PrepareControl( IF_CRAFTHELP_WIN_SEARCH_TEXT );
	

	
	m_closeBtn->PrepareControl( IF_CRAFTHELP_WIN_CLOSE_BTN_0 );
	m_prevBtn->PrepareControl( IF_CRAFTHELP_WIN_PREV_BTN_0 );
	m_nextBtn->PrepareControl( IF_CRAFTHELP_WIN_NEXT_BTN_0 );
	m_totalViewBtn->PrepareControl( IF_CRAFTHELP_WIN_TOTALVIEW_BTN_0 );
	m_searchViewBtn->PrepareControl( IF_CRAFTHELP_WIN_SEARCHVIEW_BTN_0 );
	m_searchBtn->PrepareControl( IF_CRAFTHELP_WIN_SEARCH_BTN_0 );
}

int CIFCraftHelpWin::WindowFrame()
{	
	char szTemp[128];
	m_textCtl[IT_CRAFTHELP_WIN_TITLE]->SetText( g_LPACK.GetMassage( 0, 693 ) );
	m_textCtl[IT_CRAFTHELP_WIN_TITLE]->SetColor( 0xffffffff );
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 694 ), m_totalCurCraftFormula + 1, g_itemCraftTableNumber );
	m_totalViewBtn->SetButtonText( szTemp );
	
	if( m_searchMaxCraftFormula > 0 ) sprintf( szTemp, g_LPACK.GetMassage( 0, 695 ), m_searchCurCraftFormula + 1, m_searchMaxCraftFormula );
	else sprintf( szTemp, g_LPACK.GetMassage( 0, 695 ), 0, 0 );

	m_searchViewBtn->SetButtonText( szTemp );

	m_prevBtn->SetButtonText( g_LPACK.GetMassage( 0, 696 ) );
	m_nextBtn->SetButtonText( g_LPACK.GetMassage( 0, 697 ) );
	m_searchBtn->SetButtonText( g_LPACK.GetMassage( 0, 698 ) );

	if( !m_curType ) 
	{
		m_totalViewBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		m_searchViewBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	}
	else
	{
		m_totalViewBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_searchViewBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}

	
	int cx, cy;
	cx = m_textCtl[IT_CRAFTHELP_WIN_SEARCH_POS]->GetXPos();
	cy = m_textCtl[IT_CRAFTHELP_WIN_SEARCH_POS]->GetYPos();
	g_chatManager->m_ix[3] = cx;
	g_chatManager->m_iy[3] = cy;

	return( IF_CTL_NONE );
}

void CIFCraftHelpWin::Render()
{
	int craftIdx, numSearchItemBox = 0;
	itemCraftTable_t *pCraftTbl = NULL;

	if( !State() ) return;

	WindowFrame();
	g_ifMng->SetRenderState();
	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	m_searchText->Render();

	
	m_closeBtn->Render();
	m_prevBtn->RenderTextBtn();
	m_nextBtn->RenderTextBtn();
	m_totalViewBtn->RenderTextBtn();
	m_searchViewBtn->RenderTextBtn();
	m_searchBtn->RenderTextBtn();

	if( !m_curType ) 
	{
		craftIdx = m_totalCurCraftFormula;
		pCraftTbl = &g_itemCraftTable[craftIdx];
	}
	else
	{
		if( m_searchMaxCraftFormula > 0 ) 
		{
			pCraftTbl = m_sSearchCraftTable[m_searchCurCraftFormula];
		}
	}

	g_ifMng->m_iconCtl->BeginFrame();
	{
		if( pCraftTbl != NULL )
		{
			
			for( int i = 0; i < pCraftTbl->numSourceItem; i ++ )
			{
				int itemTableIdx, cx, cy;
				cx = m_textCtl[IT_CRAFTHELP_WIN_SOURCE_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
				cy = m_textCtl[IT_CRAFTHELP_WIN_SOURCE_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
				itemTableIdx = pCraftTbl->sourceItem[i].itemIndex;
				
				if( CheckMyItem( itemTableIdx, pCraftTbl->sourceItem[i].itemCount ) )
				{
					g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
				}
				else
				{
					g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID, 1.0f, 1.0f, DISALBE_ICON_RED_COLOR );
				}

				if( m_curType )
				{
					if( strstr( m_searchStr, g_itemTable[itemTableIdx].name ) != NULL )
					{
						m_searchItemBox[numSearchItemBox]->m_info[0].clientX = cx;
						m_searchItemBox[numSearchItemBox]->m_info[0].clientY = cy;
						m_searchItemBox[numSearchItemBox]->UpdateBuffer();
						m_searchItemBox[numSearchItemBox]->Enable();
						m_searchItemBox[numSearchItemBox]->Render();

						numSearchItemBox ++;
					}
				}
			}
			for( i = 0; i < pCraftTbl->numResultItem; i ++ )
			{
				int itemTableIdx, cx, cy;
				cx = m_textCtl[IT_CRAFTHELP_WIN_RESULT_INVEN_0_POS + i]->m_info.clientX;
				cy = m_textCtl[IT_CRAFTHELP_WIN_RESULT_INVEN_0_POS + i]->m_info.clientY;
				itemTableIdx = pCraftTbl->resultItem[i].itemIndex;
				
				if( CheckMyItem( itemTableIdx, pCraftTbl->resultItem[i].itemCount ) )
				{
					g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
				}
				else
				{
					g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID, 1.0f, 1.0f, DISALBE_ICON_RED_COLOR );
				}
				if( m_curType )
				{
					if( strstr( m_searchStr, g_itemTable[itemTableIdx].name ) != NULL )
					{
						m_searchItemBox[numSearchItemBox]->m_info[0].clientX = cx;
						m_searchItemBox[numSearchItemBox]->m_info[0].clientY = cy;
						m_searchItemBox[numSearchItemBox]->UpdateBuffer();
						m_searchItemBox[numSearchItemBox]->Enable();
						m_searchItemBox[numSearchItemBox]->Render();

						numSearchItemBox ++;
					}
				}
			}
		}
	}
	g_ifMng->m_iconCtl->EndFrame();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_CRAFTHELP_WIN_TITLE]->Render();
		g_chatManager->RenderInputFrame( 3 );

		
		g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
		g_chatManager->SetReverseSize( -1 );
		g_chatManager->SetColor( 0xffffffff );
		
		if( pCraftTbl != NULL )
		{
			
			if(pCraftTbl->m_nMembership_type > -1)
			{
				
				m_MemberShipBar->Render();
				
				if(pCraftTbl->m_nMembership_type < 1)
				{
					
					g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
					m_textCtl[IF_CRAFTHELP_WIN_MEMBERSHIP_POS]->SetText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 228 ));
					m_textCtl[IF_CRAFTHELP_WIN_MEMBERSHIP_POS]->SetColor( 0xffffffff );

				}
				else
				{
					
					g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
					m_textCtl[IF_CRAFTHELP_WIN_MEMBERSHIP_POS]->SetText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 229 ));
					m_textCtl[IF_CRAFTHELP_WIN_MEMBERSHIP_POS]->SetColor( 0xffffffff );
				}
				m_textCtl[IF_CRAFTHELP_WIN_MEMBERSHIP_POS]->Render(IF_TEXT_OUTPUT_CENTER);
			}

			for( int i = 0; i < pCraftTbl->numSourceItem; i ++ )
			{
				int itemTableIdx, cx, cy, number;
				char szTemp[128];
				
				itemTableIdx = pCraftTbl->sourceItem[i].itemIndex;
				
				if( g_itemTable[itemTableIdx].stackFlag )
				{
					cx = m_textCtl[IT_CRAFTHELP_WIN_SOURCE_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
					cy = m_textCtl[IT_CRAFTHELP_WIN_SOURCE_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
					number = pCraftTbl->sourceItem[i].itemCount;
					strcpy( szTemp, g_ifMng->GetNumberString( number ) );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
			for( i = 0; i < pCraftTbl->numResultItem; i ++ )
			{
				int itemTableIdx, cx, cy, number;
				char szTemp[128];
				
				itemTableIdx = pCraftTbl->resultItem[i].itemIndex;
				
				if( g_itemTable[itemTableIdx].stackFlag )
				{
					cx = m_textCtl[IT_CRAFTHELP_WIN_RESULT_INVEN_0_POS + i]->m_info.clientX;
					cy = m_textCtl[IT_CRAFTHELP_WIN_RESULT_INVEN_0_POS + i]->m_info.clientY;
					number = pCraftTbl->resultItem[i].itemCount;
					strcpy( szTemp, g_ifMng->GetNumberString( number ) );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
	}
	g_chatManager->EndFrame();

	
	if( pCraftTbl != NULL )
	{
		
		for( int i = 0; i < pCraftTbl->numSourceItem; i ++ )
		{
			int itemTableIdx, cx, cy;
			cx = m_textCtl[IT_CRAFTHELP_WIN_SOURCE_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
			cy = m_textCtl[IT_CRAFTHELP_WIN_SOURCE_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
			itemTableIdx = pCraftTbl->sourceItem[i].itemIndex;
						
			if( m_curType )
			{
				if( strstr( g_itemTable[itemTableIdx].name, m_searchStr ) != NULL )
				{
					m_searchItemBox[numSearchItemBox]->m_info[0].clientX = cx;
					m_searchItemBox[numSearchItemBox]->m_info[0].clientY = cy;
					m_searchItemBox[numSearchItemBox]->UpdateBuffer();
					m_searchItemBox[numSearchItemBox]->Enable();
					m_searchItemBox[numSearchItemBox]->Render();
					
					numSearchItemBox ++;
				}
			}
		}
		for( i = 0; i < pCraftTbl->numResultItem; i ++ )
		{
			int itemTableIdx, cx, cy;
			cx = m_textCtl[IT_CRAFTHELP_WIN_RESULT_INVEN_0_POS + i]->m_info.clientX;
			cy = m_textCtl[IT_CRAFTHELP_WIN_RESULT_INVEN_0_POS + i]->m_info.clientY;
			itemTableIdx = pCraftTbl->resultItem[i].itemIndex;			
			if( m_curType )
			{
				if( strstr( g_itemTable[itemTableIdx].name, m_searchStr ) != NULL )
				{
					m_searchItemBox[numSearchItemBox]->m_info[0].clientX = cx;
					m_searchItemBox[numSearchItemBox]->m_info[0].clientY = cy;
					m_searchItemBox[numSearchItemBox]->UpdateBuffer();
					m_searchItemBox[numSearchItemBox]->Enable();
					m_searchItemBox[numSearchItemBox]->Render();
					
					numSearchItemBox ++;
				}
			}
		}
	}
}

int CIFCraftHelpWin::WindowSelection()
{
	int ret;
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFCraftHelpWin::WindowProcess()
{
	item_t item;

	int x, y, pos;
	int mx, my, temp;
	int craftIdx;
	itemCraftTable_t *pCraftTbl = NULL;

	GTH_InitItem( &item );
	
	if( !m_curType ) 
	{
		craftIdx = m_totalCurCraftFormula;
		pCraftTbl = &g_itemCraftTable[craftIdx];
	}
	else
	{
		if( m_searchMaxCraftFormula > 0 ) 
		{
			pCraftTbl = m_sSearchCraftTable[m_searchCurCraftFormula];
		}
	}

	if( m_textCtl[IT_CRAFTHELP_WIN_SEARCH_POS]->Selection() == IF_LBDN )
	{
		g_chatManager->ChangeInputMessage( INPUT_INDEX_CRAFTHELP );
	}

	if( m_nextBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		if( !m_curType )
		{
			m_totalCurCraftFormula ++;
			if( m_totalCurCraftFormula >= g_itemCraftTableNumber )
			{
				m_totalCurCraftFormula = g_itemCraftTableNumber - 1;
			}
		}
		else
		{
			m_searchCurCraftFormula ++;
			if( m_searchCurCraftFormula >= m_searchMaxCraftFormula )
			{
				m_searchCurCraftFormula = m_searchMaxCraftFormula - 1;
			}
		}
	}
	if( m_prevBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		if( !m_curType )
		{
			m_totalCurCraftFormula --;
			if( m_totalCurCraftFormula < 0 ) m_totalCurCraftFormula = 0;
		}
		else
		{
			m_searchCurCraftFormula --;
			if( m_searchCurCraftFormula < 0 ) m_searchCurCraftFormula = 0;
		}
	}

	if( m_searchBtn->ProcessButton() == IF_CTL_SELECT )
	{
		strcpy( m_searchStr, g_chatManager->GetInputStr( 3 ) );
		SearchCraftFormula();
	}
	if( m_totalViewBtn->ProcessButton() == IF_CTL_SELECT )
	{
		m_curType = 0;
	}
	if( m_searchViewBtn->ProcessButton() == IF_CTL_SELECT )
	{
		m_curType = 1;
	}
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
	}

	
	if( pCraftTbl == NULL ) return( IF_CTL_NONE );

	 
	pos = -1;
	if( m_textCtl[IT_CRAFTHELP_WIN_RESULT_INVEN_0_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 0;
	}
	else if( m_textCtl[IT_CRAFTHELP_WIN_RESULT_INVEN_1_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 1;
	}

	if( pos >= 0 )
	{
		MakeCraftItem( &item, pCraftTbl, 1, pos );
		if( g_input->GetLBNone() ) 
		{
			
			if( item.itemTableIdx < 0 ) return( IF_CTL_NONE );
			
			g_ifMng->InitRolloverText();
			g_ifMng->MakeItemRollover( &item );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		}
	}
	
	mx = g_input->GetPosX() - m_textCtl[IT_CRAFT_WIN_SOURCE_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_CRAFT_WIN_SOURCE_INVEN_POS]->GetYPos();

	if( mx <= 0 || my <= 0 ) return( IF_CTL_NONE );
	if( mx >= 34 * 6 ||  my >= 33 * 2 ) return( IF_CTL_NONE );

	temp = mx / 34;
	x = temp % 6;
	temp = my / 33;
	y = temp % 2;

	pos = y * 6 + x;

	
	if( pos < 0 || pos >= 12 )
	{
		return( IF_CTL_NONE );
	}
		
	MakeCraftItem( &item, pCraftTbl, 0, pos );

	
	if( g_input->GetLBNone() ) 
	{
		
		if( item.itemTableIdx < 0 ) return( IF_CTL_NONE );

		g_ifMng->InitRolloverText();
		g_ifMng->MakeItemRollover( &item );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
	}

	return IF_CTL_NONE;
}

void CIFCraftHelpWin::MakeCraftItem( item_t *item, itemCraftTable_t *pCraftTbl, int type, int itemIdx )
{
	
	if( !type )
	{
		if( itemIdx >= pCraftTbl->numSourceItem ) return;
		item->itemTableIdx = pCraftTbl->sourceItem[itemIdx].itemIndex;
		if( g_itemTable[item->itemTableIdx].stackFlag ) item->durability = pCraftTbl->sourceItem[itemIdx].itemCount - 1;
		else item->durability = g_itemTable[item->itemTableIdx].durability;

		strcpy( item->name, g_itemTable[item->itemTableIdx].name );
		item->reqLevel = g_itemTable[item->itemTableIdx].level;
	}
	
	else
	{
		
	

		

		
		
		if( itemIdx >= pCraftTbl->numResultItem ) return;
		item->itemTableIdx = pCraftTbl->resultItem[itemIdx].itemIndex;
		if( g_itemTable[item->itemTableIdx].stackFlag ) item->durability = pCraftTbl->resultItem[itemIdx].itemCount - 1;
		else item->durability = g_itemTable[item->itemTableIdx].durability;

		strcpy( item->name, g_itemTable[item->itemTableIdx].name );
		item->reqLevel = g_itemTable[item->itemTableIdx].level;
	}
}

void CIFCraftHelpWin::SetCraftHelpWin( char *searchStr )
{
	int cx, cy, cs;
	cx = m_textCtl[IT_CRAFTHELP_WIN_SEARCH_POS]->GetXPos();
	cy = m_textCtl[IT_CRAFTHELP_WIN_SEARCH_POS]->GetYPos();
	cs = m_textCtl[IT_CRAFTHELP_WIN_SEARCH_POS]->GetXSize();
	g_chatManager->SetInput( cx, cy, 32, 32, INPUT_INDEX_CRAFTHELP, 1, cs );
	g_chatManager->SetInputState( INPUT_INDEX_CRAFTHELP, 1 );
	g_chatManager->AddInputStr( INPUT_INDEX_CRAFTHELP, searchStr );
	g_chatManager->ChangeInputMessage( INPUT_INDEX_CRAFTHELP );
	strcpy( m_searchStr, searchStr );
}

void CIFCraftHelpWin::SearchCraftFormula()
{
	int i, j, itemIdx;
	itemCraftTable_t *pCraft;

	m_searchCurCraftFormula = 0;
	m_searchMaxCraftFormula = 0;
	m_sSearchCraftTable.clear();

	if( m_searchStr[0] == 0 ) 
	{
		return;	
	}

	for( i = 0; i < g_itemCraftTableNumber; i ++ )
	{
		pCraft = &g_itemCraftTable[i];
		
		for( j = 0; j < pCraft->numSourceItem; j ++ )
		{
			itemIdx = pCraft->sourceItem[j].itemIndex;
			if( strstr( g_itemTable[itemIdx].name, m_searchStr ) != NULL ) 
			{
				m_sSearchCraftTable.push_back( pCraft );
				m_searchMaxCraftFormula ++;
				break;
			}
		}
		if( j != pCraft->numSourceItem ) continue;
		
		for( j = 0; j < pCraft->numResultItem; j ++ )
		{
			itemIdx = pCraft->resultItem[j].itemIndex;
			if( strstr( g_itemTable[itemIdx].name, m_searchStr ) != NULL )
			{
				m_sSearchCraftTable.push_back( pCraft );
				m_searchMaxCraftFormula ++;
				break;
			}
		}
	}

	m_curType = 1;
}

int CIFCraftHelpWin::CheckMyItem( int itemTableIdx, int count )
{
	int i, idx;
	item_t *item;

	for( i = 0; i < MAX_INVENTORY_SIZE; i ++ )
	{
		if( g_cgv.myCharacterInfo->inventory[i] < 0 ) continue;
		idx = g_cgv.myCharacterInfo->inventory[i];
		item = &g_cgv.myCharacterInfo->item[idx];

		if( itemTableIdx == item->itemTableIdx )
		{
			if( g_itemTable[itemTableIdx].stackFlag )
			{
				if( item->durability + 1 < count )
				{
					return( false );
				}
			}

			return( true );
		}	
	}
	
	return( false );
}








CIFTutorialWin::CIFTutorialWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_TUTORIAL_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	m_selectBar = new CIFControl( this );

	
	m_closeBtn = new CIFButtonCtl( this );
	m_prevBtn = new CIFButtonCtl( this );
	m_nextBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_TUTORIAL_WINDOW_TYPE );

	m_tutorialFlag = false;
	m_tipFlag = false;
	
	m_messageStr[0] = 0;

	memset( m_selectStr, 0, sizeof( chatMessage ) * MAX_COUNT_IF_HELP_SELECT_STRING);
}

CIFTutorialWin::~CIFTutorialWin()
{

}

void CIFTutorialWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_TUTORIAL_WIN );

	
	m_textCtl[IT_TUTORIAL_WIN_TITLE]->PrepareControl( IF_TUTORIAL_WIN_TITLE_TEXT );
	m_textCtl[IT_TUTORIAL_WIN_NAME_U]->PrepareControl( IF_TUTORIAL_WIN_NAME_TEXT_U );
	m_textCtl[IT_TUTORIAL_WIN_CHAT_U]->PrepareControl( IF_TUTORIAL_WIN_CHAT_TEXT_U );
	m_textCtl[IT_TUTORIAL_WIN_SELECT_U]->PrepareControl( IF_TUTORIAL_WIN_SELECT_TEXT_U );
	
	
	m_titleBar->PrepareControl( IF_TUTORIAL_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_TUTORIAL_WIN_TITLE_BAR_BOTTOM );
	m_selectBar->PrepareControl( IF_TUTORIAL_WIN_SELECT_BAR );

	
	m_closeBtn->PrepareControl( IF_TUTORIAL_WIN_CLOSE_BTN_0 );
	m_prevBtn->PrepareControl( IF_TUTORIAL_WIN_PREV_BTN_0 );
	m_nextBtn->PrepareControl( IF_TUTORIAL_WIN_NEXT_BTN_0 );

	m_selectBarSavePos = g_ifMng->m_iPos[IF_TUTORIAL_WIN_SELECT_BAR].clientY;
}

int CIFTutorialWin::WindowFrame()
{
	char szTemp[256];

	sprintf( szTemp, g_LPACK.GetMassage( 0, 682 ) );
	m_textCtl[IT_TUTORIAL_WIN_TITLE]->SetText( szTemp );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 682 ) );
	m_textCtl[IT_TUTORIAL_WIN_NAME_U]->SetColor( 0xffffffff );
	m_textCtl[IT_TUTORIAL_WIN_NAME_U]->SetText( szTemp );

	m_textCtl[IT_TUTORIAL_WIN_CHAT_U]->SetColor( 0xffffffff );
	m_textCtl[IT_TUTORIAL_WIN_SELECT_U]->SetColor( 0xffffffff );

	return( IF_CTL_NONE );
}

void CIFTutorialWin::Render()
{
	if( !State() ) return;	

	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	m_selectBar->Render();

	
	m_closeBtn->Render();
	m_prevBtn->Render();
	m_nextBtn->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_TUTORIAL_WIN_TITLE]->Render();
		m_textCtl[IT_TUTORIAL_WIN_NAME_U]->Render();
		if( m_msgIndex >= 0 )
		{
			int x, y, xs, ys, i;
			x = m_textCtl[IT_TUTORIAL_WIN_CHAT_U]->GetXPos();
			y = m_textCtl[IT_TUTORIAL_WIN_CHAT_U]->GetYPos();
			xs = m_textCtl[IT_TUTORIAL_WIN_CHAT_U]->GetXSize();
			ys = m_textCtl[IT_TUTORIAL_WIN_CHAT_U]->GetYSize();
			g_chatManager->SetColor( 0xffffffff );
			g_chatManager->DrawTextRect( x, y, m_messageStr, xs, ys, m_curPage );
			x = m_textCtl[IT_TUTORIAL_WIN_SELECT_U]->GetXPos();
			y = m_textCtl[IT_TUTORIAL_WIN_SELECT_U]->GetYPos();
			for( i = 0; i < m_numSelectStr; i ++ )
			{
				g_chatManager->TextOut( x, y + i * 13, m_selectStr[i], CHAT_ARRANGE_NONE );
			}
		}
	}
	g_chatManager->EndFrame();
}

int CIFTutorialWin::WindowSelection()
{
	int ret;
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFTutorialWin::WindowProcess()
{
	int mx, my;
	int x1, y1, xs, ys;
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		g_TutorialMng->SetReplyMessage( 100 );
	}
	
	
	if( m_curPage > 0 ) m_prevBtn->SetButtonState( IF_BTN_TYPE_FLASH );
	else m_prevBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	if( m_curPage < m_maxPage - 1 ) m_nextBtn->SetButtonState( IF_BTN_TYPE_FLASH );
	else m_nextBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	if( m_prevBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		m_curPage --;
		if( m_curPage < 0 ) m_curPage = 0;
	}
	if( m_nextBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		m_curPage ++;
		if( m_curPage >= m_maxPage ) m_curPage = m_maxPage - 1;
	}
		
	
	mx = g_input->GetPosX();
	my = g_input->GetPosY();
	x1 = m_textCtl[IT_TUTORIAL_WIN_SELECT_U]->GetXPos();
	y1 = m_textCtl[IT_TUTORIAL_WIN_SELECT_U]->GetYPos();
	xs = m_textCtl[IT_TUTORIAL_WIN_SELECT_U]->GetXSize();
	ys = m_numSelectStr * 13;
	mx = mx - x1;
	my = my - y1;
	if( mx < 0 || my < 0 || mx > xs || my > ys ) 
	{
		m_selectedIdx = -1;
	}
	else
	{
		m_selectedIdx = my / 13;
	}
	if( m_selectedIdx >= 0 && m_selectedIdx < m_numSelectStr )
	{
		if( g_input->GetLBUp() ) 
		{
			g_TutorialMng->SetReplyMessage( m_selectedIdx );
			Enable( false );
		}
		g_ifMng->m_iPos[IF_TUTORIAL_WIN_SELECT_BAR].clientY = m_selectBarSavePos + m_selectedIdx * 13;
		m_selectBar->Enable();
	}
	else
	{
		m_selectBar->Enable( false );
	}

	return IF_CTL_NONE;
}

void CIFTutorialWin::InitHelp()
{
	int i;
	for( i = 0; i < MAX_COUNT_IF_HELP_SELECT_STRING; i ++ )
	{
		memset( m_selectStr[i], 0, sizeof( chatMessage ) );
	}
	m_numSelectStr = 0;
	m_msgIndex = -1;
	
	Enable();
	g_ifMng->SetFocus( this );
}

void CIFTutorialWin::MakeMessage( int idx )
{
	int x, y, xs, ys;

	strcpy( m_messageStr, g_LPACK.GetMassage( LPACK_TYPE_TUTORIAL, idx ) );

	m_msgIndex = idx;
	
	x = m_textCtl[IT_TUTORIAL_WIN_CHAT_U]->GetXPos();
	y = m_textCtl[IT_TUTORIAL_WIN_CHAT_U]->GetYPos();
	xs = m_textCtl[IT_TUTORIAL_WIN_CHAT_U]->GetXSize();
	ys = m_textCtl[IT_TUTORIAL_WIN_CHAT_U]->GetYSize();

	m_curPage = 0;
	m_maxPage = g_chatManager->DrawTextRect( x, y, m_messageStr, xs, ys, -1 );

	m_numSelectStr = 0;
}


void CIFTutorialWin::MakeSelectMessage( int pos, int idx )
{
	strcpy( m_selectStr[pos], g_LPACK.GetMassage( LPACK_TYPE_ANSWER, idx ) );

	if( pos >= m_numSelectStr ) m_numSelectStr = pos + 1;
}

void CIFTutorialWin::QuitHelp()
{
}








CIFOptionWin::CIFOptionWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_OPTION_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );
	m_helpBtn = new CIFButtonCtl( this );
	m_optionBtn = new CIFButtonCtl( this );
	m_restartBtn = new CIFButtonCtl( this );
	m_exitBtn = new CIFButtonCtl( this );
	m_closeBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_OPTION_WINDOW_TYPE );
}

CIFOptionWin::~CIFOptionWin()
{

}

void CIFOptionWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_OPTION_WIN );

	
	m_textCtl[IT_OPTION_WIN_TITLE]->PrepareControl( IF_OPTION_WIN_TITLE_TEXT );
	m_textCtl[IT_OPTION_WIN_HELP]->PrepareControl( IF_OPTION_WIN_HELP_TEXT );
	m_textCtl[IT_OPTION_WIN_OPTION]->PrepareControl( IF_OPTION_WIN_OPTION_TEXT );
	m_textCtl[IT_OPTION_WIN_RESTART]->PrepareControl( IF_OPTION_WIN_RESTART_TEXT );
	m_textCtl[IT_OPTION_WIN_EXIT]->PrepareControl( IF_OPTION_WIN_EXIT_TEXT );


	m_titleBar->PrepareControl( IF_OPTION_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_OPTION_WIN_TITLE_BOTTOM );
	m_helpBtn->PrepareControl( IF_OPTION_WIN_HELP_BTN_0 );
	m_optionBtn->PrepareControl( IF_OPTION_WIN_OPTION_BTN_0 );
	m_restartBtn->PrepareControl( IF_OPTION_WIN_RESTART_BTN_0 );
	m_exitBtn->PrepareControl( IF_OPTION_WIN_EXIT_BTN_0 );
	m_closeBtn->PrepareControl( IF_OPTION_WIN_CLOSE_BTN_0 );
}

int CIFOptionWin::WindowFrame()
{
	char szTemp[256];

	
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 522 ) );
	m_textCtl[IT_OPTION_WIN_TITLE]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 523 ) );
	m_textCtl[IT_OPTION_WIN_HELP]->SetText( szTemp );
	
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 524 ) );
	m_textCtl[IT_OPTION_WIN_OPTION]->SetText( szTemp );
	
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 525 ) );
	m_textCtl[IT_OPTION_WIN_RESTART]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 526 ) );
	m_textCtl[IT_OPTION_WIN_EXIT]->SetText( szTemp );

	return( IF_CTL_NONE );
}

void CIFOptionWin::Render()
{
	if( !State() ) return;
	
	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	m_titleBar->Render();
	m_titleBottom->Render();
	m_helpBtn->Render();
	m_optionBtn->Render();
	m_restartBtn->Render();
	m_exitBtn->Render();
	m_closeBtn->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_OPTION_WIN_TITLE]->Render();
		m_textCtl[IT_OPTION_WIN_HELP]->Render();
		m_textCtl[IT_OPTION_WIN_OPTION]->Render();
		m_textCtl[IT_OPTION_WIN_RESTART]->Render();
		m_textCtl[IT_OPTION_WIN_EXIT]->Render();
	}
	g_chatManager->EndFrame();
}


int CIFOptionWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return IF_CTL_NONE;
}

int CIFOptionWin::WindowProcess()
{
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_NONE;
	}
	if( m_helpBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		g_ifMng->m_helpWin->Enable( true );
		g_ifMng->SetFocus( g_ifMng->m_helpWin );
	}
	if( m_optionBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );

		g_ifMng->m_settingWin->Enable( true );
		g_ifMng->SetFocus( g_ifMng->m_settingWin );
	}
	if( m_restartBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( 0, 419 ), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_RESTART_CHARACTER );
	}
	if( m_exitBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,320), g_LPACK.GetMassage(0,321), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_EXIT_CLIENT );
	}
	
	return IF_CTL_NONE;
}












CIFSettingWin::CIFSettingWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_SETTING_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );


	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	m_closeBtn = new CIFButtonCtl( this );

	m_autoTipOnCheckBox = new CIFCheckBox( this );
	m_autoTipOffCheckBox = new CIFCheckBox( this );
	m_bgmOnCheckBox = new CIFCheckBox( this );
	m_bgmOffCheckBox = new CIFCheckBox( this );
	m_effectOnCheckBox = new CIFCheckBox( this );
	m_effectOffCheckBox = new CIFCheckBox( this );

	m_bgmScrollBar = new CIFScrollBar( this );
	m_bgmScrollBar->SetScrollBarType( 1 );
	m_bgmScrollBar->m_max = 15;
	m_bgmScrollBar->m_current = 2;

	m_effectScrollBar = new CIFScrollBar( this );
	m_effectScrollBar->SetScrollBarType( 1 );
	m_effectScrollBar->m_max = 15;
	m_effectScrollBar->m_current = 10;

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_SETTING_WINDOW_TYPE );
}

CIFSettingWin::~CIFSettingWin()
{

}

void CIFSettingWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_SETTING_WIN );

	m_textCtl[IT_SETTING_WIN_TITLE]->PrepareControl( IF_SETTING_WIN_TITLE_TEXT );
	m_textCtl[IT_SETTING_WIN_AUTOTIP]->PrepareControl( IF_SETTING_WIN_AUTOTIP_TEXT );
	m_textCtl[IT_SETTING_WIN_BGMVOLUME]->PrepareControl( IF_SETTING_WIN_BGMVOLUME_TEXT );
	m_textCtl[IT_SETTING_WIN_EFFECTVOLUME]->PrepareControl( IF_SETTING_WIN_EFFECTVOLUME_TEXT );
	m_textCtl[IT_SETTING_WIN_ON1]->PrepareControl( IF_SETTING_WIN_ON1_TEXT );
	m_textCtl[IT_SETTING_WIN_OFF1]->PrepareControl( IF_SETTING_WIN_OFF1_TEXT );
	m_textCtl[IT_SETTING_WIN_ON2]->PrepareControl( IF_SETTING_WIN_ON2_TEXT );
	m_textCtl[IT_SETTING_WIN_OFF2]->PrepareControl( IF_SETTING_WIN_OFF2_TEXT );
	m_textCtl[IT_SETTING_WIN_ON3]->PrepareControl( IF_SETTING_WIN_ON3_TEXT );
	m_textCtl[IT_SETTING_WIN_OFF3]->PrepareControl( IF_SETTING_WIN_OFF3_TEXT );

	m_titleBar->PrepareControl( IF_SETTING_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_SETTING_WIN_TITLE_BOTTOM );
	m_closeBtn->PrepareControl( IF_SETTING_WIN_CLOSE_BTN_0 );

	m_autoTipOnCheckBox->PrepareControl( IF_SETTING_WIN_AUTOTIPON_CHECKBOX1 );
	m_autoTipOffCheckBox->PrepareControl( IF_SETTING_WIN_AUTOTIPOFF_CHECKBOX1 );
	m_bgmOnCheckBox->PrepareControl( IF_SETTING_WIN_BGMON_CHECKBOX1 );
	m_bgmOffCheckBox->PrepareControl( IF_SETTING_WIN_BGMOFF_CHECKBOX1 );
	m_effectOnCheckBox->PrepareControl( IF_SETTING_WIN_EFFECTON_CHECKBOX1 );
	m_effectOffCheckBox->PrepareControl( IF_SETTING_WIN_EFFECTOFF_CHECKBOX1 );

	if( !m_autoTipOnCheckBox->GetCheck() && !m_autoTipOffCheckBox->GetCheck() )
		m_autoTipOnCheckBox->SetCheck( true );
	if( !m_bgmOnCheckBox->GetCheck() && !m_bgmOffCheckBox->GetCheck() )
		m_bgmOnCheckBox->SetCheck( true );
	if( !m_effectOnCheckBox->GetCheck() && !m_effectOffCheckBox->GetCheck() )
		m_effectOnCheckBox->SetCheck( true );

	m_bgmScrollBar->PrepareControl( IF_SETTING_WIN_SCROLL1 );
	m_effectScrollBar->PrepareControl( IF_SETTING_WIN_SCROLL2 );
}

int CIFSettingWin::WindowFrame()
{
	
	char szTemp[256];

	sprintf( szTemp, g_LPACK.GetMassage( 0, 687 ) );

	m_textCtl[IT_SETTING_WIN_TITLE]->SetText( szTemp );
	m_textCtl[IT_SETTING_WIN_TITLE]->SetColor( 0xffffffff );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 688 ) );
	m_textCtl[IT_SETTING_WIN_AUTOTIP]->SetText( szTemp );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 689 ) );
	m_textCtl[IT_SETTING_WIN_BGMVOLUME]->SetText( szTemp );
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 690 ) );
	m_textCtl[IT_SETTING_WIN_EFFECTVOLUME]->SetText( szTemp );
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 691 ) );
	m_textCtl[IT_SETTING_WIN_ON1]->SetText( szTemp );
	m_textCtl[IT_SETTING_WIN_ON2]->SetText( szTemp );
	m_textCtl[IT_SETTING_WIN_ON3]->SetText( szTemp );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 692 ) );
	m_textCtl[IT_SETTING_WIN_OFF1]->SetText( szTemp );
	m_textCtl[IT_SETTING_WIN_OFF2]->SetText( szTemp );
	m_textCtl[IT_SETTING_WIN_OFF3]->SetText( szTemp );

	

	return IF_CTL_NONE;
}

void CIFSettingWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();
	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	m_closeBtn->Render();

	m_autoTipOnCheckBox->Render();
	m_autoTipOffCheckBox->Render();
	m_bgmOnCheckBox->Render();
	m_bgmOffCheckBox->Render();
	m_effectOnCheckBox->Render();
	m_effectOffCheckBox->Render();

	m_bgmScrollBar->Render();
	m_effectScrollBar->Render();
	
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		
		m_textCtl[IT_SETTING_WIN_TITLE]->Render();
		m_textCtl[IT_SETTING_WIN_AUTOTIP]->Render();
		m_textCtl[IT_SETTING_WIN_BGMVOLUME]->Render();
		m_textCtl[IT_SETTING_WIN_EFFECTVOLUME]->Render();
		m_textCtl[IT_SETTING_WIN_ON1]->Render();
		m_textCtl[IT_SETTING_WIN_OFF1]->Render();
		m_textCtl[IT_SETTING_WIN_ON2]->Render();
		m_textCtl[IT_SETTING_WIN_OFF2]->Render();
		m_textCtl[IT_SETTING_WIN_ON3]->Render();
		m_textCtl[IT_SETTING_WIN_OFF3]->Render();


	}
	g_chatManager->EndFrame();
}

int CIFSettingWin::WindowSelection()
{
	int ret;
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );
	return( IF_CTL_NONE );
}

int CIFSettingWin::WindowProcess()
{
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		GTH_SaveOptionToFile();
		
		Enable( false );
		return IF_CTL_NONE;
	}

	if( m_autoTipOnCheckBox->ProcessCheckBox() == IF_CTL_SELECT )
	{
		m_autoTipOnCheckBox->SetCheck( true );
		m_autoTipOffCheckBox->SetCheck( false );

		g_ifMng->m_tipWin->SetTipState( true );
		return IF_CTL_SELECT;
	}
	if( m_autoTipOffCheckBox->ProcessCheckBox() == IF_CTL_SELECT )
	{
		m_autoTipOnCheckBox->SetCheck( false );
		m_autoTipOffCheckBox->SetCheck( true );

		g_ifMng->m_tipWin->SetTipState( false );
		return IF_CTL_SELECT;
	}
	if( m_bgmOnCheckBox->ProcessCheckBox() == IF_CTL_SELECT )
	{
		m_bgmOnCheckBox->SetCheck( true );
		m_bgmOffCheckBox->SetCheck( false );

		g_musicMng->SetBgm( true );
		return IF_CTL_SELECT;
	}
	if( m_bgmOffCheckBox->ProcessCheckBox() == IF_CTL_SELECT )
	{
		m_bgmOnCheckBox->SetCheck( false );
		m_bgmOffCheckBox->SetCheck( true );

		g_musicMng->SetBgm( false );
		return IF_CTL_SELECT;
	}

	if( m_effectOnCheckBox->ProcessCheckBox() == IF_CTL_SELECT )
	{
		m_effectOnCheckBox->SetCheck( true );
		m_effectOffCheckBox->SetCheck( false );

		g_musicMng->SetEffect( true );
		return IF_CTL_SELECT;
	}
	if( m_effectOffCheckBox->ProcessCheckBox() == IF_CTL_SELECT )
	{
		m_effectOnCheckBox->SetCheck( false );
		m_effectOffCheckBox->SetCheck( true );

		g_musicMng->SetEffect( false );
		return IF_CTL_SELECT;
	}

	
	if( g_musicMng->GetBgm() )
		if( m_bgmScrollBar->ProcessScrollBar() == IF_CTL_SELECT )
		{
			g_musicMng->SetBgmVolumePercentage( m_bgmScrollBar->m_current * 10 );
			g_musicMng->RefreshBgmVolume();
			return IF_CTL_SELECT;
		}
	
	if( g_musicMng->GetEffect() )
		if( m_effectScrollBar->ProcessScrollBar() == IF_CTL_SELECT )
		{
			
			g_musicMng->SetEffectVolumePercentage( m_effectScrollBar->m_current * 10 );
			g_musicMng->RefreshEffectVolume();

			
			
			return IF_CTL_SELECT;
		}

	return IF_CTL_NONE;
}

int CIFSettingWin::Selection()
{
	int x1, y1, x2, y2;
	int mx, my;

	x1 = m_info[m_current].clientX;
	y1 = m_info[m_current].clientY;
	x2 = m_info[m_current].clientX + m_info[m_current].sizeX;
	y2 = m_info[m_current].clientY + m_info[m_current].sizeY;
	
	mx = g_input->GetPosX();
	my = g_input->GetPosY();

	if( x1 <= mx && x2 >= mx && y1 <= my && y2 >= my )
	{
		if( g_input->GetLBDown() ) return( IF_LBDN );
		else if( g_input->GetRBDown() ) return( IF_RBDN );
		else if( g_input->GetLBUp() ) return( IF_LBUP );
		else if( g_input->GetRBUp() ) return( IF_RBUP );
		else if( g_input->GetLBKeep() ) return( IF_LBKEEP );
		else if( g_input->GetRBKeep() ) return( IF_RBKEEP );
		else return( IF_IN_POS );
	}
	
	return( IF_NOT_IN_POS );
}

void CIFSettingWin::SetOptionSet()
{

	m_autoTipOnCheckBox->SetCheck( m_optionSetInfo.autoTip );
	m_autoTipOffCheckBox->SetCheck( !m_optionSetInfo.autoTip );
	g_ifMng->m_tipWin->SetTipState( m_optionSetInfo.autoTip );

	m_bgmOnCheckBox->SetCheck( m_optionSetInfo.bgmOn );
	m_bgmOffCheckBox->SetCheck( !m_optionSetInfo.bgmOn );
	g_musicMng->SetBgm( m_optionSetInfo.bgmOn );

	m_effectOnCheckBox->SetCheck( m_optionSetInfo.effectOn );
	m_effectOffCheckBox->SetCheck( !m_optionSetInfo.effectOn );
	g_musicMng->SetEffect( m_optionSetInfo.effectOn );
	
	m_bgmScrollBar->m_current = m_optionSetInfo.bgmVolume;
	g_musicMng->SetBgmVolumePercentage( m_bgmScrollBar->m_current * 10 );
	g_musicMng->RefreshBgmVolume();

	m_effectScrollBar->m_current = m_optionSetInfo.effectVolume;
	g_musicMng->SetEffectVolumePercentage( m_effectScrollBar->m_current * 10 );
	g_musicMng->RefreshEffectVolume();

}

void CIFSettingWin::GetOptionSet()
{
	m_optionSetInfo.autoTip			= m_autoTipOnCheckBox->GetCheck();
	m_optionSetInfo.bgmOn			= m_bgmOnCheckBox->GetCheck();
	m_optionSetInfo.effectOn		= m_effectOnCheckBox->GetCheck();

	m_optionSetInfo.bgmVolume		= m_bgmScrollBar->m_current;
	m_optionSetInfo.effectVolume	= m_effectScrollBar->m_current;
}








CIFWorld_ChangeWin::CIFWorld_ChangeWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{

	
	m_WorldCount	 = 0;

	
	for( int i = 0; i < NUM_IT_WORLD_CHANGE_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );

	
	m_closeBtn  = new CIFButtonCtl( this );	
	m_selectBtn = new CIFButtonCtl( this );	

	m_scrollBar = new CIFScrollBar( this );
	m_cancelBtn = new CIFButtonCtl( this );

	m_ButtonBgBar = new CIFControl( this );
	m_selectBar	= new CIFControl( this );

	m_MessageBgBar = new CIFControl( this );
	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_WORLD_CHANGE_WINDOW_TYPE );
	
	m_WorldCount = 0;
	m_selectBarSavePos = 0;
	m_selectedIdx = 0;
	m_WorldItemIdx = 0;
	
	
}

CIFWorld_ChangeWin::~CIFWorld_ChangeWin()
{

}






void CIFWorld_ChangeWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_WORLD_CHANG_WIN );

	
	m_textCtl[IT_WORLD_CHANGE_WIN_TITLE]->PrepareControl( IF_WORLD_CHANG_WIN_TITLE_TEXT );
	m_textCtl[IT_WORLD_CHANGE_WIN_MESSAGE]->PrepareControl(IF_WORLD_CHANG_WIN_TITLE_MESSAGE_TEXT);	
	m_textCtl[IT_WORLD_CHANG_WIN_LIST_POS]->PrepareControl(IF_WORLD_CHANG_WIN_LIST_POS);		
	

	m_MessageBgBar->PrepareControl ( IF_WORLD_CHANG_WIN_TITLE_MESSAGE_BGBAR );
	
	m_titleBar->PrepareControl( IF_WORLD_CHANG_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_WORLD_CHANG_WIN_TITLE_BAR_BOTTOM );

	
	m_scrollBar->PrepareControl( IF_WORLD_CHANG_WIN_SCROLL );	

	
	m_closeBtn->PrepareControl( IF_WORLD_CHANG_WIN_CLOSE_BTN_0 );
	m_selectBtn->PrepareControl( IF_WORLD_CHANG_WIN_SELECT_BTN0 );
	m_cancelBtn->PrepareControl( IF_WORLD_CHANG_WIN_CANCEL_BTN0 );
	m_ButtonBgBar->PrepareControl( IF_WORLD_CHANG_WIN_BUTTON_BAR );
	m_selectBar->PrepareControl( IF_WORLD_CHANG_WIN_SELECT_BAR );
	m_scrollBar->SetBarPosition( MAX_WORLD_NUMBER,  0 );

	
	m_selectBarSavePos = g_ifMng->m_iPos[IF_WORLD_CHANG_WIN_LIST_POS].clientY + WORLD_LIST_WIN_HEIGHT;
	g_ifMng->m_iPos[IF_WORLD_CHANG_WIN_SELECT_BAR].clientY = m_selectBarSavePos;

	
	
	m_MaxDrawWorldListNum = (g_cgv.worldTableNumber - WORLD_LIST_SKIP_NUM);

}







int CIFWorld_ChangeWin::WindowFrame()
{
	char szTemp[256];

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 567 ) );
	m_textCtl[IT_WORLD_CHANGE_WIN_TITLE]->SetText( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 290 ) );
	m_textCtl[IT_WORLD_CHANGE_WIN_MESSAGE]->SetText( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 288 ) );	


	m_cancelBtn->SetButtonText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 521 ));
	m_selectBtn->SetButtonText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 289 ));


	
	
	
	

	if( m_MaxDrawWorldListNum - WORLD_LIST_WIN_MAX_NUM > 0 && 
		m_MaxDrawWorldListNum - WORLD_LIST_WIN_MAX_NUM != m_scrollBar->m_max )	
	{
		m_scrollBar->SetBarPosition( m_MaxDrawWorldListNum - WORLD_LIST_WIN_MAX_NUM , m_scrollBar->m_current );
	}
	else if( m_MaxDrawWorldListNum - WORLD_LIST_WIN_MAX_NUM <= 0 )
	{
		m_scrollBar->SetBarPosition( 0, 0 );
	}	


	return( IF_CTL_NONE );
}







void CIFWorld_ChangeWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	
	m_titleBar->Render();
	m_titleBarBottom->Render();

	m_ButtonBgBar->Render();

	
	m_closeBtn->Render();			
	m_cancelBtn->RenderTextBtn();
	m_scrollBar->Render();
	m_selectBtn->RenderTextBtn();

	m_MessageBgBar->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_WORLD_CHANGE_WIN_TITLE]->Render();
		m_textCtl[IT_WORLD_CHANGE_WIN_MESSAGE]->Render();
		m_textCtl[IT_WORLD_CHANGE_WIN_SELECT]->Render();
		m_textCtl[IT_WORLD_CHANGE_WIN_CANCEL]->Render();

		WorldListRender();
	}
	g_chatManager->EndFrame();	

	

}







int CIFWorld_ChangeWin::WindowSelection()
{
	int ret;
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}






int CIFWorld_ChangeWin::WindowProcess()
{
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		Enable( false );
		return IF_CTL_NONE;
	}

	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		Enable( false );
		return IF_CTL_NONE;
	}

	
	if ( m_selectBtn->ProcessButton() == IF_CTL_SELECT)
	{		
		
		int SaveInvenPos = g_cgv.myCharacterInfo->m_WorldChageSystem.m_SaveInvenPos;
		int InvenPos = g_cgv.myCharacterInfo->inventory[SaveInvenPos];
		int itemidx = g_cgv.myCharacterInfo->item[InvenPos].itemTableIdx;
		
		if ( itemidx > 0 )
		{
			
			if ( g_itemTable[itemidx].etcItemType == ETCITEM_TYPE_WORLD_PORTAL &&
				g_itemTable[itemidx].WorldPortal.IsActiveWorldPortal[m_selectedIdx] == TRUE)
				GTH_SendMessage_Request_WorldChange(&g_cgv.myCharacterInfo->m_WorldChageSystem);
		}
		return IF_CTL_NONE;
	}	

	if( m_scrollBar->ProcessScrollBar() == IF_CTL_SELECT )
	{
		return IF_CTL_SELECT;
	}


	
	int MouseX, MouseY, CheckPosX1, CheckPosY1, CheckPosX2, CheckPosY2;
	int seletedIdx, listNum;
	
	
	listNum = WORLD_LIST_WIN_MAX_NUM ;

	MouseX = g_input->GetPosX();
	MouseY = g_input->GetPosY();
	CheckPosX1 = m_textCtl[IT_WORLD_CHANG_WIN_LIST_POS]->GetXPos();
	CheckPosY1 = m_textCtl[IT_WORLD_CHANG_WIN_LIST_POS]->GetYPos() + WORLD_LIST_WIN_HEIGHT;
	CheckPosX2 = m_textCtl[IT_WORLD_CHANG_WIN_LIST_POS]->GetXSize() - 110;
	CheckPosY2 = (listNum * MAIL_WIN_LIST_LINE_HEIGHT) + MAIL_WIN_LIST_LINE_HEIGHT;

	MouseX = MouseX - CheckPosX1;
	MouseY = MouseY - CheckPosY1;
	
	if( MouseX < 0 || MouseY < 0 || MouseX > CheckPosX2 || MouseY > CheckPosY2 ) 
		seletedIdx = -1;
	else
		seletedIdx = MouseY / WORLD_LIST_WIN_HEIGHT;	
	

	if( seletedIdx >= 0 && seletedIdx < listNum )
	{
		if( g_input->GetLBUp() ) 
		{
			m_selectedIdx = seletedIdx + m_scrollBar->m_current;
			g_cgv.myCharacterInfo->m_WorldChageSystem.m_SelectWorldIdx = m_selectedIdx;

		}
		else if( g_input->GetLBDBClk() )
		{
			m_selectedIdx = seletedIdx + m_scrollBar->m_current;		
			g_cgv.myCharacterInfo->m_WorldChageSystem.m_SelectWorldIdx = m_selectedIdx;

			if( m_selectedIdx < 0 ) return IF_CTL_NONE;						

			int SaveInvenPos = g_cgv.myCharacterInfo->m_WorldChageSystem.m_SaveInvenPos;
			int InvenPos = g_cgv.myCharacterInfo->inventory[SaveInvenPos];
			int itemidx = g_cgv.myCharacterInfo->item[InvenPos].itemTableIdx;
			
			if ( itemidx > 0 )
			{	
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				if ( m_selectedIdx == DAN_BATTLEMAP_NUM)
					m_selectedIdx += 2;
				
				
				if ( m_selectedIdx == DAN_BATTLEMAP_NUM +1)					
					m_selectedIdx += 1;											
			
			
				
			if ( g_itemTable[itemidx].etcItemType == ETCITEM_TYPE_WORLD_PORTAL &&
				g_itemTable[itemidx].WorldPortal.IsActiveWorldPortal[m_selectedIdx] == TRUE)
				GTH_SendMessage_Request_WorldChange(&g_cgv.myCharacterInfo->m_WorldChageSystem);
				
			}
			g_ifMng->m_iPos[IF_WORLD_CHANG_WIN_SELECT_BAR].clientY = 
			m_selectBarSavePos + seletedIdx * WORLD_LIST_WIN_HEIGHT;			
			m_selectBar->Enable();
		}
	}
	else
	{
		if( g_input->GetLBUp() ) 
		{
			m_selectedIdx = -1;			
			m_selectBar->Enable(false);
		}
	}
	return IF_CTL_NONE;
}



void CIFWorld_ChangeWin::WorldListRender()
{
	int worldidx, idx;
	char buffer[WORLDNAMESTRING];	


	int SaveInvenPos = g_cgv.myCharacterInfo->m_WorldChageSystem.m_SaveInvenPos;
	int InvenPos = g_cgv.myCharacterInfo->inventory[SaveInvenPos];
	int itemidx = g_cgv.myCharacterInfo->item[InvenPos].itemTableIdx;
	
	if ( itemidx < 0)
		return;
		

	int addIdx = 0;

	
	for( worldidx = 0;  worldidx < WORLD_LIST_WIN_MAX_NUM ;  worldidx++)
	{
		idx = worldidx + m_scrollBar->m_current + addIdx;

		if ( g_cgv.worldTable[idx].idx == 0 &&  strlen(g_cgv.worldTable[idx].name) == 0)
			continue;

		
		







		
		if (g_cgv.worldTable[idx].idx == DAN_BATTLEMAP_NUM) 
		{
			addIdx += 2;	
			idx = worldidx + m_scrollBar->m_current + addIdx;
		}

		if (g_cgv.worldTable[idx].idx == DAN_BATTLEMAP_NUM+1) 
		{
			addIdx += 1;	
			idx = worldidx + m_scrollBar->m_current + addIdx;
		}


		if ( idx == m_selectedIdx )
			SetSelectBar(worldidx);
		
		if ( g_itemTable[itemidx].WorldPortal.IsActiveWorldPortal[idx] == TRUE)
			g_chatManager->SetColor( GTHCOLOR_QUESTMESSAGE ) ;
		else
			g_chatManager->SetColor( 0xff000000 ) ;

		_snprintf( buffer, WORLDNAMESTRING, g_cgv.worldTable[idx].hanName );			
		buffer[WORLDNAMESTRING-1] = NULL;

		g_chatManager->DrawText( m_textCtl[IT_WORLD_CHANG_WIN_LIST_POS]->GetXPos() + 4, 
			m_textCtl[IT_WORLD_CHANG_WIN_LIST_POS]->GetYPos()+ ((worldidx + 1) * WORLD_LIST_WIN_HEIGHT)+2, 
			buffer, -1, 13, CHAT_ARRANGE_OVERSIZECUT );	
	}	
}


void CIFWorld_ChangeWin::SetSelectBar( int idx )
{
	m_selectBar->Enable();
	g_ifMng->m_iPos[IF_WORLD_CHANG_WIN_SELECT_BAR].clientY = 
		m_selectBarSavePos + idx * WORLD_LIST_WIN_HEIGHT;
	m_selectBar->UpdateControl();
	m_selectBar->UpdateBuffer();
	m_selectBar->Render();
}








CIFServer_ListWin::CIFServer_ListWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	int i =0;
	m_selectBarSavePos = 0;
	m_selectedIdx = 0;
	
	m_Server_Sort = STATE_ASCENDINGFLAG;
	m_ServerSort_Flag = false;
	m_Server_State_Sort_Flag = false;		
	
	
	
	
	
	
	m_scrollBar         = new CIFScrollBar( this );
	
	m_selectBtn         = new CIFButtonCtl( this );	
	m_cancelBtn         = new CIFButtonCtl( this );			
	m_ServerButton      = new CIFButtonCtl( this );	
	m_ServerState       = new CIFButtonCtl( this );	
	
	m_selectBar	        = new CIFControl( this );
	
    
	
	
	
	
	
	for( i = 0; i < MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN; i ++ )
		m_ServerText[i] = new CIFText( this );
	
	for( i = 0; i < MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN; i ++ )
		m_ServerClientState[i] = new CIFText( this );
	
	
	for( i = 0; i < NUM_IT_SERVERLIST_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );	
	
	
	Enable(false);
	
	
	m_ActiveServerCount = 0;
	
}

CIFServer_ListWin::~CIFServer_ListWin()
{

}






void CIFServer_ListWin ::PrepareControl()
{
	
	CIFControl::PrepareControl( IF_SERVER_LIST_WIN);

	
	m_textCtl[IT_SERVER_LIST_WIN_LIST_POS]->PrepareControl(IF_SERVER_LIST_WIN_LIST_POS);		

	for(int  i = 0; i < MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN; i ++ )
	{
		m_ServerText[i]->PrepareControl( IF_SERVER_TEXT );				

	}
	for( i = 0; i < MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN; i ++ )
	{
		m_ServerClientState[i]->PrepareControl( IF_SERVER_STATE );				

	}	
	
	
	
	m_scrollBar->PrepareControl(IF_SERVER_LIST_WIN_SCROLL);
	m_selectBar->PrepareControl(IF_SERVER_LIST_WIN_SELECT_BAR);
	
	
	
	m_selectBtn->PrepareControl(IF_SERVER_LIST_WIN_SELECT_BTN0);
	m_cancelBtn->PrepareControl(IF_SERVER_LIST_WIN_CANCEL_BTN0);	
	m_ServerButton->PrepareControl(IF_SERVER_LIST_WIN_SERVER_BTN0);
	m_ServerState->PrepareControl(IF_SERVER_LIST_WIN_STATE_BTN0);
	
		
		
	m_selectBarSavePos = g_ifMng->m_iPos[IF_SERVER_LIST_WIN_LIST_POS].clientY
	;
	
	g_ifMng->m_iPos[IF_SERVER_LIST_WIN_SELECT_BAR].clientY = m_selectBarSavePos;

	
	m_textCtl[IT_SERVER_LIST_WIN_LIST_POS]->PrepareControl(IF_SERVER_LIST_WIN_LIST_POS);
	
	m_textCtl[IT_SERVER_LIST_TEXT_MESSAGE]->PrepareControl(IF_SERVER_LIST_WIN_TITLE_MESSAGE_TEXT);

	m_scrollBar->SetBarPosition( MAX_RENDER_SERVER_LIST_COUNT, 0 );
	
	
	int CheckPosX1 = m_textCtl[IT_SERVER_LIST_WIN_LIST_POS]->GetXPos();
	int CheckPosY1 = m_textCtl[IT_SERVER_LIST_WIN_LIST_POS]->GetYPos();
	int CheckPosX2 = m_textCtl[IT_SERVER_LIST_WIN_LIST_POS]->GetXSize();
	int CheckPosY2 = (MAX_RENDER_SERVER_LIST_COUNT * SERVER_LIST_SELECTBAR_HEIGHT);

	
	SetRect(&m_ServerListCheckRect, 
		CheckPosX1, 
		CheckPosY1, 
		CheckPosX1 + CheckPosX2, 
		CheckPosY1 + CheckPosY2);


	
	
}






void CIFServer_ListWin::Render()
{
	
	if ( State() == FALSE )
		return;

		g_ifMng->SetRenderState();	
		CIFControl::Render();	
		
		

		m_scrollBar->Render();

	
		
		m_selectBtn->Render();
		m_cancelBtn->Render();
		m_ServerButton->RenderTextBtn();
		m_ServerState->RenderTextBtn();
	
	
		for( int i = 0; i < m_vpServerList.size(); i ++ )
		{			
			if (m_vpServerList[i].active != NULL) 
			{
				m_ServerText[i]->Render();	
			}
			
		}
		for( i = 0; i < m_vpServerList.size(); i ++ )
		{
			if (m_vpServerList[i].active != NULL) 
			{
				m_ServerClientState[i]->Render();	
			}
			
		}

		m_textCtl[IT_SERVER_LIST_WIN_SELECT]->Render();
		m_textCtl[IT_SERVER_LIST_WIN_CANCEL]->Render();
		m_textCtl[IT_SERVER_LIST_TEXT_MESSAGE]->Render();		
		ServerListRender();
	
}






int CIFServer_ListWin::WindowSelection()
{
	return( IF_CTL_NONE );
}






int CIFServer_ListWin::WindowFrame()
{
	char szTemp[256];

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 567 ) );

	
	m_textCtl[IT_SERVER_LIST_TEXT_MESSAGE]->SetText( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 469 ) );	
	
		
	if( m_ActiveServerCount - MAX_RENDER_SERVER_LIST_COUNT > 0 && 
		m_ActiveServerCount - MAX_RENDER_SERVER_LIST_COUNT != m_scrollBar->m_max )	
	{
		m_scrollBar->SetBarPosition( m_ActiveServerCount - MAX_RENDER_SERVER_LIST_COUNT , 
			m_scrollBar->m_current );
	}
	
	else if( m_ActiveServerCount - MAX_RENDER_SERVER_LIST_COUNT <= 0 )
	{
		m_scrollBar->SetBarPosition( 0, 0 );
	}
	
	return( IF_CTL_NONE );
	
}






int CIFServer_ListWin::WindowProcess()
{	
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		
		Enable( false );
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,320), g_LPACK.GetMassage(0,321), 
			MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_EXIT_CLIENT );		
		
	}
	
	
	if ( m_selectBtn->ProcessButton() == IF_CTL_SELECT)
	{
		if ( m_selectedIdx < 0 || m_selectedIdx >= MAX_SERVER_GROUP)			
		{
			
			m_textCtl[IT_SERVER_LIST_TEXT_MESSAGE]->SetText
				( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 469 ) );				
			return IF_CTL_NONE;	
		}
		
		if( m_ServerText[m_selectedIdx]->State() == IF_CTL_SELECT ) 
		{
			if (m_vpServerList[m_selectedIdx].active == 0) 
			{
				return IF_CTL_NONE;
			}			
			g_cgv.selectedGroupIdx = m_vpServerList[m_selectedIdx].idx;	
			
#ifdef _GTH_ONLINE_VERSION
			GTH_SendMessage_RequestGameData();
			Enable(FALSE);
#endif
			return IF_CTL_NONE;
		}
		
		
		return IF_CTL_NONE;
	}
	
	if( m_ServerButton->ProcessButton() == IF_CTL_SELECT ) 
	{
		m_ServerSort_Flag = true;
		
		if (m_Server_Sort == SERVER_ASCENDINGFLAG) 
		{
			m_Server_Sort = SERVER_DESCENDINGFLAG;		
		}
		else if (m_Server_Sort == SERVER_DESCENDINGFLAG) 
		{
			m_Server_Sort = SERVER_ASCENDINGFLAG;
		}
		else if (m_Server_Sort != SERVER_ASCENDINGFLAG && 
			m_Server_Sort != SERVER_DESCENDINGFLAG) 
		{
			m_Server_Sort = SERVER_ASCENDINGFLAG;
		}
		
		std::vector<serverGroup_t>* vpServerList = 	&g_ifMng->m_ServerListWin->m_vpServerList;		
		std::sort(vpServerList->begin(), vpServerList->end(),
		LessServerGroup_descending_state(g_ifMng->m_ServerListWin->m_Server_Sort));
	}
	
	if( m_ServerState->ProcessButton() == IF_CTL_SELECT ) 
	{
		m_Server_State_Sort_Flag = true;
		
		if (m_Server_Sort == STATE_ASCENDINGFLAG) 
		{
			m_Server_Sort = STATE_DESCENDINGFLAG;		
		}
		else if (m_Server_Sort == STATE_DESCENDINGFLAG) 
		{
			m_Server_Sort = STATE_ASCENDINGFLAG;		
		}
		else if (m_Server_Sort != STATE_ASCENDINGFLAG && 
			m_Server_Sort != STATE_DESCENDINGFLAG) 
		{
			m_Server_Sort = STATE_ASCENDINGFLAG;
		}
		
		
		std::vector<serverGroup_t>* vpServerList = 	&g_ifMng->m_ServerListWin->m_vpServerList;		
		std::sort(vpServerList->begin(), vpServerList->end(),
		LessServerGroup_descending_state(g_ifMng->m_ServerListWin->m_Server_Sort));
		
	}
	

	if( m_scrollBar->ProcessScrollBar() == IF_CTL_SELECT )
	{
		return IF_CTL_SELECT;
	}	

	if ( CheckSelectServer() == FALSE)
	{
		m_selectedIdx = -1;
		m_selectBar->Enable(FALSE);
	}


	
	
	return IF_CTL_NONE;

}

BOOL	CIFServer_ListWin::CheckSelectServer()
{
	

	int seletedIdx, listNum;
	
	listNum = MAX_RENDER_SERVER_LIST_COUNT;
	
	POINT MousePoint;	
	MousePoint.x = g_input->GetPosX();
	MousePoint.y = g_input->GetPosY();

	
	
	
	
	if ( PtInRect(&m_ServerListCheckRect, MousePoint) )	
	{
		seletedIdx = 
		(MousePoint.y - m_textCtl[IT_SERVER_LIST_WIN_LIST_POS]->GetYPos()) 
				/ SERVER_LIST_SELECTBAR_HEIGHT;			
	}
	else	
		seletedIdx = -1;

		
	
	
	
	if( g_input->GetLBUp() ) 
	{
		m_selectedIdx = seletedIdx + m_scrollBar->m_current;
		
		
		if ( m_selectedIdx <0 || m_selectedIdx >= m_ActiveServerCount)			
			return FALSE;			
		
		else
		{
			
			if (m_vpServerList[m_selectedIdx].active == FALSE)
				return FALSE;
			
		}
	}
	
	
	else if( g_input->GetLBDBClk() )
	{					
		if ( m_selectedIdx <0 || m_selectedIdx >= m_ActiveServerCount)
			return FALSE;
		
		if (m_vpServerList[m_selectedIdx].active == 0) 			
			return FALSE;			
		
		if ( PtInRect(&m_ServerListCheckRect, MousePoint) == FALSE)
			return FALSE;		
		
		g_cgv.selectedGroupIdx =  m_vpServerList[m_selectedIdx].idx;
		
#ifdef _GTH_ONLINE_VERSION
		GTH_SendMessage_RequestGameData();
		Enable(FALSE);
#endif					
		g_ifMng->m_iPos[IF_SERVER_LIST_WIN_SELECT_BAR].clientY = 
			(listNum * SERVER_LIST_SELECTBAR_HEIGHT);									
	}				
	
	return TRUE;
}





void CIFServer_ListWin::ServerListRender()
{
	int		i,	idx, MaxListNum;
	int		x, y, addScrollBar;
	DWORD	color;
	char	buffer[WORLDNAMESTRING]				=		"";	
	char	szTemp[SERVERNAME_LEN]				=		"";
	char	serverName[SERVERNAME_LEN]			=		"";	
	char	serverClientNumber[SERVERNAME_LEN]	=		"";	
	char	szserverButton[SERVERNAME_LEN]		=		"";	
	char	szserverState[SERVERNAME_LEN]		=		"";	
	
	
	x=g_ifMng->m_loginWin->m_textCtl[IT_SERVER_LIST_WIN_LIST_POS]->GetXPos();
	y=g_ifMng->m_loginWin->m_textCtl[IT_SERVER_LIST_WIN_LIST_POS]->GetYPos();	
	
	
	strcpy( szserverButton, "SERVER" );
	m_ServerButton->SetButtonState( IF_BTN_TYPE_ENABLE );
	m_ServerButton->SetButtonText( szserverButton, 0xffffffff );
	m_ServerButton->UpdateClientPosition( m_ServerButton->GetXPos(),m_ServerButton->GetYPos());		
	m_ServerButton->Enable( true );
	
	
	strcpy( szserverState, "STATE" );
	m_ServerState->SetButtonState( IF_BTN_TYPE_ENABLE );
	m_ServerState->SetButtonText( szserverState, 0xffffffff );
	m_ServerState->UpdateClientPosition( m_ServerState->GetXPos(),m_ServerState->GetYPos());		
	m_ServerState->Enable( true );	
	
	int addIdx = 0;	
	MaxListNum = 0;
	
	if ( m_vpServerList.empty() )
		return;
	
	for( i = 0;  i < MAX_SERVER_GROUP ;  i++)
	{		
		idx = i + m_scrollBar->m_current + addIdx;
		
		if(idx < 0 || idx >= m_vpServerList.size() || m_vpServerList[idx].name == NULL ) 
			continue;		
		
		strncpy( serverName, m_vpServerList[i].name , SERVERNAME_LEN);		
		serverName[SERVERNAME_LEN-1] = NULL;
		
		
		if ( m_vpServerList[i].active )
		{
			if( m_vpServerList[i].access )
			{
				if( m_vpServerList[i].clientNumber < 100 ) 
					strncpy( serverClientNumber, "(Good)" ,SERVERNAME_LEN);					
				
				else if( m_vpServerList[i].clientNumber < 200 ) 
					strncpy( serverClientNumber, "(Normal)" , SERVERNAME_LEN);
				
				else if( m_vpServerList[i].clientNumber < 500 ) 
					strncpy( serverClientNumber, "(Busy)" , SERVERNAME_LEN);
				
				else strcpy( serverClientNumber, "(Very Busy)" );			
				color = 0xffffffff;					
				
				serverClientNumber[SERVERNAME_LEN-1] = NULL;
				MaxListNum++;
				
			}
			else
			{
				sprintf( szTemp, "(%s)", g_LPACK.GetMassage(0,346) );
				color = 0xff808080;				
				strncpy( szTemp, "(Busy)" , SERVERNAME_LEN);
			}
		}
		else 
		{
			sprintf( szTemp, "(%s)", g_LPACK.GetMassage(0,346) );
			color = 0xff808080;
			
		}
		
		
		m_ServerText[i]->SetText(serverName);
		m_ServerClientState[i]->SetText(serverClientNumber);
		
		
		if ( idx == m_selectedIdx) 
			SetSelectBar(i);
		
		addScrollBar = m_scrollBar->m_current;		
		
		if ( addScrollBar-1 >=0 && addScrollBar-1 < MAX_SERVER_GROUP) 
		{		
			m_ServerText[addScrollBar-1]->Enable(false);
			m_ServerClientState[addScrollBar-1]->Enable(false);
			m_selectBar->Enable(false);
		}			
		
		m_ServerText[i+addScrollBar]->m_info.clientY = 
			m_ServerButton->GetYPos()+ 10
			+ SERVER_LIST_SELECTBAR_HEIGHT + (i * m_ServerText[0]->m_info.sizeY);
		
		m_ServerClientState[i+addScrollBar]->m_info.clientY = 
			m_ServerText[i+addScrollBar]->m_info.clientY;


			if (m_vpServerList[i+addScrollBar].active != NULL) 
			{
				
				m_ServerText[i+addScrollBar]->Enable(true);	
				m_ServerClientState[i+addScrollBar]->Enable(true);					
				if (MaxListNum > 8) 
				{
					m_ServerText[i+addScrollBar]->Enable(false);	
					m_ServerClientState[i+addScrollBar]->Enable(false);
				}
				else if (addScrollBar > 0) 
				{
					m_ServerText[addScrollBar-1]->Enable(false);	
					m_ServerClientState[addScrollBar-1]->Enable(false);					
				}
				
			}		
			
	}	

}

void CIFServer_ListWin::SetSelectBar(int idx)
{
	
	if (m_selectBarSavePos + (idx * SERVER_LIST_SELECTBAR_HEIGHT) < SERVER_LIST_WIN_SELECT_LIST ) 
		{
			m_selectBar->Enable();
		    g_ifMng->m_iPos[IF_SERVER_LIST_WIN_SELECT_BAR].clientY =
				m_selectBarSavePos + (idx * SERVER_LIST_SELECTBAR_HEIGHT);
		    m_selectBar->UpdateControl();
		    m_selectBar->UpdateBuffer();
		    m_selectBar->Render();		
		}
	

}











CIFDanWarRequestWin::CIFDanWarRequestWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{	
	m_textCtlBoxDoc				= new CIFText( this );
	m_textCtlWorldTitle			= new CIFText( this );
	m_textCtlDanNameTitle		= new CIFText( this );
	m_textCtlDanName			= new CIFText( this );
	m_textCtlWorldName			= new CIFText( this );
	m_textCtlDanMemberNum		= new CIFText( this );
	m_textCtlDanWarTypeTitle	= new CIFText( this );
	m_textCtlDanWarTime			= new CIFText( this );
	m_textCtlDanMemberNumTitle  = new CIFText( this );
	m_textCtlDanMemberNum		= new CIFText( this );
	
	m_ComboCtlDanWarType		= new CIFComboBox( this );
	m_ComboCtlDanWarTime		= new CIFComboBox( this );
	
	m_OkBtn						= new CIFButtonCtl( this );
	m_cancelBtn					= new CIFButtonCtl( this );	





	m_textCtlLoseDanPointTitle	= new CIFText( this );	
	m_textCtlLoseDanPoint		= new CIFText( this );	
	m_textCtlMyPointTitle		= new CIFText( this );	
	m_textCtlMyPoint			= new CIFText( this );	
	m_textCtlBestIDTitle		= new CIFText( this );	
	m_textCtlBestID				= new CIFText( this );	

	
	m_PrimiumItemCheckBox		= new CIFCheckBox(this);
	m_textPrimiumItem			= new CIFText(this);

	Enable(false);		
}

CIFDanWarRequestWin::~CIFDanWarRequestWin()
{

}






void CIFDanWarRequestWin::PrepareControl()
{
	
	
	CIFControl::PrepareControl( IF_DANWAR_REQUEST_WIN );

	m_textCtlBoxDoc->PrepareControl(IF_BOXDOC_TEXT);
	m_textCtlDanNameTitle->PrepareControl(IF_DANNAMETITLE);
	m_textCtlDanName->PrepareControl(IF_DANNAME);
	m_textCtlWorldTitle->PrepareControl(IF_WORLDTITLE);
	m_textCtlWorldName->PrepareControl(IF_WORLDNAME);
	m_textCtlDanMemberNumTitle->PrepareControl(IF_DANMEMBERNUM);
	m_textCtlDanMemberNum->PrepareControl(IF_DANMEMBERTEXT);
	m_textCtlDanWarTypeTitle->PrepareControl(IF_DANWARTYPETITLE);	
	m_textCtlDanWarTime->PrepareControl(IF_DANWARTIMETEXT);
	m_ComboCtlDanWarTime->PrepareControl(IF_COMBO_DANWARTIME, 95);
	m_ComboCtlDanWarType->PrepareControl(IF_COMBO_DANWARTYPE, 95);
	m_OkBtn->PrepareControl(IF_DANREQUEST_OK_BTN_0);
	m_cancelBtn->PrepareControl(IF_DANREQUEST_CANCLE_BTN_0);

	
	m_PrimiumItemCheckBox->PrepareControl(IF_PRIMIUM_ITEM_CHECKBOX0_0);
	m_textPrimiumItem->PrepareControl(IF_PRIMIUM_ITEM_TEXT);
	
	
	
	
	
	m_textPrimiumItem->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 670));

	g_ifMng->AddFocus( this );
	SetWindowType(IF_DANWAR_REQUEST_WIN_TYPE);

	m_SaveOkBtnXPos = g_ifMng->m_iPos[IF_DANREQUEST_OK_BTN_0].clientX;
	















}






void CIFDanWarRequestWin::Render()
{
	
	if ( State() == FALSE )
		return;
		
	g_ifMng->SetRenderState();
	
	CIFControl::Render();							
	
	
	m_OkBtn->RenderTextBtn();
	m_cancelBtn->RenderTextBtn();
	
	
	

	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{	

		m_textCtlDanNameTitle->Render();
		m_textCtlBoxDoc->Render();
		m_textCtlWorldTitle->Render();		
		m_textCtlWorldName->Render();
		m_textCtlDanMemberNum->Render();	
		m_textCtlDanWarTypeTitle->Render();			
		m_textCtlDanMemberNumTitle->Render();
		m_textCtlDanWarTime->Render();
		
		
		if ( (m_ComboCtlDanWarType->GetCurSel() == 1) && (m_DanWarWinType == REQUEST)) 
		{		
			m_textPrimiumItem->Render();
			m_PrimiumItemCheckBox->Render();
		}

		if ( m_DanWarWinType == EXAMINATION )
		{
			m_textPrimiumItem->Render();
			m_PrimiumItemCheckBox->Render();		
		}

		
		if ( m_DanWarWinType == DanWarRequestWinType::REQUEST)
			g_chatManager->RenderInputFrame( INPUT_INDEX_DAN_NAME );
		else
			m_textCtlDanName->Render();				 
	}
	g_chatManager->EndFrame();

	g_ifMng->SetRenderState();
	m_ComboCtlDanWarTime->Render();
	m_ComboCtlDanWarType->Render();	
	


}






int CIFDanWarRequestWin::WindowSelection()
{	






	
	return( IF_CTL_NONE );
}






int CIFDanWarRequestWin::WindowFrame()
{
	if ( FALSE == State() )
		return 0;
	

	






	






	
	
	return( IF_CTL_NONE );
	
}






int CIFDanWarRequestWin::WindowProcess()
{	
	if( !State() ) return( IF_CTL_NONE );









	
	if ( m_DanWarWinType == REQUEST){	
		if ( m_PrimiumItemCheckBox->ProcessCheckBox()  == IF_CTL_SELECT)	
			return IF_CTL_SELECT;
	}


	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT ) 
	{	
		if ( m_DanWarWinType == DanWarRequestWinType::EXAMINATION)
		{
			GTH_SendMessage_ReplySuggest(tagGCPacket_NewDanBattle::enumCode::fail);
			Enable( false );	
			return IF_CTL_NONE;
		}		

		g_chatManager->SetInputState( INPUT_INDEX_DAN_NAME, -1 );		
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
		Enable( false );	
	}			

	if( m_OkBtn->ProcessButton() == IF_CTL_SELECT ) 	
	{			
		if ( m_DanWarWinType == DanWarRequestWinType::EXAMINATION)
		{
			GTH_SendMessage_ReplySuggest(tagGCPacket_NewDanBattle::enumCode::success);
			Enable( false );	
			return IF_CTL_NONE;
		}

		if (m_DanWarWinType == DanWarRequestWinType::RESULT)
		{
			Enable( false );	
			return IF_CTL_NONE;
		}
		
		if ( strlen(g_chatManager->GetInputStr( INPUT_INDEX_DAN_NAME )) <= 0 )
		{
			
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 628));
			return IF_CTL_NONE;
		}
		
		GTH_SendMessage_RequestDanBattle();

		Enable( false );	
		g_chatManager->SetInputState( INPUT_INDEX_DAN_NAME, -1 );		
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
		 
	}

	if ( m_ComboCtlDanWarType->ProcessComboBox() == IF_CTL_SELECT)
	{
		return IF_CTL_NONE;
	}	

	if ( m_ComboCtlDanWarTime->ProcessComboBox() == IF_CTL_SELECT)
	{

		return IF_CTL_NONE;
	}

	

	
	if ( m_textCtlDanName->Selection() == IF_LBDN )
	{		
		g_chatManager->SetInputState( INPUT_INDEX_DAN_NAME , 1);
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DAN_NAME );		
	}

	return IF_CTL_NONE;
}

void CIFDanWarRequestWin::InitDanWarWin(enum DanWarRequestWinType type, class NewDanBattleSystem* pNewDanBattleSystem)
{
	m_DanWarWinType = type;
	
	switch (type)
	{
	case DanWarRequestWinType::REQUEST:
		InitDanWarRequestWin(type, pNewDanBattleSystem);
		break;
	case DanWarRequestWinType::EXAMINATION:
		InitDanWarExaminationWin(type, pNewDanBattleSystem);
		break;
	case DanWarRequestWinType::RESULT:
		InitDanWarResultWin(type, pNewDanBattleSystem);
		break;
	}
}


void CIFDanWarRequestWin::InitDanWarExaminationWin(enum DanWarRequestWinType type, class NewDanBattleSystem* pNewDanBattleSystem)
{

	if ( pNewDanBattleSystem == NULL )
		return;

	char szString[MAX_NUM_OF_SEL_STRING];

	m_DanWarWinType = type;
	NewDanBattleSystem::DanBattleInfo* pDanBattmeInfo = pNewDanBattleSystem->GetDanBattleInfo();

	
	m_OkBtn->SetButtonText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 629));
	
	m_cancelBtn->SetButtonText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 630));	

	
	m_textCtlBoxDoc->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 631));
	
	m_textCtlDanNameTitle->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 632));
	
	m_textCtlWorldTitle->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 633));
	
	m_textCtlDanWarTypeTitle->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 634));
	
	m_textCtlDanWarTime->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 635));
	m_cancelBtn->Enable();


	g_ifMng->m_iPos[IF_DANREQUEST_OK_BTN_0].clientX = m_SaveOkBtnXPos;			
	g_ifMng->m_iPos[IF_DANREQUEST_OK_BTN_1].clientX = m_SaveOkBtnXPos;		
	g_ifMng->m_iPos[IF_DANREQUEST_OK_BTN_2].clientX = m_SaveOkBtnXPos;		
	m_OkBtn->UpdateControl();
	m_OkBtn->UpdateBuffer();

	
	if ( pDanBattmeInfo == NULL)
		return;
	
	
	m_ComboCtlDanWarType->ClearString();
	if ( pDanBattmeInfo->BattleMode == 
		NewDanBattleSystem::DanBattleInfo::enumBattleMode::BATTLEMODE_GENERAL)
		
		m_ComboCtlDanWarType->AddString(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 618));			
	else
		
		m_ComboCtlDanWarType->AddString(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 619));
	m_ComboCtlDanWarType->SetCurSel(0);
	
	
	m_ComboCtlDanWarTime->ClearString();		
	
	
	
	
	int iRemainTime = pDanBattmeInfo->fRemainSec / 1000;	
	UINT RemainnSec = iRemainTime%60;			
	UINT TempTime = (iRemainTime/60);
	
	UINT RemainMin = TempTime%60;			
	UINT RemainHour = TempTime/60;		
	
	sprintf( szString,  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 636), RemainHour, RemainMin, RemainnSec);
	
	
	m_ComboCtlDanWarTime->AddString(szString);
	m_ComboCtlDanWarTime->SetCurSel(0);
	
	
	m_textCtlDanName->SetText(pDanBattmeInfo->RequestDanInfo.szGuildName);
	
	
	itoa(pDanBattmeInfo->RequestDanInfo.nRemainPoint, szString, 10);
	m_textCtlDanMemberNum->SetText(szString);
	
	
	if ( (pDanBattmeInfo->nBattleMapIdx >= 0) || 
		(pDanBattmeInfo->nBattleMapIdx < MAX_WORLD_NUMBER))
		m_textCtlWorldName->SetText(g_cgv.worldTable[pDanBattmeInfo->nBattleMapIdx].hanName);
	
	
	m_textCtlDanMemberNumTitle->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 637));

	m_ComboCtlDanWarType->SetCurSel(0);	
	m_ComboCtlDanWarTime->SetCurSel(0);	

	
	m_PrimiumItemCheckBox->SetCheck(pDanBattmeInfo->bPlunderPremiumItem);
}






void CIFDanWarRequestWin::InitDanWarRequestWin(enum DanWarRequestWinType type, class NewDanBattleSystem* pNewDanBattleSystem)
{
	if ( pNewDanBattleSystem == NULL )
		return;

	char szString[MAX_NUM_OF_SEL_STRING];

	m_DanWarWinType = type;
	NewDanBattleSystem::DanBattleInfo* pDanBattmeInfo = pNewDanBattleSystem->GetDanBattleInfo();
	
	g_chatManager->SetInput( m_textCtlDanName->GetXPos(), m_textCtlDanName->GetYPos(),
		GUILDNAMESTRING, GUILDNAMESTRING, INPUT_INDEX_DAN_NAME, 1 );
	
	g_chatManager->SetInputState( INPUT_INDEX_DAN_NAME , 1);
	g_chatManager->ChangeInputMessage( INPUT_INDEX_DAN_NAME );


	g_ifMng->m_iPos[IF_DANREQUEST_OK_BTN_0].clientX = m_SaveOkBtnXPos;			
	g_ifMng->m_iPos[IF_DANREQUEST_OK_BTN_1].clientX = m_SaveOkBtnXPos;		
	g_ifMng->m_iPos[IF_DANREQUEST_OK_BTN_2].clientX = m_SaveOkBtnXPos;			
	m_OkBtn->UpdateControl();
	m_OkBtn->UpdateBuffer();


	
	m_textCtlBoxDoc->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 638));
	
	m_textCtlDanNameTitle->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 632));	
	
	m_textCtlWorldTitle->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 633));	
	
	m_textCtlDanWarTypeTitle->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 634));
	
	m_textCtlDanWarTime->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 635));
	m_cancelBtn->Enable();
	
	
	m_ComboCtlDanWarType->ClearString();
	
	m_ComboCtlDanWarType->AddString(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 619));
	
	m_ComboCtlDanWarType->AddString(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 618));
	m_ComboCtlDanWarType->SetCurSel(0);
	
	m_ComboCtlDanWarTime->ClearString();		

	int iRemainTime = g_NewDanBattleSystem.m_nBattleTimeForMin1 / 1000;	
	UINT RemainnSec = iRemainTime%60;			
	UINT TempTime = (iRemainTime/60);

	UINT RemainMin = TempTime%60;			
	UINT RemainHour = TempTime/60;		
	
	sprintf( szString,  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 636), RemainHour, RemainMin, RemainnSec);

	
	m_ComboCtlDanWarTime->AddString(szString);

	iRemainTime = g_NewDanBattleSystem.m_nBattleTimeForMin2 / 1000;	
	RemainnSec = iRemainTime%60;			
	TempTime = (iRemainTime/60);

	RemainMin = TempTime%60;			
	RemainHour = TempTime/60;		
	
	sprintf( szString,  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 636), RemainHour, RemainMin, RemainnSec);

	
	
	m_ComboCtlDanWarTime->AddString(szString);
	m_ComboCtlDanWarTime->SetCurSel(0);
	
	itoa(pNewDanBattleSystem->m_nMinPcCount, szString, 10);
	m_textCtlDanMemberNum->SetText(szString);		
	
	if ( (pDanBattmeInfo->nBattleMapIdx >= 0) || 
		(pDanBattmeInfo->nBattleMapIdx  < MAX_WORLD_NUMBER))
		m_textCtlWorldName->SetText(g_cgv.worldTable[pDanBattmeInfo->nBattleMapIdx].hanName);
	
	
	m_OkBtn->SetButtonText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 639));
	
	m_cancelBtn->SetButtonText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 630));	
	
	m_textCtlDanMemberNumTitle->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 640));

	m_ComboCtlDanWarType->SetCurSel(0);	
	m_ComboCtlDanWarTime->SetCurSel(0);	

	
	m_PrimiumItemCheckBox->SetCheck(false);
}




void CIFDanWarRequestWin::InitDanWarResultWin(enum DanWarRequestWinType type, class NewDanBattleSystem* pNewDanBattleSystem)
{
	if ( pNewDanBattleSystem == NULL )
		return;

	char szString[MAX_NUM_OF_SEL_STRING];

	m_DanWarWinType = type;
	NewDanBattleSystem::DanBattleInfo* pDanBattmeInfo = pNewDanBattleSystem->GetDanBattleInfo();
	
	
	m_OkBtn->SetButtonText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 549));	

	g_ifMng->m_iPos[IF_DANREQUEST_OK_BTN_0].clientX += 40;
	g_ifMng->m_iPos[IF_DANREQUEST_OK_BTN_1].clientX += 40;
	g_ifMng->m_iPos[IF_DANREQUEST_OK_BTN_2].clientX += 40;
	m_OkBtn->UpdateControl();
	m_OkBtn->UpdateBuffer();

	m_cancelBtn->Enable(FALSE);	
	if ( pDanBattmeInfo == NULL)
		return;

	
	m_textCtlBoxDoc->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 641));
	
	m_textCtlDanNameTitle->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 642));	
	
	m_textCtlWorldTitle->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 643));
	
	m_textCtlDanMemberNumTitle->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 644));
	
	m_textCtlDanWarTypeTitle->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 645));
	
	m_textCtlDanWarTime->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 646));	
	
	
	itoa(pDanBattmeInfo->RequestDanInfo.nRemainPoint, szString, 10);
	
}












CIFDanWarBattleWin::CIFDanWarBattleWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{	


	m_textCtlREMAINTIME_TITLE				= new CIFText( this );
	m_textCtlREMAINTIME_TEXT				= new CIFText( this );	
	m_textCtlREMAINPC_TITLE					= new CIFText( this );	
	m_textCtlREMAINPC_TEXT					= new CIFText( this );		
	m_textCtlMY_GUILD_KILLPOINT_TITLE		= new CIFText( this );	
	m_textCtlMY_GUILD_KILLPOINT_TEXT		= new CIFText( this );	
	m_textCtlPC_KILLPOINT_TITLE				= new CIFText( this );		
	m_textCtlPC_KILLPOINT_TEXT				= new CIFText( this );		
	m_textCtlENEMY_GUILD_KILLPOINT_TITLE	= new CIFText( this );
	m_textCtlENEMY_GUILD_KILLPOINT_TEXT		= new CIFText( this );




	Enable(false);		
}

CIFDanWarBattleWin::~CIFDanWarBattleWin()
{

}






void CIFDanWarBattleWin::PrepareControl()
{
	
	
	CIFControl::PrepareControl( IF_DANWAR_BATTLE_WIN );		

	m_textCtlREMAINTIME_TITLE->PrepareControl(IF_DANWAR_REMAINTIME_TITLE);
	m_textCtlREMAINTIME_TEXT->PrepareControl(IF_DANWAR_REMAINTIME_TEXT);
	m_textCtlREMAINPC_TITLE->PrepareControl(IF_DANWAR_REMAINPC_TITLE);
	m_textCtlREMAINPC_TEXT->PrepareControl(IF_DANWAR_REMAINPC_TEXT);
	m_textCtlMY_GUILD_KILLPOINT_TITLE->PrepareControl(IF_DANWAR_MY_GUILD_KILLPOINT_TITLE);
	m_textCtlMY_GUILD_KILLPOINT_TEXT->PrepareControl(IF_DANWAR_MY_GUILD_KILLPOINT_TEXT);
	m_textCtlPC_KILLPOINT_TITLE->PrepareControl(IF_DANWAR_PC_KILLPOINT_TITLE);
	m_textCtlPC_KILLPOINT_TEXT->PrepareControl(IF_DANWAR_PC_KILLPOINT_TEXT);
	m_textCtlENEMY_GUILD_KILLPOINT_TITLE->PrepareControl(IF_DANWAR_ENEMY_GUILD_KILLPOINT_TITLE);
	m_textCtlENEMY_GUILD_KILLPOINT_TEXT->PrepareControl(IF_DANWAR_ENEMY_GUILD_KILLPOINT_TEXT);
	
	g_ifMng->AddFocus( this );
	SetWindowType(IF_DANWAR_BATTLE_WIN_TYPE);
	
}






void CIFDanWarBattleWin::Render()
{

	if ( State() == FALSE )
		return;
	
	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo();
	if (pDanBattleInfo->BattleStatus != NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE)
		return;
	
	g_ifMng->SetRenderState();
	CIFControl::Render();

	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{				
		m_textCtlREMAINTIME_TITLE->Render(IF_TEXT_OUTPUT_CENTER);
		m_textCtlREMAINTIME_TEXT->Render(IF_TEXT_OUTPUT_CENTER);	
		m_textCtlREMAINPC_TITLE->Render(IF_TEXT_OUTPUT_CENTER);	
		m_textCtlREMAINPC_TEXT->Render(IF_TEXT_OUTPUT_CENTER);				
		m_textCtlMY_GUILD_KILLPOINT_TITLE->Render(IF_TEXT_OUTPUT_CENTER);	
		m_textCtlMY_GUILD_KILLPOINT_TEXT->Render(IF_TEXT_OUTPUT_CENTER);	
		m_textCtlPC_KILLPOINT_TITLE->Render(IF_TEXT_OUTPUT_CENTER);		
		m_textCtlPC_KILLPOINT_TEXT->Render(IF_TEXT_OUTPUT_CENTER);		
		m_textCtlENEMY_GUILD_KILLPOINT_TITLE->Render(IF_TEXT_OUTPUT_CENTER);	
		m_textCtlENEMY_GUILD_KILLPOINT_TEXT->Render(IF_TEXT_OUTPUT_CENTER);		
	}
	g_chatManager->EndFrame();
}






int CIFDanWarBattleWin::WindowSelection()
{	
	
	return( IF_CTL_NONE );
}






int CIFDanWarBattleWin::WindowFrame()
{
	if ( FALSE == State() )
		return 0;

	char szTemp[256];
	
	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo();
	if (pDanBattleInfo->BattleStatus != NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE)
		return 0;

	int iRemainTime = pDanBattleInfo->fRemainSec / 1000;
	
	
	UINT RemainnSec = iRemainTime%60;			
	UINT TempTime = (iRemainTime/60);

	UINT RemainMin = TempTime%60;			
	UINT RemainHour = TempTime/60;		












	
	sprintf( szTemp,  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 636), RemainHour, RemainMin, RemainnSec);
	
	m_textCtlREMAINTIME_TITLE->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 653));
	
	m_textCtlREMAINTIME_TEXT->SetText(szTemp);

	if ( pDanBattleInfo->fRemainSec <= 60000 )
		m_textCtlREMAINTIME_TEXT->SetColor(GTHCOLOR_ERRORMESSAGE);
	else
		m_textCtlREMAINTIME_TEXT->SetColor(0xffffffff);


	
	m_textCtlREMAINPC_TITLE->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 647));
	
	sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 648), pDanBattleInfo->AcceptDanInfo.nRemainPoint);
	m_textCtlREMAINPC_TEXT->SetText(szTemp);

	if ( pDanBattleInfo->AcceptDanInfo.nRemainPoint <= 5)
		m_textCtlREMAINPC_TEXT->SetColor(GTHCOLOR_ERRORMESSAGE);
	else
		m_textCtlREMAINPC_TEXT->SetColor(0xffffffff);
	

	
	m_textCtlMY_GUILD_KILLPOINT_TITLE->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 649));
	
	sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 613), pDanBattleInfo->AcceptDanInfo.nKillPoint);
	m_textCtlMY_GUILD_KILLPOINT_TEXT->SetText(szTemp);
	m_textCtlPC_KILLPOINT_TITLE->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 650));
	sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 613), g_cgv.myCharacterInfo->Killpoint);
	m_textCtlPC_KILLPOINT_TEXT->SetText(szTemp);
	m_textCtlENEMY_GUILD_KILLPOINT_TITLE->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 651));
	sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 613), pDanBattleInfo->RequestDanInfo.nKillPoint);
	m_textCtlENEMY_GUILD_KILLPOINT_TEXT->SetText(szTemp);

	

	return( IF_CTL_NONE );
	
}






int CIFDanWarBattleWin::WindowProcess()
{	
	if( !State() ) return( IF_CTL_NONE );
	

	return IF_CTL_NONE;
}







