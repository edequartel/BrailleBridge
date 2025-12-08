////////////////////////////////////////////////////////////////////////////////
//
// File Name    : sam.h
// File Version : 2.0
// Description  : This file defines some common actions for Braille software.
//                Use these defines in conjunction with the
//                BrldGetButtonCombination function to obtain default button
//                assignments for the actions.
//
// Copyright (C) 1997-2002 Dolphin Oceanic Ltd.
//
////////////////////////////////////////////////////////////////////////////////
// Version History:
//
// $Log: /Library/brldacts.h $
// 
// 2     4/12/02 10:08 Druck
// SAM version 2 changes
// 
// 3     1/11/02 9:20 Druck
// NUMBRLACTS value corrected
// 
// 2     28/10/02 11:15 Druck
// SAM version 2 changes
//
////////////////////////////////////////////////////////////////////////////////

#define SAMBRLACT_NO_ACTION          0	// No action
#define SAMBRLACT_WINDOW_TOPLEFT     1	// Move display to top left of window area
#define SAMBRLACT_WINDOW_BTMLEFT     2	// Move display to bottom left of window area
#define SAMBRLACT_WINDOW_FOCUS       3	// Move display to focus (or caret)
#define SAMBRLACT_WINDOW_LEFT        4	// Move display to left edge of window
#define SAMBRLACT_WINDOW_RIGHT       5	// Move display to right edge of window

#define SAMBRLACT_LINE_UP            6	// Move display one line up   
#define SAMBRLACT_LINE_DOWN          7	// Move display one line down
#define SAMBRLACT_CHAR_LEFT          8	// Move display one character left
#define SAMBRLACT_CHAR_RIGHT         9	// Move display one character right
#define SAMBRLACT_WIDTH_LEFT        10	// Move display one display width left
#define SAMBRLACT_WIDTH_RIGHT       11	// Move display one display width right
#define SAMBRLACT_HWIDTH_LEFT       12	// Move display one half width left
#define SAMBRLACT_HWIDTH_RIGHT      13	// Move display one half width right

#define SAMBRLACT_PREVIOUS          14	// Move back one window of data (wraps across lines)
#define SAMBRLACT_NEXT              15	// Move forward one width of data (wraps across lines)

#define SAMBRLACT_TOGGLE_TRACK      16	// Toggle caret/focus tracking (on/off)
#define SAMBRLACT_ATTRIB1			17	// Switch to attribute mode 1
#define SAMBRLACT_ATTRIB2			18	// Switch to attribute mode 2
#define SAMBRLACT_REPORT			19	// Switch to status report mode
#define SAMBRLACT_TOGGLE_EIGHTDOT   20	// Toggle between eight and six dot Braille
#define SAMBRLACT_TOGGLE_CTYPE      21	// Toggle through available cursor shapes
#define SAMBRLACT_TOGGLE_CVIS       22	// Toggle cursor visibility
#define SAMBRLACT_TOGGLE_AUDIO      23	// Toggle audio feedback
#define SAMBRLACT_TOGGLE_BLANKS     24	// Skip over blank lines (on/off)
#define SAMBRLACT_TOGGLE_BRAILLE    25	// Turns Braille display on and off
#define SAMBRLACT_TOGGLE_LAYOUT     26	// Cells show distance on screen
#define SAMBRLACT_TOGGLE_TREMBLE    27	// Toggle capital letters blink
#define SAMBRLACT_TOGGLE_ENHANCE    28	// Show non-plain characters in dot 8
#define SAMBRLACT_LEFTCLICK         29	// Next cursor route is single click
#define SAMBRLACT_DOUBLECLICK       30	// Next cursor route is double click
#define SAMBRLACT_RIGHTCLICK        31	// Next cursor route is right click
#define SAMBRLACT_CONFIG            32	// Activate configuration dialog

// introduced for SAM version 2
#define SAMBRLACT_LITERARY_BRAILLE	33	// Toggles between computer and literary braille
#define SAMBRLACT_TAB				34	// Duplicates the keyboard TAB key
#define SAMBRLACT_SHIFT_TAB			35	// Duplicates the keyboard Shift+TAB keys
#define SAMBRLACT_ENTER				36	// Duplicates the keyboard Enter key
#define SAMBRLACT_ESCAPE			37	// Duplicates the keyboard Escape key
#define SAMBRLACT_CURSOR_UP			38	// Duplicates the keyboard cursor up key
#define SAMBRLACT_CURSOR_DOWN		39	// Duplicates the keyboard cursor down key
#define SAMBRLACT_CURSOR_LEFT		40	// Duplicates the keyboard cursor left key
#define SAMBRLACT_CURSOR_RIGHT		41	// Duplicates the keyboard cursor right key
#define SAMBRLACT_BRAILLE_KEY_1		42	// Braille entry key row 1 left
#define SAMBRLACT_BRAILLE_KEY_2		43	// Braille entry key row 1 right
#define SAMBRLACT_BRAILLE_KEY_3		44	// Braille entry key row 2 left
#define SAMBRLACT_BRAILLE_KEY_4		45	// Braille entry key row 2 right
#define SAMBRLACT_BRAILLE_KEY_5		46	// Braille entry key row 3 left
#define SAMBRLACT_BRAILLE_KEY_6		47	// Braille entry key row 3 right
#define SAMBRLACT_BRAILLE_KEY_7		48	// Braille entry key row 4 left
#define SAMBRLACT_BRAILLE_KEY_8		49	// Braille entry key row 4 right
#define SAMBRLACT_BRAILLE_SPACE		50	// Braille space key

// Define total number of defined actions
#define NUMBRLACTS   51
