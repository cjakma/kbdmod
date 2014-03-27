#define MAX_MACRO_INDEX     52
#define MAX_MACRO_LEN       1024

extern uint8_t initMacroAddr(void);

extern void playMacroUSB(uint8_t macrokey);
extern void playMacroPS2(uint8_t macrokey);
extern void recordMacro(uint8_t macrokey);


extern int8_t flash_writeinpage (uint8_t *data, unsigned long addr);


void writepage(uint8_t *data, unsigned long addr) 
    __attribute__ ((section (".appinboot")));

