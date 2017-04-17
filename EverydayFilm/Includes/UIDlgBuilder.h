#ifndef __UIDLGBUILDER_H__
#define __UIDLGBUILDER_H__

#pragma once

class IDialogBuilderCallback
{
public:
    virtual CControlUI* CreateControl(LPCTSTR pstrClass) = 0;
	virtual CControlUI* ChangeControl(LPCTSTR pstrClass,CPaintManagerUI *pManager) = 0;
};


class CDialogBuilder
{
public:
    CControlUI* Create(STRINGorID skn ,STRINGorID xml, STRINGorID type = (UINT) 0, IDialogBuilderCallback* pCallback = NULL, CPaintManagerUI* pManager = NULL);

    void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const;
    void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const;
private:
    CControlUI* _Parse(CMarkupNode* parent, CControlUI* pParent = NULL, CPaintManagerUI* pManager = NULL);
	CControlUI* __Parse(CMarkupNode* pRoot, CControlUI* pParent = NULL, CPaintManagerUI* pManager = NULL);
    CMarkup m_xml;
    IDialogBuilderCallback* m_pCallback;
public:
	CControlUI* ChangeUI(STRINGorID skn ,STRINGorID xml, STRINGorID type, IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager);
	
};

#endif // __UIDLGBUILDER_H__
