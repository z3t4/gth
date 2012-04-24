



#if !defined(AFX_GS_TO_CC_H__F97E17C5_0C44_445A_8F88_BAAF75AE7649__INCLUDED_)
#define AFX_GS_TO_CC_H__F97E17C5_0C44_445A_8F88_BAAF75AE7649__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CGs_To_Cc  
{
public:
	CGs_To_Cc();
	virtual ~CGs_To_Cc();

	void Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType);
	
	void Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType,char *TargetPlayerName);
	void Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType,char *requestPlayerName,char *TargetPlayerName);

	void Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType,int lval, char *TargetPlayerName);
	void Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType,int val, char *requestPlayerName,char *TargetPlayerName);

	void Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType,int worldIdx,vec3_t position,char *requestPlayerName,char *TargetPlayerName);
	void Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType,int val, int worldIdx,vec3_t position,char *requestPlayerName,char *TargetPlayerName);
};

#endif 
