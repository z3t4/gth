 
 
 
 
 
 
 
 
 

 
 
 
#include "../../global_def.h"
#include "../Common.h"
#include <cassert>
#include <algorithm>

 
#define		g_charBBox				GLOBAL_BBOX

 
#define		g_matRotation			GLOBAL_MATRIX_1
#define		g_matTranslation		GLOBAL_MATRIX_2

#define		g_buffer				GLOBAL_VEC4_5

 
 
 
 
 
 
 
void	FX_CCharManager::MakeEmptyCreatureList	()
{
	m_emptyCreatureList.resize( 0 );
	m_usedCreatureList.resize( 0 );

	for	(	int	index	=	0;	\
				index	<	MAX_CREATURE_A_SCENE;	\
				++index		)
	{
		m_emptyCreatureList.push_back( &m_creatures[index] );
	}

	m_numCreatures	=	0;
}

 
 
 
 
 
 
 
Fx_CHARACTER_t*	FX_CCharManager::GetEmptyCreature	()
{
	Fx_CHARACTER_t	*creature;

	creature	=	m_emptyCreatureList.back();
	m_emptyCreatureList.pop_back();
	m_usedCreatureList.push_back( creature );
	memset( creature, 0, sizeof( Fx_CHARACTER_t ) );

	++m_numCreatures;

	assert( m_numCreatures <= MAX_CREATURE_A_SCENE );

	return	creature;	
}

 
 
 
 
 
 
 
void	FX_CCharManager::SetEmptyCreature	(	Fx_CHARACTER_t	*in_creature	)
{
	m_emptyCreatureList.push_back( in_creature );
	m_usedCreatureList.erase( find( m_usedCreatureList.begin(), m_usedCreatureList.end(), in_creature ) );
	--m_numCreatures;

	assert( m_numCreatures >= 0 );
}

 
 
 
 
 
bool	FX_CCharManager::UpdateAnimationsCreature	()
{
	Fx_CHARACTER_t	*pThisChar	=	NULL;			 
	FX_CSkel		*pSkel		=	NULL;			 
	FX_CAnim		*pAnim		=	NULL;			 

	int				endAnim		=	0;
	int				nextIdx		=	0;		 
	Fx_ANIM_INFO_t	*nextState	=	NULL;	 

	ITOR_LIST_CREATURE	itor;
	for	(	itor	=	m_usedCreatureList.begin();	\
			itor	!=	m_usedCreatureList.end();	\
			++itor	)
	{
		pThisChar	=	(*itor);

		 
		 
		 
		 
		GetBoundChar( pThisChar, g_charBBox );
		if	(	m_frustum ->Cull_Exact( &g_charBBox[ 0 ], &g_charBBox[ 3 ] )	) 
		{
			pThisChar ->ren_auraRHandData.render	=	false;
			pThisChar ->ren_auraLHandData.render	=	false;
			pThisChar ->flag_culled			=	true;
			pThisChar ->flag_outOfMySight	=	true;
		}
		else
		{
			pThisChar ->flag_culled	=	false;
				pThisChar ->flag_outOfMySight	=	false;
		}

		if	(	pThisChar ->ani_curIdx	<	0	)
		{
			g_charManager->ChangeAnimation(pThisChar->ID, ANIMTYPE_BYITEM, 0, 0, NULL, true );
			return true;
		}

#ifdef	_FXDEBUG
	CHECKERROR (	pThisChar ->attr_skelID	<	0,	"캐릭터 본 ID 설정에 문제가 있음!"	)
	CHECKERROR (	pThisChar ->ani_curIdx	<	0,	"캐릭터의 현재 애니메이션 ID 에 문제가 있음!"	)
#endif

			 
		pSkel	=	&m_pSkelManager ->m_Skels [pThisChar ->attr_skelID].skeleton;
		pAnim	=	&m_pAnimManager ->m_Anims [pThisChar ->ani_curIdx].animation;

		 
		 
		 
		 
		if	(	pThisChar ->flag_interpolation	)
		{
			if	(	InterpolateAnim( pThisChar )	)
			{
				

				pThisChar ->ip_endAnim ->TransformKeyframes ( pThisChar	);

				continue;
			}
		}

		 
		 
		 
		 
			 
		switch (	pThisChar ->ani_option	)
		{
		case	ANIMOPTION_START_NEW_ANIM:
			pThisChar ->ani_fraction	=	0.0f;
			pThisChar ->stat_sound		=	CHAR_SOUND_ENABLE;
			if	(	pThisChar->ani_curSetID	==	ANIM_ITEM_RUN	||
					pThisChar->ani_curSetID	==	ANIM_ITEM_WALK	)
			{
				if	(	pThisChar->entityType	==	ENTITY_PC	)
				{
					g_soundMng->m_pcSound[ pThisChar->tableIdx ].playCount[ ANIM_ITEM_RUN ]		=	2;
					g_soundMng->m_pcSound[ pThisChar->tableIdx ].playCount[ ANIM_ITEM_WALK ]	=	2;
				}
				else
				{
					g_soundMng->m_monSound[ pThisChar->tableIdx ].playCount[ ANIM_ITEM_RUN ]	=	2;
					g_soundMng->m_monSound[ pThisChar->tableIdx ].playCount[ ANIM_ITEM_WALK ]	=	2;
				}
			}

			memset( pThisChar->key_curIndex, 0, sizeof( int ) * MAX_COUNT_BONE_IN_CHAR );
			memset( pThisChar->key_nextIndex, 0, sizeof( int ) * MAX_COUNT_BONE_IN_CHAR );
			memset( pThisChar->key_displace, 0, sizeof( float ) * MAX_COUNT_BONE_IN_CHAR );
			pThisChar->ani_elapsed		=	0.0f;
			pThisChar->ani_oldFrame	=	0.0f;
			pThisChar->ani_option		=	ANIMOPTION_NORMAL;
			

				if	(	pThisChar ->flag_useRHandAura	)
				{
					pThisChar ->ren_auraRHandData.render	=	false;
				}
				if	(	pThisChar ->flag_useLHandAura	)
				{
					pThisChar ->ren_auraLHandData.render		=	false;
				}
					pThisChar ->atk_changeEvent	=	true;
			break;
		default:
			break;
		}

		if	(	true	==	pThisChar->ani_curState.type.movable	)
		{
			pThisChar ->ani_elapsed	+=	m_elapsedTime;
		}

		pAnim ->m_pSkeleton			=	pSkel;

		
		if	(	pThisChar ->ani_curSpeed	<=	0	)
		{
			pThisChar ->ani_curSpeed	=	1.0f;
			pThisChar ->ani_curRSpeed	=	1.0f;
		}

#ifdef	_FX_CLIENT
		if	(	pThisChar ->ani_curGroup	==	FX_ANIM_GROUP_ATTACK	)
		{
			endAnim	=	pAnim ->CheckKeyframeState	(	pThisChar,	ANIMCHECK_NORMAL	);
		}
		else
#endif	 
			endAnim	=	pAnim ->CheckKeyframeState	(	pThisChar,	ANIMCHECK_NORMAL	);

		 
		if	(	CheckSkinUpdateTimeCreature( pThisChar )	||
				GetForceUpdateSkin()	)
		{
			pAnim ->MakeInterpolatedVertices	(	pThisChar,	endAnim	);
		}

		int		startAnim;
		switch	(	endAnim	)
		{
		case	ANIMCHECK_RESULT_NORMAL: 
			break;
		case	ANIMCHECK_RESULT_END:
				 
			pThisChar ->ani_option	=	ANIMOPTION_START_NEW_ANIM;

			startAnim	=	pThisChar ->ani_curIdx;
			if	(	false	==	pThisChar ->ani_curState.type.repeatable	)
			{
				if	(	pThisChar->ani_nextGroup	==	FX_ANIM_GROUP_NULL	)
				{
					pThisChar ->ani_curIdx		=	m_pCharLookupMng ->GetAnimIDFromTable (	pThisChar ->entityType,		pThisChar ->tableIdx,
																							pThisChar ->ani_curSetType,	pThisChar ->ani_curItemSetIndex,	pThisChar ->ani_curSetID, pThisChar->ID );
					pThisChar ->ani_curGroup	=	FX_ANIM_GROUP_MOVE;
					pThisChar ->ani_curState.type.repeatable	=	true;
					pThisChar ->ani_curState.type.changable		=	true;
				}
				else
				{
					pThisChar ->ani_curIdx			=	pThisChar ->ani_nextIdx;
					pThisChar ->ani_curGroup		=	pThisChar ->ani_nextGroup;
					pThisChar ->ani_curState.type.repeatable	=	pThisChar ->ani_nextState.type.repeatable;
					pThisChar ->ani_curState.type.changable		=	pThisChar ->ani_nextState.type.changable;

					pThisChar ->ani_curSetID			=	pThisChar ->ani_nextSetID;
					pThisChar ->ani_curItemSetIndex		=	pThisChar ->ani_nextItemSetIndex;
					pThisChar ->ani_curSetType			=	pThisChar ->ani_nextSetType;

					pThisChar ->ani_curSpeed	=	pThisChar ->ani_nextSpeed;
					pThisChar ->ani_curRSpeed	=	pThisChar ->ani_nextRSpeed;

				}
				
				pThisChar->ani_nextIdx			=	-1;
				pThisChar->ani_nextGroup		=	FX_ANIM_GROUP_NULL;

				MakeInterpolator( pThisChar, pThisChar->ani_curSpeed * INTERPOLATE_FRACTION, startAnim, pThisChar->ani_curIdx, INTERPOLATE_NORMAL );
			}
			break;
		case	ANIMCHECK_RESULT_EOP:
		case	ANIMCHECK_RESULT_EOA:
			break;
		}
	}

	return	true;
}

 
 
 
 
int		FX_CCharManager::CheckSkinUpdateTimeCreature	(	Fx_CHARACTER_t	*in_creature	)
{
	if	(	m_currentTime	>	in_creature->ren_nextTime	)
		return	true;
	else
		return	false;
}

 
 
 
 
 
void	FX_CCharManager::FixCreaturePosition	()
{
	Fx_CHARACTER_t	*thisChar;

	ITOR_LIST_CREATURE	itor;
	for	(	itor	=	m_usedCreatureList.begin();	\
			itor	!=	m_usedCreatureList.end();	\
			++itor	)
	{
		thisChar	=	(*itor);

		thisChar ->stat_fakeHeight	=	thisChar ->position [2]	+ 60.0f;		 

		D3DXMatrixRotationZ (	(D3DXMATRIX *)&g_matRotation,	deg2rad ( thisChar ->angles[ YAW ] ) + __HALFPI	);

		D3DXMatrixTranslation	(	(D3DXMATRIX *)&g_matTranslation,	thisChar ->position [0],	thisChar ->position [1],	thisChar ->stat_fakeHeight	);

		GLOBAL_SCALEMATRIX.a00	=	thisChar ->stat_scale;
		GLOBAL_SCALEMATRIX.a11	=	thisChar ->stat_scale;
		GLOBAL_SCALEMATRIX.a22	=	thisChar ->stat_scale;

		D3DXMatrixMultiply ( (D3DXMATRIX *)&g_buffer,	(D3DXMATRIX *)&GLOBAL_SCALEMATRIX,	(D3DXMATRIX *)&g_matRotation );
		D3DXMatrixMultiply ( (D3DXMATRIX *)&thisChar ->stat_matTransform,	(D3DXMATRIX *)&g_buffer,	(D3DXMATRIX *)&g_matTranslation );
	}
}

 
 
 
 
 
 
 
void	FX_CCharManager::CheckNewCreature	(	Fx_CHARACTER_t	*in_char,
												syncCharacter_t	*in_data	)
{
	 
	in_char->cr_inUse	=	false;	 

	if	(	in_char->cr_inUse	)
	{
		Fx_CHARACTER_t	*creature;
		in_char->cr_creature	=	creature	=	GetEmptyCreature();
		creature->cr_owner		=	in_char;

		creature->ID			=	in_char->ID;
		creature->isCreature	=	true;
	}
}

 
 
 
 
 
 
 
void	FX_CCharManager::CheckExistCreature	(	Fx_CHARACTER_t	*in_char,
												syncCharacter_t	*in_data	)
{
#ifndef	_GTH_ONLINE_VERSION
	return;
#endif

	if	(	false	)	 
	{
		 
		SetEmptyCreature( in_char->cr_creature );
		in_char->cr_creature	=	NULL;
		in_char->cr_inUse		=	false;
		in_char->cr_creatureIndex	=	-1;
	}

	if	(	false	)	 
	{
		 
	}
}

 
 
 
 
void	FX_CCharManager::GenerateCharacterCreature	()
{
#ifdef	_GTH_ONLINE_VERSION
	return;
#endif

	MakeEmptyCreatureList();

	Fx_CHARACTER_t	*owner	=	&m_Characters[0];
	Fx_CHARACTER_t	*creature;

	owner->cr_inUse	=	true;
	owner->cr_creature	=	creature	=	GetEmptyCreature();
	creature->cr_owner	=	owner;

	creature->ID	=	owner->ID;
	creature->isCreature	=	true;

	creature->equipNumber	=	0;
	creature->attr_skelID	=	69;
	creature->ani_curIdx	=	542;
	creature->stat_scale	=	1.0f;

	creature->ani_elapsed	=	0.0f;
	creature->ani_oldFrame	=	0.0f;
	creature->ani_nextIdx	=	-1;
	creature->ani_curState.type.changable	=	true;
	creature->ani_curState.type.repeatable	=	true;
	creature->ani_curState.type.movable	=	true;
	creature->ani_curState.type.alive		=	true;
	creature->ani_curGroup		=	FX_ANIM_GROUP_MOVE;
	creature->ani_nextGroup		=	FX_ANIM_GROUP_NULL;
	creature->oldEvent			=	GTH_EV_CHAR_IDLE;
	creature->ani_curSpeed		=	3.0f;
	creature->ani_curRSpeed		=	1.0f / creature->ani_curSpeed;
	creature->ani_option		=	ANIMOPTION_START_NEW_ANIM;

	creature->ren_generalSpeed	=	100.0f;
	SetCharacterSpeed( creature, 100.0f );

	int		index;

	for	(	index	=	0;	\
			index	<	MAX_COUNT_ITEM_PART;	\
			++index		)
	{
		creature->stat_itemID[ index ]	=	-1;
	}

	for	(	index	=	0;	\
			index	<	MAX_COUNT_SKIN_PART;	\
			++index		)
	{
		creature ->stat_skinID [index]	=	-1;
	}
	creature->stat_skinID[ SKIN_PART_HEAD ]		=	GetSkinID( SKIN_PART_HEAD, 17 );
	creature->stat_skinID[ SKIN_PART_UPPER ]	=	GetSkinID( SKIN_PART_UPPER, 28 );
	creature->stat_skinID[ SKIN_PART_LOWER ]	=	GetSkinID( SKIN_PART_LOWER, 28 );
	creature->stat_skinID[ SKIN_PART_HAND ]		=	GetSkinID( SKIN_PART_HAND, 28 );
	creature->stat_skinID[ SKIN_PART_FOOT ]		=	GetSkinID( SKIN_PART_FOOT, 28 );
	VectorCopy ( creature->position, owner->position );
}
