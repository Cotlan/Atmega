/*
 * Test03-Tiimer.c
 *
 * Created: 2024-10-11 오후 2:33:39
 * Author : user
 */ 


#include <avr/io.h>

#define DDR DDRG
#define LED PORTG1
#define PORT PORTG

#define	SW1 PORTG4//0
#define SW2 PORTD0//1
#define SW3	PORTD1//2
	
#include <avr/interrupt.h>//sei()함수에 대해 헤더파일 선언

int cnt =0, cnt1=0, cntEx=5 , st=0; //int main 내부에서 바뀌지 않았다

ISR(INT0_vect)//점점 느리게
{
	cntEx++;
}

ISR(INT1_vect)//점점 빠르게
{
	cntEx--;
	if(cntEx<1)
	cntEx=1;
	
}
ISR(TIMER0_OVF_vect)
{
	cnt++;
	if(cnt>=25 ) //(1/16M)*256*256*25 =0.102sec ==100ms
	{	
		cnt=0;
		cnt1++;
		if(cnt1>= cntEx)
		{
			cnt1=0;
			if(st)
			{
				PORT &= ~_BV(LED);
				st=0;
			}
		
			else
			{
				PORT |= _BV(LED);
				st=1;//불이 켜졌다는 status 선언
			}
		}
	}
	
}


int main(void)
{
	DDRG |= _BV(LED); //LED Port 출력 설정
	PORT |= _BV(LED); //LED 키고
	StandBy();
    /* Replace with your application code */
	PORT &= ~_BV(LED); //LED 끔
	//Timer interrupt
	TIMSK |= 0x01; //TOIE
	TCCR0 |= 0x06; //분주비 1024
	//TCCR 레지스터  분주비 설정
	
	EIMSK |=0x03;
	EICRA |=0x0F;
	////  Mask Register : EIMSK
	//EIMSK |= (1<<INT0);
	//EIMSK |= (1<<INT1);
	////EIMSK |=0x03;
	//// Create Register : EICRA
		//
	//EICRA |= (1 << ISC01) | (1 << ISC00);
	//EICRA |= (1 << ISC11) | (1 << ISC10);
	sei();
	
    while (1) 
    {
    }
}

