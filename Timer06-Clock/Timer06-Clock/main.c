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
 //0:Ready, 1:up-Count, 2:Stop, ....
volatile int num=0;
volatile int sTick=0;//mode가 2일 때 버튼이 눌러질 당시의 tick값 저장
volatile int tick=0;

int digit(int num)
{
	int n4=(num/60/10);
	int n3=(num/60)%10;
	int n2=(num%60)/10;
	int n1=(num%60)%10;
	data[3] = n1;
	data[2] = n2;
	data[1] = n3;
	data[0] = n4;
	return 1;
}

volatile int Mode = 0;
ISR(INT0_vect)
{
	if(Mode==0)
	{
		Mode++;	
	}
	else if(Mode==1)
	{
		Mode++;	
	}
	else if(Mode==3)
	{
		Mode++;
	}
	else if(Mode==4)
	{
		Mode++;
	}

}
int volatile sec=0;


ISR(INT1_vect)
{
	if(Mode==1)
	{
		num+=60;
		if(num>59*60)
		{
			num=0;
		}
	}
	else if(Mode==4)
	{
		sec+=1;
	}
	else if(Mode==5)
	{
		Mode++;
	}
	else if(Mode==7)
	{
		Mode++;
	}
	
	
}

ISR(INT2_vect)
{	
	if(Mode==2)
	{
		Mode=3;
	}
	else if(Mode==3)
	{
		Mode=2;
	}

}

int CNT=0;
//4개 중에 8bit 타이머는 0번, 2번
ISR(TIMER0_COMP_vect)//OCR 이용, FND refresh//4ms
{
	if(Mode==2)//시계 모드일 때 증가
	{
		CNT++;
		if(CNT>50)
		{
			CNT=0;
			tick++;
		}
		
	}
	TCNT0=0;
}

int CNT1=0;

volatile tick2=0;

ISR(TIMER1_COMPA_vect)//OCR 이용, FND refresh//4ms
{
	
		if(Mode==6)//BTN2를 누르면 타이머 START
		{
			CNT1++;
			if(CNT1>50)
			{
				CNT1=0;
				tick2--;
			}
			
		}
		TCNT1=0;
	
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
/*
ISR(INT2_vect)
{
	Mode
}
*/
//FND에 대한 refres
int num1=0;
int sec2=0;
//4자리 숫자를 10진수를 받아서, 2진 digit으로 변경한 후에 c출력
int main(void)
{
	DDRG |= 0x01;
	CDDR |= 0x0F;//  하위 4bit 출력으로 설정
	IDDR |= 0xFF;// 8bit 전체를 출력으로 설정
	
	
	CPORT = 0x0F; //모든 자리수 선택
	IPORT = ~0xFF; //전체 세그먼트 ON [8.] 애노드여서 모든 값에 대해 bit not
	
	//Timer0: 100ms stop-watch , timer2: FND refresh
	TIMSK|=_BV(OCIE0);// 비교 일치 인터럽트  timer1 정의, 파형이 올라갈 때 어디서 자를 것인가?
	TIMSK|=_BV(TOIE2);//TOIE2 0x040 timer 2 정의, FND refresh
	TIMSK|=_BV(OCIE1A);//비교 일치 인터럽트 허용
	
	
	TCCR0|= 0x07;// 분주비 110:256, 111:1024, 분주비의 세밀한 정의 곤란
	TCCR2|= 0x04;// (1/16M) * 22 *  1024 * 7 =0.009856
	TCCR1B |= _BV(CS12)|_BV(CS10);//분주비 256으로설정
	
	OCR0=125;
	OCR1A =125;
	//timer0의 분주비는 111일대 1024
	//1/16M*223(comparator interrupt)*1024*7(CNT 개수)
	
	//Ext-Intr :Mode change 0:Ready, 1:upCount, 2:Stop, ...
	
	EIMSK |= _BV(INT0)|_BV(INT1)|_BV(INT2);//_BV(INT0)|_BV(INT1); //_BV(PORTD0)
	EICRA =  0x2A; //ISC21,ISC11,ISC01
	
	
	sei();
	digit(num);
	//StandBy();

	while(1)
	{
		num1= num+tick;
		sec2= sec+tick2;
		switch (Mode)
		{
			case 0://BTN1을 이용하여 시계설정 모드 진입
				digit(0);
				break;
			case 1://BTN2를 이용하여 분 설정  up Count
				digit(num);
				break;
			case 2://BTN3를 이용하여 시계- 타이머 모드 전환
				digit(num1);
				break;
			case 3://BTN1을 이용하여 타이머 설정 모드 진입
				digit(sec);
				break;
			case 4://BTN2를 이용하여 초단위 설정(up-Count)
				digit(sec);
				break;
			case 5://BTN1을 다시 눌러 타이머 설정 완료
				digit(sec);
				break;
			case 6://BTN2를 누르면 타이머 START
				digit(sec2);
				if(sec2==0)//설정 시간에 도달하면 타이머 정지 & BUZZER 울림
				{
					Mode++;
				}
				break;
			case 7:
				PORTG|=0x01;
				digit(0);
				break;
			case 8://BTN2를 눌러 BUZZER & Count reset
				digit(0);
				sec=0;
				tick2=0;
				PORTG&=~0x01;
				break;
				
		}
		
	}
}


