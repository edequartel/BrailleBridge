// uc2oem.h

/*************/
/* Datatypes */
/*************/
typedef struct
	{
	WCHAR uc; // Unicode value
	char ch; // corresponding character
	} UCCH;

typedef struct
	{
	DWORD len; // length of table
	UCCH table[512]; // uc2cp conversion data
	} UCCHTABLE;

/***********************/
/* Function prototypes */
/***********************/
BOOL init_uc2oem(LPTSTR, UCCHTABLE *); // load UCM file
DWORD uc2oem(UCCHTABLE *, WCHAR *, long, LPSTR); // Unicode to OEM codepage
