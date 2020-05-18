/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
	static double current_frequency;

	if(frequency != current_frequency) {
		if(!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }

		if(frequency < 0.954) {OCR3A = 0xFFFF; }

		else if (frequency > 31250) { OCR3A = 0x0000; }

		else { OCR3A = (short) (800000 / (128 * frequency)) -1; }

		TCNT3 = 0;
		current_frequency = frequency;
	}
	}

	void PWM_on() {

	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);

	}

	void PWM_off() {
        TCCR3A = 0x00;
	TCCR3B = 0x00;
	}


int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char button = 0x00;
	PWM_on();

	while(1) {
		button = ~PINA & 0x07;

	  	if(button == 0x01)  { // Conditional A
                	set_PWM(261.63);
                if(button==0)
		       	break;
        }
        	else if(button == 0x02) { // Conditional B
                	set_PWM(293.66);
                if(button == 0)
		       	break;
        }
        	else if(button == 0x04) { // Conditional C
                	set_PWM(329.63);
                if(button == 0)
		       	break;
        }
        	else set_PWM(0.0); // Default

        }

        return 1;

        }

