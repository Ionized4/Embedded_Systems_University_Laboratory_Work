# Embedded_Systems_University_Laboratory_Work
This repository contains a sample of university laboratory work related to embedded systems.  Systems were implemented using assembly language and C for the ATMEGA4809.

Some files included (primarily drivers) may not be my own and were provided by Dr Kenneth Short and Bryant Gonzaga at Stony Brook University, or may have been provided through peripheral component documentation.

The ESE381_Lab11_src folder contains the C source code for a program that read raw values from a Carbon Dioxide sensor, a temperature sensor, and a humidity sensor, parse them into their decimal values, and write them to a connected LCD display.
The CO2 value was also processed and displayed on an LED bargraph peripheral, connected to the MCU via I2C using the MCP23017 GPIO expander.

The ESE280_Lab11_temp_meas_src folder contains the assembly source code and schematic diagram for a program that read an analog voltage from a temperature sensor and converted it into a digital value and displayed it to a seven segment display.
