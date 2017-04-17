#pragma once
#include "vector"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										 CMenuUI  菜单控件
class IMenuListOwnerUI;
class CMenuWnd;

class  CMenuUI:
	public CContainerUI,
	public IMenuListOwnerUI
{
	    friend CMenuWnd;							//友元类
public:
    CMenuUI();

    LPCTSTR GetClass() const;					//获得类名
    LPVOID GetInterface(LPCTSTR pstrName);		//获得接口
	void initMenuLayout();


    UINT GetControlFlags() const;				//控件标志

    CStdString GetText() const;					//控件内容
    void SetEnabled(bool bEnable = true);		//设置活动与否

    CStdString GetDropBoxAttributeList();			//得到拖动属性列表
    void SetDropBoxAttributeList(LPCTSTR pstrList);	//设置拖动属性列表
    SIZE GetDropBoxSize() const;					//得到拖动箱子大小
    void SetDropBoxSize(SIZE szDropBox);			//设置拖动箱子的大小

    int GetCurSel() const;							//得到鼠标选择项
    bool SelectItem(int iIndex);					//选择项

    bool SetItemIndex(CControlUI* pControl, int iIndex);	//设置选择项的序号
    bool Add(CControlUI* pControl);							//添加控件
    bool AddAt(CControlUI* pControl, int iIndex);			//控件定位
    bool Remove(CControlUI* pControl);						//移除
    bool RemoveAt(int iIndex);								//移除指定位置
    void RemoveAll();										//移除全部

	bool Activate();										//激活主菜单
	bool ActivateNextMenu(POINT pt,HWND hWndPare,RECT eventrc);								//根据上次菜单的数据 激活下级菜单


	MenuListInfoUI* GetListInfo();								//列表信息
	void SetItemFont(int index);							//设置节点字体
    void SetItemTextStyle(UINT uStyle);						//设置节点文本类型
    void SetItemTextPadding(RECT rc);						//设置节点文本位置
    void SetItemTextColor(DWORD dwTextColor);				//设置节点文本的颜色
    void SetItemBkColor(DWORD dwBkColor);					//设置节点的背景颜色
	void SetItemBkImage(LPCTSTR pStrImage);
	void SetItemCheckImage(LPCTSTR pStrImage);
	void SetItemNextImage(LPCTSTR pStrImage);
	LPCTSTR GetItemNextImage();
	LPCTSTR GetItemBkImage();
    void SetItemImage(LPCTSTR pStrImage);					//设置节点的图片
    void SetSelectedItemTextColor(DWORD dwTextColor);		//设置选中节点文本背景颜色
    void SetSelectedItemBkColor(DWORD dwBkColor);			//设置选中节点背景颜色
    void SetSelectedItemImage(LPCTSTR pStrImage);			//设置选中节点图片
    void SetHotItemTextColor(DWORD dwTextColor);			//设置最新节点文本颜色
    void SetHotItemBkColor(DWORD dwBkColor);				//设置最新。。。
    void SetHotItemImage(LPCTSTR pStrImage);
    void SetDisabledItemTextColor(DWORD dwTextColor);
    void SetDisabledItemBkColor(DWORD dwBkColor);
    void SetDisabledItemImage(LPCTSTR pStrImage);
    void SetItemLineColor(DWORD dwLineColor);				//设置节点线的颜色
    bool IsItemShowHtml();									//判断是否显示html
    void SetItemShowHtml(bool bShowHtml = true);			//设置节点显示

    SIZE EstimateSize(SIZE szAvailable);					//估计大小
    void SetPos(RECT rc);									//设置大小区域
    void Event(TEventUI& event);							//定义事件
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);	//设置属性

	void DestroyMenu();
	BOOL EnableMenuItem(int labelId,BOOL pValue);						//使选择项恢复
	BOOL CheckMenuItem(int index,BOOL pValue);
	int GetMenuItemIndex(LPCTSTR pstrName);
	//void UnEnableMenuItem(int index);					//选择项不可用
	//void SetMarkCheckOk(BOOL pValue,int index);			//设置复选框  读取文件设置复选框标识
	//void SetMarkCheckCancel(BOOL pValue,int index);
    void DoPaint(HDC hDC, const RECT& rcPaint);				//控件绘画

	BOOL ModifyMenuUI(int labelId,UINT nFlags,LPCTSTR lpszNewItem = NULL );
	
	void SetMenuWidth(LPCTSTR pValue);
	int GetMenuWidth();
	SIZE GetMenuCorner();
    void SetMenuCorner(int cx, int cy);

public:
    CMenuWnd* m_pWindow;			//定义一个窗口对象
	CVerticalLayoutUI* m_pMenuLayout;
	static std::vector<HWND> hWndList;
private:
	HWND hWndParent;				//主菜单的句柄
	int MenuWidth;
    SIZE RoundCorner;
    int m_iCurSel;					//当前选择的序号
    RECT m_rcTextPadding;			//文本的位置区域
    CStdString m_sDropBoxAttributes;	//字符串 拖动箱子属性
    SIZE m_szDropBox;					//拖动箱子大小

    MenuListInfoUI m_ListInfo;				//列表框的信息		TListInfoUI为结构
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										 CMenuWnd   窗体
class CMenuWnd:
	public CWindowWnd
{
public:
	CMenuWnd();
	void Init(CMenuUI* pOwner,POINT pt);
	void InitNextMenu(CMenuUI* pOwner,POINT pt,HWND hWndPare,RECT eventrc);
    LPCTSTR GetWindowClassName() const;
	HWND GetMainWindowHwnd();
    void OnFinalMessage(HWND hWnd);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    //HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
    //HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
  
private:
	CPaintManagerUI m_pm;
	CMenuUI* m_pOwner;
	CMenuUI* m_pEventOwner;
	RECT rcshow;
public:
	CVerticalLayoutUI* m_pLayout;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										 CMenuListElementUI  窗体
class CMenuListElementUI : public CControlUI, public IMenuListItemUI
{
public:
    CMenuListElementUI();

    LPCTSTR GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetEnabled(bool bEnable = true);

    int GetIndex() const;
    void SetIndex(int iIndex);

    IMenuListOwnerUI* GetOwner();
    void SetOwner(CControlUI* pOwner);
    void SetVisible(bool bVisible = true);
	void SetButtonState(UINT state);

    bool IsSelected() const;
    bool Select(bool bSelect = true);
    bool IsExpanded() const;
    bool Expand(bool bExpand = true);

    void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
    bool Activate();

    void Event(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void DrawItemBk(HDC hDC, const RECT& rcItem);
	CStdString LabelName;			//菜单项目名字
	int LabelId;
	CStdString NextMenu;			//下级菜单   子菜单
	BOOL LabelMarkCheck;			//设置复选 判断
	CStdString NextMenuMark;		//下级菜单图标  标志

protected:
    int m_iIndex;
    bool m_bSelected;
    UINT m_uButtonState;
    IMenuListOwnerUI* m_pOwner;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										 CMenuListLabelUI  窗体
class CMenuListLabelUI:
      public CMenuListElementUI
{
public:
	CMenuListLabelUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);


	void Event(TEventUI& event);
	SIZE EstimateSize(SIZE szAvailable);
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void DrawItemText(HDC hDC, const RECT& rcItem);
	void SetNextMenu(LPCTSTR pstrValue);
	void SetLabelName(LPCTSTR pstrValue);
	CStdString GetLabelName();
	CStdString GetLabelMark();
	void SetLabelId(LPCTSTR pstrValue);
	int GetLabelId();
	void SetLabelMarkCheck(BOOL pstrValue);
	BOOL GetLabelMarkCheck();
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);	//设置属性
	CStdString GetNextMenu();

private:

};



