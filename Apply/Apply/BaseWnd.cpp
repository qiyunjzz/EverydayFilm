#include "StdAfx.h"
#include "BaseWnd.h"
#include "Apply.h"


typedef   BOOL   (FAR   PASCAL   *   FUNCTransparent)(   
											HWND   hwnd,                       //   handle   to   the   layered   window   
											COLORREF   crKey,             //   specifies   the   color   key   
											BYTE   bAlpha,                   //   value   for   the   blend   function   
											DWORD   dwFlags                 //   action   
											);   


CBaseWnd::CBaseWnd(void)
{
	pOldBitmap=NULL;
	border_Newoffset.clear();
	m_bMoveFirst=FALSE;
}

CBaseWnd::~CBaseWnd(void)
{
	if(pOldBitmap!=NULL)
	{	
		m_dcBk.SelectObject(pOldBitmap);
	    pOldBitmap=NULL;	
	}

	m_dcBk.DeleteDC();
	bmp.DeleteObject();
}

void CBaseWnd::SetTrans(int itrans)
{
	HMODULE   hModule   =   GetModuleHandle(_T("user32.dll"));   
    FUNCTransparent   SetLayeredWindowAttributes;   
    SetLayeredWindowAttributes= (FUNCTransparent)GetProcAddress(hModule,"SetLayeredWindowAttributes");      
     //  设置分层扩展标记   
    SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|0x80000L);   
    //   70%   alpha   
    SetLayeredWindowAttributes(GetSafeHwnd(),   0,   (255   *   itrans)   /   100,   0x2); 

}

void CBaseWnd::CreatePlayWnd()
{
	  CreateEx(0, 
	  AfxRegisterWndClass( 
	  NULL,LoadCursor (NULL,IDC_ARROW),(HBRUSH) GetStockObject (WHITE_BRUSH),0), 
	  NULL, 
	  WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX,
	  0, 0, 0,0,  //自定义窗口大小
	  NULL, 
	  NULL, 
	  NULL);
	   this->CenterWindow();
}

// 设置窗口区域
void CBaseWnd::SetupRegion(int border_offset[], int nSize)
{
	CDC* pDC = GetDC();

	CRect	rc;
	GetWindowRect(rc);
	rc.OffsetRect(-rc.left, -rc.top);

	CRgn	rgn;
	rgn.CreateRectRgn(0, 0, rc.Width(), rc.Height());
	CRgn	rgn_xor;
	CRect	rcXor;
	
	for (int y = 0; y < nSize; ++y)
	{
		rcXor.SetRect(0, y, border_offset[y], y + 1);
		rgn_xor.CreateRectRgn(0, y, border_offset[y], y + 1);
		rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
		rgn_xor.DeleteObject();
	}

	for (int y = 0; y < nSize; ++y)
	{
		rcXor.SetRect(rc.right - border_offset[y], y, rc.right, y + 1);
		rgn_xor.CreateRectRgn(rc.right - border_offset[y], y, rc.right, y + 1);
		rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
		rgn_xor.DeleteObject();
	}

	{
		for (int y = 0; y < nSize; ++y)
		{
			rcXor.SetRect(0, rc.bottom - y - 1, border_offset[y],  rc.bottom - y);
			rgn_xor.CreateRectRgn(0, rc.bottom - y - 1, border_offset[y],  rc.bottom - y);
			rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
			rgn_xor.DeleteObject();
		}

		for (int y = 0; y < nSize; ++y)
		{
			rcXor.SetRect(rc.right - border_offset[y], rc.bottom - y - 1, rc.right, rc.bottom -  y);
			rgn_xor.CreateRectRgn(rc.right - border_offset[y], rc.bottom - y - 1, rc.right,rc.bottom -  y);
			rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
			rgn_xor.DeleteObject();
		}
/*  		HWND hWnd = GetSafeHwnd();
  		SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE) | WS_EX_LAYERED);
  		SetLayeredWindowAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY );	*/	
	}
	SetWindowRgn((HRGN)rgn, TRUE);
	ReleaseDC(pDC);
}

void CBaseWnd::SetBkGnd(CDC* pDC)
{
	if(pOldBitmap!=NULL)
	{	
		m_dcBk.SelectObject(pOldBitmap);
	    pOldBitmap=NULL;	
	}

	m_dcBk.DeleteDC();

	CRect rcWindow;
	GetWindowRect(&rcWindow);

	m_dcBk.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rcWindow.Width(), rcWindow.Height());
	pOldBitmap = m_dcBk.SelectObject(&bmp);
	m_dcBk.BitBlt(0, 0, rcWindow.Width(), rcWindow.Height(), pDC, 0, 0, SRCCOPY);
	
	bmp.DeleteObject();
}

CDC* CBaseWnd::GetBkGnd()
{
	return &m_dcBk;
}


void CBaseWnd::PaintFrame(CDC* pDC)
{
}


void CBaseWnd::MoveControl()
{
}

void CBaseWnd::OnInitCommand()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CBaseWnd, CWnd)
ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_ERASEBKGND()
ON_WM_LBUTTONDOWN()
ON_WM_NCPAINT()
ON_WM_PAINT()
ON_WM_SIZE()
ON_WM_SIZING()
END_MESSAGE_MAP()

int CBaseWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &=WS_CLIPCHILDREN|WS_POPUP|WS_CLIPSIBLINGS|WS_SYSMENU|WS_MINIMIZEBOX| DS_3DLOOK|DS_FIXEDSYS|DS_SETFONT;

	::SetWindowLong(*this, GWL_STYLE, styleValue);

	ModifyStyleEx(NULL,WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|WS_EX_CONTROLPARENT|WS_EX_APPWINDOW);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	CRect rect;
	this->GetWindowRect(&rcClient);

	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left,rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	OnInitCommand();
	return 0;
}

void CBaseWnd::OnDestroy()
{
	CWnd::OnDestroy();
}

BOOL CBaseWnd::OnEraseBkgnd(CDC* pDC)
{	return TRUE; }

void CBaseWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!IsZoomed())	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CWnd::OnLButtonDown(nFlags, point);
}

void CBaseWnd::OnNcPaint()
{}

void CBaseWnd::OnPaint()
{
	CPaintDC pWindowDC(this);	
	CDC dcMem;
	CBitmap Bmp,dcBmp;
	HGDIOBJ pOldBmp;

	CRect rect,rcClient;
	GetWindowRect(&rect);
	GetClientRect(&rcClient);

	dcMem.CreateCompatibleDC(&pWindowDC);
	dcBmp.CreateCompatibleBitmap(&pWindowDC,rect.Width(),rect.Height());

	pOldBmp=dcMem.SelectObject(dcBmp);
	dcMem.BitBlt(0,0,rect.Width(),rect.Height(),&pWindowDC,0,0,SRCCOPY);

	PaintFrame(&dcMem);
	SetBkGnd(&dcMem);

	pWindowDC.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBmp);
	dcMem.DeleteDC();
	ReleaseDC(&pWindowDC);

	MoveControl(); //移动控件
	m_bMoveFirst=TRUE;
}

void CBaseWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	int border_offset[] = {3, 2, 1};

	SetupRegion(border_offset, 3);
	Invalidate(TRUE);
}

void CBaseWnd::OnSizing(UINT fwSide, LPRECT pRect)
{
	CWnd::OnSizing(fwSide, pRect);
	m_bMoveFirst=FALSE;
}
