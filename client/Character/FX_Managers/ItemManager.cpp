 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"
#include "Mmsystem.h"

#define		g_mat					GLOBAL_DXMATRIX_1
#define		g_disp					GLOBAL_DXVEC3_1
#define		g_selfRot				GLOBAL_QUAT_1
#define		g_vecAfterTransform		GLOBAL_DXVEC3_1
#define		g_quatAfterTransform	GLOBAL_QUAT_1
#define		g_quatTarget			GLOBAL_QUAT_2
#define		g_quatConjugate			GLOBAL_QUAT_3
#define		g_tgtPos				GLOBAL_DXVEC3_1
#define		g_direction				GLOBAL_DXVEC3_2
#define		g_scalevector			GLOBAL_DXVEC3_3

#define		g_resultMatrix			GLOBAL_MATRIX_1

#define		g_buffer				GLOBAL_VEC4_4

vec3_t		g_baseVector =	{	0.0f,	-1.0f,	0.0f	};

 
 
 
FX_CItemManager::FX_CItemManager	()
{
	 
	m_pItemRenderer	=	new	FX_CItemRenderer();
	m_pItemRenderer->SetItemManager	(	this	);

	m_iNumItems	=	0;
	
	 
	if	(	g_resCounter->Enabled()	)
	{
		g_resCounter->Add_DataType( RESCOUNTER_CHAR_ITEMMANAGER, sizeof(FX_CItemManager) );
	}
}

 
 
 
FX_CItemManager::~FX_CItemManager	()
{
}


 
 
 
bool	FX_CItemManager::LoadItemList	()
{
	int		index;
	UINT	iRefID;
	int		errorval;

	errorval	=	m_pScript->Load	(	FILENAME_ITEM_LIST	);
	if	(	! errorval )	return	false;

	m_pScript->BeginParse	();

	m_iNumItems	=	atoi ( m_pScript->GetNextToken() );

	for	(	index	=	0;	\
			index	<	m_iNumItems;	\
			++index	)
	{
		strcpy	(	m_ItemBuffer [index].name,
					m_pScript->GetNextToken()		);
		m_ItemBuffer [index].id	=	index;
		sprintf	(	m_ItemBuffer [index].texname,
					"%s",
					m_pScript->GetNextToken()		);
		iRefID	=	m_pCharShaderMng ->SearchShaderRef (	m_ItemBuffer [index].texname	);
	
		LoadItem (	m_ItemBuffer [index].name,
					m_ItemBuffer [index].id,
					iRefID	);
	}

	return	true;
}

 
 
 
void	FX_CItemManager::UpdateTextureID	()
{
	for	(	int	index	=	0;
				index	<	m_iNumItems;
				++index		)
	{
		m_Items	[ m_ItemBuffer [index].id ].iRefID	
			=	m_pCharShaderMng ->SearchShaderRef (	m_ItemBuffer [index].texname	);
	}
}

 
 
 
bool	FX_CItemManager::LoadItem	(	char		*pszItemName,
										const UINT	iItemID,
										const UINT	iRefID		)
{
	ITEM_t*		pItem;
	char		strTemp [MAX_PATH];

	 
	pItem	=	&m_Items	[iItemID];


	 
	pItem->mesh.LoadData	(	pszItemName,
								m_fileMng	);

	 
	pItem->iRefID	=	iRefID;

	 
	strcpy	(	strTemp,
				pItem->mesh.m_strMeshName	);
	
	 
	char	*chTemp	=	strrchr	(	strTemp,
									'+'	
								);	 
	 
	if	(	chTemp	)
	{
		int		pos;
		pos	=	chTemp	-	strTemp;
		strcpy	(	strTemp + pos,
					"\0"
				);	 
	}

	 
	if	(	strncmp	(	strTemp,	_T	("tag_"),	4	)	==	0	)
	{
		pItem->iTagIndex		=	m_pTagManager->GetIndex	(	strTemp	);	 
		if	(	pItem->iTagIndex		==	-1	)
			return	false;
		char	szTemp [256];
		strcpy	(	szTemp,
					m_pTagManager->GetParentName	(	pItem->iTagIndex	)
				);
		pItem->iParentTagIndex	=	m_pBoneManager->GetIndex (	szTemp	);
		if	(	pItem->iParentTagIndex	==	-1	)
			return	false;
	}

	return	true;
}


 
 
 
bool	FX_CItemManager::CountVertices	(	UINT		&iVertexResult,
											UINT		&iIndexResult	)
{
	 
	iVertexResult	=	0;
	iIndexResult	=	0;

	UINT			iIndexTemp	=	0;		 
	UINT			iVertexTemp	=	0;

	MESHCHUNK*		pMesh;

	for	(	int	index	=	0;
				index	<	m_iNumItems;
				++index		)
	{
		 
		pMesh			=	m_Items	[index].mesh.m_meshes [0].meshchunk;

		 
		m_Items [index].iVertexStart	=	iVertexTemp;
		m_Items	[index].iIndexStart		=	iIndexTemp;

		 
		m_Items [index]	.iNumVertex		=	pMesh->iNumVertexMap;

		iVertexResult	+=	pMesh->iNumVertexMap;

		 
		m_Items [index] .iPrimitiveCount	=	pMesh->iNumFace;

		iIndexResult	+=	pMesh->iNumFace	*	3;

		 
		iVertexTemp	=	iVertexResult;
		iIndexTemp	=	iIndexResult;
	}

	return	true;
}


 
 
 
void	FX_CItemManager::PrepareRenderBuffers ()
{
	if	(	m_iNumItems	>	0	)
	{
		m_pItemRenderer ->CreateBuffers ();
		m_pItemRenderer ->FillBuffers ();
	}

	return;
}


 
 
 
void	FX_CItemManager::ReleaseRenderBuffers ()
{
	if	(	m_iNumItems	>	0	)
		m_pItemRenderer ->ReleaseBuffers ();
		
	return;
}


 
 
 
 
void	FX_CItemManager::SetRenderer (	CD3DRender		*d3dRender	)
{
	m_D3DRender	=	d3dRender;

	m_pItemRenderer ->SetRenderer (	d3dRender	);

	return;
}

 
 
 
 
void	FX_CItemManager::SetShaderMng	(	CShaderMng		*shaderMng	)
{
	m_shaderMng	=	shaderMng;

	return;
}

 
 
 
bool	FX_CItemManager::RenderAll	(	int		*out_numprims	)
{
	ITEM_t			*pThisItem;
	CHAR_BUFFER_t	*pCharBuffer;	 
	Fx_CHARACTER_t	*pCharacter;	 
	int				iRefIndex;		 
	int				result;
	matrix4x4_t		*mat_trans;
	extern Fx_CHARACTER_t	*g_mouseSelectedCharacter;

#ifdef	_FX_CLIENT
	Fx_CHARACTER_t	*pTargetChar	=	NULL;

	if	(	g_cgv.state	==	CLIENT_GAME_PROCESS	)
	{
		if	(	g_mouseSelectedCharacter	!=	NULL	&&	g_cgv.pTargetChar	!=	NULL	)
			pTargetChar	=	g_cgv.pTargetChar;
		if	(	g_mouseSelectedCharacter	!=	NULL	&&	g_cgv.pTargetChar	==	NULL	)
			pTargetChar	=	g_mouseSelectedCharacter;
		if	(	g_mouseSelectedCharacter	==	NULL	&&	g_cgv.pTargetChar	!=	NULL	)
			pTargetChar	=	g_cgv.pTargetChar;
		if	(	g_mouseSelectedCharacter	==	NULL	&&	g_cgv.pTargetChar	==	NULL	)
			pTargetChar	=	NULL;
	} 
	else	
		pTargetChar	=	g_mouseSelectedCharacter;
#endif

	if	(	m_iNumItems	<=	0	)
		return	true;

	result	=	m_pItemRenderer ->PrepareRender ();

	for	(	int	index	=	0;
				index	<	m_iNumMeshInBuffer;
				++index		)
	{
		pCharBuffer		=	&m_CharBuffer [index];

		pThisItem	=	&m_Items[ pCharBuffer->meshID ];
		iRefIndex	=	pThisItem->iRefID;

		if	(	iRefIndex	<	0	)
		{
			ErrorLog( "index : %d, mesh idx : %d, RefID of ITEM has some problem!", index, pCharBuffer->meshID );
			continue;
		}

			 
		if	(	m_pCharShaderMng ->CheckIsShader ( iRefIndex )	)
		{
			m_pCharShaderMng ->PrepareShaderByRef (	iRefIndex	);
		}
		else
		{
			m_D3DRender ->SetTexture	(	0,	m_pCharShaderMng ->GetD3DTextureByRef ( iRefIndex )	);
		}
		
		for	(	int	subindex	=	0;	\
					subindex	<	pCharBuffer->count;	\
					++subindex		)
		{
			pCharacter	=	pCharBuffer->characterPtr[ subindex ];

			if	(	! pCharacter->ren_renderThisFrame		)
				continue;

			mat_trans	=	GetTagMatrix	(	pCharacter,	pCharBuffer ->tagID [subindex],	pCharBuffer ->parentTagID [subindex]	);

			 
			if	(	! mat_trans	)
				continue;

			m_D3DRender ->SetTransform	(	D3DTS_WORLD,	mat_trans	);

			if	(	(	pThisItem ->iVertexStart	<	0	)	||
					(	pThisItem ->iNumVertex	<	0	)	||
					(	pThisItem ->iIndexStart	<	0	)	||
					(	pThisItem ->iPrimitiveCount	<	0	)	)
			{
				ErrorLog( "vertex start pos : %d, num vertex : %d, index start pos : %d, prim count : %d", pThisItem->iVertexStart, pThisItem->iNumVertex, pThisItem->iIndexStart, pThisItem->iPrimitiveCount );
				continue;
			}
			
#ifdef	_FX_CLIENT
			if	(	(	pTargetChar	!=	pCharacter	)	||
					(	g_cgv.state == CLIENT_GAME_PROCESS &&	pCharacter->ID	==	0	)	)
#else
			if	(	true	)
#endif
			{
				Render (	pCharacter,	iRefIndex,	pThisItem	);
				*out_numprims	+=	pThisItem ->iPrimitiveCount;
			}
			else
			{
				Render (	pCharacter,	iRefIndex,	pThisItem	);
			}
		}
	}

	return	true;
}

 
 
 
 
void	FX_CItemManager::Render	(	Fx_CHARACTER_t	*in_char,	
									const int		in_refIdx,
									const ITEM_t	*pThisItem	)
{
	if	(	m_pCharShaderMng ->CheckIsShader ( in_refIdx )	)
	{
		shader_t	*thisShader	=	m_pCharShaderMng ->GetShaderByRef ( in_refIdx );
#ifdef	_FX_CLIENT
		if	(	thisShader ->flags	&	SHADER_TCGEN	)
			m_shaderMng ->PrepareTCGen (	thisShader,	in_char ->position	);
#endif
		m_D3DRender ->RenderMDLCustom	(	thisShader,
											pThisItem ->iVertexStart,
											pThisItem ->iNumVertex,
											pThisItem ->iIndexStart,
											pThisItem ->iPrimitiveCount * 3	);
	}
	else
	{
		m_D3DRender ->RenderDefault	(	pThisItem ->iVertexStart,
										pThisItem ->iNumVertex,
										pThisItem ->iIndexStart,
										pThisItem ->iPrimitiveCount * 3	);
	}

	return;
}

 
 
 
 
void	FX_CItemManager::EffectRender	(	Fx_CHARACTER_t	*in_char,
											const int		in_refIdx,
											const ITEM_t	*pThisItem	)
{
	if	(	m_pCharShaderMng ->CheckIsEffectShader ( in_refIdx )	)
	{
		shader_t	*thisShader	=	m_pCharShaderMng ->GetShaderByRef ( in_refIdx );
#ifdef	_FX_CLIENT
		if	(	thisShader ->flags	&	SHADER_TCGEN	)
			m_shaderMng ->PrepareTCGen (	thisShader,	in_char ->position	);
#endif

		m_D3DRender ->RenderMDLCustom	(	m_pCharShaderMng ->GetEffectShaderByRef ( in_refIdx ),
											pThisItem ->iVertexStart,
											pThisItem ->iNumVertex,
											pThisItem ->iIndexStart,
											pThisItem ->iPrimitiveCount * 3	);
	}
	else
	{
		m_D3DRender ->RenderDefault	(	pThisItem ->iVertexStart,
										pThisItem ->iNumVertex,
										pThisItem ->iIndexStart,
										pThisItem ->iPrimitiveCount * 3	);
	}

	return;
}

 
 
 
void	FX_CItemManager::SetTexManager	(	CD3DTextureMng	*texMng	)
{
	m_texMng	=	texMng;

	return;
}


 
 
 
void	FX_CItemManager::SetDevice(	LPDIRECT3DDEVICE8	device	)
{
	m_pd3dDevice	=	device;

	m_pItemRenderer->SetDevice	(	m_pd3dDevice	);

	return;
}

 
 
 
bool	FX_CItemManager::UpdateCharBuffer	()
{
	if	(	m_iNumItems	<=	0	)	return	true;
	
	 
	for	(	int	index	=	0;
				index	<	MAX_COUNT_ITEM_MESH;
				++index		)
	{
		m_iBufferIndexMap [index]	=	-1;
	}
	m_iNumMeshInBuffer	=	0;

	 

	Fx_CHARACTER_t	*pCharacter;		 
	int				iItemID;			 
	CHAR_BUFFER_t	*pCharBuffer;		 

	 
	for	(	index	=	0;
			index	<	m_pCharManager->m_iNumCharacters;
			++index		)
	{
		 
		pCharacter	=	&m_pCharManager->m_Characters [index];
		
		 
		for	(	int	subindex	=	0;
					subindex	<	MAX_COUNT_ITEM_PART;
					++subindex	)
		{
			 
			iItemID		=	pCharacter ->stat_itemID [subindex];

			 
			if	(	iItemID	==	-1	)
				continue;

			if	(	m_iBufferIndexMap [iItemID]	==	-1	)
			{	 
				 
				m_iBufferIndexMap [iItemID]		=	m_iNumMeshInBuffer;		 
				 
				pCharBuffer		=	&m_CharBuffer [m_iNumMeshInBuffer];
				pCharBuffer->meshID			=	iItemID;
				pCharBuffer->count			=	1;
				pCharBuffer->characterPtr [0]	=	pCharacter;
				pCharBuffer->tagID [0]			=	subindex;
				pCharBuffer->parentTagID [0]	=	m_pBoneManager ->GetIndex ( m_pTagManager ->GetParentName ( subindex ) );

				 
				++m_iNumMeshInBuffer;
			}
			else
			{	 
				 
				pCharBuffer		=	&m_CharBuffer [m_iBufferIndexMap [iItemID]];
				 
				++(pCharBuffer->count);
				pCharBuffer->characterPtr[ pCharBuffer->count - 1 ]		=	pCharacter;
				pCharBuffer->tagID[ pCharBuffer->count - 1 ]			=	subindex;
				pCharBuffer->parentTagID[ pCharBuffer->count - 1 ]	=	m_pBoneManager->GetIndex( m_pTagManager->GetParentName( subindex ) );
			}
		}
	}

	return	true;		
}

 
 
 
void	FX_CItemManager::Cleanup	()
{
	for	(	int	index	=	0;
				index	<	m_iNumItems;
				++index		)
	{
		m_Items [index]	.mesh	.Cleanup();
	}

	m_pItemRenderer->Cleanup();

	GTH_SAFE_DELETE ( m_pItemRenderer );

	return;
}

 
 
 
 
matrix4x4_t*	FX_CItemManager::GetTagMatrixOut	(	Fx_CHARACTER_t	*in_char,
														const int		in_tag	)
{
	return	GetTagMatrix( in_char, in_tag, m_pBoneManager->GetIndex( m_pTagManager->GetParentName( in_tag ) ) );
}

 
 
 
 
matrix4x4_t*	FX_CItemManager::GetTagMatrix	(	Fx_CHARACTER_t	*in_char,
													const int		in_tag,
													const int		in_parentTag	)
{
	if	(	m_pSkelManager ->m_iBoneLookUp [in_char ->attr_skelID][in_parentTag]	<	0	)
	{
		ErrorLog( "skel index : %d, parent tag : %d, bone index : %d", in_char->attr_skelID, in_parentTag, m_pSkelManager ->m_iBoneLookUp [in_char ->attr_skelID][in_parentTag] );
		return	NULL;
	}

	if	(	in_char ->flag_tagMatrixMade [in_tag]	==	false	)
	{
		KEYFRAME		*thisFrame	=	&in_char ->key_resultFrames [m_pSkelManager ->m_iBoneLookUp [in_char ->attr_skelID][in_parentTag]];
		TAG				*thisTag	=	&m_pSkelManager ->m_Skels [in_char ->attr_skelID].skeleton.m_Tag [in_tag];
		MakeTagMatrix( in_char, thisFrame, thisTag, in_tag, in_char->ren_tagMatrixData[ in_tag ], true );
		in_char ->flag_tagMatrixMade [in_tag]	=	true;
	}

	return	&in_char ->ren_tagMatrixData [in_tag];
}


 
 
 
 
 
void	FX_CItemManager::MakeTagMatrix	(	const Fx_CHARACTER_t	*in_char,
											const KEYFRAME			*in_frame,
											const TAG				*in_tag,
											const int				in_tagID,
											matrix4x4_t				&out_matrix,
											const bool				in_transform	)
{
	int				overallTime;
	FX_TAGSCRIPT_t	*script;
	int				angle;	
	float			*quat;
	float			preval;
	float			yaw	=	0.0f, pitch	=	0.0f, roll	=	0.0f;

	switch	(	in_tagID	)
	{
	case	ITEM_PART_OVERHEAD_FAR:
	case	ITEM_PART_OVERHEAD_NEAR:
	case	ITEM_PART_MIDDLEAIR_R:
	case	ITEM_PART_MIDDLEAIR_L:
		script	=	m_pTagManager ->GetScript (in_char ->stat_moveID [in_tagID]);

			 
		if	(	script ->x_func.func	!=	POS_FIXED	)
		{
			angle	=	m_pCharUtil ->MakeAngleWithTime (	m_timer ->GetAppTime () * script ->x_func.angle_mul,
															script ->velocity	);
			g_disp [0]	=	script ->x_func.scale * m_pTagManager ->GetDisplacement (	script ->x_func.func,	angle	);
		}
		else
			g_disp [0]	=	0.0f;

		if	(	script ->y_func.func	!=	POS_FIXED	)
		{
			angle	=	m_pCharUtil ->MakeAngleWithTime (	m_timer ->GetAppTime () * script ->y_func.angle_mul,
															script ->velocity	);
			g_disp [1]	=	script ->y_func.scale * m_pTagManager ->GetDisplacement (	script ->y_func.func,	angle	);
		}
		else
			g_disp [1]	=	0.0f;

		if	(	script ->z_func.func	!=	POS_FIXED	)
		{
			angle	=	m_pCharUtil ->MakeAngleWithTime (	m_timer ->GetAppTime () * script ->z_func.angle_mul,
															script ->velocity	);
			g_disp [2]	=	script ->z_func.scale * m_pTagManager ->GetDisplacement (	script ->z_func.func,	angle	);
		}
		else
			g_disp [2]	=	0.0f;

		 
		g_vecAfterTransform [0]	=	( script ->pos [0]	+	in_frame ->vecOffset [0]	+	g_disp [0] ) * in_char->stat_scale;
		g_vecAfterTransform [1]	=	( script ->pos [1]	+	in_frame ->vecOffset [1]	+	g_disp [1] ) * in_char->stat_scale;
		g_vecAfterTransform [2]	=	( script ->pos [2]	+	in_frame ->vecOffset [2]	+	g_disp [2] ) * in_char->stat_scale;

			 
		switch (	script ->look	)
		{
		case	ALWAYS_LOOK_TGT:
			TagLookTarget (	g_vecAfterTransform,	in_char ->targetIdx,	g_quatAfterTransform	);
			quat	=	g_quatAfterTransform;
			break;
		case	ALWAYS_LOOK_FWD:
			quat	=	NULL;
			break;
		case	ALWAYS_LOOK_CENTER:
			VectorNegate ( g_disp,	g_disp );
			TagLookCenter (	g_disp,	g_quatAfterTransform	);
			quat	=	g_quatAfterTransform;
			break;
		case	FREE_LOOK:
			int		timeFactor	=	int (m_timer ->GetAppTime () * 0.5f)	%	3;
			quat	=	g_quatAfterTransform;
			switch	(	timeFactor	)
			{
			case	0:
				TagLookTarget (	g_vecAfterTransform,	in_char ->targetIdx,	g_quatAfterTransform	);
				break;
			case	1:
				quat	=	NULL;
				break;
			case	2:
				VectorNegate ( g_disp,	g_disp );
				TagLookCenter (	g_disp,	g_quatAfterTransform	);
				break;
			}
		}

		preval	=	__TWOPI * m_timer ->GetAppTime ();

			 
		if	(	script ->x_rotation	>	0	)
		{
			overallTime	=	script ->x_rotation + script ->x_delay;
				 
			if	(	(int (m_timer ->GetAppTime ())	% overallTime)	<	script ->x_rotation	)
			{
				 
				pitch	=	script ->x_rotation * preval;
			}
			else
			{
				 
				pitch	=	0.0f;
			}
		}

		if	(	script ->y_rotation	>	0	)
		{
			overallTime	=	script ->y_rotation + script ->y_delay;
				 
			if	(	(int (m_timer ->GetAppTime ())	% overallTime)	<	script ->y_rotation	)
			{
				 
				yaw	=	script ->y_rotation * preval;
			}
			else
			{
				 
				yaw	=	0.0f;
			}
		}

		if	(	script ->z_rotation	>	0	)
		{
			overallTime	=	script ->z_rotation + script ->z_delay;
				 
			if	(	(int (m_timer ->GetAppTime ())	% overallTime)	<	script ->z_rotation	)
			{
				 
				roll	=	script ->z_rotation * preval;
			}
			else
			{
				 
				roll	=	0.0f;
			}
		}

		if	(	(script ->x_rotation + script ->y_rotation + script ->z_rotation)	>	0	)
		{
			D3DXQuaternionRotationYawPitchRoll (	(D3DXQUATERNION *)g_selfRot,	yaw,	pitch,	roll	);
			if	(	quat	)
			{
				D3DXQuaternionMultiply (	(D3DXQUATERNION *)quat,	(D3DXQUATERNION *)g_selfRot,	(D3DXQUATERNION *)quat	);
				D3DXMatrixTransformation	(	(D3DXMATRIX *)&g_mat,	NULL,	NULL,	NULL,	NULL,	(D3DXQUATERNION *)quat,	(D3DXVECTOR3 *)g_vecAfterTransform	);
			}
			else
				D3DXMatrixTransformation	(	(D3DXMATRIX *)&g_mat,	NULL,	NULL,	NULL,	NULL,	(D3DXQUATERNION *)g_selfRot,	(D3DXVECTOR3 *)g_vecAfterTransform	);
		}
		else
		{
			D3DXMatrixTransformation	(	(D3DXMATRIX *)&g_mat,	NULL,	NULL,	NULL,	NULL,	(D3DXQUATERNION *)quat,	(D3DXVECTOR3 *)g_vecAfterTransform	);
		}
		
		if	(	in_transform	)
			D3DXMatrixMultiply ( (D3DXMATRIX *)&out_matrix,	(D3DXMATRIX *)&g_mat,	(D3DXMATRIX *)&in_char ->stat_matTransform );
		else
			memcpy( &out_matrix, &g_mat, sizeof(matrix4x4_t) );

		break;
	case	ITEM_PART_ARM_R:				
	case	ITEM_PART_ARM_L:				
	case	ITEM_PART_NECK:					
	case	ITEM_PART_BODY:					
	case	ITEM_PART_HAND_R:				
	case	ITEM_PART_HAND_L:				
	case	ITEM_PART_BACKHAND_R:			
	case	ITEM_PART_BACKHAND_L:			
	case	ITEM_PART_KNEE_R:				
	case	ITEM_PART_KNEE_L:				
	case	ITEM_PART_WAIST:				
	case	ITEM_PART_SHOULDER:				
	case	ITEM_PART_BACK:					
	case	ITEM_PART_WAIST_R:				
	case	ITEM_PART_WAIST_L:				
	case	ITEM_PART_ELBOW_R:				
	case	ITEM_PART_ELBOW_L:				
	case	ITEM_PART_HAIR:					
	case	ITEM_PART_HEAD:					
	case	ITEM_PART_FACE:					
	case	ITEM_PART_TRANS:				
	case	ITEM_PART_GROUNDAIR_R:			
	case	ITEM_PART_GROUNDAIR_L:			
	case	ITEM_PART_CREATURE_TYPE_0:		
	case	ITEM_PART_CREATURE_TYPE_1:		
	case	ITEM_PART_CREATURE_TYPE_2:		
	case	ITEM_PART_CREATURE_TYPE_3:		
	case	ITEM_PART_CREATURE_TYPE_4:		
		g_quatTarget [0]	=	in_tag ->position [0];
		g_quatTarget [1]	=	in_tag ->position [1];
		g_quatTarget [2]	=	in_tag ->position [2];
		g_quatTarget [3]	=	0.0f;
	
		D3DXQuaternionConjugate	( (D3DXQUATERNION *)g_quatConjugate,	(D3DXQUATERNION *)in_frame ->quatCoordinate	);
		Fx_Quat3Multiply( g_quatAfterTransform,	g_buffer,	in_frame ->quatCoordinate,	g_quatTarget,	g_quatConjugate );
		
		g_vecAfterTransform [0]	=	g_quatAfterTransform [0]	+
										in_frame ->vecOffset [0];
		g_vecAfterTransform [1]	=	g_quatAfterTransform [1]	+
										in_frame ->vecOffset [1];
		g_vecAfterTransform [2]	=	g_quatAfterTransform [2]	+
										in_frame ->vecOffset [2];
	
		D3DXQuaternionMultiply ( (D3DXQUATERNION *)g_quatAfterTransform,	(D3DXQUATERNION *)in_frame ->quatCoordinate,	(D3DXQUATERNION *)in_tag ->rotation	);
		
		D3DXQuaternionInverse ( (D3DXQUATERNION *)g_quatAfterTransform,		(D3DXQUATERNION *)g_quatAfterTransform );

		D3DXMatrixTransformation ( (D3DXMATRIX *)&g_mat,	NULL,	NULL,	NULL,	NULL,	(D3DXQUATERNION *)g_quatAfterTransform,	(D3DXVECTOR3 *)g_vecAfterTransform );

		if	(	in_transform	)
			D3DXMatrixMultiply ( (D3DXMATRIX *)&out_matrix,	(D3DXMATRIX *)&g_mat,	(D3DXMATRIX *)&in_char ->stat_matTransform );
		else
			memcpy( &out_matrix, &g_mat, sizeof(matrix4x4_t) );

		break;
	case	ITEM_PART_GROUND:
		g_vecAfterTransform [0]	=	in_char ->position [0];
		g_vecAfterTransform [1]	=	in_char ->position [1];
		g_vecAfterTransform [2]	=	5.0f;
		
		D3DXQuaternionRotationYawPitchRoll ( (D3DXQUATERNION *)g_quatAfterTransform,	deg2rad ( in_char ->angles [YAW] ) + __HALFPI,	0.0f,	0.0f );
					
		D3DXQuaternionInverse (	(D3DXQUATERNION *)g_quatAfterTransform,	(D3DXQUATERNION *)g_quatAfterTransform );

		D3DXMatrixTransformation ( (D3DXMATRIX *)&out_matrix,	NULL,	NULL,	NULL,	NULL,	
									(D3DXQUATERNION *)g_quatAfterTransform,	(D3DXVECTOR3 *)g_vecAfterTransform );
			
		break;
	}
}

 
 
 
 
void	FX_CItemManager::TagLookTarget	(	const vec3_t	in_mypos,	
											const int		in_tgtIdx,
											vec4_t			out_quat	)
{
	g_direction [0]	=	1.0f;
	g_direction [1]	=	1.0f;
	g_direction [2]	=	1.0f;

	BuildQuaternion (	g_baseVector,	g_direction,	out_quat	);

	return;
}

 
 
 
 
void	FX_CItemManager::TagLookCenter	(	const vec3_t	in_direction,
											vec4_t			out_quat	)
{
	BuildQuaternion (	g_baseVector,	in_direction,	out_quat	);

	return;
}

 
 
 
 
void	FX_CItemManager::GetTagMatrixUntransformed	(	Fx_CHARACTER_t	*in_char,
														const int		in_tag,
														const int		in_parentTag,	
														matrix4x4_t		&out_matrix	)
{
	KEYFRAME		*thisFrame	=	&in_char ->key_resultFrames [m_pSkelManager ->m_iBoneLookUp [in_char ->attr_skelID][in_parentTag]];
	TAG				*thisTag	=	&m_pSkelManager ->m_Skels [in_char ->attr_skelID].skeleton.m_Tag [in_tag];
	MakeTagMatrix ( in_char,	thisFrame,	thisTag,	in_tag,	out_matrix,	false );
}


 
 
 
 
 
 
 
 
 
#ifdef	_FX_CLIENT

 
 
 
 
void	FX_CItemManager::GetCharPosByIdxOnSvr	(	const int	in_targetIdx,
													vec3_t		out_pos		)
{
	Fx_CHARACTER_t	*thisChar;
	for	(	int	index	=	0;	\
				index	<	m_pCharManager ->m_iNumCharacters;	\
				++index		)
	{
		if	(	m_pCharManager ->m_Characters [index].idxOnServer	==	in_targetIdx	)
		{
			thisChar	=	&m_pCharManager ->m_Characters [index];
			break;
		}
	}

	out_pos [0]	=	thisChar ->position [0];
	out_pos [1]	=	thisChar ->position [1];
	out_pos [2]	=	thisChar ->position [2];

	return;
}



#endif