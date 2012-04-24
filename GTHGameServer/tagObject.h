


#ifndef _TAGOBJECT_H_
#define _TAGOBJECT_H_


struct tagObject{
	public:
		enum enumType{
			Player,
			Monster,
			NPC,
			Object,
		};
	public:
		enum enumType enumType;	
		int idx;
};



#endif