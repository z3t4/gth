



#include "../global_def.h"
#include "IFCommon.h"






CIFComboBox::CIFComboBox( CIFControlNode *pParentControl ) : CIFControl( pParentControl )
{
	m_combo = new CIFControl( this );
	m_comboExpansion = new CIFControl( this );
	m_comboBtn = new CIFButtonCtl( this );

	m_state = true;
	m_bExpansion = false;
	m_numString = 0;
	m_curSel = -1;
	m_tempCurSel = -1;

	for (int i=0; i<MAX_NUM_OF_SEL_STRING; i++)
	{
		memset( m_string[i], 0, sizeof( chatMessage ) );
		m_strColor[i] = 0;
	}

}

CIFComboBox::~CIFComboBox()
{
	GTH_SAFE_DELETE( m_combo );
	GTH_SAFE_DELETE( m_comboExpansion );
	GTH_SAFE_DELETE( m_comboBtn );
}


void CIFComboBox::SetDevice( LPDIRECT3DDEVICE8 pD3DDevice )
{
	m_combo->SetDevice( pD3DDevice );
	m_comboExpansion->SetDevice( pD3DDevice );
	m_comboBtn->SetDevice( pD3DDevice );
	CIFControlNode::SetDevice( pD3DDevice );
}

void CIFComboBox::SetTexManager( CD3DTextureMng *texMng )
{
	m_combo->SetTexManager( texMng );
	m_comboExpansion->SetTexManager( texMng );
	m_comboBtn->SetTexManager( texMng );
	CIFControlNode::SetTexManager( texMng );
}

void CIFComboBox::PrepareControl( int comboID, int size )
{
	
	m_combo->PrepareControl( comboID );
	
	
	m_size = size;
	

	
	m_combo->m_info[0].sizeX = g_ifMng->m_iPos[IF_SERVER_BTN_0].sizeX;
	m_combo->m_info[0].sizeY = 18;

	m_combo->m_info[0].texCoordX = g_ifMng->m_iPos[IF_SERVER_BTN_0].texCoordX;
	m_combo->m_info[0].texCoordY = g_ifMng->m_iPos[IF_SERVER_BTN_0].texCoordY;
	m_combo->m_info[0].texIndex = g_ifMng->m_iPos[IF_SERVER_BTN_0].texIndex;
	
	
	m_comboBtn->PrepareControl( IF_CHARMAKE_WIN_STAT_FORCE_DN_BTN_0 );
	
	m_comboExpansion->PrepareControl( IF_COMBO_EXPANSION_BACKGROUND );
}

void CIFComboBox::UpdateControl()
{
	m_combo->UpdateControl();
	CIFControlNode::UpdateControl();
}

void CIFComboBox::PrepareBuffer()
{
	m_combo->PrepareBuffer();
	m_comboExpansion->PrepareBuffer();
	m_comboBtn->PrepareBuffer();
	
	CIFControlNode::PrepareBuffer();
}

void CIFComboBox::UpdateBuffer()
{
	if( m_bExpansion )
	{
		
		m_comboExpansion->m_info[0].sizeX = m_size;

		if ( m_numString > 0 )
			m_comboExpansion->m_info[0].sizeY = COMBO_EXPANSION_HEIGHT *  m_numString  ;
		else
			m_comboExpansion->m_info[0].sizeY = 0;

		m_comboExpansion->m_info[0].clientX = m_combo->m_info[0].clientX;
		m_comboExpansion->m_info[0].clientY = m_combo->m_info[0].clientY + 18;

		m_comboExpansion->UpdateBuffer( m_size, COMBO_EXPANSION_HEIGHT * m_numString );
	}

	m_comboBtn->UpdateClientPosition( m_combo->m_info[0].clientX + m_size, m_combo->m_info[0].clientY );

	CIFControlNode::UpdateBuffer();

	
	m_combo->UpdateBuffer( m_size, 18 );
}

void CIFComboBox::Release()
{
	m_combo->Release();
	m_comboExpansion->Release();
	m_comboBtn->Release();

	CIFControlNode::Release();
}

void CIFComboBox::Render()
{
	if( !State() ) return;

	m_combo->Render();
	m_comboBtn->Render();
	if( m_bExpansion ) m_comboExpansion->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );

	g_chatManager->SetReverseColor( 0xff808080 );
	g_chatManager->BeginFrame();
	{
		int x, y;

		if( m_curSel >= 0 )
		{
			x = m_combo->m_info[0].clientX + 5;
			y = m_combo->m_info[0].clientY + 4;

			g_chatManager->SetIndexedColor( m_strColor[m_curSel] );
			g_chatManager->Output( x, y, m_string[m_curSel] );
		}
		if( m_bExpansion )
		{
			x = m_comboExpansion->m_info[0].clientX + 5;
			y = m_comboExpansion->m_info[0].clientY + 2;

			for( int i = 0; i < m_numString; i ++ )
			{
				g_chatManager->SetIndexedColor( m_strColor[i] );
				if( i == m_tempCurSel ) 
					g_chatManager->Output( x, y + i * COMBO_EXPANSION_HEIGHT, m_string[i], true );
				else 
					g_chatManager->Output( x, y + i * COMBO_EXPANSION_HEIGHT, m_string[i], false );
			}
		}
	}
	g_chatManager->EndFrame();

	
	g_chatManager->SetColor( 0xffffffff );		
	g_chatManager->SetReverseColor( 0xd0000000 );

	g_ifMng->SetRenderState();
}

int CIFComboBox::ProcessComboBox()
{

	int mx, my;
	mx = g_input->GetPosX();
	my = g_input->GetPosY();
	mx -= m_comboExpansion->m_info[0].clientX;
	my -= m_comboExpansion->m_info[0].clientY;

	if ( m_bExpansion )
	{
		if ( g_input->GetLBUp() )
		{
			m_bExpansion = false;
			if ( m_curSel == m_tempCurSel )
				return( IF_CTL_NONE );

			m_curSel = m_tempCurSel;
			return( IF_CTL_EVENT );
		}

		if( mx < 0 || my < 0 || mx > m_combo->m_info[0].sizeX || my >= m_numString * COMBO_EXPANSION_HEIGHT )
		{
			if( g_input->GetLBDown() )
			{
				m_bExpansion = false;
			}

			return ( IF_CTL_NONE );
		}

		my /= COMBO_EXPANSION_HEIGHT;
		m_tempCurSel = my;

		if ( g_input->GetLBDown() )
		{
			return ( IF_CTL_NONE );
		}

	}
	else
	{
		if ( m_comboBtn->ProcessButton() == IF_CTL_SELECT )
		{
			if ( m_numString > 0 )
			{
				m_bExpansion = true;
				m_tempCurSel = m_curSel;
			}
			
			return( IF_CTL_SELECT );
		}
	}

	return( IF_CTL_NONE );
}


int CIFComboBox::AddString( char *str, int color )
{
	if( m_numString >= 64 ) return -1;
	
	strcpy( m_string[m_numString], str );

	
	m_strColor[m_numString] = color;

	return( m_numString ++ );
}

int CIFComboBox::ReadString( char *str, int sel )
{
	strcpy( str, m_string[sel] );
	return 0;
}

int CIFComboBox::ReadCurrentString( char *str )
{
	strcpy( str, m_string[m_curSel] );
	return 0;
}

int CIFComboBox::SetCurSel( int sel )
{
	int oldSel = m_curSel;
	m_curSel = sel;
	if( m_curSel >= m_numString ) m_curSel = m_numString - 1;
	
	return( oldSel );
}

int CIFComboBox::GetCurSel()
{
	return( m_curSel );
}


void CIFComboBox::ClearString()
{
	memset( m_string, 0, sizeof(m_string) );
	
	m_bExpansion = false;
	m_numString = 0;
	m_curSel = -1;
	m_tempCurSel = -1;
}


void CIFComboBox::SetItemData( int sel, DWORD data )
{
	m_itemData[sel] = data;
}

DWORD CIFComboBox::GetItemData( int sel )
{
	return( m_itemData[sel] );
}
