 
 
 
 
 
 
 
 
 

 
 
 
#ifndef		_FX_OBJECT_DEFINITIONS_H_ADF05H0DFF
#define		_FX_OBJECT_DEFINITIONS_H_ADF05H0DFF

#define		MAX_NEXT_OBJECT		10
#define		MAX_NUM_OBJECTS		10

#define		MAX_NUM_SAMPLE_OBJ	10

#define		MAX_NUM_SAMPLE_IN_CLASS		10

#define		MAX_NUM_OBJCLASS	10

	 
typedef enum
{
	FX_OBJTYPE_PARTICLE,
	FX_OBJTYPE_MAGIC,
	SIZE_FX_OBJTYPE

}	FX_OBJTYPE_t;

	 
typedef enum
{
	FX_OBJPROPERTY_NONE,
	SIZE_FX_OBJPROPERTY

}	FX_OBJPROPERTY_t;

	 
typedef enum
{
	FX_OBJPOSUPDATE_NONE,
	SIZE_FX_OBJPOSUPDATE

}	FX_OBJPOSUPDATE_t;

	 
typedef	enum
{
	FX_OBJDESTYPE_NONE,
	SIZE_FX_DESTTYPE

}	FX_OBJDESTTYPE_t;

	 
typedef enum
{
	FX_OBJTARGET_NONE,
	SIZE_FX_OBJTARGET

}	FX_OBJTARGET_t;

















typedef	struct
{
	int					inuse;

	FX_OBJTYPE_t		classType;				 
	int					idxMDLEntity;			 
	char				itemName [MAX_SIZE_ITEM_NAME];		 

	vec3_t				origin;					 
	vec3_t				angles;					 

	vec3_t				mins;
	vec3_t				maxs;

	int					linkedCluster;

	FX_OBJPROPERTY_t	property;				 
	
	int					target;					 
	int					collided;				 

	float				time_created;			 
	float				time_elapsed;			 
	float				time_nextposupdate;		 
	float				time_destroy;			 

	vec3_t				velocity;				 
	
	vec3_t				pos_end;				 
	vec3_t				pos_dirvector;			 
	
	FX_OBJPOSUPDATE_t	type_posupdate;			 
	FX_OBJDESTTYPE_t	type_destroy;			 
	FX_OBJTARGET_t		type_target;			 

	int					next_object [MAX_NEXT_OBJECT];			 
	float				time_nextobject [MAX_NEXT_OBJECT];		 
	
}	FX_OBJECT_t;




















typedef struct
{

	int		level_min;
	int		level_max;
	int		sampleId;
}	FX_OBJ_SAMPLE_t;

typedef	struct
{
	char	name [32];

	int					num_samples;
	FX_OBJ_SAMPLE_t		samples [MAX_NUM_SAMPLE_IN_CLASS];
}	FX_OBJ_CLASS_t;



#endif