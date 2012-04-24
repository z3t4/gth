//	inliners for AnimationManager
#include <ASSERT.H>

//
//	
//	ATTACKMOTIONINFO
//
//

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 ATTACKMOTIONINFO::GetStartFrame()                                                    早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline WORD		ATTACKMOTIONINFO::GetStartFrame	(	int		in_frameIdx		)
{
	assert( ( in_frameIdx >= 0 ) && ( in_frameIdx < MAX_DAMAGE_FRAMES ) );

	return	startframes[ in_frameIdx ];
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 ATTACKMOTIONINFO::GetEndFrame()                                                      早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline WORD		ATTACKMOTIONINFO::GetDamageFrame	(	int		in_frameIdx		)
{
	assert( ( in_frameIdx >= 0 ) && ( in_frameIdx < MAX_DAMAGE_FRAMES ) );

	return	damageframes[ in_frameIdx ];
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 ATTACKMOTIONINFO::GetFreeFrame()                                                     早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline WORD		ATTACKMOTIONINFO::GetFreeFrame	(	int		in_frameIdx		)
{
	assert( ( in_frameIdx >= 0 ) && ( in_frameIdx < MAX_DAMAGE_FRAMES ) );

	return	freeframes[ in_frameIdx ];
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 ATTACKMOTIONINFO::GetAttackFrame()                                                   早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline WORD		ATTACKMOTIONINFO::GetAttackFrame	(	int		in_frameIdx	)
{
	return	( GetFreeFrame( in_frameIdx ) - GetStartFrame( in_frameIdx ) );
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 ATTACKMOTIONINFO::SetStartFrame()                                                    早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline void		ATTACKMOTIONINFO::SetStartFrame	(	int		in_frameIdx,
													WORD	in_startFrame	)
{
	assert( ( in_frameIdx >= 0 ) && ( in_frameIdx < MAX_DAMAGE_FRAMES ) );
	assert( in_startFrame >= 0 );

	startframes[ in_frameIdx ]	=	in_startFrame;
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 ATTACKMOTIONINFO::SetDamageFrame()                                                   早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline void		ATTACKMOTIONINFO::SetDamageFrame	(	int		in_frameIdx,
														WORD	in_damageFrame	)
{
	assert( ( in_frameIdx >= 0 ) && ( in_frameIdx < MAX_DAMAGE_FRAMES ) );
	assert( in_damageFrame >= 0 );

	damageframes[ in_frameIdx ]	=	in_damageFrame;
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 ATTACKMOTIONINFO::SetFreeFrame()                                                     早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline void		ATTACKMOTIONINFO::SetFreeFrame	(	int		in_frameIdx,
													WORD	in_freeFrame	)
{
	assert( ( in_frameIdx >= 0 ) && ( in_frameIdx < MAX_DAMAGE_FRAMES ) );
	assert( in_freeFrame >= 0 );

	freeframes[ in_frameIdx ]	=	in_freeFrame;
}

//
//
//	CParamBlock_CalcFreeTime
//
//

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 CParamBlock_CalcFreeTime::SetAnimIndex()                                             早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline void		CParamBlock_CalcFreeTime::SetAnimIndex	(	int		in_animIndex	)
{
	assert( in_animIndex >= 0 );

	animIndex	=	in_animIndex;
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 CParamBlock_CalcFreeTime::SetDesiredTime()                                           早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline void		CParamBlock_CalcFreeTime::SetDesiredTime	(	float	in_desiredTime	)
{
	assert( in_desiredTime >= 0.0f );

	desiredTime	=	in_desiredTime;
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 CParamBlock_CalcFreeTime::SetCurrentComboCount()                                     早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline void		CParamBlock_CalcFreeTime::SetCurrentComboCount	(	float	in_currentComboCount	)
{
	assert( in_currentComboCount >= 0 );

	currentComboCount	=	in_currentComboCount;
}

//
//
//	FX_CAnimManager
//
//


/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::GetComboCount()                                                     早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline int	FX_CAnimManager::GetComboCount	(	int		in_animIndex	)
{
	return	GetAttackMotionInfo( in_animIndex ).GetComboCount();
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::GetDamageFrame()                                                    早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline int	FX_CAnimManager::GetDamageFrame	(	int		in_animIndex,
												int		in_comboIndex	)
{
	return	GetAttackMotionInfo( in_animIndex ).GetDamageFrame( in_comboIndex );
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::GetFreeFrame()                                                      早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline int	FX_CAnimManager::GetFreeFrame	(	int		in_animIndex,
												int		in_comboIndex	)
{
	return	GetAttackMotionInfo( in_animIndex ).GetFreeFrame( in_comboIndex );
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::GetAuraStartFrame()                                                 早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline int	FX_CAnimManager::GetAuraStartFrame	(	int		in_animIndex	)
{
	return	GetAttackMotionInfo( in_animIndex ).GetAuraStartFrame();
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::GetAuraEndFrame()                                                   早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline int	FX_CAnimManager::GetAuraEndFrame	(	int		in_animIndex	)
{
	return	GetAttackMotionInfo( in_animIndex ).GetAuraEndFrame();
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::GetStartFrame()                                                     早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline int	FX_CAnimManager::GetStartFrame	(	int		in_animIndex,
												int		in_comboIndex	)
{
	return	GetAttackMotionInfo( in_animIndex ).GetStartFrame( in_comboIndex );
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::GetAttackFrame()                                                    早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline int	FX_CAnimManager::GetAttackFrame	(	int		in_animIndex,
												int		in_comboIndex	)
{
	return	GetAttackMotionInfo( in_animIndex ).GetAttackFrame( in_comboIndex );
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::IsComboAttack()                                                     早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline int	FX_CAnimManager::IsComboAttack	(	int		in_animIndex	)
{
	if	(	GetAttackMotionInfo( in_animIndex ).GetComboCount()	>=	1	)
		return	true;
	else
		return	false;
}

/*旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 IsSkillAttack()                                                                      早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* CREATED(LAST MODIFIED) : 2004-05-01(2004-05-01) */ 
/* TODO : */ 
inline int	FX_CAnimManager::IsSkillAttack	(	int		in_animIndex	)
{
	if	(	GetAttackMotionInfo( in_animIndex ).IsSkillAttack()	)
		return	true;
	return	false;
}

/*旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::ShowAttackEffect()                                                  早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* CREATED(LAST MODIFIED) : 2004-05-14(2004-05-14) */ 
/* TODO : */ 
inline int	FX_CAnimManager::ShowAttackEffect	(	int		in_animIndex	)
{
	/* TUNA>> 2004-07-20 : 蹺陛 attack motion info 陛 橈朝 non-attack animation 曖 唳辦 default 高檣 1 擊 奩�納炵絲� 蹺陛 */ 
	if	(	! GetAttackMotionInfo( in_animIndex ).IsComboAttack()	)
		return	true;
	
	if	(	GetAttackMotionInfo( in_animIndex ).ShowAttackEffect()	)
		return	true;
	return	false;
}

/*旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::ShowAttackAura()                                                    早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* CREATED(LAST MODIFIED) : 2004-05-14(2004-05-14) */ 
/* TODO : */ 
inline int	FX_CAnimManager::ShowAttackAura	(	int		in_animIndex	)
{
	if	(	GetAttackMotionInfo( in_animIndex ).ShowAttackAura()	)
		return	true;
	return	false;
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::CalcAttackTime()                                                    早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline float	FX_CAnimManager::CalcAttackTimeInplace	(	CParamBlock_CalcFreeTime	&in_paramBlock	)
{
	return	GetAttackFrame( in_paramBlock.GetAnimIndex(), in_paramBlock.GetCurrentComboCount() ) * m_Anims[ in_paramBlock.GetAnimIndex() ].animation.m_timePerFrame * 1000.0f;
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::CalcAllPartialFreeFrames()                                          早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline float	FX_CAnimManager::CalcAllPartialFreeFrames	(	CParamBlock_CalcFreeTime	&in_paramBlock	)
{
	float	freeFrames;

	if	(	in_paramBlock.GetCurrentComboCount()	==	0	)
	{
		freeFrames	=	GetStartFrame( in_paramBlock.GetAnimIndex(), 0 );
		freeFrames	+=	GetStartFrame( in_paramBlock.GetAnimIndex(), 1 ) - GetFreeFrame( in_paramBlock.GetAnimIndex(), 0 );
	}
	else if	(	in_paramBlock.GetCurrentComboCount()	==	( GetComboCount( in_paramBlock.GetAnimIndex() ) - 1 )	)
	{
		freeFrames	=	m_Anims[ in_paramBlock.GetAnimIndex() ].animation.m_totalFrames - GetFreeFrame( in_paramBlock.GetAnimIndex(), in_paramBlock.GetCurrentComboCount() );
	}
	else
	{
		freeFrames	=	GetStartFrame( in_paramBlock.GetAnimIndex(), in_paramBlock.GetCurrentComboCount() + 1 ) - GetFreeFrame( in_paramBlock.GetAnimIndex(), in_paramBlock.GetCurrentComboCount() );
	}
	
	return	freeFrames;
}

/*旨收 2004-04-27 :  收旬 */ 
/*曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 */ 
/*早 FX_CAnimManager::GetAttackMotionInfo()                                               早 */ 
/*曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 */ 
/* DESCRIPTION   : */ 
/* LAST MODIFIED : 2004-04-27 */ 
/* TODO : */ 
inline ATTACKMOTIONINFO_t&	FX_CAnimManager::GetAttackMotionInfo	(	int		in_motionIndex	)
{
	assert( ( in_motionIndex >= 0 ) && ( in_motionIndex < MAX_COUNT_ANIM ) );

	return	m_attackMotionInfos[ in_motionIndex ];
}
