 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"
#include <ASSERT.H>

#define		g_quatConjugate				GLOBAL_QUAT_1
#define		g_quatAfterTransform		GLOBAL_QUAT_2
#define		g_quatBuffer				GLOBAL_QUAT_3

#define		g_rot_matrix		GLOBAL_DXMATRIX_1
#define		g_unit_x			GLOBAL_DXVEC3_3
#define		g_inter				GLOBAL_DXVEC3_2
#define		g_after_start		GLOBAL_DXVEC4_1
#define		g_after_end			GLOBAL_DXVEC4_2
#define		g_interpolated		GLOBAL_DXVEC4_3

#define		g_groundvector		GLOBAL_VEC3_1

 
 
 
	 
FX_CAnim::FX_CAnim	()
{
	m_eopframeIdx		=	NULL;
	m_eoaframeIdx		=	NULL;
	
	m_iNumTrack		=	0;

	m_dwNumIndices	=	0;
	m_dwNumVertices =	0;
}

 
 
 
FX_CAnim::~FX_CAnim()
{
}


 
 
 
bool	FX_CAnim::Clear()
{
	m_iNumTrack		=	0;

	m_dwNumIndices	=	0;
	m_dwNumVertices =	0;

	m_eopframeIdx	=	NULL;
	m_eoaframeIdx	=	NULL;

	return	true;
}

 
 
 
bool	FX_CAnim::LoadData	(	char		*pszFilename,
								CFileMng	*fileMng	)
{
	BYTE	*pFileBuffer;
	int		offset	=	0;
	int		fileSize;
	pFileBuffer	=	fileMng->ReadFileFromPack( pszFilename );
	fileSize	=	(int)fileMng->GetFileSizeFromPack( pszFilename );
	 
	int		allocatedMemory	=	0;

#define	__GETDATA(a, b)	GetDataFromBuffer( (void *)(a), pFileBuffer, (b), offset )
	
#define	__CALC_ALLOCATED_MEMORY(a)	if( g_resCounter->Enabled() ) { allocatedMemory += (a); }
	
	
	char	version [MAX_PATH];
	__GETDATA( version, strlen("VERSION") );
	version [strlen("VERSION")]	=	NULL;
	if	(	strcmp ( version, "VERSION" )	)
	{
		m_fileVersion	=	100;
		offset	=	4;
	}
	else
	{
		__GETDATA( version, 3 );
		version [3]	=	NULL;
		m_fileVersion	=	atoi ( version );
		if	(	m_fileVersion	<	100	)
		{
			 

			m_fileVersion	=	100;
		}
		__GETDATA( version, 4 );
		version [4]	=	NULL;
		if	(	strcmp ( version, "ANIM" )	)
		{
			 
		}
	}

	 
	int		iDescLength;
	__GETDATA( &iDescLength, sizeof(int) );
	__GETDATA( m_strDesc, iDescLength );
	m_strDesc[ iDescLength ]	=	'\0';

	 
	__GETDATA( &m_pBoneStID, sizeof(FX_BONESTRUCT_ID) );

		 
	__GETDATA( &m_animProperty, sizeof(ANIM_PROPERTY_t) );

	 
	__GETDATA( &m_totalFrames, sizeof(int) );
	assert( m_totalFrames > 0 );

		 
	__GETDATA( &m_timePerFrame, sizeof(float) );
	assert( m_timePerFrame >= 0.0f );

	m_framePerTime	=	1.0f / m_timePerFrame;
	m_totalTime		=	m_timePerFrame * m_totalFrames;
	m_rcptotalTime	=	1.0f / m_totalTime;

	 
	__GETDATA( &m_iNumTrack, sizeof(int) );
	assert( m_iNumTrack > 0 && m_iNumTrack <= MAX_COUNT_BONE_IN_CHAR );

	 
	m_Tracks		=	new TRACK[ m_iNumTrack ];
	m_eopframeIdx	=	new	int[ m_iNumTrack ];
	m_eoaframeIdx	=	new	int[ m_iNumTrack ];
	__CALC_ALLOCATED_MEMORY( sizeof(TRACK) * m_iNumTrack - sizeof(TRACK*) )
	__CALC_ALLOCATED_MEMORY( sizeof(int) * m_iNumTrack - sizeof(int*) )
	__CALC_ALLOCATED_MEMORY( sizeof(int) * m_iNumTrack - sizeof(int*) )

	for	(	int	iTrackID	=	0;	\
				iTrackID	<	m_iNumTrack;	\
				++iTrackID	)
	{
		TRACK	*thisTrack	=	&m_Tracks[ iTrackID ];
		
		 
		__GETDATA( &thisTrack->iBoneID, sizeof(int) );
		assert( thisTrack->iBoneID >= 0 );

		 
		__GETDATA( &thisTrack->iNumKeyframe, sizeof(int) );
		assert( thisTrack->iNumKeyframe > 0 );

		 
		thisTrack->pKeyframe	=	new KEYFRAME[ thisTrack->iNumKeyframe ];
		__CALC_ALLOCATED_MEMORY( sizeof(KEYFRAME) * thisTrack->iNumKeyframe - sizeof(KEYFRAME*) )

			 
		__GETDATA( thisTrack->pKeyframe, sizeof(KEYFRAME) * thisTrack->iNumKeyframe );
	}

		 
	int		numFrame;
	__GETDATA( &numFrame, sizeof(int) );
	m_boundBox		=	new	bboxf_t[ numFrame ];
	__GETDATA( m_boundBox, sizeof(bboxf_t) * numFrame );
	__CALC_ALLOCATED_MEMORY( sizeof(bboxf_t) * numFrame - sizeof(bboxf_t*) )

	if	(	fileSize	<=	offset	)
	{
		m_eopframe	=	ANIM_FRAME_ERROR_VALUE;
		m_eoaframe	=	ANIM_FRAME_ERROR_VALUE;
	}
	else 
	{
		__GETDATA( &m_eopframe, sizeof(float) );
		__GETDATA( &m_eoaframe, sizeof(float) );
	}

	if	(	m_eopframe	>	100000.0f	)
		m_eopframe	=	ANIM_FRAME_ERROR_VALUE;
	if	(	m_eoaframe	>	100000.0f	)
		m_eoaframe	=	ANIM_FRAME_ERROR_VALUE;

	SetEOPFrame();
	SetEOAFrame();

	m_initialHeight	=	m_Tracks[ 0 ].pKeyframe[ 0 ].vecOffset[ 1 ];

	GTH_SAFE_FREE( pFileBuffer );

	__CALC_ALLOCATED_MEMORY( sizeof(FX_CAnim) - sizeof(FX_CAnim*) )
	
	 
	if	(	g_resCounter->Enabled()	)
	{
		char	*lowercase	=	_strlwr( _strdup( pszFilename ) );

		if	(	strstr( lowercase, "mon" )	)
		{
			g_resCounter->Add_DataType( RESCOUNTER_ANIM_CHAR_MONSTER, allocatedMemory );
		}
		else if	(	strstr( lowercase, "ava" ) )
		{
			g_resCounter->Add_DataType( RESCOUNTER_ANIM_CHAR_PC, allocatedMemory );
		}
		else if (	strstr( lowercase, "npc" )	)
		{
			g_resCounter->Add_DataType( RESCOUNTER_ANIM_CHAR_NPC, allocatedMemory );
		}
		free( lowercase );
	}

	return	true;
}

 
 
 
void	FX_CAnim::Cleanup	()
{
	GTH_SAFE_DELETE_ARRAY ( m_boundBox );

	for	(	int	i	=	0;	\
				i	<	m_iNumTrack;	\
				++i		)
	{
		GTH_SAFE_DELETE_ARRAY ( m_Tracks [i].pKeyframe );
	}
	GTH_SAFE_DELETE ( m_eoaframeIdx );
	GTH_SAFE_DELETE ( m_eopframeIdx );

	GTH_SAFE_DELETE_ARRAY ( m_Tracks );

	return;
}

 
 
 
HRESULT	FX_CAnim::Interpolate	(	const KEYFRAME	*prevKeyframe,
									const KEYFRAME	*nextKeyframe, 
									const float		in_displacement,
									KEYFRAME		*result	)
{
	ZeroMemory( result, sizeof(KEYFRAME) );

	result->frame	=	prevKeyframe->frame + (nextKeyframe->frame - prevKeyframe->frame) * in_displacement;

	 
	result->vecOffset[ 0 ]	=	prevKeyframe ->vecOffset[ 0 ] + in_displacement * ( nextKeyframe->vecOffset[ 0 ] - prevKeyframe->vecOffset[ 0 ] );
	result->vecOffset[ 1 ]	=	prevKeyframe ->vecOffset[ 1 ] + in_displacement * ( nextKeyframe->vecOffset[ 1 ] - prevKeyframe->vecOffset[ 1 ] );
	result->vecOffset[ 2 ]	=	prevKeyframe ->vecOffset[ 2 ] + in_displacement * ( nextKeyframe->vecOffset[ 2 ] - prevKeyframe->vecOffset[ 2 ] );

	 
	D3DXQuaternionSlerp(	(D3DXQUATERNION *)result->quatRotate, 
							(D3DXQUATERNION *)prevKeyframe->quatRotate,
							(D3DXQUATERNION *)nextKeyframe->quatRotate, 
							in_displacement	);

	D3DXQuaternionSlerp(	(D3DXQUATERNION *)result->quatCoordinate, 
							(D3DXQUATERNION *)prevKeyframe->quatCoordinate,
							(D3DXQUATERNION *)nextKeyframe->quatCoordinate, 
							in_displacement	);

	return	S_OK;
}

 
 
 
void	FX_CAnim::MakeInterpolatedVertices	(	Fx_CHARACTER_t	*in_char,
												const int		in_ended	)
{
	int		index;
	KEYFRAME	*curKeyframe;
	KEYFRAME	*nextKeyframe;

	if	(	ANIMCHECK_RESULT_NORMAL	==	in_ended	)
	{
		for	(	index	=	0;	\
				index	<	m_iNumTrack;	\
				++index		)
		{
			curKeyframe		=	&m_Tracks[ index ].pKeyframe[ in_char->key_curIndex[ index ] ];
			nextKeyframe	=	&m_Tracks[ index ].pKeyframe[ in_char->key_nextIndex[ index ] ];
			Interpolate( curKeyframe, nextKeyframe, in_char->key_displace[ index ], &in_char->key_curFrames[ index ] );
		}
	}
	else
	{
		for	(	index	=	0;	\
				index	<	m_iNumTrack;	\
				++index		)
		{
			assert( in_char->key_curIndex[ index ] < m_Tracks[ index ].iNumKeyframe );
			curKeyframe	=	&m_Tracks[ index ].pKeyframe[ in_char->key_curIndex[ index ] ];
		}
	}

	TransformKeyframes( in_char );

	return;
}






int		FX_CAnim::CheckKeyframeState (	Fx_CHARACTER_t	*in_char,
										const int		in_option	)
{
	
	
	if( m_iNumTrack > MAX_COUNT_BONE_IN_CHAR || m_iNumTrack <= 0 )
	{
		ErrorLog( "num track is over %s, %d anim(%d numTrack)", in_char->name, in_char->ani_curIdx, m_iNumTrack );
	}
	
	int		index;
	TRACK	*thisTrack;
	int		endOfAnim	=	ANIMCHECK_RESULT_NORMAL;

	int		curFrame;
	int		nextFrame;
	int		finalFrame;

	float	oldTimeFrame	=	in_char ->ani_oldFrame;
	float	curTimeFrame;
	
	
	if	(	true	==	in_char->ani_curState.type.movable	)
	{
		float fFrameSpeed = (in_char ->ani_curSpeed * m_timePerFrame) * in_char ->ren_generalSpeed * 0.01f;
		if(fFrameSpeed > 0.0f)
		{
			curTimeFrame	=	oldTimeFrame  +  g_timer .GetElapsedTime () / fFrameSpeed;
		}
		else
		{
			curTimeFrame = oldTimeFrame;
		}		
	}
	else
	{
		

		float fFrameSpeed = (in_char ->ani_curSpeed * m_timePerFrame) * in_char ->ren_generalSpeed * 0.01f;
		if(fFrameSpeed > 0.0f)
		{
			curTimeFrame	=	oldTimeFrame  +  g_timer .GetElapsedTime () / fFrameSpeed;
		}
		else
		{
			curTimeFrame	=oldTimeFrame;
		}		
		
	}
	
	in_char ->ani_oldFrame	=	curTimeFrame;
	in_char ->ani_fraction	=	curTimeFrame / m_totalFrames;

	if	(	(	in_option	==	ANIMCHECK_EOP	)	&&
			(	curTimeFrame	>	m_eopframe	)	)
	{
		memcpy( in_char->key_curIndex, m_eopframeIdx, sizeof(int) * m_iNumTrack );
		memcpy( in_char->key_nextIndex, m_eopframeIdx, sizeof(int) * m_iNumTrack );
		memset( in_char->key_displace, 0, sizeof (float) * m_iNumTrack );
		endOfAnim	=	ANIMCHECK_RESULT_EOP;
	}
	else if	(	(	in_option	==	ANIMCHECK_EOA	)	&&
				(	curTimeFrame	>	m_eoaframe	)	)
	{
		memcpy(	in_char->key_curIndex, m_eoaframeIdx, sizeof(int) * m_iNumTrack );
		memcpy(	in_char->key_nextIndex, m_eoaframeIdx, sizeof(int) * m_iNumTrack );
		memset(	in_char->key_displace, 0, sizeof(float) * m_iNumTrack );
		endOfAnim	=	ANIMCHECK_RESULT_EOA;
	}
	else
	{
		









		
		if( m_Tracks == NULL )
		{
			ErrorLog( "num track is over %s, %d anim(%d numTrack)", in_char->name, in_char->ani_curIdx, m_iNumTrack );
		}

		thisTrack		=	&m_Tracks [0];
		for	(	index	=	0;	\
				index	<	m_iNumTrack;	\
				++index,	++thisTrack		)
		{
			curFrame	=	in_char ->key_curIndex [index];
			finalFrame	=	thisTrack ->iNumKeyframe - 1;
			
			if ( finalFrame > curFrame )
			{
				while	(	(	curTimeFrame	>	thisTrack ->pKeyframe [curFrame + 1] .frame	)	&&
					(	curFrame	!=	finalFrame	)	)
				{
					++curFrame;

					if (curFrame>=finalFrame)
					{
						curFrame=finalFrame;
						break;
					}
				}
			}

			if	(	curFrame	!=	finalFrame	)
			{
				nextFrame	=	curFrame + 1;		
				in_char ->key_displace [index]	=	( curTimeFrame  -  thisTrack ->pKeyframe [curFrame].frame ) / 
														( thisTrack ->pKeyframe [nextFrame].frame  -  thisTrack ->pKeyframe [curFrame].frame );
			}
			else
			if (curFrame==finalFrame)
			{
				nextFrame	=	curFrame;
				endOfAnim	=	ANIMCHECK_RESULT_END;
			}
			else
			{
				nextFrame	=	curFrame + 1;
				endOfAnim	=	ANIMCHECK_RESULT_END;
			}

			in_char ->key_curIndex [index]	=	curFrame;
			in_char ->key_nextIndex [index]	=	nextFrame;
		}
	}

	return	endOfAnim;
}

 
 
 
 
void	FX_CAnim::TransformKeyframes	(	Fx_CHARACTER_t	*in_char	)
{
	KEYFRAME	*in_curKeyframe,	*in_resultKeyframe;
	in_curKeyframe		=	in_char ->key_curFrames;
	in_resultKeyframe	=	in_char	->key_resultFrames;

	static D3DXVECTOR3		vecGround;
	static D3DXQUATERNION	quatConjugate;
	static D3DXQUATERNION	quatAfterTransform;
	static D3DXQUATERNION	quatBuffer;
	static const D3DXQUATERNION		QUAT_UNIT_X( 1.0f, 0.0f, 0.0f, 0.0f );

	 
	for	(	int	index	=	0;	\
				index	<	m_iNumTrack;	\
				++index	)
	{
		m_iBoneID	= m_Tracks[ index ].iBoneID;

		if	(	-1	==	m_pSkeleton->m_pBones[ m_iBoneID ].iParentID	)
		{
			in_char->stat_disp_x	=	in_curKeyframe[ index ].vecOffset[ 0 ] * in_char->stat_scale;
			in_char->stat_disp_y	=	in_curKeyframe[ index ].vecOffset[ 1 ] * in_char->stat_scale;
			in_char->stat_height	=	in_curKeyframe[ index ].vecOffset[ 2 ] * in_char->stat_scale;
			
				 
				 
			DXVectorSet3( vecGround, 0.0f, 0.0f, -in_char->stat_height + 5.0f );
			m_pSkeleton->SetGroundTag( (float *)&vecGround );

			VectorSet( in_resultKeyframe[ index ].vecOffset, in_char->stat_disp_x, in_char->stat_disp_y, 0.0f );

			Vector4Copy( in_resultKeyframe[ index ].quatCoordinate,	in_curKeyframe[ index ].quatCoordinate );
			Vector4Copy( in_resultKeyframe[ index ].quatRotate,	in_curKeyframe[ index ].quatRotate );
		}
		else if	(	0	==	m_pSkeleton->m_pBones[ m_iBoneID ].iParentID	)
		{
			VectorCopy( in_resultKeyframe[ index ].vecOffset, in_resultKeyframe[ m_pSkeleton->m_pBones[ m_iBoneID ].iParentID ].vecOffset );

			D3DXQuaternionMultiply(	(D3DXQUATERNION *)in_resultKeyframe[ index ].quatCoordinate, 
									(D3DXQUATERNION *)in_resultKeyframe[ m_pSkeleton->m_pBones[ m_iBoneID ].iParentID ].quatCoordinate,
									(D3DXQUATERNION *)in_curKeyframe[ index ].quatCoordinate );
		}
		else
		{
			D3DXQuaternionConjugate( &quatConjugate, (D3DXQUATERNION *)in_curKeyframe[ index ].quatRotate );

			Fx_Quat3Multiply( (float *)&quatAfterTransform, (float *)&quatBuffer, in_curKeyframe[ index ].quatRotate, QUAT_UNIT_X, (float *)&quatConjugate );

			in_resultKeyframe[ index ].vecOffset[ 0 ]	=	quatAfterTransform.x * m_pSkeleton->m_pBones[ m_iBoneID ].fLength	+	
															in_resultKeyframe[ m_pSkeleton->m_pBones[ m_iBoneID ].iParentID ].vecOffset[ 0 ];
			in_resultKeyframe[ index ].vecOffset[ 1 ]	=	quatAfterTransform.y * m_pSkeleton->m_pBones[ m_iBoneID ].fLength	+	
															in_resultKeyframe[ m_pSkeleton->m_pBones[ m_iBoneID ].iParentID ].vecOffset[ 1 ];
			in_resultKeyframe[ index ].vecOffset[ 2 ]	=	quatAfterTransform.z * m_pSkeleton->m_pBones[ m_iBoneID ].fLength	+	
															in_resultKeyframe[ m_pSkeleton->m_pBones[ m_iBoneID ].iParentID ].vecOffset[ 2 ];

			Vector4Copy( quatBuffer, in_curKeyframe[ index ].quatCoordinate );
			Fx_Quat2Multiply( in_resultKeyframe[ index ].quatCoordinate,	in_resultKeyframe[ m_pSkeleton->m_pBones[ m_iBoneID ].iParentID ].quatCoordinate, quatBuffer );
		}
	}

	return;
}

 
 
 
 
void	FX_CAnim::QuatInterpolate	(	vec4_t			out_quat,
										const vec4_t	in_start,
										const vec4_t	in_end,
										const float		in_disp	)
{
	const float		error	=	0.0001f;
	g_unit_x [0]	=	1.0f;
	g_unit_x [1]	=	0.0f;
	g_unit_x [2]	=	0.0f;

	D3DXMatrixRotationQuaternion	(	(D3DXMATRIX *)&g_rot_matrix,	(D3DXQUATERNION *)in_start	);
	D3DXVec3Transform	(	(D3DXVECTOR4 *)g_after_start,	(D3DXVECTOR3 *)g_unit_x,	(D3DXMATRIX *)&g_rot_matrix	);

	D3DXMatrixRotationQuaternion	(	(D3DXMATRIX *)&g_rot_matrix,	(D3DXQUATERNION *)in_end	);
	D3DXVec3Transform	(	(D3DXVECTOR4 *)g_after_end,		(D3DXVECTOR3 *)g_unit_x,	(D3DXMATRIX *)&g_rot_matrix	);

	g_inter [0]	=	g_after_start [0]	+	( g_after_end [0] - g_after_start [0] ) * in_disp;
	g_inter [1]	=	g_after_start [1]	+	( g_after_end [1] - g_after_start [1] ) * in_disp;
	g_inter [2]	=	g_after_start [2]	+	( g_after_end [2] - g_after_start [2] ) * in_disp;

	if	(	VectorLength ( g_inter )	<	error	)
		D3DXQuaternionIdentity	(	(D3DXQUATERNION *)out_quat	);
	else
	{
		D3DXVec3Normalize	(	(D3DXVECTOR3 *)g_inter,	(D3DXVECTOR3 *)g_inter	);
		BuildQuaternion	(	g_unit_x,	g_inter,	out_quat	);
	}
	
	return;
}

 
 
 
 
void	FX_CAnim::SetEOPFrame	()
{
	int		index,	subindex;

	if	(	m_eopframe	>=	ANIM_FRAME_ERROR_VALUE	)
		return;

	float	desiredFrame	=	m_eopframe	+	FX_ANIM_ERROR;
	TRACK	*thisTrack;

	for	(	index	=	0;	\
			index	<	m_iNumTrack;	\
			++index		)
	{
		thisTrack	=	&m_Tracks [index];
		for	(	subindex	=	0;	\
				subindex	<	thisTrack ->iNumKeyframe;	\
				++subindex		)
		{
			if	(	desiredFrame	<	thisTrack ->pKeyframe [subindex].frame	)
				break;
		}
		m_eopframeIdx [index]	=	subindex - 1;
	}
	
	return;
}

 
 
 
 
void	FX_CAnim::SetEOAFrame	()
{
	int		index,	subindex;

	if	(	m_eoaframe	<	0	)
		return;

	float	desiredFrame	=	m_eoaframe	+	FX_ANIM_ERROR;
	TRACK	*thisTrack;

	for	(	index	=	0;	\
			index	<	m_iNumTrack;	\
			++index		)
	{
		thisTrack	=	&m_Tracks [index];
		for	(	subindex	=	0;	\
				subindex	<	thisTrack ->iNumKeyframe;	\
				++subindex		)
		{
			if	(	desiredFrame	<	thisTrack ->pKeyframe [subindex].frame	)
				break;
		}
		m_eoaframeIdx [index]	=	subindex - 1;
	}
	
	return;
}

 
 
 
 
void	FX_CAnim::SetComboStart	(	Fx_CHARACTER_t*		in_char	)
{
	float	speedFactor		=	in_char ->ani_curSpeed * m_timePerFrame;
	
	memcpy(	in_char ->key_curIndex,	m_eopframeIdx,	sizeof(int) * m_iNumTrack	);

	in_char ->ani_elapsed	=	m_eopframe * speedFactor;

	if	(	in_char ->flag_useRHandAura	)
	{
		in_char ->ren_auraRHandData.nextUpdate	=	in_char ->ani_elapsed	-	FX_ANIM_ERROR;
	}
	if	(	in_char ->flag_useLHandAura	)
	{
		in_char ->ren_auraLHandData.nextUpdate	=	in_char ->ani_elapsed	-	FX_ANIM_ERROR;
	}

	return;
}

 
 
 
 
float	FX_CAnim::GetInitialRadius	()
{
	vec3_t	subtract;
	vec3_t	min;
	vec3_t	max;

	min [0]	=	m_boundBox [0][0];
	min [1]	=	m_boundBox [0][1];
	min [2]	=	0.0f;

	max [0]	=	m_boundBox [0][3];
	max [1]	=	m_boundBox [0][4];
	max [2]	=	0.0f;

	VectorSubtract ( subtract, max,	min );

	return	( VectorLength ( subtract ) * 0.5f );
}

