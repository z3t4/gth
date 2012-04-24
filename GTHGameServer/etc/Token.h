extern char *curpos, *endpos;

#define CSV_ReadField	0
#define CSV_EndOfFile	1
#define CSV_EndOfLine	2

int		LoadTokenBuffer( char* filename );
int		DeleteTokenBuffer();
int		SetTokenBuffer( byte *buffer , int size );
char*   NextToken();
char*   NextArg();
int		NextCSVToken( char token[] );
