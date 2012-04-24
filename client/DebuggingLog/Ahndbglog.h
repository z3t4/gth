
















#ifndef __AHN_DEBUGLOG__
#define __AHN_DEBUGLOG__

	
	
	

	
	
	
	
	

	
	
	

	
	
	
	
	
	
	#pragma pack(push, before_imagehlp, 8)
	#include <imagehlp.h>
	#pragma pack(pop, before_imagehlp)

	
	
	

	
    

	#define AHN_SEH_START	SEHinitProcess(); __try {
	#define AHN_SEH_END     } __except(SEHlogProcess(GetExceptionInformation())) { SEHexitProcess(); }

	
    

	void SEHinitProcess(void);
	

	DWORD SEHlogProcess(EXCEPTION_POINTERS *ep);
	

	void SEHexitProcess(void);
	

	
	
	

	
	

	struct StackFunction;
	extern StackFunction* g_SEHfunction;

#endif
