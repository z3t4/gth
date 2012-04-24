



#if !defined(AFX_POSTSYSTEM_H__9C90C004_6FE9_43AF_AD79_A1E4ACE78D18__INCLUDED_)
#define AFX_POSTSYSTEM_H__9C90C004_6FE9_43AF_AD79_A1E4ACE78D18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <VECTOR>


class CPostManager
{

public:
	enum 
	{
		MAX_POSTPACKAGE_COUNT		= 50,
		POST_SENDTIME_INFO_STR_LEN	= 32,
		POST_TITLESIZE				= 100,
		POST_STRSIZE				= 2000,
		MAX_ITEMCOUNT_IN_POSTPACKAGE = 12,
	};

	enum	enumPostPackageState
	{
		POSTPACKAGE_UNCHECK = 0,
		POSTPACKAGE_CHECK,
		

	};
	enum	enumPostSystemMode
	{
		POSTSYSTEM_READMODE,		
		POSTSYSTEM_WRITEMODE,
		POSTSYSTEM_NONE,
	};

	enum	enumPostSendType
	{
		POST_SENDTYPE_WEB,
		POST_SENDTYPE_CHAR
	};

public:

	struct tagDetail{
		int		ItemNumber;		
		item_t	Items[MAX_ITEMCOUNT_IN_POSTPACKAGE];
	};


	struct  PostPackage_t{
		BOOL    bActive;
		int		iPostIdx;
		char	szFromName				[NAMESTRING+1];		
		char	szFromAccountID			[IDSTRING+1];
		char	szPostTitle				[POST_TITLESIZE+1];
		char	szSendPostTime			[POST_SENDTIME_INFO_STR_LEN+1];
		char	szMailStr				[POST_STRSIZE+1];
		
		
		union{		
			item_t	Items[MAX_ITEMCOUNT_IN_POSTPACKAGE];	
			int     ItemBox[MAX_ITEMCOUNT_IN_POSTPACKAGE];
		};
		int		ItemNumber;
		


		enum	enumPostPackageState	MailState;		
		enum	enumPostSendType		PostSendType;
		int		iRemainDays;										
		int		Nak;									
		bool	bCheck;				
	};	
protected:

	enumPostSystemMode				m_PostSystemState;
	std::vector<PostPackage_t>		m_PostEntity;

	PostPackage_t*					m_pOpenPost;

public:		
	int								m_OpenPostIdx;	




	int				GetState();
	PostPackage_t*	GetPostPackage(int Idx);
	PostPackage_t*	GetOpenPost();
	int				GetPostCount();	


	bool			SetPostPackage(PostPackage_t*);
	bool			AddPostPackage(PostPackage_t*);
	bool			DeletePostPackage(PostPackage_t*);
	void			SetPostSystem(enumPostSystemMode);
	void			SetOpenPost(int idx);
	
	bool			IsValidPost(int idx);

	void			InitPostSystem();

	static BOOL		InitPost(PostPackage_t*);


	CPostManager::enumPostSystemMode	GetPostState();


public:
	CPostManager();
	virtual ~CPostManager();
};
#endif
						

