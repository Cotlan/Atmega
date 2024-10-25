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
#define LED_Y PORTB4
#define DDR DDRG
#define PORT PORTG

volatile int unsigned period = 500;//기본적으로 signed -> 4byte
volatile int unsigned DutyRate = 50;

int led_y_st = 0;
int cnt =0,cnt_ref=1;
ISR(INT0_vect)
{
	period -= 50;
	if(period<0) period = 500;
}

ISR(INT1_vect)
{
	DutyRate += 10;
	if(DutyRate>90) DutyRate = 10;
}

ISR(INT2_vect)
{
	//cnt_ref--;
	//if(cnt_ref<0) cnt_ref=50;
	OCR0+=10;//기본 주기에서 OCR 값 변경-> dutyrate 변경
}

 
ISR(TIMER0_COMP_vect)//HW PWM
{
	/*
	if(++cnt>cnt_ref)//cnt_ref에 도달하면 점멸 동작 수행 다시 주기 reset
	{
		if(led_y_st)//현재값을 가져와서 출력으로 되어있는 pin 값의 status를 가져옴
		{
			PORT &= ~(1<<LED_Y);
			led_y_st=0;
		}
		else
		{
			PORT |= (1<<LED_Y);
			led_y_st=1;
		}
		cnt=0;
	}
	
	//toggle
	
	TCNT0=0;//OCR 값에 설정된 reference 에 도달할 때마다 reset
	*/
}

int main(void)
{
	EIMSK |= (1<<INT1)|(1<<INT0)|(1<<INT2);
	EICRA |= 0x2A; //3개의 interrupt falling edge 적용 00101010
	DDR |= (1<<LED_R)|(1<<LED_G)|(1<<LED_Y);
	
	//volume이 pwm 의 duty rate와 관련되어 있다
	//period는 음의 높낮이 led는 깜빡임 motor는 
	TIMSK |= (1<<OCIE0);
	TCCR0 |= 0x07; //(1<<CS00)|(1<<CS01)|(1<<CS02) 1024 64us
	TCCR0 |=(1<<WGM01)|(1<<WGM00);// 11:고속 PWM 
	TCCR0 |=(1<<COM01);//10 normal mode 비반전 모드
	OCR0 = 157;//TCNT 255보다 커질 수 없다. 10ms
	
	
	DDRB |=0x10;
	sei();
	
	while (1)
	{
		PORT |= (1<<LED_R)|(1<<LED_G); _delay_ms(period*DutyRate/100); //T_H
		PORT &= ~((1<<LED_R)|(1<<LED_G)); _delay_ms(period*(100-DutyRate)/100); //
	}
}
