 
 
 
 
 
 
 
 
 

 
 
 
#ifndef _RENDER_CONSTANT_H_298HF9H19H21
#define _RENDER_CONSTANT_H_298HF9H19H21

struct TEXTUREVERTEX
{
	vec3_t			vecPos;
	vec3_t			vecNormal;
	FLOAT			tu;
	FLOAT			tv;
};

struct DIFFUSETEXVERTEX
{
	vec3_t			vecPos;
	D3DCOLOR		d3dColor;
	FLOAT			tu;
	FLOAT			tv;
};

struct	NORMALVERTEX
{
	vec3_t			vecPos;
	vec3_t			vecNormal;

};


#define D3DFVF_TEXTUREVERTEX	( D3DFVF_NORMAL | D3DFVF_XYZ | D3DFVF_TEX1 )
#define	D3DFVF_DIFFUSETEXVERTEX	(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

































#define	D3DCOLOR_GOLD					0xffffd700		 

































































#define	D3DCOLOR_WHITE					0xffffffff		 


































#define	D3DCOLOR_LIGHTBLUE				0xffadd8e6



#define	D3DCOLOR_LIGHTGREEN				0xff90ee90

#define	D3DCOLOR_LIGHTPINK				0xffffb6c1







 
 
 

#endif
 