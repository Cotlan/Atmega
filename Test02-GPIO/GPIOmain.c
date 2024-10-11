/*
 * Test02-GPIO.c
 *
 * Created: 2024-10-10 오전 9:36:10
 * Author : user
 */ 
#define __DELAY_BACKWARD_COMPATIBLE__
#define F_CPU 16000000UL
#include <avr/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>//intterupt 헤더파일 sei 

#define	SW1 PORTG4//0
#define SW2 PORTD0//1
#define SW3	PORTD1//2
#define LED1 PORTG1



volatile int ival =1000;//지역변수를 전역변수로 선언,최적화 금지 변수로 선언

ISR(INT0_vect)//점점 느리게
{
		if((PING & _BV(SW2))==0) ival+=500;
}

ISR(INT1_vect)//점점 빠르게 
{
	if((PING & _BV(SW3))==0)
	{
		ival-=500;
		if(ival<10)
		ival=100;
	}
}




int main(void)
{
    /* Replace with your application code */
	
	
	
	DDRG &=~(0x10);//1100 ==>0011 입력 마스크
	DDRG |=0x02;//xx11 출력 마스크
	
	
	DDRD &=~(0x03);// GPIO in, out과 관련된 DDR 그래서 선언하지 않아도 된다!
	PORTG |=_BV(LED1);
	StandBy();
	PORTG &=~_BV(LED1);
	
	//  Mask Register : EIMSK
	EIMSK |= (1<<INT0);
	EIMSK |= (1<<INT1);
	//EIMSK |=0x03;
	// Create Register : EICRA
	
	EICRA |= (1 << ISC01) | (1 << ISC00); 
	EICRA |= (1 << ISC11) | (1 << ISC10); 
	//EICRA |=0x0F;-> 
	//
	//1010이 mask일 때 정상적인 bit 연산이 이루어지지 않는다!
	
	
	sei();
	
	//standby후 버튼이 눌리면 탈출하고 main routine으로 이동
	while (1)
	{
		//if((PING & _BV(SW2))==0) ival+=500;
		//else if((PING & _BV(SW3))==0) 
		//{
			//ival-=500;
			//if(ival<10)
				//ival=10;
		//}
		//버튼을 누르는 시점 타이밍이 딱 맞아야, 
		PORTG |= _BV(LED1); //LED on
		_delay_ms(ival);//  main clock에 종속 compiler가 const값으로 처리해서 변동 불가인 것이 default
		PORTG &= ~_BV(LED1);	//LED off
		_delay_ms(ival);
	}
	
}

