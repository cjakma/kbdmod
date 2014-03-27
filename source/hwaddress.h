
#define EEPADDR_LED_STATUS   (uint8_t *)11
#define EEPADDR_USBPS2_MODE  (uint8_t *)12
#define EEPADDR_KEYLAYER     (uint8_t *)13
#define EEPADDR_SWAPCTRLCAPS (uint8_t *)14
#define EEPADDR_SWAPALTGUI   (uint8_t *)15

#define EEPADDR_MACRO_SET    (uint8_t *)100   // 100~152 (1Byte x 52)


#define KEYMAP_LAYER0         0x9000
#define KEYMAP_LAYER1         0x9100
#define KEYMAP_LAYER2         0x9200
#define KEYMAP_LAYER3         0x9300
#define KEYMAP_LAYER4         0x9400
#define KEYMAP_LAYER5         0x9500
#define KEYMAP_LAYER6         0x9600
#define KEYMAP_LAYER7         0x9700


#define LEDMODE_ADDRESS          0x9800      // 0x9800:0x98FF  (256Bytes)
#define MACRO_ADDR_START         0x10000     // 0x10000 ~ 1CFFF  (52KBytes - 1KB x 52)

