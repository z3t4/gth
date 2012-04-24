



#include "global.h"
#include "CPremiumPlayerCtrl.h"




CPremiumPlayerCtrl::CPremiumPlayerCtrl()
{
	m_bOK=FALSE;



	
	if(FALSE == LoadIniFile()) return;



	

	m_bOK=TRUE;
}







CPremiumPlayerCtrl::~CPremiumPlayerCtrl()
{

}





BOOL CPremiumPlayerCtrl::isOK(void) const
{
	return m_bOK; 

}




BOOL CPremiumPlayerCtrl::LoadIniFile(void)
{
	char szWorkDir[_MAX_PATH];
	::GetCurrentDirectory(_MAX_PATH,szWorkDir);	

	char szIniFileName[_MAX_PATH];
	
	wsprintf(szIniFileName,"%s\\free.Premium.ini",szWorkDir);	
	m_PremiumLevelTable[playerCharacter_t::tagPremiumInfo::enumMeberShipType::Free].iMemberShipType=
		playerCharacter_t::tagPremiumInfo::enumMeberShipType::Free;

	LoadIniFile(szIniFileName,&m_PremiumLevelTable[playerCharacter_t::tagPremiumInfo::enumMeberShipType::Free]);
	

	wsprintf(szIniFileName,"%s\\pay.Premium.ini",szWorkDir);	
	m_PremiumLevelTable[playerCharacter_t::tagPremiumInfo::enumMeberShipType::Pay].iMemberShipType=
		playerCharacter_t::tagPremiumInfo::enumMeberShipType::Pay;

	LoadIniFile(szIniFileName,&m_PremiumLevelTable[playerCharacter_t::tagPremiumInfo::enumMeberShipType::Pay]);
	

	return TRUE;
}




BOOL CPremiumPlayerCtrl::
isAccessMap(const int MapIdx,const playerCharacter_t* pPlayerRecord) const
{
	if(MapIdx < 0) return FALSE;
	if(MapIdx > playerCharacter_t::tagPremiumInfo::maxmap_number) return FALSE;

	return pPlayerRecord->PremiumInfo.Map[MapIdx].Cur.bAllow;
}





void CPremiumPlayerCtrl::

SetData(enum playerCharacter_t::tagPremiumInfo::enumMeberShipType MemberShipType,playerCharacter_t* pPlayerRecord) const

{
	switch(MemberShipType){
		default:
		case playerCharacter_t::tagPremiumInfo::enumMeberShipType::Free:
			memcpy(&(pPlayerRecord->PremiumInfo),
				   &(m_PremiumLevelTable[playerCharacter_t::tagPremiumInfo::enumMeberShipType::Free]),
				   sizeof(struct playerCharacter_t::tagPremiumInfo));
			return;
		case playerCharacter_t::tagPremiumInfo::enumMeberShipType::Pay:
			memcpy(&(pPlayerRecord->PremiumInfo),
				   &(m_PremiumLevelTable[playerCharacter_t::tagPremiumInfo::enumMeberShipType::Pay]),
				   sizeof(struct playerCharacter_t::tagPremiumInfo));
			return;
	}
}





void CPremiumPlayerCtrl::Init_PremiumLevelTable_Pay(void)
{
	struct playerCharacter_t::tagPremiumInfo* pPremium = &(m_PremiumLevelTable[playerCharacter_t::tagPremiumInfo::enumMeberShipType::Pay]);

	memset(pPremium,0x00,sizeof(struct playerCharacter_t::tagPremiumInfo));



	
	pPremium->iMemberShipType = playerCharacter_t::tagPremiumInfo::enumMeberShipType::Pay;

	
	for(int idx=0; idx < playerCharacter_t::tagPremiumInfo::maxmap_number; idx++){

		pPremium->Map[idx].Default.bAllow = TRUE;
		pPremium->Map[idx].Default.iRemainSecond=0x7fffffff;
		
		pPremium->Map[idx].Default.bDecreseRemainSecond=FALSE;									
		pPremium->Map[idx].Default.bDecreseAllRemainSecond = FALSE;

						
		pPremium->Map[idx].Default.fAddExpRatio = 1.0f;											
		pPremium->Map[idx].Default.fAddGenExpRatio = 1.0f;										
		pPremium->Map[idx].Default.fAddGenCapabilityRatio=1.0f;									
		pPremium->Map[idx].Default.fAddItemDropRatio = 1.0f;										
		pPremium->Map[idx].Default.fAddNarkRatio = 1.0f;									

		pPremium->Map[idx].Cur = pPremium->Map[idx].Default;
	}



	
	pPremium->Gamble.fAddPriceRatio = 1.0f;												
	for(idx=0; idx < playerCharacter_t::tagPremiumInfo::tagGamble::maxnum_apperance_item_group; idx++){
		pPremium->Gamble.ItemGroupArray[idx].bApperance=TRUE;
	}






	
	pPremium->ItemOptionUpgrade.iMaxlevel = 10;			
	pPremium->ItemOptionUpgrade.fAddPriceRatio = 1.0f;	


	
	pPremium->Die.fExpRatio = 1.0f;												
	pPremium->Die.fNakRatio = 1.0f;												



	
	pPremium->bCreatePremiumBooth = FALSE;

	
	pPremium->iMaxLevelItemCraft = 10;
	
	pPremium->fItemPrecocityTimeRatio = 1.0f;

	
	pPremium->bItemRecycle = TRUE;

	
	pPremium->GonyounPracticeBattle.lDateV2 = time(NULL);
	pPremium->GonyounPracticeBattle.iMaxCount = 5;
	pPremium->GonyounPracticeBattle.iDecreseCount = pPremium->GonyounPracticeBattle.iMaxCount;

	
	pPremium->WorldChatting.lDate = 0;
	pPremium->WorldChatting.iMaxCount = 10000;
	pPremium->WorldChatting.iDecreaseCount = pPremium->WorldChatting.iMaxCount;


	
	pPremium->Status.iDefaultInitCount= 1;
	pPremium->Status.iInitCount=1000;

	
	pPremium->ItemDurability.fRepareCostRate = 1.0f;
	pPremium->ItemDurability.iPrepareRequirdurability = 0;	
	

}

char* GetFLOATString(const float fValue)
{
	static char str[20];
	sprintf(str,"%3.02f",fValue);
	return str;
}



char* GetINTString(const int iValue)
{
	static char str[20];
	wsprintf((char*)str,"%d",iValue);
	return str;
}

char* GetBOOLString(const BOOL bIs)
{
	static char str[2][10]={"YES","NO"};
	if(TRUE == bIs) return str[0];
	return str[1];
}






void CPremiumPlayerCtrl::Init_PremiumLevelTable_Free(void)
{
	char szWorkDir[_MAX_PATH];
	::GetCurrentDirectory(_MAX_PATH,szWorkDir);	

	char szIniFileName[_MAX_PATH];
	wsprintf(szIniFileName,"%s\\free.Premium.ini",szWorkDir);

	char AppName[_MAX_PATH];
	char Session[_MAX_PATH];

	


	struct playerCharacter_t::tagPremiumInfo* pPremium = &(m_PremiumLevelTable[playerCharacter_t::tagPremiumInfo::enumMeberShipType::Free]);

	memset(pPremium,0x00,sizeof(struct playerCharacter_t::tagPremiumInfo));



	
	pPremium->iMemberShipType = playerCharacter_t::tagPremiumInfo::enumMeberShipType::Free;

	
	for(int idx=0; idx < playerCharacter_t::tagPremiumInfo::maxmap_number; idx++){

		wsprintf(AppName,"Map%02d",idx);

		pPremium->Map[idx].Default.bAllow = TRUE;
		::WritePrivateProfileString(AppName,"Allow",GetBOOLString(pPremium->Map[idx].Default.bAllow),szIniFileName);

		pPremium->Map[idx].Default.iRemainSecond=0x7fffffff;
		::WritePrivateProfileString(AppName,"iRemainSecond",GetINTString(pPremium->Map[idx].Default.iRemainSecond),szIniFileName);


		
		pPremium->Map[idx].Default.bDecreseRemainSecond=FALSE;									
		::WritePrivateProfileString(AppName,"bDecreseRemainSecond",GetBOOLString(pPremium->Map[idx].Default.bDecreseRemainSecond),szIniFileName);

		pPremium->Map[idx].Default.bDecreseAllRemainSecond = FALSE;
		::WritePrivateProfileString(AppName,"bDecreseAllRemainSecond",GetBOOLString(pPremium->Map[idx].Default.bDecreseAllRemainSecond),szIniFileName);


						
		pPremium->Map[idx].Default.fAddExpRatio = 1.0f;											
		::WritePrivateProfileString(AppName,"fAddExpRatio",GetFLOATString(pPremium->Map[idx].Default.fAddExpRatio),szIniFileName);





		pPremium->Map[idx].Default.fAddGenExpRatio = 1.0f;										
		::WritePrivateProfileString(AppName,"fAddGenExpRatio",GetFLOATString(pPremium->Map[idx].Default.fAddGenExpRatio),szIniFileName);

		pPremium->Map[idx].Default.fAddGenCapabilityRatio=1.0f;									
		::WritePrivateProfileString(AppName,"fAddGenCapabilityRatio",GetFLOATString(pPremium->Map[idx].Default.fAddGenCapabilityRatio),szIniFileName);

		pPremium->Map[idx].Default.fAddItemDropRatio = 1.0f;										
		::WritePrivateProfileString(AppName,"fAddItemDropRatio",GetFLOATString(pPremium->Map[idx].Default.fAddItemDropRatio),szIniFileName);

		pPremium->Map[idx].Default.fAddNarkRatio = 1.0f;									
		::WritePrivateProfileString(AppName,"fAddNarkRatio",GetFLOATString(pPremium->Map[idx].Default.fAddNarkRatio),szIniFileName);

		pPremium->Map[idx].Cur = pPremium->Map[idx].Default;
	}

	pPremium->Map[Light_Lake].Cur.bAllow=FALSE;



	wsprintf(AppName,"Gameble");
	
	pPremium->Gamble.fAddPriceRatio = 1.0f;												
	::WritePrivateProfileString(AppName,"fAddPriceRatio",GetFLOATString(pPremium->Gamble.fAddPriceRatio),szIniFileName);
	for(idx=0; idx < playerCharacter_t::tagPremiumInfo::tagGamble::maxnum_apperance_item_group; idx++){
		wsprintf(Session,"Group%02d",idx);
		pPremium->Gamble.ItemGroupArray[idx].bApperance=TRUE;
		::WritePrivateProfileString(AppName,Session,GetBOOLString(pPremium->Gamble.ItemGroupArray[idx].bApperance),szIniFileName);
	}





	wsprintf(AppName,"ItemOptionUpgrade");
	
	pPremium->ItemOptionUpgrade.iMaxlevel = 10;			
	::WritePrivateProfileString(AppName,"iMaxlevel",GetINTString(pPremium->ItemOptionUpgrade.iMaxlevel),szIniFileName);

	pPremium->ItemOptionUpgrade.fAddPriceRatio = 1.0f;	
	::WritePrivateProfileString(AppName,"fAddPriceRatio",GetFLOATString(pPremium->ItemOptionUpgrade.fAddPriceRatio),szIniFileName);




	wsprintf(AppName,"Die");
	
	pPremium->Die.fExpRatio = 1.0f;												
	::WritePrivateProfileString(AppName,"fExpRatio",GetFLOATString(pPremium->Die.fExpRatio),szIniFileName);
	pPremium->Die.fNakRatio = 1.0f;												
	::WritePrivateProfileString(AppName,"fNakRatio",GetFLOATString(pPremium->Die.fNakRatio),szIniFileName);



	
	pPremium->bCreatePremiumBooth = FALSE;
	::WritePrivateProfileString("PremiumBooth","Use",GetBOOLString(pPremium->bCreatePremiumBooth),szIniFileName);

	
	pPremium->iMaxLevelItemCraft = 10;
	::WritePrivateProfileString("ItemCraft","iMaxLevel",GetINTString(pPremium->iMaxLevelItemCraft),szIniFileName);



	
	pPremium->fItemPrecocityTimeRatio = 1.0f;
	::WritePrivateProfileString("ItemPrecocity","TimeRatio",GetFLOATString(pPremium->fItemPrecocityTimeRatio),szIniFileName);





	
	pPremium->bItemRecycle = TRUE;
	::WritePrivateProfileString("ItemRecycle","Use",GetBOOLString(pPremium->bItemRecycle),szIniFileName);

	
	pPremium->GonyounPracticeBattle.lDateV2 = time(NULL);
	pPremium->GonyounPracticeBattle.iMaxCount = 5;
	::WritePrivateProfileString("GonyounPracticeBattle","iMaxCount",GetINTString(pPremium->GonyounPracticeBattle.iMaxCount),szIniFileName);
	pPremium->GonyounPracticeBattle.iDecreseCount = pPremium->GonyounPracticeBattle.iMaxCount;

	
	pPremium->WorldChatting.lDate = time(NULL);
	pPremium->WorldChatting.iMaxCount = 10;
	::WritePrivateProfileString("WorldChatting","iMaxCount",GetINTString(pPremium->WorldChatting.iMaxCount),szIniFileName);
	pPremium->WorldChatting.iDecreaseCount = pPremium->WorldChatting.iMaxCount;


	
	pPremium->Status.iDefaultInitCount= 1;
	::WritePrivateProfileString("Status","iDefaultInitCount",GetINTString(pPremium->Status.iDefaultInitCount),szIniFileName);
	pPremium->Status.iInitCount=1000;
	::WritePrivateProfileString("Status","iInitCount",GetINTString(pPremium->Status.iInitCount),szIniFileName);

	
	pPremium->ItemDurability.fRepareCostRate = 1.0f;
	::WritePrivateProfileString("Itemdurability","fAddRepareCostRate",GetINTString(pPremium->ItemDurability.fRepareCostRate),szIniFileName);
	pPremium->ItemDurability.iPrepareRequirdurability = 0;
	::WritePrivateProfileString("Itemdurability","iPrepareRequirdurability",GetINTString(pPremium->ItemDurability.iPrepareRequirdurability),szIniFileName);	
	


}



BOOL GetBOOL(const char* pStr)
{
	if(0 == strcmp(pStr,"YES")) return TRUE;
	if(0 == strcmp(pStr,"yes")) return TRUE;
	if(0 == strcmp(pStr,"Yes")) return TRUE;

	return FALSE;
}

int GetINT(const char* pStr)
{
	return atoi(pStr);
}


float GetFLOAT(const char* pStr)
{
	return atof(pStr);
}




void CPremiumPlayerCtrl::
LoadIniFile(const char* szpIniFileName,struct playerCharacter_t::tagPremiumInfo* pPremium)
{


	char AppName[_MAX_PATH];
	char inStr[_MAX_PATH];
	char Session[_MAX_PATH];	

	
	

	
	
	for(int idx=0; idx < playerCharacter_t::tagPremiumInfo::maxmap_number; idx++){

		wsprintf(AppName,"Map%02d",idx);

		
		::GetPrivateProfileString(AppName,"Allow","YES",inStr,_MAX_PATH,szpIniFileName);
		pPremium->Map[idx].Default.bAllow = GetBOOL(inStr);

		
		::GetPrivateProfileString(AppName,"iRemainSecond","2147483647",inStr,_MAX_PATH,szpIniFileName);
		pPremium->Map[idx].Default.iRemainSecond=GetINT(inStr);

		
		::GetPrivateProfileString(AppName,"bDecreseRemainSecond","NO",inStr,_MAX_PATH,szpIniFileName);
		pPremium->Map[idx].Default.bDecreseRemainSecond=GetBOOL(inStr);

		
		::GetPrivateProfileString(AppName,"bDecreseAllRemainSecond","NO",inStr,_MAX_PATH,szpIniFileName);
		pPremium->Map[idx].Default.bDecreseAllRemainSecond = GetBOOL(inStr);

		
		::GetPrivateProfileString(AppName,"fAddExpRatio","1.0f",inStr,_MAX_PATH,szpIniFileName);
		pPremium->Map[idx].Default.fAddExpRatio = GetFLOAT(inStr);

		
		::GetPrivateProfileString(AppName,"fAddGenExpRatio","1.0f",inStr,_MAX_PATH,szpIniFileName);
		pPremium->Map[idx].Default.fAddGenExpRatio = GetFLOAT(inStr);

		
		::GetPrivateProfileString(AppName,"fAddGenCapabilityRatio","1.0f",inStr,_MAX_PATH,szpIniFileName);
		pPremium->Map[idx].Default.fAddGenCapabilityRatio=GetFLOAT(inStr);

		
		::GetPrivateProfileString(AppName,"fAddItemDropRatio","1.0f",inStr,_MAX_PATH,szpIniFileName);
		pPremium->Map[idx].Default.fAddItemDropRatio =GetFLOAT(inStr);

		
		::GetPrivateProfileString(AppName,"fAddNarkRatio","1.0f",inStr,_MAX_PATH,szpIniFileName);
		pPremium->Map[idx].Default.fAddNarkRatio = GetFLOAT(inStr);
		

		pPremium->Map[idx].Cur = pPremium->Map[idx].Default;
	}




	
	wsprintf(AppName,"Gameble");
	
	::GetPrivateProfileString(AppName,"fAddPriceRatio","1.0f",inStr,_MAX_PATH,szpIniFileName);
	pPremium->Gamble.fAddPriceRatio = GetFLOAT(inStr);

	for(idx=0; idx < playerCharacter_t::tagPremiumInfo::tagGamble::maxnum_apperance_item_group; idx++){
		wsprintf(Session,"Group%02d",idx);
		
		::GetPrivateProfileString(AppName,Session,"YES",inStr,_MAX_PATH,szpIniFileName);
		pPremium->Gamble.ItemGroupArray[idx].bApperance=GetBOOL(inStr);
	}



	wsprintf(AppName,"ItemOptionUpgrade");
	
	
	::GetPrivateProfileString(AppName,"iMaxlevel","10",inStr,_MAX_PATH,szpIniFileName);
	pPremium->ItemOptionUpgrade.iMaxlevel = GetINT(inStr);


	
	::GetPrivateProfileString(AppName,"fAddPriceRatio","1.0f",inStr,_MAX_PATH,szpIniFileName);
	pPremium->ItemOptionUpgrade.fAddPriceRatio = GetFLOAT(inStr);	

	
	
	wsprintf(AppName,"Die");
	
	::GetPrivateProfileString(AppName,"fExpRatio","1.0f",inStr,_MAX_PATH,szpIniFileName);
	pPremium->Die.fExpRatio = GetFLOAT(inStr);
	
	
	::GetPrivateProfileString(AppName,"fNakRatio","1.0f",inStr,_MAX_PATH,szpIniFileName);
	pPremium->Die.fNakRatio = GetFLOAT(inStr);
	



	
	
	::GetPrivateProfileString("PremiumBooth","Use","FALSE",inStr,_MAX_PATH,szpIniFileName);
	pPremium->bCreatePremiumBooth = GetBOOL(inStr);
	


	
	
	::GetPrivateProfileString("ItemCraft","iMaxLevel","10",inStr,_MAX_PATH,szpIniFileName);
	pPremium->iMaxLevelItemCraft = GetINT(inStr);
	


	
	
	::GetPrivateProfileString("ItemPrecocity","TimeRatio","1.0f",inStr,_MAX_PATH,szpIniFileName);
	pPremium->fItemPrecocityTimeRatio = GetFLOAT(inStr);
	


	
	
	::GetPrivateProfileString("ItemRecycle","Use","YES",inStr,_MAX_PATH,szpIniFileName);
	pPremium->bItemRecycle = GetBOOL(inStr);

	
	pPremium->GonyounPracticeBattle.lDateV2 = time(NULL);
	
	::GetPrivateProfileString("GonyounPracticeBattle","iMaxCount","5",inStr,_MAX_PATH,szpIniFileName);
	pPremium->GonyounPracticeBattle.iMaxCount = GetINT(inStr);	
	pPremium->GonyounPracticeBattle.iDecreseCount = pPremium->GonyounPracticeBattle.iMaxCount;



	
	pPremium->WorldChatting.lDate = time(NULL);
	
	::GetPrivateProfileString("WorldChatting","iMaxCount","10",inStr,_MAX_PATH,szpIniFileName);
	pPremium->WorldChatting.iMaxCount = GetINT(inStr);	
	pPremium->WorldChatting.iDecreaseCount = pPremium->WorldChatting.iMaxCount;




	
	
	::GetPrivateProfileString("Status","iDefaultInitCount","1",inStr,_MAX_PATH,szpIniFileName);
	pPremium->Status.iDefaultInitCount= GetINT(inStr);
	
	
	::GetPrivateProfileString("Status","iInitCount","1000",inStr,_MAX_PATH,szpIniFileName);
	pPremium->Status.iInitCount=GetINT(inStr);

	
	::GetPrivateProfileString("ItemDurability","fRepareCostRate","1.0f",inStr,_MAX_PATH,szpIniFileName);
	pPremium->ItemDurability.fRepareCostRate = GetFLOAT(inStr);	
	::GetPrivateProfileString("ItemDurability","iPrepareRequirdurability","0",inStr,_MAX_PATH,szpIniFileName);
	pPremium->ItemDurability.iPrepareRequirdurability = GetINT(inStr);	
	

}








void CPremiumPlayerCtrl::SaveIniFile(const char* szpIniFileName,const struct playerCharacter_t::tagPremiumInfo* pPremium)
{
	char AppName[_MAX_PATH];
	char Session[_MAX_PATH];	


	
	for(int idx=0; idx < playerCharacter_t::tagPremiumInfo::maxmap_number; idx++){
		wsprintf(AppName,"Map%02d",idx);

		
		::WritePrivateProfileString(AppName,"Allow",GetBOOLString(pPremium->Map[idx].Default.bAllow),szpIniFileName);

		
		::WritePrivateProfileString(AppName,"iRemainSecond",GetINTString(pPremium->Map[idx].Default.iRemainSecond),szpIniFileName);
		
		
		::WritePrivateProfileString(AppName,"bDecreseRemainSecond",GetBOOLString(pPremium->Map[idx].Default.bDecreseRemainSecond),szpIniFileName);

		
		::WritePrivateProfileString(AppName,"bDecreseAllRemainSecond",GetBOOLString(pPremium->Map[idx].Default.bDecreseAllRemainSecond),szpIniFileName);

		
		
		::WritePrivateProfileString(AppName,"fAddExpRatio",GetFLOATString(pPremium->Map[idx].Default.fAddExpRatio),szpIniFileName);

		
		::WritePrivateProfileString(AppName,"fAddGenExpRatio",GetFLOATString(pPremium->Map[idx].Default.fAddGenExpRatio),szpIniFileName);

		
		::WritePrivateProfileString(AppName,"fAddGenCapabilityRatio",GetFLOATString(pPremium->Map[idx].Default.fAddGenCapabilityRatio),szpIniFileName);

		
		::WritePrivateProfileString(AppName,"fAddItemDropRatio",GetFLOATString(pPremium->Map[idx].Default.fAddItemDropRatio),szpIniFileName);

		
		::WritePrivateProfileString(AppName,"fAddNarkRatio",GetFLOATString(pPremium->Map[idx].Default.fAddNarkRatio),szpIniFileName);

		
	}





	wsprintf(AppName,"Gameble");
	
	
	::WritePrivateProfileString(AppName,"fAddPriceRatio",GetFLOATString(pPremium->Gamble.fAddPriceRatio),szpIniFileName);
	for(idx=0; idx < playerCharacter_t::tagPremiumInfo::tagGamble::maxnum_apperance_item_group; idx++){
		wsprintf(Session,"Group%02d",idx);
		
		::WritePrivateProfileString(AppName,Session,GetBOOLString(pPremium->Gamble.ItemGroupArray[idx].bApperance),szpIniFileName);
	}


	wsprintf(AppName,"ItemOptionUpgrade");
	
	
	::WritePrivateProfileString(AppName,"iMaxlevel",GetINTString(pPremium->ItemOptionUpgrade.iMaxlevel),szpIniFileName);
	
	::WritePrivateProfileString(AppName,"fAddPriceRatio",GetFLOATString(pPremium->ItemOptionUpgrade.fAddPriceRatio),szpIniFileName);




	wsprintf(AppName,"Die");
	
	
	::WritePrivateProfileString(AppName,"fExpRatio",GetFLOATString(pPremium->Die.fExpRatio),szpIniFileName);
	
	::WritePrivateProfileString(AppName,"fNakRatio",GetFLOATString(pPremium->Die.fNakRatio),szpIniFileName);



	
	
	::WritePrivateProfileString("PremiumBooth","Use",GetBOOLString(pPremium->bCreatePremiumBooth),szpIniFileName);

	
	
	::WritePrivateProfileString("ItemCraft","iMaxLevel",GetINTString(pPremium->iMaxLevelItemCraft),szpIniFileName);



	
	
	::WritePrivateProfileString("ItemPrecocity","TimeRatio",GetFLOATString(pPremium->fItemPrecocityTimeRatio),szpIniFileName);





	
	
	::WritePrivateProfileString("ItemRecycle","Use",GetBOOLString(pPremium->bItemRecycle),szpIniFileName);

	
	
	
	::WritePrivateProfileString("GonyounPracticeBattle","iMaxCount",GetINTString(pPremium->GonyounPracticeBattle.iMaxCount),szpIniFileName);
	

	
	
	
	::WritePrivateProfileString("WorldChatting","iMaxCount",GetINTString(pPremium->WorldChatting.iMaxCount),szpIniFileName);
	


	
	
	::WritePrivateProfileString("Status","iDefaultInitCount",GetINTString(pPremium->Status.iDefaultInitCount),szpIniFileName);
	
	::WritePrivateProfileString("Status","iInitCount",GetINTString(pPremium->Status.iInitCount),szpIniFileName);
}
