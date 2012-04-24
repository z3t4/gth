    #include "../global_def.h"

 
 
 
 
 
 
bool	CMDLMng::RenderEffectPreCheck	(	mdleffectentity_t	**in_entity,
											Fx_CHARACTER_t		**out_attacker,
											Fx_CHARACTER_t		**out_defender		)
{
	if	(	! (*in_entity)->visible	)
	{
		(*in_entity)	=	(*in_entity)->next;
		return	false;
	}


	if	(	(*in_entity)->attackerIdx	>=	0	)
	{
		(*out_attacker)	=	g_charManager->FindCharacterByServerIndex( (*in_entity)->attackerIdx, (*in_entity)->attackerType );
		if	(	! (*out_attacker)	)
		{
			(*in_entity)	=	DeleteEffectManagerEntity( (*in_entity) );
			return	false;
		}
	}
	else
	{
		(*out_attacker)	=	NULL;
	}

	if	(	(*in_entity)->defenderIdx	>=	0	)
	{
		(*out_defender)	=	g_charManager->FindCharacterByServerIndex( (*in_entity)->defenderIdx, (*in_entity)->defenderType );
		if	(	! (*out_defender)	)
		{
			(*in_entity)	=	DeleteEffectManagerEntity( (*in_entity) );
			return	false;
		}
	}
	else
	{
		(*out_defender)	=	NULL;
	}

	return	true;
}

 
 
 
 
 
 
float	CMDLMng::RenderEffectEvaluateFunction	(	mdleffectclassstate_t	*in_classState	)
{
	m_waveFunc.amplitude = in_classState->bobHeight;
	m_waveFunc.frequency = in_classState->bobSpeed;

	return	m_shaderMng->EvaluateFunc( &m_waveFunc, m_currTime );
}	

 
 
 
 
 
 
bool	CMDLMng::RenderEffectCheckRenderCondition	(	mdleffectclasspart_t	*in_classPart,
														Fx_CHARACTER_t	*in_attacker,
														Fx_CHARACTER_t	*in_defender,
														int		in_followAttacker,
														int		in_state,
														int		in_effectLevel,
														float	in_startTime,
														float	&in_partStartTime	)
{
	if	(	in_classPart->onlyTag	)
		return	false;

		 
	if	(	in_followAttacker	)
	{
		if	(	(	in_attacker	==	NULL	)	||
				(	in_attacker ->flag_culled	)	)
			return	false;
	}
	else if	(	(	in_state	==	GTH_EFFECT_STATE_HITTED	)	||
				(	in_state	==	GTH_EFFECT_STATE_MISSED	)	)
	{
		if	(	(	in_defender	==	NULL	)	||
				(	in_defender ->flag_culled	)	)
			return	false;
	}

		 
	if	(	in_classPart ->req_level	<	0	)
	{
		if	(	in_effectLevel	!=	( -in_classPart ->req_level - 1 )		)
			return	false;
	}
	else
	{
		if	(	in_effectLevel	<	in_classPart ->req_level	)
			return	false;
	}
		 
	in_partStartTime	=	( in_startTime + in_classPart ->startTime );
	if	(	( m_currMilliTime - in_partStartTime )	<	0	)
		return	false;

		 
	if	(	(	in_classPart ->endTime	>	0.0f	)	&&
		 

			(	in_followAttacker	<=	0	)			&&
			(	in_state	!=	GTH_EFFECT_STATE_FIRED	)	)
	{
		if	(	m_currMilliTime	>	( in_startTime + in_classPart ->endTime )	)
			return	false;
	}

	return	true;
}

 
 
 
 
 
 
void	CMDLMng::RenderEffectPlaySound	(	bool	in_soundflag[][EFFECT_ENTITY_MAX_PARTS],
											vec3_t	in_origin,
											int		in_state,
											CMDL	*in_mdl,
											int		in_subindex	)
{
	if	(	(	in_mdl->m_soundIdx	>	-1	)	&&
			(	! in_soundflag[ in_state ][ in_subindex ]	)	)
	{
		g_musicMng ->PlaySample3D ( in_mdl ->m_soundIdx, GTH_SOUND_PLAY_ONCE, in_origin );
		in_soundflag[ in_state ][ in_subindex ]	=	true;
	}
}

 
 
 
 
 
 
void	CMDLMng::RenderEffectCalcCurrentFrame	(	CMDL	*in_mdl,
													mdleffectclasspart_t	*in_classPart,
													float	in_startTime,
													float	in_partStartTime	)
{
	if	(	in_mdl->m_numBoneFrames	>	1	)
	{
		m_isInterpolated	=	true;
		if	(	in_classPart->endTime	<	0	)
		{
			 
			m_interPol	=	( m_currMilliTime - in_partStartTime ) / in_classPart ->msecPerFrame;
		}
		else
		{
			 
			m_interPol	=	( m_currMilliTime - in_partStartTime ) / ( ( in_classPart ->endTime + in_startTime ) - in_partStartTime ) * ( in_mdl ->m_numBoneFrames - 1 );
		}
		m_currFrame	=	( (int) m_interPol ) % ( in_mdl ->m_numBoneFrames - 1 );
		m_interPol	=	m_interPol - ( (int) m_interPol );
	}
	else
	{
		m_isInterpolated	= false;
		m_interPol			=	0.0f;
		m_currFrame			=	0;
	}
}

 
 
 
 
 
 
void	CMDLMng::RenderEffectPrepareBuffers	(	CMDL	*in_mdl	)
{
	int	index;
	if	(	m_isInterpolated	)
	{
		for	(	index	=	0;	\
				index	<	in_mdl->m_numMeshes;	\
				index++	)
			PrepareInterpolateMDL( in_mdl, &in_mdl->m_meshes[ index ] );

		m_D3DRender->SetStreamSource( 0, m_dynamicVB->GetD3DVertexBuffer(), m_dynamicVB->GetStrideSize() );
		m_D3DRender->SetIndices( m_dynamicIB->GetD3DIndexBuffer(), 0 );
		m_currBank	=	-1;
	}
	else
	{
		if	(	in_mdl->m_needDynVB		)
		{
			for	(	index	=	0;	\
					index	<	in_mdl->m_numMeshes;	\
					index++		)
				PrepareMDL( in_mdl,	&in_mdl->m_meshes[ index ] );

			m_D3DRender->SetStreamSource ( 0 , m_dynamicVB ->GetD3DVertexBuffer () , m_dynamicVB ->GetStrideSize () );
			m_D3DRender->SetIndices ( m_dynamicIB ->GetD3DIndexBuffer () , 0 );
			m_currBank	=	-1;
		}
		else
		{
			if	(	m_currBank	!=	in_mdl ->m_bank	)
			{
				m_D3DRender ->SetStreamSource ( 0 , m_staticVB [in_mdl ->m_bank] ->GetD3DVertexBuffer () , m_staticVB [in_mdl ->m_bank] ->GetStrideSize () );
				
				m_currBank	=	in_mdl ->m_bank;
			}
			m_D3DRender ->SetIndices ( m_staticEB ->GetD3DIndexBuffer () , 0 );
		}
	}
}

 
 
 
 
 
 
bool	CMDLMng::RenderEffectSetTransform	(	mdleffectclasspart_t	*in_classPart,
												int		in_followAttacker,
												vec3_t	in_offset,
												int		in_state,
												vec3_t	in_origin,
												vec3_t	in_angles,
												mdleffectclassstate_t	*in_classState,
												Fx_CHARACTER_t	*in_attacker,
												Fx_CHARACTER_t	*in_defender,
												vec3_t	out_position,
												float	in_bob	)
{
	vec3_t	scale,	angles;
	matrix4x4_t	selfRot;
	matrix4x4_t	tmp;

	m_transform.Identity ();

	selfRot.Identity ();
	VectorSet ( angles , 0.0f , 0.0f , 0.0f );
	if	(	in_classPart ->selfRotSpeed	==	0.0f	)
		angles [YAW]	=	0.0f;
	else
		angles [YAW]	=	in_classPart ->selfRotSpeed * m_currTime * 180.0f;
	if	(	in_classPart ->billboard	)
	{
		angles [YAW]	=	angles [YAW] + g_camera .m_angles [YAW] + 90.0f;
		angles [PITCH]	=	angles [PITCH] + g_camera .m_angles [PITCH];
	}

	selfRot.RotateXZ ( angles );
	
		 
	if	(	in_followAttacker	)
	{
		vec3_t	trans;
		if	(	in_attacker	==	NULL	)
			return	false;
		if	(	in_classState ->setground		)
		{
			trans [0]	=	in_attacker ->position [0];
			trans [1]	=	in_attacker ->position [1];
			trans [2]	=	in_attacker ->position [2] - in_attacker ->stat_height;
		}
		else
			VectorCopy ( trans , in_attacker ->position );
		VectorAdd ( trans , trans , in_offset );
		m_transform.Translate ( trans );
	}
	else
	{
		if	(	in_classState ->setground		)
		{
			if	(	(	in_state	==	GTH_EFFECT_STATE_HITTED	)	||
					(	in_state	==	GTH_EFFECT_STATE_MISSED	)	)
			{
				VectorSet ( out_position , in_origin [0] , in_origin [1] , in_origin [2] - in_defender ->stat_height );
			}
			else if	(	in_attacker	)
				VectorSet ( out_position , in_origin [0] , in_origin [1] , in_origin [2] - in_attacker ->stat_height );
			else
				VectorSet ( out_position , in_origin [0] , in_origin [1] , in_origin [2] );
		}
		else
		{
			VectorSet ( out_position , in_origin [0] , in_origin [1] , in_origin [2] + in_bob );
		}
		m_transform.Translate ( out_position );
	}

	if	(	! in_classPart ->billboard	)
	{
		VectorCopy ( angles , in_angles );

		if	(	in_classPart ->rotSpeed	==	0.0f	)
			angles [YAW]	=	in_angles[ YAW ];
		else
			angles [YAW]	=	in_classPart ->rotSpeed * m_currTime * 180.0f;

		VectorAdd ( angles , angles , in_classPart ->angleOffset );

		if	(	in_state	==	GTH_EFFECT_STATE_FIRED	)
		{
			m_transform.RotateXZ ( angles );
		}
		else
		{
			if	(	in_followAttacker	)
			{
				if	(	in_attacker	!=	NULL	)
				{
					m_transform.RotateZ ( in_attacker ->angles [YAW] + 90.0f );
				}
				else
				{
					return	false;
				}
			}
			else
			{
				m_transform.RotateZ ( angles [YAW] );
			}
		}
	}

	m_transform.Translate ( in_classPart ->offset );
	m_transform	*=	selfRot;

	VectorSet (	scale,	in_classPart ->scale,	in_classPart ->scale,	in_classPart ->scale );
	tmp.Scale ( scale );
	m_transform	*=	tmp;

	m_D3DRender ->SetTransform ( D3DTS_WORLD , &m_transform );

	return	true;
}

 
 
 
 
 
 
void	CMDLMng::RenderEffectDoRender	(	CMDL	*in_mdl,
											vec3_t	in_position	)
{
	mdlmesh_t	*mesh;
	shader_t	*shader;
	int	index;

	mesh	=	&in_mdl ->m_meshes [0];
	for	(	index	=	0;	\
			index	<	in_mdl ->m_numMeshes;	\
			index++,	mesh++	)
	{
		if	(	in_mdl ->m_shaderRefs [mesh ->skinInfo].isShader	)
			m_shaderMng ->PrepareShader ( in_mdl ->m_shaderRefs [mesh ->skinInfo].shader,	m_frameFlag );
	}

	mesh	=	&in_mdl ->m_meshes [0];
	for	(	index	=	0;	\
			index	<	in_mdl ->m_numMeshes;	\
			index++ , mesh++	)
	{
		if	(	in_mdl ->m_shaderRefs [mesh ->skinInfo].isShader	)
		{
			shader	=	&m_shaderMng ->m_shaders [in_mdl ->m_shaderRefs [mesh ->skinInfo].shader];
			if	(	shader ->flags	&	SHADER_TCGEN	)
				m_shaderMng ->PrepareTCGen ( shader , in_position );
			m_D3DRender ->RenderMDLCustom ( shader , mesh ->offsetVB , mesh ->numVertices , mesh ->offsetEB , mesh ->numElems );
		}
		else
		{
			m_D3DRender ->SetTexture ( 0 , m_texMng ->GetD3DTexture ( in_mdl ->m_shaderRefs [mesh ->skinInfo].shader ) );
			m_D3DRender ->RenderDefault ( mesh ->offsetVB , mesh ->numVertices , mesh ->offsetEB , mesh ->numElems );
		}

		m_totalRenderPolygon	+=	in_mdl ->m_meshes [index].numElems / 3;
	}
}

 
 
 
 
void	CMDLMng::RenderEffectEntity_Neo	()
{
	int		index, subindex;
	mdleffectentitylink_t	*linked;
	mdleffectclass_t		*effectClass;
	mdleffectclassstate_t	*classState;
	mdleffectclasspart_t	*classPart;
	mdleffectentity_t		*effectEntity;
	CMDL		*mdl;
	float	bob;
	vec3_t	position;

	int			*linkedCount;

	Fx_CHARACTER_t		*attacker;
	Fx_CHARACTER_t		*defender;
	float		partStartTime;
	
	linked		=	&m_linkedMDLEffectEntities[ 0 ];
	effectClass	=	&m_MDLEffectClasses[ 0 ];
	linkedCount	=	&m_linkedVisibleCount.effectClasses[ 0 ];

	for	(	index	=	0;	\
			index	<	m_numMDLEffectClasses;	\
			index++,	linked++,	linkedCount++,	effectClass++	)
	{
		if	(	! linked ->linked	)		continue;
		if	(	*linkedCount	<	1	)
		{
			continue;
		}

		effectEntity	=	linked ->linked;

		while	(	effectEntity	)
		{
			 

			if	(	! RenderEffectPreCheck( &effectEntity, &attacker, &defender )	)
			{
				continue;
			}

			classState		=	&effectClass->states[ effectEntity->state ];
			bob	=	RenderEffectEvaluateFunction( classState );

			classPart	=	&classState ->parts [0];
			for	(	subindex	=	0;	\
					subindex	<	classState ->numParts;	\
					subindex++,	classPart++	)
			{
				if	(	! RenderEffectCheckRenderCondition( classPart, attacker, defender, effectEntity->follow_attacker, effectEntity->state, effectEntity->effect_level, effectEntity->startTime, partStartTime )	)
				{
					continue;
				}

				
				mdl = GetMDL( classPart->mdlIdx );
				if( mdl == NULL ) continue;



				RenderEffectPlaySound( effectEntity->soundflag, effectEntity->origin, effectEntity->state, mdl, subindex );
				RenderEffectCalcCurrentFrame( mdl, classPart, effectEntity->startTime, partStartTime );
				RenderEffectPrepareBuffers( mdl );
				if	(	! RenderEffectSetTransform( classPart, effectEntity->follow_attacker, effectEntity->offset, effectEntity->state, effectEntity->origin, effectEntity->angles, classState, attacker, defender, position, bob )	)
				{
					continue;
				}

				RenderEffectDoRender( mdl, position );
			}

			 
			 
			if	(	(	effectClass->states[ GTH_EFFECT_STATE_FIRED ].startTime	<	0	)	&&
					(	effectEntity->state	==	GTH_EFFECT_STATE_FIRED	)	)
			{
				classState		=	&effectClass->states[ GTH_EFFECT_STATE_ARMED ];
				bob	=	RenderEffectEvaluateFunction( classState );

				classPart	=	&classState ->parts [0];
				for	(	subindex	=	0;	\
						subindex	<	classState ->numParts;	\
						subindex++,	classPart++	)
				{
					if	(	! RenderEffectCheckRenderCondition( classPart, attacker, defender, effectEntity->follow_attacker, effectEntity->state, effectEntity->effect_level, effectEntity->backup_startTime, partStartTime )	)
						continue;

					
					mdl	=	GetMDL( classPart->mdlIdx );
					if( mdl == NULL ) continue;

	

					RenderEffectPlaySound( effectEntity->soundflag, effectEntity->backup_origin, effectEntity->state, mdl, subindex );
					RenderEffectCalcCurrentFrame( mdl, classPart, effectEntity->backup_startTime, partStartTime );
					RenderEffectPrepareBuffers( mdl );
					if	(	! RenderEffectSetTransform( classPart, effectEntity->follow_attacker, effectEntity->offset, effectEntity->state, effectEntity->backup_origin, effectEntity->backup_angles, classState, attacker, defender, position, bob )	)
						continue;

					RenderEffectDoRender( mdl, position );
				}
			}
			else if	(	(	effectClass->states[ GTH_EFFECT_STATE_FIRED ].numParts	<=	0	)	&&
						(	(	effectClass->states[ GTH_EFFECT_STATE_HITTED ].startTime	<	0	)	&&
							(	effectEntity->state	==	GTH_EFFECT_STATE_HITTED	)	)	)
			{
				classState		=	&effectClass->states[ GTH_EFFECT_STATE_ARMED ];
				bob	=	RenderEffectEvaluateFunction( classState );

				classPart	=	&classState ->parts [0];
				for	(	subindex	=	0;	\
						subindex	<	classState ->numParts;	\
						subindex++,	classPart++	)
				{
					if	(	! RenderEffectCheckRenderCondition( classPart, attacker, defender, effectEntity->follow_attacker, effectEntity->state, effectEntity->effect_level, effectEntity->backup_startTime, partStartTime )	)
						continue;

					
					mdl	=	GetMDL( classPart->mdlIdx );
					if( mdl == NULL ) continue;

	

					RenderEffectPlaySound( effectEntity->soundflag, effectEntity->backup_origin, effectEntity->state, mdl, subindex );
					RenderEffectCalcCurrentFrame( mdl, classPart, effectEntity->backup_startTime, partStartTime );
					RenderEffectPrepareBuffers( mdl );
					if	(	! RenderEffectSetTransform( classPart, effectEntity->follow_attacker, effectEntity->offset, effectEntity->state, effectEntity->backup_origin, effectEntity->backup_angles, classState, attacker, defender, position, bob )	)
						continue;

					RenderEffectDoRender( mdl, position );
				}
			}
			effectEntity	=	effectEntity->next;
		}
	}	
}