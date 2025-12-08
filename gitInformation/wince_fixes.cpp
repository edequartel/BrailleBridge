////////////////////////////////////////////////////////////////////////////////
//
// File Name    : wince_fixes.cpp
// Description  : Various fixes to enable compilation under Windows CE
// Information  : Used by Orpheus 2, Sam 2 and Mars6
//
// Copyright (C) 2002-2005 Dolphin Oceanic Ltd.
//
////////////////////////////////////////////////////////////////////////////////
// Version History:
//
// $Log: /Library/wince_fixes.cpp $
// 
// 18    1/07/08 10:11 Druck
// NULL lpDefault handled in GetPrivateProfileString()
// 
// 17    26/07/07 14:16 Druck
// _splitpath and _makepath changed to _t variants for SAM
// 
// 16    12/01/07 15:15 Mike
// bug fix in GetPrivateProfileString() - incorrect return code when
// enumeration keys or sections and your buffer was too short.
// 
// 15    15/08/05 11:18 Mike
// 
// 14    9/08/05 11:54 Mike
// Added GetWindowsDirectory() API
// 
// 13    4/05/05 11:13 Malcolm
// Allow CE to compile
// 
// 12    3/05/05 10:09 John
// typo correction
// 
// 11    3/05/05 9:30 John
// Added IsBadStringPtr() - implement with call to IsBadReadPtr()
// 
// 10    24/09/04 14:03 Mike
// 
// 9     8/04/04 12:33 Druck
// time() implementation excluded from CE3 due to lack of time.h
// 
// 8     22/03/04 16:17 Druck
// Single and double quotes stripped in GetPrivateProfileString()
// OpenSemaphore() added
// Standard C Library time() added
// 
// 7     26/02/04 9:27 Druck
// Section and key enumeration added to GetPrivateProfileString()
// 
// 6     6/02/04 11:54 Druck
// tchar function replaced with WCHAR
// 
// 5     16/01/04 9:45a Mike
// 
// 4     25/11/03 9:58 Druck
// Additions for Supernova and WinCE version 4
// 
////////////////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4514) // disable unreferenced inline function warning
#pragma warning( disable : 4201) // disable nameless struct/union warning
#include <windows.h>
#pragma warning( default : 4201)
#pragma warning( default : 4100)
#include <winuser.h>
#include <tchar.h>
#include <stdio.h>
#include <Shlobj.h>
#include <wince_fixes.h>

#define	PPS_LINELEN		1024		// line length for PrivateProfileString functions

#ifdef _WIN32_WCE

// --- C runtime ---------------------------------------------------------------

#if(_WIN32_WCE >= 400)
#include <time.h>

_TCHAR *getenv(const _TCHAR *varname)
{
	// system variables not supported
	return NULL;
}

wchar_t *_wgetenv(const wchar_t *varname)
{
	// system variables not supported
	return NULL;
}

const __int64 n1SecIn100NS = (__int64)10000000;

time_t time(time_t *timer)
{
	// get base tine of 1 Jan 1970
	static __int64 t0 = 0;
	if(t0==0)
	{
		FILETIME   f1970;
		SYSTEMTIME s1970;
		s1970.wYear         = 1970;
		s1970.wMonth        = 1;
		s1970.wDayOfWeek    = 1; //assumed
		s1970.wDay          = 1;
		s1970.wHour         = 0;
		s1970.wMinute       = 0;
		s1970.wSecond       = 0;
		s1970.wMilliseconds = 0;
		SystemTimeToFileTime( &s1970, &f1970 );
		t0   = f1970.dwHighDateTime;
		t0 <<= 32;
		t0  |= f1970.dwLowDateTime;
	}

	// get current time from CE
	SYSTEMTIME sNow;
	FILETIME   fNow;
	GetLocalTime(&sNow);
	SystemTimeToFileTime( &sNow, &fNow );

	// convert to 64 bit ints
	__int64 t1 = fNow.dwHighDateTime;
	t1       <<= 32;				
	t1        |= fNow.dwLowDateTime;

	// Take the difference of 64-bit ints.
	// This should be the number of 100-nanosecond intervals since Jan. 1970.
	// Divide by 10,000,000 to get the number of seconds since Jan. 1970.
	__int64 iTimeDiff = (t1 - t0) / n1SecIn100NS;

	if(timer)
		*timer = (time_t)iTimeDiff;

	return (time_t)iTimeDiff;
}
#endif // _WIN32_WCE >= 400

// --- Windows WinUser.h -------------------------------------------------------

#define GCW_ATOM            (-32)

HWND GetTopWindow(HWND hWnd)
{
	return(GetWindow(hWnd, GW_CHILD));
}

LONG GetMessageTime(VOID)
{
	// dont know message time, return current time
	return GetTickCount();
}

BOOL GetWindowPlacement(HWND hWnd, WINDOWPLACEMENT *lpwndpl)
{
	// dummy implementation
	lpwndpl->length                  = sizeof(WINDOWPLACEMENT);
    lpwndpl->flags                   = 0;
    lpwndpl->showCmd                 = SW_SHOWNORMAL;
    lpwndpl->ptMinPosition.x         = 0;
    lpwndpl->ptMaxPosition.y         = 0;
    lpwndpl->rcNormalPosition.left   = 0;
    lpwndpl->rcNormalPosition.bottom = 0;
    lpwndpl->rcNormalPosition.right  = 0;
    lpwndpl->rcNormalPosition.top    = 0;
	return TRUE;
}

// code from CE MFC wcealt.cpp
int GetMenuItemCount(HMENU hMenu)
{
	const int MAX_NUM_ITEMS = 256;
	int  iPos, iCount;

	MENUITEMINFO mii;
	memset((char *)&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);

	iCount = 0;
	for (iPos = 0; iPos < MAX_NUM_ITEMS; iPos++)
	{
		if(!GetMenuItemInfo(hMenu, (UINT)iPos, TRUE, &mii))
			break;
		iCount++;
	}

	return iCount;
}

// code from CE MFC wcealt.cpp
int GetMenuString(HMENU hMenu, UINT uIDItem, LPWSTR lpString, int nMaxCount, UINT uFlag)
{ 
	MENUITEMINFO mii;
	memset((char *)&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);

	if (!GetMenuItemInfo(hMenu, 0, TRUE, &mii))
		return 0;
	
	mii.fMask      = MIIM_TYPE;  // to get dwTypeData
	mii.fType      = MFT_STRING; // to get dwTypeData
	mii.dwTypeData = lpString;
	mii.cch        = nMaxCount;

	if (uFlag == MF_BYPOSITION)
		GetMenuItemInfo(hMenu, uIDItem, TRUE, &mii);
	else
	{
		ASSERT(uFlag == MF_BYCOMMAND);
		GetMenuItemInfo(hMenu, uIDItem, FALSE, &mii);
	}

	if (mii.dwTypeData != NULL)
		return wcslen(lpString);

	return 0;
}

// code from CE MFC wcealt.cpp
UINT GetMenuState(HMENU hMenu, UINT uId, UINT uFlags)
{
	MENUITEMINFO mii;

	memset((char *)&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask  = MIIM_ID;

	if (uFlags & MF_BYPOSITION)
		GetMenuItemInfo(hMenu, uId, TRUE, &mii);
	else
		GetMenuItemInfo(hMenu, uId, FALSE, &mii);

	return mii.fState;
}

// --- Windows Winbase.h -------------------------------------------------------

DWORD GetPrivateProfileString(
    LPCTSTR lpAppName,
    LPCTSTR lpKeyName,
    LPCTSTR lpDefault,
    LPTSTR  lpReturnedString,
    DWORD   nSize,
    LPCTSTR lpFileName
    )
{
	TCHAR line[PPS_LINELEN];
	TCHAR match[PPS_LINELEN];
	// NOTE: open as text to allow reading of ANSI even in unicode mode
	FILE *f    = _tfopen(lpFileName, _T("r"));
	bool found = false;
	int  len   = 0;

	if(lpDefault==NULL)
		lpDefault = _T("");

	if(f)
	{
		// find section
		if(lpAppName)
		{
			len = _stprintf(match, _T("[%s]"), lpAppName);

			while(!feof(f) && !found)
			{
				// will convert text to unicode if appropriate
				if(_fgetts(line, PPS_LINELEN, f) && _tcsnicmp(line, match, len)==0)
					found = true;
			}
		}

		// enumating sections or keys
		if(lpAppName==NULL || (lpKeyName==NULL && found))
		{
			bool   finished = false;
			_TCHAR *string  = NULL;
			len             = 0;

			while(!feof(f) && !finished && len<(int)nSize-1)
			{
				found = false;

				// read line,
				_fgetts(line, PPS_LINELEN, f);

				if(lpAppName==NULL)
				{
					if(line[0]=='[')
					{
						// extract section name
						string    = line + 1;
						_TCHAR *s = _tcschr(string,']');
						if(s)
						{	
							*s    = 0;
							found = true;
						}
					}
				}
				else
				{
					if(line[0]!='[' && line[0]!=';')
					{
						// terminate keyname at any =
						string    = line;
						_TCHAR *s = _tcschr(string,'=');
						if(s)
						{
							*s    = 0;
							found = true;
						}
					}
					else
					{
						// finish once next section is reached
						if(line[0]=='[')
							finished = true;
					}
				}
					
				// if section heading or key
				if(found)
				{
					// copy to buffer checking for end reached
					int len2 = _tcslen(string);
					_tcsncpy(lpReturnedString+len, string, nSize-len);

					if(len+len2>(int)nSize-1)
					{
						// two nulls to mark end of buffer
						lpReturnedString[nSize-2] = 0;
						lpReturnedString[nSize-1] = 0;
						len = nSize;
					}
					else
					{
						// seperate with a null
						len += len2+1;
					}
				}
			} // while !feof(f)...
			fclose(f);

			// terminating second null
			if(len<(int)nSize-1)
			{
				lpReturnedString[len] = 0;
				return(len);
			}
			else
			{
				return nSize-2;
			}
		} // if lpAppName==NULL || (lpKeyName==NULL && found)

		// find key
		if(found)
		{
			// find key
			len         = _stprintf(match, _T("%s="), lpKeyName);
			found       = false;
			bool failed = false;

			while(!feof(f) && !found && !failed)
			{
				// will convert text to unicode if appropriate
				if(_fgetts(line, PPS_LINELEN, f))
				{
					// check for next section
					if(line[0]=='[')
						failed = true;
					else if(_tcsnicmp(line, match, len)==0)
						found = true;
				}
			}

			if(found)
			{
				// remove any trailing newline
				TCHAR *s = _tcsrchr(line, '\n');
				if(s)
					*s = 0;

				// strip any enclosing single or double quotes
				s = line + _tcslen(line)-1;

				if((line[len]=='\'' && *s=='\'') ||
				   (line[len]=='"'  && *s=='"'))
				{
					len++;
					*s = 0;
				}

				// copy key contents from line to users buffer
				_tcsncpy(lpReturnedString, line+len, nSize);
			}
		}
		fclose(f);
	}

	if(!found)
	{
		// copy default string to users buffer
		_tcsncpy(lpReturnedString, lpDefault, nSize);
	}


	// terminate and return length
	lpReturnedString[nSize-1] = 0;
	return _tcslen(lpReturnedString);
}

UINT GetPrivateProfileInt(
	LPCTSTR lpAppName,  // section name
	LPCTSTR lpKeyName,  // key name
	INT     nDefault,   // return value if key name not found
	LPCTSTR lpFileName  // initialization file name
)
{
	_TCHAR lpDefault[PPS_LINELEN];
	_TCHAR lpReturnedString[PPS_LINELEN];

	_stprintf(lpDefault, _T("%d"), nDefault);
	GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, lpReturnedString, PPS_LINELEN, lpFileName);
	return (UINT)_ttoi(lpReturnedString);
}

BOOL WritePrivateProfileString(
    LPCTSTR lpAppName,
    LPCTSTR lpKeyName,
    LPCTSTR lpString,
    LPCTSTR lpFileName
    )
{
	TCHAR line[PPS_LINELEN];
	TCHAR match[PPS_LINELEN];
	TCHAR lpFileName2[_MAX_PATH];
	bool foundSection = false;
	bool foundKey     = false;
	bool nextSection  = false;
	int  len;

	if(lpFileName==NULL)
		return FALSE;

	// temporary file to write output to
	_tcscpy(lpFileName2, lpFileName);
	lpFileName2[_tcslen(lpFileName2)-1] = '~';
	
	// NOTE: open as text to allow reading of ANSI even in unicode mode
	FILE *f1 = _tfopen(lpFileName,  _T("r"));
	FILE *f2 = _tfopen(lpFileName2, _T("w"));

	if(f2==NULL)
		return FALSE;

	len = _stprintf(match, _T("[%s]"), lpAppName);

	if(f1)
	{
		// look for existing section, copying lines to new file
		while(!feof(f1) && !foundSection)
		{
			// will convert text to unicode if appropriate
			_fgetts(line, PPS_LINELEN, f1);

			if(!feof(f1))
			{
				_fputts(line, f2);

				if(_tcsnicmp(line, match, len)==0)
					foundSection = true;
			}
		}

		if(foundSection)
		{
			// look for existing key
			len = _stprintf(match, _T("%s="), lpKeyName);

			while(!feof(f1) && !foundKey && !nextSection)
			{
				// will convert text to unicode if appropriate
				_fgetts(line, PPS_LINELEN, f1);

				if(!feof(f1))
				{
					// check for next section or a match, if not copy line
					if(line[0]=='[')
						nextSection = true;
					else if(_tcsnicmp(line, match, len)==0)
						foundKey = true;
					else
						_fputts(line, f2);
				}
			}
		}
	}

	if(!foundSection)
	{
		// leave a blank line if not first section in the file
		if(ftell(f2)!=0)
			_fputts(_T("\n"), f2);
		
		// write the new section with newline
		_fputts(match,    f2);
		_fputts(_T("\n"), f2);
	}

	// write the new key if not NULL
	_fputts(lpKeyName, f2);
	_fputts(_T("="),   f2);
	_fputts(lpString,  f2);
	_fputts(_T("\n"),  f2);

	if(f1)
	{
		// write the next section if just inserted before
		if(nextSection)
		{
			_fputts(_T("\n"), f2);
			_fputts(line,     f2);
		}

		// copy remained of the file
		while(!feof(f1))
		{
			_fgetts(line, PPS_LINELEN, f1);
			if(!feof(f1))
				_fputts(line, f2);
		}

		fclose(f1);
		DeleteFile(lpFileName);
	}

	fclose(f2);

	// remove existing file and rename new file
	return MoveFile(lpFileName2, lpFileName);
}

UINT GlobalGetAtomName(ATOM nAtom, LPTSTR lpBuffer, int nSize)
{
	ASSERT(true);
	return 0;
}

HANDLE OpenSemaphore(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName)
{
	HANDLE s = CreateSemaphore(NULL, 1, 1, lpName);

	if(GetLastError()!=ERROR_ALREADY_EXISTS)
	{
		CloseHandle(s);
		s = NULL;
		SetLastError(ERROR_SEM_NOT_FOUND);
	}
	return s;
}

// Windows CE version 3 only 
#if(_WIN32_WCE < 400)

BOOL DuplicateHandle(
  HANDLE hSourceProcessHandle,
  HANDLE hSourceHandle,
  HANDLE hTargetProcessHandle,
  LPHANDLE lpTargetHandle,
  DWORD dwDesiredAccess,
  BOOL bInheritHandle,
  DWORD dwOptions)
{
	(void)hSourceProcessHandle;
	(void)hTargetProcessHandle;
	(void)dwDesiredAccess;
	(void)bInheritHandle;
	(void)dwOptions;
	*lpTargetHandle = hSourceHandle;
	return 1;
}

void ExitProcess(UINT uExitCode)
{
	exit(uExitCode);
}

ATOM WINAPI GlobalFindAtom(LPCTSTR lpString)
{
	HWND hWnd = FindWindow(lpString, lpString);
	ATOM atom = 0;

	if(hWnd)
		atom = (ATOM)GetClassLong(hWnd, GCW_ATOM);

	return atom;
}

#endif // _WIN32_WCE < 400

BOOL IsBadStringPtr(LPCTSTR lpsz, UINT_PTR ucchMax)
{
	UINT_PTR p = 0;
	
	// check each byte of the string up to null terminator or max size
	do
	{
		if(IsBadReadPtr(lpsz, 1))
			return FALSE;

		lpsz++;
		p++;
	}
	while(*lpsz!=0 && p<ucchMax);

	return TRUE;
}


// --- SDK stdlib.h ------------------------------------------------------------

void _tsplitpath( const _TCHAR *path, _TCHAR *drive, _TCHAR *dir, _TCHAR *fname, _TCHAR *ext )
{
	// drive letters not supported under Windows CE
	if(drive)
		_tcscpy(drive, _T(""));

	if(dir)
	{
		_tcscpy(dir, path);
		_TCHAR *s = _tcsrchr(dir, '\\');
		if(s)
			*(++s) = 0;
		else
			_tcscpy(dir, _T(""));
	}

	if(fname)
	{
		_TCHAR *s = _tcsrchr(path, '\\');
		_tcscpy(fname, s ? s+1 : path);
		if((s = _tcsrchr(fname, '.'))!=NULL)
			*s = 0;
	}

	if(ext)
	{
		_TCHAR *s = _tcsrchr(path, '.');
		_tcscpy(ext, s ? s : _T(""));
	}
}

void _tmakepath( _TCHAR *path, const _TCHAR *drive, const _TCHAR *dir, const _TCHAR *fname, const _TCHAR *ext )
{
	_tcscpy(path, _T(""));

	if(dir)
	{
		// drive letters not supported under Windows CE
		if(drive && dir[0]!='\\')
			_tcscat(path, _T("\\"));

		_tcscat(path, dir);
	}

	if(fname)
	{
		if(path[_tcslen(path)-1] != '\\')
			_tcscat(path, _T("\\"));

		_tcscat(path, fname);
	}

	if(ext)
	{
		if(path[_tcslen(path)-1] != '.')
			_tcscat(path, _T("."));

		_tcscat(path, ext);
	}
}

// --- wingdi.h ----------------------------------------------------------------

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
)
{
	LOGFONT lf;

	lf.lfHeight         = nHeight; 
	lf.lfWidth          = nWidth; 
	lf.lfEscapement     = nEscapement; 
	lf.lfOrientation    = nOrientation; 
	lf.lfWeight         = fnWeight; 
 	lf.lfItalic         = (BYTE)fdwItalic; 
	lf.lfUnderline      = (BYTE)fdwUnderline; 
	lf.lfStrikeOut      = (BYTE)fdwStrikeOut; 
	lf.lfCharSet        = (BYTE)fdwCharSet; 
	lf.lfOutPrecision   = (BYTE)fdwOutputPrecision; 
 	lf.lfClipPrecision  = (BYTE)fdwClipPrecision; 
 	lf.lfQuality        = (BYTE)fdwQuality; 
	lf.lfPitchAndFamily = (BYTE)fdwPitchAndFamily; 
	_tcscpy(lf.lfFaceName, lpszFace); 

	return CreateFontIndirect(&lf);
}


unsigned int GetWindowsDirectory(LPWSTR lpBuffer,UINT uSize)
	{
	LPWSTR buff[MAX_PATH];
	SHGetSpecialFolderPath(NULL,(LPWSTR)buff,CSIDL_WINDOWS,0);
	if (wcslen((LPWSTR)buff)<=uSize)
		wcscpy(lpBuffer,(LPWSTR)buff);
	return(wcslen(lpBuffer));
	}

// -----------------------------------------------------------------------------

#endif // _WIN32_WCE
