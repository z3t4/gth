#ifndef _MATRIX_H
#define _MATRIX_H

#include <math.h>
#include "fastmath.h"

typedef float mat3x3_t[9];
typedef float mat4x4_t[16];

struct matrix4x4_t
{
    matrix4x4_t() { Identity(); }
    matrix4x4_t( const float* s )
	{
        xx = s[0];  xy = s[1];  xz = s[2];  xw = s[3];
        yx = s[4];  yy = s[5];  yz = s[6];  yw = s[7];
        zx = s[8];  zy = s[9];  zz = s[10]; zw = s[11];
        wx = s[12]; wy = s[13]; wz = s[14]; ww = s[15];
    }
    matrix4x4_t( const matrix4x4_t& s )
	{
        xx = s.xx; xy = s.xy; xz = s.xz; xw = s.xw;
        yx = s.yx; yy = s.yy; yz = s.yz; yw = s.yw;
        zx = s.zx; zy = s.zy; zz = s.zz; zw = s.zw;
        wx = s.wx; wy = s.wy; wz = s.wz; ww = s.ww;
    }

    
	matrix4x4_t& operator=( const matrix4x4_t& s )
	{
        xx = s.xx; xy = s.xy; xz = s.xz; xw = s.xw;
        yx = s.yx; yy = s.yy; yz = s.yz; yw = s.yw;
        zx = s.zx; zy = s.zy; zz = s.zz; zw = s.zw;
        wx = s.wx; wy = s.wy; wz = s.wz; ww = s.ww;
		return (*this);
	}
    matrix4x4_t& operator=( const float* s ) 
	{
        xx = s[0];  xy = s[1];  xz = s[2];  xw = s[3];
        yx = s[4];  yy = s[5];  yz = s[6];  yw = s[7];
        zx = s[8];  zy = s[9];  zz = s[10]; zw = s[11];
        wx = s[12]; wy = s[13]; wz = s[14]; ww = s[15];
        return (*this);
    }

	inline void Identity();
    inline void RotateX( const float angle);
    inline void RotateY( const float angle);
    inline void RotateZ( const float angle);
	inline void RotateXZ( const vec3_t angles );
    inline void Scale( const float x , const float y , const float z );

	void Scale( vec3_t v ) { Scale( v[0] , v[1] , v[2] ); }

    inline void Translate( const float x , const float y , const float z );

	void Translate( vec3_t v ) { Translate( v[0] , v[1] , v[2] ); }

    inline void Frustum( double left , double right , double bottom ,
		                 double top , double zNear , double zFar );
	inline void Projection( double fovy , double aspect , double zNear , double zFar );
	inline void Projection_LH( double fovy , double aspect, double zNear , double zFar );
	inline void LookAt( const vec3_t eye , const vec3_t at , const vec3_t up );

	inline matrix4x4_t operator*( const matrix4x4_t& s ) const;
	inline void operator*=( const matrix4x4_t& s ) 
	{
		(*this) = (*this)*s;
	}

	inline void  TransformCoord( vec3_t d , const vec3_t v );
    inline void  TransformNormal( vec3_t d , const vec3_t v );
    inline float Transform ( vec3_t d , const vec3_t v );

    inline void Multiply( vec4_t d , const vec4_t s );

	const float operator[]( int i) const { return m[i]; }
	float& operator[]( int i ) { return m[i]; }
	operator const float*( void ) const { return m; }

    static inline matrix4x4_t& Transpose( matrix4x4_t& d , const matrix4x4_t& s );
    static inline matrix4x4_t& Invert( matrix4x4_t& d , const matrix4x4_t& s );
    static inline matrix4x4_t& Multiply( matrix4x4_t& d , const matrix4x4_t& b , const matrix4x4_t& a );

	static inline void  TransformCoord( vec3_t d , const matrix4x4_t& m , const vec3_t s );
    static inline void  TransformNormal( vec3_t d , const matrix4x4_t& m , const vec3_t s );
    static inline float Transform( vec3_t d , const matrix4x4_t& m , const vec3_t s );

    static inline void Multiply( vec4_t d , const matrix4x4_t& m , const vec4_t s );

    union 
	{
	    float m[4*4];
        struct 
		{
            float xx,xy,xz,xw;
            float yx,yy,yz,yw;
            float zx,zy,zz,zw;
            float wx,wy,wz,ww;
        };
        struct 
		{
            float a00,a10,a20,a30;
            float a01,a11,a21,a31;
            float a02,a12,a22,a32;
            float a03,a13,a23,a33;
        };          
    };
};

inline void matrix4x4_t::Identity() 
{
    xx = 1; xy = 0; xz = 0; xw = 0;
    yx = 0; yy = 1; yz = 0; yw = 0;
    zx = 0; zy = 0; zz = 1; zw = 0;
    wx = 0; wy = 0; wz = 0; ww = 1;
}

inline void matrix4x4_t::RotateX( const float angle ) 
{
    const float c = (float) cos( deg2rad( angle ) );
	const float s = (float) sin( deg2rad( angle ) );

    yy = c;
    yz = s;
    zy = -s;
    zz = c;
}

inline void matrix4x4_t::RotateY( const float angle ) 
{
    const float c = (float) cos( deg2rad( angle ) );
	const float s = (float) sin( deg2rad( angle ) );

    xx = c;   
    xz = -s;
    zx = s;  
    zz = c;
}

inline void matrix4x4_t::RotateZ( const float angle ) 
{
    const float c = (float) cos( deg2rad( angle ) );
	const float s = (float) sin( deg2rad( angle ) );

    xx = c;   
    xy = s;
    yx = -s;  
    yy = c;
}

inline void matrix4x4_t::RotateXZ( const vec3_t angles ) 
{
	float cosPitch = (float) cos( deg2rad( angles[0] ) );
	float sinPitch = (float) sin( deg2rad( angles[0] ) );
	float cosYaw = (float) cos( deg2rad( angles[1] ) );
	float sinYaw = (float) sin( deg2rad( angles[1] ) );

	xx = cosYaw; 
    xy = sinYaw;
	xz = 0.0f;
    yx = cosPitch * (-sinYaw);
    yy = cosPitch * cosYaw;
	yz = sinPitch;
	zx = sinPitch * sinYaw;
	zy = (-sinPitch) * cosYaw;
	zz = cosPitch;

	
}

inline void matrix4x4_t::Scale( const float x , const float y , const float z )
{
    xx = x;   
    yy = y;
    zz = z;  
}

inline void matrix4x4_t::Translate( const float x , const float y , const float z )
{
    wx += xx * x + yx * y + zx * z;
    wy += xy * x + yy * y + zy * z;
    wz += xz * x + yz * y + zz * z;
    ww += xw * x + yw * y + zw * z;
}


inline void matrix4x4_t::Frustum( double left , double right , double bottom ,
		                          double top , double zNear , double zFar )
{
	a00 = (float)( ( 2.0 * zNear ) / ( right - left ) );
	a10 = 0.0f;
	a20 = 0.0f;
	a30 = 0.0f;

	a01 = 0.0f;
	a11 = (float)( ( 2.0 * zNear ) / ( top - bottom ) );
	a21 = 0.0f;
	a31 = 0.0f;

	a02 = (float)( ( right + left ) / ( right - left ) );
	a12 = (float)( ( top + bottom ) / ( top - bottom ) );
	a22 = -(float)( ( zFar + zNear ) / ( zFar - zNear ) );
	a32 = -1.0f;

	a03 = 0.0f;
	a13 = 0.0f;
	a23 = -(float)( ( 2.0 * zFar * zNear ) / ( zFar - zNear ) );
	a33 = 0.0f;
}

inline void matrix4x4_t::Projection( double fovy , double aspect , double zNear , double zFar )
{
	double ymax = zNear * tan( fovy *__PID / 360.0 );
	double ymin = -ymax;
	double xmin = ymin * aspect;
	double xmax = ymax * aspect;

	Frustum( xmin , xmax , ymin , ymax, zNear , zFar );
}


inline void matrix4x4_t::Projection_LH( double fovy , double aspect, double zNear , double zFar )
{
    Projection( fovy , aspect , zNear , zFar );
    
    
    a00 = -a00;
    a11 = -a11;
    a22 = -a22;
    a32 = -a32;
}


inline void matrix4x4_t::LookAt( const vec3_t eye , const vec3_t at , const vec3_t up )
{
	vec3_t x , y , z;
	z[0] = eye[0] - at[0];
	z[1] = eye[1] - at[1];
	z[2] = eye[2] - at[2];
	VectorSubtract( z, eye, at );
	VectorNormalize( z );
	VectorCrossProduct( x , up , z );
	VectorNormalize( x );
	VectorCrossProduct( y , z , x );

	m[0]  = x[0];
	m[4]  = x[1];
	m[8]  = x[2];
	m[12] = -VectorDotProduct( x , eye );

	m[1]  = y[0];
	m[5]  = y[1];
	m[9]  = y[2];
	m[13] = -VectorDotProduct( y , eye );

	m[2]  = z[0];
	m[6]  = z[1];
	m[10] = z[2];
	m[14] = -VectorDotProduct( z , eye );

	m[3] = m[7] = m[11] = 0.0f;
	m[15] = 1.0f;
}

inline matrix4x4_t matrix4x4_t::operator*( const matrix4x4_t& s ) const
{
	matrix4x4_t r;
	r.m[0]  = m[0] * s.m[0]  + m[4] * s.m[1]  + m[8]  * s.m[2]  + m[12] * s.m[3];
	r.m[1]  = m[1] * s.m[0]  + m[5] * s.m[1]  + m[9]  * s.m[2]  + m[13] * s.m[3];
	r.m[2]  = m[2] * s.m[0]  + m[6] * s.m[1]  + m[10] * s.m[2]  + m[14] * s.m[3];
	r.m[3]  = m[3] * s.m[0]  + m[7] * s.m[1]  + m[11] * s.m[2]  + m[15] * s.m[3];
	r.m[4]  = m[0] * s.m[4]  + m[4] * s.m[5]  + m[8]  * s.m[6]  + m[12] * s.m[7];
	r.m[5]  = m[1] * s.m[4]  + m[5] * s.m[5]  + m[9]  * s.m[6]  + m[13] * s.m[7];
	r.m[6]  = m[2] * s.m[4]  + m[6] * s.m[5]  + m[10] * s.m[6]  + m[14] * s.m[7];
	r.m[7]  = m[3] * s.m[4]  + m[7] * s.m[5]  + m[11] * s.m[6]  + m[15] * s.m[7];
	r.m[8]  = m[0] * s.m[8]  + m[4] * s.m[9]  + m[8]  * s.m[10] + m[12] * s.m[11];
	r.m[9]  = m[1] * s.m[8]  + m[5] * s.m[9]  + m[9]  * s.m[10] + m[13] * s.m[11];
	r.m[10] = m[2] * s.m[8]  + m[6] * s.m[9]  + m[10] * s.m[10] + m[14] * s.m[11];
	r.m[11] = m[3] * s.m[8]  + m[7] * s.m[9]  + m[11] * s.m[10] + m[15] * s.m[11];
	r.m[12] = m[0] * s.m[12] + m[4] * s.m[13] + m[8]  * s.m[14] + m[12] * s.m[15];
	r.m[13] = m[1] * s.m[12] + m[5] * s.m[13] + m[9]  * s.m[14] + m[13] * s.m[15];
	r.m[14] = m[2] * s.m[12] + m[6] * s.m[13] + m[10] * s.m[14] + m[14] * s.m[15];
	r.m[15] = m[3] * s.m[12] + m[7] * s.m[13] + m[11] * s.m[14] + m[15] * s.m[15];

	return r;
}

inline void matrix4x4_t::TransformCoord( vec3_t d , const vec3_t s )
{
    d[0] = s[0] * xx + s[1] * yx + s[2] * zx + wx;
    d[1] = s[0] * xy + s[1] * yy + s[2] * zy + wy;
    d[2] = s[0] * xz + s[1] * yz + s[2] * zz + wz;
}

inline void matrix4x4_t::TransformNormal( vec3_t d , const vec3_t s )
{
    d[0] = s[0] * xx + s[1] * yx + s[2] * zx;
    d[1] = s[0] * xy + s[1] * yy + s[2] * zy;
    d[2] = s[0] * xz + s[1] * yz + s[2] * zz;
}

inline float matrix4x4_t::Transform( vec3_t d , const vec3_t s )
{
    const float w = 1.0f / ( s[0] * xw + s[1] * yw + s[2] * zw + ww );
    d[0] = ( s[0] * xx + s[1] * yx + s[2] * zx + wx ) * w;
    d[1] = ( s[0] * xy + s[1] * yy + s[2] * zy + wy ) * w;
    d[2] = ( s[0] * xz + s[1] * yz + s[2] * zz + wz ) * w;

	return w;
}

inline void matrix4x4_t::Multiply( vec4_t d , const vec4_t s )
{
	d[0] = s[0] * xx + s[1] * xy + s[2] * xz + s[3] * xw;
	d[1] = s[0] * yx + s[1] * yy + s[2] * yz + s[3] * yw;
	d[2] = s[0] * zx + s[1] * zy + s[2] * zz + s[3] * zw;
	d[3] = s[0] * wx + s[1] * wy + s[2] * wz + s[3] * ww;
}

inline matrix4x4_t& matrix4x4_t::Transpose( matrix4x4_t& d , const matrix4x4_t& s )
{
    d.xx = s.xx;
    d.yx = s.xy;
    d.zx = s.xz;
    d.wx = s.xw;

    d.xy = s.yx;
    d.yy = s.yy;
    d.zy = s.yz;
    d.wy = s.yw;

    d.xz = s.zx;
    d.yz = s.zy;
    d.zz = s.zz;
    d.wz = s.zw;

    d.xw = s.wx;
    d.yw = s.wy;
    d.zw = s.wz;
    d.ww = s.ww;

    return d;
}


inline matrix4x4_t& matrix4x4_t::Invert( matrix4x4_t& d , const matrix4x4_t& s )
{
    d.xx = s.xx;
    d.xy = s.yx;
    d.xz = s.zx;
    d.yx = s.xy;
    d.yy = s.yy;
    d.yz = s.zy;
    d.zx = s.xz;
    d.zy = s.yz;
    d.zz = s.zz;
  
      
    d.wx = -( s.wx * d.xx + s.wy * d.yx + s.wz * d.zx );
    d.wy = -( s.wx * d.xy + s.wy * d.yy + s.wz * d.zy );
    d.wz = -( s.wx * d.xz + s.wy * d.yz + s.wz * d.zz );
  
    
    d.xw = d.yw = d.zw = 0.0; 
    d.ww = 1.0;

    return d;
}

inline matrix4x4_t& matrix4x4_t::Multiply( matrix4x4_t& d , const matrix4x4_t& b , const matrix4x4_t& a )
{
	d[0]  = a[0] * b[0]  + a[4] * b[1]  + a[8]  * b[2]  + a[12] * b[3];
	d[1]  = a[1] * b[0]  + a[5] * b[1]  + a[9]  * b[2]  + a[13] * b[3];
	d[2]  = a[2] * b[0]  + a[6] * b[1]  + a[10] * b[2]  + a[14] * b[3];
	d[3]  = a[3] * b[0]  + a[7] * b[1]  + a[11] * b[2]  + a[15] * b[3];
	d[4]  = a[0] * b[4]  + a[4] * b[5]  + a[8]  * b[6]  + a[12] * b[7];
	d[5]  = a[1] * b[4]  + a[5] * b[5]  + a[9]  * b[6]  + a[13] * b[7];
	d[6]  = a[2] * b[4]  + a[6] * b[5]  + a[10] * b[6]  + a[14] * b[7];
	d[7]  = a[3] * b[4]  + a[7] * b[5]  + a[11] * b[6]  + a[15] * b[7];
	d[8]  = a[0] * b[8]  + a[4] * b[9]  + a[8]  * b[10] + a[12] * b[11];
	d[9]  = a[1] * b[8]  + a[5] * b[9]  + a[9]  * b[10] + a[13] * b[11];
	d[10] = a[2] * b[8]  + a[6] * b[9]  + a[10] * b[10] + a[14] * b[11];
	d[11] = a[3] * b[8]  + a[7] * b[9]  + a[11] * b[10] + a[15] * b[11];
	d[12] = a[0] * b[12] + a[4] * b[13] + a[8]  * b[14] + a[12] * b[15];
	d[13] = a[1] * b[12] + a[5] * b[13] + a[9]  * b[14] + a[13] * b[15];
	d[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
	d[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];

	return d;
}

inline void matrix4x4_t::TransformCoord( vec3_t d , const matrix4x4_t& m , const vec3_t s )
{
    d[0] = s[0] * m.xx + s[1] * m.yx + s[2] * m.zx + m.wx;
    d[1] = s[0] * m.xy + s[1] * m.yy + s[2] * m.zy + m.wy;
    d[2] = s[0] * m.xz + s[1] * m.yz + s[2] * m.zz + m.wz;
}

inline void matrix4x4_t::TransformNormal( vec3_t d , const matrix4x4_t& m , const vec3_t s )
{
    d[0] = s[0] * m.xx + s[1] * m.yx + s[2] * m.zx;
    d[1] = s[0] * m.xy + s[1] * m.yy + s[2] * m.zy;
    d[2] = s[0] * m.xz + s[1] * m.yz + s[2] * m.zz;
}

inline float matrix4x4_t::Transform( vec3_t d , const matrix4x4_t& m , const vec3_t s )
{
    const float w = 1.0f / ( s[0] * m.xw + s[1] * m.yw + s[2] * m.zw + m.ww );
    d[0] = ( s[0] * m.xx + s[1] * m.yx + s[2] * m.zx + m.wx ) * w;
    d[1] = ( s[0] * m.xy + s[1] * m.yy + s[2] * m.zy + m.wy ) * w;
    d[2] = ( s[0] * m.xz + s[1] * m.yz + s[2] * m.zz + m.wz ) * w;

	return w;
}

inline void matrix4x4_t::Multiply( vec4_t d , const matrix4x4_t& m , const vec4_t s )
{
	d[0] = s[0] * m.xx + s[1] * m.xy + s[2] * m.xz + s[3] * m.xw;
    d[1] = s[0] * m.yx + s[1] * m.yy + s[2] * m.yz + s[3] * m.yw;
    d[2] = s[0] * m.zx + s[1] * m.zy + s[2] * m.zz + s[3] * m.zw;
    d[3] = s[0] * m.wx + s[1] * m.wy + s[2] * m.wz + s[3] * m.ww;
}


#endif