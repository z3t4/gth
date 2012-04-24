



#include "../Global.h"

#include <windowsx.h>
#include <assert.h>
#include "tagGolryunBattle.h"
struct tagGolryunBattleVar gGolryunBattleVar;

#include "CGolryunBattle.h"







#include "CGolryunBattle_Log.h"
CGolryunBattle_Log* gcpGolryunBattleLog=NULL;

#include "CGolryunBattle_Observer.h"
CGolryunBattle_Observer* gcpGolryunBattle_Observer=NULL;


CGolryunBattle::CGolryunBattle()
{

	m_bOK=FALSE;
	m_cpData		= NULL;
	m_cpCommon		= NULL;
	m_cpEncoder		= NULL;
	m_cpDecoder		= NULL;
	m_cpProcess		= NULL;
	
	m_cpDBV2		= NULL;


	


	if(FALSE == Init()) return;
	m_bOK=TRUE;
}

CGolryunBattle::~CGolryunBattle()
{
	


	
	if(NULL != m_cpProcess){ delete m_cpProcess; m_cpProcess=NULL; }	
	if(NULL != m_cpDecoder){ delete m_cpDecoder; m_cpDecoder=NULL; }
	if(NULL != m_cpEncoder){ delete m_cpEncoder; m_cpEncoder=NULL; }
	
	if(NULL != m_cpDBV2)   { delete m_cpDBV2; m_cpDBV2=NULL; }
	if(NULL != m_cpCommon) { delete m_cpCommon; m_cpCommon=NULL; }
	if(NULL != m_cpData)   { delete m_cpData; m_cpData=NULL; }
	if(NULL != gcpGolryunBattleLog){ delete gcpGolryunBattleLog; gcpGolryunBattleLog=NULL; }
	if(NULL != gcpGolryunBattle_Observer){ delete gcpGolryunBattle_Observer; gcpGolryunBattle_Observer=NULL; }
	
	

}

BOOL CGolryunBattle::isOK(void) const
{
	return m_bOK;
}

BOOL CGolryunBattle::Init(void)
{
	if(FALSE == LoadServerConfigFile()) return FALSE;


	gcpGolryunBattle_Observer=new CGolryunBattle_Observer();
	if(NULL == gcpGolryunBattle_Observer) return FALSE;
	if(FALSE == gcpGolryunBattle_Observer->isOK()){ delete gcpGolryunBattle_Observer; gcpGolryunBattle_Observer=NULL; return FALSE; }

	gcpGolryunBattleLog=new CGolryunBattle_Log();
	if(NULL == gcpGolryunBattleLog) return FALSE;
	if(FALSE == gcpGolryunBattleLog->isOK()){ delete gcpGolryunBattleLog; gcpGolryunBattleLog=NULL; return FALSE; }

	m_cpCommon= new CGolryunBattle_Common();
	if(NULL == m_cpCommon) return FALSE;
	if(FALSE == m_cpCommon->isOK()){ delete m_cpCommon; m_cpCommon=NULL; return FALSE; }

	m_cpData= new CGolryunBattle_Data();
	if(NULL == m_cpData) return FALSE;
	if(FALSE == m_cpData->isOK()){ delete m_cpData; m_cpData=NULL; return FALSE; }

	

	m_cpDBV2 = new CGolryunBattle_DBV2();
	if(NULL == m_cpDBV2) return FALSE;
	if(FALSE == m_cpDBV2->isOK()){ delete m_cpDBV2; m_cpDBV2=NULL; return FALSE; }

	m_cpEncoder= new CGolryunBattle_Encoder(m_cpData);
	if(NULL == m_cpEncoder) return FALSE;
	if(FALSE == m_cpEncoder->isOK()){ delete m_cpEncoder; m_cpEncoder=NULL; return FALSE; }	

	m_cpProcess=new CGolryunBattle_Process(m_cpDBV2,m_cpData,m_cpEncoder);
	if(NULL == m_cpProcess) return FALSE;
	if(FALSE == m_cpProcess->isOK()){ delete m_cpProcess; m_cpProcess=NULL; return FALSE; }
	m_cpEncoder->SetProcessClassPointer(m_cpProcess);

	m_cpDecoder= new CGolryunBattle_Decoder(m_cpDBV2,m_cpEncoder,m_cpProcess);
	if(NULL == m_cpDecoder) return FALSE;
	if(FALSE == m_cpDecoder->isOK()){ delete m_cpDecoder; m_cpDecoder=NULL; return FALSE; }

	gcpGolryunBattleLog->Write("GolryunBattle Start....");
	return TRUE;
}




void CGolryunBattle::GTH_ProcessGBBattleSys()
{
	




	m_cpProcess->UpdateHandleStatus();	
	gcpGolryunBattleLog->Update();
	m_cpDBV2->Process();
}


CGolryunBattle_Data* CGolryunBattle::GetDataClassPointer(void) const
{
	return m_cpData;
}

CGolryunBattle_Common* CGolryunBattle::GetCommonClassPointer(void) const
{
	return m_cpCommon;
}

CGolryunBattle_Encoder* CGolryunBattle::GetEncoderClassPointer(void) const
{
	return m_cpEncoder;
}

CGolryunBattle_Process* CGolryunBattle::GetProcessClassPointer(void) const
{
	return m_cpProcess;
}


CGolryunBattle_Decoder* CGolryunBattle::GetDecoderClassPointer(void) const
{
	return m_cpDecoder;
}



CGolryunBattle_DBV2*    CGolryunBattle::GetDBV2ClassPointer(void) const
{
	return m_cpDBV2;
}





BOOL CGolryunBattle::LoadServerConfigFile(void)
{	
	gGolryunBattleVar.Log.bOutput=FALSE;
	gGolryunBattleVar.Log.UpdateTimeMiliSecond=1 * 1000;
	gGolryunBattleVar.GOLRYUNBATTLE_BEFORE_MATCH_WAIT_TIME= 5 * 1000;
	gGolryunBattleVar.GOLRYUNBATTLE_CEREMONY_WAIT_TIME= 20 * 1000;

	int size;
	char *token;
	char* pFileBuffer=NULL;

	
	{
		FILE* fp = NULL;
		fp = fopen( "server.cfg" , "rb" );
		if(NULL == fp) return FALSE;

		fseek(fp,0,SEEK_END);
		size = ftell(fp);
		pFileBuffer=(char*)GlobalAllocPtr(GMEM_FIXED,sizeof(char) * size);
		if(NULL == pFileBuffer){
			if(NULL != fp){ fclose(fp); fp=NULL; }
			return FALSE;
		}
		fseek(fp,0,SEEK_SET);
		fread(pFileBuffer,size,1,fp);
		if(NULL != fp){ fclose(fp); fp=NULL; }
	}

	curpos = pFileBuffer;
	endpos = curpos + size;	
	

	while((token = NextToken()) != NULL) {

		if(0 == stricmp(token,"GOLRYUNBATTLE_LOG_OUTPUT")){
			char* pTemp = NextArg();
			if((NULL != strstr("yes",pTemp)) || (NULL != strstr("YES",pTemp))) 
				gGolryunBattleVar.Log.bOutput=TRUE;
			else gGolryunBattleVar.Log.bOutput=FALSE;			
			continue;
		}


		if(0 == stricmp(token,"GOLRYUNBATTLE_LOG_UPDATE_MILISECOND")){
			gGolryunBattleVar.Log.UpdateTimeMiliSecond = atoi(NextArg());
			continue;
		}


		if(0 == stricmp(token,"GOLRYUNBATTLE_BEFORE_MATCH_WAIT_TIME")){
			gGolryunBattleVar.GOLRYUNBATTLE_BEFORE_MATCH_WAIT_TIME = atoi(NextArg());
			continue;
		}


		if(0 == stricmp(token,"GOLRYUNBATTLE_CEREMONY_WAIT_TIME")){
			gGolryunBattleVar.GOLRYUNBATTLE_CEREMONY_WAIT_TIME = atoi(NextArg());
			continue;
		}
	}

	if(NULL != pFileBuffer){ GlobalFreePtr(pFileBuffer); pFileBuffer=NULL; }
	return TRUE;
}


