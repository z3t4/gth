

#ifndef _TAGQUERYSET_H_
#define _TAGQUERYSET_H_

#define QUERYSIZE 4096

typedef struct tagQuerySet
{
	public:

		struct tagQuestStatus{
			questStatus_t		questStatus[MAX_COUNT_QUEST_STATUS];
			int					questNumber;						
		};

		struct tagQuest{
			questVariable_t		questVar[MAX_COUNT_QUEST_VARIABLE];
			questInventory_t	questInventory[MAX_COUNT_QUEST_INVENTORY];
		};



		struct tagCharactor{
			public:
				struct tagGonryunBattle{
					int					m_iwin;						
					int					m_idefeat;					
					int					m_idraw;						
					long				lDateV2;		

					int					iBattleGUID;
					int					iTournamentLevelIdx;
					int					manor;
				};

				

			public:

				char				name[NAMESTRING+1];		
				int					pcClass;				
				int					pcJob;
				int					skinPart[DEFAULT_SKINPART_NUMBER];
				char				rank[NAMESTRING+1];		
				int					rankPoint;				
				int					age;					
				int					worldIdx;					
				vec3_t				position;					
				vec3_t				angles;					
				int					level;					
				__int64				exp;					
				int					increaseExpRate;		
				int					selectedGen;			
				int					genLevel;
				__int64				genExp;
				int					increaseGenExpRate;
				int					genCapability[GEN_NUMBER];
				int					force; 			
				int					soulpower;		
				int					agility; 		
				int					vitality;		
				int					bonusStatusPoint;	
				float				curRA;				
				float				curSA;				
				int					curChargeNSE;		
				int					curChargeSE;		
				float				curChargeRA;		
				float				curChargeSA;		
				int					depotSE;
				int					curWeapon;
				int					bindingIdx;				
				int					itemNumber;
				int					inventory[MAX_INVENTORY_SIZE];		
				int					depot[MAX_DEPOT_SIZE];
				int					equipment[MAX_EQUIPMENT];		
				int					mouseInventory; 
				int					bonusSkillPoint;
				int					skillVariable[MAX_NUMBER_OF_SKILL_VARIABLE];
				int					hotkeyType[MAX_NUMBER_OF_HOTKEY];
				int					hotkeyIdx[MAX_NUMBER_OF_HOTKEY];
				char				organizerName[NAMESTRING+1];			
				int					organizeServer;						
				int					organizeTime;						
				int					questPoint;
				int					penaltyTime;			
				int					levelUpTime;			
				int					genLevelUpTime;			
				int					gameMaster;				
				int					flagChangeServer;		
				int					precocityInventory[MAX_PRECOCITY_SIZE];
				int					precocityTime[MAX_PRECOCITY_SIZE];
				int					chaosPoint;				
				short				guildIdx;
				int					m_igonryunUseCount;			
				int					iDefaultInitCount;	
				BOOL				m_bHide;		
				
				int					nAccKillPoint;
				
				
				int					GuildDeconstructionTime;
				int					GuildsecedeTime;
				
				

				struct tagGonryunBattle GonryunBattle;
				struct playerCharacter_t::tagGM GM;
		};


		struct tagPremium{
			public:
				struct tagWorldChatting{
					long lDate;
					int  iDecreaseCount;
				};

			public:				
				enum playerCharacter_t::tagPremiumInfo::enumMeberShipType iMemberShipType;
				int  iRemainSecond;
				struct tagWorldChatting WorldChatting;
		};
		
		struct tagQUERY_SAVECHA_PACKAGE{
			int packageIdx;
		};
			

		struct tagPostSystem_LoadPackageCnt{
			int iUnReadedNum;
			int iReadedNum;
		};

		struct tagSaveGameVar{
			enum enumWhySaveGame why;			
		};

		struct tagGiftCard{
			enum CGiftCard::enumCode code;
			int                      type;
			int						 Cnt;
		};

		struct tagGiftCardItemList{
			public:
				struct tagRecord{
					int ItemTableIdx;
					int Cnt;
					int Rate;
				};
			public:
				struct tagRecord* pRecord;
				int  Cnt;
		};

		struct tagPostSystem_DeletePackage{
			int PostPackageIdx;

		};

		struct tagServerStatus{
			int Value;
		};

		
		struct tageDanBattle{			
			enum tagGCPacket_NewDanBattle::enumDanBattleInsertcode code;
		};

		struct tageDanBattleGuildInfo{
			int WinPoint;
			int LosePoint;
			int AccPoint;
		};



	public:
		int		ownerIdx;	
		char	ownerID[IDSTRING+1];
		int		queryType;
		int     parameter;
		char	query[QUERYSIZE+1];
		HSTMT	hStmt;
		HDBC	hDbc;
		RETCODE	retCode;
		void	*retStructVariable;

		union{
			void*  pData;
			struct tagPremium Premium;
			
			struct tagQUERY_SAVECHA_PACKAGE QUERY_SAVECHA_PACKAGE;	
			struct tagSaveGameVar SaveGameVar;

			
			struct tagGiftCard         GiftCard;
			struct tagGiftCardItemList GiftCardItemList;

			
			struct tagPostSystem_LoadPackageCnt PostSystem_LoadPackageCnt;
			struct tagPostSystem_DeletePackage  PostSystem_DeletePackage;

			
			struct tagCharactor*       pCharactor;
			struct tagQuest*           pQuest;
			struct tagQuestStatus*     pQuestStatus;

			
			struct tagServerStatus     ServerStatus;

			
			struct tageDanBattle	   Danbattle;
			struct tageDanBattleGuildInfo DanBattleGuildInfo;
		};
	

} querySet_t;



#endif


