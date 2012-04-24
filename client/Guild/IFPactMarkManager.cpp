#include "../global_def.h"
#include "IFPactMarkManager.h"


const char* CIFPMManager::IFPM_ERROR_MESSAGES[]	=
{
	"No errors occured.",
	"Failed to get Device Caps. Couldn't find out the device's maximum texture size.",
	"Failed to open BMP file.",
	"Failed to identify the loaded BMP file size.",
	"Failed to read BMP data from file.",
	"Loaded BMP file is not 24-bit color format.",
	"Required texture size exceeds device's maximum texture size. Can't make texture atlas.",
	"Failed to create D3D texture.",
	"Item for this identifier already exists.",
	"Failed to allocate enough memory.",
	"Number of sub-textures in texture atlas exceeds maximum capacity of atlas.",
	"Failed to lock texture atlas.",
	"Failed to unlock texture atlas.",
	"Tried to draw unregistered item.",
	"Failed to create vertex buffer.",
	"Failed to create index buffer.",
	"Failed to lock index buffer.",
	"Failed to unlock index buffer.",
	"Failed to lock vertex buffer.",
	"Failed to unlock vertex buffer.",
	"Failed on DrawIndexedPrimitive.",
};


 
 
 
 
 
 
CIFPMManager::CIFPMManager	()
{
	InitializePactTextureMembers();
}

 
 
 
 
 
 
CIFPMManager::~CIFPMManager	()
{
	CleanupPactMarksMemory();
}

 
 
 
 
 
 
void	CIFPMManager::InitializePactTextureMembers	()
{
	m_maxDeviceTextureWidth		=	0;
	m_maxDeviceTextureHeight	=	0;
	m_pactTextureWidth			=	0;
	m_pactTextureHeight			=	0;
	m_pactTextureAtlasWidth		=	0;
	m_pactTextureAtlasHeight	=	0;
	
	m_textureForPact	=	NULL;
	m_vbPactMark	=	NULL;
	m_ibPactMark	=	NULL;

	m_itemCountForOneDimension	=	0;

	m_curPactItemColumnOffset	=	0;
	m_curPactItemRowOffset		=	0;

	m_bufferForRestore	=	NULL;

	m_numMarksToDraw = 0;

	
	
}

 
 
 
 
 
 
bool	CIFPMManager::GetMaxDeviceTextureSize	(	DWORD	&out_width,	
													DWORD	&out_height	)
{
	HRESULT		hr;
	D3DCAPS8	devCaps;
	
	if	(	FAILED( hr = g_d3dDevice.GetD3DDevice()->GetDeviceCaps( &devCaps ) )	)
	{
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_GET_DEVICE_CAPS;
		return	false;
	}
	
	out_height	=	devCaps.MaxTextureHeight;
	out_width	=	devCaps.MaxTextureWidth;

	return	true;
}







 
 
 
 
 
 

bool	CIFPMManager::CreateTextureForPact	(	int	in_numMarksToCreate,
													int	in_textureWidth,
													int	in_textureHeight	)
{
#define	POW_SIZE(a)		(int)pow( ( 1<<(a) ), 2 )
	static const int	predefSizeArraySize	=	14;
	static const int	predefSizeArray[predefSizeArraySize]	=	{	POW_SIZE(0),	POW_SIZE(1),	POW_SIZE(2),	POW_SIZE(3),	POW_SIZE(4),	
																		POW_SIZE(5),	POW_SIZE(6),	POW_SIZE(7),	POW_SIZE(8),	POW_SIZE(9),	
																		POW_SIZE(10),	POW_SIZE(11),	POW_SIZE(12),	POW_SIZE(13)		};
#undef	POW_SIZE

	DWORD	maxTextureWidth,	maxTextureHeight;
	if	(	! GetMaxDeviceTextureSize( maxTextureWidth, maxTextureHeight )	)
		return	false;

	int		requiredSize	=	in_numMarksToCreate * in_textureWidth * in_textureHeight;

	int		itemCountForOneDimension;
	for	(	itemCountForOneDimension	=	0;	\
			itemCountForOneDimension	<	predefSizeArraySize;	\
			++itemCountForOneDimension	)
	{
		if	(	requiredSize	<	predefSizeArray[ itemCountForOneDimension ]	)
			break;
	}
	
	
	int		textureSize	=	1 << itemCountForOneDimension;

	if	(	textureSize	>	maxTextureWidth	)
	{
		
		m_errorIndex	=	IFPM_ERROR_TEXTURE_SIZE_EXCEED_DEVICE_CAP;
		return	false;
	}
		

	m_bufferForRestore	=	new	WORD[ textureSize * textureSize ];
	memset( m_bufferForRestore, 0, sizeof(WORD) * textureSize * textureSize ); 
	if	(	! m_bufferForRestore	)
	{
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_ALLOCATE_MEMORY;
		return	false;
	}

	m_pactTextureWidth	=	in_textureWidth;
	m_pactTextureHeight	=	in_textureHeight;
	m_pactTextureAtlasWidth		=	textureSize;
	m_pactTextureAtlasHeight	=	textureSize;

	m_itemCountForOneDimension	=	textureSize / in_textureWidth;

	return	true;
}




 
 
 
 
 
 
bool	CIFPMManager::CalculateTextureCoordsForPactInfo	(	PACTMARKINFO	*in_info	)
{
	float	rcpPactTextureAtlasWidth	=	1.0f / (float)m_pactTextureAtlasWidth;
	float	halfPixelWidthSize	=	0.5f * rcpPactTextureAtlasWidth;
	float	intermediate	=	m_pactTextureWidth * rcpPactTextureAtlasWidth;
	in_info->texWidthStart	=	m_curPactItemColumnOffset * intermediate + halfPixelWidthSize;
	in_info->texWidthEnd	=	( m_curPactItemColumnOffset + 1 ) * intermediate - halfPixelWidthSize;

	float	rcpPactTextureAtlasHeight	=	1.0f / (float)m_pactTextureAtlasHeight;
	float	halfPixelHeightSize	=	0.5f * rcpPactTextureAtlasHeight;
	intermediate	=	m_pactTextureHeight * rcpPactTextureAtlasHeight;
	in_info->texHeightStart	=	m_curPactItemRowOffset * intermediate + halfPixelHeightSize;
	in_info->texHeightEnd	=	( m_curPactItemRowOffset + 1 ) * intermediate - halfPixelHeightSize;

	++m_curPactItemColumnOffset;

	if	(	m_curPactItemColumnOffset	==	m_itemCountForOneDimension	)
	{
		m_curPactItemColumnOffset	=	0;
		++m_curPactItemRowOffset;
		if	(	m_curPactItemRowOffset	==	m_itemCountForOneDimension	)
		{
			
			m_errorIndex	=	IFPM_ERROR_SUB_TEXTURE_EXCEED_MAX_CAP;
			return	false;
		}
	}

	return	true;
}








 
 
 
 
 
 
BOOL	CIFPMManager::
RegisterPactMarkDrawee(
	const float	in_screenX,
	const float	in_screenY,
	const float	in_depthZ,
	const float	in_depthW,	
	const int		in_identifier)
{
	if(m_numMarksToDraw >= MAXDRAWNUMperRENDER) return FALSE;

	ITOR_MAP_PACTMARKINFO	itor;
	itor	=	m_pactMarkInfoMap.find( in_identifier );
	if(itor	==	m_pactMarkInfoMap.end()){
		 
		m_errorIndex	=	IFPM_ERROR_TRIED_TO_DRAW_UNREGISTERED_ITEM;
		return	FALSE;
	}

	PACTMARKINFO	*info	=	itor->second;

	m_lockedVertexBuffer->pos.x	=	in_screenX;
	m_lockedVertexBuffer->pos.y	=	in_screenY;
	m_lockedVertexBuffer->pos.z	=	in_depthZ;
	m_lockedVertexBuffer->pos.w	=	in_depthW;
	m_lockedVertexBuffer->tu	=	info->texWidthStart;
	m_lockedVertexBuffer->tv	=	info->texHeightStart;
	m_lockedVertexBuffer++;

	m_lockedVertexBuffer->pos.x	=	in_screenX + m_pactTextureWidth;
	m_lockedVertexBuffer->pos.y	=	in_screenY;
	m_lockedVertexBuffer->pos.z	=	in_depthZ;
	m_lockedVertexBuffer->pos.w	=	in_depthW;
	m_lockedVertexBuffer->tu	=	info->texWidthEnd;
	m_lockedVertexBuffer->tv	=	info->texHeightStart;
	m_lockedVertexBuffer++;

	m_lockedVertexBuffer->pos.x	=	in_screenX;
	m_lockedVertexBuffer->pos.y	=	in_screenY + m_pactTextureHeight;
	m_lockedVertexBuffer->pos.z	=	in_depthZ;
	m_lockedVertexBuffer->pos.w	=	in_depthW;
	m_lockedVertexBuffer->tu	=	info->texWidthStart;
	m_lockedVertexBuffer->tv	=	info->texHeightEnd;
	m_lockedVertexBuffer++;

	m_lockedVertexBuffer->pos.x	=	in_screenX + m_pactTextureWidth;
	m_lockedVertexBuffer->pos.y	=	in_screenY + m_pactTextureHeight;
	m_lockedVertexBuffer->pos.z	=	in_depthZ;
	m_lockedVertexBuffer->pos.w	=	in_depthW;
	m_lockedVertexBuffer->tu	=	info->texWidthEnd;
	m_lockedVertexBuffer->tv	=	info->texHeightEnd;
	m_lockedVertexBuffer++;
	m_numMarksToDraw+=1;


	return	TRUE;
}

 
 
 
 
 
 
bool	CIFPMManager::PreparePactMarkBuffers	(	int		in_numMarks	)
{
	HRESULT		hr;
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();

	hr	=	pd3dDevice->CreateVertexBuffer( in_numMarks * 4 * sizeof(PACTMARKVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_PACT, D3DPOOL_DEFAULT, &m_vbPactMark );
	if	(	FAILED( hr )	)
	{
		 
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_CREATE_VERTEX_BUFFER;
		return	false;
	}

	hr	=	pd3dDevice->CreateIndexBuffer( in_numMarks * 6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_ibPactMark );
	if	(	FAILED( hr )	)
	{
		 
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_CREATE_INDEX_BUFFER;
		return	false;
	}

	WORD	*indices;
	hr	=	m_ibPactMark->Lock( 0, 0, (BYTE **)&indices, 0 );
	if	(	FAILED( hr )	)
	{
		 
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_LOCK_INDEX_BUFFER;
		return	false;
	}

	int	indexOffset		=	0;
	int	vertexOffset	=	0;
	for	(	int	index	=	0;	\
				index	<	in_numMarks;	\
				++index	)
	{
		indices[ indexOffset++ ]	=	vertexOffset;
		indices[ indexOffset++ ]	=	vertexOffset + 1;
		indices[ indexOffset++ ]	=	vertexOffset + 2;

		indices[ indexOffset++ ]	=	vertexOffset + 2;
		indices[ indexOffset++ ]	=	vertexOffset + 1;
		indices[ indexOffset++ ]	=	vertexOffset + 3;

		vertexOffset	+=	4;
	}

	hr	=	m_ibPactMark->Unlock();
	if	(	FAILED( hr )	)
	{
		 
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_UNLOCK_INDEX_BUFFER;
		return	false;
	}

	return	true;
}





 
 
 
 
 
 
BOOL CIFPMManager::PrepareToRegisterDrawee(void)
{
	HRESULT	hr;
	hr	=	m_vbPactMark->Lock( 0, MAXDRAWNUMperRENDER * 4 * sizeof(PACTMARKVERTEX), (BYTE **)&m_lockedVertexBuffer, D3DLOCK_DISCARD );
	if(FAILED(hr)){
		
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_LOCK_VERTEX_BUFFER;
		return	FALSE;
	}	
	m_numMarksToDraw=0;
	return	TRUE;
}






 
 
 
 
 
 
BOOL	CIFPMManager::FinishToRegisterDrawee(void)
{
	HRESULT	hr;
	hr	=	m_vbPactMark->Unlock();
	if	(	FAILED( hr )	)
	{
		 
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_UNLOCK_VERTEX_BUFFER;
		return	FALSE;
	}

	return	TRUE;
}

 
 
 
 
 
 
BOOL CIFPMManager::DrawPactMarks(void)
{
	HRESULT	hr;
	
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();

	pd3dDevice->SetStreamSource( 0, m_vbPactMark, sizeof(PACTMARKVERTEX) );
	pd3dDevice->SetIndices( m_ibPactMark, 0 );
	pd3dDevice->SetVertexShader( D3DFVF_PACT );
	pd3dDevice->SetTexture( 0, m_textureForPact );
	
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLOROP   , D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG1 , D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_DIFFUSE );

	hr	=	pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, m_numMarksToDraw * 4, 0, m_numMarksToDraw * 2 );
	if	(	FAILED( hr )	)
	{
		m_errorIndex	=	IFPM_ERROR_FAILED_ON_DRAW_INDEXED_PRIMITIVE;
		return	FALSE;
	}

	return	TRUE;
}

 
 
 
 
 
 
void	CIFPMManager::CleanupPactMarksMemory(void)
{
	GTH_SAFE_RELEASE( m_textureForPact );
	GTH_SAFE_RELEASE( m_vbPactMark );
	GTH_SAFE_RELEASE( m_ibPactMark );
	 
	GTH_SAFE_DELETE_ARRAY( m_bufferForRestore );
	
	ITOR_MAP_PACTMARKINFO	itor;
	for	(	itor	=	m_pactMarkInfoMap.begin();	\
			itor	!=	m_pactMarkInfoMap.end();	\
			++itor	)
	{
		if	(	itor->second	)
		{
			GTH_SAFE_DELETE( itor->second );
		}
	}
}

 
 
 
 
 
 
void	CIFPMManager::InvalidateDeviceObjects	()
{
	GTH_SAFE_RELEASE( m_textureForPact );
	GTH_SAFE_RELEASE( m_vbPactMark );
	GTH_SAFE_RELEASE( m_ibPactMark );
}

 
 
 
 
 
 
bool	CIFPMManager::RestoreDeviceObjects	()
{
	HRESULT	hr;

	

	hr	=	D3DXCreateTexture( 
				g_ifMng->m_pd3dDevice, 
				m_pactTextureAtlasWidth, 
				m_pactTextureAtlasHeight, 
				1, 
				0, 
				D3DFMT_A4R4G4B4, 
				D3DPOOL_MANAGED, 
				&m_textureForPact);



	hr	=	m_textureForPact->LockRect( 0, &m_lockedRect, 0, 0 );
	if	(	FAILED( hr )	)
	{
		 
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_LOCK_TEXTURE_ATLAS;
		return	false;
	}

	memcpy( m_lockedRect.pBits, m_bufferForRestore, m_pactTextureAtlasWidth * m_pactTextureAtlasHeight * sizeof(WORD) );

	hr	=	m_textureForPact->UnlockRect( 0 );
	if	(	FAILED( hr )	)
	{
		 
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_UNLOCK_TEXTURE_ATLAS;
		return	false;
	}
	return	true;
}


bool	CIFPMManager::UpdateGuildTexture()
{
	HRESULT	hr;

	if( m_textureForPact == NULL ) return false;

	hr	=	m_textureForPact->LockRect( 0, &m_lockedRect, 0, 0 );
	if	(	FAILED( hr )	)
	{
		 
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_LOCK_TEXTURE_ATLAS;
		return	false;
	}

	memcpy( m_lockedRect.pBits, m_bufferForRestore, m_pactTextureAtlasWidth * m_pactTextureAtlasHeight * sizeof(WORD) );
	hr	=	m_textureForPact->UnlockRect( 0 );
	if	(	FAILED( hr )	)
	{
		 
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_UNLOCK_TEXTURE_ATLAS;
		return	false;
	}
	return true;
}








void CIFPMManager::ChangeImageAtRestoreBuffer(PACTMARKINFO* pInfo,const BYTE* pImage)
{
	assert(NULL != pImage);	

	
	unsigned short int* pDestLineEnd = m_bufferForRestore + 
									   (pInfo->ImageArea.top * m_pactTextureAtlasWidth) + 
									   pInfo->ImageArea.left + (m_pactTextureWidth-1);
	unsigned short int* pDestTemp;
	
	const unsigned char*      pSrcLineEnd  = pImage + 
								       (m_pactTextureHeight-1) * (m_pactTextureWidth *3) + 
									   ((m_pactTextureWidth-1) *3);
	

	for(int lineIdx=0; lineIdx < m_pactTextureHeight; lineIdx++,pDestLineEnd += m_pactTextureAtlasWidth){
		pDestTemp = pDestLineEnd;

		for(int ColIdx=0; ColIdx < m_pactTextureWidth; ColIdx++,pDestTemp--,pSrcLineEnd -= 3){

			*pDestTemp = ( 0xF << 12 ) | 
						 (( *(pSrcLineEnd+2) >> 4 ) << 8 ) |
						 (( *(pSrcLineEnd+1) >> 4 ) << 4 ) |
						 ( *(pSrcLineEnd) >> 4 );

		}
	}
}






void CIFPMManager::AddImageAtRestoreBuffer(PACTMARKINFO* pInfo,const BYTE* pImage)
{	
	assert(NULL != pImage);

	pInfo->ImageArea.left  = m_curPactItemColumnOffset * m_pactTextureWidth;
	assert(pInfo->ImageArea.left >= 0);
	assert(pInfo->ImageArea.left < m_pactTextureAtlasWidth);

	pInfo->ImageArea.top    = m_curPactItemRowOffset    * m_pactTextureHeight;
	assert(pInfo->ImageArea.top >= 0);
	assert(pInfo->ImageArea.top < m_pactTextureAtlasHeight);

	pInfo->ImageArea.right  = pInfo->ImageArea.left+(m_pactTextureWidth-1);
	assert(pInfo->ImageArea.right >= 0);
	assert(pInfo->ImageArea.right < m_pactTextureAtlasWidth);

	pInfo->ImageArea.bottom = pInfo->ImageArea.top +(m_pactTextureWidth-1);
	assert(pInfo->ImageArea.bottom >= 0);
	assert(pInfo->ImageArea.bottom <m_pactTextureAtlasHeight);


	ChangeImageAtRestoreBuffer(pInfo,pImage);
}




BOOL CIFPMManager::
INSERTorCHANGE_PactMarks(
	const int	in_identifier,	
	const BYTE* in_markBuffer)								
{
	ITOR_MAP_PACTMARKINFO	itor;
	itor = m_pactMarkInfoMap.find( in_identifier );
	if(itor	!=	m_pactMarkInfoMap.end()){		
		PACTMARKINFO* pInfo = itor->second;
		ChangeImageAtRestoreBuffer(pInfo,in_markBuffer);
		return TRUE;		
	}

	PACTMARKINFO	*newInfo	=	new	PACTMARKINFO;
	if(NULL	==	newInfo){		
		m_errorIndex	=	IFPM_ERROR_FAILED_TO_ALLOCATE_MEMORY;
		return	FALSE;
	}

	m_pactMarkInfoMap[ in_identifier ]	=	newInfo;	
	AddImageAtRestoreBuffer(newInfo,in_markBuffer );

	if(! CalculateTextureCoordsForPactInfo( newInfo )){
		assert(FALSE);
		return	FALSE;
	}
	return	TRUE;	
}



