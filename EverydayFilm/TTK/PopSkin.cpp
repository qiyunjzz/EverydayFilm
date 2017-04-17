#include "StdAfx.h"
#include "PopSkin.h"

CPopSkin::CPopSkin() { };

LPCTSTR CPopSkin::GetWindowClassName() const { return _T("PopSkin"); };
 
UINT CPopSkin::GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
    
void CPopSkin::OnFinalMessage(HWND /*hWnd*/) { delete this; };

void CPopSkin::Init() 
{
	TCHAR path[MAX_PATH] = {0};
	::GetModuleFileName(NULL,path,MAX_PATH);
	m_sknSet = path;            //获得程序路径
	m_sknSet.Replace(_T("FilmEveryday.exe"),_T("ttkvodConfig.db"));  // 主程序  \\QvodPlayer\\QvodCfg.ini
	m_pHLay=static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("set_skin")));
    m_pOpt = static_cast<COptionUI*>(m_pm.FindControl(_T("alway_skin")));

	m_pBlack = static_cast<COptionUI*>(m_pm.FindControl(_T("black")));
	m_pBlue = static_cast<COptionUI*>(m_pm.FindControl(_T("blue")));
	m_pPurple = static_cast<COptionUI*>(m_pm.FindControl(_T("purple")));

	ReadFile();
}

void CPopSkin::ReadFile()
{
	int isSelected;
	isSelected=::GetPrivateProfileInt(_T("SetSkin"),_T("alway_skin"),TRUE,m_sknSet);
	m_iSkin = ::GetPrivateProfileInt(_T("SetSkin"),_T("selected"),MM_SKIN_BLACK,m_sknSet);

	if(m_iSkin == MM_SKIN_BLACK) m_pBlack->Selected(TRUE);
	else if(m_iSkin == MM_SKIN_PURPLE) m_pPurple->Selected(TRUE);
	else if(m_iSkin == MM_SKIN_BLUE) m_pBlue->Selected(TRUE);

	m_pOpt->Selected(isSelected);
}

void CPopSkin::Notify(TNotifyUI& msg)      
{
    if( msg.sType == MST_SELECTCHANGED)
	{
		HWND hWndParent=::GetParent(m_hWnd);
		CString name=msg.pSender->GetName();
		if(msg.pSender==m_pBlack)	
		{
			if(m_iSkin!=MM_SKIN_BLACK)
			{
				::SendMessage(hWndParent,MSG_MENUS,MM_SKIN_BLACK,0);
				m_iSkin = MM_SKIN_BLACK;
			}
			::WritePrivateProfileString(_T("SetSkin"),_T("selected"),_T("200"),m_sknSet);
		}
		else if(msg.pSender==m_pPurple)	
		{
			if(m_iSkin!=MM_SKIN_PURPLE)
			{
				::SendMessage(hWndParent,MSG_MENUS,MM_SKIN_PURPLE,0);
				m_iSkin = MM_SKIN_PURPLE;
			}
			::WritePrivateProfileString(_T("SetSkin"),_T("selected"),_T("201"),m_sknSet);
		}
		else if(msg.pSender==m_pBlue)
		{
			if(m_iSkin!=MM_SKIN_BLUE)
			{
				::SendMessage(hWndParent,MSG_MENUS,MM_SKIN_BLUE,0);
				m_iSkin = MM_SKIN_BLUE;
			}
			::WritePrivateProfileString(_T("SetSkin"),_T("selected"),_T("202"),m_sknSet);
		}
		else if(msg.pSender==m_pOpt)
		{
			if(!m_pOpt->IsSelected())
			{
				::WritePrivateProfileString(_T("SetSkin"),_T("alway_skin"),_T("0"),m_sknSet);
				return;
			}
			else
			{
				::WritePrivateProfileString(_T("SetSkin"),_T("alway_skin"),_T("1"),m_sknSet);
				return ;
			}
		}
	}
}

LRESULT CPopSkin::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
   LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
   styleValue &= ~WS_CAPTION;
   ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
   RECT rcClient;
   ::GetClientRect(*this, &rcClient);
   ::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
         rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

   m_pm.Init(m_hWnd);
   CDialogBuilder builder;

   CControlUI* pRoot = builder.Create(skn.GetData(),_T("change_skin.xml"), (UINT)0, (UINT)0, &m_pm);	
   ASSERT(pRoot && "Failed to parse XML");
   m_pm.AttachDialog(pRoot);
   m_pm.AddNotifier(this);

   SIZE szInitWindowSize = m_pm.GetInitSize();
   if( szInitWindowSize.cx != 0 ) 
       ::SetWindowPos(*this, NULL, 0, 0, szInitWindowSize.cx, szInitWindowSize.cy, SWP_NOZORDER | SWP_NOMOVE);
     
    Init();
    return 0;
}

LRESULT CPopSkin::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
   return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CPopSkin::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
   return 0;
}

LRESULT CPopSkin::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
   return 0;
}

LRESULT CPopSkin::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(*this, &pt);

    RECT rcClient;
    ::GetClientRect(*this, &rcClient);

    RECT rcCaption = m_pm.GetCaptionRect();
      if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
         && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
             CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
             if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 )
                return HTCAPTION;
        }

        return HTCLIENT;
}

LRESULT CPopSkin::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  if( m_hWnd != (HWND) wParam ) 
	  ShowWindow(false,false);
	 return 0;
}

BOOL CPopSkin::MoveWindow(LPRECT rect, BOOL bRepaint)
{
	return ::MoveWindow(m_hWnd,rect->left,rect->top,rect->right,rect->bottom,bRepaint);
}

BOOL CPopSkin::MoveWidow(int x, int y, int width, int height, BOOL bRepaint)
{
	//return ::SetWindowPos(m_hWnd,NULL,x,y,width,height,NULL);
	return ::MoveWindow(m_hWnd,x,y,width,height,bRepaint);
}


LRESULT CPopSkin::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    SIZE szRoundCorner = m_pm.GetRoundCorner();
    if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            RECT rcClient;
            ::GetClientRect(*this, &rcClient);
            RECT rc = { rcClient.left, rcClient.top + szRoundCorner.cx, rcClient.right, rcClient.bottom };
            HRGN hRgn1 = ::CreateRectRgnIndirect( &rc );
            HRGN hRgn2 = ::CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right + 1, rcClient.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
            ::CombineRgn( hRgn1, hRgn1, hRgn2, RGN_OR );
            ::SetWindowRgn(*this, hRgn1, TRUE);
			//::SetWindowRgn(*this, hRgn2, TRUE);
            ::DeleteObject(hRgn1);
            ::DeleteObject(hRgn2);
        }

        bHandled = FALSE;
        return 0;
}

LRESULT CPopSkin::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;
    BOOL bHandled = TRUE;
    switch( uMsg ) {
    case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
    case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
    case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
    case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
    case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
    case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:     lRes=OnKillFocus(uMsg, wParam, lParam, bHandled);break;
    default:
        bHandled = FALSE;
	}
    if( bHandled ) return lRes;
    if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}