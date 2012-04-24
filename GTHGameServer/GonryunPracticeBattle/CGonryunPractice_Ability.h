#ifndef  _CGONRYUNPRACTICE_ABILITY_H_
#define	 _CGONRYUNPRACTICE_ABILITY_H_

# include <string> 
using namespace std; 


class CGonryunPractice_Ability{
	
public:
	
	CGonryunPractice_Ability();
	CGonryunPractice_Ability(const CGonryunPractice_Ability& rReword);
	virtual ~CGonryunPractice_Ability(){}


	CGonryunPractice_Ability& operator = (const CGonryunPractice_Ability& rReword);

public:
	
	void SetID(const int& id);
	const int& GetID()const;
	
	BOOL SetName(const char* pname);
	const char* GetName()const;
	
	void SetType(const int& type);
	const int& GetType()const;

	void SetExperience(const int& Experience);
	const int& GetExperience()const;

	void SetExpRate(const int& ExpRate);
	const int& GetExpRate()const;

	void SetItemID(const int& itemID);
	const int& GetItemID()const;

	void SetItemRate(const int& itemRate);
	const int& GetItemRate()const;

private:
	BOOL IsvalidNameLength(const char* pname)const;
	void Init();
public:
	enum _define{
		NAMEARRAY = 20,
	};
	
private:
	int  m_irewordID;						
	char m_szrewordName[NAMEARRAY+1];		
	int  m_irewordType;						
	int	 m_irewordExperience;				
	int  m_irewordExperienceRate;			
	int  m_irewordItemID;					
	int  m_irewordItemRate;					
};

#endif