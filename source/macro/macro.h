#define MAX_MACRO_INDEX     48
#define MAX_MACRO_LEN       1024
#define MACRO_ADDR_START    0x3000

extern uint8_t initMacroAddr(void);

extern void playMacroUSB(uint8_t macrokey);
extern void playMacroPS2(uint8_t macrokey);
extern void recordMacro(uint8_t macrokey);

