



#include "../global_def.h"
#include "IFCommon.h"





CIFBarCtl::CIFBarCtl( CIFControlNode *pParentControl ) : CIFControlNode( pParentControl )
{
	m_bar = new CIFControl( this );
	m_emptyBar = new CIFControl( this );

	m_barID = -1;
	m_emptyBarID = -1;

	m_curPos = 1;
	m_maxPos = 1;

	m_barPosX = 0;

	m_state = true;
}

CIFBarCtl::~CIFBarCtl()
{
	GTH_SAFE_DELETE( m_bar );
	GTH_SAFE_DELETE( m_emptyBar );
}

void CIFBarCtl::SetDevice( LPDIRECT3DDEVICE8 pD3DDevice )
{
	m_bar->SetDevice( pD3DDevice );
	m_emptyBar->SetDevice( pD3DDevice );

	CIFControlNode::SetDevice( pD3DDevice );
}

void CIFBarCtl::SetTexManager( CD3DTextureMng *texMng )
{
	m_bar->SetTexManager( texMng );
	m_emptyBar->SetTexManager( texMng );

	CIFControlNode::SetTexManager( texMng );
}

void CIFBarCtl::PrepareControl( int barID, int emptyBarID )
{
	m_barID = barID;
	m_emptyBarID = emptyBarID;

	m_bar->PrepareControl( barID );
	if( m_emptyBarID >= 0 ) m_emptyBar->PrepareControl( m_emptyBarID );

	m_barSize = m_bar->m_info[0].sizeX;
	m_barPosX = m_bar->m_info[0].clientX;
}

void CIFBarCtl::UpdateControl()
{
	m_bar->UpdateControl();
	if( m_emptyBarID >= 0 ) m_emptyBar->UpdateControl();

	CIFControlNode::UpdateControl();
}

	
void CIFBarCtl::PrepareBuffer()
{
	m_bar->PrepareBuffer();
	m_emptyBar->PrepareBuffer();

	CIFControlNode::PrepareBuffer();
}

void CIFBarCtl::UpdateBuffer()
{
	m_bar->UpdateBuffer();
	if( m_emptyBarID >= 0 ) m_emptyBar->UpdateBuffer();

	CIFControlNode::UpdateBuffer();
}

void CIFBarCtl::Release()
{
	m_bar->Release();
	m_emptyBar->Release();

	CIFControlNode::Release();
}

void CIFBarCtl::Render()
{
	if( !State() ) return;

	if( m_emptyBarID >= 0 ) m_emptyBar->Render();
	m_bar->Render();
}

void CIFBarCtl::UpdateBarState( int curPos, int maxPos )
{
	float ratio = 0.0f;

	m_curPos = curPos;
	m_maxPos = maxPos;

	if( m_maxPos > 0 ) ratio = (float)m_curPos / (float)m_maxPos;
	if( ratio < 0.0f ) ratio = 0.0f;
	if( ratio > 1.0f ) ratio = 1.0f;

	m_bar->m_info[0].sizeX = (int)( (float)m_barSize * ratio );
}

void CIFBarCtl::UpdateBarState_Inverse( int curPos, int maxPos )
{
	float ratio = 0.0f;

	m_curPos = curPos;
	m_maxPos = maxPos;

	if( m_maxPos > 0 ) ratio = (float)m_curPos / (float)m_maxPos;
	if( ratio < 0.0f ) ratio = 0.0f;
	if( ratio > 1.0f ) ratio = 1.0f;

	m_bar->m_info[0].sizeX = (int)( (float)m_barSize * ratio );

	int damageSize = m_barSize - m_bar->m_info[0].sizeX;

	m_bar->m_info[0].clientX = m_barPosX + damageSize;

}
