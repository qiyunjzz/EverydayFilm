#ifndef HTMLCTRL_H
#define HTMLCTRL_H
#pragma once

#include "afxhtml.h"
#include <comdef.h>
#include <afxocc.h>
#include "DocHostSite.h"


class CHtmlCtrl:
	public CHtmlView
{
	DECLARE_DYNCREATE(CHtmlCtrl)
	DECLARE_DISPATCH_MAP()
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
protected:
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);

//////////////////////////////////////////////////////////////////////////
public:
	CHtmlCtrl(BOOL bShow=TRUE);
	~CHtmlCtrl(void);

	virtual void PostNcDestroy();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);

	virtual BOOL CreateControlSite(COleControlContainer* pContainer,COleControlSite** ppSite, UINT nID, REFCLSID clsid);	
	
	virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO* pInfo);
	
	virtual HRESULT OnGetExternal(LPDISPATCH *lppDispatch);

	virtual HRESULT OnShowContextMenu(DWORD dwID, LPPOINT ppt,LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved);

	 virtual HRESULT OnTranslateAccelerator(LPMSG lpMsg,const GUID * pguidCmdGroup,DWORD nCmdID);

//////////////////////////////////////////////////////////////////////
public:
	BOOL CreateFromStatic(UINT nID, CWnd* pParent=NULL,CRect rc=CRect(0,0,0,0));

	BOOL Create(CRect rect,CWnd* pParent,UINT nID);

	void SetHostInfoFlag(DWORD dwFlag);

	void OnNewWindow3( LPDISPATCH* ppDisp, BOOL* Cancel, DWORD dwFlags, LPCTSTR bstrUrlContext, LPCTSTR bstrUrl );

	BOOL CallJS(CString strFunc, CString strParam, _variant_t* pVar);

	BOOL GetJScript(CComPtr<IDispatch>& spDisp);

protected:
	const CString GetSystemErrorMessage(DWORD dwError);

	BOOL SetScriptDocument();


public:


///////////////////////////////////////////////////////////////////////////


private:
	DWORD m_HostInfoFlag;
	BOOL m_IsView;

	CComPtr<IHTMLDocument2> m_spDoc;

	BOOL m_bComplete;

public:
	BOOL m_bFirst;
public:
	virtual void OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	virtual BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};



#endif