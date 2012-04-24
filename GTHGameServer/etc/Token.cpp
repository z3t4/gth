#include "../global.h"

char *curpos, *endpos;
byte *tokenBuffer = NULL;

int LoadTokenBuffer( char* filename )
{
	FILE *fp;
	int size;

	if( tokenBuffer != NULL ) DeleteTokenBuffer();
	
	fp = fopen( filename, "rb" );

	if( !fp ) return 0;
	
	fseek( fp, 0, SEEK_END );
	
	size = ftell( fp );
	
	tokenBuffer = new byte[size+1];
	
	fseek( fp, 0, SEEK_SET );
	fread( tokenBuffer, size, 1, fp );
	fclose( fp );
	
	SetTokenBuffer( tokenBuffer, size );

	return 1;
}

int DeleteTokenBuffer()
{
	delete tokenBuffer;
	tokenBuffer = NULL;

	return 1;
}

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
	        


			
			
	        while ( 1 )
			{
				if( IsDBCSLeadByte( (BYTE)*curpos ) )
				{
					curpos += 2;
					continue;
				}
				else if( *curpos++ == '\n' ) break;
				if (curpos == endpos)   return NULL;
			}
			
		        
	        
	        continue;
		}

	    
	    token = curpos;



		
		
	    while ( 1 )
		{
			if( IsDBCSLeadByte( (BYTE)*curpos ) )
			{
				curpos ++;
			}
			else if( *curpos == ' ' || *curpos == '\t' || *curpos == '\n' || *curpos == '\r' )
			{
				break;
			}
			if (++curpos == endpos)    break;
		}
	    
	    *curpos++ = '\0';
	    return token;
    }
    return NULL;
}


void NextCVSTokenReturn( char token[], int &endFile )
{
	int l = 0;
	endFile = 0;
    
	memset( token, 0, sizeof( token ) );

    while (curpos < endpos)
    {
	    while (*curpos != '\r' )
		{
			token[l] = *curpos;
			l ++;
			curpos ++;
	        if ( curpos == endpos )
			{
				endFile = 1;
				break;
			}
		}
	    
		if( *curpos == '\r' )
		{
			curpos ++;
			curpos ++;
	        if ( curpos == endpos )
			{
				endFile = 1;
				break;
			}
			
			endFile = 2;
		}
		else
		{
			curpos ++;
		}
	    token[l] = '\0';
	    return;
    }
	endFile = 1;
    return;
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
		
		



		while ( 1 )
		{
			if( IsDBCSLeadByte( (BYTE)*curpos ) )
			{
				curpos ++;
			}
			else if( *curpos == ' ' || *curpos == '\t' || *curpos == '\n' || *curpos == '\r' )
			{
				break;
			}
			if (++curpos == endpos)    break;
		}

	    
	    *curpos++ = '\0';
	    return arg;
    }
    return NULL;
}


int NextCSVToken( char token[] )
{
	int l = 0;
	int ret = 0;
    
	memset( token, 0, sizeof( token ) );

    while (curpos < endpos)
    {
		while ( 1 )
		{
			if( IsDBCSLeadByte( (BYTE)*curpos ) )
			{
				token[l] = *curpos;		l ++;	curpos ++;
				token[l] = *curpos;		l ++;	curpos ++;
			}
			else if( *curpos == ',' || *curpos == '\r' )
			{
				break;
			}
	        else if ( curpos == endpos )
			{
				
				ret = 1;
				break;
			}
			else
			{
				token[l] = *curpos;		l ++;	curpos ++;
			}
		}

		if( *curpos == '\r' )
		{
			curpos ++;
			curpos ++;
	        if ( curpos == endpos )
			{
				
				ret = 1;
				break;
			}
			
			ret = 2;
		}
		else
		{
			curpos ++;
		}
	    token[l] = '\0';
	    return( ret );
    }

	ret = 1;
    return( ret );
}
