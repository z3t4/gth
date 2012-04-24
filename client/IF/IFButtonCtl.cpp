



#include "../global_def.h"
#include "IFCommon.h"






#define IF_FLASH_BUTTON_DELAY_TIME		500




int CIFButtonCtl::m_focusedControlID = -1;

CIFButtonCtl::CIFButtonCtl( CIFControlNode *pParentControl ) : CIFControl( pParentControl )
{
	m_btnState			=	IF_BTN_TYPE_ENABLE;
	m_str[0]			=	0;
	m_color				=	0xffffffff;
	m_saveButtonTime	=	(int)g_timer.GetAppMilliTime();
	m_flickerMode		=	0;
	m_flickerCurTime	=	400;
	m_spriteNo			=	0;
}

CIFButtonCtl::~CIFButtonCtl()
{

}

void CIFButtonCtl::PrepareControl( int id )
{
	CIFControl::PrepareControl( id, 0 );
	CIFControl::PrepareControl( id + 1, 1 );
	CIFControl::PrepareControl( id + 2, 2 );
}

void CIFButtonCtl::UpdateControl()
{
	if( m_flickerMode == 1 )
	{
		m_flickerCurTime -= g_timer.GetElapsedMilliTime();

		if( m_flickerCurTime < 0.0f )
		{
			m_spriteNo++;

			if( m_spriteNo == 2 ) m_spriteNo = 0;
			m_flickerCurTime = 400;
		}

		ChangeControlSprite( m_spriteNo );
	}

	int tempsprIdx = m_current;

	for( int i = 0; i < 3; i ++ )
	{
		m_current = i;
		CIFControl::UpdateControl();
	}
	m_current = tempsprIdx;
	CIFControlNode::UpdateControl();
}

void CIFButtonCtl::UpdateClientPosition( int x, int y )
{
	for( int i = 0; i < 3; i ++ )
	{
		m_info[i].clientX = x;
		m_info[i].clientY = y;
	}
	UpdateBuffer();
}

void CIFButtonCtl::SetButtonState( int state )
{
	m_btnState = state;
}

int CIFButtonCtl::ProcessButton()
{
	if( !State() ) return( IF_CTL_NONE );

	switch( m_btnState )
	{
	case IF_BTN_TYPE_DISABLE :
		ChangeControlSprite( 2 );
		return IF_CTL_NONE;
		break;
	case IF_BTN_TYPE_ENABLE :
		{
			ChangeControlSprite( 0 );

			switch( Selection() )
			{
			case IF_LBDN :
				ChangeControlSprite( 2 );
				
				m_focusedControlID = m_info[0].ID;
				return IF_CTL_NONE;

			case IF_LBKEEP :
				ChangeControlSprite( 2 );
				return IF_CTL_KEEP;

			case IF_LBUP :
				
				if( m_focusedControlID == m_info[0].ID )
				{
					m_focusedControlID = -1; 
					return IF_CTL_SELECT;
				}
				else
					return IF_CTL_NONE;

			case IF_IN_POS :
				ChangeControlSprite( 1 );
				return IF_CTL_NONE;
				break;

			default :				
				return IF_CTL_NONE;
				break;
			}
		}
		break;
	case IF_BTN_TYPE_FLASH :
		{
			if( m_saveButtonTime + IF_FLASH_BUTTON_DELAY_TIME < (int)g_timer.GetAppMilliTime() )
			{
				m_saveButtonTime = (int)g_timer.GetAppMilliTime();
				m_current = ( m_current + 1 ) % 2;
			}
			switch( Selection() )
			{
			case IF_LBUP :
				return IF_CTL_SELECT;
				break;
			default :				
				return IF_CTL_NONE;
				break;
			}
		}
		break;
	}
	return 0;
}


void CIFButtonCtl::RenderTextBtn()
{
	int x, y, xs, ys;

	if( !State() ) return;

	g_ifMng->SetRenderState();
	Render();
	
	if( m_str[0] == 0 ) return;

	x = m_info[0].clientX;
	y = m_info[0].clientY;
	xs = m_info[0].sizeX;
	ys = m_info[0].sizeY;

	y += ys / 2 - 6;

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		g_chatManager->SetColor( m_color );
		g_chatManager->TextOut( x, y, m_str, CHAT_ARRANGE_CENTER, xs );
	}
	g_chatManager->EndFrame();

	g_ifMng->SetRenderState();
}

void CIFButtonCtl::SetButtonText( char *str, DWORD color )
{
	strcpy( m_str, str );
	m_color = color;
}


void CIFButtonCtl::SetFlickerMode( bool mode )
{
	if( mode )
		m_flickerMode = 1;
	else
		m_flickerMode = 0;
}


