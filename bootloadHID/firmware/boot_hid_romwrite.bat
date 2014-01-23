avrdude -c stk500v2 -P com9 -p atmega128 -U efuse:w:0xFF:m -U hfuse:w:0xDA:m -U lfuse:w:0xEF:m
avrdude -c stk500v2 -P com9 -p atmega128 -U flash:w:main.hex:i
;avrdude -c stk500v2 -P com9 -p atmega128 -U lfuse:r:low:r -U hfuse:r:high:r -U lock:r:lock:r
pause;