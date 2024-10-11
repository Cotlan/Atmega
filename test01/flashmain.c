/*
 * Test01.c
 *
 * Created: 2024-10-02 오후 6:04:40
 * Author : user
 */ 
#define F_CPU 16000000UL//16MHz
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>


void delay_ms_variable(int ms)
{
	while (ms > 0)
	{
		_delay_ms(1);
		ms--;
	}
}
int Check0()
{
	char v= PING & 0x01;

	if(v==0)
		return 1;// Pushed !! 
	else
		return 0;
}

int Check1()
{
	char v= PING & 0x02;
	_delay_ms(50);
	if(v==0)
	return 1;// Pushed !!
	else
	return 0;
}

int Check2()
{
	char v= PING & 0x04;
	_delay_ms(50);
	if(v==0)
	return 1;// Pushed !!
	else
	return 0;	

}

int main(void)
{
	DDRG |= 0x10;//4번 핀을 출력으로
	DDRG &= ~(0x07);//0,1,2번째를 입력으로 --> DDR 레지스터를 통해 입력;/출력 설정
	//SW Port : PG3, input
	//LED Port : PG4, output
    /* Replace with your application code */
   
	char mode0=0;
	//mode = 0 : disable, mode = 1 : active 
	//char v=1;//변수를 while loop 외부에서 선언
	// 스택 메모리 사용증가X, 변수 값 유지
	while(1)
	{
		if(Check0())
		{
			mode0=1;
			break;
		}
	}
	//값을 읽어옴
	int m=300;
	while(1)
	{	
		
		if(mode0 == 1)
		{
			//bit and를 해주는 작업? 0을 넣어주면 됨
			//bit not으로 반전 시켜줌 
			
			PORTG |= 0x10;//bit mask
			_delay_ms(m);
			PORTG &= ~(0x10);
			_delay_ms(m);// 끄고 나서 한 번 더 지연
			//DDG3=1 //bit에 직접 접근, const-> read only ==> 0x10 = 1 의미
			if(Check1())
			{
				m-=50;
				if(m<50);
					m=50;
			}
			else if(Check2())
			{
				m+=50;

			}
			
		}
		//동작하지 않는다....? ->
		//채터링 현상, switch는 전기적인 동작을 일으키긴 하지만 물리적인 기구
		//전기적인 파형이 붙어있다가 떨어졌다가를 반복(bounce) --> 스위치에 디캡을 달아 해결
		//v= PING & 0x08;
		if(Check0()) //pushed--> toggle mode 수정
		{
			if(mode0==1)
				mode0=0;
			else 
				mode0=1;
			_delay_ms(500);
		}
		_delay_ms(100);//delay를 삽입해서 동작 안정화
	}
}

