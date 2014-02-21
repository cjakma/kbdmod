#define KEYBD_EXTERN

#include "global.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>     /* for _delay_ms() */

#include <stdio.h>
#include <avr/wdt.h>

#include "keysta.h"
#include "keymap.h"
#include "usbdrv.h"
#include "usbmain.h"
#include "ps2main.h"
#include "keymap.h"
#include "matrix.h"
#define MAX_MACROLEN    10




/**
 * Send a single report to the computer. This function is not used during
 * normal typing, it is only used to send non-pressed keys to simulate input.
 * \param mode modifier-byte
 * \param key key-code
 */
void usbSendReport(uint8_t mode, uint8_t key) {
    // buffer for HID reports. we use a private one, so nobody gets disturbed
    uint8_t repBuffer[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    repBuffer[0] = mode;
    repBuffer[2] = key;
    while (!usbInterruptIsReady()); // wait
    usbSetInterrupt(repBuffer, sizeof(repBuffer)); // send
}

/**
 * This structure can be used as a container for a single 'key'. It consists of
 * the key-code and the modifier-code.
 */
typedef struct {
    uint8_t mode;
    uint8_t key;
} Key;


extern MODIFIERS modifierBitmap[];
/**
 * Convert an ASCII-character to the corresponding key-code and modifier-code
 * combination.
 * \parm character ASCII-character to convert
 * \return structure containing the combination
 */
Key charToKey(char character) {
    Key key;
    // initialize with reserved values
    key.mode = MOD_NONE;
    key.key = KEY_NONE;
    if ((character >= 'a') && (character <= 'z')) {
        // a..z
        key.key = (character - 'a') + 0x04;
    } else if ((character >= 'A') && (character <= 'Z')) {
        // A..Z
        key.mode = MOD_SHIFT_LEFT;
        key.key = (character - 'A') + 0x04;
    } else if ((character >= '1') && (character <= '9')) {
        // 1..9
        key.key = (character - '1') + 0x1E;
    }
    // we can't map the other characters directly, so we switch...
    switch (character) {
        case '0':
            key.key = KEY_0; break;
        case '!':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_1; break;
        /*
        case '@':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_2; break;
        case '#':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_3; break;
        */
        case '$':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_4; break;
        case '%':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_5; break;
        case '^':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_6; break;
        case '&':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_7; break;
        case '*':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_8; break;
        case '(':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_9; break;
        case ')':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_0; break;
        case ' ':
            key.key = KEY_SPACE; break;
        case '-':
            key.key = KEY_MINUS; break;
        case '_':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_MINUS; break;
        case '=':
            key.key = KEY_EQUAL; break;
        case '+':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_EQUAL; break;
        case '[':
            key.key = KEY_LBR; break;
        case '{':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_LBR; break;
        case ']':
            key.key = KEY_RBR; break;
        case '}':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_RBR; break;
        case '\\':
            key.key = KEY_BKSLASH; break;
        case '|':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_BKSLASH; break;
        case '#':
            key.key = KEY_HASH; break;
        case '@':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_HASH; break;
        case ';':
            key.key = KEY_COLON; break;
        case ':':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_COLON; break;
#if 0
        case '\'':
            key.key = KEY_apostroph; break;
        case '"':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_apostroph; break;
#endif            
        case '`':
            key.key = KEY_HASH; break;
        case '~':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_HASH; break;
        case ',':
            key.key = KEY_COMMA; break;
        case '<':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_COMMA; break;
        case '.':
            key.key = KEY_DOT; break;
        case '>':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_DOT; break;
        case '/':
            key.key = KEY_SLASH; break;
        case '?':
            key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_SLASH; break;
    }
    if (key.key == KEY_NONE) {
        // still reserved? WTF? return question mark...
        key.mode = MOD_SHIFT_LEFT;
        key.key = KEY_SLASH;
    }
    return key;
} 


/**
 * Send a key to the computer, followed by the release of all keys. This can be
 * used repetitively to send a string.
 * \param keytosend key structure to send
 */
void sendKey(Key keytosend) {
    usbSendReport(keytosend.mode, keytosend.key);
    usbSendReport(0, 0);
}

/**
 * Send a string to the computer. This function converts each character of an
 * ASCII-string to a key-structure and uses sendKey() to send it.
 * \param string string to send
 */
void sendString(char* string) {
    uint8_t i;
    Key key;

    for (i = 0; i < strlen(string); i++) {
        key = charToKey(string[i]);
        sendKey(key);
    }
    key.key = KEY_ENTER;
    sendKey(key);
}
uint8_t macrobuffer[256] = {KEY_LSHIFT, KEY_H, KEY_LSHIFT, KEY_I, KEY_G, KEY_H, KEY_LSHIFT, KEY_1, KEY_LSHIFT, KEY_NONE};
uint8_t macrostart[] = "recording start";
uint8_t macroend[] = "recording end";
#if 0
void playMacroUSB(uint8_t *buff)
{
    uint8_t i;
    Key key;
    key.mode = 0;
    key.key = 0;
    
    for (i = 0; i < MAX_MACROLEN; i++)
    {
        if((KEY_Modifiers < *buff) && (*buff < KEY_Modifiers_end))
            key.mode ^= modifierBitmap[(*buff++) -KEY_Modifiers];

        while(((*buff < KEY_Modifiers) || (KEY_Modifiers_end < *buff)) && *buff != KEY_NONE )
        {
            key.key = *buff++;
            sendKey(key);
        }
        if(*buff == KEY_NONE)
            break;

    }
    
    key.mode = 0;
    key.key = 0;
    sendKey(key);
}
#else
void playMacroUSB(uint8_t *buff)
{
    uint8_t i;
    Key key;
    key.mode = 0;
    key.key = 0;
    uint8_t index = 0;

    for (i = 0; i < MAX_MACROLEN; i++)
    {
        if((KEY_Modifiers < pgm_read_byte_far((long)0x4800+(long)index)) && ((pgm_read_byte_far((long)0x4800+(long)index) < KEY_Modifiers_end)))
        {
            key.mode ^= modifierBitmap[(pgm_read_byte_far((long)0x4800+(long)index)) -KEY_Modifiers];
            index++;
        }
        while(((pgm_read_byte_far((long)0x4800+(long)index)< KEY_Modifiers) || (KEY_Modifiers_end < pgm_read_byte_far((long)0x4800+(long)index))) && pgm_read_byte_far((long)0x4800+(long)index) != KEY_NONE )
        {
            key.key = pgm_read_byte_far((long)0x4800+(long)index);
            sendKey(key);
            index++;
        }
        if(pgm_read_byte_far((long)0x4800+(long)index) == KEY_NONE)
            break;

    }
    
    key.mode = 0;
    key.key = 0;
    sendKey(key);
}
#endif



void playMacroPS2(uint8_t *buff)
{
    uint8_t i;
    Key key;
    uint8_t keyval;
    key.mode = 0;
    key.key = 0;

    for (i = 0; i < MAX_MACROLEN; i++)
    {
        if((KEY_Modifiers < *buff) && (*buff < KEY_Modifiers_end))
        {
            key.mode ^= modifierBitmap[(*buff) -KEY_Modifiers];
            if(key.mode & modifierBitmap[(*buff) -KEY_Modifiers])
            {
                putKey(*buff++,1);
            }else
            {
                putKey(*buff++,0);
            }
            while((keyval = pop()) !=SPLIT)
            {
                while(!(kbd_flags & FLA_TX_OK));
                _delay_us(10);
                tx_state(keyval, STA_NORMAL);
            }
        }else
        {
            putKey(*buff,1);
            while((keyval = pop()) !=SPLIT)
            {
                while(!(kbd_flags & FLA_TX_OK));
                _delay_us(10);
                tx_state(keyval, STA_NORMAL);
            }
        
            putKey(*buff++,0);
            while((keyval = pop()) !=SPLIT)
            {
                while(!(kbd_flags & FLA_TX_OK));
                _delay_us(10);
                tx_state(keyval, STA_NORMAL);
            }
        }

        if(*buff == KEY_NONE)
            return;
    }
}

#if 1

typedef union ADDRESS_U{
    long  l;
    unsigned int    s[sizeof(long)/2];
    uchar   c[sizeof(long)];
}ADDRESS;


void writepage(uchar *data, unsigned long addr) 
    __attribute__ ((section (".appinboot")));

void writepage(uchar *data, unsigned long addr)
{
    uchar   isLast;
    uchar len;
#if 1
    ADDRESS address;

    do{
        long prevAddr;
#if SPM_PAGESIZE > 256
        uint pageAddr;
#else
        uchar pageAddr;
#endif
        pageAddr = address.s[0] & (SPM_PAGESIZE - 1);
        if(pageAddr == 0){              /* if page start: erase */
            cli();
//            boot_page_erase(address.l); /* erase page */
            sei();
//            boot_spm_busy_wait();       /* wait until page is erased */
        }
        cli();
//        boot_page_fill(address.l, *(short *)data);
        sei();
        prevAddr = address.l;
        address.l += 2;
        data += 2;
        /* write page when we cross page boundary */
        pageAddr = address.s[0] & (SPM_PAGESIZE - 1);
        if(pageAddr == 0){
            cli();
//            boot_page_write(prevAddr);
            sei();
//            boot_spm_busy_wait();
        }
        len -= 2;
    }while(len);
#endif
    return;
}



void recordMacro(void)
{
   int8_t col, row;
   uint32_t prev, cur;
   uint8_t prevBit, curBit;
   uint8_t keyidx;
   uint8_t matrixState = 0;
   uint8_t retVal = 0;
   int8_t i;
   int8_t index;
   uint8_t t_layer;
   Key key;
   index = 0;

   key.mode = 0;
      
   wdt_reset();
   for (i = 0; i < 10; i++)
      macrobuffer[i] = 0x00;

   for (i = 0; i < 5; i++)
   {
      macrostart[i] = pgm_read_byte_far((long)0x11000+(long)i);
   }

   sendString(macrostart);


   for(col = 0; col < MAX_COL; col++)
   {
      for(row = 0; row < MAX_ROW; row++)
      {
         debounceMATRIX[col][row] = -1;
      }
   }

   while(1)
   {
      
      wdt_reset();
      matrixState = scanmatrix();
      
      t_layer = getLayer(matrixFN[layer]);

      // debounce cleared => compare last matrix and current matrix
      for(col = 0; col < MAX_COL; col++)
      {
         prev = MATRIX[col];
         cur  = curMATRIX[col];
         MATRIX[col] = curMATRIX[col];
         for(i = 0; i < MAX_ROW; i++)
         {
            prevBit = (uint8_t)prev & 0x01;
            curBit = (uint8_t)cur & 0x01;
            prev >>= 1;
            cur >>= 1;

            if (i < 8)
            {
               row = 10 + i;
            }else if (i < 16)
            {
               row = -6 + i;
            }else
            {
               row = -16 + i;
            }
            keyidx = pgm_read_byte(keymap[t_layer]+(col*MAX_ROW)+row);

         if (keyidx == KEY_NONE)
            continue;

#if 0
         if (!prevBit && curBit)   //pushed
         {
            if (processFNkeys(keyidx))
            continue;
         }
#endif

         if (!prevBit && curBit)   //pushed
         {
            debounceMATRIX[col][row] = 0;    //triger

         }else if (prevBit && !curBit)  //released
         {
            debounceMATRIX[col][row] = 0;    //triger
         }

         if(debounceMATRIX[col][row] >= 0)
         {                
            if(debounceMATRIX[col][row]++ >= DEBOUNCE_MAX)
            {
               if(curBit)
               {
                  if (keyidx == KEY_FN)
                  {
                     macrobuffer[index] = KEY_NONE;
                     writepage(macrobuffer, (long)0x4800);
                     sendString(macroend);
                     return;
                  }
                  else
                  {
                     macrobuffer[index] = keyidx;
                     if ((KEY_Modifiers < keyidx)  && (keyidx < KEY_Modifiers_end))
                     {
                        key.key = KEY_SLASH;
                     }else
                     {
                        key.key = macrobuffer[index];
                     }
                     sendKey(key);
                     index++;
                  }
               }else
               {
                  if ((KEY_Modifiers < keyidx)  && (keyidx < KEY_Modifiers_end))
                  {
                     macrobuffer[index++] = keyidx;
                     key.key = KEY_SLASH;
                     sendKey(key);
                  }
               }

               debounceMATRIX[col][row] = -1;
            }
         }
      }
      }
   }
}
#endif
