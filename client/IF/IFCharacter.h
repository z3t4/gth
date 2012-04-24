



#if !defined(AFX_IFCHARWIN_H__122077A2_7DF6_4EE3_ACE5_EF9AC56C7A22__INCLUDED_)
#define AFX_IFCHARWIN_H__122077A2_7DF6_4EE3_ACE5_EF9AC56C7A22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 





#define MAX_COUNT_IF_CHAR_WIN_LINE	5

enum
{
	IT_CHAR_WIN_TITLE			=	0,
	IT_CHAR_WIN_NAME_U,
	IT_CHAR_WIN_AGE,
	IT_CHAR_WIN_AGE_U,
	IT_CHAR_WIN_LEVEL,
	IT_CHAR_WIN_LEVEL_U,
	IT_CHAR_WIN_GENLEVEL,
	IT_CHAR_WIN_GENLEVEL_U,
	IT_CHAR_WIN_FAME,
	IT_CHAR_WIN_FAME_U,
	IT_CHAR_WIN_RA,
	IT_CHAR_WIN_SA,
	IT_CHAR_WIN_RA_U,
	IT_CHAR_WIN_SA_U,
	IT_CHAR_WIN_EXP,
	IT_CHAR_WIN_GENEXP,
	IT_CHAR_WIN_EXP_U,
	IT_CHAR_WIN_GENEXP_U,
	IT_CHAR_WIN_GEN_ATT,
	IT_CHAR_WIN_GEN_GRADE_0,
	IT_CHAR_WIN_GEN_GRADE_1,
	IT_CHAR_WIN_GEN_GRADE_2,
	IT_CHAR_WIN_GEN_GRADE_3,
	IT_CHAR_WIN_GEN_GRADE_U_0,
	IT_CHAR_WIN_GEN_GRADE_U_1,
	IT_CHAR_WIN_GEN_GRADE_U_2,
	IT_CHAR_WIN_GEN_GRADE_U_3,
	IT_CHAR_WIN_ATK_ATT,
	IT_CHAR_WIN_ATK,
	IT_CHAR_WIN_ATK_U,
	IT_CHAR_WIN_MAG,
	IT_CHAR_WIN_MAG_U,
	IT_CHAR_WIN_DEF,
	IT_CHAR_WIN_DEF_U,
	IT_CHAR_WIN_PRES,
	IT_CHAR_WIN_PRES_U,
	IT_CHAR_WIN_NRES,
	IT_CHAR_WIN_NRES_U,
	IT_CHAR_WIN_ATR,
	IT_CHAR_WIN_ATR_U,
	IT_CHAR_WIN_AVO,
	IT_CHAR_WIN_AVO_U,
	IT_CHAR_WIN_ATS,
	IT_CHAR_WIN_ATS_U,
	IT_CHAR_WIN_VEL,
	IT_CHAR_WIN_VEL_U,
	IT_CHAR_WIN_CRI,
	IT_CHAR_WIN_CRI_U,
	IT_CHAR_WIN_STAT,
	IT_CHAR_WIN_STAT_POINT,
	IT_CHAR_WIN_STAT_POINT_U,
	IT_CHAR_WIN_FOR_POINT,
	IT_CHAR_WIN_FOR_POINT_U,
	IT_CHAR_WIN_AGI_POINT,
	IT_CHAR_WIN_AGI_POINT_U,
	IT_CHAR_WIN_SOU_POINT,
	IT_CHAR_WIN_SOU_POINT_U,
	IT_CHAR_WIN_VIT_POINT,
	IT_CHAR_WIN_VIT_POINT_U,		
	
	NUM_IT_CHAR_WIN
};

class CIFCharWin	: public CIFWindow
{
public:
	
	
	CIFText			*m_textCtl[NUM_IT_CHAR_WIN];
	
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;
	CIFControl		*m_genAttBar;
	CIFControl		*m_atkAttBar;
	CIFControl		*m_statAttBar;
	CIFControl		*m_genMarkSelection[4];
	CIFControl		*m_genMark[4];
	CIFControl		*m_winLine[MAX_COUNT_IF_CHAR_WIN_LINE];

	
	CIFBarCtl		*m_raBar;
	CIFBarCtl		*m_saBar;
	CIFBarCtl		*m_expBar;
	CIFBarCtl		*m_genexpBar;

	
	CIFButtonCtl	*m_closeBtn;
	CIFButtonCtl	*m_statsBtn[4];

	CIFCharWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFCharWin();
	
	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
};





enum
{
	IT_NAME_WIN_LEVEL			=	0,
	IT_NAME_WIN_LEVEL_U,
	IT_NAME_WIN_NAME_U,
	IT_NAME_WIN_RA_U,
	IT_NAME_WIN_SA_U,
	IT_NAME_WIN_GENLEVEL,
	IT_NAME_WIN_GENLEVEL_U,
	IT_NAME_WIN_EXP_TEXT_U,
	IT_NAME_WIN_GENEXP_TEXT_U,

	IT_TARGET_WIN_NAME_U,

	NUM_IT_NAME_WIN
};

class CIFNameWin : public CIFWindow
{
public:

	
	CIFText			*m_textCtl[NUM_IT_NAME_WIN];
	
	CIFBarCtl		*m_raBar;
	CIFBarCtl		*m_saBar;
	CIFBarCtl		*m_expBar;
	CIFBarCtl		*m_genexpBar;

	
	CIFControl		*m_targetWin;
	CIFBarCtl		*m_targetBar;
	CIFBarCtl		*m_castingBar;

	
	CIFControl		*m_skillIconOutlineCtl;
	
	
	CIFControl		*m_destroyUpper;
	CIFControl		*m_destroyLower;
	CIFControl		*m_destroyBoots;
	CIFControl		*m_destroyGloves;
	CIFControl		*m_destroyProtector;
	CIFControl		*m_destroyWeapon;
	
	CIFControl		*m_leftSelectCharSphere;
	CIFControl		*m_rightSelectCharSphere;


	int				m_affectSkillIndex[MAX_AFFECT];
	float			m_affectTime[MAX_AFFECT];
	int				m_affectTwinkle[MAX_AFFECT];
	float			m_affectTwinkleTime[MAX_AFFECT];
	
	float			m_affectTwinkleResetTime[MAX_AFFECT];
	int				m_numAffectSkill;

	CIFNameWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFNameWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		AffectProcess( int affectNumber, int* affectIdx );
};





enum
{
	IT_HOT_WIN_PAGE_U		=	0,
	IT_HOT_WIN_ICON_U,

	NUM_IT_HOT_WIN
};

class CIFHotWin  : public CIFWindow
{
public:

	CIFText					*m_textCtl[NUM_IT_HOT_WIN];
	
	CIFButtonCtl			*m_prevBtn;
	CIFButtonCtl			*m_nextBtn;
	CIFButtonCtl			*m_changeBtn;

	int						m_curPage;

	int						m_clickedIdx;

	CIFHotWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFHotWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		HotKeyProcess( int pos );

};





enum
{
	IT_SKILL_WIN_TITLE		=	0,
	IT_SKILL_WIN_TAB_BASIC_SKILL,
	IT_SKILL_WIN_TAB_GEN_SKILL,
	IT_SKILL_WIN_TAB_AWAKEN_SKILL,
	IT_SKILL_WIN_INVEN_POS_0,
	IT_SKILL_WIN_INVEN_POS_1,

	NUM_IT_SKILL_WIN
};

typedef struct 
{
	int			skillTableIdx;
	int			learnIdx;
} skillWinInfo_t;

class CIFSkillWin  : public CIFWindow
{

public:
	CIFSkillWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFSkillWin();

	CIFText				*m_textCtl[NUM_IT_SKILL_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBottom;
	CIFControl			*m_tabBar;
	CIFControl			*m_tabBasicSkillLine;
	CIFControl			*m_tabGenSkillLine;
	CIFControl			*m_invenBar1;
	CIFControl			*m_invenBar2;
	
	CIFButtonCtl		*m_passivePrevBtn;
	CIFButtonCtl		*m_passiveNextBtn;

	CIFButtonCtl		*m_activePrevBtn;
	CIFButtonCtl		*m_activeNextBtn;
	CIFButtonCtl		*m_closeBtn;

	int					m_curType;
	int					m_curPage;
	int					m_curSelect;

	int					m_sortSkillIdxTable[MAX_SKILL_NUMBER];
	int					m_numSortSkillIdxTable;

	skillWinInfo_t		m_genPassiveSkillIdxList[MAX_SKILL_NUMBER];
	int					m_numGenPassiveSkillIdxList;
	skillWinInfo_t		m_genActiveSkillIdxList[MAX_SKILL_NUMBER];
	int					m_numGenActiveSkillIdxList;

	skillWinInfo_t		m_awakenSkillIdxList[MAX_SKILL_NUMBER];
	int					m_numAwakenSkillIdxList;

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void				MakeSortSkillIdxTable();

};





enum
{
	IT_LEARN_WIN_TITLE			=	0,
	IT_LEARN_WIN_LIST,
	IT_LEARN_WIN_INVEN_POS,
	IT_LEARN_WIN_NAME_U,
	IT_LEARN_WIN_INFO_U,
	IT_LEARN_WIN_LEVELUP,

	NUM_IT_LEARN_WIN
};

typedef struct
{
	
	int			idx;
	int			enableLevelUp;
	int			reqGenCapability;
	int			reqGenLevel;
	int			curLevel;

} learnSkillInfo_t;

class CIFLearnWin  : public CIFWindow
{
public:

	CIFText					*m_textCtl[NUM_IT_LEARN_WIN];
	CIFControl				*m_titleBar;
	CIFControl				*m_titleBottom;
	CIFControl				*m_listBar;
	CIFControl				*m_invenBar1;
	CIFControl				*m_invenBar2;
	CIFControl				*m_nameBar;
	CIFControl				*m_infoLine;

	CIFControl				*m_selectBox;


	CIFButtonCtl			*m_prevBtn;
	CIFButtonCtl			*m_nextBtn;
	CIFButtonCtl			*m_levelUpBtn;
	CIFButtonCtl			*m_closeBtn;

	int						m_genType;
	learnSkillInfo_t		m_skillInfo[MAX_NUMBER_OF_SKILL];
	int						m_numSkillInfo;

	int						m_curPage;
	int						m_selectedIdx;

	CIFLearnWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFLearnWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		InitLearnWindow( int genType );
	void		MakeLearnSkillInfo();
};





enum
{
	IT_QUEST_WIN_TITLE			=	0,
	IT_QUEST_WIN_LIST,
	IT_QUEST_WIN_LIST_0_U,
	IT_QUEST_WIN_LIST_1_U,
	IT_QUEST_WIN_LIST_2_U,
	IT_QUEST_WIN_LIST_3_U,
	IT_QUEST_WIN_LIST_4_U,
	IT_QUEST_WIN_ITEM,
	IT_QUEST_WIN_INVEN_POS,

	NUM_IT_QUEST_WIN
};

class CIFQuestWin  : public CIFWindow
{
public:

	
	CIFText				*m_textCtl[NUM_IT_QUEST_WIN];
	
	CIFControl			*m_titleBar;
	CIFControl			*m_titleBottom;
	CIFControl			*m_listBar;
	CIFControl			*m_selectBar;
	CIFControl			*m_itemBar;
	CIFControl			*m_listLine;
	CIFControl			*m_invenBar;

	CIFButtonCtl		*m_closeBtn;
	CIFButtonCtl		*m_stopBtn;

	int					m_selectedIdx;
	int					m_selectBarSavePos;
	int					m_selectedQuestIdx;
	
	int					m_selectedQuestStatusIdx;

	CIFQuestWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFQuestWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	int MakeRewardString( int idx, int value1, int value2, char *szTemp );


};





enum
{
	IT_GUILD_BATTLE_INFO_WIN_TIME			=	0,
	IT_GUILD_BATTLE_INFO_WIN_ATEAM_NAME,	
	IT_GUILD_BATTLE_INFO_WIN_ATEAM_JOIN_INFO,
	IT_GUILD_BATTLE_INFO_WIN_ATEAM_KILL_INFO,
	IT_GUILD_BATTLE_INFO_WIN_BTEAM_NAME,
	IT_GUILD_BATTLE_INFO_WIN_BTEAM_JOIN_INFO,
	IT_GUILD_BATTLE_INFO_WIN_BTEAM_KILL_INFO,	
	NUM_IT_GUILD_BATTLE_INFO_WIN
};




class CIFGuild_battle_info_win : public CIFWindow
{
public:

	
	CIFText				*m_textCtl[NUM_IT_GUILD_BATTLE_INFO_WIN];	
	CIFControl			*m_title;
	CIFControl			*m_AteamCtl;
	CIFControl			*m_BteamCtl;
	
	int					m_AteamPoint;
	int					m_AGuildTotal;	
	int					m_AJoinTotal;
	char				m_AteamName[NAMESTRING];	

	int					m_BteamPoint;
	int					m_BGuildTotal;	
	int					m_BJoinTotal;
	char				m_BteamName[NAMESTRING];	

	

	
	DWORD				m_RemainTime;
	DWORD				m_EffectFlag;
	


	void				SetBattleTeams(int atemidx, int bteamidx);

	
	void				SetBattleRemainTime(DWORD remaintime);
	


	CIFGuild_battle_info_win( CIFControlNode *pParentControl = NULL );
	virtual ~CIFGuild_battle_info_win();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
};






enum
{
	IT_SURGERY_WIN_FACE			=	0,
	IT_SURGERY_WIN_HAIR,
	IT_SURGERY_WIN_FACE_U,
	IT_SURGERY_WIN_HAIR_U,
	IT_SURGERY_WIN_OK,
	IT_SURGERY_WIN_CANCEL,

	NUM_IT_SURGERY_WIN
};

class CIFSurgeryWin  : public CIFWindow
{
public:

	
	CIFText				*m_textCtl[NUM_IT_SURGERY_WIN];
	
	CIFControl			*m_titleBar;
	CIFControl			*m_titleBottom;
	CIFControl			*m_faceIcon;
	CIFControl			*m_hairIcon;
	CIFControl			*m_faceBar;
	CIFControl			*m_hairBar;
	
	CIFButtonCtl		*m_facePrevBtn;
	CIFButtonCtl		*m_faceNextBtn;
	CIFButtonCtl		*m_hairPrevBtn;
	CIFButtonCtl		*m_hairNextBtn;
	CIFButtonCtl		*m_okBtn;
	CIFButtonCtl		*m_cancelBtn;

	int					m_FaceCount;
	int					m_HairCount;
	int					m_maxFaceCount;
	int					m_maxHairCount;
	int					m_BackUpFaceIdx;
	int					m_BackUpHairIdx;

	CIFSurgeryWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFSurgeryWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

};


class CIFGonryunBattleProgressInfo : public CIFWindow
{
public:

	
	CIFText				*m_textCtl[NUM_IT_GUILD_BATTLE_INFO_WIN];	
	CIFControl			*m_title;
	CIFControl			*m_AteamCtl;
	CIFControl			*m_BteamCtl;
	
	int					m_AteamPoint;
	int					m_AGuildTotal;	
	int					m_AJoinTotal;
	char				m_AteamName[NAMESTRING];	

	int					m_BteamPoint;
	int					m_BGuildTotal;	
	int					m_BJoinTotal;
	char				m_BteamName[NAMESTRING];	

	int					m_RemainTime;

	void				SetBattleTeams(int atemidx, int bteamidx);

	CIFGonryunBattleProgressInfo( CIFControlNode *pParentControl = NULL );
	virtual ~CIFGonryunBattleProgressInfo();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
};

#endif 
