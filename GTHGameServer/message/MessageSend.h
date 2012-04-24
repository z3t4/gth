
enum
{
	MESSAGE_NORMAL_ATTACK	=		0,
	MESSAGE_USEINFO,
};


void AI_MakeMessage( playerCharacter_t* pc, message_t* message, int type, messageInfo_t* msginfo = NULL );
void AI_MakeAttackMessage( playerCharacter_t* pc, message_t* message  );
void AI_MakeMsgInfoAttack( playerCharacter_t *pc, message_t* message, messageInfo_t* msginfo );
void AI_MakeMsgInfoAffect( playerCharacter_t *pc, message_t* message, messageInfo_t* msginfo );


void AI_MakeMessage( monsterCharacter_t* monster, message_t* message, int type, messageInfo_t* msginfo = NULL );
void AI_MakeAttackMessage( monsterCharacter_t* monster, message_t* message  );


void AI_Make_Monster_MsgInfoAttack( monsterCharacter_t *pMonster, message_t* message, messageInfo_t* msginfo );
void AI_Make_Monster_MsgInfoAffect( monsterCharacter_t *pMonster, message_t* message, messageInfo_t* msginfo );
