



#if !defined(AFX_IFNUMCTL_H__9D36D817_F6EA_4284_A21B_35B470869671__INCLUDED_)
#define AFX_IFNUMCTL_H__9D36D817_F6EA_4284_A21B_35B470869671__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

typedef struct
{
	int		idx;
	int		type;

	float	startTime;
	float	receiveTime;
	float	velocity;
	float	x;
	float	y;

	int		stun;

	int		attackerIdx;
	int		attackerType;
	int		defenderIdx;
	int		defenderType;

	int		damage;

} IFDmgNumber_t;

typedef struct
{
	int		texCoordX;
	int		texCoordY;
	int		sizeX;
	int		sizeY;
} IFDmgNumberTexInfo_t;

#define MAX_COUNT_IF_DAMAGE_NUMBER		256

class CIFNumCtl  
{
public:
	CIFNumCtl();
	virtual ~CIFNumCtl();

	IFDmgNumberTexInfo_t	m_dmgTexInfo[64];

	LPDIRECT3DDEVICE8		m_pd3dDevice;
	CD3DTextureMng			*m_texMng;

	IFDmgNumber_t			m_dmgMsg[MAX_COUNT_IF_DAMAGE_NUMBER];
	int						m_nDmgMsg;

	CD3DBuffer				*m_damageVexbuf;			
	int						m_nDamageVex;			
	int						m_nNumDamage;
	int						m_vexCount;

	
	void		SetDevice( LPDIRECT3DDEVICE8 pD3DDevice )	{ m_pd3dDevice = pD3DDevice; }
	void		SetTexManager( CD3DTextureMng *texMng )		{ m_texMng = texMng; }
	
	void		PrepareBuffer();			

	void		AddDamageNumber( int idx, int type, int attackerIdx, int attackerType, int damage, int stun );
	void		ReadyDamage( Fx_CHARACTER_t* character );
	void		ReadyDamage( int attackerType, int attackerIdx, int defenderType, int defenderIdx );
	int			CheckHitTarget( int attackerType, int attackerIdx, int defenderType, int defenderIdx );
	void		RenderDamage();

	void		ProcessDamage();

	void		Release();
	void		Render();
};

#endif 
