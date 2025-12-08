////////////////////////////////////////////////////////////////////////////////
//
// File Name    : samproto.h
// File Version : 2.0
// Description  : Header file for SAM drivers
//                Contains driver API function prototypes
// Information  : this .h file assumes MSVC4.x or MSVC1.5
//                may need to add more ifdefs to support other compilers
//                may need to change these import or export defns if a different
//                compiler is used.
//
// Copyright (C) 1997-2002 Dolphin Oceanic Ltd.
//
////////////////////////////////////////////////////////////////////////////////
// Version History:
//
// $Log: /Library/sam_driver_proto.h $
// 
// 7     23/01/08 15:26 Druck
// Initial limited implementation of Licensing API
// 
// 6     24/02/04 12:33 Druck
// Registry key pointers changed to LCHAR to always char's on Windows and
// WCHAR's on CE
// 
// 5     26/06/03 10:06 Druck
// DLLIMPDEF not defined when compiling for CE, so .DEF file is used to
// give correct linkage
// 
// 4     17/06/03 12:17 Druck
// CE port, use of tchar for unicode support
// 
// 3     7/05/03 15:37 Druck
// Changes to DLL import for WinCE to allow DolOSam to link without errors
// 
// 2     4/12/02 10:08 Druck
// SAM version 2 changes
// 
// 2     28/10/02 11:16 Druck
// SAM version 2 changes
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __INC_SAM_DRIVER_PROTO // Only include once
#define __INC_SAM_DRIVER_PROTO

#include <tchar.h>
#include "text_macros.h"

#ifdef _WIN32_WCE
# define DLLIMPDEF	// if on WinCE use DEF file instead
# define ptr *
#else 
# ifdef WIN32
#  ifdef SAM_DRIVER		// if we are compiling sam32.dll
#   define DLLIMPDEF _declspec(dllexport)
#  else				// else this file is included by a client
#   define DLLIMPDEF _declspec(dllimport)
#  endif // SAMDLL
#  define ptr *
# else
// we must be compiling sam16.dll
# define ptr _far *
# define DLLIMPDEF
# endif // WIN32
#endif // _WIN32_WCE

#ifdef __cplusplus
extern "C" {
#endif


// Synthesiser version 1 functions
DWORD DLLIMPDEF WINAPI SyndGetCharSet(
	DWORD     unitid,				// unit identifier
	DWORD     charset,				// charset number
	WCHAR ptr buffer,				// pointer to buffer
	DWORD ptr size,					// size of buffer
	DWORD     language				// requested language
	);

DWORD DLLIMPDEF WINAPI SyndAppend(
	DWORD     unitid,				// unit identifier
	WCHAR ptr text,					// pointer to unicode text
	DWORD     length,				// number of characters
	DWORD     index,				// index identifier
	DWORD ptr vblock,				// pointer to paramter block
	DWORD     flags					// speech flags
	);

DWORD DLLIMPDEF WINAPI SyndMute(
	DWORD unitid					// unit identifier
	);

DWORD DLLIMPDEF WINAPI SyndSpeak(
	DWORD unitid,					// unit identifier
	DWORD index,					// index identifier				
	DWORD flags						// speech flags
	);

DWORD DLLIMPDEF WINAPI SyndIndex(
	DWORD     unitid,				// unit identifier
	DWORD ptr index,				// address of DWORD to place index
	DWORD ptr flags					// address of DWORD for flags
	);

void DLLIMPDEF WINAPI SyndInitialise(
	_LCHAR ptr regkey,				// path to registry key
	DWORD    reserved				// reserved and should be zero
	);

void DLLIMPDEF WINAPI SyndCloseDown();

DWORD DLLIMPDEF WINAPI SyndControl(
	DWORD mode						// reason code
	);

DWORD DLLIMPDEF WINAPI SyndOpenSynth (
	DWORD unitid					// unit number
	);

DWORD DLLIMPDEF WINAPI SyndCloseSynth (
	DWORD unitid					// unit identifier
	);

DWORD DLLIMPDEF WINAPI SyndQuerySynth (
	DWORD           unitid,			// unit number
	SYNTHPARAMS ptr lpunit,			// address of SYNTHPARAMS structure
	DWORD           langid			// language identifier
	);

DWORD DLLIMPDEF WINAPI SyndQueryParam (
	DWORD        unitid,			// unit number
	DWORD        pnum,				// parameter number
	SAMPARAM ptr param				// address of parameter info block
	);

DWORD DLLIMPDEF WINAPI SyndQueryParamDesc (
	DWORD     unitid,				// unit id
	DWORD     pnum,					// parameter number
	DWORD     language,				// requested langid
	WCHAR ptr description,			// address of output buffer
	DWORD ptr size
	);

DWORD DLLIMPDEF WINAPI SyndQueryParamValue (
	DWORD     unitid,				// unit id
	DWORD     pnum,					// parameter number
	DWORD     val,					// value index
	DWORD ptr pvalue				// pointer to DWORD to receive result
	);

DWORD DLLIMPDEF WINAPI SyndQueryParamChoice (
	DWORD      unitid,				// unit id
	DWORD      pnum,				// parameter number
	DWORD      val,					// actual paramter value
	DWORD     language,				// requested language
	WCHAR ptr description,			// address of buffer
	DWORD ptr size					// size of buffer	
	);

DWORD DLLIMPDEF WINAPI SyndGetLangId (
	DWORD     unitid,				// unit id
	DWORD     index,				// n'th language
	DWORD ptr langid				// return language identifier
	);

DWORD DLLIMPDEF WINAPI SyndGetVoice (
	DWORD     unitid,				// unit id
	DWORD     vnum,					// voice number
	DWORD ptr voiceblock			// address of voice block buffer
	);

// Synthesiser version 2 functions

DWORD DLLIMPDEF WINAPI SyndGetDefaultVoice (
	DWORD     unitid,				// unit id
	DWORD     langid,				// spoken language ID
	DWORD ptr voiceblock			// address of voice block buffer
	);

// Braille display functions, SAM Driver API versions 1 and 2

void DLLIMPDEF WINAPI BrldInitialise(
	_LCHAR ptr regkey,				// path to registry key
	DWORD    reserved				// reserved and should be zero
	);

void DLLIMPDEF WINAPI BrldCloseDown();

DWORD DLLIMPDEF WINAPI BrldControl(
	DWORD mode						// reason code
	);

DWORD DLLIMPDEF WINAPI BrldOpenBraille (
	DWORD      unitid,				// unit number
	BUTTONFUNC lpfn,				// Callback function for buttons
	DWORD      devid				// identifier for 
	);

DWORD DLLIMPDEF WINAPI BrldCloseBraille (
	DWORD unitid					// unit identifier
	);

DWORD DLLIMPDEF WINAPI BrldGetLangId (
	DWORD     unitid,				// unit id
	DWORD     index,				// n'th language
	DWORD ptr langid				// return language identifier
	);

// Braille functions, SAM Client API version 1 (depreciated)

DWORD DLLIMPDEF WINAPI BrldQueryBraille (
	DWORD             unitid,		// unit number
	BRAILLEPARAMS ptr lpunit,		// address of BRAILLEPARAMS structure
	DWORD             langid		// language identifier
	);

DWORD DLLIMPDEF WINAPI BrldQueryButton (
	DWORD     unitid,				// unit identifier
	DWORD     num,					// Button number
	WCHAR ptr description,			// Description text
	DWORD ptr size,					// Size of description
	DWORD     langid				// Language id
	);

BOOL DLLIMPDEF WINAPI BrldIsKeyValid (
	DWORD unitid,					// unit identifier
	DWORD KEY						// key combination
	);

DWORD DLLIMPDEF WINAPI BrldGetButtonCombination (
	DWORD     unitid,				// Unit number
	DWORD     action,				// Braille related action code
	DWORD ptr button				// Returned combination
	);

DWORD DLLIMPDEF WINAPI BrldSetDisplay (
	DWORD    unitid,				// unit identifier
	WORD ptr dispdata,				// Dot pattern for main display area
	WORD ptr statdata				// Dot pattern for status cells
	);

DWORD DLLIMPDEF WINAPI BrldClearDisplay (
	DWORD unitid					// unit identifier
	);

DWORD DLLIMPDEF WINAPI BrldSetCursor (
	DWORD unitid,					// unit identifier
	long  pos,						// Position of cursor
	WORD  shape,					// Dot pattern of cursor
	DWORD rate						// Blink rate
	);

// Braille functions, SAM Driver API version 2 (to be used in preference)

DWORD DLLIMPDEF WINAPI BrldQueryBrailleEx(
	DWORD               unitid,		// unit number
	BRAILLEPARAMSEX ptr lpunit,		// address of BRAILLEPARAMSEX structure
	DWORD           ptr size,		// expected/returned size of BRAILLEPARAMSEX
	DWORD               langid		// description language
	);

DWORD DLLIMPDEF WINAPI BrldQueryStrip(
    DWORD           unitid,			// unit number
    DWORD           strip,			// strip number
    STRIPPARAMS ptr lpstrip,		// address of STRIPPARAMS structure
    DWORD       ptr size,       	// expected/returned size of STRIPPARAMS
    DWORD           langid 			// description language
	);

DWORD DLLIMPDEF WINAPI BrldSetDisplayStrip(
	DWORD    unitid,				// unit identifier
	DWORD    strip,					// strip number
	WORD ptr data					// Dot pattern for cells
	);

DWORD DLLIMPDEF WINAPI BrldClearDisplayStrip(
	DWORD unitid,					// unit identifier
	DWORD strip						// strip number
	);

DWORD DLLIMPDEF WINAPI BrldSetCursorStrip(
	DWORD unitid,					// unit identifier
	DWORD strip,					// strip number
	DWORD pos,						// cell position
	WORD  shape,					// dot pattern
	DWORD rate						// blink rate
	);

DWORD DLLIMPDEF WINAPI BrldQueryButtonStrip(
	DWORD     unitid,				// unit identifier
	DWORD     strip,				// strip number
	DWORD     num,					// button or comb index
	WCHAR ptr description,			// buffer or NULL
	DWORD ptr size,					// size returned/required
	DWORD     langid				// description language
	);

BOOL DLLIMPDEF WINAPI BrldIsKeyValidStrip(
	DWORD     unitid,				// unit identifier
	DWORD     strip,				// strip number
	BYTE  ptr pkey					// pointer to key bitmask
	);

DWORD DLLIMPDEF WINAPI BrldGetButtonCombinationStrip(
	DWORD     unitid, 				// Unit number
	DWORD     action, 				// Braille related action code
	DWORD ptr strip,				// Strip number returned  
	DWORD ptr pbutton				// Returned button information
	);

// driver notify function
void DLLIMPDEF WINAPI DrvrSetNotify(
	NOTIFYFUNC   lpfn,				// Driver notify Callback function
	DWORD		 devid);			// identifier for device

DWORD DLLIMPDEF WINAPI DrvrSetLicensing (
	DWORD ptr licensetable			// address of licence table
	);

#ifdef __cplusplus
}
#endif

#endif // ifndef __INC_SAM_DRIVER_PROTO
