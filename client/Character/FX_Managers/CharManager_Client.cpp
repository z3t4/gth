 
 
 
 
 
 
 
 
 

 
 
 

#pragma warning( disable : 4786 )	

#include <assert.h>
#include <algorithm>

#ifdef	_FX_CLIENT

 
 
 
#include "../../global_def.h"
#include "../Common.h"

#define		g_temporaryPanelPos		GLOBAL_VEC3_1
#define		g_temporaryPanelBBox	GLOBAL_BBOX

#define		g_worldPosPelvis		GLOBAL_DXVEC3_1
#define		g_afterTransform		GLOBAL_DXVEC4_1
#define		g_camCharDistance		GLOBAL_DXVEC3_2
#define		g_screenPosition		GLOBAL_DXVEC3_3

#define		g_cdfinalangle			GLOBAL_VEC3_1
#define		g_cdconvertedvector		GLOBAL_VEC3_2
#define		g_cdpoint				GLOBAL_DXVEC3_1
#define		g_cdnormal				GLOBAL_DXVEC3_2
#define		g_cdoutpoint			GLOBAL_DXVEC4_1

#define		g_inverse				GLOBAL_DXMATRIX_1
#define		g_xformed_start_pre		GLOBAL_DXVEC4_1
#define		g_xformed_end_pre		GLOBAL_DXVEC4_2
#define		g_xformed_start			GLOBAL_DXVEC3_8
#define		g_xformed_end			GLOBAL_DXVEC3_9
#define		g_collidedPoint			GLOBAL_DXVEC3_10
#define		g_collidedPointNormal	GLOBAL_DXVEC3_11

#define		g_line_mid				GLOBAL_DXVEC3_1
#define		g_line_fquarter			GLOBAL_DXVEC3_2
#define		g_line_bquarter			GLOBAL_DXVEC3_3
#define		g_expansion_pre			GLOBAL_DXVEC3_4
#define		g_expansion				GLOBAL_DXVEC3_5
#define		g_displace				GLOBAL_DXVEC3_6
#define		g_thisPoint				GLOBAL_DXVEC3_7

#define		g_charBBox				GLOBAL_BBOX

 
 
 
void	FX_CCharManager::ChangeAnimation	(	const int	iCharID, 	
												const int	aniSetType,		
												const int	aniSetIndex, 	
												const int	aniSetID,
												const float	*in_animSpeed,
												const bool	in_forceChage	)
{

	 
	Fx_CHARACTER_t*		pThisChar	=	&m_Characters [iCharID];

	Fx_ANIM_INFO_t	*nextState;

	int				nextIdx;

	if	(	(	aniSetType	<	0	)	||
			(	aniSetIndex	<	0	)	||
			(	aniSetID	<	0	)	)
		return;

	nextIdx		=	m_pCharLookupMng ->GetAnimIDFromTable (	pThisChar->entityType,	pThisChar->tableIdx, aniSetType,	aniSetIndex,	aniSetID, pThisChar->ID	);

	
	if( nextIdx < 0 || nextIdx > MAX_COUNT_ANIM )
		return;

	nextState	=	&m_pCharLookupMng ->m_aniInfo [nextIdx];

	if	(	(	true	==	pThisChar ->ani_curState.type.changable	)	||
			in_forceChage	)
	{  
		 
		if	(	pThisChar ->ani_elapsed	==	0.0f	)
			MakeInterpolator( pThisChar, 0.05f, pThisChar->ani_curIdx, nextIdx, INTERPOLATE_NORMAL );
		else 
			MakeInterpolator( pThisChar, 0.05f, pThisChar->ani_curIdx, nextIdx, INTERPOLATE_USE_CURFRAME );

		pThisChar ->ani_nextIdx		=	-1;
		pThisChar ->ani_nextGroup	=	FX_ANIM_GROUP_NULL;

		pThisChar ->ani_option		=	ANIMOPTION_START_NEW_ANIM;

		pThisChar ->ani_curIdx			=	nextIdx;
		pThisChar ->ani_curGroup		=	nextState ->groupID;
		pThisChar ->ani_curState.value	=	nextState ->state.value;

		pThisChar ->ani_curSetType		=	aniSetType;

		
		if( aniSetType == ANIMTYPE_BYITEM ) pThisChar ->ani_curItemSetIndex	=	aniSetIndex;
		pThisChar ->ani_curSetID		=	aniSetID;
		pThisChar ->ani_fraction		=	0.0f;

		if	(	(	in_animSpeed	!=	NULL	)	)
		{
			SetAnimationSpeedInSec (	pThisChar,	*in_animSpeed, false	);
			pThisChar ->stat_attackTime	=	(*in_animSpeed);
		}
		else
		{
			pThisChar ->ani_curSpeed	=	1.0f;		 
			pThisChar ->ani_curRSpeed	=	1.0f;
		}
	}
	else
	{  
		 
		pThisChar ->ani_nextIdx			=	nextIdx;
		pThisChar ->ani_nextGroup		=	nextState ->groupID;
		pThisChar ->ani_nextState.value	=	nextState ->state.value;

		pThisChar->ani_nextSetType		=	aniSetType;
		pThisChar->ani_nextItemSetIndex	=	aniSetIndex;
		pThisChar->ani_nextSetID			=	aniSetID;
		if	(	(	in_animSpeed	!=	NULL	)	)
		{
			SetAnimationSpeedInSec (	pThisChar,	*in_animSpeed, true	);
		}
		else
		{
			pThisChar ->ani_nextSpeed	=	1.0f;		 
			pThisChar ->ani_nextRSpeed	=	1.0f;
		}
	}

	return;
}

 
 
 
 
float	FX_CCharManager::GetCharPanelScreenPos	(	Fx_CHARACTER_t	*in_char,
													vec3_t			out_pos	)
{
	float	w;

	g_temporaryPanelPos [0]	=	in_char ->position [0];
	g_temporaryPanelPos [1]	=	in_char ->position [1];
	g_temporaryPanelPos [2]	=	in_char ->position [2] + in_char ->attr_tall;

	w	=	g_camera .GetTransformMatrix() ->Transform (	out_pos,	g_temporaryPanelPos	);

	out_pos [0]	=	( 1.0f + out_pos [0] ) * g_cursor->m_viewport.Width * 0.5f;
	out_pos [1]	=	( 1.0f - out_pos [1] ) * g_cursor->m_viewport.Height * 0.5f;
	
	return( w );
}

 
 
 
 
Fx_CHARACTER_t	*FX_CCharManager::SearchCursorEntityNeo2 (	const vec3_t	in_camPos,			 
															const vec3_t	in_camAngle,		 
															matrix4x4_t		*in_camTransMat,	 
															const float		in_minSquareDist,	 
															const vec2_t	in_mousePos,		 
															const vec2_t	in_viewportSize	)	 
{
	Fx_CHARACTER_t		*thisChar			=	NULL;
	Fx_CHARACTER_t		*firstCandidate		=	NULL;
	Fx_CHARACTER_t		*thisCandidate		=	NULL;
	Fx_CHARACTER_t		*closestCandidate	=	NULL;
	bool				isFirstCandidate	=	true;
	
	bboxf_t		bbox;
	float		halfWidth,	halfHeight;
	int			index;
	float		red_ratio;
	float		diff;
	float		angle;
	float		minx,	maxx,	miny,	maxy,	minz,	maxz;
	float		sqr_length;
	float		trival;
	float		ang_diff;

	const float	DISCARD_X		=	100.0f;
	const float	DISCARD_Y		=	100.0f;

	m_minSquareDist	=	100000000.0f;

	 
	thisChar		=	&m_Characters [0];
	for	(	index	=	0;	\
			index	<	m_iNumCharacters;	\
			++index,	++thisChar	)
	{
		if	(	thisChar ->flag_culled	)	continue;

		g_worldPosPelvis [0]	=	thisChar ->stat_disp_x;
		g_worldPosPelvis [1]	=	thisChar ->stat_disp_y;
		g_worldPosPelvis [2]	=	0.0f;
		 
		D3DXVec3Transform	(	(D3DXVECTOR4 *)g_afterTransform,	(D3DXVECTOR3 *)g_worldPosPelvis,	(D3DXMATRIX *)&thisChar ->stat_matTransform	);

		VectorCopy ( thisChar ->ren_xformedPelvisPos,	g_afterTransform );

		 
		VectorSubtract ( g_camCharDistance,	thisChar ->ren_xformedPelvisPos,	in_camPos );
		thisChar ->ren_squareDist	=	VectorDotProduct ( g_camCharDistance, g_camCharDistance );
		if	(	thisChar ->ren_squareDist	>	m_minSquareDist	)
			continue;

		if	(	thisChar ->flag_disappear	)	continue;

		
		
		if( thisChar ->entityType != ENTITY_PC )
		{
			
			if( g_cgv.myCharacterInfo->pcJob == J_STRANGER )
			{
				if( thisChar->stat_alive > MON_ALIVE_STATE_CORPSE ) continue;
			}
			
			else
			{
				if( thisChar->stat_alive >= MON_ALIVE_STATE_CORPSE ) continue;
			}
		}

		
		if( !GTH_RenderGM( thisChar ) ) continue;

		thisChar ->ren_closerChar	=	NULL;
		thisChar ->ren_fartherChar	=	NULL;
		if	(	! isFirstCandidate	)
		{
			thisCandidate	=	firstCandidate;
			if	(	thisChar ->ren_squareDist	<	firstCandidate ->ren_squareDist	)
			{
				while	(1)
				{
					if	(	thisCandidate ->ren_squareDist	<	thisChar ->ren_squareDist	)
					{
						if	(	thisCandidate ->ren_fartherChar	)
						{
							thisChar ->ren_fartherChar	=	thisCandidate ->ren_fartherChar;
							thisCandidate ->ren_fartherChar ->ren_closerChar	=	thisChar;
						}
						thisCandidate ->ren_fartherChar	=	thisChar;
						thisChar ->ren_closerChar		=	thisCandidate;
						break;
					}
					else if	(	thisCandidate ->ren_closerChar	==	NULL	)
					{
						thisCandidate ->ren_closerChar	=	thisChar;
						thisChar ->ren_fartherChar		=	thisCandidate;
						closestCandidate			=	thisChar;
						break;
					}
					thisCandidate	=	thisCandidate ->ren_closerChar;
				}
			}
			else
			{
				while	(1)
				{
					if	(	thisCandidate ->ren_squareDist	>	thisChar ->ren_squareDist	)
					{
						if	(	thisCandidate ->ren_closerChar	)
						{
							thisChar ->ren_closerChar	=	thisCandidate ->ren_closerChar;
							thisCandidate ->ren_closerChar ->ren_fartherChar	=	thisChar;
						}
						thisCandidate ->ren_closerChar	=	thisChar;
						thisChar ->ren_fartherChar	=	thisCandidate;
						break;
					}
					else if	(	thisCandidate ->ren_fartherChar	==	NULL	)
					{
						thisCandidate ->ren_fartherChar	=	thisChar;
						thisChar ->ren_closerChar		=	thisCandidate;
						break;
					}
					thisCandidate	=	thisCandidate ->ren_fartherChar;
				}
			}
		}
		else
		{
			firstCandidate		=	thisChar;
			closestCandidate	=	thisChar;
			isFirstCandidate	=	false;
		}
	}
	
	if	(	closestCandidate	==	NULL	)	return	NULL;

	halfWidth	=	(float)in_viewportSize [0] * 0.5f;
	halfHeight	=	(float)in_viewportSize [1] * 0.5f;
	float	ratioFactor	=	0.5f / (float)tan (g_camera.m_projectParm.fov / g_camera.m_projectParm.aspect * 0.5f * FX_DEGTORAD);

	thisChar		=	closestCandidate;
	do
	{
		 
		red_ratio	=	in_viewportSize [1] * ratioFactor / (float)sqrt (thisChar ->ren_squareDist);

		 
		in_camTransMat ->Transform (	g_screenPosition,	(float *)&thisChar ->ren_xformedPelvisPos	);
		g_screenPosition [0]	=	( 1.0f + g_screenPosition [0] ) * halfWidth;
		g_screenPosition [1]	=	( 1.0f - g_screenPosition [1] ) * halfHeight;

		 
		diff	=	fabs ( in_mousePos [1] - g_screenPosition [1] );
		if	(	diff	>	DISCARD_Y * red_ratio	 )		goto	SKIP;
		diff	=	fabs ( in_mousePos [0] - g_screenPosition [0] );
		if	(	diff	>	DISCARD_X * red_ratio	 )		goto	SKIP;

		 
		GetBoundCharPreTransform( thisChar, bbox );

		 
		diff	*=	diff;		 
		ang_diff	=	in_camAngle [YAW]	-	( thisChar ->angles [YAW] -	90.0f );
		if	(	ang_diff	<	0.0f	)		ang_diff	+=	360.0f;

		 
		minx	=	fabs (thisChar ->stat_disp_x - bbox [0]);
		maxx	=	fabs (thisChar ->stat_disp_x - bbox [3]);
		miny	=	fabs (thisChar ->stat_disp_y - bbox [1]);
		maxy	=	fabs (thisChar ->stat_disp_y - bbox [4]);

		if	(	(	ang_diff	>=	0.0f	)	&&
				(	ang_diff	<	90.0f	)	)
		{
			 
			if	(	in_mousePos [0]	<	g_screenPosition [0]	)
			{
				sqr_length	=	maxx*maxx + miny*miny;
				angle		=	(float)atan2 ( miny, maxx )	+	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs (sin (angle)) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
			else
			{
				sqr_length	=	minx*minx + maxy*maxy;
				angle		=	(float)atan2 ( maxy, minx )	+	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs (sin (angle)) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
		}
		else if	(	(	ang_diff	>=	90.0f	)	&&
					(	ang_diff	<	180.0f	)	)
		{
			 
			if	(	in_mousePos [0]	<	g_screenPosition [0]	)
			{
				sqr_length	=	maxx*maxx + maxy*maxy;
				angle		=	(float)atan2 ( maxy, maxx )	-	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs (sin (angle)) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
			else
			{
				sqr_length	=	minx*minx + miny*miny;
				angle		=	(float)atan2 ( miny, minx )	-	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs (sin (angle)) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
		}
		else if	(	(	ang_diff	>=	180.0f	)	&&
					(	ang_diff	<	270.0f	)	)
		{
			 
			if	(	in_mousePos [0]	<	g_screenPosition [0]	)
			{
				sqr_length	=	minx*minx + maxy*maxy;
				angle		=	(float)atan2 ( maxy, minx )	+	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs (sin (angle)) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
			else
			{
				sqr_length	=	maxx*maxx + miny*miny;
				angle		=	(float)atan2 ( miny, maxx )	+	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs (sin (angle)) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
		}
		else
		{
			 
			if	(	in_mousePos [0]	<	g_screenPosition [0]	)
			{
				sqr_length	=	minx*minx + miny*miny;
				angle		=	(float)atan2 ( miny, minx )	-	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs (sin (angle)) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
			else
			{
				sqr_length	=	maxx*maxx + maxy*maxy;
				angle		=	(float)atan2 ( maxy, maxx )	-	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs (sin (angle)) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
		}

		diff	=	fabs ( in_mousePos [1] - g_screenPosition [1] );
		diff	*=	diff;		 
		ang_diff	=	-in_camAngle [PITCH];
		if	(	ang_diff	<	0.0f	)		ang_diff	+=	180.0f;

		minz	=	fabs (thisChar ->stat_height - bbox [2]);
		maxz	=	fabs (thisChar ->stat_height - bbox [5]);

		if	(	(	in_camAngle	[PITCH]	>=	-90.0f	)	&&
				(	in_camAngle	[PITCH]	<	0.0f	)	)
		{
			if	(	in_mousePos [1]	<	g_screenPosition [1]	)
			{
				sqr_length	=	maxy*maxy + maxz*maxz;
				angle		=	(float)atan2 ( maxz,	maxy )	+	ang_diff * FX_DEGTORAD;
				trival		=	(float)sin (angle) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival  )	)
					goto	SKIP;
			}
			else
			{
				sqr_length	=	minz*minz + miny*miny;
				angle		=	(float)atan2 ( minz,	miny )	+	ang_diff * FX_DEGTORAD;
				trival		=	(float)sin (angle) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
		}
		else
		{
			if	(	in_mousePos [1]	<	g_screenPosition [1]	)
			{
				sqr_length	=	miny*miny + maxz*maxz;
				angle		=	(float)atan2 ( maxz,	miny )	-	ang_diff * FX_DEGTORAD;
				trival		=	(float)sin (angle) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival  )	)
					goto	SKIP;
			}
			else
			{
				sqr_length	=	minz*minz + maxy*maxy;
				angle		=	(float)atan2 ( minz,	maxy )	-	ang_diff * FX_DEGTORAD;
				trival		=	(float)sin (angle) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
		}

		m_minSquareDist	=	thisChar ->ren_squareDist;

		if( thisChar->idxOnServer == g_cgv.myPCIdxOnServer && thisChar->entityType == ENTITY_PC )
		{
			if( g_cgv.state == CLIENT_GAME_PROCESS ) return NULL;
		}
		return		thisChar;
SKIP:
		thisChar	=	thisChar ->ren_fartherChar;
	}	while	(	thisChar	!=	NULL	);

	return	NULL;
}

 
 
 
 
float	FX_CCharManager::GetAnimFraction	(	Fx_CHARACTER_t	*in_char	)
{
	return	in_char ->ani_fraction;
}










































 
 
 
 
void	FX_CCharManager::EquipAllItem (	const int	iCharID	)
{
	int		i,	itemTableIdx;
	const Fx_CHARACTER_t	*pNewChar	=	&m_Characters [iCharID];

	for	(	i	=	0;	\
			i	<	pNewChar ->equipNumber;	\
			i++		)
	{
		itemTableIdx	=	pNewChar ->equipment [i];
		EquipItem (	iCharID,	&g_itemTable [itemTableIdx]	);
	}
}

 
  
 
 
void	FX_CCharManager::SetAnimationSpeedInSec	(	Fx_CHARACTER_t	*in_char,
													const float		in_milisecond, int isNext	)
{
	if( !isNext )
	{
		in_char ->ani_curSpeed		=	in_milisecond / 1000.0f * m_pAnimManager ->m_Anims [in_char ->ani_curIdx].animation.m_rcptotalTime;
		in_char ->ani_curRSpeed	=	1.0f / in_char ->ani_curSpeed;
	}
	else
	{
		in_char ->ani_nextSpeed		=	in_milisecond / 1000.0f * m_pAnimManager ->m_Anims [in_char ->ani_nextIdx].animation.m_rcptotalTime;
		in_char ->ani_nextRSpeed	=	1.0f / in_char ->ani_nextSpeed;
	}

	return;
}







































































































































 
 
 
 
void	FX_CCharManager::SetTargetID	()
{
	int		index,	subindex;

	Fx_CHARACTER_t	*thisChar	=	&m_Characters [0];
	for	(	index	=	0;	\
			index	<	m_iNumCharacters;	\
			++index,	++thisChar		)
	{
		for	(	subindex	=	0;	\
				subindex	<	m_iNumCharacters;	\
				++subindex		)
		{
			if	(	m_Characters [subindex].idxOnServer	==	thisChar ->targetIdx	)
			{
				thisChar ->targetID	=	subindex;
				break;
			}
		}
	}

	return;
}

 
 
 
 
void FX_CCharManager::AddEffect	( int type, int in_effectID, Fx_CHARACTER_t *in_char, float in_zpos )
{
	vec3_t origin, target;
	int targetID;
	Fx_CHARACTER_t *defender = NULL;
	
	targetID = g_GameTarget.GTH_GetTargetClientID( in_char );
	defender = g_GameTarget.GTH_GetTargetPointer( in_char );

	
	if( targetID < 0 || defender == NULL ) return;

	switch( type )
	{
	
	case FX_CCharManager::EFFECT_NORMAL :
		if( defender == NULL ) return;

		VectorCopy( origin, in_char->position ); 
		VectorCopy( target, defender->position );
		origin[2] += in_zpos;
		target[2] += in_zpos;

		g_effectMng ->Add( GTH_EFFECT_CLASS_MODEL, 0, target , NULL, NULL, -20.0f ,
			in_char->entityType, in_char->idxOnServer, defender->entityType, defender->idxOnServer );

		break;
	
	case FX_CCharManager::EFFECT_MISSILE :
		if( defender == NULL ) return;

		VectorCopy( origin, in_char->position ); 
		VectorCopy( target, defender->position );
		origin[2] += in_char->stat_height;
		target[2] += defender->stat_height;

		 
		g_effectMng ->Add( GTH_EFFECT_CLASS_MULTI_MODEL, in_effectID, origin, target, NULL, 0.0f, 
			in_char->entityType, in_char->idxOnServer, defender->entityType, defender->idxOnServer, 0, false, in_char ->idxOnServer );
		break;
		
	case FX_CCharManager::EFFECT_BASIC_ATTACK_EFFECT:
		{
			if( defender == NULL ) return;
						
			VectorCopy( origin, in_char->position ); 
			VectorCopy( target, defender->position );

			target[2] -= defender->stat_height;

			g_effectMng ->Add( (GTH_EFFECT_CLASS_TYPE)GTH_EFFECT_CLASS_MULTI_MODEL, in_effectID, origin, target, NULL, 0.0f, 
				in_char->entityType, in_char->idxOnServer, defender->entityType, defender->idxOnServer, 0, false );

		} break;
	
	}
	return;
}

 
 
 
 
void	FX_CCharManager::SetTrembling	(	const vec3_t	in_position,	
											const float		in_preperation,
											const float		in_duration,
											const float		in_power	)
{
	if	(	! m_tbUse	)		return;

	const float	POWER_CONSTANT	=	0.00001f;

	m_tbPrepare		=	true;
	m_tbDuration	=	in_duration;
	 
	vec3_t			distance;
	VectorSubtract ( distance,	m_Characters [0].position,	in_position	);
	float	sqr_length	=	VectorDotProduct (	distance,	distance	) * POWER_CONSTANT;
	m_tbPower		=	in_power / (in_power + sqr_length);
	
	m_tbPreperationDuration	=	in_preperation;
	m_tbStartTime	=	g_timer .GetAppTime ();

	return;
}

 
 
 
 
void	FX_CCharManager::StopTrembling	()
{
	if	(	! m_tbDo	||	! m_tbUse	)	return;

	m_tbDo		=	false;
	VectorCopy	(	g_camera .m_position,	m_tbOriginalCamPos	);

	return;
}

 
 
 
 
void	FX_CCharManager::CheckTrembling	()
{
	if	(	! m_tbUse	)	return;

	if	(	m_tbPrepare	)
	{
		 
		if	(	( g_timer .GetAppTime () - m_tbStartTime )	>	m_tbPreperationDuration	)
		{
			m_tbPrepare		=	false;
			m_tbDo			=	true;
			VectorCopy	(	m_tbOriginalCamPos,	g_camera .m_position	);
			m_tbStartTime	=	g_timer .GetAppTime ();
		}
	}
	else if	(	m_tbDo	)
	{
		if	(	( g_timer .GetAppTime () - m_tbStartTime )	>	m_tbDuration	)
		{
			StopTrembling ();
		}
	}

	return;
}

 
 
 
 
int		FX_CCharManager::SetCharacterSpeed	(	Fx_CHARACTER_t	*in_char,
												float			in_percent	)
{
	


	
		
	if	(	in_percent	<=	0	)
	{
		StopAnimation ( in_char ->ID,	false	);
		return	1;
	}

	StopAnimation ( in_char ->ID,	true	);
	in_char ->ren_oldGeneralSpeed	=	in_char ->ren_generalSpeed;
	in_char ->ren_generalSpeed		=	in_percent;
	
	return 0;
}

 
 
 
 
void	FX_CCharManager::RenderShadow	()
{

	
	if	(	! m_drawShadows	)	return;

	  
	m_pd3dDevice ->SetRenderState ( D3DRS_LIGHTING,	FALSE );
	m_pd3dDevice ->SetRenderState ( D3DRS_SRCBLEND,		D3DBLEND_INVSRCCOLOR );
	m_pd3dDevice ->SetRenderState ( D3DRS_DESTBLEND,	D3DBLEND_DESTCOLOR );
	m_pd3dDevice ->SetRenderState ( D3DRS_ALPHAREF,		0x000000FE );
	m_pd3dDevice ->SetRenderState ( D3DRS_ALPHAFUNC,	D3DCMP_GREATER );
	m_pd3dDevice ->SetTextureStageState ( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
	m_pd3dDevice ->SetTextureStageState ( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );


	 
	g_pApp ->m_worldBSPs [g_pApp ->m_currWorld] ->SetDecalBuffer();

	int		index;

	Fx_CHARACTER_t	*thisChar	=	&m_Characters [0];
	for	(	index	=	0;	\
			index	<	m_iNumCharacters;	\
			++index,	++thisChar	)
	{
		if	(	! thisChar->ren_renderThisFrame	)	continue;
		if	(	thisChar ->flag_disappear	)		continue;
		
		MakeShadow(	thisChar );
	}

	ITOR_LIST_CREATURE	itor;
	for	(	itor	=	m_usedCreatureList.begin();	\
			itor	!=	m_usedCreatureList.end();	\
			++itor	)
	{
		MakeShadow( (*itor) );
	}

	m_pd3dDevice ->SetRenderState ( D3DRS_ALPHAREF,		0x00000000 );
}

 
 
 
 
void	FX_CCharManager::MakeShadow	(	Fx_CHARACTER_t	*in_char	)
{
	vec3_t		lightAngle;
	vec3_t		lightVector;
	vec3_t		firstCP;		 
	vec3_t		secondCP;		 
	vec2_t		firstPCP;		 
	vec2_t		secondPCP;		 
	vec2_t		centerPoint;	 
	float		dispPS;			 
	vec3_t		sunAngle;		 

	 
	GetBoundCharPreTransform( in_char, g_charBBox );

	const float	OVERSIZE	=	10000.0f;
	if	(	( fabs ( g_charBBox[ 5 ] - g_charBBox[ 2 ] )	>	OVERSIZE )	||
			( fabs ( g_charBBox[ 4 ] - g_charBBox[ 1 ] )	>	OVERSIZE )	||
			( fabs ( g_charBBox[ 3 ] - g_charBBox[ 0 ] )	>	OVERSIZE )	)
		return;

	VectorToAngles ( g_pApp ->m_mapInfos [0].sunAngle, sunAngle );

	 

	lightAngle[PITCH] = 500.0f;
	

	lightAngle [YAW]	=	sunAngle [YAW]	-	( in_char ->angles [YAW] + 90.0f )	-	180.0f;
	lightAngle [ROLL]	=	in_char ->angles [ROLL];

#ifdef	_DEBUG
	if	(	in_char ->ID	==	0	)
	{
		g_chatManager ->DPrint ( 500, 520, "Light Yaw : %.2f", sunAngle [YAW] );
		g_chatManager ->DPrint ( 500, 540, "Char Yaw : %.2f", in_char ->angles [YAW] );
	}
#endif

	if	(	lightAngle [YAW]	<	-360.0f	)	lightAngle [YAW]	+=	720;
	else if	(	lightAngle [YAW]	<	0	)	lightAngle [YAW]	+=	360;

	AngleToVector ( lightAngle,	lightVector );

	assert (	(	lightAngle [YAW]	>=	0	)	&&
				(	lightAngle [YAW]	<	360	)	);

	vec3_t		sizeVector;
	VectorSubtract ( sizeVector, &g_charBBox [3], &g_charBBox [0] );
	VectorScale ( sizeVector, sizeVector, 0.3f );

	VectorSubtract ( &g_charBBox [0], &g_charBBox [0], sizeVector );
	VectorAdd ( &g_charBBox [3], &g_charBBox [3], sizeVector );

	if	(	lightAngle [YAW]	<	90	)
	{
		firstCP [0]		=	g_charBBox [3];
		firstCP [1]		=	g_charBBox [4];
		secondCP [0]	=	g_charBBox [0];
		secondCP [1]	=	g_charBBox [1];
	}
	else if	(	lightAngle [YAW]	<	180	)
	{
		firstCP [0]		=	g_charBBox [0];
		firstCP [1]		=	g_charBBox [4];
		secondCP [0]	=	g_charBBox [3];
		secondCP [1]	=	g_charBBox [1];
	}
	else if (	lightAngle [YAW]	<	270	)
	{
		firstCP [0]		=	g_charBBox [0];
		firstCP [1]		=	g_charBBox [1];
		secondCP [0]	=	g_charBBox [3];
		secondCP [1]	=	g_charBBox [4];
	}
	else if	(	lightAngle [YAW]	<	360	)
	{
		firstCP [0]		=	g_charBBox [3];
		firstCP [1]		=	g_charBBox [1];
		secondCP [0]	=	g_charBBox [0];
		secondCP [1]	=	g_charBBox [4];
	}
	firstCP [2]		=	g_charBBox [2];
	secondCP [2]	=	g_charBBox [5];

		 
		 
		 
		 
		 
		 
		 
		 

	 
	float	param;
	param	=	( 2.0f - firstCP [2] ) / lightVector [2];
	firstPCP [0]	=	lightVector [0] * param + firstCP [0];
	firstPCP [1]	=	lightVector [1] * param + firstCP [1];

	 
	param	=	( 2.0f - secondCP [2] ) / lightVector [2];
	secondPCP [0]	=	lightVector [0] * param + secondCP [0];
	secondPCP [1]	=	lightVector [1] * param + secondCP [1];

	 
	centerPoint [0]	=	( firstPCP [0] + secondPCP [0] ) * 0.5f;
	centerPoint [1]	=	( firstPCP [1] + secondPCP [1] ) * 0.5f;

	 
	float	diffX,	diffY;
	diffX	=	fabs ( firstPCP [0] - secondPCP [0] );
	diffY	=	fabs ( firstPCP [1] - secondPCP [1] );

	dispPS	=	( diffX > diffY ) ? ( diffX * 0.5f ) : ( diffY * 0.5f );

	 
	D3DXMatrixLookAtLH (	(D3DXMATRIX *)&m_shadowView,
							&D3DXVECTOR3 ( centerPoint [0],	centerPoint [1],	100.0f ),
							&D3DXVECTOR3 ( centerPoint [0],	centerPoint [1],	0.0f ),
							&D3DXVECTOR3 ( 0.0f,	-1.0f,	0.0f ) );

	D3DXMatrixOrthoOffCenterLH	(	(D3DXMATRIX *)&m_shadowProj,
									-dispPS,	dispPS,
									-dispPS,	dispPS,
									0,		1000	);

	 

	float tempLightHeight = 1400.0f;
	

	float	divider	=	tempLightHeight / lightVector [2];
	VectorScale ( lightVector,	lightVector,	divider );
	VectorCopy ( m_shadowLightVector, lightVector );			 

	m_shadowPlane [3]	=	in_char ->stat_height;

	D3DXVec4Normalize ( (D3DXVECTOR4 *)&m_shadowNormalizedPlane, (D3DXVECTOR4 *)&m_shadowPlane );
	
	D3DXMatrixShadow (	(D3DXMATRIX *)&m_shadowMatrix,
						(D3DXVECTOR4 *)&m_shadowLightVector,
						(D3DXPLANE *)&m_shadowNormalizedPlane );

	m_pd3dDevice ->SetRenderTarget ( m_shadowSurface,	NULL );

	m_D3DRender ->SetTransform ( D3DTS_WORLD,		&m_shadowMatrix );
	m_D3DRender ->SetTransform ( D3DTS_VIEW,		&m_shadowView );
	m_D3DRender ->SetTransform ( D3DTS_PROJECTION,	&m_shadowProj );

	 
	
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,	D3DCULL_CW );
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,	FALSE );

	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 );
	m_pd3dDevice->SetTexture( 0, NULL );
	m_D3DRender->SetVertexShader( GTH_CHAR_FORMAT );

	 
	m_pSkinRenderer->RenderShadow( in_char,	m_pSkinManager );
	m_pItemManager->m_pItemRenderer->RenderShadow( in_char, m_pItemManager, m_pBoneManager, &m_shadowMatrix );

	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	m_pd3dDevice->SetVertexShader( D3DFVF_SHADOWTEST );
	m_pd3dDevice->SetStreamSource( 0, m_shadowVB, sizeof( SHADOWTESTFORMAT ) );
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 8 );

	m_pd3dDevice->SetRenderTarget( m_backBuffer, m_zBuffer );

	 
	float		scaler	=	dispPS / ( SHADOW_TEX_DIMENSION * 0.5f );
	vec2_t		newCenter;

	float		angle	=	deg2rad ( lightAngle [YAW] - sunAngle [YAW] + 180.0f );
	float		cosval	=	cos ( angle );
	float		sinval	=	sin ( angle );
	newCenter [0]	=	cosval * centerPoint [0]  +  sinval * centerPoint [1];
	newCenter [1]	=	-sinval * centerPoint [0]  +  cosval * centerPoint [1];

	 
	D3DXMatrixTranslation ( (D3DXMATRIX *)&m_shadowTranslation, newCenter [0], newCenter [1], -(in_char ->stat_height - 2.0f) );
	D3DXMatrixMultiply ( (D3DXMATRIX *)&m_shadowMatrix, (D3DXMATRIX *)&in_char ->stat_matTransform, (D3DXMATRIX *)&m_shadowTranslation );

	vec3_t	newOrigin;
	float	newSize;
	newOrigin [0]	=	in_char ->position [0]  +  newCenter [0];
	newOrigin [1]	=	in_char ->position [1]  +  newCenter [1];
	newOrigin [2]	=	in_char ->position [2]  -  in_char ->stat_height;
	newSize	=	dispPS * m_sqrtTwo;

	float	charHeight	=	g_charBBox [5]  -  g_charBBox [2];
	float	maxHeightLimit	=	in_char ->position [2]	+	charHeight;
	float	minHeightLimit	=	in_char ->position [2]	-	0.5f * charHeight;

	CBSP* pBsp = g_pApp ->m_worldBSPs [g_pApp ->m_currWorld];
	pBsp->SetDecalInit((matrix4x4_t *)&m_shadowMatrix,
					   lightVector,
					   g_pApp ->m_mapInfos [0].sunAngle,
					   dispPS,
					   minHeightLimit,
					   maxHeightLimit);

	 
	int size = 0;
	if	(!pBsp->RenderProjectiveShadowNeo ( newSize, newOrigin, m_shadowVBNeo, size ))
	{
		m_pd3dDevice ->SetTransform ( D3DTS_VIEW,		(D3DXMATRIX *)&g_camera .m_viewMat );
		m_pd3dDevice ->SetTransform ( D3DTS_PROJECTION,	(D3DXMATRIX *)&g_camera .m_projMat );

		return;
	}
	
	m_pd3dDevice ->SetTransform ( D3DTS_WORLD,		(D3DXMATRIX *)&IDENTITY_MATRIX );
	m_pd3dDevice ->SetTransform ( D3DTS_VIEW,		(D3DXMATRIX *)&g_camera .m_viewMat );
	m_pd3dDevice ->SetTransform ( D3DTS_PROJECTION,	(D3DXMATRIX *)&g_camera .m_projMat );

	m_pd3dDevice ->SetRenderState ( D3DRS_CULLMODE,	D3DCULL_CCW );
	m_pd3dDevice ->SetRenderState ( D3DRS_ALPHABLENDENABLE,	TRUE );
	m_pd3dDevice ->SetRenderState ( D3DRS_ALPHATESTENABLE,	TRUE );
	m_pd3dDevice ->SetRenderState ( D3DRS_ZENABLE,	TRUE );
	
	m_pd3dDevice ->SetTexture (	0,	m_shadowTexture	);

	m_D3DRender ->SetStreamSource	(	0,	m_shadowVBNeo ->GetD3DVertexBuffer(),	sizeof (shadow_vertex_t)	);
	m_D3DRender ->SetVertexShader	(	GTH_SHADOW_FORMAT_NEO	);
    m_pd3dDevice ->DrawPrimitive ( D3DPT_TRIANGLELIST, m_shadowVBNeo ->GetBase (), size / 3 );

	return;
}

#endif	 

 
 
 
 
void	FX_CCharManager::ClearDamageBuffer	(	Fx_CHARACTER_t	*in_char	)
{
	 
	memset ( in_char ->atk_infoBuffer, 0, sizeof (in_char ->atk_infoBuffer) );

	 
	in_char ->atk_nextStartFrame	=	-1;
	in_char ->atk_indexStart		=	0;
	in_char ->atk_nextBeginFrame	=	-1;
	in_char ->atk_indexBegin		=	0;
	in_char ->atk_nextDamageFrame	=	-1;
	in_char ->atk_indexDamage		=	0;
	in_char ->atk_nextFreeFrame		=	-1;
	in_char ->atk_indexFree			=	0;
	in_char ->atk_changeEvent		=	false;
	in_char ->atk_resend			=	false;
}


























 
 
 
 
int		FX_CCharManager::StartNewComboAttack	(	Fx_CHARACTER_t	*in_char	)
{
	 

	 
	ClearDamageBuffer ( in_char );

	 
	SetNextBeginFrame ( in_char );

	 
	SetNextDamageFrame ( in_char );

	 
	SetNextFreeFrame ( in_char );

	 
	SetNextStartFrame ( in_char );
	
	return	true;
}

 
 
 
 
int		FX_CCharManager::SetNextBeginFrame	(	Fx_CHARACTER_t	*in_char	)
{
	if	(	in_char ->atk_indexBegin	>=	m_pAnimManager ->GetComboCount ( in_char ->ani_curIdx )	)
	{
		in_char ->atk_nextBeginFrame	=	-1;
		in_char ->atk_indexBegin		=	0;
	}
	else
	{
		in_char ->atk_nextBeginFrame	=	m_pAnimManager ->GetDamageFrame ( in_char ->ani_curIdx, in_char ->atk_indexBegin ) - COMBO_DAMAGE_REQUEST_FRAME_OFFSET;
		if	(	in_char ->atk_nextBeginFrame	<	0	)
			in_char ->atk_nextBeginFrame	=	0;
		++in_char ->atk_indexBegin;
	}

	return	true;
}

 
 
 
 
int		FX_CCharManager::SetNextDamageFrame	(	Fx_CHARACTER_t	*in_char	)
{
	if	(	in_char ->atk_indexDamage	>=	m_pAnimManager ->GetComboCount ( in_char ->ani_curIdx )	)
	{
		in_char ->atk_nextDamageFrame	=	-1;
		in_char ->atk_indexDamage		=	0;
	}
	else
	{
		in_char ->atk_nextDamageFrame	=	m_pAnimManager ->GetDamageFrame ( in_char ->ani_curIdx, in_char ->atk_indexDamage );
		if	(	in_char ->atk_nextDamageFrame	<	0	)
			in_char ->atk_nextDamageFrame	=	0;
		++in_char ->atk_indexDamage;
	}

	return	true;
}

 
 
 
 
int		FX_CCharManager::SetNextFreeFrame	(	Fx_CHARACTER_t	*in_char	)
{
	if	(	in_char ->atk_indexFree	>=	m_pAnimManager ->GetComboCount ( in_char ->ani_curIdx )	)
	{
		in_char ->atk_nextFreeFrame	=	-1;
		in_char ->atk_indexFree	=	0;
	}
	else
	{
		in_char ->atk_nextFreeFrame	=	m_pAnimManager ->GetFreeFrame ( in_char ->ani_curIdx, in_char ->atk_indexFree );
		if	(	in_char ->atk_nextFreeFrame	<	0	)
			in_char ->atk_nextFreeFrame	=	0;
		++in_char ->atk_indexFree;
	}

	return	true;
}

 
 
 
 
int		FX_CCharManager::GotDamageMessage	(	int		in_defenderIdx,
												int		in_defenderType,
												int		in_attackerIdx,
												int		in_attackerType,
												int		in_damage,
												int		in_stun		)
{
	Fx_CHARACTER_t	*attacker;

	if( in_damage < 0 )	{
		in_damage = 0;
		in_stun	  = 0;
	}

	attacker	=	FindCharacterByServerIndex ( in_attackerIdx, in_attackerType );
	
	if	(	attacker	==	NULL	)
		return	false;

	 
	int		thisEntryIndex	= FindDamageBufferEntry ( attacker, false );

	if	(	thisEntryIndex	<	0	)
	{
		 
		return	false;
	}

	ATTACKINFO_t&	atkInfo	= attacker ->atk_infoBuffer [thisEntryIndex];

	if	(	atkInfo.expired	)
	{
		g_ifMng ->m_numCtl ->AddDamageNumber ( in_defenderIdx, in_defenderType, in_attackerIdx, in_attackerType, in_damage, in_stun );
		g_ifMng ->m_numCtl ->ReadyDamage ( attacker );

		 
		DeleteDamageBufferEntry ( attacker, thisEntryIndex );
	}
	else
	{
		 
		atkInfo.used	=	true;
		 
		atkInfo.attackerIdx		=	(WORD)in_attackerIdx;
		atkInfo.attackerType	=	(WORD)in_attackerType;
		atkInfo.defenderIdx		=	(WORD)in_defenderIdx;
		atkInfo.defenderType	=	(WORD)in_defenderType;
		atkInfo.damage			=	(WORD)in_damage;
		atkInfo.stun			=	(WORD)in_stun;
		 
		atkInfo.identifier		=	g_pApp ->m_frameFlag;
	}

	return	true;
}

 
 
 
 
int		FX_CCharManager::FindDamageBufferEntry	(	Fx_CHARACTER_t	*in_char,
													bool			in_used		)	 
{
	int		index;
	for	(	index	=	0;	\
			index	<	MAX_COUNT_ATTACK_INFOS;	\
			++index		)
	{
		if	(	in_used	==	in_char ->atk_infoBuffer [index].used	)
			return	index;
	}

	 
	return	-1;
}

 
 
 
 
int		FX_CCharManager::DeleteDamageBufferEntry	(	Fx_CHARACTER_t	*in_char,
														int				in_index	)
{
	assert ( in_index >= 0 );
	assert ( in_index < MAX_COUNT_ATTACK_INFOS );

	if	(	in_index	<	( MAX_COUNT_ATTACK_INFOS - 1 )	)
	{
		memcpy ( &in_char ->atk_infoBuffer [in_index], &in_char ->atk_infoBuffer [in_index + 1], sizeof(ATTACKINFO_t) * ( MAX_COUNT_ATTACK_INFOS - in_index - 1 ) );
	}

	memset ( &in_char ->atk_infoBuffer [MAX_COUNT_ATTACK_INFOS - 1], 0, sizeof(ATTACKINFO_t) );

	return	true;
}

 
 
 
 
int		FX_CCharManager::ProcessDamageFrame	(	Fx_CHARACTER_t	*in_char	)
{
	 
	int		usedEntry	=	FindDamageBufferEntry ( in_char, true );

	if	(	usedEntry	<	0	)
	{	 
		 
		int	index;
		for	(	index	=	0;	\
				index	<	MAX_COUNT_ATTACK_INFOS;	\
				++index		)
		{
			if	(	! in_char ->atk_infoBuffer [index].expired	)
			{
				in_char ->atk_infoBuffer [index].expired	=	true;
				break;
			}
		}

		 
		 
		return	false;
	}
	else
	{	 
		ShowDamage ( in_char, usedEntry );
	}

	return	true;
}

 
 
 
 
int		FX_CCharManager::SetNextStartFrame	(	Fx_CHARACTER_t	*in_char	)
{
	if	(	in_char ->atk_indexStart	>=	m_pAnimManager ->GetComboCount ( in_char ->ani_curIdx )	)
	{
		in_char ->atk_nextStartFrame	=	-1;
		in_char ->atk_indexStart	=	0;
	}
	else
	{
		in_char ->atk_nextStartFrame	=	m_pAnimManager ->GetStartFrame ( in_char ->ani_curIdx, in_char ->atk_indexStart );
		if	(	in_char ->atk_nextStartFrame	<	0	)
			in_char ->atk_nextStartFrame	=	0;
		++in_char ->atk_indexStart;
	}

	return	true;
}

 
 
 
 
void	FX_CCharManager::ShowAttackEffect	(	Fx_CHARACTER_t	*in_attacker	, int	damage	)
{
	int		targetID;
	
	
	targetID	=	g_GameTarget.GTH_GetTargetClientID( in_attacker );
	if( targetID < 0 ) return;

	Fx_CHARACTER_t	*defender	=	&m_Characters [targetID];

	vec3_t	targetPos;
	vec3_t	targetAngle;
	vec3_t	targetOrigin;

	float	height;

	VectorSubtract ( targetPos, in_attacker ->position, defender ->position );
	VectorToAngles ( targetPos, targetAngle );

	VectorCopy ( targetOrigin, defender ->position );

	if	(	defender ->stat_height	>	in_attacker ->stat_height	)
	{
		height	=	in_attacker ->stat_height;
	}
	else
	{
		height	=	defender ->stat_height;
	}

	targetOrigin [2]	+=	height;
	targetOrigin [2]	+=	( rand () / (float)RAND_MAX ) * height * 0.5f;

	float	random	=	rand () / (float)RAND_MAX;

	if	(	random	<	0.3f	)
	{
		g_effectMng ->Add ( GTH_EFFECT_CLASS_MODEL,	0,	targetOrigin, NULL, targetAngle );
	}
	else if	(	random	<	0.6f	)
	{
		g_effectMng ->Add ( GTH_EFFECT_CLASS_MODEL,	1,	targetOrigin, NULL, targetAngle );
	}
	else
	{
		g_effectMng ->Add ( GTH_EFFECT_CLASS_MODEL,	2,	targetOrigin, NULL, targetAngle );
	}

	 
	if	(	( rand() / (float)RAND_MAX ) < 1.0f )
		SetCharTrembling ( targetID, 0.1f, 0.5f );

	if( damage > 0 )
		g_soundMng ->PlayDamageSound ( defender );
}

 
 
 
 
 
Fx_CHARACTER_t*	FX_CCharManager::FindCharacterByServerIndex	(	int		in_serverIdx,
																int		in_type		)
{
	for	(	int	index	=	0;	\
				index	<	MAX_CHAR_A_SCENE;	\
				++index		)
	{
		if	(	(	m_Characters [index].idxOnServer	==	in_serverIdx	)	&&
				(	m_Characters [index].entityType		==	in_type		)	)
			return	&m_Characters [index];
	}

	return	NULL;
}

 
 
 
 
int		FX_CCharManager::ShowDamage	(	Fx_CHARACTER_t	*in_char, 
										int				in_entryIdx	)
{
	 
	ATTACKINFO_t	*atkInfo;
	atkInfo	= &in_char ->atk_infoBuffer [in_entryIdx];
	g_ifMng ->m_numCtl ->AddDamageNumber ( atkInfo ->defenderIdx, atkInfo ->defenderType, atkInfo ->attackerIdx, atkInfo ->attackerType, atkInfo ->damage, atkInfo ->stun );
	g_ifMng ->m_numCtl ->ReadyDamage ( in_char );

	DWORD	identifier	=	atkInfo ->identifier;

	 
	DeleteDamageBufferEntry ( in_char, in_entryIdx );

	 
	while	(	( in_entryIdx	=	FindDamageBufferEntry ( in_char, true )	)	>=	0	)
	{
		atkInfo	=	&in_char ->atk_infoBuffer [in_entryIdx];
		if	(	atkInfo ->identifier	==	identifier	)
		{
			g_ifMng ->m_numCtl ->AddDamageNumber ( atkInfo ->defenderIdx, atkInfo ->defenderType, atkInfo ->attackerIdx, atkInfo ->attackerType, atkInfo ->damage, atkInfo ->stun );
			g_ifMng ->m_numCtl ->ReadyDamage ( in_char );

			 
			DeleteDamageBufferEntry ( in_char, in_entryIdx );
		}
		else
			break;
	}

	return	true;
}

 
 
 
 
 
float	FX_CCharManager::CalcAttackLength	(	Fx_CHARACTER_t	*in_char	)
{
	int itemTableIdx	=	in_char ->equipment [0];
	int classIdx		=	g_itemTable [itemTableIdx].classIdx;
	if	(	g_itemClassTable [classIdx].attackAtt	)
		return	g_itemTable [itemTableIdx].atkLength;
	else
		return	-1.0f;
}

 
 
 
 
void	FX_CCharManager::SetCharTrembling	(	const int		in_charIdx,
												const float		in_duration,
												const float		in_power	)
{
	Fx_CHARACTER_t	*thisChar	=	&m_Characters [in_charIdx];

	thisChar ->tb_duration	=	in_duration;

	 
	thisChar ->tb_power		=	in_power;
	thisChar ->tb_startTime	=	g_timer .GetAppTime ();
	thisChar ->tb_do		=	true;
	VectorCopy	(	thisChar ->tb_originalPos,	thisChar ->position	);

	return;
}

 
 
 
 
void	FX_CCharManager::StopCharTrembling	(	const int	in_charIdx	)
{
	Fx_CHARACTER_t	*thisChar	=	&m_Characters [in_charIdx];

	if	(	! thisChar ->tb_do	)	return;

	thisChar ->tb_do	=	false;
	VectorCopy	(	thisChar ->position,	thisChar ->tb_originalPos	);

	return;
}

 
 
 
 
void	FX_CCharManager::CheckCharTrembling	()
{
	Fx_CHARACTER_t	*thisChar	=	&m_Characters [0];

	for	(	int	index	=	0;	\
				index	<	MAX_CHAR_A_SCENE;	\
				++index,	++thisChar	)
	{
		if	(	thisChar ->tb_do	)
		{
			if	(	( g_timer .GetAppTime () - thisChar ->tb_startTime )	>	thisChar ->tb_duration	)
			{
				StopCharTrembling ( index );
			}
		}
	}

	return;
}
