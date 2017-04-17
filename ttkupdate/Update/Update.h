// Update.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "UpdateDlg.h"
#include "DownLoadControl.h"


// CUpdateApp:
// 有关此类的实现，请参阅 Update.cpp
//

class CUpdateApp : public CWinApp
{
public:
	enum enDownMode{DL_NORMAL=0,DL_FORCE,DL_CHECK};	

public:
	CUpdateApp();
	BOOL OnlyOneExe();
	void OnInitFile();
	int OnGetUpdateFile(CString strUrl);
	int OnGetRunMode();
	BOOL OnRunExe();
	BOOL GetFileExist(CString strFile);
	CString GetExeVision(CString strExe);
	BOOL CompareVision(CString strUpdate,CString strExe);
	BOOL IsUpdateNow();  
	CString GetExeModulePath();

// 重写
	public:
	virtual BOOL InitInstance();

public:
	CString m_strFile,m_strFileBuffer;
	CString m_strUrl;
	CString m_strFilePath,m_strFileName,m_strExePath;
	CString m_strFileVision;
	CDownLoadControl m_downControl;
	enDownMode  m_enDlState;

	BOOL m_bDownComplete;
// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CUpdateApp theApp;