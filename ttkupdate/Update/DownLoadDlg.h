#pragma once


// CDownLoadDlg 对话框

class CDownLoadDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownLoadDlg)

public:
	CDownLoadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownLoadDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


private:
	CFont    m_Font; 
	CString m_strUpTip;

public:
	CProgressCtrl* GetProgressCtrlDown();
	CListBox*	GetListBoxDown();
	CButton *   GetBtnAtOnce();
	CButton *   GetBtnTryAggin();
	CStatic *   GetStaticShow();


public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnBnClickedBtnTryAgain();
	afx_msg void OnBnClickedBtnLookCarefully();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
