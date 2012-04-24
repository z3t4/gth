



#if !defined(AFX_GAMETARGET_H__AE7EED36_1E63_482B_BAC9_FC0857A068ED__INCLUDED_)
#define AFX_GAMETARGET_H__AE7EED36_1E63_482B_BAC9_FC0857A068ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CGameTarget  
{
public:
	CGameTarget();
	virtual ~CGameTarget();

};


int			GTH_GetTargetPosition( monsterCharacter_t *pMonster, vec3_t pos );
int			GTH_GetTargetPosition( playerCharacter_t *pc, vec3_t pos );
float		GTH_GetDistanceFromTarget( playerCharacter_t *pc );
float		GTH_GetDistanceFromTarget( vec3_t pos, int targetIdx, int targetType );
char*		GTH_GetTargetName( int entityType, int entityIdx );
void		AddTargetList( playerCharacter_t *pc, char *name );
int			InTargetList( playerCharacter_t *pc, char* name );


#endif 
