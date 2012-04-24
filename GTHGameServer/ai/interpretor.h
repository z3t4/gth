#ifndef _AI_INTERPRETOR_H_
#define _AI_INTERPRETOR_H_



#define MAX_SCRIPT_VAL_LEN		50



int AI_InterpretorFunction( monsterCharacter_t* monster );

int AI_InterpretorDefine( monsterCharacter_t* monster, int paramAddr );




int AI_LoadScript( monsterCharacter_t* monster, char* filename );

char AI_GetCharData( monsterCharacter_t* monster );

int AI_GetIntData( monsterCharacter_t* monster );

int AI_Interpretor( monsterCharacter_t* monster );


void AI_InterpretorCall( monsterCharacter_t* monster );

void AI_InterpretorOperation( monsterCharacter_t* monster );

void AI_InterpretorParameter( monsterCharacter_t* monster, int param[], int n );

int AI_InterpretorCase( monsterCharacter_t* monster );

void AI_InterpretorThen( monsterCharacter_t* monster, int addrInfo[] );

void AI_InterpretorElse( monsterCharacter_t* monster, int addrInfo[] );

void AI_InterpretorIF( monsterCharacter_t* monster );

void AI_InterpretImmediately( monsterCharacter_t* monster );
#endif