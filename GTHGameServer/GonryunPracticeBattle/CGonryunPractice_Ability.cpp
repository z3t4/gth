#include "..\global.h"
#include "CGonryunPractice_Ability.h"


CGonryunPractice_Ability::CGonryunPractice_Ability()
{
	Init();		
}

CGonryunPractice_Ability::CGonryunPractice_Ability(const CGonryunPractice_Ability& rReword)
{
	m_irewordID					= rReword.m_irewordID;						
	strncpy(m_szrewordName ,	rReword.m_szrewordName,NAMEARRAY);
	m_szrewordName[NAMEARRAY]   = NULL;
	m_irewordType				= rReword.m_irewordType;				
	m_irewordExperience			= rReword.m_irewordExperience;		
	m_irewordExperienceRate		= rReword.m_irewordExperienceRate;			
	m_irewordItemID				= rReword.m_irewordItemID;				
	m_irewordItemRate			= rReword.m_irewordItemRate;	
}

CGonryunPractice_Ability& CGonryunPractice_Ability::operator = (const CGonryunPractice_Ability& rReword)
{
	m_irewordID					= rReword.m_irewordID;						
	strncpy(m_szrewordName ,	rReword.m_szrewordName,NAMEARRAY);
	m_szrewordName[NAMEARRAY]   = NULL;
	m_irewordType				= rReword.m_irewordType;				
	m_irewordExperience			= rReword.m_irewordExperience;		
	m_irewordExperienceRate		= rReword.m_irewordExperienceRate;			
	m_irewordItemID				= rReword.m_irewordItemID;				
	m_irewordItemRate			= rReword.m_irewordItemRate;	
	
	return *this;
}

void CGonryunPractice_Ability::Init()
{
	m_irewordID				= 0;						
	m_szrewordName[0]		= NULL;	
	m_irewordType			= 0;				
	m_irewordExperience		= 0;		
	m_irewordExperienceRate = 0;			
	m_irewordItemID			= 0;				
	m_irewordItemRate		= 0;	
}


void CGonryunPractice_Ability::SetID(const int& id)
{
	m_irewordID = id;
}

const int& CGonryunPractice_Ability::GetID()const
{
	return m_irewordID;
}

BOOL CGonryunPractice_Ability::SetName(const char* pname)
{
	if(FALSE == IsvalidNameLength(pname)) return FALSE; 
	
	strncpy(m_szrewordName,pname,NAMEARRAY);
	m_szrewordName[NAMEARRAY] = NULL;
	
	return TRUE;
}

const char* CGonryunPractice_Ability::GetName()const
{
	return m_szrewordName;
}	

void CGonryunPractice_Ability::SetType(const int& type)
{
	m_irewordType = type;
}

const int& CGonryunPractice_Ability::GetType()const
{
	return m_irewordType;
}

void CGonryunPractice_Ability::SetExperience(const int& Experience)
{
	m_irewordExperience = Experience;
}

const int& CGonryunPractice_Ability::GetExperience()const
{
	return m_irewordExperience;
}

void CGonryunPractice_Ability::SetExpRate(const int& ExpRate)
{
	m_irewordExperienceRate = ExpRate;
}

const int& CGonryunPractice_Ability::GetExpRate()const
{
	return m_irewordExperienceRate;
}

void CGonryunPractice_Ability::SetItemID(const int& itemID)
{
	m_irewordItemID = itemID;
}

const int& CGonryunPractice_Ability::GetItemID()const
{
	return m_irewordItemID;
}

void CGonryunPractice_Ability::SetItemRate(const int& itemRate)
{
	m_irewordItemRate = itemRate;
}

const int& CGonryunPractice_Ability::GetItemRate()const
{
	return m_irewordItemRate;
}

BOOL CGonryunPractice_Ability::IsvalidNameLength(const char* pname)const
{
	int length = strlen(pname);

	return (( 0 < length ) && ( NAMEARRAY >= length ) ) ? TRUE : FALSE;
}