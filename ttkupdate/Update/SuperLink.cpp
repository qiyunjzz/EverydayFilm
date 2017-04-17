#include "StdAfx.h"
#include "SuperLink.h"


#pragma warning(disable:4311;disable:4312;disable:4996)

#define TOOLTIP_ID 1

//构造函数
CSuperLink::CSuperLink()
{
	m_hLinkCursor       = NULL;                 // No cursor as yet
	m_crLinkColour      = RGB(  0,   0, 238);   // Blue
	m_crVisitedColour   = RGB( 85,  26, 139);   // Purple
	m_crHoverColour     = ::GetSysColor(COLOR_HIGHLIGHT);
	m_bOverControl      = FALSE;                // Cursor not yet over control
	m_bVisited          = FALSE;                // Hasn't been visited yet.
	m_bUnderline        = TRUE;                 // Underline the link?
	m_bAdjustToFit      = TRUE;                 // Resize the window to fit the text?
	m_strURL.Empty();
	
	bmp.DeleteObject();
	pOldBitmap=NULL;

	m_bIsUrl=FALSE;
	m_strTextColor=RGB(255,255,255);

	m_bOver=FALSE;
	m_Tracking=FALSE;
}

//析构函数
CSuperLink::~CSuperLink()
{
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CSuperLink, CStatic)
	//{{AFX_MSG_MAP(CSuperLink)
	ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()

void CSuperLink::OnInitCtrl()
{
	// We want to get mouse clicks via STN_CLICKED
	DWORD dwStyle = GetStyle();
	::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

	m_Font.CreateFont(12,0,0,0,FW_NORMAL/*FW_BOLD*/,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("宋体"));   
	SetFont(&m_Font);

	PositionWindow();        // Adjust size of window to fit URL if necessary
	SetDefaultCursor();      // Try and load up a "hand" cursor

	// Create the tooltip
	CRect rect; 
	GetClientRect(rect);
	m_ToolTip.Create(this);
	m_ToolTip.AddTool(this, m_strURL, rect, TOOLTIP_ID);
}

void CSuperLink::SetLink(BOOL bGo)
{
	m_bIsUrl=bGo;
}

void CSuperLink::SetTextStaticColor(COLORREF color)
{
	m_strTextColor=color;
}


void CSuperLink::SetBkGnd(CDC* pDC)
{
	if(pOldBitmap!=NULL)
	{	
		m_dcBk.SelectObject(pOldBitmap);
	    pOldBitmap=NULL;	
	}

	m_dcBk.DeleteDC();

	CRect rcWindow;
	GetParent()->GetWindowRect(&rcWindow);

	m_dcBk.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rcWindow.Width(), rcWindow.Height());
	pOldBitmap = m_dcBk.SelectObject(&bmp);
	m_dcBk.BitBlt(0, 0, rcWindow.Width(), rcWindow.Height(), pDC, 0, 0, SRCCOPY);
	
	bmp.DeleteObject();		
}

void CSuperLink::PaintBk(CDC* pDC)
{
	CRect rcClient,rcWindow;
	GetClientRect(&rcClient);
    GetWindowRect(&rcWindow);
	GetParent()->ScreenToClient(&rcWindow);    //预存的内存DC 画背景
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(),&m_dcBk, rcWindow.left, rcWindow.top, SRCCOPY);
}




//消息解析
BOOL CSuperLink::PreTranslateMessage(MSG* pMsg) 
{
	m_ToolTip.RelayEvent(pMsg);
	return CStatic::PreTranslateMessage(pMsg);
}

//点击
void CSuperLink::OnClicked()
{
	if(!m_bIsUrl) return;
	int result = (int)GotoURL(m_strURL, SW_SHOW);
	m_bVisited = (result > HINSTANCE_ERROR);
	if (!m_bVisited) 
	{
		MessageBeep(MB_ICONEXCLAMATION);     // Unable to follow link
		ReportError(result);
	}
	else 
		SetVisited();                        // Repaint to show visited colour
//	m_bVisited=FALSE;
}

//控件颜色
HBRUSH CSuperLink::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	ASSERT(nCtlColor == CTLCOLOR_STATIC);

	if(m_bIsUrl)
	{
		//if (m_bOverControl)
		//	pDC->SetTextColor(m_crHoverColour);
		//else if (m_bVisited)
		//	pDC->SetTextColor(m_crVisitedColour);
		//else
		//	pDC->SetTextColor(m_crLinkColour);	
		if(m_bOver) pDC->SetTextColor(m_crHoverColour);
		else pDC->SetTextColor(m_crLinkColour);
	}
	else pDC->SetTextColor(m_strTextColor);

	// transparent text.
	pDC->SetBkMode(TRANSPARENT);
	PaintBk(pDC);
	//RECT rc;
	//GetClientRect(&rc);

	//CBrush br(RGB(244,244,244));
	//pDC->FillRect(&rc,&br);

	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

//鼠标移动
void CSuperLink::OnMouseMove(UINT nFlags, CPoint point) 
{
	CStatic::OnMouseMove(nFlags, point);
	
	if(!m_bIsUrl)  return;

	if (m_bOverControl)        // Cursor is currently over control
	{
		CRect rect;
		GetClientRect(rect);

		if (!rect.PtInRect(point))
		{
			m_bOverControl = FALSE;
			ReleaseCapture();
			RedrawWindow();
			return;
		}
	}
	else                      // Cursor has just moved over control
	{
		m_bOverControl = TRUE;
		RedrawWindow();
		SetCapture();
	}

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	m_Tracking=_TrackMouseEvent(&tme);	
}

//设置光标
BOOL CSuperLink::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
	if (m_hLinkCursor)
	{
		::SetCursor(m_hLinkCursor);
		return TRUE;
	}
	return FALSE;
}

//控件子类化
void CSuperLink::PreSubclassWindow() 
{
	CStatic::PreSubclassWindow();
}

//设置URL
void CSuperLink::SetURL(CString strURL)
{
	m_strURL = strURL;

	if (::IsWindow(GetSafeHwnd())) {
		PositionWindow();
		m_ToolTip.UpdateTipText(strURL, this, TOOLTIP_ID);
	}
}

//得到URL
CString CSuperLink::GetURL() const
{ 
	return m_strURL;   
}

//设置颜色集
void CSuperLink::SetColours(COLORREF crLinkColour, COLORREF crVisitedColour,
							COLORREF crHoverColour /* = -1 */) 
{ 
	m_crLinkColour    = crLinkColour; 
	m_crVisitedColour = crVisitedColour;

	if (crHoverColour == -1)
		m_crHoverColour = ::GetSysColor(COLOR_HIGHLIGHT);
	else
		m_crHoverColour = crHoverColour;

	if (::IsWindow(m_hWnd))
		Invalidate(); 
}

//得到超链接颜色
COLORREF CSuperLink::GetLinkColour() const
{ 
	return m_crLinkColour; 
}

//得到进入后的颜色
COLORREF CSuperLink::GetVisitedColour() const
{
	return m_crVisitedColour; 
}

//得到划过的颜色
COLORREF CSuperLink::GetHoverColour() const
{
	return m_crHoverColour;
}

//设置参观过的颜色
void CSuperLink::SetVisited(BOOL bVisited /* = TRUE */) 
{ 
	m_bVisited = bVisited; 

	if (::IsWindow(GetSafeHwnd()))
		Invalidate(); 
}

//得到是否参观过
BOOL CSuperLink::GetVisited() const
{ 
	return m_bVisited; 
}

//设置光标
void CSuperLink::SetLinkCursor(HCURSOR hCursor)
{ 
	m_hLinkCursor = hCursor;
	if (m_hLinkCursor == NULL)
		SetDefaultCursor();
}

//得到光标
HCURSOR CSuperLink::GetLinkCursor() const
{
	return m_hLinkCursor;
}

//设置下划线
void CSuperLink::SetUnderline(BOOL bUnderline /* = TRUE */)
{
	m_bUnderline = bUnderline;

	if (::IsWindow(GetSafeHwnd()))
	{
		LOGFONT lf;
		GetFont()->GetLogFont(&lf);
		lf.lfUnderline = m_bUnderline;

		m_Font.DeleteObject();
		m_Font.CreateFontIndirect(&lf);
		SetFont(&m_Font);

		Invalidate(); 
	}
}

//得到是否有下划线
BOOL CSuperLink::GetUnderline() const
{ 
	return m_bUnderline; 
}

//设置自动大小
void CSuperLink::SetAutoSize(BOOL bAutoSize /* = TRUE */)
{
	m_bAdjustToFit = bAutoSize;

	if (::IsWindow(GetSafeHwnd()))
		PositionWindow();
}

//得到是否自动大小
BOOL CSuperLink::GetAutoSize() const
{ 
	return m_bAdjustToFit; 
}

//调整位置
void CSuperLink::PositionWindow()
{
	if (!::IsWindow(GetSafeHwnd()) || !m_bAdjustToFit) 
		return;

	// Get the current window position
	CRect rect;
	GetWindowRect(rect);

	CWnd* pParent = GetParent();
	if (pParent)
		pParent->ScreenToClient(rect);

	// Get the size of the window text
	CString strWndText;
	GetWindowText(strWndText);

	CDC* pDC = GetDC();
	CFont* pOldFont = pDC->SelectObject(&m_Font);
	CSize Extent = pDC->GetTextExtent(strWndText);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	// Get the text justification via the window style
	DWORD dwStyle = GetStyle();

	// Recalc the window size and position based on the text justification
	if (dwStyle & SS_CENTERIMAGE)
		rect.DeflateRect(0, (rect.Height() - Extent.cy)/2);
	else
		rect.bottom = rect.top + Extent.cy;

	if (dwStyle & SS_CENTER)   
		rect.DeflateRect((rect.Width() - Extent.cx)/2, 0);
	else if (dwStyle & SS_RIGHT) 
		rect.left  = rect.right - Extent.cx;
	else // SS_LEFT = 0, so we can't test for it explicitly 
		rect.right = rect.left + Extent.cx;

	// Move the window
	SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
}

/////////////////////////////////////////////////////////////////////////////
// CSuperLink implementation

// The following appeared in Paul DiLascia's Jan 1998 MSJ articles.
// It loads a "hand" cursor from the winhlp32.exe module
void CSuperLink::SetDefaultCursor()
{
	if(!m_bIsUrl) return;
	if (m_hLinkCursor == NULL)                // No cursor handle - load our own
	{
		//// Get the windows directory		Windows XP
		//CString strWndDir;
		//GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
		//strWndDir.ReleaseBuffer();

		//strWndDir += _T("\\winhlp32.exe");
		//// This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
		//HMODULE hModule = LoadLibrary(strWndDir);
		//if (hModule) {
		//	HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
		//	if (hHandCursor)
		//		m_hLinkCursor = CopyCursor(hHandCursor);
		//}
		//FreeLibrary(hModule);
       m_hLinkCursor=::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));
	}
}

LONG CSuperLink::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

	if (retval == ERROR_SUCCESS) {
		long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];
		RegQueryValue(hkey, NULL, data, &datasize);
		lstrcpy(retdata,data);
		RegCloseKey(hkey);
	}

	return retval;
}

void CSuperLink::ReportError(int nError)
{
	CString str;
	switch (nError) {
		case 0:                       str = "The operating system is out\nof memory or resources."; break;
		case SE_ERR_PNF:              str = "The specified path was not found."; break;
		case SE_ERR_FNF:              str = "The specified file was not found."; break;
		case ERROR_BAD_FORMAT:        str = "The .EXE file is invalid\n(non-Win32 .EXE or error in .EXE image)."; break;
		case SE_ERR_ACCESSDENIED:     str = "The operating system denied\naccess to the specified file."; break;
		case SE_ERR_ASSOCINCOMPLETE:  str = "The filename association is\nincomplete or invalid."; break;
		case SE_ERR_DDEBUSY:          str = "The DDE transaction could not\nbe completed because other DDE transactions\nwere being processed."; break;
		case SE_ERR_DDEFAIL:          str = "The DDE transaction failed."; break;
		case SE_ERR_DDETIMEOUT:       str = "The DDE transaction could not\nbe completed because the request timed out."; break;
		case SE_ERR_DLLNOTFOUND:      str = "The specified dynamic-link library was not found."; break;
		case SE_ERR_NOASSOC:          str = "There is no application associated\nwith the given filename extension."; break;
		case SE_ERR_OOM:              str = "There was not enough memory to complete the operation."; break;
		case SE_ERR_SHARE:            str = "A sharing violation occurred. ";
		default:                      str.Format(_T("Unknown Error (%d) occurred."), nError); break;
	}
	str = _T("Unable to open hyperlink:\n\n") + str;
	AfxMessageBox(str, MB_ICONEXCLAMATION | MB_OK);
}

HINSTANCE CSuperLink::GotoURL(LPCTSTR url, int showcmd)
{
	TCHAR key[MAX_PATH + MAX_PATH];

	// First try ShellExecute()
	HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

	// If it failed, get the .htm regkey and lookup the program
	if ((UINT)result <= HINSTANCE_ERROR) {

		if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) {
			lstrcat(key, _T("\\shell\\open\\command"));

			if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) {
				TCHAR *pos;
				pos = _tcsstr(key, _T("\"%1\""));
				if (pos == NULL) {                     // No quotes found
					char p[256];
					sprintf(p,"%s",key);
					char pStr[256] = "%1";
					pos = (TCHAR*)strstr(p, pStr);       // Check for %1, without quotes 
					if (pos == NULL)                   // No parameter at all...
						pos = key+lstrlen(key)-1;
					else
						*pos = '\0';                   // Remove the parameter
				}
				else
					*pos = '\0';                       // Remove the parameter

				lstrcat(pos, _T(" "));
				lstrcat(pos, url);
				result = (HINSTANCE) WinExec((LPCSTR)key,showcmd);
			}
		}
	}

	return result;
}
int CSuperLink::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	OnInitCtrl();
	return 0;
}

BOOL CSuperLink::OnEraseBkgnd(CDC* pDC)
{   return TRUE; }

void CSuperLink::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Tracking=FALSE;

	if(m_bOver)
	{
		m_bOver=FALSE;
		Invalidate(FALSE);	
	}

	CStatic::OnMouseLeave();
}

void CSuperLink::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(!m_bOver)
	{
		m_bOver=TRUE;
		Invalidate(FALSE);
	}

	CStatic::OnMouseHover(nFlags, point);
}
