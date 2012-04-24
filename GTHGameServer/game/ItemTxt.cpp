




#include "../global.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum
{
	EXCEL_FIELD_NORMAL=0,
	EXCEL_FIELD_FILE_END,
	EXCEL_FIELD_LINE_END,
};








CItemTxt g_TxtApp;





CItemTxt::CItemTxt()
{
	m_NameList.reserve(MAX_NUMBER_OF_ITEM_GENERATE_TABLE);
	m_DescList.reserve(MAX_NUMBER_OF_ITEM_GENERATE_TABLE);
}

CItemTxt::~CItemTxt()
{

}





void CItemTxt::ClearNameList()
{
	int size=m_NameList.size();
	for(int i=0;i<size;i++)
	{
		m_NameList.pop_back();
	}
}

LPSTR CItemTxt::GetName(int idx)
{
	int size=m_NameList.size();
	
	for(int i=0;i<size;i++)
	{
		if(m_NameList[i].m_Index == idx)
		{
			return m_NameList[i].m_Txt;
		}
	}

	return "NULL";
}



LPSTR CItemTxt::GetDesc(int idx)
{
	int size=m_DescList.size();

	for(int i=0;i<size;i++)
	{
		if(m_DescList[i].m_Index == idx)
		{
			return m_DescList[i].m_Txt;
		}
	}

	return "NULL";
}


void CItemTxt::ClearDescList()
{
	int size=m_DescList.size();
	for(int i=0;i<size;i++)
	{
		m_DescList.pop_back();
	}
}







void CItemTxt::ClearClassNameList()
{
	int size=m_ClassNameList.size();
	for(int i=0;i<size;i++)
	{
		m_ClassNameList.pop_back();
	}
}





void CItemTxt::GetOnlyTxt()
{
	int i,k,j;
	
	char txt[2][256];
	wsprintf(txt[0],"%s",m_Token);

	char ttt[222];
	wsprintf(ttt,"aaa");
	
	int len=strlen(m_Token);

	for(i=0;i<len;i++)
	{
		if(txt[0][i]=='"')
		{
			i++;
			break;
		}
	}

	j=0;
	for(k=i;k<len;k++)
	{
		if(txt[0][k]=='"') break;

		txt[1][j++]=txt[0][k];
	}

	txt[1][j]='\0';

	

	wsprintf(m_Token,"%s",txt[1]);
}

void CItemTxt::BindItemTable()
{
	for(int i=0;i<MAX_NUMBER_OF_ITEM_TABLE;i++)
	{
		
	}

}

void CItemTxt::STRCPY(char &Dest, char &Source)
{
	int DestLen = lstrlen(&Dest);
	int SourceLen = lstrlen(&Source);

	int DestSize = sizeof(Dest);
	int SourceSize = sizeof(Source);

	int CopyLen = SourceLen;
	if(DestSize < SourceLen)
	{
		CopyLen = DestSize;

		
	}

	lstrcpyn(&Dest,&Source,CopyLen);
}

bool CItemTxt::LoadFile(char * FileName)
{	

	char seps[]   = ":\t\n";

	ClearNameList();
	ClearDescList();
	ClearClassNameList();

	char *str_token;

	int n=0, endfile,skipFlag = false;
	char token[255];

	if(!LoadTokenBuffer((char *) (LPCTSTR) FileName)) return false;

	m_NameList.clear();
	m_DescList.clear();

	CItemTxtData tempTxt;

	while( 1 )
	{
		NextCVSTokenReturn(token, endfile );
		if( token[0] != NULL ) 
		{
			
			if(token[0] == '/' || token[0] == '[') continue;

			
			str_token = strtok( token, seps );
			while( str_token != NULL )
			{
				
				if(str_token[0] == '/')
				{
				}
				else if(str_token[0] >= 0x30 && str_token[0] <= 0x39)
				{
					if(tempTxt.m_Index >=0)
					{	
						
						wsprintf(m_Token,"%s",tempTxt.m_Txt);
						GetOnlyTxt();
						
						wsprintf(tempTxt.m_Txt,"%s",m_Token);

						if(tempTxt.m_Index >= 20000)	
						{
							m_ClassNameList.push_back(tempTxt);
						}
						else if(tempTxt.m_Index >= 10000 && tempTxt.m_Index < 20000)	
						{
							m_DescList.push_back(tempTxt);
						}
						else if(tempTxt.m_Index >= 1000 && tempTxt.m_Index < 10000)	
						{
							m_NameList.push_back(tempTxt);
						}
						n++;
					}

					tempTxt.Init();
					tempTxt.m_Index = atoi(str_token);
				}
				else
				{
					strcat(tempTxt.m_Txt,str_token);
					
				}
			
				str_token = strtok( NULL, seps );
			}	
		}
		else
		{
			
			if( endfile == EXCEL_FIELD_FILE_END ) 
			{
				break;
			}
			continue;
		}
		
		if( endfile == EXCEL_FIELD_FILE_END ) 
		{
			break;
		}
	}

	if(tempTxt.m_Index >=0)
	{						
		GetOnlyTxt();

		if(tempTxt.m_Index >= 20000)	
		{			
			m_ClassNameList.push_back(tempTxt);
			
		}
		else if(tempTxt.m_Index >= 10000 && tempTxt.m_Index < 20000)	
		{			
			m_DescList.push_back(tempTxt);
			
		}
		else if(tempTxt.m_Index >= 1000 && tempTxt.m_Index < 10000)	
		{		
			m_NameList.push_back(tempTxt);
			
		}
		n++;
	}

	DeleteTokenBuffer();

	return true;
}

