



#include "../global_def.h"
#include "IFCommon.h"





CIFIconCtl::CIFIconCtl()
{
	m_nVexCount = 0;
	m_nIconInfo = 0;
	m_vexbuf = NULL;
}

CIFIconCtl::~CIFIconCtl()
{

}

void CIFIconCtl::LoadConfig( char* filename )
{
	
	for( int i = 0; i < MAX_COUNT_IF_ICON_EACH_TEXTURE; i ++ )
	{
		m_itemIconTexInfo[i].group	= -1;
		m_skillIconTexInfo[i].group = -1;
		m_etcIconTexInfo[i].group	= -1;
	}

	
	strcpy( m_itemIconTexInfo[0].filename, "itemicon.dds" );
	m_itemIconTexInfo[0].texSizeX = 512;
	m_itemIconTexInfo[0].texSizeY = 512;
	m_itemIconTexInfo[0].group = 0;
	strcpy( m_itemIconTexInfo[1].filename, "itemicon2.dds" );
	m_itemIconTexInfo[1].texSizeX = 512;
	m_itemIconTexInfo[1].texSizeY = 512;
	m_itemIconTexInfo[1].group = 0;

	strcpy( m_itemIconTexInfo[2].filename, "itemicon3.dds" );
	m_itemIconTexInfo[2].texSizeX = 512;
	m_itemIconTexInfo[2].texSizeY = 512;
	m_itemIconTexInfo[2].group = 0;
	
	
	strcpy( m_itemIconTexInfo[3].filename, "itemicon4.dds" );
	m_itemIconTexInfo[3].texSizeX = 512;
	m_itemIconTexInfo[3].texSizeY = 512;
	m_itemIconTexInfo[3].group = 0;
	
	
	

	
	strcpy( m_skillIconTexInfo[0].filename, "skillicon.dds" );
	m_skillIconTexInfo[0].texSizeX = 512;
	m_skillIconTexInfo[0].texSizeY = 512;
	m_skillIconTexInfo[0].group = 0;
	strcpy( m_skillIconTexInfo[1].filename, "skillicon2.dds" );
	m_skillIconTexInfo[1].texSizeX = 512;
	m_skillIconTexInfo[1].texSizeY = 512;
	m_skillIconTexInfo[1].group = 0;
}

void CIFIconCtl::AddTextureToManager()
{
	int i;
	for( i = 0; i < MAX_COUNT_IF_ICON_EACH_TEXTURE; i ++ )
	{
		if( m_itemIconTexInfo[i].group < 0 ) continue;
		m_texMng->Add( m_itemIconTexInfo[i].filename, OWNER_TYPE_INTERFACE );
	}
	for( i = 0; i < MAX_COUNT_IF_ICON_EACH_TEXTURE; i ++ )
	{
		if( m_skillIconTexInfo[i].group < 0 ) continue;
		m_texMng->Add( m_skillIconTexInfo[i].filename, OWNER_TYPE_INTERFACE );
	}
	for( i = 0; i < MAX_COUNT_IF_ICON_EACH_TEXTURE; i ++ )
	{
		if( m_etcIconTexInfo[i].group < 0 ) continue;
		m_texMng->Add( m_etcIconTexInfo[i].filename, OWNER_TYPE_INTERFACE );
	}
}

void CIFIconCtl::UpdateTextureID()
{
	int i;
	for( i = 0; i < MAX_COUNT_IF_ICON_EACH_TEXTURE; i ++ )
	{
		if( m_itemIconTexInfo[i].group < 0 ) continue;
		m_itemIconTexInfo[i].texID = m_texMng->Search( m_itemIconTexInfo[i].filename, OWNER_TYPE_INTERFACE );
	}
	for( i = 0; i < MAX_COUNT_IF_ICON_EACH_TEXTURE; i ++ )
	{
		if( m_skillIconTexInfo[i].group < 0 ) continue;
		m_skillIconTexInfo[i].texID = m_texMng->Search( m_skillIconTexInfo[i].filename, OWNER_TYPE_INTERFACE );
	}
	for( i = 0; i < MAX_COUNT_IF_ICON_EACH_TEXTURE; i ++ )
	{
		if( m_etcIconTexInfo[i].group < 0 ) continue;
		m_etcIconTexInfo[i].texID = m_texMng->Search( m_etcIconTexInfo[i].filename, OWNER_TYPE_INTERFACE );
	}
}
	
void CIFIconCtl::PrepareBuffer()
{
	m_vexbuf = new CD3DBuffer();
	m_vexbuf->Create( MAX_COUNT_IF_ICON_VERTEX_BUFFER, GTH_FORMAT_XYZRHW |  GTH_FORMAT_DIFFUSE | GTH_FORMAT_TEXVERTEX(1),
		GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY );
}

void CIFIconCtl::Release()
{
	if( m_vexbuf ) m_vexbuf->Release();
	GTH_SAFE_DELETE( m_vexbuf );
}

void CIFIconCtl::UpdateBuffer()
{
	int n = m_nIconInfo, texIdx, i, nVex2 = 0;
	float x, y, xs, ys;
	float tx1, ty1, tx2, ty2;
	DWORD color;
	IFTEXTUREVERTEX *pVertices = NULL;
	pVertices = (IFTEXTUREVERTEX *)m_vexbuf->Lock();


	float tempzpos = 0.0f;
	float tempwpos = 1.0f;

	for( i = 0; i < n; i ++ )
	{
		x = (float)m_iconInfo[i].x;
		y = (float)m_iconInfo[i].y;

		texIdx = m_iconInfo[i].texIdx;

		color = m_iconInfo[i].color;

		tx1 = (float)m_iconInfo[i].texCoordX;
		ty1 = (float)m_iconInfo[i].texCoordY;

		xs = m_iconInfo[i].xs;
		ys = m_iconInfo[i].ys;

		tx2 = tx1 + xs;
		ty2 = ty1 + ys;

		tx1 /= 512.0f;
		ty1 /= 512.0f;
		tx2 /= 512.0f;
		ty2 /= 512.0f;

		pVertices[m_nVexCount].position = D3DXVECTOR4( x - 0.5f, y - 0.5f, tempzpos, tempwpos );
		pVertices[m_nVexCount].color = color;
		pVertices[m_nVexCount].tu = tx1;
		pVertices[m_nVexCount].tv = ty1;
		m_nVexCount ++;

		pVertices[m_nVexCount].position = D3DXVECTOR4( x + xs - 0.5f, y - 0.5f, tempzpos, tempwpos );
		pVertices[m_nVexCount].color = color;
		pVertices[m_nVexCount].tu = tx2;
		pVertices[m_nVexCount].tv = ty1;	
		m_nVexCount ++;

		pVertices[m_nVexCount].position = D3DXVECTOR4( x + xs - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
		pVertices[m_nVexCount].color = color;
		pVertices[m_nVexCount].tu = tx2;
		pVertices[m_nVexCount].tv = ty2;
		m_nVexCount ++;

		pVertices[m_nVexCount].position = D3DXVECTOR4( x - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
		pVertices[m_nVexCount].color = color;
		pVertices[m_nVexCount].tu = tx1;
		pVertices[m_nVexCount].tv = ty2;
		m_nVexCount ++;

	}
	m_vexbuf->Unlock();
}

void CIFIconCtl::Draw( int x, int y, int type, int idx, float curCooltime, float maxCooltime, DWORD color )
{
	int n = m_nIconInfo;
	int iconIdx = idx % 256;

	m_iconInfo[n].x = (float)x;
	m_iconInfo[n].y = (float)y;
	m_iconInfo[n].xs = 32.0f;
	m_iconInfo[n].ys = 32.0f;

	switch( type )
	{
	case IF_ICON_ITEM :
		m_iconInfo[n].texIdx = m_itemIconTexInfo[idx / 256].texID;
		break;
	case IF_ICON_SKILL :
		m_iconInfo[n].texIdx = m_skillIconTexInfo[idx / 256].texID;
		break;
	case IF_ICON_ETC :
		m_iconInfo[n].texIdx = m_etcIconTexInfo[idx / 256].texID;
		break;
	}

	m_iconInfo[n].texCoordX = (float)( ( iconIdx % 16 ) * 32 );
	m_iconInfo[n].texCoordY = (float)( ( iconIdx / 16 ) * 32 );

	m_iconInfo[n].color = 0xffffffff;

	n ++;

	if( maxCooltime > 0.0f && curCooltime > 0.0f )
	{
		m_iconInfo[n].x = x;
		m_iconInfo[n].y = y;
		m_iconInfo[n].texIdx = m_skillIconTexInfo[0].texID;
		
		float rsize = 32.0f * ( curCooltime / maxCooltime );

		if( rsize > 32.0f ) rsize = 32.0f;
		if( rsize < 0.0f ) rsize = 0.0f;

		m_iconInfo[n].x += ( 32.0f - rsize );
	
		m_iconInfo[n].xs = rsize;
		m_iconInfo[n].ys = 32.0f;
		
		m_iconInfo[n].color = color;

		m_iconInfo[n].texCoordX = 0.0f;
		m_iconInfo[n].texCoordY = 0.0f;

		n ++;
	}

	m_nIconInfo = n;
}



void CIFIconCtl::DrawPart( int x, int y, int stancil_x, int stancil_y, int type, int idx, float curCooltime, float maxCooltime, DWORD color )
{
	int n = m_nIconInfo;
	int iconIdx = idx % 256;

	int diff_x, diff_y;

	diff_x = stancil_x - x;
	diff_y = stancil_y - y;

	if ( diff_x >= 0 && diff_x < 32 )
	{
		m_iconInfo[n].x = (float)stancil_x;
		m_iconInfo[n].xs = 32.0f - (float)diff_x;
	}
	else if ( diff_x >= 32 || diff_x <= -32 )
	{
		m_iconInfo[n].x = (float)x;
		m_iconInfo[n].xs = 0.0f;
	}
	else
	{
		m_iconInfo[n].x = (float)x;
		m_iconInfo[n].xs = 32.0f + (float)diff_x;
	}

	if ( diff_y >= 0 && diff_y < 32 )
	{
		m_iconInfo[n].y = (float)stancil_y;
		m_iconInfo[n].ys = 32.0f - (float)diff_y;
	}
	else if ( diff_y >= 32 || diff_y <= -32 )
	{
		m_iconInfo[n].y = (float)y;
		m_iconInfo[n].ys = 0.0f;
	}
	else
	{
		m_iconInfo[n].y = (float)y;
		m_iconInfo[n].ys = 32.0f + (float)diff_y;
	}

	switch( type )
	{
	case IF_ICON_ITEM :
		m_iconInfo[n].texIdx = m_itemIconTexInfo[idx / 256].texID;
		break;
	case IF_ICON_SKILL :
		m_iconInfo[n].texIdx = m_skillIconTexInfo[idx / 256].texID;
		break;
	case IF_ICON_ETC :
		m_iconInfo[n].texIdx = m_etcIconTexInfo[idx / 256].texID;
		break;
	}

	if ( diff_x > 0 && diff_x < 32 )
		m_iconInfo[n].texCoordX = (float)( ( iconIdx % 16 ) * 32 + diff_x );
	else
		m_iconInfo[n].texCoordX = (float)( ( iconIdx % 16 ) * 32 );

	if ( diff_y > 0 && diff_y < 32 )
		m_iconInfo[n].texCoordY = (float)( ( iconIdx / 16 ) * 32 + diff_y );
	else
		m_iconInfo[n].texCoordY = (float)( ( iconIdx / 16 ) * 32 );

	m_iconInfo[n].color = 0xffffffff;

	n ++;

	if( maxCooltime > 0.0f && curCooltime > 0.0f )
	{
		m_iconInfo[n].x = x;
		m_iconInfo[n].y = y;
		m_iconInfo[n].texIdx = m_skillIconTexInfo[0].texID;
		
		float rsize = 32.0f * ( curCooltime / maxCooltime );

		if( rsize > 32.0f ) rsize = 32.0f;
		if( rsize < 0.0f ) rsize = 0.0f;

		m_iconInfo[n].x += ( 32.0f - rsize );
	
		m_iconInfo[n].xs = rsize;
		m_iconInfo[n].ys = 32.0f;
		
		m_iconInfo[n].color = color;

		m_iconInfo[n].texCoordX = 0.0f;
		m_iconInfo[n].texCoordY = 0.0f;

		n ++;
	}

	m_nIconInfo = n;

}

void CIFIconCtl::BeginFrame()
{
	m_nVexCount = 0;
	m_nIconInfo = 0;
}

void CIFIconCtl::EndFrame()
{
	int curTexID, oldTexID = -1, i;
	UpdateBuffer();

	m_pd3dDevice->SetStreamSource( 0, m_vexbuf->GetD3DVertexBuffer(), sizeof(IFTEXTUREVERTEX) );
    m_pd3dDevice->SetVertexShader( D3DFVF_IFVERTEX );

	for( i = 0; i < m_nVexCount / 4; i ++ )
	{
		curTexID = m_iconInfo[i].texIdx;
		
		if( curTexID != oldTexID )
		{
			oldTexID = curTexID;
			m_pd3dDevice->SetTexture( 0, m_texMng->GetD3DTexture( curTexID ) );
		}

		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, i * 4, 2 );
	}

	m_vexbuf->Discard();
}

