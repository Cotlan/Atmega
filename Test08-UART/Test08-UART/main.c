/*
 * Test08-UART.c
 *
 * Created: 2024-10-25 오전 9:41:38
 * Author : user
 */ 
#define F_CPU 16000000L//long이다. 
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

void UART0_putc(char c)
{
	while(1)
	{
		if(UCSR0A&(1<<UDRE0))
		break;
	}
	// UDRE0가 enable 이 되어 있는지-> 아니면 무한 대기
	// 다음 데이터를 전송할 준비가 되어 있는지, 
	UDR0 = c; //send 동작
}

void UART0_puts(char *str)
{
	while(*str) UART0_putc(*str++);
	// s가 같은 곳을 가리키다가 0이 아니면
	// UDRE0 가 enable 되면, 한번 송신 동작 하고, 다음 문자를 가리킴
	//O까지 동작을 수행 \0 에서 끝나고 빠져 나옴.
	{
		//while(!(UCSR0A&(1<<UDRE0)));
		
	}
}

void UART0_init()
{
	//UART Register setting1 - Baud Rate: 9600
	
	//Baud Rate register (BRR)
	//UART는 0번은 USB에 1은 PE에 연결(별도의 device)
	//Daemon 과 Device통신
	//UART0의 runtime 으로 사용 -> 해당되는 USB를 RS232(serial file)로 사용
	//COM x 번호로 serial 통신 가능
	//UCSRnA register의 배속 설정  U2Xn 설정 communication status register
	// UART Register setting - 9600 N 8 1 -> 모두 default 설정
	
	UCSR0A = (1<<U2X0);
	UBRR0H = 0;
	UBRR0L = 207;// baud rate 9600 4800에서 2배속 모드
	
	//C register의 4,5번 bit
	UCSR0C &= ~((1<<UPM00)|(1<<UPM01));// Parity : 00 None 10 Even 11 Odd
	UCSR0B &= ~(1<<UCSZ02);
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);//Data bits: 011: 8
	
	//stop bit
	UCSR0C &= ~(1<<USBS0);//stop bit를 0으로 만들어줌  Stop bit 0: 1(default) 1: 2
	
	//해당 포트의 기능 활성화 open
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	
}



char *str = "Button Pushed!!!\r\n";// delay 추가 
//string printf -> 범용 메모리 공간 필요
char buf[256];


int n=0;
ISR(INT0_vect)
{
	sprintf(buf,"Button Pushed %d times\r\n",n++);
	UART0_puts(buf);
}



int main(void)
{
	UART0_init();
	UART0_puts("\033[2J");
	// screen clear -ESC[2J 터미널 화면 전체를 지우는 명령
	// ESC는 제어 시퀀스의 시작을 나타냄, 터미널의 화면을 지우고, 
	//커서를 화면의 첫번째 위치에 놓음
	UART0_puts("\033[0;0H");
	// move cursor to LeftTop -ESC[0;0H
	// H 명령은 커서를 특정 위치로 이동시키는 역할 0;0은 좌표 
	// 좌측상단(0,0)으로 이동 -> 화면을 지운 후 커서를 다시 좌측 상단으로 이동
	//이스케이프 시퀀스는 터미널 에뮬레이터에서 사용
	EIMSK |=(1<<INT0);
	EICRA |=0x02;// (1<<ISC01);//하강 엣지
	sei();
	//순차적으로 하나씩 나가는 직렬 통신 
	//해당하는 송신 bit를 체크하고
	//버퍼가 준비 되어 있을 때 write해야 한다. UDREn
    /* Replace with your application code */
    while (1) 
    {
    }
}

