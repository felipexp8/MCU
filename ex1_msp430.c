#include "msp430gxxxx.h"

#define True 1
#define False 0

int modo = 0

void main(void) 
{
   WDTCTL = WDTPW + WDTHOLD;  
  
   //reset de todos os pinos do port 1:
   P1OUT &= 0x00;
   P1DIR &= 0x00;
  
   P1DIR |= ~0xFE;  //todos os pinos como saida, exceto o 1
    
   P1IE |= 0x01; //habilita interrupcao do pino 1

   P1REN |= BIT1; // pull up interno
   P1IES |= BIT1; //detector de borda de subida
  
   __bis_SR_register(GIE); // Habilita todas as interrupcoes
  
   while(True)
   {}
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
	
	while(True) //PWM 50%
	{
		for(count = 0; count < 1000 ; count++)
		{ 
			P1OUT ^= saida;
		}
		
		saida ^= BIT2; //toggle
		
		for(count = 0; count < 1000 ; count++)
		{ 
			P1OUT ^= saida
		}
	}
   
}
