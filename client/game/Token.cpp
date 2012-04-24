#include "../global_def.h"

char *curpos, *endpos;
byte *g_tokenBuffer = NULL;
FILE *g_tokenFileBuffer;

int SetTokenBuffer( byte *buffer , int size )
{
	curpos = (char *)buffer;
	endpos = curpos + size;

	if( size <= 0 ) 
		return false;

	return true;
}

char* NextToken()
{
    char *token;
    
    while (curpos < endpos)
    {
	    
	    while (*curpos == ' ' || *curpos == '\t' || *curpos == '\n' || *curpos == '\r')
	        if (++curpos == endpos)    return NULL;

	    
	    if (curpos[0] == '/' && curpos[1] == '/')
		{
	        
	        while (*curpos++ != '\n')
		        if (curpos == endpos)   return NULL;
	        
	        continue;
		}

	    
	    token = curpos;
	    while (*curpos != ' ' && *curpos != '\t' && *curpos != '\n' && *curpos != '\r')
	        if (++curpos == endpos)    break;

	    
	    *curpos++ = '\0';
	    return token;
    }
    return NULL;
}


char* NextArg()
{
    char *arg;

    while (curpos < endpos)
    {
	    
	    while (*curpos == ' ' || *curpos == '\t')
	        if (++curpos == endpos)    return NULL;

	    
	    if( *curpos == '\n' || *curpos == '\r' ||
		   (curpos[0] == '/' && curpos[1] == '/') )
	        return NULL;
	
	    
	    arg = curpos;
	    while (*curpos != ' ' && *curpos != '\t' && *curpos != '\n' && *curpos != '\r')
	        if (++curpos == endpos)    break;

	    
	    *curpos++ = '\0';
	    return arg;
    }
    return NULL;
}

int BeginTokenScript( char* filename )
{
	int size;

	g_tokenFileBuffer = fopen( filename , "rb" );
	if( !g_tokenFileBuffer )   
	{
		return false;
	}

	fseek(g_tokenFileBuffer,0,SEEK_END);

	size = ftell(g_tokenFileBuffer);

	g_tokenBuffer = new byte[size];

	fseek(g_tokenFileBuffer,0,SEEK_SET);
	fread(g_tokenBuffer,size,1,g_tokenFileBuffer);
	fclose(g_tokenFileBuffer);

	curpos = (char *)g_tokenBuffer;
	endpos = curpos + size;

	return( true );
}

void EndTokenScript()
{
	if( g_tokenBuffer != NULL ) delete g_tokenBuffer;
}
