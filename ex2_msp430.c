#include "msp430gxxxx.h"
 
unsigned int valor =0x00;

void ConfigurarADC(void)
{
	//configura o registrador referente ao Clock do ADC
	//escolhe um canal e o divisor de clock (/4)
	ADC10CTL1 = INCH_1 + ADC10DIV_3 ; 

	ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE; //Tensoes de referencia Vss

	//ajusta o pino de entrada do ADC - geralmente o P1.5
	ADC10AE0 |= BIT5; 
}

void main(void)
{
   //configuracoes iniciais dos clocks do MSP430 e do whatdog
   //isto aqui é um porre mesmo, 
   WDTCTL = WDTPW + WDTHOLD; 
   BCSCTL1 = CALBC1_1MHZ;  
   DCOCTL = CALDCO_1MHZ;
   BCSCTL2 &= ~(DIVS_3); // SMCLK = DCO = 1MHz

   P1SEL |= BIT5; //libera o Port1 do I/O (Apenas o quinto pino)

   ConfigurarADC();

   __enable_interrupt();  

   while(1)
   {
      //tempo de warmaup do ADC somando o tempo do conversao
      __delay_cycles(1000);  

      //inicia a conversao com um set start no registrador
      ADC10CTL0 |= ENC + ADC10SC;

	  //Habita a interrupcao do LMP0
      __bis_SR_register(CPUOFF + GIE); 

      valor = ADC10MEM; //finalmente faz a leitura
   }
}

//Diretiva para ajustar o vetor de interrupcao

#pragma vector=ADC10_VECTOR

//modulo para recuperar apos o sleep mode
__interrupt void ADC10_ISR (void)
{
   __bic_SR_register_on_exit(CPUOFF);

}
