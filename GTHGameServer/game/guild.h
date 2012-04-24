





#ifndef GUILD_H
#define GUILD_H
#ifdef _WIN32
#pragma once
#endif



#define MAX_GUILD_MASTERS						1		
#define MAX_GUILD_SUBMASTERS					4		
#define MAX_GUILD_FACTIONMASTERS				8		
#define MAX_GUILD_MATES							50		
#define GUILDMARKSIZE							768		


#define GUILD_ORGANIZABLE_NAK					4000000	
#define GUILD_ORGANIZABLE_LEVEL					30		
#define GUILD_ORGANIZABLE_RANKPOINT				100		
#define GUILD_SUBSCRIBABLE_LEVEL				10		

enum
{
	GUILD_AUTHORITY_ADDUSER = 1,
	GUILD_AUTHORITY_REMOVEUSER = 2,

};

enum
{
	GUILD_MASTER = 1,
	GUILD_SUBMASTER = 2,
	GUILD_FACTIONMASTER = 4,
	GUILD_MATE = 8,
	GUILD_ALLRANK = 15,

};

enum
{
	GUILD_ERROR_SAMEGUILDNAME		= 0,
	GUILD_ERROR_ALREADYJOINED,
	GUILD_ERROR_NAK,
	GUILD_ERROR_LEVEL,
	GUILD_ERROR_RANKPOINT,
	GUILD_ERROR_NOTJOINED,
	GUILD_ERROR_NOMASTER,
	GUILD_ERROR_NOGUILD,
	GUILD_ERROR_NOTEMPTY,
	GUILD_ERROR_CANNOTFIND,
	GUILD_ERROR_NOTAWAKENED,
	GUILD_ERROR_SAMEMATENAME,
	GUILD_ERROR_CANNOTAPPOINT,

	GUILD_ERROR_OFFLINE,			
	GUILD_ERROR_SAMEWORLD,			
	GUILD_ERROR_AUTHORITY,			

	GUILD_ERROR_INVITE_NONEPCINDEX,
	GUILD_ERROR_INVITE_NOTREADY,
	GUILD_ERROR_INVITE_DIFFWORLDINDEX,
	GUILD_ERROR_INVITE_PCSTATE,
	GUILD_ERROR_INVITE_ALREADY,
	GUILD_ERROR_INVITE_NOTLEVEL,
	GUILD_ERROR_INVITE_NOTMASTER,

	GUILD_ERROR_SECEDE_0,
	GUILD_ERROR_SECEDE_1,
	GUILD_ERROR_SECEDE_2,
	GUILD_ERROR_SECEDE_3,

	GUILD_ERROR_BANISH_0,
	GUILD_ERROR_BANISH_1,
	GUILD_ERROR_BANISH_2,
	GUILD_ERROR_BANISH_3,
	GUILD_ERROR_BANISH_4,
	GUILD_ERROR_BANISH_5,

	
	GUILD_ERROR_MAXOVERGUILDNUM,
	GUILD_ERROR_ALLOW_CREATE, 
	GUILD_ERROR_ALLOW_JOIN,
	

};


enum enumGuildRemoveType
{
	POWER_REMOVE,	
	GENERAL_REMOVE,	
};


#ifdef _DEV_TEST
	#define	DELAY_DAN_CREATE_SEC					120	
	#define	DELAY_DAN_REJOIN_SEC_FOR_GENERAL_REMOVE	60 
	#define	DELAY_DAN_REJOIN_SEC_FOR_POWER_REMOVE	30 
#else
	#define	DELAY_DAN_CREATE_SEC					432000	
	#define	DELAY_DAN_REJOIN_SEC_FOR_GENERAL_REMOVE	172800 
	#define	DELAY_DAN_REJOIN_SEC_FOR_POWER_REMOVE	86400 
#endif



#define	GUILD_IDX							0x00000001
#define	GUILD_SERVERGROUPID					0x00000002
#define	GUILD_NAME							0x00000004
#define	GUILD_MARK							0x00000008
#define	GUILD_MARKUPDATETIME				0x00000010
#define	GUILD_NOTICE						0x00000020
#define	GUILD_FAMEPOINT						0x00000040
#define	GUILD_EXPPOINT						0x00000080
#define	GUILD_RAIDPOINT						0x00000100
#define	GUILD_PVPPOINT						0x00000200
#define	GUILD_HUNTERPOINT					0x00000400
#define	GUILD_LEVEL							0x00000800
#define	GUILD_MASTERNAME					0x00001000
#define	GUILD_SUBMASTERNAME					0x00002000
#define	GUILD_FACTIONMASTERNAME				0x00004000

#define	GUILD_MATENAME						0x00008000
#define	GUILD_MATEAUTHORITY					0x00010000
#define	GUILD_MATELASTCONNECTTIME			0x00020000
#define	GUILD_MASTERNOTE					0x00040000

#define GUILD_USERLEVEL						0x00080000
#define GUILD_USERJOB						0x00100000
#define GUILD_USERLOCATION					0x00200000

#define	GUILD_ALL1							0x00007FFF
#define	GUILD_ALL2							0xFFFF8000
#define	GUILD_ALL							0xFFFFFFFF

#define	GUILD_UPDATE_ORGANIZE				0x00000000
#define	GUILD_UPDATE_DISORGANIZE			0x00000000
#define	GUILD_UPDATE_ADDUSER				( GUILD_USERLEVEL | GUILD_USERJOB | GUILD_USERLOCATION )
#define	GUILD_UPDATE_REMOVEUSER				( GUILD_USERLEVEL | GUILD_USERJOB | GUILD_USERLOCATION )



#define	GUILD_UPDATE_USER					( GUILD_USERLEVEL | GUILD_USERJOB | GUILD_USERLOCATION )
#define	GUILD_UPDATE_ALL					0xFFFFFFFF


#define GUILD_MASTER_AUTHORITY					( GUILD_AUTHORITY_ADDMATE | GUILD_AUTHORITY_DELETEMATE )
#define GUILD_SUBMASTER_AUTHORITY				( 0 )
#define GUILD_FACTIONMASTER_AUTHORITY			( 0 )
#define GUILD_MATE_AUTHORITY					( 0 )




typedef struct
{
	short			idx;														
	char			serverGroupID;												
	char			name[GUILDNAMESTRING+1];										
	char			mark[GUILDMARKSIZE];										
	unsigned int	markUpdateTime;												
	int				markChangeDate;												
	char			notice[GUILDNOTICESTRING];									
	int				famePoint;													
	int				expPoint;													
	int				raidPoint;													
	int				pvpPoint;													
	int				hunterPoint;												
	unsigned short	level;														

	
	int				nKillPoint;													
	int				wincount;
	int				Losecount;
	
	

	char			masterName[NAMESTRING+1];										
	char			subMasterName[MAX_GUILD_SUBMASTERS][NAMESTRING+1];			
	char			factionMasterName[MAX_GUILD_FACTIONMASTERS][NAMESTRING+1];	
	char			mateName[MAX_GUILD_MATES][NAMESTRING+1];						


	unsigned char	mateAuthority[MAX_GUILD_MATES];								
	unsigned int	mateLastConnectTime[MAX_GUILD_MATES];						
	char			masterNote[MAX_GUILD_MATES][MASTERNOTESTRING];				

	
	int				userLevel[MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES];
	int				userJob[MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES];
	int				userLocation[MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES];
} guild_t;

void GTH_InitGuildList();
void GTH_InitGuild( guild_t *guild );
int GTH_GetEmptyGuildIndex();


void GTH_GetGuildNameByIdx(const int idx, char* szString_OUT);
int GTH_GetGuildIndexByName( char* guildName );
int GTH_GetGuildEmptyIndex( short guildIdx, int rank, char *name );
int GTH_GetGuildMemberIndex( short guildIdx, int rank, char *name );
int GTH_GetMateIndexByName( short guildIdx, char *name );

short GTH_GuildOrganize( char *name, char* guildName, char* guildMark, int serverGroupID, int &error );
int GTH_GuildDisorganize( short guildIdx, char *name, int &error );

void GTH_GuildUpdate( short guildIdx, int type, int value1=GUILD_ALLRANK, int value2=-1, int rank=GUILD_ALLRANK, int rankIdx=-1 );

int GTH_GuildChangeMaster( short guildIdx, char *reqName, char *accName, int job, int &rankIdxInGuild, int &error );
int GTH_GuildAddSubMaster( short guildIdx, char *name, int job, int &rankIdxInGuild, int &error );
int GTH_GuildAddFactionMaster( short guildIdx, char *name, int job, int &rankIdxInGuild, int &error );
int GTH_GuildAddUser( short guildIdx, char *name, int job, int &rankIdxInGuild, int &error );
int GTH_GuildRemoveMaster( short guildIdx, char *name, int &error );
int GTH_GuildRemoveSubMaster( short guildIdx, char *name, int &error );
int GTH_GuildRemoveFactionMaster( short guildIdx, char *name, int &error );
int GTH_GuildRemoveUser( short guildIdx, char *name, int &error );

void GTH_GuildSetUserInfo( short guildIdx, char *name, int &rankInGuild, int &rankIdxInGuild );

void GTH_GuildUpdateUser( short guildIdx, char *name, int rank, int rankIdx, int type, int value );

void GTH_GuildChatMessage( short guildIdx, char *name, char *chatMessage );

void GTH_GuildUpdateDisorganize( short guildIdx, int result, int error );


playerCharacter_t* GTH_GetGuildNoneBusyMemberPtr(int GuildIdx, int WorldIdx, int MemberType);


#endif 