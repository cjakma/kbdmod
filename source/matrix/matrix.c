#define KEYBD_EXTERN
#include "global.h"
#include "timer128.h"
#include "keymap.h"
#include "print.h"
#include "led.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <util/delay.h>     /* for _delay_ms() */

#include "hwport.h"
#include "usbdrv.h"
#include "matrix.h"
#include "usbmain.h"
#include "eepaddress.h"
#include "macro.h"

uint32_t scankeycntms = 0;
	
// 17*8 bit matrix
uint32_t MATRIX[MAX_COL];
uint32_t curMATRIX[MAX_COL];
int8_t debounceMATRIX[MAX_COL][MAX_ROW];
uint8_t svkeyidx[MAX_COL][MAX_ROW];

uint8_t matrixFN[MAX_LAYER];           // (col << 4 | row)
uint8_t layer = 0;
uint8_t kbdsleepmode = 0;
uint8_t ledPortBackup = 0;
uint16_t macrokeypushedcnt;
uint16_t ledkeypushedcnt;
uint16_t macroresetcnt;
uint16_t winkeylockcnt;
uint16_t keylockcnt;
uint8_t keylock = 0;
#define SWAP_TIMER  0x400
#define KEYLOCK_TIMER  0x600
#define KEYLOCK_COUNTER_START 0x8000

uint8_t swapCtrlCaps = 0x80;
uint8_t swapAltGui =  0x80;
uint16_t cntLcaps = 0;
uint16_t cntLctrl = 0;
uint16_t cntLAlt = 0;
uint16_t cntLGui = 0;

uint8_t isLED3000 = 0;

extern int8_t usbmode;



static uint8_t findFNkey(void)
{
    uint8_t col, row;
    uint8_t keyidx;
    uint8_t i;
    for(i = 0; i < MAX_LAYER; i++)
    {
        matrixFN[i] = 0x00;
    	for(col=0;col<MAX_COL;col++)
    	{
    		for(row=0;row<MAX_ROW;row++)
            {
               keyidx = pgm_read_byte(keymap[i]+((uint32_t)col*MAX_ROW)+(uint32_t)row);
    			if(keyidx == KEY_FN)
    			{
                    matrixFN[i] = col << 5 | row;
    			}
    		}
        }
        if (matrixFN[i] == 0x00)
        {
            matrixFN[i] = matrixFN[0];  // default FN position
        }
        
    }
    return 0;
}


void keymap_init(void) 
{
	int i, j, keyidx;

	// set zero for every flags
	for(i=0;i<MAX_KEY;i++)
		KFLA[i]=0;
	
	// set flags
	for(i=0;(keyidx=pgm_read_byte(&keycode_set2_special[i]))!=KEY_NONE;i++)
		KFLA[keyidx] |= KFLA_SPECIAL;
	for(i=0;(keyidx=pgm_read_byte(&keycode_set2_makeonly[i]))!=KEY_NONE;i++)
		KFLA[keyidx] |= KFLA_MAKEONLY;
	for(i=0;(keyidx=pgm_read_byte(&keycode_set2_make_break[i]))!=KEY_NONE;i++)
		KFLA[keyidx] |= KFLA_MAKE_BREAK;
	for(i=0;(keyidx=pgm_read_byte(&keycode_set2_extend[i]))!=KEY_NONE;i++)
		KFLA[keyidx] |= KFLA_EXTEND;
	for(i=0;(keyidx=pgm_read_byte(&keycode_set2_proc_shift[i]))!=KEY_NONE;i++)
		KFLA[keyidx] |= KFLA_PROC_SHIFT;


	for(i=0;i<MAX_ROW;i++)
		MATRIX[i]=0;
    
    findFNkey();

	for(i=0;i<MAX_COL;i++)
	{
        for(j=0;j<MAX_ROW;j++)
        {
            debounceMATRIX[i][j] = -1;
        }
        curMATRIX[i] = 0;
	}

    layer = eeprom_read_byte(EEPADDR_KEYLAYER);
    if (layer >= MAX_LAYER)
        layer = 0;
    keyidx = pgm_read_byte(keymap[layer]+((uint32_t)5*MAX_ROW)+(uint32_t)15);
        isLED3000 = (keyidx == KEY_LEFT)? 1 : 0;
           
   swap_load();
}


uint8_t macrokeystring[3][30] = {
                                {KEY_H, KEY_E, KEY_L, KEY_L, KEY_O, KEY_H, KEY_E, KEY_L, KEY_L, KEY_O,KEY_H, KEY_E, KEY_L, KEY_L, KEY_O,KEY_H, KEY_E, KEY_L, KEY_L, KEY_O,KEY_H, KEY_E, KEY_L, KEY_L, KEY_O, KEY_NONE},
                                {KEY_RCTRL, KEY_V, KEY_RCTRL, KEY_C, KEY_P, KEY_NONE},
                                {KEY_LSHIFT, KEY_H, KEY_LSHIFT, KEY_I, KEY_G, KEY_H, KEY_LSHIFT, KEY_1, KEY_LSHIFT, KEY_NONE}
                             };


uint8_t processPushedFNkeys(uint8_t keyidx)
{
    uint8_t retVal = 0;
    uint8_t ledblock;
    uint8_t f1position;
    uint8_t key;
    
    if(keyidx >= KEY_LED0 && keyidx <= KEY_LED3)
    {
        retVal = 1;
    }else if(keyidx >= KEY_LFX && keyidx <= KEY_LARR)
    {
        retVal = 1;
    }else if(keyidx >= KEY_L0 && keyidx <= KEY_L6)
    {
        layer = keyidx - KEY_L0;
        
        key = pgm_read_byte(keymap[layer]+((uint32_t)5*MAX_ROW)+(uint32_t)15);
        isLED3000 = (key == KEY_LEFT)? 1 : 0;
        
        eeprom_write_byte(EEPADDR_KEYLAYER, layer);
        led_mode_init();
        retVal = 1;
    }else if(keyidx >= KEY_M01 && keyidx <= KEY_M48)
    {
        retVal = 1;
    }else if(keyidx == KEY_RESET)
    {
        extern int portInit(void);
        extern AppPtr_t Bootloader;
        portInit();
        Reset_AVR();
    }else if(keyidx == KEY_MRESET)
    {
        retVal = 1;
    }else if(keyidx == KEY_KEYLOCK)
    {
         keylock ^= 0x02;
         retVal = 1;
    }else if(keyidx == KEY_WINKEYLOCK)
    {
         keylock ^= 0x01;
         retVal = 1;
    }
    return retVal;
}

uint8_t processReleasedFNkeys(uint8_t keyidx)
{
    uint8_t retVal = 0;
    uint8_t ledblock;
    uint8_t f1position;
    uint8_t key;
        
    if(keyidx >= KEY_LED0 && keyidx <= KEY_LED3)
    {
        ledmodeIndex = keyidx-KEY_LED0;
        for (ledblock = LED_PIN_ESC; ledblock < LED_PIN_ARROW30; ledblock++)
        {
            led_mode_change(ledblock, ledmode[ledmodeIndex][ledblock]);
        }
        led_mode_save();
        retVal = 1;
    }else if(keyidx >= KEY_LFX && keyidx <= KEY_LARR)
    {
        retVal = 1;
    }else if(keyidx >= KEY_M01 && keyidx <= KEY_M48)
    {
        if(usbmode)
             playMacroUSB(keyidx);
         else
             playMacroPS2(keyidx);
         retVal = 1;
    }else if(keyidx == KEY_RESET)
    {
        extern AppPtr_t Bootloader;
        Reset_AVR();
    }else if(keyidx == KEY_MRESET)
    {
        retVal = 1;
    }
    return retVal;
}



uint8_t getLayer(uint8_t FNcolrow)
{
    uint8_t col;
    uint8_t row;
    uint8_t cur;
    
    col = (FNcolrow >> 5) & 0x0f;
    row = FNcolrow & 0x1f;
	
	DDRA  = BV(col);
	PORTA = ~BV(col);
	
	_delay_us(10);

	if(row<2)	{				// for 0..7, PORTA 0 -> 7
		cur = ~PING & BV(row);
	}
	else if(row>=3 && row<10) {	// for 8..15, PORTC 7 -> 0
		cur = ~PINC & BV(row-2);
	}else if(row >= 10 && row<18){
	    cur = ~PINF & BV(row-10);
	}

    if(cur)
        return 7;        // FN layer or beyondFN layer
    else
        return layer;                   // Normal layer
}


uint8_t scanmatrix(void)
{
	uint8_t col, row;
	uint8_t prev, cur;
    uint8_t vPinG;
    uint8_t vPinC;
    uint8_t vPinF;

	uint8_t matrixState = 0;
    uint8_t ledblock;
    
    if (scankeycntms++ >= 136364)   // 5min
    {
        scankeycntms--;
        kbdsleepmode = 1;
        ledmodeIndex = 4;       // hidden OFF index

        for (ledblock = LED_PIN_PRT; ledblock < LED_PIN_ARROW30; ledblock++)
        {
            led_mode_change(ledblock, ledmode[ledmodeIndex][ledblock]);
        }
    }
    

	// scan matrix 
	for(col=0; col<MAX_COL; col++)
	{
      // Col -> set only one port as input and all others as output low
      DDRA  = BV(col);        //  only target col bit is output and others are input
      PORTA = ~BV(col);       //  only target col bit is LOW and other are pull-up

      _delay_us(10);

      vPinG = ~PING;
      vPinC = ~PINC;
      vPinF = ~PINF;

      curMATRIX[col] = (uint32_t)(vPinG & 0x03) << 16 | (uint32_t)vPinC << 8 | (uint32_t)vPinF;
      if(curMATRIX[col])
      {
         matrixState |= SCAN_DIRTY;
      }
 	}
    return matrixState;
}

void swap_save(void)
{
   eeprom_write_byte(EEPADDR_SWAPALTGUI, swapAltGui);
   eeprom_write_byte(EEPADDR_SWAPCTRLCAPS, swapCtrlCaps);
}

void swap_load(void)
{
   swapAltGui = eeprom_read_byte(EEPADDR_SWAPALTGUI);
   swapCtrlCaps = eeprom_read_byte(EEPADDR_SWAPCTRLCAPS);
}

uint8_t cntKey(uint8_t keyidx, uint8_t clearmask)
{
    switch (keyidx)
    {
        case KEY_CAPS:
            if (clearmask == 0)
            {
                swapCtrlCaps |= 0x80;
                cntLcaps = 0;
            }
            if (cntLcaps++ >= SWAP_TIMER)
                cntLcaps = SWAP_TIMER;
            break;
        case KEY_LCTRL:
            if (clearmask == 0)
            {
                swapCtrlCaps |= 0x80;
                cntLctrl = 0;
            }
            if (cntLctrl++ >= SWAP_TIMER)
                cntLctrl = SWAP_TIMER;
            break;
        case KEY_LALT:
            if (clearmask == 0)
            {
                swapAltGui |= 0x80;
                cntLAlt = 0;
            }
            if (cntLAlt++ >= SWAP_TIMER)
                cntLAlt = SWAP_TIMER;
            break;
        case KEY_LGUI:
            if (clearmask == 0)
            {
                swapAltGui |= 0x80;
                cntLGui = 0;
            }
            if (cntLGui++ >= SWAP_TIMER)
                cntLGui = SWAP_TIMER;
            break;
    }
    if((cntLcaps == SWAP_TIMER) && (cntLctrl == SWAP_TIMER) && (swapCtrlCaps & 0x80))
    {
        swapCtrlCaps ^= 1;
        swapCtrlCaps &= 0x7F;
        swap_save();
    }
    if((cntLGui == SWAP_TIMER) && (cntLAlt == SWAP_TIMER) && (swapAltGui & 0x80))
    {
        swapAltGui ^= 1;
        swapAltGui &= 0x7F;
        swap_save();
    }
    if(keyidx >= KEY_M01 && keyidx <= KEY_M48)
    {
         if(clearmask == 0x0000)
        {
            macrokeypushedcnt = 0;
        }
        if (macrokeypushedcnt++ == SWAP_TIMER)
        {
            recordMacro(keyidx);
            macrokeypushedcnt = 0;
        }
        
    }else if (keyidx >= KEY_LED0 && keyidx <= KEY_LED3)
    {
        if(clearmask == 0x0000)
        {
            ledkeypushedcnt = 0;
        }
        if (ledkeypushedcnt++ == SWAP_TIMER)
        {
            recordLED(keyidx);
            ledkeypushedcnt = 0;
        }
        
    }else if (keyidx == KEY_MRESET)
    {
         if(clearmask == 0x0000)
        {
            macroresetcnt = 0;
        }
        if(macroresetcnt++ == SWAP_TIMER)
        {
            resetMacro();
        }
    }
}


uint8_t swap_key(uint8_t keyidx)
{
    if(keylock & 0x02)
    {
      keyidx = KEY_NONE;
      return;
    }
    if(swapCtrlCaps & 0x01)
    {
        if(keyidx == KEY_CAPS)
        {
            keyidx = KEY_LCTRL;
        }
        else if(keyidx == KEY_LCTRL)
        {
            keyidx = KEY_CAPS;
        }
    }
    if(swapAltGui & 0x01)
    {
        if(keyidx == KEY_LALT)
        {
            keyidx = KEY_LGUI;
        }
        else if(keyidx == KEY_LGUI)
        {
            keyidx = KEY_LALT;
        }
    }
    if (keylock & 0x01)
    {
      if ((keyidx == KEY_LGUI) || (keyidx == KEY_RGUI))
         keyidx = KEY_NONE;
    }
    return keyidx;
}

// return : key modified
uint8_t scankey(void)
{
	int8_t col, row;
	uint32_t prev, cur;
    uint8_t prevBit, curBit;
	uint8_t keyidx;
	uint8_t matrixState = 0;
	uint8_t retVal = 0;
    uint8_t debounce = 0;
    int8_t i;


    matrixState = scanmatrix();

   if(!kbdsleepmode)
   {
      led_PRTIndicater(keylock);
      led_ESCIndicater(layer);
   }
   //static int pushedLevel_prev = 0;

    /* LED Blinker */
    led_blink(matrixState);

    /* LED Fader */
    led_fader();

    clearReportBuffer();
	uint8_t t_layer = getLayer(matrixFN[layer]);

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
            keyidx = pgm_read_byte(keymap[t_layer]+((uint32_t)col*MAX_ROW)+(uint32_t)row);

            if (keyidx == KEY_NONE)
                continue;

            if(curBit && !(keylock & 0x01))
               cntKey(keyidx, 0xFFFF);
            
            if (!prevBit && curBit)   //pushed
            {
                led_pushed_level_cal();          /* LED_EFFECT_PUSHED_LEVEL calculate */        
                if (processPushedFNkeys(keyidx))
                    continue;
            }else if (prevBit && !curBit)  //released
            {
                cntKey(keyidx, 0x0000);
                if (processReleasedFNkeys(keyidx))
                    continue;
            }

            keyidx = swap_key(keyidx);

            if ((KEY_L0 <= keyidx && keyidx <= KEY_L6) || (KEY_LED0 <= keyidx && keyidx <= KEY_FN) || (KEY_M01 <= keyidx) || (keyidx == KEY_NONE))
               continue;
            

            if(usbmode)
            {
                if(curBit)
                {
                    if(debounceMATRIX[col][row]++ >= DEBOUNCE_MAX)
                    {
                        retVal = buildHIDreports(keyidx);
                        debounceMATRIX[col][row] = DEBOUNCE_MAX*2;
                    }
                }else
                {
                    if(debounceMATRIX[col][row]-- >= DEBOUNCE_MAX)
                    {
                        retVal = buildHIDreports(keyidx);
                    }else
                    {
                        debounceMATRIX[col][row] = 0;
                    }
                }
            }else
            {
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
                            putKey(keyidx, 1);
                            svkeyidx[col][row] = keyidx;
                        }else
                        {
                            if (keyidx <= KEY_M01)  // ignore FN keys
                              putKey(svkeyidx[col][row], 0);
                        }
                                               
                        debounceMATRIX[col][row] = -1;
                   }
  
                }
 		    }
		}
	}
 
    retVal |= 0x05;
	return retVal;
}


