#pragma once

#include <vector>
#include <atlstr.h>

class CMyButton:public CButtonUI
{
	LPCTSTR GetClass() const
	{
		return _T("ListButton");
	}

	void Event(TEventUI& event)
	{
		if( event.Type == UIEVENT_KEYDOWN && IsEnabled() )
		{
			return ;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
				Invalidate();
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();
			}
			return;
		}
		CButtonUI::Event(event);
	}
};

class CMyListElement:public CListContainerElementUI
{
	CHorizontalLayoutUI *pCtner,*pLeft,*pRight;
	CTextUI *ctText;
public:
	CMyButton *btnClose;
	CMyListElement()
	{
		pCtner = new CHorizontalLayoutUI;
		pLeft = new CHorizontalLayoutUI;
		pRight = new CHorizontalLayoutUI;
		btnClose = new CMyButton;
		ctText = new CTextUI;
		//btnClose.set
		btnClose->SetVisible(false);
		btnClose->SetNormalImage(_T("file='menu.png' source='32,390,39,397'"));
		btnClose->SetHotImage(_T("file='menu.png' source='39,390,46,397'"));
		btnClose->SetPushedImage(_T("file='menu.png' source='46,390,53,397'"));
		Add(pCtner);
		pCtner->Add(pLeft);
		pCtner->Add(pRight);
		pRight->Add(ctText);
		pRight->Add(btnClose);
		pCtner->SetMouseEnabled(false);
		pLeft->SetMouseEnabled(false);
		pRight->SetMouseEnabled(false);
		btnClose->SetMaxHeight(7);
		btnClose->SetMaxWidth(7);
		pRight->SetFixedWidth(60);
		ctText->SetMinWidth(48);
		ctText->SetTextColor(0xFFFFFFFF);
		ctText->SetMouseEnabled(false);
		RECT rc = {0,8,0,0};
		pRight->SetPadding(rc);
	}

	void SetStateText(LPCTSTR strText)
	{
		ctText->SetText(strText);
	}

	LPCTSTR GetClass() const
	{
		return _T("myelement");
	}

	void DoPaint(HDC hDC, const RECT& rcPaint)
	{
		if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
		DrawItemBk(hDC, m_rcItem);
		DrawItemText(hDC, m_rcItem);
		CContainerUI::DoPaint(hDC, rcPaint);
	}

	void Event(TEventUI& event)
	{	
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( IsEnabled() ) {
				m_uButtonState |= UISTATE_HOT;
				btnClose->SetVisible();
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( (m_uButtonState & UISTATE_HOT) != 0 ) {
				if(!::PtInRect(&btnClose->GetPos(),event.ptMouse))//the mouse not in the close btn
				{
					m_uButtonState &= ~UISTATE_HOT;
					btnClose->SetVisible(false);
					Invalidate();
				}
			}
			return;
		}
		CListContainerElementUI::Event(event);
	}

	void DrawItemText(HDC hDC, const RECT& rcItem)
	{
		if( m_sText.IsEmpty() ) return;

		if( m_pOwner == NULL ) return;
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		DWORD iTextColor = pInfo->dwTextColor;
		if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			iTextColor = pInfo->dwHotTextColor;
		}
		if( IsSelected() ) {
			iTextColor = pInfo->dwSelectedTextColor;
		}
		if( !IsEnabled() ) {
			iTextColor = pInfo->dwDisabledTextColor;
		}
		int nLinks = 0;
		RECT rcText = rcItem;
		rcText.left += pInfo->rcTextPadding.left;
		rcText.right -= pInfo->rcTextPadding.right+60;
		rcText.top += pInfo->rcTextPadding.top;
		rcText.bottom -= pInfo->rcTextPadding.bottom;

		if( pInfo->bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rcText, m_sText, iTextColor, \
			NULL, NULL, nLinks, DT_SINGLELINE | DT_END_ELLIPSIS |pInfo->uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rcText, m_sText, iTextColor, \
			pInfo->nFont, DT_SINGLELINE | DT_END_ELLIPSIS |pInfo->uTextStyle);
	}
};

class CMyList : public CListUI
{
public:
	struct NodeData
	{
		int _level;
		bool _child_visible;
		bool IsDowning;
		CString _text;
		CString _url;
		CStringA taskid;
		float downpos;
		CMyListElement* _pListElement;
	};

	class Node
	{
		typedef std::vector <Node*>	Children;
		Children	_children;
		Node*		_parent;
		NodeData    _data;

	private:
		void set_parent(Node* parent) { _parent = parent; }

	public:
		Node() : _parent (NULL) {}
		explicit Node(NodeData t) : _data (t), _parent (NULL) {}
		Node(NodeData t, Node* parent)	: _data (t), _parent (parent) {}
		~Node() 
		{
			for (int i = 0; i < num_children(); i++)
				delete _children[i]; 
		}
		NodeData& data() { return _data; }	
		int num_children() const { return _children.size(); }
		Node* child(int i)	{ return _children[i]; }
		Node* parent() { return ( _parent);	}
		bool has_children() const {	return num_children() > 0; }
		void add_child(Node* child) 
		{
			child->set_parent(this); 
			_children.push_back(child); 
		}
		void remove_child(Node* child)
		{
			Children::iterator iter = _children.begin();
			for( ; iter < _children.end(); ++iter )
			{
				if( *iter == child ) 
				{
					_children.erase(iter);
					return;
				}
			}
		}
		void remove_all_child()
		{
			_children.clear();
		}
		Node* get_last_child()
		{
			if( has_children() )
			{
				return child(num_children() - 1)->get_last_child();
			}
			else return this;
		}
	};	

	CMyList();

	~CMyList();

	bool Add(CControlUI* pControl);

	bool AddAt(CControlUI* pControl, int iIndex);

	bool Remove(CControlUI* pControl);

	bool RemoveAt(int iIndex);

	void RemoveAll();
	Node* GetRoot();

	Node* AddNode(LPCTSTR name,LPCTSTR url,char *taskid = NULL , Node* parent = NULL);

	void RemoveNode(Node* node);

	void SetChildVisible(Node* node, bool visible);

	SIZE GetExpanderSizeX(Node* node) const;

	void Event(TEventUI& event);

private:
	Node* _root;
};

class CListBuilder : public IDialogBuilderCallback
{
public:
	CControlUI* CreateControl(LPCTSTR pstrClass) 
	{
		if( _tcscmp(pstrClass, _T("Mylist")) == 0 )
			return new CMyList;
		else
			return NULL;
	}
	CControlUI *ChangeControl(LPCTSTR pstrClass,CPaintManagerUI *pManager)
	{
		return NULL;
	}
};
