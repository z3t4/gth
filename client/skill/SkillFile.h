



#if !defined(AFX_SKILLFILE_H__E0CC4C4B_3A0E_41FA_B42A_EF517E955D53__INCLUDED_)
#define AFX_SKILLFILE_H__E0CC4C4B_3A0E_41FA_B42A_EF517E955D53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CSkillFile  
{
public:
	BYTE		*m_Buffer;
public:
	CSkillFile();
	virtual ~CSkillFile();

	bool		GetData(void *p,int size);
	int			LoadSkillTable( char *filename);

	bool		CheckSize(int Val , LPSTR str);
	bool		LoadSkillTableCheckValue();
	bool		LoadSkillTableCheckSize();
	void		DebugOutString(LPCTSTR lpszFormat, ...);
};



extern CSkillFile g_SkillFile;

#endif 
