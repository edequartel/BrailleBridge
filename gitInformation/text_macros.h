//
//		Name:		text_macros.h
//
//		Version:	0.01
//
//		Description:
//		Text macros for Windows ANSI, Windows UNICODE, and Windows CE
//
//		Version History:
//
//		Release Description										Date	   Name
//		0.01	Initial version									17/12/02	DJR
//
//		Copyright (C) 2002 Dolphin Oceanic Ltd.
//
//
//

#ifndef _text_macros_h
#define _text_macros_h

// === Macros ==================================================================

/*
 * Macros to use unicode under WIN CE or ANSI on Windows
 * 
 * For example GetProcAddress() requires ANSI on both Windows ANSI and UNICODE
 * but UNICODE on Windows CE
 */
// 
#ifdef _WIN32_WCE

// string type, and literal string casting macro
#define LPLSTR		LPWSTR	
#define _LCHAR		WCHAR
#define	_L(text)	L##text

// standard C library string functions aliases
#define lcslen		wcslen
#define lcscat		wcscat
#define lcscpy		wcscpy
#define lcsncpy		wcsncpy
#define lcscmp		wcscmp
#define lcsncmp		wcsncmp
#define lcsicmp		wcsicmp
#define lcsstr		wcsstr
#define lcschr		wcschr
#define lcsrchr		wcsrchr
#define ltoupper	towupper
#define ltolower	towlower

#else // _WIN32_WCE

// string type, and literal string casting macro
#define LPLSTR		LPSTR		
#define _LCHAR		char
#define	_L(text)	text

// standard C library string functions aliases
#define lcslen		strlen
#define lcscat		strcat
#define lcscpy		strcpy
#define lcsncpy		strncpy
#define lcscmp		strcmp
#define lcsncmp		strncmp
#define lcsicmp		stricmp
#define lcsstr		strstr
#define lcschr		strchr
#define lcsrchr		strrchr
#define ltoupper	toupper
#define ltolower	tolower

#endif // _WIN32_WCE

#endif // text_macros
