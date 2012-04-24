#include "../global_def.h"

inline void	SetSample( char	*name, int maxBuffers, int idxSample, int is3D )
{
	CSoundSample	*sample;
	sample	=	&g_musicMng ->m_samples [idxSample];

	strcpy( sample->m_name, name ); 
	sample->m_is3D = is3D;
	sample->m_idxSound = -1;

	sample->m_sample = NULL;

	g_musicMng->m_numSamples++;
}

void ConfigGameSamples()
{
	int i, j, soundID = 0;
	int is3D;

	
	for( i = 0; i < g_soundMng->m_soundResource.numNoramlID; i ++ )
	{
		if( g_soundMng->m_soundResource.normalID[i].is3D ) is3D = true;
		else is3D = false;

		SetSample( g_soundMng->m_soundResource.normalID[i].filename, g_soundMng->m_soundResource.normalID[i].numDup, 
			soundID, is3D );

		g_soundMng->m_soundResource.normalID[i].soundID = soundID;

		soundID ++;
	}
	
	for( i = 0; i < g_soundMng->m_soundResource.numItemID; i ++ )
	{
		if( g_soundMng->m_soundResource.itemID[i].is3D ) is3D = true;
		else is3D = false;

		SetSample( g_soundMng->m_soundResource.itemID[i].filename, g_soundMng->m_soundResource.itemID[i].numDup, 
			soundID, is3D );

		g_soundMng->m_soundResource.itemID[i].soundID = soundID;

		soundID ++;
	}
	
	for( i = 0; i < MAX_ANIMCOUNT_TYPE; i ++ )
	{
		for( j = 0; j < g_soundMng->m_soundResource.numEventID[i]; j ++ )
		{
			if( g_soundMng->m_soundResource.eventID[i][j].is3D ) is3D = true;
			else is3D = false;
			
			SetSample( g_soundMng->m_soundResource.eventID[i][j].filename, g_soundMng->m_soundResource.eventID[i][j].numDup, 
				soundID, is3D );

			g_soundMng->m_soundResource.eventID[i][j].soundID = soundID;
	
			soundID ++;
		}
	}
}

void ConfigMDLSamples()
{
	int		i;

	for( i = 0;	i < g_soundMng->m_nummdlSoundID; i ++ )
	{
		SetSample ( g_soundMng->m_mdlSoundID[i].soundname, 10, g_soundMng->m_mdlSoundID[i].idx , true );
	}
}

int GetGameSampleID( char *path )
{
	return -1;
}
