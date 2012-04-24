void MONSTER_InitAI( monsterCharacter_t* monster, int isLost = false );

int AI_CheckCorrectWay( monsterCharacter_t* monster, int wayType );
void MONSTER_Lost( monsterCharacter_t* monster );
void MONSTER_ForcedDelete( monsterCharacter_t* monster );

int MONSTER_AdjustNextAngle( monsterCharacter_t* monster );

void MONSTER_MovePath( monsterCharacter_t* monster );

void MONSTER_MoveWayPoint( monsterCharacter_t* monster );
void MONSTER_Move( monsterCharacter_t* monster );
void MONSTER_ExecuteAIEvent( monsterCharacter_t* monster );
void MONSTER_Attach( monsterCharacter_t* monster );