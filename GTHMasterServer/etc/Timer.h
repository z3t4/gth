class CTimer
{
	int  m_stopped;
	int  m_inited;
	int  m_usingQPF;

	double  m_lastElapsedTime;
	double  m_baseTime;
	double  m_stopTime;
	double  m_currSysTime;
	double  m_currElapsedTime;
	double  m_baseMilliTime;
	double  m_currSysMilliTime;
	double  m_currElapsedMilliTime;

	LONGLONG  m_QPFTicksPerSec;
	LONGLONG  m_QPFStopTime;
	LONGLONG  m_QPFLastElapsedTime;
	LONGLONG  m_QPFBaseTime;

	LARGE_INTEGER m_QPFTime;

public:

	CTimer();
	virtual ~CTimer();

	void  Start();
	void  Stop();
	void  Advance();
	void  Reset();
	float Tick();
	inline float GetAppTime()     { return (float) ( m_currSysTime - m_baseTime ); }
	inline float GetElapsedTime() { return (float) m_currElapsedTime; }
	inline float GetSysTime()     { return (float) m_currSysTime; }
	inline float GetAppMilliTime()     { return (float) ( m_currSysMilliTime - m_baseMilliTime ); }
	inline float GetElapsedMilliTime() { return (float) m_currElapsedMilliTime; }
};
