



#include "../global_def.h"
#include "IFCommon.h"





CIFNumCtl::CIFNumCtl()
{
	for( int i = 0; i < MAX_COUNT_IF_DAMAGE_NUMBER; i ++ )
	{
		memset( &m_dmgMsg[i], 0, sizeof( IFDmgNumber_t ) );
		m_dmgMsg[i].idx = -1;
	}
	m_nDmgMsg = 0;

	
	m_nDamageVex = 0;
	m_nNumDamage = 0;
	m_vexCount = 0;
	m_damageVexbuf = NULL;
}

CIFNumCtl::~CIFNumCtl()
{

}

void CIFNumCtl::PrepareBuffer()
{
	m_damageVexbuf = new CD3DBuffer();
	m_damageVexbuf->Create( 2000, GTH_FORMAT_XYZRHW |  GTH_FORMAT_DIFFUSE | GTH_FORMAT_TEXVERTEX(1),
		GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY );

	for( int i = IF_FONT_DAMAGE_0; i <= IF_FONT_DAMAGE_AVOID; i ++ )
	{
		m_dmgTexInfo[i].texCoordX = g_ifMng->m_iPos[i].texCoordX;
		m_dmgTexInfo[i].texCoordY = g_ifMng->m_iPos[i].texCoordY;
		m_dmgTexInfo[i].sizeX = g_ifMng->m_iPos[i].sizeX;
		m_dmgTexInfo[i].sizeY = g_ifMng->m_iPos[i].sizeY;
	}
}

void CIFNumCtl::Release()
{
	if(	m_damageVexbuf ) m_damageVexbuf->Release();
	GTH_SAFE_DELETE ( m_damageVexbuf );
}

void CIFNumCtl::Render()
{
	 
	if	(	m_vexCount	<=	0	)
		return;

	int texID;

	texID = g_ifMng->m_texInfo[3].texID;

	m_pd3dDevice->SetTexture( 0, m_texMng->GetD3DTexture( texID ) );
	m_pd3dDevice->SetStreamSource( 0, m_damageVexbuf->GetD3DVertexBuffer(), sizeof(IFTEXTUREVERTEX) );
    m_pd3dDevice->SetVertexShader( D3DFVF_IFVERTEX );
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_vexCount );

	m_damageVexbuf->Discard();
}

#define IF_DAMAGE_NUMBER_RENDER_TIME		800
#define IF_DAMAGE_NUMBER_ACCELERATION		-0.0001
#define IF_DAMAGE_NUMBER_VELOCITY			0.2
#define IF_DAMAGE_NUMBER_MAX_SIZE			2.0
#define IF_DAMAGE_NUMBER_MIN_SIZE			0.2
#define IF_DAMAGE_NUMBER_VIEW_DISTANCE		2000

void CIFNumCtl::AddDamageNumber( int idx, int type, int attackerIdx, int attackerType, int damage, int stun )
{
	int i, n;

	for( i = 0; i < MAX_COUNT_IF_DAMAGE_NUMBER; i ++ )
	{
		if( m_dmgMsg[i].idx < 0 ) break;
	}
	if( i == MAX_COUNT_IF_DAMAGE_NUMBER ) return;
	
	if( i == m_nDmgMsg ) m_nDmgMsg ++;

	n = i;

	m_dmgMsg[n].startTime = -1.0f;
	m_dmgMsg[n].receiveTime = g_timer.GetAppMilliTime();
	m_dmgMsg[n].velocity = (float)IF_DAMAGE_NUMBER_VELOCITY;
	m_dmgMsg[n].x = 0.0f;
	m_dmgMsg[n].y = 30.0f;

	m_dmgMsg[n].type = type;
	m_dmgMsg[n].idx = idx;

	m_dmgMsg[n].attackerIdx = attackerIdx;
	m_dmgMsg[n].attackerType = attackerType;

	m_dmgMsg[n].damage = damage;
	m_dmgMsg[n].stun = stun;
}

void CIFNumCtl::ReadyDamage( Fx_CHARACTER_t* character )
{
	int j;




	for( j = 0; j < m_nDmgMsg; j ++ )
	{
		if( m_dmgMsg[j].idx < 0 ) continue;
		if( m_dmgMsg[j].startTime > 0.0f ) continue;

		if( m_dmgMsg[j].attackerIdx == character->idxOnServer &&
			m_dmgMsg[j].attackerType == character->entityType )
		{
			 


			m_dmgMsg[j].startTime = g_timer.GetAppMilliTime();

			 





			break;
		}
	}
}

void CIFNumCtl::ReadyDamage( int attackerType, int attackerIdx, int defenderType, int defenderIdx )
{
	int j;

	for( j = 0; j < m_nDmgMsg; j ++ )
	{
		if( m_dmgMsg[j].idx < 0 ) continue;
		if( m_dmgMsg[j].startTime > 0.0f ) continue;

		if( m_dmgMsg[j].attackerIdx == attackerIdx &&
			m_dmgMsg[j].attackerType == attackerType )
		{
			m_dmgMsg[j].startTime = g_timer.GetAppMilliTime();
			break;
		}
	}
}

int	CIFNumCtl::CheckHitTarget( int attackerType, int attackerIdx, int defenderType, int defenderIdx )
{
	int j;

	for( j = 0; j < m_nDmgMsg; j ++ )
	{
		if( m_dmgMsg[j].idx < 0 ) continue;
		if( m_dmgMsg[j].startTime < 0.0f ) continue;

		if( m_dmgMsg[j].attackerIdx == attackerIdx &&
			m_dmgMsg[j].attackerType == attackerType &&
			m_dmgMsg[j].idx == defenderIdx &&
			m_dmgMsg[j].type == defenderType )
		{
			if( m_dmgMsg[j].damage > 0 ) return( 1 );
			else return( 0 );
		}
	}
	return( 0 );
}

void CIFNumCtl::ProcessDamage()
{
	int i, j, numCharacter;
	int alpha = 0xff;
	float temp, temp2;
	float z, w, dist;
	char str[255];
	float dmgSize = 0.0f, maxDmgSize;

	int len;
	int pos;
	float x, y, xs, ys, cy;
	float tx1, ty1, tx2, ty2;
	DWORD color;
	D3DXVECTOR3	charPos;
	Fx_CHARACTER_t *character;

	for( i = 0; i < m_nDmgMsg; i ++ )
	{
		if( m_dmgMsg[i].idx < 0 ) continue;
		if( m_dmgMsg[i].receiveTime + 5000.0f < g_timer.GetAppMilliTime() )
		{
			m_dmgMsg[i].idx = -1;
			continue;
		}
		if( m_dmgMsg[i].startTime < 0.0f ) continue;

		temp = g_timer.GetAppMilliTime() - m_dmgMsg[i].startTime;
		temp2 = m_dmgMsg[i].velocity * temp + 0.5f * temp * temp * IF_DAMAGE_NUMBER_ACCELERATION;
		m_dmgMsg[i].y = -temp2 + 60.0f;
	}

	IFTEXTUREVERTEX *pVertices = NULL;
	pVertices = (IFTEXTUREVERTEX *)m_damageVexbuf->Lock();

	for( i = 0; i < m_nDmgMsg; i ++ )
	{
		if( m_dmgMsg[i].idx < 0 ) continue;
		if( m_dmgMsg[i].startTime < 0.0f ) continue;

		character = &g_charManager->m_Characters[0];
		numCharacter = g_charManager->m_iNumCharacters;

		for ( j = 0; j < numCharacter; j++, character++) 
		{
			if( character->entityType == m_dmgMsg[i].type &&
				character->idxOnServer == m_dmgMsg[i].idx )
			{
				if( character->entityType == ENTITY_MONSTER )
				{
					color = 0x00ffffff;
				} else color = 0x00ff0000;
				break;
			}
		}
		if( j >= numCharacter ) continue;
		if( character->flag_culled ) continue;

		w = g_charManager->GetCharPanelScreenPos( character, charPos );

		x = (float)m_dmgMsg[i].x + charPos[0];
		y = (float)m_dmgMsg[i].y + charPos[1];
		z = (float)charPos[2];

		dist = GTH_GetDistance( character->position, g_pApp->m_myCharacter->position );
		dist -= g_camera.m_distance;
		if( dist > IF_DAMAGE_NUMBER_VIEW_DISTANCE ) dist = IF_DAMAGE_NUMBER_VIEW_DISTANCE;
		maxDmgSize = IF_DAMAGE_NUMBER_MIN_SIZE + IF_DAMAGE_NUMBER_MAX_SIZE * ( IF_DAMAGE_NUMBER_VIEW_DISTANCE - dist ) / IF_DAMAGE_NUMBER_VIEW_DISTANCE;

		temp = IF_DAMAGE_NUMBER_RENDER_TIME - ( g_timer.GetAppMilliTime() - m_dmgMsg[i].startTime );
		if( temp < 0.0f ) 
		{
			temp = 0.0f;
			m_dmgMsg[i].idx = -1;
			continue;
		}
		alpha = 0xff;

		temp = g_timer.GetAppMilliTime() - m_dmgMsg[i].startTime;
		temp = temp / ( IF_DAMAGE_NUMBER_RENDER_TIME * 0.1f );

		if( temp > 1.0f ) temp = 1.0f;

		dmgSize = 0.1f + maxDmgSize * temp;
		if( dmgSize > maxDmgSize ) dmgSize = maxDmgSize;

		temp = g_timer.GetAppMilliTime() - m_dmgMsg[i].startTime;

		if( temp > IF_DAMAGE_NUMBER_RENDER_TIME * 0.5f )
		{
			temp -= IF_DAMAGE_NUMBER_RENDER_TIME * 0.5f;
			temp /= IF_DAMAGE_NUMBER_RENDER_TIME * 0.5f;
			temp = 1.0f - temp;
			if( temp > 1.0f ) temp = 1.0f;
			if( temp < 0.0f ) temp = 0.0f;
			if( ( m_dmgMsg[i].attackerIdx == g_cgv.myPCIdxOnServer && m_dmgMsg[i].attackerType == ENTITY_PC ) || \
				( m_dmgMsg[i].idx == g_cgv.myPCIdxOnServer && m_dmgMsg[i].type == ENTITY_PC ) ) alpha = (int)( (float)255.0f * temp );
			else alpha = (int)( (float)128.0f * temp );
		}

		if( alpha > 0xff ) alpha = 0xff;
		else if( alpha < 0 ) 
		{
			m_dmgMsg[i].idx = -1;
			alpha = 0;
		}

		alpha <<= 24;

		color = color | alpha;

		if( m_dmgMsg[i].damage > 0 )
		{
			if( m_dmgMsg[i].stun > 0 )
			{
				if( character->entityType == ENTITY_MONSTER ) 
				{
					
					
					if( m_dmgMsg[i].stun == 1 ) sprintf( str, "c%d", m_dmgMsg[i].damage );
					else sprintf( str, "o%d", m_dmgMsg[i].damage );
				}
				else 
				{
					sprintf( str, "o%d", m_dmgMsg[i].damage );
				}
			}
			else
			{
				sprintf( str, "%d", m_dmgMsg[i].damage );
			}
		} else if( m_dmgMsg[i].damage == 0 )
		{
			if( character->entityType == ENTITY_MONSTER ) 
				sprintf( str, "m" );
			else sprintf( str, "a" );
		}
		len = strlen( str );

		for( j = 0; j < len; j ++ )
		{
			if( ( str[j] >= '0' && str[j] <= '9' ) )
			{	
				pos = str[j] - '0';
				cy = 0.0f;
				pos += IF_FONT_DAMAGE_0;
			} else
			{
				cy = 0.0f;
				if( str[j] == 'm' )
				{
					pos = IF_FONT_DAMAGE_MISS;
				} else if( str[j] == 'o' )
				{
					cy = 1.0f;
					pos = IF_FONT_DAMAGE_SHOCK;
				} else if( str[j] == 'c' )
				{
					cy = 1.0f;
					pos = IF_FONT_DAMAGE_CRITICAL;
				} else if( str[j] == 'a' )
				{
					pos = IF_FONT_DAMAGE_AVOID;
				} else continue;
			}

			ys = (float)m_dmgTexInfo[pos].sizeY;
			tx1 = (float)m_dmgTexInfo[pos].texCoordX;
			xs = (float)m_dmgTexInfo[pos].sizeX;
			ty1 = (float)m_dmgTexInfo[pos].texCoordY;

			if( cy > 0.0f ) cy = ( ys * 1.2f ) * maxDmgSize;

			tx2 = tx1 + xs;
			ty2 = ty1 + ys;
			
			tx1 /= 256.0f;
			ty1 /= 128.0f;
			tx2 /= 256.0f;
			ty2 /= 128.0f;

			xs *= dmgSize;
			ys *= dmgSize;

			if( j == 0 ) 
			{
				x -= xs / 2;
				y -= ys * maxDmgSize;
			}
			cy = y - cy;

			float tempzpos = z;
			float tempwpos = w;
			
			pVertices[m_nDamageVex].position = D3DXVECTOR4( x - 0.5f, cy + ys - 0.5f, tempzpos, tempwpos );
			pVertices[m_nDamageVex].color = color;
			pVertices[m_nDamageVex].tu = tx1;
			pVertices[m_nDamageVex].tv = ty2;
			
			m_nDamageVex ++;
			
			pVertices[m_nDamageVex].position = D3DXVECTOR4( x - 0.5f, cy - 0.5f, tempzpos, tempwpos );
			pVertices[m_nDamageVex].color = color;
			pVertices[m_nDamageVex].tu = tx1;
			pVertices[m_nDamageVex].tv = ty1;
			
			m_nDamageVex ++;
			
			pVertices[m_nDamageVex].position = D3DXVECTOR4( x + xs - 0.5f, cy + ys - 0.5f, tempzpos, tempwpos );
			pVertices[m_nDamageVex].color = color;
			pVertices[m_nDamageVex].tu = tx2;
			pVertices[m_nDamageVex].tv = ty2;
			
			m_nDamageVex ++;
			
			pVertices[m_nDamageVex].position = D3DXVECTOR4( x + xs - 0.5f, cy - 0.5f, tempzpos, tempwpos );
			pVertices[m_nDamageVex].color = color;
			pVertices[m_nDamageVex].tu = tx2;
			pVertices[m_nDamageVex].tv = ty1;
			
			m_nDamageVex ++;
			
			pVertices[m_nDamageVex].position = D3DXVECTOR4( x + xs - 0.5f, cy + ys - 0.5f, tempzpos, tempwpos );
			pVertices[m_nDamageVex].color = color;
			pVertices[m_nDamageVex].tu = tx2;
			pVertices[m_nDamageVex].tv = ty2;
			
			m_nDamageVex ++;
			
			pVertices[m_nDamageVex].position = D3DXVECTOR4( x - 0.5f, cy - 0.5f, tempzpos, tempwpos );
			pVertices[m_nDamageVex].color = color;
			pVertices[m_nDamageVex].tu = tx1;
			pVertices[m_nDamageVex].tv = ty1;
			
			m_nDamageVex ++;
			
			m_nNumDamage += 2;

			if( ( str[j] >= '0' && str[j] <= '9' ) ) x += xs + 1.0f;
		}
	}
	m_damageVexbuf->Unlock();

	m_vexCount = m_nNumDamage;
	
	m_nDamageVex = 0;
	m_nNumDamage = 0;
}
