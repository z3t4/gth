enum
{
	GTH_SOUND_ID_BUTTON_CLICK_1			=	0,
	GTH_SOUND_ID_BUTTON_CLICK_2,
	GTH_SOUND_ID_GEN_CLICK,
	GTH_SOUND_ID_CHAR_CLICK,
	GTH_SOUND_ID_OPEN_WINDOW,
	GTH_SOUND_ID_CLOSE_WINDOW,
	GTH_SOUND_ID_LOGIN_WINDOW,
	GTH_SOUND_ID_LOGIN_WINDOW_2,
	GTH_SOUND_ID_ITEM_CLICK_1,
	GTH_SOUND_ID_ITEM_CLICK_2,
	GTH_SOUND_ID_DROP_ITEM,
	GTH_SOUND_ID_PICKUP_ITEM,
	GTH_SOUND_ID_EQUIP_ITEM,
	GTH_SOUND_ID_WARNING_1,
	GTH_SOUND_ID_WARNING_2,
	GTH_SOUND_ID_NOTICE,
	GTH_SOUND_ID_DISAPPEAR_MON,
	GTH_SOUND_ID_REPAIRWARNING,
	GTH_SOUND_ID_LOGIN_WINDOW3,
	GTH_SOUND_ID_LOGIN_BUTTONSHOW,
	GTH_SOUND_ID_LOGIN_ITEMABSORB,
	GTH_SOUND_ID_GAMELOGIN_OK,
	GTH_SOUND_ID_REPAIR,
	GTH_SOUND_ID_SPAWNFIELD_MON,
	GTH_SOUND_ID_SPAWNDUN_MON,
	GTH_SOUND_ID_SPAWN_ITEM,
	GTH_SOUND_ID_PC_HEARTBEAT_0,
	GTH_SOUND_ID_PC_HEARTBEAT_1,
	GTH_SOUND_ID_PC_HEARTBEAT_2,
	GTH_SOUND_ID_PC_RECOVERY_RA,
	GTH_SOUND_ID_PC_RECOVERY_SA,
	
	GTH_SOUND_ID_DISAPPEAR_MON2,
	GTH_SOUND_ID_LEVELUP,
	GTH_SOUND_ID_CHARSPAWN,

	
	GTH_SOUND_ID_QUESTSCROLL_DROP,
	GTH_SOUND_ID_QUESTSCROLL_WRITING,
	
	GTH_SOUND_ID_BATTLE_COUNT,	
	GTH_SOUND_ID_CONG,			
	GTH_SOUND_ID_BATTLE_FAIL,	
	GTH_SOUND_ID_BATTLE_FIGHT,	
	GTH_SOUND_ID_BATTLE_READY,	
	GTH_SOUND_ID_BATTLE_SUCCESS,
	GTH_SOUND_ID_BATTLE_READYFIGHT,
};


#define MAX_COUNT_GTH_PC_SOUND			8			
#define MAX_COUNT_GTH_MONSTER_SOUND		400			
#define	MAX_COUNT_GTH_ITEM_SOUND		1000		


#define	MAX_COUNT_GTH_MDL_SOUND_ID		600											




typedef struct
{
	int			idx;
	char		filename[128];
	int			numDup;
	int			is3D;

	int			soundID;

} gthSoundID_t;

typedef struct
{
	gthSoundID_t		normalID[128];
	int					numNoramlID;
	
	gthSoundID_t		itemID[128];
	int					numItemID;

	gthSoundID_t		eventID[MAX_ANIMCOUNT_TYPE][128];
	int					numEventID[MAX_ANIMCOUNT_TYPE];

} gthSoundResource_t;

 
typedef struct
{
	int			idx;
	char		mdlname [255];
	char		soundname [255];
} mdlSoundID_t;
	


typedef struct
{
	int		idx;						
	int		soundID[MAX_ANIMCOUNT_TYPE];

	float	playTime[MAX_ANIMCOUNT_TYPE];
	int		frequency[MAX_ANIMCOUNT_TYPE];
	int		playCount[MAX_ANIMCOUNT_TYPE];
} monsterSound_t;


typedef struct
{
	
	int		idx;						
	int		soundID[MAX_ANIMCOUNT_TYPE];

	float	playTime[MAX_ANIMCOUNT_TYPE];
	int		frequency[MAX_ANIMCOUNT_TYPE];
	int		playCount[MAX_ANIMCOUNT_TYPE];
} pcSound_t;


typedef struct
{
	int		idx;						
	int		soundID;
	float	playTime;
} itemSound_t;


typedef struct
{
	int		idx;						
	int		soundID;
} skillSound_t;

class CSoundMng
{
public:	

	gthSoundResource_t	m_soundResource;

	 
	int					m_maxIdx;
	int					m_nummdlSoundID;
	mdlSoundID_t		m_mdlSoundID [MAX_COUNT_GTH_MDL_SOUND_ID];
		

	monsterSound_t		m_monSound[MAX_COUNT_GTH_MONSTER_SOUND];
	pcSound_t			m_pcSound[MAX_COUNT_GTH_PC_SOUND];
	itemSound_t			m_itemSound[MAX_COUNT_GTH_ITEM_SOUND];

	CSoundMng();
	~CSoundMng();

	void		LoadSoundConfig( char* filename );
	 
	void		LoadMDLSoundConfig	(	char	*in_filename	);
		
	void		Initialize();
	void		Play( Fx_CHARACTER_t* character );
	void		PlayMonsterSound( Fx_CHARACTER_t* character );
	void		PlayCharSound( Fx_CHARACTER_t* character );
	void		PlayDamageSound( Fx_CHARACTER_t* character );

	int			GetNormalSoundID( int idx );
	int			GetItemSoundID( int idx );
	int			GetEventSoundID( int event, int idx );
};




inline void  SetSample( char *name , int maxBuffers , int idxSample , int is3D );
void         ConfigGameSamples();
 
void		 ConfigMDLSamples();
int GetGameSampleID( char *path );
