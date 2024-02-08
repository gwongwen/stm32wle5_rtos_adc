# Code for 6sens Omnitilt Project : test of STM32 ADC

## Overview
This first code allows us to convert a analog voltage to numeric value and adds processing to it :

 - take sample of analogic geophone value on ADC1 pin (PB13)
 - store value in external EEPROM memory (ST M95M04)

This code can therefore both test writing a digitized random value to the eeprom, reading the stored value and analog-to-digital conversion. The analog value is generated by a voltage divider bridge of a 3.7V Li-Po battery.

## Building and Running

The following commands clean build folder, build and flash the sample:

**Command to use**

west build -t pristine

west build -p always -b stm32wl_dw1000_iot_board applications/stm32wle5_rtos_adc

west flash --runner stm32cubeprogrammer