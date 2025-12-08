//
//		Name:		sam_driver_langinfo.h
//
//		Version:	1.01
//
//		Description:
//
//			Table of basic language info for SAM drivers.
//			No functions are provided, just use the data in the table.
//
//		Copyright (C) 2002 Dolphin Oceanic Ltd.
//


// The LANGINFO datatype
typedef struct
{
	int	codepage; // codepage of the Apollo ROM (if known)
	DWORD	code; // Language number in synth
	WCHAR	*itemdesc; // Default description of the language
	DWORD	samid; // SAM language ID
} LANG_INFO;


// The data table
extern LANG_INFO LangInfo[];
