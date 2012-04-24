#include "..\global.h"
#include "CGonryunPractice_RewordItem.h"

CGonryunPractice_RewordItem::CGonryunPractice_RewordItem()
{
	Init(); 
}

CGonryunPractice_RewordItem::CGonryunPractice_RewordItem(const CGonryunPractice_RewordItem& rRewordItem)
{
	 m_irewordItemIndex		= rRewordItem.m_irewordItemIndex;	
	 m_irewordItemType		= rRewordItem.m_irewordItemType; 
   	 m_irewordItemNumber	= rRewordItem.m_irewordItemNumber;
	 m_irewordItemRate		= rRewordItem.m_irewordItemRate;
	 m_irewordItemCount		= rRewordItem.m_irewordItemCount;
}

CGonryunPractice_RewordItem& CGonryunPractice_RewordItem::operator = (const CGonryunPractice_RewordItem& rRewordItem)
{
	m_irewordItemIndex		= rRewordItem.m_irewordItemIndex;	
	 m_irewordItemType		= rRewordItem.m_irewordItemType; 
   	 m_irewordItemNumber	= rRewordItem.m_irewordItemNumber;
	 m_irewordItemRate		= rRewordItem.m_irewordItemRate;
	 m_irewordItemCount		= rRewordItem.m_irewordItemCount;
	 return *this;
}

void CGonryunPractice_RewordItem::Init()
{
	m_irewordItemIndex		= 0;	
	 m_irewordItemType		= 0; 
   	 m_irewordItemNumber	= 0;
	 m_irewordItemRate		= 0;
	 m_irewordItemCount		= 0;
}


void CGonryunPractice_RewordItem::SetItemIndex(const int& index)
{
	m_irewordItemIndex = index;
}

const int& CGonryunPractice_RewordItem::GetItemIndex()const
{
	return m_irewordItemIndex;
}

void CGonryunPractice_RewordItem::SetItemType(const int& type)
{
	m_irewordItemType = type;
}
	
const int& CGonryunPractice_RewordItem::GetItemType()const
{
	return m_irewordItemType;
}

void CGonryunPractice_RewordItem::SetItemNumber(const int& number)
{
	m_irewordItemNumber = number;
}

const int& CGonryunPractice_RewordItem::GetItemNumber() const
{
	return m_irewordItemNumber;
}

void CGonryunPractice_RewordItem::SetItemRate(const int& rate)
{
	m_irewordItemRate = rate;
}

const int& CGonryunPractice_RewordItem::GetItemRate() const
{
	return m_irewordItemRate;
}

void CGonryunPractice_RewordItem::SetItemCount(const int& count)
{
	m_irewordItemCount = count;
}

const int& CGonryunPractice_RewordItem::GetItemCount() const
{
	return m_irewordItemCount;
}