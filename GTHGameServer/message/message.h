#ifndef _AI_MESSAGE_H_
#define _AI_MESSAGE_H_

#define		MON_MIN_RATTACK_RANGE		20
#define		SENDALL_RANGE				99999



int AI_SendMessageToPC( playerCharacter_t* pc, message_t* message );

int AI_SendMessageToMONSTER( monsterCharacter_t* monster, message_t* message );



void AI_SendMessage(message_t* message,CAISendMessage *AImsg =NULL);


void AI_ReceiveMessage( monsterCharacter_t* monster );
void AI_ReceiveMessage( playerCharacter_t* pc );

void AI_InitMessage( playerCharacter_t* pc );
void AI_InitMessage( monsterCharacter_t* monster );

#endif
