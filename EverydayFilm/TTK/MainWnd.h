#pragma once
#include "MyList.h"
#include "QvodCtrl.h"
#include "SetDialog.h"
#include "FileManager.h"
#include "WaitPoweroff.h"
#include "COpenUrl.h"
#include "PopSkin.h"
#include <atlstr.h>
#include "CRC.h"

#define WM_ADD WM_USER+100
#define WM_CANSET WM_USER+101
#define WM_SENDEXE WM_USER+110
#define WM_ERROR_PLAY WM_USER+111


#define MODE int

typedef enum _PlayMode
{
	oneplay = 0,
	onecycle = (oneplay+1),
	sort	 = (onecycle+1),
	sortcycle= (sort+1)
	//random = (sortcycle+1)
} PLAYMODE;
//need to reset

//need to reset
#define  MAX_IDE_DRIVES  4
#define  IDENTIFY_BUFFER_SIZE  512

//  IOCTL commands
#define  DFP_GET_VERSION          0x00074080
#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088

//  Valid values for the bCommandReg member of IDEREGS.
#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.

#define  SENDIDLENGTH  sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE

#define  FILE_DEVICE_SCSI              0x0000001b
#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ((FILE_DEVICE_SCSI << 16) + 0x0501)
#define  IOCTL_SCSI_MINIPORT 0x0004D008  //  see NTDDSCSI.H for definition
//need to reset

typedef struct _IDSECTOR
{
	USHORT  wGenConfig;
	USHORT  wNumCyls;
	USHORT  wReserved;
	USHORT  wNumHeads;
	USHORT  wBytesPerTrack;
	USHORT  wBytesPerSector;
	USHORT  wSectorsPerTrack;
	USHORT  wVendorUnique[3];
	CHAR    sSerialNumber[20];
	USHORT  wBufferType;
	USHORT  wBufferSize;
	USHORT  wECCSize;
	CHAR    sFirmwareRev[8];
	CHAR    sModelNumber[40];
	USHORT  wMoreVendorUnique;
	USHORT  wDoubleWordIO;
	USHORT  wCapabilities;
	USHORT  wReserved1;
	USHORT  wPIOTiming;
	USHORT  wDMATiming;
	USHORT  wBS;
	USHORT  wNumCurrentCyls;
	USHORT  wNumCurrentHeads;
	USHORT  wNumCurrentSectorsPerTrack;
	ULONG   ulCurrentSectorCapacity;
	USHORT  wMultSectorStuff;
	ULONG   ulTotalAddressableSectors;
	USHORT  wSingleWordDMA;
	USHORT  wMultiWordDMA;
	BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;

typedef struct _SRB_IO_CONTROL
{
	ULONG HeaderLength;
	UCHAR Signature[8];
	ULONG Timeout;
	ULONG ControlCode;
	ULONG ReturnCode;
	ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;

//  IDE registers
typedef struct _IDEREGS
{
	BYTE bFeaturesReg;       // Used for specifying SMART "commands".
	BYTE bSectorCountReg;    // IDE sector count register
	BYTE bSectorNumberReg;   // IDE sector number register
	BYTE bCylLowReg;         // IDE low order cylinder value
	BYTE bCylHighReg;        // IDE high order cylinder value
	BYTE bDriveHeadReg;      // IDE drive/head register
	BYTE bCommandReg;        // Actual IDE command.
	BYTE bReserved;          // reserved for future use.  Must be zero.
} IDEREGS, *PIDEREGS, *LPIDEREGS;

//  GETVERSIONOUTPARAMS contains the data returned from the 
//  Get Driver Version function.
typedef struct _GETVERSIONOUTPARAMS
{
	BYTE bVersion;      // Binary driver version.
	BYTE bRevision;     // Binary driver revision.
	BYTE bReserved;     // Not used.
	BYTE bIDEDeviceMap; // Bit map of IDE devices.
	DWORD fCapabilities; // Bit mask of driver capabilities.
	DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

//  SENDCMDINPARAMS contains the input parameters for the 
//  Send Command to Drive function.
typedef struct _SENDCMDINPARAMS
{
	DWORD     cBufferSize;   //  Buffer size in bytes
	IDEREGS   irDriveRegs;   //  Structure with drive register values.
	BYTE bDriveNumber;       //  Physical drive number to send 
	//  command to (0,1,2,3).
	BYTE bReserved[3];       //  Reserved for future expansion.
	DWORD     dwReserved[4]; //  For future use.
	BYTE      bBuffer[1];    //  Input buffer.
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;

// Status returned from driver
typedef struct _DRIVERSTATUS
{
	BYTE  bDriverError;  //  Error code from driver, or 0 if no error.
	BYTE  bIDEStatus;    //  Contents of IDE Error register.
	//  Only valid when bDriverError is SMART_IDE_ERROR.
	BYTE  bReserved[2];  //  Reserved for future expansion.
	DWORD  dwReserved[2];  //  Reserved for future expansion.
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;

// Structure returned by PhysicalDrive IOCTL for several commands
typedef struct _SENDCMDOUTPARAMS
{
	DWORD         cBufferSize;   //  Size of bBuffer in bytes
	DRIVERSTATUS  DriverStatus;  //  Driver status structure.
	BYTE          bBuffer[1];    //  Buffer of arbitrary length in which to store the data read from the                                                       // drive.
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

//need to reset

static char B64[64] =
{
	'A','B','C','D','E','F','G','H',
	'I','J','K','L','M','N','O','P',
	'Q','R','S','T','U','V','W','X',
	'Y','Z',
	'a','b','c','d','e','f','g','h',
	'i','j','k','l','m','n','o','p',
	'q','r','s','t','u','v','w','x',
	'y','z',
	'0','1','2','3','4','5','6','7',
	'8','9','+','/'
};

class ad
{
public:
	CString name;
	CString url;
};

class CMainWnd : public CWindowWnd, public INotifyUI
{
public:
	CMainWnd(void);
	~CMainWnd(void);
	CRITICAL_SECTION cs;
	std::vector<ad> ad_list;
public:
	CPaintManagerUI* m_pm;
	//获得系统版本
	OSVERSIONINFO OsVersion;
	CString HomePage;
	CString m_strComm;
private:
	
	CButtonUI* m_pChangeSkin;
	CButtonUI* m_pSimpleBtn;//精简
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn,*m_pRestoreBtn;//最大化，还原
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;//主菜单
	CButtonUI* m_pPlayBtn,*m_pPauseBtn;//播放暂停
	CButtonUI* m_pStopBtn;
	CButtonUI* m_pCleanAll;//列表中删除全部
	CButtonUI* m_pSoundBtn,*m_pMuteBtn;//静音,非静音
	CButtonUI* m_pLeftBtn,*m_pRightBtn;//前一个，后一个
	CButtonUI* m_pLstBtn,*m_pLstBtn2;//list 伸缩
	CButtonUI* m_pHideBtn,*m_pShowBtn;//list隐藏显示
	CButtonUI* m_pFullScreenBtn;
	CButtonUI* m_pOpenFileBtn;
	CButtonUI* m_pTextBtn;//播放列表上面的字
	CButtonUI* m_pMiniReturn;
	CVerticalLayoutUI* m_pControlBar;
	CTextUI*   m_pStatusText,*m_pSpeedText,*m_pAdText;
	CSliderUI* m_pSoundBar;
	CSliderUI* m_pSlider;
	CComQIPtr<IWebBrowser2> m_home,m_apply;
	CComQIPtr<IMMPlayer> m_player;
	CListBuilder m_lb;
	CMyList *m_list;
	CMyList::Node* pUserNode;
	QvodCtrl *m_qvod;
	LPTASK_ITEM m_itemCurrentPlay;
	CMyList::Node* pPlayingNode,*pLastPlayingNode;
	CMyList::Node* pLocalNode;
	CSetDialog *pDialog;
	CFileManager *pFileManager;
	CWaitPoweroff *pPowerWnd;
	COpenUrl *pOpenurl;
	CPopSkin* m_pSkin;
public:
	CString m_exePath,m_listPath,m_cfgPath,m_qvodPath,m_skinPath;//主程序路径
	CString m_strExeUpdate;
	CString m_sknSet;
	BOOL m_bMinpause;

	CMenuUI* m_pMainMenu,*m_pTrayMenu,*m_pPlayMenu,*m_pListMenu,*m_pIeMenu;// 菜单

	std::vector<int> t;

	BOOL IsListLoaded;//列表是否已经加载
	BOOL IsClose,//关闭后退出
		IsRemember,//记录播放位置
		IsClickPause,//单击暂停
		IsHidePause;//隐藏暂停
	FRAMERATIO FrameRatio;
	int RenderType;
	int TopType;
	int AutoPoweroff;
	int iSpeed;
	BOOL bMini;//0 NOMINI; 1 MINI; 2 DRUMP;
	MODE PlayMode;
	NOTIFYICONDATA m_nd; //系统托盘图标

public:
	void Init(void);
	void OnPrepare(void);
	void Notify(TNotifyUI& msg);
	UINT GetClassStyle() const;
	LPCTSTR GetWindowClassName() const ;
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM  lParam, BOOL& bHandled); 
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM  lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM  lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM  lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM  lParam, BOOL& bHandled);
	LRESULT OnAdd(UINT uMsg,WPARAM wParam, LPARAM lParam,BOOL& bHandled);
	LRESULT OnSetTimer(UINT uMsg,WPARAM wParam, LPARAM lParam,BOOL& bHandled);
	LRESULT OnActivexMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnMouseWhell(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnExeMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnCopyData(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnErrorPlay(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void StartQvod(void);
	HRESULT play(CMyList::Node *pNode);
	CString Time2Str(long second);
	CString BrowseFolder();
	void HomeCpy(void);
	void HomePaste(void);
	void PopupSetDialog(void);
	char* base64_encode(char *s);
	void ReadCfg(void);
	void WriteCfg(void);
	void SetAutoRun(void);
	void DeleteAutoRun(void);
	LRESULT OnSysTrayNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void SafeQuit(void);
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void SafeList(void);
	void LoadList(void);
	void ChangeKuo(CString& str);
	void ChangeToKuo(CString& str);
	void StopPlaying(void);
	LRESULT OnPlayComplete(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnContext(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPlayLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnScreenChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	DWORD CMainWnd::FindProcessID(CString szExe);
	void DisableScrSaver(void);
	void EnableScrSaver(void);
	void DisablePmmSaver(void);
	void EnablePmmSaver(void);
	void PowerOff(BOOL bPoweroff);
	void DeleteItem(CMyList::Node* node);
	BOOL IsFileExist(CString strFile);
private:
	BOOL bSuo;
	BOOL bStartNext;
	BOOL bApplyNavigated,bDelstartup,bQuitAcc,m_bTsuyoku;
	UINT m_nLowpowerTimeout,m_nPoweroffTimeout;

	int m_timer;

	UINT m_iSkin;

	BOOL bFullScreen;
public:
	// 现在的ad id
	LRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//need to reset///////////////////////
	BYTE IdOutCmd [sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];
	BOOL SetSomething(CStringA& str);
	CString URLEncode(CStringA sIn);
	inline BYTE toHex(const BYTE &x){return x > 9 ? x + 55: x + 48;};
	int ReadIdeDriveAsScsiDriveInNT (char *out);
	int ReadPhysicalDriveInNT (char *out);
	char *ConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex);
	BOOL DoIDENTIFY (HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
		PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
		PDWORD lpcbBytesReturned);
	void SendHttpMsg();
	//need to reset/////////////////////////
	PROCESS_INFORMATION accPi;
	STARTUPINFO accSi;
	HWND hWndAcc;
};
