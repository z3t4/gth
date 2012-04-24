



#if !defined(AFX_IFGIFTCARDWIN_H__862C4AF4_1574_4C97_93B7_8D665396EFAD__INCLUDED_)
#define AFX_IFGIFTCARDWIN_H__862C4AF4_1574_4C97_93B7_8D665396EFAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CIFGiftCardWin  : public CIFWindow
{
	enum 
	{
		IT_GIFTCARD_WIN_TITLE,
		IT_GIFTCARD_WIN_ID,	
		IT_GIFTCARD_WIN_ID_INPUT,
		IT_GIFTCARD_WIN_PASSWORD,	
		IT_GIFTCARD_WIN_PASSWORD_INPUT,
		NUM_IF_TEXT,
	};

	enum
	{
		MAX_INPUT_STRLEN = 20,
	};
	
public:
	
	CIFControl			*m_GiftCardTitleBar;
	CIFControl			*m_GiftCardTitleBottom;
	CIFControl			*m_GiftCardBar;
	CIFControl			*m_GiftCardLeftBar;
	CIFControl			*m_GiftCardRightBar;
	CIFControl			*m_GiftCardIDBar;	
	CIFControl			*m_GiftCardPasswordBar;
	CIFButtonCtl		*m_GiftCardOkBtn;
	CIFButtonCtl		*m_GiftCardCancelBtn;


	CIFText				*m_textCtl[NUM_IF_TEXT];

public:
	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

public:
	void InitGiftCardWin();

public:
	CIFGiftCardWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFGiftCardWin();

};

#endif 
