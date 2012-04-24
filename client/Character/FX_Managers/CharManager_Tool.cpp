 
 
 
 
 
 
 
 
 

 
 
 


#ifdef	_FX_TOOL

 
 
 
#include "../../global_def.h"
#include "../Common.h"

 
 
 
 
int	FX_CCharManager::CheckCorrectBone	(	Fx_CHARACTER_t	*pThisChar	)
{
	FX_CSkel	*pSkel;			 
	FX_CAnim	*pAnim;			 

	int		aniID;

	 
	if	(	pThisChar ->attr_skelID	<	0	)	return	false;
	pSkel	=	&m_pSkelManager ->m_Skels [pThisChar ->attr_skelID].skeleton;

	 
	aniID	=	m_pCharLookupMng ->GetAnimIDFromTable ( pThisChar ->entityType,		pThisChar ->tableIdx,
														pThisChar ->ani_curSetType,	pThisChar ->ani_curItemSetIndex,	pThisChar ->ani_curSetID, pThisChar->ID );
	
	if( aniID < 0 || aniID >= MAX_COUNT_ANIM )
		return false;

	pAnim	=	&m_pAnimManager ->m_Anims [aniID].animation;

	if	(	! (	pSkel ->m_pBoneStID.sub [0]	==	pAnim ->m_pBoneStID.sub [0]		&&
				pSkel ->m_pBoneStID.sub [1]	==	pAnim ->m_pBoneStID.sub [1]		&&
				pSkel ->m_pBoneStID.sub [2]	==	pAnim ->m_pBoneStID.sub [2]		&&
				pSkel ->m_pBoneStID.sub [3]	==	pAnim ->m_pBoneStID.sub [3]	)	)
		return	false;

	return	true;
}

 
 
 
 
void	FX_CCharManager::GenerateAniToolsCharacter	(	int		entityType, 
														int		skinIdx, 
														int		aniIdx		)
{
	Fx_CHARACTER_t	*pNewChar;
	int				i;

	pNewChar	=	&m_Characters [0];
	m_iNumCharacters	=	1;

	pNewChar ->position [0]	=	0.0f;
	pNewChar ->position [1] =	0.0f;
	pNewChar ->position [2]	=	0.0f;
	pNewChar ->ID			=	0;
	pNewChar ->flag_interpolation	=	false;

	pNewChar ->useInterpolation	=	false;
	pNewChar ->ani_elapsed		=	0.0f;
	pNewChar ->ani_oldFrame			=	0.0f;
	pNewChar ->ani_curIdx		=	m_pCharLookupMng ->GetIdleAnimation (	0,	0,	0,	0	);
	pNewChar ->ani_nextIdx		=	-1;
	pNewChar ->ani_curState.type.changable	=	true;
	pNewChar ->ani_curState.type.repeatable	=	true;
	pNewChar ->ani_curState.type.movable	=	true;
	pNewChar ->ani_curState.type.alive		=	true;
	pNewChar ->ani_curGroup		=	FX_ANIM_GROUP_MOVE;
	pNewChar ->ani_nextGroup	=	FX_ANIM_GROUP_NULL;
	pNewChar ->oldEvent			=	GTH_EV_CHAR_IDLE;
	pNewChar ->attr_skelID		=	-1;
	pNewChar ->stat_scale		=	1.0f;		 
	
	if	(	entityType	==	ENTITY_PC	)
	{
		pNewChar ->entityType	=	ENTITY_PC;
		pNewChar ->tableIdx		=	aniIdx;
		
		pNewChar ->skinPart [0]	=	0;
		pNewChar ->skinPart [1]	=	0;
		pNewChar ->skinPart [2]	=	0;
		pNewChar ->skinPart [3]	=	0;

		SetDefaultCharacter ( 0 );

		InitNewChar ( pNewChar );

		pNewChar ->ani_curSpeed		=	1.0f;
		pNewChar ->ani_curRSpeed	=	1.0f / pNewChar ->ani_curSpeed;
		pNewChar ->ani_option		=	ANIMOPTION_START_NEW_ANIM;

		CheckAuraExist ( 0 );

		pNewChar ->attr_skelID	=	m_pCharLookupMng ->GetBoneIDFromTable ( pNewChar ->entityType,	pNewChar ->tableIdx );	 
	}
	else if	(	entityType	==	ENTITY_MONSTER	)
	{
		pNewChar ->entityType	=	ENTITY_MONSTER;
		pNewChar ->tableIdx		=	aniIdx;

		for	(	i	=	0;	i	<	MAX_COUNT_ITEM_PART;	i++	)		pNewChar ->stat_itemID [i]	=	-1;
		for	(	i	=	0;	i	<	MAX_COUNT_SKIN_PART;	i++	)		pNewChar ->stat_skinID [i]	=	-1;
		pNewChar ->stat_skinID [SKIN_PART_MONSTER]	=	GetSkinID ( SKIN_PART_MONSTER,	skinIdx );
		pNewChar ->attr_skelID	=	m_pCharLookupMng ->GetBoneIDFromTable ( pNewChar ->entityType,	aniIdx ); 

		InitNewChar ( pNewChar );

		pNewChar ->ani_curSpeed		=	1.0f;
		pNewChar ->ani_curRSpeed	=	1.0f / pNewChar ->ani_curSpeed;
		pNewChar ->ani_option		=	ANIMOPTION_START_NEW_ANIM;

		CheckAuraExist ( 0 );
	}
	
	pNewChar ->ani_curSetType		=	ANIMTYPE_BYCOMMON;
	pNewChar ->ani_curItemSetIndex	=	0;
	pNewChar ->ani_curSetID			=	0;
	
	pNewChar ->SetupVectors ();
	UpdatePosition ( pNewChar ->ID,	pNewChar ->position, pNewChar ->angles );
	UpdateSysVBInfo ();
	UpdateCharacterState ();
}

#endif	 