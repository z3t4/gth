#include "core_def.h"
#include <stdio.h>
#include <string.h>
#include <TIME.H>

void Error( GTH_ERROR_TYPE error , char *str )
{
	char szTemp[256];
	sprintf( szTemp, "GTH Error : %s", str );

}

FILE *g_logFile;
char *g_logFilename = "../error.txt";
int g_logFirstWrite = true;

void ErrorLog( char* str, ... )
{
	
	return;

	char		cTime[32];
	time_t		logtime;
	char		buf[4096];
	va_list		marker;

	memset( buf, 0, sizeof( buf ) );
	va_start( marker, str );
	vsprintf( buf, str, marker);

	time( &logtime );
	strcpy( cTime, ctime( &logtime ) );
	
	cTime[24]	=	'\0';	
	if( g_logFirstWrite ) 
	{
		g_logFile	=	fopen( g_logFilename, "wt" );
		g_logFirstWrite = false;
	}
	else 
	{
		g_logFile	=	fopen( g_logFilename, "at" );
	}

	fprintf( g_logFile, "[%s]\t%s\n", cTime, buf );
	fclose( g_logFile );
}
