 
 
 
 
 
 
 
 
 
#ifndef _ANIMATION_MANAGER_H_FJ109R081HE0812
#define _ANIMATION_MANAGER_H_FJ109R081HE0812

 
 
 
	 
typedef struct
{
	FX_CAnim		animation;
	int				inUse;
} ANIMATION_t;

 
#define	MAX_DAMAGE_FRAMES	10

#pragma warning( disable : 4800 )

typedef struct	ATTACKMOTIONINFO
{
	WORD	used;			 
	WORD	combocount;		 
	WORD	startframes [MAX_DAMAGE_FRAMES];
	WORD	damageframes [MAX_DAMAGE_FRAMES];
	WORD	freeframes [MAX_DAMAGE_FRAMES];
	WORD	aurastart;
	WORD	auraend;
	WORD	skill;
	WORD	showAttackEffect;
	WORD	showAttackAura;
	
	WORD	reserved [11];

	inline bool	IsComboAttack		()	{	return	(bool)used;	}
	inline WORD	GetComboCount		()	{	return	combocount;	}
	inline WORD	GetAuraStartFrame	()	{	return	aurastart;	}
	inline WORD	GetAuraEndFrame		()	{	return	auraend;	}
	inline WORD	GetStartFrame	(	int		in_frameIdx	);
	inline WORD	GetDamageFrame	(	int		in_frameIdx	);
	inline WORD	GetFreeFrame	(	int		in_frameIdx	);
	inline WORD	GetAttackFrame	(	int		in_frameIdx	);
	inline bool IsSkillAttack		()	{	return	(bool)skill;	}
	inline bool	ShowAttackEffect	()	{	return	(bool)showAttackEffect;	}
	inline bool	ShowAttackAura		()	{	return	(bool)showAttackAura;	}

	inline void	SetIsComboAttack	(	bool	in_isComboAttack	)	{	used		=	in_isComboAttack;	}
	inline void SetComboCount		(	WORD	in_comboCount		)	{	combocount	=	in_comboCount;		}
	inline void	SetAuraStartFrame	(	WORD	in_auraStartFrame	)	{	aurastart	=	in_auraStartFrame;	}
	inline void	SetAuraEndFrame		(	WORD	in_auraEndFrame		)	{	auraend		=	in_auraEndFrame;	}
	inline void	SetStartFrame	(	int		in_frameIdx,	WORD	in_startFrame	);
	inline void	SetDamageFrame	(	int		in_frameIdx,	WORD	in_damageFrame	);
	inline void	SetFreeFrame	(	int		in_frameIdx,	WORD	in_freeFrame	);
	inline void	SetIsSkillAttack	(	bool	in_isSkillAttack	)	{	skill	=	in_isSkillAttack;	}
	inline void	SetShowAttackEffect	(	bool	in_showAttackEffect	)	{	showAttackEffect	=	in_showAttackEffect;	}
	inline void	SetShowAttackAura	(	bool	in_showAttackAura	)	{	showAttackAura		=	in_showAttackAura;	}
}	ATTACKMOTIONINFO_t;

class	CParamBlock_CalcFreeTime
{
private:
	int		animIndex;
	float	desiredTime;
	int		currentComboCount;

public:
	inline void		SetAnimIndex	(	int		in_animIndex	);
	inline void		SetDesiredTime	(	float	in_desiredTime	);
	inline void		SetCurrentComboCount	(	float	in_currentComboCount	);
	
	inline int		GetAnimIndex	()		{	return	animIndex;	}
	inline float	GetDesiredTime	()		{	return	desiredTime;	}
	inline int		GetCurrentComboCount()	{	return	currentComboCount;	}
};

 
 
 
class FX_CAnimManager
{
	 
public:
	FX_CAnimManager();
	~FX_CAnimManager()	{};

	 
public:
		 
	ANIMATION_t		m_Anims		[MAX_COUNT_ANIM];		 
	int				m_iNumAnims;						 
	KEYFRAME		*m_pCurKeyframe;					 
	Fx_ANIM_BUF_t	m_AnimBuffer [MAX_COUNT_ANIM];

		 
	CScript				*m_pScript;
	CFileMng			*m_fileMng;
	
	int			m_notUsedMonAnims[ MAX_COUNT_ANIM ];
	int			m_numNotUsedMonAnims;

	 
public:
	bool		LoadAnimList();
	void		Cleanup();

		 
	void		SetScript		(	CScript		*script		)		{	m_pScript	=	script;		}
	void		SetFileManager	(	CFileMng	*fileMng	)		{	m_fileMng	=	fileMng;	}
	void		GetBoundBox (	Fx_CHARACTER_t	*in_char,	int	in_frame,	bboxf_t	&out_boundbox	);

private:
	bool		LoadAnim	(	char	*pszFileName,	const unsigned int	&iAnimID	);

protected:
	ATTACKMOTIONINFO_t	m_attackMotionInfos [MAX_COUNT_ANIM];
	int			m_AMInfoVersion;
	
public:
	bool		LoadAMInfos		();
	 
	inline int	GetComboCount	(	int		in_animIndex	);
	inline int	GetDamageFrame	(	int		in_animIndex,	int		in_comboIndex	);
	inline int	GetFreeFrame	(	int		in_animIndex,	int		in_comboIndex	);
	inline int	GetAuraStartFrame	(	int		in_animIndex	);
	inline int	GetAuraEndFrame		(	int		in_animIndex	);
	inline int	GetStartFrame	(	int		in_animIndex,	int		in_comboIndex	);
	inline int	GetAttackFrame	(	int		in_animIndex,	int		in_comboIndex	);
	inline int	IsComboAttack	(	int		in_animIndex	);
	inline int	IsSkillAttack	(	int		in_animIndex	);
	inline int	ShowAttackEffect	(	int		in_animIndex	);
	inline int	ShowAttackAura		(	int		in_animIndex	);

	float	CalcFreeTime	(	CParamBlock_CalcFreeTime	&in_paramBlock	);
	inline ATTACKMOTIONINFO_t&	GetAttackMotionInfo	(	int		in_motionIndex	);
	inline float	CalcAttackTimeInplace	(	CParamBlock_CalcFreeTime	&in_paramBlock	);
	inline float	CalcAllPartialFreeFrames	(	CParamBlock_CalcFreeTime	&in_paramBlock	);

	int			FindAnimIndexInAnimBuffer	(	char	*in_animName	);

	float	CalcAttackSpeedRatio	(	int		in_animIndex,	float	in_desiredTime	);
	float	CalcFreeTimeVariable	(	int		in_animIndex,	float	in_desiredTime	);

	void	CalcAttackTime	(	int		in_animIndex,	float	in_totalTime,	int	*in_timeList	);

	void	LoadActualData	();
};

#include "AnimationManager.inl"

#endif