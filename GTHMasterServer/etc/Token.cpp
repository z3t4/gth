#include "../global.h"

char *curpos, *endpos;

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