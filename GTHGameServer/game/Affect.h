
void GTH_DeleteAffectAll( void *ent, int entType );
void GTH_DeleteAffect( void *ent, int entType, int skillTableIdx );
void GTH_DeleteTransform( void *ent, int entType = ENTITY_PC );
void GTH_DeleteToggle( void *ent, int entType, int skillTableIdx );
void GTH_AffectTransform( void *ent, int transformIdx, int entType = ENTITY_PC );
void GTH_Affect( void *ent, message_t *msg, int entType = ENTITY_PC );
void GTH_AffectProcess( void *ent, int entType = ENTITY_PC );
void GTH_ApplyCommonAffect( void *ent, AFFECTDATA_COMMON *data, int entType = ENTITY_PC );
void GTH_ApplySpecialAffect( void *ent, AFFECTDATA_SPECIAL *spe, int entType = ENTITY_PC );




void GTH_ApplyAffect( void *ent, int entType = ENTITY_PC );
int GTH_MakeAffectTableIdx( void *ent, int affectIdx[], int entType = ENTITY_PC );
int GTH_MakeAffectTime( void *ent, int affectTime[], int entType );
int GTH_GetEntityCurseState( int entType, int entIdx, int curseType );
int GTH_RemoveEntityCurseState( int entType, int entIdx, int curseType );


bool GTH_IsActiveAffect( void *ent, int entType, int skillTableIdx );




