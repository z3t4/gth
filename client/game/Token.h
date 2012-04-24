extern char *curpos, *endpos;

int SetTokenBuffer( byte *buffer , int size );
char*   NextToken();
char*   NextArg();

int BeginTokenScript( char* filename );
void EndTokenScript();
