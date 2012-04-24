#define MAX_MASTER_SERVER		3

typedef struct
{
	int					idx;		
	int					active;

	char				hostName[NAMESTRING];
	char				serviceIP[IPSTRING];
	int					servicePort;
	char				localIP[IPSTRING];
	int					localPort;

	char				name[NAMESTRING]; 
	int					worldNumber;			
	int					worldList[MAX_SERVICE_WORLD]; 
	int					clientNumber;
	int					maxtClientNumber;
} memberServerInfo_t;

typedef struct
{
	int					idx;	
	int					active;

	char				name[NAMESTRING]; 
	int					concurrentClientNumber;			
	int					maxConcurrentClientNumber;

	char 				DSN[NAMESTRING];

	int					memberServerNumber;
	memberServerInfo_t	memberServer[MAX_MEMBER_SERVER];
} serverGroupInfo_t;


typedef struct 
{
	int					idx;						
	int					active;

	char				hostName[NAMESTRING];
	char				serviceIP[IPSTRING];
	int					servicePort;
	char				localIP[IPSTRING];
	int					localPort;

	char 				DSN[NAMESTRING];

	char				version[VERSIONSTRING];
	int					filterYn; 
	int					clientNumber;
	int					maxClientNumber;

	int					concurrentClientNumber;			
	int					maxConcurrentClientNumber;
} masterServerInfo_t;
