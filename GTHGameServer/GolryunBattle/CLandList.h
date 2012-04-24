





#ifndef _CLANDLIST_H_
#define _CLANDLIST_H_

class CRandList  
{
	public:
		typedef std::list<int>		NUMLIST;
		typedef NUMLIST::iterator	NUMLIST_ITOR;

		NUMLIST						m_numList;

	public:
		CRandList();
		virtual ~CRandList();

		void		Initialize();
		void		PushData(int iData);
		BOOL		GetRandomData(int &iData);

};


#endif 
