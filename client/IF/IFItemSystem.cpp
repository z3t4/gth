 



#include "../global_def.h"
#include "IFCommon.h"
#include "../game/GolryunBattle/GolryunBattle.h"


extern CItemUseProc g_ItemUseProc;
extern CItemRecycleProc		gItemRecycleProc;


/*-------------------------------------------------------------------------------------------------*/

















































































































































































































































































































































































































































































































































































































































































CIFDepotWin::CIFDepotWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i = 0; i < NUM_IT_DEPOT_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );
	m_depotBar = new CIFControl( this );
	m_invenBar1 = new CIFControl( this );
	m_invenBar2 = new CIFControl( this );
	m_seBar	= new CIFControl( this );
		
	m_saveBtn = new CIFButtonCtl( this );
	m_loadBtn = new CIFButtonCtl( this );
	m_closeBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_DEPOT_WINDOW_TYPE );
}

CIFDepotWin::~CIFDepotWin()
{

}

void CIFDepotWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_DEPOT_WIN );

	m_textCtl[IT_DEPOT_WIN_TITLE]->PrepareControl( IF_DEPOT_WIN_TITLE_TEXT );
	m_textCtl[IT_DEPOT_WIN_DEPOT]->PrepareControl( IF_DEPOT_WIN_DEPOT_TEXT );
	m_textCtl[IT_DEPOT_WIN_INVEN_POS]->PrepareControl( IF_DEPOT_WIN_INVEN_POS );
	m_textCtl[IT_DEPOT_WIN_SE]->PrepareControl( IF_DEPOT_WIN_SE_TEXT );
	m_textCtl[IT_DEPOT_WIN_SE_U]->PrepareControl( IF_DEPOT_WIN_SE_TEXT_U );

	m_titleBar->PrepareControl( IF_DEPOT_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_DEPOT_WIN_TITLE_BOTTOM );
	m_depotBar->PrepareControl( IF_DEPOT_WIN_DEPOT_BAR );
	m_invenBar1->PrepareControl( IF_DEPOT_WIN_INVEN_1 );
	m_invenBar2->PrepareControl( IF_DEPOT_WIN_INVEN_2 );
	m_seBar->PrepareControl( IF_DEPOT_WIN_SE_BAR );
		
	m_saveBtn->PrepareControl( IF_DEPOT_WIN_SAVE_BTN_0 );
	m_loadBtn->PrepareControl( IF_DEPOT_WIN_LOAD_BTN_0 );
	m_closeBtn->PrepareControl( IF_DEPOT_WIN_CLOSE_BTN_0 );
}

int CIFDepotWin::WindowFrame()
{
	char szTemp[256];
	
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 484 ) );
	m_textCtl[IT_DEPOT_WIN_TITLE]->SetText( szTemp );

	sprintf( szTemp, "%s", g_cgv.myCharacterInfo->name );
	m_textCtl[IT_DEPOT_WIN_DEPOT]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
	m_textCtl[IT_DEPOT_WIN_SE]->SetText( szTemp );

	strcpy( szTemp, g_ifMng->GetNumberString( g_cgv.myCharacterInfo->depotSE ) );
	m_textCtl[IT_DEPOT_WIN_SE_U]->SetText( szTemp );

	return IF_CTL_NONE;
}

void CIFDepotWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();

	m_titleBar->Render();
	m_titleBottom->Render();
	m_depotBar->Render();


	m_seBar->Render();
		
	m_saveBtn->Render();
	m_loadBtn->Render();
	m_closeBtn->Render();

	
	g_ifMng->m_iconCtl->BeginFrame();
	{
		
		for( int i = 0; i < MAX_DEPOT_SIZE; i ++ )
		{
			int itemIdx, itemTableIdx, cx, cy;
			itemIdx = g_cgv.myCharacterInfo->depot[i];
			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_DEPOT_WIN_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
				cy = m_textCtl[IT_DEPOT_WIN_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}
	}
	g_ifMng->m_iconCtl->EndFrame();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_DEPOT_WIN_TITLE]->Render();
		m_textCtl[IT_DEPOT_WIN_DEPOT]->Render();
		m_textCtl[IT_DEPOT_WIN_SE]->Render();
		m_textCtl[IT_DEPOT_WIN_SE_U]->Render( IF_TEXT_OUTPUT_RIGHT );
		
		g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
		g_chatManager->SetReverseSize( -1 );
		g_chatManager->SetColor( 0xffffffff );
		
		for( int i = 0; i < MAX_DEPOT_SIZE; i ++ )
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->depot[i];

			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->item[itemIdx] ) )
				{
					cx = m_textCtl[IT_DEPOT_WIN_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
					cy = m_textCtl[IT_DEPOT_WIN_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
					number = g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
					
					strcpy( szTemp, g_ifMng->GetNumberString( number ) );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
	}
	g_chatManager->EndFrame();
}

int CIFDepotWin::WindowSelection()
{
	int ret;
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFDepotWin::WindowProcess()
{
	char szTemp[256], szDepotPrice[256];
	
	
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT ) GTH_SendMessage_RequestQuitNpcChat();
	if( m_saveBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_ifMng->m_stackWin->InitStack( g_LPACK.GetMassage( 0, 553 ), NULL, IF_STACK_TYPE_DEPOT_SAVE_SE );
	}
	if( m_loadBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_ifMng->m_stackWin->InitStack( g_LPACK.GetMassage( 0, 553 ), NULL, IF_STACK_TYPE_DEPOT_LOAD_SE );
	}

	
	int x, y, pos;
	int mx, my, temp;
	
	mx = g_input->GetPosX() - m_textCtl[IT_DEPOT_WIN_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_DEPOT_WIN_INVEN_POS]->GetYPos();

	if( mx <= 0 || my <= 0 ) return( IF_CTL_NONE );
	if( mx >= 34 * 6 ||  my >= 33 * 7 ) return( IF_CTL_NONE );

	temp = mx / 34;
	x = temp % 6;
	temp = my / 33;
	y = temp % 7;

	pos = y * 6 + x;

	
	if( pos < 0 || pos >= 42 )
	{
		return( IF_CTL_NONE );
	}
		
	int itemIdx = g_cgv.myCharacterInfo->depot[pos];
	int mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;
	item_t *item;

	
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
		if( mouseItemIdx >= 0 )
		{
			if( g_cgv.myCharacterInfo->mouseInventoryType == MOUSEINVEN_TYPE_DEPOT )
			{

				GTH_SendMessage_ItemSystem_DepotGetItem( pos );
			}
			else
			{
				if( itemIdx < 0 )
				{
					int depotPrice = GTH_GetDepotPrice( &g_cgv.myCharacterInfo->item[mouseItemIdx] );

					
					if( g_cgv.myCharacterInfo->curChargeSE < depotPrice )
					{
						g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,590) );
						return( IF_CTL_NONE );
					}

					
					strcpy( szDepotPrice, g_ifMng->GetNumberString( depotPrice ) );
					sprintf( szTemp, g_LPACK.GetMassage( 0, 588 ), g_cgv.myCharacterInfo->item[mouseItemIdx].name, szDepotPrice );
					g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_DEPOT_ITEM );
					m_saveDepotPos = pos;
				}
			}
		}
		else
		{
			if( itemIdx >= 0 )
			{
				item = &g_cgv.myCharacterInfo->item[itemIdx];
				
				if( !GTH_IsStackItem( item ) ) 
				{	

					GTH_SendMessage_ItemSystem_DepotGetItem( pos );
				}
				
				else
				{
					g_ifMng->m_stackWin->InitStack( g_LPACK.GetMassage( 0, 498 ), item, IF_STACK_TYPE_DEPOT, pos );
				}
			}
		}
	}
	return IF_CTL_NONE;
}












CIFPrecocityWin::CIFPrecocityWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_PRECOCITY_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );

	
	m_closeBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_PRECOCITY_WINDOW_TYPE );
}

CIFPrecocityWin::~CIFPrecocityWin()
{

}

void CIFPrecocityWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_PRECOCITY_WIN );

	m_textCtl[IT_PRECOCITY_WIN_TITLE]->PrepareControl( IF_PRECOCITY_WIN_TITLE_TEXT );
	m_textCtl[IT_PRECOCITY_WIN_INVEN_POS_0]->PrepareControl( IF_PRECOCITY_WIN_INVEN_POS_0 );
	m_textCtl[IT_PRECOCITY_WIN_INVEN_POS_1]->PrepareControl( IF_PRECOCITY_WIN_INVEN_POS_1 );
	m_textCtl[IT_PRECOCITY_WIN_INVEN_POS_2]->PrepareControl( IF_PRECOCITY_WIN_INVEN_POS_2 );
	m_textCtl[IT_PRECOCITY_WIN_INVEN_POS_3]->PrepareControl( IF_PRECOCITY_WIN_INVEN_POS_3 );
	m_textCtl[IT_PRECOCITY_WIN_INVEN_POS_4]->PrepareControl( IF_PRECOCITY_WIN_INVEN_POS_4 );
	m_textCtl[IT_PRECOCITY_WIN_INVEN_POS_5]->PrepareControl( IF_PRECOCITY_WIN_INVEN_POS_5 );

	m_titleBar->PrepareControl( IF_PRECOCITY_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_PRECOCITY_WIN_TITLE_BAR_BOTTOM );
	m_closeBtn->PrepareControl( IF_PRECOCITY_WIN_CLOSE_BTN_0 );
}

int CIFPrecocityWin::WindowFrame()
{
	m_textCtl[IT_PRECOCITY_WIN_TITLE]->SetText( g_LPACK.GetMassage( 0, 591 ) );
	m_textCtl[IT_PRECOCITY_WIN_TITLE]->SetColor( 0xffffffff );

	return IF_CTL_NONE;
}

void CIFPrecocityWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();

	
	m_closeBtn->Render();

	g_ifMng->m_iconCtl->BeginFrame();
	{
		
		for( int i = 0; i < MAX_PRECOCITY_SIZE; i ++ )
		{
			int itemIdx, itemTableIdx, cx, cy;
			int precocityTime, precocityCurrentTime;
			itemIdx = g_cgv.myCharacterInfo->precocityInventory[i];

			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_PRECOCITY_WIN_INVEN_POS_0 + i]->m_info.clientX;
				cy = m_textCtl[IT_PRECOCITY_WIN_INVEN_POS_0 + i]->m_info.clientY;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;

				if( g_itemTable[itemTableIdx].etcItemType == ETCITEM_TYPE_PRECOCITY )
				{
					
					if(gGameConfigFlag.m_bPrecocity_Time)
					{
						precocityTime = 
							g_itemTable[itemTableIdx].precocity.precocityTime * g_cgv.myCharacterInfo->m_fItemPrecocityTimeRatio;
					}
					else
					{
						precocityTime = g_itemTable[itemTableIdx].precocity.precocityTime;
					}

					

					precocityCurrentTime = g_cgv.myCharacterInfo->precocityTime[i];
				}
				else
				{
					precocityTime = 1;
					precocityCurrentTime = 0;
				}

				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID, 
					(float)precocityCurrentTime, (float)precocityTime );
			}
		}
	}
	g_ifMng->m_iconCtl->EndFrame();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_PRECOCITY_WIN_TITLE]->Render();

		g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
		g_chatManager->SetReverseSize( -1 );
		g_chatManager->SetColor( 0xffffffff );

		for( int i = 0; i < MAX_PRECOCITY_SIZE; i ++ )
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->precocityInventory[i];

			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->item[itemIdx] ) )
				{
					cx = m_textCtl[IT_PRECOCITY_WIN_INVEN_POS_0 + i]->m_info.clientX;
					cy = m_textCtl[IT_PRECOCITY_WIN_INVEN_POS_0 + i]->m_info.clientY;
					number = g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
					strcpy( szTemp, g_ifMng->GetNumberString( number ) );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
	}
	g_chatManager->EndFrame();
}

int CIFPrecocityWin::WindowSelection()
{
	int ret;
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFPrecocityWin::WindowProcess()
{
	char szTemp[256], szTemp2[256];
	
	
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT ) GTH_SendMessage_RequestQuitNpcChat();
	
	
	for( int i = 0; i < MAX_PRECOCITY_SIZE; i ++ )
	{
		if( m_textCtl[IT_PRECOCITY_WIN_INVEN_POS_0 + i]->Selection() != IF_NOT_IN_POS ) break;
	}

	
	if( i == MAX_PRECOCITY_SIZE )
	{
		return( IF_CTL_NONE );
	}
	int pos = i;
		
	int itemIdx = g_cgv.myCharacterInfo->precocityInventory[pos];
	int mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;

	
	if( g_input->GetLBNone() ) 
	{
		
		if( itemIdx < 0 ) return( IF_CTL_NONE );

		g_ifMng->InitRolloverText();
		g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[itemIdx] );
		
		if( g_cgv.myCharacterInfo->precocityTime[pos] > 0 ) 
		{
			strcpy( szTemp2, g_ifMng->GetTimeString( g_cgv.myCharacterInfo->precocityTime[pos] ) );
			sprintf( szTemp, g_LPACK.GetMassage( 0, 594 ), szTemp2 );
			g_ifMng->AddRolloverText( szTemp, 2 );
		}
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
	}
	
	if( g_input->GetLBUp() ) 
	{
		
		if( g_cgv.myCharacterInfo->curItemWeight > g_cgv.myCharacterInfo->calItemWeight )
		{
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( 0, 589 ) );
			return( IF_CTL_NONE );
		}
		if( mouseItemIdx >= 0 )
		{
			if( itemIdx < 0 )
			{	
				GTH_SendMessage_ItemSystem_PrecocityItem( pos );
			}
			else
			{
				int itemTableIdx;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
				if( g_itemTable[itemTableIdx].etcItemType == ETCITEM_TYPE_PRECOCITY )
				{
					m_savePrecocityPos = pos;
					g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( 0, 599 ), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_PRECOCITY_ITEM );
				}
				else
				{
					GTH_SendMessage_ItemSystem_PrecocityItem( pos );
				}
			}
		}
		else
		{
			if( itemIdx >= 0 )
			{
				int itemTableIdx;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
				if( g_itemTable[itemTableIdx].etcItemType == ETCITEM_TYPE_PRECOCITY )
				{
					m_savePrecocityPos = pos;
					g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( 0, 599 ), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_PRECOCITY_ITEM );
				}
				else
				{
					GTH_SendMessage_ItemSystem_PrecocityItem( pos );
				}
			}
		}
	}
	return IF_CTL_NONE;
}












CIFCraftWin::CIFCraftWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_CRAFT_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	
	
	m_progressWin = new CIFControl( this );
	m_progressBar = new CIFBarCtl( this );
	m_progressAni = new CIFControl( this );

	
	m_okBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );

	m_craftingFlag = false;
	m_craftCurrentTime = 0.0f;
	m_craftMaxTime = 1.0f;

	m_nCraftIdx = -1;

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_CRAFT_WINDOW_TYPE );
}

CIFCraftWin::~CIFCraftWin()
{

}

void CIFCraftWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_CRAFT_WIN );

	m_textCtl[IT_CRAFT_WIN_TITLE]->PrepareControl( IF_CRAFT_WIN_TITLE_TEXT );
	m_textCtl[IT_CRAFT_WIN_RESULT_INVEN_0_POS]->PrepareControl( IF_CRAFT_WIN_RESULT_INVEN_0 );
	m_textCtl[IT_CRAFT_WIN_RESULT_INVEN_1_POS]->PrepareControl( IF_CRAFT_WIN_RESULT_INVEN_1 );
	m_textCtl[IT_CRAFT_WIN_SOURCE_INVEN_POS]->PrepareControl( IF_CRAFT_WIN_SOURCE_INVEN_POS );
	m_textCtl[IT_CRAFT_WIN_PROGRESS]->PrepareControl( IF_CRAFT_WIN_PROGRESS_TEXT );
	m_textCtl[IT_CRAFT_WIN_OK]->PrepareControl( IF_CRAFT_WIN_OK_TEXT );
	m_textCtl[IT_CRAFT_WIN_CANCEL]->PrepareControl( IF_CRAFT_WIN_CANCEL_TEXT );

	
	m_titleBar->PrepareControl( IF_CRAFT_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_CRAFT_WIN_TITLE_BAR_BOTTOM );
	
	
	m_progressWin->PrepareControl( IF_CRAFT_WIN_PROGRESS_WIN );
	m_progressBar->PrepareControl( IF_CRAFT_WIN_PROGRESS_BAR );
	m_progressAni->PrepareAnimControl( IF_CRAFT_WIN_PROGRESS_ANIM_0, 4, 300.0f );

	
	m_okBtn->PrepareControl( IF_CRAFT_WIN_OK_BTN_0 );
	m_cancelBtn->PrepareControl( IF_CRAFT_WIN_CANCEL_BTN_0 );
}

int CIFCraftWin::WindowFrame()
{
	
	m_textCtl[IT_CRAFT_WIN_TITLE]->SetText( g_LPACK.GetMassage( 0, 630 ) );
	m_textCtl[IT_CRAFT_WIN_TITLE]->SetColor( 0xffffffff );
	m_textCtl[IT_CRAFT_WIN_PROGRESS]->SetText( g_LPACK.GetMassage( 0, 632 ) );
	m_textCtl[IT_CRAFT_WIN_PROGRESS]->SetColor( 0xffffffff );
	m_textCtl[IT_CRAFT_WIN_OK]->SetText( g_LPACK.GetMassage( 0, 631 ) );
	m_textCtl[IT_CRAFT_WIN_OK]->SetColor( 0xffffffff );
	m_textCtl[IT_CRAFT_WIN_CANCEL]->SetText( g_LPACK.GetMassage( 0, 521 ) );
	m_textCtl[IT_CRAFT_WIN_CANCEL]->SetColor( 0xffffffff );

	m_progressBar->UpdateBarState( (int)( m_craftMaxTime - m_craftCurrentTime ), (int)m_craftMaxTime );

	return( IF_CTL_NONE );
}

void CIFCraftWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	
	
	m_okBtn->Render();
	m_cancelBtn->Render();

	g_ifMng->m_iconCtl->BeginFrame();
	{
		
		for( int i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
		{
			int itemIdx, itemTableIdx, cx, cy;
			itemIdx = g_cgv.myCharacterInfo->craftSourceInventory[i];
			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_CRAFT_WIN_SOURCE_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
				cy = m_textCtl[IT_CRAFT_WIN_SOURCE_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}
		for( i = 0; i < MAX_CRAFT_RESULT_ITEM_SIZE; i ++ )
		{
			int itemIdx, itemTableIdx, cx, cy;
			itemIdx = g_cgv.myCharacterInfo->craftResultInventory[i];
			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_CRAFT_WIN_RESULT_INVEN_0_POS + i]->m_info.clientX;
				cy = m_textCtl[IT_CRAFT_WIN_RESULT_INVEN_0_POS + i]->m_info.clientY;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}

	}
	g_ifMng->m_iconCtl->EndFrame();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_CRAFT_WIN_TITLE]->Render();
		m_textCtl[IT_CRAFT_WIN_OK]->Render();
		m_textCtl[IT_CRAFT_WIN_CANCEL]->Render();

		
		g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
		g_chatManager->SetReverseSize( -1 );
		g_chatManager->SetColor( 0xffffffff );
		
		for( int i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->craftSourceInventory[i];

			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->item[itemIdx] ) )
				{
					cx = m_textCtl[IT_CRAFT_WIN_SOURCE_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
					cy = m_textCtl[IT_CRAFT_WIN_SOURCE_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
					number = g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
					strcpy( szTemp, g_ifMng->GetNumberString( number ) );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
		for( i = 0; i < MAX_CRAFT_RESULT_ITEM_SIZE; i ++ )
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->craftResultInventory[i];

			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->item[itemIdx] ) )
				{
					cx = m_textCtl[IT_CRAFT_WIN_RESULT_INVEN_0_POS + i]->m_info.clientX;
					cy = m_textCtl[IT_CRAFT_WIN_RESULT_INVEN_0_POS + i]->m_info.clientY;
					number = g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
					strcpy( szTemp, g_ifMng->GetNumberString( number ) );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
	}
	g_chatManager->EndFrame();

	
	if( m_craftingFlag )
	{
		g_ifMng->SetRenderState();
		m_progressWin->Render();
		m_progressBar->Render();
		m_progressAni->Render();
		
		g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
		g_chatManager->BeginFrame();
		{
			m_textCtl[IT_CRAFT_WIN_PROGRESS]->Render( IF_TEXT_OUTPUT_CENTER );
		}
		g_chatManager->EndFrame();
	}
}

int CIFCraftWin::WindowSelection()
{
	int ret;
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFCraftWin::WindowProcess()
{
	int itemIdx, mouseItemIdx;
	int x, y, pos;
	int mx, my, temp;

	
	if( m_craftingFlag )
	{
		m_craftCurrentTime -= g_timer.GetElapsedMilliTime();
		if( m_craftCurrentTime < 0.0f )
		{
			GTH_SendMessage_ItemSystem_CraftComplete();
			m_craftingFlag = false;
		}
	}

	
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		if( !m_craftingFlag ) 
		{
			GTH_SendMessage_ItemSystem_CraftClose();
		}
		else
		{
			m_craftingFlag = false;
			m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		}
	}


	if( m_okBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		GTH_SendMessage_ItemSystem_CraftPrepare();
	}

	
	if( m_craftingFlag ) return( IF_CTL_NONE );

	
	pos = -1;
	if( m_textCtl[IT_CRAFT_WIN_RESULT_INVEN_0_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 0;
	}
	else if( m_textCtl[IT_CRAFT_WIN_RESULT_INVEN_1_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 1;
	}

	if( pos >= 0 )
	{
		itemIdx = g_cgv.myCharacterInfo->craftResultInventory[pos];
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
			if( mouseItemIdx < 0 && itemIdx >= 0 ) 
			{
				
				GTH_SendMessage_ItemSystem_CraftItem( 1, pos,m_nCraftIdx);
			}
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
		
	itemIdx = g_cgv.myCharacterInfo->craftSourceInventory[pos];
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

		
		GTH_SendMessage_ItemSystem_CraftItem( 0, pos ,m_nCraftIdx);
	}

	return IF_CTL_NONE;
}

void CIFCraftWin::InitCraft()
{
	for( int i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		g_cgv.myCharacterInfo->craftSourceInventory[i] = -1;
	}
	for( i = 0; i < MAX_CRAFT_RESULT_ITEM_SIZE; i ++ )
	{
		g_cgv.myCharacterInfo->craftResultInventory[i] = -1;
	}
	m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	m_craftingFlag = false;
	m_nCraftIdx = -1;
}

void CIFCraftWin::UndoCraft()
{
	item_t *item;
	int itemIdx, inventoryPos;

	for( int i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		itemIdx = g_cgv.myCharacterInfo->craftSourceInventory[i];
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

	for( i = 0; i < MAX_CRAFT_RESULT_ITEM_SIZE; i ++ )
	{
		itemIdx = g_cgv.myCharacterInfo->craftResultInventory[i];
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
	
	InitCraft();
	Enable( false );
}

void CIFCraftWin::StartCraft( float craftMaxTime )
{
	m_craftMaxTime = craftMaxTime;
	m_craftCurrentTime = craftMaxTime;
	m_progressBar->UpdateBarState( m_craftMaxTime - m_craftCurrentTime, (int)m_craftMaxTime );

	m_craftingFlag = true;
}












CIFUpgradeWin::CIFUpgradeWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_UPGRADE_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	
	
	m_progressWin = new CIFControl( this );
	m_progressBar = new CIFBarCtl( this );
	m_progressAni = new CIFControl( this );

	
	m_okBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );

	
	
	m_optionCombo = new CIFComboBox( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_UPGRADE_WINDOW_TYPE );

	m_upgradeFlag = false;
	m_upgradeCurrentTime = 0.0f;
	m_upgradeMaxTime = 1.0f;
}

CIFUpgradeWin::~CIFUpgradeWin()
{

}

void CIFUpgradeWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_UPGRADE_WIN );

	m_textCtl[IT_UPGRADE_WIN_TITLE]->PrepareControl( IF_UPGRADE_WIN_TITLE_TEXT );
	m_textCtl[IT_UPGRADE_WIN_INVEN_0_POS]->PrepareControl( IF_UPGRADE_WIN_INVEN_0 );
	m_textCtl[IT_UPGRADE_WIN_INVEN_1_POS]->PrepareControl( IF_UPGRADE_WIN_INVEN_1 );
	m_textCtl[IT_UPGRADE_WIN_INVEN_2_POS]->PrepareControl( IF_UPGRADE_WIN_INVEN_2 );
	m_textCtl[IT_UPGRADE_WIN_RESULT_INVEN_POS]->PrepareControl( IF_UPGRADE_WIN_RESULT_INVEN );
	m_textCtl[IT_UPGRADE_WIN_PROGRESS]->PrepareControl( IF_UPGRADE_WIN_PROGRESS_TEXT );
	m_textCtl[IT_UPGRADE_WIN_OK]->PrepareControl( IF_UPGRADE_WIN_OK_TEXT );
	m_textCtl[IT_UPGRADE_WIN_CANCEL]->PrepareControl( IF_UPGRADE_WIN_CANCEL_TEXT );

	
	m_titleBar->PrepareControl( IF_UPGRADE_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_UPGRADE_WIN_TITLE_BAR_BOTTOM );
	
	
	m_progressWin->PrepareControl( IF_UPGRADE_WIN_PROGRESS_WIN );
	m_progressBar->PrepareControl( IF_UPGRADE_WIN_PROGRESS_BAR );
	m_progressAni->PrepareAnimControl( IF_UPGRADE_WIN_PROGRESS_ANIM_0, 4, 300.0f );

	
	m_okBtn->PrepareControl( IF_UPGRADE_WIN_OK_BTN_0 );
	m_cancelBtn->PrepareControl( IF_UPGRADE_WIN_CANCEL_BTN_0 );

	
	m_optionCombo->PrepareControl( IF_UPGRADE_WIN_OPTION_COMBOBOX, 190 );
}

int CIFUpgradeWin::WindowFrame()
{

	if( !m_bDeleteOption )
	{
		m_textCtl[IT_UPGRADE_WIN_PROGRESS]->SetText( g_LPACK.GetMassage( 0, 643 ) ); 
		m_textCtl[IT_UPGRADE_WIN_PROGRESS]->SetColor( 0xffffffff );
	}
	else
	{
		m_textCtl[IT_UPGRADE_WIN_PROGRESS]->SetText( g_LPACK.GetMassage( 0, 699 ) ); 
		m_textCtl[IT_UPGRADE_WIN_PROGRESS]->SetColor( 0xffffffff );
	}
	
	m_textCtl[IT_UPGRADE_WIN_TITLE]->SetText( g_LPACK.GetMassage( 0, 642 ) );
	m_textCtl[IT_UPGRADE_WIN_TITLE]->SetColor( 0xffffffff );
	m_textCtl[IT_UPGRADE_WIN_OK]->SetText( g_LPACK.GetMassage( 0, 644 ) );
	m_textCtl[IT_UPGRADE_WIN_OK]->SetColor( 0xffffffff );
	m_textCtl[IT_UPGRADE_WIN_CANCEL]->SetText( g_LPACK.GetMassage( 0, 521 ) );
	m_textCtl[IT_UPGRADE_WIN_CANCEL]->SetColor( 0xffffffff );

	m_progressBar->UpdateBarState( (int)( m_upgradeMaxTime - m_upgradeCurrentTime ), (int)m_upgradeMaxTime );

	return( IF_CTL_NONE );
}

void CIFUpgradeWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	
	
	m_okBtn->Render();
	m_cancelBtn->Render();

	g_ifMng->m_iconCtl->BeginFrame();
	{
		
		for( int i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
		{
			int itemIdx, itemTableIdx, cx, cy;
			itemIdx = g_cgv.myCharacterInfo->upgradeSourceInventory[i];
			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_UPGRADE_WIN_INVEN_0_POS + i]->m_info.clientX;
				cy = m_textCtl[IT_UPGRADE_WIN_INVEN_0_POS + i]->m_info.clientY;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}
		{
			int itemIdx, itemTableIdx, cx, cy;
			itemIdx = g_cgv.myCharacterInfo->upgradeResultInventory;
			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_UPGRADE_WIN_RESULT_INVEN_POS]->m_info.clientX;
				cy = m_textCtl[IT_UPGRADE_WIN_RESULT_INVEN_POS]->m_info.clientY;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}

	}
	g_ifMng->m_iconCtl->EndFrame();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_UPGRADE_WIN_TITLE]->Render();
		m_textCtl[IT_UPGRADE_WIN_OK]->Render();
		m_textCtl[IT_UPGRADE_WIN_CANCEL]->Render();

		
		g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
		g_chatManager->SetReverseSize( -1 );
		g_chatManager->SetColor( 0xffffffff );
		
		for( int i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->upgradeSourceInventory[i];

			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->item[itemIdx] ) )
				{
					cx = m_textCtl[IT_UPGRADE_WIN_INVEN_0_POS + i]->m_info.clientX;
					cy = m_textCtl[IT_UPGRADE_WIN_INVEN_0_POS + i]->m_info.clientY;
					number = g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
					strcpy( szTemp, g_ifMng->GetNumberString( number ) );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->upgradeResultInventory;

			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->item[itemIdx] ) )
				{
					cx = m_textCtl[IT_UPGRADE_WIN_RESULT_INVEN_POS]->m_info.clientX;
					cy = m_textCtl[IT_UPGRADE_WIN_RESULT_INVEN_POS]->m_info.clientY;
					number = g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
					strcpy( szTemp, g_ifMng->GetNumberString( number ) );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
	}
	g_chatManager->EndFrame();

	g_ifMng->SetRenderState();
	
	m_optionCombo->Render();

	
	if( m_upgradeFlag )
	{
		m_progressWin->Render();
		m_progressBar->Render();
		m_progressAni->Render();
		
		g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
		g_chatManager->BeginFrame();
		{
			m_textCtl[IT_UPGRADE_WIN_PROGRESS]->Render( IF_TEXT_OUTPUT_CENTER );
		}
		g_chatManager->EndFrame();
	}
}

int CIFUpgradeWin::WindowSelection()
{
	int ret;

	if ( this != g_ifMng->m_winControl[0] )
		m_optionCombo->m_bExpansion = false;
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );

	
}

int CIFUpgradeWin::WindowProcess()
{
	int itemIdx, mouseItemIdx;
	int pos;

	
	if( m_upgradeFlag )
	{
		m_upgradeCurrentTime -= g_timer.GetElapsedMilliTime();
		if( m_upgradeCurrentTime < 0.0f )
		{
			if( !m_bDeleteOption )
				GTH_SendMessage_ItemSystem_UpgradeComplete( m_optionCombo->GetItemData( m_optionCombo->GetCurSel() ) );
			else
			{
				

				GTH_SendMessage_ItemSystem_DeleteComplete( m_optionCombo->GetItemData( m_optionCombo->GetCurSel() ) );
			}

			m_upgradeFlag = false;
		}
	}
	else
	{
		
		if( m_optionCombo->ProcessComboBox() == IF_CTL_SELECT )
		{
			return( IF_CTL_NONE );
		}
	}

	
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		if( !m_upgradeFlag ) 
		{
			GTH_SendMessage_ItemSystem_UpgradeClose();
		}
		else
		{
			m_upgradeFlag = false;
			m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		}
	}
	if( m_okBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( !m_bDeleteOption )
			GTH_SendMessage_ItemSystem_UpgradePrepare( m_optionCombo->GetItemData( m_optionCombo->GetCurSel() ) );
		else
		{
			if( m_optionCombo->GetCurSel() == -1 )
			{
				
			}

			
			
			GTH_SendMessage_ItemSystem_DeletePrepare( m_optionCombo->GetItemData( m_optionCombo->GetCurSel() ) );
		}
	}

	if( m_upgradeFlag || m_optionCombo->m_bExpansion ) return( IF_CTL_NONE );
	

	
	pos = -1;
	if( m_textCtl[IT_UPGRADE_WIN_RESULT_INVEN_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 0;
	}
	if( pos >= 0 )
	{
		itemIdx = g_cgv.myCharacterInfo->upgradeResultInventory;
		mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;

		if( g_input->GetLBNone() ) 
		{
			
			if( itemIdx < 0 ) return( IF_CTL_NONE );
			
			g_ifMng->InitRolloverText();
			g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[itemIdx] );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		}
		if( g_input->GetLBDown() ) 
		{
			if( mouseItemIdx < 0 && itemIdx >= 0 )
			{
				GTH_SendMessage_ItemSystem_UpgradeItem( 1, pos );
			}
		}
		return( IF_CTL_NONE );
	}

	pos = -1;
	if( m_textCtl[IT_UPGRADE_WIN_INVEN_0_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 0;
	}
	if( m_textCtl[IT_UPGRADE_WIN_INVEN_1_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 1;
	}
	if( m_textCtl[IT_UPGRADE_WIN_INVEN_2_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 2;
	}
	
	
	if( pos < 0 || pos >= 3 )
	{
		return( IF_CTL_NONE );
	}
		
	itemIdx = g_cgv.myCharacterInfo->upgradeSourceInventory[pos];
	mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;

	
	if( g_input->GetLBNone() ) 
	{
		
		if( itemIdx < 0 ) return( IF_CTL_NONE );

		g_ifMng->InitRolloverText();
		g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[itemIdx] );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
	}

	
	if( g_input->GetLBDown() ) 
	{
		
		if( g_cgv.myCharacterInfo->curItemWeight > g_cgv.myCharacterInfo->calItemWeight )
		{
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( 0, 589 ) );
			return( IF_CTL_NONE );
		}
		if( mouseItemIdx < 0 && itemIdx < 0 ) return IF_CTL_NONE;

		GTH_SendMessage_ItemSystem_UpgradeItem( 0, pos );
	}

	return IF_CTL_NONE;
}

void CIFUpgradeWin::InitUpgrade()
{
	for( int i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
	{
		g_cgv.myCharacterInfo->upgradeSourceInventory[i] = -1;
	}
	g_cgv.myCharacterInfo->upgradeResultInventory = -1;

	m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	m_upgradeFlag = false;

	
	m_optionCombo->Enable( false );
	m_optionCombo->ClearString();
	m_bDeleteOption = false;
}

void CIFUpgradeWin::UndoUpgrade()
{
	item_t *item;
	int itemIdx, inventoryPos;

	for( int i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
	{
		itemIdx = g_cgv.myCharacterInfo->upgradeSourceInventory[i];
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

	itemIdx = g_cgv.myCharacterInfo->upgradeResultInventory;
	if( itemIdx >= 0 )
	{
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
	
	InitUpgrade();
	Enable( false );
}

void CIFUpgradeWin::StartUpgrade( float upgradeMaxTime )
{
	m_upgradeMaxTime = upgradeMaxTime;
	m_upgradeCurrentTime = upgradeMaxTime;
	m_progressBar->UpdateBarState( m_upgradeMaxTime - m_upgradeCurrentTime, (int)m_upgradeMaxTime );

	m_upgradeFlag = true;
}

void CIFUpgradeWin::SetOptionComboBox()
{
	
	int optSphereType, optColor = 0;

	m_optionCombo->Enable( true );

	int itemIdx = g_cgv.myCharacterInfo->upgradeSourceInventory[0];
	if( itemIdx < 0 ) 
	{
		ClearOptionComboBox();
		return;
	}
	
	char	buffer[128];
	char	szTemp[128];

	item_t	*item	=	&g_cgv.myCharacterInfo->item[itemIdx];
	int		i;
	int		j;
	int		optionIdx[MAX_NUMBER_OF_ITEM_OPTION];
	int		optionValue[MAX_NUMBER_OF_ITEM_OPTION];
	int		indexSequence[MAX_NUMBER_OF_ITEM_OPTION];

	
	for( i = 0; i < item->optionNumber; i ++ )
	{
		optionIdx[i]		=	item->optionIdx[i];
		optionValue[i]		=	item->optionValue[i]; 
		indexSequence[i]	=	i;
	}
	for( i = 0; i < item->optionNumber; i ++ )
	{
		for( j = i; j < item->optionNumber; j ++ )
		{
			if( optionIdx[i] < optionIdx[j] )
			{
				int tempOpt;

				tempOpt				=	optionIdx[i];
				optionIdx[i]		=	optionIdx[j];
				optionIdx[j]		=	tempOpt;
				
				tempOpt				=	optionValue[i];
				optionValue[i]		=	optionValue[j];
				optionValue[j]		=	tempOpt;

				tempOpt				=	indexSequence[i];
				indexSequence[i]	=	indexSequence[j];
				indexSequence[j]	=	tempOpt;
			}
		}
	}
	
	m_optionCombo->ClearString();

	for( i=0; i<item->optionNumber; i++ )
	{

		
		if( strstr( g_optionTable[optionIdx[i]].printing, "%" ) != NULL )
		{
			sprintf( szTemp, g_optionTable[optionIdx[i]].printing, 
				(int)OPTION_GetOptionValue( item, optionIdx[i], optionValue[i] ) );
		}
		else
		{
			strcpy( szTemp, g_optionTable[optionIdx[i]].printing );
		}
		sprintf( buffer, "[+%d] %s", optionValue[i], szTemp );
		
		

		optSphereType = OPTION_GetOptionStoneGrade( item );
		optColor = 2;

		
		
		m_optionCombo->SetItemData( m_optionCombo->AddString( buffer, 0 ), indexSequence[i] );
	}

	if( item->optionNumber > 0 )
		m_optionCombo->SetCurSel( 0 );
}

void CIFUpgradeWin::ClearOptionComboBox()
{
	m_optionCombo->ClearString();
	m_optionCombo->Enable( false );
}





CIFNpcInvWin::CIFNpcInvWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_NPCINV_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	
	
	m_okBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_NPCINVENTORY_WINDOW_TYPE );




}

CIFNpcInvWin::~CIFNpcInvWin()
{

}

void CIFNpcInvWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_NPCINV_WIN );

	m_textCtl[IT_NPCINV_WIN_TITLE]->PrepareControl( IF_NPCINV_WIN_TITLE_TEXT );
	m_textCtl[IT_NPCINV_WIN_INVEN_0_POS]->PrepareControl( IF_NPCINV_WIN_INVEN_0 );
	m_textCtl[IT_NPCINV_WIN_INVEN_1_POS]->PrepareControl( IF_NPCINV_WIN_INVEN_1 );
	m_textCtl[IT_NPCINV_WIN_INVEN_2_POS]->PrepareControl( IF_NPCINV_WIN_INVEN_2 );
	m_textCtl[IT_NPCINV_WIN_INVEN_3_POS]->PrepareControl( IF_NPCINV_WIN_INVEN_3 );
	m_textCtl[IT_NPCINV_WIN_INVEN_4_POS]->PrepareControl( IF_NPCINV_WIN_INVEN_4 );
	m_textCtl[IT_NPCINV_WIN_INVEN_5_POS]->PrepareControl( IF_NPCINV_WIN_INVEN_5 );
	m_textCtl[IT_NPCINV_WIN_OK]->PrepareControl( IF_NPCINV_WIN_OK_TEXT );
	m_textCtl[IT_NPCINV_WIN_CANCEL]->PrepareControl( IF_NPCINV_WIN_CANCEL_TEXT );

	
	m_titleBar->PrepareControl( IF_NPCINV_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_NPCINV_WIN_TITLE_BAR_BOTTOM );
	
	
	m_okBtn->PrepareControl( IF_NPCINV_WIN_OK_BTN_0 );
	m_cancelBtn->PrepareControl( IF_NPCINV_WIN_CANCEL_BTN_0 );
}

int CIFNpcInvWin::WindowFrame()
{
	
	m_textCtl[IT_NPCINV_WIN_TITLE]->SetText( g_LPACK.GetMassage( 0, 642 ) );
	m_textCtl[IT_NPCINV_WIN_TITLE]->SetColor( 0xffffffff );
	m_textCtl[IT_NPCINV_WIN_OK]->SetText( g_LPACK.GetMassage( 0, 549 ) );
	m_textCtl[IT_NPCINV_WIN_OK]->SetColor( 0xffffffff );
	m_textCtl[IT_NPCINV_WIN_CANCEL]->SetText( g_LPACK.GetMassage( 0, 551 ) );
	m_textCtl[IT_NPCINV_WIN_CANCEL]->SetColor( 0xffffffff );

	return( IF_CTL_NONE );
}

void CIFNpcInvWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	
	
	m_okBtn->Render();
	m_cancelBtn->Render();

	g_ifMng->m_iconCtl->BeginFrame();
	{
		
		for( int i = 0; i < MAX_AUTO_TRADEITEMFILD; i ++ )
		{
			int itemIdx, itemTableIdx, cx, cy;
			itemIdx = g_cgv.myCharacterInfo->upgradeSourceInventory[i];
			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_NPCINV_WIN_INVEN_0_POS + i]->m_info.clientX;
				cy = m_textCtl[IT_NPCINV_WIN_INVEN_0_POS + i]->m_info.clientY;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}

	}
	g_ifMng->m_iconCtl->EndFrame();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_NPCINV_WIN_TITLE]->Render();
		m_textCtl[IT_NPCINV_WIN_OK]->Render();
		m_textCtl[IT_NPCINV_WIN_CANCEL]->Render();

		
		g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
		g_chatManager->SetReverseSize( -1 );
		g_chatManager->SetColor( 0xffffffff );
		
		for( int i = 0; i < MAX_AUTO_TRADEITEMFILD; i ++ )
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->upgradeSourceInventory[i];

			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->item[itemIdx] ) )
				{
					cx = m_textCtl[IT_NPCINV_WIN_INVEN_0_POS + i]->m_info.clientX;
					cy = m_textCtl[IT_NPCINV_WIN_INVEN_0_POS + i]->m_info.clientY;
					number = g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
					strcpy( szTemp, g_ifMng->GetNumberString( number ) );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
	}
	g_chatManager->EndFrame();

	g_ifMng->SetRenderState();

}

int CIFNpcInvWin::WindowSelection()
{
	int ret;
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFNpcInvWin::WindowProcess()
{
	int itemIdx, mouseItemIdx;
	
	int pos=-1;
	

	
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT ) 
	{

		{
			GTH_SendMessage_ItemSystem_NpcInventoryClose();
		}

		{


		}
	}
	if( m_okBtn->ProcessButton() == IF_CTL_SELECT ) 
		GTH_SendMessage_ItemSystem_AutoCraftPrepare();

	

	if( pos >= 0 )
	{
		itemIdx = g_cgv.myCharacterInfo->upgradeResultInventory;
		mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;

		if( g_input->GetLBNone() ) 
		{
			
			if( itemIdx < 0 ) return( IF_CTL_NONE );
			
			g_ifMng->InitRolloverText();
			g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[itemIdx] );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		}
		if( g_input->GetLBDown() ) 
		{
			if( mouseItemIdx < 0 && itemIdx >= 0 )
			{
				GTH_SendMessage_ItemSystem_AutoCraftItem( 1, pos );
			}
		}
		return( IF_CTL_NONE );
	}

	pos = -1;
	if( m_textCtl[IT_NPCINV_WIN_INVEN_0_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 0;
	}
	if( m_textCtl[IT_NPCINV_WIN_INVEN_1_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 1;
	}
	if( m_textCtl[IT_NPCINV_WIN_INVEN_2_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 2;
	}
	if( m_textCtl[IT_NPCINV_WIN_INVEN_3_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 3;
	}
	if( m_textCtl[IT_NPCINV_WIN_INVEN_4_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 4;
	}
	if( m_textCtl[IT_NPCINV_WIN_INVEN_5_POS]->Selection() != IF_NOT_IN_POS )
	{
		pos = 5;
	}
	
	
	if( pos < 0 || pos >= 6 )
	{
		return( IF_CTL_NONE );
	}
		
	itemIdx = g_cgv.myCharacterInfo->upgradeSourceInventory[pos];
	mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;

	
	if( g_input->GetLBNone() ) 
	{
		
		if( itemIdx < 0 ) return( IF_CTL_NONE );

		g_ifMng->InitRolloverText();
		g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[itemIdx] );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
	}

	
	if( g_input->GetLBDown() ) 
	{
		
		if( g_cgv.myCharacterInfo->curItemWeight > g_cgv.myCharacterInfo->calItemWeight )
		{
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( 0, 589 ) );
			return( IF_CTL_NONE );
		}
		if( mouseItemIdx < 0 && itemIdx < 0 ) return IF_CTL_NONE;

		GTH_SendMessage_ItemSystem_AutoCraftItem( 0, pos );
	}

	return IF_CTL_NONE;	
}

void CIFNpcInvWin::InitNpcInventory()
{
	for( int i = 0; i < MAX_AUTO_TRADEITEMFILD; i ++ )
	{
		g_cgv.myCharacterInfo->upgradeSourceInventory[i] = -1;
	}
	g_cgv.myCharacterInfo->upgradeResultInventory = -1;

	m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );



}

void CIFNpcInvWin::UndoNpcInventory()
{
	item_t *item;
	int itemIdx, inventoryPos;

	for( int i = 0; i < MAX_AUTO_TRADEITEMFILD; i ++ )
	{
		itemIdx = g_cgv.myCharacterInfo->upgradeSourceInventory[i];
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

	itemIdx = g_cgv.myCharacterInfo->upgradeResultInventory;
	if( itemIdx >= 0 )
	{
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
	
	InitNpcInventory();
	Enable( false );
}

void CIFNpcInvWin::StartNpcInventory( float upgradeMaxTime )
{




}












CIFQuestScrollWin::CIFQuestScrollWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );

	
	for (int i=0; i<NUM_IT_QUESTSCROLL_WIN; i++)
	{
		m_textCtl[i] = new CIFText( this );
	}

	
	m_comboDepart = new CIFComboBox( this );
	m_comboLevel = new CIFComboBox( this );
	m_comboReward = new CIFComboBox( this );

	
	m_progressWin = new CIFControl( this );
	m_progressBar = new CIFBarCtl( this );
	m_progressAni = new CIFControl( this );

	
	m_okBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );

	m_physics = new CInterfacePhysics;

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_QUESTSCROLL_WINDOW_TYPE );

	m_writingFlag = false;
	m_writingMaxTime = 1.0f;
	m_writingCurrentTime = 0.0f;

	m_itemAniFlag = false;
	m_itemAniMaxTime = 1.0f;
	m_itemAniCurrentTime = 0.0f;

	m_completeFlag = false;
	m_viewItemFlag = false;
}

CIFQuestScrollWin::~CIFQuestScrollWin()
{

}

void CIFQuestScrollWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_QUESTSCROLL_WIN );

	
	m_titleBar->PrepareControl( IF_QUESTSCROLL_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_QUESTSCROLL_WIN_TITLE_BAR_BOTTOM );

	
	m_textCtl[IT_QUESTSCROLL_WIN_TITLE]->PrepareControl( IF_QUESTSCROLL_WIN_TITLE_BAR_TEXT );
	m_textCtl[IT_QUESTSCROLL_WIN_DEPART]->PrepareControl( IF_QUESTSCROLL_WIN_DEPART_TEXT );
	m_textCtl[IT_QUESTSCROLL_WIN_LEVEL]->PrepareControl( IF_QUESTSCROLL_WIN_LEVEL_TEXT );
	m_textCtl[IT_QUESTSCROLL_WIN_REWARD]->PrepareControl( IF_QUESTSCROLL_WIN_REWARD_TEXT );
	m_textCtl[IT_QUESTSCROLL_WIN_PROGRESS]->PrepareControl( IF_QUESTSCROLL_WIN_PROGRESS_TEXT );
	m_textCtl[IT_QUESTSCROLL_WIN_OK]->PrepareControl( IF_QUESTSCROLL_WIN_OK_TEXT );
	m_textCtl[IT_QUESTSCROLL_WIN_CANCEL]->PrepareControl( IF_QUESTSCROLL_WIN_CANCEL_TEXT );

	
	m_comboDepart->PrepareControl( IF_QUESTSCROLL_WIN_DEPART_COMBOBOX, 95 );
	m_comboLevel->PrepareControl( IF_QUESTSCROLL_WIN_LEVEL_COMBOBOX, 95 );
	m_comboReward->PrepareControl( IF_QUESTSCROLL_WIN_REWARD_COMBOBOX, 95 );

	
	m_progressWin->PrepareControl( IF_QUESTSCROLL_WIN_PROGRESS_WIN );
	m_progressBar->PrepareControl( IF_QUESTSCROLL_WIN_PROGRESS_BAR );
	m_progressAni->PrepareAnimControl( IF_QUESTSCROLL_WIN_PROGRESS_ANIM_0, 4, 500.0f );

	
	m_okBtn->PrepareControl( IF_QUESTSCROLL_WIN_OK_BTN_0 );
	m_cancelBtn->PrepareControl( IF_QUESTSCROLL_WIN_CANCEL_BTN_0 );

}

void CIFQuestScrollWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	
	
	m_okBtn->Render();
	m_cancelBtn->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_QUESTSCROLL_WIN_TITLE]->Render();
		m_textCtl[IT_QUESTSCROLL_WIN_DEPART]->Render();
		m_textCtl[IT_QUESTSCROLL_WIN_LEVEL]->Render();
		m_textCtl[IT_QUESTSCROLL_WIN_REWARD]->Render();
		m_textCtl[IT_QUESTSCROLL_WIN_OK]->Render();
		m_textCtl[IT_QUESTSCROLL_WIN_CANCEL]->Render();

	}
	g_chatManager->EndFrame();

	if ( m_itemAniFlag || m_viewItemFlag )
	{
		g_ifMng->SetRenderState();
		g_ifMng->m_iconCtl->BeginFrame();
		{
			int x, y, s_x, s_y;
			float dy;

			s_x = this->m_info[0].clientX + 91;
			s_y = this->m_info[0].clientY + 144;

			m_physics->GetCurrentPosition( 0, dy );

			x = s_x;
			y = (int)dy;

			g_ifMng->m_iconCtl->DrawPart( x, y, s_x, s_y, IF_ICON_ITEM, g_itemTable[QUESTSCROLL_ITEM_INDEX].iconItemID );
		}
		g_ifMng->m_iconCtl->EndFrame();
	}

	g_ifMng->SetRenderState();
	
	m_comboReward->Render();
	m_comboLevel->Render();
	m_comboDepart->Render();

	
	if( m_writingFlag )
	{
		m_progressWin->Render();
		m_progressBar->Render();
		m_progressAni->Render();
		
		g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
		g_chatManager->BeginFrame();
		{
			m_textCtl[IT_QUESTSCROLL_WIN_PROGRESS]->Render();
		}
		g_chatManager->EndFrame();
	}

}

int CIFQuestScrollWin::WindowFrame()
{

	
	m_textCtl[IT_QUESTSCROLL_WIN_TITLE]->SetText( g_LPACK.GetMassage( 0, 741 ) );
	m_textCtl[IT_QUESTSCROLL_WIN_TITLE]->SetColor( 0xffffffff );

	m_textCtl[IT_QUESTSCROLL_WIN_DEPART]->SetText( g_LPACK.GetMassage( 0, 742 ) );
	m_textCtl[IT_QUESTSCROLL_WIN_DEPART]->SetColor( 0xffffffff );

	m_textCtl[IT_QUESTSCROLL_WIN_LEVEL]->SetText( g_LPACK.GetMassage( 0, 743 ) );
	m_textCtl[IT_QUESTSCROLL_WIN_LEVEL]->SetColor( 0xffffffff );

	m_textCtl[IT_QUESTSCROLL_WIN_REWARD]->SetText( g_LPACK.GetMassage( 0, 744 ) );
	m_textCtl[IT_QUESTSCROLL_WIN_REWARD]->SetColor( 0xffffffff );

	m_textCtl[IT_QUESTSCROLL_WIN_PROGRESS]->SetText( g_LPACK.GetMassage( 0, 747 ) );
	m_textCtl[IT_QUESTSCROLL_WIN_PROGRESS]->SetColor( 0xffffffff );

	m_textCtl[IT_QUESTSCROLL_WIN_OK]->SetText( g_LPACK.GetMassage( 0, 745 ) );
	m_textCtl[IT_QUESTSCROLL_WIN_OK]->SetColor( 0xff000000 );

	m_textCtl[IT_QUESTSCROLL_WIN_CANCEL]->SetText( g_LPACK.GetMassage( 0, 746 ) );
	m_textCtl[IT_QUESTSCROLL_WIN_CANCEL]->SetColor( 0xff000000 );

	m_progressBar->UpdateBarState( (int)( m_writingMaxTime - m_writingCurrentTime ), (int)m_writingMaxTime );

	return IF_CTL_NONE;

}

int CIFQuestScrollWin::FindDepartIndex()
{
	int depart, k = -1;
	for ( int i=0; i<3; i++ )
	{
		if ( m_dptList[i] == 1 )
		{
			k++;
			if (k == m_comboDepart->m_curSel)
				depart = i;
		}
	}

	return depart;
}

int CIFQuestScrollWin::WindowProcess()
{
	int depart, level, reward;
	char str[16];

	depart = FindDepartIndex();
	m_comboLevel->ReadCurrentString( str );
	level = atoi( str );
	reward = m_comboReward->m_curSel;

	if ( m_completeFlag )
	{
		GTH_SendMessage_QuestScroll_ObtainComplete( depart, level, reward, m_selectedQSTableIdx	);
		InitWriting();
		m_completeFlag = false;
	}

	if ( m_itemAniFlag )
	{
		m_itemAniCurrentTime -= g_timer.GetElapsedMilliTime();
		if ( m_itemAniCurrentTime < 0.0f )
		{
			m_itemAniFlag = false;
			m_completeFlag = true;
		}
	}

	if( m_writingFlag )
	{
		m_writingCurrentTime -= g_timer.GetElapsedMilliTime();
		if( m_writingCurrentTime < 0.0f )
		{
			StartItemAni();
		}
	}

	
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		if( !m_writingFlag && !m_itemAniFlag ) 
		{
			GTH_SendMessage_QuestScroll_ObtainClose();
		}
		else
		{
			InitWriting();
		}
	}
	
	if( m_okBtn->ProcessButton() == IF_CTL_SELECT )
	{
 		GTH_SendMessage_QuestScroll_ObtainPrepare( depart, level, reward );
	}

	if( m_writingFlag ) return( IF_CTL_NONE );

	switch ( m_comboDepart->ProcessComboBox() )
	{
	case IF_CTL_EVENT :
		GTH_SendMessage_QuestScroll_ComboListChange( FindDepartIndex() );
		return( IF_CTL_NONE );

	case IF_CTL_SELECT :
		return( IF_CTL_NONE );

	default :
		break;
	}

	if( m_comboLevel->ProcessComboBox() == IF_CTL_SELECT )
	{
		return( IF_CTL_NONE );
	}

	if( m_comboReward->ProcessComboBox() == IF_CTL_SELECT )
	{
		return( IF_CTL_NONE );
	}

	return IF_CTL_NONE;

}

int CIFQuestScrollWin::WindowSelection()
{
	int ret;

	if ( this != g_ifMng->m_winControl[0] )
	{
		m_comboDepart->m_bExpansion = false;
		m_comboLevel->m_bExpansion = false;
		m_comboReward->m_bExpansion = false;
	}
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return IF_CTL_NONE;
}

void CIFQuestScrollWin::InitWriting()
{
	m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	m_cancelBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

	m_writingFlag = false;
	m_itemAniFlag = false;
	m_viewItemFlag = false;
}

void CIFQuestScrollWin::StartWriting( float writingMaxTime )
{
	m_writingMaxTime = writingMaxTime;
	m_writingCurrentTime = writingMaxTime;
	m_progressBar->UpdateBarState( m_writingMaxTime - m_writingCurrentTime, (int)m_writingMaxTime );

	m_writingFlag = true;

	m_okBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	
	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_QUESTSCROLL_WRITING ), GTH_SOUND_PLAY_ONCE );
}

void CIFQuestScrollWin::StartItemAni()
{
	m_itemAniFlag = true;
	m_viewItemFlag = true;

	m_itemAniCurrentTime = 2000.0f;

	int sp, ep;

	ep = this->m_info[0].clientY + 144;
	sp = ep - 32;
	
	m_physics->SetInterfacePhysics( 0, sp, ep, 0.0f, 0.0004f, g_timer.GetAppMilliTime(), 2 );
	m_physics->SetRestitution( 0, 0.7f );

	m_writingFlag = false;

	m_cancelBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	
	g_musicMng->PlaySample( 
		g_soundMng->GetNormalSoundID( GTH_SOUND_ID_QUESTSCROLL_DROP ), GTH_SOUND_PLAY_ONCE );
}











CIFCashMallItem::CIFCashMallItem( CIFControlNode *pParentControl) : CIFWindow( pParentControl )
{	
	m_titleBar			= new CIFControl (this);	
	m_BottomBar			= new CIFControl (this);
	m_BottomBgBar		= new CIFControl (this);
	m_SelectBar			= new CIFControl (this);
	m_OkBtn				= new CIFButtonCtl (this);
	m_Cancel			= new CIFButtonCtl (this);
	m_Exitbtn			= new CIFButtonCtl (this);
	m_Myinven_bg		= new CIFButtonCtl (this);

	
	for (int iTextIdx = 0; iTextIdx < MAX_NUM_TEXT_CONTROL; iTextIdx++)
		m_textCtl[iTextIdx] = new CIFText (this);

	m_ItemIven			= new CIFControl (this);	
	m_OptionListPos		= new CIFText (this);	

	Enable( false );
	g_ifMng->AddFocus( this );
	SetWindowType(IF_CASH_MALLITEM_WINDOW_TYPE);

	

	m_ResizeRatio = 1.0f;
	
	m_MaxOptionStrLen = 0;
	
}







void CIFCashMallItem::PrepareControl()
{
	CIFControl::PrepareControl( IF_CASH_MALLITEM );		

	m_titleBar->PrepareControl( IF_CASH_MALLITEM_TITLE_BAR );
	m_BottomBar->PrepareControl( IF_CASH_MALLITEM_BUTTON_BAR );		
	m_BottomBgBar->PrepareControl( IF_CASH_MALLITEM_TITLE_BAR_BOTTOM );
	
	m_textCtl[TITLE_ITEM_NAME]->PrepareControl( IF_CASH_MALLITEM_TITLE_TEXT );	
	m_textCtl[TITLE_CASHMAILL_DOC]->PrepareControl( IF_CASH_MALLITEM_TITLE_MESSAGE_TEXT) ;		

	

	m_OkBtn->PrepareControl( IF_CASH_MALLITEM_OK_BTN0 );
	m_Cancel->PrepareControl( IF_CASH_MALLITEM_CANCEL_BTN0 );
	m_Exitbtn->PrepareControl( IF_CASH_MALLITEM_CLOSE_BTN_0 );

	m_SelectBar->PrepareControl( IF_CASH_MALLITEM_SELECT_BAR );			
	m_ItemIven->PrepareControl( IF_CASH_MALLITEM_MYINVEN );
	m_OptionListPos->PrepareControl( IF_CASH_MALLITEM_LIST_POS );
	m_Myinven_bg->PrepareControl( IF_CASH_MALLITEM_MYINVEN_BG );


	
	IFControlInfo_t* pIfControlInfo = NULL;
	pIfControlInfo = &g_ifMng->m_iPos[IF_CASH_MALLITEM_OK_BTN0];		
	m_OkBtnPos.x  = pIfControlInfo->clientX;
	m_OkBtnPos.y  = pIfControlInfo->clientY;

	pIfControlInfo = &g_ifMng->m_iPos[IF_CASH_MALLITEM_CANCEL_BTN0];		
	m_CanBtnPos.x  = pIfControlInfo->clientX;
	m_CanBtnPos.y  = pIfControlInfo->clientY;

	pIfControlInfo = &g_ifMng->m_iPos[IF_CASH_MALLITEM_CLOSE_BTN_0];		
	m_ExitBtnPos.x  = pIfControlInfo->clientX;
	m_ExitBtnPos.y  = pIfControlInfo->clientY;

	pIfControlInfo = &g_ifMng->m_iPos[IF_CASH_MALLITEM_MYINVEN];	
	m_ItemIvenPos.x  = pIfControlInfo->clientX;
	m_ItemIvenPos.y  = pIfControlInfo->clientY;





	
	



}







int CIFCashMallItem::WindowFrame()
{
	if ( State() == FALSE )
		return 0;


	
	m_textCtl[TITLE_ITEM_NAME]->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 521));

	
	m_Cancel->SetButtonText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 521 ));
	
	m_OkBtn->SetButtonText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 549 ));	

	if ( g_cgv.myCharacterInfo != NULL )
	{
		switch(g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemMode)
		{
		case CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE:
			
			m_textCtl[TITLE_CASHMAILL_DOC]->SetText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 522 ));	
			
			break;
			
		case CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY:
			
			m_textCtl[TITLE_CASHMAILL_DOC]->SetText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 523 ));	
			break;

		case CashMallItem_OptionStone::enumUseItemMode::STONE_OF_EACH_NAME:
			
			m_textCtl[TITLE_CASHMAILL_DOC]->SetText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 524 ));	
			break;
		}		
	}

	if ( m_iSelectIdx >= 0)
	{	 
		m_SelectBar->Enable(TRUE);
		
		g_ifMng->m_iPos[IF_CASH_MALLITEM_SELECT_BAR].clientY = 
			g_ifMng->m_iPos[IF_CASH_MALLITEM_LIST_POS].clientY +
			( ( m_iSelectIdx  ) * OPTION_LIST_WIN_HEIGHT);		
		
		m_SelectBar->UpdateControl();
		m_SelectBar->UpdateBuffer();
		
	}
	else 
		m_SelectBar->Enable(FALSE);	


	
	return 1;

}






void CIFCashMallItem::Render()
{
	
	if( !State() ) return;		

	g_ifMng->SetRenderState();	

	
	ReSizeWindow();	
	

	CIFControl::Render();
	m_titleBar->Render();	
	m_BottomBgBar->Render();

	m_textCtl[TITLE_ITEM_NAME]->Render();
	m_textCtl[TITLE_CASHMAILL_DOC]->Render();	
	
	m_ItemIven->Render();

	m_OkBtn->RenderTextBtn();
	m_Cancel->RenderTextBtn();
	m_Exitbtn->Render();
	

	if ( g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx < 0 ||
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx > MAX_NUMBER_OF_OWNITEM )
		return;

	int ItemIdx = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx;	
	
	int ItemTableIdx = g_cgv.myCharacterInfo->item[ItemIdx].itemTableIdx;

	if ( ItemTableIdx > 0 || ItemTableIdx < MAX_NUMBER_OF_ITEM_TABLE)
	{		
		g_ifMng->m_iconCtl->BeginFrame();
		{	
			g_ifMng->m_iconCtl->Draw(m_ItemIven->GetXPos() + 2, m_ItemIven->GetYPos() + 2,
				IF_ICON_ITEM, g_itemTable[ItemTableIdx].iconItemID);
		}
		g_ifMng->m_iconCtl->EndFrame();	
	}
	m_SelectBar->Render();	
	
	if ( g_itemTable[ItemTableIdx].wearPosition != ITEM_WEAR_POS_EPACK &&
		ItemTableIdx != SUMMONCARD_ITEM_INDEX)
	{	
		g_chatManager->BeginFrame();	
		{
			g_chatManager->SetColor( GTHCOLOR_TRADEMESSAGE );
			m_MaxOptionStrLen = 0;
			RenderItemOptionList();	
			g_chatManager->SetColor( 0xff000000 ) ;
		}
		g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
		g_chatManager->EndFrame();		
	}

	m_SelectBar->Render();

}










void CIFCashMallItem::SortItemOptionList()
{	

	item_t *pItem = NULL;		
	pItem = &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx];		

	int *pOptionArry = NULL;
	int *pOptionValueArry = NULL;

	pOptionArry = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iSortOptionIdx;	
	pOptionValueArry = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iSortOptionValue;		

	if ( pOptionArry == NULL)
		return;

	if ( pItem == NULL)
		return;

	if ( pOptionValueArry == NULL)
		return;
	
	int itemTableIdx = pItem->itemTableIdx;
	int wearPosition = g_itemTable[itemTableIdx].wearPosition;	

	
	
	if( g_itemTable[itemTableIdx].etcItemType == ETCITEM_TYPE_OPTIONSPHERE )
		return;

	
	for( int OptionArryIdx = 0; OptionArryIdx < pItem->optionNumber; OptionArryIdx ++ )
	{
		pOptionArry[OptionArryIdx] = pItem->optionIdx[OptionArryIdx];

		if( pOptionArry[OptionArryIdx] < 0 ) 
			pOptionValueArry[OptionArryIdx] = 0;

		else if( pItem->optionValue[OptionArryIdx] > 10 ) 
			pOptionValueArry[OptionArryIdx] = 10;

		else pOptionValueArry[OptionArryIdx] = pItem->optionValue[OptionArryIdx];		
	}

	 
	for( OptionArryIdx = 0; OptionArryIdx < pItem->optionNumber; OptionArryIdx ++ )
	{
		for( int OptionArryIdx2 = OptionArryIdx; OptionArryIdx2 < pItem->optionNumber; OptionArryIdx2 ++ )
		{			
			if( pOptionArry[OptionArryIdx] < pOptionArry[OptionArryIdx2] )
			{
				int temp = pOptionArry[OptionArryIdx];
				pOptionArry[OptionArryIdx] = pOptionArry[OptionArryIdx2];
				pOptionArry[OptionArryIdx2] = temp;

				temp = pOptionValueArry[OptionArryIdx];
				pOptionValueArry[OptionArryIdx] = pOptionValueArry[OptionArryIdx2];
				pOptionValueArry[OptionArryIdx2] = temp;
			}
		}
	}
}






void CIFCashMallItem::RenderItemOptionList()
{	
	item_t *pItem = NULL;		
	pItem = &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx];	

	int *pOptionArry = NULL;
	int *pOptionValueArry = NULL;
	pOptionArry = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iSortOptionIdx;	
	pOptionValueArry = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iSortOptionValue;	
	

	if ( pOptionArry == NULL)
		return;

	if ( pOptionValueArry == NULL)
		return;


	if ( pItem == NULL)
		return;

	char szTemp[256], szTemp2[256];	
	int itemTableIdx = pItem->itemTableIdx;
	int wearPosition = g_itemTable[itemTableIdx].wearPosition;	
	
	if( g_itemTable[itemTableIdx].etcItemType == ETCITEM_TYPE_OPTIONSPHERE ) return;
	int optColor, optSphereType;

	optSphereType = OPTION_GetOptionStoneGrade( pItem );
	
	if( pItem->optionNumber > 0 )
	{
		
		if( !( wearPosition == ITEM_WEAR_POS_RING || wearPosition == ITEM_WEAR_POS_NECKLACE ||
			wearPosition == ITEM_WEAR_POS_BRACELET || wearPosition == ITEM_WEAR_POS_EARRING ) )
		{
			sprintf( szTemp, g_LPACK.GetMessage(0,932), g_LPACK.GetMessage( 0, 778 + optSphereType ) );			
		}		
	}
	
	for( int OptionArryIdx = 0; OptionArryIdx < pItem->optionNumber; OptionArryIdx ++ )
	{		
		memset( szTemp2, 0, sizeof(szTemp2) );
	
		if( strstr( g_optionTable[pOptionArry[OptionArryIdx]].printing, "%" ) != NULL )
		{
			sprintf( szTemp, g_optionTable[pOptionArry[OptionArryIdx]].printing, 
				(int)OPTION_GetOptionValue( pItem, pOptionArry[OptionArryIdx], 
				pOptionValueArry[OptionArryIdx] ) );
		}
		else
		{
			strcpy( szTemp, g_optionTable[pOptionArry[OptionArryIdx]].printing );
		}
		
		optColor = 2;		

		if( optSphereType >= 0 && optSphereType < 5 )
		{
			sprintf( szTemp2, "[+%d] %s", pOptionValueArry[OptionArryIdx], szTemp );
			optColor = 2;

		}
		else
		{
			sprintf( szTemp2, "%s", szTemp );
		}

		g_chatManager->DrawText( m_OptionListPos->GetXPos() + 4, 
		m_OptionListPos->GetYPos() + (OptionArryIdx * OPTION_LIST_WIN_HEIGHT),
		szTemp2, -1, 13, CHAT_ARRANGE_NONE );

		int maxSizeY, maxSizeX;
		g_chatManager->GetTextRect( -1, 13, szTemp2, maxSizeX, maxSizeY);		

		if ( m_MaxOptionStrLen < maxSizeX)
			m_MaxOptionStrLen = maxSizeX;

	}
}






int CIFCashMallItem::ItemProcess()
{
	if ( g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx < 0 ||
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx > MAX_NUMBER_OF_OWNITEM )
		return( IF_CTL_NONE );

	int ItemIdx = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx;
	
	g_ifMng->InitRolloverText();
	g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[ItemIdx] );
	g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
}








int CIFCashMallItem::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();

	if( ret == IF_LBDN ) 
		return( IF_CTL_BEGIN_MOVING );

	ret = Selection();

	if( ret == IF_LBDN ) 
		return( IF_CTL_SELECT );


	

		
	int tempSelectIdx = -1;
	POINT		MousePoint;	

	MousePoint.x = g_input->GetPosX();
	MousePoint.y = g_input->GetPosY();

	RECT CheckRect;	
	CheckRect.left		= m_OptionListPos->GetXPos();
	CheckRect.top		= m_OptionListPos->GetYPos();
	CheckRect.right		= m_OptionListPos->GetXPos() + m_OptionListPos->GetXSize();
	CheckRect.bottom	= m_OptionListPos->GetYPos() +
						( MAX_NUMBER_OF_ITEM_OPTION * OPTION_LIST_WIN_HEIGHT ) ;


	
	if ( PtInRect( &CheckRect, MousePoint) )	
		tempSelectIdx = (MousePoint.y - CheckRect.top) / (OPTION_LIST_WIN_HEIGHT);	
	else 
		tempSelectIdx = -1;


	
	 if ( g_input->GetLBUp() )		 
	 {	 
		 if ( !m_OkBtn->Selection() )
			m_iSelectIdx = tempSelectIdx;

	 }
	 
	 

	return( IF_CTL_NONE );
}







item_t* CIFCashMallItem::GetEditItem()
{
	int ItemIdx = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx;

	if ( ItemIdx <0 || ItemIdx >= MAX_NUMBER_OF_OWNITEM)
		return NULL;

	return &g_cgv.myCharacterInfo->item[ItemIdx];
}







int CIFCashMallItem::FIndOptionArryIdx()
{	
	int ItemIdx = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx;

	if ( ItemIdx <0 || ItemIdx >= MAX_NUMBER_OF_OWNITEM)
		return -1;

	if ( m_iSelectIdx <0 ||	m_iSelectIdx >= MAX_NUMBER_OF_ITEM_OPTION)
		return -1;

	int OptionTableIdx = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iSortOptionIdx[m_iSelectIdx];
	
	for (int i= 0; i < MAX_NUMBER_OF_ITEM_OPTION; i++)
	{								
		if (OptionTableIdx == g_cgv.myCharacterInfo->item[ItemIdx].optionIdx[i])			
		{			 
			return i;		
		}
	}

	return -1;
}






void CIFCashMallItem::GetItemOptionString(char* szText, int Len, int OptionArryIdx)
{	
	if ( Len < 1)
		return;

	char szMsg2[256];
	
	item_t* pItem = NULL;
	pItem = GetEditItem();
	
	if ( pItem == NULL)
		return;
	
	if ( OptionArryIdx < 0 || OptionArryIdx >= MAX_NUMBER_OF_ITEM_OPTION)
		return;

	int OptionTableIdx = pItem->optionIdx[OptionArryIdx];

	if ( OptionTableIdx < 0 || OptionTableIdx>= MAX_COUNT_OPTION_TABLE)
		return;
	
	if( strstr( g_optionTable[OptionTableIdx].printing, "%" ) != NULL )
	{
		_snprintf(szMsg2, 256, g_optionTable[OptionTableIdx].printing, 
			(int)OPTION_GetOptionValue( pItem, OptionTableIdx, pItem->optionValue[OptionArryIdx] ) );;						
	}
	else
	{			
		_snprintf(szMsg2, 256, g_optionTable[OptionTableIdx].printing);
	}
	szText[255] = NULL;
	
	int optSphereType = OPTION_GetOptionStoneGrade( pItem );

	if( optSphereType >= 0 && optSphereType < 5 )
	{		 
		_snprintf( szText, Len, "[+%d] %s", pItem->optionValue[OptionArryIdx], szMsg2 );			
	}
	else
	{
		_snprintf( szText, Len, "%s", szMsg2 );
	}
	
	szText[Len-1] = NULL;

}





int CIFCashMallItem::WindowProcess()
{

	if( m_Exitbtn->ProcessButton() == IF_CTL_SELECT )
	{

		GTH_SendMessageRequest_OptionItemWinClose();
		return IF_CTL_SELECT;
	}

	if( m_Cancel->ProcessButton() == IF_CTL_SELECT )
	{			
		GTH_SendMessageRequest_OptionItemWinClose();
		return IF_CTL_SELECT;
	}

	if( m_OkBtn->ProcessButton() == IF_CTL_SELECT )
	{			
		m_OptionArryIdx = FIndOptionArryIdx();


		CashMallItem_OptionStone::enumUseItemMode mode = 
			g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemMode;

		char szOptionText[256];
		char szMsg[256];		

		switch ( mode )
		{
		case CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE:
			{
				
				if ( m_OptionArryIdx == -1)
				{
					
					g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), 
						g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 525 ));			
					return IF_CTL_SELECT;
				}				

				GetItemOptionString(szOptionText, 256, m_OptionArryIdx);
				



				
						
				_snprintf( szMsg, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 526)
					,szOptionText,g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_CashMallItem_SuccecsRate,
					 g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_CashMallItem_DestroyRate);

			}
			break;
		case CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY:
			{				
				
				
				_snprintf( szMsg, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 527),
					g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_CashMallItem_SuccecsRate,
					g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_CashMallItem_DestroyRate);

			}
			break;
		case CashMallItem_OptionStone::enumUseItemMode::STONE_OF_EACH_NAME:
			{
				
				if ( m_OptionArryIdx == -1)
				{
					
					g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), 
						g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 525 ));			
					return IF_CTL_SELECT;
				}
				GetItemOptionString(szOptionText, 256, m_OptionArryIdx);
				
				_snprintf( szMsg, 256, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 528 ),szOptionText);
			}
			break;
		}		
		szMsg[255] = NULL;		

		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szMsg, 
			MSG_BUTTONTYPE_OKCANCEL ,IF_MESSAGE_MODIFY_OPTION);			

		return IF_CTL_SELECT;		
	}




	POINT MousePoint;	
	MousePoint.x = g_input->GetPosX();	
	MousePoint.y = g_input->GetPosY();		
	
	int ItemIdx = -1;
	int ItemInven = -1;

	if ( g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx >= 0 &&
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx < MAX_NUMBER_OF_OWNITEM )		
	{			
		ItemIdx = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx;
		ItemInven = g_cgv.myCharacterInfo->mouseInventory;		
		
		int ItemTableIdx = g_cgv.myCharacterInfo->item[ItemIdx].itemTableIdx;
		g_ifMng->m_iconCtl->Draw(m_ItemIven->GetXPos(), m_ItemIven->GetYPos(),
			IF_ICON_ITEM, g_itemTable[ItemTableIdx].iconItemID);
	}


	RECT EditCashInven;
	EditCashInven.left	= m_ItemIven->GetXPos();
	EditCashInven.top	= m_ItemIven->GetYPos();
	EditCashInven.right = m_ItemIven->GetXPos() + m_ItemIven->GetXSize();
	EditCashInven.bottom= m_ItemIven->GetYPos() + m_ItemIven->GetYSize();


	
	

	if ( PtInRect(&EditCashInven, MousePoint ) == TRUE )
	{		
		
		if( ( g_input->GetLBNone()) )	
		{
			if ( ItemIdx>= 0 &&	ItemIdx < MAX_NUMBER_OF_OWNITEM )						
			{			
				g_ifMng->InitRolloverText();
				g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[ItemIdx] );
				g_ifMng->MakeRolloverText( MousePoint.x, MousePoint.y, true, 250 );
			}
		}

		if( (g_input->GetLBUp() ) )
		{	
			if( ItemIdx < 0 && g_cgv.myCharacterInfo->mouseInventory < 0 )		
				return( IF_CTL_NONE );
			else				
				GTH_SendMessageRequest_OptionItemEdit();
		}
	}
}







void CIFCashMallItem::InitCashMallItem()
{
	m_iSelectIdx					= -1;
	m_selectBarSavePos				= -1;
	

}











void	CIFCashMallItem::MoveControl(IFControlInfo_t* pIfControlInfo, 
		CIFControl* pIfControl, float XMoveRatio, float YMoveRatio, POINT &OriginalPos)
{
	if ( pIfControlInfo == NULL  )
		return;

	if ( pIfControl == NULL)
		return;	

	pIfControlInfo->clientX = OriginalPos.x + ( OriginalPos.x * XMoveRatio);	
	pIfControlInfo->clientY = OriginalPos.y + ( OriginalPos.y * YMoveRatio);

	pIfControl->UpdateControl();	
	pIfControl->UpdateBuffer();


	
	
	
}



void CIFCashMallItem::ReSizeWindow()
{	
	

	
	if ( m_MaxOptionStrLen > CIFControl::GetXSize() )	
	{		
		
		m_ResizeRatio = ( (  ( (float)m_MaxOptionStrLen + 20 ) 
			- (float)CIFControl::GetXSize() ) 
			/(float)CIFControl::GetXSize() );
	}				
	else
		m_ResizeRatio = 0.0F;
	
	
	if ( GetEditItem() == NULL )			
		m_ResizeRatio = 0.0F;


	
	CIFControl::UpdateControl();
	CIFControl::UpdateBuffer( CIFControl::GetXSize()+ 
		(CIFControl::GetXSize() * m_ResizeRatio), 300 );

	m_BottomBgBar->UpdateControl();
	m_BottomBgBar->UpdateBuffer( m_BottomBgBar->GetXSize() + 
		(m_BottomBgBar->GetXSize() * m_ResizeRatio), 
		m_BottomBgBar->GetYSize() );	
	
	
	m_titleBar->UpdateControl();
	m_titleBar->UpdateBuffer( m_titleBar->GetXSize() + 
		(m_titleBar->GetXSize() * m_ResizeRatio), 
		m_titleBar->GetYSize() );	
	
	
	m_SelectBar->UpdateControl();
	m_SelectBar->UpdateBuffer(m_SelectBar->GetXSize() + 
		(m_SelectBar->GetXSize() * m_ResizeRatio), m_SelectBar->GetYSize());


	

	
	
	IFControlInfo_t* pIfControlInfo = NULL;
	CIFControl*	pIfControl			= NULL;

	pIfControl = m_Cancel;	
	for ( int i = IF_CASH_MALLITEM_CANCEL_BTN0; i < IF_CASH_MALLITEM_CANCEL_BTN0 + 3 ; i++)	
		MoveControl(&g_ifMng->m_iPos[i], pIfControl, m_ResizeRatio  , 0, m_CanBtnPos);			

	pIfControl = m_Exitbtn;
	for ( i = IF_CASH_MALLITEM_CLOSE_BTN_0; i < IF_CASH_MALLITEM_CLOSE_BTN_0 + 3 ; i++)	
		MoveControl(&g_ifMng->m_iPos[i], pIfControl, m_ResizeRatio , 0, m_ExitBtnPos);	

	pIfControl = m_ItemIven;	
	MoveControl(&g_ifMng->m_iPos[IF_CASH_MALLITEM_MYINVEN], pIfControl, 
		m_ResizeRatio , 0, m_ItemIvenPos);	

	pIfControl = m_OkBtn;
	for ( i = IF_CASH_MALLITEM_OK_BTN0; i < IF_CASH_MALLITEM_OK_BTN0 + 3 ; i++)	
		MoveControl(&g_ifMng->m_iPos[i], pIfControl, m_ResizeRatio , 0, m_OkBtnPos);		

	
	
	
	
}

