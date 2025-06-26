# Embedded Systems Laboratory Work

This repository contains selected laboratory assignments from coursework in Embedded Systems at Stony Brook University. The projects demonstrate embedded software programming using assembly and C languages on the ATmega4809 microcontroller platform.

## Lab Summaries:

### 📌 [ESE381 Lab 11: Sensor Data Acquisition and Display](ESE381_Lab11_src/)
- **Language:** C
- **Objective:** Read data from carbon dioxide, temperature, and humidity sensors.
- **Peripherals Used:** LCD display, LED bargraph, MCP23017 GPIO expander (I²C)
- **Description:** Sensor data is parsed into readable formats and displayed on an LCD. The CO₂ sensor readings also drive an LED bargraph indicator.

### 📌 [ESE280 Lab 11: Temperature Measurement](ESE280_Lab11_temp_meas_src/)
- **Language:** Assembly
- **Objective:** Convert analog voltage from a temperature sensor to a digital value.
- **Peripherals Used:** Seven-segment display, ADC module
- **Description:** Reads sensor data via ADC, performs conversion, and displays results on a seven-segment display.

## Attribution:
- Most code is original work. Some peripheral drivers were provided by Dr. Kenneth Short and Bryant Gonzaga (Stony Brook University) or extracted from official peripheral documentation.

## Build and Execution:
- **Assembly projects:** Assemble using AVR-GCC assembler or similar tools.
- **C projects:** Compile using AVR-GCC or your preferred IDE (e.g., Atmel Studio, Microchip MPLAB X).
- Schematics are provided within relevant lab directories.

## Microcontroller Platform:
- **Target MCU:** ATmega4809
