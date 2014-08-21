enum {
	K_NONE=0,
    ErrorRollOver,
    POSTFail,
    ErrorUndefined,
    K_A,              // 0x04
    K_B,
    K_C,
    K_D,
    K_E,
    K_F,
    K_G,
    K_H,
    K_I,
    K_J,
    K_K,
    K_L,

    K_M,              // 0x10
    K_N,
    K_O,
    K_P,
    K_Q,
    K_R,
    K_S,
    K_T,
    K_U,
    K_V,
    K_W,
    K_X,
    K_Y,
    K_Z,
    K_1,              //       1 and !
    K_2,              //       2 and @

    K_3,              // 0x20  3 and #
    K_4,              //       4 and $
    K_5,              //       5 and %
    K_6,              //       6 and ^
    K_7,              //       7 and &
    K_8,              //       8 and *
    K_9,              //       9 and (
    K_0,              // 0x27  0 and )
    K_ENTER,          // 0x28  enter
    K_ESC,            // 0x29
    K_BKSP,           // 0x2A  backspace
    K_TAB,            // 0x2B
    K_SPACE,          // 0x2C
    K_MINUS,          // 0x2D  - and _
    K_EQUAL,          // 0x2E  = and +
    K_LBR,            // 0x2F  [ and {

    K_RBR,            // 0x30  ] and }
    K_BKSLASH,        // 0x31  \ and |
    K_Europe1,        // 0x32  non-US # and ~
    K_COLON,          // 0x33  ; and :
    K_QUOTE,          // 0x34  ' and "
    K_HASH,           // 0x35  grave accent and tilde
    K_COMMA,          // 0x36  , and <
    K_DOT,            // 0x37  . and >
    K_SLASH,          // 0x38  / and ?
    K_CAPS,           // 0x39
    K_F1,
    K_F2,
    K_F3,
    K_F4,
    K_F5,
    K_F6,

    K_F7,             // 0x40
    K_F8,
    K_F9,
    K_F10,
    K_F11,
    K_F12,
    K_PRNSCR,
    K_SCRLCK,
    K_PAUSE,          //Break
    K_INSERT,
    K_HOME,
    K_PGUP,
    K_DEL,
    K_END,
    K_PGDN,
    K_RIGHT,
    
    K_LEFT,           // 0x50
    K_DOWN,
    K_UP,
    K_NUMLOCK,        // Clear
    K_KP_SLASH,
    K_KP_AST,
    K_KP_MINUS,
    K_KP_PLUS,
    K_KP_ENTER,
    K_KP_1,           // End
    K_KP_2,           // Down Arrow
    K_KP_3,           // Page Down
    K_KP_4,           // Left Arrow
    K_KP_5,
    K_KP_6,           // Right Arrow
    K_KP_7,           // Home
    
    K_KP_8,           // 0x60  Up Arrow
    K_KP_9,           // Page Up
    K_KP_0,           // Insert
    K_KP_DOT,         // Delete
    K_Europe2,        // non-US \ and |
    K_APPS,		    // 102
    K_POWER_HID,
    K_KP_EQUAL,
    K_LED0,           //
    K_LED1,           //
    K_LED2,           //
    K_LED3,           //
    K_LFX,       //
    K_LPAD,      //
    K_LFULL,     //
    K_LASD,     //

    K_LARR,    // 0x70
    K_LVESEL,   
    K_MRESET, 
    K_RESET,          //254
    K_FN,             //255
    K_HELP,
    K_MENU,
    K_SEL,
    K_STOP_HID,
    K_AGAIN,
    K_UNDO,
    K_CUT,
    K_COPY,
    K_PASTE,
    K_FIND,
    K_MUTE_HID,
    
    K_VOLUP,          // 0x80
    K_VOLDN,
    K_KL_CAP,
    K_KL_NUM,
    K_KL_SCL,
    K_KP_COMMA,
    K_EQUAL_SIGN,
    K_L0,             //
    K_L1,             //
    K_L2,             //
    K_L3,             //
    K_L4,             //
    K_L5,             //
    K_L6,             //
    K_INTL8,
    K_INTL9,
    
    K_HANJA,          // 0x90
    K_HANGLE,
    K_KATA,
    K_HIRA,
    K_System,         //K_LANG5,
    K_POWER,          //K_LANG6,
    K_SLEEP,          //K_LANG7,
    K_WAKE,           //K_LANG8,
    K_KEYLOCK,        //K_LANG9,
    K_WINKEYLOCK,
    K_SYSREQ,
    K_CANCEL,
    K_CLEAR,
    K_PRIOR,
    K_RETURN,
    K_SPERATOR,
    
    K_OUT,            // 0xA0
    K_OPER,
    K_CLR_AGIN,
    K_CRSEL,
    K_EXCEL,
    
/* These are NOT standard USB HID - handled specially in decoding, 
     so they will be mapped to the modifier byte in the USB report */
	K_Modifiers,
	K_LCTRL,          // 0x01
	K_LSHIFT,         // 0x02
	K_LALT,           // 0x04
	K_LGUI,           // 0x08
	K_RCTRL,          // 0x10
	K_RSHIFT,         // 0x20
	K_RALT,           // 0x40
	K_RGUI,           // 0x80
	K_Modifiers_end,

   K_NEXT_TRK,        
   K_PREV_TRK,        // 0xB0
   K_STOP,
   K_PLAY,
   K_MUTE,
   K_BASS_BST,
   K_LOUDNESS,
   K_VOL_UP,
   K_VOL_DOWN,
   K_BASS_UP,
   K_BASS_DN,
   K_TRE_UP,
   K_TRE_DN,

   K_MEDIA_SEL,      // 0xc0
   K_MAIL,
   K_CALC,
   K_MYCOM,
   K_WWW_SEARCH,
   K_WWW_HOME,
   K_WWW_BACK,
   K_WWW_FORWARD,
   K_WWW_STOP,
   K_WWW_REFRESH,
   K_WWW_FAVORITE,
   K_EJECT,
   K_SCREENSAVE,
   K_REC,
	K_REWIND,
	K_MINIMIZE,

   K_M01,            // 0xd0 //K_F13,
   K_M02,            //K_F14,
   K_M03,            //K_F15,
   K_M04,            //K_F16,
   K_M05,            //K_F17,
   K_M06,            //K_F18,
   K_M07,            //K_F19,
   K_M08,            //K_F20,
   K_M09,            //K_F21,        // 0x70
   K_M10,            //K_F22,
   K_M11,            //K_F23,
   K_M12,            //K_F24,
   K_M13,            //K_INTL1,
   K_M14,            //K_INTL2,
   K_M15,            //K_INTL3,
   K_M16,            //K_INTL4,

   K_M17,            // 0xe0 //K_INTL5,
   K_M18,            //K_INTL6,
   K_M19,            //K_INTL7,
   K_M20,        
   K_M21,        
   K_M22,        
   K_M23,       
   K_M24,   
   K_M25,            //213
   K_M26,            //214
   K_M27,            //215
   K_M28,            //216
   K_M29,            //217
   K_M30,            //218
   K_M31,            //219
   K_M32,            //220

   K_M33,            // 0xf0//221
   K_M34,            //222
   K_M35,            //223
   K_M36,            //224
   K_M37,            //225
   K_M38,            //226
   K_M39,            //227
   K_M40,            //228
   K_M41,            //229
   K_M42,            //230
   K_M43,            //231
   K_M44,            //232
   K_M45,            //233
   K_M46,            //234
   K_M47,            //235
   K_M48,            //236
   K_M49,
   K_M50,
   K_M51,
   K_M52
};
   

