#ifndef	_IF_PACT_MARK_MANAGER_H_QPQPQSBQBAF82343147ODJLASWQQGRTJLSDF
#define	_IF_PACT_MARK_MANAGER_H_QPQPQSBQBAF82343147ODJLASWQQGRTJLSDF

#pragma warning( disable : 4786 )
#include <MAP>























 
 
 
 
 
 
class	CIFPMManager
{
	public:
		enum {
			MAXDRAWNUMperRENDER = MAX_CHARACTERS,
			MAXNUM_REGISTER_MARKTEXTURE = MAX_GUILDS,
		};

		struct	PACTMARKINFO{
			public:
				struct tagBufferRect{
					int	left,top,right,bottom;
				};

			public:
				float	texWidthStart;
				float	texWidthEnd;
				float	texHeightStart;
				float	texHeightEnd;	
				struct tagBufferRect ImageArea;
		};

		#define		MAP_PACTMARKINFO		std::map< int, PACTMARKINFO* >
		#define		ITOR_MAP_PACTMARKINFO	std::map< int, PACTMARKINFO* >::iterator

		struct	PACTMARKVERTEX
		{
			D3DXVECTOR4		pos;
			float			tu;
			float			tv;
		};

		#define		D3DFVF_PACT		( D3DFVF_XYZRHW | D3DFVF_TEX1 )

		enum
		{
			IFPM_NO_ERROR	=	0,
			IFPM_ERROR_FAILED_TO_GET_DEVICE_CAPS,
			IFPM_ERROR_FAILED_TO_OPEN_BMP_FILE,
			IFPM_ERROR_FAILED_TO_IDENTIFY_FILE_SIZE,
			IFPM_ERROR_FAILED_TO_LOAD_BMP_DATA,
			IFPM_ERROR_NOT_24_BIT_COLOR_FORMAT,
			IFPM_ERROR_TEXTURE_SIZE_EXCEED_DEVICE_CAP,
			IFPM_ERROR_FAILED_TO_CREATE_D3D_TEXTURE,
			IFPM_ERROR_ITEM_ALREADY_EXIST,
			IFPM_ERROR_FAILED_TO_ALLOCATE_MEMORY,
			IFPM_ERROR_SUB_TEXTURE_EXCEED_MAX_CAP,
			IFPM_ERROR_FAILED_TO_LOCK_TEXTURE_ATLAS,
			IFPM_ERROR_FAILED_TO_UNLOCK_TEXTURE_ATLAS,
			IFPM_ERROR_TRIED_TO_DRAW_UNREGISTERED_ITEM,
			IFPM_ERROR_FAILED_TO_CREATE_VERTEX_BUFFER,
			IFPM_ERROR_FAILED_TO_CREATE_INDEX_BUFFER,
			IFPM_ERROR_FAILED_TO_LOCK_INDEX_BUFFER,
			IFPM_ERROR_FAILED_TO_UNLOCK_INDEX_BUFFER,
			IFPM_ERROR_FAILED_TO_LOCK_VERTEX_BUFFER,
			IFPM_ERROR_FAILED_TO_UNLOCK_VERTEX_BUFFER,
			IFPM_ERROR_FAILED_ON_DRAW_INDEXED_PRIMITIVE,
		};		

		const static char* IFPM_ERROR_MESSAGES[];

public:
	CIFPMManager();
	~CIFPMManager();	

public:		
	
	bool	PreparePactMarkBuffers	(	int		in_numCharacters	);	
	bool	CreateTextureForPact	(	int	in_numMarksToCreate,	int	in_textureWidth	=	16,	int	in_textureHeight	=	16	);	
	
	void	InvalidateDeviceObjects	();
	bool	RestoreDeviceObjects	();
	bool	UpdateGuildTexture();
	const char*	GetErrorMessage	()	{	return	IFPM_ERROR_MESSAGES[ m_errorIndex ];	}
	int		GetErrorCode	()		{	return	m_errorIndex;	}		
	


public:
	BOOL    INSERTorCHANGE_PactMarks(const int	in_identifier,const BYTE* in_markBuffer);





public:
	BOOL	DrawPactMarks(void);
	BOOL	PrepareToRegisterDrawee(void);
	BOOL	RegisterPactMarkDrawee(
				const float	in_screenX,	
				const float	in_screenY,	
				const float	in_depthZ,
				const float	in_depthW,
				const int	in_identifier);

	BOOL	FinishToRegisterDrawee(void);

	
	
private:
	
	void	InitializePactTextureMembers();
	void	CleanupPactMarksMemory(void);
	
protected:	
	bool GetMaxDeviceTextureSize	(	DWORD	&out_width,	DWORD	&out_height	);	
	bool CalculateTextureCoordsForPactInfo	(	PACTMARKINFO	*in_info	);	
	void AddImageAtRestoreBuffer(PACTMARKINFO* pInfo,const BYTE* pImage);
	void ChangeImageAtRestoreBuffer(PACTMARKINFO* pInfo,const BYTE* pImage);	
	

	
protected:
	DWORD	m_maxDeviceTextureWidth;		
	DWORD	m_maxDeviceTextureHeight;		
	DWORD	m_pactTextureWidth;				
	DWORD	m_pactTextureHeight;			
	DWORD	m_pactTextureAtlasWidth;		
	DWORD	m_pactTextureAtlasHeight;		
	
	LPDIRECT3DTEXTURE8	m_textureForPact;	
	D3DLOCKED_RECT		m_lockedRect;

	MAP_PACTMARKINFO	m_pactMarkInfoMap;	

	int		m_itemCountForOneDimension;		
											
	int		m_curPactItemColumnOffset;		
	int		m_curPactItemRowOffset;			

	LPDIRECT3DVERTEXBUFFER8		m_vbPactMark;	
	LPDIRECT3DINDEXBUFFER8		m_ibPactMark;	
	PACTMARKVERTEX				*m_lockedVertexBuffer;	
	int		m_numMarksToDraw;	
	int		m_errorIndex;
	WORD	*m_bufferForRestore;
};

#endif	
