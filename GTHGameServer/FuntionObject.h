#ifndef _FUNTION_OBJECT_H_
#define _FUNTION_OBJECT_H_


template<typename T>
class Tfind_if: public std::binary_function< T*, T* ,bool>{
public:
	bool operator()(const T *rT ,const T *rTdec)const
	{
		if(*rT==*rTdec) return TRUE;
		else return FALSE;
	}
};

template<typename T>
struct TDelete: public std::unary_function<const T*,void>
{
  template<typename T> void operator()(T* pT) const 
  {
	  if(pT){
		delete pT;
		pT=NULL;
	  }
   
  }
};

#endif _FUNTION_OBJECT_H_