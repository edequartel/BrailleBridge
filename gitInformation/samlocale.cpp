// locale conversion
#include <windows.h>
#include "sam.h"
#include "samlocale.h"

typedef struct LCID_CP
{
	WORD locale;
	WORD cp;
	DWORD sam_lang;
} LCID_CP;

#define LOCLE(sublang,lang)	(LANG_##lang | (SUBLANG_##sublang << 8))

LCID_CP lcid_cp[] =
{
	/*Afrikaans*/
	{0x0436,1252,LANGID_Afrikaans},//oem_cp=850
	/*Albania*/
	{0x041C,1250,0},//oem_cp=0
	/*Arabic (Saudi Arabia)*/
	{0x0401,1256,LANGID_Arabic},//oem_cp=0
	/*Arabic,(Iraq)*/
	{0x0801,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (Egypt)*/
	{0x0C01,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (Libya)*/
	{0x1001,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (Algeria)*/
	{0x1401,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (Morocco)*/
	{0x1801,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (Tunisia)*/
	{0x1C01,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (Oman)*/
	{0x2001,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (Yemen)*/
	{0x2401,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (Syria)*/
	{0x2801,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (Jordan)*/
	{0x2C01,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (Lebanon)*/
	{0x3001,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (Kuwait)*/
	{0x3401,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (United Arab Emirates)*/
	{0x3801,1256,LANGID_Arabic},//oem_cp=0
	/*Arabic (Bahrain)*/
	{0x3C01,1256,LANGID_Arabic},//oem_cp=864
	/*Arabic (Qatar)*/
	{0x4001,1256,LANGID_Arabic},//oem_cp=864
	/*Armenian*/
	{0x042B,0,0},//oem_cp=0
	/*Assamese*/
	{0x044D,0,0},//oem_cp=0
	/*Azeri (Latin)*/
	{0x042C,0,0},//oem_cp=0
	/*Azeri (Cyrillic)*/
	{0x082C,0,0},//oem_cp=0
	/*Basque*/
	{0x042D,1252,0},//oem_cp=850
	/*Bengali*/
	{0x0445,0,LANGID_Bengali},//oem_cp=0
	/*Byelorussian*/
	{0x0423,1251,0},//oem_cp=866
	/*Bulgarian*/
	{0x0402,1251,LANGID_Bulgarian},//oem_cp=866
	/*Catalan*/
	{0x0403,1252,LANGID_Catalan},//oem_cp=850
	/*Chinese (Taiwan)*/
	{0x0404,950,LANGID_Chinese},//oem_cp=950
	/*Chinese (People's Republic of China)*/
	{0x0804,936,LANGID_Chinese + LANGID_CODEPAGE_ALT1},//oem_cp=936
	/*Chinese (Hong Kong)*/
	{0x0C04,950,LANGID_Cantonese},//oem_cp=950
	/*Chinese (Singapore)*/
	{0x1004,936,LANGID_Cantonese + LANGID_CODEPAGE_ALT1},//oem_cp=936
	/*Chinese (Macau)*/
	{0x1404,936,LANGID_Cantonese + LANGID_CODEPAGE_ALT1}, // Guess!//oem_cp=936
	/*Croatia*/
	{0x041A,1250,LANGID_Serbo_Croat},//oem_cp=852
	/*Czech*/
	{0x0405,1250,LANGID_Czech},//oem_cp=852
	/*Danish*/
	{0x0406,1252,LANGID_Danish},//oem_cp=850
	/*Dutch (Standard)*/
	{0x0413,1252,LANGID_Dutch},//oem_cp=850
	/*Dutch (Belgium)*/
	{0x0813,1252,LANGID_Dutch},//oem_cp=850
	/*English (United States)*/
	{0x0409,1252,LANGID_USA_English},//oem_cp=0
	/*English (Britain)*/
	{0x0809,1252,LANGID_UK_English},//oem_cp=850
	/*English (Australia)*/
	{0x0C09,1252,LANGID_USA_English},//oem_cp=850
	/*English (Canada)*/
	{0x1009,1252,LANGID_USA_English},//oem_cp=850
	/*English (New Zealand)*/
	{0x1409,1252,LANGID_USA_English},//oem_cp=0
	/*English (Ireland)*/
	{0x1809,1252,LANGID_UK_English},//oem_cp=850
	/*English (South Africa)*/
	{0x1C09,1252,LANGID_UK_English},//oem_cp=850
	/*English (Jamaica)    */
	{0x2009,1252,LANGID_UK_English},//oem_cp=850
	/*English (Caribbean)  */
	{0x2409,1252,LANGID_UK_English},//oem_cp=850
	/*English (Belize)     */
	{0x2809,1252,LANGID_UK_English},//oem_cp=850
	/*English (Trinidad)   */
	{0x2C09,1252,LANGID_UK_English},//oem_cp=850
	/*English (Zimbabwe)   */
	{0x3009,1252,LANGID_UK_English},//oem_cp=850
	/*English (Philippines)*/
	{0x3409,1252,LANGID_UK_English},//oem_cp=850
	/*Estonian*/
	{0x0425,1257,LANGID_Estonian},//oem_cp=775
	/*Faeroese*/
	{0x0438,1250,LANGID_Faeroese},//oem_cp=850
	/*Farsi*/
	{0x0429,1256,0},// !!!! modified version of 1256!!!!//oem_cp=0
	/*Finnish*/
	{0x040B,1252,LANGID_Finnish},//oem_cp=850
	/*French*/
	{0x040C,1252,LANGID_French},//oem_cp=850
	/*French (Belgium)*/
	{0x080C,1252,LANGID_French},//oem_cp=850
	/*French (Switzerland)*/
	{0x100C,1252,LANGID_French},//oem_cp=850
	/*French (Canada)*/
	{0x0C0C,1252,LANGID_French},//oem_cp=850
	/*French (Luxembourg)*/
	{0x140C,1252,LANGID_French},//oem_cp=850
	/*French (Monaco)*/
	{0x180C,1252,LANGID_French},//oem_cp=850
	/*Georgian*/
	{0x0437,0,0},//oem_cp=0
	/*German*/
	{0x0407,1252,LANGID_German},//oem_cp=850
	/*German (Switzerland)*/
	{0x0807,1252,LANGID_German},//oem_cp=850
	/*German (Austria)*/
	{0x0C07,1252,LANGID_German},//oem_cp=850
	/*German (Luxembourg)*/
	{0x1007,1252,LANGID_German},//oem_cp=850
	/*German (Liechtenstein)*/
	{0x1407,1252,LANGID_German},//oem_cp=850
	/*Greek*/
	{0x0408,1253,LANGID_Greek},//oem_cp=737
	/*Gujarati*/
	{0x0447,0,0},//oem_cp=0
	/*Hebrew*/
	{0x040D,1255,LANGID_Hebrew},//oem_cp=862
	/*Hindi*/
	{0x0439,CP_UTF8,LANGID_Hindi},//oem_cp=0
	/*Hungarian*/
	{0x040E,1250,LANGID_Hungarian},//oem_cp=852
	/*Icelandic*/
	{0x040F,1252,LANGID_Icelandic},//oem_cp=850
	/*Indonesian*/
	{0x0421,1252,LANGID_Indonesian},//oem_cp=437
	/*Italian */
	{0x0410,1252,LANGID_Italian},//oem_cp=850
	/*Italian (Switzerland)*/
	{0x0810,1252,LANGID_Italian},//oem_cp=850
	/*Japanese*/
	{0x0411,932,LANGID_Japanese},//oem_cp=932
	/*Kannada*/
	{0x044B,0,0},//oem_cp=0
	/*Kashmiri*/
	{0x0460,0,0},//oem_cp=0
	/*Kashmiri (India)*/
	{0x0860,0,0},//oem_cp=0
	/*Kazak*/
	{0x043F,0,0},//oem_cp=0
	/*Konkani*/
	{0x0457,0,0},//oem_cp=0
	/*Korean*/
	{0x0412,949,LANGID_Korean},//oem_cp=949
	/*Korean (Johab)*/
	{0,1361,0},//oem_cp=0
	/*Latvian*/
	{0x0426,1257,LANGID_Latvian},//oem_cp=0
	/*Lithuanian*/
	{0x0427,1257,LANGID_Lithuanian},//oem_cp=0
	/*Macedonian (the Former Yugoslav Republic of Macedonia)*/
	{0x042F,0,0},//oem_cp=0
	/*Malay (Malaysia)*/
	{0x043E,0,LANGID_Malay},//oem_cp=0
	/*Malay (Brunei Darussalam)*/
	{0x083E,0,LANGID_Malay},//oem_cp=0
	/*Malayalam*/
	{0x044C,0,0},//oem_cp=0
	/*Manipuri*/
	{0x0458,0,0},//oem_cp=0
	/*Marathi*/
	{0x044E,0,0},//oem_cp=0
	/*Nepali*/
	{0x0461,0,0},//oem_cp=0
	/*Nepali (India)*/
	{0x0861,0,0},//oem_cp=0
	/*Norwegian (Bokmål)*/
	{0x0414,1252,LANGID_Norwegian},//oem_cp=850
	/*Norwegian (Nynorsk)*/
	{0x0814,1252,LANGID_Norwegian},//oem_cp=850
	/*Oriya*/
	{0x0448,0,0},//oem_cp=0
	/*Polish*/
	{0x0415,1250,LANGID_Polish},//oem_cp=852
	/*Portuguese*/
	{0x0816,1252,LANGID_Portuguese},//oem_cp=850
	/*Portuguese (Brazil)*/
	{0x0416,1252,LANGID_Brazil_Portuguese},//oem_cp=850
	/*Punjabi*/
	{0x0446,0,LANGID_Punjabi},//oem_cp=0
	/*Romanian*/
	{0x0418,1250,LANGID_Romanian},//oem_cp=852
	/*Russian*/
	{0x0419,1251,LANGID_Russian},//oem_cp=866
	/*Sanskrit*/
	{0x044F,0,0},//oem_cp=0
	/*Serbian*/
	{0x041A,1250,LANGID_Serbo_Croat},//oem_cp=852
	/*Serbian (Latin)*/
	{0x081A,1250,LANGID_Serbo_Croat},//oem_cp=852
	/*Serbian (Cyrillic)*/
	{0x0C1A,1250,LANGID_Serbo_Croat},// Guess!//oem_cp=852
	/*Sindhi*/
	{0x0459,0,0},//oem_cp=0
	/*Slovak*/
	{0x041B,1250,LANGID_Slovak},//oem_cp=852
	/*Slovenian*/
	{0x0424,1250,LANGID_Slovene},//oem_cp=852
	/*Spanish (Castilian)*/
	{0x040A,1252,LANGID_Castilian_Spanish},//oem_cp=0
	/*Spanish (Mexican)*/
	{0x080A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Modern)*/
	{0x0C0A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Guatemala)         */
	{0x100A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Costa Rica)        */
	{0x140A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Panama)            */
	{0x180A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Dominican Republic)*/
	{0x1C0A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Venezuela)         */
	{0x200A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Colombia)          */
	{0x240A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Peru)              */
	{0x280A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Argentina)         */
	{0x2C0A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Ecuador)           */
	{0x300A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Chile)             */
	{0x340A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Uruguay)           */
	{0x380A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Paraguay)          */
	{0x3C0A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Bolivia)           */
	{0x400A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (El Salvador)       */
	{0x440A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Honduras)          */
	{0x480A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Nicaragua)         */
	{0x4C0A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Spanish (Puerto Rico)       */
	{0x500A,1252,LANGID_Latin_Spanish},//oem_cp=850
	/*Swedish*/
	{0x041D,1252,LANGID_Swedish},//oem_cp=850
	/*Swedish (Finland)*/
	{0x081D,1252,LANGID_Swedish},//oem_cp=850
	/*Tamil*/
	{0x0449,0,LANGID_Tamil},//oem_cp=0
	/*Tatar*/
	{0x0444,0,0},//oem_cp=0
	/*Telugu*/
	{0x044a,0,LANGID_Telugu},//oem_cp=0
	/*Thai*/
	{0x041E,874,0},//oem_cp=874
	/*Turkish*/
	{0x041F,1254,LANGID_Turkish},//oem_cp=857
	/*Ukrainian*/
	{0x0422,1251,LANGID_Russian},//oem_cp=866
	/*Urdu (Pakistan)*/
	{0x0420,0,LANGID_Urdu},//oem_cp=0
	/*Urdu (India)*/
	{0x0820,0,LANGID_Urdu},//oem_cp=0
	/*Uzbek (Latin)*/
	{0x0443,0,0},//oem_cp=0
	/*Uzbek (Cyrillic)*/
	{0x0843,0,0},//oem_cp=0
	/*Vietnamese*/
	{0x042A,0,0},//oem_cp=0

	{0,1252,LANGID_Welsh},

// the following SAM Id's have no Microsoft Locale id's

//	LANGID_Hausa
//	LANGID_Iranian
//	LANGID_Irish
//	LANGID_Maltese
//	LANGID_Maori
//	LANGID_Swahili
//	LANGID_Zulu
};

DWORD GetLangIdFromLocale(LCID locale)
{
	locale &= NLS_VALID_LOCALE_MASK;

	LCID locale_default = MAKELANGID(PRIMARYLANGID(locale), SUBLANG_NEUTRAL);

	LCID_CP *table_exact=NULL;
	LCID_CP lcid_cp_default = {0x0809,1252,LANGID_UK_English};
	LCID_CP *table_default=&lcid_cp_default;

	LCID_CP *table;
	for(table = lcid_cp;table<&lcid_cp[sizeof(lcid_cp)/sizeof(lcid_cp[0])];table++)
	{
		if(table->locale)
		{
			if(table->locale == locale)
			{
				table_exact = table;
				break;
			}
			if(table->locale == locale_default)
				table_default = table;
		}
	}
	if(!table_exact)
	{
		// then look for a match on the primary language only
		for(table = lcid_cp;table<&lcid_cp[sizeof(lcid_cp)/sizeof(lcid_cp[0])];table++)
		{
			if(table->locale)
			{
				if(PRIMARYLANGID(table->locale) == PRIMARYLANGID(locale))
				{
					table_exact = table;
					break;
				}
				if(table->locale == locale_default)
					table_default = table;
			}
		}	
	}

	if(!table_exact)
		table_exact = table_default;

	if(table_exact->sam_lang)
		return(table_exact->sam_lang);

	return(LANGID_UK_English);
}

LCID_CP* GetTableFromLangId(DWORD LangId)
{
	LCID_CP *table_exact=NULL;
	LCID_CP lcid_cp_default = {0x0809,1252,LANGID_UK_English};
	LCID_CP *table_default=&lcid_cp_default;

	LCID_CP *table;
	
	for(table = lcid_cp;table<&lcid_cp[sizeof(lcid_cp)/sizeof(lcid_cp[0])];table++)
	{
		if(table->sam_lang == LangId)
		{
			table_exact = table;
			break;
		}
	}

	if(!table_exact)
		table_exact = table_default;

	return(table_exact);
}

LCID GetLocaleFromLangId(DWORD LangId)
{
	LCID_CP *table = GetTableFromLangId(LangId);
	return(table->locale);
}

DWORD GetCodePageFromLangId(DWORD LangId)
{
	LCID_CP *table = GetTableFromLangId(LangId);
	return(table->cp);
}

DWORD GetUserLangId()
{
	return (GetLangIdFromLocale(GetUserDefaultLCID()));
}

int OkLangIdInACP(DWORD langid)
{
	if(langid != LANGID_UK_English && langid != LANGID_USA_English)
	{
		UINT acp = GetACP();
		LCID_CP *table;
		for(table = lcid_cp;table<&lcid_cp[sizeof(lcid_cp)/sizeof(lcid_cp[0])];table++)
		{
			if(table->sam_lang == langid && table->cp)
				return(acp == table->cp);
		}
	}
	return(1);
}

DWORD SamLangId(DWORD langid)
{
	return(langid - ((langid % 10000)/1000)*1000);
}