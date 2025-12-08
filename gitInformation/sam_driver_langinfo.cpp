//
//		Name:		sam_driver_langinfo.cpp
//
//		Version:	1.01
//
//		Description:
//
//			Table of basic language info for SAM drivers.
//			No functions are provided, just use the data in the table.
//			The table includes all Apollo ROMs and Orpheus languages.
//			This is why for example there are three entries for Russian.
//
//		Version History:
//
//		Release Description										Date	   Name
//		1.01	First Version									24/06/02	JRB
//
//		Copyright (C) 2002 Dolphin Oceanic Ltd.
//

#pragma warning( disable : 4514) // disable unreferenced inline function warning
#pragma warning( disable : 4201) // disable nameless struct/union warning
#include <windows.h>
#pragma warning( default : 4201)
#include "sam.h"
#include "sam_driver_langinfo.h"


// The language information table
LANG_INFO LangInfo[] =
{
	437,     1,L"U S English", LANGID_USA_English,
	855,     7,L"Russian", LANGID_Russian, // New code
	866,  1007,L"Russian", LANGID_Russian,
	437,    20,L"Arabic", LANGID_Arabic,
	850,    27,L"Afrikaans",LANGID_Afrikaans,
	437, 10027,L"Zulu",LANGID_Zulu,
	737,    30,L"Greek", LANGID_Greek,
	437,    31,L"Dutch", LANGID_Dutch,
	437,    33,L"French", LANGID_French,
	850,    34,L"Castilian Spanish", LANGID_Castilian_Spanish,
	437, 10034,L"Catalan", LANGID_Catalan,
	852,    36,L"Hungarian", LANGID_Hungarian,
	852,    38,L"Serbo-Croat", LANGID_Serbo_Croat,
	437, 10038,L"Croatian", LANGID_Serbo_Croat,
	850,    39,L"Italian", LANGID_Italian,
	852,    40,L"Romanian", LANGID_Romanian,
	895,    42,L"Czech", LANGID_Czech,
	852,  1042,L"Czech", LANGID_Czech,
	895, 10042,L"Slovak", LANGID_Slovak,
	852, 11042,L"Slovak", LANGID_Slovak,
	437,    44,L"UK English", LANGID_UK_English,
	437, 10044,L"Welsh", LANGID_Welsh,
	865,    45,L"Danish", LANGID_Danish,
	850,    46,L"Swedish", LANGID_Swedish,
	865,    47,L"Norwegian", LANGID_Norwegian,
	123,    48,L"Polish",	LANGID_Polish,	// A Dolphin OEM codepage
	850,    49,L"German", LANGID_German,
	850,    52,L"Latin American Spanish", LANGID_Latin_Spanish,
	850,    55,L"Brazilian Portuguese", LANGID_Brazil_Portuguese,
	437,    60,L"Malay", LANGID_Malay,
	437,    62,L"Indonesian", LANGID_Indonesian,
	437,    64,L"Maori", LANGID_Maori,
	437,    81,L"Japanese", LANGID_Japanese,
	437,    86,L"Mandarin Chinese", LANGID_Chinese, // check codepage
	437, 10086,L"Cantonese", LANGID_Cantonese, // check codepage
	853,    90,L"Turkish", LANGID_Turkish,
	850,    91,L"Hindi", LANGID_Hindi,
	850, 10091,L"Tamil",LANGID_Tamil,
	850,    92,L"Urdu",LANGID_Urdu,
	850, 10092,L"Punjabi",LANGID_Punjabi,
	850,    94,L"Telugu",LANGID_Telugu,
	437,    98,L"Iranian", LANGID_Iranian, // Check codepage
	437,   234,L"Hausa",LANGID_Hausa,
	437,   254,L"Swahili", LANGID_Swahili,
	860,   351,L"Portuguese", LANGID_Portuguese,
	437,   353,L"Irish", LANGID_Irish,
	861,   354,L"Icelandic",	LANGID_Icelandic, // used to be codepage 461 probly.
	853,   356,L"Maltese", LANGID_Maltese,
	437,   358,L"Finnish", LANGID_Finnish,
	855,   359,L"Bulgarian", LANGID_Bulgarian,
	771,   370,L"Lithuanian", LANGID_Lithuanian,
	771,   371, L"Latvian", LANGID_Latvian, // Check codepage
	771,   372, L"Estonian", LANGID_Estonian, // Check codepage
	852,   386,L"Slovene", LANGID_Slovene,
	850,   880,L"Bengali", LANGID_Bengali,
	437,   972,L"Hebrew", LANGID_Hebrew,
	855,  7095,L"Russian", LANGID_Russian, // Old code number
	  0,     0,L"Unrecognised Language", LANGID_UNKNOWN
};

