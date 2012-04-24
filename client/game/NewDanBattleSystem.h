



#if !defined(AFX_NEWDANBATTLESYSTEM_H__12E6C569_671E_44EE_93DD_EE548AC47392__INCLUDED_)
#define AFX_NEWDANBATTLESYSTEM_H__12E6C569_671E_44EE_93DD_EE548AC47392__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif  _MSC_VER > 1000

class NewDanBattleSystem
{
public:

	struct DanBaseInfo
	{
		int		nDanIdx;				
		char	szGuildName[GUILDNAMESTRING];
		int		nKillPoint;				
		int		naccumulationKillpoint;	
		int		nRemainPoint;			

	};


	struct DanBattleInfo
	{	
		
		enum enumBattleMode
		{
			BATTLEMODE_FRIENDSHIP,
			BATTLEMODE_GENERAL,
			
		};

		enum enumBattleStatus
		{
			BATTLESTATUS_NONE,
			BATTLESTATUS_REGIST,
			BATTLESTATUS_READY,
			BATTLESTATUS_BATTLE,
			BATTLESTATUS_END,
		};

		enum enumEndType
		{
			REMAIN_MEMBER_COUNT =0,
			TIME_OUT,
			SYSTEM,
		};

		enum enumBattleMode		BattleMode;	
		enum enumBattleStatus	BattleStatus;

		struct DanBaseInfo		RequestDanInfo;			
		struct DanBaseInfo		AcceptDanInfo;			

		float					fRemainSec;				
														
														
		int						nBattleMapIdx;
		float					fItemDropRate;			
		int						nDanBattleIdx;
		float					fWaitTImeForReady;
		float					fWaitTImeForEnd;
		BOOL					bPlunderPremiumItem;	
	};
	
	struct sNoticeMsg{
		float						m_noticeTime;
		char						m_noticeMsg[256];
		DWORD						m_noticeColor;
	};

	enum noticetype
	{
		notice = 0,
		killpoint_notice,
		plunder_notice,
		max_notice,
	};

	sNoticeMsg					m_notice[noticetype::max_notice];

public:
	BOOL						m_bActive;
	int							m_nMinPcCount;			
	int							m_nBattleTimeForMin1;	
	int							m_nBattleTimeForMin2;	
	float						m_fItemDropRate;		
	INT							m_EffectFlag;

public:
	
	void						RenderDanBattle_Notice();
	void						AddDanBattleNoticeMsg(char* message, DWORD color = GTHCOLOR_ITEMNOTICE_MESSAGE, noticetype type = noticetype::notice);


protected:
	DanBattleInfo				m_DanBattleInfo;		

public:
	void						InitDanBattleInfo();	

	DanBaseInfo*				GetMyDanBattleInfo();		
	DanBaseInfo*				GetDestDanBattleInfo();		

	DanBaseInfo*				GetRequestDanBattleInfo();	
	DanBaseInfo*				GetAcceptDanBattleInfo();	

	DanBattleInfo*				GetDanBattleInfo();									
	void						ShoeEffectCount(INT RemainTime);
	void						SetDanBattleStatus(DanBattleInfo::enumBattleStatus status);
	BOOL						IsDanBattleEnemy(int pcidx);
	vector<int>					v_EffectPC;

public:
	void						Update();
	void						BattleReady();
	void						Battle();
	void						BattleEnd();


public:
	NewDanBattleSystem();
	virtual ~NewDanBattleSystem();

};

#endif 
