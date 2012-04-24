


#include "../global_def.h"








CIFGiftCardWin::CIFGiftCardWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	m_GiftCardTitleBar		= new CIFControl( this );
	m_GiftCardTitleBottom	= new CIFControl( this );
	m_GiftCardBar			= new CIFControl( this );
	m_GiftCardLeftBar		= new CIFControl( this );
	m_GiftCardRightBar		= new CIFControl( this );
	m_GiftCardIDBar			= new CIFControl( this );	
	m_GiftCardPasswordBar   = new CIFControl( this );	
	m_GiftCardOkBtn			= new CIFButtonCtl( this );
	m_GiftCardCancelBtn		= new CIFButtonCtl( this );

	for (int TextCntIdx =0; TextCntIdx < NUM_IF_TEXT; TextCntIdx++)
		m_textCtl[TextCntIdx] = new CIFText(this);

	Enable(false);

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_GIFT_CARD_WINDOW_TYPE );
	
}
CIFGiftCardWin::~CIFGiftCardWin()
{

}








void CIFGiftCardWin::PrepareControl()
{

	CIFControl::PrepareControl( IF_GIFTCARD_WIN );	
	m_GiftCardTitleBar->PrepareControl( IF_GIFTCARD_WIN_TITLE_BAR );
	m_GiftCardTitleBottom->PrepareControl( IF_GIFTCARD_WIN_TITLE_BOTTOM );
	m_GiftCardBar->PrepareControl( IF_GIFTCARD_WIN_ID_BAR );
	m_GiftCardLeftBar->PrepareControl( IF_GIFTCARD_WIN_LEFT_BAR );
	m_GiftCardRightBar->PrepareControl( IF_GIFTCARD_WIN_RIGHT_BAR );
	m_GiftCardIDBar->PrepareControl( IF_GIFTCARD_WIN_ID_BAR );	
	m_GiftCardPasswordBar->PrepareControl( IF_GIFTCARD_WIN_PASSWORD_BAR );	
	
	m_GiftCardOkBtn->PrepareControl( IF_GIFTCARD_WIN_OK_BTN_0 );
	m_GiftCardCancelBtn->PrepareControl( IF_GIFTCARD_WIN_CANCEL_BTN_0 );
	
	m_textCtl[IT_GIFTCARD_WIN_TITLE]->PrepareControl( IF_GIFTCARD_WIN_LOGIN_TEXT );
	m_textCtl[IT_GIFTCARD_WIN_ID]->PrepareControl( IF_GIFTCARD_WIN_ID_TEXT );	
	m_textCtl[IT_GIFTCARD_WIN_ID_INPUT]->PrepareControl( IF_GIFTCARD_WIN_ID_TEXT_U );	

	m_textCtl[IT_GIFTCARD_WIN_PASSWORD]->PrepareControl( IF_GIFTCARD_WIN_PASSWORD_TEXT );	
	m_textCtl[IT_GIFTCARD_WIN_PASSWORD_INPUT]->PrepareControl( IF_GIFTCARD_WIN_PASSWORD_TEXT_U );	


	int cx, cy;
	cx = m_textCtl[IT_GIFTCARD_WIN_ID_INPUT]->GetXPos();
	cy = m_textCtl[IT_GIFTCARD_WIN_ID_INPUT]->GetYPos();

	g_chatManager->SetInput( cx, cy, MAX_INPUT_STRLEN, 	MAX_INPUT_STRLEN, 
			INPUT_INDEX_GIFT_CARD_ID, 	1, 	90 );


	cx = m_textCtl[IT_GIFTCARD_WIN_PASSWORD_INPUT]->GetXPos();
	cy = m_textCtl[IT_GIFTCARD_WIN_PASSWORD_INPUT]->GetYPos();

	g_chatManager->SetInput( cx, cy, MAX_INPUT_STRLEN, 	MAX_INPUT_STRLEN, 
			INPUT_INDEX_GIFT_CARD_PASSWORD, 	2, 	90 );


	
}






void CIFGiftCardWin::Render()
{

	if( State() == 0 ) return;	
	
	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	m_GiftCardTitleBar->Render();
	m_GiftCardTitleBottom->Render();
	m_GiftCardBar->Render();
	m_GiftCardLeftBar->Render();
	m_GiftCardRightBar->Render();
	m_GiftCardIDBar->Render();
	m_GiftCardPasswordBar->Render();
	
	m_GiftCardOkBtn->Render();
	m_GiftCardCancelBtn->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		
		m_textCtl[IT_GIFTCARD_WIN_TITLE]->Render();
		m_textCtl[IT_GIFTCARD_WIN_ID]->Render();		
		m_textCtl[IT_GIFTCARD_WIN_PASSWORD]->Render();

		g_chatManager->RenderInputFrame( INPUT_INDEX_GIFT_CARD_ID );
		g_chatManager->RenderInputFrame( INPUT_INDEX_GIFT_CARD_PASSWORD );		
	}
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->EndFrame();
	

}


int	CIFGiftCardWin::WindowFrame()
{	
	m_textCtl[IT_GIFTCARD_WIN_TITLE]->SetText( "GiftCard" );

	
	m_textCtl[IT_GIFTCARD_WIN_ID]->SetText( "Card ID" );	

	
	m_textCtl[IT_GIFTCARD_WIN_PASSWORD]->SetText( "Card P/W" );	



	int cx, cy;
	cx = m_textCtl[IT_GIFTCARD_WIN_ID_INPUT]->GetXPos();
	cy = m_textCtl[IT_GIFTCARD_WIN_ID_INPUT]->GetYPos();

	g_chatManager->m_ix[INPUT_INDEX_GIFT_CARD_ID] = cx;
	g_chatManager->m_iy[INPUT_INDEX_GIFT_CARD_ID] = cy;

	


	cx = m_textCtl[IT_GIFTCARD_WIN_PASSWORD_INPUT]->GetXPos();
	cy = m_textCtl[IT_GIFTCARD_WIN_PASSWORD_INPUT]->GetYPos();
	g_chatManager->m_ix[INPUT_INDEX_GIFT_CARD_PASSWORD] = cx;
	g_chatManager->m_iy[INPUT_INDEX_GIFT_CARD_PASSWORD] = cy;


	






	return 1;
}

int	CIFGiftCardWin::WindowProcess()
{

	
	if( m_GiftCardIDBar->Selection() == IF_LBDN )		
	{	
		
		g_chatManager->SetInputState( INPUT_INDEX_GIFT_CARD_ID , 1);
		g_chatManager->ChangeInputMessage( INPUT_INDEX_GIFT_CARD_ID );		
		return IF_CTL_NONE;
	}	

	
	if ( m_GiftCardPasswordBar->Selection() == IF_LBDN)
	{
		
		g_chatManager->SetInputState( INPUT_INDEX_GIFT_CARD_PASSWORD , 2);
		g_chatManager->ChangeInputMessage( INPUT_INDEX_GIFT_CARD_PASSWORD );		
		return IF_CTL_NONE;
	}
	
	
	if( m_GiftCardOkBtn->ProcessButton() == IF_CTL_SELECT || g_input->KeyDown( DIK_RETURN ) )	
	{

		
		
		

		g_GiftCard.GTH_SendMessage_Request_item(
			g_chatManager->GetInputStr(INPUT_INDEX_GIFT_CARD_ID ),
			g_chatManager->GetInputStr(INPUT_INDEX_GIFT_CARD_PASSWORD )
			);





		return IF_CTL_NONE;
	}	


	
	if( m_GiftCardCancelBtn->ProcessButton() == IF_CTL_SELECT )	
	{
		
		
		Enable( false );

		g_chatManager->SetInputState( INPUT_INDEX_GIFT_CARD_ID, -1 );
		g_chatManager->SetInputState( INPUT_INDEX_GIFT_CARD_PASSWORD, -1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );

		return IF_CTL_NONE;
	}
	

	return IF_CTL_NONE;

}


int	CIFGiftCardWin::WindowSelection()
{
	
	int ret = IF_CTL_NONE;
	ret = m_GiftCardTitleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}






void CIFGiftCardWin::InitGiftCardWin()
{
	int cx, cy;

	cx = m_textCtl[IT_GIFTCARD_WIN_ID_INPUT]->GetXPos();
	cy = m_textCtl[IT_GIFTCARD_WIN_ID_INPUT]->GetYPos();

	g_chatManager->SetInput( cx, cy, MAX_INPUT_STRLEN, 	MAX_INPUT_STRLEN, 
			INPUT_INDEX_GIFT_CARD_ID, 	1, 	90 );


	cx = m_textCtl[IT_GIFTCARD_WIN_PASSWORD]->GetXPos();
	cy = m_textCtl[IT_GIFTCARD_WIN_PASSWORD_INPUT]->GetYPos();

	g_chatManager->SetInput( cx, cy, MAX_INPUT_STRLEN, 	MAX_INPUT_STRLEN, 
			INPUT_INDEX_GIFT_CARD_PASSWORD,  2, 90 );
	
	g_chatManager->ChangeInputMessage( INPUT_INDEX_GIFT_CARD_ID );
}