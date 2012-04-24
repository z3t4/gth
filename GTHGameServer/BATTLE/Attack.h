



#if !defined(AFX_ATTACK_H__8C3E9498_9475_42CC_AAA1_436CF7B70C7B__INCLUDED_)
#define AFX_ATTACK_H__8C3E9498_9475_42CC_AAA1_436CF7B70C7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CAttack  
{
public:
	CAttack();
	virtual ~CAttack();

};

void GTH_PC_Attack(playerCharacter_t *pc);
void GTH_ProcessEventMessage_Attack();


#endif 
