#include "StdAfx.h"
#include "MyList.h"

CMyList::CMyList(void) : _root(NULL)
{
	SetItemShowHtml(true);

	_root = new Node;
	_root->data()._level = -1;
	_root->data()._child_visible = true;
	_root->data()._pListElement = NULL;
}

CMyList::~CMyList(void)
{
	if(_root) 
		delete _root;
}
bool CMyList::Add(CControlUI* pControl)
{
	if( !pControl ) return false;
	if( _tcscmp(pControl->GetClass(), _T("myelement")) == 0 ) return false;
	return CListUI::Add(pControl);
}

bool CMyList::AddAt(CControlUI* pControl, int iIndex)
{
	if( !pControl ) return false;
	if( _tcscmp(pControl->GetClass(), _T("myelement")) == 0 ) return false;
	return CListUI::AddAt(pControl, iIndex);
}

bool CMyList::Remove(CControlUI* pControl)
{
	if( !pControl ) return false;
	if( _tcscmp(pControl->GetClass(), _T("myelement")) == 0 ) return false;
	return CListUI::Remove(pControl);
}

bool CMyList::RemoveAt(int iIndex)
{
	CControlUI* pControl = GetItemAt(iIndex);
	if( !pControl ) return false;
	if( _tcscmp(pControl->GetClass(), _T("myelement")) == 0 ) return false;
	return CListUI::RemoveAt(iIndex);
}

void CMyList::RemoveAll()
{
	CListUI::RemoveAll();
	delete _root;
	_root = new Node;
	_root->data()._level = -1;
	_root->data()._child_visible = true;
	_root->data()._pListElement = NULL;
}

CMyList::Node* CMyList::GetRoot()
{ 
	return _root;
}

CMyList::Node* CMyList::AddNode(LPCTSTR name,LPCTSTR url,char *taskid, Node* parent)
{
	if( !parent ) parent = _root;

	//判断是否存在，存在则不能重复加入
	if(parent == _root)
	{
		int num = _root->num_children();
		for(int i = 0 ;i<num;i++)
		{
			Node *tmp = _root->child(i);
			_bstr_t a = tmp->data()._text,b = name;
			if(a == b)//找到相同的
				return tmp;
		}
	}
	else
	{
		int num = parent->num_children();
		for(int i = 0 ;i<num;i++)
		{
			Node *tmp = parent->child(i);
			_bstr_t a = tmp->data()._url;
			_bstr_t b = url;
			if(a == b)//找到相同的
				return tmp;
		}
	}

	CMyListElement* pListElement = new CMyListElement;//new item
	Node* node = new Node;//new node
	node->data()._level = parent->data()._level + 1;//深度为父节点深度加一
	if( node->data()._level == 0 ) node->data()._child_visible = true;//如果父节点为root
	else node->data()._child_visible = false;//如果父节点不为root
	node->data().IsDowning = FALSE;
	node->data()._text = name;
	node->data()._url = url;
	node->data().taskid = taskid;
	node->data().downpos = 0.0;
	node->data()._pListElement = pListElement;

	if( !parent->data()._child_visible )
	{
		pListElement->SetVisible(false);
	}
	if( parent != _root && !parent->data()._pListElement->IsVisible() )
	{
		pListElement->SetVisible(false);
	}

	CStdString html_text;
	html_text += _T("<x 6>");
	for( int i = 0; i < node->data()._level; ++i )
	{
		html_text += _T("<x 24>");
	}
	if( node->data()._level < 1 ) {
		if( node->data()._child_visible ) html_text += _T("<a><i tree_expand.png 1></a> ");
		else html_text += _T("<a><i tree_expand.png 0></a> ");
	}
	//if(node->data()._text.GetLength() > 10 )
	//{
	//	html_text += node->data()._text.Left(10);
	//	html_text += _T("…");
	//}
	//else
		html_text += node->data()._text;
	pListElement->SetText(html_text);
	pListElement->SetTag((UINT_PTR)node);
	pListElement->SetFixedHeight(28);
	pListElement->SetAttribute(_T("tooltip"),node->data()._text);
	//if( node->data()._level == 0 ) pListElement->SetFixedHeight(28);
	//else pListElement->SetFixedHeight(24);
	
	//if( node->data()._level == 0 ) {
	//	pListElement->SetBkImage(_T("file='GameSkin\\tree_top.png' fade='100'"));
	//}

	int index = 0;
	if( parent->has_children() )
	{
		Node* prev = parent->get_last_child();
		index = prev->data()._pListElement->GetIndex() + 1;
	}
	else 
	{
		if( parent == _root ) index = 0;
		else index = parent->data()._pListElement->GetIndex() + 1;
	}
	if( !CListUI::AddAt(pListElement, index) ) {
		delete pListElement;
		delete node;
		node = NULL;
	}

	parent->add_child(node);
	return node;
}

void CMyList::RemoveNode(Node* node)
{
	if( !node || node == _root ) return;

	while(0 != node->num_children())
	{
		Node* child = node->child(0);
		RemoveNode(child);
	}

	if(m_pManager->GetFocus() == node->data()._pListElement || 
		m_pManager->GetFocus() == node->data()._pListElement->btnClose)
	{
		m_pManager->SetFocus(this);
	}
	CListUI::Remove(node->data()._pListElement);
//	delete node->data()._pListElement; 父层如果设置了auto 则自动会释放 
	node->parent()->remove_child(node);
	delete node;
}

void CMyList::SetChildVisible(Node* node, bool visible)
{
	if( !node || node == _root ) return;

	if( node->data()._child_visible == visible ) return;
	node->data()._child_visible = visible;

	CStdString html_text;
	html_text += _T("<x 6>");
	for( int i = 0; i < node->data()._level; ++i )
	{
		html_text += _T("<x 24>");
	}
	if( node->data()._level < 1 ) {
		if( node->data()._child_visible ) html_text += _T("<a><i tree_expand.png 1></a> ");
		else html_text += _T("<a><i tree_expand.png 0></a> ");
	}
	//if(node->data()._text.GetLength() > 10 )
	//{
	//	html_text += node->data()._text.Left(10);
	//	html_text += _T("…");
	//}
	//else
		html_text += node->data()._text;
	node->data()._pListElement->SetText(html_text);

	if( !node->data()._pListElement->IsVisible() ) return;
	if( !node->has_children() ) return;

	Node* begin = node->child(0);
	Node* end = node->get_last_child();
	for( int i = begin->data()._pListElement->GetIndex(); i <= end->data()._pListElement->GetIndex(); ++i )
	{
		CControlUI* control = GetItemAt(i);
		if( _tcscmp(control->GetClass(), _T("myelement")) == 0 )
		{
			if( !visible ) 
			{
				control->SetVisible(false);
			}
			else
			{
				Node* local_parent = ((CMyList::Node*)control->GetTag())->parent();
				if( local_parent->data()._child_visible && local_parent->data()._pListElement->IsVisible() )
				{
					control->SetVisible(true);
				}
			}
		}
	}
}

SIZE CMyList::GetExpanderSizeX(Node* node) const
{
	if( !node || node == _root ) return CSize();
	if( node->data()._level >= 1 ) return CSize();

	SIZE szExpander = {0};
	szExpander.cx = 6 + 24 * node->data()._level - 4/*适当放大一点*/;
	szExpander.cy = szExpander.cx + 16 + 8/*适当放大一点*/;
	return szExpander;
}

void CMyList::Event(TEventUI& event)
{
	if(event.Type == UIEVENT_SCROLLWHEEL)
	{
		switch( LOWORD(event.wParam) ) 
		{
		case SB_LINEUP:
			LineUp();
			LineUp();
			LineUp();
			return;
		case SB_LINEDOWN:
			LineDown();
			LineDown();
			LineDown();
			return;
		}
	}
	CListUI::Event(event);
}