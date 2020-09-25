/*
 * main.c
 *
 *  Created on: 25/09/2020
 *      Author: Felipe
 */

#include <msp430.h>
#include "util.h"

//ciclo de trabalho em 50%:
#define CICLOUP 1000
#define CICLODOWN 1000

unsigned int modo = 0;

int main(void)
{
   INICIARCLOCKS();

   //reset de todos os pinos do port 1:
   P1OUT &= 0x00;
   P1DIR &= 0x00;

   P1DIR |= ~0xFE;  //todos os pinos como saida, exceto o 1

   P1IE |= 0x01; //habilita interrupcao do pino 1

   P1REN |= BIT1; // pull up interno
   P1IES |= BIT1; //detector de borda de subida

   __bis_SR_register(GIE); // Habilita todas as interrupcoes

	while(TRUE)
	{
		None;
	}

	return 0x00;

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	int count = 0;

	char saida = BIT2;

	P1OUT &= 0x01; //reseta Port_1 com exceção do pino 1 que faz a interrupcao

	modo ^= 1; //togle pwm

	if (modo)
	{
		saida = BIT2;
	}
	else {
		saida = BIT3;
	}

	while(TRUE) //PWM 50%
	{
		for(count = 0; count < CICLOUP ; count++)
		{
			P1OUT ^= saida;
		}

		saida ^= BIT2; //toggle

		for(count = 0; count < CICLODOWN ; count++)
		{
			P1OUT ^= saida;
		}
	}

}

