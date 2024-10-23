/*
 * Clockmain.c
 *
 * Created: 2024-10-18 오후 12:27:03
 *  Author: user
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL
#define CDDR DDRB
#define CPORT PORTB
#define IDDR DDRA
#define IPORT PORTA
#include <avr/delay.h>
#include <avr/interrupt.h>
unsigned char img[] = { 0x3F, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6F};
	//0123456789//3F 06 5B 4F 66 6D 7D 27 7F 67

//함수를 만들어 num 변수를 1000, 100, 10, 1 자리수를 추출하여 data[] 배열에 저장하자
char data[5]={1,2,3,4};
volatile int Mode = 0; //0:Ready, 1:up-Count, 2:Stop, ....
volatile int num=0;
volatile int sTick=0;//mode가 2일 때 버튼이 눌러질 당시의 tick값 저장
volatile int tick=0;

int digit(int num)
{
	int n1= num%10;
	int n2=(num/10)%10;
	int n3=(num/100)%10;
	int n4=(num/1000);
	data[3] = n1;
	data[2] = n2;
	data[1] = n3;
	data[0] = n4;
	return 1;
}

ISR(INT0_vect)
{
	
	switch (++Mode)//조건 문에 들어가는 것은 앞에 붙이는 게 좋음
	{
		case 1://Start
			tick=0;//시작됨과 동시에 tick을 0으로
			break;
		case 2: //Stop
			sTick=tick;// tick 값을 sTick 에 저장
			break;
		default: //Mode>2
			Mode = 0;
			tick = sTick = 0;
			break;
		
		
	}
	
}


//4개 중에 8bit 타이머는 0번, 2번
ISR(TIMER0_COMP_vect)//OCR 이용, FND refresh//4ms
{
		tick++;
		TCNT0=0;
	
}

int tcnt=0;
ISR(TIMER2_OVF_vect)//OCR 이용, FND refresh//4ms 마다 인터럽트가 발생// 28.3ms
{
	 tcnt++;
	 if(tcnt > 2)
	 {
		 tcnt = 0;
		 for(int i = 0; i < 4; i++)
		 {
			 CPORT = _BV(i); // 1 << i;
			 if(i==1)
			 {
				 IPORT = ~(img[data[i]]|0x80);
			 }
			 else
			 {
				 IPORT =~img[data[i]];
			 }
			 _delay_ms(4);
		 }
		 
	 }
	
}
//FND에 대한 refres

//4자리 숫자를 10진수를 받아서, 2진 digit으로 변경한 후에 c출력
int main(void)
{
	
	CDDR |= 0x0F;//  하위 4bit 출력으로 설정
	IDDR |= 0xFF;// 8bit 전체를 출력으로 설정
	
	
	CPORT = 0x0F; //모든 자리수 선택
	IPORT = ~0xFF; //전체 세그먼트 ON [8.] 애노드여서 모든 값에 대해 bit not
	
	//Timer0: 100ms stop-watch , timer2: FND refresh 
	TIMSK|=_BV(OCIE0);// 비교 일치 인터럽트  timer1 정의, 파형이 올라갈 때 어디서 자를 것인가?
	TIMSK|=_BV(TOIE2);//TOIE2 0x040 timer 2 정의, FND refresh
	
	TCCR0|= 0x07;// 분주비 110:256, 111:1024, 분주비의 세밀한 정의 곤란
	TCCR2|= 0x04;// (1/16M) * 22 *  1024 * 7 =0.009856
	OCR0=40;
	//timer0의 분주비는 111일대 1024
	//1/16M*223(comparator interrupt)*1024*7(CNT 개수)
	
	//Ext-Intr :Mode change 0:Ready, 1:upCount, 2:Stop, ...
	
	EIMSK |= 0x01; //_BV(PORTD0)
	EICRA |= 0x03; //rising edge
	
	
	
	sei();
	int num =0;
	digit(num);
	//StandBy();

	while(1)
	{
		switch (Mode)
		{
			case 0://Ready
				digit(0);
				break;
			case 1://Up-Count
				digit(tick);			
				break;
			case 2://Stop
				digit(sTick);
				break;
			default:
				break;
		}
		
	}
}
