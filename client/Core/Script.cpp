



#include "script.h"
#include "filemng.h"

CScript g_script;





CScript::CScript()
{
	m_name[0]    = NULL;
	m_token[0]   = NULL;
	m_buffer     = NULL;
	m_prevOffset = NULL;
	m_currOffset = NULL;
	m_bufferSize = 0;
	m_lines      = 0;
}

CScript::~CScript()
{
	GTH_SAFE_FREE( m_buffer );
}

void CScript::Reset()
{
	GTH_SAFE_FREE( m_buffer );

	m_name[0]    = NULL;
	m_token[0]   = NULL;
	m_prevOffset = NULL;
	m_currOffset = NULL;
	m_bufferSize = 0;
	m_lines      = 0;
}

int CScript::Load( char *fName )
{
	if( m_buffer )  Reset();

	strcpy( m_name , fName );

	m_bufferSize = g_fileMng.GetFileSizeFromPack( fName );
	if( !m_bufferSize ) return false;

	m_buffer = g_fileMng.ReadFileFromPack( fName );
	if( !m_buffer ) return false;

	m_buffer[ m_bufferSize - 1 ] = 0;
	return true;
}

int CScript::Load( char *fName , DWORD size , BYTE *data )
{
	if( !size ) return false;
	if( !data ) return false;

	if( m_buffer )  Reset();

	strcpy( m_name , fName );

	m_bufferSize = size;
	m_buffer = data;

	m_buffer[ m_bufferSize - 1 ] = 0;
	return true;
}


int CScript::SkipSpace()
{
	int ch;
	int hasNewLines = false;

	while( (ch = *m_currOffset) <= ' ') {
		if( !ch ) {
			m_currOffset = NULL;
			return hasNewLines;
		}
		if( ch == '\n' ) {
			m_lines++;
			hasNewLines = true;
		}
		m_currOffset++;
	}

	return hasNewLines;
}


char * CScript::GetNextToken()
{
	return GetNextToken( true );
}


char * CScript::GetNextToken(int allowLineBreak)
{
	int ch = 0, len;
	int hasNewLines = false;
	
	m_currOffset = m_prevOffset;
	len = 0;
	m_token[0] = 0;

	
	if ( !m_currOffset )
	{
		m_prevOffset = NULL;
		return m_token;
	}

	while ( 1 )
	{
		
		hasNewLines = SkipSpace();
		if ( !m_currOffset ) 
		{
			m_prevOffset = NULL;
			return m_token;
		}
		if ( hasNewLines && !allowLineBreak )
		{
			m_prevOffset = m_currOffset;
			return m_token;
		}

		ch = *m_currOffset;

		
		if ( ch == '/' && m_currOffset[1] == '/' )
		{
			while (*m_currOffset && *m_currOffset != '\n')
				m_currOffset++;
		}
		
		else if ( ch == '/' && m_currOffset[1] == '*' ) 
		{
			while ( *m_currOffset && ( *m_currOffset != '*' || m_currOffset[1] != '/' ) ) 
			{
				m_currOffset++;
			}
			if ( *m_currOffset ) 
			{
				m_currOffset += 2;
			}
		}
		else
		{
			break;
		}
	}

	
	if (ch == '\"')
	{
		m_currOffset++;
		while (1)
		{
			ch = *m_currOffset++;
			if (ch == '\"' || !ch)
			{
				m_token[len] = 0;
				m_prevOffset = m_currOffset;
				return m_token;
			}
			if (len < GTH_MAX_SIZE_TOKEN)
			{
				m_token[len] = ch;
				len++;
			}
		}
	}

	
	do
	{
		if (len < GTH_MAX_SIZE_TOKEN)
		{
			m_token[len] = ch;
			len++;
		}
		m_currOffset++;
		ch = *m_currOffset;
		if ( ch == '\n' )
			m_lines++;
	} while (ch > 32);

	if (len == GTH_MAX_SIZE_TOKEN)
	{
		GTH_ERROR( GTH_ERROR_OVERFLOW_SIZE , m_name );
		len = 0;
	}
	m_token[len] = 0;

	m_prevOffset = m_currOffset;
	return m_token;
}

char * CScript::GetCurrToken()
{
	return m_token;
}

int CScript::SetOffset(int offset)
{
	return true;
}


void CScript::SkipLine()
{
	BYTE *p;
	int		  ch;

	p = m_currOffset;
	while ( (ch = *p++) != 0 ) {
		if ( ch == '\n' ) {
			m_lines++;
			break;
		}
	}

	m_currOffset = p;
	m_prevOffset = m_currOffset;
}

char * CScript::GetName()
{
	return m_name;
}

void CScript::BeginParse()
{
	m_currOffset = m_buffer;
	m_prevOffset = m_buffer;

	m_token[0]   = NULL;

	m_lines = 0;
}

int CScript::GetCurrParseLine()
{
	return m_lines;
}

void CScript::SkipBracedSection()
{
	char			*token;
	int				depth;

	depth = 0;
	do {
		token = GetNextToken( true );
		if( token[1] == 0 ) {
			if( token[0] == '{' ) {
				depth++;
			}
			else if( token[0] == '}' ) {
				depth--;
			}
		}
	} while( depth && m_currOffset );
}

int CScript::MatchToken(char *match)
{
	char	*token;

	token = GetNextToken( true );
	if ( stricmp( token, match ) ) {
		GTH_ERROR( GTH_ERROR_PRINT , match );
		return false;
	}

	return true;
}



int CScript::GetCSVToken( char token[] )
{
	int l = 0;
	int ret = 0;
	BYTE *endOffset = m_prevOffset + m_bufferSize;
    
	memset( token, 0, sizeof( token ) );

    while ( m_currOffset < endOffset )
    {
		while ( 1 )
		{
			if( IsDBCSLeadByte( (BYTE)*m_currOffset ) )
			{
				token[l] = *m_currOffset;		l ++;	m_currOffset ++;
				token[l] = *m_currOffset;		l ++;	m_currOffset ++;
			}
			else if( *m_currOffset == ',' || *m_currOffset == '\r' )
			{
				break;
			}
	        else if( m_currOffset >= endOffset )
			{
				
				ret = 1;
				break;
			}
			else
			{
				token[l] = *m_currOffset;		l ++;	m_currOffset ++;
			}
		}

		if( *m_currOffset == '\r' )
		{
			m_currOffset ++;
			m_currOffset ++;
	        if( m_currOffset >= endOffset )
			{
				
				ret = 1;
				break;
			}
			
			ret = 2;
		}
		else
		{
			m_currOffset ++;
		}
	    token[l] = '\0';
	    return( ret );
    }

	ret = 1;
    return( ret );
}
