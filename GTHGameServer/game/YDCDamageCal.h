



#if !defined(AFX_YDCDAMAGECAL_H__F48B2DEC_F20F_4933_8C12_E339828971E0__INCLUDED_)
#define AFX_YDCDAMAGECAL_H__F48B2DEC_F20F_4933_8C12_E339828971E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class YDCDamageCal  
{
	private:
		struct tagYDCharactor{
			public:
				struct tagEntity{
					int Idx;
					entityType_t Type;
				};

			public:
				struct tagEntity	Entity;
				float				optValue[NUM_OPTION_VALUE];
				float				calDefensePower;
				float				calPositiveRes;
				float				calNegativeRes;	
				int					level;
				int					calCriticalDmg;
				float				calMaxRA;
		};

 
		struct tagYDResistanceVar{
			float physics;   
			float positive;  
			float negative;  
		};

		struct tagYDDamageVar{
			float physics;
			float positive;
			float negative;
		};
	private:
		BOOL m_bOK;

	private:
		static inline void	YDMakeCharactorStruct(struct tagYDCharactor& out_Charactor,const playerCharacter_t* in_pPlayer);	
		static void			YDCalResistance(const struct tagYDCharactor* in_pDefenceCharactor,struct tagYDResistanceVar& out_ResistanceVar);
		static inline void	YDMakeCharactorStruct(struct tagYDCharactor& out_Charactor,const monsterCharacter_t* in_pMonster);
		static void			YDCalAdditionalDamage(const struct tagYDCharactor& in_pDefenceCharactor,const message_t* in_pMessage,struct tagYDDamageVar& out_AdditionalDamageVar);
		static void			YDCalDamage(const message_t* in_pMessage,struct tagYDDamageVar& out_DamageVar);
		static void			YDApplyResistanceAtDamage(
								const message_t* in_pMessage,
								const struct tagYDDamageVar&	 in_DamageVar,
								const struct tagYDDamageVar&	 in_AdditionalDamageVar,
								const struct tagYDResistanceVar& in_ResistanceVar,
								struct tagYDDamageVar&			 out_ApplyDamageVar,
								int	in_AttackerLevel);

		static void			YDApplyRatioAtDamage(
								const message_t*			 in_pMessage,
								const struct tagYDCharactor& in_AttackCharactor,
								const struct tagYDCharactor& in_DefenceCharactor,
								const struct tagYDDamageVar& in_ApplyDamageVar,
								struct tagYDDamageVar&       out_ApplyRatioDamageVar);

	public:
		static int YDGetDamage( int TargetEntityIdx, int TargetEntityType, message_t* message, int eachDmg[]);
		BOOL isOK(void) const;


	public:
		YDCDamageCal();
		virtual ~YDCDamageCal();

};

#endif 
