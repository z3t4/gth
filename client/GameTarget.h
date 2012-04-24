



#if !defined(AFX_GAMETARGET_H__9E9D90B0_AA20_44AF_B54F_DFCB48AD4C47__INCLUDED_)
#define AFX_GAMETARGET_H__9E9D90B0_AA20_44AF_B54F_DFCB48AD4C47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CGameTarget  
{
public:
	CGameTarget();
	virtual ~CGameTarget();

	Fx_CHARACTER_t*	GTH_GetTargetPointer( Fx_CHARACTER_t* pThisChar );
	Fx_CHARACTER_t*	GTH_GetTargetPointer( int targetType, int targetIdx );
	
	int			GTH_GetTargetName( int targetType, int targetIdx, char *name );
	void		GTH_ChangeTarget( Fx_CHARACTER_t *character );
	float		GTH_GetTargetDistance( Fx_CHARACTER_t *pThisChar );
	int			GTH_GetTargetPosition( Fx_CHARACTER_t* pThisChar, vec3_t pos );
	int			GTH_GetTargetAlive( Fx_CHARACTER_t* pThisChar );
	int			GTH_GetTargetClientID( Fx_CHARACTER_t* pThisChar );
	int			GTH_SetAngleToTarget( Fx_CHARACTER_t* pThisChar );
	int			GTH_GetTargetServerIdxFromName( char* name );
	int			GTH_GetTargetPosition( int targetType, int targetIdx, vec3_t position );
};

extern CGameTarget g_GameTarget;

#endif 
