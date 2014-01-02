#include "keymap.h"


const uint8_t PROGMEM keycode_set2[NUM_KEY] =	{
    0x00, 0x00, 0xFC, 0x00,
    0x1C, 0x32, 0x21, 0x23, 0x24, 0x2B, 0x34, 0x33, 0x43, 0x3B,	0x42, 0x4B,                             // KEY_A    (0x04)
    0x3A, 0x31, 0x44, 0x4D, 0x15, 0x2D, 0x1B, 0x2C, 0x3C, 0x2A, 0x1D, 0x22, 0x35, 0x1A, 0x16, 0x1E,     // KEY_M    (0x10)
    0x26, 0x25,	0x2E, 0x36, 0x3D, 0x3E, 0x46, 0x45, 0x5A, 0x76, 0x66, 0x0D,	0x29, 0x4E, 0x55, 0x54,     // KEY_3     (0x20)
    0x5B, 0x5D, 0x5D, 0x4C, 0x52, 0x0E, 0x41, 0x49, 0x4A, 0x58, 0x05, 0x06, 0x04, 0x0C, 0x03, 0x0B,     // KEY_RBR  (0x30)
    0x83, 0x0A, 0x01, 0x09, 0x78, 0x07, 0x7C, 0x7E, 0x77, 0x70, 0x6C, 0x7D, 0x71, 0x69, 0x7A, 0x74,     // KEY_F7    (0x40)
    0x6B, 0x72, 0x75, 0x77, 0x4A, 0x7C, 0x7B, 0x79, 0x5A, 0x69, 0x72, 0x7A, 0x6B, 0x73, 0x74, 0x6C,     // KEY_LEFT (0x50)
    0x75, 0x7D, 0x70, 0x71, 0x61, 0x2F,	0x37, 0x0f, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40,		// KEY_KP8  (0x60)
    0x48, 0x50, 0x57, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // KEY_F21  (0x70)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x6D, 0x00, 0x51, 0x13, 0x6A, 0x64, 0x67, 0x27, 0x00, 0x00, 0x00,     // KEY_VOLUP (0x80)
    0xF2, 0xF1, 0x63, 0x62, 0x5F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // KEY_HANJA (0x90)
    0x00, 0x00, 0x00, 0x00, 0x00,                                                                       // ~KEY_EXCEL (0xA4)
   		
   	0x00,                                                                                               // KEY_Modifiers (0xA5)
    0x14, 0x12, 0x11, 0x1F, 0x14, 0x59, 0x11, 0x27, 
    0x00,		                                                                                        // KEY_Modifiers_end (0xAE)
    0x00, 0x00, 0x00,                                                                                    // ~KEY_000(0xB1)
    
    // now only for PS/2
    0X00,
    0x4D, 0x15, 0x3B, 0x34, 0x23, 0x00, 0x00, 0x32, 0x21, 0x00, 0x00, 0x00, 0x00, 0x50, 0x48, 0x2B,
    0x40, 0x10, 0x3A, 0x38, 0x30, 0x28, 0x20, 0x18,

    0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x37, 0x3F, 0x5E
};

// key information for each keys
uint8_t KFLA[NUM_KEY];

const uint8_t PROGMEM keycode_set2_special[] = 
{ 	KEY_PRNSCR, KEY_PAUSE,
	KEY_NONE };

const uint8_t PROGMEM keycode_set2_makeonly[] = 
{ 	KEY_PAUSE, KEY_HANGLE, KEY_HANJA,
	KEY_NONE };

const uint8_t PROGMEM keycode_set2_make_break[] =
{ 	KEY_POWER, KEY_SLEEP, KEY_WAKE,
	KEY_NONE };

const uint8_t PROGMEM keycode_set2_extend[] =
{	KEY_LGUI, KEY_RCTRL, KEY_RGUI, KEY_RALT, KEY_APPS, KEY_PRNSCR,
	KEY_INSERT, KEY_HOME, KEY_PGUP, KEY_DEL, KEY_END, KEY_PGDN, 
	KEY_UP, KEY_LEFT, KEY_RIGHT, KEY_DOWN, KEY_KP_SLASH, KEY_KP_ENTER,
	KEY_POWER, KEY_SLEEP, KEY_WAKE, KEY_MAIL, KEY_WWW_SEARCH, KEY_WWW_HOME,
	KEY_WWW_BACK, KEY_WWW_FORWARD, KEY_WWW_STOP, KEY_WWW_REFRESH, KEY_WWW_FAVORITE,
	KEY_NEXT_TRK, KEY_PREV_TRK, KEY_STOP, KEY_PLAY, KEY_MUTE, KEY_VOL_UP, 
	KEY_VOL_DOWN, KEY_MEDIA_SEL, KEY_CALC, KEY_MYCOM, KEY_SCREENSAVE, KEY_REC,
	KEY_REWIND, KEY_MINIMIZE, KEY_EJECT, 
	KEY_NONE };

const uint8_t PROGMEM keycode_set2_proc_shift[] = 
{
	KEY_INSERT, KEY_DEL, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN, KEY_LGUI, KEY_RGUI, KEY_APPS,
 	KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_KP_SLASH,
	KEY_NONE };

uint32_t keymap[MAX_LAYER] = {
   0x4800,  0x4880,  0x4900,  0x4980,  0x4A00,  0x4A80,  0x4B00,  0x4B80};


#if 0 //def MX3018
const uint8_t PROGMEM keymap_code[MAX_LAYER][MAX_COL][MAX_ROW] =  {
	{
		// LAYER 0 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_ESC,   KEY_NONE,   KEY_F1, KEY_F2, KEY_F3,	KEY_F4,	KEY_F5, KEY_F6, KEY_F7,     KEY_F8, KEY_F9,     KEY_F10,    KEY_F11,    KEY_F12,    KEY_FN,     KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_INSERT, KEY_HOME,   KEY_PGUP},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT}  // COL5
	},
	{	
		// LAYER 1 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_ESC,   KEY_F1,     KEY_F2, KEY_F3,	KEY_F4,	KEY_F5, KEY_F6, KEY_F7, KEY_F8,     KEY_F9, KEY_F10,    KEY_F11,    KEY_F12,    KEY_NONE,   KEY_NONE,   KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_KP_7,   KEY_KP_8,   KEY_KP_9},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_KP_4,   KEY_KP_5,   KEY_KP_6},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_KP_1,   KEY_KP_2,   KEY_KP_3},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_KP_0,   KEY_UP,     KEY_KP_DOT},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT}  // COL5

	},
	{	
         // LAYER 2 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_2,     KEY_F1,     KEY_F2, KEY_F3,	KEY_F4,	KEY_F5, KEY_F6, KEY_F7, KEY_F8,     KEY_F9, KEY_F10,    KEY_F11,    KEY_F12,    KEY_NONE,   KEY_NONE,   KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_INSERT, KEY_HOME,   KEY_PGUP},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT}  // COL5

	},
	{	
        // LAYER 3 : 1800
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_ESC,   KEY_F1,     KEY_F2, KEY_F3, KEY_F4,	KEY_F5, KEY_F6, KEY_F7, KEY_F8,     KEY_F9, KEY_F10,    KEY_F11,    KEY_F12,    KEY_DEL,    KEY_FN,     KEY_HOME,   KEY_PGUP,   KEY_NUMLOCK}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_END,    KEY_PGDN,   KEY_KP_MINUS},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_KP_7,   KEY_KP_8,   KEY_KP_9},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_KP_4,   KEY_KP_5,   KEY_KP_6},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_UP,     KEY_NONE,   KEY_KP_1,   KEY_KP_2,   KEY_KP_3},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,   KEY_LEFT,   KEY_DOWN,   KEY_RIGHT,  KEY_KP_0,   KEY_KP_DOT}  // COL5

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
            KEY_WAKE,KEY_WAKE,KEY_00,KEY_000,},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT}  // COL5

	},
	{	
         // LAYER 5 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_5,     KEY_F1,     KEY_F2, KEY_F3,	KEY_F4,	KEY_F5, KEY_F6, KEY_F7, KEY_F8,     KEY_F9, KEY_F10,    KEY_F11,    KEY_F12,    KEY_NONE,   KEY_NONE,   KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_INSERT, KEY_HOME,   KEY_PGUP},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT}  // COL5

	},
	{
         // LAYER 6: normal
        // ROW0        ROW1            ROW2     ROW3     ROW4   ROW5    ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
        {KEY_6,     KEY_F1,     KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8,     KEY_F9, KEY_F10,    KEY_F11,    KEY_F12,    KEY_NONE,   KEY_NONE,   KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE}, // COL0
        {KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_INSERT, KEY_HOME,   KEY_PGUP},  // COL1
        {KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN},  // COL2
        {KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE},  // COL3
        {KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE},  // COL4
        {KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT}  // COL5

     },
        

	{	
		// LAYER  FN
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_L0,    KEY_L1,     KEY_L2, KEY_L3, KEY_L4,	KEY_LED0, KEY_LED1, KEY_LED2, KEY_LED3, KEY_LED4, KEY_LED5, KEY_LED6,   KEY_LED7,   KEY_DEL, KEY_FN,    KEY_HOME,   KEY_PGUP,   KEY_RESET}, // COL0
		{KEY_M48,   KEY_M49,    KEY_M50,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_KP_AST, KEY_KP_SLASH, KEY_KP_PLUS},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_KP_7,   KEY_KP_8,   KEY_KP_9},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_KP_4,   KEY_KP_5,   KEY_KP_6},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_KP_1,   KEY_KP_2,   KEY_KP_3},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,   KEY_LEFT,   KEY_DOWN,   KEY_RIGHT,  KEY_KP_0,   KEY_KP_DOT}  // COL5

	}
	
 };
#endif
