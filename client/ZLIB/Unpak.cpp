
#include <windows.h>

#include "unpak.h"


int	PackFileOpen (packfile_t *pf, const char *zname)
{
	int		x, max;
	unz_file_info	file_info;

	strcpy (pf->pak_name, zname);

	pf->uf = unzOpen (pf->pak_name);
	if (!pf->uf)
	{
		return ERROR_PACK_FILE_NOT_EXIST;
	}
	unzGetGlobalInfo (pf->uf, &pf->gi);
	pf->fi = (fileinfo_t *)malloc (sizeof(fileinfo_t)*pf->gi.number_entry);
	if (!pf->fi)
	{
		unzClose (pf->uf);
		return ERROT_NOT_ENOUGHT_MEMORY;
	}

	max = pf->gi.number_entry;

	for (x=0; x<max; x++)
	{
		unzGetCurrentFileInfo (pf->uf,&file_info, pf->fi[x].name,sizeof(pf->fi[x].name),NULL,0,NULL,0);
		pf->fi[x].attr = file_info.external_fa;
		pf->fi[x].offset = file_info.offset;
		pf->fi[x].size = file_info.uncompressed_size;
		pf->fi[x].c_offset = file_info.c_offset;
		pf->fi[x].date = file_info.dosDate;
		_strlwr( pf->fi[x].name );
		unzGoToNextFile (pf->uf);
	}
	return 1;
}

void PackFileClose (packfile_t *pf)
{
	 
	if	(	! pf->uf	)
		return;

	if (pf->fi)
	{
		free (pf->fi);
		pf->fi = NULL;
	}
	if (pf->uf)
	{
		unzClose (pf->uf);
		pf->uf = NULL;
	}
}

int PackFileGetFileWithName (packfile_t *pf, char *fname, char **buf)
{
	int		num;
	DWORD	err;
	for (num = 0; num<(int)pf->gi.number_entry; num++)
	{
		if (!stricmp(fname, pf->fi[num].name)) break;
	}

	if (num >= (int)pf->gi.number_entry)
		return ERROR_FILE_NOT_EXIST;
	
	*buf = (char *)malloc (pf->fi[num].size);

	unzLocateFileMy (pf->uf, num, pf->fi[num].c_offset);

	unzOpenCurrentFile (pf->uf);
	err = unzReadCurrentFile (pf->uf,*buf,pf->fi[num].size);
	unzCloseCurrentFile (pf->uf);

	if (err!=pf->fi[num].size)
	{
		free (*buf);
		*buf = NULL;
		return ERROR_FILE_NOT_EXTRACTED;
	}

	return pf->fi[num].size;
}

int PackFileGetFileWithIndex (packfile_t *pf, int idx , char **buf)
{
	DWORD	err;
	
	if (idx >= (int)pf->gi.number_entry)
		return ERROR_FILE_NOT_EXIST;
	
	*buf = (char *)malloc (pf->fi[idx].size);

	unzLocateFileMy (pf->uf, idx, pf->fi[idx].c_offset);

	unzOpenCurrentFile (pf->uf);
	err = unzReadCurrentFile (pf->uf,*buf,pf->fi[idx].size);
	unzCloseCurrentFile (pf->uf);

	if (err!=pf->fi[idx].size)
	{
		free (*buf);
		*buf = NULL;
		return ERROR_FILE_NOT_EXTRACTED;
	}

	return pf->fi[idx].size;
}

int PackFileGetEntrySize (packfile_t *pf)
{
	return pf->gi.number_entry;
}

char *PackFileGetFileName (packfile_t *pf, int num)
{
	if (num >= (int)pf->gi.number_entry) return NULL;
	return pf->fi[num].name;
}

uLong PackFileGetFileSizeWithName( packfile_t *pf , char *fname )
{
	int		num;

	for (num = 0; num<(int)pf->gi.number_entry; num++)
	{
		if (!stricmp(fname, pf->fi[num].name)) break;
	}

	if (num >= (int)pf->gi.number_entry)
		return ERROR_FILE_NOT_EXIST;

	return pf->fi[num].size;
}

uLong PackFileGetFileSizeWithIndex( packfile_t *pf , int idx )
{
	if (idx >= (int)pf->gi.number_entry)
		return ERROR_FILE_NOT_EXIST;

	return pf->fi[idx].size;
}

int PackFileSearchFile( packfile_t *pf , char *fname )
{
	 
	if	(	! pf->uf	)
		return ERROR_FILE_NOT_EXIST;

	int		num;

	for (num = 0; num<(int)pf->gi.number_entry; num++)
	{
		if (!stricmp(fname, pf->fi[num].name)) break;
	}

	if (num >= (int)pf->gi.number_entry)
		return ERROR_FILE_NOT_EXIST;

	return num;
}

 
 
 
 
 
 
 
int	PackFileSearchRecentFile	(	packfile_t	*pf,
									char		*fname	)
{
	if	(	! pf->uf	)
		return	ERROR_FILE_NOT_EXIST;

	int	recent	=	-1;

	int	num;
	for	(	num	=	0;	\
			num	<	(int)pf->gi.number_entry;	\
			++num	)
	{
		if	(	! stricmp( fname, pf->fi[ num ].name )	)
			break;
	}

	if	(	num	>=	(int)pf->gi.number_entry	)
		return	ERROR_FILE_NOT_EXIST;

	recent	=	num;
	for	(	;	\
			num	<	(int)pf->gi.number_entry;	\
			++num	)
	{
		if	(	! stricmp( fname, pf->fi[ num ].name )	)
		{
			if	(	pf->fi[ recent ].date	<	pf->fi[ num ].date	)
			{
				recent	=	num;
			}
		}
	}

	return	recent;
}

char *GetFileExt( char *fname );
int PackFileSearchFilesExt( packfile_t *pf , char *ext , int pack , packsearch_t *list )
{
	 
	if	(	! pf->uf	)
		return	0;

	unsigned long i;
	int numFound;
	char *extention;

	numFound = 0;
	for( i = 0 ; i < pf->gi.number_entry ; i++ )
	{
		extention = GetFileExt( pf->fi[i].name );
		if( !extention ) continue;		

		if( !stricmp( extention , ext ) ) 
		{
			list[ numFound ].pack  = pack;
			list[ numFound ].entry = i;
			list[ numFound ].size  = pf->fi[i].size;
			list[ numFound ].date  = pf->fi[i].date;
			strcpy( list[ numFound++ ].name , pf->fi[i].name );
		}
	}

	return numFound;
}

char *GetFileExt( char *fname )
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