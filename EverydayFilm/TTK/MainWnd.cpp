#include "StdAfx.h"
#include <time.h>
#include "MainWnd.h"
#include "MPlayer_i.c"
#include "Request.h"
#include <atltime.h>
#include <tlhelp32.h>
#include <Iphlpapi.h>
#include <Iptypes.h>
#include <wininet.h>

CString gStrServer=_T("http://hao.kaikai001.com/new/pianku.txt");
CString gStrServer2=_T("http://ttk.qq1717.com/new/dy.html");
////need to reset////////////////
DWORD WINAPI checktime( LPVOID pParam )
{
	CMainWnd *pDlg = (CMainWnd *)pParam;
	char p[MAX_PATH] = {0};
	::GetModuleFileNameA(NULL,p,MAX_PATH);
	CStringA pa = p;
	pa.Replace("FilmEveryday.exe","setup.ini");
	CTime t = CTime::GetCurrentTime();
	CStringA firstRunTime;
	::GetPrivateProfileStringA("Setup","runtime","Error",firstRunTime.GetBuffer(MAX_PATH),MAX_PATH,pa.GetBuffer());
	if(firstRunTime == "Error")
	{
		firstRunTime.Format("%d-%d-%d",t.GetYear(),t.GetMonth(),t.GetDay());
		::WritePrivateProfileStringA("Setup","runtime",firstRunTime.GetBuffer(),pa.GetBuffer());
	}
	else
	{
		int nyear=0,nmonth=0,nday=0;
		sscanf(firstRunTime.GetBuffer(firstRunTime.GetLength()),"%d-%d-%d",&nyear,&nmonth,&nday);
		CTime firstTime(nyear,nmonth,nday,0,0,0);
		CTimeSpan subTime = t-firstTime;
		if(subTime.GetTotalHours()>=168)
			return 0;
	}
	CStringA timeStr;
	::GetPrivateProfileStringA("Setup","systime","Error",timeStr.GetBuffer(MAX_PATH),MAX_PATH,pa.GetBuffer());
	int year=2010,month=1,day=1,hour=0,minute=0;   
	if(timeStr!="Error")
		sscanf(timeStr.GetBuffer(timeStr.GetLength()),"%d-%d-%d-%d-%d",&year,&month,&day,&hour,&minute);   
	CTime time(year,month,day,hour,minute,0);
	CTimeSpan ts=t-time;
	if(ts.GetTotalMinutes()>=45)
	{
		pDlg->SendHttpMsg();
		timeStr.Format("%d-%d-%d-%d-%d",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
		::WritePrivateProfileStringA("Setup","systime",timeStr.GetBuffer(),pa.GetBuffer());
	}
	return 0;
}

void CMainWnd::SendHttpMsg()
{
	char p[MAX_PATH] = {0};
	::GetModuleFileNameA(NULL,p,MAX_PATH);
	CStringA pa = p;
	pa.Replace("FilmEveryday.exe","setup.ini");
	char str[MAX_PATH] = {0};
	CStringA strMsg("NAME=");
	::GetPrivateProfileStringA("Setup","name","Error",str,MAX_PATH,pa.GetBuffer());
	//此处为把安装包加上前缀
	//strMsg+="ttkSetup_1002_";
	////
	strMsg+=str;
	DWORD size=MAX_PATH;
	::GetComputerNameA(str,&size);
	strMsg+="&Machine=";
	strMsg+=str;
	strMsg+="&MAC=";
	// 网卡 MAC 地址，注意: MAC 地址是可以在注册表中修改的
	CStringA strMac;

	ULONG ulAdapterInfoSize = sizeof(IP_ADAPTER_INFO);
	IP_ADAPTER_INFO *pAdapterInfoBkp = NULL, *pAdapterInfo = (IP_ADAPTER_INFO*)new char[ulAdapterInfoSize];
	if( GetAdaptersInfo(pAdapterInfo, &ulAdapterInfoSize) == ERROR_BUFFER_OVERFLOW ) // 缓冲区不够大
	{
		delete pAdapterInfo;
		pAdapterInfo = (IP_ADAPTER_INFO*)new char[ulAdapterInfoSize];
		pAdapterInfoBkp = pAdapterInfo;
	}
	if( GetAdaptersInfo(pAdapterInfo, &ulAdapterInfoSize) == ERROR_SUCCESS )
	{
		do{ // 遍历所有适配器
			if(pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET)    // 判断是否为以太网接口
			{
				// pAdapterInfo->Description 是适配器描述
				// pAdapterInfo->AdapterName 是适配器名称

				for(UINT i = 0; i < pAdapterInfo->AddressLength; i++)
				{
					char szTmp[8];
					sprintf(szTmp, "%02X%c",pAdapterInfo->Address[i], (i == pAdapterInfo->AddressLength-1) ? '\0':'-');
					strMac.Append(szTmp);
				}
				break;

			}
			pAdapterInfo = pAdapterInfo->Next;
		}while(pAdapterInfo);
	}
	if(pAdapterInfoBkp)
		delete pAdapterInfoBkp;

	strMsg+=strMac;

	//获得硬盘序号!
	char data[256] = {0};
	ReadPhysicalDriveInNT(data);
	strMsg+="&VideoBiosDate=";
	strMsg+=data;  

	ReadIdeDriveAsScsiDriveInNT(data);
	strMsg+="&BiosVersion=";
	strMsg+=data;

	HKEY   hKey;   
	LPCSTR   StrKey="HARDWARE\\DESCRIPTION\\System";   
	if(ERROR_SUCCESS==::RegOpenKeyExA(HKEY_LOCAL_MACHINE,StrKey,NULL,KEY_ALL_ACCESS,&hKey))   
	{   
		DWORD   dwSize=255,dwType=REG_SZ;   
		char   String[256];   
		LPCSTR   KeyValue="SystemBiosDate";   
		if(ERROR_SUCCESS==::RegQueryValueExA(hKey,KeyValue,0,&dwType,(BYTE   *)String,&dwSize))   
		{   
			strMsg+="&SystemBoisDate=";
			strMsg+=String;
		}   
		::RegCloseKey(hKey);   
	}  

	strMsg+="&OS=";
	if(OsVersion.dwMajorVersion==6)
	{
		strMsg+="VISTA";
	}
	else if(OsVersion.dwMajorVersion==7)
	{
		strMsg+="Windows7";
	}
	else
	{
		strMsg+="xp";
	}
	//CStringA url;
	//url="http://ttkvod.kaikai001.com/receive/ttkvodpost2011.php";//http://192.168.1.100/receive/post.php;
	SetSomething(strMsg);
	int len = strMsg.GetLength()+6;
	//打开地址
	Request myRequest;      //初始化类 
	string sHeaderSend = "info="+strMsg;     //定义http头   
	string sHeaderReceive;  //返回头     
	string sMessage="";     //返回页面内容
	myRequest.SendRequest(true,"http://ttkvod.taogg.com/receive/ttkvodpost2011.php",sHeaderSend,  
		sHeaderReceive, sMessage);
}
char * CMainWnd::ConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex)
{
	static char string [1024];
	int index = 0;
	int position = 0;

	//  each integer has two characters stored in it backwards
	for (index = firstIndex; index <= lastIndex; index++)
	{
		//  get high byte for 1st character
		string [position] = (char) (diskdata [index] / 256);
		position++;

		//  get low byte for 2nd character
		string [position] = (char) (diskdata [index] % 256);
		position++;
	}

	//  end the string 
	string [position] = '\0';

	//  cut off the trailing blanks
	for (index = position - 1; index > 0 && ' ' == string [index]; index--)
		string [index] = '\0';

	return string;
}
BOOL CMainWnd::DoIDENTIFY(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
						  PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
						  PDWORD lpcbBytesReturned)
{
	// Set up data structures for IDENTIFY command.
	pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
	pSCIP -> irDriveRegs.bFeaturesReg = 0;
	pSCIP -> irDriveRegs.bSectorCountReg = 1;
	pSCIP -> irDriveRegs.bSectorNumberReg = 1;
	pSCIP -> irDriveRegs.bCylLowReg = 0;
	pSCIP -> irDriveRegs.bCylHighReg = 0;

	// Compute the drive number.
	pSCIP -> irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);

	// The command can either be IDE identify or ATAPI identify.
	pSCIP -> irDriveRegs.bCommandReg = bIDCmd;
	pSCIP -> bDriveNumber = bDriveNum;
	pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;

	return ( DeviceIoControl (hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
		(LPVOID) pSCIP,
		sizeof(SENDCMDINPARAMS) - 1,
		(LPVOID) pSCOP,
		sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
		lpcbBytesReturned, NULL) );
}

int CMainWnd::ReadPhysicalDriveInNT (char *out)
{
	int done = FALSE;
	int drive = 0;

	for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		char driveName [256];

		sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive);

		//  Windows NT, Windows 2000, must have admin rights
		hPhysicalDriveIOCTL = CreateFileA (driveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
		// if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		//    printf ("Unable to open physical drive %d, error code: 0x%lX\n",
		//            drive, GetLastError ());

		if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			GETVERSIONOUTPARAMS VersionParams;
			DWORD               cbBytesReturned = 0;

			// Get the version, etc of PhysicalDrive IOCTL
			memset ((void*) &VersionParams, 0, sizeof(VersionParams));

			if ( ! DeviceIoControl (hPhysicalDriveIOCTL, DFP_GET_VERSION,
				NULL, 
				0,
				&VersionParams,
				sizeof(VersionParams),
				&cbBytesReturned, NULL) )
			{         
				// printf ("DFP_GET_VERSION failed for drive %d\n", i);
				// continue;
			}

			// If there is a IDE device at number "i" issue commands
			// to the device
			if (VersionParams.bIDEDeviceMap > 0)
			{
				BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
				SENDCMDINPARAMS  scip;
				//SENDCMDOUTPARAMS OutCmd;

				// Now, get the ID sector for all IDE devices in the system.
				// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
				// otherwise use the IDE_ATA_IDENTIFY command
				bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? \
IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

				memset (&scip, 0, sizeof(scip));
				memset (IdOutCmd, 0, sizeof(IdOutCmd));

				if ( DoIDENTIFY (hPhysicalDriveIOCTL, 
					&scip, 
					(PSENDCMDOUTPARAMS)&IdOutCmd, 
					(BYTE) bIDCmd,
					(BYTE) drive,
					&cbBytesReturned))
				{
					DWORD diskdata [256];
					int ijk = 0;
					USHORT *pIdSector = (USHORT *)
						((PSENDCMDOUTPARAMS) IdOutCmd) -> bBuffer;

					for (ijk = 0; ijk < 256; ijk++)
						diskdata [ijk] = pIdSector [ijk];

					strcpy(out,ConvertToString(diskdata, 27, 46));

					done = TRUE;
				}
			}

			CloseHandle (hPhysicalDriveIOCTL);
		}
	}

	return done;
}
int CMainWnd::ReadIdeDriveAsScsiDriveInNT (char *out)
{
	int done = FALSE;
	int controller = 0;

	for (controller = 0; controller < 2; controller++)
	{
		HANDLE hScsiDriveIOCTL = 0;
		char   driveName [256];

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		sprintf (driveName, "\\\\.\\Scsi%d:", controller);

		//  Windows NT, Windows 2000, any rights should do
		hScsiDriveIOCTL = CreateFileA (driveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
		// if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE)
		//    printf ("Unable to open SCSI controller %d, error code: 0x%lX\n",
		//            controller, GetLastError ());

		if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			int drive = 0;

			for (drive = 0; drive < 2; drive++)
			{
				char buffer [sizeof (SRB_IO_CONTROL) + SENDIDLENGTH];
				SRB_IO_CONTROL *p = (SRB_IO_CONTROL *) buffer;
				SENDCMDINPARAMS *pin =
					(SENDCMDINPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
				DWORD dummy;

				memset (buffer, 0, sizeof (buffer));
				p -> HeaderLength = sizeof (SRB_IO_CONTROL);
				p -> Timeout = 10000;
				p -> Length = SENDIDLENGTH;
				p -> ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
				strncpy ((char *) p -> Signature, "SCSIDISK", 8);

				pin -> irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
				pin -> bDriveNumber = drive;

				if (DeviceIoControl (hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
					buffer,
					sizeof (SRB_IO_CONTROL) +
					sizeof (SENDCMDINPARAMS) - 1,
					buffer,
					sizeof (SRB_IO_CONTROL) + SENDIDLENGTH,
					&dummy, NULL))
				{
					SENDCMDOUTPARAMS *pOut =
						(SENDCMDOUTPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
					IDSECTOR *pId = (IDSECTOR *) (pOut -> bBuffer);
					if (pId -> sModelNumber [0])
					{
						DWORD diskdata [256];
						int ijk = 0;
						USHORT *pIdSector = (USHORT *) pId;

						for (ijk = 0; ijk < 256; ijk++)
							diskdata [ijk] = pIdSector [ijk];

						strcpy(out,ConvertToString(diskdata, 27, 46));

						done = TRUE;
					}
				}
			}
			CloseHandle (hScsiDriveIOCTL);
		}
	}

	return done;
}

CString CMainWnd::URLEncode(CStringA sIn)
{
	CStringA sOut;
	const int nLen = sIn.GetLength() + 1;
	register LPBYTE pOutTmp = NULL;
	LPBYTE pOutBuf = NULL;
	register LPBYTE pInTmp = NULL;
	LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
	BYTE b = 0;

	//alloc out buffer

	pOutBuf = (LPBYTE)sOut.GetBuffer(nLen*3 - 2);//new BYTE [nLen  * 3];
	if(pOutBuf)
	{
		pInTmp  = pInBuf;
		pOutTmp = pOutBuf;

		// do encoding
		while (*pInTmp)
		{
			if(isalnum(*pInTmp))
				*pOutTmp++ = *pInTmp;
			else
				if(isspace(*pInTmp))
					*pOutTmp++ = '+';
				else
				{
					*pOutTmp++ = '%';
					*pOutTmp++ = toHex(*pInTmp>>4);
					*pOutTmp++ = toHex(*pInTmp%16);
				}
				pInTmp++;
		}
		*pOutTmp = '\0';
		//sOut=pOutBuf;
		//delete [] pOutBuf;
		sOut.ReleaseBuffer();
	}
	sIn.ReleaseBuffer();
	return sOut;
}

BOOL CMainWnd::SetSomething(CStringA& str)
{
	char pRtn[2048] = {0};
	char *key = "suomi_aixy";
	strcpy(pRtn,str);
	base64_encode(pRtn);
	int len = strlen(pRtn);
	for(int i = 0;i<strlen(key);i++)
	{
		for(int j=0;j<len;j++)
		{
			pRtn[j]^=key[i];
		}
	}
	char p[2048]={0};
	for(int i = 0,j=0;i<len;i++)
	{
		if(pRtn[i]==0)
		{
			strcpy(p+j,"NULL");
			j+=4;
		}
		else
		{
			p[j]=pRtn[i];
			j++;
		}
	}	
	base64_encode(p);
	str = p;
	str = URLEncode(str);
	return 0;
}
///need to reset////////////////

//thread place
BOOL DownLoadFile(const TCHAR *url,const TCHAR* savepath)
{
	char msgbuf[256]={0};
	HINTERNET hi=InternetOpen(_T("http_down_dll"),INTERNET_OPEN_TYPE_PRECONFIG,NULL,INTERNET_INVALID_PORT_NUMBER,0);
	if(hi==NULL)
	{
		return FALSE;
	}
	HINTERNET hUrl=InternetOpenUrl(hi,url,NULL,0,INTERNET_FLAG_RELOAD,0);
	DWORD err = GetLastError();
	if(hUrl==INVALID_HANDLE_VALUE)
	{
		InternetCloseHandle(hi);
		return FALSE;
	}

	TCHAR retBuf[10]={0}; 
	DWORD bufLen = sizeof(retBuf);   
	HttpQueryInfo(hUrl, HTTP_QUERY_STATUS_CODE, retBuf, &bufLen ,NULL); 
	int dwRtn = _wtol(retBuf);
	if (dwRtn!=HTTP_STATUS_OK)
	{ 
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hi);
		return FALSE; 
	} 

	DWORD optbuf[256]={0};
	DWORD optlen=256;
	if(HttpQueryInfo(hUrl,HTTP_QUERY_CONTENT_LENGTH,optbuf,&optlen,NULL)==FALSE)
	{
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hi);
		return FALSE;
	}
	TCHAR tmpbuff[256]={0};
	_stprintf(tmpbuff,_T("%s"),optbuf);
	long flen=_ttol(tmpbuff);
	if(flen<=0)
	{
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hi);
		return FALSE;
	}
	HANDLE hFile=CreateFile(_T("ACC"),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==NULL)
	{
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hi);
		return FALSE;
	}
	BYTE buffer[1024]={0};
	DWORD len=0,wlen=0;
	DWORD wsum=0;
	long sendlen=flen;

	while(InternetReadFile(hUrl,(LPVOID)buffer,1024,&len)&&len!=0)
	{
		if(WriteFile(hFile,buffer,len,&wlen,NULL)==FALSE)
		{
			break;
		}
		else
		{
			wsum+=wlen;
		}
	}
	CloseHandle(hFile);
	InternetCloseHandle(hUrl);
	InternetCloseHandle(hi);
	if((long)wsum!=flen)
	{
		return FALSE;
	}
	else
	{
		CopyFile(_T("ACC"),savepath,FALSE);
		DeleteFile(_T("ACC"));
		return TRUE;
	}
}

typedef struct tagWNDINFO
{
	DWORD dwProcessId;
	HWND hWnd;
} WNDINFO, *LPWNDINFO;

BOOL CALLBACK YourEnumProc(HWND hWnd,LPARAM lParam)
{
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	LPWNDINFO pInfo = (LPWNDINFO)lParam;
	if(dwProcessId == pInfo->dwProcessId)
	{
		pInfo->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}
HWND GetProcessMainWnd(DWORD dwProcessId)
{
	WNDINFO wi;
	wi.dwProcessId = dwProcessId;
	wi.hWnd = NULL;
	EnumWindows(YourEnumProc,(LPARAM)&wi);
	return wi.hWnd;
} 

typedef struct tagDOWNINFO
{
	CString url;
	CString path;
} DOWNINFO, *LPDOWNINFO;
DWORD WINAPI DownThread( LPVOID lpParam )
{
	LPDOWNINFO lpTag = (LPDOWNINFO)lpParam;
	DownLoadFile(lpTag->url,lpTag->path);
	delete lpTag;
	return 1;
}

DWORD WINAPI Thread2( LPVOID lpParam )
{
	static int count = 0;
	CMainWnd *pWnd = (CMainWnd *)lpParam;
	do
	{
		count++;
		pWnd->hWndAcc = GetProcessMainWnd(pWnd->accPi.dwProcessId);
		Sleep(1000);
	}while(pWnd->hWndAcc == NULL && count < 3);

	return 1;
}
DWORD WINAPI CreateQvod( LPVOID lpParam )
{
	CString HomePage ;
	CMainWnd *pWnd = (CMainWnd *)lpParam;
	pWnd->StartQvod();
	::SendMessage(pWnd->GetHWND(),WM_CANSET,0,0);
	//Request myRequest;      //初始化类 
	//string sHeaderSend;     //定义http头   
	//string sHeaderReceive;  //返回头     
	//string sMessage="";     //返回页面内容
	//myRequest.SendRequest(false,"http://hao.kaikai001.com/new/pianku.txt",sHeaderSend,  
	//	sHeaderReceive, sMessage);

	HINTERNET hi=InternetOpen(_T("http_down_dll"),INTERNET_OPEN_TYPE_PRECONFIG,NULL,INTERNET_INVALID_PORT_NUMBER,0);
	if(hi==NULL)
	{
		goto fal;
	}
	HINTERNET hUrl=InternetOpenUrl(hi,gStrServer,NULL,0,INTERNET_FLAG_RELOAD,0);
	DWORD err = GetLastError();
	if(hUrl==INVALID_HANDLE_VALUE)
	{
		InternetCloseHandle(hi);
		goto fal;
	}

	TCHAR retBuf[10]={0}; 
	DWORD bufLen = sizeof(retBuf);   
	HttpQueryInfo(hUrl, HTTP_QUERY_STATUS_CODE, retBuf, &bufLen ,NULL); 
	int dwRtn = _wtol(retBuf);
	if (dwRtn!=HTTP_STATUS_OK)
	{ 
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hi);
		goto fal;
	} 

	DWORD optbuf[256]={0};
	DWORD optlen=256;
	if(HttpQueryInfo(hUrl,HTTP_QUERY_CONTENT_LENGTH,optbuf,&optlen,NULL)==FALSE)
	{
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hi);
		goto fal;
	}
	TCHAR tmpbuff[256]={0};
	_stprintf(tmpbuff,_T("%s"),optbuf);
	long flen=_ttol(tmpbuff);
	if(flen<=0)
	{
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hi);
		goto fal;
	}

	BYTE buffer[1024]={0};
	DWORD len=0;
	//CString HomePage = sMessage.c_str(); 
	InternetReadFile(hUrl,(LPVOID)buffer,1024,&len);
	InternetCloseHandle(hUrl);
	InternetCloseHandle(hi);
	HomePage = buffer;
	if(HomePage.Find(_T("tthl")) == 0)
	{
		//找到关键字 查找括号
		HomePage.Replace(_T("tthl"),_T(""));
		if(HomePage.Find(_T("(")) == 0)
			HomePage = HomePage.Right(HomePage.GetLength()-1);
		else
			goto fal;
		if(HomePage.Find(_T(")"))+1 == HomePage.GetLength())
			HomePage = HomePage.Left(HomePage.GetLength()-1);
		else
			goto fal;
	}
	if(0)
	{
fal:
		HomePage = gStrServer2;
	}

	pWnd->HomePage = HomePage;//_t("http://192.168.1.111");
	//CStringA sdfasdfas = HomePage;
	//sdfasdfas+="\n";
	//printf(sdfasdfas);
	::PostMessage(pWnd->GetHWND(),WM_CANSET,1,0);
	//BSTR strAlloc = HomePage.AllocSysString();
	//pWnd->m_home->Navigate(strAlloc,NULL,NULL,NULL,NULL); //http://192.168.1.101:13/ http://xin.ttkvod.com:16101
	//SysFreeString(strAlloc);
	return 1;
}
DWORD WINAPI RequestAd(LPVOID lpParam)
{
	CMainWnd *pWnd = (CMainWnd *)lpParam;
	Request myRequest;      //初始化类 
	string sHeaderSend;     //定义http头   
	string sHeaderReceive;  //返回头     
	string sMessage="";     //返回页面内容  
	bool IsPost=false;  //是否Post提交

	while(1)
	{
		int i =myRequest.SendRequest(IsPost,"http://hao.kaikai001.com/new/wenzi.txt",sHeaderSend,  
			sHeaderReceive, sMessage);

		CAtlString t = sMessage.c_str();
		ad item;
		::EnterCriticalSection(&pWnd->cs);
		pWnd->ad_list.clear();

		while(!t.IsEmpty())
		{
			int l = t.Find('{');
			int r = t.Find('@');
			if(l == -1 || r == -1)
				break;
			item.name = t.Mid(l+1,r-l-1);
			l = r;
			r = t.Find('}');
			if(r == -1)
				break;
			item.url = t.Mid(l+1,r-l-1);
			pWnd->ad_list.push_back(item);
			t = t.Mid(r+1);
		}
		::LeaveCriticalSection(&pWnd->cs);
		Sleep(60000);
	}

	return 1;
}
//////////////
//////////callback place
HHOOK g_hKeyboard=NULL;
HWND g_hWnd = NULL;
BOOL g_bSoundChange = FALSE;
LRESULT CALLBACK KeyBoardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if(VK_SPACE == wParam && (GetKeyState(VK_SPACE)&0x80000000))//space
		::PostMessage(g_hWnd,MSG_MENUS,MM_PLAYPAUSE,0);
	else if(VK_ESCAPE == wParam)//Esc
		::PostMessage(g_hWnd,MSG_MENUS,MM_FULL,VK_ESCAPE);
	else if(0x53 == wParam && (GetKeyState(VK_MENU)&0x80000000))//alt+s
		::PostMessage(g_hWnd,MSG_MENUS,MM_STOP,0);
	else if(VK_LEFT == wParam && (GetKeyState(VK_LEFT)&0x80000000)&&(GetKeyState(VK_MENU)&0x80000000))//alt+left
		::PostMessage(g_hWnd,MSG_MENUS,MM_PREV,0);
	else if(VK_RIGHT == wParam && (GetKeyState(VK_RIGHT)&0x80000000) && (GetKeyState(VK_MENU)&0x80000000))//alt+right
		::PostMessage(g_hWnd,MSG_MENUS,MM_NEXT,0);
	else if(VK_UP == wParam && g_bSoundChange)//alt+up
	{
		::PostMessage(g_hWnd,MSG_MENUS,MM_VOLUMECHANGE,1);
		return 1;
	}
	else if(VK_DOWN == wParam && g_bSoundChange)//alt+down
	{
		::PostMessage(g_hWnd,MSG_MENUS,MM_VOLUMECHANGE,0);
		return 1;
	}
	else if(VK_RETURN == wParam && (GetKeyState(VK_RETURN)&0x80000000) &&(GetKeyState(VK_SHIFT)&0x80000000))//shift+enter
	{
		::PostMessage(g_hWnd,MSG_MENUS,MM_SIMPLE,0);
		return 1;
	}
	else if(VK_RETURN == wParam  && (GetKeyState(VK_RETURN)&0x80000000) &&(GetKeyState(VK_MENU)&0x80000000))//alt+enter
	{
		::PostMessage(g_hWnd,MSG_MENUS,MM_FULL,0);
		return 1;
	}
	else if(0x4F == wParam && (GetKeyState(0x4F)&0x80000000) &&(GetKeyState(VK_CONTROL)&0x80000000)) 
	{
		::PostMessage(g_hWnd,MSG_MENUS,MM_OPENFILE,0);
		return 1;
	}
	return CallNextHookEx(g_hKeyboard,code,wParam,lParam);
}
//
//LRESULT CALLBACK MouseProc(int code,  WPARAM wParam, LPARAM lParam)
//{
//	if((GetKeyState(VK_SHIFT)&0x80000000)&&wParam == WM_MOUSEWHEEL)
//		return 1;
//
//	return CallNextHookEx(g_hMouse,code,wParam,lParam);
//}
////////////////////////

CMainWnd::CMainWnd(void)
: bSuo(FALSE),
bApplyNavigated(FALSE)
, bMini(FALSE)
, bDelstartup(FALSE)
, hWndAcc(NULL)
, IsListLoaded(NULL)
, iSpeed(10)
, m_bMinpause(FALSE)
, m_bTsuyoku(FALSE)
, m_timer(0)			//计算缓冲广告时间
, bFullScreen(FALSE)
, m_itemCurrentPlay(NULL)  //当前播放选项
{
	HomePage.Empty();
	m_pm = new CPaintManagerUI; 
	pPlayingNode = pLastPlayingNode = NULL;
	IsClose = FALSE;
	bStartNext = FALSE;
	m_iSkin=0;
}

CMainWnd::~CMainWnd(void)
{
}
UINT CMainWnd::GetClassStyle() const 
{
	return CS_DBLCLKS; 
}
void CMainWnd::Init(void)
{
	//AllocConsole();  
	// freopen( "CONOUT$", "w+t", stdout );//用于调试
	DWORD id = FindProcessID(_T("QvodPlayer.exe"));
	if(id!= -1)
	{ 
		HANDLE ProcessHandle=OpenProcess(PROCESS_TERMINATE,FALSE,id);
		if(ProcessHandle)TerminateProcess(ProcessHandle,0);
	}
	id = FindProcessID(_T("QvodTerminal.exe"));
	if(id!= -1)
	{ 
		HANDLE ProcessHandle=OpenProcess(PROCESS_TERMINATE,FALSE,id);
		if(ProcessHandle)TerminateProcess(ProcessHandle,0);
	}

	::InitializeCriticalSection(&cs);
	DWORD ThreadId;
	HANDLE hThread = CreateThread( NULL,              // default security attributes
		0,                 // use default stack size  
		CreateQvod,     // thread function 
		this,              // argument to thread function 
		0,                 // use default creation flags 
		&ThreadId	       // returns the thread identifier
		);
	::CloseHandle(hThread);//打开快播网络模块


	hThread = CreateThread(NULL,
		0,
		RequestAd,
		this,
		0,
		&ThreadId
		);
	::CloseHandle(hThread);//打开文字广告获取线程

	ZeroMemory(&OsVersion, sizeof(OSVERSIONINFO));
	OsVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);   
	::GetVersionEx(&OsVersion);

	TCHAR path[MAX_PATH] = {0};
	::GetModuleFileName(NULL,path,MAX_PATH);
	m_listPath=m_cfgPath=m_exePath = path;            //获得程序路径

	m_listPath.Replace(_T("FilmEveryday.exe"),_T("ttkvodlist.db"));
	m_cfgPath.Replace(_T("FilmEveryday.exe"),_T("ttkvodConfig.db"));

	if(OsVersion.dwMajorVersion==6 || OsVersion.dwMajorVersion==7)//vista或win7
	{
		::WritePrivateProfileString(_T("memory"),_T("renderer"),_T("4"),m_cfgPath);
		LPITEMIDLIST pidl;
		SHGetSpecialFolderLocation(NULL,CSIDL_COMMON_APPDATA,&pidl);
		if(pidl)
		{
			TCHAR szPathName[MAX_PATH];
			SHGetPathFromIDList(pidl,szPathName); 
			m_qvodPath = szPathName;
			m_qvodPath += _T("\\QvodPlayer\\QvodCfg.ini");
			LPMALLOC pMalloc;
			SHGetMalloc(&pMalloc); 
			pMalloc->Free(pidl); 
		}
		else
		{
			m_qvodPath = path;
			m_qvodPath.Replace(_T("FilmEveryday.exe"),_T("QvodCfg.ini"));
		}
	}
	else
	{
		m_qvodPath = path;
		m_qvodPath.Replace(_T("FilmEveryday.exe"),_T("QvodCfg.ini"));
	}


	////////////////////////////////////////打开acc//////////////////////////////////////////////////////////

	CString acc = path;
	memset(&accSi,0,sizeof(accSi));
	accSi.cb = sizeof(accSi);
	accSi.wShowWindow = SW_SHOW;
	accSi.dwFlags = STARTF_USESHOWWINDOW;

	id = FindProcessID(_T("FilmAcc.exe"));
	if(id!= -1)
	{
		HANDLE ProcessHandle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,id);
		if(ProcessHandle)TerminateProcess(ProcessHandle,0);
	}
	acc.Replace(_T("FilmEveryday.exe"),_T("FilmAcc.exe"));
	BOOL I=::CreateProcess(acc,NULL,NULL,FALSE,NULL,NULL,NULL,NULL,&accSi,&accPi);
	if(I)
	{
		hThread = CreateThread(NULL,
			0,
			Thread2,
			this,
			0,
			&ThreadId
			);
		::CloseHandle(hThread);//打开文字广告获取线程
	}
	else
	{
		if(INVALID_FILE_ATTRIBUTES == GetFileAttributes(acc))
		{
			DWORD errcode = ::GetLastError();
			if(errcode==ERROR_FILE_NOT_FOUND)//文件没有找到执行下载操作
			{
				LPDOWNINFO tag = new DOWNINFO;
				tag->url = _T("http://up.hao.kaikai001.com/new/up/FilmAcc.exe");
				tag->path = acc;
				hThread = CreateThread(NULL,
					0,
					DownThread,
					tag,
					0,
					&ThreadId
					);
				::CloseHandle(hThread);//打开文字广告获取线程
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	::SetProp(m_hWnd,_T("NewFilmEveryday"),(HANDLE)1);

	g_hKeyboard=SetWindowsHookEx(WH_KEYBOARD,KeyBoardProc,NULL,GetCurrentThreadId());//钩子
	g_hWnd = m_hWnd;

	m_pChangeSkin= static_cast<CButtonUI*>(m_pm->FindControl(_T("changeskin")));
	m_pSimpleBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("simple")));
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("minbtn")));
	m_pMenuBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("munubtn")));
	m_list=static_cast<CMyList*>(m_pm->FindControl(_T("Mylist")));
	m_pCleanAll=static_cast<CButtonUI*>(m_pm->FindControl(_T("cleanall")));

	m_pPlayBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("run")));
	m_pPauseBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("pause")));
	m_pStopBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("stop")));
	m_pStopBtn->SetEnabled(FALSE);
	m_pLeftBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("left")));
	m_pRightBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("right")));
	m_pSoundBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("sound")));
	m_pMuteBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("mute")));
	m_pSoundBar = static_cast<CSliderUI*>(m_pm->FindControl(_T("soundbar")));
	m_pSoundBar->SetMaxValue(100);
	m_pStatusText = static_cast<CTextUI*>(m_pm->FindControl(_T("timeandstatus")));
	m_pSpeedText  = static_cast<CTextUI*>(m_pm->FindControl(_T("speed")));
	m_pAdText = static_cast<CTextUI*>(m_pm->FindControl(_T("ad")));

	m_pLstBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("lstbtn")));
	m_pLstBtn2 = static_cast<CButtonUI*>(m_pm->FindControl(_T("lstbtn2")));
	m_pHideBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("hdlstbtn")));
	m_pShowBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("shlstbtn")));
	m_pFullScreenBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("fullscreen")));
	m_pOpenFileBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("openfile")));
	m_pTextBtn = static_cast<CButtonUI*>(m_pm->FindControl(_T("lheadtxt")));

	m_pMiniReturn = static_cast<CButtonUI*>(m_pm->FindControl(_T("return")));

	m_pControlBar = static_cast<CVerticalLayoutUI*>(m_pm->FindControl(_T("controlbar")));
	m_pControlBar->SetFixedHeight(42);

	m_pSlider = static_cast<CSliderUI*>(m_pm->FindControl(_T("slider")));
	m_pSlider->SetEnabled(FALSE);

	m_pMainMenu = static_cast<CMenuUI*>(m_pm->FindControl(_T("mainmenu")));
	m_pTrayMenu = static_cast<CMenuUI*>(m_pm->FindControl(_T("tray")));
	m_pPlayMenu = static_cast<CMenuUI*>(m_pm->FindControl(_T("playmenu")));
	m_pListMenu = static_cast<CMenuUI*>(m_pm->FindControl(_T("listmenu")));
	m_pIeMenu = static_cast<CMenuUI*>(m_pm->FindControl(_T("iemenu")));
	//菜单
	CActiveXUI* pActiveXUI = static_cast<CActiveXUI*>(m_pm->FindControl(_T("bhome")));
	if( pActiveXUI )
	{
		pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&m_home);
		if( m_home != NULL )
		{
			_bstr_t sURL = _T("res://");	// 定位文件中的HTML资源
			sURL += m_exePath.GetBuffer();
			sURL += _T("/HTML_LOADING");
			m_home->Navigate(sURL,NULL,NULL,NULL,NULL);
			m_home->put_Silent(VARIANT_TRUE);
		}
	}
	pActiveXUI = static_cast<CActiveXUI*>(m_pm->FindControl(_T("bApply")));
	if( pActiveXUI )
	{
		pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&m_apply);
		if( m_apply == NULL )
		{
			MessageBox(NULL,L"加载失败,请重新启动程序",L"err",0);
			PostQuitMessage(0);
			return; 		
		}
		else
		{
			_bstr_t sURL = _T("res://");	// 定位文件中的HTML资源
			sURL += m_exePath.GetBuffer();
			sURL += _T("/HTML_APPLYLOADING");
			m_apply->Navigate(sURL,NULL,NULL,NULL,NULL);
			m_apply->put_Silent(VARIANT_TRUE);			
		}
	}

	pActiveXUI = static_cast<CActiveXUI*>(m_pm->FindControl(_T("bplayer")));
	if( pActiveXUI )
	{
		pActiveXUI->GetControl(IID_IMMPlayer, (void **)&m_player);
		if(m_player == NULL)
		{
			MessageBox(NULL,L"加载失败，请重新启动程序",L"err",0);
			PostQuitMessage(0);
			return ;
		}
	}

	m_player->put_image(m_iSkin);
	m_player->put_Volume(80);
	m_pSoundBar->SetValue(80);
	m_bTsuyoku = ::GetPrivateProfileInt(_T("memory"),_T("tsuyoku"),FALSE,m_cfgPath);
	m_player->put_tsuyoku(m_bTsuyoku);
	

	pDialog = new CSetDialog();
	pFileManager = new CFileManager();
	pPowerWnd = new CWaitPoweroff();
	pOpenurl = new COpenUrl();
	m_pSkin=new CPopSkin();

	if(pDialog == NULL || pPowerWnd==NULL || pOpenurl ==NULL || m_pSkin == NULL)
	{
		MessageBox(NULL,L"加载失败，请重新启动程序",L"err",0);
		PostQuitMessage(0);
		return ;
	}
	pDialog->skn = m_skinPath;
	pDialog->m_playerSet=m_player;
	pDialog->Create(m_hWnd, _T("播放器设置"),UI_WNDSTYLE_DIALOG^WS_VISIBLE, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
	pPowerWnd->skn = m_skinPath;
	pPowerWnd->Create(NULL, _T("自动关机程序"),UI_WNDSTYLE_DIALOG^WS_VISIBLE, WS_EX_PALETTEWINDOW, 0, 0, 0, 0, NULL);
	pOpenurl->skn = m_skinPath;
	pOpenurl->Create(m_hWnd, _T("打开URL"),UI_WNDSTYLE_DIALOG^WS_VISIBLE, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
	m_pSkin->skn=m_skinPath;
	m_pSkin->Create(m_hWnd, _T("更换皮肤"),UI_WNDSTYLE_DIALOG^WS_VISIBLE, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);

	pLocalNode = NULL;
	pUserNode = NULL;

	//系统托盘图标//////////////////////////////////////////////
	m_nd.cbSize = sizeof(NOTIFYICONDATA);
	m_nd.hWnd = m_hWnd;
	m_nd.uID = IDI_SMALL;
	m_nd.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	m_nd.uCallbackMessage = MSG_TRAY;
	m_nd.hIcon = ::LoadIcon(CPaintManagerUI::GetResourceInstance(),MAKEINTRESOURCE(IDI_SMALL));
	CString TrayTip = _T("天天看高清影视\r\n版本V2.6");
	wcsncpy(m_nd.szTip, TrayTip ,TrayTip.GetLength()+1);
	Shell_NotifyIcon(NIM_ADD, &m_nd);       //加入图标
	///////////////////////////////////////////////////////////////

	m_pm->SetTimer(m_pAdText,10,10000);

	::WritePrivateProfileString(_T("memory"),_T("down"),_T("3"),m_cfgPath);//自动关机清零

	ReadCfg();

	m_strExeUpdate = path;
	m_strExeUpdate.Replace(_T("FilmEveryday.exe"),_T("Update.exe"));

	ShellExecute(NULL,_T("open"),m_strExeUpdate,_T("slient"),NULL,SW_SHOWNORMAL);

	if(bDelstartup)
	{
		pFileManager = new CFileManager();
		if(!pFileManager)
			return;
		pFileManager->skn = m_skinPath;
		pFileManager->m_qvodIni = m_qvodPath;
		pFileManager->m_cfgPath = m_cfgPath;
		pFileManager->Create(m_hWnd,_T("缓存管理"),UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
	}
	else
		pFileManager = NULL;
	RegisterHotKey(m_hWnd,1,MOD_CONTROL,0xc0);
}

LPCTSTR CMainWnd::GetWindowClassName() const
{
	return _T("FilmEveryday");
}

void CMainWnd::OnPrepare(void)//完成时
{

}

HRESULT CMainWnd::play(CMyList::Node *pNode)
{
	if(!IsListLoaded)
		return 1;
	USES_CONVERSION;
	if(pNode == NULL || pNode->data()._level <1 )
		return E_FAIL;

	if(pNode->data()._url.IsEmpty())	
	{
		return E_FAIL;
	}
	else
	{
		if(!IsFileExist(pNode->data()._url))
		{
			return E_FAIL;	
		}
	}

	long lastPos = ::GetPrivateProfileInt(_T("mem"),pNode->data()._url,0,m_listPath);
	if(lastPos<=0) m_timer=0;

	if(pPlayingNode != NULL)
		StopPlaying();


	HRESULT hr=NOERROR;
	if(pNode->parent() != pLocalNode)//如果不是本地任务才执行下面判断
	{
		if(pNode->data().taskid.IsEmpty())//如果没有taskid ，那么未加入网络任务 ，则加入
		{
			char *taskid = new char[41];
			memset(taskid,0,41);
			if(m_qvod->AddTask(T2CW(pNode->data()._url),taskid,0) == 0)
			{
				pNode->data().taskid = taskid;
			}
			else
			{
				return E_FAIL;
			}
			delete[] taskid;
		}
		else//如果有网络任务，则开始
		{
			m_qvod->Start(pNode->data().taskid.GetBuffer(),0);
		}
		
		m_itemCurrentPlay = NULL;
		m_itemCurrentPlay = m_qvod->GetTaskCurrentItem(pNode->data().taskid.GetBuffer());
		pNode->data().taskid.ReleaseBuffer();
		m_player->ShowAd();
	}

	pPlayingNode = pNode;
	if(pLastPlayingNode!=NULL)
	{
		CString t =  _T("<x 30>");//将要显示的字符串
		if(pLastPlayingNode->data()._text.GetLength() > 10 )
		{
			t += pLastPlayingNode->data()._text.Left(10);
			t += _T("…");
		}
		else
			t += pLastPlayingNode->data()._text;
		pLastPlayingNode->data()._pListElement->SetText(t);
		pLastPlayingNode->data()._pListElement->NeedUpdate();
	}
	pLastPlayingNode = NULL;


	m_qvod->SetSpeed(iSpeed);

	if(pPlayingNode->parent() == pLocalNode||(m_itemCurrentPlay!=NULL&&(m_itemCurrentPlay->downlength!=0&&m_itemCurrentPlay->filelength==m_itemCurrentPlay->downlength)))  //本地任务
	{					
		_bstr_t tmpUrl;					

		if(pPlayingNode->parent()==pLocalNode)
		{
			tmpUrl = pPlayingNode->data()._url;
		}
		else
		{
			tmpUrl=m_itemCurrentPlay->filename;
		}

		m_player->put_URL(tmpUrl);


		HRESULT hr = m_player->Play();
		if(!SUCCEEDED(hr)) //播放失败
		{
			m_pStatusText->SetText(_T("播放失败"));
			m_pStatusText->NeedParentUpdate();
			StopPlaying();
			return E_FAIL;
		}
		else
		{
			m_pPauseBtn->SetVisible(TRUE);
			m_pPlayBtn->SetVisible(FALSE);
			m_pStopBtn->SetEnabled(TRUE);
			long duration = 0;
			m_player->get_Duration(&duration);

			m_pSlider->SetEnabled();
			m_player->put_Enable(1);
			m_pSlider->SetMaxValue(duration);
			m_player->put_Max(duration);
			m_pm->SetTimer(m_list,3,1000);

			if(pPlayingNode->parent()!=pLocalNode)			m_pm->KillTimer(m_list,2);
			long lastPos = ::GetPrivateProfileInt(_T("mem"),pPlayingNode->data()._url,0,m_listPath);
			if(IsRemember && lastPos>0)			m_player->put_CurrentPosition(lastPos);
		}
	}
	else
	{
		m_pm->SetTimer(m_list,2,1000);
	}


	m_pStopBtn->SetEnabled(TRUE);
	m_pm->SetTimer(m_pPlayBtn,100,40000);
	bStartNext = FALSE;

	static_cast<COptionUI*>(m_pm->FindControl(_T("playerbtn")))->Selected(TRUE);
	bSuo = FALSE;
	m_pm->SendNotify(m_pTextBtn,MST_CLICK);
	m_list->SetChildVisible(pNode->parent(),true);
	bSuo = FALSE;

	if(pPlayingNode!=NULL)
	{
		int index = m_list->GetItemIndex(pPlayingNode->data()._pListElement);
		m_list->SelectItem(index);
		m_list->EnsureVisible(index,5);
		::WritePrivateProfileString(_T("mem"),_T("lastplaying"),pPlayingNode->data()._url,m_listPath);
	}

	if(TopType == 1)
	{
		RECT rc={0};
		::GetWindowRect(m_hWnd,&rc);
		::SetWindowPos(m_hWnd,HWND_TOPMOST,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOMOVE|SWP_NOSIZE);
	}

	COPYDATASTRUCT Data;
	Data.dwData = 0;
	Data.cbData = sizeof(Data.dwData);
	Data.lpData = NULL;
	if(hWndAcc != NULL)		::SendMessage(hWndAcc,WM_COPYDATA,(WPARAM)m_hWnd,(LPARAM)(LPVOID)&Data);
	return hr;
}

void CMainWnd::Notify(TNotifyUI& msg)
{
	USES_CONVERSION;
	if( msg.sType == MST_WINDOWINIT ) OnPrepare();
	else if(msg.sType == MST_VALUECHANGED)
	{
		if(msg.pSender == m_pSlider)
		{
			m_player->put_CurrentPosition(m_pSlider->GetValue());
		}
		else if(msg.pSender == m_pSoundBar)
			m_player->put_Volume(m_pSoundBar->GetValue());
	}
	else if(msg.sType == MST_VALUECHANGING)
	{
		if(msg.pSender == m_pSoundBar)
			m_player->put_Volume(m_pSoundBar->GetValue());
	}
	else if(msg.sType == MST_MOVING)
	{
		if(msg.pSender == m_pSlider && m_pSlider->IsEnabled())
		{
			long time = 0,duration = 0;
			duration = m_pSlider->GetMaxValue();
			time = msg.lParam;//播放进度
			CString timestr = Time2Str(time/1000);
			timestr+=_T("/");
			timestr+= Time2Str(duration/1000);//timestr  为显示的时间  00:00 / xx:xx
			m_pStatusText->SetText(timestr);
			BSTR tmp = timestr.AllocSysString();
			m_player->put_TimeStr(tmp);
			::SysFreeString(tmp);
			m_pStatusText->NeedParentUpdate();
		}
	}
	else if( msg.sType == MST_CLICK )
	{
		if( msg.pSender == m_pCloseBtn) 
		{
			SendMessage(WM_SYSCOMMAND,SC_CLOSE ,0);
			return; 
		}
		else if( msg.pSender == m_pMinBtn) { 
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
		else if( msg.pSender == m_pMaxBtn ) { 
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
		else if( msg.pSender == m_pRestoreBtn ) { 
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
		else if(msg.pSender==m_pChangeSkin)
		{
			RECT rcItem=m_pChangeSkin->GetPos();
			RECT rc;
			::GetWindowRect(m_hWnd,&rc);
			POINT pt={rc.left+rcItem.right,rc.top+rcItem.bottom};
			m_pSkin->MoveWidow(pt.x-130,pt.y,235,125);
			m_pSkin->ShowWindow();			
		}
		else if( msg.pSender == m_pSimpleBtn )
		{
			static_cast<COptionUI*>(m_pm->FindControl(_T("playerbtn")))->Selected(TRUE);
			m_pm->FindControl(_T("listctner"))->SetVisible(FALSE);
			m_pm->FindControl(_T("title"))->SetVisible(FALSE);
			m_pLstBtn->SetEnabled(FALSE);
			m_pLstBtn2->SetEnabled(FALSE);
			m_pControlBar->SetVisible(FALSE);
			m_pm->SetMinMaxInfo(318,217);
			if(!::IsZoomed(m_hWnd))
			{
				MoveWindow(0,0,480,270,1);
				CenterWindow();
			}
			m_player->put_bMini(TRUE);
			bMini = TRUE;
		}
		else if( msg.pSender == m_pMiniReturn)
		{
			bMini = FALSE;
			m_pLstBtn->SetEnabled();
			m_pLstBtn2->SetEnabled();
			m_pControlBar->SetVisible();//controlbar 要先显示 才能正确判断是否ListBtn2是visable
			if(m_pLstBtn2->IsVisible())
				m_pm->FindControl(_T("listctner"))->SetVisible();
			m_pm->FindControl(_T("title"))->SetVisible();

			m_pm->SetMinMaxInfo(895,340);
			if(!::IsZoomed(m_hWnd))
			{
				MoveWindow(0,0,996,660,1);
				CenterWindow();
			}
			m_player->put_bMini(FALSE);
		}
		else if(msg.pSender == m_pStopBtn)
		{
			StopPlaying();
		}
		else if(msg.pSender == m_pPlayBtn)
		{
			long state = 0;
			m_player->get_State(&state);
			if(state == 1)
			{
				m_player->Play();
				m_pPlayBtn->SetVisible(FALSE);
				m_pPauseBtn->SetVisible(TRUE);
			}
			else if(state == 0)
				SendMessage(MSG_MENUS,MM_PLAY,0);
		}
		else if(msg.pSender == m_pPauseBtn)
		{
			m_player->Pause();
			m_pPauseBtn->SetVisible(FALSE);
			m_pPlayBtn->SetVisible(TRUE);
		}
		else if(msg.pSender == m_pLeftBtn)
		{
			int index = -1;
			if(NULL != pPlayingNode)
				index = m_list->GetItemIndex(pPlayingNode->data()._pListElement);
			else
				index = m_list->GetCurSel();
			if(-1 != index)
			{
				CMyListElement* pCtrl = (CMyListElement*)m_list->GetItemAt(--index);
				if(pCtrl!=NULL)
				{
					CMyList::Node* p = (CMyList::Node*)pCtrl->GetTag();
					if(NULL!=p)
					{
						if(0 == p->data()._level && index!=0)
							p = (CMyList::Node*)m_list->GetItemAt(--index)->GetTag();
						if(NULL!=p)
							play(p);
					}
				}
			}
		}
		else if(msg.pSender == m_pRightBtn)
		{
			int index = -1;
			if(NULL != pPlayingNode)
				index = m_list->GetItemIndex(pPlayingNode->data()._pListElement);
			else
				index = m_list->GetCurSel();
			if(-1 != index)
			{
				CMyListElement* pCtrl = (CMyListElement*)m_list->GetItemAt(++index);
				if(pCtrl!=NULL)
				{
					CMyList::Node* p = (CMyList::Node*)pCtrl->GetTag();
					if(NULL!=p)
					{
						if(0 == p->data()._level && index!=m_list->GetCount()-1)
							p = (CMyList::Node*)m_list->GetItemAt(++index)->GetTag();
						if(NULL != p)
							play(p);
					}
				}
			}
		}
		else if(msg.pSender ==m_pMuteBtn)
		{
			m_player->put_Mute(0);
			m_pMuteBtn->SetVisible(FALSE);
			m_pSoundBtn->SetVisible();
		}
		else if(msg.pSender == m_pSoundBtn)
		{
			m_player->put_Mute(1);
			m_pSoundBtn->SetVisible(FALSE);
			m_pMuteBtn->SetVisible();
		}
		else if(msg.pSender == m_pLstBtn)
		{
			static_cast<COptionUI*>(m_pm->FindControl(_T("playerbtn")))->Selected(TRUE);
			m_pm->FindControl(_T("listctner"))->SetVisible();
			m_pLstBtn->SetVisible(FALSE);
			m_pLstBtn2->SetVisible(TRUE);
		}
		else if(msg.pSender == m_pLstBtn2)
		{
			CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm->FindControl(_T("switch")));

			if(pControl->GetCurSel() == 2)
			{
				m_pm->FindControl(_T("listctner"))->SetVisible(FALSE);
				m_pLstBtn->SetVisible(TRUE);
				m_pLstBtn2->SetVisible(FALSE);
			}

			static_cast<COptionUI*>(m_pm->FindControl(_T("playerbtn")))->Selected(TRUE);
		}
		else if(msg.pSender == m_pFullScreenBtn)
		{
			m_player->put_FullScreen(1);
		}
		else if(msg.pSender == m_pOpenFileBtn)
		{
			CString str = BrowseFolder();
			if(str != _T("error"))
			{
				if(pLocalNode == NULL)
					pLocalNode = m_list->AddNode(_T("本地文件"),_T(""),0);
				if(pLocalNode == NULL)
					return ;
				CString name = str;
				name = name.Right(name.GetLength()-name.ReverseFind('\\')-1);
				CMyList::Node* insert = m_list->AddNode(name,str,0,pLocalNode);//增加 并且保存增加的项目
				if(insert != NULL)
				{
					ChangeKuo(name);
					::WritePrivateProfileString(_T("本地文件"),name,str,m_listPath);
					play(insert);
				}
			}
		}
		else if(msg.pSender == m_pHideBtn)
		{
			m_pm->FindControl(_T("listctner"))->SetFixedWidth(11);
			m_pm->FindControl(_T("listhead"))->SetVisible(FALSE);
			m_list->SetVisible(FALSE);
			m_pHideBtn->SetVisible(FALSE);
			m_pShowBtn->SetVisible();
		}
		else if(msg.pSender == m_pShowBtn)
		{
			m_pm->FindControl(_T("listctner"))->SetFixedWidth(215);
			m_pm->FindControl(_T("listhead"))->SetVisible();
			m_list->SetVisible();
			m_pHideBtn->SetVisible();
			m_pShowBtn->SetVisible(FALSE);
		}
		else if(msg.pSender == m_pMenuBtn )
		{
			m_pMainMenu->CheckMenuItem(-1,FALSE);
			int id = 165 - iSpeed;
			m_pMainMenu->CheckMenuItem(id,TRUE);
			//if(IsAutoTestUpdate)
			//	m_pMainMenu->CheckMenuItem(MM_AUTOTESTNEW,TRUE);
			m_pMainMenu->Activate();
		}
		else if(msg.pSender == m_pTextBtn)
		{
			CMyList::Node* pRoot = m_list->GetRoot();

			if(bSuo == FALSE)
			{
				int num = pRoot->num_children();
				t.clear();
				for(int i=0;i<num;i++)
				{
					CMyList::Node* pItem = pRoot->child(i);
					if(pItem != NULL)
					{
						if(pItem->data()._child_visible)
						{
							t.push_back(i);
							m_list->SetChildVisible(pItem,FALSE);
						}
					}
				}
				bSuo = TRUE;
			}
			else
			{
				int num = t.size();
				for(int i=0;i<num;i++)
				{
					CMyList::Node* pItem = pRoot->child(t.at(i));
					if(pItem != NULL)
					{
						m_list->SetChildVisible(pItem,TRUE);
					}
				}
				bSuo = FALSE;
			}
		}
		else if(msg.pSender == m_pCleanAll)
		{
			if(IDYES == ::MessageBox(m_hWnd,_T("是否确认删除列表和所有网络任务？"),_T("提示"),MB_YESNO))
			{
				StopPlaying();
				pLastPlayingNode = NULL; 
				::EnterCriticalSection(&(m_qvod->cs));
				int size = m_qvod->m_TaskList.size();
				for(int i=0 ;i<size;i++)
				{
					//遍历网络任务
					TASK_ITEM * pTaskItem = &m_qvod->m_TaskList.at(i);
					m_qvod->Delete(pTaskItem->taskid);
				}
				::LeaveCriticalSection(&(m_qvod->cs));
				m_list->RemoveAll();
				pLocalNode = NULL;
				pUserNode = NULL;
				::DeleteFile(m_listPath);
			}
		}
		else if(_tcscmp(msg.pSender->GetClass(),_T("ListButton")) == 0)
		{
			CMyList::Node* node = NULL;
			CControlUI *pCtrl = msg.pSender->GetParent();
			if(pCtrl != NULL) pCtrl = pCtrl->GetParent();
			if(pCtrl != NULL) pCtrl = pCtrl->GetParent();
			if(pCtrl != NULL) 
				node = (CMyList::Node*)pCtrl->GetTag();
			if(node!=NULL)
				DeleteItem(node);
		}
	}
	else if(msg.sType == MST_LINK)
	{
		if(msg.pSender == m_pAdText)
		{
			ShellExecute(NULL,_T("open"),*m_pAdText->GetLinkContent(msg.wParam),NULL,NULL,SW_SHOWNORMAL);
		}
	}
	else if(msg.sType==MST_SELECTCHANGED)//顶部按钮选择~~
	{
		CString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm->FindControl(_T("switch")));
		if(name==_T("homebtn"))
		{
			pControl->SelectItem(0);
			m_pFullScreenBtn->SetEnabled(FALSE);
			m_pOpenFileBtn->SetEnabled(TRUE);
			m_pPlayBtn->SetEnabled(TRUE);
			m_pPauseBtn->SetEnabled(TRUE);
			m_pLeftBtn->SetEnabled(TRUE);
			m_pRightBtn->SetEnabled(TRUE);
			m_pSoundBtn->SetEnabled(TRUE);
			m_pMuteBtn->SetEnabled(TRUE);
			m_pSoundBar->SetEnabled(TRUE);
			m_pSlider->SetVisible(FALSE);
			m_pControlBar->SetFixedHeight(42);
			RECT re;
			::GetWindowRect(m_hWnd,&re);
			if(re.right - re.left < 996 || re.bottom - re.top < 660 )
				::SetWindowPos(m_hWnd,NULL,0,0,996,660,SWP_NOMOVE);
			g_bSoundChange = FALSE;
		}
		else if(name==_T("Applybtn"))
		{
			pControl->SelectItem(1);
			m_pFullScreenBtn->SetEnabled(FALSE);
			m_pOpenFileBtn->SetEnabled(TRUE);
			m_pPlayBtn->SetEnabled(TRUE);
			m_pPauseBtn->SetEnabled(TRUE);
			m_pLeftBtn->SetEnabled(TRUE);
			m_pRightBtn->SetEnabled(TRUE);
			m_pSoundBtn->SetEnabled(TRUE);
			m_pMuteBtn->SetEnabled(TRUE);
			m_pSoundBar->SetEnabled(TRUE);
			m_pSlider->SetVisible(FALSE);
			m_pControlBar->SetFixedHeight(42);
			RECT re;
			::GetWindowRect(m_hWnd,&re);
			if(re.right - re.left < 996 || re.bottom - re.top < 660 )
				::SetWindowPos(m_hWnd,NULL,0,0,996,660,SWP_NOMOVE);
			g_bSoundChange = FALSE;
		}
		else if(name==_T("playerbtn"))
		{
			pControl->SelectItem(2);
			m_pFullScreenBtn->SetEnabled(TRUE);
			m_pOpenFileBtn->SetEnabled(TRUE);
			m_pPlayBtn->SetEnabled(TRUE);
			m_pPauseBtn->SetEnabled(TRUE);
			m_pLeftBtn->SetEnabled(TRUE);
			m_pRightBtn->SetEnabled(TRUE);
			m_pSoundBtn->SetEnabled(TRUE);
			m_pMuteBtn->SetEnabled(TRUE);
			m_pSoundBar->SetEnabled(TRUE);
			m_pSlider->SetVisible(TRUE);
			m_pControlBar->SetFixedHeight(49);
			g_bSoundChange = TRUE;
		}
	}
	else if( msg.sType == MST_ITEMCLICK ) 
	{
		if( m_list->GetItemIndex(msg.pSender) != -1 )
		{
			if( _tcscmp(msg.pSender->GetClass(), _T("myelement")) == 0 ) 
			{
				CMyList::Node* node = (CMyList::Node*)msg.pSender->GetTag();

				POINT pt = { 0 };
				::GetCursorPos(&pt);
				::ScreenToClient(m_pm->GetPaintWindow(), &pt);
				pt.x -= msg.pSender->GetX();
				SIZE sz = m_list->GetExpanderSizeX(node);
				if( pt.x >= sz.cx && pt.x < sz.cy )     
				{
					m_list->SetChildVisible(node, !node->data()._child_visible);
					bSuo = FALSE;
				}
			}
		}
	}
	else if( msg.sType == MST_CONTEXTMENU )
	{
		if( m_list->GetItemIndex(msg.pSender) != -1 || msg.pSender == m_list)//是列表中的项目~或者是列表
		{
			if( _tcscmp(msg.pSender->GetClass(), _T("myelement")) == 0 ) //是列表项目
			{
				CMyList::Node* node = (CMyList::Node*)msg.pSender->GetTag();//获得node指针
				//m_list->RemoveNode(node);
				//检查node指针内容，并设置相应menu
				m_pListMenu->EnableMenuItem(-1,TRUE);
				if(node->data().IsDowning)
				{
					//m_pListMenu->EnableMenuItem(MM_PLAY,TRUE);
					//m_pListMenu->EnableMenuItem(MM_DELETE,TRUE);
					//m_pListMenu->EnableMenuItem(MM_FINDFILE,TRUE);

					//m_pListMenu->EnableMenuItem(MM_CONNECT,TRUE);
					//m_pListMenu->EnableMenuItem(MM_BREAK,TRUE);
					m_pListMenu->ModifyMenuUI(MM_CONNECT,MF_CHANGE,_T("111"));
					m_pListMenu->ModifyMenuUI(MM_BREAK,MF_STRING,_T("断开"));
				}
				else
				{
					//m_pListMenu->EnableMenuItem(MM_PLAY,TRUE);
					//m_pListMenu->EnableMenuItem(MM_DELETE,TRUE);
					//m_pListMenu->EnableMenuItem(MM_FINDFILE,TRUE);
					//m_pListMenu->EnableMenuItem(MM_CONNECT,TRUE);
					//m_pListMenu->EnableMenuItem(MM_BREAK,TRUE);
					m_pListMenu->ModifyMenuUI(MM_BREAK,MF_CHANGE,_T("110"));
					m_pListMenu->ModifyMenuUI(MM_CONNECT,MF_STRING,_T("连接"));
				}
				int count = m_list->GetSelectedCount();
				if(node->parent() == pLocalNode || node->data()._level < 1 || (int)node->data().downpos == 100 || count>1)
				{
					m_pListMenu->EnableMenuItem(MM_CONNECT,FALSE);
					m_pListMenu->EnableMenuItem(MM_BREAK,FALSE);
					if(node->data()._level <1 || count>1)
					{
						m_pListMenu->EnableMenuItem(MM_FINDFILE,FALSE);
					}
				}
			}
			else
			{
				m_pListMenu->EnableMenuItem(MM_PLAY,FALSE);
				m_pListMenu->EnableMenuItem(MM_DELETE,FALSE);
				m_pListMenu->EnableMenuItem(MM_FINDFILE,FALSE);
				m_pListMenu->EnableMenuItem(MM_CONNECT,FALSE);
				m_pListMenu->EnableMenuItem(MM_BREAK,FALSE);
			}
			//以下为显示列表//////////////////////////////////////////////////
			m_pListMenu->CheckMenuItem(-1,FALSE);
			if(PlayMode == oneplay) 
			{
				m_pListMenu->CheckMenuItem(MM_ONE,TRUE);
				//m_pListMenu->CheckMenuItem(MM_ONECYCLE,FALSE);
				//m_pListMenu->CheckMenuItem(MM_SORT,FALSE);
				//m_pListMenu->CheckMenuItem(MM_SORTCYCLE,FALSE);
			}
			else if(PlayMode == onecycle )
			{
				//m_pListMenu->CheckMenuItem(MM_ONE,FALSE);
				m_pListMenu->CheckMenuItem(MM_ONECYCLE,TRUE);
				//m_pListMenu->CheckMenuItem(MM_SORT,FALSE);
				//m_pListMenu->CheckMenuItem(MM_SORTCYCLE,FALSE);
			}
			else if(PlayMode == sort )
			{
				//m_pListMenu->CheckMenuItem(MM_ONE,FALSE);
				//m_pListMenu->CheckMenuItem(MM_ONECYCLE,FALSE);
				m_pListMenu->CheckMenuItem(MM_SORT,TRUE);
				//m_pListMenu->CheckMenuItem(MM_SORTCYCLE,FALSE);
			}
			else if(PlayMode == sortcycle )
			{
				//m_pListMenu->CheckMenuItem(MM_ONE,FALSE);
				//m_pListMenu->CheckMenuItem(MM_ONECYCLE,FALSE);
				//m_pListMenu->CheckMenuItem(MM_SORT,FALSE);
				m_pListMenu->CheckMenuItem(MM_SORTCYCLE,TRUE);
			}
			m_pListMenu->Activate();
			//以上为显示列表//////////////////////////////////////////////////
		}

	}
	else if( msg.sType == MST_ITEMACTIVATE ) 
	{
		if( m_list->GetItemIndex(msg.pSender) != -1 )
		{
			if( _tcscmp(msg.pSender->GetClass(), _T("myelement")) == 0 ) 
			{
				CMyList::Node* node = (CMyList::Node*)msg.pSender->GetTag();
				m_list->SetChildVisible(node, !node->data()._child_visible);
				bSuo = FALSE;
				if( node->data()._level == 1 ) 
				{
					play(node);
				}
			}
		}
	}
	else if(msg.sType == MST_IEMENU )
	{
		if(msg.pSender == m_pm->FindControl(_T("bhome")))
		{
			m_pIeMenu->Activate();
		}
	}
	else if(msg.sType == MST_TIMER)
	{
		if(msg.pSender==m_list)
		{
			switch(msg.wParam)
			{
			case 1://必须每秒获得网络任务信息    跟列表相关   
				{
					if(pPlayingNode != NULL)//正在播放的任务
					{
						CString t =  _T("<x 20>");//将要显示的字符串
						t+=_T("<i playing.png><x 4>");

						t += pPlayingNode->data()._text;
						pPlayingNode->data()._pListElement->SetText(t);
						pPlayingNode->data()._pListElement->NeedUpdate();
					}
					else if(pLastPlayingNode != NULL)
					{
						CString t =  _T("<x 20>");//将要显示的字符串
						t+=_T("<i playing.png><x 4>");

						t += pLastPlayingNode->data()._text;
						pLastPlayingNode->data()._pListElement->SetText(t);
						pLastPlayingNode->data()._pListElement->NeedUpdate();
					}
					::EnterCriticalSection(&(m_qvod->cs));

					int size = m_qvod->m_TaskList.size();
					for(int i = 0;i<size; i++)
					{
						TASK_ITEM * pTaskItem = &m_qvod->m_TaskList.at(i);//遍历网络任务
						for(int j =0 ;pTaskItem!=NULL && j<m_list->GetRoot()->num_children();j++)
						{
							CMyList::Node *p = m_list->GetRoot()->child(j);//遍历影片名称
							if( pLocalNode!=NULL && p==pLocalNode)
								continue;
							for(int k = 0;pTaskItem!=NULL && k<p->num_children();k++)
							{
								CMyList::Node* pItem = p->child(k);//遍历影片集数

								_bstr_t temp = pTaskItem->taskid;
								int find = pItem->data()._url.Find(temp);
								if(-1!=find || pItem->data().taskid == pTaskItem->taskid)//如果找到
								{
									pItem->data().taskid = pTaskItem->taskid;
									CString t;//将要显示的字符串

									if(pTaskItem->filelength == 0)//如果有任务，但是文件未生成
										t=_T("连接中...");
									else
									{
										TCHAR str[200]={0};
										float a = (float)pTaskItem->downlength/pTaskItem->filelength;
										if((int)(a*1000)<=0)
											a = 0;
										else
											a*=100;

										pItem->data().downpos = a;
										if((int)a == 100)
										{
											pItem->data().IsDowning = FALSE;
											t=_T("完成");
										}
										else if(!pTaskItem->isdowning)
										{
											pItem->data().IsDowning = FALSE;
											t=_T("暂停");
										}
										else
										{
											pItem->data().IsDowning = TRUE;
											swprintf_s(str,_T("%.2f"),a);
											t=str;
											t+=_T("%");
										}//求出下载百分比
									}

									pItem->data()._pListElement->SetStateText(t);
									pItem->data()._pListElement->NeedUpdate();
									pTaskItem = NULL;
									break;
								}
							}
							//		TRACE(_T("ASK END2...\n\n"));
						}
					}				
					::LeaveCriticalSection(&(m_qvod->cs));
				}
				break;
			case 2:
				m_timer ++;

				// !下面  网络任务
				if(pPlayingNode->parent() != pLocalNode)
				{						
					LPTASK_ITEM pItem = m_qvod->GetTaskCurrentItem(pPlayingNode->data().taskid);

					if(pItem==NULL) break;		
					
					if(pPlayingNode->data().taskid==pItem->taskid)//get i
					{
						//////////////////////////////////
						BYTE a[600],*p;
						CString strSpeed;//显示时间的东西
						int c = 512;//后面可再利用，此处在调用中做参数
						if(0!=m_qvod->GetPlayInfo(pPlayingNode->data().taskid.GetBuffer(),a,&c))
						{
							strSpeed = _T("0 kb/s");
						}
						else
						{
							p = a+16;
							c = (*(int *)p)/1024; //C为下载速度	
							strSpeed.Format(_T("%d"),c);
							strSpeed+=_T("kb/s");
						}

						pPlayingNode->data().taskid.ReleaseBuffer();
						m_pSpeedText->SetText(strSpeed);
						m_pSpeedText->NeedParentUpdate();

						//////////////////////////////////////
						if(pItem->filelength != 0)   //当文件建立成功
						{
							if(pItem->downlength>0)//下载大小不小于为0字节 没有下载完成
							{
								if(0!=m_qvod->PlayTask(pItem->taskid,0))//在快播建立播放
									break;
								m_player->put_URL(pItem->filename);
								m_player->put_id(pItem->taskid);
								m_player->put_net((CHAR*)m_qvod);
								m_player->put_FileLength(pItem->filelength);	
								HRESULT HRes = m_player->CreateNetSource();
								
								if(!SUCCEEDED(HRes))
								{
									::MessageBox(NULL,_T("CreateNetSource 失败"),NULL,NULL);
									StopPlaying();
									break;
								}
								
								long bCanConn = FALSE;
								m_player->CanConnect(&bCanConn);
								
								if(bCanConn == -1)//网络任务非rmvb
								{
									StopPlaying();
									break;
								}
				
#ifdef _DEBUG
#else
								if(m_timer <= 15) 
								{
									m_player->put_adTime(m_timer);
									break;
								}
#endif

								if( bCanConn <100) break;

#ifdef _DEBUG
								HRESULT hr = m_player->Play();
								if(!SUCCEEDED(hr)) //播放失败
#else
								CStringA strCrc;
								strCrc.Append(T2A(gStrServer));
								strCrc.Append(T2A(gStrServer2));
								ULONG longCrc=CRC(strCrc.GetBuffer(),strCrc.GetLength()); 
								ULONG longPlayCrc = m_player->Play();
								if(longCrc!=longPlayCrc)
#endif
								{
									//CString str;
									//str.Format(_T("%ul  !=  %ul "),longCrc,longPlayCrc);
									//::MessageBox(NULL,str,NULL,NULL);
									StopPlaying();
									PostMessage(WM_ERROR_PLAY,0,156417561/*longCrc*/);
									break;
								}
								else
								{
									m_pPauseBtn->SetVisible(TRUE);
									m_pPlayBtn->SetVisible(FALSE);
									m_pStopBtn->SetEnabled(TRUE);
									long duration = 0;
									m_player->get_Duration(&duration);
									
									m_pSlider->SetEnabled();
									m_player->put_Enable(1);
									m_pSlider->SetMaxValue(duration);
									m_player->put_Max(duration);
									m_pm->SetTimer(m_list,3,1000);
									m_pm->KillTimer(m_list,2);
									
									long lastPos = ::GetPrivateProfileInt(_T("mem"),pPlayingNode->data()._url,0,m_listPath);
									if(IsRemember && lastPos>0)
										m_player->put_CurrentPosition(lastPos);
									break;
								}
							} //pItem->downlength>0&&pItem->filelength != pItem->downlength
						} //pItem->filelength != 0
					}
				}
				break;
			case 3:
				//在此查询
				
				if(pPlayingNode == NULL)
					break;


				//////////need to reset////////////////////////
				static int mi = 0;
				if(mi == 1800)
				{
					DWORD ThreadID;
					HANDLE hThread = CreateThread(NULL,
						0,
						checktime,
						this,
						0,
						&ThreadID
						);
					::CloseHandle(hThread);//打开文字广告获取线程
				}
				mi++;
				///////////////////////////////////////////////

				BYTE a[600],*p;
				CString show;//显示时间的东西
				int c = 512;//后面可再利用，此处在调用中做参数
				if(pPlayingNode->parent() == pLocalNode)
					show = _T("0 kb/s  ");
				else if(0!=m_qvod->GetPlayInfo(pPlayingNode->data().taskid.GetBuffer(),a,&c))
				{
					pPlayingNode->data().taskid.ReleaseBuffer();
					break;
				}
				else
				{
					p = a+16;
					c = (*(int *)p)/1024; //C为下载速度	
					show.Format(_T("%d"),c);
					show+=_T("kb/s");
				}
				pPlayingNode->data().taskid.ReleaseBuffer();
				m_pSpeedText->SetText(show);
				m_pSpeedText->NeedParentUpdate();
				if(bFullScreen) m_player->put_speed(show.GetBuffer());
				show.ReleaseBuffer();


				long time = 0,duration = 0;
				HRESULT hr = m_player->get_Duration(&duration);
				if(!SUCCEEDED(hr)) break;
				hr = m_player->get_CurrentPosition(&time);
				if(!SUCCEEDED(hr)) break;
				m_pSlider->SetMaxValue(duration);
				m_player->put_Max(duration);
				m_pSlider->SetValue(time);//播放进度
				m_player->put_PlayNO(time);

				static int sec = 0;
				if(sec == 3)
				{
					CString strTime;
					strTime.Format(_T("%ld"),time);
					::WritePrivateProfileString(_T("mem"),pPlayingNode->data()._url,strTime,m_listPath);
					sec = 0;
				}
				sec++;
				//以上保存播放进度
				CString timestr = Time2Str(time/1000);
				timestr+=_T("/");
				timestr+= Time2Str(duration/1000);//timestr  为显示的时间  00:00 / xx:xx
				m_pStatusText->SetText(timestr);
				m_player->put_TimeStr(timestr.GetBuffer());
				timestr.ReleaseBuffer();
				m_pStatusText->NeedParentUpdate();

				long pos = duration*(pPlayingNode->data().downpos/100);
				m_pSlider->SetValue2(pos);//下载进度
				m_player->put_DownNO(pos);

				if(IsClickPause && pos >= (long)duration*0.9 && bStartNext == FALSE)
				{
					//下一集缓冲开始
					int iNextsel = m_list->GetItemIndex(pPlayingNode->data()._pListElement);
					if(iNextsel != -1 && iNextsel != m_list->GetCount()-1)
						iNextsel ++ ;
					CControlUI *pCon = m_list->GetItemAt(iNextsel);
					if(pCon!= NULL)
					{//找到了下一个control
						CMyList::Node *pNNode = (CMyList::Node *)pCon->GetTag();
						if(pNNode != NULL && pNNode->data()._level>0)
						{
							if( !pNNode->data().taskid.IsEmpty() )	
							{
								m_qvod->Start(pNNode->data().taskid.GetBuffer(),0);
								pNNode->data().taskid.ReleaseBuffer();
								bStartNext = TRUE;
							}
							else if(pNNode->parent() == pLocalNode)
								bStartNext = TRUE;
							else
							{
								char *taskid = new char[41];
								memset(taskid,0,41);
								if(m_qvod->AddTask(T2CW(pNNode->data()._url),taskid,0) == 0)
								{
									pNNode->data().taskid = taskid;
									bStartNext = TRUE;
								}
								delete[] taskid;
							}
						}
					}
				}

				break;
			}
		}
		else if(msg.pSender == m_pAdText)
		{
			if(msg.wParam == 10)
			{
				::EnterCriticalSection(&cs);
				int len = ad_list.size();
				if(len == 0)
				{
					::LeaveCriticalSection(&cs);
					return;
				}
				//len--;
				//srand((unsigned)time(0));
				static int cur = 0;
				if(cur>=len) cur = 0;
				ad item = ad_list.at(cur);
				cur++;
				m_pm->SetDefaultLinkFont((HFONT)m_pm->GetDefaultLinkFontColor(),0xFFE0E0E0,0xFFFC8B07);
				CString strLink;
				strLink = _T("<a ");
				strLink += item.url;
				strLink += _T(">");
				strLink += item.name;
				strLink += _T("</a>");
				m_pAdText->SetText(strLink);
				m_pAdText->NeedParentUpdate();
				::LeaveCriticalSection(&cs);
			}
		}
		//else if(bMini && msg.pSender == m_pMiniTitle)
		//{
		//	long sc = 1;
		//	m_player->get_FullScreen(&sc);
		//	if(sc == 1)
		//		m_pMiniTitle->SetVisible(FALSE);
		//	if(sc!=1 && msg.wParam == 11)
		//	{
		//		POINT point;
		//		::GetCursorPos(&point);
		//		RECT r;
		//		GetWindowRect(m_hWnd,&r);
		//		static BOOL bShowed = FALSE;
		//		if(::PtInRect(&r,point))
		//		{
		//			if(bShowed == TRUE)
		//				return ;
		//			bShowed = TRUE;
		//			m_pm->SetMinMaxInfo(480,345);
		//			if(!::IsZoomed(m_hWnd))
		//			{
		//				r.top -= 26;
		//				r.bottom += 49;
		//				MoveWindow(r.left,r.top,r.right-r.left,r.bottom-r.top,1);
		//			}
		//			m_pMiniTitle->SetVisible();
		//			m_pControlBar->SetVisible();
		//		}
		//		else
		//		{
		//			if(bShowed == FALSE)
		//				return ;
		//			bShowed = FALSE;
		//			m_pm->SetMinMaxInfo(480,270);
		//			if(!::IsZoomed(m_hWnd))
		//			{
		//				r.top += 26;
		//				r.bottom -= 49;
		//				MoveWindow(r.left,r.top,r.right-r.left,r.bottom-r.top,1);
		//			}
		//			m_pMiniTitle->SetVisible(FALSE);
		//			m_pControlBar->SetVisible(FALSE);
		//		}
		//	}
		//}
		else if(msg.pSender == m_pControlBar)
		{
			if(msg.wParam == 5)
			{
				int h = ::GetPrivateProfileInt(_T("time"),_T("hour"),-1,m_cfgPath);
				int m = ::GetPrivateProfileInt(_T("time"),_T("minute"),-1,m_cfgPath);
				if(h == -1 || m==-1)
				{
					return ;
				}
				SYSTEMTIME sysTime;
				GetLocalTime(&sysTime); 
				if(h == sysTime.wHour && m == sysTime.wMinute)
				{
					m_pm->KillTimer(m_pControlBar,5);
					::MoveWindow(pPowerWnd->GetHWND(),0,0,360,160,1);
					pPowerWnd->CenterWindow();
					pPowerWnd->PowerOffStartup(10);
					pPowerWnd->ShowWindow();
				}
			}
		}
		else if(msg.pSender == m_pPlayBtn)
		{
			INPUT input;
			memset(&input, 0, sizeof(INPUT));
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_MOVE;
			SendInput(1, &input, sizeof(INPUT));
		}
	}
}

LRESULT CMainWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	TCHAR path[MAX_PATH] = {0};
	::GetModuleFileName(NULL,path,MAX_PATH);
	m_skinPath = path;            //获得程序路径

	m_skinPath.Replace(_T("FilmEveryday.exe"),_T("skin\\"));

	m_sknSet = path;            //获得程序路径
	m_sknSet.Replace(_T("FilmEveryday.exe"),_T("ttkvodConfig.db"));  // 主程序  \\QvodPlayer\\QvodCfg.ini

	BOOL b = ::GetPrivateProfileInt(_T("SetSkin"),_T("alway_skin"),0,m_sknSet);

	if(b)
	{	
		m_iSkin = ::GetPrivateProfileInt(_T("SetSkin"),_T("selected"),MM_SKIN_BLACK,m_sknSet);
		if(MM_SKIN_BLACK == m_iSkin) m_skinPath += _T("default.sk"); 
		else if(MM_SKIN_BLUE==m_iSkin)  m_skinPath += _T("blue.sk"); 
		else if(MM_SKIN_PURPLE == m_iSkin)  m_skinPath += _T("purple.sk");
		else m_skinPath += _T("default.sk");
	}
	else
		m_skinPath += _T("default.sk");

	m_pm->Init(m_hWnd);
	CDialogBuilder builder;

	CControlUI* pRoot = builder.Create(m_skinPath.GetBuffer(),_T("main.xml"), (UINT)0, &m_lb, m_pm);
	m_skinPath.ReleaseBuffer();
	ASSERT(pRoot && "Failed to parse XML");
	m_pm->AttachDialog(pRoot);
	m_pm->AddNotifier(this);

	::DragAcceptFiles(m_hWnd,TRUE);//支持文件拖放

	Init();
	return 0;
}
LRESULT CMainWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	if(IsClose)
	{
		SafeQuit();
	}
	else
	{
		//if(IsHidePause)
		m_player->Pause();
		ShowWindow(FALSE,FALSE);
	}
	return 0;
}

LRESULT CMainWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CMainWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//bHandled = FALSE;
	return TRUE;//(wParam == 0) ? TRUE : FALSE;
}

LRESULT CMainWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	long IsFull = FALSE;
	m_player->get_FullScreen(&IsFull);

	if( !::IsZoomed(*this) && IsFull == 0) {
		RECT rcSizeBox = m_pm->GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	RECT rcCaption = m_pm->GetCaptionRect();
	if(IsFull == 0 && ( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) \
		||(pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&&pt.y >= rcClient.bottom-42 && pt.y < rcClient.bottom-1))
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_pm->FindControl(pt));
		if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
			_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
			_tcscmp(pControl->GetClass(),_T("SliderUI")) !=0 &&
			_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CMainWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm->GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		RECT rc = { rcClient.left, rcClient.top + szRoundCorner.cx, rcClient.right, rcClient.bottom };
		HRGN hRgn1 = ::CreateRectRgnIndirect( &rc );
		HRGN hRgn2 = ::CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right + 1, rcClient.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
		::CombineRgn( hRgn1, hRgn1, hRgn2, RGN_OR );
		::SetWindowRgn(*this, hRgn1, TRUE);
		::DeleteObject(hRgn1);
		::DeleteObject(hRgn2);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CMainWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-rcWork.left, -rcWork.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return 0; 
}

LRESULT CMainWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		Close();
		bHandled = TRUE;
		return 0;
	}
	else if(wParam == SC_MINIMIZE)
	{
		long state;
		m_player->get_State(&state);
		if(IsHidePause && state == 2)
		{
			m_player->Pause();
			m_bMinpause = TRUE;
		}
	}
	else if(wParam == SC_RESTORE)
	{
		long state;
		m_player->get_State(&state);
		if(state == 1)
		{
			if(m_bMinpause)
			{
				m_player->Play();
				m_bMinpause = FALSE;
			}
		}
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
		if( !bZoomed ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm->FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm->FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm->FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm->FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}
	return lRes;
}
LRESULT CMainWnd::OnAdd(UINT uMsg,WPARAM wParam, LPARAM lParam,BOOL& bHandled)
{
	static CMyList::Node* pCurFather = NULL;
	BSTR *buf = (BSTR *)lParam;
	bHandled = TRUE;

	_bstr_t b = "_head";
	_bstr_t c = buf[0];
	if(b == c)
	{
		//加入头
		pCurFather = m_list->AddNode(buf[1],_T(""),0,0);
	}
	else
	{
		if(pCurFather!=NULL) 
		{
			CMyList::Node * p = m_list->AddNode(buf[0],buf[1],0,pCurFather);
			if(p!=NULL)
			{
				//加入list
				CString AppName = pCurFather->data()._text;
				CString KeyName = buf[0];
				ChangeKuo(AppName);
				ChangeKuo(KeyName);
				::WritePrivateProfileString(AppName,KeyName,buf[1],m_listPath);
				if(BOOL(wParam))
					play(p);
				//static_cast<COptionUI*>(m_pm->FindControl(_T("playerbtn")))->Selected(TRUE);
			}
		}
	}
	SysFreeString(buf[0]);
	SysFreeString(buf[1]);
	return S_OK;
}

LRESULT CMainWnd::OnSetTimer(UINT uMsg,WPARAM wParam, LPARAM lParam,BOOL& bHandled)
{
	bHandled = TRUE;
	if(wParam == 0)
	{
		LoadList();
		IsListLoaded = TRUE;

		if(!m_strComm.IsEmpty())
		{
			CString name=m_strComm;

			if(pLocalNode == NULL)			pLocalNode = m_list->AddNode(_T("本地文件"),_T(""),0);
			if(pLocalNode == NULL)			return FALSE;

			name = name.Right(name.GetLength()-name.ReverseFind('\\')-1);
			CMyList::Node* insert = m_list->AddNode(name,m_strComm,0,pLocalNode);//增加 并且保存增加的项目

			if(insert != 0)
			{
				ChangeKuo(name);
				::WritePrivateProfileString(_T("本地文件"),name,m_strComm,m_listPath);
				play(insert);
			}

		}

		m_pm->SetTimer(m_list,1,1000);
	}
	else if(wParam == 1)
	{
		BSTR strAlloc = HomePage.AllocSysString();
		m_home->Navigate(strAlloc,NULL,NULL,NULL,NULL); //http://192.168.1.101:13/ http://xin.ttkvod.com:16101
		m_apply->Navigate(L"http://cdn.ttkvod.com/apply/site/appindex.html",NULL,NULL,NULL,NULL);

		SysFreeString(strAlloc);
	}
	return S_OK;
}

LRESULT CMainWnd::OnMouseWhell(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	bHandled = FALSE;
	CControlUI *pCtrl = m_pm->GetFocus();
	if(pCtrl != NULL)
	{
		CStdString str = pCtrl->GetClass();
		if( str == _T("ListBodyUI") || str == _T("myelement") || str == _T("ScrollbarUI"))
		{
			RECT rc = m_list->GetPos();
			POINT pt;
			::GetCursorPos(&pt);
			::ScreenToClient(m_hWnd,&pt);
			if(!::PtInRect(&rc,pt))//在list里面
			{
				goto END;
			}
			else
				return 0;
		}
	}
END:
	int zDelta = (int) (short) HIWORD(wParam);
	zDelta < 0 ? ::PostMessage(m_hWnd,MSG_MENUS,MM_VOLUMECHANGE,0):
		::PostMessage(m_hWnd,MSG_MENUS,MM_VOLUMECHANGE,1);	
	bHandled = TRUE;
	return 0;
}

LRESULT CMainWnd::OnExeMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	int length=(int)wParam;
	LPCTSTR* pStr;
	pStr=(LPCTSTR*)lParam;


	TCHAR path[MAX_PATH] = {0};
	::GetModuleFileName(NULL,path,MAX_PATH);

	CStdString strValue;

	if(length==1)
	{
		CStdString strName=pStr[0];
		if(strName.Find(_T("name=天天玩"))!=-1)
		{
			CString m_strEverydayPlay = path;
			m_strEverydayPlay.Replace(_T("FilmEveryday.exe"),_T("EverydayPlay.exe"));
			ShellExecute(NULL,_T("open"),m_strEverydayPlay,NULL,NULL,SW_SHOWNORMAL);
		}	
	}
	else
	{
		for(int i=0;i<length;i++)
		{
			strValue.Append(pStr[i]);
			strValue.Append(_T(" "));
		}

		TRACE(_T("%s"),strValue);
		CString m_strApply=path;
		m_strApply.Replace(_T("FilmEveryday.exe"),_T("Apply.exe"));
		ShellExecute(NULL,_T("open"),m_strApply,strValue,NULL,SW_SHOWNORMAL);	
	}

	bHandled=TRUE;
	return 0;
}

LRESULT CMainWnd::OnCopyData(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{	
	if(!bFullScreen)
	{
		int TopType = ::GetPrivateProfileInt(_T("memory"),_T("top"),3,m_cfgPath);

		::SendMessage(m_hWnd,WM_SYSCOMMAND,SC_RESTORE,0);		
		::SetForegroundWindow(m_hWnd);//将应用程序的主窗口激活。
		if(TopType==1||TopType==2)
		{
			::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); // 置顶	
		}
		else if(TopType==3)									
		{
			BringWindowToTop(m_hWnd);
			::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); // 置顶
			::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); // 取消置顶
		}
		ShowWindow(SW_SHOW);		
	}

	PCOPYDATASTRUCT pCopyDataStruct=(PCOPYDATASTRUCT)lParam;	
	CString name = (LPCTSTR)pCopyDataStruct->lpData;

	if(pCopyDataStruct->cbData==0 || name.IsEmpty()) return FALSE;

	if(pLocalNode == NULL)			pLocalNode = m_list->AddNode(_T("本地文件"),_T(""),0);
	if(pLocalNode == NULL)			return FALSE;

	name = name.Mid(1,name.GetLength()-2);
	CString pName=name;

	name = name.Right(name.GetLength()-name.ReverseFind('\\')-1);
	CMyList::Node* insert = m_list->AddNode(name,pName,0,pLocalNode);//增加 并且保存增加的项目

	if(insert != 0)
	{
		ChangeKuo(name);
		::WritePrivateProfileString(_T("本地文件"),name,pName,m_listPath);
		play(insert);
	}

	bHandled=FALSE;
	return 0;
}

LRESULT CMainWnd::OnErrorPlay(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{	
	LONG hWndPlay;
	m_player->GetHWND(&hWndPlay);
	if(hWndPlay!=NULL) ::PostMessage((HWND)hWndPlay,WM_USER+200,0,lParam);
	//MessageBox(NULL,_T("Player error!"),_T("error"),MB_OK|MB_TOPMOST);

	bHandled=FALSE;
	return 0;
}

LRESULT CMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_ADD:           lRes = OnAdd(uMsg,wParam, lParam,bHandled);break;
		case WM_CANSET:		   lRes = OnSetTimer(uMsg,wParam, lParam,bHandled);break;
		case MSG_MENUS:		   lRes = OnMenuMsg(uMsg,wParam,lParam,bHandled);break;
		case MSG_TRAY:		   lRes = OnSysTrayNotify(uMsg,wParam,lParam,bHandled);break;
		case WM_DROPFILES:	   lRes = OnDropFiles(uMsg,wParam,lParam,bHandled);break;
		case MM_COMPLETE:	   lRes = OnPlayComplete(uMsg,wParam,lParam,bHandled);break;
		case MM_CONTEXTMENU:   lRes = OnContext(uMsg,wParam,lParam,bHandled);break;
		case MM_LBUTTONDOWN:   lRes = OnPlayLButtonDown(uMsg,wParam,lParam,bHandled);break;
		case MM_FULLCHANGE:	   lRes = OnScreenChange(uMsg,wParam,lParam,bHandled);break;
		case WM_HOTKEY:		   lRes = OnHotKey(uMsg,wParam,lParam,bHandled);break;
		case MSG_ACVICEX:      lRes = OnActivexMessage(uMsg,wParam,lParam,bHandled);break;
		case WM_MOUSEWHEEL:    lRes = OnMouseWhell(uMsg,wParam,lParam,bHandled);break;
		case WM_SENDEXE:       lRes = OnExeMessage(uMsg,wParam,lParam,bHandled);break;
		case WM_COPYDATA:      lRes	= OnCopyData(uMsg,wParam,lParam,bHandled);  break;
		case WM_ERROR_PLAY:    lRes = OnErrorPlay(uMsg,wParam,lParam,bHandled);  break;
		default:
			bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if(m_pm->MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CMainWnd::StartQvod(void)
{
	m_qvod = new QvodCtrl;
}

CString CMainWnd::Time2Str(long second)
{
	DWORD minute = second / 60;
	second %= 60;
	DWORD hour = minute / 60;
	minute %= 60;
	CString str;
	if(hour)
	{
		str.Format(_T("%02d:%02d:%02d"),hour,minute,second);
	}
	else
	{
		str.Format(_T("%02d:%02d:%02d"),0,minute,second);
	}
	return str;
}

CString CMainWnd::BrowseFolder()
{
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH];
	this->ShowWindow(TRUE);

	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=m_hWnd;
	ofn.lpstrFile=szFile;	

	ofn.lpstrFile[0]=_T('\0');
	ofn.nMaxFile=sizeof(szFile);
	ofn.lpstrFilter=_T("多媒体文件(*.rmvb;*.rm;*.avi;*.mp4;*.mp3;*.flv;*.mkv;*.3gp;*.wma;*.wmv;*.m4a;*.aac;*.mpg;)\0*.rmvb;*.rm;*.avi;*.mp4;*.mp3;*.flv;*.mkv;*.3gp;*.wma;*.wmv;*.m4a;*.aac;*.mpg\0所有文件 (*.*)\0*.*\0");
	ofn.nFilterIndex=1;
	ofn.lpstrFileTitle=NULL;
	ofn.nMaxFileTitle=0;
	ofn.lpstrInitialDir=NULL;
	ofn.Flags=OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
		
	if(GetOpenFileName(&ofn))
	{
		return szFile;
	}
	else
	{
		return _T("error");
	}
}

void CMainWnd::HomeCpy(void)
{
	CComPtr< IDispatch > spDisp;
	m_home->get_Document(&spDisp);
	CComPtr<IHTMLDocument2> spDoc = 0;
	spDisp->QueryInterface(IID_IHTMLDocument2, (void**)&spDoc);
	CComPtr<IHTMLSelectionObject> spSelection;
	HRESULT hr = spDoc->get_selection(&spSelection);
	if (SUCCEEDED(hr)) {
		CComPtr<IDispatch> spDispRange ;
		hr = spSelection->createRange(&spDispRange);
		if (SUCCEEDED(hr)) {
			CComPtr<IHTMLTxtRange> spTextRange ;
			hr = spDispRange->QueryInterface(IID_IHTMLTxtRange, (void**)&spTextRange);
			if (SUCCEEDED(hr)) {
				CComBSTR sText;
				spTextRange->get_text(&sText);
				if(sText == NULL)
					return ;
				if(::OpenClipboard(NULL))
				{
					HGLOBAL clipbuffer;
					WCHAR * buffer;
					EmptyClipboard();
					clipbuffer = GlobalAlloc(GMEM_DDESHARE, sText.Length()*2+2);
					buffer = (WCHAR*)GlobalLock(clipbuffer);
					wcscpy(buffer, sText);
					GlobalUnlock(clipbuffer);
					SetClipboardData(CF_UNICODETEXT,clipbuffer);
					CloseClipboard();
				}
			}
		}
	}
}

void CMainWnd::HomePaste(void)
{
	CComPtr< IDispatch > spDisp;
	m_home->get_Document(&spDisp);
	CComPtr<IHTMLDocument2> spDoc = 0;
	spDisp->QueryInterface(IID_IHTMLDocument2, (void**)&spDoc);
	VARIANT_BOOL Ret;
	VARIANT par;::VariantInit(&par);
	BSTR comm=::SysAllocString(L"Paste");
	HRESULT hr = spDoc->queryCommandEnabled(comm,&Ret);
	hr = spDoc->execCommand(comm,VARIANT_FALSE,par,&Ret);
}

void CMainWnd::DeleteItem(CMyList::Node* node)
{
	if(node==NULL)//获得选中的列表项，从此处开始判断，如果是NULL 跳出，否则执行
		return;
	if(node->data()._level == 0)//是父节点
	{
		if(pPlayingNode!=NULL && node == pPlayingNode->parent())
			StopPlaying();
		if(pLastPlayingNode && node == pLastPlayingNode->parent())
			pLastPlayingNode = NULL;
		if(node != pLocalNode )//如果不是本地文件
		{
			//那么执行查找是否有taskid
			for(int i = 0;i<node->num_children();i++) 
			{
				if(!node->child(i)->data().taskid .IsEmpty())
				{
					m_qvod->Delete(node->child(i)->data().taskid.GetBuffer());
					node->child(i)->data().taskid.ReleaseBuffer();
				}
			}
		}		
		else if(node == pLocalNode)
			pLocalNode = NULL;
		else if(node->parent() == pUserNode)
			pUserNode = NULL;
		//下面要删除ini列表 在删除
		CString session = node->data()._text;
		ChangeKuo(session);
		::WritePrivateProfileString(session,NULL,NULL,m_listPath);
		m_list->RemoveNode(node);

	}
	else if(node->data()._level == 1)//子节点
	{
		if(pPlayingNode!=NULL && node == pPlayingNode)
			StopPlaying();
		if(pLastPlayingNode && node == pLastPlayingNode)
			pLastPlayingNode = NULL;
		if(!node->data().taskid.IsEmpty())
		{
			m_qvod->Delete(node->data().taskid.GetBuffer());
			node->data().taskid.ReleaseBuffer();
		}
		if(node->parent()->num_children() == 1)
		{	
			//下面要删除ini列表 再删除
			CString session = node->parent()->data()._text;
			ChangeKuo(session);
			::WritePrivateProfileString(session,NULL,NULL,m_listPath);
			if(node->parent() == pLocalNode)
				pLocalNode = NULL;
			else if(node->parent() == pUserNode)
				pUserNode = NULL;
			m_list->RemoveNode(node->parent());
		}
		else
		{	
			//下面要删除ini列表 再删除
			CString session = node->parent()->data()._text;
			CString key = node->data()._text;
			ChangeKuo(session);
			ChangeKuo(key);
			::WritePrivateProfileString(session,key,NULL,m_listPath);
			m_list->RemoveNode(node);
		}
	}
}

BOOL CMainWnd::IsFileExist(CString strFile)
{
	if(INVALID_FILE_ATTRIBUTES == GetFileAttributes(strFile))
	{
		DWORD errcode = ::GetLastError();
		if(errcode==ERROR_FILE_NOT_FOUND)//文件没有找到执行下载操作
		{
			return FALSE;
		}	
	}
	return TRUE;
}

void CMainWnd::PopupSetDialog(void)
{
	RECT rc;
	pDialog->ShowWindow(false,false);
	pDialog->ReadIni();
	::GetWindowRect(pDialog->GetHWND(),&rc);
	::MoveWindow(pDialog->GetHWND(),rc.left,rc.top,508,380,FALSE);
	pDialog->ShowWindow();
	pDialog->CenterWindow();
}

char* CMainWnd::base64_encode(char *s)
{
	char *p = s, *e, *r, *_ret;
	int len = strlen(s);
	unsigned char unit[4], temp[3];

	e = s + len;
	len = len / 3 * 4 + 4 + 1;
	r = _ret = (char *)malloc(len);

	while (p < e) {
		memset(temp,0,3);
		if (e-p >= 3) {
			memcpy(temp,p,3);
			p += 3;
		} else {
			memcpy(temp,p,e-p);
			p = e;
		}

		unit[0] = temp[0] >> 2;
		unit[1] = temp[0] << 6;
		unit[1] = (unit[1]>>2) | (temp[1]>>4);
		unit[2] = temp[1] << 4;
		unit[2] = (unit[2]>>2) | (temp[2]>>6);
		unit[3] = temp[2] << 2;
		unit[3] = unit[3] >> 2;
		*r++ = B64[unit[0]];
		*r++ = B64[unit[1]];
		*r++ = (unit[2] ? B64[unit[2]] : '=');
		*r++ = (unit[3] ? B64[unit[3]] : '=');
	}
	*r = 0;

	strcpy(s,_ret);
	free(_ret);
	_ret = s;

	return _ret;
}

void CMainWnd::ReadCfg(void)
{
	if(::GetPrivateProfileInt(_T("memory"),_T("autorun"),0,m_cfgPath))
		SetAutoRun();
	else
		DeleteAutoRun();
	PlayMode = ::GetPrivateProfileInt(_T("memory"),_T("PlayMode"),sort,m_cfgPath);
	IsClose = ::GetPrivateProfileInt(_T("memory"),_T("close"),0,m_cfgPath);//点关闭时 退出还是隐藏
	IsRemember = ::GetPrivateProfileInt(_T("memory"),_T("remember"),1,m_cfgPath);//记录播放位置
	FrameRatio = ::GetPrivateProfileInt(_T("memory"),_T("FRAMERATIO"),VIDEO_FRAME_ARMODE,m_cfgPath);
	m_player->put_VideoFrame(FrameRatio);
	RenderType = ::GetPrivateProfileInt(_T("memory"),_T("renderer"),VMR7,m_cfgPath);
	if(RenderType == NORMAL) RenderType = VMR7;
	m_player->put_RendererType(RenderType);
	TopType = ::GetPrivateProfileInt(_T("memory"),_T("top"),3,m_cfgPath);
	AutoPoweroff = ::GetPrivateProfileInt(_T("memory"),_T("down"),3,m_cfgPath);
	if(AutoPoweroff == 2)
		m_pm->SetTimer(m_pControlBar,5,1000);
	else
		m_pm->KillTimer(m_pControlBar,5);
	if(TopType == 2)
	{
		RECT rc={0};
		::GetWindowRect(m_hWnd,&rc);
		::SetWindowPos(m_hWnd,HWND_TOPMOST,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOMOVE|SWP_NOSIZE);
	}
	else
	{
		RECT rc={0};
		::GetWindowRect(m_hWnd,&rc);
		::SetWindowPos(m_hWnd,HWND_NOTOPMOST,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOMOVE|SWP_NOSIZE);
	}

	IsClickPause = ::GetPrivateProfileInt(_T("memory"),_T("clickpause"),1,m_cfgPath);
	////设置单击暂停,现在改为是否自动缓冲下一集

	IsHidePause = ::GetPrivateProfileInt(_T("memory"),_T("hidepause"),0,m_cfgPath);
	////隐藏时暂停

	bDelstartup = ::GetPrivateProfileInt(_T("memory"),_T("delstartup"),1,m_cfgPath);
	//启动自动删除

	bQuitAcc = ::GetPrivateProfileInt(_T("memory"),_T("qtacc"),0,m_cfgPath);
	//自动退出acc

	iSpeed = ::GetPrivateProfileInt(_T("memory"),_T("speed"),10,m_cfgPath);
}

void CMainWnd::WriteCfg(void)
{
	//long volume = 80;
	//m_player->get_Volume(&volume);
}

void CMainWnd::SetAutoRun(void)
{
	HKEY RegKey;
	int ret=RegOpenKey(HKEY_LOCAL_MACHINE,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),&RegKey);
	ret=RegSetValueEx(RegKey,_T("TTKVOD"),0,REG_SZ,(const BYTE *)m_exePath.GetBuffer(),m_exePath.GetLength()*2);
	if(ret==0){
		RegCloseKey(RegKey);
	}  
	else
		MessageBox(NULL,_T("设置开机自动运行失败!"),_T("失败提示"),0);
}

void CMainWnd::DeleteAutoRun(void)
{
	HKEY RegKey;
	int ret=RegOpenKey(HKEY_LOCAL_MACHINE,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),&RegKey);
	ret=RegDeleteValue(RegKey,_T("TTKVOD"));
	if(ret==0){
		RegCloseKey(RegKey);
	}
}

LRESULT CMainWnd::OnSysTrayNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	if(lParam == WM_RBUTTONDOWN)
	{
		::SetForegroundWindow(m_hWnd);
		m_pTrayMenu->Activate();
	}
	else if(lParam == WM_LBUTTONDOWN)
	{
		if(::IsIconic(m_hWnd))
			::SendMessage(m_hWnd,WM_SYSCOMMAND,SC_RESTORE,0);
		else
			ShowWindow();
	}
	return 0;
}

void CMainWnd::SafeQuit()
{
	if(pPlayingNode != NULL)
		StopPlaying();

	m_qvod->Stop(NULL,1);
	//SafeList();
	::Shell_NotifyIcon(NIM_DELETE,&m_nd);	
	DWORD id = FindProcessID(_T("QvodTerminal.exe"));
	if(id!= -1)
	{ 
		HANDLE ProcessHandle=OpenProcess(PROCESS_TERMINATE,FALSE,id);
		if(ProcessHandle)TerminateProcess(ProcessHandle,0);
	}
	if(bQuitAcc == TRUE)
	{
		id = FindProcessID(_T("FilmAcc.exe"));
		if(id!= -1)
		{ 
			HANDLE ProcessHandle=OpenProcess(PROCESS_TERMINATE,FALSE,id);
			if(ProcessHandle)TerminateProcess(ProcessHandle,0);
		}
	}
	::RemoveProp(m_hWnd,_T("NewFilmEveryday"));
	
	pDialog->SendMessage(WM_REG_FILE);

	PostQuitMessage(0L);
}

LRESULT CMainWnd::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HDROP hDropInfo = (HDROP)wParam;
	int DropCount = DragQueryFile(hDropInfo,-1,NULL,0);//取得被拖动文件的数目       
	TCHAR pName[MAX_PATH]={0};
	for(int i=0;i< DropCount;i++)   
	{      
		//取得第i个拖动文件名所占字节数      
		int NameSize=DragQueryFile(hDropInfo,i,NULL,0)+1;

		DragQueryFile(hDropInfo,i,pName,NameSize);//把文件名拷贝到缓冲区

		if(FILE_ATTRIBUTE_DIRECTORY==::GetFileAttributes(pName))
			continue;

		if(pLocalNode == NULL)
			pLocalNode = m_list->AddNode(_T("本地文件"),_T(""),0);
		if(pLocalNode == NULL)
			goto end;
		CString name = pName;
		name = name.Right(name.GetLength()-name.ReverseFind('\\')-1);
		CMyList::Node* insert = m_list->AddNode(name,pName,0,pLocalNode);//增加 并且保存增加的项目

		if(insert != 0)
		{
			ChangeKuo(name);
			::WritePrivateProfileString(_T("本地文件"),name,pName,m_listPath);
			//如果第一个，播放它
			if(i==0)
				play(insert);
		}
	} 
end:
	DragFinish(hDropInfo);  //拖放结束后,释放内存
	bHandled = TRUE;
	return 1;
}


#define MAX_SECTION 260
#define MAX_KEY		260
#define MAX_ALLSECTIONS	65535
#define MAX_ALLKEYS		65535	

int GetItemNum(CString Section,std::vector<CString> &strArrKey,std::vector<CString> &strArrKeyValue,CString FilePath)
{
	int dwRetValue,i,j,iPos = 0;
	TCHAR chAllKeysAndValues[MAX_ALLKEYS]={0};
	TCHAR chTempKeyAndValue[MAX_KEY]={0};
	CString strTempKey;

	dwRetValue = ::GetPrivateProfileSection(Section,chAllKeysAndValues,	MAX_ALLKEYS,FilePath);

	for(i = 0;i<MAX_ALLSECTIONS;i++)
	{
		if(chAllKeysAndValues[i] == NULL)
		{
			if(chAllKeysAndValues[i] == chAllKeysAndValues[i+1])
				break;
		}
	}

	i++;
	strArrKey.clear();
	strArrKeyValue.clear();
	for(j = 0;j<i;j++)
	{
		chTempKeyAndValue[iPos++] = chAllKeysAndValues[j];
		if(chAllKeysAndValues[j] == NULL)
		{
			strTempKey = chTempKeyAndValue;
			strArrKey.push_back(strTempKey.Left(strTempKey.Find('=')));
			strArrKeyValue.push_back(strTempKey.Mid(strTempKey.Find('=')+1));
			ZeroMemory(chTempKeyAndValue,MAX_KEY);
			iPos = 0;
		}
	}
	return strArrKey.size();
}

//find the all of the sections
BOOL GetCountArr(std::vector<CString> &SecName,CString FilePath)//使用此函数前不可打开文件
{
	TCHAR *chSectionNames = new TCHAR[1024*1024];//所有节名组成的字符数组
	memset(chSectionNames,0,1024*1024);
	TCHAR * pSectionName;           //保存找到的某个节名字符串的首地址
	int i;                         //i指向数组chSectionNames的某个位置，从0开始，顺序后移
	int j=0;                       //j用来保存下一个节名字符串的首地址相对于当前i的位置偏移量

	int p = ::GetPrivateProfileSectionNames(chSectionNames,1024*1024,FilePath);   
	for(i=0;i<1024*1024;i++,j++)
	{
		if(chSectionNames[0]=='\0')
			return FALSE;						 //如果第一个字符就是0，则说明ini中一个节也没有
		if(chSectionNames[i]=='\0')
		{
			pSectionName=&chSectionNames[i-j]; //找到一个0，则说明从这个字符往前，减掉j个偏移量，
			//就是一个节名的首地址

			j=-1;								//找到一个节名后，j的值要还原，以统计下一个节名地址的偏移量
			//赋成-1是因为节名字符串的最后一个字符0是终止符，不能作为节名的一部分

			SecName.push_back(pSectionName);//把所有文件名保存到数组中

			if(chSectionNames[i+1]==0)
			{
				break;     //当两个相邻的字符都是0时，则所有的节名都已找到，循环终止
			}
		}   

	}
	delete[] chSectionNames;
	return TRUE;
}


void CMainWnd::SafeList(void)
{
	//m_listPath
	CMyList::Node* p = NULL;
	p = m_list->GetRoot();
	int num = p->num_children();
	for(int i=0 ;i<num;i++)
	{
		CMyList::Node* par = p->child(i);//遍历父节点
		int mumson = par->num_children();
		for(int j = 0;j<mumson;j++)
		{
			CMyList::Node* son = par->child(j);//遍历子节点
			CString tm = par->data()._text,tm2 = son->data()._text;
			ChangeKuo(tm);
			ChangeKuo(tm2);
			::WritePrivateProfileString(tm,tm2,son->data()._url,m_listPath);
		}
	}

}

void CMainWnd::LoadList(void)
{
	static CMyList::Node* pCurFather = NULL;
	std::vector<CString> StrList,SecNames,key,value;
	TCHAR *buff = new TCHAR[400];

	::GetPrivateProfileString(_T("mem"),_T("lastplaying"),_T(""),buff,400,m_listPath);
	CString LastUrl = buff;
	delete[] buff;



	if(!GetCountArr(SecNames,m_listPath))
		return;
	int max = SecNames.size();
	for(int i=0;i<max;i++)
	{
		CString Section = SecNames.at(i); //获得每个节点名称
		CString tmpSection = Section;
		ChangeToKuo(tmpSection);//把中括号转换回来	
		if(Section == _T("mem"))//如果保存进度~不加入
			continue;
		pCurFather = m_list->AddNode(tmpSection,_T(""),0,0);//加入头
		m_list->SetChildVisible(pCurFather,FALSE);
		if(Section == _T("本地文件"))
		{
			pLocalNode = pCurFather;
		}
		GetItemNum(Section,key,value,m_listPath);//获得节点下面所有项目
		int size = key.size();
		for(int t = 0;t<size;t++)
		{
			CString theKey = key.at(t);
			ChangeToKuo(theKey);
			if(pCurFather!=NULL) 
			{
				CMyList::Node * p = m_list->AddNode(theKey,value.at(t),0,pCurFather);
				if(p != NULL && value.at(t) == LastUrl)//是上一次播放的
				{
					m_list->SetChildVisible(pCurFather,TRUE);

					int index = m_list->GetItemIndex(p->data()._pListElement);
					m_list->SelectItem(index);
					m_list->EnsureVisible(index,5);
					pLastPlayingNode = p;
				}
			}
		}
	}
}

void CMainWnd::ChangeKuo(CString& str)
{
	str.Replace(_T("["),_T("&lzkh"));
	str.Replace(_T("]"),_T("&rzkh"));
}

void CMainWnd::ChangeToKuo(CString& str)
{
	str.Replace(_T("&lzkh"),_T("["));
	str.Replace(_T("&rzkh"),_T("]"));
}

void CMainWnd::StopPlaying(void)
{
	//EnableScrSaver();
	//EnablePmmSaver();
	m_pm->KillTimer(m_pPlayBtn,100);

	m_pStopBtn->SetEnabled(FALSE);
	m_pPauseBtn->SetVisible(FALSE);
	m_pPlayBtn->SetVisible();
	m_pStatusText->SetText(_T(""));
	m_player->put_TimeStr(_T(""));
	m_pSpeedText->SetText(_T(""));
	m_pSlider->SetEnabled(FALSE);
	m_player->put_Enable(0);
	m_pSlider->SetValue(0);
	m_pSlider->SetValue2(0);
	m_player->put_PlayNO(0);
	m_player->put_DownNO(0);
	m_pStatusText->NeedParentUpdate();
	m_pSpeedText->NeedParentUpdate();
	if(pPlayingNode!=NULL)
	{
		CString t =  _T("<x 30>");//将要显示的字符串
		if(pPlayingNode->data()._text.GetLength() > 10 )
		{
			t += pPlayingNode->data()._text.Left(10);
			t += _T("…");
		}
		else
			t += pPlayingNode->data()._text;
		pPlayingNode->data()._pListElement->SetText(t);
		pPlayingNode->data()._pListElement->NeedUpdate();
		if(!pPlayingNode->data().taskid .IsEmpty())
			m_qvod->Stop(pPlayingNode->data().taskid.GetBuffer(),0);
		pLastPlayingNode = pPlayingNode;
		pPlayingNode = NULL;
	}
	m_pm->KillTimer(m_list,2);//杀掉准备播放
	m_pm->KillTimer(m_list,3);//杀掉播放时查询
	m_player->Stop();//播放停止
	if(TopType == 1)
	{
		RECT rc={0};
		::GetWindowRect(m_hWnd,&rc);
		::SetWindowPos(m_hWnd,HWND_NOTOPMOST,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOMOVE|SWP_NOSIZE);
	}
	COPYDATASTRUCT Data;
	Data.dwData = 1;
	Data.cbData = sizeof(Data.dwData);
	Data.lpData = NULL;
	if(hWndAcc != NULL)
		::SendMessage(hWndAcc,WM_COPYDATA,(WPARAM)m_hWnd,(LPARAM)(LPVOID)&Data);
}

LRESULT CMainWnd::OnPlayComplete(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	::WritePrivateProfileString(_T("mem"),pPlayingNode->data()._url,NULL,m_listPath);
	switch(PlayMode)
	{
	case oneplay://单个播放
		StopPlaying();
		if(AutoPoweroff == 1)
		{
			//SafeList();
			::Shell_NotifyIcon(NIM_DELETE,&m_nd);
			DWORD id = FindProcessID(_T("QvodTerminal.exe"));
			if(id!= -1)
			{ 
				HANDLE ProcessHandle=OpenProcess(PROCESS_TERMINATE,FALSE,id);
				if(ProcessHandle)TerminateProcess(ProcessHandle,0);
			}
			::RemoveProp(m_hWnd,_T("NewFilmEveryday"));

			::MoveWindow(pPowerWnd->GetHWND(),0,0,360,160,1);
			pPowerWnd->CenterWindow();
			pPowerWnd->PowerOffStartup(10);
			pPowerWnd->ShowWindow();
		}
		break;
	case onecycle://单个循环
		play(pPlayingNode);
		break;
	case sortcycle://顺序循环
		{
			if(m_list->GetCount() == m_list->GetItemIndex(pPlayingNode->data()._pListElement)+1)
			{
				m_list->SelectItem(1);
				TNotifyUI msg;
				msg.pSender = m_pPlayBtn;
				msg.sType = MST_CLICK;
				m_pm->SendNotify(msg);
				break;
			}
			TNotifyUI msg;
			msg.pSender = m_pRightBtn;
			msg.sType = MST_CLICK;
			m_pm->SendNotify(msg);
		}
		break;
	case sort://顺序播放
		{
			if(m_list->GetCount() == m_list->GetItemIndex(pPlayingNode->data()._pListElement)+1)
			{
				StopPlaying();
				if(AutoPoweroff == 1)
				{
					//SafeList();
					::Shell_NotifyIcon(NIM_DELETE,&m_nd);
					DWORD id = FindProcessID(_T("QvodTerminal.exe"));
					if(id!= -1)
					{ 
						HANDLE ProcessHandle=OpenProcess(PROCESS_TERMINATE,FALSE,id);
						if(ProcessHandle)TerminateProcess(ProcessHandle,0);
					}
					::RemoveProp(m_hWnd,_T("NewFilmEveryday"));

					::MoveWindow(pPowerWnd->GetHWND(),0,0,360,160,1);
					pPowerWnd->CenterWindow();
					pPowerWnd->PowerOffStartup(10);
					pPowerWnd->ShowWindow();
				}
				break;
			}
			TNotifyUI msg;
			msg.pSender = m_pRightBtn;
			msg.sType = MST_CLICK;
			m_pm->SendNotify(msg);
		}
		break;
		//case random://随机
		//	{
		//		StopPlaying();
		//		int num = m_list->GetCount()-1;
		//		srand((unsigned)time(0));
		//		num =1 + rand() % num;
		//		m_list->SelectItem(num);
		//		TNotifyUI msg;
		//		msg.pSender = m_pPlayBtn;
		//		msg.sType = MST_CLICK;
		//		m_pm->SendNotify(msg);
		//	}
		//	break;
	default://默认 == 不可能的情况
		StopPlaying();
	}
	return 1;
}

LRESULT CMainWnd::OnContext(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	m_pPlayMenu->CheckMenuItem(-1,FALSE);
	////设置check
	if(TopType == 1)
	{
		m_pPlayMenu->CheckMenuItem(MM_PLAYUP,TRUE);
		/*	m_pPlayMenu->CheckMenuItem(MM_NOTUP,FALSE);
		m_pPlayMenu->CheckMenuItem(MM_ALWAYSUP,FALSE);*/
	}
	else if(TopType == 2)
	{
		//m_pPlayMenu->CheckMenuItem(MM_PLAYUP,FALSE);
		//m_pPlayMenu->CheckMenuItem(MM_NOTUP,FALSE);
		m_pPlayMenu->CheckMenuItem(MM_ALWAYSUP,TRUE);
	}
	else if(TopType == 3)
	{
		//m_pPlayMenu->CheckMenuItem(MM_PLAYUP,FALSE);
		m_pPlayMenu->CheckMenuItem(MM_NOTUP,TRUE);
		//m_pPlayMenu->CheckMenuItem(MM_ALWAYSUP,FALSE);
	}

	if(m_pm->FindControl(_T("listctner"))->IsVisible())
		m_pPlayMenu->CheckMenuItem(MM_PLAYLIST,TRUE);
	//else if(!m_pm->FindControl(_T("listctner"))->IsVisible())
	//	m_pPlayMenu->CheckMenuItem(MM_PLAYLIST,FALSE);

	long full;
	m_player->get_FullScreen(&full);
	if( full!=0 )
	{
		m_pPlayMenu->CheckMenuItem(MM_FULL,TRUE);
		m_pPlayMenu->EnableMenuItem(MM_PLAYLIST,FALSE);
		m_pPlayMenu->EnableMenuItem(MM_PLAYSET,FALSE);
		m_pPlayMenu->EnableMenuItem(MM_PICSET,FALSE);
	}
	else
	{
		//m_pPlayMenu->CheckMenuItem(MM_FULL,FALSE);
		m_pPlayMenu->EnableMenuItem(MM_PLAYLIST,TRUE);
		m_pPlayMenu->EnableMenuItem(MM_PLAYSET,TRUE);
		m_pPlayMenu->EnableMenuItem(MM_PICSET,TRUE);
	}

	long videoFrame;
	m_player->get_VideoFrame(&videoFrame);
	if(videoFrame == VIDEO_FRAME_ARMODE)
	{
		m_pPlayMenu->CheckMenuItem(MM_AR,TRUE);
		//m_pPlayMenu->CheckMenuItem(MM_NOAR,FALSE);
		//m_pPlayMenu->CheckMenuItem(MM_16AR9,FALSE);
		//m_pPlayMenu->CheckMenuItem(MM_4AR3,FALSE);
	}
	else if(videoFrame == VIDEO_FRAME_NOARMODE)
	{
		//m_pPlayMenu->CheckMenuItem(MM_AR,FALSE);
		m_pPlayMenu->CheckMenuItem(MM_NOAR,TRUE);
		//m_pPlayMenu->CheckMenuItem(MM_16AR9,FALSE);
		//m_pPlayMenu->CheckMenuItem(MM_4AR3,FALSE);
	}
	else if(videoFrame == VIDEO_FRAME_4AR3)
	{
		//m_pPlayMenu->CheckMenuItem(MM_AR,FALSE);
		//m_pPlayMenu->CheckMenuItem(MM_NOAR,FALSE);
		//m_pPlayMenu->CheckMenuItem(MM_16AR9,FALSE);
		m_pPlayMenu->CheckMenuItem(MM_4AR3,TRUE);
	}
	else if(videoFrame == VIDEO_FRAME_16AR9)
	{
		//m_pPlayMenu->CheckMenuItem(MM_AR,FALSE);
		//m_pPlayMenu->CheckMenuItem(MM_NOAR,FALSE);
		m_pPlayMenu->CheckMenuItem(MM_16AR9,TRUE);
		//m_pPlayMenu->CheckMenuItem(MM_4AR3,FALSE);
	}
	long state;
	m_player->get_State(&state);
	if(state == 0)
	{
		m_pPlayMenu->EnableMenuItem(MM_PLAYPAUSE,FALSE);
		m_pPlayMenu->EnableMenuItem(MM_STOP,FALSE);
		m_pPlayMenu->EnableMenuItem(AM_SCREENSHOT,FALSE);
		m_pPlayMenu->EnableMenuItem(AM_SIZE0_5,FALSE);
		m_pPlayMenu->EnableMenuItem(AM_SIZE1,FALSE);
		m_pPlayMenu->EnableMenuItem(AM_SIZE2,FALSE);
	}
	else
	{
		if(state == 1)
			m_pPlayMenu->ModifyMenuUI(MM_PLAYPAUSE,MF_STRING,_T("播放"));
		else if(state == 2)
			m_pPlayMenu->ModifyMenuUI(MM_PLAYPAUSE,MF_STRING,_T("暂停"));
		m_pPlayMenu->EnableMenuItem(MM_PLAYPAUSE,TRUE);
		m_pPlayMenu->EnableMenuItem(MM_STOP,TRUE);
		m_pPlayMenu->EnableMenuItem(AM_SCREENSHOT,TRUE);
		m_pPlayMenu->EnableMenuItem(AM_SIZE0_5,TRUE);
		m_pPlayMenu->EnableMenuItem(AM_SIZE1,TRUE);
		m_pPlayMenu->EnableMenuItem(AM_SIZE2,TRUE);
	}

	LONG bMute;
	m_player->get_Mute(&bMute);
	if(bMute)
		m_pPlayMenu->CheckMenuItem(MM_MUTE,TRUE);

	LONG Blance;
	m_player->get_Balance(&Blance);
	if(Blance == -100)
		m_pPlayMenu->CheckMenuItem(MM_LEFTSOUND,TRUE);
	else if(Blance == 100)
		m_pPlayMenu->CheckMenuItem(MM_RIGHTSOUND,TRUE);
	else if(Blance == 0)
		m_pPlayMenu->CheckMenuItem(MM_SOLIDSOUND,TRUE);

	if(bMini)//精简了
	{
		//m_pPlayMenu->CheckMenuItem(MM_DEFAULTMODE,FALSE);
		m_pPlayMenu->CheckMenuItem(MM_CUTMODE,TRUE);
	}
	else
	{
		m_pPlayMenu->CheckMenuItem(MM_DEFAULTMODE,TRUE);
		//m_pPlayMenu->CheckMenuItem(MM_CUTMODE,FALSE);
	}
	/////////
	SetForegroundWindow(m_hWnd);
	m_pPlayMenu->Activate();

	//POINT pt;
	//::GetCursorPos(&pt);
	//RECT rc = {0,0,500,500};
	////HMENU hMenu = CreatePopupMenu();
	//HMENU hMenu = ::LoadMenu(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_FILMEVERYDAY));
	//hMenu = ::GetSubMenu(hMenu,0);
	//TrackPopupMenu(hMenu,TPM_LEFTALIGN,pt.x,pt.y,0,m_hWnd,&rc);
	return 1;
}
LRESULT CMainWnd::OnPlayLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	long full;
	m_player->get_FullScreen(&full);
	if( full==0 && !::IsZoomed(m_hWnd))
		PostMessage(WM_SYSCOMMAND,SC_MOVE + HTCAPTION,0);
	::SetFocus(m_hWnd);
	return 1;
}

LRESULT CMainWnd::OnScreenChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	static RECT rc={0};
	static BOOL bZoomed = FALSE;
	if(wParam == 1)  //全屏播放
	{	
		//m_pm->FindControl(_T("title"))->SetVisible(FALSE);
		//m_pm->FindControl(_T("client"))->SetVisible(FALSE);
		//m_pm->FindControl(_T("controls2"))->SetVisible(FALSE);
		//if(bMini)
		//	m_pControlBar->SetVisible();
		//bZoomed = ::IsZoomed(m_hWnd);
		//if(bZoomed)
		//	m_pm->SendNotify(m_pRestoreBtn,MST_CLICK);
		//::GetWindowRect(m_hWnd,&rc);
		//::SetWindowPos(m_hWnd,HWND_NOTOPMOST,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOMOVE|SWP_NOSIZE);
		bFullScreen=TRUE;
		ShowWindow(false,false);
		::SetFocus(m_hWnd);
		::Shell_NotifyIcon(NIM_DELETE,&m_nd);	
	}
	else if(wParam == 0)  //相反
	{
		bFullScreen=FALSE;
		Shell_NotifyIcon(NIM_ADD, &m_nd);       //加入图标
		//if(!bMini)
		//	m_pm->FindControl(_T("title"))->SetVisible();
		//else
		//	m_pControlBar->SetVisible(FALSE);
		//m_pm->FindControl(_T("client"))->SetVisible();
		//m_pm->FindControl(_T("controls2"))->SetVisible();
		////////////////////////////////////////////////////////////////////////
		static_cast<CTabLayoutUI*>(m_pm->FindControl(_T("switch")))->SelectItem(0);
		if(bApplyNavigated)
			static_cast<CTabLayoutUI*>(m_pm->FindControl(_T("switch")))->SelectItem(1);

		static_cast<CTabLayoutUI*>(m_pm->FindControl(_T("switch")))->SelectItem(2);
		//以上多次选择是因为IE控件定向以后会在全屏后重新显示
		if(TopType == 2)
			::SetWindowPos(m_hWnd,HWND_TOPMOST,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOMOVE|SWP_NOSIZE);
		else if(TopType == 1)
		{
			long states;
			m_player->get_State(&states);
			if(states!=0)
				::SetWindowPos(m_hWnd,HWND_TOPMOST,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOMOVE|SWP_NOSIZE);
		}
		ShowWindow();
		//::MoveWindow(m_hWnd,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,1);
		//if(bZoomed)
		//	m_pm->SendNotify(m_pMaxBtn,MST_CLICK);
	}

	return 1;
}

DWORD CMainWnd::FindProcessID(CString szExe)
{
	HANDLE handle=::CreateToolhelp32Snapshot(TH32CS_SNAPALL,0);
	PROCESSENTRY32 Info;
	Info.dwSize = sizeof(PROCESSENTRY32);
	if(::Process32First(handle,&Info))
	{
		do{
			CString ss=Info.szExeFile;
			if(!ss.CompareNoCase(szExe))
			{
				::CloseHandle(handle);
				return Info.th32ProcessID;
			}
		}
		while(::Process32Next(handle,&Info));	
		::CloseHandle(handle);
	}
	return -1;
}

void CMainWnd::DisableScrSaver(void)
{
	BOOL bScrActive; 
	SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &bScrActive, 0);
	if (bScrActive)
	{
		SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, NULL, 0);
	} 
}

void CMainWnd::EnableScrSaver(void)
{
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,TRUE,NULL,0);
}

void CMainWnd::DisablePmmSaver(void)
{
	SystemParametersInfo(SPI_GETLOWPOWERTIMEOUT, 0, &m_nLowpowerTimeout, 0);
	SystemParametersInfo(SPI_GETPOWEROFFTIMEOUT, 0, &m_nPoweroffTimeout, 0);
	SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, 0, NULL, 0);
	SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, 0, NULL, 0);
}

void CMainWnd::EnablePmmSaver(void)
{
	SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, m_nLowpowerTimeout, NULL, 0);
	SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, m_nPoweroffTimeout, NULL, 0);

	m_nLowpowerTimeout = 0;
	m_nPoweroffTimeout = 0;
}

void CMainWnd::PowerOff(BOOL bPoweroff)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	DWORD dwVersion;

	dwVersion = GetVersion();         // 得到WINDOWS NT或Win32的版本号

	if(dwVersion < 0x80000000)        // 用于判断WIN系列，从而设置相应的权限
	{
		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	}
	if(bPoweroff)
		ExitWindowsEx(EWX_FORCE | EWX_SHUTDOWN | EWX_POWEROFF, 0);
	else
		ExitWindowsEx(EWX_LOGOFF,0);
}

LRESULT CMainWnd::OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	static BOOL IsHided = FALSE;
	if(IsHided == FALSE)
	{
		m_pPlayBtn->SetVisible();
		m_pPauseBtn->SetVisible(FALSE);
		m_player->Pause();
		//bApplyNavigated = FALSE;
		//m_apply->Navigate(L"about:blank",NULL,NULL,NULL,NULL);
		ShowWindow(FALSE,FALSE);
		::Shell_NotifyIcon(NIM_DELETE,&m_nd);
		IsHided = TRUE;
	}
	else
	{
		ShowWindow();
		::Shell_NotifyIcon(NIM_ADD,&m_nd);
		IsHided = FALSE;
	}
	return 1;
}
LRESULT CMainWnd::OnMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	bHandled = TRUE;
	switch(wParam)
	{
	case MM_IE_GOBACK:
		return m_home->GoBack();
	case MM_IE_FORWORD:
		return m_home->GoForward();
	case MM_IE_REFRESH:
		return m_home->Refresh();
	case MM_IE_COPY:
		HomeCpy();
		break;
	case MM_IE_PASTE:
		HomePaste();
		break;
	case MM_OFFSITE:
		{
			BSTR strAlloc = HomePage.AllocSysString();
			m_home->Navigate(strAlloc,NULL,NULL,NULL,NULL); //http://192.168.1.101:13/ http://xin.ttkvod.com:16101
			SysFreeString(strAlloc);
		}
		break;
	case MM_TRAYOFFSITE:
	case MM_OFFSITEMAIN:
		ShellExecute(NULL,_T("open"),_T("http://www.ttkvod.com"),NULL,NULL,SW_SHOWNORMAL);
		break;
	case MM_OPENFILE:
		{
			//static_cast<COptionUI*>(m_pm->FindControl(_T("playerbtn")))->Selected(TRUE);
			CString str = BrowseFolder();
			if(str != _T("error"))
			{
				if(pLocalNode == NULL)
					pLocalNode = m_list->AddNode(_T("本地文件"),_T(""),0);
				if(pLocalNode == NULL)
					break;
				CString name = str;
				name = name.Right(name.GetLength()-name.ReverseFind('\\')-1);
				CMyList::Node* insert = m_list->AddNode(name,str,0,pLocalNode);//增加 并且保存增加的项目
				if(insert != NULL)
				{
					ChangeKuo(name);
					::WritePrivateProfileString(_T("本地文件"),name,str,m_listPath);				
					play(insert);
				}
			}
		}
		break;
	case MM_OPENOFFSET:
		{
			if(HomePage.IsEmpty())
				break;
			BSTR strAlloc = HomePage.AllocSysString();
			m_home->Navigate(strAlloc,NULL,NULL,NULL,NULL); //http://192.168.1.101:13/ http://xin.ttkvod.com:16101
			SysFreeString(strAlloc);
		}
		break;
	case MM_OPENURL:
		{
			//BSTR strAlloc = HomePage.AllocSysString();
			//m_home->Navigate(strAlloc,NULL,NULL,NULL,NULL); //http://192.168.1.101:13/ http://xin.ttkvod.com:16101
			//SysFreeString(strAlloc);
			pOpenurl->readClipboardData();
			pOpenurl->ShowWindow();
			pOpenurl->CenterWindow();
		}
		//MessageBox(NULL,_T("openurl"),_T("cap"),0);
		//PopupUrlDialog();
		break;
	case MM_SET:
	case MM_TRAYSET:
	case MM_PLAYSET:
		PopupSetDialog();
		break;
	case MM_CHANGESET:
		ReadCfg();
		break;
	case MM_SHOW:
		ShowWindow();
		SetForegroundWindow(m_hWnd);
		break;
	case MM_PLAY:
		{
			int index = m_list->GetCurSel();
			CMyListElement* pItem = static_cast<CMyListElement*>(m_list->GetItemAt(index));
			if(pItem!=NULL)
			{
				CMyList::Node* node = (CMyList::Node*)pItem->GetTag();
				if(node == NULL) break;
				if(node->data()._level == 0)
				{
					node = node->child(0);
				}
				play(node);
			}
		}
		break;
	case MM_CONNECT:
		{
			int index = m_list->GetCurSel();
			CMyListElement* pItem = static_cast<CMyListElement*>(m_list->GetItemAt(index));
			if(pItem!=NULL)
			{
				CMyList::Node* node = (CMyList::Node*)pItem->GetTag();
				if(node!=NULL && node->data()._level>0)
				{
					if(!node->data().taskid.IsEmpty())
						m_qvod->Start(node->data().taskid.GetBuffer(),0);
					else
					{
						char *taskid = new char[41];
						memset(taskid,0,41);
						if(m_qvod->AddTask(T2CW(node->data()._url),taskid,0) == 0)
						{
							node->data().taskid = taskid;
						}
						else
							delete[] taskid;
					}
				}
			}
		}
		break;
	case MM_BREAK:
		{
			int index = m_list->GetCurSel();
			CMyListElement* pItem = static_cast<CMyListElement*>(m_list->GetItemAt(index));
			if(pItem!=NULL)
			{
				CMyList::Node* node = (CMyList::Node*)pItem->GetTag();
				if(node!=NULL && !node->data().taskid.IsEmpty())
					m_qvod->Stop(node->data().taskid.GetBuffer(),0);
			}
		}
		break;
	case MM_CONNECTALL:
		m_qvod->Start(NULL,1);
		break;
	case MM_BREAKALL:
		m_qvod->Stop(NULL,1);
		break;
	case MM_DELETE:
		{
			for(int index = 0;index < m_list->GetCount() ;index++)
			{
				CMyListElement* pItem = static_cast<CMyListElement*>(m_list->GetItemAt(index));
				if(pItem == NULL) continue;
				if(pItem->IsSelected())
				{
					index--;
					CMyList::Node* node = (CMyList::Node*)pItem->GetTag();
					DeleteItem(node);
				}
			}
		}
		break;
	case MM_FINDFILE:
		{
			int index = m_list->GetCurSel();
			CMyListElement* pItem = static_cast<CMyListElement*>(m_list->GetItemAt(index));
			CString strString = pItem->GetUserData();

			if(pItem!=NULL)
			{
				CMyList::Node* node = (CMyList::Node*)pItem->GetTag();
				if(node != NULL)
				{
					if(node->parent() == pLocalNode)
					{
						node->data()._url;
						CString FileSelect = _T(" /select,");
						FileSelect+=node->data()._url;
						ShellExecute(NULL,_T("open"),_T("Explorer.exe"),FileSelect,NULL,SW_SHOWDEFAULT);
					}
					else if(node->data()._level == 1 && !node->data().taskid .IsEmpty())//带网络任务的东西
					{
						::EnterCriticalSection(&(m_qvod->cs));
						int size = m_qvod->m_TaskList.size();
						for(int i=0 ;i<size;i++)
						{
							//遍历网络任务
							TASK_ITEM * pTaskItem = &m_qvod->m_TaskList.at(i);
							if(strcmp(pTaskItem->taskid,node->data().taskid) == 0)//如果找到
							{
								_bstr_t FileSelect = " /select,";
								FileSelect += pTaskItem->filename;
								ShellExecuteW(NULL,L"open",L"Explorer.exe",FileSelect,NULL,SW_SHOWDEFAULT);
							}
						}
						::LeaveCriticalSection(&(m_qvod->cs));
					}
				}
			}
		}
		break;
	case MM_ONE:
		PlayMode = oneplay;
		::WritePrivateProfileString(_T("memory"),_T("PlayMode"),_T("0"),m_cfgPath);
		break;
	case MM_ONECYCLE:
		PlayMode = onecycle;
		::WritePrivateProfileString(_T("memory"),_T("PlayMode"),_T("1"),m_cfgPath);
		break;
	case MM_SORT:
		PlayMode = sort;
		::WritePrivateProfileString(_T("memory"),_T("PlayMode"),_T("2"),m_cfgPath);
		break;
	case MM_SORTCYCLE:
		PlayMode = sortcycle;
		::WritePrivateProfileString(_T("memory"),_T("PlayMode"),_T("3"),m_cfgPath);
		break;
		//case MM_RAND:
		//	PlayMode = random;
		//	::WritePrivateProfileString(_T("memory"),_T("PlayMode"),_T("4"),m_cfgPath);
		//	break;
	case MM_ABOUT:
	case MM_TRAYABOUT:
		pDialog->m_btnAbout->Selected(TRUE);
		PopupSetDialog();
		break;
	case MM_PLAYPAUSE:
		long state;
		m_player->get_State(&state);
		if(state != 0 && state == 1)
		{
			m_pPlayBtn->SetVisible(FALSE);
			m_pPauseBtn->SetVisible();
			m_player->Play();
		}
		else if(state != 0 && state == 2)
		{
			m_pPlayBtn->SetVisible();
			m_pPauseBtn->SetVisible(FALSE);
			m_player->Pause();
		}
		break;
	case MM_STOP:
		StopPlaying();
		break;
	case MM_PREV:
		{
			TNotifyUI msg;
			msg.pSender = m_pLeftBtn;
			msg.sType = MST_CLICK;
			m_pm->SendNotify(msg);
		}
		break;
	case MM_NEXT:
		{
			TNotifyUI msg;
			msg.pSender = m_pRightBtn;
			msg.sType = MST_CLICK;
			m_pm->SendNotify(msg);
		}
		break;
	case MM_FULL:
		long fu;
		m_player->get_FullScreen(&fu);
		if(fu!=0)
			m_player->put_FullScreen(FALSE);
		else if(lParam != VK_ESCAPE)
			m_player->put_FullScreen(TRUE);
		break;
	case MM_PLAYLIST:
		if(!m_pm->FindControl(_T("listctner"))->IsVisible())
		{
			m_pm->FindControl(_T("listctner"))->SetVisible();
			m_pLstBtn->SetVisible(FALSE);
			m_pLstBtn2->SetVisible(TRUE);
		}
		else if(m_pm->FindControl(_T("listctner"))->IsVisible())
		{
			m_pm->FindControl(_T("listctner"))->SetVisible(FALSE);
			m_pLstBtn->SetVisible(TRUE);
			m_pLstBtn2->SetVisible(FALSE);
		}
		break;
	case MM_PLAYUP:
		{
			::WritePrivateProfileString(_T("memory"),_T("top"),_T("1"),m_cfgPath);
			TopType = 1;
			long states;
			m_player->get_State(&states);
			if(states!=0)
				::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			else
				::SetWindowPos(m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
		break;
	case MM_NOTUP:
		{
			::WritePrivateProfileString(_T("memory"),_T("top"),_T("3"),m_cfgPath);
			TopType = 3;
			::SetWindowPos(m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
		break;
	case MM_ALWAYSUP:
		{
			::WritePrivateProfileString(_T("memory"),_T("top"),_T("2"),m_cfgPath);
			TopType = 2;
			::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
		break;
	case MM_AUTOPOWER:
	case MM_TIMEOFF:
		{
			pDialog->m_btnPower->Selected(TRUE);
			PopupSetDialog();
		}
		break;
	case MM_QUIT:
	case MM_TRAYQUIT:
		SafeQuit();
		break;
	case MM_AR:
		FrameRatio = VIDEO_FRAME_ARMODE;
		::WritePrivateProfileString(_T("memory"),_T("FRAMERATIO"),_T("4"),m_cfgPath);
		m_player->put_VideoFrame(FrameRatio);
		break;
	case MM_NOAR:
		FrameRatio = VIDEO_FRAME_NOARMODE;
		::WritePrivateProfileString(_T("memory"),_T("FRAMERATIO"),_T("5"),m_cfgPath);
		m_player->put_VideoFrame(FrameRatio);
		break;
	case MM_4AR3:
		FrameRatio = VIDEO_FRAME_4AR3;
		::WritePrivateProfileString(_T("memory"),_T("FRAMERATIO"),_T("6"),m_cfgPath);
		m_player->put_VideoFrame(FrameRatio);
		break;
	case MM_16AR9:
		FrameRatio = VIDEO_FRAME_16AR9;
		::WritePrivateProfileString(_T("memory"),_T("FRAMERATIO"),_T("8"),m_cfgPath);
		m_player->put_VideoFrame(FrameRatio);
		break;
	case MM_VOLUMECHANGE:
		if(lParam)
		{
			m_pSoundBar->SetValue(m_pSoundBar->GetValue()+3);
			m_pm->SendNotify(m_pSoundBar,MST_VALUECHANGING);
		}
		else
		{
			m_pSoundBar->SetValue(m_pSoundBar->GetValue()-3);
			m_pm->SendNotify(m_pSoundBar,MST_VALUECHANGING);
		}
		break;
	case MM_SIMPLE:
		m_player->put_FullScreen(0);
		if(!bMini)
			m_pm->SendNotify(m_pSimpleBtn,MST_CLICK,0,0);
		else
			m_pm->SendNotify(m_pMiniReturn,MST_CLICK,0,0);
		break;
	case MM_PICCHANGE:
		{
			Settings *pCfg = (Settings *)lParam;
			m_player->put_hue(pCfg->hue);
			m_player->put_contrast(pCfg->lumGain);
			m_player->put_lightness(pCfg->lumOffset);
			m_player->put_saturation(pCfg->saturation);
			delete pCfg;
		}
		break;
	case MM_ADDURL:
		//char *taskid = new char[41];
		//memset(taskid,0,41);
		//m_qvod->AddTask(T2CW((LPCTSTR)lParam),taskid,0);
		//delete[] taskid;
		{
			if(pUserNode == NULL)
				pUserNode = m_list->AddNode(_T("用户列表"),_T(""));
			CString url = (LPCTSTR)lParam,name;
			name = url.Left(url.ReverseFind('|'));
			name = name.Right(name.GetLength()-name.ReverseFind('|')-1);
			CMyList::Node * pItem = m_list->AddNode(name,url,NULL,pUserNode);
			ChangeKuo(name);
			::WritePrivateProfileString(_T("用户列表"),name,url,m_listPath);
			if(pItem!=NULL)				play(pItem);
		}
		break;
	case MM_PICSET:
		{
			pDialog->m_btnPicSet->Selected(TRUE);
			PopupSetDialog();
		}
		break;
	case MM_DEFAULTMODE:
		{
			m_player->put_FullScreen(0);
			if(bMini)
				m_pm->SendNotify(m_pMiniReturn,MST_CLICK);
		}
		break;
	case MM_CUTMODE:
		{
			m_player->put_FullScreen(0);
			if(!bMini)
				m_pm->SendNotify(m_pSimpleBtn,MST_CLICK);
		}
		break;
	case MM_CLEARALL:

		m_pm->SendNotify(m_pCleanAll,MST_CLICK);

		break;
	//case MM_AUTOTESTNEW:
	//	{
	//		if(IsAutoTestUpdate)
	//		{
	//			IsAutoTestUpdate = FALSE;
	//			::WritePrivateProfileString(_T("memory"),_T("autoupdate"),_T("0"),m_cfgPath);
	//		}
	//		else
	//		{
	//			IsAutoTestUpdate = TRUE;
	//			::WritePrivateProfileString(_T("memory"),_T("autoupdate"),_T("1"),m_cfgPath);
	//		}
	//	}
	//	break;
	case MM_UPDATE:
		ShellExecute(NULL,_T("open"),m_strExeUpdate,NULL,NULL,SW_SHOWNORMAL);
		break;
	case MM_SPEED100:
		iSpeed = 10;
		::WritePrivateProfileString(_T("memory"),_T("speed"),_T("10"),m_cfgPath);
		m_qvod->SetSpeed(iSpeed);
		break;
	case MM_SPEED90:
		iSpeed = 9;
		::WritePrivateProfileString(_T("memory"),_T("speed"),_T("9"),m_cfgPath);
		m_qvod->SetSpeed(iSpeed);
		break;
	case MM_SPEED80:
		iSpeed = 8;
		::WritePrivateProfileString(_T("memory"),_T("speed"),_T("8"),m_cfgPath);
		m_qvod->SetSpeed(iSpeed);
		break;
	case MM_SPEED70:
		iSpeed = 7;
		::WritePrivateProfileString(_T("memory"),_T("speed"),_T("7"),m_cfgPath);
		m_qvod->SetSpeed(iSpeed);
		break;
	case MM_SPEED60:
		iSpeed = 6;
		::WritePrivateProfileString(_T("memory"),_T("speed"),_T("6"),m_cfgPath);
		m_qvod->SetSpeed(iSpeed);
		break;
	case MM_SPEED50:
		iSpeed = 5;
		::WritePrivateProfileString(_T("memory"),_T("speed"),_T("5"),m_cfgPath);
		m_qvod->SetSpeed(iSpeed);
		break;
	case MM_SPEED40:
		iSpeed = 4;
		::WritePrivateProfileString(_T("memory"),_T("speed"),_T("4"),m_cfgPath);
		m_qvod->SetSpeed(iSpeed);
		break;
	case MM_SPEED30:
		iSpeed = 3;
		::WritePrivateProfileString(_T("memory"),_T("speed"),_T("3"),m_cfgPath);
		m_qvod->SetSpeed(iSpeed);
		break;
	case MM_SPEED20:
		iSpeed = 2;
		::WritePrivateProfileString(_T("memory"),_T("speed"),_T("2"),m_cfgPath);
		m_qvod->SetSpeed(iSpeed);
		break;
	case MM_SPEED10:
		iSpeed = 1;
		::WritePrivateProfileString(_T("memory"),_T("speed"),_T("1"),m_cfgPath);
		m_qvod->SetSpeed(iSpeed);
		break;
	case MM_LEFTSOUND:
		m_player->put_Balance(-100);
		break;
	case MM_RIGHTSOUND:
		m_player->put_Balance(100);
		break;
	case MM_SOLIDSOUND:
		m_player->put_Balance(0);
		break;
	case MM_ADDSOUND:
		::PostMessage(g_hWnd,MSG_MENUS,MM_VOLUMECHANGE,1);
		break;
	case MM_REDUCESOUND:
		::PostMessage(g_hWnd,MSG_MENUS,MM_VOLUMECHANGE,0);
		break;
	case MM_MUTE:
		{
			long bSound;
			m_player->get_Mute(&bSound);
			if(bSound)
				m_pm->SendNotify(m_pMuteBtn,MST_CLICK);
			else
				m_pm->SendNotify(m_pSoundBtn,MST_CLICK);
		}
		break;
	case MM_TRAYOPENMAIN:
		if(::IsIconic(m_hWnd))
			::SendMessage(m_hWnd,WM_SYSCOMMAND,SC_RESTORE,0);
		ShowWindow();
		break;
	case MM_SKIN_BLACK:
		{			
			m_skinPath = m_exePath;
			m_skinPath.Replace(_T("FilmEveryday.exe"),_T("skin\\default.sk"));
			m_pm->ChangeUI(m_skinPath.GetBuffer(),_T("main.xml"), (UINT)0,  &m_lb);
			pDialog->m_pm->ChangeUI(m_skinPath.GetBuffer(),_T("dialog.xml"), (UINT)0,  NULL);
			pOpenurl->m_pm.ChangeUI(m_skinPath.GetBuffer(),_T("urldlg.xml"),(UINT)0,NULL);
			COptionUI *pCtrl = static_cast<COptionUI*>(m_pm->FindControl(_T("playerbtn")));
			if(pCtrl!=NULL && pCtrl->IsSelected())
			{
				m_pControlBar->SetFixedHeight(49);
			}
			if(!m_pHideBtn->IsTrueVisible() && m_pShowBtn->IsTrueVisible())
				m_pm->FindControl(_T("listctner"))->SetFixedWidth(11);
			m_player->put_image(MM_SKIN_BLACK);
		}
		break;
	case MM_SKIN_PURPLE:
		{

			m_skinPath = m_exePath;
			m_skinPath.Replace(_T("FilmEveryday.exe"),_T("skin\\purple.sk"));
			m_pm->ChangeUI(m_skinPath.GetBuffer(),_T("main.xml"), (UINT)0,  &m_lb);
			pDialog->m_pm->ChangeUI(m_skinPath.GetBuffer(),_T("dialog.xml"), (UINT)0,  NULL);
			pOpenurl->m_pm.ChangeUI(m_skinPath.GetBuffer(),_T("urldlg.xml"),(UINT)0,NULL);
			COptionUI *pCtrl = static_cast<COptionUI*>(m_pm->FindControl(_T("playerbtn")));
			if(pCtrl!=NULL && pCtrl->IsSelected())
			{
				m_pControlBar->SetFixedHeight(49);
			}
			if(!m_pHideBtn->IsTrueVisible() && m_pShowBtn->IsTrueVisible())
				m_pm->FindControl(_T("listctner"))->SetFixedWidth(11);
			m_player->put_image(MM_SKIN_PURPLE);
		}
		break;
	case MM_SKIN_BLUE:
		{

			m_skinPath = m_exePath;
			m_skinPath.Replace(_T("FilmEveryday.exe"),_T("skin\\blue.sk"));
			m_pm->ChangeUI(m_skinPath.GetBuffer(),_T("main.xml"), (UINT)0,  &m_lb);
			pDialog->m_pm->ChangeUI(m_skinPath.GetBuffer(),_T("dialog.xml"), (UINT)0,  NULL);
			pOpenurl->m_pm.ChangeUI(m_skinPath.GetBuffer(),_T("urldlg.xml"),(UINT)0,NULL);
			COptionUI *pCtrl = static_cast<COptionUI*>(m_pm->FindControl(_T("playerbtn")));
			if(pCtrl!=NULL && pCtrl->IsSelected())
			{
				m_pControlBar->SetFixedHeight(49);
			}
			if(!m_pHideBtn->IsTrueVisible() && m_pShowBtn->IsTrueVisible())
				m_pm->FindControl(_T("listctner"))->SetFixedWidth(11);
			m_player->put_image(MM_SKIN_BLUE);
		}
		break;
	case AM_SIZE0_5:
		{
			BOOL a;
			OnActivexMessage(0,AM_SIZE0_5,0,a);
		}
		break;
	case AM_SIZE1:
		{
			BOOL a;
			OnActivexMessage(0,AM_SIZE1,0,a);
		}
		break;
	case AM_SIZE2:
		{
			BOOL a;
			OnActivexMessage(0,AM_SIZE2,0,a);
		}
		break;
	case AM_SCREENSHOT:
		{
			BOOL a;
			OnActivexMessage(0,AM_SCREENSHOT,0,a);
		}
		break;
	case MM_OPENSCREENPATH:
		{
			TCHAR tcharpath[MAX_PATH] = {0};
			::GetPrivateProfileString(_T("memory"),_T("picpath"),_T("nopath"),tcharpath,MAX_PATH,m_cfgPath);
			CString picp = tcharpath;
			if(picp == _T("nopath"))
				break;
			ShellExecute(NULL,_T("open"),picp,NULL,NULL,SW_SHOWDEFAULT);
		}
		break;
	case MM_SCREENCUTSET:
		pDialog->m_btnPathSet->Selected(TRUE);
		PopupSetDialog();
		break;
	}
	return LRESULT(1);
}

LRESULT CMainWnd::OnActivexMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	RECT rect;
	::GetWindowRect(m_hWnd,&rect);
	switch(wParam)
	{
	case AM_PLAY:
		m_pm->SendNotify(m_pPlayBtn,MST_CLICK);
		break;
	case AM_PAUSE:
		m_pm->SendNotify(m_pPauseBtn,MST_CLICK);
		break;
	case AM_SLIDERVALUECHANGE:
		m_pSlider->SetValue(lParam);
		m_pm->SendNotify(m_pSlider,MST_VALUECHANGED);
		break;
	case AM_SOUNDVALUECHANGE:
		m_pSoundBar->SetValue(lParam);
		m_pm->SendNotify(m_pSoundBar,MST_VALUECHANGING);
		break;
	case AM_SIZE0_5:
		{
			m_player->put_FullScreen(0);
			long x,y;
			HWND hWnd;
			m_player->GetHWND((long*)&hWnd);
			RECT rc;
			::GetWindowRect(hWnd,&rc);
			m_player->GetSourceSize(&x,&y);
			rect.right+=(x/2 - rc.right + rc.left);
			rect.bottom+=(y/2 - rc.bottom + rc.top);
			MoveWindow(&rect);
			CenterWindow();
		}
		break;
	case AM_SIZE1:
		{
			m_player->put_FullScreen(0);
			long x,y;
			HWND hWnd;
			m_player->GetHWND((long*)&hWnd);
			RECT rc;
			::GetWindowRect(hWnd,&rc);
			m_player->GetSourceSize(&x,&y);
			rect.right+=(x - rc.right + rc.left);
			rect.bottom+=(y - rc.bottom + rc.top);
			MoveWindow(&rect);
			CenterWindow();
		}
		break;
	case AM_SIZE2:
		{
			m_player->put_FullScreen(0);
			long x,y;
			HWND hWnd;
			m_player->GetHWND((long*)&hWnd);
			RECT rc;
			::GetWindowRect(hWnd,&rc);
			m_player->GetSourceSize(&x,&y);
			rect.right+=(x*2 - rc.right + rc.left);
			rect.bottom+=(y*2 - rc.bottom + rc.top);
			MoveWindow(&rect);
			CenterWindow();
		}
		break;
	case AM_TSUYOKU:
		m_player->put_tsuyoku(TRUE);
		::WritePrivateProfileString(_T("memory"),_T("tsuyoku"),_T("1"),m_cfgPath);
		m_bTsuyoku = TRUE;
		break;
	case AM_TSUYOKUNAYI:
		m_player->put_tsuyoku(FALSE);
		::WritePrivateProfileString(_T("memory"),_T("tsuyoku"),_T("0"),m_cfgPath);
		m_bTsuyoku = FALSE;
		break;
	case AM_SCREENSHOT:
		{
			TCHAR tcharpath[MAX_PATH] = {0};
			::GetPrivateProfileString(_T("memory"),_T("picpath"),_T("nopath"),tcharpath,MAX_PATH,m_cfgPath);
			CString picp = tcharpath;
			if(picp == _T("nopath"))
				break;
			::CreateDirectory(picp,NULL);
			DWORD tick = ::GetTickCount();
			CString strtick;
			strtick.Format(_T("%d.bmp"),tick);
			picp+=strtick;
			BSTR path = picp.AllocSysString();
			m_player->GetPic(path,NULL);	
			::SysFreeString(path);
		}
		break;
	case AM_TOPMOST:
		TopType = 2;
		::WritePrivateProfileString(_T("memory"),_T("top"),_T("2"),m_cfgPath);
		::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		break;
	case AM_NOTOPMOST:
		TopType = 3;
		::WritePrivateProfileString(_T("memory"),_T("top"),_T("3"),m_cfgPath);
		::SetWindowPos(m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		break;
	case AM_MINBUTTON:
		m_pm->SendNotify(m_pMinBtn,MST_CLICK);
		break;
	case AM_CLOSE:
		m_pm->SendNotify(m_pCloseBtn,MST_CLICK);
		break;
	}
	return LRESULT(1);
}