/*
 * Timermain16.c
 *
 * Created: 2024-10-16 오전 11:48:22
 *  Author: user
 */ 
#define  F_CPU 16000000UL//default 1MHz -> 16MHz 
#include <avr/io.h>
#include <avr/interrupt.h>


#define DDR DDRG
#define PORT PORTG
#define LED0 PORTG0
#define LED1 PORTG1
#define LED2 PORTG2

int st3=0, st1=0;
ISR(TIMER1_OVF_vect)
{
	if(st1)
	{
		st1=0;
		PORT |= _BV(LED0);
	}
	else
	{
		st1=1;
		PORT &= ~_BV(LED0);
	}
}
ISR(TIMER3_OVF_vect)
{
	if(st3)
	{
		st3=0;
		PORT |= _BV(LED1);
	}
	else
	{
		st3=1;
		PORT &= ~_BV(LED1);
	}
}
int main(void)
{
	DDR |= 0x07; //DDR|=_BV(LED0)
	
	TIMSK |=_BV(TOIE1);//0x04: 0100b , CCR 1번 타이머의 OVF 인터럽트
	ETIMSK |=_BV(TOIE3);//0x04: 0100b , CCR 3번 타이머의 OVF 인터럽트
	TCCR1B |= 0x03; //(1/16M)*(분주비 )*65535(counter의 max값)=OVF Timer 인터럽트 주기
	TCCR3B |= 0x03; //(1/16M)*(분주비 )*65535(counter의 max값)=OVF Timer 인터럽트 주기
	//분주비가 64 -~250ms 
	sei();
	
	while(1)
	{
		
	}
	
}