#ifndef __UILIST_H__
#define __UILIST_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////////////
//

class CListHeaderUI;

#define UILIST_MAX_COLUMNS 32

typedef struct tagTListInfoUI
{
    int nColumns;
    RECT rcColumn[UILIST_MAX_COLUMNS];
    int nFont;
    UINT uTextStyle;
    RECT rcTextPadding;
    DWORD dwTextColor;
    DWORD dwBkColor;
    CStdString sImage;
    DWORD dwSelectedTextColor;
    DWORD dwSelectedBkColor;
    CStdString sSelectedImage;
    DWORD dwHotTextColor;
    DWORD dwHotBkColor;
    CStdString sHotImage;
    DWORD dwDisabledTextColor;
    DWORD dwDisabledBkColor;
    CStdString sDisabledImage;
    DWORD dwLineColor;
    bool bShowHtml;
    bool bExpandable;
    bool bMultiExpandable;
	bool bMultiSelect;
} TListInfoUI;

typedef struct tagMenuListInfoUI:public TListInfoUI
{
   CStdString sBkImage;
   CStdString sCheckImage;
   CStdString sNextImage;
} MenuListInfoUI;

struct MenuNode
{
	HWND m_thishWnd;
	struct MenuNode *nextMenu;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										 IMenuListOwnerUI  控件

class IMenuListOwnerUI				//列表框类
{
public:
    virtual MenuListInfoUI* GetListInfo() = 0;			//列表框信息
    virtual int GetCurSel() const = 0;				//获得当前选择节点
    virtual bool SelectItem(int iIndex) = 0;			//根据序号  选择节点
    virtual void Event(TEventUI& event) = 0;			//执行事件
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										 IMenuListItemUI  窗体
class IMenuListItemUI
{
public:
    virtual int GetIndex() const = 0;
    virtual void SetIndex(int iIndex) = 0;
    virtual IMenuListOwnerUI* GetOwner() = 0;
    virtual void SetOwner(CControlUI* pOwner) = 0;
    virtual bool IsSelected() const = 0;
    virtual bool Select(bool bSelect = true) = 0;
    virtual bool IsExpanded() const = 0;
    virtual bool Expand(bool bExpand = true) = 0;
    virtual void DrawItemText(HDC hDC, const RECT& rcItem) = 0;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class IListCallbackUI
{
public:
    virtual LPCTSTR GetItemText(CControlUI* pList, int iItem, int iSubItem) = 0;
};

class IListOwnerUI
{
public:
    virtual TListInfoUI* GetListInfo() = 0;
    virtual int GetCurSel() const = 0;
    virtual bool SelectItem(int iIndex) = 0;
    virtual void Event(TEventUI& event) = 0;
};

class IListUI : public IListOwnerUI
{
public:
    virtual CListHeaderUI* GetHeader() const = 0;
    virtual CContainerUI* GetList() const = 0;
    virtual IListCallbackUI* GetTextCallback() const = 0;
    virtual void SetTextCallback(IListCallbackUI* pCallback) = 0;
    virtual bool ExpandItem(int iIndex, bool bExpand = true) = 0;
    virtual int GetExpandedItem() const = 0;
	virtual bool SelectItems(int iIndex) = 0;
};

class IListItemUI
{
public:
    virtual int GetIndex() const = 0;
    virtual void SetIndex(int iIndex) = 0;
    virtual IListOwnerUI* GetOwner() = 0;
    virtual void SetOwner(CControlUI* pOwner) = 0;
    virtual bool IsSelected() const = 0;
    virtual bool Select(bool bSelect = true) = 0;
    virtual bool IsExpanded() const = 0;
    virtual bool Expand(bool bExpand = true) = 0;
    virtual void DrawItemText(HDC hDC, const RECT& rcItem) = 0;
	virtual bool SelectMutli(bool bSelect = true) = 0;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CListBodyUI;
class CListHeaderUI;

class  CListUI : public CVerticalLayoutUI, public IListUI
{
public:
    CListUI();

    LPCTSTR GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    int GetCurSel() const;
    bool SelectItem(int iIndex);
	bool SelectItems(int iIndex);
	bool ClearSelects();

    CListHeaderUI* GetHeader() const;  
    CContainerUI* GetList() const;
    TListInfoUI* GetListInfo();

    CControlUI* GetItemAt(int iIndex) const;
    int GetItemIndex(CControlUI* pControl) const;
    bool SetItemIndex(CControlUI* pControl, int iIndex);
    int GetCount() const;
	int GetSelectedCount() const;
    bool Add(CControlUI* pControl);
    bool AddAt(CControlUI* pControl, int iIndex);
    bool Remove(CControlUI* pControl);
    bool RemoveAt(int iIndex);
    void RemoveAll();

    void EnsureVisible(int iIndex,int iShowIndex = -1);//iShowIndex表示需要显示在什么index的位置上
    void Scroll(int dx, int dy);

    int GetChildPadding() const;
    void SetChildPadding(int iPadding);

    void SetItemFont(int index);
    void SetItemTextStyle(UINT uStyle);
    void SetItemTextPadding(RECT rc);
    void SetItemTextColor(DWORD dwTextColor);
    void SetItemBkColor(DWORD dwBkColor);
    void SetItemImage(LPCTSTR pStrImage);
    void SetSelectedItemTextColor(DWORD dwTextColor);
    void SetSelectedItemBkColor(DWORD dwBkColor);
    void SetSelectedItemImage(LPCTSTR pStrImage); 
    void SetHotItemTextColor(DWORD dwTextColor);
    void SetHotItemBkColor(DWORD dwBkColor);
    void SetHotItemImage(LPCTSTR pStrImage);
    void SetDisabledItemTextColor(DWORD dwTextColor);
    void SetDisabledItemBkColor(DWORD dwBkColor);
    void SetDisabledItemImage(LPCTSTR pStrImage);
    void SetItemLineColor(DWORD dwLineColor);
    bool IsItemShowHtml();
    void SetItemShowHtml(bool bShowHtml = true);
	void DoPaint(HDC hDC, const RECT& rcPaint);

    void SetExpanding(bool bExpandable);
    void SetMultiExpanding(bool bMultiExpandable); 
	void SetMultiSelect(bool bMultiSelect);
    int GetExpandedItem() const;
    bool ExpandItem(int iIndex, bool bExpand = true);

    void SetPos(RECT rc);
    void Event(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    IListCallbackUI* GetTextCallback() const;
    void SetTextCallback(IListCallbackUI* pCallback);

    SIZE GetScrollPos() const;
    SIZE GetScrollRange() const;
    void SetScrollPos(SIZE szPos);
    void LineUp();
    void LineDown();
    void PageUp();
    void PageDown();
    void HomeUp();
    void EndDown();
    void LineLeft();
    void LineRight();
    void PageLeft();
    void PageRight();
    void HomeLeft();
    void EndRight();
    void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
    virtual CScrollbarUI* GetVerticalScrollbar() const;
    virtual CScrollbarUI* GetHorizontalScrollbar() const;

protected:
    int m_iCurSel;
	int m_iEnsure;
	int m_iShowIndex;
    int m_iExpandedItem;
    IListCallbackUI* m_pCallback;
    CListBodyUI* m_pList;
    CListHeaderUI* m_pHeader;
    TListInfoUI m_ListInfo;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class  CListBodyUI : public CVerticalLayoutUI
{
public:
	CListBodyUI(CListUI* pOwner);

	void SetScrollPos(SIZE szPos);
	void SetPos(RECT rc);
	LPCTSTR GetClass() const
	{
		return _T("ListBodyUI");
	}

protected:
    CListUI* m_pOwner;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class  CListHeaderUI : public CHorizontalLayoutUI
{
public:
    CListHeaderUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    SIZE EstimateSize(SIZE szAvailable);
};


/////////////////////////////////////////////////////////////////////////////////////
//

class  CListHeaderItemUI : public CControlUI
{
public:
    CListHeaderItemUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    void SetEnabled(bool bEnable = true);

    void SetDragable(bool bDragable);
    void SetSepWidth(int iWidth);
    void SetTextStyle(UINT uStyle);
    void SetTextColor(DWORD dwTextColor);
    void SetFont(int index);
    bool IsShowHtml();
    void SetShowHtml(bool bShowHtml = true);
    LPCTSTR GetNormalImage();
    void SetNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetHotImage();
    void SetHotImage(LPCTSTR pStrImage);
    LPCTSTR GetPushedImage();
    void SetPushedImage(LPCTSTR pStrImage);
    LPCTSTR GetFocusedImage();
    void SetFocusedImage(LPCTSTR pStrImage);
    LPCTSTR GetSepImage() const;
    void SetSepImage(LPCTSTR pStrImage);

    void Event(TEventUI& event);
    SIZE EstimateSize(SIZE szAvailable);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    RECT GetThumbRect() const;

    void PaintText(HDC hDC);
    void PaintStatusImage(HDC hDC);

protected:
    POINT ptLastMouse;
    bool m_bDragable;
    UINT m_uButtonState;
    int m_iSepWidth;
    DWORD m_dwTextColor;
    int m_iFont;
    UINT m_uTextStyle;
    bool m_bShowHtml;
    CStdString m_sNormalImage;
    CStdString m_sHotImage;
    CStdString m_sPushedImage;
    CStdString m_sFocusedImage;
    CStdString m_sSepImage;
    CStdString m_sSepImageModify;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class  CListElementUI : public CControlUI, public IListItemUI
{
public:
    CListElementUI();

    LPCTSTR GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetEnabled(bool bEnable = true);

    int GetIndex() const;
    void SetIndex(int iIndex);

    IListOwnerUI* GetOwner();
    void SetOwner(CControlUI* pOwner);
    void SetVisible(bool bVisible = true);

    bool IsSelected() const;
    bool Select(bool bSelect = true);
	bool SelectMutli(bool bSelect = true);
    bool IsExpanded() const;
    bool Expand(bool bExpand = true);

    void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
    bool Activate();

    void Event(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void DrawItemBk(HDC hDC, const RECT& rcItem);

protected:
    int m_iIndex;
    bool m_bSelected;
    UINT m_uButtonState;
    IListOwnerUI* m_pOwner;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class  CListLabelElementUI : public CListElementUI
{
public:
    CListLabelElementUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void Event(TEventUI& event);
    SIZE EstimateSize(SIZE szAvailable);
    void DoPaint(HDC hDC, const RECT& rcPaint);

    void DrawItemText(HDC hDC, const RECT& rcItem);
};


/////////////////////////////////////////////////////////////////////////////////////
//

class  CListTextElementUI : public CListLabelElementUI
{
public:
    CListTextElementUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    void SetOwner(CControlUI* pOwner);
    CStdString* GetLinkContent(int iIndex);

    void Event(TEventUI& event);
    SIZE EstimateSize(SIZE szAvailable);

    void DrawItemText(HDC hDC, const RECT& rcItem);

protected:
    enum { MAX_LINK = 8 };
    int m_nLinks;
    RECT m_rcLinks[MAX_LINK];
    CStdString m_sLinks[MAX_LINK];
    int m_nHoverLink;
    IListUI* m_pOwner;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class  CListExpandElementUI : public CListTextElementUI
{
public:
    CListExpandElementUI();
    virtual ~CListExpandElementUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetVisible(bool bVisible = true);
    void SetInternVisible(bool bVisible = true);
    void SetMouseEnabled(bool bEnable = true);

    void SetPos(RECT rc);
    void Event(TEventUI& event);
    SIZE EstimateSize(SIZE szAvailable);
    void DoPaint(HDC hDC, const RECT& rcPaint);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void SetManager(CPaintManagerUI* pManager, CControlUI* pParent);
    CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

    bool IsExpanded() const;
    bool Expand(bool bExpand = true);
    bool GetExpandHideSelf() const;
    void SetExpandHideSelf(bool bHideSelf);
    RECT GetExpanderRect() const;
    void SetExpanderRect(RECT rc);
    void SetExpandItem(CControlUI* pControl);

    void DrawItemText(HDC hDC, const RECT& rcItem);

protected:
    bool m_bExpanded;
    bool m_bHideSelf;
    RECT m_rcExpander;
    CControlUI* m_pSubControl;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class  CListContainerElementUI : public CContainerUI, public IListItemUI
{
public:
    CListContainerElementUI();

    LPCTSTR GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    int GetIndex() const;
    void SetIndex(int iIndex);

    IListOwnerUI* GetOwner();
    void SetOwner(CControlUI* pOwner);
    void SetVisible(bool bVisible = true);
    void SetEnabled(bool bEnable = true);

    bool IsSelected() const;
    bool Select(bool bSelect = true);
    bool IsExpanded() const;
    bool Expand(bool bExpand = true);

    void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
    bool Activate();

    void Event(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    void DoPaint(HDC hDC, const RECT& rcPaint);

    void DrawItemText(HDC hDC, const RECT& rcItem);    

    void DrawItemBk(HDC hDC, const RECT& rcItem);
	bool SelectMutli(bool bSelect = true);

protected:
    int m_iIndex;
    bool m_bSelected;
    UINT m_uButtonState;
    IListOwnerUI* m_pOwner;
};

#endif // __UILIST_H__
