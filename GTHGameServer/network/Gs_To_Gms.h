



#if !defined(AFX_GS_TO_GMS_H__0F18DB24_883B_40D2_9130_7A1951B44389__INCLUDED_)
#define AFX_GS_TO_GMS_H__0F18DB24_883B_40D2_9130_7A1951B44389__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CGs_To_Gms  
{
public:
	enum enumPacketID
	{
	};

	enum enumCheckType
	{
		CHECK_PLAYER1=0,
		CHECK_PLAYER2
	};

public:
	CGs_To_Gms();
	virtual ~CGs_To_Gms();

	void	Decoder(playerCharacter_t* pPlayer);

	int		Send_GMS(int SystemType,int mainType, int subType,char *FromPlayerName,char *TargetPlayerName,int gameServerNo,int CheckTarget);
	int		Send_GMS(int SystemType,int mainType, int subType,long value,char *FromPlayerName,char *TargetPlayerName,int gameServerNo,int CheckTarget);
	int		Send_GMS(int SystemType,int mainType, int subType, int worldIdx, vec3_t position, char *FromPlayerName,char *TargetPlayerName,int gameServerNo,int CheckTarget);
	int		Send_GMS(int SystemType,int mainType, int subType, int val, int worldIdx, vec3_t position, char *FromPlayerName,char *TargetPlayerName,int gameServerNo,int CheckTarget);
};

#endif 
