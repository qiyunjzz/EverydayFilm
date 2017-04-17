#pragma once
#include "Register.h"
#include <atlstr.h>

#define WM_REG_FILE WM_USER+112


class CSetDialog : public CWindowWnd, public INotifyUI
{
public:
	CSetDialog() {m_pm = new CPaintManagerUI;};
	LPCTSTR GetWindowClassName() const { return _T("SettingDialog"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/); 
	void Init();
	//œ‘ æ÷Æ«∞
	void OnPrepare()
	{

	}
	void Notify(TNotifyUI& msg);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);;
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMoveWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnRegFile(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled);	
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool IsRegistered(CString ext,CString exe,CString icon);
	bool DelRegister(CString ext);
	bool RegisterExt(CString ext,CString exe,CString icon);

	void ReadIni();
	CStdString BrowseFolder(HWND hWnd, LPCTSTR lpTitle);
	CPaintManagerUI* m_pm;
	COptionUI*  m_btnAbout;
	COptionUI*  m_btnPower;
	CButtonUI*	m_btnClose;
	CButtonUI*  m_btnChangePath,*m_btnChangePicpath;
	COptionUI*  m_btnPicSet;
	COptionUI*  m_btnPathSet;

	CButtonUI*  m_btnSure;
	CButtonUI*  m_btnGiveup;
	CButtonUI*  m_btnallFileAssociations,*m_btnnoFileAssociations,*m_tbndefaultFileAssociations;
	CStdString skn;
	CStdString m_cfgPath,m_qvodiniPath,m_strDesttop;
	CString m_strExePath,m_strIcoPath;

	//CContainerUI* pFileAssociationsContainer;
	IMMPlayer* m_playerSet;


	CSliderUI *pbrightness,*saturation,*contrast,*hue;
	CButtonUI *suavity,*bright,*restore;
	int oldhue,oldcontrast,oldbrightness,oldsaturation;
	int bAlwaysAssociation;
};
