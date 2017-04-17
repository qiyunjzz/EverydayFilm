 #include "StdAfx.h"
 #include "HtmlCtrl.h"
 #include "Header.h"
 #include "Apply.h"

#define DISPID_NEWWINDOW3 273
 
IMPLEMENT_DYNCREATE(CHtmlCtrl, CHtmlView)

BEGIN_DISPATCH_MAP(CHtmlCtrl,CHtmlView)
//	DISP_FUNCTION(CHtmlCtrl,"WobbleWnd",WobbleWnd,VT_EMPTY,VTS_NONE)
END_DISPATCH_MAP()

BEGIN_MESSAGE_MAP(CHtmlCtrl, CHtmlView)
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CHtmlCtrl, CHtmlView)
    ON_EVENT(CHtmlCtrl, AFX_IDW_PANE_FIRST, DISPID_NEWWINDOW3, CHtmlCtrl::OnNewWindow3, VTS_PDISPATCH VTS_PBOOL VTS_UI4 VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP()

void CHtmlCtrl::OnNewWindow3( LPDISPATCH* ppDisp, BOOL* Cancel, DWORD dwFlags, LPCTSTR bstrUrlContext, LPCTSTR bstrUrl )
{
}

CHtmlCtrl::CHtmlCtrl(BOOL bShow)
{
	m_IsView = FALSE;
	m_spDoc = NULL;
	m_bComplete=FALSE;
	m_bFirst = FALSE;

	EnableAutomation();
}

CHtmlCtrl::~CHtmlCtrl(void)
{
}

/////////////////////////////////////////////////////////////////////////
/////////////


int CHtmlCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	return 0;
}

int CHtmlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_IsView)
		return CHtmlView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	else
		return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CHtmlCtrl::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if(::IsWindow(m_wndBrowser.m_hWnd))
	{
		CRect rect;
		GetClientRect(rect);
		::AdjustWindowRectEx(rect,GetStyle(),FALSE,WS_EX_CLIENTEDGE);
		m_wndBrowser.SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOACTIVATE|SWP_NOZORDER);	
	}
}

BOOL CHtmlCtrl::Create(CRect rect,CWnd* pParent,UINT nID)
{
	if(!CHtmlView::Create(NULL,NULL,(WS_CHILD|WS_VISIBLE),rect,pParent,nID,NULL))
		return FALSE;

	m_IsView=FALSE;
	return TRUE;
}

 void CHtmlCtrl::OnDestroy()
{
	if(m_IsView)
       CHtmlView::OnDestroy();
	else
	{
		if(m_pBrowserApp)
		{
			m_pBrowserApp.Release();
			m_pBrowserApp=NULL;		
		}
	     CWnd::OnDestroy();
	}
}

 void CHtmlCtrl::PostNcDestroy() 
{
	if(m_IsView)
		CHtmlView::PostNcDestroy();
}

 BOOL CHtmlCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style|=WS_CLIPSIBLINGS;
	if(m_IsView)
		cs.dwExStyle |=WS_EX_CLIENTEDGE;

	return CHtmlView::PreCreateWindow(cs);
}

 void CHtmlCtrl::OnDocumentComplete(LPCTSTR lpszURL)
{
	// TODO: 在此添加专用代码和/或调用基类

	CHtmlView::OnDocumentComplete(lpszURL);
	//SetScriptDocument();
}

BOOL CHtmlCtrl::GetJScript(CComPtr<IDispatch>& spDisp)
{
	if( m_spDoc==NULL )
		return FALSE;

	HRESULT hr = m_spDoc->get_Script(&spDisp);
	ATLASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}

 BOOL CHtmlCtrl::CallJS(CString strFunc, CString strParam, _variant_t* pVar)
{
	CComPtr<IDispatch> spScript;
	if(!GetJScript(spScript))
	{
		//MessageBox("函数GetJScrip调用失败！");
		return FALSE;
	}
	CComBSTR bstrFunc(strFunc);
	DISPID dispid = NULL;
	HRESULT hr = spScript->GetIDsOfNames(IID_NULL,&bstrFunc,1,
											LOCALE_SYSTEM_DEFAULT,&dispid);
	if(FAILED(hr))
	{
		//MessageBox("错了！");
		return FALSE;
	}

	DISPPARAMS dispparams;
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = (UINT)1;
	dispparams.rgvarg = new VARIANT[dispparams.cArgs];

	CComBSTR bstr = strParam; // back reading
	bstr.CopyTo(&dispparams.rgvarg[0].bstrVal);
	dispparams.rgvarg[0].vt = VT_BSTR;
	dispparams.cNamedArgs = 0;

	EXCEPINFO excepInfo;
	memset(&excepInfo, 0, sizeof excepInfo);
   	_variant_t vaResult;
	UINT nArgErr = (UINT)-1;  // initialize to invalid arg
         
	hr = spScript->Invoke(dispid,IID_NULL,0,
							DISPATCH_METHOD,&dispparams,&vaResult,&excepInfo,&nArgErr);

	delete [] dispparams.rgvarg;
	if(FAILED(hr))
	{
//		MessageBox(GetSystemErrorMessage(hr));
	//	MessageBox("错~");
		return FALSE;
	}
	
	if(pVar)
	{
		*pVar = vaResult;
	}
	return TRUE;
}

void CHtmlCtrl::DocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
	if(this->GetDlgCtrlID()==IE_EXPLOERER)
	{
		if(_tcscmp(URL->bstrVal,_T("about:blank"))==0)  return;

		 LPDISPATCH lpWBDisp = NULL;
		 HRESULT    hr = NULL;

		 hr = m_pBrowserApp->QueryInterface(IID_IDispatch, (void**)&lpWBDisp);
		 ASSERT(SUCCEEDED(hr));

		 if (pDisp == lpWBDisp)	//页面加载完成
		 {
			// Sleep(1000);
			 m_bComplete=TRUE;
			this ->GetOwner()->SendMessage(WM_COMPLETE_HTML);
		 }

		 lpWBDisp->Release();
	}
	CHtmlView::DocumentComplete(pDisp,URL);
}

 BOOL CHtmlCtrl::CreateControlSite(COleControlContainer* pContainer,COleControlSite** ppSite, UINT nID, REFCLSID clsid)
{
		*ppSite = new CDocHostSite(pContainer, this);// 创建自己的控制站点实例
		return (*ppSite) ? TRUE : FALSE;
}

 BOOL CHtmlCtrl::CreateFromStatic(UINT nID, CWnd* pParent,CRect rc)
 {
	 if(rc.IsRectEmpty())
	 {
		 if(!CHtmlView::Create(NULL,                  // class name
			 NULL,                             // title
			 WS_CHILD,						   // style
			 rc,								// rectangle
			 pParent,                          // parent
			 nID,                              // control ID
			 NULL))  return FALSE;                            // frame/doc context not used	 
	 }
	 else
	 {
		 if(!CHtmlView::Create(NULL,                  // class name
			 NULL,                             // title
			 (WS_CHILD| WS_VISIBLE),						   // style
			 rc,								// rectangle
			 pParent,                          // parent
			 nID,                              // control ID
			 NULL))  return FALSE;                            // frame/doc context not used	 
	 }

	 m_IsView = FALSE;
	 return TRUE;
 } 

void CHtmlCtrl::SetHostInfoFlag(DWORD dwFlag)
{
	m_HostInfoFlag=dwFlag;
}


///////////////////////////////////////////////////////////////////////////////
////

 HRESULT CHtmlCtrl::OnShowContextMenu(DWORD dwID, LPPOINT ppt,LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved)
{
	return S_OK;
}

 HRESULT  CHtmlCtrl::OnTranslateAccelerator(LPMSG lpMsg,const GUID * pguidCmdGroup,DWORD nCmdID)
{
	 BOOL bShield=FALSE;
  if(lpMsg->message==WM_KEYDOWN)
 {
	 BOOL bCtrl=GetAsyncKeyState(VK_CONTROL)<0;
	 if(bCtrl)
	 {
		 switch(lpMsg->wParam)
		 {
		   case 0x41://A   全选  Ctrl+A
		   case 0x61://a   全选
			bShield=TRUE;
			break;
		   case 0x50://P  打印  Ctrl+P
		   case 0x70://p
			bShield=TRUE;
			break;
		   case 0x4f://O  Ctrl+o
		   case 0x6f://o
			bShield=TRUE;
			break;
		   case 0x4e://N Ctrl+N
		   case 0x6e://n
			bShield=TRUE;
			break;
		   case 0x53://S  Ctrl+S
		   case 0x73://s
			bShield=TRUE;
			break;
		   case 0x46://F Ctrl+F
		   case 0x66://f
			bShield=TRUE;
			break;
		   case VK_F5://F5 Ctrl+F5
			bShield=TRUE;
			break;
		   default:
			break;
		 }
	 }
  }
	
  if(bShield)	  return S_OK;

	return S_FALSE;
}



 HRESULT CHtmlCtrl::OnGetHostInfo(DOCHOSTUIINFO* pInfo)
{
	if(theApp.m_sCommandLine.m_bScroll)
		m_HostInfoFlag =DOCHOSTUIFLAG_THEME | DOCHOSTUIFLAG_NO3DBORDER;
	else 
		m_HostInfoFlag = DOCHOSTUIFLAG_THEME | DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_SCROLL_NO;

	pInfo->cbSize=sizeof(DOCHOSTUIINFO);
	pInfo->dwFlags=m_HostInfoFlag;
	pInfo->dwDoubleClick=DOCHOSTUIDBLCLK_DEFAULT;

	return S_OK;
}

void CHtmlCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
}


HRESULT CHtmlCtrl::OnGetExternal(LPDISPATCH *lppDispatch)
{
	*lppDispatch = GetIDispatch(TRUE);// 返回自身的IDispatch接口
	return S_OK;
}


////////////////////////////////////////////////////////////////////////////////
/////

const CString CHtmlCtrl::GetSystemErrorMessage(DWORD dwError)
{
	CString strError;
	LPTSTR lpBuffer;

	if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,dwError,MAKELANGID(LANG_NEUTRAL,SUBLANG_SYS_DEFAULT),(LPTSTR)&lpBuffer,0,NULL))
	{
		strError=_T("FormatMessage Netive Error");
	}
	else
	{
		strError=lpBuffer;
		LocalFree(lpBuffer);
	}
	return strError;
}

BOOL CHtmlCtrl::SetScriptDocument()
{
	LPDISPATCH pDisp=GetHtmlDocument();

	if(pDisp==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_spDoc=NULL;

	CComPtr<IDispatch> spDisp=pDisp;

	HRESULT hr=spDisp->QueryInterface(IID_IHTMLDocument2,(void**)&m_spDoc);

	if(FAILED(hr))	return FALSE;
	
	return TRUE;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
////////
void CHtmlCtrl::OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel)
{
	if(!m_bComplete)
	{
		_bstr_t sURL = _T("res://");	// 定位文件中的HTML资源
		TCHAR path[MAX_PATH] = {0};
		::GetModuleFileName(NULL,path,MAX_PATH);
		sURL += path;
		if(this->GetDlgCtrlID()==IE_EXPLOERER)
		{
			sURL += _T("/HTML_ERR");
			this->Navigate(sURL);
		}

	}

	CHtmlView::OnNavigateError(lpszURL, lpszFrame, dwError, pbCancel);
}


void CHtmlCtrl::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	if(GetDlgCtrlID()==IE_EXPLOERER)
	{
		if(!m_bComplete)			m_bComplete=FALSE;
	}
	else if(GetDlgCtrlID()==IE_BUFFER_EXPLOERE&&!theApp.m_sCommandLine.m_strAdUrl.IsEmpty())
	{
		if(m_bFirst)
		{
			GetParent()->PostMessage(IE_BUFFER_EXPLOERE);
			m_bFirst =	FALSE;
			return;
		}
	}

	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CHtmlCtrl::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel)
{
	if(theApp.m_sCommandLine.m_bShield&&GetDlgCtrlID()!=IE_BUFFER_EXPLOERE)
	{
		*ppDisp = ((CIeWnd*)this->GetParent())->m_htmlBuff.GetApplication();
		((CIeWnd*)this->GetParent())->m_htmlBuff.m_bFirst = TRUE;
	}
	else 
	{
		CHtmlView::OnNewWindow2(ppDisp, Cancel);
	}
}

BOOL CHtmlCtrl::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar)
{
	return CHtmlView::OnAmbientProperty(pSite, dispid, pvar);
}

BOOL CHtmlCtrl::PreTranslateMessage(MSG* pMsg)
{
	return CHtmlView::PreTranslateMessage(pMsg);
}

BOOL CHtmlCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return CHtmlView::OnWndMsg(message, wParam, lParam, pResult);
}
