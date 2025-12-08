// uc2oem.cpp - Unicode to OEM codepage conversion

#include <windows.h>
#include <stdio.h> // for sscanf
#include "uc2oem.h"

/***********************/
/* Function prototypes */
/***********************/
void read_a_line(char **ptr, LPSTR str); // Read a line of input and update pointer
int uc2oem_compare(const void*, const void*); // for qsort
DWORD BinSearch(UCCHTABLE *tbl, WCHAR s); // search for uc in table


/************************/
/* Function definitions */
/************************/
BOOL init_uc2oem(LPTSTR filename, UCCHTABLE *table)
	{ // Load unicode to codepage mapping file
	HANDLE hFile;
	DWORD filesize; // File size;
	DWORD ret; // Return from file read
	LPSTR file = NULL; // memory version of the file
	LPSTR ptr; // pointer into the file
	char line[256]; // Store line of input
	DWORD uc; // Unicode-character pair read 
	DWORD ch;

	// Create a default entry in case the rest fails
	table->len = 128; // 128 entries in this table: basic latin
	for (WORD ii = 0; ii < 128; ii++)
		{
		table->table[ii].uc = ii;
		table->table[ii].ch = (char)ii;
		}

	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	// File opened, allocate memory and prepare to read
	filesize = GetFileSize(hFile, NULL);
	if (filesize == 0xffffffff) // Error
		{
		CloseHandle(hFile);
		return FALSE;
		}

	file = new char[filesize+1];
	ptr = file; // Point to start of file
	if (!file)
		{
		CloseHandle(hFile);
		delete file;
		return FALSE;
		}

	ReadFile(hFile, file, filesize, &ret, NULL);
	CloseHandle(hFile);
	if (ret != filesize) // Probably error
		{
		delete file;
		return FALSE;
		}

	file[filesize] = '\0'; // to stop falling off the end if last line not ended with eol

	// Parse and set up the table
	table->len = 0; // Currently no entries
	while (ptr < file+filesize && table->len < 512)
		{
		read_a_line(&ptr, line);
		// Parse line
		// The format is very strict as:
		// "xx xxxx\0"
		// where x = hex digit
		ret = sscanf(line, " %2x %4x", &ch, &uc);
		if (ret != 2) // sscanf failed to convert, probably invalid line
			continue;
		table->len++;
		table->table[table->len].uc = (WCHAR)uc;
		table->table[table->len].ch = (char)ch;
		}

	delete file;

	// Sort the table into ascending order of unicodes
	qsort(table->table, table->len + 1, sizeof(UCCH), uc2oem_compare);

	return TRUE;
	}


void read_a_line(char **ptr, LPSTR str)
	{ // Read line of file to \n, store in str and update ptr
	while (**ptr != '\n' && **ptr != '\0')
		{
		if (**ptr != '\r') // don't store \r\n
			*(str++) = **ptr;
		(*ptr)++;
		}
	(*ptr)++; // Skip over the newline
	*str = '\0'; // Terminate the string
	}


int uc2oem_compare(const void *arg1, const void *arg2)
	{ // compare function for qsort
	UCCH u1 = *(UCCH *)arg1;
	UCCH u2 = *(UCCH *)arg2;
	return (u1.uc - u2.uc);
	}


DWORD uc2oem(UCCHTABLE *table, WCHAR *src, long len, LPSTR dest)
	{ // Unicode to OEM codepage translation
	DWORD num = 0; // Start inserting characters at the beginning
	DWORD idx; // index into table

	if (len == -1) // Determine length
		len = wcslen(src) + 1; // Don't forget trailing \0

	for (long ii = 0; ii < len; ii++)
		{
		idx = BinSearch(table, src[ii]);
		if (idx < table->len)
			{
			if (dest)
				dest[num] = table->table[idx].ch;
			num++; // if dest == NULL, don't store but return result
			}
		}
	return num;
	}



DWORD BinSearch(UCCHTABLE *tbl, WCHAR s)
	{ // Search for s in a unicode to character mapping table
	DWORD	i,il,iu,niters;

	if (tbl->len <= 0)	return(tbl->len);			// validate length to search

	// Set up first binary search pointers
	il = 0;							// 
	iu = tbl->len - 1;						// range to add on each iteration
	niters = 2 * (tbl->len + 1);				// max number of goes if 16-bit numbers input

	while (niters /= 2) // should give at least log2(n) + 1 iterations
		{
		i = (il + iu)/2; // range to search on each iteration
		if (s > tbl->table[i].uc) il = i+1;		// s> so adjust lower limit
		else
			if (s < tbl->table[i].uc) iu = i-1;	// s< so adjust upper limit
			else
				return (i);			// return value 0-(n-1) for success
		}
	return tbl->len; // return length of table for failure
	}
