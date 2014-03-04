#include <stdio.h>
#include "keymap.h"

#define MAX_COL     6
#define MAX_ROW     18

#define MAX_LAYER   8
#define MAXHEXLINE 32	/* the maximum number of bytes to put in one line */


// Total 132 keys + one none


unsigned char keymap_code[MAX_LAYER][MAX_COL][MAX_ROW] =  {
	{
		// LAYER 0 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_ESC,   KEY_NONE,   KEY_F1, KEY_F2, KEY_F3,	KEY_F4,	KEY_F5, KEY_F6, KEY_F7,     KEY_F8, KEY_F9,     KEY_F10,    KEY_F11,    KEY_F12,    KEY_FN,     KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_INSERT, KEY_HOME,   KEY_PGUP},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS, KEY_RCTRL,  KEY_NONE,   KEY_NONE,   KEY_LEFT,   KEY_DOWN,   KEY_RIGHT}  // COL5
	},
	{	
		// LAYER 1 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_ESC,   KEY_NONE,   KEY_F1, KEY_F2, KEY_F3,	KEY_F4,	KEY_F5, KEY_F6, KEY_F7,     KEY_F8, KEY_F9,     KEY_F10,    KEY_F11,    KEY_F12,    KEY_FN,     KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_INSERT, KEY_HOME,   KEY_PGUP},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT}  // COL5
	},
	{	
         // LAYER 2 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_2,     KEY_F1,     KEY_F2, KEY_F3,	KEY_F4,	KEY_F5, KEY_F6, KEY_F7, KEY_F8,     KEY_F9, KEY_F10,    KEY_F11,    KEY_F12,    KEY_NONE,   KEY_FN,     KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE}, // COL0
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
            KEY_WAKE,KEY_WAKE,KEY_VOL_UP,KEY_BASS_DN,},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT}  // COL5

	},
	{	
         // LAYER 5 : normal
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_5,     KEY_F1,     KEY_F2, KEY_F3,	KEY_F4,	KEY_F5, KEY_F6, KEY_F7, KEY_F8,     KEY_F9, KEY_F10,    KEY_F11,    KEY_F12,    KEY_NONE,   KEY_FN,   KEY_PRNSCR, KEY_SCRLCK, KEY_PAUSE}, // COL0
		{KEY_HASH,  KEY_1,      KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  KEY_7,  KEY_8,      KEY_9,  KEY_0,      KEY_MINUS,  KEY_EQUAL,  KEY_BKSP,   KEY_NONE,   KEY_INSERT, KEY_HOME,   KEY_PGUP},  // COL1
		{KEY_TAB,   KEY_Q,      KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I,      KEY_O,  KEY_P,      KEY_LBR,    KEY_RBR,    KEY_BKSLASH,KEY_NONE,   KEY_DEL,    KEY_END,    KEY_PGDN},  // COL2
		{KEY_CAPS,  KEY_A,      KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,      KEY_L,  KEY_COLON,  KEY_QUOTE,  KEY_ENTER,  KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_NONE},  // COL3
		{KEY_LSHIFT,KEY_Z,      KEY_X,  KEY_C,  KEY_V,  KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,KEY_SLASH,  KEY_NONE,   KEY_RSHIFT, KEY_NONE,   KEY_NONE,   KEY_NONE,   KEY_UP,     KEY_NONE},  // COL4
		{KEY_LCTRL, KEY_LGUI,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_RGUI,KEY_APPS,KEY_RCTRL,KEY_NONE,KEY_NONE,         KEY_LEFT,   KEY_DOWN,   KEY_RIGHT}  // COL5

	},
	{
         // LAYER 6: normal
        // ROW0        ROW1            ROW2     ROW3     ROW4   ROW5    ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
        {KEY_LFX,   KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_LFX,    KEY_FN,     KEY_LFX,    KEY_LFX,    KEY_LFX}, // COL0
        {KEY_LFULL, KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LPAD,   KEY_LPAD,   KEY_LPAD},  // COL1
        {KEY_LFULL, KEY_LFULL,  KEY_LASD,   KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_NONE,   KEY_LPAD,   KEY_LPAD,   KEY_LPAD},  // COL2
        {KEY_LFULL, KEY_LASD,   KEY_LASD,   KEY_LASD,   KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_NONE,   KEY_NONE,   KEY_LPAD,   KEY_LPAD,   KEY_LPAD},  // COL3
        {KEY_LFULL, KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LARR,   KEY_NONE,   KEY_LPAD,   KEY_LARR,   KEY_LPAD},  // COL4
        {KEY_LFULL, KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LFULL,  KEY_LARR,   KEY_LARR,   KEY_LARR,   KEY_LARR,   KEY_LARR}  // COL5

     },
        

	{	
		// LAYER  FN
		// ROW0        ROW1            ROW2     ROW3     ROW4 	ROW5	ROW6     ROW7     ROW8             ROW9     ROW10          ROW11           ROW12          ROW13           ROW14          ROW15           ROW16          ROW17
		{KEY_L0,    KEY_M02,    KEY_M03,  KEY_M04,  KEY_M05,  KEY_M06,  KEY_M07,  KEY_M08,  KEY_M09, KEY_M10,  KEY_M11, KEY_M12,   KEY_M13,    KEY_DEL,     KEY_FN,     KEY_MRESET, KEY_KEYLOCK,    KEY_RESET}, // COL0
		{KEY_M01,   KEY_L1,     KEY_L2,   KEY_L3,   KEY_L4,	KEY_LED0, KEY_LED1, KEY_LED2, KEY_LED3,KEY_9,    KEY_0,   KEY_M48,   KEY_M49,    KEY_BKSP,    KEY_NONE,   KEY_KP_AST, KEY_KP_SLASH,   KEY_KP_PLUS},  // COL1
		{KEY_TAB,   KEY_M14,    KEY_M15,  KEY_M16,  KEY_M17,  KEY_M18,  KEY_M19,  KEY_M20,  KEY_M21, KEY_M22,  KEY_M23, KEY_M24,   KEY_M25,    KEY_M26,     KEY_NONE,   KEY_KP_7,   KEY_KP_8,       KEY_KP_9},  // COL2
		{KEY_CAPS,  KEY_M27,    KEY_M28,  KEY_M29,  KEY_M30,  KEY_M31,  KEY_M32,  KEY_M33,  KEY_M34, KEY_M35,  KEY_M36, KEY_M37,   KEY_ENTER,  KEY_NONE,    KEY_NONE,   KEY_KP_4,   KEY_KP_5,       KEY_KP_6},  // COL3
		{KEY_LSHIFT,KEY_M38,    KEY_M39,  KEY_M40,  KEY_M41,  KEY_M42,  KEY_M43,  KEY_M44,  KEY_M45, KEY_M46,  KEY_M47, KEY_NONE,  KEY_RSHIFT, KEY_NONE,    KEY_NONE,   KEY_KP_1,   KEY_KP_2,       KEY_KP_3},  // COL4
		{KEY_LCTRL, KEY_WINKEYLOCK,   KEY_LALT,KEY_NONE,KEY_NONE,KEY_NONE,KEY_SPACE,KEY_NONE,KEY_NONE,KEY_RALT,KEY_WINKEYLOCK,KEY_APPS,KEY_RCTRL,   KEY_LEFT,   KEY_DOWN,   KEY_RIGHT,  KEY_KP_0,   KEY_KP_DOT}  // COL5

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

