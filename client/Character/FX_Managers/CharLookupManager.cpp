 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"

#define	CHAR_LOOKUP_VERSION_TEXT	"101"
#define	CHAR_LOOKUP_VERSION_VALUE	101


 
 
 
void	FX_CCharLookupMng::LoadTableInfo	()
{
	BYTE	*pFileBuffer;
	int		offset	=	0;
	pFileBuffer		=	m_fileMng ->ReadFileFromPack ( FILENAME_CHARTABLE );

	for	(	int	index	=	0;	\
				index	<	NUM_GEN_LOOKUPS;	\
				++index		)
	{
		GetDataFromBuffer ( (void *)&m_charLookups [index], pFileBuffer, sizeof (Fx_CHAR_LOOKUP_t), offset );
	}

	GTH_SAFE_FREE ( pFileBuffer );

	return;
}

 
 
 
 
void	FX_CCharLookupMng::InitTableInfo	()
{
	int	i,	j,	k;
	for	(	i	=	0;	\
			i	<	NUM_GEN_LOOKUPS;	\
			++i		)
	{
		for	(	j	=	0;	\
				j	<	MAX_ANIMCOUNT_ITEM;	\
				++j		)
		{
			for	(	k	=	0;	\
					k	<	MAX_ANIMCOUNT_TYPE;	\
					++k		)
			{
				m_charLookups [i].animset.anim_item [j].anim_type [k]	=	-1;
#ifdef	_GTH_NEW_VERSION
				m_creatureLookups [i].animset.anim_item [j].anim_type [k]	=	-1;
#endif
			}
			m_charLookups [i].animset.anim_item [j].desc [0]	=	NULL;
#ifdef	_GTH_NEW_VERSION
			m_creatureLookups [i].animset.anim_item [j].desc [0]	=	NULL;
#endif
		}
		for	(	j	=	0;	\
				j	<	MAX_ANIMCOUNT_COMMON;	\
				++j	)
		{
			m_charLookups [i].animset.anim_common [j]	=	-1;
#ifdef	_GTH_NEW_VERSION
			m_creatureLookups [i].animset.anim_common [j]	=	-1;
#endif
		}
		m_charLookups [i].animset.commonDesc [0]	=	NULL;
#ifdef	_GTH_NEW_VERSION
		m_creatureLookups [i].animset.commonDesc [0]	=	NULL;
#endif
		for	(	j	=	0;	\
				j	<	MAX_ANIMCOUNT_SKILL;	\
				++j		)
		{
			m_charLookups [i].animset.anim_skill [j]	=	-1;
#ifdef	_GTH_NEW_VERSION
			m_creatureLookups [i].animset.anim_skill [j]	=	-1;
#endif
		}
		m_charLookups [i].animset.skillDesc [0]	=	NULL;
#ifdef	_GTH_NEW_VERSION
		m_creatureLookups [i].animset.skillDesc [0]	=	NULL;
#endif
		m_charLookups [i].boneIdx	=	-1;
	}

	for	(	i	=	0;	\
			i	<	NUM_MON_LOOKUPS;	\
			++i		)
	{
		for	(	j	=	0;	\
				j	<	MAX_ANIMCOUNT_TYPE;	\
				++j		)
		{
			m_monLookups [i].anim_monster [j]	=	-1;
		}
		m_monLookups [i].desc [0]	=	NULL;
		m_monLookups [i].boneIdx	=	-1;
	}

	for	(	i	=	0;	\
			i	<	MAX_COUNT_ANIM;	\
			++i		)
	{
		memset ( &m_aniInfo [i], 0, sizeof (Fx_ANIM_INFO_t) );
	}
}

 
 
 
 
void	FX_CCharLookupMng::SaveTableInfoFromFile	(	char	*filename	)
{
	int	i;

	FILE	*fp;
	fp	=	fopen ( filename, "wb" );

#ifdef	_GTH_NEW_VERSION
	char	version [MAX_PATH];
	sprintf ( version, "VERSION%3d", CHAR_LOOKUP_VERSION_VALUE );
	fwrite ( version, strlen(version), 1, fp );
#endif

	for( i = 0; i < NUM_GEN_LOOKUPS; i ++ )
	{
		fwrite( & m_charLookups[i], sizeof( Fx_CHAR_LOOKUP_t ), 1, fp );
	}

	if	(	CHAR_LOOKUP_VERSION_VALUE	>	100	)
	{
		for( i = 0; i < NUM_GEN_LOOKUPS; i ++ )
		{
			fwrite( & m_creatureLookups[i], sizeof( Fx_CHAR_LOOKUP_t ), 1, fp );
		}
	}

	for( i = 0; i < NUM_MON_LOOKUPS; i ++ )
	{
		fwrite( &m_monLookups[i], sizeof( Fx_MON_LOOKUP_t ), 1, fp );
	}
	for( i = 0; i < MAX_COUNT_ANIM; i ++ )
	{
		fwrite( &m_aniInfo[i], sizeof( Fx_ANIM_INFO_t), 1, fp );
	}
	fclose( fp );
}

 
 
 
 
void FX_CCharLookupMng::LoadTableInfoFromFile( char* filename )
{
	int i;

	FILE* fp;
	fp = fopen( filename, "rb" );

#ifdef	_GTH_NEW_VERSION
	char	version [MAX_PATH];
	fread ( version, strlen("VERSION"), 1, fp );
	version [strlen("VERSION")]	=	NULL;
	if	(	strcmp ( version, "VERSION" )	)
	{
		m_fileVersion	=	100;
		fseek ( fp, 0L, SEEK_SET );
	}
	else
	{
		fread ( &m_fileVersion, sizeof(int), 1, fp );
	}
#endif

	for( i = 0; i < NUM_GEN_LOOKUPS; i ++ )
	{
		fread( & m_charLookups[i], sizeof( Fx_CHAR_LOOKUP_t ), 1, fp );
	}

	if	(	m_fileVersion	>	100	)
	{
		for( i = 0; i < NUM_GEN_LOOKUPS; i ++ )
		{
			fread( & m_creatureLookups[i], sizeof( Fx_CHAR_LOOKUP_t ), 1, fp );
		}
	}

	for( i = 0; i < NUM_MON_LOOKUPS; i ++ )
	{
		fread( &m_monLookups[i], sizeof( Fx_MON_LOOKUP_t ), 1, fp );
	}
	for( i = 0; i < MAX_COUNT_ANIM; i ++ )
	{
		fread( &m_aniInfo[i], sizeof( Fx_ANIM_INFO_t), 1, fp );
	}

	fclose( fp );
}

 
 
 
 
void	FX_CCharLookupMng::LoadTableInfoFromBuffer	(	char	*filename	)
{
	int i;

	BYTE	*pFileBuffer;
	int		offset	=	0;
	pFileBuffer	=	m_fileMng->ReadFileFromPack	( filename );

#ifdef	_GTH_NEW_VERSION
	char	version [MAX_PATH];
	GetDataFromBuffer ( (void *)version, pFileBuffer, strlen("VERSION"), offset );
	version [strlen("VERSION")]	=	NULL;
	if	(	strcmp ( version, "VERSION" )	)
	{
		m_fileVersion	=	100;
		offset	=	0;
	}
	else
	{
		GetDataFromBuffer ( (void *)&m_fileVersion, pFileBuffer, sizeof(int), offset );
	}
#endif

	for( i = 0; i < NUM_GEN_LOOKUPS; i ++ )
	{
		GetDataFromBuffer ( (void *)&m_charLookups [i], pFileBuffer, sizeof (Fx_CHAR_LOOKUP_t), offset );
	}

	if	(	m_fileVersion	>	100	)
	{
		for( i = 0; i < NUM_GEN_LOOKUPS; i ++ )
		{
			GetDataFromBuffer ( (void *)&m_creatureLookups [i], pFileBuffer, sizeof (Fx_CHAR_LOOKUP_t), offset );
		}
	}

	for( i = 0; i < NUM_MON_LOOKUPS; i ++ )
	{
		GetDataFromBuffer ( (void *)&m_monLookups [i], pFileBuffer, sizeof (Fx_MON_LOOKUP_t), offset );
	}
	for( i = 0; i < MAX_COUNT_ANIM; i ++ )
	{
		GetDataFromBuffer ( (void *)&m_aniInfo [i], pFileBuffer, sizeof (Fx_ANIM_INFO_t), offset );
	}

	GTH_SAFE_FREE ( pFileBuffer );

	return;
}

 
 
 
 
int		FX_CCharLookupMng::GetItemAnimIDFromTable	(	const int	in_entityIdx,
														const int	in_itemIdx,
														const int	in_animIdx		)
{
	return	m_charLookups [in_entityIdx].animset.anim_item [in_itemIdx].anim_type [in_animIdx];
}

 
 
 
 
int		FX_CCharLookupMng::GetCommonAnimIDFromTable	(	const int	in_entityIdx,
														const int	in_animIdx		)
{
	return	m_charLookups [in_entityIdx].animset.anim_common [in_animIdx];
}

 
 
 
 
int		FX_CCharLookupMng::GetSkillAnimIDFromTable	(	const int	in_entityIdx,
														const int	in_animIdx		)
{
	return	m_charLookups [in_entityIdx].animset.anim_skill [in_animIdx];
}

 
 
 
 
int		FX_CCharLookupMng::GetAnimIDFromTable	(	const int	in_entityType,	
													const int	in_entityIdx,	
													const int	in_animType,
													const int	in_itemIdx,	
													const int	in_animIdx, 
													const int	in_charID,
													const int	in_isCreature		)
{
	Fx_CHAR_LOOKUP_t	*lookup;
	Fx_CHARACTER_t		*pThisChar	=	&g_charManager ->m_Characters [in_charID];

	if	(	in_entityType	==	ENTITY_PC	)
	{
#ifdef	_FX_CLIENT
		if	(	! pThisChar ->isTransform	)
		{
#endif
			if	(	in_entityIdx	<	0					||	
					in_animType		<	ANIMTYPE_BYITEM		||
					in_animType		>	ANIMTYPE_BYSKILL	||
					in_itemIdx		<	0					||
					in_animIdx		<	0	)
			{
				return	-1;
			}
#ifdef	_FX_CLIENT
		}
#endif

	}
	else if	(	in_entityType	==	ENTITY_MONSTER	||
				in_entityType	==	ENTITY_NPC	)
	{
		if	(	in_entityIdx	<	0	||
				in_itemIdx		<	0	|| 
				in_animIdx		<	0	)
		{
			return	-1;
		}
	}

#ifdef	_FX_CLIENT
	int	monAniSetIdx;
#endif

	switch	(	in_entityType	)
	{
	case	ENTITY_PC:
		{
#ifdef	_FX_CLIENT
			if	(	! pThisChar ->isTransform	)
			{
#endif
				if	(	! in_isCreature	)
				{
					lookup	=	&m_charLookups [in_entityIdx];
				}
				else
				{
					lookup	=	&m_creatureLookups [in_entityIdx];
				}

				switch	(	in_animType	)
				{
				case	ANIMTYPE_BYITEM:
					return	lookup ->animset.anim_item [in_itemIdx].anim_type [in_animIdx];
				case	ANIMTYPE_BYCOMMON:
					return	lookup ->animset.anim_common [in_animIdx];
				case	ANIMTYPE_BYSKILL:
					return	lookup ->animset.anim_skill [in_animIdx];
				}
#ifdef	_FX_CLIENT
			}		
			else
			{
				monAniSetIdx	=	g_monsterTable [pThisChar ->transformEntityIdx].aniSetType;

				
				if( monAniSetIdx < 0 || monAniSetIdx >= NUM_MON_LOOKUPS )
					return -1;

				 
				if	(	m_monLookups [monAniSetIdx].anim_monster [in_animIdx]	<	0	)
				{
					return	m_monLookups [monAniSetIdx].anim_monster [ANIM_ITEM_IDLE];
				}
				else
				{
					return	m_monLookups [monAniSetIdx].anim_monster [in_animIdx];
				}
			}
#endif
		}
#ifdef	_FX_CLIENT
	case	ENTITY_MONSTER:
	case	ENTITY_NPC:
		monAniSetIdx	=	g_monsterTable [in_entityIdx].aniSetType;

		
		if( monAniSetIdx < 0 || monAniSetIdx >= NUM_MON_LOOKUPS )
			return -1;

		return	m_monLookups [monAniSetIdx].anim_monster [in_animIdx];
#else
	case	ENTITY_MONSTER:
		
		if( in_entityIdx < 0 || in_entityIdx >= NUM_MON_LOOKUPS )
			return -1;

		return	m_monLookups [in_entityIdx].anim_monster [in_animIdx];
#endif
	}

	return	-1;		 
}

 
 
 
 
int		FX_CCharLookupMng::GetAnimTypeFromID	(	const int	in_entityType,
													const int	in_entityIdx,
													const int	in_animType,
													const int	in_itemIdx,
													const int	in_globalAnimIdx	)
{
	Fx_CHAR_LOOKUP_t	*lookup	=	NULL;
	int		*searcher	=	NULL;
	int		counter		=	0;
#ifdef	_FX_CLIENT
	int		monAniSetIdx;
#endif

	switch ( in_entityType )
	{
	case	ENTITY_PC:
		lookup	=	&m_charLookups [in_entityIdx];
		
		switch ( in_animType )
		{
		case	ANIMTYPE_BYITEM:
			searcher	=	&lookup ->animset.anim_item [in_itemIdx].anim_type [0];
			counter		=	MAX_ANIMCOUNT_TYPE;
		case	ANIMTYPE_BYCOMMON:
			searcher	=	&lookup ->animset.anim_common [0];
			counter		=	MAX_ANIMCOUNT_COMMON;
		case	ANIMTYPE_BYSKILL:
			searcher	=	&lookup ->animset.anim_skill [0];
			counter		=	MAX_ANIMCOUNT_SKILL;
		}
#ifdef	_FX_CLIENT
	case	ENTITY_MONSTER:
	case	ENTITY_NPC:
		monAniSetIdx	=	g_monsterTable [in_entityIdx].aniSetType;

		
		if( monAniSetIdx < 0 || monAniSetIdx >= NUM_MON_LOOKUPS )
			return -1;

		searcher	=	&m_monLookups [monAniSetIdx].anim_monster [0];
		counter		=	MAX_ANIMCOUNT_TYPE;
#else
	case	ENTITY_MONSTER:

		
		if( in_entityIdx < 0 || in_entityIdx >= NUM_MON_LOOKUPS )
			return -1;

		searcher	=	&m_monLookups [in_entityIdx].anim_monster [0];
		counter		=	MAX_ANIMCOUNT_TYPE;
#endif
	}

	for	(	int	index	=	0;	\
				index	<	counter;	\
				++index,	++searcher	)
	{
		if	(	in_globalAnimIdx	==	(*searcher)	)
			return	index;
	}

	return	-1;		 
}

 
 
 
 
int		FX_CCharLookupMng::GetBoneIDFromTable	(	const int	in_entityType,
													const int	in_entityIdx	)
{
	
	if( in_entityIdx < 0 )
		return -1;

#ifdef	_FX_CLIENT
	int		monAniSetIdx;
#endif

	switch ( in_entityType )
	{
	case	ENTITY_PC:

		
		if( in_entityIdx >= NUM_GEN_LOOKUPS )
			return -1;

		return	m_charLookups [in_entityIdx].boneIdx;
#ifdef	_FX_CLIENT
	case	ENTITY_MONSTER:
	case	ENTITY_NPC:

		
		if( in_entityIdx >= MAX_NUMBER_OF_MONSTER_TABLE )
			return -1;

		monAniSetIdx	=	g_monsterTable[in_entityIdx].aniSetType;

		
		if( monAniSetIdx >= NUM_MON_LOOKUPS )
			return -1;

		return	m_monLookups [monAniSetIdx].boneIdx;
#else
	case	ENTITY_MONSTER:

		
		if( in_entityIdx >= NUM_MON_LOOKUPS )
			return -1;

		return	m_monLookups [in_entityIdx].boneIdx;
#endif	 
	}
	return	-1;
}

 
 
 
 
int		FX_CCharLookupMng::GetMonAnimIDFromTable	(	const int	in_entityIdx,
														const int	in_animIdx		)
{
#ifdef	_FX_CLIENT
	int		monAniSetIdx;
	monAniSetIdx	=	g_monsterTable [in_entityIdx].aniSetType;
	return	m_monLookups [monAniSetIdx].anim_monster [in_animIdx];
#else
	return	m_monLookups [in_entityIdx].anim_monster [in_animIdx];
#endif	 
}


 
 
 
 
int		FX_CCharLookupMng::GetIdleAnimation	(	const int	in_entityType,	
												const int	in_entityIdx,	
												const int	in_animType,	
												const int	in_itemIdx	)
{
	return	0;
}


#ifdef	_GTH_NEW_VERSION

 
 
 
 
 
int		FX_CCharLookupMng::GetCreatureItemAnimIDFromTable	(	const int	in_entityIdx,
																const int	in_itemIdx,
																const int	in_animIdx		)
{
	return	m_creatureLookups [in_entityIdx].animset.anim_item [in_itemIdx].anim_type [in_animIdx];
}

 
 
 
 
int		FX_CCharLookupMng::GetCreatureCommonAnimIDFromTable	(	const int	in_entityIdx,
																const int	in_animIdx		)
{
	return	m_creatureLookups [in_entityIdx].animset.anim_common [in_animIdx];
}

 
 
 
 
int		FX_CCharLookupMng::GetCreatureSkillAnimIDFromTable	(	const int	in_entityIdx,
																const int	in_animIdx		)
{
	return	m_creatureLookups [in_entityIdx].animset.anim_skill [in_animIdx];
}

#endif