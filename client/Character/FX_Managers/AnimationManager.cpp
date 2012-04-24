 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"

#include <assert.h>

 
 
 
FX_CAnimManager::FX_CAnimManager()
{
	if	(	g_resCounter->Enabled()	)
	{
		g_resCounter->Add_DataType( RESCOUNTER_CHAR_ANIMATIONMANAGER, sizeof(FX_CAnimManager) );
	}
	
	memset( m_Anims, 0, sizeof(m_Anims) );
	memset( m_notUsedMonAnims, 0, sizeof(m_notUsedMonAnims) );
	m_numNotUsedMonAnims	=	0;
}

 
 
 
bool	FX_CAnimManager::LoadAnimList()
{
	int		result;

	result	=	m_pScript ->Load	(	FILENAME_ANIM_LIST	);
#ifdef	_FXDEBUG
	CHECKERROR (	! result,	"Failed to load animation list! Check filename!"	)
#endif

	m_pScript ->BeginParse ();

	m_iNumAnims	=	atoi ( m_pScript ->GetNextToken () );

	for	(	int	index	=	0;	\
				index	<	m_iNumAnims;	\
				++index		)
	{
		strcpy	(	m_AnimBuffer [index].name,
					m_pScript ->GetNextToken ()		);
		m_AnimBuffer [index].id	=	atoi ( m_pScript->GetNextToken() );
	}

	return	true;
}

 
 
 
 
 
 
 
void	FX_CAnimManager::LoadActualData	()
{
	m_numNotUsedMonAnims	=	GTH_GetUsedMonster_AnimationList( m_notUsedMonAnims );

	for	(	int	index	=	0;	\
				index	<	m_iNumAnims;	\
				++index		)
	{
		g_SkillFile.DebugOutString("LoadAnim %d %s \n",index , m_AnimBuffer[ index ].name);

		if(index == 1033)
		{
			int aa=0;
		}
		LoadAnim( m_AnimBuffer[ index ].name, m_AnimBuffer[ index ].id );
	}
}

 
 
 
bool	FX_CAnimManager::LoadAnim	(	char		*pszFileName,
										const UINT	&iAnimID	)
{
	int		animID	=	(int)iAnimID;
	
	ANIMATION_t		*pAnim;
	pAnim	=	&m_Anims[ iAnimID ];

	char	*newString;
	char	*found;
	newString	=	_strlwr( _strdup( pszFileName ) );
	found	=	strstr( newString, "ava" );
	if	(	! found		)
	{
		for	(	int	index	=	0;	\
					index	<	m_numNotUsedMonAnims;	\
					++index		)
		{
			if	(	animID	==	m_notUsedMonAnims[ index ]	)
				break;
		}

		if	(	index	==	m_numNotUsedMonAnims	)
		{
			if	(	pAnim->inUse	)
				pAnim->animation.Cleanup();
			pAnim->inUse	=	false;
			return	true;
		}
	}
	free( newString );

	if	(	pAnim->inUse	)
		return	true;

	pAnim->animation.LoadData( pszFileName, m_fileMng );

	pAnim->inUse	=	true;

	return	true;
}

 
 
 
void	FX_CAnimManager::Cleanup()
{
	for	(	int	index	=	0;	\
				index	<	m_iNumAnims;	\
				++index		)
	{
		if	(	m_Anims[ index ].inUse	)
			m_Anims [index]	.animation	.Cleanup();
	}

	memset( m_Anims, 0, sizeof(m_Anims) );

	return;
}


 
 
 
 
void	FX_CAnimManager::GetBoundBox (	Fx_CHARACTER_t	*in_char,
										int			in_frame,
										bboxf_t		&out_boundbox	)
{

	if	(	(	in_char ->ani_curIdx	<	0	)	||
			(	in_frame	<	0	)				||
			(	in_char ->attr_skelID	<	0	)	)		return;

	FX_CAnim	*thisAnim	=	&m_Anims [in_char ->ani_curIdx].animation;
	bboxf_t		&thisBound	=	thisAnim ->m_boundBox [in_frame];
	if(thisBound == NULL) return;

	out_boundbox [1]	=	( thisBound [0]	-	in_char ->stat_disp_x ) * in_char ->stat_scale;
	out_boundbox [0]	=	( thisBound [1]	) * in_char ->stat_scale;
	out_boundbox [2]	=	( thisBound [2] );

	out_boundbox [4]	=	( thisBound [3]	-	in_char ->stat_disp_x ) * in_char ->stat_scale;
	out_boundbox [3]	=	( thisBound [4] ) * in_char ->stat_scale;
	out_boundbox [5]	=	( thisBound [5] ) * in_char ->stat_scale;

	return;
}


 
 
 
 
bool	FX_CAnimManager::LoadAMInfos	()
{
	int		result;

	result	= m_pScript ->Load ( FILENAME_ATTACKMOTION_INFO );
#ifdef	_FXDEBUG
	CHECKERROR (	! result,	"Failed to load attack motion info file! Check filename!"	)
#endif

	m_pScript ->BeginParse ();

	 
	if	(	0	!=	stricmp ( m_pScript ->GetNextToken (), "version" )	)
	{
		ShowMessage ( "ERROR!", "File corrupted!", __FILE__, __LINE__ );
		return	false;
	}

	m_AMInfoVersion	=	atoi ( m_pScript ->GetNextToken () );

	 

	char	*token;
	int		infoCount	=	0;
	
	struct	_TEMPINFO
	{
		char	tempname [MAX_PATH];
		WORD	tempcount;
		WORD	tempstart [MAX_DAMAGE_FRAMES];
		WORD	tempframes [MAX_DAMAGE_FRAMES];
		WORD	tempfreeframes [MAX_DAMAGE_FRAMES];
		WORD	temprangestart;
		WORD	temprangeend;
		WORD	tempskill;	 
		WORD	tempshowattackeffect;
		WORD	tempshowattackaura;
	}	tempInfo;

	 
	memset ( m_attackMotionInfos, 0, sizeof(m_attackMotionInfos) );

	while	(	1	)
	{
		token	= m_pScript ->GetNextToken ( true );

		if	(	! token [0]	)
			break;

		if	(	token [0]	!=	'{'	)
			continue;

		if	(	 infoCount	>=	MAX_COUNT_ANIM	)
		{
			ShowMessage ( "ERROR!", "Attack Motion Info overflow!", __FILE__, __LINE__ );
			return	false;
		}

		memset ( &tempInfo, 0, sizeof (_TEMPINFO) );
		tempInfo.tempshowattackeffect	=	1;
		tempInfo.tempshowattackaura		=	1;

		while	(	1	)
		{
			token	=	m_pScript ->GetNextToken ( true );

			if	(	! token [0]	)
				break;

			if	(	token [0]	==	'}'	)
			    break;
			else
			{
				if	(	0	==	stricmp ( "name", token )	)
				{
					strcpy ( tempInfo.tempname, m_pScript ->GetNextToken ( true ) );
				}
				else if	(	0	==	stricmp ( "combocount", token )	)
				{
					tempInfo.tempcount	= atoi ( m_pScript ->GetNextToken ( true ) );
				}
				else if	(	0	==	stricmp ( "damageframe", token )	)
				{
					if	(	tempInfo.tempcount	==	0	)
					{
						ShowMessage ( "ERROR!", "combocount 는 반드시 damageframe 이전에 위치해야 합니다!", __FILE__, __LINE__ );
						return	false;
					}

					for	(	int	index	=	0;	\
								index	<	tempInfo.tempcount;	\
								++index	)
					{
						tempInfo.tempframes [index]	= atoi ( m_pScript ->GetNextToken ( true ) );
					}
				}
				else if (	0	==	stricmp ( "freeframe", token )	)
				{
					if	(	tempInfo.tempcount	==	0	)
					{
						ShowMessage ( "ERROR!", "combocount 는 반드시 freeframe 이전에 위치해야 합니다!", __FILE__, __LINE__ );
						return	false;
					}

					for	(	int	index	=	0;	\
								index	<	tempInfo.tempcount;	\
								++index	)
					{
						tempInfo.tempfreeframes [index]	= atoi ( m_pScript ->GetNextToken ( true ) );
					}
				}
				else if	(	0	==	stricmp ( "aurarange", token )	)
				{
					tempInfo.temprangestart	= atoi ( m_pScript ->GetNextToken ( true ) );
					tempInfo.temprangeend	= atoi ( m_pScript ->GetNextToken ( true ) );
				}
				else if	(	0	==	stricmp ( "startframe", token )	)
				{
					if	(	tempInfo.tempcount	==	0	)
					{
						ShowMessage ( "ERROR!", "combocount 는 반드시 damageframe 이전에 위치해야 합니다!", __FILE__, __LINE__ );
						return	false;
					}

					for	(	int	index	=	0;	\
								index	<	tempInfo.tempcount;	\
								++index	)
					{
						tempInfo.tempstart [index]	= atoi ( m_pScript ->GetNextToken ( true ) );
					}
				}
				else if	(	0	==	stricmp( "skill", token )	)
				{
					tempInfo.tempskill	=	atoi( m_pScript->GetNextToken( true ) );
				}
				else if	(	0	==	stricmp( "showattackeffect", token )	)
				{
					tempInfo.tempshowattackeffect	=	atoi( m_pScript->GetNextToken( true ) );
				}
				else if	(	0	==	stricmp( "showattackaura", token )	)
				{
					tempInfo.tempshowattackaura		=	atoi( m_pScript->GetNextToken( true ) );
				}
				else
				{
					 
				}
			}
		}

		if	(	tempInfo.tempname [0]	!=	NULL	)
		{
			int	animIndex	=	FindAnimIndexInAnimBuffer ( tempInfo.tempname );
			if	(	animIndex	<	0	)
			{
				ShowMessage ( "ERROR!", "AttackMotionInfo 파일의 animation 이름에 문제가 있는 것 같습니다!", __FILE__, __LINE__ );
				return	false;
			}
			
			ATTACKMOTIONINFO_t	&amInfo	=	m_attackMotionInfos [animIndex];
			amInfo.used	=	true;
			amInfo.combocount	=	tempInfo.tempcount;
			memcpy ( &amInfo.startframes, tempInfo.tempstart, sizeof(WORD) * amInfo.combocount );
			memcpy ( &amInfo.damageframes, tempInfo.tempframes, sizeof(WORD) * amInfo.combocount );
			memcpy ( &amInfo.freeframes, tempInfo.tempfreeframes, sizeof(WORD) * amInfo.combocount );
			amInfo.aurastart	=	tempInfo.temprangestart;
			amInfo.auraend		=	tempInfo.temprangeend;
			amInfo.skill		=	tempInfo.tempskill;
			amInfo.showAttackEffect	=	tempInfo.tempshowattackeffect;
			amInfo.showAttackAura	=	tempInfo.tempshowattackaura;
		}
    }

	return	true;
}


 
 
 
 
int	FX_CAnimManager::FindAnimIndexInAnimBuffer	(	char	*in_animName	)
{
	for	(	int	index	=	0;	\
				index	<	MAX_COUNT_ANIM;	\
				++index		)
	{
		if	(	0	==	stricmp ( in_animName, m_AnimBuffer [index].name )	)
			return	m_AnimBuffer [index].id;
	}

	return	-1;
}

 
 
 
 
 
float	FX_CAnimManager::CalcAttackSpeedRatio	(	int		in_animIndex,
													float	in_desiredTime	)
{
 
 
 	

	 
	assert ( IsComboAttack ( in_animIndex ) );

	ATTACKMOTIONINFO_t	&info	=	m_attackMotionInfos [in_animIndex];

	int		totalAttackFrames	=	0;

	for	(	int	index	=	0;	\
				index	<	info.combocount;	\
				++index		)
	{
		totalAttackFrames	+=	( info.freeframes [index] - info.startframes [index] );
	}

	float	totalAttackTime	=	(float)totalAttackFrames * m_Anims [in_animIndex].animation.m_timePerFrame * 1000.0f;

	 
	assert ( fabs ( m_Anims [in_animIndex].animation.m_totalTime * 1000.0f - totalAttackTime )	>	0.0001 );
	 
	assert ( ( in_desiredTime - totalAttackTime )	>	0.0f	);

	return	( ( m_Anims [in_animIndex].animation.m_totalTime * 1000.0f - totalAttackTime ) / ( in_desiredTime - totalAttackTime ) * 100.0f );
	 
	 
}

 
 
 
 
float	FX_CAnimManager::CalcFreeTimeVariable	(	int		in_animIndex,
													float	in_desiredTime	)
{
 
 
 

	 
	assert ( IsComboAttack ( in_animIndex ) );

	ATTACKMOTIONINFO_t	&info		=	m_attackMotionInfos [in_animIndex];
	FX_CAnim			&thisAnim	=	m_Anims [in_animIndex].animation;

	float	totalAttackFrames	=	0.0f;

	for	(	int	index	=	0;	\
				index	<	info.combocount;	\
				++index		)
	{
		totalAttackFrames	+=	(float)( info.freeframes [index] - info.startframes [index] );
	}

	float	totalAttackTime	=	totalAttackFrames * thisAnim.m_timePerFrame * 1000.0f;
	float	totalFreeFrame	=	(float)thisAnim.m_totalFrames - totalAttackFrames;

	return	(	( in_desiredTime - totalAttackFrames * thisAnim.m_timePerFrame * 1000.0f ) * (float)thisAnim.m_totalFrames / totalFreeFrame	);
}

 
 
 
 
void	FX_CAnimManager::CalcAttackTime	(	int		in_animIndex,
											float	in_attackTime,
											int		*in_timeList	)
{
	int	index;
	ATTACKMOTIONINFO_t	&info	=	m_attackMotionInfos [in_animIndex];
	FX_CAnim	*thisAnim		=	&m_Anims [in_animIndex].animation;
	const float	MULTIPLIER_FOR_COMBO	=	0.8f;
	const float	desiredTime				=	in_attackTime * GetComboCount ( in_animIndex ) * MULTIPLIER_FOR_COMBO;
	float	totalAttackFrames	=	0.0f;

	for	(	index	=	0;	\
			index	<	info.combocount;	\
			++index		)
	{
		totalAttackFrames	+=	(float)( info.freeframes [index] - info.startframes [index] );
	}

	const float	militimePerFrame	=	thisAnim ->m_timePerFrame * 1000.0f;

	const float	totalAttackTime	=	totalAttackFrames * militimePerFrame;
	const float	fraction	=	( desiredTime - totalAttackTime ) / ( thisAnim ->m_totalTime * 1000.0f - totalAttackTime );

	WORD	attackFrames		=	0;
	WORD	freeFrames			=	0;
	short	beginFrameBefore	=	0;
	short	beginFrameAfter		=	0;
	
	for	(	index	=	0;	\
			index	<	( info.combocount - 1 );	\
			++index		)
	{
		beginFrameBefore	=	info.damageframes [index] - COMBO_DAMAGE_REQUEST_FRAME_OFFSET;
		if	(	beginFrameBefore	<	0	)	
			beginFrameBefore	=	0;
		beginFrameAfter		=	info.damageframes [index + 1] - COMBO_DAMAGE_REQUEST_FRAME_OFFSET;
		if	(	beginFrameAfter	<	0	)
			beginFrameAfter		=	0;

		assert ( beginFrameAfter	>	info.freeframes [index]	);

		if	(	info.startframes [index]	>	beginFrameBefore	)
		{
			attackFrames	=	info.freeframes [index] - info.startframes [index];
			freeFrames		=	info.startframes [index] - beginFrameBefore;
			if	(	info.startframes [index + 1]	>	beginFrameAfter	)
			{
				freeFrames		+=	( beginFrameAfter - info.freeframes [index] );
			}
			else
			{
				attackFrames	+=	( beginFrameAfter - info.startframes [index + 1] );
				freeFrames		+=	( info.startframes [index + 1] - info.freeframes [index] );
			}
		}
		else
		{
			attackFrames	=	info.freeframes [index] - beginFrameBefore;
			if	(	info.startframes [index + 1]	>	beginFrameAfter	)
			{
				freeFrames		=	beginFrameAfter - info.freeframes [index];
			}
			else
			{
				attackFrames	+=	( beginFrameAfter - info.startframes [index + 1] );
				freeFrames	=	info.startframes [index + 1] - info.freeframes [index];
			}
		}

		 
		in_timeList [index]	=	int( ( attackFrames + freeFrames * fraction ) * militimePerFrame ) - 1.0f;
	}

	index	=	info.combocount - 1;
	beginFrameBefore	=	info.damageframes [index] - COMBO_DAMAGE_REQUEST_FRAME_OFFSET;
	if	(	beginFrameBefore	<	0	)	
			beginFrameBefore	=	0;
	if	(	info.startframes [index]	>	beginFrameBefore	)
	{
		attackFrames	=	info.freeframes [index] - info.startframes [index];
		freeFrames		=	info.startframes [index] - beginFrameBefore;
	}
	else
	{
		attackFrames	=	info.freeframes [index] - beginFrameBefore;
		freeFrames		=	0;
	}
	
	in_timeList [index]	=	int( ( attackFrames + freeFrames * fraction ) * militimePerFrame ) - 1.0f;
}

 
 
 
 
 
 
float	FX_CAnimManager::CalcFreeTime	(	CParamBlock_CalcFreeTime	&in_paramBlock	)
{
	assert( IsComboAttack( in_paramBlock.GetAnimIndex() ) );

	float	attackTime	=	CalcAttackTimeInplace( in_paramBlock );
	float	freeFrames	=	CalcAllPartialFreeFrames( in_paramBlock );

	float	retVal	=	( in_paramBlock.GetDesiredTime() - GetAttackMotionInfo( in_paramBlock.GetAnimIndex() ).GetAttackFrame( in_paramBlock.GetCurrentComboCount() ) * m_Anims[ in_paramBlock.GetAnimIndex() ].animation.m_timePerFrame * 1000.0f );
	retVal	*=	(float)m_Anims[ in_paramBlock.GetAnimIndex() ].animation.m_totalFrames / freeFrames;

	return	retVal;
}
