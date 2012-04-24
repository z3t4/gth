#define PITCH 0
#define YAW   1
#define ROLL  2

#define vec_init(d) {d[0]=0.0f;d[1]=0.0f;d[2]=0.0f;}
#define vec_set(v,a,b,c) {v[0]=a;v[1]=b;v[2]=c;}
#define vec_copy(s,d) {d[0]=s[0];d[1]=s[1];d[2]=s[2];}
#define vec_add(a,b,c) {c[0]=a[0]+b[0];c[1]=a[1]+b[1];c[2]=a[2]+b[2];}
#define vec_sub(a,b,c) {c[0]=a[0]-b[0];c[1]=a[1]-b[1];c[2]=a[2]-b[2];} 
#define vec_scale(a,b,c) {c[0]=b*a[0];c[1]=b*a[1];c[2]=b*a[2];}
#define vec_dot(a,b) (a[0]*b[0]+a[1]*b[1]+a[2]*b[2])
#define vec_avg(a,b,c) {c[0]=(a[0]+b[0])*0.5f;c[1]=(a[1]+b[1])*0.5f;c[2]=(a[2]+b[2])*0.5f;}
#define vec_cmp(a,b) (a[0]==b[0]&&a[1]==b[1]&&a[2]==b[2])
#define	vec_ma(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))

#define vec2_copy(s,d) {d[0]=s[0];d[1]=s[1];}
#define vec2_avg(a,b,c) {c[0]=(a[0]+b[0])*0.5f;c[1]=(a[1]+b[1])*0.5f;}
#define M_PI ((float)3.141592654)
#define DEG2RAD( a ) ( ( (a) * M_PI ) / 180.0F )
#define RAD2DEG( a ) ( ( (a) * 180.0f ) / M_PI )

typedef int				grid_t[2];
typedef float			vec3_t[3];

void Normalize( float* n );
void RotateXCoord(vec3_t A, float Angle);
void RotateYCoord(vec3_t A, float Angle);
void RotateZCoord(vec3_t A, float Angle);

void vectoangles( const vec3_t value1, vec3_t angles );