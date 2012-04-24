

















#include <windows.h>
#include <stdio.h>
#include <time.h>

#include <vector>
#include <string>

#include "Ahndbglog.h"








#if _MSC_VER > 1000
#pragma once
#endif 

#pragma warning(disable: 4786)

using namespace std;

#define gle				(GetLastError())

#define lenof(a)		(sizeof(a) / sizeof((a)[0]))

#define MAXNAMELEN		128  

#define IMGSYMLEN		(sizeof (IMAGEHLP_SYMBOL))

#define TTBUFLEN		512 





typedef BOOL (__stdcall *tSC)(IN HANDLE hProcess);


typedef PVOID (__stdcall *tSFTA)(HANDLE hProcess, DWORD AddrBase);


typedef BOOL (__stdcall *tSGLFA)(IN  HANDLE hProcess,        IN  DWORD dwAddr,
								 OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE Line);


typedef DWORD (__stdcall *tSGMB)(IN HANDLE hProcess, IN DWORD dwAddr);


typedef BOOL (__stdcall *tSGMI)(IN HANDLE hProcess, IN DWORD dwAddr, OUT PIMAGEHLP_MODULE ModuleInfo);


typedef DWORD (__stdcall *tSGO)(VOID);


typedef BOOL (__stdcall *tSGSFA)(IN  HANDLE hProcess,        IN  DWORD dwAddr,
								 OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_SYMBOL Symbol);


typedef BOOL (__stdcall *tSI)(IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess);


typedef DWORD (__stdcall *tSLM)(IN HANDLE hProcess, IN HANDLE hFile,
                               	IN PSTR ImageName,  IN PSTR ModuleName, 
								IN DWORD BaseOfDll, IN DWORD SizeOfDll);


typedef DWORD (__stdcall *tSSO)(IN DWORD SymOptions);


typedef BOOL (__stdcall *tSW)(DWORD MachineType, HANDLE hProcess,
							  HANDLE hThread,    LPSTACKFRAME StackFrame, PVOID ContextRecord,
							  PREAD_PROCESS_MEMORY_ROUTINE   ReadMemoryRoutine,
							  PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine,
							  PGET_MODULE_BASE_ROUTINE       GetModuleBaseRoutine,
							  PTRANSLATE_ADDRESS_ROUTINE     TranslateAddress);


typedef DWORD (__stdcall WINAPI *tUDSN)(PCSTR DecoratedName,     PSTR  UnDecoratedName,
										DWORD UndecoratedLength, DWORD Flags);


struct StackFunction
{
	HINSTANCE pLib;   
	tSC       pSC;    
	tSFTA     pSFTA;  
	tSGLFA    pSGLFA; 
	tSGMB     pSGMB;  
	tSGMI     pSGMI;  
	tSGO      pSGO;   
	tSGSFA    pSGSFA; 
	tSI       pSI;    
	tSLM      pSLM;   
	tSSO      pSSO;   
	tSW       pSW;    
	tUDSN     pUDSN;  
};

struct ModuleEntry
{
	std::string imageName;
	std::string moduleName;
	DWORD baseAddress;
	DWORD size;
};

typedef std::vector< ModuleEntry > ModuleList;

typedef ModuleList::iterator ModuleListIter;




void ShowStack(HANDLE hThread, CONTEXT& c);

bool fillModuleList(ModuleList& modules, DWORD pid, FILE* file);




StackFunction* g_SEHfunction = NULL;








void SEHinitProcess(void)

{
	if(g_SEHfunction == NULL)
	{
		g_SEHfunction = new StackFunction;

		memset(g_SEHfunction, 0, sizeof(StackFunction));

		
		
		g_SEHfunction->pLib = LoadLibrary("imagehlp.dll");

		if(g_SEHfunction->pLib != NULL)
		{
			g_SEHfunction->pSC    = (tSC)    GetProcAddress(g_SEHfunction->pLib, "SymCleanup");
			g_SEHfunction->pSFTA  = (tSFTA)  GetProcAddress(g_SEHfunction->pLib, "SymFunctionTableAccess");
			g_SEHfunction->pSGLFA = (tSGLFA) GetProcAddress(g_SEHfunction->pLib, "SymGetLineFromAddr");
			g_SEHfunction->pSGMB  = (tSGMB)  GetProcAddress(g_SEHfunction->pLib, "SymGetModuleBase");
			g_SEHfunction->pSGMI  = (tSGMI)  GetProcAddress(g_SEHfunction->pLib, "SymGetModuleInfo");
			g_SEHfunction->pSGO   = (tSGO)   GetProcAddress(g_SEHfunction->pLib, "SymGetOptions");
			g_SEHfunction->pSGSFA = (tSGSFA) GetProcAddress(g_SEHfunction->pLib, "SymGetSymFromAddr");
			g_SEHfunction->pSI    = (tSI)    GetProcAddress(g_SEHfunction->pLib, "SymInitialize");
			g_SEHfunction->pSSO   = (tSSO)   GetProcAddress(g_SEHfunction->pLib, "SymSetOptions");
			g_SEHfunction->pSW    = (tSW)    GetProcAddress(g_SEHfunction->pLib, "StackWalk");
			g_SEHfunction->pUDSN  = (tUDSN)  GetProcAddress(g_SEHfunction->pLib, "UnDecorateSymbolName");
			g_SEHfunction->pSLM   = (tSLM)   GetProcAddress(g_SEHfunction->pLib, "SymLoadModule");
		}
	}
}











DWORD SEHlogProcess(EXCEPTION_POINTERS *ep)

{
	HANDLE hThread;

	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
		            GetCurrentProcess(), &hThread, 
					0, false, DUPLICATE_SAME_ACCESS);

	ShowStack(hThread, *(ep->ContextRecord));

	CloseHandle(hThread);

	return EXCEPTION_EXECUTE_HANDLER;
}





void ShowStack(HANDLE hThread, CONTEXT& c)
{
	
	DWORD imageType = IMAGE_FILE_MACHINE_I386;

	HANDLE hProcess = GetCurrentProcess(); 

	int    frameNum;         

	DWORD  offsetFromSymbol; 

	DWORD  symOptions;       

	IMAGEHLP_SYMBOL *pSym = (IMAGEHLP_SYMBOL*) malloc(IMGSYMLEN + MAXNAMELEN);

	char undName[MAXNAMELEN];     
	char undFullName[MAXNAMELEN]; 

	IMAGEHLP_MODULE Module;
	IMAGEHLP_LINE   Line;

	std::string symSearchPath;

	char *tt = NULL, *p;

	STACKFRAME s; 
	memset(&s, '\0', sizeof(s));

	
	
	

	tt = new char[TTBUFLEN]; 

	char	fname[128];
	time_t	clock;
	tm*		newtime;

	time(&clock);
	newtime = localtime( &clock );

	wsprintf(fname, "../weo-%d%02d%02d-%02d-%02d-%02d.dbg", 
            newtime->tm_year+1900, newtime->tm_mon+1, 
            newtime->tm_mday, newtime->tm_hour, 
            newtime->tm_min, newtime->tm_sec);

	
	if((symOptions = GetModuleFileName(0, tt, TTBUFLEN)) != NULL)
	
	{
		p = tt + symOptions - 3;

		strcpy(p, "dbg");

		
		
		FILE* dbgfile = fopen(fname, "w");
		if(dbgfile != NULL)
		
		{
			
			

			fprintf(dbgfile, "================================================================================\n");
			fprintf(dbgfile, "==                      VANILLA Exception Report ver 0.01.00                  ==\n");
			fprintf(dbgfile, "================================================================================\n\n");

			
			

			fprintf(dbgfile, "== <TIME INFO> == \n");

			
			
			
			time(&clock);

			if((newtime = localtime(&clock)) != NULL)
			{
				fprintf(dbgfile, " Reported Time : %s\n", asctime(newtime));
			}
			else
			{
				fprintf(dbgfile, " Reported Time : No data\n");
			}

			
			

			fprintf(dbgfile, "== <FILE INFO> == \n");

			
			symSearchPath = "";

			
			
			for ( p = tt + strlen( tt ) - 1; p >= tt; -- p )
			{
				
				if ( *p == '\\' || *p == '/' || *p == ':' )
					break;
			}
			
			
			if ( p != tt ) 
			{
				if ( *p == ':' ) 
					++ p;
				*p = '\0'; 
				symSearchPath += tt + std::string( ";" );
			}
			fprintf(dbgfile, " Working     Directory : %s\n", tt);

			
			if(!g_SEHfunction->pSI(hProcess, NULL, false))
			{
				fprintf(dbgfile, "\n ! Error in SymInitialize(): GetLastError No. = %lu\n", gle);
				fprintf(dbgfile, "! Can't make report anymore");
			}
			else
			{
				
				symOptions  = g_SEHfunction->pSGO();
				symOptions |= SYMOPT_LOAD_LINES;
				symOptions &= ~SYMOPT_UNDNAME;
				g_SEHfunction->pSSO( symOptions ); 

				
				

				
				

				fprintf(dbgfile, "\n== <MODULE INFO> == \n");

				ModuleList     modules;
				ModuleListIter it;

				char *img, *mod;

				if(fillModuleList(modules, GetCurrentProcessId(), dbgfile))
				{
					
					

					fprintf(dbgfile, "\n== <SYMBOL INFO> == \n");

					for(it = modules.begin(); it != modules.end(); ++ it)
					{
						
						img = new char[(*it).imageName.size() + 1];
						strcpy(img, (*it).imageName.c_str());

						mod = new char[(*it).moduleName.size() + 1];
						strcpy(mod, (*it).moduleName.c_str());

						if(g_SEHfunction->pSLM(hProcess, 0, img, mod, (*it).baseAddress, (*it).size) == 0)
						{
							fprintf(dbgfile, " Simbol not loaded : %s\n", (*it).moduleName.c_str());
						}
						else
						{
							fprintf(dbgfile, " Symbols loaded    : %s\n", (*it).moduleName.c_str());
						}

						delete [] img;
						delete [] mod;
					}

					
					

					
					
					
					
					s.AddrPC.Offset    = c.Eip;
					s.AddrPC.Mode      = AddrModeFlat;
					s.AddrFrame.Offset = c.Ebp;
					s.AddrFrame.Mode   = AddrModeFlat;

					memset(pSym, '\0', IMGSYMLEN + MAXNAMELEN);
					pSym->SizeOfStruct  = IMGSYMLEN;
					pSym->MaxNameLength = MAXNAMELEN;

					memset(&Line, '\0', sizeof Line);
					Line.SizeOfStruct = sizeof Line;

					memset(&Module, '\0', sizeof Module);
					Module.SizeOfStruct = sizeof Module;

					offsetFromSymbol = 0;

					fprintf(dbgfile, "\n== <CALL STACK INFO> == \n");
					fprintf(dbgfile, "--------------------------------------------------------------------------------\n");
					fprintf(dbgfile, " No  | Far | Vir | EIP Addr | Ret Addr | Frame Ptr | Stack Ptr | Symbol & Offset\n");
					fprintf(dbgfile, "--------------------------------------------------------------------------------\n");

					for(frameNum = 0; ; ++frameNum)
					{
						
						
						
						
						if(!g_SEHfunction->pSW(imageType, hProcess, hThread, &s, &c, NULL, g_SEHfunction->pSFTA, g_SEHfunction->pSGMB, NULL)) break;

						
						fprintf(dbgfile, " %3d |  %c  |  %c  | %08lx | %08lx | %09lx | %09lx |",
							    frameNum, 
								s.Far? 'F': '.', 
								s.Virtual? 'V': '.',
							    s.AddrPC.Offset, 
								s.AddrReturn.Offset,
							    s.AddrFrame.Offset, 
								s.AddrStack.Offset);

						if(s.AddrPC.Offset == 0)
						{
							fprintf(dbgfile, "(-nosymbols- PC == 0)\n");
						}
						else
						{ 
							
							if(!g_SEHfunction->pSGSFA(hProcess, s.AddrPC.Offset, &offsetFromSymbol, pSym))
							{
								fprintf(dbgfile, " Error No. = %lu\n", gle);
								fprintf(dbgfile, "--------------------------------------------------------------------------------\n");

								fprintf(dbgfile, " Sig    : Error No. = %lu\n", gle);
								fprintf(dbgfile, " Decl   : Error No. = %lu\n", gle);
							}
							else
							{
								
								g_SEHfunction->pUDSN(pSym->Name, undName,     MAXNAMELEN, UNDNAME_NAME_ONLY);
								g_SEHfunction->pUDSN(pSym->Name, undFullName, MAXNAMELEN, UNDNAME_COMPLETE);

								fprintf(dbgfile, " %s", undName);
								if(offsetFromSymbol != 0)
								{
									fprintf(dbgfile, " %+ld bytes", (long)offsetFromSymbol);
								}
								fprintf(dbgfile, "\n");
								fprintf(dbgfile, "--------------------------------------------------------------------------------\n");

								fprintf(dbgfile, " Sig    : %s\n", pSym->Name);
								fprintf(dbgfile, " Decl   : %s\n", undFullName);
							}

							
							if(g_SEHfunction->pSGLFA != NULL)
							{   
								if(!g_SEHfunction->pSGLFA(hProcess, s.AddrPC.Offset, &offsetFromSymbol, &Line))
								{
									fprintf(dbgfile, " Line   : Error No. = %lu\n", gle);
								}
								else
								{
									fprintf(dbgfile, " Line   : %s (%lu line), %+ld bytes\n",
											Line.FileName, 
											Line.LineNumber, 
											offsetFromSymbol);
								}
							} 

							
							if(!g_SEHfunction->pSGMI(hProcess, s.AddrPC.Offset, &Module))
							{
								fprintf(dbgfile, " Module : Error No. = %lu\n", gle);
							}
							else
							{   
								char ty[80];

								switch(Module.SymType)
								{
									case SymNone:
										strcpy(ty, "-nosymbols-");
										break;
									case SymCoff:
										strcpy(ty, "COFF");
										break;
									case SymCv:
										strcpy(ty, "CV");
										break;
									case SymPdb:
										strcpy(ty, "PDB");
										break;
									case SymExport:
										strcpy(ty, "-exported-");
										break;
									case SymDeferred:
										strcpy(ty, "-deferred-");
										break;
									case SymSym:
										strcpy(ty, "SYM");
										break;
									default:
										_snprintf(ty, sizeof ty, "symtype = %ld", (long) Module.SymType);
										break;
								}

								fprintf(dbgfile, " Module : %s[%s], base: %08lxh\n",
										Module.ModuleName, 
										Module.ImageName, 
										Module.BaseOfImage);

								fprintf(dbgfile, " Sym    : |type| %s, |file| %s\n",
										ty, 
										Module.LoadedImageName);
							} 

							
							LPVOID lpMsgBuf;
							FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
								NULL, gle, 0, (LPTSTR) &lpMsgBuf, 0, NULL );
							fprintf(dbgfile, "ErrorMsg : %s\n", (LPCTSTR)lpMsgBuf);

							fprintf(dbgfile, "--------------------------------------------------------------------------------\n");

						} 

						
						if(s.AddrReturn.Offset == 0)
						{
							
							SetLastError(0);
							break;
						}
					} 

					
					

					char* stkPtr  = (char*)(c.Esp);

					fprintf(dbgfile, "\n== <STACK INFO> == \n");
					fprintf(dbgfile, "Stack : 1024 bytes starting at (ESP = %08lx)\n", stkPtr);
					fprintf(dbgfile, "--------------------------------------------------------------------------------\n");

					for(int i = 0; i < 64; ++i)
					{
						fprintf(dbgfile, " %08lx : ", stkPtr);

						for(int j = 0; j < 16; ++j, ++stkPtr)
						{
							fprintf(dbgfile, "%02hX ", (BYTE)*stkPtr);

							if((j & 3) == 3) fprintf(dbgfile, " ");
						}

						stkPtr -= 16;

						for(j = 0; j < 16; ++j, ++stkPtr)
						{
							if(*stkPtr < 32)
							{
								fprintf(dbgfile, ".");
							}
							else
							{
								fprintf(dbgfile, "%c", *stkPtr);
							}
						}

						fprintf(dbgfile, "\n");
					}

					
					

					fprintf(dbgfile, "\n== <MEMORY INFO> == \n");

				}
				else
				{
					fprintf(dbgfile, "Error : Processing error in kernel32.dll & tlhelp32.dll, so we can't load symbol\n");
				}
			}

			
			

			fclose(dbgfile);

			ResumeThread(hThread);

			
			g_SEHfunction->pSC(hProcess);

			free(pSym);

			delete [] tt;
		}
	}
}







#define MAX_MODULE_NAME32   255
#define TH32CS_SNAPMODULE   0x00000008

#pragma pack(push, 8)
typedef struct tagMODULEENTRY32
{
    DWORD   dwSize;
    DWORD   th32ModuleID;       
    DWORD   th32ProcessID;      
    DWORD   GlblcntUsage;       
    DWORD   ProccntUsage;       
    BYTE*   modBaseAddr;        
    DWORD   modBaseSize;        
    HMODULE hModule;            
    char    szModule[MAX_MODULE_NAME32 + 1];
    char    szExePath[MAX_PATH];
} MODULEENTRY32;
typedef MODULEENTRY32*  PMODULEENTRY32;
typedef MODULEENTRY32*  LPMODULEENTRY32;
#pragma pack(pop)


typedef HANDLE (__stdcall *tCT32S)( DWORD dwFlags, DWORD th32ProcessID );


typedef BOOL (__stdcall *tM32F)( HANDLE hSnapshot, LPMODULEENTRY32 lpme );


typedef BOOL (__stdcall *tM32N)( HANDLE hSnapshot, LPMODULEENTRY32 lpme );


bool fillModuleList(ModuleList& modules, DWORD pid, FILE* file)
{
	
	const char *dllname[] = { "kernel32.dll", "tlhelp32.dll" };
	HINSTANCE hToolhelp;

	tCT32S pCT32S = NULL;
	tM32F  pM32F  = NULL;
	tM32N  pM32N  = NULL;

	HANDLE hSnap;

	MODULEENTRY32 me = { sizeof me };

	bool keepGoing;

	ModuleEntry e;

	
	for(int i = 0; i < lenof( dllname ); ++ i)
	{
		hToolhelp = LoadLibrary(dllname[i]);
		if(hToolhelp == NULL) continue;

		pCT32S = (tCT32S) GetProcAddress(hToolhelp, "CreateToolhelp32Snapshot");
		pM32F  = (tM32F)  GetProcAddress(hToolhelp, "Module32First");
		pM32N  = (tM32N)  GetProcAddress(hToolhelp, "Module32Next");

		if(pCT32S != 0 && pM32F != 0 && pM32N != 0) break; 
		
		FreeLibrary(hToolhelp);
		hToolhelp = NULL;
	}

	if(hToolhelp == NULL) return false;
	

	if((hSnap = pCT32S(TH32CS_SNAPMODULE, pid)) == (HANDLE) -1) return false;

	
	

	fprintf(file, "--------------------------------------------------------------------------------\n");
	fprintf(file, "BaseAddress | BaseSize | Path\n");
	fprintf(file, "--------------------------------------------------------------------------------\n");

	keepGoing = !!pM32F(hSnap, &me);
	while(keepGoing)
	{
		
		fprintf(file, "%10lXh | %8lu | %s\n", me.modBaseAddr, me.modBaseSize, me.szExePath);

		e.imageName   = me.szExePath;
		e.moduleName  = me.szModule;
		e.baseAddress = (DWORD) me.modBaseAddr;
		e.size        = me.modBaseSize;

		modules.push_back(e);

		keepGoing = !!pM32N(hSnap, &me);
	}

	CloseHandle(hSnap);

	FreeLibrary(hToolhelp);

	return true;
}





void SEHexitProcess(void)

{
	FreeLibrary(g_SEHfunction->pLib);

	delete g_SEHfunction;
}
