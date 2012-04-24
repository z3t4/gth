



#if !defined(AFX_POSTSYSTEM_H__9C90C004_6FE9_43AF_AD79_A1E4ACE78D18__INCLUDED_)
#define AFX_POSTSYSTEM_H__9C90C004_6FE9_43AF_AD79_A1E4ACE78D18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <VECTOR>
#include "global_def.h"

#define IDSTRING               20
#define PASSWORDSTRING         20
#define NAMESTRING             20
#define WORLDNAMESTRING        64


class CPostManager
{
public:
	enum 
	{
		MAX_POSTPACKAGE_COUNT		= 50,
		POST_SENDTIME_INFO_STR_LEN	= 32,
		POST_TITLESIZE				= 32,
		POST_STRSIZE				= 2000,
		MAX_ITEMCOUNT_IN_POSTPACKAGE= 12,

		MAX_MESSAGE_LINE			= 6,

	};

	enum	enumPostPackageState
	{
		POSTPACKAGE_UNCHECK = 0,
		POSTPACKAGE_CHECK,
	};
	enum	enumPostPackageMode
	{
		POSTSYSTEM_READMODE,
		POSTSYSTEM_WRITEMODE
	};

	enum	enumPostSendType
	{
		POST_SENDTYPE_WEB,
		POST_SENDTYPE_CHAR
		
	};

public:
	struct  PostPackage_t{
		BOOL	bActive;
		int		iPostIdx;
		char	szFromName				[NAMESTRING+1];
		char	szFromAccountID			[IDSTRING+1];
		char	szPostTitle				[POST_TITLESIZE+1];
		char	szSendPostTime			[POST_SENDTIME_INFO_STR_LEN+1];
		char	szMailStr				[POST_STRSIZE+1];
		item_t	Items					[MAX_ITEMCOUNT_IN_POSTPACKAGE];		
		UINT	ItemNumber;	
		enum	enumPostPackageState	MailState;				
		enum	enumPostSendType		PostSendType;
		int		iRemainDays;
		int		Nak;
		bool	bCheck;
	};	

protected:
	enumPostPackageMode				m_PostSystemState;
	std::vector<PostPackage_t>		m_PostEntity;
	PostPackage_t*					m_pOpenPost;


public:
	int								m_OpenPostIdx;
	

public:





	void			InverseState(void);

	int				GetState();									
	PostPackage_t*	GetPostPackage(int Idx);					
	int				GetPostCount();								
	PostPackage_t*	GetOpenPost();
	int				GetCheckCount();


	bool			SetPostPackage(PostPackage_t*);				
	void			AddPostPackage(const PostPackage_t&);
	bool			DeletePostPackage(int postidx);				
	void			SetOpenPost(int idx);
	bool			IsValidPost(int idx);
	BOOL			InitPackage(PostPackage_t* pPostPackage);
	void			SetPostSystem(enumPostPackageMode);






public:
	void			InitPostSystem();

public:
	CPostManager();
	virtual ~CPostManager();

};

#endif 
