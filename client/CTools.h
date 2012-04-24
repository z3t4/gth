



#if !defined(AFX_CTOOLS_H__B2A34627_FBC4_4ED9_9AF4_E16AE9AA4393__INCLUDED_)
#define AFX_CTOOLS_H__B2A34627_FBC4_4ED9_9AF4_E16AE9AA4393__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CTools  
{
	private:
		BOOL m_bOK;
	public:
		BOOL isOK(void) const;
		static void Replace_singleQUOTATIONmark_by_doubleQUOTATIONmark(char* ntspString);
		static void Replace_doubleQUOTATIONmark_by_singleQUOTATIONmark(char* ntspString);

	public:
		
		
		int	  GetEpkChargeCount(item_t *pItem);

		
		
		void  SetEpkChargeCount(item_t *pItem,int count);

		
		Fx_CHARACTER_t *  FindCharacter(const int EntityType,const int IdxOnServer);
		bool			  IfMyCharacter(const Fx_CHARACTER_t *character);

		
		
		bool		IfSkillCasting_NotItemUse();
		bool		DeleteInventoryItem(int invenPos);
		void		CcToGs(int sysType,int mainType,int subType);
		void		CcToGs(int sysType,int mainType,int subType,int val);
		void		CcToGs(int sysType,int mainType,int subType,char *TargetPlayerName);
		void		CcToGs(int sysType,int mainType,int subType,int val, char *TargetPlayerName);
		void		CcToGs(int sysType,int mainType,int subType,int worldIdx,vec3_t position, char *TargetPlayerName);
		void		CcToGs(int sysType,int mainType,int subType, char *RequestPlayerName, char *TargetPlayerName);
		void		CcToGs(int sysType,int mainType,int subType, int worldIdx, vec3_t position, char *RequestPlayerName, char *TargetPlayerName);
		

	public:
		CTools();
		virtual ~CTools();

};

#endif 
