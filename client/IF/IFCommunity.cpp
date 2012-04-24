



#include "../global_def.h"
#include "IFCommon.h"
#include "../game/GolryunBattle/GolryunBattle.h"

#include "..\GM(Game Master)\CGMCtrl.h"
extern CGMCtrl* gcpGMCtrl;










CIFDlgWin::CIFDlgWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i = 0; i < NUM_IT_DLG_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	m_scrollBar = new CIFScrollBar( this );
	m_optionBtn = new CIFButtonCtl( this );

	m_allMsgCurPos = 0;
	m_viewMsgCurPos = 0;
	m_viewOutPos = 0;
	m_numAllMsg = 0;
	m_numViewMsg = 0;

	memset( m_allMsg, 0, sizeof( m_allMsg ) );
	memset( m_viewMsg, 0, sizeof( m_viewMsg ) );

	m_curSendType = IF_CHATMSG_TYPE_NORMAL;
	memset( m_whisperName, 0, sizeof( m_whisperName ) );

	for( int idx = 0; idx<5 ; idx++ )
	{
		for( int i=0; i<6; i++ )
			m_checkViewType[idx][i] = false;

		switch( idx )
		{
		case 0:
			m_checkViewType[idx][0] = true;
			m_checkViewType[idx][1] = true;
			m_checkViewType[idx][2] = true;
			m_checkViewType[idx][3] = true;
			m_checkViewType[idx][4] = true;
			m_checkViewType[idx][5] = true;
			break;
		case 1:
			m_checkViewType[idx][1] = true;
			m_checkViewType[idx][4] = true;
			break;
		case 2:
			m_checkViewType[idx][2] = true;
			m_checkViewType[idx][4] = true;
			break;
		case 3:
			m_checkViewType[idx][3] = true;
			m_checkViewType[idx][4] = true;
			break;
		case 4:
			m_checkViewType[idx][4] = true;
			break;
		}
	}

	Enable( true );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_DLG_WINDOW_TYPE );
}

CIFDlgWin::~CIFDlgWin()
{

}

void CIFDlgWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_C_WIN );

	m_textCtl[IT_DLG_WIN_TITLE_0]->PrepareControl( IF_C_WIN_TITLE_TEXT_0_U );
	m_textCtl[IT_DLG_WIN_TITLE_1]->PrepareControl( IF_C_WIN_TITLE_TEXT_1_U );
	m_textCtl[IT_DLG_WIN_TITLE_2]->PrepareControl( IF_C_WIN_TITLE_TEXT_2_U );
	m_textCtl[IT_DLG_WIN_TITLE_3]->PrepareControl( IF_C_WIN_TITLE_TEXT_3_U );
	m_textCtl[IT_DLG_WIN_TITLE_4]->PrepareControl( IF_C_WIN_TITLE_TEXT_4_U );
	m_textCtl[IT_DLG_WIN_INPUT_U]->PrepareControl( IF_C_WIN_INPUT_TEXT_U );
	m_textCtl[IT_DLG_WIN_OUTPUT_U]->PrepareControl( IF_C_WIN_OUTPUT_TEXT_U );

	m_optionBtn->PrepareControl( IF_C_WIN_OPTION_BTN_0 );
	m_scrollBar->PrepareControl( IF_C_WIN_SCROLL );
	m_scrollBar->SetBarPosition( 0, -1 );
}

int CIFDlgWin::WindowFrame()
{
	
	m_textCtl[IT_DLG_WIN_TITLE_0]->SetText( g_LPACK.GetMassage( 0, 486 ) );
	
	m_textCtl[IT_DLG_WIN_TITLE_1]->SetText( g_LPACK.GetMassage( 0, 487 ) );
	
	m_textCtl[IT_DLG_WIN_TITLE_2]->SetText( g_LPACK.GetMassage( 0, 488 ) );
	
	m_textCtl[IT_DLG_WIN_TITLE_3]->SetText( g_LPACK.GetMassage( 0, 489 ) );
	
	m_textCtl[IT_DLG_WIN_TITLE_4]->SetText( g_LPACK.GetMassage( 0, 490 ) );

	for( int i = 0; i < 5; i ++ )
	{
		m_textCtl[IT_DLG_WIN_TITLE_0 + i]->SetColor( 0xff808080 );
	}
	m_textCtl[IT_DLG_WIN_TITLE_0 + m_curSendType]->SetColor( 0xffffffff );

	if( m_numViewMsg - NUM_OUTPUT_CHAT_MESSAGE > 0 && 
		m_numViewMsg - NUM_OUTPUT_CHAT_MESSAGE != m_scrollBar->m_max )
	{
		if( m_viewOutPos == m_viewMsgCurPos )
			m_scrollBar->SetBarPosition( m_numViewMsg - NUM_OUTPUT_CHAT_MESSAGE, m_numViewMsg - NUM_OUTPUT_CHAT_MESSAGE );
		else
			m_scrollBar->SetBarPosition( m_numViewMsg - NUM_OUTPUT_CHAT_MESSAGE, m_scrollBar->m_current );
	}
	else if( m_numViewMsg - NUM_OUTPUT_CHAT_MESSAGE <= 0 )
	{
		m_scrollBar->SetBarPosition( 0, -1 );
	}

	return IF_CTL_NONE;
}

void CIFDlgWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();

	m_scrollBar->Render();
	m_optionBtn->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_DLG_WIN_TITLE_0]->Render();
		m_textCtl[IT_DLG_WIN_TITLE_1]->Render();
		m_textCtl[IT_DLG_WIN_TITLE_2]->Render();
		m_textCtl[IT_DLG_WIN_TITLE_3]->Render();
		m_textCtl[IT_DLG_WIN_TITLE_4]->Render();
		
		int i, mIdx, x, y;

		x = m_textCtl[IT_DLG_WIN_OUTPUT_U]->GetXPos();
		y = m_textCtl[IT_DLG_WIN_OUTPUT_U]->GetYPos();

	
		for( i = 0; i < NUM_OUTPUT_CHAT_MESSAGE; i ++ )
			{
				mIdx = m_viewOutPos - NUM_OUTPUT_CHAT_MESSAGE + i;
				if( mIdx >= MAX_OUTPUT_CHAT_MESSAGE ) mIdx -= MAX_OUTPUT_CHAT_MESSAGE;
				if( mIdx < 0 ) mIdx += MAX_OUTPUT_CHAT_MESSAGE;
				if( m_viewMsg[mIdx].str[0] == 0 ) continue;
				
				g_chatManager->SetFontColor( m_viewMsg[mIdx].color );
				g_chatManager->Output( x, y - ( NUM_OUTPUT_CHAT_MESSAGE - i - 1 ) * 13, m_viewMsg[mIdx].str );
			}
	
	
		g_chatManager->RenderInputFrame( INPUT_INDEX_DIALOGBOX );

	}
	g_chatManager->EndFrame();

}

int CIFDlgWin::WindowSelection()
{
	int ret;

	
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFDlgWin::WindowProcess()
{
	if( m_optionBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( g_ifMng->m_chatOptionWin->ToggleState() )
		{
			g_ifMng->SetFocus( g_ifMng->m_chatOptionWin );

			for( int i=0; i<6; i++ )
			{
				g_ifMng->m_chatOptionWin->m_checkBtn[i]->SetCheck( m_checkViewType[m_curSendType][i] );
			}
		}

		return IF_CTL_NONE;
	}

	if( m_textCtl[IT_DLG_WIN_INPUT_U]->Selection() == IF_LBDN )
	{
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
	}

	for( int i = 0; i < 5; i ++ )
	{
		if( m_textCtl[IT_DLG_WIN_TITLE_0 + i]->Selection() == IF_LBUP )
		{
			SetViewTypeDefault( i );

			if( i == 4 )
			{
				SetWhisperFromTarget();
			}
		
			m_curSendType = i;
			SetSendString();
		}
	}

	if( m_numViewMsg == 0 ) return IF_CTL_NONE;

	if( m_scrollBar->ProcessScrollBar() == IF_CTL_SELECT )
	{

		if( m_scrollBar->m_max != 0 )
		{
			m_viewOutPos = m_viewMsgCurPos - ( m_scrollBar->m_max - m_scrollBar->m_current );
			if( m_viewOutPos >= MAX_OUTPUT_CHAT_MESSAGE ) m_viewOutPos -= MAX_OUTPUT_CHAT_MESSAGE;
		}

		return IF_CTL_SELECT;
	}
	
	return IF_CTL_NONE;
}

void CIFDlgWin::SetViewTypeDefault( int idx )
{
	char szTemp[255];

	
	switch( idx )
	{
	case 0:
		strcpy( szTemp, g_LPACK.GetMessage(9,0) );
		break;
	case 1:
		strcpy( szTemp, g_LPACK.GetMessage(9,1) );
		break;
	case 2:
		strcpy( szTemp, g_LPACK.GetMessage(9,2) );
		break;
	case 3:
		strcpy( szTemp, g_LPACK.GetMessage(9,3) );
		break;
	case 4:
		strcpy( szTemp, g_LPACK.GetMessage(9,4) );
		break;
	}

	g_ifMng->m_chatOptionWin->m_textCtl[IT_CHATOPTION_WIN_TITLE]->SetText( szTemp );

	for( int i=0; i<6; i++ )
	{
		g_ifMng->m_chatOptionWin->m_checkBtn[i]->SetCheck( m_checkViewType[idx][i] );
	}
}


void CIFDlgWin::AddChatPosition()
{

	if( m_numAllMsg < MAX_OUTPUT_CHAT_MESSAGE ) 
		m_numAllMsg ++;
	else
		m_numAllMsg = MAX_OUTPUT_CHAT_MESSAGE;

	if( AddViewMsg() )
	{
		if( m_viewMsgCurPos == m_viewOutPos ) 
		{
			m_viewOutPos ++;
			if( m_viewOutPos >= MAX_OUTPUT_CHAT_MESSAGE ) m_viewOutPos -= MAX_OUTPUT_CHAT_MESSAGE;
		}

		m_viewMsgCurPos ++;

		if( m_viewMsgCurPos >= MAX_OUTPUT_CHAT_MESSAGE ) 
		{
			m_viewMsgCurPos = 0;
		}
	}

	m_allMsgCurPos ++;

	if( m_allMsgCurPos >= MAX_OUTPUT_CHAT_MESSAGE ) 
	{
		m_allMsgCurPos = 0;
	}

	
	if( m_scrollBar->m_max != 0 )
	{
		m_viewOutPos = m_viewMsgCurPos - ( m_scrollBar->m_max - m_scrollBar->m_current );
		if( m_viewOutPos >= MAX_OUTPUT_CHAT_MESSAGE ) m_viewOutPos -= MAX_OUTPUT_CHAT_MESSAGE;
	}

}



void CIFDlgWin::AddChatMessage( char* message, DWORD color, int type )
{
	char szTemp[CHATSTRING];
	char msg[CHATSTRING];

	int len, size;
	int sPos = 0, cPos = 0, templen;

	size = m_textCtl[IT_DLG_WIN_OUTPUT_U]->GetXSize();

	sstrncpy( msg, message, CHATSTRING );
	
	len = g_chatManager->GetChatStrSizeChat( msg );
	if( len < size )
	{
		if( len > 0 )
		{
			strcpy( m_allMsg[m_allMsgCurPos].str, msg );
			m_allMsg[m_allMsgCurPos].color = color;
			m_allMsg[m_allMsgCurPos].type = type;

			AddChatPosition();
		}
		return;
	}

	while( len > size )
	{
		memset( szTemp, 0, sizeof( szTemp ) );

		if( IsDBCSLeadByte( msg[cPos + sPos] ) ) 
		{
			memcpy( szTemp, msg + sPos, cPos );
			templen = g_chatManager->GetChatStrSizeChat( szTemp );

			if( templen + 12 >= size )
			{
				sPos += cPos;
				len -= templen;
				cPos = 0;

				strcpy( m_allMsg[m_allMsgCurPos].str, szTemp );
				m_allMsg[m_allMsgCurPos].color = color;
				m_allMsg[m_allMsgCurPos].type = type;

				AddChatPosition();
			}
			else cPos += 2;
		}
		else 
		{
			memcpy( szTemp, msg + sPos, cPos );
			templen = g_chatManager->GetChatStrSizeChat( szTemp );

			if( templen + 8 >= size )
			{
				sPos += cPos;
				len -= templen;
				cPos = 0;

				strcpy( m_allMsg[m_allMsgCurPos].str, szTemp );
				m_allMsg[m_allMsgCurPos].color = color;
				m_allMsg[m_allMsgCurPos].type = type;

				AddChatPosition();
			}
			else cPos ++;
		}
	}

	memset( szTemp, 0, sizeof( szTemp ) );
	cPos = strlen( msg + sPos );

	if( cPos > 0 )
	{
		memcpy( szTemp, msg + sPos, cPos );
		
		strcpy( m_allMsg[m_allMsgCurPos].str, szTemp );
		m_allMsg[m_allMsgCurPos].color = color;
		m_allMsg[m_allMsgCurPos].type = type;

		AddChatPosition();
		return;
	}
}

void CIFDlgWin::SetDlgInput( int flag )
{
	int cx = m_textCtl[IT_DLG_WIN_INPUT_U]->GetXPos();
	int cy = m_textCtl[IT_DLG_WIN_INPUT_U]->GetYPos();

	g_chatManager->SetInput( cx, cy, MAX_OUTPUT_CHAT_STRING * 2, MAX_OUTPUT_CHAT_STRING, INPUT_INDEX_DIALOGBOX, 1, 305 );
	g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
	if( !flag ) g_chatManager->SetInputState( INPUT_INDEX_DIALOGBOX, -1 );
}


void CIFDlgWin::SetSendString()
{
	memset( g_chatManager->m_returnStr, 0, sizeof( chatMessage ) );
	g_chatManager->InitInputStr( INPUT_INDEX_DIALOGBOX );
	
	switch( m_curSendType )	
	{
	case IF_CHATMSG_TYPE_NORMAL :
		break;
	case IF_CHATMSG_TYPE_PARTY :
		{
			g_chatManager->AddInputStr( 0, "\"" );
		}
		break;
	case IF_CHATMSG_TYPE_DAN :
		{
			g_chatManager->AddInputStr( 0, "#" );
		}
		break;
	case IF_CHATMSG_TYPE_TRADE :
		{
			g_chatManager->AddInputStr( 0, "&" );
		}
		break;
	case IF_CHATMSG_TYPE_WHISPER :
		{
			char szTemp[CHATSTRING];

			if( m_whisperName[0] == 0 ) sprintf( szTemp, "'" );
			else sprintf( szTemp, "'%s ", m_whisperName );

			g_chatManager->AddInputStr( 0, szTemp );
		}
		break;
	}
}

void CIFDlgWin::SetWhisper( char *name )
{
	strcpy( m_whisperName, name );
	SetSendString();
}

void CIFDlgWin::SetWhisperFromTarget()
{
	if( g_pApp->m_myCharacter->targetType !=  ENTITY_PC ) return;
	if( g_pApp->m_myCharacter->targetIdx == g_cgv.myPCIdxOnServer ) return;

	if( g_cgv.pTargetChar != NULL ){

		if(TRUE == g_cgv.syncCharacter[(g_cgv.pTargetChar)->ID].hide) return;
		
		SetWhisper( g_cgv.pTargetChar->name );
	}

	m_curSendType = 4;
	SetSendString();
}

bool CIFDlgWin::UpdateViewMsg()
{
	int idx = 0;
	for( int i=0; i<m_numAllMsg; i++ )
	{
		if( IsCheckedType( m_allMsg[i].type ) )
		{
			memcpy( &m_viewMsg[idx], &m_allMsg[i], sizeof( chatMessage_t ) );
			idx ++;
		}
	}

	m_viewMsgCurPos = idx;
	m_viewOutPos = m_viewMsgCurPos;
	m_numViewMsg = idx;

	return true;
}

bool CIFDlgWin::AddViewMsg()
{
	if( IsCheckedType( m_allMsg[m_allMsgCurPos].type ) )
	{
		if( m_numViewMsg < MAX_OUTPUT_CHAT_MESSAGE ) 
			m_numViewMsg ++;
		else
			m_numViewMsg = MAX_OUTPUT_CHAT_MESSAGE;

		memcpy( &m_viewMsg[m_viewMsgCurPos], &m_allMsg[m_allMsgCurPos], sizeof( chatMessage_t ) );

		return true;
	}
	return false;
}

bool CIFDlgWin::IsCheckedType( int type )
{
	if ( type == IF_CHATMSG_TYPE_SYSTEM )
		return true;
	
	for( int i=0; i<6; i++ )
	{
		if( m_checkViewType[m_curSendType][i] )
		{
			if( type == i )
				return true;
		}
	}
	return false;
}

void CIFDlgWin::SetScrollBarPositionMax()
{
	m_scrollBar->SetBarPositionMax();
	
	if( m_scrollBar->m_max != 0 )
	{
		m_viewOutPos = m_viewMsgCurPos - ( m_scrollBar->m_max - m_scrollBar->m_current );
		if( m_viewOutPos >= MAX_OUTPUT_CHAT_MESSAGE ) m_viewOutPos -= MAX_OUTPUT_CHAT_MESSAGE;
	}

}





CIFChatOptionWin::CIFChatOptionWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	int i;
	
	for( i = 0; i < NUM_IT_CHATOPTION_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	
	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );

	m_saveBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );

	for( i=0; i<6; i++ )
	{
		m_checkBtn[i] = new CIFCheckBox( this );
	}

	Enable( false );

	g_ifMng->AddFocus( this );
	SetWindowType( IF_CHATOPTION_WINDOW_TYPE );
}

CIFChatOptionWin::~CIFChatOptionWin()
{

}

void CIFChatOptionWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_CHATOPTION_WIN );

	m_textCtl[IT_CHATOPTION_WIN_TITLE]->PrepareControl( IF_CHATOPTION_WIN_TITLE_TEXT );
	m_textCtl[IT_CHATOPTION_WIN_NORMALCHAT]->PrepareControl( IF_CHATOPTION_WIN_NORMALCHAT );
	m_textCtl[IT_CHATOPTION_WIN_PARTYCHAT]->PrepareControl( IF_CHATOPTION_WIN_PARTYCHAT );
	m_textCtl[IT_CHATOPTION_WIN_DANCHAT]->PrepareControl( IF_CHATOPTION_WIN_DANCHAT );
	m_textCtl[IT_CHATOPTION_WIN_TRADECHAT]->PrepareControl( IF_CHATOPTION_WIN_TRADECHAT );
	m_textCtl[IT_CHATOPTION_WIN_WHISPERCHAT]->PrepareControl( IF_CHATOPTION_WIN_WHISPERCHAT );
	m_textCtl[IT_CHATOPTION_WIN_WORLDCHAT]->PrepareControl( IF_CHATOPTION_WIN_WORLDCHAT );

	m_titleBar->PrepareControl( IF_CHATOPTION_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_CHATOPTION_WIN_TITLE_BOTTOM );

	m_saveBtn->PrepareControl( IF_CHATOPTION_WIN_SAVEBTN_0 );
	m_cancelBtn->PrepareControl( IF_CHATOPTION_WIN_CANCELBTN_0 );

	m_checkBtn[0]->PrepareControl( IF_CHATOPTION_WIN_CHECKBOX0_0 );
	m_checkBtn[1]->PrepareControl( IF_CHATOPTION_WIN_CHECKBOX1_0 );
	m_checkBtn[2]->PrepareControl( IF_CHATOPTION_WIN_CHECKBOX2_0 );
	m_checkBtn[3]->PrepareControl( IF_CHATOPTION_WIN_CHECKBOX3_0 );
	m_checkBtn[4]->PrepareControl( IF_CHATOPTION_WIN_CHECKBOX4_0 );
	m_checkBtn[5]->PrepareControl( IF_CHATOPTION_WIN_CHECKBOX5_0 );

	for( int i=0; i<6; i++ )
	{
		m_checkBtn[i]->SetCheck( true );
	}

	
	m_textCtl[IT_CHATOPTION_WIN_TITLE]->SetText( g_LPACK.GetMessage(9,0) );
}

int CIFChatOptionWin::WindowFrame()
{


	m_textCtl[IT_CHATOPTION_WIN_NORMALCHAT]->SetText( g_LPACK.GetMessage(0,967) );
	m_textCtl[IT_CHATOPTION_WIN_PARTYCHAT]->SetText( g_LPACK.GetMessage(0,968) );
	m_textCtl[IT_CHATOPTION_WIN_DANCHAT]->SetText( g_LPACK.GetMessage(0,969) );
	m_textCtl[IT_CHATOPTION_WIN_TRADECHAT]->SetText( g_LPACK.GetMessage(0,970) );
	m_textCtl[IT_CHATOPTION_WIN_WHISPERCHAT]->SetText( g_LPACK.GetMessage(0,971) );
	m_textCtl[IT_CHATOPTION_WIN_WORLDCHAT]->SetText( g_LPACK.GetMessage(0,972) );

	m_textCtl[IT_CHATOPTION_WIN_NORMALCHAT]->SetColor( 0xffffffff );
	m_textCtl[IT_CHATOPTION_WIN_PARTYCHAT]->SetColor( 0xffffffff );
	m_textCtl[IT_CHATOPTION_WIN_DANCHAT]->SetColor( 0xffffffff );
	m_textCtl[IT_CHATOPTION_WIN_TRADECHAT]->SetColor( 0xffffffff );
	m_textCtl[IT_CHATOPTION_WIN_WHISPERCHAT]->SetColor( 0xffffffff );
	m_textCtl[IT_CHATOPTION_WIN_WORLDCHAT]->SetColor( 0xffffffff );

	m_saveBtn->SetButtonText( g_LPACK.GetMessage(0,973) );
	m_cancelBtn->SetButtonText( g_LPACK.GetMessage(0,521) );

	return IF_CTL_NONE;
}

void CIFChatOptionWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();

	m_titleBar->Render();
	m_titleBottom->Render();

	m_saveBtn->RenderTextBtn();
	m_cancelBtn->RenderTextBtn();

	for( int i=0; i<6; i++ )
	{
		m_checkBtn[i]->Render();
	}
	
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_CHATOPTION_WIN_TITLE]->Render();
		m_textCtl[IT_CHATOPTION_WIN_NORMALCHAT]->Render();
		m_textCtl[IT_CHATOPTION_WIN_PARTYCHAT]->Render();
		m_textCtl[IT_CHATOPTION_WIN_DANCHAT]->Render();
		m_textCtl[IT_CHATOPTION_WIN_TRADECHAT]->Render();
		m_textCtl[IT_CHATOPTION_WIN_WHISPERCHAT]->Render();
		m_textCtl[IT_CHATOPTION_WIN_WORLDCHAT]->Render();
	}
	g_chatManager->EndFrame();
}

int CIFChatOptionWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return IF_CTL_NONE;

}

int CIFChatOptionWin::WindowProcess()
{
	int i;
	
	if( m_saveBtn->ProcessButton() == IF_CTL_SELECT )
	{
		for( i=0; i<6; i++ )
		{
			int curSendType = g_ifMng->m_dlgWin->m_curSendType;
			g_ifMng->m_dlgWin->m_checkViewType[curSendType][i] = m_checkBtn[i]->GetCheck();
		}

		GTH_SaveOptionToFile();
		
		Enable( false );
		return IF_CTL_NONE;
	}

	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_NONE;
	}

	for( i=0; i<6; i++ )
	{
		if( m_checkBtn[i]->ProcessCheckBox() == IF_CTL_SELECT )
		{
			return IF_CTL_SELECT;
		}
	}

	return IF_CTL_NONE;
}






CIFWorldChatWin::CIFWorldChatWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_WORLDCHAT_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );
	m_bgMessage = new CIFControl( this );

	m_sendBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );
	m_exitBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_WORLDCHAT_WINDOW_TYPE );
}

CIFWorldChatWin::~CIFWorldChatWin()
{

}

void CIFWorldChatWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_WORLDCHAT_WIN );

	
	m_textCtl[IT_WORLDCHAT_WIN_TITLE]->PrepareControl( IF_WORLDCHAT_WIN_TITLE );
	m_textCtl[IT_WORLDCHAT_WIN_NOTICE]->PrepareControl( IF_WORLDCHAT_WIN_NOTICE );

	
	if(gGameConfigFlag.m_bWorld_Chat_Limit)
	{
		m_textCtl[IT_WORLDCHAT_WIN_FREE_COUNT]->PrepareControl( IF_WORLDCHAT_WIN_FREE_COUNT );
	}
	
	m_titleBar->PrepareControl( IF_WORLDCHAT_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_WORLDCHAT_WIN_TITLE_BOTTOM );
	m_bgMessage->PrepareControl( IF_WORLDCHAT_WIN_MESSAGE );

	m_sendBtn->PrepareControl( IF_WORLDCHAT_WIN_SEND_BTN_0 );
	m_cancelBtn->PrepareControl( IF_WORLDCHAT_WIN_CANCEL_BTN_0 );
	m_exitBtn->PrepareControl( IF_WORLDCHAT_WIN_EXIT_BTN_0 );

	int cx, cy;
	cx = m_bgMessage->GetXPos();
	cy = m_bgMessage->GetYPos();

	g_chatManager->SetInput( cx, cy, MAX_OUTPUT_CHAT_STRING * 2, MAX_OUTPUT_CHAT_STRING * 2, INPUT_INDEX_WORLDCHAT, 1, 190, true );

}






void CIFWorldChatWin::
Enable(int state,enum enumType Type)
{ 
	CIFWindow::Enable(state);
	
	m_Type=Type;	
	switch(m_Type){
		case none:
			strncpy(m_Title,g_LPACK.GetMessage(0,972),MAXBYTES_TITLE);
			m_Title[MAXBYTES_TITLE]=NULL;
			strncpy(m_Desc,g_LPACK.GetMessage(LPACK_TYPE_NORMAL,993),MAXBYTES_DESC);
			m_Desc[MAXBYTES_DESC]=NULL;
			break;
		case speaker:
			strncpy(m_Title,g_LPACK.GetMessage(0,972),MAXBYTES_TITLE);
			m_Title[MAXBYTES_TITLE]=NULL;
			strncpy(m_Desc,g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,291),MAXBYTES_DESC);
			m_Desc[MAXBYTES_DESC]=NULL;
			break;
	}
}

int CIFWorldChatWin::WindowFrame()
{
	m_textCtl[IT_WORLDCHAT_WIN_TITLE]->SetText(m_Title);
	m_textCtl[IT_WORLDCHAT_WIN_NOTICE]->SetText(m_Desc);
	
	
	if(gGameConfigFlag.m_bWorld_Chat_Limit && g_cgv.myCharacterInfo->m_nMemberShipType <=0)
	{
		char szTemp[256];
		sprintf(szTemp,g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,254),
			g_cgv.myCharacterInfo->WorldChatting.iDecreaseCount,
			g_cgv.myCharacterInfo->WorldChatting.iMaxCount	);
		m_textCtl[IT_WORLDCHAT_WIN_FREE_COUNT]->SetText(szTemp);
	}

	
	m_sendBtn->SetButtonText( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,974) );
	m_cancelBtn->SetButtonText( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,521) );

	g_chatManager->m_ix[INPUT_INDEX_WORLDCHAT] = m_bgMessage->GetXPos();
	g_chatManager->m_iy[INPUT_INDEX_WORLDCHAT] = m_bgMessage->GetYPos();

	return( IF_CTL_NONE );
}

void CIFWorldChatWin::Render()
{
	if( !State() ) return;
	
	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	m_titleBar->Render();
	m_titleBottom->Render();

	m_sendBtn->RenderTextBtn();
	m_cancelBtn->RenderTextBtn();
	m_exitBtn->Render();

	m_bgMessage->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_WORLDCHAT_WIN_TITLE]->Render();
		m_textCtl[IT_WORLDCHAT_WIN_NOTICE]->Render();

		




		g_chatManager->RenderInputFrame( INPUT_INDEX_WORLDCHAT );
	}
	g_chatManager->EndFrame();
}


int CIFWorldChatWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) 
	{
		g_chatManager->SetInputState( INPUT_INDEX_WORLDCHAT, 1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_WORLDCHAT );
		return( IF_CTL_SELECT );
	}

	return IF_CTL_NONE;
}

enum CIFWorldChatWin::enumType CIFWorldChatWin::GetType(void) const
{
	return m_Type;

}

int CIFWorldChatWin::WindowProcess()
{
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_chatManager->SetInputState( INPUT_INDEX_WORLDCHAT, -1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );

		Enable( false );
		return IF_CTL_NONE;
	}

	if( m_sendBtn->ProcessButton() == IF_CTL_SELECT )
	{

		char msg[CHATSTRING];
		strcpy( msg, g_chatManager->GetInputStr( INPUT_INDEX_WORLDCHAT ) );
		g_chatManager->InitInputStr( INPUT_INDEX_WORLDCHAT );
		g_chatManager->SetInputState( INPUT_INDEX_WORLDCHAT, -1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
		Enable( false,m_Type );


		if(NULL != msg[0]){
			if(FALSE == g_cgv.GM.bChatAllow){
				g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), 
									 g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,490), 1, IF_MESSAGE_NONE );
				(gcpGMCtrl->m_cpChatBlockCtrl)->TransmitePacket_GMsystem_ChatBan_Req_ReaminTime();

				
				return IF_CTL_NONE;
				
			}		

			
			switch(m_Type){
				case none:
					GTH_SendMessage_WorldChatting( msg );
					break;
				case speaker:
					GTH_SendMessage_WorldChatting( msg ,TRUE);
					break;
			}
			
		}


		

		return IF_CTL_NONE;
	}

	if( m_exitBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_chatManager->SetInputState( INPUT_INDEX_WORLDCHAT, -1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );

		Enable( false );
		return IF_CTL_NONE;
	}
	
	return IF_CTL_NONE;
}










CIFChatWin::CIFChatWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_CHAT_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	m_selectBar = new CIFControl( this );

	
	m_closeBtn = new CIFButtonCtl( this );
	m_prevBtn = new CIFButtonCtl( this );
	m_nextBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_CHAT_WINDOW_TYPE );

	m_npcName[0] = 0;
	m_npcMessageStr[0] = 0;
	memset( m_selectStr, 0, sizeof( chatMessage ) * MAX_COUNT_IF_CHAT_SELECT_STRING);
	m_saveSelectStr[0] = 0;
}

CIFChatWin::~CIFChatWin()
{

}

void CIFChatWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_CHAT_WIN );

	
	m_textCtl[IT_CHAT_WIN_TITLE]->PrepareControl( IF_CHAT_WIN_TITLE_TEXT );
	m_textCtl[IT_CHAT_WIN_NPC_NAME_U]->PrepareControl( IF_CHAT_WIN_NPC_NAME_TEXT_U );
	m_textCtl[IT_CHAT_WIN_NPC_CHAT_U]->PrepareControl( IF_CHAT_WIN_NPC_CHAT_TEXT_U );
	m_textCtl[IT_CHAT_WIN_SELECT_U]->PrepareControl( IF_CHAT_WIN_SELECT_TEXT_U );
	
	
	m_titleBar->PrepareControl( IF_CHAT_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_CHAT_WIN_TITLE_BAR_BOTTOM );
	m_selectBar->PrepareControl( IF_CHAT_WIN_SELECT_BAR );

	
	m_closeBtn->PrepareControl( IF_CHAT_WIN_CLOSE_BTN_0 );
	m_prevBtn->PrepareControl( IF_CHAT_WIN_PREV_BTN_0 );
	m_nextBtn->PrepareControl( IF_CHAT_WIN_NEXT_BTN_0 );

	m_selectBarSavePos = g_ifMng->m_iPos[IF_CHAT_WIN_SELECT_BAR].clientY;
}

int CIFChatWin::WindowFrame()
{
	char szTemp[256];

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 483 ) );
	m_textCtl[IT_CHAT_WIN_TITLE]->SetText( szTemp );

	sprintf( szTemp, "%s", m_npcName );
	m_textCtl[IT_CHAT_WIN_NPC_NAME_U]->SetText( szTemp );
	m_textCtl[IT_CHAT_WIN_NPC_NAME_U]->SetColor( 0xffffff00 );

	return( IF_CTL_NONE );
}

void CIFChatWin::Render()
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
		m_textCtl[IT_CHAT_WIN_TITLE]->Render();
		m_textCtl[IT_CHAT_WIN_NPC_NAME_U]->Render();
		if( m_msgIndex >= 0 )
		{
			int x, y, xs, ys, i;
			x = m_textCtl[IT_CHAT_WIN_NPC_CHAT_U]->GetXPos();
			y = m_textCtl[IT_CHAT_WIN_NPC_CHAT_U]->GetYPos();
			xs = m_textCtl[IT_CHAT_WIN_NPC_CHAT_U]->GetXSize();
			ys = m_textCtl[IT_CHAT_WIN_NPC_CHAT_U]->GetYSize();
			g_chatManager->SetColor( 0xffffffff );
			g_chatManager->DrawTextRect( x, y, m_npcMessageStr, xs, ys, m_curPage );
			
			g_chatManager->SetColor( 0xffffffff );
			x = m_textCtl[IT_CHAT_WIN_SELECT_U]->GetXPos();
			y = m_textCtl[IT_CHAT_WIN_SELECT_U]->GetYPos();
			for( i = 0; i < m_numSelectStr; i ++ )
			{
				g_chatManager->DrawText( x, y + i * 13, m_selectStr[i] );
			}
		}
	}
	g_chatManager->EndFrame();
}

int CIFChatWin::WindowSelection()
{
	int ret;
	
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFChatWin::WindowProcess()
{
	int mx, my;
	int x1, y1, xs, ys;
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT ) 
	{
		GTH_SendMessage_RequestQuitNpcChat();
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
	x1 = m_textCtl[IT_CHAT_WIN_SELECT_U]->GetXPos();
	y1 = m_textCtl[IT_CHAT_WIN_SELECT_U]->GetYPos();
	xs = m_textCtl[IT_CHAT_WIN_SELECT_U]->GetXSize();
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
			GTH_SendMessage_RequestNpcSelectMessage( m_selectedIdx );
			strcpy( m_saveSelectStr, m_selectStr[m_selectedIdx] );
		}
		g_ifMng->m_iPos[IF_CHAT_WIN_SELECT_BAR].clientY = m_selectBarSavePos + m_selectedIdx * 13;
		m_selectBar->Enable();
	}
	else
	{
		m_selectBar->Enable( false );
	}

	return IF_CTL_NONE;
}

void CIFChatWin::InitChat()
{
	int i;
	for( i = 0; i < MAX_COUNT_IF_CHAT_SELECT_STRING; i ++ )
	{
		memset( m_selectStr[i], 0, sizeof( chatMessage ) );
	}
	m_numSelectStr = 0;
	m_msgIndex = -1;
	
	Enable();
	g_ifMng->SetFocus( this );
}

void CIFChatWin::MakeNpcMessage( int idx )
{
	int x, y, xs, ys;


	strcpy( m_npcMessageStr, g_LPACK.GetMassage( LPACK_TYPE_NPC, idx ) );

	m_msgIndex = idx;
	
	x = m_textCtl[IT_CHAT_WIN_NPC_CHAT_U]->GetXPos();
	y = m_textCtl[IT_CHAT_WIN_NPC_CHAT_U]->GetYPos();
	xs = m_textCtl[IT_CHAT_WIN_NPC_CHAT_U]->GetXSize();
	ys = m_textCtl[IT_CHAT_WIN_NPC_CHAT_U]->GetYSize();

	m_curPage = 0;
	m_maxPage = g_chatManager->DrawTextRect( x, y, m_npcMessageStr, xs, ys, -1 );

	m_numSelectStr = 0;
}


void CIFChatWin::MakeSelectMessage( int pos, int idx )
{

	strcpy( m_selectStr[pos], g_LPACK.GetMassage( LPACK_TYPE_ANSWER, idx ) );
	if( pos >= m_numSelectStr ) m_numSelectStr = pos + 1;
}

void CIFChatWin::QuitChat()
{
	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_storeWin->Enable( false );
	g_ifMng->m_depotWin->Enable( false );
	g_ifMng->m_precocityWin->Enable( false );
	g_ifMng->m_craftWin->Enable( false );
	
	
	g_ifMng->m_EventCraftWin->Enable( false );
	
	g_ifMng->m_upgradeWin->Enable( false );
}












CIFPartyWin::CIFPartyWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	Enable( false );
	
	for( int i = 0; i < NUM_IT_PARTY_WIN; i ++ )
	{
		m_textCtl[i] = new CIFText( this );
	}
	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		m_memberWin[i] = new CIFControl( this );
		m_memberRABar[i] = new CIFBarCtl( this );
		m_memberSABar[i] = new CIFBarCtl( this );
		m_memberWin[i]->Enable( false );
		m_memberRABar[i]->Enable( false );
		m_memberSABar[i]->Enable( false );
	}

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_PARTY_WINDOW_TYPE );

	m_selectedMemberIdx = -1;
	m_otherMemberName[0] = 0;
}

CIFPartyWin::~CIFPartyWin()
{
}
	
void CIFPartyWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_PARTY_WIN );
	
	m_textCtl[IT_PARTY_WIN]->PrepareControl( IF_PARTY_WIN_TEXT );
	m_textCtl[IT_PARTY_WIN_M0_LEVEL]->PrepareControl( IF_PARTY_WIN_M0_LEVEL_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M0_NAME]->PrepareControl( IF_PARTY_WIN_M0_NAME_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M1_LEVEL]->PrepareControl( IF_PARTY_WIN_M1_LEVEL_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M1_NAME]->PrepareControl( IF_PARTY_WIN_M1_NAME_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M2_LEVEL]->PrepareControl( IF_PARTY_WIN_M2_LEVEL_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M2_NAME]->PrepareControl( IF_PARTY_WIN_M2_NAME_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M3_LEVEL]->PrepareControl( IF_PARTY_WIN_M3_LEVEL_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M3_NAME]->PrepareControl( IF_PARTY_WIN_M3_NAME_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M4_LEVEL]->PrepareControl( IF_PARTY_WIN_M4_LEVEL_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M4_NAME]->PrepareControl( IF_PARTY_WIN_M4_NAME_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M5_LEVEL]->PrepareControl( IF_PARTY_WIN_M5_LEVEL_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M5_NAME]->PrepareControl( IF_PARTY_WIN_M5_NAME_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M6_LEVEL]->PrepareControl( IF_PARTY_WIN_M6_LEVEL_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M6_NAME]->PrepareControl( IF_PARTY_WIN_M6_NAME_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M7_LEVEL]->PrepareControl( IF_PARTY_WIN_M7_LEVEL_TEXT_U );
	m_textCtl[IT_PARTY_WIN_M7_NAME]->PrepareControl( IF_PARTY_WIN_M7_NAME_TEXT_U );

	
	for( int i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		m_memberWin[i]->PrepareControl( IF_PARTY_WIN_M0 + i * 5 );
		m_memberRABar[i]->PrepareControl( IF_PARTY_WIN_M0_RA_BAR + i * 5 );
		m_memberSABar[i]->PrepareControl( IF_PARTY_WIN_M0_SA_BAR + i * 5 );
	}
}

int CIFPartyWin::WindowFrame()
{
	char szTemp[256];

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 527 ) );
	m_textCtl[IT_PARTY_WIN]->SetText( szTemp );

	if( g_cgv.myCharacterInfo->organizeServer < 0 ) return( IF_CTL_NONE );

	if( !State() ) return( IF_CTL_NONE );

	return( IF_CTL_NONE );
}

void CIFPartyWin::Render()
{
	if( g_cgv.myCharacterInfo->organizeServer >= 0 ) Enable();
	else 
	{
		m_selectedMemberIdx = -1;
		Enable( false );
	}
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();

	for( int i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		m_memberWin[i]->Render();
		m_memberRABar[i]->Render();
		m_memberSABar[i]->Render();
	}

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_PARTY_WIN]->Render();
		if( m_memberWin[0]->State() )
		{
			m_textCtl[IT_PARTY_WIN_M0_LEVEL]->Render();
			m_textCtl[IT_PARTY_WIN_M0_NAME]->Render();
		}
		if( m_memberWin[1]->State() )
		{
			m_textCtl[IT_PARTY_WIN_M1_LEVEL]->Render();
			m_textCtl[IT_PARTY_WIN_M1_NAME]->Render();
		}
		if( m_memberWin[2]->State() )
		{
			m_textCtl[IT_PARTY_WIN_M2_LEVEL]->Render();
			m_textCtl[IT_PARTY_WIN_M2_NAME]->Render();
		}
		if( m_memberWin[3]->State() )
		{
			m_textCtl[IT_PARTY_WIN_M3_LEVEL]->Render();
			m_textCtl[IT_PARTY_WIN_M3_NAME]->Render();
		}
		if( m_memberWin[4]->State() )
		{
			m_textCtl[IT_PARTY_WIN_M4_LEVEL]->Render();
			m_textCtl[IT_PARTY_WIN_M4_NAME]->Render();
		}
		if( m_memberWin[5]->State() )
		{
			m_textCtl[IT_PARTY_WIN_M5_LEVEL]->Render();
			m_textCtl[IT_PARTY_WIN_M5_NAME]->Render();
		}
		if( m_memberWin[6]->State() )
		{
			m_textCtl[IT_PARTY_WIN_M6_LEVEL]->Render();
			m_textCtl[IT_PARTY_WIN_M6_NAME]->Render();
		}
		if( m_memberWin[7]->State() )
		{
			m_textCtl[IT_PARTY_WIN_M7_LEVEL]->Render();
			m_textCtl[IT_PARTY_WIN_M7_NAME]->Render();
		}
	}
	g_chatManager->EndFrame();
}

int CIFPartyWin::WindowSelection()
{
	int ret;

	
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );

	return( IF_CTL_NONE );
}

int CIFPartyWin::WindowProcess()
{
	if( g_cgv.myCharacterInfo->organizeServer < 0 ) return IF_CTL_NONE;

	int idx;
	for( int i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( !m_memberWin[i]->State() ) continue;
		if( m_memberWin[i]->Selection() == IF_LBDN )
		{
			idx = m_memberIdx[i];
			if( g_cgv.myCharacterInfo->isLeader ) 
			{
				m_selectedMemberIdx = idx;
				strcpy( m_otherMemberName, g_cgv.myCharacterInfo->member[idx].memberName );
			}

			GetPartyTarget( idx );
		}
	}

	return IF_CTL_NONE;
}

void CIFPartyWin::GetPartyTarget( int idx )
{
	Fx_CHARACTER_t *character;


	int numCharacter = g_charManager->m_iNumCharacters;

	if( g_cgv.myCharacterInfo->member[idx].serverIndex < 0 ) return;

	character = &g_charManager->m_Characters[0];
	for( int i = 0; i < numCharacter; i ++, character ++ )
	{
		if( character->entityType == ENTITY_PC )
		{
			if( !stricmp( character->name, g_cgv.myCharacterInfo->member[idx].memberName ) )
			{
				if( g_GameTarget.GTH_GetTargetDistance( character ) < AVAILABLE_DISTANCE_ENTITYMENU )
				{
					g_GameTarget.GTH_ChangeTarget( character );
				}
				return;
			}
		}
	}
}

void CIFPartyWin::SetPartyMember( int idx, char *name )
{
	m_otherMemberIdxOnServer = idx;
	strcpy( m_otherMemberName, name );
}

void CIFPartyWin::InitMemberInfo()
{
	for( int i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		m_memberWin[i]->Enable( false );
		m_memberRABar[i]->Enable( false );
		m_memberSABar[i]->Enable( false );
	}
}

void CIFPartyWin::UpdateMemberInfo( int idx, int memberIdx, int memberActive, int curRA, int curSA, int maxRA, int maxSA )
{
	char szTemp[256];

	m_memberWin[idx]->Enable();
	sprintf( szTemp, "LV.%d", g_cgv.myCharacterInfo->member[memberIdx].level );
	m_textCtl[IT_PARTY_WIN_M0_LEVEL + idx * 2]->SetText( szTemp );

	sprintf( szTemp, "%s", g_cgv.myCharacterInfo->member[memberIdx].memberName );
	m_textCtl[IT_PARTY_WIN_M0_NAME + idx * 2]->SetText( szTemp );
	if( !memberActive ) m_textCtl[IT_PARTY_WIN_M0_NAME + idx * 2]->SetColor( 0xff808080 );
	else
	{
		if( g_cgv.myCharacterInfo->member[memberIdx].isLeader )
		{
			m_textCtl[IT_PARTY_WIN_M0_NAME + idx * 2]->SetColor( 0xff00ffff );
		}
		else
		{
			m_textCtl[IT_PARTY_WIN_M0_NAME + idx * 2]->SetColor( 0xffffffff );
		}
		if( m_selectedMemberIdx == memberIdx )
		{
			m_textCtl[IT_PARTY_WIN_M0_NAME + idx * 2]->SetColor( 0xffffd700 );
		}
	}
	
	
	if( curRA > 0 ) 
	{
		m_memberRABar[idx]->Enable();
		m_memberSABar[idx]->Enable();
		m_memberRABar[idx]->UpdateBarState( curRA, maxRA );
		m_memberSABar[idx]->UpdateBarState( curSA, maxSA );
	}
	else
	{
		m_memberRABar[idx]->Enable( false );
		m_memberSABar[idx]->Enable( false );
	}

	m_memberIdx[idx] = memberIdx;
}












CIFStoreWin::CIFStoreWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_STORE_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	m_pageBar = new CIFControl( this );
	m_repairBar = new CIFControl( this );
	m_invenBar = new CIFControl( this );
	
	
	m_closeBtn = new CIFButtonCtl( this );
	m_repairOnceBtn = new CIFButtonCtl( this );
	m_repairAllBtn = new CIFButtonCtl( this );

	InitStore();

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_STORE_WINDOW_TYPE );

	
	m_LockGambleStore = FALSE;

}

CIFStoreWin::~CIFStoreWin()
{

}

void CIFStoreWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_STORE_WIN );

	
	m_textCtl[IT_STORE_WIN_TITLE]->PrepareControl( IF_STORE_WIN_TITLE_TEXT );
	m_textCtl[IT_STORE_WIN_PAGE_1]->PrepareControl( IF_STORE_WIN_PAGE_1_TEXT );
	m_textCtl[IT_STORE_WIN_PAGE_2]->PrepareControl( IF_STORE_WIN_PAGE_2_TEXT );
	m_textCtl[IT_STORE_WIN_PAGE_3]->PrepareControl( IF_STORE_WIN_PAGE_3_TEXT );
	m_textCtl[IT_STORE_WIN_REPAIR_ONCE]->PrepareControl( IF_STORE_WIN_REPAIR_ONCE_TEXT );
	m_textCtl[IT_STORE_WIN_REPAIR_ALL]->PrepareControl( IF_STORE_WIN_REPAIR_ALL_TEXT );
	m_textCtl[IT_STORE_WIN_INVEN_POS]->PrepareControl( IF_STORE_WIN_INVEN_POS );
	
	
	m_titleBar->PrepareControl( IF_STORE_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_STORE_WIN_TITLE_BAR_BOTTOM );
	m_pageBar->PrepareControl( IF_STORE_WIN_PAGE_BAR );
	m_repairBar->PrepareControl( IF_STORE_WIN_REPAIR_BAR );
	m_invenBar->PrepareControl( IF_STORE_WIN_INVEN );
	
	
	m_closeBtn->PrepareControl( IF_STORE_WIN_CLOSE_BTN_0 );
	m_repairOnceBtn->PrepareControl( IF_STORE_WIN_REPAIR_ONCE_BTN_0 );
	m_repairAllBtn->PrepareControl( IF_STORE_WIN_REPAIR_ALL_BTN_0 );
}

int CIFStoreWin::WindowFrame()
{
	char szTemp[256];
	
	
	SetPageName();
	
	

	if( m_curPage == 0 )
	{
		m_textCtl[IT_STORE_WIN_PAGE_1]->SetColor( 0xffffffff );
	} 
	else if( m_curPage == 1 )
	{
		m_textCtl[IT_STORE_WIN_PAGE_2]->SetColor( 0xffffffff );
	} 
	else if( m_curPage == 2 )
	{
		m_textCtl[IT_STORE_WIN_PAGE_3]->SetColor( 0xffffffff );
	}
	

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 541 ) );
	m_textCtl[IT_STORE_WIN_REPAIR_ONCE]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 542 ) );
	m_textCtl[IT_STORE_WIN_REPAIR_ALL]->SetText( szTemp );

	return( IF_CTL_NONE );
}






void CIFStoreWin::SetPageName()
{		
	
	char Title [STORE_STRLEN+1]      = {0};	
	char PageText_1 [STORE_STRLEN+1] = {0};	
	char PageText_2 [STORE_STRLEN+1] = {0};	
	char PageText_3 [STORE_STRLEN+1] = {0};	

	
	if (m_StoreType ==  GAMBLE_ITEM_STORE)
	{
		
		_snprintf( Title, STORE_STRLEN,"%s", g_LPACK.GetMassage( 9, 38 ) );								
		
		_snprintf( PageText_1, STORE_STRLEN,"%s", g_LPACK.GetMassage( 9, 39 ) );					
		
		_snprintf( PageText_2, STORE_STRLEN,"%s", g_LPACK.GetMassage( 9, 40 ) );						
		
		_snprintf( PageText_3, STORE_STRLEN,"%s", g_LPACK.GetMassage( 9, 41 ) );					
	}
	else 
	{
		_snprintf( Title, STORE_STRLEN, "%s", g_LPACK.GetMassage( 0, 540 ) );				
		if ( g_cgv.pTargetChar != NULL )			
		{
			switch(g_cgv.pTargetChar->tableIdx)
			{
				
			case 108:	
			case 105:	
			case 13:	
			case 112:	
				{
					
					_snprintf( PageText_1,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 423 ));	
					
					_snprintf( PageText_2,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 173 ));									
				}
				break;
				
			case 104:	
			case 18:	
			case 111:	
			case 107:	
				{
					
					_snprintf( PageText_1,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 40 ));											
					
					_snprintf( PageText_2,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 424 ));									
				}break;
				
			case 122:	
				{			
					
					_snprintf( PageText_1,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 425 ));											
					
					_snprintf( PageText_2,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 426 ));									
				}break;
				
			case 121:	
				{			
					
					_snprintf( PageText_1,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 427 ));											
					
					_snprintf( PageText_2,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 426 ));									
				}break;
			case 119:	
				{			
					
					_snprintf( PageText_1,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 428 ));											
					
					_snprintf( PageText_2,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 429 ));									
					
					_snprintf( PageText_3,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 173 ));									
					
				}break;
			case 124:	
				{			
					
					_snprintf( PageText_1,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 430 ));													
					
					_snprintf( PageText_2,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 431 ));									
				}break;
			case 123:	
				{			
					
					_snprintf( PageText_1,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 432 ));											
					
					_snprintf( PageText_2,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 433 ));				
					
				}break;
			case 120:	
				{			
					
					_snprintf( PageText_1,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 434 ));											
					
					_snprintf( PageText_2,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 435 ));					
					
					_snprintf( PageText_3,  STORE_STRLEN, "%s", g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 173 ));				
					
				}break;
			}
			
		}				

	}

	Title[STORE_STRLEN]		 = NULL;
	PageText_1[STORE_STRLEN] = NULL;
	PageText_2[STORE_STRLEN] = NULL;
	PageText_3[STORE_STRLEN] = NULL;

	m_textCtl[IT_STORE_WIN_TITLE]->SetText( Title );		
	m_textCtl[IT_STORE_WIN_PAGE_1]->SetText( PageText_1 );
	m_textCtl[IT_STORE_WIN_PAGE_2]->SetText( PageText_2 );
	m_textCtl[IT_STORE_WIN_PAGE_3]->SetText( PageText_3 );	
	
	m_textCtl[IT_STORE_WIN_PAGE_1]->SetColor( 0xff808080 );
	m_textCtl[IT_STORE_WIN_PAGE_2]->SetColor( 0xff808080 );
	m_textCtl[IT_STORE_WIN_PAGE_3]->SetColor( 0xff808080 );
	
}

void CIFStoreWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	m_pageBar->Render();
	m_repairBar->Render();
	m_invenBar->Render();
	
	
	m_closeBtn->Render();
	m_repairOnceBtn->Render();
	m_repairAllBtn->Render();

	
	g_ifMng->m_iconCtl->BeginFrame();
	{
		
		for( int i = 0; i < 36; i ++ )
		{
			int idx, itemTableIdx, cx, cy;
			idx = m_curPage * 36 + i;
			itemTableIdx = m_storeItem[idx].item.itemTableIdx;
			if( itemTableIdx >= 0 )
			{
				cx = m_textCtl[IT_STORE_WIN_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
				cy = m_textCtl[IT_STORE_WIN_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
				if( itemTableIdx == STORE_ITEM_INDEX_EPACK_RA )
				{
					g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, 142 );
				}
				else if( itemTableIdx == STORE_ITEM_INDEX_EPACK_SA )
				{
					g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, 143 );
				}
				else if( itemTableIdx >= STORE_SKILL_MASK )
				{
					int skillTableIdx = itemTableIdx - STORE_SKILL_MASK;
					
					

					
					int nIconID=0;
					CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
					if(pSkillTable!=NULL) 
					{
						nIconID = pSkillTable->m_nIconID;
					}

					if( !GTH_CheckRequire( g_cgv.myCharacterInfo, 1, &skillTableIdx ) && m_StoreType != GAMBLE_ITEM_STORE)
					{
						g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL,nIconID + 1, 1.0f, 1.0f, 0x80ff0000 );
					}
					else
					{
						g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL,nIconID + 1);
					}				}
				else
				{
					
					
					if( !GTH_CheckRequire( g_cgv.myCharacterInfo, 0, &m_storeItem[idx].item ) &&  m_StoreType != GAMBLE_ITEM_STORE)					
					{
						g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID, 1.0f, 1.0f, 0x80ff0000 );
					}
					else
					{
						g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
					}
				}
			}
		}
	}
	g_ifMng->m_iconCtl->EndFrame();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_STORE_WIN_TITLE]->Render();
		m_textCtl[IT_STORE_WIN_PAGE_1]->Render();
		m_textCtl[IT_STORE_WIN_PAGE_2]->Render();
		m_textCtl[IT_STORE_WIN_PAGE_3]->Render();
		g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
		m_textCtl[IT_STORE_WIN_REPAIR_ONCE]->Render();
		m_textCtl[IT_STORE_WIN_REPAIR_ALL]->Render();
	}
	g_chatManager->EndFrame();
}

int CIFStoreWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFStoreWin::WindowProcess()
{
	char szTemp[256], szTemp2[128];
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT ) GTH_SendMessage_RequestQuitNpcChat();
	if( m_textCtl[IT_STORE_WIN_PAGE_1]->Selection() == IF_LBDN ) 
		m_curPage = 0;
	if( m_textCtl[IT_STORE_WIN_PAGE_2]->Selection() == IF_LBDN ) 
		m_curPage = 1;
	if( m_textCtl[IT_STORE_WIN_PAGE_3]->Selection() == IF_LBDN ) 
		m_curPage = 2;

	if( m_repairAllBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		
		
		
		GTH_SendMessageRequest_RepairItemConfirm(REPAIR_ITEM_ALL, INVENTYPE_INVENTORY, 0);
		
		
		
	}
	if( m_repairOnceBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( g_input->GetCursorState( CURSOR_POINTER ) ) g_input->ChangeCursor( CURSOR_REPAIR, MOUSE_CURSOR_TYPE );
	}


	
	
	int x, y, pos;
	int mx, my, temp;
	
	mx = g_input->GetPosX() - m_textCtl[IT_STORE_WIN_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_STORE_WIN_INVEN_POS]->GetYPos();

	if( mx <= 0 || my <= 0 ) return( IF_CTL_NONE );
	if( mx >= 34 * 6 ||  my >= 33 * 6 ) return( IF_CTL_NONE );

	temp = mx / 34;
	x = temp % 6;
	temp = my / 33;
	y = temp % 6;

	pos = y * 6 + x;

	
	if( pos < 0 || pos >= 36 )
	{
		return( IF_CTL_NONE );
	}
	
	int itemIdx = m_curPage * 36 + pos;
	int itemTableIdx = m_storeItem[itemIdx].item.itemTableIdx;
	int mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;
	int price;
	item_t *item;

	
	if( g_input->GetLBNone() ) 
	{
		
		if( itemTableIdx < 0 ) return( IF_CTL_NONE );
		if( itemTableIdx == STORE_ITEM_INDEX_EPACK_RA )
		{
			g_ifMng->InitRolloverText();
			int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
			
			if( epackIdx < 0 ) return IF_CTL_NONE;

			int idx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;
		
			
			price = (int)GetEpackRechargePrice_RA( g_cgv.myCharacterInfo->onceRecoveryRA,
													m_raCost,
													(float)g_itemTable[idx].level );

			strcpy( szTemp2, g_ifMng->GetNumberString( price ) );
			sprintf( szTemp, g_LPACK.GetMassage( 0, 426 ), g_cgv.myCharacterInfo->onceRecoveryRA, szTemp2 );
			g_ifMng->AddRolloverText( szTemp );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		}
		else if( itemTableIdx == STORE_ITEM_INDEX_EPACK_SA )
		{
			g_ifMng->InitRolloverText();
			
			int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
			
			if( epackIdx < 0 ) return IF_CTL_NONE;

			int idx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;
			
			
			price = (int)GetEpackRechargePrice_SA( g_cgv.myCharacterInfo->onceRecoverySA,
													m_saCost,
													(float)g_itemTable[idx].level );

			strcpy( szTemp2, g_ifMng->GetNumberString( price ) );
			sprintf( szTemp, g_LPACK.GetMassage( 0, 427 ), g_cgv.myCharacterInfo->onceRecoverySA, szTemp2 );
			g_ifMng->AddRolloverText( szTemp );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		}
		else if( itemTableIdx >= STORE_SKILL_MASK )
		{
			g_ifMng->InitRolloverText();
			g_ifMng->MakeSkillRollover( itemTableIdx - STORE_SKILL_MASK, 1 );
			
			strcpy( szTemp2, g_ifMng->GetNumberString( m_storeItem[itemIdx].price ) );
			sprintf( szTemp, g_LPACK.GetMassage( 0, 365 ), szTemp2 );
			g_ifMng->AddRolloverText( " " );
			g_ifMng->AddRolloverText( szTemp, 0 );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		}
		else
		{
			g_ifMng->InitRolloverText();
			g_ifMng->MakeItemRollover( &m_storeItem[itemIdx].item, false );
			
			strcpy( szTemp2, g_ifMng->GetNumberString( m_storeItem[itemIdx].price ) );

			if ( m_StoreType == GAMBLE_ITEM_STORE)			
				sprintf( szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 532), szTemp2 );
			else
				sprintf( szTemp, g_LPACK.GetMassage( 0, 318 ), szTemp2 );

			g_ifMng->AddRolloverText( szTemp, 0 );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		}
	}
	
	
	if( g_input->GetLBUp() )
	{
		
		if( mouseItemIdx < 0 && itemTableIdx >= 0 )
		{ 
			
			if( itemTableIdx == STORE_ITEM_INDEX_EPACK_RA ||
				itemTableIdx == STORE_ITEM_INDEX_EPACK_SA )
			{
				int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
				
				if( epackIdx < 0 ) return IF_CTL_NONE;

				int itemTableIdx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;

				
				if( g_itemTable[itemTableIdx].epkChargeMethod == TypeItemTable_t::EPK_CHARGE_TYPE_ITEM)
				{
					
					
					g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 409 ));
					return IF_CTL_NONE;
				}
			}

			if( itemTableIdx == STORE_ITEM_INDEX_EPACK_RA )
			{
				
				g_ifMng->m_stackWin->InitStack( g_LPACK.GetMassage( 0, 543 ), &m_storeItem[itemIdx].item, IF_STACK_TYPE_EPACK, 0, m_raCost );
			}
			else if( itemTableIdx == STORE_ITEM_INDEX_EPACK_SA )
			{
				
				g_ifMng->m_stackWin->InitStack( g_LPACK.GetMassage( 0, 543 ), &m_storeItem[itemIdx].item, IF_STACK_TYPE_EPACK, 1, m_saCost );
			}
			else if( itemTableIdx >= STORE_SKILL_MASK )
			{
				int skillTableIdx = itemTableIdx - STORE_SKILL_MASK; 

				
				m_buyItemIdx = itemIdx;
				
				strcpy( szTemp2, g_ifMng->GetNumberString( m_storeItem[itemIdx].price ) );
				
				
				
				sprintf( szTemp, g_LPACK.GetMassage( 0, 544 ), g_SkillTableDB.GetSkillName(skillTableIdx), szTemp2 );

				g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_STORE_BUY_SKILL );
			}
			else
			{
				if( m_storeItem[itemIdx].price == 0 )
				{
					g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( 0, 22 ) );
					return IF_CTL_NONE;
				}
				else if( GTH_IsStackItem( &m_storeItem[itemIdx].item ) )
				{
					
					g_ifMng->m_stackWin->InitStack( g_LPACK.GetMassage( 0, 545 ), &m_storeItem[itemIdx].item, IF_STACK_TYPE_BUY_ITEM, itemIdx, (float)m_storeItem[itemIdx].price );
				}
				else
				{
					
					m_buyItemIdx = itemIdx;
					
					strcpy( szTemp2, g_ifMng->GetNumberString( m_storeItem[itemIdx].price ) );

					if ( m_StoreType == GAMBLE_ITEM_STORE)			
					{
						
						if ( m_LockGambleStore == TRUE)
							return IF_CTL_NONE;

						sprintf( szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 533), m_storeItem[itemIdx].item.name, szTemp2 );
					}
					else
						sprintf( szTemp, g_LPACK.GetMassage( 0, 546 ), m_storeItem[itemIdx].item.name, szTemp2 );

					g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_STORE_BUY_ITEM );
				}
			}
		}
		
		else if( mouseItemIdx >= 0 )
		{
			m_sellItemIdx = mouseItemIdx;
			item = &g_cgv.myCharacterInfo->item[mouseItemIdx];
			int itemTableIdx = item->itemTableIdx;
			switch(itemTableIdx)
			{
				
			case CGolryunBattle::TWOGYUN_100000_ITEM_INDEX:
				g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,397));
				return IF_CTL_NONE;
			case CGolryunBattle::TWOGYUN_1000000_ITEM_INDEX:
				g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,397));
				return IF_CTL_NONE;
			}
			
			price = GTH_GetSellPrice( item );
			
			
			strcpy( szTemp2, g_ifMng->GetNumberString( price ) );
			sprintf( szTemp, g_LPACK.GetMassage( 0, 547 ), item->name, szTemp2 );
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_STORE_SELL_ITEM );
		}
	}
	
	return IF_CTL_NONE;
}

void CIFStoreWin::InitStore()
{
	for( int i = 0; i < MAX_NUMBER_OF_STORE_ITEM; i ++ )
	{
		GTH_InitItem( &m_storeItem[i].item );
		m_storeItem[i].idxOnServer = -1;
	}
	m_curPage = 0;
}













CIFTradeWin::CIFTradeWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_TRADE_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );
	m_otherBar = new CIFControl( this );
	m_otherInvenBar = new CIFControl( this );
	m_otherLine = new CIFControl( this );
	m_otherSENumBar = new CIFControl( this );
	m_myBar = new CIFControl( this );
	m_myInvenBar = new CIFControl( this );
	m_myLine = new CIFControl( this );
	m_mySENumBar = new CIFControl( this );

	m_okBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );
	m_admitBtn = new CIFButtonCtl( this );
	m_closeBtn = new CIFButtonCtl( this );
	m_seInputBtn = new CIFButtonCtl( this );
	
	
	m_otherIdxOnServer = 0;
	strcpy( m_otherName, "none" );

	m_myTradeSE = 0;
	m_otherTradeSE = 0;

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_TRADE_WINDOW_TYPE );
}

CIFTradeWin::~CIFTradeWin()
{

}
	
void CIFTradeWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_TRADE_WIN );

	m_textCtl[IT_TRADE_WIN_TITLE]->PrepareControl( IF_TRADE_WIN_TITLE_TEXT );
	m_textCtl[IT_TRADE_WIN_OTHER_NAME_U]->PrepareControl( IF_TRADE_WIN_OTHER_NAME_TEXT_U );
	m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->PrepareControl( IF_TRADE_WIN_OTHER_INVEN_POS );
	m_textCtl[IT_TRADE_WIN_OTHER_SE]->PrepareControl( IF_TRADE_WIN_OTHER_SE_TEXT );
	m_textCtl[IT_TRADE_WIN_OTHER_SE_U]->PrepareControl( IF_TRADE_WIN_OTHER_SE_TEXT_U );
	m_textCtl[IT_TRADE_WIN_OTHER_OK]->PrepareControl( IF_TRADE_WIN_OTHER_OK_TEXT );
	m_textCtl[IT_TRADE_WIN_OTHER_ADMIT]->PrepareControl( IF_TRADE_WIN_OTHER_ADMIT_TEXT );
	m_textCtl[IT_TRADE_WIN_MY_NAME_U]->PrepareControl( IF_TRADE_WIN_MY_NAME_TEXT_U );
	m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->PrepareControl( IF_TRADE_WIN_MY_INVEN_POS );
	m_textCtl[IT_TRADE_WIN_MY_SE]->PrepareControl( IF_TRADE_WIN_MY_SE_TEXT );
	m_textCtl[IT_TRADE_WIN_MY_SE_U]->PrepareControl( IF_TRADE_WIN_MY_SE_TEXT_U );
	m_textCtl[IT_TRADE_OK]->PrepareControl( IF_TRADE_OK_TEXT );
	m_textCtl[IT_TRADE_CANCEL]->PrepareControl( IF_TRADE_CANCEL_TEXT );
	m_textCtl[IT_TRADE_ADMIT]->PrepareControl( IF_TRADE_ADMIT_TEXT );
	m_textCtl[IT_TRADE_SEINPUT]->PrepareControl( IF_TRADE_SEINPUT_TEXT );

	m_titleBar->PrepareControl( IF_TRADE_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_TRADE_WIN_TITLE_BOTTOM );
	m_otherBar->PrepareControl( IF_TRADE_WIN_OTHER_BAR );
	m_otherInvenBar->PrepareControl( IF_TRADE_WIN_OTHER_INVEN );
	m_otherLine->PrepareControl( IF_TRADE_WIN_OTHER_LINE );
	m_otherSENumBar->PrepareControl( IF_TRADE_WIN_OTHER_SE_NUM_BAR );
	m_myBar->PrepareControl( IF_TRADE_WIN_MY_BAR );
	m_myInvenBar->PrepareControl( IF_TRADE_WIN_MY_INVEN );
	m_myLine->PrepareControl( IF_TRADE_WIN_MY_LINE );
	m_mySENumBar->PrepareControl( IF_TRADE_WIN_MY_SE_NUM_BAR );

	m_okBtn->PrepareControl( IF_TRADE_OK_BTN_0 );
	m_cancelBtn->PrepareControl( IF_TRADE_CANCEL_BTN_0 );
	m_admitBtn->PrepareControl( IF_TRADE_ADMIT_BTN_0 );
	m_closeBtn->PrepareControl( IF_TRADE_WIN_CLOSE_BTN_0 );
	m_seInputBtn->PrepareControl( IF_TRADE_SEINPUT_BTN_0 );
	
}

int CIFTradeWin::WindowFrame()
{
	char szTemp[256];

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 548 ) );
	m_textCtl[IT_TRADE_WIN_TITLE]->SetText( szTemp );

	sprintf( szTemp, "%s", m_otherName );
	m_textCtl[IT_TRADE_WIN_OTHER_NAME_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
	m_textCtl[IT_TRADE_WIN_OTHER_SE]->SetText( szTemp );

	strcpy( szTemp, g_ifMng->GetNumberString( m_otherTradeSE ) );
	m_textCtl[IT_TRADE_WIN_OTHER_SE_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 549 ) );
	m_textCtl[IT_TRADE_WIN_OTHER_OK]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 550 ) );
	m_textCtl[IT_TRADE_WIN_OTHER_ADMIT]->SetText( szTemp );

	sprintf( szTemp, "%s", g_cgv.myCharacterInfo->name );
	m_textCtl[IT_TRADE_WIN_MY_NAME_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
	m_textCtl[IT_TRADE_WIN_MY_SE]->SetText( szTemp );

	strcpy( szTemp, g_ifMng->GetNumberString( m_myTradeSE ) );
	m_textCtl[IT_TRADE_WIN_MY_SE_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 549 ) );
	m_textCtl[IT_TRADE_OK]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 551 ) );
	m_textCtl[IT_TRADE_CANCEL]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 548 ) );
	m_textCtl[IT_TRADE_ADMIT]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 552 ) );
	m_textCtl[IT_TRADE_SEINPUT]->SetText( szTemp );
	
	return( IF_CTL_NONE );
}

void CIFTradeWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	m_titleBar->Render();
	m_titleBottom->Render();
	m_otherBar->Render();
	m_otherInvenBar->Render();
	m_otherLine->Render();
	m_otherSENumBar->Render();
	m_myBar->Render();
	m_myInvenBar->Render();
	m_myLine->Render();
	m_mySENumBar->Render();
	
	m_okBtn->Render();
	m_cancelBtn->Render();
	m_admitBtn->Render();
	m_closeBtn->Render();
	m_seInputBtn->Render();
	
	
	g_ifMng->m_iconCtl->BeginFrame();
	{
		int i, itemIdx, cx, cy, itemTableIdx;
		
		for( i = 0; i < 12; i ++ )
		{
			itemIdx = g_cgv.myCharacterInfo->otherTrade[i];
			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->GetXPos() + ( i % 6 ) * 34;
				cy = m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->GetYPos() + ( i / 6 ) * 33;
				itemTableIdx = g_cgv.myCharacterInfo->tradeItem[itemIdx].itemTableIdx;
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}
				
		
		for( i = 0; i < 12; i ++ )
		{
			itemIdx = g_cgv.myCharacterInfo->myTrade[i];
			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->GetXPos() + ( i % 6 ) * 34;
				cy = m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->GetYPos() + ( i / 6 ) * 33;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}
	}
	g_ifMng->m_iconCtl->EndFrame();
	
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_TRADE_WIN_TITLE]->Render();
		m_textCtl[IT_TRADE_WIN_OTHER_NAME_U]->Render();
		m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->Render();
		m_textCtl[IT_TRADE_WIN_OTHER_SE]->Render();
		m_textCtl[IT_TRADE_WIN_OTHER_SE_U]->Render( IF_TEXT_OUTPUT_RIGHT );
		m_textCtl[IT_TRADE_WIN_OTHER_OK]->Render();
		m_textCtl[IT_TRADE_WIN_OTHER_ADMIT]->Render();
		m_textCtl[IT_TRADE_WIN_MY_NAME_U]->Render();
		m_textCtl[IT_TRADE_WIN_MY_SE]->Render();
		m_textCtl[IT_TRADE_WIN_MY_SE_U]->Render( IF_TEXT_OUTPUT_RIGHT );
		m_textCtl[IT_TRADE_OK]->Render();
		m_textCtl[IT_TRADE_CANCEL]->Render();
		m_textCtl[IT_TRADE_ADMIT]->Render();
		m_textCtl[IT_TRADE_SEINPUT]->Render();

		
		g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
		g_chatManager->SetReverseSize( -1 );
		g_chatManager->SetColor( 0xffffffff );
		for( int i = 0; i < 12; i ++ )
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->otherTrade[i];
			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->tradeItem[itemIdx] ) )
				{
					cx = m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->GetXPos() + ( i % 6 ) * 34;
					cy = m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->GetYPos() + ( i / 6 ) * 33;
					number = g_cgv.myCharacterInfo->tradeItem[itemIdx].durability + 1;
					sprintf( szTemp, "%d", number );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
		
		for( i = 0; i < 12; i ++ )
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->myTrade[i];

			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->item[itemIdx] ) )
				{
					cx = m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->GetXPos() + ( i % 6 ) * 34;
					cy = m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->GetYPos() + ( i / 6 ) * 33;
					number = g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
					sprintf( szTemp, "%d", number );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
	}
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->EndFrame();
}

int CIFTradeWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );
	
	return( IF_CTL_NONE );
}

int CIFTradeWin::WindowProcess()
{
	
	if( m_readyFlag )
	{
		m_okBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		m_cancelBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	}
	else
	{
		m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_cancelBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}

	if( !m_readyFlag && !m_otherReadyFlag )
	{
		m_seInputBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	}
	else
	{
		m_seInputBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}

	m_admitBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	if( m_readyFlag && m_otherReadyFlag )
	{
		if( !m_confirmFlag ) m_admitBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	}

	if( m_otherReadyFlag )
	{
		m_textCtl[IT_TRADE_WIN_OTHER_OK]->SetColor( 0xffffff00 );
	}
	else
	{
		m_textCtl[IT_TRADE_WIN_OTHER_OK]->SetColor( 0xff000000 );
	}

	m_prepareReadyTime -= g_timer.GetElapsedMilliTime();

	if( m_prepareReadyTime > 0.0f )
	{
		m_okBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		m_cancelBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		m_seInputBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		m_admitBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}

	
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		GTH_SendMessage_PCTrade_Exit();
		return IF_CTL_SELECT;
	}
	
	if( m_okBtn->ProcessButton() == IF_CTL_SELECT )
	{
		GTH_SendMessage_PCTrade_Ok( m_otherIdxOnServer );
	}
	
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		GTH_SendMessage_PCTrade_Cancel( m_otherIdxOnServer );
	}
	
	if( m_admitBtn->ProcessButton() == IF_CTL_SELECT )
	{
		GTH_SendMessage_PCTrade_Confirm( m_otherIdxOnServer, true );
		m_confirmFlag = true;
	}
	
	if( m_seInputBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_ifMng->m_stackWin->InitStack( g_LPACK.GetMassage( 0, 553 ), NULL, IF_STACK_TYPE_TRADE_SE, m_myTradeSE );
	}

	
	int x, y, pos, inOtherPos = true;
	int mx, my, temp;
	
	mx = g_input->GetPosX() - m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->GetYPos();

	if( mx <= 0 || my <= 0 ) 
	{
		inOtherPos = false;
	}
	else if( mx >= 34 * 6 ||  my >= 33 * 2 )
	{
		inOtherPos = false;
	}
	else
	{
		temp = mx / 34;
		x = temp % 6;
		temp = my / 33;
		y = temp % 2;
		
		pos = y * 6 + x;
		
		
		if( pos < 0 || pos >= 12 )
		{
			inOtherPos = false;
		}
	}

	if( inOtherPos )
	{
		int itemIdx = g_cgv.myCharacterInfo->otherTrade[pos];

		
		if( g_input->GetLBNone() ) 
		{
			
			if( itemIdx >= 0 )
			{
				g_ifMng->InitRolloverText();
				g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->tradeItem[itemIdx] );
				g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
			}
		}
		return( IF_CTL_NONE );
	}
	
	mx = g_input->GetPosX() - m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->GetYPos();

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
	
	int itemIdx = g_cgv.myCharacterInfo->myTrade[pos];
	
	
	if( g_input->GetLBNone() ) 
	{
		
		if( itemIdx >= 0 )
		{
			g_ifMng->InitRolloverText();
			g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[itemIdx] );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		}
	}
	
	if( g_input->GetLBUp() && !m_readyFlag && !m_otherReadyFlag ) 
	{
		GetTradeItem( pos );
	}

	return IF_CTL_NONE;
}

void CIFTradeWin::SetTradeInfo( int otherIdxOnServer, char* name )
{
	m_otherIdxOnServer = otherIdxOnServer;
	strcpy( m_otherName, name );
}

void CIFTradeWin::InitTrade()
{
	char szTemp[256];

	
	sprintf( szTemp, g_LPACK.GetMassage(0,213), m_otherName );
	
	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 125, 100) );

	
	g_ifMng->m_tradeWin->Enable();
	g_ifMng->m_itemWin->Enable();
	g_ifMng->SetFocus( g_ifMng->m_tradeWin );

	
	for( int i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		memset( &g_cgv.myCharacterInfo->tradeItem[i], 0, sizeof( item_t ) );
		g_cgv.myCharacterInfo->tradeItem[i].itemTableIdx = -1;
		g_cgv.myCharacterInfo->otherTrade[i] = -1;
		g_cgv.myCharacterInfo->myTrade[i] = -1;
	}
	m_myTradeSE = 0;
	m_otherTradeSE = 0;

	m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

	m_readyFlag = false;
	m_otherReadyFlag = false;
	m_confirmFlag = false;
	m_otherConfirmFlag = false;

	m_textCtl[IT_TRADE_WIN_OTHER_ADMIT]->SetColor( 0xff000000 );
	m_textCtl[IT_TRADE_WIN_OTHER_OK]->SetColor( 0xff000000 );

	m_prepareReadyTime = 0.0f;
}

void CIFTradeWin::CancelTrade()
{
	item_t *item;
	int itemIdx, inventoryPos;

	for( int i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		itemIdx = g_cgv.myCharacterInfo->myTrade[i];
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
	for( i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		memset( &g_cgv.myCharacterInfo->tradeItem[i], 0, sizeof( item_t ) );
		g_cgv.myCharacterInfo->tradeItem[i].itemTableIdx = -1;
		g_cgv.myCharacterInfo->otherTrade[i] = -1;
		g_cgv.myCharacterInfo->myTrade[i] = -1;
	}
	Enable( false );
}

void CIFTradeWin::ConfirmTrade()
{
	int i;
	int itemIdx, inventoryPos;
	item_t *item;
	char szTemp[256];

	
	g_cgv.myCharacterInfo->curChargeSE -= m_myTradeSE;
	g_cgv.myCharacterInfo->curChargeSE += m_otherTradeSE;

	
	for( i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		if( g_cgv.myCharacterInfo->myTrade[i] < 0 ) continue;
		itemIdx = g_cgv.myCharacterInfo->myTrade[i];
		item = &g_cgv.myCharacterInfo->item[itemIdx];
		GTH_DeleteMyItem( item );
	}
	
	for( i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		if( g_cgv.myCharacterInfo->otherTrade[i] < 0 ) continue;
		itemIdx = g_cgv.myCharacterInfo->otherTrade[i];
		item = &g_cgv.myCharacterInfo->tradeItem[itemIdx];

		int stackItemIdx = GTH_StackMyItem( item );

		if( stackItemIdx >= 0 )
		{
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += item->durability + 1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
		}
		else
		{
			inventoryPos = GTH_FindEmptyInventoryPos();
			if( inventoryPos >= 0 ) 
			{
				itemIdx = GTH_AddMyItem( item );
				g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;
			}
		}
	}	
	
	sprintf( szTemp, g_LPACK.GetMassage(0,215), m_otherName );
	
	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 125, 100) );
	
	Enable( false );
}

int CIFTradeWin::GetTradeItem( int pos )
{
	int itemIdx;

	

	itemIdx = g_cgv.myCharacterInfo->myTrade[pos];
	
	

	
	if( itemIdx < 0 && g_cgv.myCharacterInfo->mouseInventory < 0 ) 
	{
		return( 0 );
	}
	else
	{
		GTH_SendMessage_PCTrade_Item( m_otherIdxOnServer, pos );
	}
	return( 1 );
}

void CIFTradeWin::SetReady( int type, int flag )
{
	if( type )
	{
		if( flag ) m_readyFlag = true;
		else m_readyFlag = false;
	}
	else
	{
		if( flag ) m_otherReadyFlag = true;
		else m_otherReadyFlag = false;
	}

	
	if( !flag )
	{
		m_confirmFlag = false;
		m_otherConfirmFlag = false;
		m_textCtl[IT_TRADE_WIN_OTHER_ADMIT]->SetColor( 0xff000000 );

		m_prepareReadyTime = 3000.0f;
	}
}

void CIFTradeWin::SetConfirm( int flag )
{
	m_otherConfirmFlag = true;
	m_textCtl[IT_TRADE_WIN_OTHER_ADMIT]->SetColor( 0xffffff00 );
}













CIFBoothWin::CIFBoothWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i = 0; i < NUM_IT_BOOTH_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );
	m_animCombo = new CIFComboBox( this );

	m_editBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_BOOTH_WINDOW_TYPE );
	
	memset( m_buyerName, 0, sizeof( m_buyerName ) );
	m_sellerName[0] = 0;
	m_boothName[0] = 0;
	m_boothInfo[0] = 0;
}

CIFBoothWin::~CIFBoothWin()
{

}

void CIFBoothWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_BOOTH_WIN );
	
	m_textCtl[IT_BOOTH_WIN_TITLE]->PrepareControl( IF_BOOTH_WIN_TITLE_TEXT );
	m_textCtl[IT_BOOTH_WIN_NAME_BAR]->PrepareControl( IF_BOOTH_WIN_NAME_BAR );
	m_textCtl[IT_BOOTH_WIN_NAME]->PrepareControl( IF_BOOTH_WIN_NAME_TEXT );
	m_textCtl[IT_BOOTH_WIN_INFO_BAR_0]->PrepareControl( IF_BOOTH_WIN_INFO_BAR_0 );
	m_textCtl[IT_BOOTH_WIN_INFO_0]->PrepareControl( IF_BOOTH_WIN_INFO_TEXT_0 );
	m_textCtl[IT_BOOTH_WIN_ANIM]->PrepareControl( IF_BOOTH_WIN_ANIM_TEXT );
	m_textCtl[IT_BOOTH_WIN_INVEN_POS]->PrepareControl( IF_BOOTH_WIN_INVEN_POS );
	m_textCtl[IT_BOOTH_WIN_NAK_BAR]->PrepareControl( IF_BOOTH_WIN_NAK_BAR );
	m_textCtl[IT_BOOTH_WIN_NAK]->PrepareControl( IF_BOOTH_WIN_NAK_TEXT );
	m_textCtl[IT_BOOTH_WIN_EDIT]->PrepareControl( IF_BOOTH_WIN_EDIT_TEXT );
	m_textCtl[IT_BOOTH_WIN_CANCEL]->PrepareControl( IF_BOOTH_WIN_CANCEL_TEXT );

	m_titleBar->PrepareControl( IF_BOOTH_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_BOOTH_WIN_TITLE_BOTTOM );
	m_animCombo->PrepareControl( IF_BOOTH_WIN_ANIM_COMBOBOX );

	m_editBtn->PrepareControl( IF_BOOTH_WIN_EDIT_BTN_0 );
	m_cancelBtn->PrepareControl( IF_BOOTH_WIN_CANCEL_BTN_0 );
}

int CIFBoothWin::WindowFrame()
{
	char szTemp[256];

	int itemIdx, itemTableIndex;

	
	m_totalSellNak = 0;
	for( int i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
	{
		if( !m_boothType )
		{
			if( m_boothBuyItem[i].item.itemTableIdx < 0 ) continue;
			itemTableIndex = m_boothBuyItem[i].item.itemTableIdx;
			if( g_itemTable[itemTableIndex].stackFlag )
			{
				m_totalSellNak += m_boothBuyItem[i].price * ( m_boothBuyItem[i].item.durability + 1 );
			}
			else
			{
				m_totalSellNak += m_boothBuyItem[i].price;
			}
		}
		else
		{
			if( g_cgv.myCharacterInfo->boothInventory[i].itemIdx < 0 ) continue;

			itemIdx = g_cgv.myCharacterInfo->boothInventory[i].itemIdx;
			itemTableIndex = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
			if( g_itemTable[itemTableIndex].stackFlag )
			{
				m_totalSellNak += g_cgv.myCharacterInfo->boothInventory[i].price * ( g_cgv.myCharacterInfo->item[itemIdx].durability + 1 );
			}
			else
			{
				m_totalSellNak += g_cgv.myCharacterInfo->boothInventory[i].price;
			}
		}
	}

	
	if( !m_boothType ) 
	{
		m_textCtl[IT_BOOTH_WIN_TITLE]->SetText( g_LPACK.GetMassage( 0, 620 ) );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 621 ), m_sellerName );
		
		m_textCtl[IT_BOOTH_WIN_NAME]->SetText( szTemp );
	}
	else
	{
		if( !m_boothState ) m_textCtl[IT_BOOTH_WIN_TITLE]->SetText( g_LPACK.GetMassage( 0, 618 ) );
		else m_textCtl[IT_BOOTH_WIN_TITLE]->SetText( g_LPACK.GetMassage( 0, 619 ) );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 626 ), m_buyerNumber );
		
		m_textCtl[IT_BOOTH_WIN_NAME]->SetText( szTemp );
	}
	m_textCtl[IT_BOOTH_WIN_TITLE]->SetColor( 0xffffffff );

	m_textCtl[IT_BOOTH_WIN_NAME_BAR]->SetText( m_boothName );
	m_textCtl[IT_BOOTH_WIN_INFO_BAR_0]->SetText( m_boothInfo );

	
	m_textCtl[IT_BOOTH_WIN_INFO_0]->SetText( g_LPACK.GetMassage( 0, 622 ) );
	
	
	
	m_textCtl[IT_BOOTH_WIN_ANIM]->SetText( g_LPACK.GetMassage( 0, 623 ) );

	
	strcpy( szTemp, g_ifMng->GetNumberString( m_totalSellNak ) );
	m_textCtl[IT_BOOTH_WIN_NAK]->SetText( g_LPACK.GetMassage( 0, 624 ) );
	m_textCtl[IT_BOOTH_WIN_NAK_BAR]->SetText( szTemp );
	
	
	m_textCtl[IT_BOOTH_WIN_EDIT]->SetText( g_LPACK.GetMassage( 0, 625 ) );
	
	m_textCtl[IT_BOOTH_WIN_CANCEL]->SetText( g_LPACK.GetMassage( 0, 521 ) );

	int cx, cy;
	cx = m_textCtl[IT_BOOTH_WIN_NAME_BAR]->GetXPos();
	cy = m_textCtl[IT_BOOTH_WIN_NAME_BAR]->GetYPos();
	g_chatManager->m_ix[1] = cx;
	g_chatManager->m_iy[1] = cy;
	
	cx = m_textCtl[IT_BOOTH_WIN_INFO_BAR_0]->GetXPos();
	cy = m_textCtl[IT_BOOTH_WIN_INFO_BAR_0]->GetYPos();
	g_chatManager->m_ix[2] = cx;
	g_chatManager->m_iy[2] = cy;


	return IF_CTL_NONE;
}

void CIFBoothWin::Render()
{
	if( !State() ) return;

	WindowFrame();

	g_ifMng->SetRenderState();

	CIFControl::Render();

	m_titleBar->Render();
	m_titleBottom->Render();
	m_cancelBtn->Render();

	
	if( !m_boothType )
	{
		g_ifMng->m_iconCtl->BeginFrame();
		{
			
			for( int i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
			{
				int itemTableIdx, cx, cy;
				itemTableIdx = m_boothBuyItem[i].item.itemTableIdx;
				if( itemTableIdx >= 0 )
				{
					cx = m_textCtl[IT_BOOTH_WIN_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
					cy = m_textCtl[IT_BOOTH_WIN_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
					g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
				}
			}
		}
		g_ifMng->m_iconCtl->EndFrame();
		
		
		g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
		g_chatManager->BeginFrame();
		{
			m_textCtl[IT_BOOTH_WIN_TITLE]->Render();
			
			m_textCtl[IT_BOOTH_WIN_NAME_BAR]->Render();
			m_textCtl[IT_BOOTH_WIN_INFO_BAR_0]->Render( IF_TEXT_OUTPUT_RECT );
			
			m_textCtl[IT_BOOTH_WIN_NAME]->Render();
			m_textCtl[IT_BOOTH_WIN_INFO_0]->Render();
			m_textCtl[IT_BOOTH_WIN_NAK_BAR]->Render( IF_TEXT_OUTPUT_RIGHT );
			m_textCtl[IT_BOOTH_WIN_NAK]->Render();
			
			m_textCtl[IT_BOOTH_WIN_CANCEL]->Render();

			
			g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
			g_chatManager->SetReverseSize( -1 );
			g_chatManager->SetColor( 0xffffffff );
			
			for( int i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
			{
				int itemTableIdx, number, cx, cy;
				char szTemp[128];
				itemTableIdx = m_boothBuyItem[i].item.itemTableIdx;
				if( itemTableIdx >= 0 )
				{
					if( GTH_IsStackItem( &m_boothBuyItem[i].item ) )
					{
						cx = m_textCtl[IT_BOOTH_WIN_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
						cy = m_textCtl[IT_BOOTH_WIN_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
						number = m_boothBuyItem[i].item.durability + 1;
						strcpy( szTemp, g_ifMng->GetNumberString( number ) );
						g_chatManager->TextOut( cx, cy + 19, szTemp );
					}
				}
			}
		}
		g_chatManager->EndFrame();
	}
	
	else
	{
		m_animCombo->Render();
		m_editBtn->Render();

		g_ifMng->m_iconCtl->BeginFrame();
		{
			
			for( int i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
			{
				int itemIdx, itemTableIdx, cx, cy;
				itemIdx = g_cgv.myCharacterInfo->boothInventory[i].itemIdx;
				if( itemIdx >= 0 )
				{
					cx = m_textCtl[IT_BOOTH_WIN_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
					cy = m_textCtl[IT_BOOTH_WIN_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
					itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
					g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
				}
			}
		}
		g_ifMng->m_iconCtl->EndFrame();
		
		
		g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
		g_chatManager->BeginFrame();
		{
			m_textCtl[IT_BOOTH_WIN_TITLE]->Render();
			
			if( m_boothState )
			{
				m_textCtl[IT_BOOTH_WIN_NAME_BAR]->Render();
				m_textCtl[IT_BOOTH_WIN_INFO_BAR_0]->Render( IF_TEXT_OUTPUT_RECT );
			}

			m_textCtl[IT_BOOTH_WIN_NAME]->Render();
			m_textCtl[IT_BOOTH_WIN_INFO_0]->Render();
			m_textCtl[IT_BOOTH_WIN_ANIM]->Render();
			m_textCtl[IT_BOOTH_WIN_NAK_BAR]->Render( IF_TEXT_OUTPUT_RIGHT );
			m_textCtl[IT_BOOTH_WIN_NAK]->Render();
			m_textCtl[IT_BOOTH_WIN_EDIT]->Render();
			m_textCtl[IT_BOOTH_WIN_CANCEL]->Render();

			
			g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
			g_chatManager->SetReverseSize( -1 );
			g_chatManager->SetColor( 0xffffffff );
			
			for( int i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
			{
				int itemIdx, number, cx, cy;
				char szTemp[128];
				itemIdx = g_cgv.myCharacterInfo->boothInventory[i].itemIdx;
				
				if( itemIdx >= 0 )
				{
					if( GTH_IsStackItem( &g_cgv.myCharacterInfo->item[itemIdx] ) )
					{
						cx = m_textCtl[IT_BOOTH_WIN_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
						cy = m_textCtl[IT_BOOTH_WIN_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;
						number = g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
						strcpy( szTemp, g_ifMng->GetNumberString( number ) );
						g_chatManager->TextOut( cx, cy + 19, szTemp );
					}
				}
			}
			g_chatManager->RenderInputFrame( 1 );
			g_chatManager->RenderInputFrame( 2 );
		}
		g_chatManager->EndFrame();
	}
}

int CIFBoothWin::WindowSelection()
{
	int ret;
	
	if( !State() ) return( IF_CTL_NONE );

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFBoothWin::WindowProcess()
{
	char szTemp[256], szTemp2[256];
	int itemIdx, mouseItemIdx;
	int x, y, pos;
	int mx, my, temp;

	
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		if( !m_boothType )
		{
			GTH_SendMessage_ItemSystem_BoothBuyClose();
		}
		else
		{
			
			if( !m_boothState )
			{
				GTH_SendMessage_ItemSystem_BoothSellClose();
			}
			
			else
			{
				GTH_SendMessage_ItemSystem_BoothSellClose();
			}
		}
	}
	
	if( m_editBtn->ProcessButton() == IF_CTL_SELECT && m_boothType )
	{
		if( !m_boothState )
		{
			strcpy( m_boothName, g_chatManager->GetInputStr( 1 ) );
			strcpy( m_boothInfo, g_chatManager->GetInputStr( 2 ) );
			
			if( m_boothName[0] == 0 )
			{
				g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( 0, 627 ) );
				return( IF_CTL_NONE );
			}
			if( m_boothInfo[0] == 0 )
			{
				g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( 0, 628 ) );
				return( IF_CTL_NONE );
			}
			GTH_SendMessage_ItemSystem_BoothSellStart( m_boothName, m_boothInfo );
		}
		
		else
		{
			GTH_SendMessage_ItemSystem_BoothSellClose();
		}
	}
	
	if( m_boothType && !m_boothState)
	{
		if( m_textCtl[IT_BOOTH_WIN_NAME_BAR]->Selection() == IF_LBDN )
		{
			g_chatManager->ChangeInputMessage( INPUT_INDEX_BOOTH_NAME );
		}
		if( m_textCtl[IT_BOOTH_WIN_INFO_BAR_0]->Selection() == IF_LBDN )
		{
			g_chatManager->ChangeInputMessage( INPUT_INDEX_BOOTH_INFO );
		}
	}
	if( m_textCtl[IT_BOOTH_WIN_NAME]->Selection() == IF_IN_POS && m_boothType && m_boothState )
	{
		g_ifMng->InitRolloverText();
		for( int i = 0; i < 50; i ++ )
		{
			if( m_buyerName[i][0] == 0 ) continue;
			g_ifMng->AddRolloverText( m_buyerName[i] );
		}
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), false );
	}

	
	mx = g_input->GetPosX() - m_textCtl[IT_BOOTH_WIN_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_BOOTH_WIN_INVEN_POS]->GetYPos();

	if( mx <= 0 || my <= 0 ) return( IF_CTL_NONE );
	if( mx >= 34 * 6 ||  my >= 33 * 3 ) return( IF_CTL_NONE );
	temp = mx / 34;
	x = temp % 6;
	temp = my / 33;
	y = temp % 3;

	pos = y * 6 + x;

	
	if( pos < 0 || pos >= 18 )
	{
		return( IF_CTL_NONE );
	}
	
	
	if( !m_boothType )
	{
		mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;
		itemIdx = m_boothBuyItem[pos].item.itemTableIdx;
		
		if( g_input->GetLBNone() ) 
		{
			
			if( itemIdx < 0 ) return( IF_CTL_NONE );
			
			g_ifMng->InitRolloverText();
			g_ifMng->MakeItemRollover( &m_boothBuyItem[pos].item );
			strcpy( szTemp2, g_ifMng->GetNumberString( m_boothBuyItem[pos].price ) );
			sprintf( szTemp, g_LPACK.GetMassage( 0, 318 ), szTemp2 );
			g_ifMng->AddRolloverText( szTemp, 0 );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		}
		
		if( g_input->GetLBUp() ) 
		{
			if( mouseItemIdx >= 0 || itemIdx < 0 ) return IF_CTL_NONE;
			if( GTH_IsStackItem( &m_boothBuyItem[pos].item ) )
			{
				
				g_ifMng->m_stackWin->InitStack( g_LPACK.GetMassage( 0, 545 ), 
					&m_boothBuyItem[pos].item, IF_STACK_TYPE_BUY_BOOTH_ITEM, pos, (float)m_boothBuyItem[pos].price );
			}
			else
			{
				
				m_saveItemIdx = pos;
				g_ifMng->m_stackWin->m_count = 1;
				strcpy( szTemp2, g_ifMng->GetNumberString( m_boothBuyItem[pos].price ) );
				sprintf( szTemp, g_LPACK.GetMassage( 0, 546 ), m_boothBuyItem[pos].item.name, szTemp2 );
				g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_BUY_BOOTH_ITEM );
			}
		}
	}
	
	else
	{
		itemIdx = g_cgv.myCharacterInfo->boothInventory[pos].itemIdx;
		mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;

		
		if( g_input->GetLBNone() ) 
		{
			
			if( itemIdx < 0 ) return( IF_CTL_NONE );
			
			g_ifMng->InitRolloverText();
			g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[itemIdx] );
			strcpy( szTemp2, g_ifMng->GetNumberString( g_cgv.myCharacterInfo->boothInventory[pos].price ) );
			sprintf( szTemp, g_LPACK.GetMassage( 0, 318 ), szTemp2 );
			g_ifMng->AddRolloverText( szTemp, 0 );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		}

		
		if( m_boothState ) return( IF_CTL_NONE );

		
		if( g_input->GetLBUp() ) 
		{
			if( mouseItemIdx < 0 && itemIdx < 0 ) return IF_CTL_NONE;
			
			
			if( mouseItemIdx >= 0 )
			{
				g_ifMng->m_stackWin->InitStack( g_LPACK.GetMassage( 0, 611 ), 
					&g_cgv.myCharacterInfo->item[mouseItemIdx], IF_STACK_TYPE_SELL_BOOTH_PRICE, pos );
			}
			else
			{
				GTH_SendMessage_ItemSystem_BoothSellItem( pos, 0 );
			}
		}
	}
	return IF_CTL_NONE;
}

void CIFBoothWin::InitBooth( int type )
{
	int i;

	if( !type )
	{
		
		m_boothType = 0;
		for( i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
		{
			m_boothBuyItem[i].item.itemTableIdx = -1;
		}
	}
	else
	{
		
		m_boothType = 1;
		m_boothState = 0;
		for( i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
		{
			g_cgv.myCharacterInfo->boothInventory[i].itemIdx = -1;
		}
		for( i = 0; i < 50; i ++ )
		{
			memset( m_buyerName[i], 0, sizeof( m_buyerName[i] ) );
		}
		m_buyerNumber = 0;
		
		int cx, cy, cs;
		cx = m_textCtl[IT_BOOTH_WIN_NAME_BAR]->GetXPos();
		cy = m_textCtl[IT_BOOTH_WIN_NAME_BAR]->GetYPos();
		cs = m_textCtl[IT_BOOTH_WIN_NAME_BAR]->GetXSize();
		g_chatManager->SetInput( cx, cy, 24, 24, INPUT_INDEX_BOOTH_NAME, 1, cs );

		cx = m_textCtl[IT_BOOTH_WIN_INFO_BAR_0]->GetXPos();
		cy = m_textCtl[IT_BOOTH_WIN_INFO_BAR_0]->GetYPos();
		cs = m_textCtl[IT_BOOTH_WIN_INFO_BAR_0]->GetXSize();
		g_chatManager->SetInput( cx, cy, 96, 96, INPUT_INDEX_BOOTH_INFO, 1, cs, true );

		strcpy( m_boothName, "" );
		strcpy( m_boothInfo, "" );
	}
}

void CIFBoothWin::SetBoothState( int type )
{
	
	
	m_boothState = type;
	if( type )
	{
		m_editBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		g_chatManager->SetInputState( INPUT_INDEX_BOOTH_NAME, -1 );
		g_chatManager->SetInputState( INPUT_INDEX_BOOTH_INFO, -1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
	}
	else
	{
		m_editBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	
		g_chatManager->SetInputState( INPUT_INDEX_BOOTH_NAME, 1 );
		g_chatManager->SetInputState( INPUT_INDEX_BOOTH_INFO, 1 );
	}
	for( int i = 0; i < 50; i ++ )
	{
		memset( m_buyerName[i], 0, sizeof( m_buyerName[i] ) );
	}
	m_buyerNumber = 0;
}


void CIFBoothWin::CloseBooth()
{
	item_t *item;
	int itemIdx, inventoryPos;

	for( int i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
	{
		itemIdx = g_cgv.myCharacterInfo->boothInventory[i].itemIdx;
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
	g_chatManager->SetInputState( INPUT_INDEX_BOOTH_NAME, -1 );
	g_chatManager->SetInputState( INPUT_INDEX_BOOTH_INFO, -1 );
	g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
	Enable( false );
}

void CIFBoothWin::TradeItem( int type, int pos, int itemCount )
{
	item_t *item, buyItem;
	int itemIdx;

	
	if( !type )
	{
		if( m_boothBuyItem[pos].item.itemTableIdx < 0 ) return;
		item = &m_boothBuyItem[pos].item;
		
		memcpy( &buyItem, item, sizeof( item_t ) );
		
		
		if( g_itemTable[item->itemTableIdx].stackFlag )
		{
			if( item->durability + 1 - itemCount <= 0 )
			{
				item->itemTableIdx = -1;
			}
			else
			{
				item->durability -= itemCount;
			}

			buyItem.durability = itemCount - 1;
		}
		else
		{
			item->itemTableIdx = -1;
		}
		
		int stackItemIdx = GTH_StackMyItem( &buyItem );
		if( stackItemIdx >= 0 )
		{
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += buyItem.durability + 1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
		}
		else
		{
			itemIdx = GTH_AddMyItem( &buyItem );
			int inventoryPos = GTH_FindEmptyInventoryPos();
			if( inventoryPos >= 0 ) 
			{
				g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;
			}
		}
	}
	
	else
	{
		itemIdx = g_cgv.myCharacterInfo->boothInventory[pos].itemIdx;
		if( itemIdx < 0 ) return;
		item = &g_cgv.myCharacterInfo->item[itemIdx];

		
		if( g_itemTable[item->itemTableIdx].stackFlag )
		{
			if( item->durability + 1 - itemCount <= 0 )
			{
				g_cgv.myCharacterInfo->boothInventory[pos].itemIdx = -1;
				GTH_DeleteMyItem( item );
			}
			else
			{
				item->durability -= itemCount;
			}
		}
		else
		{
			g_cgv.myCharacterInfo->boothInventory[pos].itemIdx = -1;
			GTH_DeleteMyItem( item );
		}
	}
}

void CIFBoothWin::ExitBooth( int type )
{
	
	if( !type ) 
	{
		Enable( false );
		if( g_ifMng->m_msgType == IF_MESSAGE_BUY_BOOTH_ITEM ) g_ifMng->ExitMessage();
	}
	else
	{

	}
}

void CIFBoothWin::AddBuyer( char *name )
{
	for( int i = 0; i < 50; i ++ )
	{
		if( m_buyerName[i][0] == 0 ) break;
	}
	if( i == 50 ) return;
	strcpy( m_buyerName[i], name );
	m_buyerNumber ++;
}

void CIFBoothWin::RemoveBuyer( char *name )
{
	for( int i = 0; i < 50; i ++ )
	{
		if( !stricmp( m_buyerName[i], name ) ) break;
	}
	if( i == 50 ) return;
	memset( m_buyerName[i], 0, sizeof( m_buyerName[i] ) );
	m_buyerNumber --;
}






CIFCommunityWin::CIFCommunityWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i=0; i<NUM_IT_COMMUNITY_WIN; i++ )
		m_textCtl[i] = new CIFText( this );

	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	
	
	m_mailBtn = new CIFButtonCtl( this );
	m_friendBtn = new CIFButtonCtl( this );
	m_closeBtn = new CIFButtonCtl( this );
	m_exitBtn = new CIFButtonCtl( this );
	m_helperBtn = new CIFButtonCtl(this);

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_COMMUNITY_WINDOW_TYPE );

}

CIFCommunityWin::~CIFCommunityWin()
{
}

void CIFCommunityWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_COMMUNITY_WIN );
	
	m_textCtl[IT_COMMUNITY_WIN_TITLE]->PrepareControl( IF_COMMUNITY_WIN_TITLE_TEXT );
	m_textCtl[IT_COMMUNITY_MAIL]->PrepareControl( IF_COMMUNITY_WIN_MAIL_TEXT );
	m_textCtl[IT_COMMUNITY_FRIEND]->PrepareControl( IF_COMMUNITY_WIN_FRIEND_TEXT );
	m_textCtl[IT_COMMUNITY_CLOSE]->PrepareControl( IF_COMMUNITY_WIN_CLOSE_TEXT );

	m_textCtl[IT_COMMUNITY_HELPER]->PrepareControl( IF_COMMUNITY_WIN_HELPER_TEXT );


	
	m_titleBar->PrepareControl( IF_COMMUNITY_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_COMMUNITY_WIN_TITLE_BAR_BOTTOM );
	
	
	m_exitBtn->PrepareControl( IF_COMMUNITY_WIN_EXIT_BTN_0 );
	m_mailBtn->PrepareControl( IF_COMMUNITY_WIN_MAIL_BTN_0 );
	m_friendBtn->PrepareControl( IF_COMMUNITY_WIN_FRIEND_BTN_0);
	m_closeBtn->PrepareControl( IF_COMMUNITY_WIN_CLOSE_BTN_0 );
	m_helperBtn->PrepareControl(IF_COMMUNITY_WIN_HELPER_BTN_0);
}

int CIFCommunityWin::WindowFrame()
{
	m_textCtl[IT_COMMUNITY_WIN_TITLE]->SetText( g_LPACK.GetMessage(0,975) );
	m_textCtl[IT_COMMUNITY_MAIL]->SetText( g_LPACK.GetMessage(0,976) );
	m_textCtl[IT_COMMUNITY_FRIEND]->SetText( g_LPACK.GetMessage(0,977) );
	m_textCtl[IT_COMMUNITY_CLOSE]->SetText( g_LPACK.GetMessage(0,936) );


	m_textCtl[IT_COMMUNITY_HELPER]->SetText(g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,300));

	m_textCtl[IT_COMMUNITY_WIN_TITLE]->SetColor( 0xffffffff );
	m_textCtl[IT_COMMUNITY_MAIL]->SetColor( 0xffffffff );
	m_textCtl[IT_COMMUNITY_FRIEND]->SetColor( 0xffffffff );
	m_textCtl[IT_COMMUNITY_CLOSE]->SetColor( 0xffffffff );

	m_textCtl[IT_COMMUNITY_HELPER]->SetColor(0xffffffff);

	return( IF_CTL_NONE );
}

void CIFCommunityWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();
	
	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	
	
	m_mailBtn->Render();
	m_friendBtn->Render();
	m_closeBtn->Render();
	m_exitBtn->Render();
	m_helperBtn->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_COMMUNITY_WIN_TITLE]->Render();
		m_textCtl[IT_COMMUNITY_MAIL]->Render();
		m_textCtl[IT_COMMUNITY_FRIEND]->Render();
		m_textCtl[IT_COMMUNITY_CLOSE]->Render();
		m_textCtl[IT_COMMUNITY_HELPER]->Render();
	}
	g_chatManager->EndFrame();
}

int CIFCommunityWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFCommunityWin::WindowProcess()
{
	if( m_mailBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );

		g_ifMng->m_menuWin->m_communityBtn->SetFlickerMode( false );
		g_ifMng->m_communityWin->m_mailBtn->SetFlickerMode( false );

		g_ifMng->m_mailWin->SetDisplayMode( false );
		g_ifMng->m_mailWin->DeleteMailList();
		GTH_SendMessage_MailSystem_Receive();
		g_ifMng->m_mailWin->SetInterfaceType( MAIL_INTERFACE_TYPE_RECVMAIL );
		g_ifMng->m_mailWin->Enable( true );
		g_ifMng->SetFocus( g_ifMng->m_mailWin );

		return IF_CTL_SELECT;
	}

	if( m_friendBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		g_ifMng->m_friendWin->Enable( true );
		g_ifMng->SetFocus( g_ifMng->m_friendWin );

		return IF_CTL_SELECT;
	}

	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_SELECT;
	}

	if( m_exitBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_SELECT;
	}


	if(m_helperBtn->ProcessButton() == IF_CTL_SELECT)
	{
		Enable(false);
		g_HelperSystem.GTH_SendMessage_HelpSystem_GiveMeListAll();
		g_ifMng->m_HelperWin->Enable(true);
		g_ifMng->SetFocus(g_ifMng->m_HelperWin);
		return IF_CTL_SELECT;
	}


	return IF_CTL_NONE;
}





CIFMailWin::CIFMailWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	int i;

	for( i=0; i<NUM_IT_MAIL_WIN; i++ )
		m_textCtl[i]	= new CIFText( this );

	for( i=0; i<MAIL_WIN_LIST_MAX_NUM; i++ )
		m_checkBtn[i]	= new CIFCheckBox( this );

	
	m_titleBar			= new CIFControl( this );
	m_titleBarBottom	= new CIFControl( this );

	m_recvMailInfo		= new CIFControl( this );
	m_storeMailInfo		= new CIFControl( this );

	m_selectBar			= new CIFControl( this );

	
	m_recvMailBtn		= new CIFButtonCtl( this );
	m_storeMailBtn		= new CIFButtonCtl( this );
	m_writeMailBtn		= new CIFButtonCtl( this );
	m_saveMailBtn		= new CIFButtonCtl( this );
	m_deleteMailBtn		= new CIFButtonCtl( this );
	m_cancelBtn			= new CIFButtonCtl( this );
	m_exitBtn			= new CIFButtonCtl( this );

	
	m_scrollBar			= new CIFScrollBar( this );

	m_interfaceType		= MAIL_INTERFACE_TYPE_RECVMAIL;

	m_bDisplay			= false;

	for( i=0; i<2; i++)
	{
		m_mailListCount[i]	= 0;
		m_selectedIdx[i] = -1;
		m_selectBarSavePos[i] = 0;

		memset( &m_mailList[i][0], 0, sizeof( MailEntry ) * MAX_MAIL_COUNT );
	}
	
	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_MAIL_WINDOW_TYPE );

}

CIFMailWin::~CIFMailWin()
{
}

void CIFMailWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_MAIL_WIN );

	
	m_textCtl[IT_MAIL_WIN_TITLE]->PrepareControl( IF_MAIL_WIN_TITLE );
	
	m_textCtl[IT_MAIL_WIN_RECVMAIL_TITLE]->PrepareControl( IF_MAIL_WIN_RECVMAIL_TITLE );
	m_textCtl[IT_MAIL_WIN_RECVMAIL_MIN]->PrepareControl( IF_MAIL_WIN_RECVMAIL_MIN );
	m_textCtl[IT_MAIL_WIN_RECVMAIL_MAX]->PrepareControl( IF_MAIL_WIN_RECVMAIL_MAX );

	m_textCtl[IT_MAIL_WIN_STOREMAIL_TITLE]->PrepareControl( IF_MAIL_WIN_STOREMAIL_TITLE );
	m_textCtl[IT_MAIL_WIN_STOREMAIL_MIN]->PrepareControl( IF_MAIL_WIN_STOREMAIL_MIN );
	m_textCtl[IT_MAIL_WIN_STOREMAIL_MAX]->PrepareControl( IF_MAIL_WIN_STOREMAIL_MAX );

	m_textCtl[IT_MAIL_WIN_LIST_SENDER]->PrepareControl( IF_MAIL_WIN_LIST_SENDER );
	m_textCtl[IT_MAIL_WIN_LIST_TITLE]->PrepareControl( IF_MAIL_WIN_LIST_TITLE );
	m_textCtl[IT_MAIL_WIN_LIST_DATE]->PrepareControl( IF_MAIL_WIN_LIST_DATE );
	m_textCtl[IT_MAIL_WIN_LIST_STATUS]->PrepareControl( IF_MAIL_WIN_LIST_STATUS );

	
	m_titleBar->PrepareControl( IF_MAIL_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_MAIL_WIN_TITLE_BAR_BOTTOM );
	m_recvMailInfo->PrepareControl( IF_MAIL_WIN_RECVMAILINFO );
	m_storeMailInfo->PrepareControl( IF_MAIL_WIN_STOREMAILINFO );
	m_selectBar->PrepareControl( IF_MAIL_WIN_SELECT_BAR );

	
	m_recvMailBtn->PrepareControl( IF_MAIL_WIN_RECVMAIL_BTN_0 );
	m_storeMailBtn->PrepareControl( IF_MAIL_WIN_STOREMAIL_BTN_0 );
	m_writeMailBtn->PrepareControl( IF_MAIL_WIN_WRITEMAIL_BTN_0 );
	m_saveMailBtn->PrepareControl( IF_MAIL_WIN_SAVEMAIL_BTN_0 );
	m_deleteMailBtn->PrepareControl( IF_MAIL_WIN_DELETEMAIL_BTN_0 );
	m_cancelBtn->PrepareControl( IF_MAIL_WIN_CANCEL_BTN_0 );
	m_exitBtn->PrepareControl( IF_MAIL_WIN_EXIT_BTN_0 );

	
	m_scrollBar->PrepareControl( IF_MAIL_WIN_SCROLL );
	m_scrollBar->SetBarPosition( 0, 0 );

	for( int i=0; i<MAIL_WIN_LIST_MAX_NUM; i++ )
	{
		m_checkBtn[i]->PrepareControl( IF_MAIL_WIN_CHECKBOX_BTN_0 + i*2, 0.0f, (float)( MAIL_WIN_LIST_LINE_HEIGHT * i ) );
		m_checkBtn[i]->SetCheck( false );
	}

	m_selectBarSavePos[0] = g_ifMng->m_iPos[IF_MAIL_WIN_SELECT_BAR].clientY;
	m_selectBarSavePos[1] = g_ifMng->m_iPos[IF_MAIL_WIN_SELECT_BAR].clientY;
}

int CIFMailWin::WindowFrame()
{
	switch( m_interfaceType )
	{
	case MAIL_INTERFACE_TYPE_RECVMAIL:
		m_textCtl[IT_MAIL_WIN_TITLE]->SetText( g_LPACK.GetMessage(0,978) );
		m_saveMailBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		break;
	case MAIL_INTERFACE_TYPE_STOREMAIL:
		m_textCtl[IT_MAIL_WIN_TITLE]->SetText( g_LPACK.GetMessage(0,979) );
		m_saveMailBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		break;
	}

	int i;
	for( i=0; i<MAIL_WIN_LIST_MAX_NUM; i++ )
	{
		if( m_checkBtn[i]->GetCheck() ) break;
	}

	if( i == MAIL_WIN_LIST_MAX_NUM )
	{
		m_saveMailBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		m_deleteMailBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}
	else
	{
		m_saveMailBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_deleteMailBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	}

	m_textCtl[IT_MAIL_WIN_RECVMAIL_TITLE]->SetText( g_LPACK.GetMessage(0,978) );
	m_textCtl[IT_MAIL_WIN_STOREMAIL_TITLE]->SetText( g_LPACK.GetMessage(0,979) );

	m_textCtl[IT_MAIL_WIN_LIST_SENDER]->SetText( g_LPACK.GetMessage(0,980) );
	m_textCtl[IT_MAIL_WIN_LIST_TITLE]->SetText( g_LPACK.GetMessage(0,982) );	
	m_textCtl[IT_MAIL_WIN_LIST_DATE]->SetText( g_LPACK.GetMessage(0,983) );	
	m_textCtl[IT_MAIL_WIN_LIST_STATUS]->SetText( g_LPACK.GetMessage(0,984) );

	m_writeMailBtn->SetButtonText( g_LPACK.GetMessage(0,985) );
	m_saveMailBtn->SetButtonText( g_LPACK.GetMessage(0,973) );
	m_deleteMailBtn->SetButtonText( g_LPACK.GetMessage(0,244) );
	m_cancelBtn->SetButtonText( g_LPACK.GetMessage(0,521) );

	if( m_mailListCount[m_interfaceType] - MAIL_WIN_LIST_MAX_NUM > 0 && m_mailListCount[m_interfaceType] - MAIL_WIN_LIST_MAX_NUM != m_scrollBar->m_max )
	{
		m_scrollBar->SetBarPosition( m_mailListCount[m_interfaceType] - MAIL_WIN_LIST_MAX_NUM, m_scrollBar->m_current );
	}
	else if( m_mailListCount[m_interfaceType] - MAIL_WIN_LIST_MAX_NUM <= 0 )
	{
		m_scrollBar->SetBarPosition( 0, 0 );
	}

	return( IF_CTL_NONE );
}

void CIFMailWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	m_recvMailInfo->Render();
	m_storeMailInfo->Render();


	
	m_recvMailBtn->Render();
	m_storeMailBtn->Render();
	m_writeMailBtn->RenderTextBtn();
	m_saveMailBtn->RenderTextBtn();
	m_deleteMailBtn->RenderTextBtn();
	m_cancelBtn->RenderTextBtn();
	m_exitBtn->Render();
	
	
	m_scrollBar->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_MAIL_WIN_TITLE]->Render();
		m_textCtl[IT_MAIL_WIN_RECVMAIL_TITLE]->Render();
		m_textCtl[IT_MAIL_WIN_STOREMAIL_TITLE]->Render();

		m_textCtl[IT_MAIL_WIN_LIST_SENDER]->Render();
		m_textCtl[IT_MAIL_WIN_LIST_TITLE]->Render();
		m_textCtl[IT_MAIL_WIN_LIST_DATE]->Render();
		m_textCtl[IT_MAIL_WIN_LIST_STATUS]->Render();

		UpdateMailList();
	}
	g_chatManager->EndFrame();
}

int CIFMailWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFMailWin::WindowProcess()
{
	if( m_exitBtn->ProcessButton() == IF_CTL_SELECT )
	{
		for( int i=0; i<MAIL_WIN_LIST_MAX_NUM; i++ )
			m_checkBtn[i]->SetCheck( false );

		Enable( false );
		return IF_CTL_SELECT;
	}

	if( m_recvMailBtn->ProcessButton() == IF_CTL_SELECT )
	{
		SetDisplayMode( false );
		DeleteMailList();
		GTH_SendMessage_MailSystem_Receive();
		SetInterfaceType( MAIL_INTERFACE_TYPE_RECVMAIL );

		return IF_CTL_SELECT;
	}

	if( m_storeMailBtn->ProcessButton() == IF_CTL_SELECT )
	{
		SetDisplayMode( false );
		DeleteMailList();
		GTH_SendMessage_MailSystem_Receive();
		SetInterfaceType( MAIL_INTERFACE_TYPE_STOREMAIL );

		return IF_CTL_SELECT;
	}

	if( m_writeMailBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );

		g_ifMng->m_mailEditWin->DeleteMailContent();
		g_ifMng->m_mailEditWin->SetInterfaceType( MAILEDIT_INTERFACE_TYPE_WRITEMAIL );
		g_ifMng->m_mailEditWin->Enable( true );
		g_ifMng->SetFocus( g_ifMng->m_mailEditWin );

		g_chatManager->InitInputStr( INPUT_INDEX_MAIL_NAME );
		g_chatManager->InitInputStr( INPUT_INDEX_MAIL_TITLE );
		g_chatManager->InitInputStr( INPUT_INDEX_MAIL_CONTENT );

		g_chatManager->SetInputState( INPUT_INDEX_MAIL_NAME, 1 );
		g_chatManager->SetInputState( INPUT_INDEX_MAIL_TITLE, 1 );
		g_chatManager->SetInputState( INPUT_INDEX_MAIL_CONTENT, 1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_MAIL_NAME );

		return IF_CTL_SELECT;
	}

	if( m_saveMailBtn->ProcessButton() == IF_CTL_SELECT )
	{
		int mailIdx[MAX_MAIL_COUNT];
		int count = 0;

		for( int i=0; i<m_mailListCount[m_interfaceType]; i++ )
		{
			if( m_mailList[m_interfaceType][i].checkFlag )
			{
				mailIdx[count] = m_mailList[m_interfaceType][i].mailIdx;
				count++;
			}
		}

		GTH_SendMessage_MailSystem_Save( mailIdx, count );

		SetDisplayMode( false );
		DeleteMailList();

		GTH_SendMessage_MailSystem_Receive();

		return IF_CTL_SELECT;
	}

	if( m_deleteMailBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), g_LPACK.GetMessage(0,991), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_DELETE_MAIL );

		return IF_CTL_SELECT;
	}

	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		for( int i=0; i<MAIL_WIN_LIST_MAX_NUM; i++ )
			m_checkBtn[i]->SetCheck( false );
		
		Enable( false );

		return IF_CTL_SELECT;
	}

	if( m_scrollBar->ProcessScrollBar() == IF_CTL_SELECT )
	{
		return IF_CTL_SELECT;
	}

	for( int i=0; i<MAIL_WIN_LIST_MAX_NUM && i<m_mailListCount[m_interfaceType]; i++ )
	{
		if( m_checkBtn[i]->ProcessCheckBox() == IF_CTL_SELECT )
		{
			m_mailList[m_interfaceType][i+m_scrollBar->m_current].checkFlag = m_checkBtn[i]->GetCheck();
			return IF_CTL_SELECT;
		}
	}
	
	
	int mx, my, x1, y1, xs, ys;
	int seletedIdx, listNum;
	
	if( m_mailListCount[m_interfaceType] < MAIL_WIN_LIST_MAX_NUM )
		listNum = m_mailListCount[m_interfaceType];
	else
		listNum = MAIL_WIN_LIST_MAX_NUM;

	mx = g_input->GetPosX();
	my = g_input->GetPosY();
	x1 = m_textCtl[IT_MAIL_WIN_LIST_SENDER]->GetXPos();
	y1 = m_textCtl[IT_MAIL_WIN_LIST_SENDER]->GetYPos() + 19;
	xs = 545;
	ys = listNum * MAIL_WIN_LIST_LINE_HEIGHT;

	mx = mx - x1;
	my = my - y1;
	
	if( mx < 0 || my < 0 || mx > xs || my > ys ) 
		seletedIdx = -1;
	else
		seletedIdx = my / MAIL_WIN_LIST_LINE_HEIGHT;

	if( seletedIdx >= 0 && seletedIdx < listNum )
	{
		if( g_input->GetLBUp() ) 
		{
			m_selectedIdx[m_interfaceType] = seletedIdx + m_scrollBar->m_current;
		}
		else if( g_input->GetLBDBClk() )
		{
			m_selectedIdx[m_interfaceType] = seletedIdx + m_scrollBar->m_current;

			if( m_selectedIdx[m_interfaceType] < 0 ) return IF_CTL_NONE;

			Enable( false );

			g_ifMng->m_mailEditWin->SetDisplayMode( false );
			g_ifMng->m_mailEditWin->DeleteMailContent();			
			GTH_SendMessage_MailSystem_View( m_mailList[m_interfaceType][m_selectedIdx[m_interfaceType]].mailIdx );
			g_ifMng->m_mailEditWin->SetInterfaceType( MAILEDIT_INTERFACE_TYPE_READMAIL );
			g_ifMng->m_mailEditWin->Enable( true );
			g_ifMng->SetFocus( g_ifMng->m_mailEditWin );
		}
	}
	else
	{
		if( g_input->GetLBUp() ) m_selectedIdx[m_interfaceType] = -1;
	}

	return IF_CTL_NONE;
}

void CIFMailWin::SetInterfaceType( int type )
{
	m_interfaceType = type;
}

void CIFMailWin::UpdateMailList()
{
	int i, j, idx;
	char buffer[2048];

	if( !m_bDisplay )
		return;

	sprintf( buffer, "%d", m_mailListCount[MAIL_INTERFACE_TYPE_RECVMAIL] );
	g_chatManager->DrawText( m_textCtl[IT_MAIL_WIN_RECVMAIL_MIN]->GetXPos(), m_textCtl[IT_MAIL_WIN_RECVMAIL_MIN]->GetYPos(), buffer );

	sprintf( buffer, "50" );
	g_chatManager->DrawText( m_textCtl[IT_MAIL_WIN_RECVMAIL_MAX]->GetXPos(), m_textCtl[IT_MAIL_WIN_RECVMAIL_MAX]->GetYPos(), buffer );

	sprintf( buffer, "%d", m_mailListCount[MAIL_INTERFACE_TYPE_STOREMAIL] );
	g_chatManager->DrawText( m_textCtl[IT_MAIL_WIN_STOREMAIL_MIN]->GetXPos(), m_textCtl[IT_MAIL_WIN_STOREMAIL_MIN]->GetYPos(), buffer );

	sprintf( buffer, "50" );
	g_chatManager->DrawText( m_textCtl[IT_MAIL_WIN_STOREMAIL_MAX]->GetXPos(), m_textCtl[IT_MAIL_WIN_STOREMAIL_MAX]->GetYPos(), buffer );

	for( i=1, j=0; i<=MAIL_WIN_LIST_MAX_NUM && j<m_mailListCount[m_interfaceType]; j++ )
	{
		idx = j + m_scrollBar->m_current;

		m_checkBtn[i-1]->SetCheck( m_mailList[m_interfaceType][idx].checkFlag );
		
		if( idx == m_selectedIdx[m_interfaceType] )
		{
			SetSelectBar( i - 1 );
		}

		m_checkBtn[i-1]->Render();

		strcpy( buffer, m_mailList[m_interfaceType][idx].fromName );
		g_chatManager->DrawTextMail( m_textCtl[IT_MAIL_WIN_LIST_SENDER]->GetXPos(), m_textCtl[IT_MAIL_WIN_LIST_SENDER]->GetYPos()+i*MAIL_WIN_LIST_LINE_HEIGHT+2, buffer, 109, 13, CHAT_ARRANGE_OVERSIZECUT );

		strcpy( buffer, m_mailList[m_interfaceType][idx].mailTitle );
		g_chatManager->DrawTextMail( m_textCtl[IT_MAIL_WIN_LIST_TITLE]->GetXPos(), m_textCtl[IT_MAIL_WIN_LIST_TITLE]->GetYPos()+i*MAIL_WIN_LIST_LINE_HEIGHT+2, buffer, 253, 13, CHAT_ARRANGE_OVERSIZECUT );
	
		strcpy( buffer, m_mailList[m_interfaceType][idx].mailTime );
		g_chatManager->DrawTextMail( m_textCtl[IT_MAIL_WIN_LIST_DATE]->GetXPos(), m_textCtl[IT_MAIL_WIN_LIST_DATE]->GetYPos()+i*MAIL_WIN_LIST_LINE_HEIGHT+2, buffer, 121, 13, CHAT_ARRANGE_OVERSIZECUT );

		switch( m_mailList[m_interfaceType][idx].mailState )
		{
		case 0 : case 2 :
			strcpy( buffer, g_LPACK.GetMessage(0,987) );
			break;
		case 1 : case 3 :
			strcpy( buffer, g_LPACK.GetMessage(0,988) );
			break;
		default :
			strcpy( buffer, "" );
		}

		g_chatManager->DrawText( m_textCtl[IT_MAIL_WIN_LIST_STATUS]->GetXPos(), m_textCtl[IT_MAIL_WIN_LIST_STATUS]->GetYPos()+i*MAIL_WIN_LIST_LINE_HEIGHT+2, buffer, 49, 13, CHAT_ARRANGE_OVERSIZECUT );

		i++;
	}
}

void CIFMailWin::DeleteMailList()
{
	m_mailListCount[0] = 0;
	m_mailListCount[1] = 0;
	m_selectedIdx[0] = -1;
	m_selectedIdx[1] = -1;
	memset( &m_mailList[0][0], 0, sizeof( MailEntry ) * MAX_MAIL_COUNT );
	memset( &m_mailList[1][0], 0, sizeof( MailEntry ) * MAX_MAIL_COUNT );
}

void CIFMailWin::SetDisplayMode( bool mode )
{
	m_bDisplay = mode;
}

void CIFMailWin::DeleteSelectedMails()
{
	int mailIdx[MAX_MAIL_COUNT];
	int count = 0;

	for( int i=0; i<m_mailListCount[m_interfaceType]; i++ )
	{
		if( m_mailList[m_interfaceType][i].checkFlag )
		{
			mailIdx[count] = m_mailList[m_interfaceType][i].mailIdx;
			count++;
		}
	}

	GTH_SendMessage_MailSystem_Delete( mailIdx, count );

	SetDisplayMode( false );
	DeleteMailList();

	GTH_SendMessage_MailSystem_Receive();

}

void CIFMailWin::SetSelectBar( int idx )
{
	m_selectBar->Enable();
	g_ifMng->m_iPos[IF_MAIL_WIN_SELECT_BAR].clientY = m_selectBarSavePos[m_interfaceType] + idx * MAIL_WIN_LIST_LINE_HEIGHT;
	m_selectBar->UpdateControl();
	m_selectBar->UpdateBuffer();
	m_selectBar->Render();
}





CIFMailEditWin::CIFMailEditWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i=0; i<NUM_IT_MAILEDIT_WIN; i++ )
		m_textCtl[i] = new CIFText( this );

	
	m_titleBar			= new CIFControl( this );
	m_titleBarBottom	= new CIFControl( this );
	m_mailInfo			= new CIFControl( this );

	
	m_sendMailBtn		= new CIFButtonCtl( this );
	m_cancelBtn			= new CIFButtonCtl( this );
	m_exitBtn			= new CIFButtonCtl( this );

	
	m_scrollBar			= new CIFScrollBar( this );

	m_interfaceType		= MAILEDIT_INTERFACE_TYPE_READMAIL;

	m_bDisplay			= false;

	m_mailLine[MAILEDIT_INTERFACE_TYPE_READMAIL]	= 0;
	m_mailLine[MAILEDIT_INTERFACE_TYPE_WRITEMAIL]	= 0;

	m_bVisibleCursor		 = true;

	memset( &m_mail, 0, sizeof( MailEntry ) );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_MAILEDIT_WINDOW_TYPE );

}

CIFMailEditWin::~CIFMailEditWin()
{
}

void CIFMailEditWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_MAILEDIT_WIN );

	
	m_textCtl[IT_MAILEDIT_WIN_TITLE]->PrepareControl( IF_MAILEDIT_WIN_TITLE );

	m_textCtl[IT_MAILEDIT_WIN_DATE_LABEL]->PrepareControl( IF_MAILEDIT_WIN_DATE_LABEL );
	m_textCtl[IT_MAILEDIT_WIN_USERNAME_LABEL]->PrepareControl( IF_MAILEDIT_WIN_USERNAME_LABEL );
	m_textCtl[IT_MAILEDIT_WIN_MAILTITLE_LABEL]->PrepareControl( IF_MAILEDIT_WIN_MAILTITLE_LABEL );

	m_textCtl[IT_MAILEDIT_WIN_DATE]->PrepareControl( IF_MAILEDIT_WIN_DATE );
	m_textCtl[IT_MAILEDIT_WIN_USERNAME]->PrepareControl( IF_MAILEDIT_WIN_USERNAME );
	m_textCtl[IT_MAILEDIT_WIN_MAILTITLE]->PrepareControl( IF_MAILEDIT_WIN_MAILTITLE );

	m_textCtl[IT_MAILEDIT_WIN_CONTENT]->PrepareControl( IF_MAILEDIT_WIN_CONTENT );

	
	m_titleBar->PrepareControl( IF_MAILEDIT_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_MAILEDIT_WIN_TITLE_BAR_BOTTOM );
	m_mailInfo->PrepareControl( IF_MAILEDIT_WIN_MAILINFO );

	
	m_sendMailBtn->PrepareControl( IF_MAILEDIT_WIN_SENDMAIL_BTN_0 );
	m_cancelBtn->PrepareControl( IF_MAILEDIT_WIN_CANCEL_BTN_0 );
	m_exitBtn->PrepareControl( IF_MAILEDIT_WIN_EXIT_BTN_0 );

	
	m_scrollBar->PrepareControl( IF_MAILEDIT_WIN_SCROLL );
	m_scrollBar->SetBarPosition( 0, 0 );

	int cx, cy;
	cx = m_textCtl[IT_MAILEDIT_WIN_USERNAME]->GetXPos();
	cy = m_textCtl[IT_MAILEDIT_WIN_USERNAME]->GetYPos();

	g_chatManager->SetInput( cx, cy, NAMESTRING - 2, NAMESTRING - 2, INPUT_INDEX_MAIL_NAME, 1, 450 );

	cx = m_textCtl[IT_MAILEDIT_WIN_MAILTITLE]->GetXPos();
	cy = m_textCtl[IT_MAILEDIT_WIN_MAILTITLE]->GetYPos();

	g_chatManager->SetInput( cx, cy, MAIL_TITLESIZE - 1, MAIL_TITLESIZE - 1, INPUT_INDEX_MAIL_TITLE, 1, 450 );

	cx = m_textCtl[IT_MAILEDIT_WIN_CONTENT]->GetXPos();
	cy = m_textCtl[IT_MAILEDIT_WIN_CONTENT]->GetYPos();

	g_chatManager->SetInput( cx, cy, MAIL_STRSIZE - 1, MAIL_STRSIZE - 1, INPUT_INDEX_MAIL_CONTENT, 1, 560, true );
	g_chatManager->SetInputLine( INPUT_INDEX_MAIL_CONTENT, 0, MAX_MAIL_LINE );

}

int CIFMailEditWin::WindowFrame()
{
	switch( m_interfaceType )
	{
	case MAILEDIT_INTERFACE_TYPE_WRITEMAIL:
		{
			m_textCtl[IT_MAILEDIT_WIN_TITLE]->SetText( g_LPACK.GetMessage(0,985) );
			m_textCtl[IT_MAILEDIT_WIN_USERNAME_LABEL]->SetText( g_LPACK.GetMessage(0,981) );
			m_sendMailBtn->SetButtonText( g_LPACK.GetMessage(0,974) );
		
			m_mailLine[MAILEDIT_INTERFACE_TYPE_WRITEMAIL] = g_chatManager->GetInputLine( INPUT_INDEX_MAIL_CONTENT );

			if( m_bVisibleCursor )
			{
				int curLine = g_chatManager->GetCursorLine( INPUT_INDEX_MAIL_CONTENT );

				if( curLine < m_scrollBar->m_current )
					m_scrollBar->SetBarPositionCurrent( curLine );

				if( curLine >= m_scrollBar->m_current + MAX_MAIL_LINE )
					m_scrollBar->SetBarPositionCurrent( curLine - MAX_MAIL_LINE + 1 );
			}
		}

		break;
	case MAILEDIT_INTERFACE_TYPE_READMAIL:
		{
			m_textCtl[IT_MAILEDIT_WIN_TITLE]->SetText( g_LPACK.GetMessage(0,986) );
			m_textCtl[IT_MAILEDIT_WIN_USERNAME_LABEL]->SetText( g_LPACK.GetMessage(0,980) );
			m_sendMailBtn->SetButtonText( g_LPACK.GetMessage(0,989) );
		}

		break;
	}

	m_textCtl[IT_MAILEDIT_WIN_DATE_LABEL]->SetText( g_LPACK.GetMessage(0,983) );
	m_textCtl[IT_MAILEDIT_WIN_MAILTITLE_LABEL]->SetText( g_LPACK.GetMessage(0,982) );

	m_textCtl[IT_MAILEDIT_WIN_DATE]->SetText( m_mail.mailTime );
	m_textCtl[IT_MAILEDIT_WIN_USERNAME]->SetText( m_mail.fromName );
	m_textCtl[IT_MAILEDIT_WIN_MAILTITLE]->SetText( m_mail.mailTitle );


	m_cancelBtn->SetButtonText( g_LPACK.GetMessage(0,521) );

	g_chatManager->m_ix[INPUT_INDEX_MAIL_NAME] = m_textCtl[IT_MAILEDIT_WIN_USERNAME]->GetXPos();
	g_chatManager->m_iy[INPUT_INDEX_MAIL_NAME] = m_textCtl[IT_MAILEDIT_WIN_USERNAME]->GetYPos();

	g_chatManager->m_ix[INPUT_INDEX_MAIL_TITLE] = m_textCtl[IT_MAILEDIT_WIN_MAILTITLE]->GetXPos();
	g_chatManager->m_iy[INPUT_INDEX_MAIL_TITLE] = m_textCtl[IT_MAILEDIT_WIN_MAILTITLE]->GetYPos();

	g_chatManager->m_ix[INPUT_INDEX_MAIL_CONTENT] = m_textCtl[IT_MAILEDIT_WIN_CONTENT]->GetXPos();
	g_chatManager->m_iy[INPUT_INDEX_MAIL_CONTENT] = m_textCtl[IT_MAILEDIT_WIN_CONTENT]->GetYPos();


	if( m_mailLine[m_interfaceType] - MAX_MAIL_LINE > 0 && m_mailLine[m_interfaceType] - MAX_MAIL_LINE != m_scrollBar->m_max )
	{
		m_scrollBar->SetBarPosition( m_mailLine[m_interfaceType] - MAX_MAIL_LINE, m_scrollBar->m_current );
	}
	else if( m_mailLine[m_interfaceType] - MAX_MAIL_LINE <= 0 )
	{
		m_scrollBar->SetBarPosition( 0, 0 );
	}

	g_chatManager->SetInputLine( INPUT_INDEX_MAIL_CONTENT, m_scrollBar->m_current, m_scrollBar->m_current + MAX_MAIL_LINE - 1);

	return( IF_CTL_NONE );
}

void CIFMailEditWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	m_mailInfo->Render();

	
	m_sendMailBtn->RenderTextBtn();
	m_cancelBtn->RenderTextBtn();
	m_exitBtn->Render();
	
	
	m_scrollBar->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->SetColor( 0xffffffff );

	int cx, cy;
	cx = m_textCtl[IT_MAILEDIT_WIN_CONTENT]->GetXPos();
	cy = m_textCtl[IT_MAILEDIT_WIN_CONTENT]->GetYPos() - m_scrollBar->m_current * 13;

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_MAILEDIT_WIN_TITLE]->Render();
		m_textCtl[IT_MAILEDIT_WIN_DATE_LABEL]->Render();
		m_textCtl[IT_MAILEDIT_WIN_USERNAME_LABEL]->Render();
		m_textCtl[IT_MAILEDIT_WIN_MAILTITLE_LABEL]->Render();

		if( m_interfaceType == MAILEDIT_INTERFACE_TYPE_WRITEMAIL )
		{
			g_chatManager->RenderInputFrame( INPUT_INDEX_MAIL_NAME );
			g_chatManager->RenderInputFrame( INPUT_INDEX_MAIL_TITLE );

			g_chatManager->SetInputPosition( INPUT_INDEX_MAIL_CONTENT, cx, cy );
			g_chatManager->RenderInputFrame( INPUT_INDEX_MAIL_CONTENT, m_bVisibleCursor );
		}
		else
		{
			if( m_bDisplay )
			{
				m_textCtl[IT_MAILEDIT_WIN_DATE]->Render( IF_TEXT_OUTPUT_MAIL );
				m_textCtl[IT_MAILEDIT_WIN_USERNAME]->Render( IF_TEXT_OUTPUT_MAIL );
				m_textCtl[IT_MAILEDIT_WIN_MAILTITLE]->Render( IF_TEXT_OUTPUT_MAIL );

				g_chatManager->DrawTextMail( cx, cy , m_mail.mailStr, 560, 13, CHAT_ARRANGE_NONE, INPUT_INDEX_MAIL_CONTENT );
			}
		}
	}
	g_chatManager->EndFrame();
}

int CIFMailEditWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFMailEditWin::WindowProcess()
{
	if( m_exitBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		g_chatManager->SetInputState( INPUT_INDEX_MAIL_NAME, -1 );
		g_chatManager->SetInputState( INPUT_INDEX_MAIL_TITLE, -1 );
		g_chatManager->SetInputState( INPUT_INDEX_MAIL_CONTENT, -1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
		return IF_CTL_SELECT;
	}

	if( m_sendMailBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_interfaceType == MAILEDIT_INTERFACE_TYPE_READMAIL )
		{
			char szTemp[256], szTitle[256];

			g_chatManager->InitInputStr( INPUT_INDEX_MAIL_NAME );
			g_chatManager->InitInputStr( INPUT_INDEX_MAIL_TITLE );
			g_chatManager->InitInputStr( INPUT_INDEX_MAIL_CONTENT );

			g_chatManager->AddInputStr( INPUT_INDEX_MAIL_NAME, m_mail.fromName );

			sprintf( szTemp, g_LPACK.GetMessage(0,990), m_mail.mailTitle );
			sstrncpy( szTitle, szTemp, MAIL_TITLESIZE );

			g_chatManager->AddInputStr( INPUT_INDEX_MAIL_TITLE, szTitle );

			g_ifMng->m_mailEditWin->DeleteMailContent();
			
			g_ifMng->m_mailEditWin->SetInterfaceType( MAILEDIT_INTERFACE_TYPE_WRITEMAIL );
			
			g_chatManager->SetInputState( INPUT_INDEX_MAIL_NAME, 1 );
			g_chatManager->SetInputState( INPUT_INDEX_MAIL_TITLE, 1 );
			g_chatManager->SetInputState( INPUT_INDEX_MAIL_CONTENT, 1 );
			g_chatManager->ChangeInputMessage( INPUT_INDEX_MAIL_TITLE );
			
		}
		else
		{
			memset( &m_mail, 0, sizeof( m_mail ) );

			strcpy( m_mail.fromName, g_chatManager->GetInputStr( INPUT_INDEX_MAIL_NAME ) ) ;
			strcpy( m_mail.mailTitle, g_chatManager->GetInputStr( INPUT_INDEX_MAIL_TITLE ) );
			strcpy( m_mail.mailStr, g_chatManager->GetInputStr( INPUT_INDEX_MAIL_CONTENT ) );

			if( m_mail.fromName[0] == '\0' || m_mail.mailTitle[0] == '\0' || m_mail.mailStr[0] == '\0' )
			{
				g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), g_LPACK.GetMessage(0,992) );
				return IF_CTL_SELECT;
			}

			GTH_SendMessage_MailSystem_Send( m_mail.fromName, m_mail.mailTitle, m_mail.mailStr );
		}

		return IF_CTL_SELECT;
	}

	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		g_chatManager->SetInputState( INPUT_INDEX_MAIL_NAME, -1 );
		g_chatManager->SetInputState( INPUT_INDEX_MAIL_TITLE, -1 );
		g_chatManager->SetInputState( INPUT_INDEX_MAIL_CONTENT, -1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );

		g_ifMng->m_mailWin->SetDisplayMode( false );
		g_ifMng->m_mailWin->DeleteMailList();
		GTH_SendMessage_MailSystem_Receive();

		g_ifMng->m_mailWin->Enable( true );
		g_ifMng->SetFocus( g_ifMng->m_mailWin );

		return IF_CTL_SELECT;
	}

	if( m_scrollBar->ProcessScrollBar() == IF_CTL_SELECT )
	{
		m_bVisibleCursor = false;
		return IF_CTL_NONE;
	}
	
	if( m_interfaceType == MAILEDIT_INTERFACE_TYPE_WRITEMAIL )
	{
		
		int m_x, m_y, mx, my, x1, y1, xs, ys;
		int seletedIdx;

		m_x = g_input->GetPosX();
		m_y = g_input->GetPosY();

		x1 = m_textCtl[IT_MAILEDIT_WIN_USERNAME]->GetXPos();
		y1 = m_textCtl[IT_MAILEDIT_WIN_USERNAME]->GetYPos();
		xs = 450;
		ys = 17;

		mx = m_x - x1;
		my = m_y - y1;
		
		if( mx < 0 || my < 0 || mx > xs || my > ys ) 
			seletedIdx = -1;
		else
			seletedIdx = 0;

		if( seletedIdx != -1 )
		{
			if( g_input->GetLBUp() )
			{
				g_chatManager->ChangeInputMessage( INPUT_INDEX_MAIL_NAME );
				g_chatManager->SetCursorFromMousePos( m_x, m_y );
				m_bVisibleCursor = true;
			}
		}
		else
		{
			x1 = m_textCtl[IT_MAILEDIT_WIN_MAILTITLE]->GetXPos();
			y1 = m_textCtl[IT_MAILEDIT_WIN_MAILTITLE]->GetYPos();
			xs = 450;
			ys = 17;

			mx = m_x - x1;
			my = m_y - y1;
			
			if( mx < 0 || my < 0 || mx > xs || my > ys ) 
				seletedIdx = -1;
			else
				seletedIdx = 0;

			if( seletedIdx != -1 )
			{
				if( g_input->GetLBUp() )
				{
					g_chatManager->ChangeInputMessage( INPUT_INDEX_MAIL_TITLE );
					g_chatManager->SetCursorFromMousePos( m_x, m_y );
					m_bVisibleCursor = true;
				}

			}
			else
			{
				x1 = m_textCtl[IT_MAILEDIT_WIN_CONTENT]->GetXPos();
				y1 = m_textCtl[IT_MAILEDIT_WIN_CONTENT]->GetYPos();
				xs = 560;
				ys = 13 * MAX_MAIL_LINE;

				mx = m_x - x1;
				my = m_y - y1;
				
				if( mx < 0 || my < 0 || mx > xs || my > ys ) 
					seletedIdx = -1;
				else
					seletedIdx = 0;

				if( seletedIdx != -1 )
				{
					if( g_input->GetLBUp() )
					{
						g_chatManager->ChangeInputMessage( INPUT_INDEX_MAIL_CONTENT );
						g_chatManager->SetCursorFromMousePos( m_x, m_y );
						m_bVisibleCursor = true;
					}
				}
			}
		}
	}

	return IF_CTL_NONE;
}

void CIFMailEditWin::SetInterfaceType( int type )
{
	m_interfaceType = type;
}

void CIFMailEditWin::UpdateMailContent()
{

}

void CIFMailEditWin::SetMailContent( MailEntry &mail )
{
	memset( &m_mail, 0, sizeof( MailEntry ) );

	m_mail.mailIdx		= mail.mailIdx;
	m_mail.mailState	= mail.mailState;
	strcpy( m_mail.fromName, mail.fromName );
	strcpy( m_mail.mailTime, mail.mailTime );
	strcpy( m_mail.mailTitle, mail.mailTitle );
	strcpy( m_mail.mailStr, mail.mailStr );

	m_mailLine[m_interfaceType] = g_chatManager->GetViewLine( mail.mailStr, 560 );
	m_scrollBar->SetBarPosition( m_mailLine[m_interfaceType], 0 );
}

void CIFMailEditWin::DeleteMailContent()
{
	memset( &m_mail, 0, sizeof( MailEntry ) );
}

void CIFMailEditWin::SetDisplayMode( bool mode )
{
	m_bDisplay = mode;
}












CIFFriendWin::CIFFriendWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_FRIEND_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	
	
	m_closeBtn = new CIFButtonCtl( this );

	m_onlineBar = new CIFControl( this );
	m_offlineBar = new CIFControl( this );

	m_nameLevelBar = new CIFControl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_FRIEND_WINDOW_TYPE );

	
	m_selectedFriendIdx = -1;
	memset( m_selectedFriendName, 0, NAMESTRING );
	m_totalOnlineFriendCount = 0; 
	m_totalFriendCount = 0; 
}

CIFFriendWin::~CIFFriendWin()
{
}

void CIFFriendWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_FRIEND_WIN );

	
	m_textCtl[IT_FRIEND_WIN_TITLE]->PrepareControl( IF_FRIEND_WIN_TITLE_TEXT );
	m_textCtl[IT_FRIEND_WIN_NAME]->PrepareControl( IF_FRIEND_WIN_NAME_TEXT );
	m_textCtl[IT_FRIEND_WIN_LEVEL]->PrepareControl( IF_FRIEND_WIN_LEVEL_TEXT );

	
	m_titleBar->PrepareControl( IF_FRIEND_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_FRIEND_WIN_TITLE_BAR_BOTTOM );
	
	
	m_closeBtn->PrepareControl( IF_FRIEND_WIN_CLOSE_BTN_0 );

	m_onlineBar->PrepareControl( IF_FRIEND_WIN_ONLINE_BAR );
	m_offlineBar->PrepareControl( IF_FRIEND_WIN_OFFLINE_BAR );

	m_nameLevelBar->PrepareControl( IF_FRIEND_WIN_NAMELEVEL_BAR );
}

int CIFFriendWin::WindowFrame()
{
	char szTemp[256];

	sprintf( szTemp, g_LPACK.GetMessage( 0, 722 ) );
	m_textCtl[IT_FRIEND_WIN_TITLE]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMessage( 0, 724 ), m_totalOnlineFriendCount, m_totalFriendCount );
	m_textCtl[IT_FRIEND_WIN_NAME]->SetText( szTemp );

	sprintf( szTemp, "Lv" );
	m_textCtl[IT_FRIEND_WIN_LEVEL]->SetText( szTemp );

	m_textCtl[IT_FRIEND_WIN_NAME]->SetColor( 0xffffffff );
	m_textCtl[IT_FRIEND_WIN_LEVEL]->SetColor( 0xffffffff );

	return( IF_CTL_NONE );
}

void CIFFriendWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();
	
	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	
	
	m_closeBtn->Render();

	
	m_nameLevelBar->Render();

	int i, idx = 1;
	for( i = 0; i < MAX_NUMBER_OF_FRIEND; i ++ )
	{
		if( g_cgv.myCharacterInfo->friendList[i].name[0] != 0 )
		{
			

			if( g_cgv.myCharacterInfo->friendList[i].worldIdx == -1 || g_cgv.myCharacterInfo->friendList[i].worldIdx == 255 )
			{
				
				m_offlineBar->m_info[0].clientY = m_textCtl[IT_FRIEND_WIN_NAME]->GetYPos() + 15*idx;

				
				m_offlineBar->UpdateBuffer();

				m_offlineBar->Render();
			}
			else
			{
				
				m_onlineBar->m_info[0].clientY = m_textCtl[IT_FRIEND_WIN_NAME]->GetYPos() + 15*idx;

				
				m_onlineBar->UpdateBuffer();

				m_onlineBar->Render();
			}

			idx++;
		}
	}
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_FRIEND_WIN_TITLE]->Render();
		m_textCtl[IT_FRIEND_WIN_NAME]->Render();
		m_textCtl[IT_FRIEND_WIN_LEVEL]->Render();

		char szTemp[32];
		int x, y, xs, ys;
		idx = 1;
		
		y = m_textCtl[IT_FRIEND_WIN_NAME]->GetYPos();
		xs = m_textCtl[IT_FRIEND_WIN_NAME]->GetXSize();
		ys = m_textCtl[IT_FRIEND_WIN_NAME]->GetYSize();
		for( i = 0; i < MAX_NUMBER_OF_FRIEND; i ++ )
		{
			if( g_cgv.myCharacterInfo->friendList[i].name[0] != 0 )
			{
				if( m_selectedFriendIdx == idx )
					g_chatManager->SetColor( 0xff0000ff );
				else
					g_chatManager->SetColor( 0xffffffff );

				x = m_textCtl[IT_FRIEND_WIN_NAME]->GetXPos();
				g_chatManager->DrawText( x, y + idx * 15, g_cgv.myCharacterInfo->friendList[i].name );

				if( g_cgv.myCharacterInfo->friendList[i].worldIdx != -1 && g_cgv.myCharacterInfo->friendList[i].worldIdx != 255 )
				{
					x = m_textCtl[IT_FRIEND_WIN_LEVEL]->GetXPos();
					sprintf( szTemp, "Lv.%d", g_cgv.myCharacterInfo->friendList[i].level );
					g_chatManager->DrawText( x, y + idx * 15, szTemp );
				}

				idx++;
			}
		}
	}
	g_chatManager->EndFrame();
}

int CIFFriendWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFFriendWin::WindowProcess()
{
	char szTemp[256];
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_SELECT;
	}

	int mx, my;
	int x1, y1, xs, ys, i, idx;

	idx = 1;
	x1 = m_textCtl[IT_FRIEND_WIN_NAME]->GetXPos();
	xs = m_textCtl[IT_FRIEND_WIN_NAME]->GetXSize();
	ys = m_textCtl[IT_FRIEND_WIN_NAME]->GetYSize();

	for( i=0; i<MAX_NUMBER_OF_FRIEND; i++ )
	{
		if( g_cgv.myCharacterInfo->friendList[i].name[0] != '\0' )
		{
			y1 = m_textCtl[IT_FRIEND_WIN_NAME]->GetYPos() + idx * 15;
			mx = g_input->GetPosX();
			my = g_input->GetPosY();
			mx = mx - x1;
			my = my - y1;
			
			if( mx < 0 || my < 0 || mx > xs || my > ys ) 
			{
				if( g_input->GetLBDown() )
				{
					m_selectedFriendName[0] = 0;
					m_selectedFriendIdx = -1;
				}
			}
			else
			{
				if( g_input->GetLBDown() )
				{
					m_selectedFriendIdx = idx;
					strcpy( m_selectedFriendName, g_cgv.myCharacterInfo->friendList[i].name );
				}
				else if( g_input->GetRBDown() )
				{
					if( g_cgv.myCharacterInfo->friendList[i].worldIdx != -1 && g_cgv.myCharacterInfo->friendList[i].worldIdx != 255 )
					{
						m_selectedFriendIdx = idx;
						strcpy( m_selectedFriendName, g_cgv.myCharacterInfo->friendList[i].name );

						
						g_ifMng->m_dlgWin->m_curSendType = 4; 
						g_ifMng->m_dlgWin->SetWhisper( g_cgv.myCharacterInfo->friendList[i].name );
					}
				}

				if( g_cgv.myCharacterInfo->friendList[i].worldIdx != -1 && g_cgv.myCharacterInfo->friendList[i].worldIdx != 255 )
				{
					g_ifMng->InitRolloverText();

					
					
					
					g_ifMng->AddRolloverText( g_LPACK.GetMessage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, g_cgv.myCharacterInfo->friendList[i].job ) );

					sprintf( szTemp, "(%s)", g_LPACK.GetMessage( 0, 723 ) );
					
					if( g_cgv.myCharacterInfo->friendList[i].worldIdx == -1 || g_cgv.myCharacterInfo->friendList[i].worldIdx == 255 )
						sprintf( szTemp, "(%s)", g_LPACK.GetMessage( 0, 723 ) );
						
					else
						sprintf( szTemp, "(%s)", g_cgv.worldTable[ g_cgv.myCharacterInfo->friendList[i].worldIdx ].hanName );
						

					g_ifMng->AddRolloverText( szTemp );

					g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true );
					
				}

				break;
			}
			
			idx++;
		}
	}

	return IF_CTL_NONE;
}

void CIFFriendWin::AddFriend(friend_t* newFriend)
{
	
	for( int i=0; i<MAX_NUMBER_OF_FRIEND; i++ )
	{
		if( g_cgv.myCharacterInfo->friendList[i].name[0] == 0 )
		{
			g_cgv.myCharacterInfo->friendList[i].job = newFriend->job;
			g_cgv.myCharacterInfo->friendList[i].level = newFriend->level;
			strcpy( g_cgv.myCharacterInfo->friendList[i].name, newFriend->name );
			g_cgv.myCharacterInfo->friendList[i].worldIdx = newFriend->worldIdx;

			m_totalFriendCount++;
			if( g_cgv.myCharacterInfo->friendList[i].worldIdx != -1 && g_cgv.myCharacterInfo->friendList[i].worldIdx != 255 )
			{
				m_totalOnlineFriendCount++;
			}
			break;
		}
	}

	
}

void CIFFriendWin::RemoveFriend(char *friendName)
{
	for( int i=0; i<MAX_NUMBER_OF_FRIEND; i++ )
	{
		if( g_cgv.myCharacterInfo->friendList[i].name[0] != 0 )
		{
			if( strcmp(g_cgv.myCharacterInfo->friendList[i].name, friendName) == 0 )
			{
				g_cgv.myCharacterInfo->friendList[i].job = 0;
				g_cgv.myCharacterInfo->friendList[i].level = 0;
				memset( g_cgv.myCharacterInfo->friendList[i].name, 0, NAMESTRING );

				
				if( g_cgv.myCharacterInfo->friendList[i].worldIdx != -1 && g_cgv.myCharacterInfo->friendList[i].worldIdx != 255 )
				{
					m_totalOnlineFriendCount--;
				}

				g_cgv.myCharacterInfo->friendList[i].worldIdx = -1;

				m_totalFriendCount--;
				break;
			}
		}
	}
}


void CIFFriendWin::UpdateFriend(friend_t* oldFriend)
{
	for( int i=0; i<MAX_NUMBER_OF_FRIEND; i++ )
	{
		if( g_cgv.myCharacterInfo->friendList[i].name[0] != 0 )
		{
			if( strcmp(g_cgv.myCharacterInfo->friendList[i].name, oldFriend->name) == 0 )
			{
				g_cgv.myCharacterInfo->friendList[i].job = oldFriend->job;
				g_cgv.myCharacterInfo->friendList[i].level = oldFriend->level;
				strcpy( g_cgv.myCharacterInfo->friendList[i].name, oldFriend->name );

				if( g_cgv.myCharacterInfo->friendList[i].worldIdx != -1 && g_cgv.myCharacterInfo->friendList[i].worldIdx != 255 )
				{
					
					if( oldFriend->worldIdx == -1 || oldFriend->worldIdx == 255 )
					{
						m_totalOnlineFriendCount--;
					}
				}
				else
				{
					
					if( oldFriend->worldIdx != -1 && oldFriend->worldIdx != 255 )
					{
						m_totalOnlineFriendCount++;
					}
				}

				g_cgv.myCharacterInfo->friendList[i].worldIdx = oldFriend->worldIdx;
				break;
			}
		}
	}

	
}













CIFGuildWin::CIFGuildWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_GUILD_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	
	
	m_closeBtn = new CIFButtonCtl( this );

	
	m_secedeBtn = new CIFButtonCtl( this );
	m_scheduleBtn = new CIFButtonCtl( this );
	m_authorityBtn = new CIFButtonCtl( this );
	m_close2Btn = new CIFButtonCtl( this );

	m_infoTab = new CIFControl( this );
	m_questRaidTab = new CIFControl( this );
	m_hunterPvPTab = new CIFControl( this );
	m_pledgeBattleTab = new CIFControl( this );

	m_countLabelBar = new CIFControl( this );

	m_scrollBar1 = new CIFScrollBar( this );
	m_scrollBar2 = new CIFScrollBar( this );

	m_selectBar = new CIFControl( this );

	m_selectedIdx[0] = -1;
	m_selectedIdx[1] = -1;
	m_selectBarSavePos[0] = 0;
	m_selectBarSavePos[1] = 0;
	
	memset( m_guildName, 0, sizeof( m_guildName ) );
	memset( m_scheduleString, 0, sizeof( m_scheduleString ) );

	memset( &m_onlineMember[0], 0, sizeof( m_onlineMember ) );
	memset( &m_offlineMember[0], 0, sizeof( m_offlineMember ) );

	m_onlineMemberNumber = 0;
	m_offlineMemberNumber = 0;

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_GUILD_WINDOW_TYPE );
}

CIFGuildWin::~CIFGuildWin()
{
}

void CIFGuildWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_GUILD_WIN );

	
	m_textCtl[IT_GUILD_WIN_TITLE]->PrepareControl( IF_GUILD_WIN_TITLE_TEXT );

	m_textCtl[IT_GUILD_WIN_INFOTAB1]->PrepareControl( IF_GUILD_WIN_INFOTAB1_TEXT );
	m_textCtl[IT_GUILD_WIN_INFOTAB2]->PrepareControl( IF_GUILD_WIN_INFOTAB2_TEXT );
	m_textCtl[IT_GUILD_WIN_INFOTAB3]->PrepareControl( IF_GUILD_WIN_INFOTAB3_TEXT );
	m_textCtl[IT_GUILD_WIN_INFOTAB4]->PrepareControl( IF_GUILD_WIN_INFOTAB4_TEXT );
	
	m_textCtl[IT_GUILD_WIN_GUILDINFO]->PrepareControl( IF_GUILD_WIN_GUILDINFOTITLE_TEXT );

	m_textCtl[IT_GUILD_WIN_FAMEPOINT]->PrepareControl( IF_GUILD_WIN_FAMEPOINT_TEXT );
	m_textCtl[IT_GUILD_WIN_RANK]->PrepareControl( IF_GUILD_WIN_RANK_TEXT );
	m_textCtl[IT_GUILD_WIN_EXPPOINT]->PrepareControl( IF_GUILD_WIN_EXPPOINT_TEXT );
	m_textCtl[IT_GUILD_WIN_RAIDRPOINT]->PrepareControl( IF_GUILD_WIN_RAIDPOINT_TEXT );
	m_textCtl[IT_GUILD_WIN_HUNTERPOINT]->PrepareControl( IF_GUILD_WIN_HUNTERPOINT_TEXT );
	m_textCtl[IT_GUILD_WIN_PVPPOINT]->PrepareControl( IF_GUILD_WIN_PVPPOINT_TEXT );

	m_textCtl[IT_GUILD_WIN_GUILDSCHEDULETITLE]->PrepareControl( IF_GUILD_WIN_GUILDSCHEDULETITLE_TEXT );
	m_textCtl[IT_GUILD_WIN_GUILDSCHEDULE]->PrepareControl( IF_GUILD_WIN_GUILDSCHEDULE_TEXT );

	m_textCtl[IT_GUILD_WIN_USERINFOTITLE]->PrepareControl( IF_GUILD_WIN_USERINFOTITLE_TEXT );
	m_textCtl[IT_GUILD_WIN_ONLINETITLE]->PrepareControl( IF_GUILD_WIN_ONLINETITLE_TEXT );
	m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->PrepareControl( IF_GUILD_WIN_OFFLINETITLE_TEXT );

	m_textCtl[IT_GUILD_WIN_NAMETITLE]->PrepareControl( IF_GUILD_WIN_NAMETITLE_TEXT );
	m_textCtl[IT_GUILD_WIN_LEVELETITLE]->PrepareControl( IF_GUILD_WIN_LEVELETITLE_TEXT );
	m_textCtl[IT_GUILD_WIN_JOBTITLE]->PrepareControl( IF_GUILD_WIN_JOBTITLE_TEXT );
	m_textCtl[IT_GUILD_WIN_LOCATIONTITLE]->PrepareControl( IF_GUILD_WIN_LOCATIONTITLE_TEXT );
	m_textCtl[IT_GUILD_WIN_RANKTITLE]->PrepareControl( IF_GUILD_WIN_RANKTITLE_TEXT );

	m_textCtl[IT_GUILD_WIN_ONLINECOUNT]->PrepareControl( IF_GUILD_WIN_ONLINECOUNT_TEXT );
	m_textCtl[IT_GUILD_WIN_OFFLINECOUNT]->PrepareControl( IF_GUILD_WIN_OFFLINECOUNT_TEXT );
	m_textCtl[IT_GUILD_WIN_TOTALCOUNT]->PrepareControl( IF_GUILD_WIN_TOTALCOUNT_TEXT );

	
	m_textCtl[IT_GUILD_WIN_SECEDE]->PrepareControl( IF_GUILD_WIN_SECEDE_TEXT );
	m_textCtl[IT_GUILD_WIN_SCHEDULE]->PrepareControl( IF_GUILD_WIN_SCHEDULE_TEXT );
	m_textCtl[IT_GUILD_WIN_AUTHORITY]->PrepareControl( IF_GUILD_WIN_AUTHORITY_TEXT );
	m_textCtl[IT_GUILD_WIN_CLOSE]->PrepareControl( IF_GUILD_WIN_CLOSE_TEXT );

	
	m_titleBar->PrepareControl( IF_GUILD_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_GUILD_WIN_TITLE_BAR_BOTTOM );
	m_selectBar->PrepareControl( IF_GUILD_WIN_SELECT_BAR );

	
	m_closeBtn->PrepareControl( IF_GUILD_WIN_CLOSE_BTN_0 );

	
	m_secedeBtn->PrepareControl( IF_GUILD_WIN_SECEDE_BTN_0 );
	m_scheduleBtn->PrepareControl( IF_GUILD_WIN_SCHEDULE_BTN_0 );
	m_authorityBtn->PrepareControl( IF_GUILD_WIN_AUTHORITY_BTN_0 );
	m_close2Btn->PrepareControl( IF_GUILD_WIN_CLOSE2_BTN_0 );

	m_countLabelBar->PrepareControl( IF_GUILD_WIN_COUNTLABEL_BAR );

	m_scrollBar1->PrepareControl( IF_GUILD_WIN_SCROLL1 );
	m_scrollBar2->PrepareControl( IF_GUILD_WIN_SCROLL2 );

	m_scrollBar1->SetBarPosition( 0, 0 );
	m_scrollBar2->SetBarPosition( 0, 0 );

	int cx, cy, cs;
	cx = m_textCtl[IT_GUILD_WIN_GUILDSCHEDULE]->GetXPos();
	cy = m_textCtl[IT_GUILD_WIN_GUILDSCHEDULE]->GetYPos();
	cs = m_textCtl[IT_GUILD_WIN_GUILDSCHEDULE]->GetXSize();

	
	g_chatManager->SetInput( cx, cy + 36, GUILD_WIN_SCHEDULE_STRING_SIZE - 1, GUILD_WIN_SCHEDULE_STRING_SIZE - 1, INPUT_INDEX_GUILD_SCHEDULE, 1, cs );

	m_secedeBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	m_scheduleBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	m_authorityBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	m_selectBarSavePos[0] = g_ifMng->m_iPos[IF_GUILD_WIN_ONLINETITLE_TEXT].clientY + SCROLL_LINE_HEIGHT + 17;
	m_selectBarSavePos[1] = g_ifMng->m_iPos[IF_GUILD_WIN_OFFLINETITLE_TEXT].clientY + SCROLL_LINE_HEIGHT + 17;

}

int CIFGuildWin::WindowFrame()
{
	char szTemp[256];

	sprintf( szTemp, g_LPACK.GetMessage( 0, 795 ), g_cgv.myCharacterInfo->guild.name );
	m_textCtl[IT_FRIEND_WIN_TITLE]->SetText( szTemp );

	

	
	
	
	

	m_textCtl[IT_GUILD_WIN_FAMEPOINT]->SetText( g_LPACK.GetMessage( 0, 796 ) );
	m_textCtl[IT_GUILD_WIN_RANK]->SetText( g_LPACK.GetMessage( 0, 797 ) );
	m_textCtl[IT_GUILD_WIN_EXPPOINT]->SetText( g_LPACK.GetMessage( 0, 798 ) );
	m_textCtl[IT_GUILD_WIN_RAIDRPOINT]->SetText( g_LPACK.GetMessage( 0, 799 ) );
	m_textCtl[IT_GUILD_WIN_HUNTERPOINT]->SetText( g_LPACK.GetMessage( 0, 800 ) );
	m_textCtl[IT_GUILD_WIN_PVPPOINT]->SetText( g_LPACK.GetMessage( 0, 801 ) );

	m_textCtl[IT_GUILD_WIN_GUILDSCHEDULETITLE]->SetText( g_LPACK.GetMessage( 0, 802 ) );

	m_textCtl[IT_GUILD_WIN_USERINFOTITLE]->SetText( g_LPACK.GetMessage( 0, 803 ) );
	m_textCtl[IT_GUILD_WIN_ONLINETITLE]->SetText( g_LPACK.GetMessage( 0, 804 ) );
	m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->SetText( g_LPACK.GetMessage( 0, 805 ) );

	
	m_textCtl[IT_GUILD_WIN_SECEDE]->SetText( g_LPACK.GetMessage(0,933) );
	m_textCtl[IT_GUILD_WIN_SCHEDULE]->SetText( g_LPACK.GetMessage(0,934) );
	m_textCtl[IT_GUILD_WIN_AUTHORITY]->SetText( g_LPACK.GetMessage(0,935) );
	m_textCtl[IT_GUILD_WIN_CLOSE]->SetText( g_LPACK.GetMessage(0,936) );

	
	m_textCtl[IT_FRIEND_WIN_TITLE]->SetColor( 0xffffffff );

	m_textCtl[IT_GUILD_WIN_FAMEPOINT]->SetColor( 0xffffff00 );
	m_textCtl[IT_GUILD_WIN_RANK]->SetColor( 0xffffff00 );
	m_textCtl[IT_GUILD_WIN_EXPPOINT]->SetColor( 0xffffff00 );
	m_textCtl[IT_GUILD_WIN_RAIDRPOINT]->SetColor( 0xffffff00 );
	m_textCtl[IT_GUILD_WIN_HUNTERPOINT]->SetColor( 0xffffff00 );
	m_textCtl[IT_GUILD_WIN_PVPPOINT]->SetColor( 0xffffff00 );

	if( m_onlineMemberNumber - MAX_SCROLL_LINE1 > 0 && m_onlineMemberNumber - MAX_SCROLL_LINE1 != m_scrollBar1->m_max )
	{
		m_scrollBar1->SetBarPosition( m_onlineMemberNumber - MAX_SCROLL_LINE1, m_scrollBar1->m_current );
	}
	else if( m_onlineMemberNumber - MAX_SCROLL_LINE1 <= 0)
	{
		m_scrollBar1->SetBarPosition( 0, 0 );
	}

	if( m_offlineMemberNumber - MAX_SCROLL_LINE2 > 0 && m_offlineMemberNumber - MAX_SCROLL_LINE2 != m_scrollBar2->m_max )
	{
		m_scrollBar2->SetBarPosition( m_offlineMemberNumber - MAX_SCROLL_LINE2, m_scrollBar2->m_current );
	}
	else if( m_offlineMemberNumber - MAX_SCROLL_LINE2 <= 0 )
	{
		m_scrollBar2->SetBarPosition( 0, 0 );
	}

	int cx, cy;
	cx = m_textCtl[IT_GUILD_WIN_GUILDSCHEDULE]->GetXPos();
	cy = m_textCtl[IT_GUILD_WIN_GUILDSCHEDULE]->GetYPos();
	g_chatManager->m_ix[INPUT_INDEX_GUILD_SCHEDULE] = cx;
	g_chatManager->m_iy[INPUT_INDEX_GUILD_SCHEDULE] = cy + 36;

	return( IF_CTL_NONE );
}


void CIFGuildWin::PutLabelText()
{
	char buffer[32];

	m_textCtl[IT_GUILD_WIN_GUILDINFO]->Render();
	
	sprintf( buffer, g_LPACK.GetMessage( 0, 806 ) );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_GUILDINFO]->GetXPos(), m_textCtl[IT_GUILD_WIN_GUILDINFO]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_GUILDINFO]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	sprintf( buffer, g_LPACK.GetMessage( 0, 807 ) );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_INFOTAB1]->GetXPos(), m_textCtl[IT_GUILD_WIN_INFOTAB1]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_INFOTAB1]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	sprintf( buffer, g_LPACK.GetMessage( 0, 808 ) );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_INFOTAB2]->GetXPos(), m_textCtl[IT_GUILD_WIN_INFOTAB2]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_INFOTAB2]->GetXSize(), 13, CHAT_ARRANGE_CENTER );
	
	sprintf( buffer, g_LPACK.GetMessage( 0, 809 ) );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_INFOTAB3]->GetXPos(), m_textCtl[IT_GUILD_WIN_INFOTAB3]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_INFOTAB3]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	sprintf( buffer, g_LPACK.GetMessage( 0, 810 ) );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_INFOTAB4]->GetXPos(), m_textCtl[IT_GUILD_WIN_INFOTAB4]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_INFOTAB4]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	m_textCtl[IT_GUILD_WIN_FAMEPOINT]->Render();
	m_textCtl[IT_GUILD_WIN_RANK]->Render();
	m_textCtl[IT_GUILD_WIN_EXPPOINT]->Render();
	m_textCtl[IT_GUILD_WIN_RAIDRPOINT]->Render();
	m_textCtl[IT_GUILD_WIN_HUNTERPOINT]->Render();
	m_textCtl[IT_GUILD_WIN_PVPPOINT]->Render();

	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_GUILDSCHEDULETITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_GUILDSCHEDULETITLE]->GetYPos(), 
		g_LPACK.GetMessage( 0, 802 ), m_textCtl[IT_GUILD_WIN_GUILDSCHEDULETITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_USERINFOTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_USERINFOTITLE]->GetYPos(), 
		g_LPACK.GetMessage( 0, 803 ), m_textCtl[IT_GUILD_WIN_USERINFOTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );
	
	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_ONLINETITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_ONLINETITLE]->GetYPos(), 
		g_LPACK.GetMessage( 0, 804 ), m_textCtl[IT_GUILD_WIN_ONLINETITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );
	
	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYPos(), 
		g_LPACK.GetMessage( 0, 805 ), m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	g_chatManager->SetColor( 0xffffffff );

	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_NAMETITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_NAMETITLE]->GetYPos(), 
		g_LPACK.GetMessage( 0, 811 ), m_textCtl[IT_GUILD_WIN_NAMETITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_NAMETITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYPos()+m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYSize(), 
		g_LPACK.GetMessage( 0, 811 ), m_textCtl[IT_GUILD_WIN_NAMETITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_LEVELETITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_LEVELETITLE]->GetYPos(), 
		g_LPACK.GetMessage( 0, 812 ), m_textCtl[IT_GUILD_WIN_LEVELETITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_LEVELETITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYPos()+m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYSize(), 
		g_LPACK.GetMessage( 0, 812 ), m_textCtl[IT_GUILD_WIN_LEVELETITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_JOBTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_JOBTITLE]->GetYPos(), 
		g_LPACK.GetMessage( 0, 813 ), m_textCtl[IT_GUILD_WIN_JOBTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_JOBTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYPos()+m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYSize(), 
		g_LPACK.GetMessage( 0, 813 ), m_textCtl[IT_GUILD_WIN_JOBTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_LOCATIONTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_LOCATIONTITLE]->GetYPos(), 
		g_LPACK.GetMessage( 0, 814 ), m_textCtl[IT_GUILD_WIN_LOCATIONTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_LOCATIONTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYPos()+m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYSize(), 
		g_LPACK.GetMessage( 0, 814 ), m_textCtl[IT_GUILD_WIN_LOCATIONTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_RANKTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_RANKTITLE]->GetYPos(), 
		g_LPACK.GetMessage( 0, 815 ), m_textCtl[IT_GUILD_WIN_RANKTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_RANKTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYPos()+m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYSize(), 
		g_LPACK.GetMessage( 0, 815 ), m_textCtl[IT_GUILD_WIN_RANKTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	m_textCtl[IT_GUILD_WIN_SECEDE]->Render();
	m_textCtl[IT_GUILD_WIN_SCHEDULE]->Render();
	m_textCtl[IT_GUILD_WIN_AUTHORITY]->Render();
	m_textCtl[IT_GUILD_WIN_CLOSE]->Render();
}


int CIFGuildWin::GetRankInGuild( int idx )
{
	int maxMasterIdx = MAX_GUILD_MASTERS;
	int maxSubMasterIdx = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS;
	int maxFactionMasterIdx = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS;
	int maxMateIdx = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;

	if( idx == 0 )
		return 0;

	else if( idx >= maxMasterIdx && idx < maxSubMasterIdx )
		return 1;

	else if( idx >= maxSubMasterIdx && idx < maxFactionMasterIdx )
		return 2;

	else if( idx >= maxFactionMasterIdx && idx < maxMateIdx )
		return 3;

	else
		return -1;
}


void CIFGuildWin::PutContentText()
{
	int i, j, idx, worldIdx;
	char buffer[256];

	m_scheduleBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	
	if( g_cgv.myCharacterInfo->guildIdx == -1 )
	{
		m_secedeBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		m_authorityBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		return;
	}

	m_secedeBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	m_authorityBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

	
	sprintf( buffer, "%d", g_cgv.myCharacterInfo->guild.famePoint );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_FAMEPOINT]->GetXPos(), m_textCtl[IT_GUILD_WIN_FAMEPOINT]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_FAMEPOINT]->GetXSize(), 13, CHAT_ARRANGE_RIGHT );

	
	sprintf( buffer, "%d", 0 );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_RANK]->GetXPos(), m_textCtl[IT_GUILD_WIN_RANK]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_RANK]->GetXSize(), 13, CHAT_ARRANGE_RIGHT );
	
	
	sprintf( buffer, "%d", g_cgv.myCharacterInfo->guild.expPoint );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_EXPPOINT]->GetXPos(), m_textCtl[IT_GUILD_WIN_EXPPOINT]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_EXPPOINT]->GetXSize(), 13, CHAT_ARRANGE_RIGHT );
	
	
	sprintf( buffer, "%d", g_cgv.myCharacterInfo->guild.raidPoint );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_RAIDRPOINT]->GetXPos(), m_textCtl[IT_GUILD_WIN_RAIDRPOINT]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_RAIDRPOINT]->GetXSize(), 13, CHAT_ARRANGE_RIGHT );
	
	
	sprintf( buffer, "%d", g_cgv.myCharacterInfo->guild.hunterPoint );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_HUNTERPOINT]->GetXPos(), m_textCtl[IT_GUILD_WIN_HUNTERPOINT]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_HUNTERPOINT]->GetXSize(), 13, CHAT_ARRANGE_RIGHT );
	
	
	sprintf( buffer, "%d", g_cgv.myCharacterInfo->guild.pvpPoint );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_PVPPOINT]->GetXPos(), m_textCtl[IT_GUILD_WIN_PVPPOINT]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_PVPPOINT]->GetXSize(), 13, CHAT_ARRANGE_RIGHT );

	
	sprintf( buffer, "%s", g_cgv.myCharacterInfo->guild.notice );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_GUILDSCHEDULE]->GetXPos(), m_textCtl[IT_GUILD_WIN_GUILDSCHEDULE]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_GUILDSCHEDULE]->GetXSize(), 13 );

	for( i=1, j=0; i<=MAX_SCROLL_LINE1 && j<m_onlineMemberNumber; i++, j++ )
	{
		idx = j + m_scrollBar1->m_current;

		if( m_selectedIdx[0] == idx )
		{
			SetSelectBar( i - 1, 0 );
		}

		sprintf( buffer, "%s", m_onlineMember[idx].userName );
		g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_NAMETITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_ONLINETITLE]->GetYPos()+i*SCROLL_LINE_HEIGHT + 17, 
			buffer, m_textCtl[IT_GUILD_WIN_NAMETITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );
	
		sprintf( buffer, "Lv. %d", m_onlineMember[idx].userLevel );
		g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_LEVELETITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_ONLINETITLE]->GetYPos()+i*SCROLL_LINE_HEIGHT + 17, 
			buffer, m_textCtl[IT_GUILD_WIN_LEVELETITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

		sprintf( buffer, "%s", g_LPACK.GetMessage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, m_onlineMember[idx].userJob ) );
		g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_JOBTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_ONLINETITLE]->GetYPos()+i*SCROLL_LINE_HEIGHT + 17, 
			buffer, m_textCtl[IT_GUILD_WIN_JOBTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

		worldIdx = m_onlineMember[idx].userLocation;
		
		sprintf( buffer, "%s", g_cgv.worldTable[worldIdx].hanName );
		g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_LOCATIONTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_ONLINETITLE]->GetYPos()+i*SCROLL_LINE_HEIGHT + 17, 
			buffer, m_textCtl[IT_GUILD_WIN_LOCATIONTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

		sprintf( buffer, "%s", g_LPACK.GetMessage( 0, 819+m_onlineMember[idx].rank ) );
		g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_RANKTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_ONLINETITLE]->GetYPos()+i*SCROLL_LINE_HEIGHT + 17, 
			buffer, m_textCtl[IT_GUILD_WIN_RANKTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );
		
		
		if( m_onlineMember[idx].rank >= 0 && m_onlineMember[idx].rank < 3 )
		{
			if( !stricmp( g_cgv.myCharacterInfo->name, m_onlineMember[idx].userName ) )
				m_scheduleBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		}
	}

	for( i=1, j=0; i<=MAX_SCROLL_LINE2 && j<m_offlineMemberNumber; i++, j++ )
	{
		idx = j + m_scrollBar2->m_current;

		if( m_selectedIdx[1] == idx )
		{
			SetSelectBar( i - 1, 1 );
		}

		sprintf( buffer, "%s", m_offlineMember[idx].userName );
		g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_NAMETITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYPos()+i*SCROLL_LINE_HEIGHT+17, 
			buffer, m_textCtl[IT_GUILD_WIN_NAMETITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

		sprintf( buffer, "X" );
		g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_LEVELETITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYPos()+i*SCROLL_LINE_HEIGHT+17, 
			buffer, m_textCtl[IT_GUILD_WIN_LEVELETITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

		sprintf( buffer, "X" );
		g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_JOBTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYPos()+i*SCROLL_LINE_HEIGHT+17, 
			buffer, m_textCtl[IT_GUILD_WIN_JOBTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

		sprintf( buffer, g_LPACK.GetMessage(0,937) );	
		g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_LOCATIONTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYPos()+i*SCROLL_LINE_HEIGHT+17, 
			buffer, m_textCtl[IT_GUILD_WIN_LOCATIONTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

		sprintf( buffer, "%s", g_LPACK.GetMessage( 0, 819+m_offlineMember[idx].rank ) );
		g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_RANKTITLE]->GetXPos(), m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYPos()+i*SCROLL_LINE_HEIGHT+17, 
			buffer, m_textCtl[IT_GUILD_WIN_RANKTITLE]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	}

	sprintf( buffer, g_LPACK.GetMessage( 0, 816 ), m_onlineMemberNumber );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_ONLINECOUNT]->GetXPos(), m_textCtl[IT_GUILD_WIN_ONLINECOUNT]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_ONLINECOUNT]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	sprintf( buffer, g_LPACK.GetMessage( 0, 817 ), m_offlineMemberNumber );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_OFFLINECOUNT]->GetXPos(), m_textCtl[IT_GUILD_WIN_OFFLINECOUNT]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_OFFLINECOUNT]->GetXSize(), 13, CHAT_ARRANGE_CENTER );

	sprintf( buffer, g_LPACK.GetMessage( 0, 818 ), m_onlineMemberNumber + m_offlineMemberNumber );
	g_chatManager->DrawText( m_textCtl[IT_GUILD_WIN_TOTALCOUNT]->GetXPos(), m_textCtl[IT_GUILD_WIN_TOTALCOUNT]->GetYPos(), 
		buffer, m_textCtl[IT_GUILD_WIN_TOTALCOUNT]->GetXSize(), 13, CHAT_ARRANGE_CENTER );
}

void CIFGuildWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();
	
	
	m_titleBar->Render();
	m_titleBarBottom->Render();

	
	m_closeBtn->Render();

	m_countLabelBar->Render();

	m_scrollBar1->Render();
	m_scrollBar2->Render();

	m_secedeBtn->Render();
	m_scheduleBtn->Render();
	m_authorityBtn->Render();
	m_close2Btn->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_FRIEND_WIN_TITLE]->Render();

		g_chatManager->SetColor( 0xffffffff );

		PutLabelText();
		PutContentText();

		g_chatManager->RenderInputFrame( INPUT_INDEX_GUILD_SCHEDULE );
	}
	g_chatManager->EndFrame();

}

int CIFGuildWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFGuildWin::WindowProcess()
{
	char msgBuffer[256];
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_chatManager->InitInputStr( INPUT_INDEX_GUILD_SCHEDULE );
		g_ifMng->m_guildWin->m_scheduleBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

		g_chatManager->SetInputState( INPUT_INDEX_GUILD_SCHEDULE, -1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );

		Enable( false );

		return IF_CTL_SELECT;
	}

	if( m_scrollBar1->ProcessScrollBar() == IF_CTL_SELECT )
	{
		return IF_CTL_SELECT;
	}

	if( m_scrollBar2->ProcessScrollBar() == IF_CTL_SELECT )
	{
		return IF_CTL_SELECT;
	}

	
	if( m_secedeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		
		
		g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322), 
			g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 660)
			,MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_SECEDE, 10000.0f );

		return IF_CTL_SELECT;
	}

	if( m_scheduleBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_chatManager->SetInputState( INPUT_INDEX_GUILD_SCHEDULE, 1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_GUILD_SCHEDULE );

		m_scheduleBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

		return IF_CTL_SELECT;
	}

	if( m_authorityBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( g_ifMng->m_guildAuthWin->ToggleState() )
		{
			g_ifMng->SetFocus( g_ifMng->m_guildAuthWin );
		}

		return IF_CTL_SELECT;
	}

	
	if( m_close2Btn->ProcessButton() == IF_CTL_SELECT )
	{
		g_chatManager->InitInputStr( INPUT_INDEX_GUILD_SCHEDULE );
		g_ifMng->m_guildWin->m_scheduleBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

		g_chatManager->SetInputState( INPUT_INDEX_GUILD_SCHEDULE, -1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );

		Enable( false );

		return IF_CTL_SELECT;
	}

	
	
	
	
	
	int mx, my, x1, y1, xs, ys;
	int seletedIdx, listNum;
	
	if( m_onlineMemberNumber < MAX_SCROLL_LINE1 )
		listNum = m_onlineMemberNumber;
	else
		listNum = MAX_SCROLL_LINE1;

	mx = g_input->GetPosX();
	my = g_input->GetPosY();
	x1 = m_textCtl[IT_GUILD_WIN_NAMETITLE]->GetXPos();
	y1 = m_textCtl[IT_GUILD_WIN_ONLINETITLE]->GetYPos() + SCROLL_LINE_HEIGHT + 17;
	xs = 495;
	ys = listNum * SCROLL_LINE_HEIGHT;

	mx = mx - x1;
	my = my - y1;
	
	if( mx < 0 || my < 0 || mx > xs || my > ys ) 
		seletedIdx = -1;
	else
		seletedIdx = my / SCROLL_LINE_HEIGHT;

	if( seletedIdx >= 0 && seletedIdx < listNum )
	{
		if( g_input->GetLBUp() ) 
		{
			m_selectedIdx[0] = seletedIdx + m_scrollBar1->m_current;
			m_selectedIdx[1] = -1;
		}
	}
	else
	{

		
		if( m_offlineMemberNumber < MAX_SCROLL_LINE2 )
			listNum = m_offlineMemberNumber;
		else
			listNum = MAX_SCROLL_LINE2;

		mx = g_input->GetPosX();
		my = g_input->GetPosY();
		x1 = m_textCtl[IT_GUILD_WIN_NAMETITLE]->GetXPos();
		y1 = m_textCtl[IT_GUILD_WIN_OFFLINETITLE]->GetYPos() + SCROLL_LINE_HEIGHT + 17;
		xs = 495;
		ys = listNum * SCROLL_LINE_HEIGHT;

		mx = mx - x1;
		my = my - y1;
		
		if( mx < 0 || my < 0 || mx > xs || my > ys ) 
			seletedIdx = -1;
		else
			seletedIdx = my / SCROLL_LINE_HEIGHT;

		if( seletedIdx >= 0 && seletedIdx < listNum )
		{
			if( g_input->GetLBUp() ) 
			{
				m_selectedIdx[1] = seletedIdx + m_scrollBar2->m_current;
				m_selectedIdx[0] = -1;
			}
		}
		else
		{
			if( g_input->GetLBUp() )
			{
				m_selectedIdx[0] = -1;
				m_selectedIdx[1] = -1;
			}
		}

	}

	return IF_CTL_NONE;
}

void CIFGuildWin::UpdateMemberList()
{
	int onIdx = 0, offIdx = 0;
	int max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;

	memset( &m_onlineMember[0], 0, sizeof( m_onlineMember ) );
	memset( &m_offlineMember[0], 0, sizeof( m_offlineMember ) );

	for( int i=0; i<max; i++ )
	{
		if( g_cgv.myCharacterInfo->guild.userName[i][0] != 0 )
		{
			if( g_cgv.myCharacterInfo->guild.userLocation[i] != -1 )
			{
				strcpy( m_onlineMember[onIdx].userName, g_cgv.myCharacterInfo->guild.userName[i] );
				m_onlineMember[onIdx].userLevel = g_cgv.myCharacterInfo->guild.userLevel[i];
				m_onlineMember[onIdx].userLocation = g_cgv.myCharacterInfo->guild.userLocation[i];
				m_onlineMember[onIdx].userJob = g_cgv.myCharacterInfo->guild.userJob[i];
				m_onlineMember[onIdx].rank = GetRankInGuild(i);

				onIdx ++;
			}
			else
			{
				strcpy( m_offlineMember[offIdx].userName, g_cgv.myCharacterInfo->guild.userName[i] );
				m_offlineMember[offIdx].userLevel = g_cgv.myCharacterInfo->guild.userLevel[i];
				m_offlineMember[offIdx].userLocation = g_cgv.myCharacterInfo->guild.userLocation[i];
				m_offlineMember[offIdx].userJob = g_cgv.myCharacterInfo->guild.userJob[i];
				m_offlineMember[offIdx].rank = GetRankInGuild(i);
				
				offIdx ++;
			}
		}
	}

	m_onlineMemberNumber = onIdx;
	m_offlineMemberNumber = offIdx;
}

void CIFGuildWin::SetSelectBar( int idx, int base )
{
	m_selectBar->Enable();
	g_ifMng->m_iPos[IF_GUILD_WIN_SELECT_BAR].clientY = m_selectBarSavePos[base] + idx * SCROLL_LINE_HEIGHT;
	m_selectBar->UpdateControl();
	m_selectBar->UpdateBuffer();
	m_selectBar->Render();
}

int CIFGuildWin::GetSelectecIdx()
{
	int ret;
	GuildMemberList_t *selectedMember;

	if( m_selectedIdx[0] != -1 )
	{
		selectedMember = &m_onlineMember[m_selectedIdx[0]];
	}
	else if( m_selectedIdx[1] != -1 )
	{
		selectedMember = &m_offlineMember[m_selectedIdx[1]];
	}
	else
	{
		return -1;
	}

	int max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;

	for( int i=0; i<max; i++ )
	{
		if( !stricmp( g_cgv.myCharacterInfo->guild.userName[i], selectedMember->userName ) )
		{
			ret = i;
			break;
		}
	}

	if( i == max )
		return -1;

	return ret;
}




CIFGuildAuthWin::CIFGuildAuthWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_GUILDAUTH_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	
	m_titleBar		= new CIFControl( this );
	m_exitBtn		= new CIFButtonCtl( this );

	m_inviteBtn		= new CIFButtonCtl( this );
	m_danDisBtn		= new CIFButtonCtl( this );
	m_banishBtn		= new CIFButtonCtl( this );
	m_depriveBtn	= new CIFButtonCtl( this );
	m_bloodReqBtn	= new CIFButtonCtl( this );
	m_bloodDisBtn	= new CIFButtonCtl( this );
	m_bloodBatBtn	= new CIFButtonCtl( this );
	m_pvpReqBtn		= new CIFButtonCtl( this );
	m_setModeBtn	= new CIFButtonCtl( this );
	m_setMasterBtn	= new CIFButtonCtl( this );

	memset( m_targetName, 0, sizeof( m_targetName ) );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_GUILDAUTH_WINDOW_TYPE );
}

CIFGuildAuthWin::~CIFGuildAuthWin()
{
}

void CIFGuildAuthWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_GUILDAUTH_WIN );

#ifdef _GTH_KOREAN_VERSION
	
	
	m_textCtl[IT_GUILDAUTH_WIN_SUBSCRIBE]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL0 );
	m_textCtl[IT_GUILDAUTH_WIN_DANDIS]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL1 );
	m_textCtl[IT_GUILDAUTH_WIN_BANISH]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL2 );
	m_textCtl[IT_GUILDAUTH_WIN_DEPRIVE]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL3 );
	m_textCtl[IT_GUILDAUTH_WIN_BLOODREQ]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL4 );
	m_textCtl[IT_GUILDAUTH_WIN_BLOODDIS]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL5 );
	m_textCtl[IT_GUILDAUTH_WIN_BLOODBAT]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL6, -15.0f );
	m_textCtl[IT_GUILDAUTH_WIN_PVPREQ]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL7, -9.0f );
	m_textCtl[IT_GUILDAUTH_WIN_SETMODE]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL8, -12.0f );
	m_textCtl[IT_GUILDAUTH_WIN_SETMASTER]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL9 );
#endif


#if defined (_GTH_CHINESE_VERSION) || (_GTH_MALAYSIA_CHN_VERSION ) 
	m_textCtl[IT_GUILDAUTH_WIN_SUBSCRIBE]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL0, -6.0f );
	m_textCtl[IT_GUILDAUTH_WIN_DANDIS]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL1, -6.0f );
	m_textCtl[IT_GUILDAUTH_WIN_BANISH]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL2 );
	m_textCtl[IT_GUILDAUTH_WIN_DEPRIVE]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL3 );
	m_textCtl[IT_GUILDAUTH_WIN_BLOODREQ]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL4 );
	m_textCtl[IT_GUILDAUTH_WIN_BLOODDIS]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL5 );
	m_textCtl[IT_GUILDAUTH_WIN_BLOODBAT]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL6, -6.0f );
	m_textCtl[IT_GUILDAUTH_WIN_PVPREQ]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL7, -15.0f );
	m_textCtl[IT_GUILDAUTH_WIN_SETMODE]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL8, -16.0f );
	m_textCtl[IT_GUILDAUTH_WIN_SETMASTER]->PrepareControl( IF_GUILDAUTH_WIN_BTN_LABEL9 );
	
#endif
	
	
	m_titleBar->PrepareControl( IF_GUILDAUTH_WIN_TITLE_BAR );
	
	
	m_exitBtn->PrepareControl( IF_GUILDAUTH_WIN_EXIT_BTN_0 );
	m_inviteBtn->PrepareControl( IF_GUILDAUTH_WIN_BTN0_0 );
	m_danDisBtn->PrepareControl( IF_GUILDAUTH_WIN_BTN1_0 );
	m_banishBtn->PrepareControl( IF_GUILDAUTH_WIN_BTN2_0 );
	m_depriveBtn->PrepareControl( IF_GUILDAUTH_WIN_BTN3_0 );
	m_bloodReqBtn->PrepareControl( IF_GUILDAUTH_WIN_BTN4_0 );
	m_bloodDisBtn->PrepareControl( IF_GUILDAUTH_WIN_BTN5_0 );
	m_bloodBatBtn->PrepareControl( IF_GUILDAUTH_WIN_BTN6_0 );
	m_pvpReqBtn->PrepareControl( IF_GUILDAUTH_WIN_BTN7_0 );
	m_setModeBtn->PrepareControl( IF_GUILDAUTH_WIN_BTN8_0 );
	m_setMasterBtn->PrepareControl( IF_GUILDAUTH_WIN_BTN9_0 );

}

int CIFGuildAuthWin::WindowFrame()
{
	
	m_textCtl[IT_GUILDAUTH_WIN_SUBSCRIBE]->SetText( g_LPACK.GetMessage(9,5) );
	m_textCtl[IT_GUILDAUTH_WIN_DANDIS]->SetText( g_LPACK.GetMessage(9,6) );
	m_textCtl[IT_GUILDAUTH_WIN_BANISH]->SetText( g_LPACK.GetMessage(9,7) );
	m_textCtl[IT_GUILDAUTH_WIN_SETMASTER]->SetText( g_LPACK.GetMessage(9,8) );
	m_textCtl[IT_GUILDAUTH_WIN_BLOODREQ]->SetText( g_LPACK.GetMessage(9,9) );
	m_textCtl[IT_GUILDAUTH_WIN_BLOODDIS]->SetText( g_LPACK.GetMessage(9,10) );
	m_textCtl[IT_GUILDAUTH_WIN_BLOODBAT]->SetText( g_LPACK.GetMessage(9,11) );
	m_textCtl[IT_GUILDAUTH_WIN_PVPREQ]->SetText( g_LPACK.GetMessage(9,12) );
	m_textCtl[IT_GUILDAUTH_WIN_SETMODE]->SetText( g_LPACK.GetMessage(9,13) );
	m_textCtl[IT_GUILDAUTH_WIN_DEPRIVE]->SetText( g_LPACK.GetMessage(9,14) );

	int myAuth = GetMyGuildAuthority();

	SetAllButtonDisable();

	switch( myAuth )
	{
	
	case 0:
		m_inviteBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_danDisBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_banishBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_depriveBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_setMasterBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

		



		m_bloodBatBtn->SetButtonState( IF_BTN_TYPE_ENABLE );


		break;
	
	case 1:
		m_inviteBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_banishBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		



		m_bloodBatBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

		break;
	
	case 2:
		m_inviteBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		



		break;
	
	case 3:

		break;
	}
	
	return( IF_CTL_NONE );
}

void CIFGuildAuthWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();
	
	
	m_titleBar->Render();

	
	m_exitBtn->Render();
	m_inviteBtn->Render();
	m_danDisBtn->Render();
	m_banishBtn->Render();
	m_setMasterBtn->Render();
	m_bloodReqBtn->Render();
	m_bloodDisBtn->Render();
	m_bloodBatBtn->Render();
	m_pvpReqBtn->Render();
	m_setModeBtn->Render();
	m_depriveBtn->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->SetColor( 0xffffffff );

	g_chatManager->BeginFrame();
	{
		for( int i=0; i<10; i++ )
			m_textCtl[IT_GUILDAUTH_WIN_SUBSCRIBE+i]->Render();
	}
	g_chatManager->EndFrame();

}

int CIFGuildAuthWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFGuildAuthWin::WindowProcess()
{
	char szTemp[255];
	char name[128];

	if( m_exitBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_SELECT;
	}
	
	
	if( m_inviteBtn->ProcessButton() == IF_CTL_SELECT )
	{
		GetTargetName( name );

		if( name[0] != '\0' )
		{
			GTH_SendMessage_Guild_Invite_Request( name );
		}
		else
		{
			
			strcpy( szTemp, g_LPACK.GetMessage(9,15) );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp );
		}

		return IF_CTL_SELECT;
	}

	
	if( m_danDisBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		
		
		g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322), g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 662)
			, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_DISORGANIZE1, 10000.0f );
	
		return IF_CTL_SELECT;
	}

	
	if( m_banishBtn->ProcessButton() == IF_CTL_SELECT )
	{
		int idx = g_ifMng->m_guildWin->GetSelectecIdx();

		if( idx != -1 )
			strcpy( name, g_cgv.myCharacterInfo->guild.userName[idx] );
		else
			name[0] = 0;

		if( name[0] != '\0' )
		{
			if( g_cgv.myCharacterInfo->guildIdx != -1 )
			{
				strcpy( g_ifMng->m_tempString, name );

				
				sprintf( szTemp, g_LPACK.GetMessage(0,862), name );
				g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_BANISH, 10000.0f );
			}
		}
		else
		{
			
			strcpy( szTemp, g_LPACK.GetMessage(9,16) );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp );
		}

		return IF_CTL_SELECT;
	}

	
	if( m_depriveBtn->ProcessButton() == IF_CTL_SELECT )
	{
		int idx = g_ifMng->m_guildWin->GetSelectecIdx();

		if( idx != -1 )
			strcpy( name, g_cgv.myCharacterInfo->guild.userName[idx] );
		else
			name[0] = 0;

		if( name[0] != '\0' )
		{
			if( g_cgv.myCharacterInfo->guildIdx != -1 )
			{
				strcpy( g_ifMng->m_tempString, name );

				
				sprintf( szTemp, g_LPACK.GetMessage(0,918), name );
				g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_DELETEMASTER, 10000.0f );
			}
		}
		else
		{
			
			strcpy( szTemp, g_LPACK.GetMessage(9,19) );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp );
		}

		return IF_CTL_SELECT;
	}


	if( m_bloodReqBtn->ProcessButton() == IF_CTL_SELECT )
	{
		return IF_CTL_SELECT;
	}


	
	if( m_bloodDisBtn->ProcessButton() == IF_CTL_SELECT )
	{
		return IF_CTL_SELECT;
	}
	if( m_pvpReqBtn->ProcessButton() == IF_CTL_SELECT )
	{
		return IF_CTL_SELECT;
	}
	if( m_bloodBatBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		g_ifMng->m_guildWin->Enable(FALSE);
		g_ifMng->m_guildAuthWin->Enable(FALSE);
		GTH_SendMessage_Request_NewDanBattleWinOpen();
		return IF_CTL_SELECT;
	}
	if( m_setModeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		return IF_CTL_SELECT;
	}

	if( m_setMasterBtn->ProcessButton() == IF_CTL_SELECT )
	{
		int idx = g_ifMng->m_guildWin->GetSelectecIdx();

		if( idx != -1 )
			strcpy( name, g_cgv.myCharacterInfo->guild.userName[idx] );
		else
			name[0] = 0;

		if( name[0] != '\0' )
		{
			if( g_cgv.myCharacterInfo->guildIdx != -1 )
			{
				strcpy( g_ifMng->m_tempString, name );

				
				
				sprintf( szTemp, g_LPACK.GetMessage(9,26), name );
				g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_CHANGE_MASTER1, 10000.0f );
			}
		}
		else
		{
			
			strcpy( szTemp, g_LPACK.GetMessage(9,27) );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp );
		}



		return IF_CTL_SELECT;
	}

	return IF_CTL_NONE;
}

void CIFGuildAuthWin::SetAllButtonDisable()
{
	
	m_inviteBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	m_danDisBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	m_banishBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	m_depriveBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	
	m_bloodReqBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	m_bloodDisBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	m_pvpReqBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	m_bloodBatBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	m_setMasterBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	m_setModeBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
}

int CIFGuildAuthWin::GetMyGuildAuthority()
{
	int maxMember = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;
	for( int i=0; i<maxMember; i++ )
	{
		if( !stricmp( g_cgv.myCharacterInfo->name, g_cgv.myCharacterInfo->guild.userName[i] ) )
			return g_ifMng->m_guildWin->GetRankInGuild( i );
	}
	return -1;
}

void CIFGuildAuthWin::GetTargetName( char name[] )
{
	name[0] = 0;

	if( g_pApp->m_myCharacter->targetType !=  ENTITY_PC ) return;
	if( g_pApp->m_myCharacter->targetIdx == g_cgv.myPCIdxOnServer ) return;

	if( g_cgv.pTargetChar != NULL )
	{
		strcpy( name, g_cgv.pTargetChar->name );
	}
}






CIFGuildPvPWin::CIFGuildPvPWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i=0; i<NUM_IT_GUILDPVP_WIN; i++ )
		m_textCtl[i] = new CIFText( this );

	m_titleBar			= new CIFControl( this );
	m_titleBarBottom	= new CIFControl( this );

	m_closeBtn			= new CIFButtonCtl( this );
	m_enterBtnA			= new CIFButtonCtl( this );
	m_enterBtnB			= new CIFButtonCtl( this );
	m_cancelBtn			= new CIFButtonCtl( this );
	
	m_timeSelectBtn		= new CIFSpinCtl( this );

	m_scrollBar			= new CIFScrollBar( this );
	
	Enable( false );
	g_ifMng->AddFocus( this );
	SetWindowType( IF_GUILDPVP_WINDOW_TYPE );
}

CIFGuildPvPWin::~CIFGuildPvPWin()
{
	
}

void CIFGuildPvPWin::PrepareControl()
{

}

void CIFGuildPvPWin::Render()
{

}

int CIFGuildPvPWin::WindowFrame()
{

	return IF_CTL_NONE;
}

int CIFGuildPvPWin::WindowProcess()
{

	return IF_CTL_NONE;
}

int CIFGuildPvPWin::WindowSelection()
{

	return IF_CTL_NONE;
}







CIFGuildArenaWin::CIFGuildArenaWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i=0; i<NUM_IT_GUILDARENA_WIN; i++ )
		m_textCtl[i] = new CIFText( this );





















	m_title				= new CIFControl( this );	
	m_title_bar			= new CIFControl( this );	
	m_scrollBarStandby	= new CIFScrollBar( this );
	m_requestbtn		= new CIFButtonCtl( this );
	m_startBtn			= new CIFButtonCtl( this );
	m_cancelBtn			= new CIFButtonCtl( this );
	m_closeBtn			= new CIFButtonCtl( this );

	m_selectStandbyIdx = 1;	
	m_StandByCount	= 0;

	
	m_RegAteamIdx = 0;
	m_RegBteamIdx = -1;
	
	

	Enable( false );
	m_requestbtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	g_ifMng->AddFocus( this );
	m_ReadyBattleFlag	= STATE_NONE;
	
	ClearStandbyList();
	
	SetWindowType( IF_GUILDARENA_WINDOW_TYPE );
}

CIFGuildArenaWin::~CIFGuildArenaWin()
{

}

void CIFGuildArenaWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_GUILD_WAR_WIN );

	m_title_bar->PrepareControl(IF_GUILD_WAR_WIN_BAR);	
	m_textCtl[IT_GUILD_WAR_WIN_TITLE_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_TITLE_TEXT);	
	m_textCtl[IT_GUILD_WAR_WIN_INFO_TITLE_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_INFO_TITLE_TEXT);	
	m_textCtl[IT_GUILD_WAR_WIN_ATEAMTEXT]->PrepareControl(IF_GUILD_WAR_WIN_ATEAMTEXT);
	m_textCtl[IT_GUILD_WAR_WIN_BTEAMTEXT]->PrepareControl(IF_GUILD_WAR_WIN_BTEAMTEXT);

	m_textCtl[IT_GUILD_WAR_WIN_ATEAM_NAMETEXT]->PrepareControl(IF_GUILD_WAR_WIN_ATEAM_NAMETEXT);
	m_textCtl[IT_GUILD_WAR_WIN_BTEAM_NAMETEXT]->PrepareControl(IF_GUILD_WAR_WIN_BTEAM_NAMETEXT);	
	m_textCtl[IT_GUILD_WAR_WIN_ATEAM_INFOTEXT]->PrepareControl(IF_GUILD_WAR_WIN_ATEAM_INFOTEXT);
	m_textCtl[IT_GUILD_WAR_WIN_BTEAM_INFOTEXT]->PrepareControl(IF_GUILD_WAR_WIN_BTEAM_INFOTEXT);
	
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_TITLE_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_TITLE_TEXT);	
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST1_TEXT); 
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST2_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST2_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST3_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST3_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST4_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST4_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST5_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST5_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST6_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST6_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST7_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST7_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST8_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST8_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST9_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST9_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST10_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST10_TEXT);	
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST11_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST11_TEXT); 
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST12_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST12_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST13_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST13_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST14_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST14_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST15_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST15_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST16_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST16_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST17_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST17_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST18_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST18_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST19_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST19_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST20_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST20_TEXT);

	
	
	
	m_requestbtn->PrepareControl( IF_GUILD_WAR_WIN_REQUEST_BTN0_0 );
	m_startBtn->PrepareControl( IF_GUILD_WAR_WIN_START_BTN0_0 );
	m_cancelBtn->PrepareControl( IF_GUILD_WAR_WIN_CANCAL_BTN0_0 );
	m_closeBtn->PrepareControl(IF_GUILD_WAR_WIN_CLOSE_BTN_0);
	
}

void CIFGuildArenaWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();	
	
	m_requestbtn->RenderTextBtn();
	m_startBtn->RenderTextBtn();
	m_cancelBtn->RenderTextBtn();
	m_closeBtn->Render();

	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{








		
		for (int i = IT_GUILD_WAR_WIN_TITLE_TEXT; i < NUM_IT_GUILDARENA_WIN; i ++)		
			m_textCtl[i]->Render();

	}
	g_chatManager->EndFrame();

}

int CIFGuildArenaWin::WindowFrame()
{

	
	char szTemp[256];	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 540 ) );

	
	sprintf( szTemp, g_LPACK.GetMassage( 9, 44 ) );
	m_textCtl[IT_GUILD_WAR_WIN_TITLE_TEXT]->SetText(szTemp);
	
	sprintf( szTemp, g_LPACK.GetMassage( 9, 45 ) );
	m_textCtl[IT_GUILD_WAR_WIN_INFO_TITLE_TEXT]->SetText(szTemp);
	
	sprintf( szTemp, g_LPACK.GetMassage( 9, 46 ) );
	m_textCtl[IT_GUILD_WAR_WIN_ATEAMTEXT]->SetText(szTemp);
	
	sprintf( szTemp, g_LPACK.GetMassage( 9, 47 ) );
	m_textCtl[IT_GUILD_WAR_WIN_BTEAMTEXT]->SetText(szTemp);

	




	if ( m_RegAteamIdx  >= 0 )
		m_textCtl[IT_GUILD_WAR_WIN_ATEAM_NAMETEXT]->SetText(m_szStandbyList[m_RegAteamIdx]);
	else
		m_textCtl[IT_GUILD_WAR_WIN_ATEAM_NAMETEXT]->SetText("");	

	if ( m_RegBteamIdx  >= 0 )
		m_textCtl[IT_GUILD_WAR_WIN_BTEAM_NAMETEXT]->SetText(m_szStandbyList[m_RegBteamIdx]);
	else
		m_textCtl[IT_GUILD_WAR_WIN_BTEAM_NAMETEXT]->SetText("");	

	



	
	sprintf( szTemp, g_LPACK.GetMassage( 9, 48 ) );	
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_TITLE_TEXT]->SetText(szTemp);


	
	int RegCount =0;
	for (int i = 0; i < MAX_STANDBYCOUNT ; i ++)
	{		
		

		if ( m_RegBteamIdx == i || m_RegAteamIdx == i || m_szStandby_danIdx_List[i] < 0)
		{
			m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+RegCount]->SetText("");			
			continue;
		}
		sprintf( szTemp, g_LPACK.GetMassage( 9, 49 ),m_szStandbyList[i]);
		m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+RegCount]->SetText(szTemp);		
		m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+RegCount]->SetColor(0xff808080);
		RegCount++;
	}
	
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT + m_selectStandbyIdx - 1]->SetColor(0xffffffff);

	m_textCtl[IT_GUILD_WAR_WIN_TITLE_TEXT]->SetColor( 0xffffffff );
	m_textCtl[IT_GUILD_WAR_WIN_INFO_TITLE_TEXT]->SetColor( 0xffffffff );
	m_textCtl[IT_GUILD_WAR_WIN_ATEAMTEXT]->SetColor( 0xffffffff );	
	m_textCtl[IT_GUILD_WAR_WIN_BTEAMTEXT]->SetColor( 0xffffffff );
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_TITLE_TEXT]->SetColor( 0xffffffff );





	m_requestbtn->SetButtonText(g_LPACK.GetMassage( 9, 50 ));
	m_startBtn->SetButtonText(g_LPACK.GetMassage( 9,51 ));
	m_cancelBtn->SetButtonText(g_LPACK.GetMassage( 9, 52 ));	

	return IF_CTL_NONE;
}

int CIFGuildArenaWin::WindowProcess()
{	
	if( m_ReadyBattleFlag == STATE_REQUEST)	
	{
		
		m_requestbtn->SetButtonText(g_LPACK.GetMassage( 9, 50 ), 0xffffffff );
		m_requestbtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	}	
	else	
	{
		
		m_requestbtn->SetButtonText(g_LPACK.GetMassage( 9, 50 ), 0xffffff00 );	
		m_requestbtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}

	
	if( m_ReadyBattleFlag == STATE_READY )	
	{
		m_startBtn->SetButtonText(g_LPACK.GetMassage( 9, 51 ), 0xffffff00 );	
		m_startBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	}
	else
	{
		m_startBtn->SetButtonText(g_LPACK.GetMassage( 9, 51 ), 0xffffff00 );	
		m_startBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}
	
		

	return IF_CTL_NONE;
}

int CIFGuildArenaWin::WindowSelection()
{

	
	
	
	for (int i = 0; i < m_StandByCount;  i++)
	{
		if( m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT + i]->Selection() == IF_LBDN ) 	
		{
			m_selectStandbyIdx = i+1;
			return IF_CTL_SELECT;	
		}
	}	
	
	
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		GTH_SendMessage_RequestNpcChat( g_cgv.pTargetChar->idxOnServer );
		g_DanBattle.SendCancelRequest();

		
		m_RegAteamIdx = -1;
		m_RegBteamIdx = -1;
		
		
		return IF_CTL_SELECT;
	}

	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		GTH_SendMessage_RequestNpcChat( g_cgv.pTargetChar->idxOnServer );
		g_DanBattle.SendCancelRequest();
		
		m_RegBteamIdx = -1;
		
		return IF_CTL_SELECT;
	}

	
	if ( m_requestbtn->ProcessButton() == IF_CTL_SELECT)
	{		
		g_DanBattle.SendSuggestBattle(m_szStandby_danIdx_List[m_selectStandbyIdx],m_szStandbyList[m_selectStandbyIdx]);

		
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,53), MSG_BUTTONTYPE_CANCEL, IF_MESSAGE_DAN_BATTLE_SUGGEST_ACCEPT, 10000.0f );
		

		
		m_RegBteamIdx = m_selectStandbyIdx;
		
		return IF_CTL_SELECT;
	}
	
	if ( m_startBtn->ProcessButton() == IF_CTL_SELECT)
	{
		
		
		g_DanBattle.SendBattleReady();
		g_ifMng->m_guildArenaWin->Enable(FALSE);

		
		g_ifMng->m_guildArenaInfoWin->SetBattleTeams(g_ifMng->m_guildArenaWin->m_RegAteamIdx, g_ifMng->m_guildArenaWin->m_RegBteamIdx);
		
		
		
		
		
		
		return IF_CTL_SELECT;
	}		

	int ret;
	ret = m_title_bar->Selection();
	if( ret == IF_LBDN ) 
		return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) 
		return( IF_CTL_SELECT );

	return IF_CTL_NONE;
	
}

void CIFGuildArenaWin::ClearStandbyList()
{
	memset(m_szATeamList, NULL, sizeof ( char ) * NAMESTRING);
	memset(m_szBTeamList, NULL, sizeof ( char ) * NAMESTRING);
	
	for (int i = 0; i < MAX_STANDBYCOUNT; i ++)
	{
		memset(m_szStandbyList[i], NULL , sizeof (char) * NAMESTRING );
		m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+i]->SetText("");		
		m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+i]->SetColor(0xff808080);
		m_szStandby_danIdx_List[i] = -1;
	}

	m_ReadyBattleFlag = STATE_NONE;
	m_StandByCount = 0;
	m_selectStandbyIdx = 1;	


}

void CIFGuildArenaWin::ClearRegTeams()
{
	
	m_RegBteamIdx = -1;
}





CIFGonryunBattlePractice::CIFGonryunBattlePractice( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{


	for( int i=0; i<NUM_IT_GUILDARENA_WIN; i++ )
		m_textCtl[i] = new CIFText( this );





















	m_title				= new CIFControl( this );	
	m_title_bar			= new CIFControl( this );	
	m_scrollBarStandby	= new CIFScrollBar( this );
	m_requestbtn		= new CIFButtonCtl( this );
	m_startBtn			= new CIFButtonCtl( this );
	m_cancelBtn			= new CIFButtonCtl( this );
	m_closeBtn			= new CIFButtonCtl( this );

	m_selectStandbyIdx = 1;	
	m_StandByCount	= 0;

	
	m_RegAteamIdx = -1;
	m_RegBteamIdx = -1;
	
	m_isLeader=FALSE;
	m_currentUserCount=0;
	m_MyTextIdxinStandby=-1;
	Enable( false );
	m_requestbtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	g_ifMng->AddFocus( this );
	m_ReadyBattleFlag	= STATE_NONE;
	m_bbattleMode=FALSE;
	ClearStandbyList();
	
	SetWindowType( IF_GONRYUN_BATTLE_PRACTICE_WINDOW_TYPE);
}

CIFGonryunBattlePractice::~CIFGonryunBattlePractice()
{

}

void CIFGonryunBattlePractice::PrepareControl()
{
	CIFControl::PrepareControl( IF_GUILD_WAR_WIN );

	m_title_bar->PrepareControl(IF_GUILD_WAR_WIN_BAR);	
	m_textCtl[IT_GUILD_WAR_WIN_TITLE_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_TITLE_TEXT);	
	m_textCtl[IT_GUILD_WAR_WIN_INFO_TITLE_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_INFO_TITLE_TEXT);	
	m_textCtl[IT_GUILD_WAR_WIN_ATEAMTEXT]->PrepareControl(IF_GUILD_WAR_WIN_ATEAMTEXT);
	m_textCtl[IT_GUILD_WAR_WIN_BTEAMTEXT]->PrepareControl(IF_GUILD_WAR_WIN_BTEAMTEXT);

	m_textCtl[IT_GUILD_WAR_WIN_ATEAM_NAMETEXT]->PrepareControl(IF_GUILD_WAR_WIN_ATEAM_NAMETEXT);
	m_textCtl[IT_GUILD_WAR_WIN_BTEAM_NAMETEXT]->PrepareControl(IF_GUILD_WAR_WIN_BTEAM_NAMETEXT);	
	m_textCtl[IT_GUILD_WAR_WIN_ATEAM_INFOTEXT]->PrepareControl(IF_GUILD_WAR_WIN_ATEAM_INFOTEXT);
	m_textCtl[IT_GUILD_WAR_WIN_BTEAM_INFOTEXT]->PrepareControl(IF_GUILD_WAR_WIN_BTEAM_INFOTEXT);
	
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_TITLE_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_TITLE_TEXT);	
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST1_TEXT); 
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST2_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST2_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST3_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST3_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST4_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST4_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST5_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST5_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST6_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST6_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST7_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST7_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST8_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST8_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST9_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST9_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST10_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST10_TEXT);	
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST11_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST11_TEXT); 
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST12_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST12_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST13_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST13_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST14_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST14_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST15_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST15_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST16_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST16_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST17_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST17_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST18_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST18_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST19_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST19_TEXT);
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST20_TEXT]->PrepareControl(IF_GUILD_WAR_WIN_STANDBY_LIST20_TEXT);

	
	
	
	m_requestbtn->PrepareControl( IF_GUILD_WAR_WIN_REQUEST_BTN0_0 );
	m_startBtn->PrepareControl( IF_GUILD_WAR_WIN_START_BTN0_0 );
	m_cancelBtn->PrepareControl( IF_GUILD_WAR_WIN_CANCAL_BTN0_0 );
	m_closeBtn->PrepareControl(IF_GUILD_WAR_WIN_CLOSE_BTN_0);
	
}

void CIFGonryunBattlePractice::Render()
{

	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();	
	
	m_requestbtn->RenderTextBtn();
	m_startBtn->RenderTextBtn();
	m_cancelBtn->RenderTextBtn();
	m_closeBtn->Render();

	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		
		for (int i = IT_GUILD_WAR_WIN_TITLE_TEXT; i < NUM_IT_GUILDARENA_WIN; i ++)		
			m_textCtl[i]->Render();

	}
	g_chatManager->EndFrame();


}

int CIFGonryunBattlePractice::WindowFrame()
{

	
	char szTemp[256];	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 540 ));

	
	sprintf( szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 121 ));

	m_textCtl[IT_GUILD_WAR_WIN_INFO_TITLE_TEXT]->SetText(szTemp);
	
	sprintf( szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 131 ));

	m_textCtl[IT_GUILD_WAR_WIN_ATEAMTEXT]->SetText(szTemp);
	sprintf( szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 132 ));

	m_textCtl[IT_GUILD_WAR_WIN_BTEAMTEXT]->SetText(szTemp);

	
	if ( m_RegAteamIdx  >= 0	 )
	{
		m_textCtl[IT_GUILD_WAR_WIN_ATEAM_NAMETEXT]->SetText(m_szATeamList);
		memset(szTemp,0,256);
		sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 120 ) ,m_arrayUserInfo[m_RegAteamIdx].win,m_arrayUserInfo[m_RegAteamIdx].lose,
			m_arrayUserInfo[m_RegAteamIdx].draw);
			m_textCtl[IT_GUILD_WAR_WIN_ATEAM_INFOTEXT]->SetText(szTemp);
	}
	else
		m_textCtl[IT_GUILD_WAR_WIN_ATEAM_NAMETEXT]->SetText("");	

	
	if ( m_RegBteamIdx  >= 0 )
	{
		m_textCtl[IT_GUILD_WAR_WIN_BTEAM_NAMETEXT]->SetText(m_szStandbyList[m_RegBteamIdx]);
		memset(szTemp,0,256);
		sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 120 ) ,m_arrayUserInfo[m_RegBteamIdx].win,m_arrayUserInfo[m_RegBteamIdx].lose,
			m_arrayUserInfo[m_RegBteamIdx].draw);
			m_textCtl[IT_GUILD_WAR_WIN_BTEAM_INFOTEXT]->SetText(szTemp);
	}
	
	else
		m_textCtl[IT_GUILD_WAR_WIN_BTEAM_NAMETEXT]->SetText("");	


	
	
	sprintf( szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 48 ) );	
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_TITLE_TEXT]->SetText(szTemp);


	
	int RegCount =0;
	for (int i = 0; i < m_currentUserCount; i++)
	{		
	
		strcpy(szTemp,m_szStandbyList[i]);
		m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+RegCount]->SetText(szTemp);
		if(m_arrayUserInfo[m_MyTextIdxinStandby].level==0)continue;
		int differ=abs(m_arrayUserInfo[i].level-m_arrayUserInfo[m_MyTextIdxinStandby].level);
		if( differ > g_GonryunBattlePractic.m_iLevelDiffer){
			m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+RegCount]->SetColor(0xffff0000);
		}
		else m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+RegCount]->SetColor(0xff808080);
		
		RegCount++;
	}
	
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT + m_selectStandbyIdx]->SetColor(0xffffffff);

	m_textCtl[IT_GUILD_WAR_WIN_TITLE_TEXT]->SetColor( 0xffffffff );
	m_textCtl[IT_GUILD_WAR_WIN_INFO_TITLE_TEXT]->SetColor( 0xffffffff );
	m_textCtl[IT_GUILD_WAR_WIN_ATEAMTEXT]->SetColor( 0xffffffff );	
	m_textCtl[IT_GUILD_WAR_WIN_BTEAMTEXT]->SetColor( 0xffffffff );
	m_textCtl[IT_GUILD_WAR_WIN_STANDBY_TITLE_TEXT]->SetColor( 0xffffffff );
	
	m_requestbtn->SetButtonText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,127));

	
	m_startBtn->SetButtonText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,126));

	m_cancelBtn->SetButtonText("cancel");	

	return IF_CTL_NONE;
}

int CIFGonryunBattlePractice::WindowProcess()
{	
    
	if( m_currentUserCount >= 2)	
	{
		
		
		if(m_isLeader==TRUE)
		{
			m_requestbtn->SetButtonText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 50 ), 0xffffffff);
			m_requestbtn->SetButtonState( IF_BTN_TYPE_ENABLE );
			m_startBtn->SetButtonText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 51 ), 0xffffff00 );	
			m_startBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		}
		else
		{
			m_requestbtn->SetButtonText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 50 ), 0xffffffff );
			m_requestbtn->SetButtonState( IF_BTN_TYPE_DISABLE );
			m_startBtn->SetButtonText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 51 ), 0xffffff00 );	
			m_startBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		}
	
	}	
	else	
	{
		m_requestbtn->SetButtonText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 50 ), 0xffffffff );
		m_requestbtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		m_startBtn->SetButtonText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 51 ), 0xffffff00 );	
		m_startBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	
	}
	return IF_CTL_NONE;
}

int CIFGonryunBattlePractice::WindowSelection()
{
	
	for (int i = 1; i <MAX_STANDBYCOUNT;  i++)
	{
		if( m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT + i]->Selection() == IF_LBDN ) 	
		{
			
			m_selectStandbyIdx = i;
			m_RegBteamIdx = i;
			return IF_CTL_SELECT;	
		}
	}	
	
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );

        
		g_GonryunBattlePractic.SendCancelRequest();
		ClearStandbyList();
		return IF_CTL_SELECT;
	}

	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		g_GonryunBattlePractic.SendCancelRequest();
		ClearStandbyList();
		return IF_CTL_SELECT;
	}

	
	if ( m_requestbtn->ProcessButton() == IF_CTL_SELECT)
	{	
		
		
		char cz_batting[256+1];

		sprintf(cz_batting,g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,112),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,127));
				cz_batting[256]=NULL;
        int compare = strcmp(m_szStandbyList[m_selectStandbyIdx],g_pApp->m_myCharacter->name);
		if(0 == compare || m_arrayUserInfo[m_selectStandbyIdx].level == 0
			|| m_selectStandbyIdx <=0)	return IF_CTL_NONE;

		
		for(int i=1; i<MAX_STANDBYCOUNT; i++)
		{
			if(0 == strcmp(m_szStandbyList[i],m_szStandbyList[m_selectStandbyIdx]))
			{
				char selectUserName[NAMESTRING+1];
			
				int differ=abs(m_arrayUserInfo[m_selectStandbyIdx].level-m_arrayUserInfo[0].level);
				if(differ>g_GonryunBattlePractic.m_iLevelDiffer){
			
				  g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(9,111), 1, IF_MESSAGE_NONE );
				  return IF_CTL_NONE;
			  }
			
			

				strncpy(selectUserName,m_szStandbyList[m_selectStandbyIdx],NAMESTRING);
				selectUserName[NAMESTRING]=NULL;
				GTH_SendMessage_PCTrade_RequestBatting( selectUserName);

				g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), cz_batting, MSG_BUTTONTYPE_CANCEL,
					IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING, 30000.0f );
				m_RegBteamIdx = m_selectStandbyIdx;
				return IF_CTL_SELECT;
			}
			
		}
		return IF_CTL_NONE;
	
	}
	
	if ( m_startBtn->ProcessButton() == IF_CTL_SELECT)
	{
		char cz_generate[256+1];
	
		if(m_arrayUserInfo[m_selectStandbyIdx].level==0 || m_selectStandbyIdx<=0)
		{		
			return IF_CTL_NONE;
		}
		
		
		int differ=abs(m_arrayUserInfo[m_selectStandbyIdx].level-m_arrayUserInfo[0].level);
				if(differ>g_GonryunBattlePractic.m_iLevelDiffer)
				{
					g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(9,111), 1, IF_MESSAGE_NONE );
					return IF_CTL_NONE;
				}
		
		
		sprintf(cz_generate,g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,112),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,126));
		cz_generate[256]=NULL;
		int compare=strcmp(m_szStandbyList[m_selectStandbyIdx],g_pApp->m_myCharacter->name);
		if(0==compare) return IF_CTL_NONE;

		
		for(int i=1; i <= MAX_STANDBYCOUNT; i++)
		{
			compare=strcmp(m_szStandbyList[i],m_szStandbyList[m_selectStandbyIdx]);
			if(0==compare)
			{
				char selectUserName[NAMESTRING+1];
				strncpy(selectUserName,m_szStandbyList[m_selectStandbyIdx],NAMESTRING);
				selectUserName[NAMESTRING]=NULL;
				g_GonryunBattlePractic.RequestBattleGenerate(selectUserName);
				
				g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), cz_generate, MSG_BUTTONTYPE_CANCEL,
					IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_GENERATE, 30000.0f );
				m_RegBteamIdx = m_selectStandbyIdx;
				return IF_CTL_SELECT;
			}
			
		}
		return IF_CTL_NONE;
	}		

	int ret;
	ret = m_title_bar->Selection();
	if( ret == IF_LBDN ) 
		return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) 
		return( IF_CTL_SELECT );

	return IF_CTL_NONE;
	
}

void CIFGonryunBattlePractice::ClearStandbyList()
{

	memset(m_szATeamList, NULL, sizeof ( char ) * NAMESTRING);
	memset(m_szBTeamList, NULL, sizeof ( char ) * NAMESTRING);

	for (int i = 0; i < MAX_STANDBYCOUNT; i ++)
	{
		memset(m_szStandbyList[i], NULL , sizeof (char) * NAMESTRING );
		m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+i]->SetText("");		
		m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+i]->SetColor(0xff808080);
		m_arrayUserInfo[i].win=	m_arrayUserInfo[i].draw=m_arrayUserInfo[i].lose=m_arrayUserInfo[i].level=0;
	
	}

	m_ReadyBattleFlag = STATE_NONE;
	m_StandByCount = 0;
	m_selectStandbyIdx = -1;	
	m_currentUserCount=0;
	m_isLeader=FALSE;
	m_MyTextIdxinStandby=-1;

}

void CIFGonryunBattlePractice::ClearRegTeams()
{
	
	m_RegBteamIdx = -1;
}



CIFBattingWindow::CIFBattingWindow( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_TRADE_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );
	m_otherBar = new CIFControl( this );
	m_otherInvenBar = new CIFControl( this );
	m_otherLine = new CIFControl( this );
	m_otherSENumBar = new CIFControl( this );
	m_myBar = new CIFControl( this );
	m_myInvenBar = new CIFControl( this );
	m_myLine = new CIFControl( this );
	m_mySENumBar = new CIFControl( this );

	m_okBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );
	m_admitBtn = new CIFButtonCtl( this );
	m_closeBtn = new CIFButtonCtl( this );
	m_seInputBtn = new CIFButtonCtl( this );
	
	
	m_otherIdxOnServer = 0;
	strcpy( m_otherName, "none" );

	m_myTradeSE = 0;
	m_otherTradeSE = 0;

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_GONRYUN_BATTLE_PRACTICE_WINDOW_BATTING_TYPE );
}

CIFBattingWindow::~CIFBattingWindow()
{

}
	
void CIFBattingWindow::PrepareControl()
{
	CIFControl::PrepareControl( IF_TRADE_WIN );

	m_textCtl[IT_TRADE_WIN_TITLE]->PrepareControl( IF_TRADE_WIN_TITLE_TEXT );
	m_textCtl[IT_TRADE_WIN_OTHER_NAME_U]->PrepareControl( IF_TRADE_WIN_OTHER_NAME_TEXT_U );
	m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->PrepareControl( IF_TRADE_WIN_OTHER_INVEN_POS );
	m_textCtl[IT_TRADE_WIN_OTHER_SE]->PrepareControl( IF_TRADE_WIN_OTHER_SE_TEXT );
	m_textCtl[IT_TRADE_WIN_OTHER_SE_U]->PrepareControl( IF_TRADE_WIN_OTHER_SE_TEXT_U );
	m_textCtl[IT_TRADE_WIN_OTHER_OK]->PrepareControl( IF_TRADE_WIN_OTHER_OK_TEXT );
	m_textCtl[IT_TRADE_WIN_OTHER_ADMIT]->PrepareControl( IF_TRADE_WIN_OTHER_ADMIT_TEXT );
	m_textCtl[IT_TRADE_WIN_MY_NAME_U]->PrepareControl( IF_TRADE_WIN_MY_NAME_TEXT_U );
	m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->PrepareControl( IF_TRADE_WIN_MY_INVEN_POS );
	m_textCtl[IT_TRADE_WIN_MY_SE]->PrepareControl( IF_TRADE_WIN_MY_SE_TEXT );
	m_textCtl[IT_TRADE_WIN_MY_SE_U]->PrepareControl( IF_TRADE_WIN_MY_SE_TEXT_U );
	m_textCtl[IT_TRADE_OK]->PrepareControl( IF_TRADE_OK_TEXT );
	m_textCtl[IT_TRADE_CANCEL]->PrepareControl( IF_TRADE_CANCEL_TEXT );
	m_textCtl[IT_TRADE_ADMIT]->PrepareControl( IF_TRADE_ADMIT_TEXT );
	m_textCtl[IT_TRADE_SEINPUT]->PrepareControl( IF_TRADE_SEINPUT_TEXT );

	m_titleBar->PrepareControl( IF_TRADE_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_TRADE_WIN_TITLE_BOTTOM );
	m_otherBar->PrepareControl( IF_TRADE_WIN_OTHER_BAR );
	m_otherInvenBar->PrepareControl( IF_TRADE_WIN_OTHER_INVEN );
	m_otherLine->PrepareControl( IF_TRADE_WIN_OTHER_LINE );
	m_otherSENumBar->PrepareControl( IF_TRADE_WIN_OTHER_SE_NUM_BAR );
	m_myBar->PrepareControl( IF_TRADE_WIN_MY_BAR );
	m_myInvenBar->PrepareControl( IF_TRADE_WIN_MY_INVEN );
	m_myLine->PrepareControl( IF_TRADE_WIN_MY_LINE );
	m_mySENumBar->PrepareControl( IF_TRADE_WIN_MY_SE_NUM_BAR );

	m_okBtn->PrepareControl( IF_TRADE_OK_BTN_0 );
	m_cancelBtn->PrepareControl( IF_TRADE_CANCEL_BTN_0 );
	m_admitBtn->PrepareControl( IF_TRADE_ADMIT_BTN_0 );
	m_closeBtn->PrepareControl( IF_TRADE_WIN_CLOSE_BTN_0 );
	m_seInputBtn->PrepareControl( IF_TRADE_SEINPUT_BTN_0 );
	
}

int CIFBattingWindow::WindowFrame()
{
	char szTemp[256];

	
	sprintf( szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 124 )  );
	m_textCtl[IT_TRADE_WIN_TITLE]->SetText( szTemp );

	sprintf( szTemp, "%s", m_otherName );
	m_textCtl[IT_TRADE_WIN_OTHER_NAME_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
	m_textCtl[IT_TRADE_WIN_OTHER_SE]->SetText( szTemp );

	strcpy( szTemp, g_ifMng->GetNumberString( m_otherTradeSE ) );
	m_textCtl[IT_TRADE_WIN_OTHER_SE_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 549 ) );
	m_textCtl[IT_TRADE_WIN_OTHER_OK]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 550 ) );
	m_textCtl[IT_TRADE_WIN_OTHER_ADMIT]->SetText( szTemp );

	sprintf( szTemp, "%s", g_cgv.myCharacterInfo->name );
	m_textCtl[IT_TRADE_WIN_MY_NAME_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 485 ) );
	m_textCtl[IT_TRADE_WIN_MY_SE]->SetText( szTemp );

	strcpy( szTemp, g_ifMng->GetNumberString( m_myTradeSE ) );
	m_textCtl[IT_TRADE_WIN_MY_SE_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 549 ) );
	m_textCtl[IT_TRADE_OK]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 551 ) );
	m_textCtl[IT_TRADE_CANCEL]->SetText( szTemp );

	
	sprintf( szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 125 ) );
	m_textCtl[IT_TRADE_ADMIT]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 552 ) );
	m_textCtl[IT_TRADE_SEINPUT]->SetText( szTemp );
	
	return( IF_CTL_NONE );
}

void CIFBattingWindow::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	m_titleBar->Render();
	m_titleBottom->Render();
	m_otherBar->Render();
	m_otherInvenBar->Render();
	m_otherLine->Render();
	m_otherSENumBar->Render();
	m_myBar->Render();
	m_myInvenBar->Render();
	m_myLine->Render();
	m_mySENumBar->Render();
	
	m_okBtn->Render();
	m_cancelBtn->Render();
	m_admitBtn->Render();
	m_closeBtn->Render();
	m_seInputBtn->Render();
	
	
	g_ifMng->m_iconCtl->BeginFrame();
	{
		int i, itemIdx, cx, cy, itemTableIdx;
		
		for( i = 0; i < 12; i ++ )
		{
			itemIdx = g_cgv.myCharacterInfo->m_iotherTrade[i];
			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->GetXPos() + ( i % 6 ) * 34;
				cy = m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->GetYPos() + ( i / 6 ) * 33;
				itemTableIdx = g_cgv.myCharacterInfo->m_itradeItem[itemIdx].itemTableIdx;
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}
				
		
		for( i = 0; i < 12; i ++ )
		{
			itemIdx = g_cgv.myCharacterInfo->m_imyTrade[i];
			if( itemIdx >= 0 )
			{
				cx = m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->GetXPos() + ( i % 6 ) * 34;
				cy = m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->GetYPos() + ( i / 6 ) * 33;
				itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );
			}
		}
	}
	g_ifMng->m_iconCtl->EndFrame();
	
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_TRADE_WIN_TITLE]->Render();
		m_textCtl[IT_TRADE_WIN_OTHER_NAME_U]->Render();
		m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->Render();
		m_textCtl[IT_TRADE_WIN_OTHER_SE]->Render();
		m_textCtl[IT_TRADE_WIN_OTHER_SE_U]->Render( IF_TEXT_OUTPUT_RIGHT );
		m_textCtl[IT_TRADE_WIN_OTHER_OK]->Render();
		m_textCtl[IT_TRADE_WIN_OTHER_ADMIT]->Render();
		m_textCtl[IT_TRADE_WIN_MY_NAME_U]->Render();
		m_textCtl[IT_TRADE_WIN_MY_SE]->Render();
		m_textCtl[IT_TRADE_WIN_MY_SE_U]->Render( IF_TEXT_OUTPUT_RIGHT );
		m_textCtl[IT_TRADE_OK]->Render();
		m_textCtl[IT_TRADE_CANCEL]->Render();
		m_textCtl[IT_TRADE_ADMIT]->Render();
		m_textCtl[IT_TRADE_SEINPUT]->Render();

		
		g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
		g_chatManager->SetReverseSize( -1 );
		g_chatManager->SetColor( 0xffffffff );
		for( int i = 0; i < 12; i ++ )
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->m_iotherTrade[i];
			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->m_itradeItem[itemIdx] ) )
				{
					cx = m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->GetXPos() + ( i % 6 ) * 34;
					cy = m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->GetYPos() + ( i / 6 ) * 33;
					number = g_cgv.myCharacterInfo->m_itradeItem[itemIdx].durability + 1;
					sprintf( szTemp, "%d", number );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
		
		for( i = 0; i < 12; i ++ )
		{
			int itemIdx, number, cx, cy;
			char szTemp[128];
			itemIdx = g_cgv.myCharacterInfo->m_imyTrade[i];

			if( itemIdx >= 0 )
			{
				if( GTH_IsStackItem( &g_cgv.myCharacterInfo->item[itemIdx] ) )
				{
					cx = m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->GetXPos() + ( i % 6 ) * 34;
					cy = m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->GetYPos() + ( i / 6 ) * 33;
					number = g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
					sprintf( szTemp, "%d", number );
					g_chatManager->TextOut( cx, cy + 19, szTemp );
				}
			}
		}
	}
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->EndFrame();
}

int CIFBattingWindow::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );
	
	return( IF_CTL_NONE );
}

int CIFBattingWindow::WindowProcess()
{
	
	if( m_readyFlag )
	{
		m_okBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		m_cancelBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	}
	else
	{
		m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_cancelBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}

	if( !m_readyFlag && !m_otherReadyFlag )
	{
		m_seInputBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	}
	else
	{
		m_seInputBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}

	m_admitBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	if( m_readyFlag && m_otherReadyFlag )
	{
		if( !m_confirmFlag ) m_admitBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	}

	if( m_otherReadyFlag )
	{
		m_textCtl[IT_TRADE_WIN_OTHER_OK]->SetColor( 0xffffff00 );
	}
	else
	{
		m_textCtl[IT_TRADE_WIN_OTHER_OK]->SetColor( 0xff000000 );
	}

	m_prepareReadyTime -= g_timer.GetElapsedMilliTime();

	if( m_prepareReadyTime > 0.0f )
	{
		m_okBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		m_cancelBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		m_seInputBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		m_admitBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}

	
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		GTH_SendMessage_PCTrade_ExitBatting();
		return IF_CTL_SELECT;
	}
	
	if( m_okBtn->ProcessButton() == IF_CTL_SELECT )
	{
		GTH_SendMessage_PCTrade_OkBatting( m_otherIdxOnServer );
	}
	
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		GTH_SendMessage_PCTrade_CancelBatting( m_otherIdxOnServer );
	}
	
	if( m_admitBtn->ProcessButton() == IF_CTL_SELECT )
	{
		GTH_SendMessage_PCTrade_ConfirmBatting( m_otherIdxOnServer, true );
		m_confirmFlag = TRUE;
	}
	
	if( m_seInputBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_ifMng->m_stackWin->InitStack( g_LPACK.GetMassage( 0, 553 ), NULL, IF_STACK_TYPE_BATTING_TRADE, m_myTradeSE );
	}

	
	int x, y, pos, inOtherPos = true;
	int mx, my, temp;
	
	mx = g_input->GetPosX() - m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_TRADE_WIN_OTHER_INVEN_POS]->GetYPos();

	if( mx <= 0 || my <= 0 ) 
	{
		inOtherPos = false;
	}
	else if( mx >= 34 * 6 ||  my >= 33 * 2 )
	{
		inOtherPos = false;
	}
	else
	{
		temp = mx / 34;
		x = temp % 6;
		temp = my / 33;
		y = temp % 2;
		
		pos = y * 6 + x;
		
		
		if( pos < 0 || pos >= 12 )
		{
			inOtherPos = false;
		}
	}

	if( inOtherPos )
	{
		int itemIdx = g_cgv.myCharacterInfo->m_iotherTrade[pos];

		
		if( g_input->GetLBNone() ) 
		{
			
			if( itemIdx >= 0 )
			{
				g_ifMng->InitRolloverText();
				g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->m_itradeItem[itemIdx] );
				g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
			}
		}
		return( IF_CTL_NONE );
	}

	
	mx = g_input->GetPosX() - m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_TRADE_WIN_MY_INVEN_POS]->GetYPos();

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
	
	int itemIdx = g_cgv.myCharacterInfo->m_imyTrade[pos];
	
	
	if( g_input->GetLBNone() ) 
	{
		
		if( itemIdx >= 0 )
		{
			g_ifMng->InitRolloverText();
			g_ifMng->MakeItemRollover( &g_cgv.myCharacterInfo->item[itemIdx] );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		}
	}
	
	
	if( g_input->GetLBUp() && !m_readyFlag && !m_otherReadyFlag ) 
	{
		GetTradeItem( pos );
	}

	return IF_CTL_NONE;
}

void CIFBattingWindow::SetTradeInfo( int otherIdxOnServer, char* name )
{
	m_otherIdxOnServer = otherIdxOnServer;
	strcpy( m_otherName, name );
}

void CIFBattingWindow::InitTrade()
{
	char szTemp[256];

		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,135), m_otherName );
	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 125, 100) );

	g_ifMng->m_pgonryunBattleBattingwindow->Enable();
	g_ifMng->m_itemWin->Enable();
	g_ifMng->SetFocus( g_ifMng->m_pgonryunBattleBattingwindow );
	g_ifMng->m_pgonryunBattlePractice->Enable(false);
	g_ifMng->m_pgonryunBattlePractice->ClearStandbyList();
	
	for( int i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		memset( &g_cgv.myCharacterInfo->m_itradeItem[i], 0, sizeof( item_t ) );
		g_cgv.myCharacterInfo->m_itradeItem[i].itemTableIdx = -1;
		g_cgv.myCharacterInfo->m_iotherTrade[i] = -1;
		g_cgv.myCharacterInfo->m_imyTrade[i] = -1;
		memset( &g_GonryunBattlePractic.m_itradeItemtemp[i], 0, sizeof( item_t ) );
		g_GonryunBattlePractic.m_itradeItemtemp[i].itemTableIdx = -1;
		g_GonryunBattlePractic.m_iotherTradetemp[i] = -1;
		g_GonryunBattlePractic.m_imyTradetemp[i] = -1;

	}
	g_GonryunBattlePractic.m_myTradeSEtemp=0;
	g_GonryunBattlePractic.m_otherTradeSEtemp=0;
	m_myTradeSE = 0;
	m_otherTradeSE = 0;
	m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

	m_readyFlag = FALSE;
	m_otherReadyFlag = FALSE;
	m_confirmFlag = FALSE;
	m_otherConfirmFlag = FALSE;

	m_textCtl[IT_TRADE_WIN_OTHER_ADMIT]->SetColor( 0xff000000 );
	m_textCtl[IT_TRADE_WIN_OTHER_OK]->SetColor( 0xff000000 );

	m_prepareReadyTime = 0.0f;
}

void CIFBattingWindow::CancelTrade()
{
	item_t *item;
	int itemIdx, inventoryPos;

	for( int i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		itemIdx = g_cgv.myCharacterInfo->m_imyTrade[i];
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
	for( i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		memset( &g_cgv.myCharacterInfo->m_itradeItem[i], 0, sizeof( item_t ) );
		g_cgv.myCharacterInfo->m_itradeItem[i].itemTableIdx = -1;
		g_cgv.myCharacterInfo->m_iotherTrade[i] = -1;
		g_cgv.myCharacterInfo->m_imyTrade[i] = -1;
	}
	Enable( false );
}


void CIFBattingWindow::ConfirmTradeDefeat()
{
	int i;
	int itemIdx;
	item_t *item;
	char szTemp[256];

	
	g_cgv.myCharacterInfo->curChargeSE -= g_GonryunBattlePractic.m_myTradeSEtemp;


	
	
	for( i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		if( g_GonryunBattlePractic.m_imyTradetemp[i] < 0 ) continue;
		itemIdx = g_GonryunBattlePractic.m_imyTradetemp[i];
		item = &g_cgv.myCharacterInfo->item[itemIdx];
		GTH_DeleteMyItem( item );
	}

	
	

	

	
	Enable( false );
}


void CIFBattingWindow::ConfirmTrade()
{
	int i;
	int itemIdx, inventoryPos;
	item_t *item;
	char szTemp[256];

	




	for( i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		if( g_GonryunBattlePractic.m_iotherTradetemp[i] < 0 ) continue;
		itemIdx = g_GonryunBattlePractic.m_iotherTradetemp[i];
		item = &g_GonryunBattlePractic.m_itradeItemtemp[itemIdx];
		
		int stackItemIdx = GTH_StackMyItem( item );
		
		if( stackItemIdx >= 0 )
		{
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += item->durability + 1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
		}
		else
		{
			inventoryPos = GTH_FindEmptyInventoryPos();
			if( inventoryPos >= 0 ) 
			{
				itemIdx = GTH_AddMyItem( item );
				g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;
			}
		}
	}	
	
	Enable( false );
}

int CIFBattingWindow::GetTradeItem( int pos )
{
	int itemIdx;

	

	itemIdx = g_cgv.myCharacterInfo->m_imyTrade[pos];
	
	

	
	if( itemIdx < 0 && g_cgv.myCharacterInfo->mouseInventory < 0 ) 
	{
		return( 0 );
	}
	else
	{
		GTH_SendMessage_PCTrade_ItemBatting( m_otherIdxOnServer, pos );
	}
	return( 1 );
}

void CIFBattingWindow::SetReady( BOOL type, BOOL flag )
{
	if( TRUE == type )
	{
		if(TRUE == flag ) m_readyFlag = TRUE;
		else m_readyFlag = FALSE;
	}
	else
	{
		if(TRUE == flag ) m_otherReadyFlag = TRUE;
		else m_otherReadyFlag = FALSE;
	}

	
	if( FALSE == flag )
	{
		m_confirmFlag = FALSE;
		m_otherConfirmFlag = FALSE;
		m_textCtl[IT_TRADE_WIN_OTHER_ADMIT]->SetColor( 0xff000000 );

		m_prepareReadyTime = 3000.0f;
	}
}

void CIFBattingWindow::SetConfirm( int flag )
{
	m_otherConfirmFlag = TRUE;
	m_textCtl[IT_TRADE_WIN_OTHER_ADMIT]->SetColor( 0xffffff00 );
}






void CIFPostWin::InitIFPost()
{
	for( int CheckIFIdx=0; CheckIFIdx<MAIL_WIN_LIST_MAX_NUM; CheckIFIdx++ )
		m_checkBtn[CheckIFIdx]->SetCheck(false);

}
CIFPostWin::CIFPostWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	int i;

	for( i=0; i<NUM_IT_MAIL_WIN; i++ )
		m_textCtl[i]	= new CIFText( this );

	for( i=0; i<MAIL_WIN_LIST_MAX_NUM; i++ )
		m_checkBtn[i]	= new CIFCheckBox( this );

	
	m_titleBar			= new CIFControl( this );
	m_titleBarBottom	= new CIFControl( this );

	m_recvMailInfo		= new CIFControl( this );
	m_storeMailInfo		= new CIFControl( this );

	m_selectBar			= new CIFControl( this );

	
	m_recvMailBtn		= new CIFButtonCtl( this );
	m_storeMailBtn		= new CIFButtonCtl( this );
	m_writeMailBtn		= new CIFButtonCtl( this );
	
	m_deleteMailBtn		= new CIFButtonCtl( this );
	m_cancelBtn			= new CIFButtonCtl( this );
	m_exitBtn			= new CIFButtonCtl( this );

	
	m_scrollBar			= new CIFScrollBar( this );

	m_interfaceType		= MAIL_INTERFACE_TYPE_RECVMAIL;

	m_bDisplay			= false;	

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_POST_WINDOW_TYPE );

}

CIFPostWin::~CIFPostWin()
{
}

void CIFPostWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_POST_WIN );

	
	m_textCtl[IT_MAIL_WIN_TITLE]->PrepareControl( IF_POST_WIN_TITLE );

	m_textCtl[IT_MAIL_WIN_LIST_SENDER]->PrepareControl( IF_POST_WIN_LIST_SENDER );
	m_textCtl[IT_MAIL_WIN_LIST_TITLE]->PrepareControl( IF_POST_WIN_LIST_TITLE );
	m_textCtl[IT_MAIL_WIN_LIST_DATE]->PrepareControl( IF_POST_WIN_LIST_DATE );
	m_textCtl[IT_MAIL_WIN_LIST_STATUS]->PrepareControl( IF_POST_WIN_LIST_STATUS );

	
	m_titleBar->PrepareControl( IF_POST_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_POST_WIN_TITLE_BAR_BOTTOM );	
	m_selectBar->PrepareControl( IF_POST_WIN_SELECT_BAR );

	m_writeMailBtn->PrepareControl( IF_POST_WIN_WRITEPOST_BTN_0 );	

	m_deleteMailBtn->PrepareControl( IF_POST_WIN_DELETEPOST_BTN_0 );
	m_cancelBtn->PrepareControl( IF_POST_WIN_CANCEL_BTN_0 );
	m_exitBtn->PrepareControl( IF_POST_WIN_EXIT_BTN_0 );

	
	m_scrollBar->PrepareControl( IF_POST_WIN_SCROLL );
	m_scrollBar->SetBarPosition( 0, 0 );

	for( int i=0; i<MAIL_WIN_LIST_MAX_NUM; i++ )
	{
		m_checkBtn[i]->PrepareControl( IF_POST_WIN_CHECKBOX_BTN_0 + i*2, 0.0f, (float)( MAIL_WIN_LIST_LINE_HEIGHT * i ) );				
		m_checkBtn[i]->SetCheck( false );
	}

	m_selectBarSavePos = g_ifMng->m_iPos[IF_POST_WIN_SELECT_BAR].clientY;
}

int CIFPostWin::WindowFrame()
{
	switch( m_interfaceType )
	{
	case MAIL_INTERFACE_TYPE_RECVMAIL:
		m_textCtl[IT_MAIL_WIN_TITLE]->SetText( g_LPACK.GetMessage(0,978) );		
		break;
	case MAIL_INTERFACE_TYPE_STOREMAIL:
		m_textCtl[IT_MAIL_WIN_TITLE]->SetText( g_LPACK.GetMessage(0,979) );		
		break;
	}


















	if ( g_cgv.myCharacterInfo->m_PostMng.GetCheckCount() > 0 )
		m_deleteMailBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	else 
		m_deleteMailBtn->SetButtonState( IF_BTN_TYPE_DISABLE );









	
	

	m_textCtl[IT_MAIL_WIN_LIST_SENDER]->SetText( g_LPACK.GetMessage(0,980) );
	m_textCtl[IT_MAIL_WIN_LIST_TITLE]->SetText( g_LPACK.GetMessage(0,982) );

	
	

	m_textCtl[IT_MAIL_WIN_LIST_DATE]->SetText( g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,280) );
	m_textCtl[IT_MAIL_WIN_LIST_STATUS]->SetText( g_LPACK.GetMessage(0,984) );

	m_writeMailBtn->SetButtonText( g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,311));
	m_deleteMailBtn->SetButtonText( g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,223) );
	m_cancelBtn->SetButtonText( g_LPACK.GetMessage(0,521) );
	
	int PostCount = g_cgv.myCharacterInfo->m_PostMng.GetPostCount();
	if( PostCount - MAIL_WIN_LIST_MAX_NUM > 0 && PostCount - MAIL_WIN_LIST_MAX_NUM != m_scrollBar->m_max )	
	{
		m_scrollBar->SetBarPosition( PostCount - MAIL_WIN_LIST_MAX_NUM, m_scrollBar->m_current );
	}
	else if( PostCount - MAIL_WIN_LIST_MAX_NUM <= 0 )
	{
		m_scrollBar->SetBarPosition( 0, 0 );
	}

	return( IF_CTL_NONE );
}

void CIFPostWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();	
	m_writeMailBtn->RenderTextBtn();	
	m_deleteMailBtn->RenderTextBtn();
	m_cancelBtn->RenderTextBtn();
	m_exitBtn->Render();
	
	
	m_scrollBar->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_MAIL_WIN_TITLE]->Render();
		m_textCtl[IT_MAIL_WIN_LIST_SENDER]->Render();
		m_textCtl[IT_MAIL_WIN_LIST_TITLE]->Render();
		m_textCtl[IT_MAIL_WIN_LIST_DATE]->Render();
		m_textCtl[IT_MAIL_WIN_LIST_STATUS]->Render();

		UpdateMailList();
	}
	g_chatManager->EndFrame();
}

int CIFPostWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFPostWin::WindowProcess()
{
	if( m_exitBtn->ProcessButton() == IF_CTL_SELECT )
	{
		for( int i=0; i<MAIL_WIN_LIST_MAX_NUM; i++ )
			m_checkBtn[i]->SetCheck( false );

	    
		g_PostFunc.GTH_ProcessMessage_PostSystem_ExitWindow();

		Enable( false );
		m_bDisplay = false;
		return IF_CTL_SELECT;
	}

	if( m_writeMailBtn->ProcessButton() == IF_CTL_SELECT )
	{









	
		if(g_cgv.myCharacterInfo->m_PostMng.GetState() == CPostManager::POSTSYSTEM_READMODE)
		{
			g_PostFunc.GTH_SendMessage_PostSystem_State();	   
		}
		g_PostFunc.GTH_SendMessage_PostSystem_WriteOpen();
	

		
		
		
		return IF_CTL_SELECT;
	}

	if( m_deleteMailBtn->ProcessButton() == IF_CTL_SELECT )
	{
		
		g_PostFunc.GTH_SendMessage_PostSystem_DeletePost();

		return IF_CTL_SELECT;
	}

	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		for( int i=0; i<MAIL_WIN_LIST_MAX_NUM; i++ )
			m_checkBtn[i]->SetCheck( false );
		
		g_PostFunc.GTH_ProcessMessage_PostSystem_ExitWindow();

		Enable( false );
		m_bDisplay = false;
		return IF_CTL_SELECT;
	}

	if( m_scrollBar->ProcessScrollBar() == IF_CTL_SELECT )
	{
		return IF_CTL_SELECT;
	}

	int PostCount = g_cgv.myCharacterInfo->m_PostMng.GetPostCount();	
	
	

	for( int i=0; i<MAIL_WIN_LIST_MAX_NUM && i<PostCount; i++ )
	{
		if( m_checkBtn[i]->ProcessCheckBox() == IF_CTL_SELECT )
		{
			CPostManager::PostPackage_t* pPostPackage = NULL;	
			pPostPackage = g_cgv.myCharacterInfo->m_PostMng.GetPostPackage(i+m_scrollBar->m_current);

			if ( pPostPackage == NULL)
			{		 
				m_checkBtn[i]->SetCheck(false);
				continue;	
			}			

			
			pPostPackage->bCheck = m_checkBtn[i]->GetCheck();
			return IF_CTL_SELECT;
		}
	}
	
	
	int mx, my, x1, y1, xs, ys;
	int seletedIdx, listNum;
	
	if( PostCount < MAIL_WIN_LIST_MAX_NUM )
		listNum = PostCount;
	else
		listNum = MAIL_WIN_LIST_MAX_NUM;

	mx = g_input->GetPosX();
	my = g_input->GetPosY();
	x1 = m_textCtl[IT_MAIL_WIN_LIST_SENDER]->GetXPos();
	y1 = m_textCtl[IT_MAIL_WIN_LIST_SENDER]->GetYPos() + 19;
	xs = 545;
	ys = listNum * MAIL_WIN_LIST_LINE_HEIGHT;

	mx = mx - x1;
	my = my - y1;
	
	if( mx < 0 || my < 0 || mx > xs || my > ys ) 
		seletedIdx = -1;
	else
		seletedIdx = my / MAIL_WIN_LIST_LINE_HEIGHT;

	if( seletedIdx >= 0 && seletedIdx < listNum )
	{
		if( g_input->GetLBUp() ) 
		{
			m_selectedIdx = seletedIdx + m_scrollBar->m_current;
		}
		else if( g_input->GetLBDBClk() )
		{
			m_selectedIdx = seletedIdx + m_scrollBar->m_current;

			

			if( m_selectedIdx < 0 ) return IF_CTL_NONE;

			

			
			
			g_cgv.myCharacterInfo->m_PostMng.SetOpenPost(m_selectedIdx);			
			CPostManager::PostPackage_t* pPost = g_cgv.myCharacterInfo->m_PostMng.GetOpenPost();
			g_PostFunc.GTH_SendMessage_MailBox_Open((int)pPost->PostSendType, m_selectedIdx);



		}
	}
	else
	{
		if( g_input->GetLBUp() ) m_selectedIdx = -1;
	}

	return IF_CTL_NONE;
}

void CIFPostWin::SetInterfaceType( int type )
{
	m_interfaceType = type;
}

void CIFPostWin::UpdateMailList()
{
	int i, j, idx;
	char buffer[2048];

	if( !m_bDisplay )
		return;

	int PostCount = g_cgv.myCharacterInfo->m_PostMng.GetPostCount();
	for( i=1, j=0; i<=MAIL_WIN_LIST_MAX_NUM && j< PostCount; j++ )
	{
		

		

		idx = j + m_scrollBar->m_current;
		CPostManager::PostPackage_t* pPostPackage = 
			g_cgv.myCharacterInfo->m_PostMng.GetPostPackage(idx);

		if ( (pPostPackage == NULL)  ||  (pPostPackage->iPostIdx < 0 ))
			continue;

		
		

		m_checkBtn[i-1]->SetCheck( pPostPackage->bCheck );
		
		if( idx == m_selectedIdx )
		{
			SetSelectBar( i - 1 );
		}

		m_checkBtn[i-1]->Render();

		
		strcpy( buffer,  pPostPackage->szFromName);
		g_chatManager->DrawTextMail( m_textCtl[IT_MAIL_WIN_LIST_SENDER]->GetXPos(), m_textCtl[IT_MAIL_WIN_LIST_SENDER]->GetYPos()+i*MAIL_WIN_LIST_LINE_HEIGHT+2, buffer, 109, 13, CHAT_ARRANGE_OVERSIZECUT );

		strcpy( buffer, pPostPackage->szPostTitle );
		g_chatManager->DrawTextMail( m_textCtl[IT_MAIL_WIN_LIST_TITLE]->GetXPos(), m_textCtl[IT_MAIL_WIN_LIST_TITLE]->GetYPos()+i*MAIL_WIN_LIST_LINE_HEIGHT+2, buffer, 253, 13, CHAT_ARRANGE_OVERSIZECUT );
	
		
		
		_snprintf( buffer, 2048, g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,281), pPostPackage->iRemainDays );
		buffer[2047] = NULL;

		g_chatManager->DrawTextMail( m_textCtl[IT_MAIL_WIN_LIST_DATE]->GetXPos(), 
			m_textCtl[IT_MAIL_WIN_LIST_DATE]->GetYPos()+i*MAIL_WIN_LIST_LINE_HEIGHT+2, 
			buffer, 121, 13, CHAT_ARRANGE_OVERSIZECUT );

		
		switch( pPostPackage->MailState  )
		{
		case CPostManager::enumPostPackageState::POSTPACKAGE_UNCHECK:
			strcpy( buffer, g_LPACK.GetMessage(0,987) );
			break;
		case CPostManager::enumPostPackageState::POSTPACKAGE_CHECK:
			strcpy( buffer, g_LPACK.GetMessage(0,988) );
			break;
		default :
			strcpy( buffer, "" );
		}

		g_chatManager->DrawText( m_textCtl[IT_MAIL_WIN_LIST_STATUS]->GetXPos(), m_textCtl[IT_MAIL_WIN_LIST_STATUS]->GetYPos()+i*MAIL_WIN_LIST_LINE_HEIGHT+2, buffer, 49, 13, CHAT_ARRANGE_OVERSIZECUT );

		i++;
	}
}

void CIFPostWin::DeleteMailList()
{


}

void CIFPostWin::SetDisplayMode( bool mode )
{
	m_bDisplay = mode;
}


void CIFPostWin::SetSelectBar( int idx )
{
	m_selectBar->Enable();
	g_ifMng->m_iPos[IF_POST_WIN_SELECT_BAR].clientY = m_selectBarSavePos + idx * MAIL_WIN_LIST_LINE_HEIGHT;
	m_selectBar->UpdateControl();
	m_selectBar->UpdateBuffer();
	m_selectBar->Render();
}







CIFMailBoxWin::CIFMailBoxWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_TRADE_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	m_titleBar = new CIFControl( this );
	m_okBtn = new CIFButtonCtl( this );	
	m_recvBtn = new CIFButtonCtl( this );	
	

	m_bDisplay			= false;
	m_bVisibleCursor	= true;
	m_nak				= 0;



	m_scrollBar		= new CIFScrollBar(this);
	m_mailLine[CPostManager::POSTSYSTEM_READMODE] = 0;
	m_mailLine[CPostManager::POSTSYSTEM_WRITEMODE]= 0;

	memset(&m_message, 0, sizeof(m_message));
	memset(&m_name, 0, sizeof(m_name));

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_MAILBOX_WINDOW_TYPE );
}

CIFMailBoxWin::~CIFMailBoxWin() { }
	
void CIFMailBoxWin::PrepareControl()
{

	
	CIFControl::PrepareControl( IF_MAILBOX_WIN );

	m_textCtl[IT_MAILBOX_WIN_TITLE]->PrepareControl(IF_MAILBOX_WIN_TITLE_TEXT);
	m_textCtl[IT_MAILBOX_WIN_SENDER_TEXT]->PrepareControl(IF_MAILBOX_WIN_SENDER_TEXT);	
	m_textCtl[IT_MAILBOX_WIN_SENDER_NAME]->PrepareControl(IF_MAILBOX_WIN_SENDER_NAME);

	m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE_TEXT]->PrepareControl(IF_MAILBOX_WIN_MESSAGETITLE_TEXT);
	m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE]->PrepareControl(IF_MAILBOX_WIN_MESSAGETITLE);

	m_textCtl[IT_MAILBOX_WIN_MESSAGE_TEXT]->PrepareControl(IF_MAILBOX_WIN_MESSAGE_TEXT);
	m_textCtl[IT_MAILBOX_WIN_MESSAGE]->PrepareControl(IF_MAILBOX_WIN_MESSAGE);	
	m_textCtl[IT_MAILBOX_WIN_PRICE_TEXT]->PrepareControl(IF_MAILBOX_WIN_PRICE_TEXT);
	m_textCtl[IT_MAILBOX_WIN_PRICE]->PrepareControl(IF_MAILBOX_WIN_PRICE);
	m_textCtl[IT_MAILBOX_WIN_OK_TEXT]->PrepareControl(IF_MAILBOX_WIN_OK_TEXT);
	m_textCtl[IT_MAILBOX_WIN_RECV_TEXT]->PrepareControl(IF_MAILBOX_WIN_RECV_TEXT);
	m_textCtl[IT_MAILBOX_WIN_MY_INVEN_POS]->PrepareControl(IF_MAILBOX_WIN_MYINVEN);		

	m_titleBar->PrepareControl( IF_MAILBOX_WIN_TITLE_BAR );				
	m_okBtn->PrepareControl( IF_MAILBOX_OK_BTN_0 );		
	m_recvBtn->PrepareControl( IF_MAILBOX_RECV_BTN_0 );	
	
	m_scrollBar->PrepareControl(IF_MAILBOX_WIN_SCROLL);
	m_scrollBar->SetBarPosition(0, 0);

	int cx, cy, cs;

	cx = m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE]->GetXPos();
	cy = m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE]->GetYPos();
	g_chatManager->SetInput( cx, cy, CPostManager::POST_TITLESIZE-2, CPostManager::POST_TITLESIZE-2, INPUT_INDEX_POST_TITLE, 1, 210);

	cx = m_textCtl[IT_MAILBOX_WIN_SENDER_NAME]->GetXPos();
	cy = m_textCtl[IT_MAILBOX_WIN_SENDER_NAME]->GetYPos();
	g_chatManager->SetInput( cx, cy, NAMESTRING -2, NAMESTRING-2, INPUT_INDEX_POST_NAME, 1, 210);

	cx = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetXPos();
	cy = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetYPos();
	cs = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetXSize();
	g_chatManager->SetInput(cx, cy, CPostManager::POST_STRSIZE-1, CPostManager::POST_STRSIZE-1,
							INPUT_INDEX_POST_MESSAGE, 1, cs, true);
	g_chatManager->SetInputLine(INPUT_INDEX_POST_MESSAGE, 0, CPostManager::MAX_MESSAGE_LINE);
}

int CIFMailBoxWin::WindowFrame()
{

	switch(g_cgv.myCharacterInfo->m_PostMng.GetState())
	{
	case CPostManager::POSTSYSTEM_READMODE:
		{
			m_textCtl[IT_MAILBOX_WIN_TITLE]->SetText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 272 ));
			m_textCtl[IT_MAILBOX_WIN_SENDER_TEXT]->SetText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 259 ));	
			m_textCtl[IT_MAILBOX_WIN_RECV_TEXT]->SetText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 222 ));
			m_textCtl[IT_MAILBOX_WIN_OK_TEXT]->SetText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 278));

			CPostManager::PostPackage_t* pPostPackage = NULL;
			pPostPackage = g_cgv.myCharacterInfo->m_PostMng.GetOpenPost();
			if(pPostPackage != NULL)
			{
				int csx = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetXSize();
				m_mailLine[CPostManager::POSTSYSTEM_READMODE] = g_chatManager->GetViewLine( pPostPackage->szMailStr, csx );
				if(m_bVisibleCursor)
				{
					int curLine = m_mailLine[CPostManager::POSTSYSTEM_READMODE];
					m_scrollBar->SetBarPositionCurrent(curLine);
				}
			}
		}
		break;
	case CPostManager::POSTSYSTEM_WRITEMODE:
		{
			m_textCtl[IT_MAILBOX_WIN_TITLE]->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 258));
			m_textCtl[IT_MAILBOX_WIN_SENDER_TEXT]->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 273));	
			
			m_textCtl[IT_MAILBOX_WIN_OK_TEXT]->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 277));
			
			m_textCtl[IT_MAILBOX_WIN_RECV_TEXT]->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 276));
			
			m_mailLine[CPostManager::POSTSYSTEM_WRITEMODE] = g_chatManager->GetInputLine(INPUT_INDEX_POST_MESSAGE);
			
			if(m_bVisibleCursor)
			{
				int curLine = g_chatManager->GetCursorLine(INPUT_INDEX_POST_MESSAGE);

				if(curLine < m_scrollBar->m_current)
					m_scrollBar->SetBarPositionCurrent(curLine);
				if(curLine >= m_scrollBar->m_current + CPostManager::MAX_MESSAGE_LINE)
					m_scrollBar->SetBarPositionCurrent(curLine - CPostManager::MAX_MESSAGE_LINE + 1);
			}
		}
		break;
	}
	
	
	m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE_TEXT]->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 982));
	
	
	
	m_textCtl[IT_MAILBOX_WIN_MESSAGE_TEXT]->SetText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 260 ));
	m_textCtl[IT_MAILBOX_WIN_PRICE_TEXT]->SetText(g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 261 ));	
	
	g_chatManager->m_ix[INPUT_INDEX_POST_TITLE] = m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE]->GetXPos();
	g_chatManager->m_iy[INPUT_INDEX_POST_TITLE] = m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE]->GetYPos();

	g_chatManager->m_ix[INPUT_INDEX_POST_NAME] = m_textCtl[IT_MAILBOX_WIN_SENDER_NAME]->GetXPos();
	g_chatManager->m_iy[INPUT_INDEX_POST_NAME] = m_textCtl[IT_MAILBOX_WIN_SENDER_NAME]->GetYPos();

	g_chatManager->m_ix[INPUT_INDEX_POST_MESSAGE] = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetXPos();
	g_chatManager->m_iy[INPUT_INDEX_POST_MESSAGE] = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetYPos();

	if(m_mailLine[g_cgv.myCharacterInfo->m_PostMng.GetState()] - CPostManager::MAX_MESSAGE_LINE > 0 && m_mailLine[g_cgv.myCharacterInfo->m_PostMng.GetState()] - CPostManager::MAX_MESSAGE_LINE != m_scrollBar->m_max)
	   m_scrollBar->SetBarPosition(m_mailLine[g_cgv.myCharacterInfo->m_PostMng.GetState()] - CPostManager::MAX_MESSAGE_LINE, m_scrollBar->m_current);
	else if(m_mailLine[g_cgv.myCharacterInfo->m_PostMng.GetState()] - CPostManager::MAX_MESSAGE_LINE <= 0)
		m_scrollBar->SetBarPosition(0, 0);
	
	g_chatManager->SetInputLine(INPUT_INDEX_POST_MESSAGE, m_scrollBar->m_current, m_scrollBar->m_current + CPostManager::MAX_MESSAGE_LINE - 1);

	return( IF_CTL_NONE );
}

void CIFMailBoxWin::Render()
{
	if( !State() ) return;

	char szTemp[128+1];

	CPostManager::PostPackage_t* pPostPackage = NULL;
	
	pPostPackage = g_cgv.myCharacterInfo->m_PostMng.GetOpenPost();


	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	m_scrollBar->Render();
	
	m_textCtl[IT_MAILBOX_WIN_TITLE]->Render();
	m_textCtl[IT_MAILBOX_WIN_SENDER_TEXT]->Render();
	m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE_TEXT]->Render();
	m_textCtl[IT_MAILBOX_WIN_MESSAGE_TEXT]->Render();
	m_textCtl[IT_MAILBOX_WIN_PRICE_TEXT]->Render();
	m_textCtl[IT_MAILBOX_WIN_OK_TEXT]->Render();
	m_textCtl[IT_MAILBOX_WIN_RECV_TEXT]->Render();


	m_okBtn->Render();	
	m_recvBtn->Render();
	
	int MaxItemCount = CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE;

	GeneralItem_Render(pPostPackage, MaxItemCount);
	StackItem_Render(pPostPackage, MaxItemCount, szTemp);
	InputText_Render(pPostPackage, szTemp);
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
}


void CIFMailBoxWin::GeneralItem_Render(CPostManager::PostPackage_t* pPostPackage, int MaxItemCount)
{
	
	g_ifMng->m_iconCtl->BeginFrame();
	{
		if (g_cgv.myCharacterInfo->m_PostMng.GetState() == CPostManager::POSTSYSTEM_READMODE) 
		{
			for(int ItemIdxInPostPackage = 0; ItemIdxInPostPackage< CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; ItemIdxInPostPackage++ )
			{
				int  cx, cy;					
				item_t* pItem = NULL;
				pItem = &pPostPackage->Items[ItemIdxInPostPackage];				
					
				if ( pItem->itemTableIdx >= 0  )
				{
					cx = m_textCtl[IT_MAILBOX_WIN_MY_INVEN_POS]->GetXPos() + 
						( ItemIdxInPostPackage % 6 ) * 34;

					cy = m_textCtl[IT_MAILBOX_WIN_MY_INVEN_POS]->GetYPos() + 
						( ItemIdxInPostPackage / 6 ) * 33;
					g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, 
						g_itemTable[pItem->itemTableIdx].iconItemID );
					}
				}
			}
		else 
		{
			int itemIdx, itemTableIdx;
			for(int i = 0; i < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; i++)
			{
				itemIdx = g_cgv.myCharacterInfo->m_ItemsInPost[i];
				if(itemIdx >= 0)
				{
					int cx = m_textCtl[IT_MAILBOX_WIN_MY_INVEN_POS]->GetXPos() + (i % 6) * 34;
					int cy = m_textCtl[IT_MAILBOX_WIN_MY_INVEN_POS]->GetYPos() + (i / 6) * 33;
					itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
					g_ifMng->m_iconCtl->Draw(cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID);
				}
			}
		}
	}
	g_ifMng->m_iconCtl->EndFrame();
	g_chatManager->SetOutputType(CHAT_OUTPUT_NORMAL);
}

	

void CIFMailBoxWin::StackItem_Render(CPostManager::PostPackage_t *pPostPackage, int MaxItemCount, char *szTemp)
{
	g_chatManager->BeginFrame();
	{	
	
		g_chatManager->SetOutputType( CHAT_OUTPUT_REVERSE );
		g_chatManager->SetReverseSize( -1 );
		g_chatManager->SetColor( 0xffffffff );
		
		if (g_cgv.myCharacterInfo->m_PostMng.GetState() == CPostManager::POSTSYSTEM_READMODE) {
			for( int ItemIdxInPostPackage = 0; ItemIdxInPostPackage< MaxItemCount; ItemIdxInPostPackage++ ) {
				int  number, cx, cy;								
				item_t* pItem = NULL;
				pItem = &pPostPackage->Items[ItemIdxInPostPackage];

				if ( pItem->itemTableIdx >= 0   ) {
					if( GTH_IsStackItem( pItem )) {
						cx = m_textCtl[IT_MAILBOX_WIN_MY_INVEN_POS]->GetXPos() + ( ItemIdxInPostPackage % 6 ) * 34;
						cy = m_textCtl[IT_MAILBOX_WIN_MY_INVEN_POS]->GetYPos() + ( ItemIdxInPostPackage / 6 ) * 33;
						number  = pItem->durability + 1;
						_snprintf( szTemp, 128, "%d", number );
						szTemp[128] = NULL;
						g_chatManager->TextOut( cx, cy + 19, szTemp );
					}
				}
			}
		}
		else {
			for(int i = 0; i < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; i++) {
				int itemIdx = -1, cx, cy, number;
				char cTemp[128];
				itemIdx = g_cgv.myCharacterInfo->m_ItemsInPost[i];
				if(itemIdx >= 0) {
					if(GTH_IsStackItem(&g_cgv.myCharacterInfo->item[itemIdx])) {
						cx = m_textCtl[IT_MAILBOX_WIN_MY_INVEN_POS]->GetXPos() + (i % 6) * 34;
						cy = m_textCtl[IT_MAILBOX_WIN_MY_INVEN_POS]->GetYPos() + (i / 6) * 33;
						number =  g_cgv.myCharacterInfo->item[itemIdx].durability + 1;
						strcpy(cTemp, g_ifMng->GetNumberString(number));
						g_chatManager->TextOut( cx, cy + 19, cTemp );
					}

				}
			}
		}
	}
	g_chatManager->EndFrame();
	g_chatManager->SetOutputType(CHAT_OUTPUT_NORMAL);
}

void CIFMailBoxWin::InputText_Render(CPostManager::PostPackage_t *pPostPackage, char *szTemp)
{
	g_chatManager->SetOutputType(CHAT_OUTPUT_NORMAL);
	g_chatManager->SetColor(0xffffffff);
	
	int y = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetYPos() - m_scrollBar->m_current * 13;
	
	g_chatManager->BeginFrame();
	{
		int cx, cy, csx, csy;
		cx = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetXPos();
		cy = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetYPos();
		csx = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetXSize();
		csy = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetYSize();


		int xx = m_textCtl[IT_MAILBOX_WIN_SENDER_NAME]->GetXPos();
		int yy = m_textCtl[IT_MAILBOX_WIN_SENDER_NAME]->GetYPos();

		int xm = m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE]->GetXPos();
		int ym = m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE]->GetYPos();

		g_chatManager->RenderInputFrame(INPUT_INDEX_POST_NAME);
		g_chatManager->RenderInputFrame(INPUT_INDEX_POST_TITLE);
		g_chatManager->SetInputPosition(INPUT_INDEX_POST_MESSAGE, cx, y);
		g_chatManager->RenderInputFrame(INPUT_INDEX_POST_MESSAGE, m_bVisibleCursor);
		
	
		if(g_cgv.myCharacterInfo->m_PostMng.GetState() == CPostManager::POSTSYSTEM_READMODE)
		{
			if(pPostPackage != NULL)
			{
				if(m_bDisplay)
				{
					_snprintf(szTemp, 128, "%d",pPostPackage->Nak);
					szTemp[128] = NULL;
					m_textCtl[IT_MAILBOX_WIN_PRICE]->SetText(szTemp);
					m_textCtl[IT_MAILBOX_WIN_PRICE]->Render(IF_TEXT_OUTPUT_RIGHT);
					m_textCtl[IT_MAILBOX_WIN_SENDER_NAME]->Render(IF_TEXT_OUTPUT_RIGHT);
					m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE]->Render(IF_TEXT_OUTPUT_RIGHT);
					g_chatManager->TextOut(xx, yy, pPostPackage->szFromName, CHAT_ARRANGE_NONE, 210);
					g_chatManager->TextOut(xm, ym, pPostPackage->szPostTitle, CHAT_ARRANGE_NONE, 210);
					g_chatManager->DrawTextMail(cx, y, pPostPackage->szMailStr, csx, 13, CHAT_ARRANGE_NONE, INPUT_INDEX_POST_MESSAGE);
				}
			}
		}
		else
		{
			char *temp = g_ifMng->GetNumberString(m_nak);
			m_textCtl[IT_MAILBOX_WIN_PRICE]->SetText(temp);
			
			if(m_bDisplay)
			{
				m_textCtl[IT_MAILBOX_WIN_PRICE]->Render(IF_TEXT_OUTPUT_RIGHT);	
				g_chatManager->TextOut(xx, yy, m_name, CHAT_ARRANGE_NONE, 210);
				g_chatManager->TextOut(xm, ym, m_title, CHAT_ARRANGE_NONE, 210);
				g_chatManager->DrawTextMail(cx, y, m_message, csx, 13, CHAT_ARRANGE_NONE, INPUT_INDEX_POST_MESSAGE);
			}
		}
	}
	g_chatManager->EndFrame();
}


int CIFMailBoxWin::WindowSelection()
{
	int ret;

	
	if (ret = m_titleBar->Selection())
	{
		if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
		ret = Selection();
		if( ret == IF_LBDN ) return( IF_CTL_SELECT );
	}
	return( IF_CTL_NONE );
}


int CIFMailBoxWin::WindowProcess()
{
	
	CPostManager::PostPackage_t* pPostPackage = NULL;
	pPostPackage = g_cgv.myCharacterInfo->m_PostMng.GetOpenPost();	

	if(g_cgv.myCharacterInfo->m_PostMng.GetState() == CPostManager::POSTSYSTEM_WRITEMODE)
		WriteModeProcess();
	else
	{
		int m_x, m_y, x1, y1, xs, ys, mx, my;
		int selectedIdx;

		m_x = g_input->GetPosX();
		m_y = g_input->GetPosY();

		g_chatManager->SetInputState(INPUT_INDEX_POST_NAME, 0);
		g_chatManager->SetInputState(INPUT_INDEX_POST_TITLE, 0);
		g_chatManager->SetInputState(INPUT_INDEX_POST_MESSAGE, 0);

		
		
		
		
		
		g_chatManager->GetCursorLine(INPUT_INDEX_POST_MESSAGE);
		
		
		
		
		x1 = m_textCtl[IT_MAILBOX_WIN_SENDER_NAME]->GetXPos();
		y1 = m_textCtl[IT_MAILBOX_WIN_SENDER_NAME]->GetYPos();
		xs = 210;
		ys = 20;
		mx = x1+xs;
		my = y1+ys;
		
		if(x1 > m_x || y1 > m_y || mx < m_x || my < m_y)
			selectedIdx = -1;
		else
			selectedIdx = 0;
		
		if( selectedIdx != -1 )
		{
			if( g_input->GetLBUp() )
			{
				g_chatManager->ChangeInputMessage(INPUT_INDEX_POST_NAME);
				g_chatManager->SetCursorFromMousePos(m_x, m_y);
				m_bVisibleCursor = true;
			}
		}
		else
		{
			x1 = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetXPos();
			y1 = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetYPos();
			
			xs = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetXSize();
			ys = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetYSize();
			
			mx = m_x - x1;
			my = m_y - y1;
			
			if(mx < 0 || my < 0 || mx > xs || my > ys) 
				selectedIdx = -1;
			else
				selectedIdx = 0;
			if(selectedIdx != -1)
			{
				if( g_input->GetLBUp() )
				{
					g_chatManager->ChangeInputMessage(INPUT_INDEX_POST_MESSAGE);
					g_chatManager->SetCursorFromMousePos(m_x, m_y);
					m_bVisibleCursor = true;
				}
			}
			else {
				x1 = m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE]->GetXPos();
				y1 = m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE]->GetYPos();
				xs = 210;
				ys = 20;

				mx = x1+xs;
				my = y1+ys;

				if(x1 > m_x || y1 > m_y || mx < m_x || my < m_y)
					selectedIdx = -1;
				else
					selectedIdx = 0;
				
				if( selectedIdx != -1 )
				{
					if( g_input->GetLBUp() )
					{
						g_chatManager->ChangeInputMessage(INPUT_INDEX_POST_TITLE);
						g_chatManager->SetCursorFromMousePos(m_x, m_y);
						m_bVisibleCursor = true;
					}
				}
			}
		}
	}

	if( m_scrollBar->ProcessScrollBar() == IF_CTL_SELECT )
	{
		m_bVisibleCursor = false;
		return IF_CTL_NONE;
	}
	

	
	if( m_okBtn->ProcessButton() == IF_CTL_SELECT) 		
		return RecvButtonProcess(pPostPackage);

	
	if( m_recvBtn->ProcessButton() == IF_CTL_SELECT )
		return OKButtonProcess(pPostPackage);

	return ItemInfomationProcess(pPostPackage);

	return IF_CTL_NONE;
}

	

int CIFMailBoxWin::OKButtonProcess(CPostManager::PostPackage_t* pPostPackage)
{
	if(g_cgv.myCharacterInfo->m_PostMng.GetState() == CPostManager::POSTSYSTEM_WRITEMODE)  
	{
		Enable(false);
		g_chatManager->SetInputState(INPUT_INDEX_POST_NAME, -1);
		g_chatManager->SetInputState(INPUT_INDEX_POST_MESSAGE, -1);
		g_chatManager->ChangeInputMessage(INPUT_INDEX_DIALOGBOX);
		if(g_PostFunc.GTH_SendMessage_MailBox_UndoInvenItem())
			g_PostFunc.GTH_SendMessage_MailBox_Exit();
		else
			InitMailBoxWin();

		return IF_CTL_SELECT;
	}
	else
	{						
		Enable(false);
		g_PostFunc.GTH_SendMessage_PostSystem_ReceiveItems();
		return IF_CTL_SELECT;	
	}
	return IF_CTL_NONE;
}

int CIFMailBoxWin::RecvButtonProcess(CPostManager::PostPackage_t* pPostPackage)
{
	if(g_cgv.myCharacterInfo->m_PostMng.GetState() == CPostManager::POSTSYSTEM_READMODE)
	{
		
		if ( pPostPackage->ItemNumber > 0)  
		{
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 271),
				MSG_BUTTONTYPE_OK, IF_MESSAGE_POSTPACKAGE_REMAIN_EXIT);
			
			
			
			Enable(false);
			InitMailBoxWin();
			g_PostFunc.GTH_SendMessage_MailBox_Exit();
			
			return IF_CTL_SELECT;
		}
		else 
		{
			Enable(false);
			InitMailBoxWin();
			g_PostFunc.GTH_SendMessage_MailBox_Exit();
			return IF_CTL_SELECT;
		}		
	}
	else
	{
		
		if(m_name[0] == '\0')
		{
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 279),
			MSG_BUTTONTYPE_OK, IF_MESSAGE_POSTPACKAGE_REMAIN_EXIT);
			if(g_PostFunc.GTH_SendMessage_MailBox_UndoInvenItem())
			{
				g_chatManager->SetInputState(INPUT_INDEX_POST_NAME, -1);
				g_chatManager->SetInputState(INPUT_INDEX_POST_MESSAGE, -1);
				g_chatManager->SetInputState(INPUT_INDEX_POST_TITLE, -1);
				g_chatManager->ChangeInputMessage(INPUT_INDEX_DIALOGBOX);

				
				
				
				

				g_PostFunc.GTH_SendMessage_MailBox_Exit();
			}
			return IF_CTL_NONE;
		}
		else
		{

			char msg[256];
			_snprintf(msg, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 449), m_name);
			msg[255] = NULL;
			
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				msg, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_POSTPACKAGE_SEND);

			
			







			return IF_CTL_SELECT;
		}
	}
	return IF_CTL_NONE;
}
	
void CIFMailBoxWin::WriteModeProcess(void)
{
	int m_x, m_y, my, mx, x1, y1, xs, ys;
	int selectedIdx;
	
	g_chatManager->SetInputState(INPUT_INDEX_POST_NAME, 1);
	g_chatManager->SetInputState(INPUT_INDEX_POST_MESSAGE, 1);
	g_chatManager->SetInputState(INPUT_INDEX_POST_TITLE, 1);
	
	memset(&m_name, 0, sizeof(m_name));
	memset(&m_message, 0, sizeof(m_message));
	memset(&m_title, 0, sizeof(m_title));
	
	strcpy(m_name, g_chatManager->GetInputStr(INPUT_INDEX_POST_NAME)) ;
	strcpy(m_message, g_chatManager->GetInputStr(INPUT_INDEX_POST_MESSAGE));
	strcpy(m_title, g_chatManager->GetInputStr(INPUT_INDEX_POST_TITLE));
	
	m_x = g_input->GetPosX();
	m_y = g_input->GetPosY();
	
	x1 = m_textCtl[IT_MAILBOX_WIN_SENDER_NAME]->GetXPos();
	y1 = m_textCtl[IT_MAILBOX_WIN_SENDER_NAME]->GetYPos();
	xs = 210;
	ys = 20;
	mx = x1+xs;
	my = y1+ys;
	
	if(x1 > m_x || y1 > m_y || mx < m_x || my < m_y)
		selectedIdx = -1;
	else
		selectedIdx = 0;
	
	if( selectedIdx != -1 )
	{
		if( g_input->GetLBUp() )
		{
			g_chatManager->ChangeInputMessage(INPUT_INDEX_POST_NAME);
			g_chatManager->SetCursorFromMousePos(m_x, m_y);
			m_bVisibleCursor = true;
		}
	}
	else
	{
		x1 = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetXPos();
		y1 = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetYPos();
		
		xs = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetXSize();
		ys = m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetYSize();
		
		mx = m_x - x1;
		my = m_y - y1;
		
		if(mx < 0 || my < 0 || mx > xs || my > ys) 
			selectedIdx = -1;
		else
			selectedIdx = 0;
		if(selectedIdx != -1)
		{
			if( g_input->GetLBUp() )
			{
				g_chatManager->ChangeInputMessage(INPUT_INDEX_POST_MESSAGE);
				g_chatManager->SetCursorFromMousePos(m_x, m_y);
				m_bVisibleCursor = true;
			}
		}
		else {
			x1 = m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE]->GetXPos();
			y1 = m_textCtl[IT_MAILBOX_WIN_MESSAGETITLE]->GetYPos();
			xs = 210;
			ys = 20;
			mx = x1+xs;
			my = y1+ys;
			
			if(x1 > m_x || y1 > m_y || mx < m_x || my < m_y)
				selectedIdx = -1;
			else
				selectedIdx = 0;
			
			if( selectedIdx != -1 )
			{
				if( g_input->GetLBUp() )
				{
					g_chatManager->ChangeInputMessage(INPUT_INDEX_POST_TITLE);
					g_chatManager->SetCursorFromMousePos(m_x, m_y);
					m_bVisibleCursor = true;
				}
			}
		}
	}
	x1 = m_textCtl[IT_MAILBOX_WIN_PRICE]->GetXPos();
	y1 = m_textCtl[IT_MAILBOX_WIN_PRICE]->GetYPos();
	xs = m_textCtl[IT_MAILBOX_WIN_PRICE]->GetXSize();
	ys = 20;
	
	mx = m_x - x1;
	my = m_y - y1;
	
	if(mx < 0 || my < 0 || mx > xs || my > ys) 
		selectedIdx = -1;
	else
		selectedIdx = 0;
	if(selectedIdx != -1)
	{
		if( g_input->GetLBUp() )
			g_ifMng->m_stackWin->InitStack(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 274), NULL, IF_STACK_TYPE_MAILBOX_SEND);
			
	}
}

int CIFMailBoxWin::ItemInfomationProcess(CPostManager::PostPackage_t* pPostPackage)
{
	
	int x, y, pos, inOtherPos = true;
	int mx, my, temp;

	mx = g_input->GetPosX() - m_textCtl[IT_MAILBOX_WIN_MY_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_MAILBOX_WIN_MY_INVEN_POS]->GetYPos();

	if( mx <= 0 || my <= 0 ) return( IF_CTL_NONE );
	if( mx >= 34 * 6 ||  my >= 33 * 2 ) return( IF_CTL_NONE );

	temp = mx / 34;
	x = temp % 6;
	temp = my / 33;
	y = temp % 2;

	pos = y * 6 + x;

	
	if( pos < 0 || pos >= CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE)
		return( IF_CTL_NONE );


	int ItemInven = g_cgv.myCharacterInfo->mouseInventory;
	if(g_input->GetLBNone())
	{
		if(pPostPackage && (g_cgv.myCharacterInfo->m_PostMng.GetState() == CPostManager::POSTSYSTEM_READMODE))
		{
			if (pPostPackage->Items[pos].itemTableIdx >= 0)
			{
				g_ifMng->InitRolloverText();
				g_ifMng->MakeItemRollover( &pPostPackage->Items[pos]);
				g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
			}
		}
		else
		{
			int ItemIdx = g_cgv.myCharacterInfo->m_ItemsInPost[pos];
			if(ItemIdx >= 0)
			{
				if(g_cgv.myCharacterInfo->item[ItemIdx].itemTableIdx >= 0)
				{
					
					g_ifMng->InitRolloverText();
					g_ifMng->MakeItemRollover(&g_cgv.myCharacterInfo->item[ItemIdx]);
					g_ifMng->MakeRolloverText(g_input->GetPosX(), g_input->GetPosY(), true, 250);
				}
			}
		}
	}



	if( g_input->GetLBUp() ) 
	{	
		int itemIdx;
		itemIdx = g_cgv.myCharacterInfo->m_ItemsInPost[pos];

		if( itemIdx < 0 && ItemInven < 0 )		
			return( 0 );
		else	
			GetPostItem(pos);
	}

	return( IF_CTL_NONE );
}

			
		
void CIFMailBoxWin::SendItemDelete(void)
{
	item_t* deleteItem;

	g_cgv.myCharacterInfo->curChargeSE -= m_nak;

	for(int MailBoxSlot = 0; MailBoxSlot < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; MailBoxSlot++)
	{
		int itemIdx	= g_cgv.myCharacterInfo->m_ItemsInPost[MailBoxSlot];
		if(itemIdx < 0) continue;
		deleteItem = &g_cgv.myCharacterInfo->item[itemIdx];
		GTH_DeleteMyItem(deleteItem);
	}
}

void CIFMailBoxWin::UndoMailBoxInventory(void)
{
	item_t *pSrcitem;
	int itemIdx, inventoryPos;
	
	for(int i = 0; i < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; i++)
	{
		itemIdx = g_cgv.myCharacterInfo->m_ItemsInPost[i];
		if(itemIdx < 0) continue;
		
		pSrcitem = &g_cgv.myCharacterInfo->item[itemIdx];
		int stackItemIdx = GTH_StackMyItem(pSrcitem);
		
		if(stackItemIdx >= 0)
		{
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += pSrcitem->durability+1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability  >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
			GTH_DeleteMyItem( pSrcitem );
		}
		else
		{
			inventoryPos = GTH_FindEmptyInventoryPos();

			if( inventoryPos >= 0 ) 
				g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;
			else
				GTH_DeleteMyItem( pSrcitem );

		}
	}
	
	InitMailBoxWin();
	Enable( false );
}

void CIFMailBoxWin::InitMailBoxWin()
{
	m_nak = 0;
	memset(&m_name, 0, sizeof(m_name));
	memset(&m_message, 0, sizeof(m_message));
	memset(&m_title, 0, sizeof(m_title));
	
	strcpy(m_name, m_name) ;
	strcpy(m_message, m_message);
	strcpy(m_title, m_title);

	g_chatManager->SetInputState(INPUT_INDEX_POST_NAME, -1);
	g_chatManager->SetInputState(INPUT_INDEX_POST_MESSAGE, -1);
	g_chatManager->SetInputState(INPUT_INDEX_POST_TITLE, -1);
	g_chatManager->ChangeInputMessage(INPUT_INDEX_DIALOGBOX);

	for( int i = 0; i < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; i ++ )
		g_cgv.myCharacterInfo->m_ItemsInPost[i] = -1;

	m_mailLine[g_cgv.myCharacterInfo->m_PostMng.GetState()] = g_chatManager->GetViewLine(m_message, m_textCtl[IT_MAILBOX_WIN_MESSAGE]->GetYPos());
	m_scrollBar->SetBarPosition(m_mailLine[g_cgv.myCharacterInfo->m_PostMng.GetState()], 0);
	m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	m_recvBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
}


int CIFMailBoxWin::GetPostItem( int pos )
{
	g_PostFunc.GTH_SendMessage_MailBox_Item(m_otherIdxOnServer, pos);
	return( 1 );
}
void CIFMailBoxWin::SetDisplayMode(bool mode)
{
	m_bDisplay = mode;
}
