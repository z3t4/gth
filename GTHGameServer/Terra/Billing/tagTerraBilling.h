

#ifndef _TAGTERRABILLING_H_
#define _TAGTERRABILLING_H_

#include "tagAddBill.h"

struct tagTerraBilling{
	
	public:
		char	IP[IPSTRING+1];
		int		Port;
		char	serverGUIDstring[tagAddBillPacket::maxbytes_serverGUIDstring];
		int     Game_No;
		

		
		struct tagLog{
			BOOL bOutput;
			int  UpdateCycle;
		};

		struct tagLog Log;

};



#endif