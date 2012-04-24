



#include "../global_def.h"
#include "IFCommon.h"






CIFScrollBar::CIFScrollBar( CIFControlNode *pParentControl ) : CIFControlNode( pParentControl )
{
	m_bgBar = new CIFControl( this );
	m_bar = new CIFControl( this );
	m_upBtn = new CIFButtonCtl( this );
	m_dnBtn = new CIFButtonCtl( this );

	m_state = true;

	m_max = 5;
	m_current = 0;
	
	
	m_barType = 0;

	m_barOriginalPos = 0;
	m_barPos = 0;
	m_barSize = 0;
}

CIFScrollBar::~CIFScrollBar()
{

}

void CIFScrollBar::SetDevice( LPDIRECT3DDEVICE8 pD3DDevice )
{
	m_bgBar->SetDevice( pD3DDevice );
	m_bar->SetDevice( pD3DDevice );
	m_upBtn->SetDevice( pD3DDevice );
	m_dnBtn->SetDevice( pD3DDevice );

	CIFControlNode::SetDevice( pD3DDevice );
}

void CIFScrollBar::SetTexManager( CD3DTextureMng *texMng )
{
	m_bgBar->SetTexManager( texMng );
	m_bar->SetTexManager( texMng );
	m_upBtn->SetTexManager( texMng );
	m_dnBtn->SetTexManager( texMng );

	CIFControlNode::SetTexManager( texMng );
}

void CIFScrollBar::PrepareControl( int id )
{
	
	m_bgBar->PrepareControl( id+1 );
	m_upBtn->PrepareControl( id+2 );
	m_dnBtn->PrepareControl( id+5 );
	m_bar->PrepareControl( id+8 );

	
	if( m_barType == 0 )
	{
		m_barOriginalPos = g_ifMng->m_iPos[ m_bar->m_info[0].ID ].clientY;
		m_barSize = m_bgBar->m_info[0].sizeY - ( m_upBtn->m_info[0].sizeY + m_dnBtn->m_info[0].sizeY + m_bar->m_info[0].sizeY );
	}
	
	else
	{
		m_barOriginalPos = g_ifMng->m_iPos[ m_bar->m_info[0].ID ].clientX;
		m_barSize = m_bgBar->m_info[0].sizeX - ( m_upBtn->m_info[0].sizeX + m_dnBtn->m_info[0].sizeX + m_bar->m_info[0].sizeX );
	}
	
	SetBarPosition( m_max, m_current );

	m_bar->UpdateControl();
}

void CIFScrollBar::UpdateControl()
{
	m_bgBar->UpdateControl();
	m_bar->UpdateControl();
	m_upBtn->UpdateControl();
	m_dnBtn->UpdateControl();

	CIFControlNode::UpdateControl();
}

void CIFScrollBar::PrepareBuffer()
{
	m_bgBar->PrepareBuffer();
	m_bar->PrepareBuffer();
	m_upBtn->PrepareBuffer();
	m_dnBtn->PrepareBuffer();

	CIFControlNode::PrepareBuffer();
}

void CIFScrollBar::UpdateBuffer()
{
	m_bgBar->UpdateBuffer();
	m_bar->UpdateBuffer();
	m_upBtn->UpdateBuffer();
	m_dnBtn->UpdateBuffer();

	CIFControlNode::UpdateBuffer();
}

void CIFScrollBar::Release()
{
	m_bgBar->Release();
	m_bar->Release();
	m_upBtn->Release();
	m_dnBtn->Release();

	CIFControlNode::Release();
}

void CIFScrollBar::Render()
{
	if( !State() ) return;

	m_bgBar->Render();
	m_bar->Render();
	m_upBtn->Render();
	m_dnBtn->Render();
}

int CIFScrollBar::ProcessScrollBar()
{
	
	if( m_max > 0 )
	{
		if( m_upBtn->ProcessButton() == IF_CTL_SELECT )
		{
			
			if( m_current > 0 )
			{
				m_current--;
				
				SetBarPosition( m_max, m_current );
			}

			return IF_CTL_SELECT;
		}
		
		if( m_dnBtn->ProcessButton() == IF_CTL_SELECT )
		{
			
			if( m_current < m_max )
			{
				m_current++;

				SetBarPosition( m_max, m_current );
			}

			return IF_CTL_SELECT;
		}

		
		if( m_bar->Selection() == IF_LBDN )
		{
			m_scrollBarDragFlag = true;
		}

		if( g_input->GetLBNone() ) m_scrollBarDragFlag = false;

		if( m_scrollBarDragFlag )
		{
			int mousePos, mousePosIdx = -1;
			int scrollSize;
			float scrollUnitSize;
			int numScrollMsg;

			
			if( m_barType == 0 )
				scrollSize = m_bgBar->GetYSize() - ( m_upBtn->GetYSize() + m_dnBtn->GetYSize() + m_bar->GetYSize() );
			
			else
				scrollSize = m_bgBar->GetXSize() - ( m_upBtn->GetXSize() + m_dnBtn->GetXSize() + m_bar->GetXSize() );

			numScrollMsg = m_max;
			if( numScrollMsg <= 0 ) return IF_CTL_NONE;
			
			scrollUnitSize = (float)scrollSize / (float)numScrollMsg;

			
			if( m_barType == 0 )
			{
				mousePos = g_input->GetPosY();
				mousePos -= m_bgBar->GetYPos() + m_upBtn->GetYSize() + m_bar->GetYSize() / 2;

				for( int i=0; i<=numScrollMsg; i++ )
				{
					if( mousePos >= ( i * scrollUnitSize - m_bar->GetYSize() / 2 ) && mousePos <= ( i * scrollUnitSize + m_bar->GetYSize() / 2 ) )
						mousePosIdx = i;
				}
			}
			
			else
			{
				mousePos = g_input->GetPosX();
				mousePos -= m_bgBar->GetXPos() + m_upBtn->GetXSize() + m_bar->GetXSize() / 2;

				for( int i=0; i<=numScrollMsg; i++ )
				{
					if( mousePos >= ( i * scrollUnitSize - m_bar->GetXSize() / 2 ) && mousePos <= ( i * scrollUnitSize + m_bar->GetXSize() / 2 ) )
						mousePosIdx = i;
				}
			}

			if( mousePosIdx == -1 ) 
				return IF_CTL_NONE;

			m_current = mousePosIdx;

			if( m_current < 0 )
				m_current = 0;

			SetBarPosition( m_max, m_current );

			return( IF_CTL_SELECT );
		}
	}	

	return( IF_CTL_NONE );
}

void CIFScrollBar::SetBarPosition(int max, int cur)
{
	if( max != -1 )
		m_max = max;

	if( cur != -1 )
		m_current = cur;
	else
		m_current = 0;
	
	if( m_max < m_current )
	{
		m_current = m_max;
	}

	if( m_max != 0 )
		m_barPos = (int)( m_barSize * ( (float)m_current / (float)m_max ) );
	else
		m_barPos = 0;

	
	if( m_barType == 0 )
		g_ifMng->m_iPos[ m_bar->m_info[0].ID ].clientY = m_barOriginalPos + m_barPos;
	
	else
		g_ifMng->m_iPos[ m_bar->m_info[0].ID ].clientX = m_barOriginalPos + m_barPos;

	if( cur == -1 )
	{
		
		if( m_barType == 0 )
			g_ifMng->m_iPos[ m_bar->m_info[0].ID ].clientY = m_barOriginalPos + m_barSize;
		
		else
			g_ifMng->m_iPos[ m_bar->m_info[0].ID ].clientX = m_barOriginalPos + m_barSize;
	}

	m_bar->UpdateControl();
}

void CIFScrollBar::SetBarPositionCurrent( int cur )
{
	SetBarPosition( m_max, cur );
}

void CIFScrollBar::SetBarPositionMax()
{
	SetBarPosition( m_max, m_max );
}

