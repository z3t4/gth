



#include "../global_def.h"
#include "IFCommon.h"





CIFSpinCtl::CIFSpinCtl( CIFControlNode *pParentControl ) : CIFControlNode( pParentControl )
{
	m_upBtn		= new CIFButtonCtl( this );
	m_downBtn	= new CIFButtonCtl( this );

	m_state		= true;
	m_min		= 0;
	m_max		= 0;
	m_current	= 0;
}

CIFSpinCtl::~CIFSpinCtl()
{

}

void CIFSpinCtl::SetDevice( LPDIRECT3DDEVICE8 pD3DDevice )
{
	m_upBtn->SetDevice( pD3DDevice );
	m_downBtn->SetDevice( pD3DDevice );

	CIFControlNode::SetDevice( pD3DDevice );
}

void CIFSpinCtl::SetTexManager( CD3DTextureMng *texMng )
{
	m_upBtn->SetTexManager( texMng );
	m_downBtn->SetTexManager( texMng );

	CIFControlNode::SetTexManager( texMng );
}

void CIFSpinCtl::PrepareControl( int id )
{
	m_upBtn->PrepareControl( id );
	m_downBtn->PrepareControl( id+3 );
}

void CIFSpinCtl::UpdateControl()
{
	m_upBtn->UpdateControl();
	m_downBtn->UpdateControl();

	CIFControlNode::UpdateControl();
}

void CIFSpinCtl::PrepareBuffer()
{
	m_upBtn->PrepareBuffer();
	m_downBtn->PrepareBuffer();

	CIFControlNode::PrepareBuffer();
}

void CIFSpinCtl::UpdateBuffer()
{
	m_upBtn->UpdateBuffer();
	m_downBtn->UpdateBuffer();

	CIFControlNode::UpdateBuffer();
}

void CIFSpinCtl::Release()
{
	m_upBtn->Release();
	m_downBtn->Release();

	CIFControlNode::Release();
}

void CIFSpinCtl::Render()
{
	if( !State() ) return;

	m_upBtn->Render();
	m_downBtn->Render();
}

int CIFSpinCtl::ProcessControl()
{
	if( m_upBtn->ProcessButton() == IF_CTL_SELECT )
	{
		m_current ++;
		
		if( m_current > m_max )
			m_current = m_min;

		return IF_CTL_SELECT;
	}
	
	if( m_downBtn->ProcessButton() == IF_CTL_SELECT )
	{
		m_current --;

		if( m_current < m_min )
			m_current = m_max;
		
		return IF_CTL_SELECT;
	}

	return IF_CTL_NONE;
}

void CIFSpinCtl::SetValueRange( int minValue, int maxValue )
{
	if( minValue > maxValue )
		return;

	m_min = minValue;
	m_max = maxValue;
}

