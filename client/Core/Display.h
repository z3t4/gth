#ifndef _DISPLAY_H
#define _DISPLAY_H

class CDisplay
{
    
    struct sDisplayParams
	{
        float   Contrast;
        float   Gamma;
        float   Brightness;

        sDisplayParams()
		{
			Contrast   = 1.0f;
			Gamma      = 1.3f;
			Brightness = 0.6f;
		}
    };

	sDisplayParams m_DisplayParams;
public:
	 CDisplay();
	~CDisplay();

	
	int		m_gammaRampType;

    
    void  SetupGamma();
    float GetBrightness()  const            { return m_DisplayParams.Brightness; }
    float GetGamma()       const            { return m_DisplayParams.Gamma; }
    float GetContrast()    const            { return m_DisplayParams.Contrast; }
    void  SetBrightness(float Brightness)   { m_DisplayParams.Brightness = Brightness; }
    void  SetGamma(float Gamma)             { m_DisplayParams.Gamma = Gamma; }
    void  SetContrast(float Contrast)       { m_DisplayParams.Contrast = Contrast; }
};

extern CDisplay g_display;


#endif