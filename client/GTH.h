




#ifndef AFX_GTH_H__E64DF2E8_A208_4C1A_8346_70BBA1960703__INCLUDED_
#define AFX_GTH_H__E64DF2E8_A208_4C1A_8346_70BBA1960703__INCLUDED_








#define DXAPP_KEY        TEXT("Software\\GTH\\GTH")

#define SIZE_MOUSE_ROTATE 10.0f

#define MAX_SIZE_WORLD_BSP 32
#define MAX_SIZE_CHARACTER_NAME 64


#include "Guild\CGuildCtrl.h"




 
typedef enum
{
	GTH_RES_800_600	=	0,
	GTH_RES_1024_768,
	GTH_RES_1280_1024,
	GTH_RES_1600_1200,
}	GTH_RES;


#define NONE_MASTERSERVER_CONNECT	1

typedef struct
{
	char    name[ MAX_SIZE_CHARACTER_NAME ];
	int     charID;
	float   height;
	vec3_t  mins;
	vec3_t  maxs;
	vec3_t  position;
	vec3_t  angles;   

	movetype_t  moveType;
	float       moveSpeed;

	vec3_t  forward , right , up;

	void GetFwdVector( vec3_t vec )
	{
		VectorSet( vec , forward[0] , forward[1] , forward[2] );
		
	}
	void GetRightVector( vec3_t vec )
	{
		VectorSet( vec , right[0] , right[1] , right[2] );
		
	}
    void GetUpVector( vec3_t vec )
	{
		VectorSet( vec , up[0] , up[1] , up[2] );
		
	}
	
	void SetupVectors() { AngleVectors( angles , forward , right , up ); }
} characterstate_t;


#define GTH_MAX_SIZE_MAP_SPAWN_POS 10
 
#define GTH_MAX_SIZE_MONSTER_KIND	50
	

typedef struct
{
	char    mapName   [ GTH_MAX_SIZE_FILENAME ];
	char    objectName[ GTH_MAX_SIZE_FILENAME ];
	char    modelName [ GTH_MAX_SIZE_FILENAME ];
	char    itemName  [ GTH_MAX_SIZE_FILENAME ];
	char    effectName[ GTH_MAX_SIZE_FILENAME ];
	char    BGMusic   [ GTH_MAX_SIZE_FILENAME ];
	int     idxWorldBSP;
	vec3_t  spawnPos[ GTH_MAX_SIZE_MAP_SPAWN_POS ];
	float   spawnAngle[ GTH_MAX_SIZE_MAP_SPAWN_POS ];
	int     numSpawnPos;
	int     numSpawnAngles;
	vec3_t  sunAngle;
	vec3_t  sunColor;
	float   worldVisible;
	float   modelVisible;
	float   itemVisible;
	float   effectVisible;
	DWORD   fogColor;
	float   fogStart;
	float   fogEnd;
	 
	int		monsterKind;		 
	unsigned short	monsters [ GTH_MAX_SIZE_MONSTER_KIND ];
	 
	int		bgVolume;
	int		effectVolume;
		
	 
	TIMESTATE	weather;
		
	
} mapinfo_t;







class GTH : public CD3DApplication
{
public:
	

	 
	BOOL		m_renderEnabled;		

    BOOL		m_bLoadingApp;          
    ID3DXFont	*m_pD3DXFont;            

	CBSP		*m_worldBSPs[ MAX_SIZE_WORLD_BSP ];
	int			m_numWorldBSPs;

	CMDLMng		*m_MDLMng;

	mapinfo_t	m_mapInfos[ MAX_SIZE_WORLD_BSP ];
	int			m_numMapInfos;

	int			m_currWorld;

	DWORD		m_frameFlag;

	
	bool		m_IMEReady;

	

	
	Fx_CHARACTER_t *m_myCharacter;


   
	INT     GTH::Run();

protected:

	VOID     ReadSettings();
    VOID     WriteSettings();
	HRESULT  ConfirmDevice( D3DCAPS8*, DWORD, D3DFORMAT );
    HRESULT  RenderText();
	HRESULT  InvalidateDeviceObjects();
    HRESULT  DeleteDeviceObjects();
    HRESULT  FinalCleanup();

	
	
	void	GTH::SetMouseEvent(vec3_t	touched);

	
public:

    GTH();
	~GTH();

	int  GTH::InitEngine( HINSTANCE hInst );
	
	int  GTH::InitIME();
	
	void WritePackFileLog( char* str, ... );
	void GTH::SetMyCharacter();
	

	 
	void	GTH::ToggleFullScreen	();
	int		GTH::ChangeResolution	(	int		in_mode		);
	
	
	static void	MovePatchProgram();
	
	

protected:

	int      GTH::PrepareApp();
	int      GTH::InitApp();
	void     GTH::InitGlobalClass();
	int      GTH::RestoreApp();
	int      GTH::LoadShaders();
	int      GTH::LoadMap( char *pMap );
	int      GTH::LoadMDLClasses( mapinfo_t *mapInfo );
	int      GTH::LoadEntity( char *objectfName );

	int		 GTH::ChangeClientState( int state );
	void	 GTH::InitCharacter();

	void     GTH::ApplyTimeEnv();
	HRESULT  GTH::BeginScene();
	void     GTH::EndScene();
	HRESULT  GTH::InitDeviceObjects();
	HRESULT  GTH::OneTimeSceneInit();
	HRESULT  GTH::AppFrame();
	int      GTH::AppProcess();
	void     GTH::SetWorldTransform();
	void     GTH::WorldFrame();
	void     GTH::EntityFrame();
	void     GTH::CharacterFrame();
	void     GTH::CursorFrame();
	void     GTH::WorldRenderBefore();
	void     GTH::WorldRenderAfter();

	LRESULT  GTH::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	HRESULT  GTH::RestoreDeviceObjects();

	void     GTH::SetupCamera();
	void     GTH::SetupCursor();
	void	 GTH::SetupCursor_Neo();

	HRESULT  GTH::FreeMove();
	HRESULT  GTH::FrameMove();
	void     GTH::GetKeyState();

	int GTH::LoadConfig( char *fname );

	void GTH::OutputClientState();
	
	int GTH::ChangeStartProcessBefore();
	int GTH::ChangeStartProcessAfter();
	int GTH::RestoreStartProcess();
	int GTH::InitStartProcess();
	int GTH::StartProcessFrame();
	int	GTH::StartProcess();

	
	int GTH::ChangeLoginProcessBefore();
	int GTH::ChangeLoginProcessAfter();
	int GTH::RestoreLoginProcess();
	int GTH::InitLoginProcess();
	int GTH::LoginProcessFrame();
	int	GTH::LoginProcess();

	
	int GTH::ChangeCharProcessBefore();
	int GTH::ChangeCharProcessAfter();
	int GTH::RestoreCharProcess();
	int GTH::InitCharProcess();
	int GTH::CharProcessFrame();
	int	GTH::CharProcess();

	
	int GTH::ChangeLoadingProcessBefore();
	int GTH::ChangeLoadingProcessAfter();
	int GTH::RestoreLoadingProcess();
	int GTH::InitLoadingProcess();
	int GTH::LoadingProcessFrame();
	int	GTH::LoadingProcess();

	
	int GTH::ChangeGameProcessBefore();
	int GTH::ChangeGameProcessAfter();
	int GTH::RestoreGameProcess();
	int GTH::InitGameProcess();
	int GTH::GameProcessFrame();
	int	GTH::GameProcess();

public:
	
	int GTH::SetupKeys();
	int GTH::SetupView();
	int GTH::SetupViewWithKey();
};


#endif 
