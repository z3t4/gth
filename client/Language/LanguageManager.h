



#if !defined(AFX_LANGUAGEMANAGER_H__3F15428C_610A_4696_B1D5_F6443C231C3C__INCLUDED_)
#define AFX_LANGUAGEMANAGER_H__3F15428C_610A_4696_B1D5_F6443C231C3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <vector>
#include "language.h"
using namespace std;

class CLanguageManager  
{	
protected:	
	CLPack*			m_Language [ LPACK_TYPE_COUNT ];
	CFileMng*		m_pFileMng;

public:
	void			InsertLanuage( CLPack* pack );
	void			SetFileManager( CFileMng* filemng );

	char*			GetMassage( int type, int num, int etc1 = 0, int etc2 = 0 )
	{						
					if ( type > LPACK_TYPE_COUNT ) return CLPack::m_pUnknownMsg_RangeOverflow_Type;
					if ( m_Language[type] == NULL ) return CLPack::m_pUnknownMsg_Empty;
					return m_Language[type]->GetMessage(num, etc1, etc2);			
	};

	char*			Get( int type, int num)
	{						
					if ( type > LPACK_TYPE_COUNT ) return CLPack::m_pUnknownMsg_RangeOverflow_Type;
					if ( m_Language[type] == NULL ) return CLPack::m_pUnknownMsg_Empty;
					return m_Language[type]->Get(num);
	};

	char*			GetMessage( int type, int num, int etc1 = 0, int etc2 = 0 )
	{						
					if ( type > LPACK_TYPE_COUNT ) return CLPack::m_pUnknownMsg_RangeOverflow_Type;
					if ( m_Language[type] == NULL ) return CLPack::m_pUnknownMsg_Empty;				
					return m_Language[type]->GetMessage(num, etc1, etc2);			
	};
public:
	void			Init();
	void			Destroy();

	bool			CheckAbuseWord(char* String);

	
	bool			CheckCharacterName(char* String);


	
	inline	int		GetMessageCount( int type )
	{						
					if ( type > LPACK_TYPE_COUNT ) return 0;
					if ( m_Language[type] == NULL ) return 0;
					return m_Language[type]->GetMessageCount();
	};

	CLanguageManager();
	virtual ~CLanguageManager();
};


#endif 
