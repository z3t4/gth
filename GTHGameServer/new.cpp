 #include "Global.h"
#include "New.h"
#include <assert.h>

#include "GonryunPracticeBattle/CGonryunPractice_Ability.h"
#include "GonryunPracticeBattle/CGonryunPracticeBattle.h"
#include "GonryunPracticeBattle/CGonryunPractice_RewordCtrl.h"
#include "GonryunPracticeBattle/CGonryunPractice_Decoder.h"
#include "GonryunPracticeBattle/CGonryunPractice_RewordItem.h"
class CGonryunPractice_ProcessCtrl;
extern CGonryunPracticeBattle	g_GonryunPracticeBattle;	

#include "GolryunBattle\CGolryunBattle.h"
extern CGolryunBattle* gcpGolryunBattle;

#include "Terra\GiftCard\CGiftCard.h"
extern CGiftCard* gcpGiftCard;

#include "GM(Game Master)\CGMCtrl.h"
extern CGMCtrl* gcpGMCtrl;

#include "network/Gs_To_Gms.h"
extern CGs_To_Gms g_GsToGms;

#include "network/Gms_To_Gs.h"
extern CGms_To_Gs g_GmsToGs;



extern CItem_Scroll_Proc		g_Item_Scroll_Proc;


#include "AHNLAB\AHNLAB.h"
extern CAhnLab* g_cpAhnLab;



int LoadGonryunBattlePracticeTableReword(char *filenameReword, CGonryunPractice_RewordCtrl* pPracticeRewordManager)
{

	FILE *pfileReword=NULL;
   	int size;
	byte *buffer;
	char token[512];

	if ( ( pfileReword = fopen( filenameReword, "rb" ) ) == NULL )
		return 0;

	fseek(pfileReword,0,SEEK_END);

	size = ftell(pfileReword);

	buffer = new byte[size+1];

	fseek(pfileReword,0,SEEK_SET);
	fread(buffer,size,1,pfileReword);
	fclose(pfileReword);

	curpos = (char *)buffer;
	endpos = curpos + size;

	int ret, idx = 0;

     
	while (1) 
	{
		ret  = NextCSVToken( token );
		if ( ret == 1 )	break;
		
		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;
			
			while (1)
			{
				ret  = NextCSVToken( token );
				if ( ret == 1 )
				{
					
					if(buffer!=NULL)
					{
						delete [] buffer;
						buffer=NULL;
					}
					
					
					return 1;
				}
				if ( ret == 2 )	break;
			}
			continue;
		}

		

		CGonryunPractice_Ability* pReword = new CGonryunPractice_Ability;
		pReword->SetID(atoi( token ));
		ret  = NextCSVToken( token );
			pReword->SetName(token);
		ret  = NextCSVToken( token );
		pReword->SetType(atoi( token ));	
		ret  = NextCSVToken( token );
		pReword->SetExperience(atoi( token ));	
		ret  = NextCSVToken( token );
		pReword->SetExpRate(atoi( token ));	
		ret  = NextCSVToken( token );
		pReword->SetItemID(atoi( token ));	
		ret  = NextCSVToken( token );
		pReword->SetItemRate(atoi( token ));	
		ret  = NextCSVToken( token );
		
		if ( ret == 1 )	
		{
			pPracticeRewordManager->GetRewordList().push_back(pReword);
			
			if(buffer!=NULL)
			{
				delete [] buffer;
			}
			
			
			return 1;
		}
		if ( ret == 2 )	
		{
			pPracticeRewordManager->GetRewordList().push_back(pReword);
			continue;
		}


	}

	
	if(buffer!=NULL)
	{
		delete [] buffer;
	}
	
	
	return 1;
}


int LoadGonryunBattlePracticeTableRewordItem(char *filenameRewordItem, CGonryunPractice_RewordCtrl* pPracticeRewordManager)
{

	FILE *pfileRewordItem=NULL;
   	int size;
	byte *buffer;
	char token[512];

	if ( ( pfileRewordItem = fopen( filenameRewordItem, "rb" ) ) == NULL )
		return 0;

	fseek(pfileRewordItem,0,SEEK_END);

	size = ftell(pfileRewordItem);

	buffer = new byte[size+1];

	fseek(pfileRewordItem,0,SEEK_SET);
	fread(buffer,size,1,pfileRewordItem);
	fclose(pfileRewordItem);

	curpos = (char *)buffer;
	endpos = curpos + size;
	


	 int ret=0, idx = 0;
    
	 while (1) {

		ret  = NextCSVToken( token );
		
		if ( ret == 1 )	break;
		
		
		if ( !( token[0] > 47 && token[0] < 58 ) )
		{
			if ( ret == 2 )	break;
			
			while (1)
			{
				ret  = NextCSVToken( token );
				if ( ret == 1 )	
				{
					
					if(buffer!=NULL)
					{
						delete [] buffer;
					}
					
					return 1;
				}

				if ( ret == 2 )	break;
			}
			continue;
		}
		
		
		CGonryunPractice_RewordItem* pWrewordItem= new CGonryunPractice_RewordItem();
		pWrewordItem->SetItemIndex(atoi( token ));
		ret  = NextCSVToken( token );
		pWrewordItem->SetItemType(atoi( token ));	
		ret  = NextCSVToken( token );
		pWrewordItem->SetItemNumber(atoi( token ));	
		ret  = NextCSVToken( token );
		pWrewordItem->SetItemCount(atoi( token ));	
		ret  = NextCSVToken( token );
		pWrewordItem->SetItemRate(atoi( token ));	
		ret  = NextCSVToken( token );
		
		
		
		if ( ret == 1 )	
		{
			pPracticeRewordManager->GetRewordItemList().push_back(pWrewordItem);
			
			if(buffer!=NULL)
			{
				delete [] buffer;
			}
			
			return 1;
		}

		if ( ret == 2 )	
		{
			pPracticeRewordManager->GetRewordItemList().push_back(pWrewordItem);
			continue;
		}
	}


	 
	 if(buffer!=NULL)
	 {
		 delete [] buffer;
	 }
	
	return 1;
}

int GTH_ParseGameLogicConfig()
{
	FILE *fp;
	int size;
	byte *buffer;
	char *token;

	

	int nextGrade;

	
	itemUpgradeProbabilityTable_t * pUpgradeTable;

	
	memset( &g_logic, 0, sizeof( gthGameLogicCofing_t ) );

	g_logic.expRate				=	1.0f;
	g_logic.nseRate				=	1.0f;
	g_logic.cheatEnable			=	false;
	
	
	g_logic.worldChattingPrice	=	1000;		

	
	
	


	g_logic.m_SpawnPosOverRate = 1.0f;

	fp = fopen( "logic.cfg" , "rb" );

	if( !fp )   
	{
		MessageBox(NULL, "does not exist 'logic.cfg' file", "Error", MB_ICONHAND|MB_OK);
		return false;
	}

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	curpos = (char *)buffer;
	endpos = curpos + size;

	while ((token = NextToken()) != NULL)
    {
		if( !stricmp( token, "EXP_RATE" ) )
		{
			g_logic.expRate = atof( NextArg() );
		}
		else if( !stricmp( token, "NSE_RATE" ) )
		{
			g_logic.nseRate = atof( NextArg() );
		}
		else if( !stricmp( token, "ITEMDROP_RATE" ) )
		{
			g_logic.itemDropRate = atof( NextArg() );
		}
		else if( !stricmp( token, "GENCAPABILITY_RATE" ) )
		{
			g_logic.genCapabilityRate = atof( NextArg() );
		}
		
		else if( !stricmp( token, "CHEAT_ENABLE" ) )
		{
			token = NextToken();
			if( !stricmp( token, "TRUE" ) ) g_logic.cheatEnable = true;
		}
		
		else if( !stricmp( token, "UPGRADE" ) )
		{
			token = NextToken();
			

			if( !stricmp( token, "LEAFQUALITY" ) )
				pUpgradeTable = &g_logic.leafStoneProTbl;
			else if( !stricmp( token, "MADQUALITY" ) )
				pUpgradeTable = &g_logic.madStoneProTbl;
			else if( !stricmp( token, "WAVEQUALITY" ) )
				pUpgradeTable = &g_logic.waveStoneProTbl;
			else if( !stricmp( token, "FIREQUALITY" ) )
				pUpgradeTable = &g_logic.fireStoneProTbl;
			else if( !stricmp( token, "ARTQUALITY" ) )
				pUpgradeTable = &g_logic.artStoneProTbl;
			else
				break;

			token = NextToken();
			if( token[0] == '{' )		
			{
				while( 1 )
				{
					token = NextToken();
					if( token[0] == '}' ) break;		
					if( !stricmp( token, "GRADE" ) )
					{
						token = NextToken();
						nextGrade = atoi( token );
						token = NextToken();

						if( token[0] == '{' )		
						{
							while( 1 )
							{
								token = NextToken();
								if( token[0] == '}' ) break;		
								if( !stricmp( token, "SUCCESS_PRO" ) )
								{
									token = NextToken();
									pUpgradeTable->successPro[nextGrade] = atof( token );
								}
								else if( !stricmp( token, "COST" ) )
								{
									token = NextToken();
									pUpgradeTable->cost[nextGrade] = atoi( token );
								}
								else if( !stricmp( token, "VANISH_PRO" ) )
								{
									token = NextToken();
									pUpgradeTable->vanishPro[nextGrade] = atof( token );
								}
								else if( !stricmp( token, "GRADEDOWN_PRO" ) )
								{
									token = NextToken();
									pUpgradeTable->gradeDownPro[nextGrade] = atof( token );
								}
								else if( !stricmp( token, "GRADEDOWN_LEVEL" ) )
								{
									token = NextToken();
									pUpgradeTable->gradeDownLevel[nextGrade] = atof( token );
								}
							}
						}
					}
				}
			}
		}
		
		
		else if( !stricmp( token, "GAMBLESTONE" ) )
		{
			int quality, grade;
			token = NextToken();

			
			g_logic.gambleTable[0].gradePro[4] = 0.0;

			if( !stricmp( token, "LOWQUALITY" ) )
			{
				quality = 0;
			}
			else if( !stricmp( token, "HIGHQUALITY" ) )
			{
				quality = 1;
			}

			token = NextToken();
			if( token[0] == '{' )		
			{
				while( 1 )
				{
					token = NextToken();
					if( token[0] == '}' ) break;		
					if( !stricmp( token, "SUCCESS_PRO" ) )
					{
						token = NextToken();
						g_logic.gambleTable[quality].successPro = atof( token ) / 100.0f;
					}
					else if( !stricmp( token, "COST" ) )
					{
						token = NextToken();
						g_logic.gambleTable[quality].cost = atoi( token );
					}
					else if( !stricmp( token, "VANISH_PRO" ) )
					{
						token = NextToken();
						g_logic.gambleTable[quality].vanishPro = atof( token ) / 100.0f;
					}
					else if( !stricmp( token, "GRADE" ) )
					{
						token = NextToken();
						grade = atoi( token ) - 1 - quality*4;
						token = NextToken();

						if( token[0] == '{' )		
						{
							while( 1 )
							{
								token = NextToken();
								if( token[0] == '}' ) break;		
								if( !stricmp( token, "GRADE_PRO" ) )
								{
									token = NextToken();
									g_logic.gambleTable[quality].gradePro[grade] = atof( token ) / 100.0f;
								}
							}
						}
					}
				}
			}
		}
		
		
		else if( !stricmp( token, "PENALTY" ) )
		{
			int chaosClass;
			token = NextToken();
			if( token[0] == '{' )		
			{
				while( 1 )
				{
					
					token = NextToken();
					if( token[0] == '}' ) break;		
					chaosClass = atoi( token );
					
					token = NextToken();
					g_logic.dieItemLossRate[chaosClass] = atof( token );
					token = NextToken();
					g_logic.dieExpLossRate[chaosClass] = atof( token );
				}
			}
		}
		
		
		else if( !stricmp( token, "WORLDCHATTING_PRICE" ) )
		{
			token = NextToken();
			g_logic.worldChattingPrice = atoi( token );
		}
		else if ( !stricmp (token, "DAN_BATTLE_PRICE" ) )
		{	
			token = NextToken();
			g_logic.DanBattlePrice = atoi(token);
		}

		
		else if ( !stricmp (token, "GAMBLE_OPTION" ) )
		{	
			token = NextToken();			
			int CurOpiontNumber = 0;
			int GradeCount = 0;			
			if ( !stricmp (token, "{" ) )
			{
				while ( 1)
				{
					token = NextToken();			
					if ( !stricmp (token, "}" ) )
						break;		
					if ( !stricmp (token, "GRADE_COUNT" ) )
					{
						token = NextToken();		
						GradeCount = atoi(token);
					}					

					if ( !stricmp (token, "SELECT_RATE" ) )
					{
						token = NextToken();			
						g_logic.gambleSelectOptionTable[CurOpiontNumber].SelectOptionRate = atof(token);
						g_logic.gambleSelectOptionTable[CurOpiontNumber].SelectOptionCount = CurOpiontNumber;
					}

					if ( !stricmp (token, "GAMBLE_PRICE_RATE" ) )
					{
						token = NextToken();		
						g_logic.GamblePriceRate = atof(token);
					}										
					
					if ( !stricmp (token, "SELECT_GRADE" ) )
					{	
						for (int i = 0; i < GradeCount; i ++)
						{
							token = NextToken();
							g_logic.gambleSelectOptionTable[CurOpiontNumber].SelectOptionGraderate[i] = atof(token);
						}
						CurOpiontNumber++;
					}
					if ( !stricmp (token, "ESSENCE_RATE" ) )
					{
						token = NextToken();			
						g_logic.gambleitemSelectRate[0] = atof(token);
					}
					if ( !stricmp (token, "ADVANCED_RATE" ) )
					{
						token = NextToken();			
						g_logic.gambleitemSelectRate[1] = atof(token);
					}
					if ( !stricmp (token, "MYSTIC_RATE" ) )
					{
						token = NextToken();			
						g_logic.gambleitemSelectRate[2] = atof(token);
					}
					if ( !stricmp (token, "CLASSIC_RATE" ) )
					{
						token = NextToken();			
						g_logic.gambleitemSelectRate[3] = atof(token);
					}
					if ( !stricmp (token, "UNIQUE_RATE" ) )
					{
						token = NextToken();			
						g_logic.gambleitemSelectRate[4] = atof(token);
					}

				}			
			}
			
		}	
		else if( !stricmp( token, "DS_TIME_STARTUP" ) )									
		{
			token = NextToken();
			g_logic.danBattleStatusTime[DBTL_STATUS_STARTUP] = atoi( token );
		}
		else if( !stricmp( token, "DS_TIME_READY" ) )									
		{
			token = NextToken();
			g_logic.danBattleStatusTime[DBTL_STATUS_READY] = atoi( token );
		}
		else if( !stricmp( token, "DS_TIME_MOVE" ) )									
		{
			token = NextToken();
			g_logic.danBattleStatusTime[DBTL_STATUS_MOVE] = atoi( token );
		}
		else if( !stricmp( token, "DS_TIME_START" ) )									
		{
			token = NextToken();
			g_logic.danBattleStatusTime[DBTL_STATUS_START] = atoi( token );
		}
		else if( !stricmp( token, "DS_TIME_END" ) )										
		{
			token = NextToken();
			g_logic.danBattleStatusTime[DBTL_STATUS_END] = atoi( token );
		}
		else if( !stricmp( token, "DS_TIME_LOOSE_MOVE" ) )								
		{
			token = NextToken();
			g_logic.danBattleStatusTime[DBTL_STATUS_LOOSE_MOVE] = atoi( token );
		}
		else if( !stricmp( token, "DS_TIME_CLEAR" ) )									
		{
			token = NextToken();
			g_logic.danBattleStatusTime[DBTL_STATUS_CLEAR] = atoi( token );
		}
		
		
		else if( !stricmp( token, "DAN_BATTLE_PORTAL_START_POS" ) )									
		{
			token = NextToken();
			g_logic.danbattlePortal[DANBATTLE_START_PORTAL].StartworldIdx = atoi( token );

			for (int i = 0; i < 3; i ++)
			{	
				token = NextToken();
				g_logic.danbattlePortal[DANBATTLE_START_PORTAL].StartPos[i] = atof( token );
			}

		}
		else if( !stricmp( token, "DAN_BATTLE_PORTAL_TARGET_POS" ) )									
		{
			token = NextToken();			
			g_logic.danbattlePortal[DANBATTLE_START_PORTAL].TargetWorldIdx = atoi( token );
			for (int i = 0; i < 3; i ++)
			{	
				token = NextToken();
				g_logic.danbattlePortal[DANBATTLE_START_PORTAL].TargetPos[i] = atof( token );
			}
		}
		else if( !stricmp( token, "DAN_BATTLE_PORTAL_TARGET_POS2" ) )									
		{
			token = NextToken();			
			g_logic.danbattlePortal[DANBATTLE_START_PORTAL].TargetWorldIdx2 = atoi( token );
			for (int i = 0; i < 3; i ++)
			{	
				token = NextToken();
				g_logic.danbattlePortal[DANBATTLE_START_PORTAL].TargetPos2[i] = atof( token );
			}
		}

		else if( !stricmp( token, "DAN_BATTLE_ATEAM_START_PORTAL_POS" ))		
		{
			token = NextToken();			
			g_logic.danbattlePortal[DANBATTLE_ATEAM_PORTAL].StartworldIdx = atoi( token );
			for (int i = 0; i < 3; i ++)
			{	
				token = NextToken();
				g_logic.danbattlePortal[DANBATTLE_ATEAM_PORTAL].StartPos[i] = atof( token );
			}
		}
		else if( !stricmp( token, "DAN_BATTLE_ATEAM_TARGET_PORTAL_POS" ) )									
		{
			token = NextToken();			
			g_logic.danbattlePortal[DANBATTLE_ATEAM_PORTAL].TargetWorldIdx = atoi( token );
			for (int i = 0; i < 3; i ++)
			{	
				token = NextToken();
				g_logic.danbattlePortal[DANBATTLE_ATEAM_PORTAL].TargetPos[i] = atof( token );
			}
		}
		else if( !stricmp( token, "DAN_BATTLE_BTEAM_START_PORTAL_POS" ) )									
		{
			token = NextToken();			
			g_logic.danbattlePortal[DANBATTLE_BTEAM_PORTAL].StartworldIdx = atoi( token );
			for (int i = 0; i < 3; i ++)
			{	
				token = NextToken();
				g_logic.danbattlePortal[DANBATTLE_BTEAM_PORTAL].StartPos[i] = atof( token );
			}
		}
		else if( !stricmp( token, "DAN_BATTLE_BTEAM_TARGET_PORTAL_POS" ) )
		{
			token = NextToken();
			g_logic.danbattlePortal[DANBATTLE_BTEAM_PORTAL].TargetWorldIdx = atoi( token );
			for (int i = 0; i < 3; i ++)
			{	
				token = NextToken();
				g_logic.danbattlePortal[DANBATTLE_BTEAM_PORTAL].TargetPos[i] = atof( token );
			}
		}		
		else if( !stricmp( token, "DAN_BATTLE_WIN_REWARD_FOR_DAN" ) )					
		{
			token = NextToken();
			g_logic.danBattleReward[DANB_WIN_DAN] = atoi( token );
		}		
		else if( !stricmp( token, "DAN_BATTLE_WIN_REWARD_FOR_USER" ) )
		{
			token = NextToken();
			g_logic.danBattleReward[DANB_WIN_USER] = atoi( token );
		}		
		else if( !stricmp( token, "DAN_BATTLE_LOOSE_REWARD_FOR_DAN" ) )
		{
			token = NextToken();
			g_logic.danBattleReward[DANB_LOOSE_DAN] = atoi( token );
		}		
		else if( !stricmp( token, "DAN_BATTLE_LOOSE_REWARD_FOR_USER" ) )
		{
			token = NextToken();
			g_logic.danBattleReward[DANB_LOOSE_USER] = atoi( token );
		}		
		
		else if( !stricmp(token,"DAN_MARK_CHANGE_DELAY_HOUR"))
		{
			token = NextToken();
			g_logic.danMarkChangeDelayHour = atoi(token);
			assert(g_logic.danMarkChangeDelayHour >= 0);
			assert(g_logic.danMarkChangeDelayHour <= 24 * 30);

			if(g_logic.danMarkChangeDelayHour < 0) g_logic.danMarkChangeDelayHour=1;
			if(g_logic.danMarkChangeDelayHour > 24 * 30) g_logic.danMarkChangeDelayHour=24 * 30;		

		}
		
		
			
		else if( !stricmp( token, "PCB_NONE" ) )									
		{
			token = NextToken();
				g_logic.PCBGonryunCondition.PCBStatusTime[CGonryunPractice_ProcessCtrl::Step::STEP_NONE] = atoi( token );
		}
		else if( !stricmp( token, "PCB_SUGGUEST" ) )								
		{
			token = NextToken();
				g_logic.PCBGonryunCondition.PCBStatusTime[CGonryunPractice_ProcessCtrl::Step::STEP_SUGGUEST] = atoi( token );
		}
		else if( !stricmp( token, "PCB_BATTING" ) )									
		{
			token = NextToken();
				g_logic.PCBGonryunCondition.PCBStatusTime[CGonryunPractice_ProcessCtrl::Step::STEP_BATTING] = atoi( token );
		}
		else if( !stricmp( token, "PCB_READY" ) )									
		{
			token = NextToken();
				g_logic.PCBGonryunCondition.PCBStatusTime[CGonryunPractice_ProcessCtrl::Step::STEP_BATTLE_READY] = atoi( token );
		}

		else if( !stricmp( token, "PCB_START" ) )									
		{
			token = NextToken();
				g_logic.PCBGonryunCondition.PCBStatusTime[CGonryunPractice_ProcessCtrl::Step::STEP_FIGHTING] = atoi( token );
		}

		else if( !stricmp( token, "PCB_END" ) )										
		{
			token = NextToken();
				g_logic.PCBGonryunCondition.PCBStatusTime[CGonryunPractice_ProcessCtrl::Step::STEP_BATTLE_END] = atoi( token );
		}
	
		else if( !stricmp( token, "PCB_WAIT" ) )										
		{
			token = NextToken();
				
		}
		
		else if( !stricmp( token, "PCB_MINILEVEL" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_iminiLevel = atoi( token );
		}
		
		else if( !stricmp( token, "PCB_MAXLEVEL" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_imaxLevel = atoi( token );
		}
		
		else if( !stricmp( token, "PCB_DIFFERLEVEL" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_idifferLevel = atoi( token );
		}
		
		else if( !stricmp( token, "PCB_ROYALTY" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_iroyalty= atoi( token );
		}
		
		
		else if( !stricmp( token, "PCB_BATTINGROYALTY" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_ibattingRoyalty = atoi( token );
		}
		
		
		else if( !stricmp( token, "PCB_USECOUNT" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_iuseCount = atoi( token );
		}
		
		else if( !stricmp( token, "PCB_ACTIVE" ) )										
		{
			token = NextToken();
				g_GonryunPracticeBattle.GetProcessCtrl()->SetActive(atoi( token ));
		}
		
		
		else if( !stricmp( token, "PCB_SYEN_LEVEL" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_isyenminilevel = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_isyenmaxlevel = atoi( token );
		}
		
		else if( !stricmp( token, "PCB_POTALPOSITION_ALAYAN_READER_IN" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayanReaderPos.m_fbattlein_x = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayanReaderPos.m_fbattlein_y = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayanReaderPos.m_fbattlein_z = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayanReaderPos.m_fbattlein_yaw = atoi( token );
		}

		else if( !stricmp( token, "PCB_POTALPOSITION_ALAYAN_OPENENT_IN" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayanOpenentPos.m_fbattlein_x = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayanOpenentPos.m_fbattlein_y = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayanOpenentPos.m_fbattlein_z = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayanOpenentPos.m_fbattlein_yaw = atoi( token );
		}

		else if( !stricmp( token, "PCB_POTALPOSITION_ALAYAN_WINNER_OUT" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayanwinnerOutPos.m_fbattlein_x = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayanwinnerOutPos.m_fbattlein_y = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayanwinnerOutPos.m_fbattlein_z = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayanwinnerOutPos.m_fbattlein_yaw = atoi( token );
		}
		
		else if( !stricmp( token, "PCB_POTALPOSITION_ALAYAN_DEFEATOR_OUT" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayandefeatorOutPos.m_fbattlein_x = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayandefeatorOutPos.m_fbattlein_y = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayandefeatorOutPos.m_fbattlein_z = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tarayandefeatorOutPos.m_fbattlein_yaw = atoi( token );
		}



		else if( !stricmp( token, "PCB_POTALPOSITION_SYEN_READER_IN" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tasyenReaderPos.m_fbattlein_x = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tasyenReaderPos.m_fbattlein_y = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tasyenReaderPos.m_fbattlein_z = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tasyenReaderPos.m_fbattlein_yaw = atoi( token );
		}
	
	



		else if( !stricmp( token, "PCB_POTALPOSITION_SYEN_OPENENT_IN" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tasyenOpenentPos.m_fbattlein_x = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tasyenOpenentPos.m_fbattlein_y = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tasyenOpenentPos.m_fbattlein_z = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tasyenOpenentPos.m_fbattlein_yaw = atoi( token );
		}

		else if( !stricmp( token, "PCB_POTALPOSITION_SYEN_OUT" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tsyenOutPos.m_fbattlein_x = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tsyenOutPos.m_fbattlein_y = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.m_tsyenOutPos.m_fbattlein_z = atoi( token );
		
		}

		
		else if( !stricmp( token, "PCB_ORIGINPOSITION_ALAYAN" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.origin_ArayanOutPos[0] = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.origin_ArayanOutPos[1] = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.origin_ArayanOutPos[2] = atoi( token );
		
		}


		else if( !stricmp( token, "PCB_ORIGINPOSITION_SYEN" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.origin_syenOutPos[0] = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.origin_syenOutPos[1] = atoi( token );
			token = NextToken();
			g_logic.PCBGonryunCondition.origin_syenOutPos[2] = atoi( token );
		
		}

	
		else if( !stricmp( token, "PCB_BATTLE_IN" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.pcpBattleIn = atoi( token );
		}

	


		

		
		else if( !stricmp( token, "PCB_PAY_EXPRATE" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.fExpRate = atoi( token );
		}
		
		else if( !stricmp( token, "PCB_PAY_ITEMRATE" ) )										
		{
			token = NextToken();
			g_logic.PCBGonryunCondition.fItemRate = atoi( token );
		}
	
		
		
		else if(!stricmp( token, "HELPER_LEVEL_POINT" ))
		{
			
			token = NextToken();
			g_logic.HelperLevelPoint.iLevel1 = atoi( token );
			token = NextToken();
			g_logic.HelperLevelPoint.iLevel4 = atoi( token );
			token = NextToken();
			g_logic.HelperLevelPoint.iLevel10 = atoi( token );
			token = NextToken();
			g_logic.HelperLevelPoint.iLevel20 = atoi( token );
			token = NextToken();
			g_logic.HelperLevelPoint.iLevel30 = atoi( token );
		
		}
		

		





		
		


		
		else if ( !stricmp (token, "CASHAMLL_ITEM" ) )
		{	
			token = NextToken();						
			int GradeCount = 0;
			CashMallItem_OptionStone::enumUseItemMode mode;
			mode = CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE;

			if ( !stricmp (token, "{" ) )
			{
				while ( 1)
				{
					token = NextToken();			
					if ( !stricmp (token, "}" ) )
						break;							

					if ( !stricmp (token, "}" ) )
						break;

					if ( !stricmp (token, "STONE_TYPE" ) )					
					{
						token = NextToken();

						if ( !stricmp(token, "STONE_OF_SAGE") ) 
							mode = CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE;

						if ( !stricmp(token, "STONE_OF_UNKNOWN") ) 
							mode = CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY;

						if ( !stricmp(token, "STONE_OF_EACH_NAME") ) 
							mode = CashMallItem_OptionStone::enumUseItemMode::STONE_OF_EACH_NAME;
					}
	
					if ( !stricmp (token, "DESTROY_RATE" ) )					
					{
						token = NextToken();
						g_logic.OptionStoneItemLogic[mode].DestroyRate = atof(token);						
					}

					
					if ( !stricmp (token, "SUCCECS_RATE" ) )					
					{
						token = NextToken();
						g_logic.OptionStoneItemLogic[mode].SuccecsRate = atof(token);						
					}					


					if ( !stricmp (token, "SELECT_GRADE" ) )
					{	
						for (int iGradeIdx = 0; iGradeIdx < MAX_NUMBER_OF_OPTION_VALUE_GRADE; iGradeIdx ++)
						{
							token = NextToken();
							g_logic.OptionStoneItemLogic[mode].OptionGrade[iGradeIdx] = atof(token);
						}						
					}
					
				}
			}
			
		}	
		
		else if (!stricmp(token, "NEW_DBATTLE_PC_COUNT"))
		{
			token = NextToken();
			g_NewDanBattleSystem.m_nMinPcCount = atoi(token);
		}
		else if (!stricmp(token, "NEW_DBATTLE_TIME1"))
		{
			token = NextToken();
			g_NewDanBattleSystem.m_nBattleTimeForMin1 = atoi(token);
		}
		else if (!stricmp(token, "NEW_DBATTLE_TIME2"))
		{
			token = NextToken();
			g_NewDanBattleSystem.m_nBattleTimeForMin2 = atoi(token);
		}			
		else if (!stricmp(token, "NEW_DBATTLE_ITEM_DROPRATE"))
		{
			token = NextToken();
			g_NewDanBattleSystem.m_fItemDropRate = atof(token);
		}
		else if (!stricmp(token, "NEW_DBATTLE_WAIT_TIME_READY"))
		{
			token = NextToken();
			g_NewDanBattleSystem.m_WaitTimeforReady = atof(token);
		}		
		else if (!stricmp(token, "NEW_DBATTLE_WAIT_TIME_END"))
		{
			token = NextToken();
			g_NewDanBattleSystem.m_WaitTimeforEnd = atof(token);
		}
		
	}

	fclose(fp);

	
	if(buffer!=NULL)
	{
		delete [] buffer;
	}
	


	return true;
}

extern char *g_lTableToken[];								
#include "CPremiumPlayerCtrl.h"
extern CPremiumPlayerCtrl* gcpPremiumPlayerCtrl;

void GTH_ProcessMessage_LogicReload()						
{
	DWORD StartTime=timeGetTime();
	DWORD Endtime;

	GTH_ParseGameLogicConfig();

	char filename[MAX_PATH];

	
	sprintf( filename, "l_tables/GonryunBattleRewardField%s.csv", g_lTableToken[g_config.languageType] );
	
	if( !LoadGonryunBattlePracticeTableReword(filename, g_GonryunPracticeBattle.GetRewordCtrl() ))
	{
		g_logSystem->Write("LoadGonryunBattlePracticeTableReword!");
		
	}
	
	sprintf( filename, "l_tables/GonryunBattleRewardItem%s.csv", g_lTableToken[g_config.languageType] );
	if( !LoadGonryunBattlePracticeTableRewordItem(filename, g_GonryunPracticeBattle.GetRewordCtrl()))
	{
		g_logSystem->Write("LoadGonryunBattlePracticeTableRewordItem!");
		
	}


	Endtime=timeGetTime();
	ShowLogInfo("Logic ReloadTime : %d",Endtime-StartTime);

	gcpPremiumPlayerCtrl->LoadIniFile();

	
	gGameConfigFlag.LoadFlagFile();
}



void GTH_ProcessMessage_Client_Extend()
{
	byte command = MSG_ReadByte();

	switch(command)
	{
		case CC_SUB_EVENT_SOCIAL_SHOUT:	
			GTH_PC_Shout(g_curPC);
			break;

		case CC_SUB_EVENT_SOCIAL_BOW:	
			GTH_PC_BOW(g_curPC);	
			break;

			
		case CC_MAILSYSTEM :
			GTH_ProcessMessage_MailSystem();
			break;

			
		case CC_WORLDCHATTING :
			GTH_ProcessMessage_WorldChatting();
			break;
			
		case CC_SAVE_SKIN_PART :
			GTH_ProcessMessage_SkinPart();
			break;
			
			
		case CC_DAN_BATTLE :															
			g_DanBattleFunc.RecvCC_DanBattleMessage();
			break;

		case GSM_DAN_BATTLE :															
			g_DanBattleFunc.RecvGSM_DanBattleMessage();
			break;
			
		case CC_REQUEST_GONRYUNBATTLEPRACTICE:
			g_GonryunPracticeBattle.GetDecoder()->recvPacket();													
			break;

			
		case CC_GB_BATTLE:
			(gcpGolryunBattle->GetDecoderClassPointer())->Decoder();
			break;
			

		case CC_RELOAD_LOGIC :															
			GTH_ProcessMessage_LogicReload();
			break;
					
		
		case CC_REQUEST_CHRISTMAS_EVENT:
			g_ChristMasEvent.GTH_Process_ChristMas_Event();
			break;

					
			
		case CC_POSTSYSTEM:
			g_PostFunc.GTH_ProcessMessage_PostSystem();
			break;
			
		case CC_MAILBOX:
			g_PostFunc.GTH_ProcessMessage_MailBox();
			break;		
			
			
			
		case CC_REQUEST_USE_EPACK_CHARG_SOUL:
			GTH_ProcessMessage_Request_Epack_ChargSoul();
			break;		
		
		
		case CC_EXTEND_WORLDCHATTING_COUNT :
			GTH_ProcessMessage_WorldChattingCount();
			break;			

		
		case CC_WORLD_CHANGE:
			GTH_WorldChangeSystem_Process();
			break;
		
			
			
	}
}






void GTH_ProcessMessage_Client_Extend_Second()
{

	enum enumExtendSecondPacket SecoundPacket;
	SecoundPacket = static_cast <enum enumExtendSecondPacket>(MSG_ReadShort());


	switch(SecoundPacket) 
	{	
	case HELPER_SYSTEM:
		g_HelperManager.ProcessMessage_Helper();
		return;

	
	case EXP_EVENT:
		g_ExpEventMng.ProcessExpEvent();
		return;

	case GMsystem:
		gcpGMCtrl->Decoder(g_curPC);
		return;
	

	
		
	
	}




	
	
	
	
	

	if(NULL ==  g_curPC) return;
	if( !g_curPC->ready ) return;


	switch(SecoundPacket) 
	{
	case GIFTCARD:
		gcpGiftCard->Decoder(g_curPC);
		return;

	case WEATHER_SYSTEM:
		g_WeatherSystem.GTH_Process_Weather_System();
		break;	

	
	case ITEM_REPAIR:
		GTH_ProcessMessage_RepairItemConfirm();
		break;

	
	case CASH_MALL_ITEM:
		GTH_DecodeCashMallItem();
		break;

	
	case GAMBLE_SYSTEM:
		GambleSystemDecode();
		break;	

	
	case ITEM_EXTEND_SYSTEM:
		GTH_ItemExtendSystemDecode();
		break;

		
	case AHNLAB:
		{
			if ( NULL != g_cpAhnLab )
			g_cpAhnLab->GTH_ProcessMessageAhnLabMessage(g_curPC);	
		}
		break;


	
	case NEW_DANBATTLE:
		GTH_DecodeDanBattleSystem();
		break;
	


	
	

	
	default:
		return;
	}
}




