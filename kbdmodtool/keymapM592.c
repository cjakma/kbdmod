#include <stdio.h>
#include <string.h>
#include "keymap.h"

#define MAX_COL     6
#define MAX_ROW     19

#define MAX_LAYER   8
#define MAXHEXLINE 32	/* the maximum number of bytes to put in one line */


const char *keycode[256] = {
        "KEY_NONE",
        "ErrorRollOver",
        "POSTFail",
        "ErrorUndefined",
        "KEY_A",              // 0x04
        "KEY_B",
        "KEY_C",
        "KEY_D",
        "KEY_E",
        "KEY_F",
        "KEY_G",
        "KEY_H",
        "KEY_I",
        "KEY_J",
        "KEY_K",
        "KEY_L",
    
        "KEY_M",              // 0x10
        "KEY_N",
        "KEY_O",
        "KEY_P",
        "KEY_Q",
        "KEY_R",
        "KEY_S",
        "KEY_T",
        "KEY_U",
        "KEY_V",
        "KEY_W",
        "KEY_X",
        "KEY_Y",
        "KEY_Z",
        "KEY_1",              //       1 and !
        "KEY_2",              //       2 and @
    
        "KEY_3",              // 0x20  3 and #
        "KEY_4",              //       4 and $
        "KEY_5",              //       5 and %
        "KEY_6",              //       6 and ^
        "KEY_7",              //       7 and &
        "KEY_8",              //       8 and *
        "KEY_9",              //       9 and (
        "KEY_0",              // 0x27  0 and )
        "KEY_ENTER",          // 0x28  enter
        "KEY_ESC",            // 0x29
        "KEY_BKSP",           // 0x2A  backspace
        "KEY_TAB",            // 0x2B
        "KEY_SPACE",          // 0x2C
        "KEY_MINUS",          // 0x2D  - and _
        "KEY_EQUAL",          // 0x2E  = and +
        "KEY_LBR",            // 0x2F  [ and {
    
        "KEY_RBR",            // 0x30  ] and }
        "KEY_BKSLASH",        // 0x31  \ and |
        "KEY_Europe1",        // 0x32  non-US # and ~
        "KEY_COLON",          // 0x33  ; and :
        "KEY_QUOTE",          // 0x34  ' and "
        "KEY_HASH",           // 0x35  grave accent and tilde
        "KEY_COMMA",          // 0x36  ", and <
        "KEY_DOT",            // 0x37  . and >
        "KEY_SLASH",          // 0x38  / and ?
        "KEY_CAPS",           // 0x39
        "KEY_F1",
        "KEY_F2",
        "KEY_F3",
        "KEY_F4",
        "KEY_F5",
        "KEY_F6",
    
        "KEY_F7",             // 0x40
        "KEY_F8",
        "KEY_F9",
        "KEY_F10",
        "KEY_F11",
        "KEY_F12",
        "KEY_PRNSCR",
        "KEY_SCRLCK",
        "KEY_PAUSE",          //Break
        "KEY_INSERT",
        "KEY_HOME",
        "KEY_PGUP",
        "KEY_DEL",
        "KEY_END",
        "KEY_PGDN",
        "KEY_RIGHT",
        
        "KEY_LEFT",           // 0x50
        "KEY_DOWN",
        "KEY_UP",
        "KEY_NUMLOCK",        // Clear
        "KEY_KP_SLASH",
        "KEY_KP_AST",
        "KEY_KP_MINUS",
        "KEY_KP_PLUS",
        "KEY_KP_ENTER",
        "KEY_KP_1",           // End
        "KEY_KP_2",           // Down Arrow
        "KEY_KP_3",           // Page Down
        "KEY_KP_4",           // Left Arrow
        "KEY_KP_5",
        "KEY_KP_6",           // Right Arrow
        "KEY_KP_7",           // Home
        
        "KEY_KP_8",           // 0x60  Up Arrow
        "KEY_KP_9",           // Page Up
        "KEY_KP_0",           // Insert
        "KEY_KP_DOT",         // Delete
        "KEY_Europe2",        // non-US \ and |
        "KEY_APPS",           // 102
        "KEY_POWER_HID",
        "KEY_KP_EQUAL",
        "KEY_LED0",           //
        "KEY_LED1",           //
        "KEY_LED2",           //
        "KEY_LED3",           //
        "KEY_LFX",       //
        "KEY_LPAD",      //
        "KEY_LFULL",     //
        "KEY_LASD",     //
    
        "KEY_LARR",    // 0x70
        "KEY_LVESEL",   
        "KEY_MRESET", 
        "KEY_RESET",          //254
        "KEY_FN",             //255
        "KEY_HELP",
        "KEY_MENU",
        "KEY_SEL",
        "KEY_STOP_HID",
        "KEY_AGAIN",
        "KEY_UNDO",
        "KEY_CUT",
        "KEY_COPY",
        "KEY_PASTE",
        "KEY_FIND",
        "KEY_MUTE_HID",
        
        "KEY_VOLUP",          // 0x80
        "KEY_VOLDN",
        "KEY_KL_CAP",
        "KEY_KL_NUM",
        "KEY_KL_SCL",
        "KEY_KP_COMMA",
        "KEY_EQUAL_SIGN",
        "KEY_L0",             //
        "KEY_L1",             //
        "KEY_L2",             //
        "KEY_L3",             //
        "KEY_L4",             //
        "KEY_L5",             //
        "KEY_L6",             //
        "KEY_INTL8",
        "KEY_INTL9",
        
        "KEY_HANJA",          // 0x90
        "KEY_HANGLE",
        "KEY_KATA",
        "KEY_HIRA",
        "KEY_System",         //KEY_LANG5,
        "KEY_POWER",          //KEY_LANG6,
        "KEY_SLEEP",          //KEY_LANG7,
        "KEY_WAKE",           //KEY_LANG8,
        "KEY_KEYLOCK",        //KEY_LANG9,
        "KEY_WINKEYLOCK",
        "KEY_SYSREQ",
        "KEY_CANCEL",
        "KEY_CLEAR",
        "KEY_PRIOR",
        "KEY_RETURN",
        "KEY_SPERATOR",
        
        "KEY_OUT",            // 0xA0
        "KEY_OPER",
        "KEY_CLR_AGIN",
        "KEY_CRSEL",
        "KEY_EXCEL",
        
    /* These are NOT standard USB HID - handled specially in decoding", 
         so they will be mapped to the modifier byte in the USB report */
        "KEY_Modifiers",
        "KEY_LCTRL",          // 0x01
        "KEY_LSHIFT",         // 0x02
        "KEY_LALT",           // 0x04
        "KEY_LGUI",           // 0x08
        "KEY_RCTRL",          // 0x10
        "KEY_RSHIFT",         // 0x20
        "KEY_RALT",           // 0x40
        "KEY_RGUI",           // 0x80
        "KEY_Modifiers_end",
    
       "KEY_NEXT_TRK",        
       "KEY_PREV_TRK",        // 0xB0
       "KEY_STOP",
       "KEY_PLAY",
       "KEY_MUTE",
       "KEY_BASS_BST",
       "KEY_LOUDNESS",
       "KEY_VOL_UP",
       "KEY_VOL_DOWN",
       "KEY_BASS_UP",
       "KEY_BASS_DN",
       "KEY_TRE_UP",
       "KEY_TRE_DN",
    
       "KEY_MEDIA_SEL",      // 0xc0
       "KEY_MAIL",
       "KEY_CALC",
       "KEY_MYCOM",
       "KEY_WWW_SEARCH",
       "KEY_WWW_HOME",
       "KEY_WWW_BACK",
       "KEY_WWW_FORWARD",
       "KEY_WWW_STOP",
       "KEY_WWW_REFRESH",
       "KEY_WWW_FAVORITE",
       "KEY_EJECT",
       "KEY_SCREENSAVE",
       "KEY_REC",
        "KEY_REWIND",
        "KEY_MINIMIZE",
    
       "KEY_M01",            // 0xd0 //KEY_F13,
       "KEY_M02",            //KEY_F14,
       "KEY_M03",            //KEY_F15,
       "KEY_M04",            //KEY_F16,
       "KEY_M05",            //KEY_F17,
       "KEY_M06",            //KEY_F18,
       "KEY_M07",            //KEY_F19,
       "KEY_M08",            //KEY_F20,
       "KEY_M09",            //KEY_F21,        // 0x70
       "KEY_M10",            //KEY_F22,
       "KEY_M11",            //KEY_F23,
       "KEY_M12",            //KEY_F24,
       "KEY_M13",            //KEY_INTL1,
       "KEY_M14",            //KEY_INTL2,
       "KEY_M15",            //KEY_INTL3,
       "KEY_M16",            //KEY_INTL4,
    
       "KEY_M17",            // 0xe0 //KEY_INTL5,
       "KEY_M18",            //KEY_INTL6,
       "KEY_M19",            //KEY_INTL7,
       "KEY_M20",        
       "KEY_M21",        
       "KEY_M22",        
       "KEY_M23",       
       "KEY_M24",   
       "KEY_M25",            //213
       "KEY_M26",            //214
       "KEY_M27",            //215
       "KEY_M28",            //216
       "KEY_M29",            //217
       "KEY_M30",            //218
       "KEY_M31",            //219
       "KEY_M32",            //220
    
       "KEY_M33",            // 0xf0//221
       "KEY_M34",            //222
       "KEY_M35",            //223
       "KEY_M36",            //224
       "KEY_M37",            //225
       "KEY_M38",            //226
       "KEY_M39",            //227
       "KEY_M40",            //228
       "KEY_M41",            //229
       "KEY_M42",            //230
       "KEY_M43",            //231
       "KEY_M44",            //232
       "KEY_M45",            //233
       "KEY_M46",            //234
       "KEY_M47",            //235
       "KEY_M48",            //236
       "KEY_M49",
       "KEY_M50",
       "KEY_M51",
       "KEY_M52"
};
    


// Total 132 keys + one none


unsigned char keymap_code[MAX_LAYER][MAX_COL][MAX_ROW] =  {
	{
		// LAYER 0 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_ESC,   KEY_NONE,   KEY_F1, KEY_F2, KEY_F3,	KEY_F4,	KEY_F5, KEY_F6, KEY_F7,     KEY_F8, KEY_F9,     KEY_F10,    KEY_F11,    KEY_F12,    KEY_FN,     KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE, KEY_NONE}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_INSERT, KEY_HOME,   KEY_PGUP, KEY_NONE},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN, KEY_NONE},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE, KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE, KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS, KEY_RCTRL,  KEY_NONE,   KEY_NONE,   KEY_LEFT,   KEY_DOWN,   KEY_RIGHT, KEY_NONE}  // COL5
	},
	{	
		// LAYER 1 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_ESC,   KEY_NONE,   KEY_F1, KEY_F2, KEY_F3,	KEY_F4,	KEY_F5, KEY_F6, KEY_F7,     KEY_F8, KEY_F9,     KEY_F10,    KEY_F11,    KEY_F12,    KEY_FN,     KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE, KEY_NONE}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_INSERT, KEY_HOME,   KEY_PGUP, KEY_NONE},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN, KEY_NONE},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE, KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE, KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT, KEY_NONE}  // COL5
	},
	{	
         // LAYER 2 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_2,     KEY_F1,     KEY_F2, KEY_F3,	KEY_F4,	KEY_F5, KEY_F6, KEY_F7, KEY_F8,     KEY_F9, KEY_F10,    KEY_F11,    KEY_F12,    KEY_NONE,   KEY_FN,     KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE, KEY_NONE}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_INSERT, KEY_HOME,   KEY_PGUP, KEY_NONE},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN, KEY_NONE},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE, KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE, KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT, KEY_NONE}  // COL5

	},
	{	
        // LAYER 3 : 1800
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_ESC,   KEY_F1,     KEY_F2, KEY_F3, KEY_F4,	KEY_F5, KEY_F6, KEY_F7, KEY_F8,     KEY_F9, KEY_F10,    KEY_F11,    KEY_F12,    KEY_DEL,    KEY_FN,     KEY_HOME,   KEY_PGUP,   KEY_NUMLOCK, KEY_NONE}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_END,    KEY_PGDN,   KEY_KP_MINUS, KEY_NONE},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_KP_7,   KEY_KP_8,   KEY_KP_9, KEY_NONE},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_KP_4,   KEY_KP_5,   KEY_KP_6, KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_UP,     KEY_NONE,   KEY_KP_1,   KEY_KP_2,   KEY_KP_3, KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,   KEY_LEFT,   KEY_DOWN,   KEY_RIGHT,  KEY_KP_0,   KEY_KP_DOT, KEY_NONE}  // COL5

	},
	{	
        // LAYER 4 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{ KEY_NEXT_TRK,
            KEY_PREV_TRK,
            KEY_STOP,
            KEY_PLAY,
            KEY_MUTE,
            KEY_BASS_BST,
            KEY_LOUDNESS,
            KEY_VOL_UP,
            KEY_VOL_DOWN,
            KEY_BASS_UP,
            KEY_BASS_DN,
            KEY_TRE_UP,
            KEY_TRE_DN,
            KEY_MEDIA_SEL,
            KEY_MAIL,
            KEY_CALC,
            KEY_MYCOM,
            KEY_WWW_SEARCH,
            KEY_WWW_SEARCH,
            }, // COL0
        		{KEY_WWW_HOME,
            KEY_WWW_BACK,
            KEY_WWW_FORWARD,
            KEY_WWW_STOP,
            KEY_WWW_REFRESH,
            KEY_WWW_FAVORITE,
            KEY_EJECT,
            KEY_SCREENSAVE,
            KEY_REC,
        	KEY_REWIND,
        	KEY_MINIMIZE,

            KEY_System,
            KEY_POWER,
            KEY_SLEEP,
            KEY_WAKE,KEY_WAKE,KEY_VOL_UP,KEY_BASS_DN,KEY_NONE},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN, KEY_NONE},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE, KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE, KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT, KEY_NONE}  // COL5

	},
	{	
         // LAYER 5 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_5,     KEY_F1,     KEY_F2, KEY_F3,	KEY_F4,	KEY_F5, KEY_F6, KEY_F7, KEY_F8,     KEY_F9, KEY_F10,    KEY_F11,    KEY_F12,    KEY_NONE,   KEY_FN,   KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE, KEY_NONE}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_INSERT, KEY_HOME,   KEY_PGUP, KEY_NONE},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN, KEY_NONE},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE, KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE, KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT, KEY_NONE}  // COL5

	},
	{
         // LAYER 6: normal
        // ROW0        ROW1            ROW2     ROW3     ROW4   ROW5    ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
        {KEY_LFX,   KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_FN,     KEY_LFX,    KEY_LFX,    KEY_LFX, KEY_NONE}, // COL0
        {KEY_LFULL, KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LPAD,   KEY_LPAD,   KEY_LPAD, KEY_NONE},  // COL1
        {KEY_LFULL, KEY_LFULL,  KEY_LASD,   KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_NONE,   KEY_LPAD,   KEY_LPAD,   KEY_LPAD, KEY_NONE},  // COL2
        {KEY_LFULL, KEY_LASD,   KEY_LASD,   KEY_LASD,   KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_NONE,   KEY_NONE,   KEY_LPAD,   KEY_LPAD,   KEY_LPAD, KEY_NONE},  // COL3
        {KEY_LFULL, KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LARR,   KEY_NONE,   KEY_LPAD,   KEY_LARR,   KEY_LPAD, KEY_NONE},  // COL4
        {KEY_LFULL, KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LARR,   KEY_LARR,   KEY_LARR,   KEY_LARR,   KEY_LARR, KEY_NONE}  // COL5

     },
        

	{	
		// LAYER  FN
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_L0,    KEY_M02,    KEY_M03,  KEY_M04,  KEY_M05,  KEY_M06,  KEY_M07,  KEY_M08,  KEY_M09, KEY_M10,  KEY_M11, KEY_M12,   KEY_M13,    KEY_M48,     KEY_FN,     KEY_MRESET, KEY_KEYLOCK,    KEY_RESET, KEY_NONE}, // COL0
		{KEY_M01,   KEY_L1,     KEY_L2,   KEY_L3,   KEY_L4,	KEY_LED0, KEY_LED1, KEY_LED2, KEY_LED3,KEY_9,    KEY_0,   KEY_M48,   KEY_M49,    KEY_BKSP,    KEY_NONE,   KEY_KP_AST, KEY_KP_SLASH,   KEY_KP_PLUS, KEY_NONE},  // COL1
		{KEY_TAB,   KEY_M14,    KEY_M15,  KEY_M16,  KEY_M17,  KEY_M18,  KEY_M19,  KEY_M20,  KEY_M21, KEY_M22,  KEY_M23, KEY_M24,   KEY_M25,    KEY_M26,     KEY_NONE,   KEY_KP_7,   KEY_KP_8,       KEY_KP_9, KEY_NONE},  // COL2
		{KEY_CAPS,  KEY_M27,    KEY_M28,  KEY_M29,  KEY_M30,  KEY_M31,  KEY_M32,  KEY_M33,  KEY_M34, KEY_M35,  KEY_M36, KEY_M37,   KEY_ENTER,  KEY_NONE,    KEY_NONE,   KEY_KP_4,   KEY_KP_5,       KEY_KP_6, KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_M38,    KEY_M39,  KEY_M40,  KEY_M41,  KEY_M42,  KEY_M43,  KEY_M44,  KEY_M45, KEY_M46,  KEY_M47, KEY_NONE,  KEY_RSHIFT, KEY_NONE,    KEY_NONE,   KEY_KP_1,   KEY_KP_2,       KEY_KP_3, KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_WINKEYLOCK,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_WINKEYLOCK,KEY_APPS,KEY_RCTRL,   KEY_LEFT,   KEY_DOWN,   KEY_RIGHT,  KEY_KP_0,   KEY_KP_DOT, KEY_NONE}  // COL5

	}
	
 };

typedef enum
{
    LED_EFFECT_FADING          = 0,
    LED_EFFECT_FADING_PUSH_ON  = 1,
    LED_EFFECT_PUSHED_LEVEL    = 2,
    LED_EFFECT_PUSH_ON         = 3,
    LED_EFFECT_PUSH_OFF        = 4,
    LED_EFFECT_ALWAYS          = 5,
    LED_EFFECT_BASECAPS        = 6,
    LED_EFFECT_OFF             = 7,
    LED_EFFECT_NONE
}LED_MODE;

#define LEDMODE_ADDRESS 0x9800
unsigned char  ledmode[5][11] ={ 
                    {LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, 
                    LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, 
                    LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS},

                    {LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, 
                    LED_EFFECT_ALWAYS, LED_EFFECT_FADING, LED_EFFECT_FADING, LED_EFFECT_FADING, 
                    LED_EFFECT_FADING, LED_EFFECT_FADING, LED_EFFECT_FADING},

                    {LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, 
                    LED_EFFECT_ALWAYS, LED_EFFECT_FADING_PUSH_ON, LED_EFFECT_FADING_PUSH_ON, LED_EFFECT_FADING_PUSH_ON, 
                    LED_EFFECT_FADING_PUSH_ON, LED_EFFECT_FADING_PUSH_ON, LED_EFFECT_FADING_PUSH_ON},

                    {LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, 
                    LED_EFFECT_ALWAYS, LED_EFFECT_PUSHED_LEVEL, LED_EFFECT_PUSHED_LEVEL, LED_EFFECT_PUSHED_LEVEL, 
                    LED_EFFECT_PUSHED_LEVEL, LED_EFFECT_PUSHED_LEVEL, LED_EFFECT_PUSHED_LEVEL},

                    {LED_EFFECT_OFF, LED_EFFECT_OFF, LED_EFFECT_OFF, LED_EFFECT_OFF, 
                    LED_EFFECT_FADING, LED_EFFECT_OFF, LED_EFFECT_OFF, LED_EFFECT_OFF, 
                    LED_EFFECT_OFF, LED_EFFECT_OFF, LED_EFFECT_OFF}
};


int keymAddress[MAX_LAYER] = {
    0x9000, // FACTORY DEFAULT
    0x9100,
    0x9200,
    0x9300,
    0x9400,
    0x9500,
    0x9600, // LED LAYER
    0x9700, // FN LAYER
    };
    

int addressExtended = 0;


char getKeyIdx(const char *keystring)
{
    unsigned int i;
    unsigned char temp;
    printf("%s", keystring);
//    scanf("%c", &temp);
    for (i = 0; i < 256; i++)
    {
        if(strcmp(keystring, keycode[i]) == 0)
        {
            printf(": found!!\n");
            return (char)i;
        }
    }
    printf("ERROR: matrix is invalied ! \n");
    return 0;
}


int interprete(const char *filename, char *pbuf)
{
    FILE *fp = fopen(filename, "r");
    char str[256];
    char keyidx;
    size_t len;
    int i, j;
    for (i = 0; i < 1024; i++)
    {
        i = 0;
        // 1 start with "K" "E" "P"
        do{
            len = fread(&str[i], 1, 1, fp);
            if(len != 1)
            {
                return 0;
            }
            printf("%c : %d", str[i], len);
        }while(str[i] != 'K' && str[i] != 'E' && str[i] != 'P');

        // 2. parse a "KEY_xxx"
        for (j = 1; j<256; j++)
        {
            len = fread(&str[j], 1, 1, fp);
//            printf("%c\n", str[j]);
            if(str[j] == ',' || str[j] == ' ')           // seperate
            {
                str[j] = '\0';
                break;
            }   
            if(len != 1)
            {
                fclose(fp);
                return 0;
            }   
        }

        // 3. get key
        keyidx = getKeyIdx(str);

        // 4. put key to matrix
        *pbuf++ = keyidx;
    }        
    fclose(fp);
    return 0;
}


void insertExtSegAddr(FILE *fp)
{
   fprintf(fp,":020000021000EC\n");
}      

void revertExtSegAddr(FILE *fp)
{
   fprintf(fp,":020000020000FC\n");
}
int buffer2Hex(FILE *fp, int address, int length, unsigned char *buffer)
{
   unsigned char checksum = 0;
   int j;
   char cnt;   

   cnt = length;

   while (length > 0)
   {
      
      if (!addressExtended && (address >= 0x10000))
      {
         insertExtSegAddr(fp);
         addressExtended = 1;
      }else if (addressExtended && (address < 0x10000))
      {
         revertExtSegAddr(fp);
         addressExtended = 0;
      }

      length = length - 0x10;

      cnt = length >= 0 ? 0x10 : length + 0x10;

      fprintf(fp,":%02X", cnt);
      checksum += cnt;
      fprintf(fp,"%04X", (unsigned short)address);
      checksum += ((address >> 8) & 0xFF);
      checksum += address & 0xFF;
      fprintf(fp,"00");
      checksum += 0x00;

      for(j = 0; j < cnt; j++)
      {
         fprintf(fp,"%02X", *buffer);
         checksum += *buffer;
         buffer++;
      }
      checksum = ~(checksum & 0xFF) + 1;
      fprintf(fp,"%02X\n", checksum & 0xFF);
      address += 0x10;
      checksum = 0;
   }
   return 0;
}

 unsigned char hello[] = "hello world";

int main(int argc, char *argv[])
{
   
   int layer;
   int address;
   unsigned char *keymap;
   
   FILE *fp = fopen("keymap.hex", "w");


   interprete("keymapM592.txt", &(keymap_code[layer][0][0]));



   for (layer = 0; layer < MAX_LAYER ; layer++)
   {
      address = keymAddress[layer];
      keymap = &(keymap_code[layer][0][0]);
      
      buffer2Hex(fp, address, MAX_COL * MAX_ROW, keymap);
   }

   keymap = &(ledmode[0][0]);

   buffer2Hex(fp, LEDMODE_ADDRESS, 256, keymap);
   
   fclose(fp);
   return 0;
}

