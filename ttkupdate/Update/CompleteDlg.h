#pragma once
#include "SuperLink.h"

// CCompleteDlg 对话框

class CCompleteDlg : public CDialog
{
	DECLARE_DYNAMIC(CCompleteDlg)

public:
	CCompleteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCompleteDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_COMPLETE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	CSuperLink m_linkSite;
    CFont    m_Font;    

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
