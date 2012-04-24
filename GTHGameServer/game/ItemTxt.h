



#if !defined(AFX_ITEMTXT_H__9C5B371A_F93F_48B7_AF4A_EB51F63A3B5A__INCLUDED_)
#define AFX_ITEMTXT_H__9C5B371A_F93F_48B7_AF4A_EB51F63A3B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


class CItemTxtData
{
public:
	CItemTxtData()
	{
		Init();
	}

	void Init()
	{
		m_Index=-1;
		m_Txt[0] = '\0';
		
	}

public:
	int     m_Index;
	char	m_Txt[256];

};

class CItemTxt  
{
public:
	CItemTxt();
	virtual ~CItemTxt();

	bool		LoadFile(char * FileName);

	void		GetOnlyTxt();

	void		ClearClassNameList();
	void		ClearDescList();
	void		ClearNameList();

	void		BindItemTable();

	LPSTR		GetName(int idx);
	LPSTR		GetDesc(int idx);
	
	void		STRCPY(char &Dest, char &Soruce);

public:
	char		m_Token[256];

	vector <CItemTxtData> m_DescList;
	vector <CItemTxtData> m_NameList;
	vector <CItemTxtData> m_ClassNameList;
};

extern CItemTxt g_TxtApp;

#endif 
