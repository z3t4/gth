

#define NUM_OF_MONSTER_AI_TYPE		14
#define NUM_OF_NPC_AI_TYPE			19

struct node_t
{
	grid_t				pos;					
	struct node_t		*parent;				
	int					cost;					
	int					total;					

	bool				onOpen;					
	bool				onClosed;				
};


struct nodeHash_t
{
	node_t				*node;
};


struct queue_t
{
	std::vector<node_t*>	heap;
};


struct path_t
{
	grid_t					goal;				
	queue_t					open;				
};


#define MAX_NUMBER_OF_SCRIPT_VARIABLE			100		
#define MAX_NUMBER_OF_SCRIPT_PROCEDURE			100		


#define PATH_FINDING_SIZE		30						


#define	MAX_PATH_BUFFER_SIZE					PATH_FINDING_SIZE * PATH_FINDING_SIZE

#define MAX_NUMBER_OF_NODE						PATH_FINDING_SIZE * PATH_FINDING_SIZE


#define	MAX_MAP_BUFFER_SIZE						PATH_FINDING_SIZE * PATH_FINDING_SIZE * 3


#define MAX_NUMBER_OF_RANDOM_POSITION			20
#define MAX_NUMBER_OF_WAYPOINT					20



enum monsterAIEvent_t
{
	AI_IDLE,
	AI_MOVE,
	AI_TRACE,
	AI_ATTACK,
	AI_RUNAWAY,
	AI_RECOVERY,
	AI_LOOK,
	AI_LOST,
	AI_ATTACH,
	AI_DIE,
	AI_STUN,
	
	AI_SKILL,
};




typedef struct
{
	int				aiType;
	
	monsterAIEvent_t	aiEvent;
	monsterAIEvent_t	OldAiEvent;		
	

	
	unsigned int	saveTime;					
	int				thinkTime;					
	char            *aiData;					
	int				aiAddr;						
	
	
	int				aiVariable[MAX_NUMBER_OF_SCRIPT_VARIABLE];
	
	int				aiProcedureAddr[MAX_NUMBER_OF_SCRIPT_PROCEDURE];
	
	
	
	int				callProcedureAddr;
	
	bool			isCall;


	
	
	int				pathFindSize;

	grid_t			startPos;						
	grid_t			curPos;							

	grid_t			*path;

	int				pathNumber;						

	grid_t			pastPos;


	node_t			*nodeBank;

	

	node_t**		nodeHashTable;

	




	int				nodeBankNumber;					
	int				searchNumber;					
	int				heruristic;						
	int				totalSearch;					

	
	
	bool			isGoal;
	
	grid_t			goalPos;
	
	path_t			pathQueue;

	
	vec3_t			wayPoint[MAX_NUMBER_OF_WAYPOINT];	
	int				wayPointNumber;						

	

	grid_t			*wayPath;		
	
	int				wayPathNumber;						
	int				curWayPath;							
	int				directWayPath;						

	
	int				attachFlag;
	int				returnFlag;

	
	int				moveType;
	int				oldMoveType;
	int				lostPathCount;

} ai_t;





#define AI_DEF_VERYSLOW_TIMER			4000
#define AI_DEF_SLOW_TIMER				2000
#define AI_DEF_NORMAL_TIMER				2000
#define AI_DEF_FAST_TIMER				1000
#define AI_DEF_VERYFAST_TIMER			500

#define AI_DEF_DAMAGED_ENTITY			0
#define AI_DEF_NEAR_ENTITY				1

#define AI_DEF_LESS_RA_ENTITY			2
#define AI_DEF_MORE_POINT_ENTITY		3



#define AI_DEF_RANDOM					0

#define AI_DEF_NEAR_FELLOW				0
#define	AI_DEF_MY_RA					0

#define	AI_DEF_MY_SA					1

#define AI_DEF_ENABLE_WALK				0
#define	AI_DEF_DISABLE_MOVE				1
#define	AI_DEF_DISABLE_WALK				2

#define	AI_DEF_ENTITY_PC				0
#define	AI_DEF_ENTITY_MONSTER			1
#define	AI_DEF_ENTITY_NPC				2

#define AI_BUFFER_SIZE					8000



enum npcAIEvent_t
{
	AI_NPC_WAIT,
	AI_NPC_RUN
};


typedef struct
{

	int				aiType;
	char            aiData[AI_BUFFER_SIZE];					

	
	npcAIEvent_t	aiEvent;

	
	unsigned int	saveTime;					
	int				thinkTime;					
	int				aiAddr;						

	
	int				aiVariable[MAX_NUMBER_OF_SCRIPT_VARIABLE];
	
	int				aiProcedureAddr[MAX_NUMBER_OF_SCRIPT_PROCEDURE];
	
	
	
	int				callProcedureAddr;
	
	bool			isCall;

} npcAi_t;

typedef struct
{
	int				aiType;
	char            aiData[AI_BUFFER_SIZE];					

	
	npcAIEvent_t	aiEvent;

	
	unsigned int	saveTime;					
	int				thinkTime;					
	int				aiAddr;						

	
	int				aiVariable[MAX_NUMBER_OF_SCRIPT_VARIABLE];
	
	int				aiProcedureAddr[MAX_NUMBER_OF_SCRIPT_PROCEDURE];
	
	
	
	int				callProcedureAddr;
	
	bool			isCall;


	int				msgWaiting;

	int				pcReplyIdx;

	int				msgFlag;
	int				sendMsgIdx;
	int				replyMsgIdxNumber;
	int				replyMsgIdx[5];

	
	int				globalData[5];

} npcAiInPc_t;
