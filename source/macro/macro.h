#define MAX_MACRO_INDEX     52
#define MAX_MACRO_LEN       1024
#define MACRO_ADDR_START    0x10000

extern uint8_t initMacroAddr(void);

extern void playMacroUSB(uint8_t macrokey);
extern void playMacroPS2(uint8_t macrokey);
extern void recordMacro(uint8_t macrokey);

void writepage(uint8_t *data, unsigned long addr) 
    __attribute__ ((section (".appinboot")));

