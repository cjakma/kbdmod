#define KEYBD_EXTERN

#include "global.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>     /* for _delay_ms() */

#include <stdio.h>
#include "keysta.h"
#include "keymap.h"
#include "usbdrv.h"
#include "usbmain.h"
#include "ps2main.h"

#define MAX_MACROLEN    128




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

void playMacroUSB(uint8_t *buff)
{
    uint8_t i;
    Key key;
    key.mode = 0;
    key.key = 0;

    for (i = 0; i < MAX_MACROLEN; i++)
    {
        if(*buff > KEY_Modifiers && *buff < KEY_Modifiers_end)
            key.mode ^= modifierBitmap[(*buff++) -KEY_Modifiers];

        while(((*buff < KEY_Modifiers) || (KEY_Modifiers_end < *buff)) && *buff != KEY_NONE )
        {
            key.key = *buff++;
            sendKey(key);
        }
        if(*buff == KEY_NONE)
            return;

    }
    
    key.mode = 0;
    key.key = 0;
    sendKey(key);
}

void playMacroPS2(uint8_t *buff)
{
    uint8_t i;
    Key key;
    uint8_t keyval;
    key.mode = 0;
    key.key = 0;

    for (i = 0; i < MAX_MACROLEN; i++)
    {
        if(KEY_Modifiers < *buff && *buff < KEY_Modifiers_end)
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


