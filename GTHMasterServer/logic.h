int GTH_LoadCharacterTable();
int GTH_CheckValidationCharacterID( i3client_t *client, int characterID );
void GTH_SetCharacterData(characterData_t *character, int characterID, charEditData_t *editData);
void GTH_InitAllCharacterData(i3client_t *client);
void GTH_InitCharacterData(characterData_t *character);

void GTH_InitCharacterTable( characterTable_t *characterTable );

void GTH_CheckGameData( i3client_t *client );
int GTH_FindEmptyInventoryPos(characterData_t *data);

