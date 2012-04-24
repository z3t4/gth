typedef struct
{
	int active;
	unsigned int setTime;
	unsigned int setSecond;
	int countdown;
	int isComplete;
} serverShutdownTimer_t;

typedef struct
{
	int				active;
	unsigned int	setTime;
	int				setRepeatCount;
	int				setTerm;
	unsigned int	nextTime;
	int				curRepeatCount;
	int				curPC;
	char			notice[256];
} noticeInfo_t;

#define SERVER_SHUTDOWN_DELAYTIME		10000	

void GTH_SystemAdmin_ServerShutdown();
void GTH_SystemAdmin_SetShutDownTimer();
void GTH_Init_ServerShutdownTimer();
void GTH_Process_ServerShutdownTimer();

void GTH_SystemAdmin_GoToPlayer(playerCharacter_t *pc);
void GTH_SystemAdmin_GoToMonster(playerCharacter_t *pc);

void GTH_SystemAdmin_Notice();

void GTH_SystemAdmin_SetNoticeInfo( char *notice );


void GTH_Init_ServerNoticeInfo();
void GTH_Process_NoticeInfo();
int	GTH_CheckValidation_GameMaster( playerCharacter_t *pc );


void GTH_SystemAdmin_RequestFindPC();
void GTH_SystemAdmin_ReplyFindPC();

void GTH_SystemAdmin_GMBusyOnOff(playerCharacter_t *pc);
void GTH_SystemAdmin_GMHideOnOff(playerCharacter_t *pc);
void GTH_SystemAdmin_SummonMonster( playerCharacter_t *pc );






