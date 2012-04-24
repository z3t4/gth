



#if !defined(AFX_IFLOGINWIN_H__6D1CF453_DFCA_4CFF_A199_FF87C60CAEBB__INCLUDED_)
#define AFX_IFLOGINWIN_H__6D1CF453_DFCA_4CFF_A199_FF87C60CAEBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#define LOGIN_PU_REMAIN_DATE_NOTICE 3


enum
{
	LOGIN_STATE_IDPW=0,
	LOGIN_STATE_SELECT_SERVER=1,
	LOGIN_STATE_SELECT_CHARACTER=2,
	LOGIN_STATE_CREATE_CHARACTER=3,
	LOGIN_STATE_SELECT_MASTER_SERVER=4,
	LOGIN_STATE_NOTICE=5,
};

enum
{
	LOGIN_SELECT_CHAR_STAGE1=0,
	LOGIN_SELECT_CHAR_STAGE2,
	LOGIN_SELECT_CHAR_STAGE3,
	LOGIN_SELECT_CHAR_STAGE4,
};


enum
{
	CREATE_NONE_CHAR=0,
	CREATE_ONLY_LEFT_CHAR=1,
	CREATE_ONLY_CENTER_CHAR=2,
	CREATE_LEFT_CENTER_CHAR=3,
	CREATE_ONLY_RIGHT_CHAR=4,	
	CREATE_LEFT_RIGHT_CHAR=5,
	CREATE_CENTER_RIGHT_CHAR=6,
	CREATE_ALL_CHAR=7,
};



enum
{
	
	LOGIN_CHAR_FREE_NEW=0,
	LOGIN_CHAR_FREE_LEFT,
	LOGIN_CHAR_FREE_CENTER,
	LOGIN_CHAR_FREE_RIGHT,
	
	
	LOGIN_CHAR_PU_NEW=10,
	LOGIN_CHAR_PU,	
};



enum
{
	LOGIN_SELECT_CHAR_LEFT=0,
	LOGIN_SELECT_CHAR_CENTER,
	LOGIN_SELECT_CHAR_RIGHT,
};



enum
{
	
	IT_LOGIN_WIN_LOGIN			=	0,
	IT_LOGIN_WIN_ID,
	IT_LOGIN_WIN_PASSWORD,
	IT_LOGIN_WIN_ID_U,
	IT_LOGIN_WIN_PASSWORD_U,

	
	IT_LOGIN_WIN_VERSION_TEXT_POS,

	
	IT_CHARINFO_WIN_CHARINFO,
	IT_CHARINFO_WIN_NAME_U,
	IT_CHARINFO_WIN_LEVEL_U,
	IT_CHARINFO_WIN_RANK_U,
	IT_CHARINFO_WIN_CLASS_U,
	IT_CHARINFO_WIN_GEN_BAR,
	IT_CHARINFO_WIN_GEN_ICON_0_U,
	IT_CHARINFO_WIN_GEN_ICON_1_U,
	IT_CHARINFO_WIN_GEN_ICON_2_U,
	IT_CHARINFO_WIN_GEN_ICON_3_U,
	IT_CHARINFO_WIN_GENCAP_0_U,
	IT_CHARINFO_WIN_GENCAP_1_U,
	IT_CHARINFO_WIN_GENCAP_2_U,
	IT_CHARINFO_WIN_GENCAP_3_U,

	
	IT_CHARINFO2_WIN_CHARINFO,
	IT_CHARINFO2_WIN_U,
	IT_CHARMAKE_WIN_CHARMAKE,
	IT_CHARMAKE_WIN_NAME,
	IT_CHARMAKE_WIN_NAME_U,
	IT_CHARMAKE_WIN_FACE,
	IT_CHARMAKE_WIN_FACE_U,
	IT_CHARMAKE_WIN_HAIR,
	IT_CHARMAKE_WIN_HAIR_U,
	IT_CHARMAKE_WIN_STAT,
	IT_CHARMAKE_WIN_STAT_FORCE,
	IT_CHARMAKE_WIN_STAT_FORCE_U,
	IT_CHARMAKE_WIN_STAT_SOUL,
	IT_CHARMAKE_WIN_STAT_SOUL_U,
	IT_CHARMAKE_WIN_STAT_AGI,
	IT_CHARMAKE_WIN_STAT_AGI_U,
	IT_CHARMAKE_WIN_STAT_VIT,
	IT_CHARMAKE_WIN_STAT_VIT_U,
	IT_CHARMAKE_WIN_OK,
	IT_CHARMAKE_WIN_CANCEL,

	NUM_IT_LOGIN_WIN
};

#define MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN		20



class CIFLoginWin   	: public CIFWindow
{
public:

	
	CIFControl			*m_login;
	CIFControl			*m_loginTitleBar;
	CIFControl			*m_loginTitleBottom;
	CIFControl			*m_loginBar;
	CIFControl			*m_loginLeftBar;
	CIFControl			*m_loginRightBar;
	CIFControl			*m_loginIDBar;
	CIFControl			*m_loginPasswordBar;
	CIFButtonCtl		*m_loginOkBtn;
	CIFButtonCtl		*m_loginCancelBtn;

	
	CIFButtonCtl		*m_serverBtn[MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN];
	CIFButtonCtl		*m_serverOkBtn;
	CIFButtonCtl		*m_serverCancelBtn;

	
	CIFControl			*m_cselInfo;
	CIFControl			*m_cselInfoTitleBar;
	CIFControl			*m_cselInfoTitleBottom;
	CIFControl			*m_cselInfoBar;
	CIFControl			*m_cselInfoGenBar;
	CIFControl			*m_cselInfoGenIcon0;
	CIFControl			*m_cselInfoGenIcon1;
	CIFControl			*m_cselInfoGenIcon2;
	CIFControl			*m_cselInfoGenIcon3;
	
	CIFButtonCtl		*m_DifferentCharBtn;

	CIFButtonCtl		*m_cselExitBtn;		
	CIFButtonCtl		*m_cselOkBtn;		
	CIFButtonCtl		*m_cselBackBtn;		
	CIFButtonCtl		*m_cselDeleteBtn;	
	CIFButtonCtl		*m_cselMakeBtn;		

	
	CIFButtonCtl		*m_cReLoginBtn;		
	
	
	CIFControl			*m_cmakeInfo;
	CIFControl			*m_cmakeInfoTitleBar;
	CIFControl			*m_cmakeInfoTitleBottom;
	CIFControl			*m_cmakeInfoBar;
	CIFControl			*m_cmake;
	CIFControl			*m_cmakeTitleBar;
	CIFControl			*m_cmakeTitleBottom;
	CIFControl			*m_cmakeBar;
	CIFControl			*m_cmakeNameIcon;
	CIFControl			*m_cmakeNameBar;
	CIFControl			*m_cmakeFaceIcon;
	CIFControl			*m_cmakeFaceBar;
	CIFControl			*m_cmakeHairIcon;
	CIFControl			*m_cmakeHairBar;
	CIFControl			*m_cmakeStatBar;
	CIFControl			*m_cmakeStatLine;
	CIFControl			*m_cmakeForceIcon;
	CIFControl			*m_cmakeForceBar;
	CIFControl			*m_cmakeSoulIcon;
	CIFControl			*m_cmakeSoulBar;
	CIFControl			*m_cmakeAgiIcon;
	CIFControl			*m_cmakeAgiBar;
	CIFControl			*m_cmakeVitIcon;
	CIFControl			*m_cmakeVitBar;

	CIFButtonCtl		*m_cmakeFacePrevBtn;
	CIFButtonCtl		*m_cmakeFaceNextBtn;
	CIFButtonCtl		*m_cmakeHairPrevBtn;
	CIFButtonCtl		*m_cmakeHairNextBtn;
	CIFButtonCtl		*m_cmakeForceUpBtn;
	CIFButtonCtl		*m_cmakeForceDnBtn;
	CIFButtonCtl		*m_cmakeSoulUpBtn;
	CIFButtonCtl		*m_cmakeSoulDnBtn;
	CIFButtonCtl		*m_cmakeAgiUpBtn;
	CIFButtonCtl		*m_cmakeAgiDnBtn;
	CIFButtonCtl		*m_cmakeVitUpBtn;
	CIFButtonCtl		*m_cmakeVitDnBtn;
	CIFButtonCtl		*m_cmakeOkBtn;
	CIFButtonCtl		*m_cmakeCancelBtn;

	CIFButtonCtl		*m_cmakePrevBtn;
	CIFButtonCtl		*m_cmakeNextBtn;

	
	CIFControl			*m_loginNotice;
	CIFButtonCtl		*m_cNoticeOkBtn;		

	bool				m_bFirstLogin;
	bool				GetFirstConnectMasterServer() { return m_bFirstLogin;	}
	void				SetFirstConnectMasterServer(bool flag) { m_bFirstLogin = flag; }

	
	CIFText				*m_textCtl[NUM_IT_LOGIN_WIN];

	int					m_loginStep;
	int					m_cameraStep;
	int					m_deleteFlag;
	int					m_makeFlag;
	int					m_curSelectedMakeCharacter;
	int					m_nextSelectedMakeCharacter;
	int					m_cameraStepFlag;
	int					m_faceFlag;
	
	
	int					m_bDeleteCharFlag;
	float				m_deleteCharRemainTime;

	int					m_numOfMakeCharacter;
	Fx_CHARACTER_t		*m_selectedCharacter;

	charEditData_t		m_makeCharData[8];
	
	
	int					m_nMembershipType;
	int					m_nDate;
	BYTE				m_btSelectCharStage;
	int					m_btTempMembershipType;
	bool				m_bLoadCharacterSlot;
	bool				m_bSendLoadCharacterSlot;
	bool				m_bCharacterShout;
	

	CIFLoginWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFLoginWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	
	
	void		MakeMasterServerGroupList();
	
	
	int			ChangeLoginStep( int step );
	void		RequestLogin();
	void		MakeServerList();
	void		RequestGameData();
	void		RequestMakeCharacter();
	void		RequestCheckCharacterName();
	void		RequestServerAddress();

	int			CharacterSelectProcess();
	void		SetFaceCamera( int state );

	
	void		SetCharacterInfo();

	

	
	void		SetDeleteCharacter();

	void		NoticeProcess();
	void		IdPwProcess();
	void		SelectServerProcess();
	void		SelectCharacterProcess();
	void		CreateCharacterProcess();
	void		SelectMasterServer();
	void		CharacterSelect_SelectScreenProcess();
	void		CharacterSelect_CreateScreenProcess();
	void		CharacterSelect_CameraProcess();
	void		SettingDrawText();
	void		IdPwRender();
	void		CreateCharacterRender();
	void		TextRender();

	
	void		SelectSlot(bool flag);
	void		InitCharacterSlot();	
	void		SettingCharacterSlot();
	bool		IsSafeMembershipType(int nType);
	void		Storoge_Action();
	bool		Storoge_SelectCharacter(Fx_CHARACTER_t *g_mouseSelectedCharacter);
	

};

#endif 
