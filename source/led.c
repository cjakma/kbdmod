#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <util/delay.h>     /* for _delay_ms() */


#include "led.h"
#include "global.h"
#include "timer128.h"
#include "matrix.h"
#include "eepaddress.h"

#define PUSHED_LEVEL_MAX        20



static uint8_t *const ledport[] = {LED_NUM_PORT, LED_CAP_PORT,LED_SCR_PORT, LED_PRT_PORT, 
                                    LED_BLOCK_ESC_PORT,LED_BLOCK_Fx_PORT,LED_BLOCK_PAD_PORT, LED_BLOCK_FULL_PORT, 
                                    LED_BLOCK_WASD_PORT,LED_BLOCK_ARROW18_PORT, LED_BLOCK_ARROW30_PORT};
    
static uint8_t const ledpin[] = {LED_NUM_PIN, LED_CAP_PIN,LED_SCR_PIN, LED_PRT_PIN, 
                                    LED_BLOCK_ESC_PIN,LED_BLOCK_Fx_PIN,LED_BLOCK_PAD_PIN,LED_BLOCK_FULL_PIN, 
                                    LED_BLOCK_WASD_PIN,LED_BLOCK_ARROW18_PIN, LED_BLOCK_ARROW30_PIN};
#if 0
static uint8_t ledmode[] = {LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, 
                            LED_EFFECT_FADING, LED_EFFECT_FADING_PUSH_ON, LED_EFFECT_ALWAYS, LED_EFFECT_PUSHED_LEVEL, 
                            LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS};

#else
static uint8_t ledmode[] = {LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, LED_EFFECT_ALWAYS, 
                            LED_EFFECT_FADING, LED_EFFECT_FADING, LED_EFFECT_FADING_PUSH_ON, LED_EFFECT_PUSHED_LEVEL, 
                            LED_EFFECT_FADING, LED_EFFECT_FADING, LED_EFFECT_FADING};
#endif


static uint8_t speed[] = {0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4};
static uint8_t brigspeed[] = {0, 0, 0, 0, 10, 10, 10, 10, 10, 10, 10};
static uint8_t pwmDir[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint32_t pwmCounter[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

LED_MODE led_mode = LED_EFFECT_FADING;

static int16_t pushedLevelStay;
static uint8_t pushedLevel;
static uint16_t pushedLevelDuty;
uint8_t LEDstate;     ///< current state of the LEDs

extern int16_t scankeycntms;
extern uint8_t kbdsleepmode;

void led_off(LED_BLOCK block)
{
    uint8_t i;
    switch(block)
    {
        case LED_BLOCK_NUMLOCK:
        case LED_BLOCK_CAPSLOCK:
        case LED_BLOCK_SCROLLOCK:
        case LED_BLOCK_PRT:

        case LED_BLOCK_ESC:
        case LED_BLOCK_Fx:
        case LED_BLOCK_PAD:
        case LED_BLOCK_FULL:
        case LED_BLOCK_WASD:
        case LED_BLOCK_ARROW18:
        case LED_BLOCK_ARROW30:
            *(ledport[block]) &= ~(BV(ledpin[block]));
            break;
        case LED_BLOCK_ALL:
            for (i = 0; i < LED_BLOCK_ALL; i++)
            {
                *(ledport[i]) &= ~BV(ledpin[i]);
            }

            break;
        default:
            break;
    }
}

void led_on(LED_BLOCK block)
{
    
    uint8_t i;
    switch(block)
    {
        case LED_BLOCK_NUMLOCK:
        case LED_BLOCK_CAPSLOCK:
        case LED_BLOCK_SCROLLOCK:
        case LED_BLOCK_PRT:

        case LED_BLOCK_ESC:
        case LED_BLOCK_Fx:
        case LED_BLOCK_PAD:
        case LED_BLOCK_FULL:
        case LED_BLOCK_WASD:
        case LED_BLOCK_ARROW18:
        case LED_BLOCK_ARROW30:
            *(ledport[block]) |= BV(ledpin[block]);
            
            break;
        case LED_BLOCK_ALL:
            for (i = 0; i < LED_BLOCK_ALL; i++)
            {
                *(ledport[i]) |= BV(ledpin[i]);
            }

            break;
        default:
            break;
    }

}


void led_wave_on(LED_BLOCK block)
{
    switch(block)
    {
        case LED_BLOCK_ESC:
            timer3PWMCOn();
            break;
        case LED_BLOCK_Fx:
            timer3PWMAOn();
            break;
        case LED_BLOCK_PAD:
            timer0PWMOn();
            break;
        case LED_BLOCK_FULL:
            timer3PWMBOn();
            break;
        case LED_BLOCK_WASD:
            timer1PWMCOn();
            break;
        case LED_BLOCK_ARROW18:
            timer1PWMBOn();
            break;
        case LED_BLOCK_ARROW30:
            timer1PWMAOn();
            break;
            
        case LED_BLOCK_ALL:
            timer3PWMCOn();
            timer3PWMAOn();
            timer3PWMBOn();
            timer1PWMCOn();
            timer1PWMBOn();
            timer1PWMAOn();
            timer0PWMOn();
            break;
        default:
            break;
    }
}

void led_wave_off(LED_BLOCK block)
{
    switch(block)
    {
        case LED_BLOCK_ESC:
            timer3PWMCOff();
            break;
        case LED_BLOCK_Fx:
            timer3PWMAOff();
            break;
        case LED_BLOCK_PAD:
            timer0PWMOff();
            break;
        case LED_BLOCK_FULL:
            timer3PWMBOff();
            break;
        case LED_BLOCK_WASD:
            timer1PWMCOff();
            break;
        case LED_BLOCK_ARROW18:
            timer1PWMBOff();
            break;
        case LED_BLOCK_ARROW30:
            timer1PWMAOff();
            break;
            
        case LED_BLOCK_ALL:
            timer3PWMCOff();
            timer3PWMAOff();
            timer3PWMBOff();
            timer1PWMCOff();
            timer1PWMBOff();
            timer1PWMAOff();
            timer0PWMOff();
            break;
        default:
            break;
    }
}




void led_wave_set(LED_BLOCK block, uint16_t duty)
{
    switch(block)
    {
        case LED_BLOCK_ESC:
            timer3PWMCSet(duty);
            break;
        case LED_BLOCK_Fx:
            timer3PWMASet(duty);
            break;
        case LED_BLOCK_PAD:
            timer0PWMSet(duty);
            break;
        case LED_BLOCK_FULL:
            timer3PWMBSet(duty);
            break;
        case LED_BLOCK_WASD:
            timer1PWMCSet(duty);
            break;
        case LED_BLOCK_ARROW18:
            timer1PWMBSet(duty);
            break;
        case LED_BLOCK_ARROW30:
            timer1PWMASet(duty);
            break;
            
        case LED_BLOCK_ALL:
            timer3PWMCSet(duty);
            timer3PWMASet(duty);
            timer3PWMBSet(duty);
            timer1PWMCSet(duty);
            timer1PWMBSet(duty);
            timer1PWMASet(duty);
            timer0PWMSet(duty);
            break;
        default:
            break;
    }



}




void led_blink(int matrixState)
{
    LED_BLOCK ledblock;

    for (ledblock = LED_BLOCK_ESC; ledblock<LED_BLOCK_ALL; ledblock++)
    {
        
        if(matrixState & SCAN_DIRTY)      // 1 or more key is pushed
        {
            switch(ledmode[ledblock])
            {

                case LED_EFFECT_FADING_PUSH_ON:
                case LED_EFFECT_PUSH_ON:
                    led_on(ledblock);
                    break;
                case LED_EFFECT_PUSH_OFF:
                    led_wave_off(ledblock);
                    led_wave_set(ledblock, 0);
                    led_off(ledblock);
                    break;
                default :
                    break;
            }
            scankeycntms = 0;
            if (kbdsleepmode == 1)
            {
                led_mode_init();
                led_3lockupdate(LEDstate);
                kbdsleepmode = 0;
            }
             
        }else{          // none of keys is pushed
            switch(ledmode[ledblock])
                 {
                     case LED_EFFECT_FADING_PUSH_ON:
                     case LED_EFFECT_PUSH_ON:
                        led_off(ledblock);
                        break;
                     case LED_EFFECT_PUSH_OFF:
                        led_on(ledblock);
                        break;
                     default :
                         break;
                 }
        }
    }
}

void led_fader(void)
{
    
    LED_BLOCK ledblock;
    for (ledblock = LED_BLOCK_ESC; ledblock<LED_BLOCK_ALL; ledblock++)
    {
        if((ledmode[ledblock] == LED_EFFECT_FADING) || ((ledmode[ledblock] == LED_EFFECT_FADING_PUSH_ON) && (scankeycntms > 1000)))
        {
            if(pwmDir[ledblock]==0)
            {
                led_wave_set(ledblock, ((uint16_t)(pwmCounter[ledblock]/brigspeed[ledblock])));        // brighter
                if(pwmCounter[ledblock]>=255*brigspeed[ledblock])
                    pwmDir[ledblock] = 1;
                    
            }
            else if(pwmDir[ledblock]==2)
            {
                led_wave_set(ledblock, ((uint16_t)(255-pwmCounter[ledblock]/speed[ledblock])));    // darker
                if(pwmCounter[ledblock]>=255*speed[ledblock])
                    pwmDir[ledblock] = 3;

            }
            else if(pwmDir[ledblock]==1)
            {
                if(pwmCounter[ledblock]>=255*speed[ledblock])
                   {
                       pwmCounter[ledblock] = 0;
                       pwmDir[ledblock] = 2;
                   }
            }else if(pwmDir[ledblock]==3)
            {
                if(pwmCounter[ledblock]>=255*brigspeed[ledblock])
                   {
                       pwmCounter[ledblock] = 0;
                       pwmDir[ledblock] = 0;
                   }
            }


            led_wave_on(ledblock);

            // pwmDir 0~3 : idle
       
            pwmCounter[ledblock]++;

        }else if (ledmode[ledblock] == LED_EFFECT_PUSHED_LEVEL)
        {
    		// 일정시간 유지

    		if(pushedLevelStay > 0){
    			pushedLevelStay--;
    		}else{
    			// 시간이 흐르면 레벨을 감소 시킨다.
    			if(pushedLevelDuty > 0){
    				pwmCounter[ledblock]++;
    				if(pwmCounter[ledblock] >= speed[ledblock]){
    					pwmCounter[ledblock] = 0;			
    					pushedLevelDuty--;
    					pushedLevel = PUSHED_LEVEL_MAX - (255-pushedLevelDuty) / (255/PUSHED_LEVEL_MAX);
    					/*if(pushedLevel_prev != pushedLevel){
    						DEBUG_PRINT(("---------------------------------decrease pushedLevel : %d, life : %d\n", pushedLevel, pushedLevelDuty));
    						pushedLevel_prev = pushedLevel;
    					}*/
    				}
    			}else{
    				pushedLevel = 0;
    				pwmCounter[ledblock] = 0;
    			}
    		}
    		led_wave_set(ledblock, pushedLevelDuty);

    	}else
        {
            led_wave_set(ledblock, 0);
            led_wave_off(ledblock);
            pwmCounter[ledblock]=0;
            pwmDir[ledblock]=0;
        }
    }
}

void led_check(uint8_t forward)
{
    led_off(LED_BLOCK_ALL);
    if(forward){
        led_on(LED_BLOCK_PRT);
    }else{
        led_on(LED_BLOCK_NUMLOCK);
    }
    _delay_ms(100);
    led_off(LED_BLOCK_ALL);
    led_on(LED_BLOCK_SCROLLOCK);

    _delay_ms(100);
    led_off(LED_BLOCK_ALL);
    if(forward){
        led_on(LED_BLOCK_NUMLOCK);
    }else{
        led_on(LED_BLOCK_PRT);
    }
    _delay_ms(100);
    led_off(LED_BLOCK_ALL);
}


void led_3lockupdate(uint8_t LEDstate)
{
        if (LEDstate & LED_NUM) { // light up caps lock
            led_on(LED_BLOCK_NUMLOCK);
        } else {
            led_off(LED_BLOCK_NUMLOCK);
        }
        if (LEDstate & LED_CAPS) { // light up caps lock
            led_on(LED_BLOCK_CAPSLOCK);
            if (led_mode == LED_EFFECT_FULLCAPS)
                led_on(LED_BLOCK_FULL);
        } else {
            led_off(LED_BLOCK_CAPSLOCK);
            if (led_mode == LED_EFFECT_FULLCAPS)
                led_off(LED_BLOCK_FULL);
        }
        if (LEDstate & LED_SCROLL) { // light up caps lock
            led_on(LED_BLOCK_SCROLLOCK);
        } else {
            led_off(LED_BLOCK_SCROLLOCK);
        }
}


void led_mode_init(void)
{
    LED_BLOCK ledblock;

    led_mode = eeprom_read_byte(EEPADDR_LED_STATUS); 
    for (ledblock = 0; ledblock < LED_BLOCK_ALL; ledblock++)
    {
        led_mode_change(ledblock, ledmode[ledblock]);
    }
}

void led_mode_change (LED_BLOCK ledblock, int mode)
{
    switch (mode)
    {
        case LED_EFFECT_FADING :
        case LED_EFFECT_FADING_PUSH_ON :
            break;
        case LED_EFFECT_PUSH_OFF :
        case LED_EFFECT_ALWAYS :
            led_on(ledblock);
            break;
        case LED_EFFECT_PUSH_ON :
        case LED_EFFECT_OFF :
        case LED_EFFECT_PUSHED_LEVEL :
        case LED_EFFECT_FULLCAPS :
            led_off(ledblock);
            led_wave_set(ledblock,0);
            led_wave_on(ledblock);
            break;
        default :
            ledmode[ledblock] = LED_EFFECT_FADING;
            break;
     }
    eeprom_write_byte(EEPADDR_LED_STATUS, ledblock);
}


void led_pushed_level_cal(void)
{
	// update pushed level
	if(pushedLevel < PUSHED_LEVEL_MAX)
	{
		pushedLevelStay = 511;
		pushedLevel++;
		pushedLevelDuty = (255 * pushedLevel) / PUSHED_LEVEL_MAX;
	}
}


