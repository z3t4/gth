#include "../global_def.h"

gthconfig_t		g_lowQualityConfig;
gthconfig_t		g_highQualityConfig;
gthconfig_t		g_userConfig;

int GTH_Setup_LoadConfig( char* filename )
{
	char* token;

	memset( &g_lowQualityConfig, 0, sizeof( gthconfig_t ) );
	memset( &g_highQualityConfig, 0, sizeof( gthconfig_t ) );
	memset( &g_userConfig, 0, sizeof( gthconfig_t ) );
	
	g_lowQualityConfig.isLoaded = false;
	g_highQualityConfig.isLoaded = false;
	g_userConfig.isLoaded = false;

	if( !BeginTokenScript( filename ) ) return 0;

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;
		
		
		if( !stricmp( token, "l_resolution" ) )
		{
			g_lowQualityConfig.isLoaded = true;
			token = NextToken();
			g_lowQualityConfig.resoution = atoi( token );
		} else if( !stricmp( token, "l_texture" ) )
		{
			g_lowQualityConfig.isLoaded = true;
			token = NextToken();
			g_lowQualityConfig.texture = atoi( token );
		} else if( !stricmp( token, "l_shadow" ) )
		{
			g_lowQualityConfig.isLoaded = true;
			token = NextToken();
			g_lowQualityConfig.shadow = atoi( token );
		} else if( !stricmp( token, "l_vision" ) )
		{
			g_lowQualityConfig.isLoaded = true;
			token = NextToken();
			g_lowQualityConfig.vision = atoi( token );
		} else if( !stricmp( token, "l_fog" ) )
		{
			g_lowQualityConfig.isLoaded = true;
			token = NextToken();
			g_lowQualityConfig.fog = atoi( token );
		} else if( !stricmp( token, "l_window" ) )
		{
			g_lowQualityConfig.isLoaded = true;
			token = NextToken();
			g_lowQualityConfig.window = atoi( token );
		}
		
		else if( !stricmp( token, "h_resolution" ) )
		{
			g_highQualityConfig.isLoaded = true;
			token = NextToken();
			g_highQualityConfig.resoution = atoi( token );
		} else if( !stricmp( token, "h_texture" ) )
		{
			g_highQualityConfig.isLoaded = true;
			token = NextToken();
			g_highQualityConfig.texture = atoi( token );
		} else if( !stricmp( token, "h_shadow" ) )
		{
			g_highQualityConfig.isLoaded = true;
			token = NextToken();
			g_highQualityConfig.shadow = atoi( token );
		} else if( !stricmp( token, "h_vision" ) )
		{
			g_highQualityConfig.isLoaded = true;
			token = NextToken();
			g_highQualityConfig.vision = atoi( token );
		} else if( !stricmp( token, "h_fog" ) )
		{
			g_highQualityConfig.isLoaded = true;
			token = NextToken();
			g_highQualityConfig.fog = atoi( token );
		} else if( !stricmp( token, "h_window" ) )
		{
			g_highQualityConfig.isLoaded = true;
			token = NextToken();
			g_highQualityConfig.window = atoi( token );
		}
		
		else if( !stricmp( token, "u_resolution" ) )
		{
			g_userConfig.isLoaded = true;
			token = NextToken();
			g_userConfig.resoution = atoi( token );
		} else if( !stricmp( token, "u_texture" ) )
		{
			g_userConfig.isLoaded = true;
			token = NextToken();
			g_userConfig.texture = atoi( token );
		} else if( !stricmp( token, "u_shadow" ) )
		{
			g_userConfig.isLoaded = true;
			token = NextToken();
			g_userConfig.shadow = atoi( token );
		} else if( !stricmp( token, "u_vision" ) )
		{
			g_userConfig.isLoaded = true;
			token = NextToken();
			g_userConfig.vision = atoi( token );
		} else if( !stricmp( token, "u_fog" ) )
		{
			g_userConfig.isLoaded = true;
			token = NextToken();
			g_userConfig.fog = atoi( token );
		} else if( !stricmp( token, "u_window" ) )
		{
			g_userConfig.isLoaded = true;
			token = NextToken();
			g_userConfig.window = atoi( token );
		}
	}

	EndTokenScript();

	return 1;
}

void GTH_Setup_SaveConfig( char* filename )
{
	FILE *fp;
	fp = fopen( filename, "w" );
	
	fprintf( fp, "// gth client config\n" );
	fprintf( fp, "// resolution\n" );
	fprintf( fp, "// 0 : 1024x768x16, 1 : 1280x1024x16\n" );
	fprintf( fp, "// texture\n" );
	fprintf( fp, "// 0 : high, 1 : low\n" );
	fprintf( fp, "// shadow\n" );
	fprintf( fp, "// 0 : on, 1 : off\n" );
	fprintf( fp, "// vision\n" );
	fprintf( fp, "// 0 : high, 1 : low\n" );
	fprintf( fp, "// fog\n" );
	fprintf( fp, "// 0 : on, 1 : off\n" );
	fprintf( fp, "// window\n" );
	fprintf( fp, "// 0 : on, 1 : off\n" );
	fprintf( fp, "\n\n" );
	fprintf( fp, "// auto config low quality\n" );
	fprintf( fp, "l_resolution\t\t%d\n", g_lowQualityConfig.resoution );
	fprintf( fp, "l_texture\t\t%d\n", g_lowQualityConfig.texture );
	fprintf( fp, "l_shadow\t\t%d\n", g_lowQualityConfig.shadow );
	fprintf( fp, "l_vision\t\t%d\n", g_lowQualityConfig.vision );
	fprintf( fp, "l_fog\t\t%d\n\n\n", g_lowQualityConfig.fog );
	fprintf( fp, "l_window\t\t%d\n", g_lowQualityConfig.window );

	fprintf( fp, "// auto config high quality\n" );
	fprintf( fp, "h_resolution\t\t%d\n", g_highQualityConfig.resoution );
	fprintf( fp, "h_texture\t\t%d\n", g_highQualityConfig.texture );
	fprintf( fp, "h_shadow\t\t%d\n", g_highQualityConfig.shadow );
	fprintf( fp, "h_vision\t\t%d\n", g_highQualityConfig.vision );
	fprintf( fp, "h_fog\t\t%d\n\n\n", g_highQualityConfig.fog );
	fprintf( fp, "h_window\t\t%d\n\n\n", g_highQualityConfig.window );
	
	fprintf( fp, "// user config\n" );
	fprintf( fp, "u_resolution\t\t%d\n", g_userConfig.resoution );
	fprintf( fp, "u_texture\t\t%d\n", g_userConfig.texture );
	fprintf( fp, "u_shadow\t\t%d\n", g_userConfig.shadow );
	fprintf( fp, "u_vision\t\t%d\n", g_userConfig.vision );
	fprintf( fp, "u_fog\t\t%d\n", g_userConfig.fog );
	fprintf( fp, "u_window\t\t%d\n\n\n", g_userConfig.window );
	
	fclose( fp );
}
