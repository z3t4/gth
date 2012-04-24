



#if !defined(AFX_IFICONCTL_H__8B293311_70C1_48AF_83EE_F262556FB8FA__INCLUDED_)
#define AFX_IFICONCTL_H__8B293311_70C1_48AF_83EE_F262556FB8FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define DISALBE_ICON_DARK_COLOR		0xff000000
#define DISALBE_ICON_RED_COLOR		0xff000000

class CIFIconCtl  
{
public:
	CIFIconCtl();
	virtual ~CIFIconCtl();


	IFTextureInfo_t			m_itemIconTexInfo[MAX_COUNT_IF_ICON_EACH_TEXTURE];
	IFTextureInfo_t			m_skillIconTexInfo[MAX_COUNT_IF_ICON_EACH_TEXTURE];
	IFTextureInfo_t			m_etcIconTexInfo[MAX_COUNT_IF_ICON_EACH_TEXTURE];

	IFIconCtlInfo_t			m_iconInfo[MAX_COUNT_IF_ICON_INFO];

	int						m_nVexCount;
	int						m_nIconInfo;

	CD3DBuffer				*m_vexbuf;		
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	CD3DTextureMng			*m_texMng;
	CFileMng				*m_fileMng;
	CScript					*m_script;

	void		LoadConfig( char* filename );

	void		SetDevice( LPDIRECT3DDEVICE8 pD3DDevice )	{ m_pd3dDevice = pD3DDevice; }
	void		SetTexManager( CD3DTextureMng *texMng )		{ m_texMng = texMng; }
	void		SetFileManager( CFileMng *fileMng	)		{ m_fileMng = fileMng; }
	void		SetScript( CScript *script )				{ m_script = script; }
	void		AddTextureToManager();
	void		UpdateTextureID();
	
	void		PrepareBuffer();
	void		UpdateBuffer();

	void		Release();

	
	void		Draw( int x, int y, int type, int idx, float curCooltime = -1.0f, float maxCooltime = -1.0f, DWORD color = DISALBE_ICON_DARK_COLOR );
	
	
	void		DrawPart( int x, int y, int stancil_x, int stancil_y, int type, int idx, float curCooltime = -1.0f, float maxCooltime = -1.0f, DWORD color = 0xffffffff );
	
	void		BeginFrame();
	void		EndFrame();
};

#endif 
