



#if !defined(AFX_FILEMNG_H__DB30E545_3A8E_44CD_AF4D_700AF6FFBAF1__INCLUDED_)
#define AFX_FILEMNG_H__DB30E545_3A8E_44CD_AF4D_700AF6FFBAF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#pragma warning (disable:4786)

#include <IOSTREAM>
#include <algorithm>
#include <STRING>
#include <MAP>

#include "../zlib/unpak.h"
#include "ResManager.h"

using namespace std;

typedef enum
{

	NOT_SUPPORTED_FILE_TYPE,

	DOWN_ZIP_FILE_TYPE,

	MODEL_AVATA_FILE_TYPE,
	MODEL_MD3_FILE_TYPE,
	MODEL_MD2_FILE_TYPE,
	MODEL_3DS_FILE_TYPE,

	WORLD_BSP_FILE_TYPE,
	WORLD_SECTOR_FILE_TYPE,
	WORLD_ZONE_FILE_TYPE,

	TEXTURE_JPEG_FILE_TYPE,
	TEXTURE_TGA_FILE_TYPE,
	TEXTURE_GIF_FILE_TYPE,
	TEXTURE_BMP_FILE_TYPE,

	SCRIPT_SHADER_FILE_TYPE,
	SCRIPT_OBJECT_FILE_TYPE,
	SCRIPT_MD3SHADER_FILE_TYPE,
	SCRIPT_CLASS_FILE_TYPE,
	SCRIPT_AVATAR_FILE_TYPE,
	SCRIPT_SKIN_FILE_TYPE,

	SOUND_WAVE_FILE_TYPE,
	SOUND_MPEG_FILE_TYPE,
	SOUND_RAW_FILE_TYPE,

	CHARACTER_CONFIG_FILE_TYPE,
	CHARACTER_SKELETON_FILE_TYPE,
	CHARACTER_ANIMATION_FILE_TYPE,
	CHARACTER_MESH_FILE_TYPE
} filetype_t;

#define MAX_SIZE_PACK_FILE    512
#define MAX_SIZE_PACK_SEARCH  1024

typedef struct
{
	
	fileinfo_t			fi;
	
	
	char				pack_name[PACK_FILE_NAME_SIZE];
	int					idx_inpack;
	int					packidx;

} packfileinfo_t;

typedef map<string, packfileinfo_t*, less<string> >	packfilemap;

class CFileMng
{
public	:
	CFileMng();
	~CFileMng();
	
	void				ClosePackFiles();

	int					PreparePackFiles( int packType );


	int 				PrepareMonsterPackFiles(int packType);
	int					PrepareNPCPackFiles(int packType);
	int					PrepareItemPackFiles(int packType);
	int					PrepareOccultPackageFiles(int packType);
	int					PrepareEarthPackageFiles(int packType);
	int					PrepareWaterPackageFiles(int packType);
	int					PrepareWindPackageFiles(int packType);

	
	void				GetBaseDirectoryFromRegistry();

	int					ReadExtFilesFromPack( char *ext , packsearch_t **list );
	BYTE*				ReadFileFromPack( packsearch_t *packSearch );
	DWORD				GetFileSizeFromPack( char *fname );
	DWORD				GetFileDateFromPack( char *fname ); 
	BYTE*				ReadFileFromPack( char *fname );
	DWORD				GetFileSize( char *fname );
	BYTE*				ReadFile( char *fname );
	void				SetBaseDirectory( char *path );
	static char*		GetFileExt( char *fname );
	static char*		GetFileName( char *fname );
	static char*		GetFilePath( char *fname );
	static void			ConvertUnixPath( char *path );
	static filetype_t	FindFileType( char* fname );

	static char*		GetFileNameOnly( char *fname );

	int					GetFilesCountInPack()	{	return( m_numFilesInPack );		}

	
	void				RegisterFile( char* fname, packfileinfo_t* fi );
	packfileinfo_t*		FindSameFilename( char *fname );
	packfileinfo_t*		GetFromFilename( char *fname );
	packfileinfo_t*		MakeFilesInfo( packfile_t *pf, int packidx, int idx );
	void				MakePackFilesMap();
	
	void				WritePackFileLog( char* str, ... );

	
	
	void				SetResourceManager( CResManager *resMng )		{	m_resMng	=	resMng;	}
	bool				RegisterFileFromPack( char* fname, RHANDLE rhID, BaseResource *pRes, DWORD initialType = 0 );

private	:

	packfilemap		m_fileMap;

	char			m_basePath[128];
	char			m_dataPath[128];


	char			m_monsterPath[128];
	char			m_NPCPath[128];
	char			m_itemPath[128];
	char			m_earthPath[128];
	char			m_occultPath[128];
	char			m_waterPath[128];
	char			m_windPath[128];


	int				m_numPackFiles;
	int				m_numFilesInPack;
	packfile_t		m_packFiles[MAX_SIZE_PACK_FILE];
	packsearch_t	m_packSearch[MAX_SIZE_PACK_SEARCH];

	
	CResManager*	m_resMng;
};

inline void CFileMng::RegisterFile( char* fname, packfileinfo_t* fi )
{
	packfileinfo_t	*pi			=	new	packfileinfo_t;
	string			fileType	=	fname;

	memcpy( pi, fi, sizeof( packfileinfo_t ) );

	m_fileMap[fileType] = pi;
}

inline packfileinfo_t* CFileMng::MakeFilesInfo( packfile_t *pf, int packidx, int idx )
{
	static packfileinfo_t	pi;

	memcpy( &pi.fi,			&pf->fi[idx],	sizeof( fileinfo_t ) );
	strcpy( pi.pack_name,	pf->pak_name );
	
	pi.packidx		=	packidx;
	pi.idx_inpack	=	idx;

	return( &pi );
}

inline packfileinfo_t* CFileMng::FindSameFilename( char *fname )
{
	if( m_fileMap.find( fname ) == m_fileMap.end() )
	{
		return NULL;
	}

	return m_fileMap[fname];
}

inline packfileinfo_t* CFileMng::GetFromFilename( char *fname )
{
	string			fileType;
	char			lwFname[PACK_FILE_NAME_SIZE];

	strcpy( lwFname, fname );
	_strlwr( lwFname );

	fileType	=	lwFname;

	return m_fileMap[fileType];
}

extern CFileMng g_fileMng;

#endif 
