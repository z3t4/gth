


#include "../global_def.h"
#include "IFCommon.h"





CIFEventCraftWin::CIFEventCraftWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	int i;

	
	for(i = 0; i < NUM_IT_EVENT_CRAFT_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	
	
	for(i=0;i< MAX_EVENT_CRAFT_ITEM;i++)
	{
		m_ifcItem[i] = new CIFControl( this );
	}

	
	m_progressWin = new CIFControl( this );
	m_progressBar = new CIFBarCtl( this );
	m_progressAni = new CIFControl( this );

	
	m_okBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );

	
	m_ResultWin = new CIFControl( this );

	m_craftingFlag = false;
	m_craftCurrentTime = 0.0f;
	m_craftMaxTime = 1.0f;

	m_nSelectEventCraftIndex=-1;

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_EVENT_CRAFT_WINDOW_TYPE );
}

CIFEventCraftWin::~CIFEventCraftWin()
{

}

void CIFEventCraftWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_EVENT_CRAFT_WIN );

	m_textCtl[IT_EVENT_CRAFT_WIN_TITLE]->PrepareControl( IF_EVENT_CRAFT_WIN_TITLE_TEXT );

	m_textCtl[IT_EVENT_CRAFT_WIN_INVEN_0_POS]->PrepareControl( IF_EVENT_CRAFT_WIN_INVEN_0 );
	m_textCtl[IT_EVENT_CRAFT_WIN_INVEN_1_POS]->PrepareControl( IF_EVENT_CRAFT_WIN_INVEN_1 );
	m_textCtl[IT_EVENT_CRAFT_WIN_INVEN_2_POS]->PrepareControl( IF_EVENT_CRAFT_WIN_INVEN_2 );
	m_textCtl[IT_EVENT_CRAFT_WIN_INVEN_3_POS]->PrepareControl( IF_EVENT_CRAFT_WIN_INVEN_3 );

	m_textCtl[IT_EVENT_CRAFT_WIN_PROGRESS]->PrepareControl( IF_EVENT_CRAFT_WIN_PROGRESS_TEXT );
	
	m_textCtl[IT_EVENT_CRAFT_WIN_OK]->PrepareControl( IF_EVENT_CRAFT_WIN_OK_TEXT );
	m_textCtl[IT_EVENT_CRAFT_WIN_CANCEL]->PrepareControl( IF_EVENT_CRAFT_WIN_CANCEL_TEXT );

	
	m_textCtl[IF_EVENT_CRAFT_WIN_RESULT_TEXT_POS]->PrepareControl( IF_EVENT_CRAFT_WIN_RESULT_TEXT );
	m_ResultWin->PrepareControl( IF_EVENT_CRAFT_WIN_RESULT_WIN );

	
	m_titleBar->PrepareControl( IF_EVENT_CRAFT_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_EVENT_CRAFT_WIN_TITLE_BAR_BOTTOM );

	
	m_ifcItem[0]->PrepareControl( IF_EVENT_CRAFT_WIN_INVEN_0);
	m_ifcItem[1]->PrepareControl( IF_EVENT_CRAFT_WIN_INVEN_1);
	m_ifcItem[2]->PrepareControl( IF_EVENT_CRAFT_WIN_INVEN_2);
	m_ifcItem[3]->PrepareControl( IF_EVENT_CRAFT_WIN_INVEN_3);
	m_ifcItem[4]->PrepareControl( IF_EVENT_CRAFT_WIN_INVEN_4);
	m_ifcItem[5]->PrepareControl( IF_EVENT_CRAFT_WIN_INVEN_5);
	m_ifcItem[6]->PrepareControl( IF_EVENT_CRAFT_WIN_INVEN_6);
	m_ifcItem[7]->PrepareControl( IF_EVENT_CRAFT_WIN_INVEN_7);

	
	m_progressWin->PrepareControl( IF_EVENT_CRAFT_WIN_PROGRESS_WIN );
	m_progressBar->PrepareControl( IF_EVENT_CRAFT_WIN_PROGRESS_BAR );
	m_progressAni->PrepareAnimControl( IF_EVENT_CRAFT_WIN_PROGRESS_ANIM_0, 4, 300.0f );

	
	m_okBtn->PrepareControl( IF_EVENT_CRAFT_WIN_OK_BTN_0 );
	m_cancelBtn->PrepareControl( IF_EVENT_CRAFT_WIN_CANCEL_BTN_0 );
}

int CIFEventCraftWin::WindowFrame()
{	
	m_textCtl[IT_EVENT_CRAFT_WIN_TITLE]->SetText( g_LPACK.GetMassage( 9, 76 ) );
	m_textCtl[IT_EVENT_CRAFT_WIN_TITLE]->SetColor( 0xffffffff );

	m_textCtl[IT_EVENT_CRAFT_WIN_PROGRESS]->SetText( g_LPACK.GetMassage( 9, 78 ) );
	m_textCtl[IT_EVENT_CRAFT_WIN_PROGRESS]->SetColor( 0xffffffff );

	m_textCtl[IT_EVENT_CRAFT_WIN_OK]->SetText( g_LPACK.GetMassage( 9, 77 ) );
	m_textCtl[IT_EVENT_CRAFT_WIN_OK]->SetColor( 0xffffffff );

	m_textCtl[IT_EVENT_CRAFT_WIN_CANCEL]->SetText( g_LPACK.GetMassage( 0, 521 ) );
	m_textCtl[IT_EVENT_CRAFT_WIN_CANCEL]->SetColor( 0xffffffff );


	m_progressBar->UpdateBarState( (int)( m_craftMaxTime - m_craftCurrentTime ), (int)m_craftMaxTime );

	return( IF_CTL_NONE );
}

void CIFEventCraftWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	
	
	if( m_craftingFlag )
	{
		g_ifMng->SetRenderState();
		m_progressWin->Render();
		m_progressBar->Render();
		m_progressAni->Render();
		
		g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
		g_chatManager->BeginFrame();
		{
			m_textCtl[IT_EVENT_CRAFT_WIN_PROGRESS]->Render( IF_TEXT_OUTPUT_CENTER );
		}
		g_chatManager->EndFrame();
		return;
	}

	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	m_ResultWin->Render();
	
	
	m_okBtn->Render();
	m_cancelBtn->Render();

	g_ifMng->m_iconCtl->BeginFrame();
	{
		
		for( int i = 0; i < MAX_EVENT_CRAFT_ITEM; i ++ )
		{
			m_ifcItem[i]->Render();
			int itemIdx, itemTableIdx, cx, cy;
			itemIdx = g_cgv.myCharacterInfo->craftSourceInventory[i];
			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_EVENT_CRAFT_WIN_INVEN_0_POS]->m_info.clientX + ( i % 4 ) * 40;
				cy = m_textCtl[IT_EVENT_CRAFT_WIN_INVEN_0_POS]->m_info.clientY + ( i / 4 ) * 40;

				if(i / 4) cy += 2;

				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
				g_ifMng->m_iconCtl->Draw( cx+2, cy+2, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}
	}

	
	
	if(m_nSelectEventCraftIndex >= 0)
	{	
		itemCraftTable_t * pData = Get(m_nSelectEventCraftIndex);
		if(pData != NULL  && pData->m_btEventType == CRAFT_GIVE_ITEM)
		{
			int itemTableIdx= pData->m_nResultMark;
			if(itemTableIdx >= 0)
			{
				int cx  = m_textCtl[IF_EVENT_CRAFT_WIN_RESULT_TEXT_POS]->m_info.clientX;
				int cy  = m_textCtl[IF_EVENT_CRAFT_WIN_RESULT_TEXT_POS]->m_info.clientY;

				g_ifMng->m_iconCtl->Draw( cx+2, cy+2, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}
	}

	g_ifMng->m_iconCtl->EndFrame();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_EVENT_CRAFT_WIN_TITLE]->Render();
		m_textCtl[IT_EVENT_CRAFT_WIN_OK]->Render();
		m_textCtl[IT_EVENT_CRAFT_WIN_CANCEL]->Render();

		
		g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
		g_chatManager->SetReverseSize( -1 );
		g_chatManager->SetColor( 0xffffffff );
	
		
		for(int i = 0; i < MAX_EVENT_CRAFT_ITEM; i ++ )
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->craftSourceInventory[i];

			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->item[itemIdx] ) )
				{
					cx = m_textCtl[IT_EVENT_CRAFT_WIN_INVEN_0_POS]->m_info.clientX + ( i % 4 ) * 40;
					cy = m_textCtl[IT_EVENT_CRAFT_WIN_INVEN_0_POS]->m_info.clientY + ( i / 4 ) * 40;

					if(i / 4) cy += 2;

					number = g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
					strcpy( szTemp, g_ifMng->GetNumberString( number ) );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
	}
	
	
	if(m_nSelectEventCraftIndex >= 0)
	{	
		itemCraftTable_t * pData = Get(m_nSelectEventCraftIndex);
		if(pData != NULL  && pData->m_btEventType == CRAFT_GIVE_ITEM)
		{
			int itemTableIdx= pData->m_nResultMark;
			if(itemTableIdx >= 0)
			{
				int cx  = m_textCtl[IF_EVENT_CRAFT_WIN_RESULT_TEXT_POS]->m_info.clientX;
				int cy  = m_textCtl[IF_EVENT_CRAFT_WIN_RESULT_TEXT_POS]->m_info.clientY;

				char szTemp[128];
				sprintf(szTemp,"%d ",pData->m_lResultNak);

				g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
				g_chatManager->SetReverseSize( -1 );
				g_chatManager->SetColor( 0xffffffff );
				g_chatManager->TextOut( cx + 40, cy + 15, szTemp );
			}
		}
	}

	g_chatManager->EndFrame();
}

int CIFEventCraftWin::WindowSelection()
{
	int ret;
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFEventCraftWin::WindowProcess()
{
	int itemIdx, mouseItemIdx;
	int pos;
	int mx, my;

	
	if( m_craftingFlag )
	{
		m_craftCurrentTime -= g_timer.GetElapsedMilliTime();
		if( m_craftCurrentTime < 0.0f )
		{
			GTH_SendMessage_ItemSystem_EventCraftComplete();
			m_craftingFlag = false;
		}
	}

	
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		if( !m_craftingFlag ) 
		{
			GTH_SendMessage_ItemSystem_EventCraftClose();
		}
		else
		{
			m_craftingFlag = false;
			m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		}
	}
	if( m_okBtn->ProcessButton() == IF_CTL_SELECT ) GTH_SendMessage_ItemSystem_EventCraftPrepare();

	
	if( m_craftingFlag ) return( IF_CTL_NONE );

	pos = -1;

	mx = g_input->GetPosX() - m_textCtl[IT_EVENT_CRAFT_WIN_INVEN_0_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_EVENT_CRAFT_WIN_INVEN_0_POS]->GetYPos();	

	

	if( mx <= 0 || my <= 0 ) return( IF_CTL_NONE );
	if( mx >= EVENT_CRAFT_ITEM_SIZE * 4 ||  my >= EVENT_CRAFT_ITEM_SIZE * 2 ) return( IF_CTL_NONE );

	
	

	int temp,x,y;

	temp = mx / EVENT_CRAFT_ITEM_SIZE;
	x = temp % 4;
	temp = my / EVENT_CRAFT_ITEM_SIZE;
	y = temp % 2;

	pos = y * 4 + x;

	
	if( pos < 0 || pos >= MAX_EVENT_CRAFT_ITEM )
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

		GTH_SendMessage_ItemSystem_EventCraftItem( 0, pos );

	}		


	return IF_CTL_NONE;
}

int CIFEventCraftWin::GetEventCraftFormula()
{
	int i, j, k, itemIdx;
	int itemTableIdx1, itemTableIdx2, checkedNumber;
	int numSourceItem, checkedItem[MAX_EVENT_CRAFT_ITEM], checkedIdx[MAX_EVENT_CRAFT_ITEM];
	item_t *sourceItem;
	
	
	numSourceItem = 0;
	for( i = 0; i < MAX_EVENT_CRAFT_ITEM; i ++ )
	{
		if( g_cgv.myCharacterInfo->craftSourceInventory[i] < 0 ) continue;
		
		sourceItem = &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->craftSourceInventory[i]];
		
		
		checkedIdx[numSourceItem] = i;
		numSourceItem ++;
	}
	if( numSourceItem == 0 ) return -1;

	for( i = 0; i < GetTableSize(); i ++ )
	{
		itemCraftTable_t *pData = Get(i);
		if(pData == NULL) break;

		
		if( numSourceItem != pData->numSourceItem ) continue;

		
		checkedNumber = numSourceItem;
		
		for( j = 0; j < MAX_EVENT_CRAFT_ITEM; j ++ )
		{
			checkedItem[j] = false;
		}
		
		for( j = 0; j < MAX_EVENT_CRAFT_ITEM; j ++ )
		{
			for( k = 0; k < numSourceItem; k ++ )
			{
				
				if( checkedItem[k] ) continue;

				itemIdx = g_cgv.myCharacterInfo->craftSourceInventory[checkedIdx[k]];
			
				sourceItem = &g_cgv.myCharacterInfo->item[itemIdx];

				
				itemTableIdx1 = sourceItem->itemTableIdx;
				itemTableIdx2 = pData->sourceItem[j].itemIndex;
				if( itemTableIdx1 != itemTableIdx2 ) continue;
				
				if( g_itemTable[itemTableIdx1].stackFlag && g_itemTable[itemTableIdx2].stackFlag )
				{
					if( pData->sourceItem[j].itemCount != sourceItem->durability + 1 ) continue;
				}
				

				break;
			}
			if( k == numSourceItem ) break;
			checkedItem[k] = true;
			checkedNumber --;
		}
		if( checkedNumber == 0 ) 
		{
		
			return i;
		}
	}

	return -1;
}


void CIFEventCraftWin::InitCraft()
{
	for( int i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		g_cgv.myCharacterInfo->craftSourceInventory[i] = -1;
	}

	m_nSelectEventCraftIndex=-1;
	m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	m_craftingFlag = false;
}

void CIFEventCraftWin::UndoCraft()
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

	InitCraft();
	Enable( false );
}

void CIFEventCraftWin::StartCraft( float craftMaxTime )
{
	m_craftMaxTime = craftMaxTime;
	m_craftCurrentTime = craftMaxTime;
	m_progressBar->UpdateBarState( m_craftMaxTime - m_craftCurrentTime, (int)m_craftMaxTime );

	m_craftingFlag = true;
}
