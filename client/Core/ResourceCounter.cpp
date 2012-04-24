#include "resourcecounter.h"
#include <string.h>
#include <STDIO.H>
#include <ASSERT.H>
#include <WINDOWS.H>

CResCounter	*g_resCounter;

char	*RES_DATATYPE_TEXT[]	=
{
	"Texture: Model - Map",
	"Texture: Model - Lightmap",
	"Texture: Model - Others",
	"Texture: Character - PC",
	"Texture: Character - NPC",
	"Texture: Character - Monster",
	"Texture: Interface",
	"Mesh: Model - Map",
	"Mesh: Model - Item",
	"Mesh: Model - Effect",
	"Mesh: Character - PC",
	"Mesh: Character - NPC",
	"Mesh: Character - Monster",
	"Anim: Character - PC",
	"Anim: Character - NPC",
	"Anim: Character - Monster",
	"Character: Character Manager",
	"Character: Animation Manager",
	"Character: Item Manager",
	"Character: Skin Manager",
	"Character: Aura Renderer",
	"Character: Item Renderer",
	"Character: Skin Renderer",
	"Skel:",
	"Etcs:",
};

char	*RES_MEMTYPE_TEXT[]	=	
{
	"Vertex Buffer - Default",
	"Vertex Buffer - Managed",
	"Index Buffer - Default",
	"Index Buffer - Managed",
	"Texture - Default",
	"Texture - Managed",
};

 
 
 
 
 
 
CResCounter::CResCounter	()
{
	Clear();

	m_enabledState	=	false;
	m_lockedState	=	false;
}

 
 
 
 
 
 
CResCounter::~CResCounter	()
{
}

 
 
 
 
 
 
void	CResCounter::Clear	()
{
	memset( m_sizeDataType, 0, sizeof(m_sizeDataType) );
	memset( m_sizeMemType, 0, sizeof(m_sizeMemType) );
	m_dataCount	=	0;
	m_memCount	=	0;
}

 
 
 
 
 
 
void	CResCounter::Reset_DataType		(	int	in_dataType	)
{
	if	(	! m_enabledState	)	return;

	m_sizeDataType[ in_dataType ]	=	0;
}

 
 
 
 
 
 
void	CResCounter::Reset_MemoryType	(	int	in_memType	)
{
	if	(	! m_enabledState	)	return;

	m_sizeMemType[ in_memType ]	=	0;
}

 
 
 
 
 
 
int		CResCounter::Add_DataType	(	int	in_dataType,
										int	in_size		)
{
	if	(	! m_enabledState	)	return	true;

	assert( in_size >= 0 );
	m_sizeDataType[ in_dataType ]	+=	in_size;
	++m_dataCount;

	return	true;
}

 
 
 
 
 
 
int		CResCounter::Add_MemoryType	(	int	in_memType,
										int	in_size		)
{
	if	(	! m_enabledState	)	return	true;

	assert( in_size >= 0 );
	m_sizeMemType[ in_memType ]	+=	in_size;
	++m_memCount;

	return	true;
}

 
 
 
 
 
 
int		CResCounter::PrintOut	()
{
	if	(	! m_enabledState	)	return	true;

	FILE	*fp;
	char	filename[ 256 ];
	 
	int		totalSize	=	0;
	SYSTEMTIME	systemTime;
	GetSystemTime( &systemTime );
	sprintf( filename, "resource_usage_%02d_%02d_%02d_%02d_%02d_%02d.log", systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond );

	char	*found	=	filename;
	while	(	found = strchr( found, ':' )	)
	{
		*found	=	'_';
		found++;
	}

	fp	=	fopen( filename, "wt" );

	int		index;
	for	(	index	=	0;	\
			index	<	RES_DATATYPE_NUM;	\
			++index		)
	{
		fprintf( fp, "%-30s : %12d\n", RES_DATATYPE_TEXT[ index ], m_sizeDataType[ index ] );
		 
		totalSize	+=	m_sizeDataType[ index ];
	}

	for	(	index	=	0;	\
			index	<	RES_MEMTYPE_NUM;	\
			++index		)
	{
		fprintf( fp, "%-30s : %12d\n", RES_MEMTYPE_TEXT[ index ], m_sizeMemType[ index ] );
		 
		totalSize	+=	m_sizeMemType[ index ];
	}

	 
	fprintf( fp, "\nTotal Size : %d bytes", totalSize );

	fclose( fp );
	
	return	true;
}

 
 
 
 
 
 
void	CResCounter::LockResCounter		()
{
	if	(	m_lockedState	)	return;

	m_lockedState	=	true;
}

 
 
 
 
 
 
void	CResCounter::EnableResCounter	()
{
	if	(	m_lockedState	)	return;

	m_enabledState	=	true;
}

 
 
 
 
 
 
void	CResCounter::DisableResCounter	()
{
	if	(	m_lockedState	)	return;

	m_enabledState	=	false;
}
