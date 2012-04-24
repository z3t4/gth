



#if !defined(AFX_CPREMIUMPLAYERCTRL_H__630C6208_F656_4AB1_91F4_4BEFF42A5CA3__INCLUDED_)
#define AFX_CPREMIUMPLAYERCTRL_H__630C6208_F656_4AB1_91F4_4BEFF42A5CA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 




class CPremiumPlayerCtrl  
{
	public:
		enum enumMapIdx{
			Ariyan=0,
			Light_Lake=1,
			Aurora_Canyon=2,
			Arth_Crystal_Mine=3,
			The_Temple_of_Artzran=4,
			Bazmo_Fortress=5,
			Illusion_Temple=6,
			Dragon_Labyrinth=7,
			Dark_Lake=8,
			Sien=9,
			Sien_Canyon=10,
			Sodo=11,
			Laznan_Level1=12,
			Laznan_Level2=13,
			Laznan_Level3=14,
			Laznan_Level4=15,
			Laznan_Level5=16,
			Judgment_Canyon=17,
			Temptation_Corridor=18,
			Forgetting_Corridor=19,
			Dan_Colosseum=20,
			Gyorun_Colosseum=21,
		};

		
		struct i3client_t::tagPremiumInfo m_PremiumLevelTable[i3client_t::tagPremiumInfo::enumMeberShipType::Max];
			


	private:
		BOOL m_bOK;
		void Init_PremiumLevelTable_Free(void);
		void Init_PremiumLevelTable_Pay(void);

		void LoadIniFile(const char* szpIniFileName,struct i3client_t::tagPremiumInfo* pPremium);
		void SaveIniFile(const char* szpIniFileName,const struct i3client_t::tagPremiumInfo* pPremium);


	public:
		BOOL isOK(void) const;
		void SetData(enum i3client_t::tagPremiumInfo::enumMeberShipType MemberShipType,i3client_t* pPlayerRecord) const;
		BOOL isAccessMap(const int MapIdx,const i3client_t* pPlayerRecord) const;
		BOOL LoadIniFile(void);

	public:
		void Test_SetPremiumPlayerInfo(i3client_t* pPlayerRecord) const;


	
	public:
		CPremiumPlayerCtrl();
		virtual ~CPremiumPlayerCtrl();

};

#endif 
