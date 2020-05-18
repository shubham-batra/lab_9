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


#define button (~PINA & 0x07)
enum STATES { DEFAULT, PLAYC, PLAYD, PLAYE } state;

void tick() {
	switch(state) { // State Paths
		case DEFAULT:
		switch(button) {
			case 0x01:
			state = PLAYC;
			break;
			case 0x02:
			state = PLAYD;
			break;
			case 0x04:
			state = PLAYE;
			break;
			default:
			state = DEFAULT;
			break;
		}
    break;
		case PLAYC:
		if (button == 0x01) {
			state = PLAYC;
		} else {
			state = DEFAULT;
		}
		break;
		case PLAYD:
		if (button == 0x02) {
			state = PLAYD;
		} else {
			state = DEFAULT;
		}
		break;
		case PLAYE:
		if (button == 0x04) {
			state = PLAYE;
		} else {
			state = DEFAULT;
		}
		break;
	}
	switch(state) { //Actions
		case DEFAULT:
		set_PWM(0);
		break;
		case PLAYC:
		set_PWM(261.63);
		break;
		case PLAYD:
		set_PWM(293.66);
		break;
		case PLAYE:
		set_PWM(329.63);
		break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
	state = DEFAULT;
	while(1) {
		tick();
	}
  return 1;
}
