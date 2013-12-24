#define KEYBD_EXTERN
#include "global.h"
#include "timer128.h"
//#include "keysta.h"
#include "keymap.h"
#include "print.h"
#include "led.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include <util/delay.h>     /* for _delay_ms() */
#include <string.h>

#include "usbdrv.h"
#include "matrix.h"
#include "usbmain.h"
#include "hwport.h"

uint8_t interfaceReady = 0;


/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

static uint8_t keyboardReport[8]; ///< buffer for HID reports
static uint8_t oldReportBuffer[8]; ///< buufer for HID reports save on Overflower
uint8_t reportIndex; // keyboardReport[0] contains modifiers



report_extra_t extraReport;
report_extra_t oldextraReport;



static uint8_t idleRate = 0;        ///< in 4ms units
static uint8_t protocolVer = 1; ///< 0 = boot protocol, 1 = report protocol
uint8_t expectReport = 0;       ///< flag to indicate if we expect an USB-report





#if USB_CFG_DESCR_PROPS_CONFIGURATION
PROGMEM const char usbDescriptorConfiguration[] = {    /* USB configuration descriptor */
    9,          /* sizeof(usbDescriptorConfiguration): length of descriptor in bytes */
    USBDESCR_CONFIG,    /* descriptor type */
    9 + (9 + 9 + 7) + (9 + 9 + 7), 0,
    //18 + 7 * USB_CFG_HAVE_INTRIN_ENDPOINT + 7 * USB_CFG_HAVE_INTRIN_ENDPOINT3 + 9, 0,
                /* total length of data returned (including inlined descriptors) */
    2,          /* number of interfaces in this configuration */
    1,          /* index of this configuration */
    0,          /* configuration name string index */
#if USB_CFG_IS_SELF_POWERED
    (1 << 7) | USBATTR_SELFPOWER,       /* attributes */
#else
    (1 << 7),                           /* attributes */
#endif
    USB_CFG_MAX_BUS_POWER/2,            /* max USB current in 2mA units */

    /*
     * Keyboard interface
     */
    /* Interface descriptor */
    9,          /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE, /* descriptor type */
    0,          /* index of this interface */
    0,          /* alternate setting for this interface */
    USB_CFG_HAVE_INTRIN_ENDPOINT, /* endpoints excl 0: number of endpoint descriptors to follow */
    USB_CFG_INTERFACE_CLASS,
    USB_CFG_INTERFACE_SUBCLASS,
    USB_CFG_INTERFACE_PROTOCOL,
    0,          /* string index for interface */
    /* HID descriptor */
    9,          /* sizeof(usbDescrHID): length of descriptor in bytes */
    USBDESCR_HID,   /* descriptor type: HID */
    0x01, 0x01, /* BCD representation of HID version */
    0x00,       /* target country code */
    0x01,       /* number of HID Report (or other HID class) Descriptor infos to follow */
    0x22,       /* descriptor type: report */
    sizeof(keyboard_hid_report), 0,  /* total length of report descriptor */
    /* Endpoint descriptor */
#if USB_CFG_HAVE_INTRIN_ENDPOINT    /* endpoint descriptor for endpoint 1 */
    7,          /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    (char)0x81, /* IN endpoint number 1 */
    0x03,       /* attrib: Interrupt endpoint */
    8, 0,       /* maximum packet size */
    USB_CFG_INTR_POLL_INTERVAL, /* in ms */
#endif

    /*
     * Mouse interface
     */
    /* Interface descriptor */
    9,          /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE, /* descriptor type */
    1,          /* index of this interface */
    0,          /* alternate setting for this interface */
    USB_CFG_HAVE_INTRIN_ENDPOINT3, /* endpoints excl 0: number of endpoint descriptors to follow */
    0x03,       /* CLASS: HID */
    0,          /* SUBCLASS: none */
    0,          /* PROTOCOL: none */
    0,          /* string index for interface */
    /* HID descriptor */
    9,          /* sizeof(usbDescrHID): length of descriptor in bytes */
    USBDESCR_HID,   /* descriptor type: HID */
    0x01, 0x01, /* BCD representation of HID version */
    0x00,       /* target country code */
    0x01,       /* number of HID Report (or other HID class) Descriptor infos to follow */
    0x22,       /* descriptor type: report */
    sizeof(mouse_hid_report), 0,  /* total length of report descriptor */
#if USB_CFG_HAVE_INTRIN_ENDPOINT3   /* endpoint descriptor for endpoint 3 */
    /* Endpoint descriptor */
    7,          /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    (char)(0x80 | USB_CFG_EP3_NUMBER), /* IN endpoint number 3 */
    0x03,       /* attrib: Interrupt endpoint */
    8, 0,       /* maximum packet size */
    USB_CFG_INTR_POLL_INTERVAL, /* in ms */
#endif
};
#endif


USB_PUBLIC usbMsgLen_t usbFunctionDescriptor(struct usbRequest *rq)
{
    usbMsgLen_t len = 0;

/*
    debug("usbFunctionDescriptor: ");
    debug_hex(rq->bmRequestType); debug(" ");
    debug_hex(rq->bRequest); debug(" ");
    debug_hex16(rq->wValue.word); debug(" ");
    debug_hex16(rq->wIndex.word); debug(" ");
    debug_hex16(rq->wLength.word); debug("\n");
*/
    switch (rq->wValue.bytes[1]) {
#if USB_CFG_DESCR_PROPS_CONFIGURATION
        case USBDESCR_CONFIG:
            usbMsgPtr = (usbMsgPtr_t)usbDescriptorConfiguration;
            len = sizeof(usbDescriptorConfiguration);
            break;
#endif
        case USBDESCR_HID:
            switch (rq->wValue.bytes[0]) {
                case 0:
                    usbMsgPtr = (usbMsgPtr_t)(usbDescriptorConfiguration + 9 + 9);
                    len = 9;
                    break;
                case 1:
                    usbMsgPtr = (usbMsgPtr_t)(usbDescriptorConfiguration + 9 + (9 + 9 + 7) + 9);
                    len = 9;
                    break;
            }
            break;
        case USBDESCR_HID_REPORT:
            /* interface index */
            switch (rq->wIndex.word) {
                case 0:
                    usbMsgPtr = (usbMsgPtr_t)keyboard_hid_report;
                    len = sizeof(keyboard_hid_report);
                    break;
                case 1:
                    usbMsgPtr = (usbMsgPtr_t)mouse_hid_report;
                    len = sizeof(mouse_hid_report);
                    break;
            }
            break;
    }
    //debug("desc len: "); debug_hex(len); debug("\n");
    return len;
}




/**
 * This function is called whenever we receive a setup request via USB.
 * \param data[8] eight bytes of data we received
 * \return number of bytes to use, or 0xff if usbFunctionWrite() should be
 * called
 */
uint8_t usbFunctionSetup(uint8_t data[8]) {
    usbRequest_t *rq = (void *)data;

    
	interfaceReady = 1;

    usbMsgPtr = (usbMsgPtr_t)keyboardReport;
    if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
        // class request type
        if (rq->bRequest == USBRQ_HID_GET_REPORT) {
            // wValue: ReportType (highbyte), ReportID (lowbyte)
            // we only have one report type, so don't look at wValue
            usbMsgPtr = (usbMsgPtr_t)keyboardReport;
            return sizeof(keyboardReport);
        } else if (rq->bRequest == USBRQ_HID_SET_REPORT) {
            if (rq->wValue.word == 0x0200 && rq->wIndex.word == 0) {
                // We expect one byte reports
                expectReport = 1;
            }
            return 0xff; // Call usbFunctionWrite with data
        } else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
            usbMsgPtr = (usbMsgPtr_t)&idleRate;
            return 1;
        } else if (rq->bRequest == USBRQ_HID_SET_IDLE) {
            idleRate = rq->wValue.bytes[1];
            DEBUG_PRINT(("idleRate = %2x\n", idleRate));
        } else if (rq->bRequest == USBRQ_HID_GET_PROTOCOL) {
            if (rq->wValue.bytes[1] < 1) {
                protocolVer = rq->wValue.bytes[1];
            }
        } else if(rq->bRequest == USBRQ_HID_SET_PROTOCOL) {
            usbMsgPtr = (usbMsgPtr_t)&protocolVer;
            return 1;
        }
    } else {
        // no vendor specific requests implemented
    }
    return 0;
}

/**
 * The write function is called when LEDs should be set. Normally, we get only
 * one byte that contains info about the LED states.
 * \param data pointer to received data
 * \param len number ob bytes received
 * \return 0x01
 */
uint8_t usbFunctionWrite(uchar *data, uchar len) {
    if (expectReport && (len == 1)) {
        LEDstate = data[0]; // Get the state of all 5 LEDs
        led_3lockupdate(LEDstate);
    }
    expectReport = 0;
    return 0x01;
}

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

uint8_t hello[] = "hello world";


uint8_t clearReportBuffer(void)
{   
    reportIndex = 2; // keyboardReport[0] contains modifiers
    memset(keyboardReport, 0, sizeof(keyboardReport)); // clear report
    extraReport.usage = 0;
    return 0;
}

uint8_t saveReportBuffer(void)
{
    memcpy(oldReportBuffer, keyboardReport, sizeof(keyboardReport));
    return 0;
}

uint8_t restoreReportBuffer(void)
{
    memcpy(keyboardReport, oldReportBuffer, sizeof(keyboardReport));
    return 0;
}


uint8_t bufcmp(uint8_t *s1, uint8_t *s2, uint8_t size)
{
    uint8_t result = 0;
    uint8_t i;
    
    for (i = 0; i < size; i++)
    {
        if(*s1 != *s2)
        {
            result = 1;
            break;
        }
        s1++;
        s2++;
        
    }
    return result;
}


uint8_t cmpReportBuffer(void)
{
    uint8_t result = 0;
    uint8_t *s1, *s2;
    uint8_t size;

    s1 = keyboardReport;
    s2 = oldReportBuffer;
    size = sizeof(keyboardReport);
    result = bufcmp(s1, s2, size);
    
    return result;
}


uint8_t usbRollOver = 0;


uint8_t buildHIDreports(uint8_t keyidx)
{
    uint8_t retval = 0;
   
    if((keyidx > KEY_Modifiers) && (keyidx < KEY_Modifiers_end))
    {
        keyboardReport[0] |= modifierBitmap[keyidx-KEY_Modifiers];
    }else if((keyidx > KEY_Consumers) && (keyidx < KEY_System))
    {
        extraReport.report_id = REPORT_ID_CONSUMER;
        extraReport.usage = KEYCODE2CONSUMER(keyidx);
        
        retval |= 0x04;                                                             // continue decoding to get modifiers

    }else if((keyidx > KEY_System))
    {
        extraReport.report_id = REPORT_ID_SYSTEM;
        extraReport.usage = KEYCODE2SYSTEM(keyidx);
        
        retval |= 0x04;                                                             // continue decoding to get modifiers

    }else
    {
        if (reportIndex >= sizeof(keyboardReport))
        {   
            // too many keycodes
            memset(keyboardReport+2, ErrorRollOver, sizeof(keyboardReport)-2);
            retval |= 0x02;                                                             // continue decoding to get modifiers
        }else
        {
            keyboardReport[reportIndex] = keyidx; // set next available entry
            reportIndex++;
        }
        
    }    
    retval |= 0x01; // must have been a change at some point, since debounce is done
    return retval;
}

#ifdef DEBUG
uint8_t toggle1 = 0;

void dumpreportBuffer(void)
{
    uint8_t i;

    DEBUG_PRINT(("RBuf "));
    for (i = 0; i < sizeof(keyboardReport); i++)
    {
        DEBUG_PRINT(("%02x", keyboardReport[i]));
    }
    DEBUG_PRINT(("\n"));
}
#endif


uint8_t usbmain(void) {
    uint8_t updateNeeded = 0;
    uint8_t idleCounter = 0;
    int interfaceCount = 0;
	interfaceReady = 0;

    DEBUG_PRINT(("USB\n"));
    
    usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    _delay_ms(10);
    usbDeviceConnect();
    sei();
    
    wdt_enable(WDTO_500MS);

    while (1) {
        // main event loop

        if(interfaceReady == 0 && interfaceCount++ > 4000){
			// move to ps/2
		   Reset_AVR();
			//break;
		}
                
        wdt_reset();
        usbPoll();

        updateNeeded = scankey();   // changes?
        if (updateNeeded == 0)      //debounce
            continue;
        
        if (idleRate == 0)                  // report only when the change occured
        {
            if (cmpReportBuffer() == 0)     // exactly same status?
            {
                updateNeeded &= ~(0x01);   // clear key report
            }
            if (bufcmp((uint8_t *)&oldextraReport, (uint8_t *)&extraReport, sizeof(extraReport)) == 0)
            {
                updateNeeded &= ~(0x04);    // clear consumer report
            }
        }

        if (TIFR & (1 << TOV0)) 
        {
            TIFR = (1 << TOV0); // reset flag
            if (idleRate != 0) 
            { // do we need periodic reports?
                if(idleCounter > 4)
                { // yes, but not yet
                    idleCounter -= 5; // 22ms in units of 4ms
                }else 
                { // yes, it is time now
                    updateNeeded |= 0x01;
                    idleCounter = idleRate;
                }
            }
        }
        // if an update is needed, send the report
      
        if((updateNeeded & 0x01)  && usbInterruptIsReady())
        {
            usbSetInterrupt(keyboardReport, sizeof(keyboardReport));
            saveReportBuffer();
        }

        if((updateNeeded & 0x04)  && usbInterruptIsReady3())
        {
            usbSetInterrupt3((uchar *)&extraReport, sizeof(extraReport));
            memcpy(&oldextraReport, &extraReport, sizeof(extraReport));
        }

    }

    wdt_disable();
	USB_INTR_ENABLE &= ~(1 << USB_INTR_ENABLE_BIT);
    return 0;
}
