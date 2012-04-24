



#include "../global_def.h"
#include "IFCommon.h"



CIFControlNode::CIFControlNode( CIFControlNode *pParentControl )
	:	m_pParentControl( NULL ),
		m_pNextControl( NULL ),
		m_pPrevControl( NULL ),
		m_pChildControl( NULL )
{
	if( pParentControl != NULL )
		pParentControl->AddChildControl(this);
}

CIFControlNode::~CIFControlNode()
{
	if ( m_pPrevControl != NULL ) {
		m_pPrevControl->m_pNextControl = m_pNextControl;
	}

	if ( m_pNextControl != NULL ) {
		m_pNextControl->m_pPrevControl = m_pPrevControl;
	}

	if ( m_pParentControl != NULL ) {
		if ( m_pParentControl->m_pChildControl == this ) {
			m_pParentControl->m_pChildControl = m_pNextControl;
		}

		m_pParentControl = NULL;
	}

	CIFControlNode * pNext;
	CIFControlNode * pCurrent = m_pChildControl;

	while ( pCurrent != NULL ) {
		pNext = pCurrent->m_pNextControl;
		SAFE_DELETE( pCurrent );
		pCurrent = pNext;
	}
}

void	CIFControlNode::AddChildControl( CIFControlNode *pChildControl )
{
	pChildControl->m_pParentControl	= this;
	pChildControl->m_pNextControl = m_pChildControl;
	m_pChildControl = pChildControl;

	if ( pChildControl->m_pNextControl != NULL ) {
		pChildControl->m_pNextControl->m_pPrevControl = pChildControl;
	}
}



void	CIFControlNode::SetDevice( LPDIRECT3DDEVICE8 pD3DDevice )
{
	CIFControlNode * pCurrent = m_pChildControl;

	while ( pCurrent != NULL ) {
		pCurrent->SetDevice( pD3DDevice );
		pCurrent = pCurrent->m_pNextControl;
	}
}

void	CIFControlNode::SetTexManager( CD3DTextureMng *texMng )
{
	CIFControlNode * pCurrent = m_pChildControl;

	while ( pCurrent != NULL ) {
		pCurrent->SetTexManager( texMng );
		pCurrent = pCurrent->m_pNextControl;
	}
}

void	CIFControlNode::Release()
{
	CIFControlNode * pCurrent = m_pChildControl;

	while ( pCurrent != NULL ) {
		pCurrent->Release();
		pCurrent = pCurrent->m_pNextControl;
	}
}

void	CIFControlNode::UpdateControl()
{
	CIFControlNode * pCurrent = m_pChildControl;

	while ( pCurrent != NULL ) {
		pCurrent->UpdateControl();
		pCurrent = pCurrent->m_pNextControl;
	}
}

void	CIFControlNode::PrepareBuffer()
{
	CIFControlNode * pCurrent = m_pChildControl;

	while ( pCurrent != NULL ) {
		pCurrent->PrepareBuffer();
		pCurrent = pCurrent->m_pNextControl;
	}
}

void	CIFControlNode::UpdateBuffer()
{
	CIFControlNode * pCurrent = m_pChildControl;

	while ( pCurrent != NULL ) {
		pCurrent->UpdateBuffer();
		pCurrent = pCurrent->m_pNextControl;
	}
}





CIFControl::CIFControl( CIFControlNode *pParentControl ) : CIFControlNode( pParentControl )
{
	m_click = 0;
	m_current = 0;
	m_enable = 1;

	
	m_animFlag = false;

	m_vexbuf = NULL;

	m_fadeState = 0;

	for( int i = 0; i < MAX_COUNT_IF_CONTROL_SPRITE; i ++ )
	{
		memset( &m_info[i], 0, sizeof( IFControlInfo_t ) );
		m_info[i].ID = -1;
		m_info[i].parentID = -1;
		m_info[i].texIndex = -1;
	}
}

CIFControl::~CIFControl()
{

}




void	CIFControl::SetDevice( LPDIRECT3DDEVICE8 pD3DDevice )
{
	m_pd3dDevice = pD3DDevice;
	CIFControlNode::SetDevice( pD3DDevice );
}

void	CIFControl::SetTexManager( CD3DTextureMng *texMng )
{
	m_texMng = texMng;
	CIFControlNode::SetTexManager( texMng );
}

void CIFControl::PrepareBuffer()
{
	m_vexbuf = new CD3DBuffer();
	m_vexbuf->Create( 6, GTH_FORMAT_XYZRHW |  GTH_FORMAT_DIFFUSE | GTH_FORMAT_TEXVERTEX(1),
		GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY );

	CIFControlNode::PrepareBuffer();
}

void CIFControl::Release()
{
	if( m_vexbuf ) m_vexbuf->Release();
	GTH_SAFE_DELETE( m_vexbuf );

	CIFControlNode::Release();
}

void CIFControl::UpdateBuffer()
{
	float x, y, xs, ys;
	float tx1, ty1, tx2, ty2;
	int id;
	int texIdx;
	int texSizeX;
	int texSizeY;
	int curIdx;

	DWORD color;

	curIdx = m_current;
	
	texIdx = m_info[m_current].texIndex;
	id = m_info[curIdx].ID;

	if( texIdx >=0 )
	{
		texSizeX = g_ifMng->m_texInfo[texIdx].texSizeX;
		texSizeY = g_ifMng->m_texInfo[texIdx].texSizeY;
	}
	else
	{
		texSizeX = 1;
		texSizeY = 1;
	}

	x = (float)m_info[curIdx].clientX;
	y = (float)m_info[curIdx].clientY;

	xs = (float)m_info[m_current].sizeX;
	ys = (float)m_info[m_current].sizeY;

	tx1 = (float)m_info[m_current].texCoordX;
	ty1 = (float)m_info[m_current].texCoordY;

	tx2 = (float)m_info[m_current].texCoordX + xs;
	ty2 = (float)m_info[m_current].texCoordY + ys;

	tx1 /= (float)texSizeX;
	ty1 /= (float)texSizeY;
	tx2 /= (float)texSizeX;
	ty2 /= (float)texSizeY;

	if( id != -1 )
	{
		if( g_ifMng->m_iBackPos[id].clientX == IF_POSITION_FULL ) xs = (float)g_ifMng->m_clientWidth;
		if( g_ifMng->m_iBackPos[id].clientY == IF_POSITION_FULL ) ys = (float)g_ifMng->m_clientHeight;
	}


	IFTEXTUREVERTEX *pVertices = NULL;

	pVertices = (IFTEXTUREVERTEX *)m_vexbuf->Lock();

	if( !m_fadeState )
	{
		if( m_info[m_current].texIndex != -1 ) color = 0xffffffff;
		else color = 0xff000000;
	}
	else 
	{
		color = m_fadeColor;
	}

	if( texIdx == -2 ) 
	{
		color = 0xc0000000;
	}

	float tempzpos = 0.0f;
	float tempwpos = 1.0f;

	
	pVertices[0].position = D3DXVECTOR4( x - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
	pVertices[0].color = color;
	pVertices[0].tu = tx1;
	pVertices[0].tv = ty2;

	pVertices[1].position = D3DXVECTOR4( x - 0.5f, y - 0.5f, tempzpos, tempwpos );
	pVertices[1].color = color;
	pVertices[1].tu = tx1;
	pVertices[1].tv = ty1;

	pVertices[2].position = D3DXVECTOR4( x + xs - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
	pVertices[2].color = color;
	pVertices[2].tu = tx2;
	pVertices[2].tv = ty2;

	pVertices[3].position = D3DXVECTOR4( x + xs - 0.5f, y - 0.5f, tempzpos, tempwpos );
	pVertices[3].color = color;
	pVertices[3].tu = tx2;
	pVertices[3].tv = ty1;

	pVertices[4].position = D3DXVECTOR4( x + xs - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
	pVertices[4].color = color;
	pVertices[4].tu = tx2;
	pVertices[4].tv = ty2;

	pVertices[5].position = D3DXVECTOR4( x - 0.5f, y - 0.5f, tempzpos, tempwpos );
	pVertices[5].color = color;
	pVertices[5].tu = tx1;
	pVertices[5].tv = ty1;

	m_vexbuf->Unlock();

	CIFControlNode::UpdateBuffer();
}

void CIFControl::UpdateBuffer( int zx, int zy )
{
	float x, y, xs, ys;
	float tx1, ty1, tx2, ty2;
	int id;
	int texIdx;
	int texSizeX;
	int texSizeY;
	int curIdx;

	DWORD color;

	curIdx = m_current;
	
	texIdx = m_info[m_current].texIndex;
	id = m_info[curIdx].ID;

	if( texIdx >=0 )
	{
		texSizeX = g_ifMng->m_texInfo[texIdx].texSizeX;
		texSizeY = g_ifMng->m_texInfo[texIdx].texSizeY;
	}
	else
	{
		texSizeX = 1;
		texSizeY = 1;
	}

	x = (float)m_info[curIdx].clientX;
	y = (float)m_info[curIdx].clientY;

	xs = (float)m_info[m_current].sizeX;
	ys = (float)m_info[m_current].sizeY;

	tx1 = (float)m_info[m_current].texCoordX;
	ty1 = (float)m_info[m_current].texCoordY;

	tx2 = (float)m_info[m_current].texCoordX + xs;
	ty2 = (float)m_info[m_current].texCoordY + ys;

	tx1 /= (float)texSizeX;
	ty1 /= (float)texSizeY;
	tx2 /= (float)texSizeX;
	ty2 /= (float)texSizeY;

	xs = zx;
	ys = zy;

	IFTEXTUREVERTEX *pVertices = NULL;

	pVertices = (IFTEXTUREVERTEX *)m_vexbuf->Lock();

	
	if( pVertices == NULL ) return;

	if( !m_fadeState )
	{
		if( m_info[m_current].texIndex != -1 ) color = 0xffffffff;
		else color = 0xff000000;
	}
	else 
	{
		color = m_fadeColor;
	}

	if( texIdx == -2 ) 
	{
		color = 0xc0000000;
	}

	float tempzpos = 0.0f;
	float tempwpos = 1.0f;

	
	pVertices[0].position = D3DXVECTOR4( x - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
	pVertices[0].color = color;
	pVertices[0].tu = tx1;
	pVertices[0].tv = ty2;

	pVertices[1].position = D3DXVECTOR4( x - 0.5f, y - 0.5f, tempzpos, tempwpos );
	pVertices[1].color = color;
	pVertices[1].tu = tx1;
	pVertices[1].tv = ty1;

	pVertices[2].position = D3DXVECTOR4( x + xs - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
	pVertices[2].color = color;
	pVertices[2].tu = tx2;
	pVertices[2].tv = ty2;

	pVertices[3].position = D3DXVECTOR4( x + xs - 0.5f, y - 0.5f, tempzpos, tempwpos );
	pVertices[3].color = color;
	pVertices[3].tu = tx2;
	pVertices[3].tv = ty1;

	pVertices[4].position = D3DXVECTOR4( x + xs - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
	pVertices[4].color = color;
	pVertices[4].tu = tx2;
	pVertices[4].tv = ty2;

	pVertices[5].position = D3DXVECTOR4( x - 0.5f, y - 0.5f, tempzpos, tempwpos );
	pVertices[5].color = color;
	pVertices[5].tu = tx1;
	pVertices[5].tv = ty1;

	m_vexbuf->Unlock();
}


void CIFControl::Update3DBuffer( float z, float w )
{
	float x, y, xs, ys;
	float tx1, ty1, tx2, ty2;
	int id;
	int texIdx;
	int texSizeX;
	int texSizeY;
	int curIdx;

	DWORD color;

	curIdx = m_current;
	
	texIdx = m_info[m_current].texIndex;
	id = m_info[curIdx].ID;

	if( texIdx >=0 )
	{
		texSizeX = g_ifMng->m_texInfo[texIdx].texSizeX;
		texSizeY = g_ifMng->m_texInfo[texIdx].texSizeY;
	}
	else
	{
		texSizeX = 1;
		texSizeY = 1;
	}

	x = (float)m_info[curIdx].clientX;
	y = (float)m_info[curIdx].clientY;

	xs = (float)m_info[m_current].sizeX;
	ys = (float)m_info[m_current].sizeY;

	tx1 = (float)m_info[m_current].texCoordX;
	ty1 = (float)m_info[m_current].texCoordY;

	tx2 = (float)m_info[m_current].texCoordX + xs;
	ty2 = (float)m_info[m_current].texCoordY + ys;

	tx1 /= (float)texSizeX;
	ty1 /= (float)texSizeY;
	tx2 /= (float)texSizeX;
	ty2 /= (float)texSizeY;

	if( id != -1 )
	{
		if( g_ifMng->m_iBackPos[id].clientX == IF_POSITION_FULL ) xs = (float)g_ifMng->m_clientWidth;
		if( g_ifMng->m_iBackPos[id].clientY == IF_POSITION_FULL ) ys = (float)g_ifMng->m_clientHeight;
	}


	IFTEXTUREVERTEX *pVertices = NULL;

	pVertices = (IFTEXTUREVERTEX *)m_vexbuf->Lock();

	if( !m_fadeState )
	{
		if( m_info[m_current].texIndex != -1 ) color = 0xffffffff;
		else color = 0xff000000;
	}
	else 
	{
		color = m_fadeColor;
	}

	if( texIdx == -2 ) 
	{
		color = 0xc0000000;
	}

	float tempzpos = z;
	float tempwpos = w;

	
	pVertices[0].position = D3DXVECTOR4( x - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
	pVertices[0].color = color;
	pVertices[0].tu = tx1;
	pVertices[0].tv = ty2;

	pVertices[1].position = D3DXVECTOR4( x - 0.5f, y - 0.5f, tempzpos, tempwpos );
	pVertices[1].color = color;
	pVertices[1].tu = tx1;
	pVertices[1].tv = ty1;

	pVertices[2].position = D3DXVECTOR4( x + xs - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
	pVertices[2].color = color;
	pVertices[2].tu = tx2;
	pVertices[2].tv = ty2;

	pVertices[3].position = D3DXVECTOR4( x + xs - 0.5f, y - 0.5f, tempzpos, tempwpos );
	pVertices[3].color = color;
	pVertices[3].tu = tx2;
	pVertices[3].tv = ty1;

	pVertices[4].position = D3DXVECTOR4( x + xs - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
	pVertices[4].color = color;
	pVertices[4].tu = tx2;
	pVertices[4].tv = ty2;

	pVertices[5].position = D3DXVECTOR4( x - 0.5f, y - 0.5f, tempzpos, tempwpos );
	pVertices[5].color = color;
	pVertices[5].tu = tx1;
	pVertices[5].tv = ty1;

	m_vexbuf->Unlock();
}

void CIFControl::UpdateRotateBuffer( float fAngle )
{
	float xs, ys;
	float tx1, ty1, tx2, ty2;
	int id;
	int texIdx;
	int texSizeX;
	int texSizeY;
	int curIdx;
	DWORD color;

	float width= (float)m_info[0].sizeY * 0.5f;
	float x, y;
	vec3_t molt, molb, mort, morb;

	molt[0] = -width;	molt[1] = -width;		molt[2] = 0.0f;
	mort[0] = width;	mort[1] = -width;		mort[2] = 0.0f;
	morb[0] = width;	morb[1] = width;		morb[2] = 0.0f;
	molb[0] = -width;	molb[1] = width;		molb[2] = 0.0f;

	CalcZAxisRotate( fAngle , molt, mort );				
	CalcZAxisRotate( fAngle , molb, morb );				

	molt[0] += width;
	molt[1] += width;
	mort[0] += width;
	mort[1] += width;
	morb[0] += width;
	morb[1] += width;
	molb[0] += width;
	molb[1] += width;

	x = (float)m_info[0].clientX;
	y = (float)m_info[0].clientY;

	molt[0]	+= x;
	molt[1]	+= y;
	mort[0]	+= x;
	mort[1]	+= y;
	morb[0]	+= x;
	morb[1]	+= y;
	molb[0]	+= x;
	molb[1]	+= y;

	curIdx = m_current;
	
	texIdx = m_info[m_current].texIndex;
	id = m_info[curIdx].ID;

	if( texIdx >=0 )
	{
		texSizeX = g_ifMng->m_texInfo[texIdx].texSizeX;
		texSizeY = g_ifMng->m_texInfo[texIdx].texSizeY;
	}
	else
	{
		texSizeX = 1;
		texSizeY = 1;
	}

	xs = (float)m_info[m_current].sizeX;
	ys = (float)m_info[m_current].sizeY;

	tx1 = (float)m_info[m_current].texCoordX;
	ty1 = (float)m_info[m_current].texCoordY;

	tx2 = (float)m_info[m_current].texCoordX + xs;
	ty2 = (float)m_info[m_current].texCoordY + ys;

	tx1 /= (float)texSizeX;
	ty1 /= (float)texSizeY;
	tx2 /= (float)texSizeX;
	ty2 /= (float)texSizeY;

	if( id != -1 )
	{
		if( g_ifMng->m_iBackPos[id].clientX == IF_POSITION_FULL ) xs = (float)g_ifMng->m_clientWidth;
		if( g_ifMng->m_iBackPos[id].clientY == IF_POSITION_FULL ) ys = (float)g_ifMng->m_clientHeight;
	}

	IFTEXTUREVERTEX *pVertices = NULL;

	pVertices = (IFTEXTUREVERTEX *)m_vexbuf->Lock();

	if( m_info[m_current].texIndex != -1 ) color = 0xffffffff;
	else color = 0xff000000;

	if( texIdx == -2 ) 
	{
		color = 0xc0000000;
	}

	float tempzpos = 0.0f;
	float tempwpos = 1.0f;
	
	pVertices[0].position = D3DXVECTOR4( molb[0] - 0.5f, molb[1] - 0.5f, tempzpos, tempwpos );
	pVertices[0].color = color;
	pVertices[0].tu = tx1;
	pVertices[0].tv = ty2;

	pVertices[1].position = D3DXVECTOR4( molt[0] - 0.5f, molt[1] - 0.5f, tempzpos, tempwpos );
	pVertices[1].color = color;
	pVertices[1].tu = tx1;
	pVertices[1].tv = ty1;

	pVertices[2].position = D3DXVECTOR4( morb[0] - 0.5f, morb[1] - 0.5f, tempzpos, tempwpos );
	pVertices[2].color = color;
	pVertices[2].tu = tx2;
	pVertices[2].tv = ty2;

	pVertices[3].position = D3DXVECTOR4( mort[0] - 0.5f, mort[1] - 0.5f, tempzpos, tempwpos );
	pVertices[3].color = color;
	pVertices[3].tu = tx2;
	pVertices[3].tv = ty1;

	pVertices[4].position = D3DXVECTOR4( morb[0] - 0.5f, morb[1] - 0.5f, tempzpos, tempwpos );
	pVertices[4].color = color;
	pVertices[4].tu = tx2;
	pVertices[4].tv = ty2;

	pVertices[5].position = D3DXVECTOR4( molt[0] - 0.5f, molt[1] - 0.5f, tempzpos, tempwpos );
	pVertices[5].color = color;
	pVertices[5].tu = tx1;
	pVertices[5].tv = ty1;

	m_vexbuf->Unlock();
}







void CIFControl::PrepareControl( int id, int idx, float diffX, float diffY )
{
	float px, py;
	int pID = -1, cID, texIdx;
	int texSizeX;
	int texSizeY;
	int tsizex, tsizey;

	m_info[idx].ID = id;

	px = 0.0f;
	py = 0.0f;

	cID = id;
	pID = g_ifMng->m_iPos[cID].parentID;
	
	m_info[idx].parentID = pID;

	tsizex = m_info[idx].sizeX = g_ifMng->m_iPos[id].sizeX;
	tsizey = m_info[idx].sizeY = g_ifMng->m_iPos[id].sizeY;

	
	while( pID >= 0 )
	{
		px += g_ifMng->m_iPos[pID].clientX;
		py += g_ifMng->m_iPos[pID].clientY;

		cID = pID;
		pID = g_ifMng->m_iPos[cID].parentID;
	}

	
	
	g_ifMng->m_iPos[id].clientX += diffX;	
	g_ifMng->m_iPos[id].clientY += diffY;

	m_info[idx].clientX = g_ifMng->m_iPos[id].clientX + (int)px;
	m_info[idx].clientY = g_ifMng->m_iPos[id].clientY + (int)py;


	
	texIdx = g_ifMng->m_iPos[id].texIndex;
	m_info[idx].texIndex = g_ifMng->m_iPos[id].texIndex;
	
	if( texIdx != -1 )
	{
		texSizeX = g_ifMng->m_texInfo[texIdx].texSizeX;
		texSizeY = g_ifMng->m_texInfo[texIdx].texSizeY;
	}
	else
	{
		texSizeX = 1;
		texSizeY = 1;
	}

	m_info[idx].texCoordX = g_ifMng->m_iPos[id].texCoordX;
	m_info[idx].texCoordY = g_ifMng->m_iPos[id].texCoordY;
}


void CIFControl::PrepareAnimControl(int id, int numFrame, float animTime )
{
	m_animCurTime = 0.0f;
	m_animTime = animTime;
	m_numAnimFrame = numFrame;

	m_animFlag = true;

	for( int i = 0; i < numFrame; i ++ )
	{
		PrepareControl( id + i, i );
	}
}


void CIFControl::UpdateControl()
{
	if ( m_animFlag )
		UpdateAnimControl();
	else
		UpdateStopControl();
}


void CIFControl::UpdateStopControl()
{
	float px, py;
	int pID = -1, cID;
	int idx = m_current;

	px = 0.0f;
	py = 0.0f;

	
	cID = m_info[idx].ID;
	if( cID >= 0 )
	{
		pID = g_ifMng->m_iPos[cID].parentID;
		
		
		while( pID >= 0 )
		{
			px += g_ifMng->m_iPos[pID].clientX;
			py += g_ifMng->m_iPos[pID].clientY;
			
			cID = pID;
			pID = g_ifMng->m_iPos[cID].parentID;
		}
		cID = m_info[idx].ID;
		
		m_info[idx].clientX = g_ifMng->m_iPos[cID].clientX + (int)px;
		m_info[idx].clientY = g_ifMng->m_iPos[cID].clientY + (int)py;
	}

	CIFControlNode::UpdateControl();
}


void CIFControl::UpdateAnimControl()
{
	int backupCurrent = m_current;
	for( int i = 0; i < m_numAnimFrame; i ++ )
	{
		m_current = i;
		UpdateStopControl();
	}
	m_current = backupCurrent;

	m_animCurTime -= g_timer.GetElapsedMilliTime();
	if( m_animCurTime < 0.0f )
	{
		m_current ++;
		if( m_current == m_numAnimFrame ) m_current = 0;
		m_animCurTime = m_animTime;
	}
}

void CIFControl::SetFade( float fadetime, float staytime )
{
	m_fadeState = 1;
	m_fadeTime = fadetime;
	m_stayTime = staytime;
	m_fadeFlag = 0;
}

void CIFControl::ChangeControlSprite( int idx )
{
	m_old = m_current;
	m_current = idx;
}

void CIFControl::ChangeControlTexInfo( int id, int idx )
{
	m_info[idx].texIndex = g_ifMng->m_iPos[id].texIndex;
	m_info[idx].texCoordX = g_ifMng->m_iPos[id].texCoordX;
	m_info[idx].texCoordY = g_ifMng->m_iPos[id].texCoordY;
	m_info[idx].sizeX = g_ifMng->m_iPos[id].sizeX;
	m_info[idx].sizeY = g_ifMng->m_iPos[id].sizeY;
}

int CIFControl::Selection()
{
	int x1, y1, x2, y2;
	int mx, my;
	
	x1 = m_info[m_current].clientX;
	y1 = m_info[m_current].clientY;
	x2 = m_info[m_current].clientX + m_info[m_current].sizeX;
	y2 = m_info[m_current].clientY + m_info[m_current].sizeY;
	
	mx = g_input->GetPosX();
	my = g_input->GetPosY();

	if( x1 <= mx && x2 >= mx && y1 <= my && y2 >= my )
	{
		if( g_input->GetLBDown() ) return( IF_LBDN );
		else if( g_input->GetRBDown() ) return( IF_RBDN );
		else if( g_input->GetLBUp() ) return( IF_LBUP );
		else if( g_input->GetRBUp() ) return( IF_RBUP );
		else if( g_input->GetLBKeep() ) return( IF_LBKEEP );
		else if( g_input->GetRBKeep() ) return( IF_RBKEEP );
		else if( g_input->GetLBDBClk() ) return( IF_LBDBCLK );
		else if( g_input->GetRBDBClk() ) return( IF_RBDBCLK );
		else return( IF_IN_POS );
	}
	
	return( IF_NOT_IN_POS );
}





void CIFControl::Render()
{
	int texID;

	if( !State() ) return;

	
	if( m_info[m_current].texIndex >= 0 )
	{
		texID = g_ifMng->m_texInfo[m_info[m_current].texIndex].texID;
		m_pd3dDevice->SetTexture( 0, m_texMng->GetD3DTexture( texID ) );
	}
	else m_pd3dDevice->SetTexture( 0, NULL );

	m_pd3dDevice->SetStreamSource( 0, m_vexbuf->GetD3DVertexBuffer(), sizeof(IFTEXTUREVERTEX) );
    m_pd3dDevice->SetVertexShader( D3DFVF_IFVERTEX );

	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );

	m_vexbuf->Discard();
}

void CIFControl::RenderFade()
{
	float tempTime;
	int r = 0xff, g = 0xff, b = 0xff;

	switch( m_fadeFlag )
	{
	
	case 0 :
		m_fadeStart = g_timer.GetAppMilliTime();
		m_fadeFlag = 1;
		m_fadeColor = 0xff000000;
		break;
	
	case 1 :
		tempTime = g_timer.GetAppMilliTime() - m_fadeStart;
		tempTime /= m_fadeTime;
		if( tempTime > 1.0f ) 
		{
			tempTime = 1.0f;
			m_fadeStart = g_timer.GetAppMilliTime();
			m_fadeFlag = 2;
		}
		r = (int)( (float)r * tempTime );
		r <<= 16;
		g = (int)( (float)g * tempTime );
		g <<= 8;
		b = (int)( (float)b * tempTime );

		m_fadeColor = (0xff000000) | r | g | b;
		break;
	
	case 2 :
		tempTime = g_timer.GetAppMilliTime() - m_fadeStart;
		if( tempTime > m_stayTime )
		{
			m_fadeStart = g_timer.GetAppMilliTime();
			m_fadeFlag = 3;
		}
		m_fadeColor = 0xffffffff;
		break;
	
	case 3 :
		tempTime = g_timer.GetAppMilliTime() - m_fadeStart;
		tempTime /= m_fadeTime;
		tempTime = 1.0f - tempTime;
		if( tempTime < 0.0f ) 
		{
			tempTime = 0.0f;
			m_fadeStart = g_timer.GetAppMilliTime();
			m_fadeFlag = 4;
		}
		r = (int)( (float)r * tempTime );
		r <<= 16;
		g = (int)( (float)g * tempTime );
		g <<= 8;
		b = (int)( (float)b * tempTime );

		m_fadeColor = (0xff000000) | r | g | b;
		break;
	}
	UpdateBuffer();
	Render();
}

