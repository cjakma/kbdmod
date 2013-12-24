#ifndef _HWPORT_H_
#define _HWPORT_H_

#include <avr/io.h>
#include <avr/wdt.h>



#define USB_LEVEL_SHIFT_PORT    PORTD
#define USB_LEVEL_SHIFT_DDR     DDRD
#define USB_LEVEL_SHIFT_PIN     6

#define PS2_CLK_PULLUP_PORT    PORTD
#define PS2_CLK_PULLUP_DDR     DDRD
#define PS2_CLK_PULLUP_PIN     7


#define LED_PRT_PORT        (uint8_t *const)&PORTD
#define LED_PRT_PIN			2

#define LED_NUM_PORT        (uint8_t *const)&PORTD
#define LED_NUM_PIN			3

#define LED_CAP_PORT        (uint8_t *const)&PORTD
#define LED_CAP_PIN			4

#define LED_SCR_PORT        (uint8_t *const)&PORTD
#define LED_SCR_PIN			5



#define LED_BLOCK_WASD_PORT       (uint8_t *const)&PORTB
#define LED_BLOCK_WASD_PIN        7

#define LED_BLOCK_PAD_PORT        (uint8_t *const)&PORTB
#define LED_BLOCK_PAD_PIN         4

#define LED_BLOCK_ARROW18_PORT          (uint8_t *const)&PORTB
#define LED_BLOCK_ARROW18_PIN           6

#define LED_BLOCK_ARROW30_PORT          (uint8_t *const)&PORTB
#define LED_BLOCK_ARROW30_PIN           5


#define LED_BLOCK_Fx_PORT         (uint8_t *const)&PORTE
#define LED_BLOCK_Fx_PIN          3

#define LED_BLOCK_FULL_PORT       (uint8_t *const)&PORTE
#define LED_BLOCK_FULL_PIN        4

#define LED_BLOCK_ESC_PORT        (uint8_t *const)&PORTE
#define LED_BLOCK_ESC_PIN         5



#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}


#endif
