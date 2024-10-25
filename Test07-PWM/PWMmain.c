/*
 * PWMmain.c
 *
 * Created: 2024-10-23 오전 9:48:26
 *  Author: user
 */ 
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#define LED_R PORTG0
#define LED_G PORTG1
#define LED_Y PORTG2
#define DDR DDRG
#define PORT PORTG

volatile int period = 500;
volatile int DutyRate = 50;

ISR(INT0_vect)
{
	period -= 50;
	if(period<0) period = 500;
}

ISR(INT1_vect)
{
	DutyRate += 10;
	if(DutyRate<90) DutyRate = 10;
}


ISR(INT1_vect)
{
	
}
int main(void)
{
	EIMSK |= (1<<INT1)|(1<<INT0);//|(1<<INT2);
	EICRA |= 0x2A; //3개의 interrupt falling edge 적용 00101010
	DDR |= (1<<LED_R)|(1<<LED_G)|(1<<LED_Y);
	sei();
	while (1)
	{
		PORT |= (1<<LED_R)|(1<<LED_G)|(1<<LED_Y); _delay_ms(period*DutyRate/100); //T_H
		PORT &= ~((1<<LED_R)|(1<<LED_G)|(1<<LED_Y)); _delay_ms(period*(100-DutyRate)/100); //
	}
}