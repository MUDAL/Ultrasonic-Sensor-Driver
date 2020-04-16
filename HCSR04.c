/*
 * HCSR04.c
 *
 * Created: 16/04/2020 11:38:07 AM
 *  Author: OLAOLUWA
 */ 


#ifndef F_CPU
#define F_CPU 16000000
#endif //F_CPU

#include <avr/io.h>
#include <avr/interrupt.h>
#include "HCSR04.h"
#define MAX 0xFFFF

void HCSR04_Init(){
	
	cli(); //clear prior interrupts
	/*Fast PWM Configuration*/
	DDRD |= (1<<DDD6); //set PD6 as output
	TCCR0A = (1<<COM0A1)|(1<<COM0A0)|(1<<WGM01)|(1<<WGM00);
	TCCR0B |= (1<<CS01); //prescaler = 8 for timer 0
	OCR0A = 235; //10uS trigger pulse, 118uS off-time (128uS repetition rate)
	/*Input Capture configuration*/
	//Timer 1 running in normal mode
	DDRB &= ~(1<<DDB0); //PB0 as input (ICP1)
	TCCR1B = (1<<ICNC1)|(1<<ICES1)|(1<<CS11); //noise canceling + positive edge detection for input capture and Prescaler = 8.
	sei();//enable global interrupts
	TIMSK1 |= (1<<ICIE1); //enable timer1 input capture interrupt

}

uint32_t getDistance(){
	static uint32_t echo_pulse_uS;
	static uint32_t distance_cm;
	extern  volatile uint32_t duty_cycle;
	
        //32768uS = 65536 clock ticks for Timer 1 with prescaler = 8
	echo_pulse_uS = (float)duty_cycle * 32768 / 65536;
	distance_cm = echo_pulse_uS * 0.034 / 2;
	return distance_cm;
}

ISR(TIMER1_CAPT_vect){
	extern  volatile uint32_t first_reading;
	extern  volatile uint32_t second_reading;
	extern  volatile uint32_t duty_cycle;
	
	if ((TCCR1B & (1<<ICES1)) == (1<<ICES1)){
		first_reading = ICR1;
	}
	else{
		second_reading = ICR1;
	}
	
	if (first_reading != 0 && second_reading != 0){
		if (second_reading > first_reading){
			duty_cycle = second_reading - first_reading;
		}
		else {
			duty_cycle = MAX + second_reading - first_reading + 1;
		}
		first_reading = 0;
		second_reading = 0;
	}
	TCCR1B ^= (1<<ICES1); //toggle edge detection bit
	TIFR1 = (1<<ICF1);//clear Input Capture Flag
}

volatile uint32_t first_reading = 0;
volatile uint32_t second_reading = 0;
volatile uint32_t duty_cycle = 0;
