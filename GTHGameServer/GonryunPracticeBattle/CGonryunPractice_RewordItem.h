#ifndef  _CGONRYUNPRACTICE_REWORDITEM_H_
#define	 _CGONRYUNPRACTICE_REWORDITEM_H_


class CGonryunPractice_RewordItem{

public:
	
	CGonryunPractice_RewordItem();
	CGonryunPractice_RewordItem(const CGonryunPractice_RewordItem& rRewordItem);
    
	virtual ~CGonryunPractice_RewordItem()
	{
	}
	
	CGonryunPractice_RewordItem& operator=(const CGonryunPractice_RewordItem& rRewordItem);

public:
	
	void SetItemIndex(const int& index);
	const int& GetItemIndex()const;
	
	void SetItemType(const int& type);
	const int& GetItemType()const;
	
	void SetItemNumber(const int& number);
	const int& GetItemNumber()const;

	void SetItemRate(const int& rate);
	const int& GetItemRate()const;

	void SetItemCount(const int& count);
	const int& GetItemCount()const;

private:
	
	void Init();
	
private:

	int m_irewordItemIndex;	
	int m_irewordItemType; 
   	int m_irewordItemNumber;
	int m_irewordItemRate;
	int m_irewordItemCount;
};

#endif