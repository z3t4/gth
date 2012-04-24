 
 
 
 
 
 
 
 
 
#ifndef _ITEM_RENDERER_H_HGF02H032HR
#define _ITEM_RENDERER_H_HGF02H032HR

 
 
 

class	FX_CItemRenderer
{
	 
public:
	FX_CItemRenderer	();
	~FX_CItemRenderer	()	{};

	 
private:
		 
	CD3DBuffer				*m_VB;
	CD3DBuffer				*m_IB;

	LPDIRECT3DDEVICE8		m_pd3dDevice;

		 
	FX_CItemManager			*m_pItemManager;
	CD3DRender				*m_D3DRender;

	 
public:
	bool		CreateBuffers	();
	bool		FillBuffers		();
	void		ReleaseBuffers	();

	bool		Render	(	const UINT	&iVertexStart,	const UINT	&iNumVertex,	const UINT	&iIndexStart,	const UINT	&iPrimitiveCount	);
	bool		PrepareRender	();
	void		SetWorldMatrix	(	matrix4x4_t	&matWorld	);
	void		Cleanup	();

		 
	void		SetDevice	(	LPDIRECT3DDEVICE8	device	)	{	m_pd3dDevice	=	device;	}
	void		SetItemManager	(	FX_CItemManager		*itemManager	)	{	m_pItemManager	=	itemManager;	};
	void		SetRenderer		(	CD3DRender		*d3dRender	)			{	m_D3DRender		=	d3dRender;		};

	void		RenderShadow (	Fx_CHARACTER_t		*in_char,	FX_CItemManager		*itemManager,	FX_CBoneManager		*m_pBoneManager,	matrix4x4_t		*in_transform	);

private:
	bool		Initialize();

};

#endif