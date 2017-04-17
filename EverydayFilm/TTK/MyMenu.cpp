#include "StdAfx.h"
#include "MyMenu.h"

CMyMenu::CMyMenu(void)
{
	m_pm = new CPaintManagerUI;
	hParent = NULL;
}

CMyMenu::~CMyMenu(void)
{
}
void CMyMenu::Init()
{
	m_pMenuIE = static_cast<CVerticalLayoutUI*>(m_pm->FindControl(_T("right1")));
	m_pMenuMain = static_cast<CVerticalLayoutUI*>(m_pm->FindControl(_T("right2")));
	m_pMenuList = static_cast<CVerticalLayoutUI*>(m_pm->FindControl(_T("right3")));
	m_pTray = static_cast<CVerticalLayoutUI*>(m_pm->FindControl(_T("right4")));
	m_pMenuPlayer = static_cast<CVerticalLayoutUI*>(m_pm->FindControl(_T("right5")));
	quit = static_cast<CButtonUI*>(m_pm->FindControl(_T("quit")));
	quit2= static_cast<CButtonUI*>(m_pm->FindControl(_T("quit2")));
	go = static_cast<CButtonUI*>(m_pm->FindControl(_T("go")));
	set = static_cast<CButtonUI*>(m_pm->FindControl(_T("set")));
	set2 = static_cast<CButtonUI*>(m_pm->FindControl(_T("set2")));
	one = static_cast<CButtonUI*>(m_pm->FindControl(_T("one")));
	brk = static_cast<CButtonUI*>(m_pm->FindControl(_T("break")));
	back = static_cast<CButtonUI*>(m_pm->FindControl(_T("back")));
	open = static_cast<CButtonUI*>(m_pm->FindControl(_T("open")));
	sort = static_cast<CButtonUI*>(m_pm->FindControl(_T("sort")));
	play = static_cast<CButtonUI*>(m_pm->FindControl(_T("play")));
	copy = static_cast<CButtonUI*>(m_pm->FindControl(_T("copy")));
	rand = static_cast<CButtonUI*>(m_pm->FindControl(_T("rand")));
	del = static_cast<CButtonUI*>(m_pm->FindControl(_T("delete")));
	paste = static_cast<CButtonUI*>(m_pm->FindControl(_T("paste")));
	about = static_cast<CButtonUI*>(m_pm->FindControl(_T("about")));
	about2 = static_cast<CButtonUI*>(m_pm->FindControl(_T("about2")));
	reflash = static_cast<CButtonUI*>(m_pm->FindControl(_T("reflash")));	
	offsite = static_cast<CButtonUI*>(m_pm->FindControl(_T("offsite")));
	offsite2 = static_cast<CButtonUI*>(m_pm->FindControl(_T("offsite2")));
	offsite3 = static_cast<CButtonUI*>(m_pm->FindControl(_T("offsite3")));
	connect = static_cast<CButtonUI*>(m_pm->FindControl(_T("connect")));
	openurl = static_cast<CButtonUI*>(m_pm->FindControl(_T("openurl")));
	autopoweroff = static_cast<CButtonUI*>(m_pm->FindControl(_T("autopoweroff")));
	connall = static_cast<CButtonUI*>(m_pm->FindControl(_T("connall")));
	findfile = static_cast<CButtonUI*>(m_pm->FindControl(_T("findfile")));
	onecycle = static_cast<CButtonUI*>(m_pm->FindControl(_T("onecycle")));
	breakall = static_cast<CButtonUI*>(m_pm->FindControl(_T("breakall")));
	sortcycle = static_cast<CButtonUI*>(m_pm->FindControl(_T("sortcycle")));
	showmain = static_cast<CButtonUI*>(m_pm->FindControl(_T("showmain")));
	playpause = static_cast<CButtonUI*>(m_pm->FindControl(_T("playpause")));
	stop = static_cast<CButtonUI*>(m_pm->FindControl(_T("stop")));
	left = static_cast<CButtonUI*>(m_pm->FindControl(_T("left")));
	right = static_cast<CButtonUI*>(m_pm->FindControl(_T("right")));
	full = static_cast<CButtonUI*>(m_pm->FindControl(_T("full")));
	list = static_cast<CButtonUI*>(m_pm->FindControl(_T("list")));
	playup = static_cast<CButtonUI*>(m_pm->FindControl(_T("playup")));
	set3 = static_cast<CButtonUI*>(m_pm->FindControl(_T("set3")));
	ar = static_cast<CButtonUI*>(m_pm->FindControl(_T("ar")));
	noar = static_cast<CButtonUI*>(m_pm->FindControl(_T("noar")));
	ar43 = static_cast<CButtonUI*>(m_pm->FindControl(_T("4ar3")));
	ar169 = static_cast<CButtonUI*>(m_pm->FindControl(_T("16ar9")));
	picset = static_cast<CButtonUI*>(m_pm->FindControl(_T("picset")));
}
void CMyMenu::Notify(TNotifyUI& msg)
{
	if( msg.sType == MST_WINDOWINIT ) 
	{
		OnPrepare();
	}
	if( msg.sType == MST_CLICK )
	{
		//first of all
		ShowWindow(false,false);
		//then
		if(msg.pSender == go)::SendMessage(hParent,MSG_MENUS,MM_IE_FORWORD,0);
		else if(msg.pSender == del)::SendMessage(hParent,MSG_MENUS,MM_DELETE,0);
		else if(msg.pSender == set || msg.pSender == set2 || msg.pSender == set3)::SendMessage(hParent,MSG_MENUS,MM_SET,0);
		else if(msg.pSender == one)::SendMessage(hParent,MSG_MENUS,MM_ONE,0);
		else if(msg.pSender == brk)::SendMessage(hParent,MSG_MENUS,MM_BREAK,0);
		else if(msg.pSender == back)::SendMessage(hParent,MSG_MENUS,MM_IE_GOBACK,0);
		else if(msg.pSender == open)::SendMessage(hParent,MSG_MENUS,MM_OPENFILE,0);
		else if(msg.pSender == sort)::SendMessage(hParent,MSG_MENUS,MM_SORT,0);
		else if(msg.pSender == play)::SendMessage(hParent,MSG_MENUS,MM_PLAY,0);
		else if(msg.pSender == copy)::SendMessage(hParent,MSG_MENUS,MM_IE_COPY,0);
		else if(msg.pSender == paste)::SendMessage(hParent,MSG_MENUS,MM_IE_PASTE,0);
		else if(msg.pSender == rand)::SendMessage(hParent,MSG_MENUS,MM_RAND,0);
		else if(msg.pSender == reflash)::SendMessage(hParent,MSG_MENUS,MM_IE_REFRESH,0);
		else if(msg.pSender == about || msg.pSender == about2)::SendMessage(hParent,MSG_MENUS,MM_ABOUT,0);
		else if(msg.pSender == breakall)::SendMessage(hParent,MSG_MENUS,MM_BREAKALL,0);
		else if(msg.pSender == findfile)::SendMessage(hParent,MSG_MENUS,MM_FINDFILE,0);
		else if(msg.pSender == offsite )::SendMessage(hParent,MSG_MENUS,MM_OPENOFFSET,0);
		else if(msg.pSender == offsite2 || msg.pSender == offsite3)::SendMessage(hParent,MSG_MENUS,MM_OFFSITE,0);
		else if(msg.pSender == openurl) ::SendMessage(hParent,MSG_MENUS,MM_OPENURL,0);
		else if(msg.pSender == autopoweroff) ::SendMessage(hParent,MSG_MENUS,MM_AUTOPOWER,0);
		else if(msg.pSender == onecycle)::SendMessage(hParent,MSG_MENUS,MM_ONECYCLE,0);
		else if(msg.pSender == connect)::SendMessage(hParent,MSG_MENUS,MM_CONNECT,0);
		else if(msg.pSender == sortcycle)::SendMessage(hParent,MSG_MENUS,MM_SORTCYCLE,0);
		else if(msg.pSender == connall)::SendMessage(hParent,MSG_MENUS,MM_CONNECTALL,0);
		else if(msg.pSender == showmain)::SendMessage(hParent,MSG_MENUS,MM_SHOW,0);
		else if(msg.pSender == quit || msg.pSender == quit2)::SendMessage(hParent,MSG_MENUS,MM_QUIT,0);
		else if(msg.pSender == playpause)::SendMessage(hParent,MSG_MENUS,MM_PLAYPAUSE,0);
		else if(msg.pSender == stop)::SendMessage(hParent,MSG_MENUS,MM_STOP,0);
		else if(msg.pSender == left)::SendMessage(hParent,MSG_MENUS,MM_PREV,0);
		else if(msg.pSender == right)::SendMessage(hParent,MSG_MENUS,MM_NEXT,0);
		else if(msg.pSender == full)::SendMessage(hParent,MSG_MENUS,MM_FULL,0);
		else if(msg.pSender == list)::SendMessage(hParent,MSG_MENUS,MM_LIST,0);
		else if(msg.pSender == playup)::SendMessage(hParent,MSG_MENUS,MM_PLAYUP,0);
		else if(msg.pSender == ar)::SendMessage(hParent,MSG_MENUS,MM_AR,0);
		else if(msg.pSender == noar)::SendMessage(hParent,MSG_MENUS,MM_NOAR,0);
		else if(msg.pSender == ar43)::SendMessage(hParent,MSG_MENUS,MM_4AR3,0);
		else if(msg.pSender == ar169)::SendMessage(hParent,MSG_MENUS,MM_16AR9,0);
		else if(msg.pSender == picset)::SendMessage(hParent,MSG_MENUS,MM_PICSET,0);
	}
}

LRESULT CMyMenu::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) 
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:		lRes = OnKillFocus(uMsg,wParam,lParam);break;
	case WM_MOVE:			lRes = OnMoveWindow(uMsg,wParam,lParam);break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) 
		return lRes;

	if( m_pm->MessageHandler(uMsg, wParam, lParam, lRes) ) 
		return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMyMenu::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	m_pm->Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create((LPCTSTR)skn,_T("menu.xml"), (UINT)0, NULL, m_pm);
	ASSERT(pRoot && "Failed to parse xml");
	m_pm->AttachDialog(pRoot);
	m_pm->AddNotifier(this);

	SIZE szInitWindowSize = m_pm->GetInitSize();
	if( szInitWindowSize.cx != 0 ) 
	{
		::SetWindowPos(*this, NULL, 0, 0, szInitWindowSize.cx, szInitWindowSize.cy, SWP_NOZORDER | SWP_NOMOVE);
	}

	Init();
	return 0;
}
LRESULT CMyMenu::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CMyMenu::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMyMenu::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMyMenu::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm->GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_pm->FindControl(pt));
		if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0)
		{
			return HTCAPTION;
		}
	}

	return HTCLIENT;
}

LRESULT CMyMenu::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm->GetRoundCorner();
	if (this == NULL)
	{
		return 0;
	}
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) 
	{
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		RECT rc = { rcClient.left, rcClient.top + szRoundCorner.cx, rcClient.right, rcClient.bottom };
		HRGN hRgn1 = ::CreateRectRgnIndirect( &rc );
		HRGN hRgn2 = ::CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right + 1, rcClient.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
		::CombineRgn( hRgn1, hRgn1, hRgn2, RGN_OR );
		::SetWindowRgn(*this, hRgn1, TRUE);
		::DeleteObject(hRgn1);
		::DeleteObject(hRgn2);
	}

	bHandled = FALSE;
	return 0;
}
LRESULT CMyMenu::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ShowWindow(false,false);
	return 0;
}
LRESULT CMyMenu::OnMoveWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rtClient;
	POINTS pt	= MAKEPOINTS(lParam);
	int x = (int)(short) LOWORD(lParam);
	int y = (int)(short) HIWORD(lParam);
	::GetWindowRect(m_hWnd,&rtClient);

	int xScreen = ::GetSystemMetrics(SM_CXSCREEN);
	int yScreen = ::GetSystemMetrics(SM_CYSCREEN);

	if(rtClient.bottom > yScreen) 
	{
		y -= rtClient.bottom-rtClient.top;
	}
	if(rtClient.right > xScreen)
	{
		x -= rtClient.right-rtClient.left;
	}
	return MoveWindow(m_hWnd,
		x,
		y,
		rtClient.right - rtClient.left,
		rtClient.bottom - rtClient.top,FALSE);
}
LRESULT CMyMenu::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) 
	{
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

void CMyMenu::CheckButton(CButtonUI* pButton, bool check)
{
	if(pButton == NULL)
		return ;
	if(check == TRUE)
		pButton->SetFtImage(_T("file='checked.png' dest='8,8,17,16'"));
	else
		if(check == FALSE)
			pButton->SetFtImage(_T(""));
}
