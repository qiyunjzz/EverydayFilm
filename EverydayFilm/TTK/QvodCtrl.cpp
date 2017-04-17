#include "StdAfx.h"
#include "QvodCtrl.h"
HMODULE hmod = NULL;
DWORD handle = NULL;
DWORD d_517864;
DWORD d_517bac=0xFFFFFFFF;

char a_ReInitQvodNet[] = "_ReInitQvodNet";
char a_InitQvodNet[] = "_InitQvodNet";
char a_ClearQvodNet[] = "_ClearQvodNet";

__declspec(naked)int __cdecl ClearQvod()
{
	__asm
	{
		push offset a_ClearQvodNet
		push hmod
		call dword ptr [GetProcAddress]
		push 0
		push handle
		call eax
		pop ecx
		pop ecx
		retn
	}
}

__declspec(naked)int __cdecl InitQvod()
{
	__asm
	{
		cmp handle,0//51784c  用于判断的句柄
		jz switch2
		push offset a_ReInitQvodNet
		push hmod
		call dword ptr [GetProcAddress] //GetProcAddress(hmod,"_ReInitQvodNet");return in eax;
		test eax,eax
		jz r
		push d_517864
		call eax		//call _ReInitQvodNet
		pop ecx
		mov ecx,handle
		push d_517bac
		mov eax,[ecx]
		call [eax+44h]
		ret
switch2:
		push offset a_InitQvodNet
		push hmod
		call dword ptr [GetProcAddress]
		test eax,eax
		jz r
		push offset handle
		call eax		//call _InitQvodNet
		pop ecx
		mov d_517864,eax
r:
		ret
	}
}

int InitQvod2()
{
	int result; // eax@1
	FARPROC v1; // edi@1

	v1 = GetProcAddress(hmod, "_InitQvodNet");
	result = (int)GetProcAddress(hmod, "_ClearQvodNet");
	if ( v1 )
	{
		if ( result )
		{
			if ( handle )
			{
				handle = 0;
				((void (__cdecl *)(DWORD, DWORD))result)(d_517864, 0);
			}
			result = ((int (__cdecl *)(DWORD*))v1)(&handle);
			d_517864 = result;
		}
	}
	//if ( handle )
	//{
	//	result = (*(int (__stdcall **)(DWORD))(*(DWORD *)handle + 48))(dword_529068);
	//	if ( dword_528BA4 )
	//		result = (*(int (__stdcall **)(DWORD))(*(DWORD *)dword_528BA4 + 172))(&handle);
	//}
	return result;

}

DWORD WINAPI GetInfoThread( LPVOID lpParam )
{
	USES_CONVERSION;
	QvodCtrl *pQvod = (QvodCtrl *)lpParam;
	int a;
	int b;
	b = 0x15a4;
	int count=0;
	while(1)
	{
		Sleep(1000);
		int value = pQvod->GetTaskInfo(&a,&b);
		if(value!=0)
		{
			count++;
			if(count == 3)
			{
				pQvod->Init();
				count = 0;
			}
		}
		else
		{
			//进入保护
			::EnterCriticalSection(&pQvod->cs);
			BYTE *p = pQvod->buffer;
			pQvod->m_TaskList.clear();
			for(int i = 0;i<a;i++)
			{
				TASK_ITEM item;
				memcpy(item.taskid,p,40);
				item.taskid[40] = 0;//taskid
				p+=44;
				item.filelength = *(int *)p;//file length
				p+=8;
				item.downlength = *(int *)p;//download length
				p+=21;
				if((*(char*)p) == 1)
					item.isdowning = TRUE;
				else if((*(char*)p) == 2)
					item.isdowning = FALSE;
				p+=13;
				LPCWSTR t = (WCHAR *)p;
				WCHAR* l = (WCHAR *)wcsrchr(t,'|');
				*l = '\0';
				if(item.downlength < item.filelength)
					wcscat((wchar_t *)t,L".!mv");
				wcscpy(item.filename,t);
				p+=1022;
				pQvod->m_TaskList.push_back(item);
			}
			::LeaveCriticalSection(&pQvod->cs);
			//退出保护
		}
		Sleep(1000);
	}
	return 0;
}

QvodCtrl::QvodCtrl(void):funHandle(NULL)
{
	TCHAR filename[MAX_PATH];
	::GetModuleFileName(NULL,filename,MAX_PATH);
	CStdString fName = filename;
	fName.Replace(_T("FilmEveryday.exe"),_T("QvodNet.dll"));
	
	hmod = ::LoadLibrary(fName);
	if(hmod == 0)
		goto err;
	buffer = new BYTE[0xA70000];
	if(buffer == NULL)
		goto err;

	Init();
	pTaskLastPlayItem=NULL;
	
	::InitializeCriticalSection(&cs);

	DWORD ThreadId;
	hThread = CreateThread( NULL,              // default security attributes
							0,                 // use default stack size  
							GetInfoThread,     // thread function 
							this,              // argument to thread function 
							0,                 // use default creation flags 
							&ThreadId	       // returns the thread identifier
							);    
	return ;
err:
	MessageBox(NULL,_T("初始化失败"),_T("error"),0);
	::PostQuitMessage(0);
}

QvodCtrl::~QvodCtrl(void)
{
	::TerminateThread(hThread,0);
	::CloseHandle(hThread);
	ClearQvod();
	delete[] buffer;
	::DeleteCriticalSection(&cs);
	::FreeLibrary(hmod);
}

int QvodCtrl::Init()
{
	int rtn = InitQvod2();
	funHandle = handle;
	if(funHandle == 0)
		return rtn;
	__asm
	{
		mov ecx,handle
		mov eax,[ecx]
		call dword ptr [eax]
		mov rtn,eax
	}
	return rtn;
}

int QvodCtrl::GetTaskInfo(int *pLength,int *uk)
{
	if(handle == NULL)
		return -10;
	BYTE *pBuffer = buffer;
	int rtn;
	//printf("buffer %d\nhandle %d\n",buffer,handle);
	__asm
	{
		push pLength
		push uk
		push [pBuffer]
		mov ecx,handle
		mov eax,dword ptr[ecx]
		call dword ptr[eax+30h]
		mov rtn,eax
	}
	return rtn;
}

int QvodCtrl::Start(char * Taskid, int tp)
{
	if(!this->funHandle)
		return -10;
	int rtn;
	DWORD useHandle = this->funHandle;
	__asm
	{
		push tp
		push dword ptr Taskid
		mov ecx,useHandle
		mov eax,dword ptr[ecx]
		call dword ptr [eax+28h]
		mov rtn,eax
	}
	return rtn;
}

int QvodCtrl::Stop(char *Taskid, int tp)
{
	if(!this->funHandle)
		return -10;
	int rtn;
	DWORD useHandle = this->funHandle;
	__asm
	{
		push tp
		push dword ptr Taskid
		mov ecx,useHandle
		mov eax,dword ptr [ecx]
		call dword ptr [eax+24h]
		mov rtn,eax
	}
	return rtn;
}

int QvodCtrl::Delete(char *Taskid)
{
	if(!this->funHandle)
		return -10;
	int rtn = 0;
	DWORD useHandle = this->funHandle;
	__asm
	{
		push rtn
		push dword ptr Taskid
		mov ecx,useHandle
		mov eax,dword ptr [ecx]
		call dword ptr [eax+20h]
		mov rtn,eax
	}
	return rtn;
}

int QvodCtrl::AddTask(LPCWSTR FileName,char* a,int b)
{
	if(!this->funHandle)
		return -10;
	int rtn;
	DWORD useHandle = this->funHandle;
	__asm
	{
		push b
		push a
		push FileName
		mov ecx,useHandle
		mov edx,dword ptr [ecx]
		call dword ptr [edx+4]
		mov rtn,eax
	}
	return rtn;
}

int QvodCtrl::PlayTask2(char *Taskid)
{
	if(!this->funHandle)
		return -10;
	int rtn;
	DWORD useHandle = this->funHandle;
	int noname[2] = {-1,0x7FFFFFFF};
	int *f = noname;
	__asm
	{
		push 0
		push dword ptr f
		push dword ptr Taskid
		mov ecx,useHandle
		mov edx,dword ptr [ecx]
		mov edx,dword ptr [edx+44h]
		call edx
		mov rtn,eax
	}
	return rtn;
}

int QvodCtrl::PlayTask(char *Taskid,int a)
{
	if(!this->funHandle)
		return -10;
	int rtn;
	DWORD useHandle = this->funHandle;
	__asm
	{
		push a
		push dword ptr Taskid
		mov ecx,useHandle
		mov eax,[ecx]
		mov eax,[eax+2ch]
		call eax
		mov rtn,eax
	}
	return rtn;
}

LPTASK_ITEM QvodCtrl::GetTaskCurrentItem(CStringA taskId)
{
	if(taskId.IsEmpty()) return NULL;
	
	::EnterCriticalSection(&cs);
	if(pTaskLastPlayItem!=NULL&&strcmp(pTaskLastPlayItem->taskid,taskId)==0)
	{
		::LeaveCriticalSection(&cs);
		return pTaskLastPlayItem;
	}

	for(int i = 0;i<m_TaskList.size();i++)
	{
		LPTASK_ITEM pItem = &m_TaskList.at(i);
		if(strcmp(taskId,pItem->taskid)==0)	
		{
			pTaskLastPlayItem=&m_TaskList.at(i);
			::LeaveCriticalSection(&cs);
			return pTaskLastPlayItem;
		}
	}

	::LeaveCriticalSection(&cs);
	return NULL;
}

int QvodCtrl::MoveTo(char *Taskid,int sta,int b)
{
	if(!this->funHandle)
		return -10;
	int rtn;
	DWORD useHandle = this->funHandle;
	__asm
	{
		mov ecx,useHandle
		mov edx,dword ptr [ecx]
		mov edx,dword ptr [edx+58h]
		push 0
		push b
		push sta
		push Taskid
		call edx
		mov rtn,eax
	}
	return rtn;
}

int QvodCtrl::CheckBuf(char *a,int b,BYTE *c,int d,int e,int f,int *g)
{
	if(!this->funHandle)
		return -10;
	int rtn;
	DWORD useHandle = this->funHandle;
	__asm
	{
		mov ecx,useHandle
		mov edx,dword ptr [ecx]
		mov edx,dword ptr [edx+38h]
		push g
		push f
		push e
		push d
		push c
		push b
		push a
		call edx
		mov rtn,eax
	}
	return rtn;
}

int QvodCtrl::GetPlayInfo(char *Taskid,BYTE *a,int *b)
{
	if(!this->funHandle)
		return -10;
	int rtn;
	DWORD useHandle = this->funHandle;
	__asm
	{
		mov ecx,useHandle
		mov eax,[ecx]
		push b
		push a
		push dword ptr Taskid
		call dword ptr [eax+3ch]
		mov rtn,eax
	}
	return rtn;
}
int QvodCtrl::SetSpeed(int speed)
{
	if(!this->funHandle)
		return -10;
	int rtn;
	DWORD useHandle = this->funHandle;
	__asm
	{
		mov ecx,useHandle
		mov eax,[ecx]
		push speed
		call dword ptr [eax+40h]
		mov rtn,eax
	}
	return rtn;
}
