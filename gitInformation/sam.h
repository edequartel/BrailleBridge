////////////////////////////////////////////////////////////////////////////////
//
// File Name    : sam.h
// File Version : 2.0
// Description  : Header file for SAM drivers and applications
//                Contains structure and literal definitions
//
// Copyright (C) 1997-2002 Dolphin Oceanic Ltd.
//
////////////////////////////////////////////////////////////////////////////////
// Version History:
//
// Release Description							Date
// 1.01	First version						--/--/--
// 1.02	Added voice language parameter		22/04/02
//
// $Log: /Library/sam.h $
// 
// 20    17/04/08 15:59 Druck
//  SAMERROR_RELOAD added
// 
// 19    9/04/08 14:52 Druck
// LANGID_Finnish and LANGID_Castilian_Spanish replaced misspellings which
// have been removed from SAM.h
// 
// 18    23/01/08 15:22 Druck
// Initial limited implementation of Licensing API
// 
// 17    18/09/07 8:09 Malcolm
// Added Faeroese, because is an Acapela Multimedia language.
// 
// 16    14/05/04 11:54 Mike
// Added LANGID_Korean
// 
// 15    29/01/04 8:12 Malcolm
// 
// 14    28/01/04 16:02 James
// Spelling
// 
// 13    28/01/04 15:56 James
// Spelling
// 
// 12    27/01/04 14:31 James
// Spelling
// 
// 11    23/12/03 16:30 Druck
// Synth and Braille structure identifier strings changed back to char on
// CE for binary compatibility with Win32 Supernova
// 
// 10    12/09/03 9:49 Druck
// SAM_HAVE_SPEECH marked as depricated
// 
// 9     12/09/03 9:46 Druck
// SAMCONTROL_SELECT_BRAILLE added
// 
// 8     29/07/03 10:01 Druck
// Incorrect include system path on text_macros.h
// 
// 7     17/06/03 13:45 Druck
// identifiers changed from _TCHAR to _LCHAR so only unicode under CE
// 
// 6     17/06/03 12:17 Druck
// CE port, use of tchar for unicode support
// 
// 5     23/01/03 9:40 Druck
// SAMCAPS_CURRENCIES added
// 
// 4     4/12/02 10:08 Druck
// SAM version 2 changes
// 
// 5     1/11/02 12:25 Druck
// SAMWHEEL_TURNR corrected (was SAMWHEEL_TRUNR) 
// SAMWHEEL_TURND  & SAMWHEEL_TURNU  added
// 
// 4     28/10/02 11:15 Druck
// SAM version 2 changes
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __INC_SAMH
#define __INC_SAMH // Only include it once

#include "text_macros.h"

#define	DESCRIPTION_LEN		128			// length of synth and braille description fields
#define IDENTIFIER_LEN		16			// length of synth and braille identifier fields

// structures are byte packed (no holes in them) - need to change this for
// non-Microsoft compilers
#pragma pack(1)

typedef struct synthparams
{
	WCHAR description[DESCRIPTION_LEN];	// unit text description
	char  identifier[IDENTIFIER_LEN];	// unique 16-byte id for this type of device
										// used to allow client to match saved 
										// parameters to existing devices
	DWORD params;						// number of parameters
	DWORD caps;							// unit capabilities
	DWORD langs;						// available description languages
	DWORD voices;						// number of preset voices
} SYNTHPARAMS;

typedef struct samparam
{
	DWORD range;						// number of values allowed
	long  first;						// offset value for user
	WORD  type;							// type of parameter
	WORD  id;							// parameter id value.
} SAMPARAM;

typedef struct specialchar
{
	WCHAR uc;							// unicode character
	WORD  padding;						// for structure alignment
	DWORD meaning;						// meaning value
} SPECIALCHAR;

typedef struct brailleparams
{
	WCHAR description[DESCRIPTION_LEN];	// unit text description
	char  identifier[IDENTIFIER_LEN];	// unique 16-byte id for this type of device
										// used to allow client to match saved 
										// parameters to existing devices
	DWORD width;						// Number of display cells along the line
	DWORD height;						// Number of lines of Braille cells in display
	DWORD status;						// Number of status cells
	DWORD buttons;						// Number of buttons
	DWORD maxrate;						// Maximum cursor blink rate
	DWORD caps;							// Unit capabilities
	DWORD langs;						// available description languages
} BRAILLEPARAMS;

typedef struct brailleparamsex			// SAM version 2 API
{
    WCHAR description[DESCRIPTION_LEN];	// unit text description
    char  identifier[IDENTIFIER_LEN]; 	// unique 16-byte id for this type of device
    DWORD strips; 						// number of strips
    DWORD maxrate; 						// maximum cursor blink rate
	DWORD caps;							// unit capabilities
    DWORD langs;						// available description languages
} BRAILLEPARAMSEX;

typedef struct stripparams				// SAM version 2 API
{
    WCHAR description[DESCRIPTION_LEN];	// description of the strip
    DWORD type; 						// strip type
    DWORD length; 						// number of cells and/or buttons
    DWORD buttoncombs;					// combinations of each button
    DWORD buttonstates;					// button state info
    DWORD orientation;					// orientations of strip
    DWORD caps;							// strip capabilities
} STRIPPARAMS;

typedef struct braillebuttonpress		// SAM version 2 API
{
	DWORD button;						// Button index
	DWORD comb;							// Augmented value from addional buttons or shifting keys
	DWORD pressed;						// Button pressed up/down/pressure information
} BRAILLEBUTTONPRESS;


// Define callback function type for driver to SAM notifications
typedef DWORD (CALLBACK *NOTIFYFUNC)(DWORD devid, DWORD reason, DWORD data);

// Define callback function type for button handling for Braille displays
typedef void  (CALLBACK *BUTTONFUNC)(DWORD devid, DWORD button, DWORD type);
typedef void  (CALLBACK *BUTTONFUNCSTRIP)(DWORD devid, DWORD pbutton, DWORD strip);

// revert to the compilers default structure packing
#pragma pack()


// function return error codes
#define SAMERROR_INVALID_ADDRESS	1		// one of the pointers in a function call was invalid
#define SAMERROR_INVALID_UNIT		2		// unitid was not valid
#define SAMERROR_INVALID_HANDLE		3		// handle was not valid
#define SAMERROR_UNIT_OPEN			4		// you tried to open an already open unit
#define SAMERROR_INVALID_PNUM		5		// pnum is invalid
#define SAMERROR_INVALID_LANGID		6		// langid is invalid
#define SAMERROR_NO_DESCRIPTION		7
#define SAMERROR_INVALID_VAL		8		// val is invalid
#define SAMERROR_INVALID_VNUM		9		// vnum is invalid
#define SAMERROR_NOSAM				10		// SAM is not installed or could not be located
#define SAMERROR_ALREADYOPEN		11		// handle is already open
#define SAMERROR_OPEN_FAILED		12		// driver reported error opening unit.
#define SAMERROR_UNIT_FAIL			13		// unit has just stopped working.
#define SAMERROR_INVALID_FLAGS		14		// flags is invalid
#define SAMERROR_CANT_SPEAK			15		// sharing manager has blocked speech request
#define SAMERROR_CANT_BRAILLE		15		// ditto (same value)
#define SAMERROR_INVALID_VBLOCK		16		// vblock contains invalid values
#define SAMERROR_UNIT_CLOSED		17		// unit is not open
#define SAMERROR_NOT_INITIALISED	18		// functions called before SamInit() 
#define SAMERROR_INTERNAL			19		// internal error - report to Dolphin.
#define SAMERROR_NOT_A_SYNTH		20		// unit is a braille device and can't talk!
#define SAMERROR_NOT_A_BRAILLE		21		// unit is a synth and cannot braille!
#define SAMERROR_WRONGSAMVERSION	22		// the SAM installed does not match the sam32.dll
#define SAMERROR_INVALID_STRIP		23		// stip number was not valid
#define SAMERROR_RELOAD				24		// driver requires reloading

// synthparams.caps for synthesisers
#define SAMCAPS_BATTERY			0x00000001	// unit is powered by batteries
#define SAMCAPS_REMOVABLE		0x00000002	// unit can be unplugged by the user (without powering down).
#define SAMCAPS_PHRASEINDEX		0x00000004	// unit can do phrase indexing.
#define SAMCAPS_WORDINDEX		0x00000008	// unit can do word indexing.
#define SAMCAPS_LETTERINDEX		0x00000010	// unit can do indexing on an individual character basis.
#define SAMCAPS_TALKING			0x00000020	// unit can report whether it is currently talking or has more to say.
#define SAMCAPS_SPEAKINCONTEXT	0x00000040	// unit supports speak in context
#define SAMCAPS_HARDWARE		0x00000080	// unit is a hardware synthesiser.
#define SAMCAPS_SOFTWARE		0x00000100	// unit is a software synthesiser (uses a sound card).
											// if neither of the above bits are set it means don't know!
											// if both are set its a combination device
#define SAMCAPS_RAWOUTPUT		0x00000200	// supports the SAMTEXT_RAW flag
#define SAMCAPS_SPELLPHONETIC	0x00000400	// supports the SYNTEXT_SPELLPHONETIC flag
#define SAMCAPS_SPELLATC		0x00000800	// supports the SYNTEXT_SPELLATC flag
#define	SAMCAPS_CURRENCIES		0x00001000	// synthesiser can speak currancies

//#define SAMCAPS_NUMBERS 1024	// can speak number sequences as words
//#define SAMCAPS_PHONEME 64		// unit supports the standard ISO Unicode phoneme character set.
//The above have been removed because capabilities may vary depending on language
//if 0-9 is in the alphabetic set then they can be spoken as words otherwise
//they may be in the spell set

// samparam.type
#define SAMPARAM_TYPEMASK		0x0000000f 
#define SAMPARAM_NUMERIC		0x00000001
#define SAMPARAM_CHOICE			0x00000002
#define SAMPARAM_COMPOUND		0x00000003
#define SAMPARAM_VARIABLERANGE	0x00000020
#define SAMPARAM_INPHRASE		0x00000040
#define SAMPARAM_DEFAULT		0x00000080

#define SAMVAL_DEFAULT			0xffffffff

// SamIndex
#define SYNDSPEECH_INPROGRESS	1

// brailleparams.caps for Braille displays
#define SAMBRLCAPS_BATTERY      0x00000001	// unit is powered by batteries
#define SAMBRLCAPS_ROUTE        0x00000002	// unit has cursor routing buttons over display cells
#define SAMBRLCAPS_STATROUTE    0x00000004	// unit has cursor routing buttons over status cells
#define SAMBRLCAPS_AUXROUTE     0x00000008	// unit has auxiliary routing buttons over display
#define SAMBRLCAPS_AUXSTATROUTE 0x00000010	// unit has auxiliary routing buttons over status
// also used in stripparams.caps for version 2 API
#define SAMBRLCAPS_EIGHTDOT     0x00000020	// unit has eight dot cells (else display is six dot)
// exclusively in stripparams.caps for version 2 API
#define SAMBRLCAPS_CURSOR		0x00000100	// cells in this strip support a cursor
#define SAMBRLCAPS_SLIDER		0x00000200	// buttons act as a slider
#define SAMBRLCAPS_DIAL			0x00000400	// buttons act as a rotatry dial control
#define SAMBRLCAPS_WHEEL		0x00000800	// buttons act as an wheel returning direction of turn
#define SAMBRLCAPS_2D			0x00001000	// buttons and button combs act as a 2D input
#define SAMBRLCAPS_PRESSURE		0x00002000	// button press codes relate to pressure input

// SamGetCharSet()/SyndGetCharSet() type
#define SAMSET_ALPHABETIC		0
#define SAMSET_MODIFIER			1
#define SAMSET_PUNCTUATION		2
#define SAMSET_SPECIAL			3

// SamAppend()/SyndAppend() flags
#define SAMTEXT_RAW				0x00000001	// instruct synth not to interpret the text
#define SAMTEXT_SPELL			0x00000002	// spell the text
#define SYNTEXT_SPELLPHONETIC	0x00000004	// use with SAMTEXT_SPELL for spelling using photenic sounds
#define SYNTEXT_SPELLATC		0x00000008	// use with SAMTEXT_SPELL for spelling using ATC words
#define SYNTEXT_SUPPRESS		0x00000010  // suppress text for speak in context feature
											// must check synth has SAMCAPS_SPEEKINCONTEXT flag

// samparam.id
#define SAMID_UNKNOWN				0xffff
#define SAMID_VOLUME				0
#define SAMID_SPEED					1
#define SAMID_PITCH					2
#define SAMID_PROSODY				3
#define SAMID_WORDPAUSE				4
#define SAMID_PHRASEPAUSE			5
#define SAMID_LANGUAGE				7
#define SAMID_BALANCE				8		// min should be left - max is right, center is mid range
#define SAMID_TREBLE				9
#define SAMID_MIDDLE				10
#define SAMID_BASS					11
#define SAMID_FADER					12		// min should be front, max is behind, center is mid range
#define SAMID_VOICELANG				13		// combined voice and language selection

// Define codes for SamControl
#define SAMCONTROL_DETECT			1
#define SAMCONTROL_NUM_SYNTH		2
#define SAMCONTROL_CONFIG			3
#define SAMCONTROL_SAM				4
#define SAMCONTROL_DEVICE_SYNTH		5
#define SAMCONTROL_SELECT_SYNTH		6
#define SAMCONTROL_OVERRIDE_ON		7
#define SAMCONTROL_OVERRIDE_OFF		8
#define SAMCONTROL_QUIT				9		// tells SAM to quit!
#define SAMCONTROL_DEVICE_BRAILLE	10
#define SAMCONTROL_NUM_BRAILLE		11
#define SAMCONTROL_SETLANG			12		// tells the driver which language to present it's
											// UI in. The lang id is stored in the HIWORD of the command
#define SAMCONTROL_SELECT_BRAILLE	13
#define SAMCONTROL_LICENSING		14

// SAM API Version 2 strip types
#define SAMSTRIP_DISPLAY			0		// has display cells and optional routing buttons
#define SAMSTRIP_STATUS				1		// has status cells and optional status routing buttons
#define SAMSTRIP_AUXILIARY			2		// has auxilary cells and optional routing buttons
#define SAMSTRIP_KEYS				3		// provides multiply press key input
#define SAMSTRIP_BUTTONS			4		// provides button/slider/rotary/2D input

// SAM API Version 2 strip orientations
#define SAMORIENT_NONE				0
#define SAMORIENT_HORIZONTAL		1
#define SAMORIENT_VERTICAL			2

// SAM API Version 2 button actions
#define SAMBUTTON_UP				0		// drivers may not provide button up information
#define SAMBUTTON_DOWN				1		// drivers must provide button down (pressed) information

// Miscelanous literals
#define SYND_DETECT_OK				1
#define SYND_DETECT_CANT			2

#define BRLD_DETECT_OK				1
#define BRLD_DETECT_CANT			2

#define SAMCONFIG_CANCEL			0
#define SAMCONFIG_REDETECT			1
#define SAMCONFIG_RELOAD			2
#define SAMCONFIG_REDETECT_ALL		3

#define SAMSTRIP_ALL			0xFFFFFFFF	// strip number used to address all strips
#define SAMCURSOR_HIDE			0xFFFFFFFF	// position value to hide cursor

#define SAMWHEEL_NOTRUN				0		// not turning
#define SAMWHEEL_TURNL				1		// turning left, horizontal orientation
#define SAMWHEEL_TURND				1		// turning down, vertical orientation
#define SAMWHEEL_TURNR				2		// turning right, horizontal orientation
#define SAMWHEEL_TURNU				2		// turning up, vertical orientation

// WM_SAM messages. The message type is sent in the LSB of wParam
#define SAM_RESPOND					1		// required SamRespond() call
#define SAM_HAVE_SPEECH				2		// Depricated - never sent
#define SAM_LOST_SPEECH				3
#define SAM_CONFIG_START			4
#define SAM_CONFIG_END				5
#define SAM_SERVER_QUIT				6		// sam has quit after user request
#define SAM_SERVER_RESTART			7		// sam has restarted
#define SAMBRLBTN_NORMAL			8		// Ordinary button
#define SAMBRLBTN_CSRDISP			9		// Cursor route button over display cell
#define SAMBRLBTN_CSRSTAT			10		// Cursor route button over status cell
#define SAMBRLBTN_AUXDISP			11		// Auxiliary routing button over display cell
#define SAMBRLBTN_AUXSTAT			12		// Auxiliary routing button over status cell
#define SAMBRLBTN_KEYPAD			13		// key on auxiliary keypad
#define SAMBRLBTN_STRIP				14		// version 2 API button ot key press

// for SAMBRLBTN messages the 2nd byte in wParam contains the unitid of the 
// device that generated the message, for SAMBRLBTN_STRIP it also contains
// the strip index
// lParam contains message specific information (button number or mask)

// msg = WM_SAM
// wParam = | MSB |     |     | LSB |
//			|	  |strip|unit#| type|

// some usefull macros for extracting WM_SAM message info
#define GET_SAM_MSG(x) ((DWORD)(LOBYTE(LOWORD(x))))
#define GET_SAM_UNIT(x) ((DWORD)(HIBYTE(LOWORD(x))))
#define GET_SAM_SPRIP(x) ((DWORD)(LOBYTE(HIWORD(x))))

// client types
#define SAM_SR 1
#define SAM_FTAP 2
#define SAM_PTAP 3
#define SAM_BTAP 4

// language identifiers

#define LANGID_Afrikaans			27		//locale=0x0436,codepage=1252
//#define LANGID_Albania					//locale=0x041C,codepage=1250
#define LANGID_Arabic				20		//locale=0x1401,codepage=1256
//#define LANGID_Armenian					//locale=0x042B,codepage=0
//#define LANGID_Assamese					//locale=0x044D,codepage=0
//#define LANGID_Azeri (Cyrillic)			//locale=0x082C,codepage=0
//#define LANGID_Azeri (Latin)				//locale=0x042C,codepage=0
//#define LANGID_Basque						//locale=0x042D,codepage=1252,oem_cp=850
#define LANGID_Bengali				880		//locale=0x0445,codepage=0
#define LANGID_Brazil_Portuguese	55		//locale=0x0416,codepage=1252
#define LANGID_Bulgarian			359		//locale=0x0402,codepage=1251
//#define LANGID_Byelorussian				//locale=0x0423,codepage=1251,oem_cp=866
#define LANGID_Cantonese 10086				//Chinese (Hong Kong) //locale=0x0C04,codepage=950
//LANGID_Cantonese + LANGID_CODEPAGE_ALT1	//Chinese (Macau) //locale=0x1404,codepage=936
//LANGID_Cantonese + LANGID_CODEPAGE_ALT1	//Chinese (Singapore) //locale=0x1004,codepage=936
#define LANGID_Castilian_Spanish	34		//locale=0x040A,codepage=1252
#define LANGID_Catalan				10034	//locale=0x0403,codepage=1252
#define LANGID_Chinese				86		//Taiwan //locale=0x0404,codepage=950
//LANGID_Chinese + LANGID_CODEPAGE_ALT1		//People's Republic of China //locale=0x0804,codepage=936
#define LANGID_Czech				42		//locale=0x0405,codepage=1250
#define LANGID_Danish				45		//locale=0x0406,codepage=1252
#define LANGID_Dutch				31		// Standard //locale=0x0413,codepage=1252
//      LANGID_Dutch						// Belgium //locale=0x0813,codepage=1252
#define LANGID_Estonian				372		//locale=0x0425,codepage=1257
#define LANGID_Faeroese				298		//locale=0x0438,codepage=1250,oem_cp=850
//#define LANGID_Farsi						//locale=0x0429,codepage=1256, !!!! modified version of 1256!!!!//oem_cp=0
#define LANGID_Finnish				358		//locale=0x040B,codepage=1252
#define LANGID_French				33		//France //locale=0x040C,codepage=1252
//      LANGID_French						//Belgium //locale=0x080C,codepage=1252
//      LANGID_French						//Canada //locale=0x0C0C,codepage=1252
//      LANGID_French						//Luxembourg //locale=0x140C,codepage=1252
//      LANGID_French						//Monaco //locale=0x180C,codepage=1252
//      LANGID_French						//Switzerland //locale=0x100C,codepage=1252
//#define LANGID_Georgian					//locale=0x0437,codepage=0
#define LANGID_German				49		//Germany //locale=0x0407,codepage=1252
//      LANGID_German						//Austria //locale=0x0C07,codepage=1252
//      LANGID_German						//Liechtenstein //locale=0x1407,codepage=1252
//      LANGID_German						//Luxembourg //locale=0x1007,codepage=1252
//      LANGID_German						//Switzerland //locale=0x0807,codepage=1252
#define LANGID_Greek				30		//locale=0x0408,codepage=1253
//#define LANGID_Gujarati					//locale=0x0447,codepage=0
#define LANGID_Hausa				234		// NO LOCALE!!
#define LANGID_Hebrew				972		//locale=0x040D,codepage=1255
#define LANGID_Hindi				91		//locale=0x0439,codepage=0
#define LANGID_Hungarian			36		//locale=0x040E,codepage=1250
#define LANGID_Icelandic			354		//locale=0x040F,codepage=1252
#define LANGID_Indonesian			62		//locale=0x0421,codepage=1252
#define LANGID_Iranian				98		// NO LOCALE!!
#define LANGID_Irish				353		// NO LOCALE!!
#define LANGID_Italian				39		//Italy //locale=0x0410,codepage=1252
//      LANGID_Italian						//Switzerland //locale=0x0810,codepage=1252
#define LANGID_Japanese				81		//locale=0x0411,codepage=932
//#define LANGID_Kannada					//locale=0x044B,codepage=0
//#define LANGID_Kashmiri (India)			//locale=0x0860,codepage=0
//#define LANGID_Kashmiri					//locale=0x0460,codepage=0
//#define LANGID_Kazak						//locale=0x043F,codepage=0
//#define LANGID_Konkani					//locale=0x0457,codepage=0
#define LANGID_Korean				82		//locale=0x0412,codepage=949 (Unified Hangeal code)
//#define LANGID_Korean				1082	//locale=0x0812,codepage=1361(Johab) -see MSDN Q170557
#define LANGID_Latin_Spanish		52		//Modern //locale=0x0C0A,codepage=1252
//      LANGID_Latin_Spanish				//Mexican //locale=0x080A,codepage=1252
//      LANGID_Latin_Spanish				//Argentina //locale=0x2C0A,codepage=1252
//      LANGID_Latin_Spanish				//Bolivia //locale=0x400A,codepage=1252
//      LANGID_Latin_Spanish				//Chile //locale=0x340A,codepage=1252
//      LANGID_Latin_Spanish				//Colombia //locale=0x240A,codepage=1252
//      LANGID_Latin_Spanish				//Costa Rica //locale=0x140A,codepage=1252
//      LANGID_Latin_Spanish				//Dominican Republic //locale=0x1C0A,codepage=1252
//      LANGID_Latin_Spanish				//Ecuador //locale=0x300A,codepage=1252
//      LANGID_Latin_Spanish				//El Salvador //locale=0x440A,codepage=1252
//      LANGID_Latin_Spanish				//Guatemala //locale=0x100A,codepage=1252
//      LANGID_Latin_Spanish				//Honduras //locale=0x480A,codepage=1252
//      LANGID_Latin_Spanish				//Nicaragua //locale=0x4C0A,codepage=1252
//      LANGID_Latin_Spanish				//Panama //locale=0x180A,codepage=1252
//      LANGID_Latin_Spanish				//Paraguay //locale=0x3C0A,codepage=1252
//      LANGID_Latin_Spanish				//Peru //locale=0x280A,codepage=1252
//      LANGID_Latin_Spanish				//Puerto Rico //locale=0x500A,codepage=1252
//      LANGID_Latin_Spanish				//Uruguay //locale=0x380A,codepage=1252
//      LANGID_Latin_Spanish				//Venezuela //locale=0x200A,codepage=1252
#define LANGID_Latvian				371		//locale=0x0426,codepage=1257
#define LANGID_Lithuanian			370		//locale=0x0427,codepage=1257
//#define LANGID_Macedonian					//(the Former Yugoslav Republic of Macedonia)	//locale=0x042F,codepage=0
#define LANGID_Malay				60		//Malaysia //locale=0x043E,codepage=0
//      LANGID_Malay						//Brunei Darussalam //locale=0x083E,codepage=0
//#define LANGID_Malayalam					//locale=0x044C,codepage=0
#define LANGID_Maltese				356		// NO LOCALE!!
//#define LANGID_Manipuri					//locale=0x0458,codepage=0
#define LANGID_Maori				64		// NO LOCALE!!
//#define LANGID_Marathi					//locale=0x044E,codepage=0
//#define LANGID_Nepali						//locale=0x0461,codepage=0
//#define LANGID_Nepali (India)				//locale=0x0861,codepage=0
#define LANGID_Norwegian			47		//Bokmål //locale=0x0414,codepage=1252
//#define LANGID_Norwegian			47		//Nynorsk //locale=0x0814,codepage=1252
//#define LANGID_Oriya						//locale=0x0448,codepage=0
#define LANGID_Polish				48		//locale=0x0415,codepage=1250
#define LANGID_Portuguese			351		//locale=0x0816,codepage=1252
#define LANGID_Punjabi				10092	//locale=0x0446,codepage=0
#define LANGID_Romanian				40		//locale=0x0418,codepage=1250
#define LANGID_Russian				7		//Russia //locale=0x0419,codepage=1251
//      LANGID_Russian						//Ukrainian //locale=0x0422,codepage=1251
//#define LANGID_Sanskrit					//locale=0x044F,codepage=0
#define LANGID_Serbo_Croat			38		//Serbian //locale=0x041A,codepage=1250
//      LANGID_Serbo_Croat					//Serbian (Cyrillic) //locale=0x0C1A,codepage=1250
//      LANGID_Serbo_Croat					//Croatia //locale=0x041A,codepage=1250
//      LANGID_Serbo_Croat					//Serbian (Latin) //locale=0x081A,codepage=1250
#define LANGID_Serbo_Croat_437		10038	// NO LOCALE!!
//#define LANGID_Sindhi						//locale=0x0459,codepage=0
#define LANGID_Slovak				10042	//locale=0x041B,codepage=1250
#define LANGID_Slovene				386		//locale=0x0424,codepage=1250
#define LANGID_Swahili				254		// NO LOCALE!!
#define LANGID_Swedish				46		//Sweden //locale=0x041D,codepage=1252
//      LANGID_Swedish						//Finland //locale=0x081D,codepage=1252
#define LANGID_Tamil				10091	//locale=0x0449,codepage=0
//#define LANGID_Tatar						//locale=0x0444,codepage=0
#define LANGID_Telugu				94		//locale=0x044a,codepage=0
//#define LANGID_Thai						//locale=0x041E,codepage=874,oem_cp=874
#define LANGID_Turkish				90		//locale=0x041F,codepage=1254
#define LANGID_UK_English			44		//Britain //locale=0x0809,codepage=1252
//      LANGID_UK_English					//Australia //locale=0x0C09,codepage=1252
//      LANGID_UK_English					//Belize //locale=0x2809,codepage=1252
//      LANGID_UK_English					//Caribbean //locale=0x2409,codepage=1252
//      LANGID_UK_English					//Ireland //locale=0x1809,codepage=1252
//      LANGID_UK_English					//Jamaica //locale=0x2009,codepage=1252
//      LANGID_UK_English					//New Zealand //locale=0x1409,codepage=1252
//      LANGID_UK_English					//Philippines //locale=0x3409,codepage=1252
//      LANGID_UK_English					//South Africa //locale=0x1C09,codepage=1252
//      LANGID_UK_English					//Trinidad //locale=0x2C09,codepage=1252
//      LANGID_UK_English					//Zimbabwe //locale=0x3009,codepage=1252
#define LANGID_Urdu					92		//India //locale=0x0820,codepage=0
//      LANGID_Urdu							//Pakistan //locale=0x0420,codepage=0
#define LANGID_USA_English			1		//USA //locale=0x0409,codepage=1252
//      LANGID_USA_English					//Canada //locale=0x1009,codepage= //locale=1252
//#define LANGID_Uzbek (Cyrillic)			//locale=0x0843,codepage=0
//#define LANGID_Uzbek (Latin)				//locale=0x0443,codepage=0
//#define LANGID_Vietnamese					//locale=0x042A,codepage=0
#define LANGID_Welsh				10044	// NO LOCALE!!
#define LANGID_Zulu					10027	// NO LOCALE!!
//#define LANGID_Czech_852			1042
//#define LANGID_Slovak_852			11042
//#define LANGID_Chinese_simplified 1086
//#define LANGID_Cantonese_simplified 11086
//#define LANGID_Russian_866		1007
//#define LANGID_Russian			7095

#define LANGID_UNKNOWN				0x40000000

#define LANGID_CODEPAGE_ALT1 1000
#define LANGID_CODEPAGE_ALT2 2000
#define LANGID_CODEPAGE_ALT3 3000
#define LANGID_CODEPAGE_ALT4 4000

#endif // ifndef __INC_SAMH
