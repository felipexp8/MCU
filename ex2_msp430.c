#include "msp430gxxxx.h"

#define True 1
#define False 0

unsigned int valor = 0x00;
char buffersaida = 0xFF;
char bufferentrada = 0xFF;
char TXByteCtr = 1; // contagem dos dados
int Rx = 0
char LeituraDoComando = 0x3F;
char UltimoValorLido = 0x5A;
char ADCLigado = False

//TODO: colocar na norma ANSI

void ConfigurarI2C(unsigned int adresss)
{
  _DINT();
  IE2 |= UCB0RXI                            //Habilita interrupcao RX
  IE2 |= UCB0TXIE;                          //Habilita interrupcao TX
  while (UCB0CTL1 & UCTXSTP)
  {
	  NULL; //aguarde até o os registradores ficarem livres para fazer o SET
  }	   
  UCB0CTL1 |= UCSWRST;                      // SW reset
  UCB0CTL0 = 0 + UCMODE_3 + UCSYNC;         // configura o I2C: somar com o UCMST para master 
											// e 0 para slave
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // usar o clock SMCLK
  UCB0BR0 = 12;                             // divisor de clock: 
  UCB0BR1 = 0;								// fSCL = SMCLK/12
  UCB0I2CSA = adresss;                      // Endereco do Slave, exemplo: 0x61
  UCB0CTL1 &= ~UCSWRST;                     // retira o SW reset
}

void EnviarAssincrono(char saida)
{
	//global buffersaida
	buffersaida = saida
    while (UCB0CTL1 & UCTXSTP) // aguarde para liberar
	{}     				       //  (o barramento que passa pelo UCTXSTP precisa ficar solto
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX: inicia
    __bis_SR_register(CPUOFF + GIE);        // LPM0 com interrupcao
}

void ReceberAssincrono()
{
	UCB0CTL1 &= ~UCTR ;                  	// Limpa o UCTR
	while (UCB0CTL1 & UCTXSTP);             // aguarda para liberar 
	UCB0CTL1 |= UCTXSTT;                    // I2C RX: inicia
	while (UCB0CTL1 & UCTXSTT);             // aguarda oara liberar o bus
	UCB0CTL1 |= UCTXSTP;                    // I2C STOP
	__bis_SR_register(CPUOFF + GIE);        // LPM0 com interrupcao
	
}

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
   //configuracoes iniciais dos clocks do MSP430 e do Watchdog
   //isto aqui é um porre mesmo, 
   WDTCTL = WDTPW + WDTHOLD; 
   BCSCTL1 = CALBC1_1MHZ;  
   DCOCTL = CALDCO_1MHZ;
   BCSCTL2 &= ~(DIVS_3); // SMCLK = DCO = 1MHz

   P1SEL |= BIT5; //libera o Port1 do I/O (Apenas o quinto pino)

   ConfigurarADC();

   __enable_interrupt();  
   
   ReceberAssincrono();
   
   while(True)
   {
	    while(ADCLigado)
		{
			  //tempo de warmaup do ADC somando o tempo do conversao
			  __delay_cycles(1000);  

			  //inicia a conversao com um set start no registrador
			  ADC10CTL0 |= ENC + ADC10SC;

			  //Habita a interrupcao para o modo Low Power
			  __bis_SR_register(CPUOFF + GIE); 

			  valor = ADC10MEM; //finalmente faz a leitura
		}
		//NOPE
   }
}

//Diretivas para ajustar os vetores de interrupcao :

//VETOR DE INTERRUPCAO: modulo para recuperar apos o sleep mode
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
   __bic_SR_register_on_exit(CPUOFF);
}

//VETOR DE INTERRUPCAO: USCI_B0 Data ISR
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	if(Rx == True) //Recebendo dados?
	{                              
	  bufferentrada = UCB0RXBUF;                   //Pega o Char recebido já disponível no register
	  __bic_SR_register_on_exit(CPUOFF);           //LPM0
	  
	  if(bufferentrada == LeituraDoComando) ADCLigado = True;  
	  
	  if(bufferentrada == UltimoValorLido) EnviarAssincrono(valor);
	  
	}

	else //Transmitindo dados:
	{                                       		   
		  if (TXByteCtr)      //Tamanho máximo do payload               		 
		  {
			  UCB0TXBUF = buffersaida;   //envia a char atual             
			  TXByteCtr--;               //decrementa a posicao atual do buffer até zero           
		  }
		  else
		  {
			  UCB0CTL1 |= UCTXSTP;                    // I2C STOP
			  IFG2 &= ~UCB0TXIFG;                     // Limpa flags
			  __bic_SR_register_on_exit(CPUOFF);      // Sai do moto LP
		  }
	}
}
