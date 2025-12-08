////////////////////////////////////////////////////////////////////////////////
//
// File Name    : wince_fixes.h
// Description  : Various fixes to enable compilation under Windows CE
// Information  : Used by Orpheus 2, Sam 2 and Mars6
//
// Copyright (C) 2002-2005 Dolphin Oceanic Ltd.
//
////////////////////////////////////////////////////////////////////////////////
// Version History:
//
// $Log: /Library/wince_fixes.h $
// 
// 26    26/07/07 14:16 Druck
// _splitpath and _makepath changed to _t variants for SAM
// 
// 25    28/11/06 15:55 Druck
// WS_OVERLAPPEDWINDOW definition WM5 exclusion removed
// 
// 24    24/10/06 15:55 Druck
// Unpriviledged certificate store used
// 
// 23    24/10/06 11:07 Mike
// 
// 22    10/10/06 12:05 Druck
// aygshell included for smartphone
// 
// 21    23/05/06 16:57 Druck
// Smartphone UI added
// 
// 20    27/03/06 8:23 Malcolm
// 
// 19    14/02/06 11:46 Mike
// Fixes for VS2005
// 
// 18    27/01/06 9:08 Ian
// TTTOOLINFOW and TTHITTESTINFOW are declared in the SDK shipped with
// Visual Studio 2005.
// 
// 17    9/08/05 11:54 Mike
// Added GetWindowsDirectory() API
// 
// 16    3/05/05 9:31 John
// Added IsBadStringPtr() - implement with call to IsBadReadPtr()
// 
// 15    28/04/05 11:47 John
// Didn't need previous header comment, and copyright date -2005.
// 
// 14    28/04/05 11:42 John
// Adjustments for toolbar enumeration in CE only apply to PPC
// 
// 13    6/04/05 13:33 Mike
// Adjustments for toolbar enumeration in CE
// 
// 12    3/03/05 9:44 Mike
// 
// 11    14/06/04 9:37 Druck
// CE Flag for full screen property sheets defined under Win32
// 
// 10    16/04/04 14:27 Druck
// WM_ICONERASEBKGND added
// 
// 9     29/03/04 15:10 Druck
// WM_INITMENU added
// 
// 8     22/03/04 16:17 Druck
// Single and double quotes stripped in GetPrivateProfileString()
// OpenSemaphore() added
// Standard C Library time() added
// 
// 7     6/02/04 11:54 Druck
// MIIM_FTYPE and MIIM_STRING removed
// 
// 6     19/12/03 14:15 Druck
// GetPrivateProfileStringW alias added
// CreateFileForMapping alias added for Win32
// 
// 5     1/12/03 10:04 Druck
// IsWindowUnicode added
// 
// 4     25/11/03 9:58 Druck
// Additions for Supernova and WinCE version 4
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef _wince_fixes_h
#define _wince_fixes_h


// === Missing WinCE Functions =================================================

#ifdef _WIN32_WCE

// --- C runtime ---------------------------------------------------------------

#ifndef getenv
_TCHAR *getenv(const _TCHAR *varname);
#endif
#ifndef _wgetenv
wchar_t *_wgetenv(const wchar_t *varname);
#endif

// --- Windows WinUser.h -------------------------------------------------------


// missing constants



// window styles


#ifndef WS_MINIMIZE
#define WS_MINIMIZE				0x20000000L
#endif
#ifndef WS_MAXIMIZE
#define WS_MAXIMIZE				0x10000000L
#endif
#ifndef WS_THICKFRAME
#define WS_THICKFRAME			0x00040000L
#endif
#ifndef WS_OVERLAPPEDWINDOW
#define WS_OVERLAPPEDWINDOW		(WS_SYSMENU)
#endif
#ifndef WS_EX_NOPARENTNOTIFY
#define WS_EX_NOPARENTNOTIFY    0x00000004L
#endif
#ifndef WS_EX_ACCEPTFILES
#define WS_EX_ACCEPTFILES       0x00000010L
#endif
#ifndef WS_EX_TRANSPARENT
#define WS_EX_TRANSPARENT		0x00000020L
#endif
#ifndef WS_EX_MDICHILD
#define WS_EX_MDICHILD          0x00000040L
#endif
#ifndef WS_EX_RIGHT
#define WS_EX_RIGHT             0x00001000L
#endif
#ifndef WS_EX_RTLREADING
#define WS_EX_RTLREADING        0x00002000L
#endif
#ifndef WS_EX_LEFTSCROLLBAR
#define WS_EX_LEFTSCROLLBAR     0x00004000L
#endif
#ifndef WS_EX_CONTROLPARENT
#define WS_EX_CONTROLPARENT     0x00010000L
#endif
#ifndef WS_EX_APPWINDOW
#define WS_EX_APPWINDOW         0x00040000L
#endif
#ifndef WS_EX_LAYOUTRTL
#define WS_EX_LAYOUTRTL         0x00400000L
#endif
#ifndef WS_EX_NOINHERITLAYOUT
#define WS_EX_NOINHERITLAYOUT   0x00100000L
#endif
#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#endif
#ifndef WS_EX_NOACTIVATE
#define WS_EX_NOACTIVATE        0x08000000L
#endif

// control styles
#ifndef DS_ABSALIGN
#define DS_ABSALIGN         0x01L
#endif
#ifndef DS_SYSMODAL
#define DS_SYSMODAL         0x02L
#endif
#ifndef DS_FIXEDSYS
#define DS_FIXEDSYS         0x0008L
#endif
#ifndef DS_NOIDLEMSG
#define DS_NOIDLEMSG        0x100L  /* WM_ENTERIDLE message will not be sent */
#endif
#ifndef DS_NOFAILCREATE
#define DS_NOFAILCREATE     0x0010L
#endif
#ifndef DS_CENTERMOUSE
#define DS_CENTERMOUSE      0x1000L
#endif
#ifndef DS_CONTEXTHELP
#define DS_CONTEXTHELP      0x2000L
#endif

#ifndef BS_ICON
#define BS_ICON				0x00000040L
#endif
#ifndef BS_BITMAP
#define BS_BITMAP           0x00000080L
#endif
#ifndef BS_FLAT
#define BS_FLAT             0x00008000L
#endif

#ifndef SS_SIMPLE
#define SS_SIMPLE           0x0000000BL
#endif
#ifndef SS_TYPEMASK
#define SS_TYPEMASK         0x0000001FL
#endif
#ifndef SS_RIGHTJUST
#define SS_RIGHTJUST        0x00000400L
#endif
#ifndef SS_REALSIZEIMAGE
#define SS_REALSIZEIMAGE    0x00000800L
#endif
#ifndef SS_SUNKEN
#define SS_SUNKEN           0x00001000L
#endif
#ifndef SS_ELLIPSISMASK
#define SS_ELLIPSISMASK     0x0000C000L
#endif

#ifndef LBS_OWNERDRAWFIXED
#define LBS_OWNERDRAWFIXED    0x0010L
#endif
#ifndef LBS_OWNERDRAWVARIABLE
#define LBS_OWNERDRAWVARIABLE 0x0020L
#endif

#ifndef CBS_OWNERDRAWFIXED
#define CBS_OWNERDRAWFIXED    0x0010L
#endif
#ifndef CBS_OWNERDRAWVARIABLE
#define CBS_OWNERDRAWVARIABLE 0x0020L
#endif

// message box styles
#ifndef MB_SYSTEMMODAL
#define MB_SYSTEMMODAL		0
#endif
#ifndef MB_TASKMODAL
#define MB_TASKMODAL		0
#endif
#ifndef MB_RTLREADING
#define MB_RTLREADING		0
#endif
#ifndef MB_RIGHT
#define MB_RIGHT			0
#endif

// messages
#ifndef WM_QUERYENDSESSION
#define WM_QUERYENDSESSION  0x0011
#endif
#ifndef WM_ENDSESSION
#define WM_ENDSESSION       0x0016
#endif
#ifndef WM_ICONERASEBKGND
#define WM_ICONERASEBKGND   0x0027
#endif

#ifndef WM_DISPLAYCHANGE
#define WM_DISPLAYCHANGE    0x007E
#endif
#ifndef WM_INITMENU
#define WM_INITMENU         0x0116
#endif
// misc
#ifndef SW_SHOWMINIMIZED
#define SW_SHOWMINIMIZED	0
#endif 
#ifndef TPM_RIGHTBUTTON
#define TPM_RIGHTBUTTON		0
#endif
#ifndef QS_HOTKEY
#define QS_HOTKEY			0x0080
#endif
#ifndef RDW_INVALIDATE
#define RDW_INVALIDATE		(0x0001)
#endif
#ifndef RDW_ALLCHILDREN
#define RDW_ALLCHILDREN		(0x0080)
#endif
#ifndef RDW_UPDATENOW
#define RDW_UPDATENOW		(0x0100)
#endif
#ifndef WPF_RESTORETOMAXIMIZED
#define WPF_RESTORETOMAXIMIZED	0
#endif
#ifndef SMTO_ABORTIFHUNG
#define SMTO_ABORTIFHUNG	SMTO_NORMAL
#endif
#ifndef SM_CXSIZEFRAME
#define SM_CXSIZEFRAME      32
#endif
#ifndef DEFAULT_GUI_FONT	// as wceimpl.h
#define DEFAULT_GUI_FONT    SYSTEM_FONT
#endif
#ifndef SBS_SIZEBOX
#define SBS_SIZEBOX     0x0008L
#endif
#ifndef TCS_TOOLTIPS
#define TCS_TOOLTIPS            0x4000
#endif
#ifndef ACS_CENTER
#define ACS_CENTER              0x0001
#endif
#ifndef ACS_TRANSPARENT
#define ACS_TRANSPARENT         0x0002
#endif
#ifndef ACS_AUTOPLAY
#define ACS_AUTOPLAY            0x0004
#endif



// structures
typedef struct _WINDOWPLACEMENT {
    UINT length;
    UINT flags;
    UINT showCmd;
    POINT ptMinPosition;
    POINT ptMaxPosition;
    RECT rcNormalPosition;
} WINDOWPLACEMENT;

// missing macros
#ifndef IsDlgButtonChecked
#define IsDlgButtonChecked(hDlg, nIDButton)		SendDlgItemMessage(hDlg, nIDButton, BM_GETCHECK, 0,      0)
#endif
#ifndef CheckDlgButton
#define CheckDlgButton(hDlg, nIDButton, uCheck)	SendDlgItemMessage(hDlg, nIDButton, BM_SETCHECK, uCheck ,0)
#endif

// functions
#ifndef GetTopWindow
HWND GetTopWindow(HWND hWnd);
#endif 

#ifndef GetMessageTime
LONG GetMessageTime(VOID);
#endif

#ifndef GetWindowPlacement
BOOL GetWindowPlacement(HWND hWnd, WINDOWPLACEMENT *lpwndpl);
#endif

#ifndef GetMenuItemCount
int GetMenuItemCount(HMENU hMenu);
#endif

#ifndef GetMenuString
int GetMenuString(HMENU hMenu, UINT uIDItem, LPTSTR lpString, int nMaxCount, UINT uFlag);
#endif

#ifndef GetMenuState
UINT GetMenuState(HMENU hMenu, UINT uId, UINT uFlags);
#endif

#ifndef IsWindowUnicode
#define IsWindowUnicode(hWnd)	(TRUE)		// All windows Unicode under CE
#endif

// --- Windows Winbase.h -------------------------------------------------------

// For CreateProcess()
#ifndef NORMAL_PRIORITY_CLASS
#define NORMAL_PRIORITY_CLASS	0
#endif

// For MapViewOfFile
#ifndef FILE_MAP_COPY
#define FILE_MAP_COPY FILE_MAP_WRITE
#endif

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

DWORD GetPrivateProfileString(
    LPCTSTR lpAppName,
    LPCTSTR lpKeyName,
    LPCTSTR lpDefault,
    LPTSTR  lpReturnedString,
    DWORD   nSize,
    LPCTSTR lpFileName
    );
#define GetPrivateProfileStringW GetPrivateProfileString

UINT GetPrivateProfileInt(
	LPCTSTR lpAppName,
	LPCTSTR lpKeyName,
	INT     nDefault,
	LPCTSTR lpFileName
    );

BOOL WritePrivateProfileString(
    LPCTSTR lpAppName,
    LPCTSTR lpKeyName,
    LPCTSTR lpString,
    LPCTSTR lpFileName
    );
#define WritePrivateProfileStringW WritePrivateProfileString

UINT GlobalGetAtomName(ATOM nAtom, LPTSTR lpBuffer, int nSize);

HANDLE OpenSemaphore(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName);

// Windows CE version 3 only 
#if(_WIN32_WCE < 400)

BOOL DuplicateHandle(
  HANDLE hSourceProcessHandle,
  HANDLE hSourceHandle,
  HANDLE hTargetProcessHandle,
  LPHANDLE lpTargetHandle,
  DWORD dwDesiredAccess,
  BOOL bInheritHandle,
  DWORD dwOptions
);

void ExitProcess(UINT uExitCode);
ATOM WINAPI GlobalFindAtom(LPCTSTR lpString);

#endif // _WIN32_WCE < 400

#ifndef FreeResource
#define FreeResource(x)
#endif
#ifndef UnlockResource
#define UnlockResource(x)
#endif
#ifndef MulDiv
#define MulDiv(a,b,c) (a*b/c)
#endif

BOOL	IsBadStringPtr( LPCTSTR lpsz, UINT_PTR ucchMax );
#define	IsBadStringPtrW IsBadStringPtr

// --- WinCE4 mmsystem.h -------------------------------------------------------

#if(_WIN32_WCE < 400)
typedef DWORD           FOURCC;         /* a four character code */

/* standard four character codes */
#define FOURCC_RIFF     mmioFOURCC('R', 'I', 'F', 'F')

/* RIFF chunk information data structure */
typedef struct _MMCKINFO
{
        FOURCC          ckid;           /* chunk ID */
        DWORD           cksize;         /* chunk size */
        FOURCC          fccType;        /* form type or list type */
        DWORD           dwDataOffset;   /* offset of data portion of chunk */
        DWORD           dwFlags;        /* flags used by MMIO functions */
} MMCKINFO, *PMMCKINFO, NEAR *NPMMCKINFO, FAR *LPMMCKINFO;
typedef const MMCKINFO *LPCMMCKINFO;
#endif // _WIN32_WCE < 400

// --- SDK stdlib.h ------------------------------------------------------------

#ifndef _MAX_FNAME
#define _MAX_FNAME  256	/* max. length of file name component */
#endif
#ifndef	_MAX_DIR
#define _MAX_DIR    256 /* max. length of path component */
#endif
#ifndef	_MAX_DRIVE
#define _MAX_DRIVE  3   /* max. length of drive component */
#endif

void _tsplitpath( const _TCHAR *path, _TCHAR *drive, _TCHAR *dir, _TCHAR *fname, _TCHAR *ext );
void _tmakepath( _TCHAR *path, const _TCHAR *drive, const _TCHAR *dir, const _TCHAR *fname, const _TCHAR *ext );

// --- crtdbg.h ----------------------------------------------------------------

#ifndef _ASSERT
#define _ASSERT ASSERT
#endif

// --- wingdi.h ----------------------------------------------------------------

#ifndef PALETTEINDEX
#define PALETTEINDEX(i)     ((COLORREF)(0x01000000 | (DWORD)(WORD)(i)))
#endif

#ifndef OUT_TT_PRECIS
#define OUT_TT_PRECIS		0	// always true type on CE
#endif
#ifndef OUT_TT_ONLY_PRECIS
#define OUT_TT_ONLY_PRECIS	0	// always true type on CE
#endif

#ifndef CreateFont
HFONT CreateFont(
  int nHeight,               // height of font
  int nWidth,                // average character width
  int nEscapement,           // angle of escapement
  int nOrientation,          // base-line orientation angle
  int fnWeight,              // font weight
  DWORD fdwItalic,           // italic attribute option
  DWORD fdwUnderline,        // underline attribute option
  DWORD fdwStrikeOut,        // strikeout attribute option
  DWORD fdwCharSet,          // character set identifier
  DWORD fdwOutputPrecision,  // output precision
  DWORD fdwClipPrecision,    // clipping precision
  DWORD fdwQuality,          // output quality
  DWORD fdwPitchAndFamily,   // pitch and family
  LPCTSTR lpszFace           // typeface name
);
#endif

// --- richedit.h --------------------------------------------------------------

#ifndef _RICHEDIT_
typedef struct _charrange
{
	LONG	cpMin;
	LONG	cpMax;
} CHARRANGE;
#endif 

// -----------------------------------------------------------------------------

// keyboard handling
#define MapVirtualKeyA error_error // use wince_keybd_fixes.h and wince_keybd_fixes.cpp
#define MapVirtualKeyW error_error // use wince_keybd_fixes.h and wince_keybd_fixes.cpp

// -----------------------------------------------------------------------------

// tooltip headers - missing from PPC sdk, but available in standard SDK

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)

#if(_MSC_VER < 1400)		// if we are using Visual Studio 2005 these *are* defined
typedef struct tagTOOLINFOW {
    UINT cbSize;
    UINT uFlags;
    HWND hwnd;
    UINT_PTR uId;
    RECT rect;
    HINSTANCE hinst;
    LPWSTR lpszText;
    LPARAM lParam;
} TTTOOLINFOW, NEAR *PTOOLINFOW, *LPTTTOOLINFOW;

typedef struct _TT_HITTESTINFOW {
    HWND hwnd;
    POINT pt;
    TTTOOLINFOW ti;
} TTHITTESTINFOW, *LPTTHITTESTINFOW;
#endif //_MSC_VER < 1400

#define LPTOOLINFOW   LPTTTOOLINFOW
#define TOOLINFOW       TTTOOLINFOW
#define TTM_HITTESTW            (WM_USER +55)
#define TTM_GETTEXTW            (WM_USER +56)
#define TTM_GETTEXT             TTM_GETTEXTW
#define TTM_GETTOOLCOUNT        (WM_USER +13)
#endif // WIN32_PLATFORM_PSPC

unsigned int GetWindowsDirectory(LPWSTR lpBuffer,UINT uSize);

#else // _WIN32_WCE

// Special CE mapping function is just CreateFile in Windows
#define CreateFileForMapping	CreateFileW


// Flag for full screen property sheets
#define PSH_MAXIMIZE	0


#endif // _WIN32_WCE

#endif // _wince_fixes.h
