// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <objbase.h>
#include <shlobj.h>
#include <Commdlg.h>
#include <exdisp.h>
#include <MsHtmcid.h>
#include <Mshtml.h>
#include <shellapi.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <comutil.h>
#include "resource.h"
//#include <atlstr.h>
#include "MPlayer_h.h"
#include "UIlib.h"

#ifdef _DEBUG
#pragma comment(lib, "lib_d.lib")
#else
#pragma comment(lib, "lib_r.lib")
#endif


#define MSG_TRAY		WM_USER+1
#define MSG_MENUS		WM_USER+200
#define MSG_ACVICEX		WM_USER+400
#define WM_FILEASSOCIATIONS  1288 


#define MM_COMPLETE		WM_USER+125
#define MM_CONTEXTMENU	WM_USER+126
#define MM_LBUTTONDOWN	WM_USER+127
#define MM_FULLCHANGE	WM_USER+135

#define MM_IE_GOBACK	 100
#define MM_IE_FORWORD	 101
#define MM_IE_REFRESH	 102
#define MM_IE_COPY		 103
#define MM_IE_PASTE	     104
#define MM_OFFSITE	     105
#define MM_OPENFILE	     106
#define MM_OPENURL	     107
#define MM_SET		     108
#define MM_PLAY		     109
#define MM_CONNECT	     110
#define MM_BREAK	     111
#define MM_CONNECTALL	 112
#define MM_BREAKALL	     113
#define MM_DELETE	     114
#define MM_FINDFILE	     115
#define MM_ONE		     116
#define MM_ONECYCLE	     117
#define MM_SORT		     118
#define MM_SORTCYCLE	 119
#define MM_RAND		     120
#define MM_ABOUT	     121
#define MM_QUIT		     122
#define MM_CHANGESET	 123
#define MM_SHOW		     124

#define MM_PLAYPAUSE		128
#define MM_STOP				129
#define MM_PREV				130
#define MM_NEXT				131
#define MM_FULL				132
#define MM_LIST				133


#define MM_AUTOPOWER		136
#define MM_AR				137
#define MM_NOAR				138
#define MM_4AR3				139
#define MM_16AR9			140
#define MM_VOLUMECHANGE		141
#define MM_SIMPLE			142
#define MM_OPENOFFSET		143
#define MM_PICSET			145
#define MM_PICCHANGE		150
#define MM_ADDURL			151
#define MM_TIMEOFF			152
#define MM_NETLIMIT			153
//#define MM_AUTOTESTNEW		154
#define MM_SPEED100			155
#define MM_SPEED90			156
#define MM_SPEED80			157
#define MM_SPEED70			158
#define MM_SPEED60			159
#define MM_SPEED50			160
#define MM_SPEED40			161
#define MM_SPEED30			162
#define MM_SPEED20			163
#define MM_SPEED10			164
#define MM_PLAYMODE			165
#define MM_CLEARALL			166
#define MM_DEFAULTMODE		167
#define MM_CUTMODE			168
#define MM_PLAYSET			169
#define MM_PLAYLIST			170

#define MM_PLAYUP			134
#define MM_NOTUP			171
#define MM_ALWAYSUP			172 //top

#define MM_COLORSET			173
#define MM_LEFTSOUND		174
#define MM_RIGHTSOUND		175
#define MM_SOLIDSOUND		176
#define MM_ADDSOUND			177
#define MM_REDUCESOUND		178
#define MM_MUTE				179

#define MM_DELFILEANDTASK	180
#define MM_DELTASKONLY		181
#define MM_TRAYOPENMAIN		182
#define MM_TRAYSET			183
#define	MM_TRAYOFFSITE		184
#define	MM_TRAYABOUT		185
#define MM_TRAYQUIT			186

#define MM_OFFSITEMAIN		187

#define MM_SKIN_BLACK       200
#define MM_SKIN_PURPLE      201
#define MM_SKIN_BLUE	    202

#define MM_UPDATE			203

//activex messages
#define AM_FULLSCREEN 300
#define AM_SIMPLIFY   301
#define AM_SIZE0_5    302
#define AM_SIZE1      303
//#define AM_SIZE1_5    304
#define AM_SIZE2      305
#define AM_SCREENSHOT 306
#define AM_TOPMOST    307
#define AM_NOTOPMOST  308
#define AM_MINBUTTON  309
#define AM_CLOSE      310
#define AM_RETURN     311
#define AM_TSUYOKU	  312
#define AM_TSUYOKUNAYI 313

#define MM_OPENSCREENPATH	314
#define MM_SCREENCUTSET	315

#define AM_SLIDERVALUECHANGE	WM_USER+401
#define AM_SOUNDVALUECHANGE		WM_USER+402

#define AM_PLAY					WM_USER+403
#define	AM_PAUSE				WM_USER+404

#define FRAMERATIO int
#define NORMAL 1
#define VMR7 2
#define VMR9 3
#define EVR 4
#define VIDEO_FRAME_ARMODE 4	//视频框架为保持原来比例
#define VIDEO_FRAME_NOARMODE 5	//拉伸
#define VIDEO_FRAME_4AR3 6		//保持4：3
#define VIDEO_FRAME_5AR4 7		//保持5：4
#define VIDEO_FRAME_16AR9 8		//保持16：9

typedef struct _tagCFG
{
public: 
	int lumGain,lumOffset,gammaCorrection;
	static const int lumGainDef=128,lumOffsetDef=0,gammaCorrectionDef=100;
	int hue,saturation;
	static const int hueDef=0,saturationDef=64;
}Settings;
// TODO: 在此处引用程序需要的其他头文件
