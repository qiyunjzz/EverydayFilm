#pragma once
#include "SuperLink.h"

// CNewDlg 对话框

class CNewDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewDlg)

public:
	CNewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NEW };

private:
//	CSuperLink m_linkSite;
	CBitmap m_bmpBk;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnOk();
};
