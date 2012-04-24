



#if !defined(AFX_CTOOLS_H__43C7808A_48D1_458A_ABF8_DF7A81B432B7__INCLUDED_)
#define AFX_CTOOLS_H__43C7808A_48D1_458A_ABF8_DF7A81B432B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "datastruct.h"
class CTools  
{
	private:
		BOOL m_bOK;
	public:
		BOOL isOK(void) const;
		static i3client_t* GetClientRecord(const int Idx);
		static characterTable_t* GetCharactorGenTableRecord(const int GenIdx);		
		static characterData_t*  GetCharactorWithCharacterID(const i3client_t* pClient,const int characterID);
			
	public:
		static BOOL IsItemUseFlag(item_t *pItem, ENUM_ITEM_USE_FLAG flag);


	
	public:
		CTools();
		virtual ~CTools();

};

#endif 
