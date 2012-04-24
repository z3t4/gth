



#if !defined(AFX_LANGUAGE_H__FD108350_EBFF_4D60_8097_F8D0100DA872__INCLUDED_)
#define AFX_LANGUAGE_H__FD108350_EBFF_4D60_8097_F8D0100DA872__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#define LPACK_SEMICOLON			59
#define LPACK_COLON				58
#define LPACK_BACKSLASH			92
#define LPACK_LINEFEED			10
#define LPACK_RETURN			13
#define LPACK_QUOTATION			34
#define TEMPBUFFER_SIZE			4096



enum
{
	LPACK_TYPE_NORMAL=0,
	LPACK_TYPE_NPC,
	LPACK_TYPE_QUESTINFO,
	LPACK_TYPE_HELP,
	LPACK_TYPE_ANSWER,
	LPACK_TYPE_ETC,
	LPACK_TYPE_ABUSE,
	LPACK_TYPE_TIP,
	LPACK_TYPE_TUTORIAL,
	LPACK_TYPE_NORMAL2,

	
	LPACK_TYPE_AUTOSCROLL,

	
	LPACK_TYPE_ABUSE2,

	
	LPACK_TYPE_SKILL,

	LPACK_TYPE_TERRA,

	
	LPACK_TYPE_CHARACTER_NAME,
	
	
	LPACK_TYPE_ITEM,
	
	LPACK_TYPE_COUNT,		
};


#pragma warning (disable : 4786)
#include <vector>
#include <string>
#include <map>

using namespace std;




class CLPack
{	
protected:			
	std::vector<std::string*>	m_vLPackData;

	typedef map<int , string*> CMap_LpackData;
	CMap_LpackData	m_strMapLPackData;	

	int				m_LPackType;
	CFileMng*		m_pFileMng;
	int				m_MaxMessageCount;



public:	
	virtual	bool	LoadFile(char* filename);
	int				GetLpackType();	
	virtual char*	GetMessage(int Idx, int etc1 = 0, int etc2 = 0);	
	int				GetMessageCount();
	inline  char*	Get(int Idx);

	
	bool			IfLineFeed(char	*TokenBuffer);

public:
	virtual void	Init();
	virtual void	Destroy();

public:
	static char*  m_pUnknownMsg_Empty;
	static char*  m_pUnknownMsg_RangeOverflow_String;
	static char*  m_pUnknownMsg_RangeOverflow_Quest;
	static char*  m_pUnknownMsg_RangeOverflow_Etc;	
	static char*  m_pUnknownMsg_RangeOverflow_Type;	

public:
	CLPack			();
	CLPack			(char *filename, CFileMng* pFileMng);
	~CLPack();
};

inline char* CLPack::Get(int Idx)
{
	CMap_LpackData::iterator iter;
	iter=m_strMapLPackData.find(Idx);

	if(iter==m_strMapLPackData.end())	return m_pUnknownMsg_Empty;

	string* pstring = (string *) iter->second;
	if ((char*)pstring ->c_str() == NULL) return m_pUnknownMsg_Empty;
	return (char*)pstring->c_str();
}

inline char* CLPack::GetMessage(int Idx, int etc1, int etc2)					
{		
	
	if ( Idx >= m_vLPackData.size() || Idx <= -1)
		return m_pUnknownMsg_RangeOverflow_String;

	
 	std::string* pstring = (m_vLPackData[Idx]);
	if ((char*)pstring ->c_str() == NULL) return m_pUnknownMsg_Empty;
	return (char*)pstring ->c_str();
}





class CLPack_Quest : public CLPack
{
public:
	enum	MESSAGETYPE
	{
		TYPE_QUESTNAME,
		TYPE_QUESTSTEP
	};

	struct sStepMessage
	{
		int			step;
		void*		Message;
	};

	struct sQuestMessage
	{		
		int			QuestNum;		
		vector		<sStepMessage*> v_StepMessage;
	};
	int				m_LastQuestMessageIdx;


private:

	vector<sQuestMessage*>	m_vQuestMessage;

public:
	bool			LoadFile(char* filename);
	char*			GetMessage(int Idx, int etc1, int etc2);

public:
	void			Init();
	void			Destroy();

public:
	CLPack_Quest(char *filename, CFileMng* pFileMng)
	{	
					Init();
					m_pFileMng	= pFileMng;	
					LoadFile(filename);	
	}
	
	CLPack_Quest(){};
	~CLPack_Quest(){};
};

inline char* CLPack_Quest::GetMessage(int messageType, int QuestNum , int Step)
{

	
	if ( messageType == TYPE_QUESTNAME)
	{
		return CLPack::GetMessage(QuestNum);
	}
	
	else
	{	
		std::string* pstring = NULL;
		if (QuestNum >= m_vQuestMessage.size()) return m_pUnknownMsg_RangeOverflow_Quest;
		for (int i = 0; i < m_vQuestMessage[QuestNum]->v_StepMessage.size(); i ++)
		{
			if (m_vQuestMessage[QuestNum]->v_StepMessage[i]->step == Step)
			{				
				pstring = (std::string*)(m_vQuestMessage[QuestNum]->v_StepMessage[i]->Message);
				return (char*)pstring->c_str();
			}
			else if (m_vQuestMessage[QuestNum]->v_StepMessage[i]->step < Step)
			{
				m_LastQuestMessageIdx = i;
			}
		}
		pstring = (std::string*)(m_vQuestMessage[QuestNum]->v_StepMessage[m_LastQuestMessageIdx]->Message);				
		return (char*)pstring->c_str();

	}
	return NULL;
}





class CLPack_Etc : public CLPack
{
public:
	enum	MESSAGETYPE
	{
			LPACK_ETCTYPE_GEN_ATTRIBUTE,
			LPACK_ETCTYPE_PC_JOB,
			LPACK_ETCTYPE_WEAPON_TYPE,
			LPACK_ETCTYPE_RARITY,
			LPACK_ETCTYPE_PATCH,
			LPACK_ETCTYPE_GOLRYUN_BATTLE,

			TYPE_COUNT
	};	
	private:
		
		
		std::vector<std::vector< void* >* >	m_vpMessage;		
		int			    m_NowvpIdx;
		
	public:
		bool			LoadFile(char* filename);
		char*			GetMessage(int Idx, int etc1, int etc2 = 0);		
	public:
		std::vector<void*>*  GetVector(int idx) 
		{
			return m_vpMessage[idx];			
		}		
	public:
		void			Init();
		void			Destroy();		
	public:
		CLPack_Etc(char *filename, CFileMng* pFileMng)
		{	
			Init();
			m_pFileMng	= pFileMng;	
			LoadFile(filename);	
		}		
		CLPack_Etc(){};
	~CLPack_Etc(){};	
};

inline char*	CLPack_Etc::GetMessage(int Idx, int etc1, int etc2)
{
	if (Idx >= m_vpMessage.size()) return m_pUnknownMsg_RangeOverflow_Etc;
	if ( etc1 >= GetVector(Idx)->size() ) return m_pUnknownMsg_RangeOverflow_String;
	std::string* pString = (std::string*)(*GetVector(Idx))[etc1];
	return (char*)pString->c_str();
	
}




#endif 
