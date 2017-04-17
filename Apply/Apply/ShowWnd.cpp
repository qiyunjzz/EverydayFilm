#include "StdAfx.h"
#include "ShowWnd.h"
#include "Apply.h"

CShowWnd::CShowWnd(void)
{
	m_hIcon=NULL;		
	m_fontTitle.CreateFont(13,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,ANSI_CHARSET,3,2,1,2,_T("宋体"));
}

CShowWnd::~CShowWnd(void)
{
	m_fontTitle.DeleteObject();
}

void CShowWnd::CreatePlayWnd()
{
	  CreateEx(0, 
	  AfxRegisterWndClass( 
	  NULL,LoadCursor (NULL,IDC_ARROW),(HBRUSH) GetStockObject (WHITE_BRUSH),0), 
	  theApp.m_sCommandLine.m_strName, 
	  WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX,
	  0, 0, theApp.m_sCommandLine.m_szSize.cx, theApp.m_sCommandLine.m_szSize.cy,  //自定义窗口大小
	  NULL, 
	  NULL, 
	  NULL);
	   this->CenterWindow();

	  DWORD   dwStyle   = ::GetWindowLong(*this,GWL_STYLE);   
	  dwStyle   &=   ~(WS_SIZEBOX);   

	 ::SetWindowLong(*this, GWL_STYLE, dwStyle);

	 if(!theApp.m_strIcon.IsEmpty())
	     m_hIcon=(HICON)::LoadImage(0,theApp.m_strIcon,IMAGE_ICON,   32,  32, LR_LOADFROMFILE );
	 else
		m_hIcon=LoadIcon(theApp.m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
}

void CShowWnd::OnInitCommand()
{
	::SetProp(this->GetSafeHwnd(),theApp.m_sCommandLine.m_strClassHandle,AfxGetApp()->m_hInstance);
	OnCreateControl();
	OnInitContro();
}

void CShowWnd::OnCreateControl()
{
	CRect rect;
	this->GetClientRect(&rect); 

	m_btnClose.SetToolTip(_T("关闭"));
	m_btnClose.Create(_T(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,BTN_CLOSE);

	m_btnMin.SetToolTip(_T("最小化"));
	m_btnMin.Create(_T(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,BTN_MIN);

	m_btnTopMost.SetToolTip(_T("置顶"));
	m_btnTopMost.Create(_T(""),WS_CHILD,CRect(0,0,0,0),this,BTN_TOPMOST);	

	m_btnNoTopMost.SetToolTip(_T("不置顶"));
	m_btnNoTopMost.Create(_T(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,BTN_NOTOPMOST);

	m_btnFeedBack.SetToolTip(_T("反馈"));
	m_btnFeedBack.Create(_T(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,BTN_FEEDBACK);	

	m_wndIe.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,CRect(2,30,rect.Width()-4,rect.Height()-60),this,WND_IE_VIEW);

	MoveControl();
}

void CShowWnd::OnInitContro()
{
	m_btnClose.SetNormalImage(_T("file='close.png' source='0,0,43,28'"));
	m_btnClose.SetHotImage(_T("file='close.png' source='43,0,86,28'"));
	m_btnClose.SetPushedImage(_T("file='close.png' source='86,0,129,28'"));
	m_btnClose.SetShowText(FALSE);

	m_btnMin.SetNormalImage(_T("file='min_normal.png' source='0,0,27,28'"));
	m_btnMin.SetHotImage(_T("file='min_normal.png' source='27,0,54,28'"));
	m_btnMin.SetPushedImage(_T("file='min_normal.png' source='54,0,81,28'"));
	m_btnMin.SetShowText(FALSE);

	m_btnTopMost.SetNormalImage(_T("file='top_pressed.png' source='0,0,27,27'"));
	m_btnTopMost.SetHotImage(_T("file='top_pressed.png' source='27,0,54,27'"));
	m_btnTopMost.SetPushedImage(_T("file='top_pressed.png' source='54,0,81,27'"));
	m_btnTopMost.SetShowText(FALSE);

	m_btnNoTopMost.SetNormalImage(_T("file='top_released.png' source='0,0,27,27'"));
	m_btnNoTopMost.SetHotImage(_T("file='top_released.png' source='27,0,54,27'"));
	m_btnNoTopMost.SetPushedImage(_T("file='top_released.png' source='54,0,81,27'"));
	m_btnNoTopMost.SetShowText(FALSE);

	//m_btnFeedBack.SetNormalImage(_T("file='icon_top_released.png' source='0,0,23,23'"));
	m_btnFeedBack.SetHotImage(_T("file='bt_mid.png' source='0,0,62,22'"));
	m_btnFeedBack.SetPushedImage(_T("file='bt_mid.png' source='62,0,124,22'"));
	m_btnFeedBack.SetTextColor(RGB(0,0,0));
	m_btnFeedBack.SetWindowText(_T("反馈"));
	
}

void CShowWnd::PaintFrame(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	m_renderEngine.DrawImageString(pDC->m_hDC,CRect(0,0,rect.Width(),rect.Height()),CRect(0,0,rect.Width(),rect.Height()),_T("file='bg_no_layed.png' source='3,3,179,179'"));

	if(m_hIcon!=NULL)  m_renderEngine.DrawIcon(pDC,&m_hIcon,CRect(9,5,0,0));

	m_renderEngine.DrawWindowText(pDC,theApp.m_sCommandLine.m_strName,CRect(30,10,rect.Width()/2,25),RGB(255,255,255),&m_fontTitle); //显示程序名


	m_renderEngine.DrawFillRect(pDC,CRect(2,rect.Height()-30,rect.Width()-2,rect.Height()-5),RGB(245,245,245));
	m_renderEngine.DrawLine(pDC,CPoint(2,rect.Height()-30),CPoint(rect.Width()-2,rect.Height()-30),RGB(198,198,198),1);
	m_renderEngine.DrawWindowText(pDC,theApp.m_sCommandLine.m_strEs,CRect(10,rect.Height()-25,rect.Width()-70,rect.Height()-5),RGB(128,128,128)); //显示附加信息


	m_renderEngine.DrawBorder(pDC,rect,2,RGB(31,31,31));
	m_renderEngine.DrawBorder2(pDC,rect,1,RGB(65,66,67));
}


void CShowWnd::MoveControl()
{
	if(m_bMoveFirst) return;

	CRect rect;
	this->GetClientRect(&rect);  

	m_btnClose.MoveButton(rect.Width()-44,1,43,28,FALSE);
	m_btnMin.MoveButton(rect.Width()-(44+27),1,27,28,FALSE);
	m_btnNoTopMost.MoveButton(rect.Width()-(44+27+35),1,27,27,FALSE);
	m_btnTopMost.MoveButton(rect.Width()-(44+27+35),1,27,27,FALSE);

	m_wndIe.MoveWindow(2,30,rect.Width()-4,rect.Height()-60);
	m_wndIe.Invalidate(TRUE);
	m_btnFeedBack.MoveButton(rect.Width()-70,rect.Height()-28,62,22);
}




///////////////////////////////////////////////////////////////////////////////////////////////////
////////

BEGIN_MESSAGE_MAP(CShowWnd, CBaseWnd)
	ON_BN_CLICKED(BTN_FEEDBACK,&CShowWnd::OnBnClickedFeedBack)
	ON_BN_CLICKED(BTN_TOPMOST,&CShowWnd::OnBnClickedTopMost)
	ON_BN_CLICKED(BTN_NOTOPMOST,&CShowWnd::OnBnClickedNoTopMost)
	ON_BN_CLICKED(BTN_MIN,&CShowWnd::OnBnClickedMin)
	ON_BN_CLICKED(BTN_CLOSE,&CShowWnd::OnBnClickedClose)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


//////////////////////
////hangle message
void CShowWnd::OnBnClickedClose()
{	PostMessage(WM_CLOSE);}

void CShowWnd::OnBnClickedMin()
{	
	SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
}

void CShowWnd::OnBnClickedNoTopMost()
{
	if(m_btnNoTopMost.IsWindowVisible())	m_btnNoTopMost.ShowWindow(FALSE);
	if(!m_btnTopMost.IsWindowVisible()) m_btnTopMost.ShowWindow(TRUE);

	::SetWindowPos(*this, HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}

void CShowWnd::OnBnClickedTopMost()
{
	if(m_btnTopMost.IsWindowVisible())	m_btnTopMost.ShowWindow(FALSE);
	if(!m_btnNoTopMost.IsWindowVisible()) m_btnNoTopMost.ShowWindow(TRUE);

	::SetWindowPos(*this, HWND_NOTOPMOST,0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
}

void CShowWnd::OnBnClickedFeedBack()
{
	ShellExecute(NULL,_T("open"), _T("http://bbs.ttkvod.com/forum.php?mod=forumdisplay&fid=54"), NULL, NULL, SW_SHOWNORMAL);
}

void CShowWnd::OnDestroy()
{
	CBaseWnd::OnDestroy();
}

void CShowWnd::OnClose()
{
	CBaseWnd::OnClose();
}
