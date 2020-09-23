# MCU
Exercicios de Microcontroladores

## Exercício
Elaborar um programa que aguarda uma interrupção em um pino IO. Ao disparar a interrupção, aciona o PWM em uma saída. Ao disparar a interrupção novamente, gera o PWM em uma segunda saída.
 
## Exercício 2
Montar um programa com I2C em modo slave em que o microcontrolador responda a dois comandos:
	0x3F = indica a leitura do ADC
	0x5A = retorna ao máster o último valor lido do ADC.
Ao finalizar a leitura do ADC, utilizar um pino de GPIO para informar o máster que a leitura foi concluída (Wake-up), por 10ms.
