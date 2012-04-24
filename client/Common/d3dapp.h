




#ifndef D3DAPP_H
#define D3DAPP_H



#include <basetsd.h> 





enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESIZEFAILED        0x8200000c
#define D3DAPPERR_NULLREFDEVICE       0x8200000d

#define MAX_CANDLIST 10
#define MAX_COMPSTRING_SIZE 256






struct D3DModeInfo
{
    DWORD      Width;      
    DWORD      Height;     
    D3DFORMAT  Format;     
    DWORD      dwBehavior; 
    D3DFORMAT  DepthStencilFormat; 
};









struct D3DDeviceInfo
{
    
    D3DDEVTYPE   DeviceType;      
    D3DCAPS8     d3dCaps;         
    const TCHAR* strDesc;         
    BOOL         bCanDoWindowed;  

    
    DWORD        dwNumModes;
    D3DModeInfo  modes[150];

    
    DWORD        dwCurrentMode;
    BOOL         bWindowed;
    D3DMULTISAMPLE_TYPE MultiSampleTypeWindowed;
    D3DMULTISAMPLE_TYPE MultiSampleTypeFullscreen;
};









struct D3DAdapterInfo
{
    
    D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
    D3DDISPLAYMODE d3ddmDesktop;      

    
    DWORD          dwNumDevices;
    D3DDeviceInfo  devices[5];

    
    DWORD          dwCurrentDevice;
};


















class CD3DApplication
{
protected:
    
    D3DAdapterInfo    m_Adapters[10];
    DWORD             m_dwNumAdapters;
    DWORD             m_dwAdapter;
    BOOL              m_bWindowed;
    BOOL              m_bActive;
    BOOL              m_bReady;
    BOOL              m_bHasFocus;

    
    BOOL              m_bFrameMoving;
    BOOL              m_bSingleStep;

    
    HRESULT DisplayErrorMsg( HRESULT hr, DWORD dwType );

    
    HRESULT BuildDeviceList();
    BOOL    FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
                D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat );
    HRESULT Initialize3DEnvironment();
    HRESULT Resize3DEnvironment();
    HRESULT ToggleFullscreen();
    HRESULT ForceWindowed();
    HRESULT UserSelectNewDevice();
    VOID    Cleanup3DEnvironment();
    HRESULT Render3DEnvironment();
    virtual HRESULT AdjustWindowForChange();
    static INT_PTR CALLBACK SelectDeviceProc( HWND hDlg, UINT msg, 
                WPARAM wParam, LPARAM lParam );

protected:
    
    D3DPRESENT_PARAMETERS m_d3dpp;         

    HWND              m_hWndFocus;         
    LPDIRECT3D8       m_pD3D;              
    LPDIRECT3DDEVICE8 m_pd3dDevice;        
    D3DCAPS8          m_d3dCaps;           
    D3DSURFACE_DESC   m_d3dsdBackBuffer;   
    DWORD             m_dwCreateFlags;     
    DWORD             m_dwWindowStyle;     
    RECT              m_rcWindowBounds;    
    RECT              m_rcWindowClient;    

    
    FLOAT             m_fTime;             
    FLOAT             m_fElapsedTime;      
    FLOAT             m_fFPS;              
    TCHAR             m_strDeviceStats[90];
    TCHAR             m_strFrameStats[90]; 

    
    TCHAR*            m_strWindowTitle;    
    BOOL              m_bUseDepthBuffer;   
    DWORD             m_dwMinDepthBits;    
    DWORD             m_dwMinStencilBits;  
    DWORD             m_dwCreationWidth;   
    DWORD             m_dwCreationHeight;  
    BOOL              m_bShowCursorWhenFullscreen; 
    BOOL              m_bClipCursorWhenFullscreen; 

    
    virtual HRESULT ConfirmDevice(D3DCAPS8*,DWORD,D3DFORMAT)   { return S_OK; }
    virtual HRESULT OneTimeSceneInit()                         { return S_OK; }
    virtual HRESULT InitDeviceObjects()                        { return S_OK; }
    virtual HRESULT RestoreDeviceObjects()                     { return S_OK; }
    virtual HRESULT FrameMove()                                { return S_OK; }
    virtual HRESULT Render()                                   { return S_OK; }
    virtual HRESULT InvalidateDeviceObjects()                  { return S_OK; }
    virtual HRESULT DeleteDeviceObjects()                      { return S_OK; }
    virtual HRESULT FinalCleanup()                             { return S_OK; }

public:
    HWND              m_hWnd;           
	BOOL			  m_bAppExit;
	BOOL			  m_bChangeDisplay;

	bool              m_bFullScreen;

	
	D3DGAMMARAMP	  m_ramp;

    
    virtual HRESULT Create( HINSTANCE hInstance );
    virtual INT     Run();
    virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual VOID    Pause( BOOL bPause );

	void			SetWindowMode( BOOL bWindow )	{ m_bWindowed = bWindow; }
	BOOL			GetWindowMode()					{ return( m_bWindowed ); }

    
    CD3DApplication();

};




#endif



