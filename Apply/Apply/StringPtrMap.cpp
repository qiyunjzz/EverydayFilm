#include "stdafx.h"
#include "StringPtrMap.h"

static UINT HashKey(LPCTSTR Key)
{
    UINT i = 0;
    SIZE_T len = _tcslen(Key);
    while( len-- > 0 ) i = (i << 5) + i + Key[len];
    return i;
}

static UINT HashKey(const CString& Key)
{
    return HashKey((LPCTSTR)Key);
};

CStringPtrMap::CStringPtrMap(int nSize)
{
    if( nSize < 16 ) nSize = 16;
    m_nBuckets = nSize;
    m_aT = new TITEM*[nSize];
    memset(m_aT, 0, nSize * sizeof(TITEM*));
}

CStringPtrMap::~CStringPtrMap()
{
    int len = m_nBuckets;
    while( len-- ) {
        TITEM* pItem = m_aT[len];
        while( pItem ) {
            TITEM* pKill = pItem;
            pItem = pItem->pNext;
            delete pKill;
        }
    }
    delete [] m_aT;
}

void CStringPtrMap::Resize(int nSize)
{
    if( m_aT ) {
        int len = m_nBuckets;
        while( len-- ) {
            TITEM* pItem = m_aT[len];
            while( pItem ) {
                TITEM* pKill = pItem;
                pItem = pItem->pNext;
                delete pKill;
            }
        }
        delete [] m_aT;
        m_aT = NULL;
    }
    
    if( nSize < 0 ) nSize = 0;
    if( nSize > 0 ) {
        m_aT = new TITEM*[nSize];
        memset(m_aT, 0, nSize * sizeof(TITEM*));
    } 
    m_nBuckets = nSize;
}

LPVOID CStringPtrMap::Find(LPCTSTR key) const
{
    if( m_nBuckets == 0 ) return NULL;

    UINT slot = HashKey(key) % m_nBuckets;
    for( const TITEM* pItem = m_aT[slot]; pItem; pItem = pItem->pNext ) {
        if( pItem->Key == key ) {
            return pItem->Data;
        }        
    }

    return NULL;
}

bool CStringPtrMap::Insert(LPCTSTR key, LPVOID pData)
{
    if( m_nBuckets == 0 ) return false;
    if( Find(key) ) return false;

    // Add first in bucket
    UINT slot = HashKey(key) % m_nBuckets;
    TITEM* pItem = new TITEM;
    pItem->Key = key;
    pItem->Data = pData;
    pItem->pNext = m_aT[slot];
    m_aT[slot] = pItem;
    return true;
}

LPVOID CStringPtrMap::Set(LPCTSTR key, LPVOID pData)
{
    if( m_nBuckets == 0 ) return pData;

    UINT slot = HashKey(key) % m_nBuckets;
    // Modify existing item
    for( TITEM* pItem = m_aT[slot]; pItem; pItem = pItem->pNext ) {
        if( pItem->Key == key ) {
            LPVOID pOldData = pItem->Data;
            pItem->Data = pData;
            return pOldData;
        }
    }

    Insert(key, pData);
    return NULL;
}

bool CStringPtrMap::Remove(LPCTSTR key)
{
    if( m_nBuckets == 0 ) return false;

    UINT slot = HashKey(key) % m_nBuckets;
    TITEM** ppItem = &m_aT[slot];
    while( *ppItem ) {
        if( (*ppItem)->Key == key ) {
            TITEM* pKill = *ppItem;
            *ppItem = (*ppItem)->pNext;
            delete pKill;
            return true;
        }
        ppItem = &((*ppItem)->pNext);
    }

    return false;
}

int CStringPtrMap::GetSize() const
{
    int nCount = 0;
    int len = m_nBuckets;
    while( len-- ) {
        for( const TITEM* pItem = m_aT[len]; pItem; pItem = pItem->pNext ) nCount++;
    }

    return nCount;
}

LPCTSTR CStringPtrMap::GetAt(int iIndex) const
{
    int pos = 0;
    int len = m_nBuckets;
    while( len-- ) {
        for( TITEM* pItem = m_aT[len]; pItem; pItem = pItem->pNext ) {
            if( pos++ == iIndex ) {
				return pItem->Key.GetBuffer();
            }
        }
    }

    return NULL;
}

LPCTSTR CStringPtrMap::operator[] (int nIndex) const
{
    return GetAt(nIndex);
}





