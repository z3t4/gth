



#if !defined(AFX_IFWINDOW_H__538BDDDB_0BFB_4637_B702_7CCE1A051682__INCLUDED_)
#define AFX_IFWINDOW_H__538BDDDB_0BFB_4637_B702_7CCE1A051682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CIFWindow  : public CIFControl
{
public:

	CIFWindow( CIFControlNode *pParentControl = NULL );
	virtual ~CIFWindow();

	void		BeginMoving()								{ m_movingFlag = true;		}
	void		EndMoving()									{ m_movingFlag = false;		}
	void		UpdateMoving( int dx, int dy );

	void		ToggleMinimize()							{ m_minimize = !m_minimize;	}
	void		Minimize( int minimize = true )				{ m_minimize = minimize;	}
	int			MinimizeState()								{ return( m_minimize );		}
	
	void		SetWindowType( int type )					{ m_windowType = type;		}
	int			GetWindowType()								{ return( m_windowType );	}

	virtual int		WindowFrame()							{ return 0; }
	virtual int		WindowProcess()							{ return 0; }
	virtual int		WindowSelection()						{ return 0; }
	virtual void	Render()								{ return;	}

	
	
	bool        BeforeState()                               { return m_bBeforeState; }
	void        SetBeforeState( bool state )                { m_bBeforeState = state; }  

private:

	int						m_windowType;
	int						m_movingFlag;
	int						m_minimize;
	
	bool					m_bBeforeState;			
};

#endif 
