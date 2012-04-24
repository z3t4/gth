



#include "../global_def.h"
#include "IFCommon.h"


extern CItemUseProc			g_ItemUseProc;
extern CItemRecycleProc		gItemRecycleProc;









CIFItemWin::CIFItemWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	m_titleBar = new CIFControl( this );
	m_weightBar = new CIFBarCtl( this );
	m_closeBtn = new CIFButtonCtl( this );
	m_weaponChangeBtn = new CIFButtonCtl( this );

	
	for( int i = 0; i < NUM_IT_ITEM_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_ITEM_WINDOW_TYPE );

	
	ClearLockItems();

}

CIFItemWin::~CIFItemWin()
{

}

void CIFItemWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_ITEM_WIN );

	m_titleBar->PrepareControl( IF_ITEM_WIN_TITLE_BAR );
	m_weightBar->PrepareControl( IF_ITEM_WIN_WEIGHT_BAR );
	m_closeBtn->PrepareControl( IF_ITEM_WIN_CLOSE_BTN_0 );
	m_weaponChangeBtn->PrepareControl( IF_ITEM_WIN_WEAPON_CHANGE_BTN_0 );

	
	m_textCtl[IT_ITEM_WIN_TITLE]->PrepareControl( IF_ITEM_WIN_TITLE_TEXT );
	m_textCtl[IT_ITEM_WIN_WEAPON]->PrepareControl( IF_ITEM_WIN_WEAPON_TEXT );
	m_textCtl[IT_ITEM_WIN_WEAPON_CHANGE]->PrepareControl( IF_ITEM_WIN_WEAPON_CHANGE_TEXT );
	m_textCtl[IT_ITEM_WIN_DEF]->PrepareControl( IF_ITEM_WIN_DEF_TEXT );
	m_textCtl[IT_ITEM_WIN_INVEN]->PrepareControl( IF_ITEM_WIN_INVEN_TEXT );
	m_textCtl[IT_ITEM_WIN_EQUIP_ATK_POS_0]->PrepareControl( IF_ITEM_WIN_EQUIP_ATK_POS_0 );
	m_textCtl[IT_ITEM_WIN_EQUIP_ATK_POS_1]->PrepareControl( IF_ITEM_WIN_EQUIP_ATK_POS_1 );
	m_textCtl[IT_ITEM_WIN_EQUIP_PROT_POS]->PrepareControl( IF_ITEM_WIN_EQUIP_PROT_POS );
	m_textCtl[IT_ITEM_WIN_EQUIP_UPPER_POS]->PrepareControl( IF_ITEM_WIN_EQUIP_UPPER_POS );
	m_textCtl[IT_ITEM_WIN_EQUIP_AIRPROT_POS]->PrepareControl( IF_ITEM_WIN_EQUIP_AIRPROT_POS );
	m_textCtl[IT_ITEM_WIN_EQUIP_GLOVES_POS]->PrepareControl( IF_ITEM_WIN_EQUIP_GLOVES_POS );
	m_textCtl[IT_ITEM_WIN_EQUIP_LOWER_POS]->PrepareControl( IF_ITEM_WIN_EQUIP_LOWER_POS );
	m_textCtl[IT_ITEM_WIN_EQUIP_TRANS_POS]->PrepareControl( IF_ITEM_WIN_EQUIP_TRANS_POS );
	m_textCtl[IT_ITEM_WIN_EQUIP_BOOTS_POS]->PrepareControl( IF_ITEM_WIN_EQUIP_BOOTS_POS );
	m_textCtl[IT_ITEM_WIN_EQUIP_ACC_POS_0]->PrepareControl( IF_ITEM_WIN_EQUIP_ACC_POS_0 );
	m_textCtl[IT_ITEM_WIN_EQUIP_ACC_POS_1]->PrepareControl( IF_ITEM_WIN_EQUIP_ACC_POS_1 );
	m_textCtl[IT_ITEM_WIN_EQUIP_ACC_POS_2]->PrepareControl( IF_ITEM_WIN_EQUIP_ACC_POS_2 );
	m_textCtl[IT_ITEM_WIN_EQUIP_ACC_POS_3]->PrepareControl( IF_ITEM_WIN_EQUIP_ACC_POS_3 );
	m_textCtl[IT_ITEM_WIN_EQUIP_EPACK_POS]->PrepareControl( IF_ITEM_WIN_EQUIP_EPACK_POS );
	m_textCtl[IT_ITEM_WIN_EQUIP_RECYCLE_POS]->PrepareControl( IF_ITEM_WIN_EQUIP_RECYCLE_POS );
	m_textCtl[IT_ITEM_WIN_SE]->PrepareControl( IF_ITEM_WIN_SE_TEXT );
	m_textCtl[IT_ITEM_WIN_SE_U]->PrepareControl( IF_ITEM_WIN_SE_TEXT_U );
	m_textCtl[IT_ITEM_WIN_WEIGHT]->PrepareControl( IF_ITEM_WIN_WEIGHT_TEXT );
	m_textCtl[IT_ITEM_WIN_WEIGHT_U]->PrepareControl( IF_ITEM_WIN_WEIGHT_TEXT_U );
	m_textCtl[IT_ITEM_WIN_INVEN_POS]->PrepareControl( IF_ITEM_WIN_INVEN_POS );
}

int	CIFItemWin::WindowFrame()
{
	char szTemp[256];

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 492 ) );
	m_textCtl[IT_ITEM_WIN_TITLE]->SetText( szTemp );
	m_textCtl[IT_ITEM_WIN_TITLE]->SetColor( 0xffffffff );
	
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 493 ) );
	m_textCtl[IT_ITEM_WIN_WEAPON]->SetText( szTemp );
	
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 494 ) );
	m_textCtl[IT_ITEM_WIN_WEAPON_CHANGE]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 495 ) );
	m_textCtl[IT_ITEM_WIN_DEF]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 496 ) );
	m_textCtl[IT_ITEM_WIN_INVEN]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
	m_textCtl[IT_ITEM_WIN_SE]->SetText( szTemp );
	m_textCtl[IT_ITEM_WIN_SE]->SetColor( 0xffffffff );

	strcpy( szTemp, g_ifMng->GetNumberString( g_cgv.myCharacterInfo->curChargeSE ) );
	m_textCtl[IT_ITEM_WIN_SE_U]->SetText( szTemp );
	m_textCtl[IT_ITEM_WIN_SE_U]->SetColor( 0xffffffff );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 497 ) );
	m_textCtl[IT_ITEM_WIN_WEIGHT]->SetText( szTemp );

	char szCurWeight[128], szMaxWeight[128];

	strcpy( szCurWeight, g_ifMng->GetNumberString( g_cgv.myCharacterInfo->curItemWeight ) );
	strcpy( szMaxWeight, g_ifMng->GetNumberString( g_cgv.myCharacterInfo->calItemWeight ) );
	sprintf( szTemp, "%s/%s", szCurWeight, szMaxWeight );
	m_textCtl[IT_ITEM_WIN_WEIGHT_U]->SetText( szTemp );
	m_textCtl[IT_ITEM_WIN_WEIGHT_U]->SetColor( 0xffffffff );

	if( !State() ) return( IF_CTL_NONE );

	m_weightBar->UpdateBarState( g_cgv.myCharacterInfo->curItemWeight, g_cgv.myCharacterInfo->calItemWeight );

	return( IF_CTL_NONE );
}

void CIFItemWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();

	m_titleBar->Render();
	m_weightBar->Render();
	m_closeBtn->Render();
	m_weaponChangeBtn->Render();
	
	

	
	

	
	g_ifMng->m_iconCtl->BeginFrame();
	{
		
		for( int i = 0; i < MAX_INVENTORY_SIZE; i ++ )
		{
			int itemIdx, itemTableIdx, cx, cy;
			itemIdx = g_cgv.myCharacterInfo->inventory[i];

			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_ITEM_WIN_INVEN_POS]->m_info.clientX + ( i % 8 ) * 34;
				cy = m_textCtl[IT_ITEM_WIN_INVEN_POS]->m_info.clientY + ( i / 8 ) * 33;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;	
				

				int		curAlphasize = 0.0f;
				int		MaxAlphasize = 0.0f;
				DWORD	color		 = DISALBE_ICON_DARK_COLOR;

				if( !GTH_CheckRequire( g_cgv.myCharacterInfo, 0, &g_cgv.myCharacterInfo->item[itemIdx] ) )					
				{
					curAlphasize = 1.0f;
					MaxAlphasize = 1.0f;
					color		 = 0x80ff0000;
				}
				if (  IsLockItem(i) == TRUE) 
				{
					curAlphasize = 1.0f;
					MaxAlphasize = 1.0f;					
				}
				
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, 
					g_itemTable[itemTableIdx].iconItemID, 
					curAlphasize, MaxAlphasize, color );


				


			}
		}
		
		for( i = EQUIP_WEAPON_1; i <= EQUIP_EPACK; i ++ )
		{
			int itemIdx, itemTableIdx, cx, cy;
			itemIdx = g_cgv.myCharacterInfo->equipment[i];

			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_ITEM_WIN_EQUIP_ATK_POS_0 + i]->m_info.clientX;
				cy = m_textCtl[IT_ITEM_WIN_EQUIP_ATK_POS_0 + i]->m_info.clientY;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;










				int		curAlphasize = 0.0f;
				int		MaxAlphasize = 0.0f;
				DWORD	color		 = DISALBE_ICON_DARK_COLOR;

				if( !GTH_CheckRequire( g_cgv.myCharacterInfo, 0, &g_cgv.myCharacterInfo->item[itemIdx] ) )					
				{
					curAlphasize = 1.0f;
					MaxAlphasize = 1.0f;					
					color		 = 0x80ff0000;
				}				
				
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, 
					g_itemTable[itemTableIdx].iconItemID, 
					curAlphasize, MaxAlphasize, color );

			}
		}
	}
	g_ifMng->m_iconCtl->EndFrame();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_ITEM_WIN_TITLE]->Render();
		m_textCtl[IT_ITEM_WIN_WEAPON]->Render();
		m_textCtl[IT_ITEM_WIN_WEAPON_CHANGE]->Render();
		m_textCtl[IT_ITEM_WIN_DEF]->Render();
		m_textCtl[IT_ITEM_WIN_INVEN]->Render();
		m_textCtl[IT_ITEM_WIN_SE]->Render();
		m_textCtl[IT_ITEM_WIN_SE_U]->Render( IF_TEXT_OUTPUT_RIGHT );
		m_textCtl[IT_ITEM_WIN_WEIGHT]->Render();
		m_textCtl[IT_ITEM_WIN_WEIGHT_U]->Render( IF_TEXT_OUTPUT_CENTER );

		
		g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
		g_chatManager->SetReverseSize( -1 );
		g_chatManager->SetColor( 0xffffffff );
		for( int i = 0; i < MAX_INVENTORY_SIZE; i ++ )
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->inventory[i];

			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->item[itemIdx] ) )
				{
					cx = m_textCtl[IT_ITEM_WIN_INVEN_POS]->m_info.clientX + ( i % 8 ) * 34;
					cy = m_textCtl[IT_ITEM_WIN_INVEN_POS]->m_info.clientY + ( i / 8 ) * 33;
					number = g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
					strcpy( szTemp, g_ifMng->GetNumberString( number ) );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
	}
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->EndFrame();
}

int CIFItemWin::WindowSelection()
{
	int ret;
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int	CIFItemWin::WindowProcess()
{
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_SELECT;
	}
	if( m_weaponChangeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		GTH_SendMessage_RequestWeaponChange();
	}

	if( InventoryProcess() == IF_CTL_SELECT ) return IF_CTL_SELECT;
	if( EquipProcess() == IF_CTL_SELECT ) return IF_CTL_SELECT;

	return IF_CTL_NONE;
}

#define IF_ITEM_WIN_INVENTORY_XSIZE	34
#define IF_ITEM_WIN_INVENTORY_YSIZE	33



int CIFItemWin::InventoryProcess()
{
	
	
	for( int i = 0; i < MAX_INVENTORY_SIZE; i ++ )
	{
		int itemIdx = g_cgv.myCharacterInfo->inventory[i];

		if( itemIdx < 0 ) continue;
		if( g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx < 0 )
		{
			


			g_cgv.myCharacterInfo->inventory[i] = -1;
		}
	}


	
	int x, y, pos;
	int mx, my, temp;

	
	mx = g_input->GetPosX() - m_textCtl[IT_ITEM_WIN_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_ITEM_WIN_INVEN_POS]->GetYPos();

	if( mx <= 0 || my <= 0 ) return( IF_CTL_NONE );
	if( mx >= IF_ITEM_WIN_INVENTORY_XSIZE * 8 ||  my >= IF_ITEM_WIN_INVENTORY_YSIZE * 6 ) return( IF_CTL_NONE );

	temp = mx / IF_ITEM_WIN_INVENTORY_XSIZE;
	x = temp % 8;
	temp = my / IF_ITEM_WIN_INVENTORY_YSIZE;
	y = temp % 6;

	pos = y * 8 + x;

	
	if( pos < 0 || pos >= MAX_INVENTORY_SIZE )
	{
		return( IF_CTL_NONE );
	}
	
	int itemIdx = g_cgv.myCharacterInfo->inventory[pos];
	int mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;
	item_t *item;

	
	if( g_input->GetLBNone() ) 
	{
		
		if( itemIdx < 0 ) return( IF_CTL_NONE );

		g_ifMng->InitRolloverText();
		g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[itemIdx] );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
	}

	
#ifndef _GTH_ONLINE_VERSION
	if( itemIdx >= 0 && mouseItemIdx < 0 )
	{
		g_cgv.myCharacterInfo->mouseInventory = itemIdx;
		g_cgv.myCharacterInfo->inventory[pos] = -1;
	}
	else if( itemIdx < 0 && mouseItemIdx >= 0 )
	{	
		g_cgv.myCharacterInfo->mouseInventory = -1;
		g_cgv.myCharacterInfo->inventory[pos] = mouseItemIdx;
	}
	else if( itemIdx >= 0 && mouseItemIdx < 0 )
	{
		g_cgv.myCharacterInfo->mouseInventory = itemIdx;
		g_cgv.myCharacterInfo->inventory[pos] = mouseItemIdx;
	}
	
#else
	if( itemIdx < 0 && mouseItemIdx < 0 ) return( IF_CTL_NONE );
	

	
	
	if( g_input->KeyEvent( DIK_LALT ) || g_input->KeyEvent( DIK_RALT ) )
	{
		if( g_input->GetLBUp() )
		{
			if( itemIdx >= 0 )
			{
				g_ifMng->m_craftHelpWin->Enable();
				g_ifMng->SetFocus( g_ifMng->m_craftHelpWin );
				g_ifMng->m_craftHelpWin->SetCraftHelpWin( g_cgv.myCharacterInfo->item[itemIdx].name );
				g_ifMng->m_craftHelpWin->SearchCraftFormula();
			}
		}
		return IF_CTL_NONE;
	}

	if( g_input->GetLBUp() ) 
	{
		int itemTableIdx;
		item = &g_cgv.myCharacterInfo->item[itemIdx];
		itemTableIdx = item->itemTableIdx;

		
		if(g_ifMng->m_surgeryWin->State())	return( IF_CTL_NONE );
		
		if ( TRUE == IsLockItem(pos) )
		{			
			
			g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,593) );
			return( IF_CTL_NONE );		
		}

		if( g_input->GetCursorState( CURSOR_REPAIR ) && itemIdx >= 0 )
		{
			
			
			
			
			m_repairType = 0;
			m_repairPos = pos;
			GTH_SendMessageRequest_RepairItemConfirm(REPAIR_ITEM_SUB1, INVENTYPE_INVENTORY, pos);
			

			return( IF_CTL_NONE );
		}
		else if( mouseItemIdx >= 0 )
		{			
			GTH_SendMessage_RequestGetInventoryItem( pos, false );
		}
		else
		{
			item_t* pItem	= &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->inventory[0]];

			if( itemIdx >= 0 )
			{
				item = &g_cgv.myCharacterInfo->item[itemIdx];				
				
				if( !GTH_IsStackItem( item ) ) 
				{						
					GTH_SendMessage_RequestGetInventoryItem( pos, false );
				}
				
				else
				{
					
					g_ifMng->m_stackWin->InitStack( g_LPACK.GetMassage( 0, 498 ), item, IF_STACK_TYPE_INVENTORY, pos );
				}
			}
		}
	}
	
	

  
	else if( g_input->GetRBUp() && mouseItemIdx < 0)

	{			
		
		
		if(g_pApp->m_myCharacter->m_bObserveMode)
		{
			
			g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,462) );
			return( IF_CTL_NONE );
		}
		

		
		if ( TRUE == IsLockItem(pos) )
		{			
			
			g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,593) );
			return( IF_CTL_NONE );
		}


		
		
		
		if( g_ifMng->CheckConditionalWindow()) return( IF_CTL_NONE );
		
		item = &g_cgv.myCharacterInfo->item[itemIdx];

		
		
		
		
		int itemTableIdx = item->itemTableIdx;
		if (g_SkillItem.IfSkillItem(itemTableIdx))
		{
			
			if(TRUE == g_pApp->m_myCharacter->m_bWaitingRoom){
				if( 616 == itemTableIdx || 617 == itemTableIdx ||
					618 == itemTableIdx || 619 == itemTableIdx ){
					
					g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ),g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 392 ) );
				}
			}
			

			
			if(!g_pApp->m_myCharacter->isAlive )
			{				
				if(g_SkillItem.IsType(itemTableIdx,CSkillItem::ITEM_TYPE_ENABLE_DEAD_SELF_ITEM))
				{
					char szTemp[256];
					g_cgv.saveInventoryIdx = pos;
					sprintf( szTemp, g_LPACK.GetMassage(0,356), g_itemTable[itemTableIdx].name );
					g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_DEAD_USE_SKILL );
				}
				return( IF_CTL_NONE );
			}
			
			else
			{
				if(g_SkillItem.IsType(itemTableIdx,CSkillItem::ITEM_TYPE_ENABLE_DEAD_SELF_ITEM))
				{
					
					g_ifMng->SetMessage(
						g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
						g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,501));

					return( IF_CTL_NONE );
				}
			}
		}
	
		
		
		if(FALSE  == g_pApp->m_myCharacter->isAlive)
		{
			return( IF_CTL_NONE );
		}

		
		if( !GTH_IsConsumptiveItem( item ) ) 
		{	
			
			if ( !CheckStateWeaponChange(g_pApp->m_myCharacter ) )
				return IF_CTL_NONE;

			GTH_SendMessage_RequestGetInventoryItem( pos, true );
		}
		
		else
		{
			int itemTableIdx;
			char szTemp[256];

			itemTableIdx = item->itemTableIdx;

			
			if(g_SkillItem.IfSkillItem(itemTableIdx) && (g_cgv.myCharacterInfo->skillCastingtime >0))
			{
				char szTemp[256];
				sprintf( szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 200 ),g_itemTable[itemTableIdx].name );
				g_ifMng->AddSysMessage( szTemp );
				return 0;
			}

			
			if (g_SkillItem.IfSkillItem(itemTableIdx))
			{
				
				if(g_SkillItem.IsType(itemTableIdx,CSkillItem::ITEM_TYPE_TRANSFORM_ITEM))
				{
					if(g_pApp->m_myCharacter->isTransform)		
					{
						
						g_SkillLogic.CancelTransform(true);
						return 0;
					}
				}

				g_cgv.saveInventoryIdx = pos;
				sprintf( szTemp, g_LPACK.GetMassage(0,356), g_itemTable[itemTableIdx].name );
				g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_USE_POTION );
				return 0;
			}

			
			
			g_ItemUseProc.UseProc(item,pos);
			
			
			
			g_ItemUseProc.UseEtcTypeProc(item,pos);
		}
	}
#endif

	return( IF_CTL_NONE );
}



int CIFItemWin::EquipProcess()
{
	
	int i, pos;
	char szTemp[256];
	for( i = IT_ITEM_WIN_EQUIP_ATK_POS_0; i <= IT_ITEM_WIN_EQUIP_RECYCLE_POS; i ++ )
	{
		if( m_textCtl[i]->Selection() != IF_NOT_IN_POS ) break;
	}
	if( i > IT_ITEM_WIN_EQUIP_RECYCLE_POS ) return( IF_CTL_NONE );
	pos = i - IT_ITEM_WIN_EQUIP_ATK_POS_0;

	int itemIdx = g_cgv.myCharacterInfo->equipment[pos];
	int mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;

	
	if( g_input->GetLBNone() ) 
	{
		
		if( itemIdx < 0 ) return( IF_CTL_NONE );

		g_ifMng->InitRolloverText();
		g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[itemIdx] );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		return( IF_CTL_NONE );
	}

	
#ifndef _GTH_ONLINE_VERSION
	if( itemIdx >= 0 && mouseItemIdx < 0 )
	{
		g_cgv.myCharacterInfo->mouseInventory = itemIdx;
		g_cgv.myCharacterInfo->equipment[pos] = -1;
	}
	else if( itemIdx < 0 && mouseItemIdx >= 0 )
	{	
		g_cgv.myCharacterInfo->mouseInventory = -1;
		g_cgv.myCharacterInfo->equipment[pos] = mouseItemIdx;
	}
	else if( itemIdx >= 0 && mouseItemIdx < 0 )
	{
		g_cgv.myCharacterInfo->mouseInventory = itemIdx;
		g_cgv.myCharacterInfo->equipment[pos] = mouseItemIdx;
	}
	g_charManager->GenerateMyCharacter();
	
#else
	if( itemIdx < 0 && mouseItemIdx < 0 ) return( IF_CTL_NONE );
	if( g_ifMng->m_tradeWin->State() ) return( IF_CTL_NONE );
	
	if( g_ifMng->m_depotWin->State() ) return( IF_CTL_NONE );
	if( g_ifMng->m_boothWin->State() ) return( IF_CTL_NONE );
	if( g_ifMng->m_craftWin->State() ) return( IF_CTL_NONE );
	
	
	if( g_ifMng->m_EventCraftWin->State() ) return( IF_CTL_NONE );

	
	if( g_ifMng->m_upgradeWin->State() ) return( IF_CTL_NONE );
	
	if( g_ifMng->m_questscrollWin->State() ) return( IF_CTL_NONE );
	
	if( g_ifMng->m_autoQuestWin->State() ) return( IF_CTL_NONE );
	if( g_ifMng->m_npcInvWin->State() ) return( IF_CTL_NONE );


	if( g_ifMng->m_pgonryunBattleBattingwindow->State() ) return( IF_CTL_NONE );

	if( g_input->GetLBDown() ) 
	{
		
		if ( !CheckStateWeaponChange(g_pApp->m_myCharacter ) )
			return IF_CTL_NONE;

		if( g_input->GetCursorState( CURSOR_REPAIR ) && itemIdx >= 0 )
		{
			
			
			
			
			g_ifMng->m_itemWin->m_repairType = 1;
			g_ifMng->m_itemWin->m_repairPos = pos;			

			GTH_SendMessageRequest_RepairItemConfirm(REPAIR_ITEM_SUB0, INVENTYPE_EQUIP, pos);			
			
			return( IF_CTL_NONE );
		}
		
		else if( pos == 14 )
		{
			if( mouseItemIdx >= 0 )
			{
				item_t *item = &g_cgv.myCharacterInfo->item[mouseItemIdx];
								
				
				
				gItemRecycleProc.RecycleProc(item);
			}
		}
		else 
		{
			if( pos == 13 )
			{
				if( mouseItemIdx < 0 ) 
				{
					g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,240), MSG_BUTTONTYPE_OK );
					return( IF_CTL_NONE );
				}
			}			
			GTH_SendMessage_RequestGetEquipItem( pos, false );
		}
	}
#endif
	return( IF_CTL_SELECT );
}







void CIFItemWin::ClearLockItems(void)
{
	for (int nInvenIdx = 0; nInvenIdx < MAX_INVENTORY_SIZE; nInvenIdx ++)
	{
		m_LockItem[nInvenIdx].nItemTableIdx	= -1;
		m_LockItem[nInvenIdx].nInvenPos		= -1;
		m_LockItem[nInvenIdx].bLock			= FALSE;
	}
}







void CIFItemWin::LockItem(const sLockItem lockitem)
{
	if ( lockitem.nInvenPos < 0 || lockitem.nInvenPos >= MAX_INVENTORY_SIZE)
		return;

	m_LockItem[lockitem.nInvenPos].nInvenPos		= lockitem.nInvenPos;
	m_LockItem[lockitem.nInvenPos].nItemTableIdx	= lockitem.nItemTableIdx;
	m_LockItem[lockitem.nInvenPos].bLock			= TRUE;	
}






void CIFItemWin::UnLockItem(const int in_nItemIdx, const int in_nInvenPos)
{
	if ( in_nInvenPos < 0 || in_nInvenPos >= MAX_INVENTORY_SIZE)
		return;	

	if ( m_LockItem[in_nInvenPos].nItemTableIdx != in_nItemIdx)
		return;
	
	m_LockItem[in_nInvenPos].bLock = FALSE;


}






BOOL CIFItemWin::IsLockItem(const int in_nInvenPos) const
{
	if ( in_nInvenPos < 0 || in_nInvenPos >= MAX_INVENTORY_SIZE)
		return -1;

	
	if ( m_LockItem[in_nInvenPos].nInvenPos != in_nInvenPos )
		return -1;
	
	


	int ItemIdx = g_cgv.myCharacterInfo->inventory[in_nInvenPos];	
	if ( ItemIdx < 0 || ItemIdx >= MAX_NUMBER_OF_OWNITEM)
		return -1;

	item_t * pItem = &g_cgv.myCharacterInfo->item[ItemIdx];

	if ( m_LockItem[in_nInvenPos].nItemTableIdx != pItem->itemTableIdx )
		return -1;

	return m_LockItem[in_nInvenPos].bLock;
	
}