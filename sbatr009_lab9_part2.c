#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
  static double current_frequency;
  if (frequency != current_frequency) {
    if (!frequency) { TCCR3B &= 0x08; }
    else { TCCR3B |= 0x03; }
    if (frequency < 0.954) { OCR3A = 0xFFFF; }
    else if (frequency > 31250) { OCR3A = 0x0000; }
    else { OCR3A = (short) (8000000 / (128 * frequency)) - 1; }
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
double NOTESARRAY[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25}; // Increment/decrement through int array
enum STATES { OFF, ON, PLAY, UP, SCALEUP, DOWN, SCALEDOWN } state;
unsigned char i;

void tick() {
	switch(state) { // Transitions
		case OFF: // No Sound
		if (button == 0x01) { // Initially off. If pressed, goes to state ON
			state = ON;
		} else {
			state = OFF;
		}
		break;

		case ON:
		if(button == 0x01) {
		 	state = OFF;
		} else {
		state = PLAY;
		}
		break;

		case PLAY:
		switch(button) {
			case 0x01:
			state = OFF;
			break;
			case 0x02: // Scale Up
			state = SCALEUP;
			break;
			case 0x04: // Scale Down
			state = SCALEDOWN;
			break;
			default: // Else, keep playing
			state = PLAY;
			break;
		}
		break;


		case UP:
		if(button == 0x01) {
		state = OFF;
		}
		state = PLAY;
		break;

		case SCALEUP:
		if(button == 0x01) {
		state = OFF;
		}
		else if (button == 0x02) {
			state = SCALEUP;
		} else {
			state = UP;
		}
		break;

		case DOWN:
		if(button == 0x01) {
			state = OFF;
		}
		state = PLAY;
		break;


		case SCALEDOWN:
		if(button == 0x01) {
			state = OFF;
		}
		else if (button == 0x04) {
			state = SCALEDOWN;
		} else {
			state = DOWN;
		}
		break;
	}
	switch(state) { // Actions
		case OFF:
		PWM_off();
		break;
		case ON:
		PWM_on();
		i = 0;
		break;
		case PLAY:
		set_PWM(NOTESARRAY[i]);
		break;
		case UP:
		if (i < 7) { // Go up in array
			i++;
		}
		break;
		case SCALEUP:
		break;
		case DOWN: // Go down in array
		if (i > 0) {
			i--;
		}
		break;
		case SCALEDOWN:
		break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
	state = OFF;
	while(1) {
		tick();
	}
}
