
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int GTH_ParseGameServerConfig();
int GTH_ParseGameLogicConfig();
int GTH_LoadGameWorld();
void GTH_UpdateFrameRate();
void GTH_UpdateTimer();
void GTH_InitGlobalTime();
void GTH_UpdateServerGroupInfo();
int LoadAllGameTable();


