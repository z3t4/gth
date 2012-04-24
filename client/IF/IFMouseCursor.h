



#if !defined(AFX_IFMOUSECURSOR_H__80203AA2_8B57_4F6E_B698_A8163178E8C5__INCLUDED_)
#define AFX_IFMOUSECURSOR_H__80203AA2_8B57_4F6E_B698_A8163178E8C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CIFMouseCursor  
{
public:
	CIFMouseCursor();
	virtual ~CIFMouseCursor();

	LPDIRECT3DDEVICE8		m_pd3dDevice;
	CFileMng				*m_fileMng;

	IFMouseCursorBuffer_t		m_mouseCursorBuffer[MAX_COUNT_CURSOR_BUFFER];
	IFMouseCursorBuffer_t		m_itemCursorBuffer[MAX_COUNT_CURSOR_BUFFER];
	IFMouseCursorBuffer_t		m_skillCursorBuffer[MAX_COUNT_CURSOR_BUFFER];

	void		CopyMouseCursor( IFCommonTexture_t *src, int x, int y, IFMouseCursorBuffer_t* dst );
	void		SetFileManager( CFileMng *fileMng ) { m_fileMng = fileMng; }
	void		SetDevice( LPDIRECT3DDEVICE8 device ) { m_pd3dDevice = device; }


	int			MakeCursor();
	int			MakeMouseCursor( int numOfCursor = 0 );
	int			MakeItemCursor( int numOfCursor = 0 );
	int			MakeSkillCursor( int numOfCursor = 0 );


	int			ChangeCursor( int cursorID, int type = MOUSE_CURSOR_TYPE );
	void		MakeCursorArrow( BYTE* src, DWORD color = 0xffff0000 );

	IFCommonTexture_t* LoadTARGA( char *filename );

};

#endif 

