int GTH_SpawnMonster();
void GTH_ExecuteMonsterEvent();
void GTH_Monster_Idle(monsterCharacter_t *m);
void GTH_Monster_Walk(monsterCharacter_t *m);
void GTH_Monster_Back(monsterCharacter_t *m);
void GTH_Monster_Run(monsterCharacter_t *m);
void GTH_Monster_TurnLeft(monsterCharacter_t *m);
void GTH_Monster_TurnRight(monsterCharacter_t *m);
void GTH_Monster_Attack(monsterCharacter_t *m);
void GTH_Monster_AutoRecovery( monsterCharacter_t *m );

void GTH_Monster_Damage( monsterCharacter_t* monster, message_t* message );



int FindRewardField( int index );
void GTH_ProcessQuestScrollExecute( monsterCharacter_t *monster, playerCharacter_t *pc );

void GTH_ProcessAutoQuestScrollExecute( monsterCharacter_t *monster, playerCharacter_t *pc);


void GTH_RewordAutoQuestScroll(playerCharacter_t* pPlayer,const int& nQuestArrayIndex);



int AI_NPC_AutoCompleteQuest(playerCharacter_t *pPc, int *pParam );

void GTH_Monster_Loot( monsterCharacter_t *monster );
void GTH_Monster_Die(monsterCharacter_t *monster );


void GTH_Monster_Skill(monsterCharacter_t *monster, int skillIndex);
void GTH_Monster_SkillCasting( monsterCharacter_t *monster );


void GTH_Monster_State(monsterCharacter_t* m);

void Monster_CalculateValue( monsterCharacter_t* monster );


int LoadMonsterSpawnTableForServer( char *filename, worldTable_t* wt );
int GTH_LoadMonsterSpawnTable(worldTable_t *worldTable);
int GTH_LoadMonsterTable( char* filename );
int GTH_LoadMonster();
int GTH_RespawnMonster(monsterCharacter_t *monster);






void MONSTER_CalculateByAffect();


void MONSTER_CalculateBySkill();

void MONSTER_CalcualteHitRate();

void MONSTER_CalculateAvoidRate();

void MONSTER_Calculate( monsterCharacter_t* monster );


void MONSTER_Initialize( monsterCharacter_t* monster, int worldIdx, int spawnIdx );
void MONSTER_Delete( monsterCharacter_t* monster );
void MONSTER_Free();

void MONSTER_Respawn( monsterCharacter_t* monster );
void MONSTER_Spawn( monsterCharacter_t* monster );


int MONSTER_GetNotActiveIdx();
int MONSTER_Summon( int monsterTableIdx, int worldIdx, vec3_t position, int aiType, entityType_t entityType = ENTITY_MONSTER, int summonerIdx = -1, int summonerType = -1, float validTime  = -1.0f);


int		GTH_ProcessMessage_SummonMonster(item_t* pItem);

void	SetMonsterOptionValue(monsterCharacter_t* monster);


void	SetMonsterTable(worldTable_t *pWorld, int worldindex);
bool	CheckSameMonsterIndex(int monsterIndex, worldTable_t* pWorld);



bool	CanSummonMonster(int monsterIndex);



int		FindMonsterSpawnPlace(int localWorldIdx, vec3_t& position );


int		AddMonster_ContributionPoint( playerCharacter_t *pPc, int monsterIdx,  int PointType,  int point);


void GTH_Process_QuestlimiteTimerCheck();



extern int g_monsterSpawnCount;