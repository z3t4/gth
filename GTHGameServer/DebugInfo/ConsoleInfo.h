
void CreateConsoleInfo();
DWORD WINAPI ConsoleInfoThread(LPVOID param);
void FreeConsoleInfo();
int ConsoleInfoProcess( char* token );

class CGTHConsoleInfo
{
public :

	int Process( char* token );

	
	void PartyOutput( serverPartyInfo_t *p );
	void Party( char* token );
	
	void CheckDupUser();
	
	void ViewPCList( int type );
	
	void ReloadTable();
	
	void RespawnAllMonster();
};