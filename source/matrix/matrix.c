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

int16_t scankeycntms = 0;
	
// 17*8 bit matrix
uint32_t MATRIX[MAX_COL];
uint32_t curMATRIX[MAX_COL];
int8_t debounceMATRIX[MAX_COL][MAX_ROW];
uint8_t svkeyidx[MAX_COL][MAX_ROW];

uint8_t matrixFN[MAX_LAYER];           // (col << 4 | row)
uint8_t layer = 0;
uint8_t kbdsleepmode = 0;
uint8_t ledPortBackup = 0;


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
                keyidx = pgm_read_byte(keymap[i]+(col*MAX_ROW)+row);
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

    for(i=0; i<MAX_LAYER; i++)
    {
        keymap[i] = &keymap_code[i][0][0];
    }

	// set zero for every flags
	for(i=0;i<NUM_KEY;i++)
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
	}

    layer = eeprom_read_byte(EEPADDR_KEYLAYER);
    if (layer >= MAX_LAYER)
        layer = 0;
}

uint8_t processFNkeys(uint8_t keyidx)
{
    uint8_t retVal = 0;
    uint8_t ledblock;
    switch(keyidx)
    {
        case KEY_LED0:
        case KEY_LED1:
        case KEY_LED2:
        case KEY_LED3:
        {
            ledmodeIndex = keyidx-KEY_LED0;
            for (ledblock = 0; ledblock < LED_BLOCK_ALL; ledblock++)
            {
                led_mode_change(ledblock, ledmode[ledmodeIndex][ledblock]);
            }
            retVal = 1;
            break;
        }
            
        case KEY_LED4:
        case KEY_LED5:
        case KEY_LED6:
        case KEY_LED7:
        {
            ledmodeIndex ++;
            if ((ledmodeIndex > 8) || (ledmodeIndex < 4))
                ledmodeIndex = 4;
            for (ledblock = 0; ledblock < LED_BLOCK_ALL; ledblock++)
            {
                    led_mode_change(ledblock, ledmode[ledmodeIndex][ledblock]);
            }
            retVal = 1;
            break;
        }
        case KEY_L0:
        case KEY_L1:
        case KEY_L2:
        case KEY_L3:            
        case KEY_L4:
        case KEY_L5:
        case KEY_L6:
            layer = keyidx - KEY_L0;
            eeprom_write_byte(EEPADDR_KEYLAYER, layer);
            retVal = 1;
            break;

        case KEY_RESET:
            Reset_AVR();
            break;
            
        default:
            break;
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


static uint8_t scanmatrix(void)
{
	uint8_t col, row;
	uint8_t prev, cur;
    uint8_t vPinG;
    uint8_t vPinC;
    uint8_t vPinF;

	uint8_t matrixState = 0;

    if (scankeycntms++ >= 60000)
    {
        scankeycntms--;
        kbdsleepmode = 1;
        led_mode = LED_EFFECT_OFF;
        timer1PWMBSet((uint16_t)(0));
        timer1PWMBOn();
        led_off(LED_BLOCK_ALL);
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
 
        curMATRIX[col] = (uint32_t)vPinG << 16 | (uint32_t)vPinC << 8 | (uint32_t)vPinF;
        if(curMATRIX[col])
        {
            matrixState |= SCAN_DIRTY;
        }
 	}
    return matrixState;
}


uint8_t toggle = 0;

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


	//static int pushedLevel_prev = 0;

    /* LED Blinker */
    led_blink(matrixState);

    /* LED Fader */
    led_fader();


    /* debounce check */
	if(matrixState & SCAN_CHANGED)
	{
		debounce = DEBOUNCE_MAX;
        return retVal;
	}
	if(debounce > 1)
	{
        debounce--;
		return retVal;
	}

  
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
            keyidx = pgm_read_byte(keymap[t_layer]+(col*MAX_ROW)+row);

            if (keyidx == KEY_NONE || keyidx == KEY_FN)
                continue;

 
            if (!prevBit && curBit)   //pushed
            {
                led_pushed_level_cal();          /* LED_EFFECT_PUSHED_LEVEL calculate */        
                if (processFNkeys(keyidx))
                    continue;
            }

            if(usbmode)
            {
                if(curBit)
                {
                    if(debounceMATRIX[col][row]++ >= DEBOUNCE_MAX)
                    {
                        retVal = buildHIDreports(keyidx);
                        debounceMATRIX[col][row] = DEBOUNCE_MAX;
                    }
                }else
                {
                    debounceMATRIX[col][row] = 0;
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
                            if (keyidx <= KEY_WAKE)  // ignore FN keys
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


