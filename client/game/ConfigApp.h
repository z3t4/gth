


typedef struct
{

	int		isLoaded;
	int		resoution;
	int		texture;
	int		shadow;
	int		vision;
	int		fog;
	int		window;

} gthconfig_t;

int GTH_Setup_LoadConfig( char* filename );
void GTH_Setup_SaveConfig( char* filename );



extern gthconfig_t		g_lowQualityConfig;
extern gthconfig_t		g_highQualityConfig;
extern gthconfig_t		g_userConfig;
