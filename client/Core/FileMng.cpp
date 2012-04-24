



#include "filemng.h"
#include "core_def.h"
#include "../skill/SkillFile.h"				



CFileMng g_fileMng;

#include <VECTOR>


typedef std::vector<string>			ReadedFileMap;
typedef ReadedFileMap::iterator		ReadedFileItor;

ReadedFileMap	readedfilemap;
DWORD			dupReadedFilesize		=	0;



CFileMng::CFileMng()
{
	m_basePath[0]		=	0;
	m_numPackFiles		=	0;
	m_numFilesInPack	=	0;
	strcpy( m_dataPath , "data/" );


	memset(m_monsterPath, 0, sizeof(m_monsterPath));
	strcpy(m_monsterPath, "monster/");

	memset(m_NPCPath, 0, sizeof(m_NPCPath));
	strcpy(m_NPCPath, "npc/");

	memset(m_itemPath,0,sizeof(m_itemPath));
	strcpy(m_itemPath, "items/");
	
	memset(m_earthPath,0,sizeof(m_earthPath));
	strcpy(m_earthPath, "earth/");
	
	memset(m_waterPath,0,sizeof(m_waterPath));
	strcpy(m_waterPath, "water/");
	
	memset(m_occultPath,0,sizeof(m_occultPath));
	strcpy(m_occultPath, "occult/");
	
	memset(m_windPath,0,sizeof(m_windPath));
	strcpy(m_windPath, "wind/");
}


CFileMng::~CFileMng()
{
	packfilemap::iterator i = m_fileMap.begin();

    while( i != m_fileMap.end() )
	{
		if( (*i).second  == NULL )
		{
			i ++;
			continue;
		}
		delete (*i ++).second;
	}
	m_fileMap.erase( m_fileMap.begin(), m_fileMap.end() );

	ClosePackFiles();
}

void CFileMng::ClosePackFiles()
{
	for( int i = 0; i < m_numPackFiles; i ++ )
		PackFileClose( &m_packFiles[ i ] );

	m_numPackFiles	=	0;
}

void CFileMng::MakePackFilesMap()
{
	int				i;
	int				num;
	packfile_t		*pf;
	packfileinfo_t	*pi, *pi_byname;
	
	for( i = 0; i < m_numPackFiles; i ++ )
	{	
		pf = &m_packFiles[i];

		for( num = 0; num < (int)pf->gi.number_entry; num ++ )
		{
			pi			= MakeFilesInfo( pf, i, num );
			pi_byname	= FindSameFilename( pi->fi.name );

			if( pi_byname == NULL )
			{
				RegisterFile( pi->fi.name, pi );

				m_numFilesInPack	++;
			}
			else
			{
				if( pi->fi.date > pi_byname->fi.date )
				{
					RegisterFile( pi->fi.name, pi );
					
					WritePackFileLog( "Duplicate file, %s(in %s) -> %s(in %s)", 
						pi_byname->fi.name, pi_byname->pack_name,
						pi->fi.name, pi->pack_name );
				}
			}
		}
	}
}

void CFileMng::WritePackFileLog( char* str, ... )
{
#ifdef _DEBUG_no
	FILE		*fp;
	char		buf[4096];
	va_list		marker;

	memset( buf, 0, sizeof( buf ) );
	
	va_start( marker, str );
	vsprintf( buf, str, marker);

	fp = fopen( "fileMng.log", "at" );
	
	fprintf( fp, "%s\n", buf );

	fclose( fp );
#endif
}

void CFileMng::GetBaseDirectoryFromRegistry()
{
	

	strcpy( m_basePath , "./" );
}



int CFileMng::PrepareMonsterPackFiles(int packType)
{
	char			path[128];
	HANDLE			hFile;
	WIN32_FIND_DATA fileData;
	char			PackageName[MAX_PATH];

	GetBaseDirectoryFromRegistry();
	
	strcpy(path, m_monsterPath);

	memset(PackageName, 0, sizeof(PackageName));
	strcpy(PackageName, m_monsterPath);
	
	switch( packType )
	{
	case ZIP_VERSION_PACK:
		{
			strcat( path , "*.pack" );
		}
		break;
	case I3D_VERSION_PACK:
		{
			strcat( path , "*.i3d" );
		}
		break;
	}
	
	hFile = FindFirstFile( path , &fileData );

	if( hFile == INVALID_HANDLE_VALUE )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no pack file" );
		return false;
	}

	strcat(PackageName, fileData.cFileName);
	::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , PackageName);
	while( FindNextFile( hFile , &fileData ) )
	{
		memset(PackageName, 0, sizeof(PackageName));
		strcpy(PackageName, m_monsterPath);
		strcat(PackageName, fileData.cFileName);

		::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , PackageName );
		
	}

	FindClose( hFile );

	return true;
}


int CFileMng::PrepareNPCPackFiles(int packType)
{
	char			path[128];
	HANDLE			hFile;
	WIN32_FIND_DATA fileData;
	char			PackageName[MAX_PATH];
	

	GetBaseDirectoryFromRegistry();

	strcpy(path, m_NPCPath);
	
	memset(PackageName, 0, sizeof(PackageName));
	strcpy(PackageName, m_NPCPath);
	
	

	switch( packType )
	{
	case ZIP_VERSION_PACK:
		{
			strcat( path , "*.pack" );
		}
		break;
	case I3D_VERSION_PACK:
		{
			strcat( path , "*.i3d" );
		}
		break;
	}
	
	hFile = FindFirstFile( path , &fileData );

	if( hFile == INVALID_HANDLE_VALUE )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no pack file" );
		return false;
	}

	strcat(PackageName, fileData.cFileName);
	::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , PackageName );
	
	while( FindNextFile( hFile , &fileData ) )
	{
		memset(PackageName, 0, sizeof(PackageName));
		strcpy(PackageName, m_NPCPath);
		strcat(PackageName, fileData.cFileName);

		::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , PackageName );
		
	}
	
	FindClose( hFile );

	return true;
}

int CFileMng::PrepareItemPackFiles(int packType)
{
	char			path[128];
	HANDLE			hFile;
	WIN32_FIND_DATA fileData;
	char			PackageName[MAX_PATH+1];

	GetBaseDirectoryFromRegistry();

	strcpy(path, m_itemPath);

	memset(PackageName, 0, sizeof(PackageName));
	strcpy(PackageName, m_itemPath);
	
	switch( packType )
	{
	case ZIP_VERSION_PACK:
		{
			strcat( path , "*.pack" );
		}
		break;
	case I3D_VERSION_PACK:
		{
			strcat( path , "*.i3d" );
		}
		break;
	}
	
	hFile = FindFirstFile( path , &fileData );
	
	if( hFile == INVALID_HANDLE_VALUE )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no pack file" );
		return false;
	}
	
	strcat(PackageName, fileData.cFileName);
	::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , fileData.cFileName );
	
	while( FindNextFile( hFile , &fileData ) )
	{
		memset(PackageName, 0, sizeof(PackageName));
		strcpy(PackageName, m_itemPath);
		strcat(PackageName, fileData.cFileName);
		
		::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , fileData.cFileName );
		
	}

	FindClose( hFile );

	return true;
}


int CFileMng::PrepareEarthPackageFiles(int packType)
{
	char			path[128];
	HANDLE			hFile;
	WIN32_FIND_DATA fileData;
	char			PackageName[MAX_PATH+1];
	
	GetBaseDirectoryFromRegistry();
	
	strcpy(path, m_earthPath);
	
	memset(PackageName, 0, sizeof(PackageName));
	strcpy(PackageName, m_itemPath);
	
	switch( packType )
	{
	case ZIP_VERSION_PACK:
		{
			strcat( path , "*.pack" );
		}
		break;
	case I3D_VERSION_PACK:
		{
			strcat( path , "*.i3d" );
		}
		break;
	}
	
	hFile = FindFirstFile( path , &fileData );
	
	if( hFile == INVALID_HANDLE_VALUE )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no pack file" );
		return false;
	}
	
	strcat(PackageName, fileData.cFileName);
	::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , fileData.cFileName );
	
	while( FindNextFile( hFile , &fileData ) )
	{
		memset(PackageName, 0, sizeof(PackageName));
		strcpy(PackageName, m_earthPath);
		strcat(PackageName, fileData.cFileName);
		
		::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , fileData.cFileName );
		
	}
	
	FindClose( hFile );
	
	return true;
}


int CFileMng::PrepareOccultPackageFiles(int packType)
{
	char			path[128];
	HANDLE			hFile;
	WIN32_FIND_DATA fileData;
	char			PackageName[MAX_PATH+1];
	
	GetBaseDirectoryFromRegistry();
	
	strcpy(path, m_occultPath);
	
	memset(PackageName, 0, sizeof(PackageName));
	strcpy(PackageName, m_itemPath);
	
	switch( packType )
	{
	case ZIP_VERSION_PACK:
		{
			strcat( path , "*.pack" );
		}
		break;
	case I3D_VERSION_PACK:
		{
			strcat( path , "*.i3d" );
		}
		break;
	}
	
	hFile = FindFirstFile( path , &fileData );
	
	if( hFile == INVALID_HANDLE_VALUE )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no pack file" );
		return false;
	}
	
	strcat(PackageName, fileData.cFileName);
	::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , fileData.cFileName );
	
	while( FindNextFile( hFile , &fileData ) )
	{
		memset(PackageName, 0, sizeof(PackageName));
		strcpy(PackageName, m_occultPath);
		strcat(PackageName, fileData.cFileName);
		
		::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , fileData.cFileName );
		
	}
	
	FindClose( hFile );
	
	return true;
}


int CFileMng::PrepareWindPackageFiles(int packType)
{
	char			path[128];
	HANDLE			hFile;
	WIN32_FIND_DATA fileData;
	char			PackageName[MAX_PATH+1];
	
	GetBaseDirectoryFromRegistry();
	
	strcpy(path, m_windPath);
	
	memset(PackageName, 0, sizeof(PackageName));
	strcpy(PackageName, m_itemPath);
	
	switch( packType )
	{
	case ZIP_VERSION_PACK:
		{
			strcat( path , "*.pack" );
		}
		break;
	case I3D_VERSION_PACK:
		{
			strcat( path , "*.i3d" );
		}
		break;
	}
	
	hFile = FindFirstFile( path , &fileData );
	
	if( hFile == INVALID_HANDLE_VALUE )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no pack file" );
		return false;
	}
	
	strcat(PackageName, fileData.cFileName);
	::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , fileData.cFileName );
	
	while( FindNextFile( hFile , &fileData ) )
	{
		memset(PackageName, 0, sizeof(PackageName));
		strcpy(PackageName, m_windPath);
		strcat(PackageName, fileData.cFileName);
		
		::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , fileData.cFileName );
		

	}
	
	FindClose( hFile );
	
	return true;
}


int CFileMng::PrepareWaterPackageFiles(int packType)
{
	char			path[128];
	HANDLE			hFile;
	WIN32_FIND_DATA fileData;
	char			PackageName[MAX_PATH+1];
	
	GetBaseDirectoryFromRegistry();
	
	strcpy(path, m_waterPath);
	
	memset(PackageName, 0, sizeof(PackageName));
	strcpy(PackageName, m_itemPath);
	
	switch( packType )
	{
	case ZIP_VERSION_PACK:
		{
			strcat( path , "*.pack" );
		}
		break;
	case I3D_VERSION_PACK:
		{
			strcat( path , "*.i3d" );
		}
		break;
	}
	
	hFile = FindFirstFile( path , &fileData );
	
	if( hFile == INVALID_HANDLE_VALUE )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no pack file" );
		return false;
	}
	
	strcat(PackageName, fileData.cFileName);
	::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , fileData.cFileName );
	
	while( FindNextFile( hFile , &fileData ) )
	{
		memset(PackageName, 0, sizeof(PackageName));
		strcpy(PackageName, m_waterPath);
		strcat(PackageName, fileData.cFileName);
		
		::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , fileData.cFileName );
		
	}
	
	FindClose( hFile );
	
	return true;
}



int CFileMng::PreparePackFiles(int packType)
{
	char			path[128];
	HANDLE			hFile;
	WIN32_FIND_DATA fileData;

	GetBaseDirectoryFromRegistry();

	strcpy( path , m_basePath );
	strcat( path , m_dataPath );

	SetCurrentDirectory( path );

	m_numPackFiles = 0;

	switch( packType )
	{
	case ZIP_VERSION_PACK:
		{
			strcpy( path , "*.pack" );
		}
		break;
	case I3D_VERSION_PACK:
		{
			strcpy( path , "*.i3d" );
		}
		break;
	}

	hFile = FindFirstFile( path , &fileData );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no pack file" );
		return false;
	}

	::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , fileData.cFileName );

	while( FindNextFile( hFile , &fileData ) )
		::PackFileOpen( &m_packFiles[ m_numPackFiles++ ] , fileData.cFileName );
		


	PrepareMonsterPackFiles(packType);
	PrepareNPCPackFiles(packType);
	PrepareItemPackFiles(packType);
	PrepareEarthPackageFiles(packType);
	PrepareOccultPackageFiles(packType);
	PrepareWindPackageFiles(packType);
	PrepareWaterPackageFiles(packType);


	FindClose( hFile );

	
	MakePackFilesMap();

	return true;
}

int CFileMng::ReadExtFilesFromPack( char *ext , packsearch_t **searchList )
{
	int				numFound;
	char			*extention;

	*searchList		=	m_packSearch;
	numFound		=	0;

	packfilemap::iterator i = m_fileMap.begin();

    while( i != m_fileMap.end() )
	{
        packfileinfo_t	*pi		=	(*i ++).second;

		if( pi == NULL )
			continue;
		
		extention				=	GetFileExt( pi->fi.name );

		if( !extention ) continue;	
		if( !stricmp( extention , ext ) ) 
		{
			m_packSearch[numFound].pack		=	pi->packidx;
			m_packSearch[numFound].entry	=	pi->idx_inpack;
			m_packSearch[numFound].size		=	pi->fi.size;
			m_packSearch[numFound].date		=	pi->fi.date;
			
			strcpy( m_packSearch[numFound].name , pi->fi.name );

			numFound	++;
		}
	}

	return numFound;
}

BYTE *CFileMng::ReadFileFromPack( packsearch_t *packSearch )
{
	char *buf;
	DWORD size;

	size = PackFileGetFileWithIndex( &m_packFiles[ packSearch->pack ] , packSearch->entry , &buf );

	string readedfname = packSearch->name;
	if( find( readedfilemap.begin(), readedfilemap.end(), readedfname ) != readedfilemap.end() )
	{
		WritePackFileLog( "%s Read Duplicated!(%.2f)", packSearch->name, (float)packSearch->size * 0.001f );
		dupReadedFilesize += packSearch->size;
		WritePackFileLog( "TotalSize : %.2fk", (float)dupReadedFilesize * 0.001f );
	}
	else
	{
		readedfilemap.push_back( readedfname );
	}

	if( size <= 0 )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is an error in extracting file" );
		WritePackFileLog( "%s There is an error in extracting file", packSearch->name );

		return NULL;
	}

	return (BYTE*) buf;
}

DWORD CFileMng::GetFileDateFromPack( char *fname )
{	
	packfileinfo_t		*pi		=	GetFromFilename( fname );

	if( pi == NULL ) 
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no such file" );
		WritePackFileLog( "%s There is no such file", fname );

		return NULL;
	}

	return	pi->fi.date;
}

DWORD CFileMng::GetFileSizeFromPack( char *fname )
{
	packfileinfo_t		*pi		=	GetFromFilename( fname );
	DWORD				size;

	if( pi == NULL ) 
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no such file" );
		WritePackFileLog( "%s There is no such file", fname );

		return NULL;
	}

	size	=	pi->fi.size;

	if( size <= 0 )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is an error in file" );
		WritePackFileLog( "%s There is an error in file", fname );

		return NULL;
	}

	return size;
}

BYTE *CFileMng::ReadFileFromPack( char *fname )
{
	packfileinfo_t		*pi		=	GetFromFilename( fname );
	DWORD				size;
	char				*buf;

	if( pi == NULL ) 
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no such file" );
		WritePackFileLog( "%s There is no such file", fname );
		return NULL;
	}

	string readedfname = fname;
	if( find( readedfilemap.begin(), readedfilemap.end(), readedfname ) != readedfilemap.end() )
	{
		WritePackFileLog( "%s Read Duplicated!(%.2fk)", fname, (float)pi->fi.size * 0.001f );
		dupReadedFilesize += pi->fi.size;
		WritePackFileLog( "TotalSize : %.2fk", (float)dupReadedFilesize * 0.001f );
	}
	else
	{
		readedfilemap.push_back( readedfname );
	}

	size	=	PackFileGetFileWithIndex( &m_packFiles[pi->packidx], pi->idx_inpack, &buf );

	if( size <= 0 )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is an error in extracting file" );
		WritePackFileLog( "%s There is an error in extracting file", fname );
		return NULL;
	}

	return (BYTE*) buf;
}

DWORD CFileMng::GetFileSize( char *fname )
{
	char   path[128];
	HANDLE hFile;
	DWORD  size;
	
	strcpy( path , m_basePath );
	strcat( path , m_dataPath );

	SetCurrentDirectory( path );

	strcat( path , fname );

	hFile = CreateFile( path , GENERIC_READ , FILE_SHARE_READ ,	NULL 
		              , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );

	if( hFile == INVALID_HANDLE_VALUE )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no such file" );
		return false;
	}

	size = ::GetFileSize( hFile , NULL );

	CloseHandle( hFile );

	return size;
}

BYTE *CFileMng::ReadFile( char *fname )
{
	char   path[128];
	HANDLE hFile;
	DWORD  size , sizeRead;
	int    ret;
	BYTE  *buf;
	
	strcpy( path , m_basePath );
	strcat( path , m_dataPath );

	SetCurrentDirectory( path );

	strcat( path , fname );

	hFile = CreateFile( path , GENERIC_READ , FILE_SHARE_READ ,	NULL 
		              , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );

	if( hFile == INVALID_HANDLE_VALUE )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no such file" );
		return false;
	}

	size = ::GetFileSize( hFile , NULL );

	buf = new BYTE[ size ];

	ret = ::ReadFile( hFile , buf , size , &sizeRead , NULL );
	
	if( !ret || ( sizeRead != size ) )
	{
		CloseHandle( hFile );
		GTH_ERROR( GTH_ERROR_PRINT , "There is an error in reading file" );
		return false;
	}

	CloseHandle( hFile );

	return buf;
}

void CFileMng::SetBaseDirectory( char *path )
{
	strcpy( m_basePath , path );
}

char *CFileMng::GetFileExt( char *fname )
{
	int i , found;
	int pathLen = strlen( fname );

	found = 0;
	for( i = 0 ; i < pathLen ; i++ )
	{
		if( fname[i] == '.' )
		{
			found = i;
			break;
		}
	}

	if( found ) return fname + found + 1;
	else        return NULL;

	
}

char *CFileMng::GetFileName( char *fname )
{
	int i,found;
	int pathLen = strlen( fname );

	found = false;
	for( i = pathLen - 1 ; i >= 0 ; i-- )
	{
		if( ( fname[i] == '/' ) || ( fname[i] == '\\' ) )
		{
			found = true;
			break;
		}
	}

	if( found ) return fname + i + 1;
	else        return fname;
}

char *CFileMng::GetFileNameOnly( char *fname )
{
	int i,found;
	int pathLen = strlen( fname );

	found = false;
	for( i = pathLen - 1 ; i >= 0 ; i-- )
	{
		if( ( fname[i] == '.' ) )
		{
			found = true;
			break;
		}
	}

	if( found )  fname[ i ] = 0;

	return fname;
}

char *CFileMng::GetFilePath( char *fname )
{
	int i,found;
	int pathLen = strlen( fname );

	found = false;
	for( i = pathLen - 1 ; i >= 0 ; i-- )
	{
		if( ( fname[i] == '/' ) || ( fname[i] == '\\' ) )
		{
			found = true;
			break;
		}
	}

	if( found )  fname[ i + 1 ] = 0;
	else         fname[0] = 0;

	return fname;
}

void CFileMng::ConvertUnixPath( char *path )
{
	int pathLen = strlen(path);

	for( int i = 0 ; i < pathLen ;  i++ )
		if( path[i] == '\\' )
			path[i] = '/';
}

filetype_t CFileMng::FindFileType( char* fname )
{
	char *extension;

	extension = GetFileExt( fname );

	if( !extension )
		return  NOT_SUPPORTED_FILE_TYPE;
	
	if      ( !strcmpi(extension, "zip") )    return  DOWN_ZIP_FILE_TYPE;
	else if ( !strcmpi(extension, "i3d") )    return  DOWN_ZIP_FILE_TYPE;
	else if ( !strcmpi(extension, "pak") )    return  DOWN_ZIP_FILE_TYPE;

	else if ( !strcmpi(extension, "md3") )    return  MODEL_MD3_FILE_TYPE;
	else if ( !strcmpi(extension, "bsp") )    return  WORLD_BSP_FILE_TYPE;
	else if ( !strcmpi(extension, "sector") ) return  WORLD_SECTOR_FILE_TYPE;
	
	else if ( !strcmpi(extension, "jpg") )    return  TEXTURE_JPEG_FILE_TYPE;
	else if ( !strcmpi(extension, "jpeg") )   return  TEXTURE_JPEG_FILE_TYPE;
	else if ( !strcmpi(extension, "tga") )    return  TEXTURE_TGA_FILE_TYPE;
	else if ( !strcmpi(extension, "bmp") )    return  TEXTURE_BMP_FILE_TYPE;
	
	else if ( !strcmpi(extension, "shader") )  return  SCRIPT_SHADER_FILE_TYPE;
	else if ( !strcmpi(extension, "object") )  return  SCRIPT_OBJECT_FILE_TYPE;
	else if ( !strcmpi(extension, "class") )   return  SCRIPT_CLASS_FILE_TYPE;
	else if ( !strcmpi(extension, "cfg") )     return  SCRIPT_AVATAR_FILE_TYPE;
	
	else if ( !strcmpi(extension, "wav") )     return  SOUND_WAVE_FILE_TYPE;
	else if ( !strcmpi(extension, "mp3") )     return  SOUND_MPEG_FILE_TYPE;
	else if ( !strcmpi(extension, "raw") )     return  SOUND_RAW_FILE_TYPE;

	else if ( !strcmpi(extension, "dat") )    return  CHARACTER_CONFIG_FILE_TYPE;
	else if ( !strcmpi(extension, "csf") )    return  CHARACTER_SKELETON_FILE_TYPE;
	else if ( !strcmpi(extension, "caf") )    return  CHARACTER_ANIMATION_FILE_TYPE;
	else if ( !strcmpi(extension, "cmf") )    return  CHARACTER_MESH_FILE_TYPE;

	else 
	   return  NOT_SUPPORTED_FILE_TYPE;
	
}


bool CFileMng::RegisterFileFromPack( char* fname, RHANDLE rhID, BaseResource *pRes, DWORD initialType )
{
	packfileinfo_t		*pi		=	GetFromFilename( fname );
	DWORD				size;
	
	if( pi == NULL )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "There is no such file" );
		WritePackFileLog( "%s There is no such file", fname );
		return false;
	}

	size		=	pi->fi.size;

	if( initialType & RES_INIT_NOTCREATE )
	{
		
		if( !m_resMng->RegisterResource( rhID, pRes ) ) return false;
		
		if( !pRes->Register( fname, size ) ) return false;
	}
	else
	{
		
		if( !m_resMng->ReserveMemory( size ) ) return false;	
		
		if( !pRes->Create( fname, size ) ) return false;
		
		if( !m_resMng->InsertResource( rhID, pRes ) ) return false;
	}

	return true;
}

