#include "StdAfx.h"
#include "IeWnd.h"
#include "Apply.h"

CIeWnd::CIeWnd(void)
{
}

CIeWnd::~CIeWnd(void)
{
}

void CIeWnd::OnInitCommand()
{
	OnCreateControl();
	OnInitContro();
}

void CIeWnd::OnCreateControl()
{	
	CRect rect;
	this->GetClientRect(&rect);  

	VERIFY(m_htmlCtrl.CreateFromStatic(IE_EXPLOERER,this));
	VERIFY(m_htmlAd.CreateFromStatic(IE_AD_EXPLOERER,this,rect));
	VERIFY(m_htmlBuff.CreateFromStatic(IE_BUFFER_EXPLOERE,this));
	MoveControl();
}

void CIeWnd::OnInitContro()
{
	m_htmlAd.SetSilent(TRUE);
	m_htmlAd.Navigate(theApp.strURL);

	SetTimer(IE_EXPLOERER,500,NULL);	
}

void CIeWnd::PaintFrame(CDC* pDC)
{
}

void CIeWnd::MoveControl()
{
	if(m_bMoveFirst) return;

	CRect rect;
	this->GetClientRect(&rect);  

	m_htmlAd.MoveWindow(&rect,FALSE);
	m_htmlCtrl.MoveWindow(&rect,FALSE);
}


/////////////////////////////////////////////////////////////////////////////////
/////////////
BEGIN_MESSAGE_MAP(CIeWnd, CBaseWnd)
	ON_MESSAGE(IE_BUFFER_EXPLOERE,OnBufferExploere)
	ON_MESSAGE(WM_COMPLETE_HTML,OnViewCompleted)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

LRESULT CIeWnd::OnViewCompleted(WPARAM wParam,LPARAM lParam)
{
	if(!m_htmlCtrl.IsWindowVisible()) m_htmlCtrl.ShowWindow(TRUE);
	if(m_htmlAd.IsWindowVisible())	m_htmlAd.ShowWindow(FALSE);
	return TRUE;
}

LRESULT CIeWnd::OnBufferExploere(WPARAM wParam,LPARAM lParam)
{
	m_htmlBuff.SetSilent(TRUE);
	m_htmlBuff.Navigate(theApp.m_sCommandLine.m_strAdUrl);	
	return TRUE;
}

void CIeWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
}

void CIeWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	Invalidate(TRUE);
}

int CIeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	OnInitCommand();
	return 0;
}

void CIeWnd::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==IE_EXPLOERER)
	{
		KillTimer(IE_EXPLOERER);
		m_htmlCtrl.SetSilent(TRUE);
		m_htmlCtrl.Navigate(theApp.m_sCommandLine.m_strWeb);	//µ¼ÈëÄ³ÍøÕ¾		
	}

	CBaseWnd::OnTimer(nIDEvent);
}

void CIeWnd::OnClose()
{
	CBaseWnd::OnClose();
}
