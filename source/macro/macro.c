#define KEYBD_EXTERN

#include "global.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

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
#include "macro.h"
#include "led.h"
#include "eepaddress.h"

extern int8_t usbmode;
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
            //key.mode = MOD_SHIFT_LEFT;
            key.key = KEY_ENTER; break;
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
void sendKey(Key keytosend) 
{
    uint8_t keyval = 0;
    if(usbmode)
    {
        usbSendReport(keytosend.mode, keytosend.key);
        usbSendReport(0, 0);
    }else
    {
        putKey(keytosend.key,1);
        while((keyval = pop()) !=SPLIT)
        {
            while(!(kbd_flags & FLA_TX_OK));
            _delay_us(10);
            tx_state(keyval, STA_NORMAL);
        }
        putKey(keytosend.key,0);
        while((keyval = pop()) !=SPLIT)
        {
            while(!(kbd_flags & FLA_TX_OK));
            _delay_us(10);
            tx_state(keyval, STA_NORMAL);
        }
    }
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
}
uint8_t macrobuffer[256] = {};
uint8_t macrostart[] = "MACRO record mode@";
uint8_t macroend[] = "@record done@";
uint8_t macroresetstart[] = "MACRO erase";
uint8_t macroresetdone[] = "done@";

long MacroAddr[MAX_MACRO_INDEX] = {};

uint8_t initMacroAddr(void)
{
    uint8_t i;
    long address;
    address = MACRO_ADDR_START;
    
    for (i = 0; i < MAX_MACRO_INDEX; i++)
    {
        MacroAddr[i] = address;
        address += 0x400;           // 1024
    }
}

uint8_t getkey(uint8_t key, uint16_t index)
{

}
void playMacroUSB(uint8_t macrokey)
{
    uint8_t i;
    uint8_t keyidx;
    Key key;
    key.mode = 0;
    key.key = 0;
    uint8_t mIndex = 0;
    long address;
    uint8_t esctoggle =0;

    mIndex = macrokey - KEY_M01;
    address = MacroAddr[mIndex];
    
    if (eeprom_read_byte(EEPADDR_MACRO_SET+mIndex) == 0)  // not recorded
    {
        return;
    }

    
    keyidx = pgm_read_byte_far(address++);
    for (i = 0; i < MAX_MACRO_LEN; i++)
    {
        if((KEY_Modifiers < keyidx) && (keyidx < KEY_Modifiers_end))
        {
            key.mode ^= modifierBitmap[keyidx -KEY_Modifiers];
            keyidx = pgm_read_byte_far(address++);
        }
        while(((keyidx < KEY_Modifiers) || (KEY_Modifiers_end < keyidx)) && keyidx != KEY_NONE )
        {
            if(esctoggle++ == 4)
            {
                led_on(LED_PIN_ESC);
                esctoggle = 0;
            }else{
                led_off(LED_PIN_ESC);
            }
        
            wdt_reset();
            key.key = keyidx;
            sendKey(key);
            keyidx = pgm_read_byte_far(address++);
        }
        if(keyidx == KEY_NONE)
            break;

    }
    
    key.mode = 0;
    key.key = 0;
    sendKey(key);

    led_mode_init();
}



void playMacroPS2(uint8_t macrokey)
{
    uint8_t i;
    uint8_t keyidx;
    Key key;
    uint8_t keyval;
    key.mode = 0;
    key.key = 0;
    long address;
    uint8_t esctoggle =0;

    address = MacroAddr[macrokey - KEY_M01];


    for (i = 0; i < MAX_MACRO_LEN; i++)
    {
        if(esctoggle++ == 4)
        {
            led_on(LED_PIN_ESC);
            esctoggle = 0;
        }else{
            led_off(LED_PIN_ESC);
        }
        
        keyidx = pgm_read_byte_far(address++);
        if(keyidx == KEY_NONE)
            return;
        if((KEY_Modifiers < keyidx) && (keyidx < KEY_Modifiers_end))
        {
            key.mode ^= modifierBitmap[keyidx -KEY_Modifiers];
            if(key.mode & modifierBitmap[keyidx -KEY_Modifiers])
            {
                putKey(keyidx,1);
            }else
            {
                putKey(keyidx,0);
            }
            while((keyval = pop()) !=SPLIT)
            {
                while(!(kbd_flags & FLA_TX_OK));
                _delay_us(10);
                tx_state(keyval, STA_NORMAL);
            }
        }else
        {
            putKey(keyidx,1);
            while((keyval = pop()) !=SPLIT)
            {
                while(!(kbd_flags & FLA_TX_OK));
                _delay_us(10);
                tx_state(keyval, STA_NORMAL);
            }
        
            putKey(keyidx,0);
            while((keyval = pop()) !=SPLIT)
            {
                while(!(kbd_flags & FLA_TX_OK));
                _delay_us(10);
                tx_state(keyval, STA_NORMAL);
            }
        }
       
    }
}

#if 1

typedef union ADDRESS_U{
    long  l;
    unsigned int    s[sizeof(long)/2];
    uchar   c[sizeof(long)];
}ADDRESS;

void writepage(uint8_t *data, unsigned long addr)
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

void resetMacro(void)
{
    uint8_t mIndex;
    long address;
    
    sendString(macroresetstart);
    for (mIndex = 0; mIndex < MAX_MACRO_INDEX; mIndex++)
    {
      wdt_reset();
      eeprom_write_byte(EEPADDR_MACRO_SET+mIndex, 0);
      sendString("-");
    }
    
    eeprom_write_byte(EEPADDR_SWAPCTRLCAPS, 0x80);
    eeprom_write_byte(EEPADDR_SWAPALTGUI, 0x80);

    sendString(macroresetdone);
}


void recordMacro(uint8_t macrokey)
{
   int8_t col, row;
   uint32_t prev, cur;
   uint8_t prevBit, curBit;
   uint8_t keyidx;
   uint8_t matrixState = 0;
   uint8_t retVal = 0;
   int16_t i;
   int16_t index;
   uint8_t mIndex;
   long page;
   uint8_t t_layer;
   Key key;
   long address;
   mIndex = macrokey - KEY_M01;
   address = MacroAddr[mIndex];
    
   index = 0;
   page = 0;

   key.mode = 0;
      
   wdt_reset();

   
   cntKey(KEY_FN, 0x0000);

   for (i = 0; i <= 255; i++)
      macrobuffer[i] = 0x00;

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
      
      t_layer = layer;

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
                     writepage(macrobuffer, address+(page*256));
                     
                     wdt_reset();
                     eeprom_write_byte(EEPADDR_MACRO_SET+mIndex, 1);

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
                     
                     if(index == 0xFF)
                     {
                         writepage(macrobuffer, address+(page*256));
                         page++;
                         index = 0;
                     }else
                     {
                        index++;
                     }
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
