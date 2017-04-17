#pragma once


// CIntroductionDlg 对话框

class CIntroductionDlg : public CDialog
{
	DECLARE_DYNAMIC(CIntroductionDlg)

public:
	CIntroductionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIntroductionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INTRODUCTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnCancel();
};
