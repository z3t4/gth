#ifndef UNPAK_H
#define	UNPAK_H

#include <windows.h>
#include "unzip.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ERROR_FILE_NOT_EXIST       -1
#define ERROR_PACK_FILE_NOT_EXIST  -2
#define ERROT_NOT_ENOUGHT_MEMORY   -3
#define ERROR_FILE_NOT_EXTRACTED   -4

#define PACK_FILE_NAME_SIZE   128

typedef struct
{
	char   name[PACK_FILE_NAME_SIZE];
	DWORD  attr;
	DWORD  offset;
	DWORD  size;
	DWORD  c_offset;
	DWORD  date;
} fileinfo_t;

typedef struct
{
	char            pak_name[PACK_FILE_NAME_SIZE];
	unzFile	        uf;
	unz_global_info	gi;
	fileinfo_t	   *fi;
} packfile_t;

typedef struct
{
	int   pack;
	int   entry;
	DWORD size;
	DWORD date;
	char  name[ PACK_FILE_NAME_SIZE ];
} packsearch_t;

int	   PackFileOpen (packfile_t *pf, const char *zname);
void   PackFileClose (packfile_t *pf);
int    PackFileGetFileWithName (packfile_t *pf, char *fname, char **buf);
int    PackFileGetFileWithIndex (packfile_t *pf, int idx , char **buf);
int	   PackFileGetEntrySize (packfile_t *pf);
char  *PackFileGetFileName (packfile_t *pf, int num);
uLong  PackFileGetFileSizeWithName( packfile_t *pf , char *fname );
uLong  PackFileGetFileSizeWithIndex( packfile_t *pf , int idx );
int    PackFileSearchFile( packfile_t *pf , char *fname );
int    PackFileSearchFilesExt( packfile_t *pf , char *ext , int pack , packsearch_t *list );
 
int		PackFileSearchRecentFile	(	packfile_t	*pf,	char	*fname	);

#ifdef __cplusplus
}
#endif

#endif