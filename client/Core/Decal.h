 
 
 
 
 
 
 
 
 

 
 
 
#ifndef	_FX_DECAL_H_ALKJASD0923804980234S
#define _FX_DECAL_H_ALKJASD0923804980234S

#include "../math/math.h"
#include "gth_bsp.h"
#include <d3dx8.h>

#define		SHADOW_VERTEX_BUFFER_SIZE	2000

 
 
 
	

	

typedef	struct 
{
	vec3_t	pos;
	float	tu,	tv;
}	shadow_vertex_t;

class	CFXDecal
{
	 
public:
	CFXDecal	();
	virtual	~CFXDecal	();

	 
public:

	inline void	SetInvTransformMatrix	(	matrix4x4_t	*in_matrix	)	{	D3DXMatrixInverse ( (D3DXMATRIX *)&m_invTransform, NULL, (D3DXMATRIX *)in_matrix );	};

	inline void	SetVertices	(	vertex_t	*in_vertices	)	{	m_vertices	=	in_vertices;	};
	inline void SetIndices	(	element_t   *in_indices		)	{	m_indices	=	in_indices;		};
	 
		 

	inline void SetLight	(	vec3_t	&in_light	)		{	VectorCopy ( m_light, in_light );	};
		
	inline void	SetSize	(	float	in_size	)	{	m_halfLength	=	in_size;	};
	void	Initialize	();
	void	Setup	(	matrix4x4_t	*in_matrix,	vertex_t	*in_vertices,	element_t        *in_indices,	vec3_t	&in_light,	float	in_size	);

	 
	void	SetMinMaxHeight	(	float	in_minHeight,	float	in_maxHeight	)	{	m_minHeight	=	in_minHeight;	m_maxHeight	=	in_maxHeight;	};
		
	 
	inline void	SetSunLight	(	vec3_t	&in_sunLight	)	{	VectorCopy ( m_sunLight, in_sunLight	);	VectorNormalize2 ( m_normLight, in_sunLight ); 	};
		
	
	
	
	bool	Process	(	face_t	&in_face	);
	inline int	CheckFaceNormalAndLightDir	(	vec3_t	&in_normal	);
	void	CalculateCrossPoint		(	vec3_t	&in_vertex,	vec3_t	&in_result	);
	inline void	CalculateNewTexCoords	(	vec3_t	in_vertex [],	int		in_vtxIndex	);
	int		CheckTriangleIsInsideDecal	();
	int		CopyVertex	(	void	*in_pointer	);
	int		GetVertexSize	();

	int		m_maxNumVertices;

	 
protected:
	vec3_t		m_origin;
	matrix4x4_t	*m_srcTransform;
	matrix4x4_t	m_invTransform;

	vec3_t		m_light;
	vertex_t	*m_vertices;
	element_t   *m_indices;
	
	float		m_halfLength;

	shadow_vertex_t		m_vtxBuffer [SHADOW_VERTEX_BUFFER_SIZE];
	vec3_t		m_tempBuffer [3];
	int			m_numVertices;

	 
	vec3_t		m_normLight;
	float		m_minHeight;
	float		m_maxHeight;
		
	 
	vec3_t		m_sunLight;
		

	int			m_triangles;
};
#endif