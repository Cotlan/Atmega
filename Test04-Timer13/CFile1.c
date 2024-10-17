/*
 * CFile1.c
 *
 * Created: 2024-10-16 오전 11:44:40
 *  Author: user
 */ 


#include <avr/io.h>

#define DDR	DDRG
#define PORT PORTG
#define LED0 PORTG0
#define LED1 PORTG1
#define LED2 PORTG2

int main(void)
{
	DDR |=0x07; (_BV(LED0)
}