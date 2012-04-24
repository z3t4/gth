



#if !defined(AFX_GMS_TO_GS_H__A5283DC2_E609_4CB2_8AB4_82108DE3EF36__INCLUDED_)
#define AFX_GMS_TO_GS_H__A5283DC2_E609_4CB2_8AB4_82108DE3EF36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CGms_To_Gs  
{
public:

	
public:
	CGms_To_Gs();
	virtual ~CGms_To_Gs();

	void	Decoder(playerCharacter_t* pPlayer);
	int		Send_GS(int SystemType,int mainType, int subType,char *FromPlayerName,char *TargetPlayerName,int gameServerNo);

};

#endif 
