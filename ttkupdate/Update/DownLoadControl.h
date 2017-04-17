#ifndef __DOWNLOADCNTL__H__
#define __DOWNLOADCNTL__H__
#pragma once
#include "DownloadMTR.h"

// 类链表结点
typedef struct node
{
	void *info;
	struct node *link;
}NODE;



class CDownLoadControl
{
public:
	CDownLoadControl(void);
	~CDownLoadControl(void);

	int GetTaskNum();			//得到任务个数
	int AddTask( TCHAR *pszUrl, TCHAR *pszSavePath, TCHAR *pszSaveFileName, int nThreadNum,	LPCTSTR lpszUsername=NULL,
		LPCTSTR lpszPassword=NULL,BOOL bForce=FALSE); //曾加任务
	int SelTask( int nPos );  // 选中一个任务
	int RemoveTask();		  // 删除选中的任务
	
	int StopTask(int nPos);		//停止任务

	// 查询一个任务的状态
	int QueryState( int nPos, 
		int &nFileTotaleSize, 
		int &nTotalDownloadedSize,
		int &nTotalDownloadedSize_ThisTimes,
		int &nDownloadElapsedTime ); 

public:
	CDownLoadMTR *m_pCurDownloadMTR;
private:
	int m_nCurDownloadMTRPos;
	NODE *m_pHead;   // 指向头结点的指针,为其info指针分配一个空间,可以用来记录链表的结点个数
	CCriticalSection m_csCntl;  // 访问共享锁
};
#endif