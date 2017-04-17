// Apply.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "Header.h"
#include "ShowWnd.h"


#define MAX_TCHAR 256
// CApplyApp:
// 有关此类的实现，请参阅 Apply.cpp
//

typedef struct tagCOMMANDLINE
{
public:
    tagCOMMANDLINE()
	{
		m_strClassHandle.Empty();
		m_strName.Empty();
		m_strWeb.Empty();
		m_strIconLink.Empty();
		m_strEs.Empty();
		m_szSize=CSize(0,0);
		m_bScroll=FALSE;
		m_bShield=FALSE;
		m_strAdUrl.Empty();
	};
public:
	CString     m_strClassHandle,//类型句柄
			    m_strName,	//程序名
			    m_strWeb,  //导入什么网页
				m_strIconLink,		//  下载图标网页
				m_strEs;			//附加信息
	BOOL		m_bScroll;
	CSize       m_szSize;		 //窗体大小
	BOOL        m_bShield;
	CString     m_strAdUrl;
}CommandLine,*pCommandLine;


class CApplyApp  : public CWinApp
{
public:
	CApplyApp();

// 重写
	public:
	virtual BOOL InitInstance();
	BOOL Parse_Command();
	void Parse_String(LPCTSTR strVal);
	void StringToSize(CString strVal);
	BOOL OnlyOneExe();
	int OnGetUpdateFile(CString szURL, CString Filename);

	CString GetExeModulePath(HINSTANCE hInstance=NULL);
	BOOL JudgeDirectoryAndCreateMulDirectory(CString strPath);
	BOOL IsFileExist(CString strFile);

public:				
	CommandLine m_sCommandLine;
	CShowWnd pShowWnd;

public:
	CString m_strIcon;
	CString sURL;

	_bstr_t strURL;
// 实现
	DECLARE_MESSAGE_MAP()
};

extern CApplyApp theApp;