

#ifndef _TAGHELPER_H_
#define _TAGHELPER_H_


struct tagHelper
{			
	public:
		enum{
			MAX_MEMBER=10,
			MAX_Accept_HELPERS	= 1,
			MAX_Accept_TAKERS	=10,
		};		
		
		struct tagPoint{
			int		iMyPoint;	
			int		iHelpPoint;
			int		iTotalHelpPoint;
		};

		struct tagMemberList{
			public:
				enum enumMode{
					HELPER	=0,
					TAKER	=1,
				};
				struct tagMember{					
					char	name[NAMESTRING+1];					
					char    id[IDSTRING+1];
					BOOL    bIsMatchHELPERandTACKER;
				};			


			
			public:
				enum enumMode	    mode;
				struct tagMember	Member[MAX_MEMBER];				
				int					count;
		};

	public:	
		struct tagPoint		 tPoint;
		struct tagMemberList List;		
		BOOL				 bBusy;
};





struct HelperEntry
{
	byte	level ;
	byte	job ;
	byte	worldIdx ;
	char	name[NAMESTRING+1];
	BOOL	bsync;
};

#endif