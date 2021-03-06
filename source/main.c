/* Copyright Jamie Honan, 2001.  Distributed under the GPL.
   This program comes with ABSOLUTELY NO WARRANTY.
   See the file COPYING for license details.
   */
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
#include "hwaddress.h"
#include "matrix.h"
#include "macro.h"

int8_t isUSB;
extern uint8_t usbmain(void);
extern uint8_t ps2main(void);


#ifdef DEBUG
void enable_printf(void)
{
	stdout = &mystdout;
    DDRD |= 0x01;

	UBRRH = (UBRR>>8);
	UBRRL = UBRR;
    
	UCSRA = 0x00;									   // asynchronous normal mode
	UCSRB = 0x08;									   // Tx enable, 8 data
	UCSRC = 0x86;									   // no parity, 1 stop, 8 data
}
#else
void enable_printf(void)
{
}
#endif



void interfaceChange(uint8_t isUSB)
{

}



int portInit(void)
{
//  initialize matrix ports - cols, rows
//    PA       0:7      col      (6, 7 reserved)
// ======KBDMOD_M5=========
//    PG       0:1      row 0, 1
//               2:4       N/A
//    PC       0:7      row 2:9
//    PF        0:7      row 10:17
// ======KBDMOD_M7=========
//    PC       0:7      row 0:7
//    PF        0:7      row 8:15
//    PG       0:2      row 16:18
//               3:4         N/A

//    PB
//    0      INDI0      //M5-N/A
//    1      SCK (ISP)
//    2      INDI1      //M5-N/A
//    3      INDI2      //M5-N/A
//    4(OC0)       LED_PIN_WASD
//    5(OC1A)     LED_PIN_PAD
//    6(OC1B)     LED_PIN_PRT
//    7(OC1C)     LED_VESEL
    
//    PD
//    0(INT0)      D+
//    1(INT1)      D-
//    2               INDI3     //M5 -PRTSCR
//    3               LED_NUM
//    4               LED_CAPS
//    5               LED_SCR
//    6               Zener Diode
//    7               D+ pull-up register
    
//    PE
//    0(PDI)        MOSI
//    1(PDO)       MISO
//    2                N/A
//    3(OC3A)     LED_PIN_Fx
//    4(OC3B)     LED_PIN_BASE
//    5(OC3C)     LED_PIN_ESC
//    6                N/A
//    7                N/A

    
// signal direction : col -> row

//  Matrix
	PORTA	= 0xFF;	//  col
	PORTC	= 0xFF; //  row
    PORTF   = 0xFF; //  row
    PORTG   = 0xFF; //  row

	DDRA	= 0x00;	// col
	DDRC	= 0x00; // row 2, 3, 4, 5, 6, 7, 8, 9
    DDRF    = 0x00; // row 10, 11, 12, 13, 14, 15, 16, 17
    DDRG    = 0x00; // row 0, 1

//  LED port
	PORTB	= 0x00;	// LED_VESEL, LED_PIN_PRT, LED_PIN_PAD, LED_PIN_WASD off          (00001111)
	DDRB 	= 0xFF;	// LED_VESEL, LED_PIN_PRT, LED_PIN_PAD, LED_PIN_WASD OUT        (11110000)

	PORTE	= 0x00;	// LED_PIN_ESC, LED_PIN_BASE, LED_PIN_Fx    off                           (11000111)
    DDRE	= 0xFF;	// LED_PIN_ESC, LED_PIN_BASE, LED_PIN_Fx    OUT                         (00111000)

    PORTD   = 0x40; // DPpull-up(Low), Zener(pull-up), LED_SCR, LED_CAPS, LED_NUM (0ff), D-(0), D+(0)
    DDRD    = 0xFC; // DPpull-up(OUT), Zener(OUT), LED_SCR, LED_CAPS, LED_NUM (OUT), D-(INPUT), D+(INPUT)
    return 0;
}


#ifdef KBDMOD_M5
#define CHECK_U (~PINC & 0x20)  // col2-row7 => U
#define CHECK_P (~PINF & 0x01)  // col2-row10 => P
#else //KBDMOD_M7
#define CHECK_U (~PINC & 0x80)
#define CHECK_P (~PINF & 0x04)

#endif


void checkInterface(void)
{
    isUSB = 0;
	DDRA  = BV(2);        //  col2
	PORTA = ~BV(2);       //
    _delay_us(10);

    if (CHECK_U)   
    {
        isUSB = 1;
        eeprom_write_byte(EEPADDR_USBPS2_MODE, isUSB);
    }else if (CHECK_P)
    {
        isUSB = 0;
        eeprom_write_byte(EEPADDR_USBPS2_MODE, isUSB);
    }else
    {
        isUSB = eeprom_read_byte(EEPADDR_USBPS2_MODE);
        if (isUSB > 1)
            isUSB = 1;    //default USB
    }

    /* control zener diode for level shift signal line
    1 : TR on - 3v level
    0 : TR off - 5v level
    */
    if (isUSB)  // USB mode : zener enable, D+ pullup disable
    {
        sbi(USB_LEVEL_SHIFT_PORT, USB_LEVEL_SHIFT_PIN);     // pullup
        cbi(USB_LEVEL_SHIFT_DDR, USB_LEVEL_SHIFT_PIN);      // INPUT

        sbi(PS2_CLK_PULLUP_DDR, PS2_CLK_PULLUP_PIN);        // OUT
        cbi(PS2_CLK_PULLUP_PORT, PS2_CLK_PULLUP_PIN);       // drive 0
    }
    else        // PS2 mode : zener disable, D+ pullup enable
    {
        sbi(USB_LEVEL_SHIFT_DDR, USB_LEVEL_SHIFT_PIN);      // OUTPUT
        cbi(USB_LEVEL_SHIFT_PORT, USB_LEVEL_SHIFT_PIN);     // drive 0
        
        sbi(PS2_CLK_PULLUP_PORT, PS2_CLK_PULLUP_PIN);       // pullup
        cbi(PS2_CLK_PULLUP_DDR, PS2_CLK_PULLUP_PIN);        // INPUT
    }
    
}        


uint8_t countertick = 0;

int8_t indicatorAction(void)
{
    countertick++;
}




int main(void)
{
   enable_printf();
   portInit();
   checkInterface();

   timerInit();
   timer0PWMInit(8);
   timer1PWMInit(8);
   timer3PWMInit(8);
   keymap_init();
   initMacroAddr();

   led_check(isUSB);
   led_mode_init();


#if 0 //def KBDMOD_M7
   timer3IntEnable();
   timerAttach(TIMER3OVERFLOW_INT, indicatorAction);
//   timerAttach(TIMER3OUTCOMPAREB_INT, indicatorAction2);
 #endif  

   if(isUSB)
   {

      usbmain();
   }
   else
   {
      ps2main();
   }
   return 0;
}

