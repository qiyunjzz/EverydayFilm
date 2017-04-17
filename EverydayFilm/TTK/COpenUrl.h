#pragma once

class COpenUrl : public CWindowWnd, public INotifyUI
{
public:
	COpenUrl()
	{ };

	LPCTSTR GetWindowClassName() const 
	{ 
		return _T("COpenUrl"); 
	};

	UINT GetClassStyle() const 
	{
		return UI_CLASSSTYLE_DIALOG;
	};

	void OnFinalMessage(HWND /*hWnd*/) 
	{ 
		delete this; 
	};

	void Notify(TNotifyUI& msg)
	{
		if(msg.sType==MST_CLICK)
		{
			if(msg.pSender->GetName()==_T("URLbtnClose"))
			{
				ShowWindow(false,false);
				return;
			}
			else if(msg.pSender->GetName()==_T("URLsure"))
			{
				CControlUI* pEditControl = m_pm.FindControl(_T("URLathedit"));
				CString pTxt = pEditControl->GetText();
				pTxt.Trim();
				if(isurl(pTxt.GetBuffer()))
					::SendMessage(::GetParent(GetHWND()),MSG_MENUS,MM_ADDURL,(LPARAM)pTxt.GetBuffer());
				ShowWindow(false,false);
				return;
			}
			else if(msg.pSender->GetName() == _T("URLgiveup"))
			{
				ShowWindow(false,false);
				return;
			}
		}
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		CControlUI* pRoot = builder.Create(skn.GetData(),_T("urldlg.xml"), (UINT)0, (UINT)0, &m_pm);			//xml 需要修改
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		::SetWindowPos(*this, NULL, 0, 0, 460,180, SWP_NOZORDER | SWP_NOMOVE);					//窗口大小定义

		return 0;
	}

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

	BOOL isurl(TCHAR* strurl)
	{	
		USES_CONVERSION;
		TCHAR* urlhttp=_T("http://");
		TCHAR* urlqvod=_T("qvod://");
		int urllen=_tcsclen(strurl)+1;
		TCHAR* pstrurl=new TCHAR[urllen];
		_tcscpy(pstrurl,strurl);
		//CControlUI* pEditControl = m_pm.FindControl(_T("URLathedit"));

		if(_tcsstr(pstrurl,urlqvod)>0/*&&_tcsstr(pstrurl,_T(".rmvb|"))*/)		//url中是否含有qvod://
		{
			TCHAR* urlEumurl=pstrurl;
			TCHAR* urlEumqvod=urlqvod;
			int eum=0;
			while(urlEumurl[eum]==urlEumqvod[eum])		//判断qvod://是否在url首位
				eum++;
			if(eum==7)
			{
				TCHAR seps[] =_T("|");
				TCHAR *token=NULL;
				TCHAR* purl[4];
				token = _tcstok( pstrurl, seps );
				int i=0;
				while( token != NULL )					//分割字符 '|'
				{
					purl[i]=token;
					token = _tcstok( NULL, seps );
					i++;
				}
				if(i==3&&_tcsclen(purl[1])==40)			//分割符'|'有四个 16进制数有四十个 
				{
					//if(_tcsstr(purl[2],_T(".rmvb")))			//是否为.rm格式
					//{
						/*pEditControl->SetText(strurl); */
						delete []pstrurl;	
						return TRUE;
					//}
				}
			}
		}
		else if(_tcsstr(pstrurl,urlhttp)>0/*&&_tcsstr(pstrurl,_T(".rmvb"))*/)
		{
			TCHAR* urlEumurl=pstrurl;
			TCHAR* urlEumqvod=urlhttp;
			int eum=0;
			while(urlEumurl[eum]==urlEumqvod[eum])		//判断qvod://是否在url首位
				eum++;
			if(eum==7)
			{
				TCHAR seps[] =_T("/");
				TCHAR *token=NULL;
				TCHAR *pur;
				int mm=0;
				token = _tcstok( pstrurl, seps );
				while(token!=NULL)
				{
					mm++;
					pur=token;
					token=_tcstok(NULL,seps);
				}
				if(mm>2/*&&_tcsstr(pur,_T(".rmvb"))*/)
				{
					//pEditControl->SetText(strurl);
					delete []pstrurl;	
					return TRUE;
				}
			}
		}
		//pEditControl->SetText(_T(""));	
		delete []pstrurl;	
		return FALSE;
	}
	void readClipboardData()
	{
		USES_CONVERSION;
		if(IsClipboardFormatAvailable (CF_UNICODETEXT))
		{
			WCHAR     *str;
			OpenClipboard (NULL) ;
			HGLOBAL hGlobal=GetClipboardData (CF_UNICODETEXT) ;
			if(hGlobal)
			{
				str = (WCHAR*)GlobalLock (hGlobal) ;
				GlobalUnlock (hGlobal);
			}
			CloseClipboard () ;
			CControlUI* pEditControl = m_pm.FindControl(_T("URLathedit"));
			CString cstr = str;
			cstr.Trim();
			if(isurl(cstr.GetBuffer()))
				pEditControl->SetText(cstr.GetBuffer());
			else 
				pEditControl->SetText(_T(""));
			
			
		}
	}


	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SIZE szRoundCorner = m_pm.GetRoundCorner();
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

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			default:
				bHandled = FALSE;
		}
		if( bHandled )
			return lRes;
		if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) )
			return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}
public:
	CPaintManagerUI m_pm;
	CStdString skn;
};