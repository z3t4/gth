 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"

 
 
 
FX_CSkel::FX_CSkel ()
{
	m_pBones	= NULL;

	Clear();
}


 
 
 
FX_CSkel::~FX_CSkel ()
{
}


 
 
 
VOID	FX_CSkel::Clear ()
{
	m_iNumBone		= 0;
	m_iNumRootBone	= 0;

	memset	(	m_iRootBoneID,	0,	sizeof (m_iRootBoneID)	);
	memset	(	m_Tag,	0,	sizeof (m_Tag)	);

	GTH_SAFE_DELETE	(	m_pBones	);

	return;
}



 
 
 
bool	FX_CSkel::LoadData	(	char		*strFilename,
								CFileMng	*fileMng	)
{
	BYTE	*pFileBuffer;
	int		offset	=	0;
	pFileBuffer	=	fileMng ->ReadFileFromPack	(	strFilename	);
	
	int		allocatedMemory	=	0;
	#define	__CALC_ALLOCATED_MEMORY(a)	if( g_resCounter->Enabled() ) { allocatedMemory += (a); }
	

	char	version [MAX_PATH];
	GetDataFromBuffer ( (void *)version, pFileBuffer, strlen("VERSION"), offset );
	version [strlen("VERSION")]	=	NULL;
	if	(	strcmp ( version, "VERSION" )	)
	{
		m_fileVersion	=	100;
		offset	=	4;
	}
	else
	{
		GetDataFromBuffer ( (void *)version, pFileBuffer, 3, offset );
		version [3]	=	NULL;
		m_fileVersion	=	atoi ( version );
		if	(	m_fileVersion	<	100	)
		{
			 

			m_fileVersion	=	100;
		}
		GetDataFromBuffer ( (void *)version, pFileBuffer, 4, offset );
		version [4]	=	NULL;
		if	(	strcmp ( version, "SKEL" )	)
		{
			 
		}
	}

	 
	int		iDescLength;
	GetDataFromBuffer	(	(void *)&iDescLength,	pFileBuffer,	sizeof(int),	offset		);
	GetDataFromBuffer	(	(void *)m_strDesc,		pFileBuffer,	iDescLength,	offset		);
	m_strDesc [iDescLength]	=	'\0';

	 
	GetDataFromBuffer	(	(void *)&m_pBoneStID,	pFileBuffer,	sizeof(FX_BONESTRUCT_ID),	offset		);

	 
	GetDataFromBuffer	(	(void *)&m_iNumBone,	pFileBuffer,	sizeof(int),	offset	);

#ifdef	_FXDEBUG
	CHECKERROR	(	m_iNumBone	<=	0,	"Failed to read bone count!"	)
#endif

	 
	m_pBones	=	new	BONE [m_iNumBone];
	__CALC_ALLOCATED_MEMORY( sizeof(BONE) * m_iNumBone - sizeof(BONE*) )

		 
	for	(	int	iBoneID	=	0;	\
				iBoneID	<	m_iNumBone;	\
				++iBoneID		)
	{
		BONE*	thisBone	=	&m_pBones [iBoneID];

		int		iNameLength;
		GetDataFromBuffer	(	(void *)&iNameLength,	pFileBuffer,	sizeof(int),	offset	);

#ifdef	_FXDEBUG
	CHECKERROR	(	iNameLength	<	1,	"Failed to read name length!"	)
#endif

		 
		char*		strBuffer;
		strBuffer	=	new char [iNameLength];
		GetDataFromBuffer	(	(void *)strBuffer,	pFileBuffer,	iNameLength,	offset		);
		memcpy	(	thisBone->strBoneName,	strBuffer,	iNameLength		);
		GTH_SAFE_DELETE_ARRAY ( strBuffer );

		 
		GetDataFromBuffer	(	(void *)&thisBone ->fLength,	pFileBuffer,	sizeof(float),	offset		);

		 
		GetDataFromBuffer	(	(void *)&thisBone ->iParentID,	pFileBuffer,	sizeof(int),	offset		);
		if	(	-1	==	thisBone ->iParentID	)
			m_iRootBoneID [m_iNumRootBone++]	=	thisBone ->iParentID;

		 
		GetDataFromBuffer	(	(void *)&thisBone->iNumChildBone,	pFileBuffer,	sizeof(int),	offset		);
 
#ifdef	_FXDEBUG
	CHECKERROR	(	thisBone ->iNumChildBone	<	0,	"Failed to read child count!"	)
#endif

			 
		GetDataFromBuffer	(	(void *)&thisBone->childBoneID [0],	pFileBuffer,	sizeof(int)	* thisBone ->iNumChildBone,	offset	);
	}

	 
		 
	GetDataFromBuffer	(	(void *)&m_iTagCount,	pFileBuffer,	sizeof(int),	offset	);

		 
	GetDataFromBuffer	(	(void *)m_Tag,	pFileBuffer,	sizeof(TAG) * m_iTagCount,	offset	);

	for	(	int	index	=	0;	\
				index	<	m_iTagCount;	\
				++index		)
	{
		if	(	m_Tag [index].rotation [3]	==	0.0f	)
			m_Tag [index].rotation [3]	=	1.0f;
	}

	GTH_SAFE_FREE ( pFileBuffer );

	__CALC_ALLOCATED_MEMORY( sizeof(FX_CSkel) - sizeof(FX_CSkel*) )
	
	 
	if	(	g_resCounter->Enabled()	)
	{
		g_resCounter->Add_DataType( RESCOUNTER_SKEL, allocatedMemory );
	}

	return true;
}


 
 
 
VOID	FX_CSkel::Cleanup ()
{
	GTH_SAFE_DELETE_ARRAY (	m_pBones );

	return;
}

 
 
 
 
void	FX_CSkel::SetGroundTag (	const vec3_t	in_pos		)
{
	VectorCopy ( m_Tag [ITEM_PART_GROUND].position,	in_pos );

	return;
}