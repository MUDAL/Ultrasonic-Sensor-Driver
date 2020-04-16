# Ultrasonic-Sensor-Driver
Bare metal code (library) to interface an Atmega328p with an HCSR04 ultrasonic sensor

Components:
1. Atmega328p micrcontroller (or an Arduino Uno)
2. HCSR04 ultrasonic sensor
3. 4 connectors

Connections:
1. Vcc pin of sensor to 5v
2. Gnd pin of sensor to Gnd
3. Trig pin of sensor to PD6 of Atmega328p or Pin 6 of Uno
4. Echo pin of sensor to PB0 of Atmega328p or Pin 8 of Uno

Working:
The library uses Timer 0 and Timer 1 of the Atmega328p. Timer 0 is used to generate PWM signals on PD6 to trigger the sensor. The echo pulse then generates a pulse whose width is measured by the Input Capture feature of Timer 1.

How to use the library:
See Example.c

Library functions:
1. void HCSR04_Init(void) :: This initializes the ultrasonic sensor
2. uint32_t getDistance(void) :: This returns the distance measured

Additional information can be found in the datasheets of the Atmega328p and the HCSR04.

Suggestions for improving the codes will be greatly appreciated


