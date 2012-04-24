#define	MSG_DAMEGED			0
#define	MSG_AFFECT			1
#define MSG_ETC				100	

enum
{
	MSG_ETC_RESURRECTION,
	MSG_ETC_HELP,
	MSG_ETC_FORCED_CHANGE_TARGET,
};

int AI_InterpretMessage( monsterCharacter_t* monster, message_t* message );
int AI_InterpretMessage_Etc( monsterCharacter_t* monster, message_t* message );

int AI_InterpretMessage( playerCharacter_t* pc, message_t* message );
int AI_InterpretMessage_Etc( playerCharacter_t* pc, message_t* message );
