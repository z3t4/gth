 
 
 
 
 
 
 
 
 

 
 
 
#include "Decal.h"

 
 
 

 
 
 
 
CFXDecal::CFXDecal	()
{
	m_maxNumVertices	=	0;
}

CFXDecal::~CFXDecal	()
{
}

 
 
 
 
void	CFXDecal::Setup	(	matrix4x4_t	*in_matrix,
							vertex_t	*in_vertices,
							element_t   *in_indices,
							vec3_t		&in_light,
							float		in_size		)
{
	SetInvTransformMatrix ( in_matrix );
	SetVertices ( in_vertices );
	SetIndices ( in_indices );
	SetLight ( in_light );
	SetSize ( in_size );

	Initialize ();
}

 
 
 
 


bool	CFXDecal::Process	(	face_t	&in_face	)
{
	if	(	! CheckFaceNormalAndLightDir ( in_face.planeNormal )	)	return	false;

	m_triangles	=	in_face.elemSize / 3;


	vec3_t	result [3];

	 

	int		vtxIndex [3];
		

	int	index,	subindex;
	int	counter	=	0;

	for	(	index	=	0;	\
			index	<	m_triangles;	\
			++index	)
	{
		for	(	subindex	=	0;	\
				subindex	<	3;	\
				++subindex,	++counter		)
		{
			vtxIndex [subindex]	=	in_face.vertexOffset  +  m_indices [in_face.elemOffset + counter];
		}

		if	(	(	m_vertices [vtxIndex [0]].point [2]	>	m_maxHeight	)	&&
				(	m_vertices [vtxIndex [1]].point [2]	>	m_maxHeight	)	&&
				(	m_vertices [vtxIndex [2]].point [2]	>	m_maxHeight	)	)
			continue;

		if	(	(	m_vertices [vtxIndex [0]].point [2]	<	m_minHeight	)	&&
				(	m_vertices [vtxIndex [1]].point [2]	<	m_minHeight	)	&&
				(	m_vertices [vtxIndex [2]].point [2]	<	m_minHeight	)	)
			continue;

		for	(	subindex	=	0;	\
				subindex	<	3;	\
				++subindex	)
		{
			VectorCopy ( m_vtxBuffer [m_numVertices + subindex].pos, m_vertices [vtxIndex [subindex]].point );

			m_invTransform.Transform ( m_tempBuffer [subindex], m_vertices [vtxIndex [subindex]].point );
			CalculateCrossPoint ( m_tempBuffer [subindex], result [subindex] );
		}

		if	(	! CheckTriangleIsInsideDecal ()	)
		{
			continue;
		}

		CalculateNewTexCoords ( result, m_numVertices );
		m_numVertices	+=	3;
#ifdef	_DEBUG
		  
		if	(	m_numVertices	>	m_maxNumVertices	)
			m_maxNumVertices	=	m_numVertices;
#endif

		
		if	(	m_numVertices	>	( SHADOW_VERTEX_BUFFER_SIZE - 3 )	)
		{
			
			return	false;
		}
	}

	return	true;
}

 
 
 
 
int		CFXDecal::CheckTriangleIsInsideDecal	()
{
	if	(	(	m_tempBuffer [0][0]	>	m_halfLength	)	&&
			(	m_tempBuffer [1][0]	>	m_halfLength	)	&&
			(	m_tempBuffer [2][0]	>	m_halfLength	)	)
		return	false;

	if	(	(	m_tempBuffer [0][0]	<	-m_halfLength	)	&&
			(	m_tempBuffer [1][0]	<	-m_halfLength	)	&&
			(	m_tempBuffer [2][0]	<	-m_halfLength	)	)
		return	false;

	if	(	(	m_tempBuffer [0][1]	>	m_halfLength	)	&&
			(	m_tempBuffer [1][1]	>	m_halfLength	)	&&
			(	m_tempBuffer [2][1]	>	m_halfLength	)	)
		return	false;

	if	(	(	m_tempBuffer [0][1]	<	-m_halfLength	)	&&
			(	m_tempBuffer [1][1]	<	-m_halfLength	)	&&
			(	m_tempBuffer [2][1]	<	-m_halfLength	)	)
		return	false;

	return	true;
}

 
 
 
 
inline int	CFXDecal::CheckFaceNormalAndLightDir	(	vec3_t	&in_normal	)
{
	 
	if	(	VectorDotProduct ( in_normal, m_normLight )	>=	0.0f	)	return	false;
		
	return	true;
}

 
 
 
 
void	CFXDecal::CalculateCrossPoint	(	vec3_t	&in_vertex,
											vec3_t	&in_result	)
{
	float	recip	=	1.0f / m_light [2];
	in_result [0]	=	in_vertex [0]  -  m_light [0] * recip * in_vertex [2];
	in_result [1]	=	in_vertex [1]  -  m_light [1] * recip * in_vertex [2];
	in_result [2]	=	0.0f;
}

 
 
 
 
inline void	CFXDecal::CalculateNewTexCoords	(	vec3_t	in_vertex [],
												int		in_vtxIndex	)
{
	m_vtxBuffer [in_vtxIndex    ].tu	=	( in_vertex [0][0]  -  ( - m_halfLength ) ) / ( 2.0f * m_halfLength );
	m_vtxBuffer [in_vtxIndex    ].tv	=	( in_vertex [0][1]  -  ( - m_halfLength ) ) / ( 2.0f * m_halfLength );

	m_vtxBuffer [in_vtxIndex + 1].tu	=	( in_vertex [1][0]  -  ( - m_halfLength ) ) / ( 2.0f * m_halfLength );
	m_vtxBuffer [in_vtxIndex + 1].tv	=	( in_vertex [1][1]  -  ( - m_halfLength ) ) / ( 2.0f * m_halfLength );

	m_vtxBuffer [in_vtxIndex + 2].tu	=	( in_vertex [2][0]  -  ( - m_halfLength ) ) / ( 2.0f * m_halfLength );
	m_vtxBuffer [in_vtxIndex + 2].tv	=	( in_vertex [2][1]  -  ( - m_halfLength ) ) / ( 2.0f * m_halfLength );
}

 
 
 
 
int	CFXDecal::CopyVertex	(	void	*in_pointer	)
{
	memcpy ( in_pointer, m_vtxBuffer, sizeof(shadow_vertex_t) * m_numVertices );

	return	m_numVertices;
}

 
 
 
 
int	CFXDecal::GetVertexSize	()
{
	return	m_numVertices;
}

 
 
 
 
void	CFXDecal::Initialize	()
{
	memset ( m_vtxBuffer, 0, sizeof(m_vtxBuffer) );
	m_numVertices	=	0;
}
