



#if !defined(AFX_SKILLAPP_H__3697DB0A_964F_474C_A321_9941001E4B8D__INCLUDED_)
#define AFX_SKILLAPP_H__3697DB0A_964F_474C_A321_9941001E4B8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "skills.h"

#include "SkillDataGlobal.h"
#include "SkillTalbeDefine.h"

#include "SkillTableMap.h"
#include "SkillProcAffectSpe.h"
#include "SkillDataAttack.h"
#include "SkillDataAffectCommon.h"
#include "SkillDataAffect.h"
#include "SkillDataBasic.h"








#include "SkillLogic.h"
#include "Skillproc.h"
#include "PcSkill.h"
#include "SkillDataMegProc.h"
#include "SkillCast.h"
#include "SkillDataSubProc.h"
#include "SkillDataCheckProc.h"
#include "SkillPartyProc.h"
#include "SkillItem.h"
#include "SkillTransInfo.h"


class CSkillApp  
{
public:
	CSkillApp();
	virtual ~CSkillApp();

	void		InitSkillTable();
	int			LoadSkillTable( char *filename,int &skillNumber );
	bool		LoadSkillTableCheckSize(FILE *fp);
	bool		CheckSize(FILE *fp, int Val , LPSTR str);
	bool		LoadSkillTableCheckValue(FILE *fp);

public:
	
	
};


extern CSkillApp g_SkillApp;

#endif 
