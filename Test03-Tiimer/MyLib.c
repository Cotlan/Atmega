/*
 * MyLib.c
 *
 * Created: 2024-10-11 오후 2:38:32
 *  Author: user
 */ 

#include <avr/io.h>


int TestBit(char pin, char mask) // input의 경우는 해당하는 register를 value를 던져 주도록!
{
	if((pin & mask) != 0) return 1;//버튼 누르지 않았을 때 1반환
	else return 0; // 버튼 눌렀을때 0반환
	//argument로 주고 받을 때는 주소 값으로 해줘야 한다.
}
//PINx PORTx -> 값을 대입

void StandBy()//PG4 pin으로 프로그램 시작 스위치 연결
{
	DDRG &= ~0x10;  //PG4 : 입력으로 설정
	PORTG |= 0x10; // PG4 : Pull-Up
	//DDRG |= 0x10;  상위 1bit 출력
	//DDRG &= ~0x07;  하위 3개 bit 0으로 만들어서 입력 DDRG &=0xF8
	
	while(!TestBit(PING,0x10));//PING의 초기값이 1임을 가정
	//프로그램이 안정화되기 전에 어느 값인지 모르고 프로그램은 이미 시작됨--> 아직 안정화되기 전에는 1이 아니라 0으로 인식, 입력되는 값이 안정화 되길 기다림
	//pull up 저항이므로 1이 될 때가지 기다림. 무한 루프를 돌아서 1이 되면 무한루프 탈출
	while (1)
	{
		if(!TestBit(PING,0x10)) break; //(1<<LED1) 0x10
		//버튼이 눌러질 때 탈출
	}
	//PING 초기값이 1이되면 standby routine으로
	
}