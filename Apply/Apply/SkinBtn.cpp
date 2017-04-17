#include "StdAfx.h"
#include "SkinBtn.h"
#include "BaseWnd.h"

#define TOOLTIP_ID 1
CSkinBtn::CSkinBtn(void)
{
	m_sNormalImage.Empty();
	m_sHotImage.Empty();
	m_sPushedImage.Empty();
	m_sFocusedImage.Empty();
	m_sDisabledImage.Empty(); 
	m_sControlName.Empty();
	m_Tracking=FALSE;
	m_bFirst=TRUE;
	m_nIndex=-1;

	m_bShowText=TRUE;
	m_bFirstToolTip=FALSE;

	m_strTextColour=RGB(255,255,255);
}

CSkinBtn::~CSkinBtn(void)
{
}

BEGIN_MESSAGE_MAP(CSkinBtn, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSEHOVER,OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_DRAWITEM()
	ON_WM_CREATE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////
/////////// Message
//擦出背景
BOOL CSkinBtn::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CSkinBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_bFirstToolTip&&!m_sToolTip.IsEmpty())
	{
		m_bFirstToolTip=TRUE;
		CRect rect; 
		GetClientRect(&rect);
		m_ToolTip.AddTool(this, m_sToolTip, rect, TOOLTIP_ID);
	}

	if (!m_Tracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize=sizeof(tme);
		tme.dwFlags=TME_HOVER|TME_LEAVE;
		tme.hwndTrack=m_hWnd;
		tme.dwHoverTime=1;
		m_Tracking=_TrackMouseEvent(&tme);  //记录鼠标事件
	}	
	CButton::OnMouseMove(nFlags, point);
}
LRESULT CSkinBtn::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	m_Tracking=FALSE;
	Invalidate();
	return 0L;
}
LRESULT CSkinBtn::OnMouseHover(WPARAM wParam,LPARAM lParam)
{
	Invalidate();
	return 0L;
}

void CSkinBtn::MoveButton(int x,int y,int cx,int cy,BOOL lRepaint)
{
	MoveWindow(x,y,cx,cy,lRepaint);
	Invalidate(FALSE);
}

void CSkinBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(m_bFirst)
	{
		CRect rect;
		GetClientRect(&rect);
		m_rcItem=m_rcPaint=rect;	
		m_bFirst=FALSE;
	}

	int nIndex=0;//默认状态下
  	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	   
	UINT nState=lpDrawItemStruct->itemState;

	if (m_Tracking)    //根据状态画图
	{
		if (nState&ODS_SELECTED)		nIndex=4;//鼠标按下
		else 			nIndex=2;//鼠标经过
	}
	else if (nState&ODS_FOCUS)		nIndex=3;//焦点下
	else if (nState&ODS_DISABLED)   nIndex=1;//不可用
   	
   DrawButton(pDC,nIndex);	
}

void CSkinBtn::PreSubclassWindow()
{
	//设置所有者绘制控制
	CButton::PreSubclassWindow();
    ModifyStyle(0, BS_OWNERDRAW);
}


void CSkinBtn::DrawButton(CDC *pDC, int nIndex)
{
	m_nIndex=nIndex;
    PaintBk(pDC);
	if(m_nIndex==1&&_tcscmp(GetDisabledImage(),_T(""))==0) m_nIndex=0;
	if(m_nIndex==2&&_tcscmp(GetHotImage(),_T(""))==0) m_nIndex=0;
	if(m_nIndex==3&&_tcscmp(GetFocusedImage(),_T(""))==0) m_nIndex=0;

	if(m_nIndex==0)	 m_renderEngine.DrawImageString(pDC->m_hDC,m_rcItem,m_rcPaint,GetNormalImage());
	else if(m_nIndex==1) m_renderEngine.DrawImageString(pDC->m_hDC,m_rcItem,m_rcPaint,GetDisabledImage());
	else if(m_nIndex==2) m_renderEngine.DrawImageString(pDC->m_hDC,m_rcItem,m_rcPaint,GetHotImage());
	else if(m_nIndex==3) m_renderEngine.DrawImageString(pDC->m_hDC,m_rcItem,m_rcPaint,GetFocusedImage());
	else if(m_nIndex==4) m_renderEngine.DrawImageString(pDC->m_hDC,m_rcItem,m_rcPaint,GetPushedImage());	
	
	if(m_bShowText)
		DrawText(pDC,m_nIndex);
}

LPCTSTR CSkinBtn::GetName()
{
    return m_sControlName;
}

void CSkinBtn::SetName(LPCTSTR pStrImage)
{
    m_sControlName = pStrImage;
}

void CSkinBtn::SetShowText(BOOL bShow)
{
	m_bShowText=bShow;
}

void CSkinBtn::SetTextColor(COLORREF TextColour)
{
	m_strTextColour=TextColour;
}

LPCTSTR CSkinBtn::GetToolTip()
{
	return m_sToolTip;
}

void CSkinBtn::SetToolTip(LPCTSTR pStr)
{
	m_sToolTip=pStr;

	if (::IsWindow(GetSafeHwnd())) {
		m_ToolTip.UpdateTipText(m_sToolTip, this, TOOLTIP_ID);
		m_bFirstToolTip=FALSE;
	}
}

LPCTSTR CSkinBtn::GetNormalImage()
{
    return m_sNormalImage;
}

void CSkinBtn::SetNormalImage(LPCTSTR pStrImage)
{
    m_sNormalImage = pStrImage;
}

LPCTSTR CSkinBtn::GetHotImage()
{
    return m_sHotImage;
}

void CSkinBtn::SetHotImage(LPCTSTR pStrImage)
{
    m_sHotImage = pStrImage;
}

LPCTSTR CSkinBtn::GetPushedImage()
{
    return m_sPushedImage;

}

void CSkinBtn::SetPushedImage(LPCTSTR pStrImage)
{
    m_sPushedImage = pStrImage;
}

LPCTSTR CSkinBtn::GetFocusedImage()
{
    return m_sFocusedImage;
}

void CSkinBtn::SetFocusedImage(LPCTSTR pStrImage)
{
    m_sFocusedImage = pStrImage;
}

LPCTSTR CSkinBtn::GetDisabledImage()
{
    return m_sDisabledImage;
}

void CSkinBtn::SetDisabledImage(LPCTSTR pStrImage)
{
    m_sDisabledImage = pStrImage;
}


void CSkinBtn::PaintBk(CDC* pDC)
{
	if(((CBaseWnd*)GetParent())->GetBkGnd()->m_hDC==NULL) return;
	CRect rcClient,rcWindow;
	GetClientRect(&rcClient);
    GetWindowRect(&rcWindow);
	GetParent()->ScreenToClient(&rcWindow);    //预存的内存DC 画背景
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), ((CBaseWnd*)GetParent())->GetBkGnd(), rcWindow.left, rcWindow.top, SRCCOPY);
}

void CSkinBtn::DrawText(CDC* pDC,int index)
{	//输出内容
	ASSERT(::GetObjectType(pDC->m_hDC)==OBJ_DC || ::GetObjectType(pDC->m_hDC)==OBJ_MEMDC);	//OBJ_DC：设备上下文环境 OBJ_MEMDC:存设备上下文环境
	::SetBkMode(pDC->m_hDC, TRANSPARENT);		//针对句柄 设置透明效果
	CRect rect;
	GetClientRect(&rect);
	//设置文本及文本颜色	
	CString strText;
	GetWindowText(strText);
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	CFont *pFont = CFont::FromHandle(hFont);
	CFont *pOldFont = (CFont*)pDC->SelectObject(pFont);

	pDC->SetBkMode(TRANSPARENT);
	if(index==4)
	{
		rect.top+=2;rect.left+=2;rect.bottom+=2;rect.right+=2;
	}

	pDC->SetTextColor(m_strTextColour);
	pDC->DrawText(strText, rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
}

int CSkinBtn::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ToolTip.Create(this);

	return 0;
}

BOOL CSkinBtn::PreTranslateMessage(MSG* pMsg)
{
	if(!m_sToolTip.IsEmpty())		m_ToolTip.RelayEvent(pMsg);
	return CButton::PreTranslateMessage(pMsg);
}
