#pragma once
#include <vector>
#include <iostream>
#include "atlstr.h"

typedef struct _TASK_ITEM
{
	char taskid[41];
	int filelength;
	int downlength;
	BOOL isdowning;
	WCHAR filename[MAX_PATH];
}TASK_ITEM,*LTASK_ITEM,*LPTASK_ITEM;

class QvodCtrl
{
public:
	QvodCtrl(void);
	~QvodCtrl(void);
	BYTE * buffer;
	int GetTaskInfo(int *pLength,int *uk);
	int Init();
	CRITICAL_SECTION cs;
	HANDLE hThread;
	int Start(char * Taskid, int tp);
	int Stop(char *Taskid, int tp);
	int Delete(char *Taskid);
	int AddTask(LPCWSTR FileName,char *a,int b);
	int PlayTask(char *Taskid,int a);
	int MoveTo(char *Taskid,int sta,int b);
	int CheckBuf(char *a,int b,BYTE *c,int d,int e,int f,int *g);
	int GetPlayInfo(char *Taskid,BYTE *a,int *b);
	int PlayTask2(char *Taskid);


	LPTASK_ITEM GetTaskCurrentItem(CStringA taskId);





	DWORD funHandle;
	std::vector <TASK_ITEM> m_TaskList;


	LPTASK_ITEM pTaskLastPlayItem;
	
	int SetSpeed(int speed);
};