#include "StdAfx.h"
#include "WaitPoweroff.h"

CWaitPoweroff::CWaitPoweroff(void)
{
	seconds = 100000;
	m_pm = new CPaintManagerUI;
}

CWaitPoweroff::~CWaitPoweroff(void)
{
}

void CWaitPoweroff::Init()
{
	m_btnClose = static_cast<CButtonUI*>(m_pm->FindControl(_T("btnClose")));
	m_timeLabel = static_cast<CLabelUI*>(m_pm->FindControl(_T("time")));
	m_btnPoweroffNow = static_cast<CButtonUI*>(m_pm->FindControl(_T("rn")));
	m_btnNoPoweroff = static_cast<CButtonUI*>(m_pm->FindControl(_T("cancel")));
	return ;
}

void CWaitPoweroff::Notify(TNotifyUI& msg)
{
	if( msg.sType == MST_WINDOWINIT ) 
	{
		OnPrepare();
	}
	else if(msg.sType == MST_CLICK)
	{
		if (msg.pSender == m_btnClose)
		{
			ShowWindow(false,false);
		}
		else if(msg.pSender == m_btnPoweroffNow)
		{
			seconds = 0;
		}
		else if(msg.pSender == m_btnNoPoweroff)
		{
			m_pm->KillTimer(m_btnClose,1);
			ShowWindow(FALSE,FALSE);
		}
	}
	else if(msg.sType == MST_TIMER)
	{
		if(msg.pSender == m_btnClose)//DWORD 1
		{
			CStdString timeStr;
			timeStr.Format(_T(" %d "),seconds);
			m_timeLabel->SetText(timeStr);
			m_timeLabel->NeedParentUpdate();
			if(seconds == 0)
			{
				m_pm->KillTimer(m_btnClose,1);
				ShowWindow(FALSE,FALSE);
				PowerOff();
				//MessageBox(NULL,_T("关机了"),_T("关机了"),0);
			}
			seconds --;
		}
	}

}

LRESULT CWaitPoweroff::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
	CControlUI* pRoot = builder.Create((LPCTSTR)skn,_T("poweroff.xml"), (UINT)0, NULL, m_pm);
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

LRESULT CWaitPoweroff::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CWaitPoweroff::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CWaitPoweroff::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CWaitPoweroff::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 )
		{
			return HTCAPTION;
		}
	}

	return HTCLIENT;
}

LRESULT CWaitPoweroff::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		HRGN hRgn2 = ::CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn2, TRUE);
		::DeleteObject(hRgn2);
	}
	bHandled = FALSE;
	return 0;
}
LRESULT CWaitPoweroff::OnMoveWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rtClient;
	POINTS pt	= MAKEPOINTS(lParam);
	int x = (int)(short) LOWORD(lParam);
	int y = (int)(short) HIWORD(lParam);
	::GetWindowRect(m_hWnd,&rtClient);
	return MoveWindow((int)(short) LOWORD(lParam),(int)(short) HIWORD(lParam),rtClient.right - rtClient.left,rtClient.bottom - rtClient.top,FALSE);;
}
LRESULT CWaitPoweroff::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CWaitPoweroff::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void CWaitPoweroff::PowerOff(BOOL bPoweroff)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	DWORD dwVersion;

	dwVersion = GetVersion();         // 得到WINDOWS NT或Win32的版本号

	if(dwVersion < 0x80000000)        // 用于判断WIN系列，从而设置相应的权限
	{
		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	}
	if(bPoweroff)
		ExitWindowsEx(EWX_FORCE | EWX_SHUTDOWN | EWX_POWEROFF, 0);
	else
		ExitWindowsEx(EWX_LOGOFF,0);
}
void CWaitPoweroff::PowerOffStartup(int SecondsLater)
{
	seconds = SecondsLater;
	m_pm->SetTimer(m_btnClose,1,1000);
}

void CWaitPoweroff::ShutDownPoweroff(void)
{
	m_pm->KillTimer(m_btnClose,1);
}

void CWaitPoweroff::ShotdownNow(void)
{
}
