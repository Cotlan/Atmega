/*
 * FNDmain.c
 *
 * Created: 2024-10-16 오후 3:17:26
 *  Author: user
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#define CDDR DDRB
#define CPORT PORTB
#define IDDR DDRA
#define IPORT PORTA
#include <avr/delay.h>
unsigned char img[] = { 0x3F, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6F};
	//0123456789//3F 06 5B 4F 66 6D 7D 27 7F 67
	
//4자리 숫자를 10진수를 받아서, 2진 digit으로 변경한 후에 c출력
int in =7575;
int main(void)
{
	StandBy();
	CDDR |= 0x0F;//  하위 4bit 출력으로 설정
	IDDR |= 0xFF;// 8bit 전체를 출력으로 설정
	
	
	CPORT = 0x0F; //모든 자리수 선택
	IPORT = ~0xFF; //전체 세그먼트 ON [8.] 애노드여서 모든 값에 대해 bit not
	int data[3];
	int in1=in;
	/*
	for (int i=0;i<4;i++)
	{
		
		data[3-i]=(in%10);
		in=in/10;
	}
	
	while(1)
	{
		for(int i=0; i<4; i++)
		{
			
			
				CPORT = _BV(i);// 1<<ibit set 하나의 bit만 1로 나머지는 모두 0으로
				//for(int j=0; j<10; j++)
				
				IPORT = ~img[data[i]];
				_delay_ms(5);//잔상효과
				
			
		}
		
		// 차례대로 데이터를 받아 처리
	}
	*/
	
	/*
	while(1)
	{
		
				for(int j=0; j<4; j++)
				{
					if(j==0)
					{
						data[j]=(in1%10);
						CPORT = _BV(3-j);// 1<<ibit set 하나의 bit만 1로 나머지는 모두 0으로
							//for(int j=0; j<10; j++)
			
						IPORT = ~img[data[j]];
						_delay_ms(1);//잔상효과
					}
					else if(j==1)
					{
						data[j]=((in1%100)/10);
						CPORT = _BV(3-j);// 1<<ibit set 하나의 bit만 1로 나머지는 모두 0으로
						//for(int j=0; j<10; j++)
						
						IPORT = ~img[data[j]];
						_delay_ms(1);//잔상효과
					}
					else if(j==2)
					{
						data[j]=((in1%1000)/100);
						CPORT = _BV(3-j);// 1<<ibit set 하나의 bit만 1로 나머지는 모두 0으로
						//for(int j=0; j<10; j++)
						
						IPORT = ~img[data[j]];
						_delay_ms(1);//잔상효과
					}
					else if(j==3)
					{
						data[j]=(in1/1000);
						CPORT = _BV(3-j);// 1<<ibit set 하나의 bit만 1로 나머지는 모두 0으로
						//for(int j=0; j<10; j++)
						
						IPORT = ~img[data[j]];
						_delay_ms(1);//잔상효과
					}
				
				}
			// 차례대로 데이터를 받아 처리
	
	}
	*/
	while(1)
	{
		for(int i=0; i<10000;i++)
		{
			data[0]=i/1000;
			data[1]=(i%1000)/100;
			data[2]=(i%100)/10;
			data[3]=i%10;
			for (int j=0;j<4;j++)
			{
				CPORT = _BV(j);// 1<<ibit set 하나의 bit만 1로 나머지는 모두 0으로
				//for(int j=0; j<10; j++)
				
				IPORT = ~img[data[j]];
				_delay_ms(5);//잔상효과
			}
		}
	}
	//3F 06 5B 4F 66 6D 7D 27 7F 67
}
