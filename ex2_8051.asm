; ADC com conversor de Rampa ligado ao FPB e um Comparador de Tensao
; Montar um programa com I2C em modo slave em que o microcontrolador responda a dois comandos:
;	  0x3F = indica a leitura do ADC
;	  0x5A = retorna ao máster o último valor lido do ADC.
; Ao finalizar a leitura do ADC, utilizar um pino de GPIO para informar o máster que a leitura foi concluída (Wake-up), por 10ms.


org 3h
jmp int1 ; vetor de interrupcao no pino 3.1
org 38h 
jmp intIC2 ; vetor de interrupcao para o IC2 (endereco ficticio)
jmp 80h ; pule para o Main

;setup:
SETB IE2 ; configura interrupcao do pino 3.1
MOV R1, 0x00 ; registrador para informar quando PMW será usado
MOV R3, 0xFF ; contador do PWM para clock de 12 MHZ

;coloque aqui a configuracao dos registradores I/O, no 8051 nao precisa

;main loop
pronto: nop ; fica aguardando eternamente até precionar o pino 3.1
jmp pronto

int1:
nop
MOV P3,R3
; fazer o envido de dados I2C aqui

SETPWM1:
MOV R3, 0xFF ; dutycicle de 50%
MOV R1, 0x01
jmp PWM

PMW:
mov R2, 0xFF

loop: ; subida e descida dos PWMs com converso de Rampa para R3

	DEC R3

	mov P1, 0x01 ; pino subida
	mov R2, R3 ; configura o dutycicle
	mov R4, 
	loop1:
	DEC R2
	 
	
	CJNE R4, #0, fimconversao
	CJNE R2, #0, loop1

	mov P1, 0x00 ; pino descida
	mov R2, 0x7E ; configura o dutycicle
	loop2:
	DEC R2
	CJNE R2, #0, loop2:
	
	
CJNE R3, #0, loop

;fim da conversao ; se zerar manda Zero mesmo para o Master
;enviando ...

mov 0x43, 0xA0 ; blablabla: configura registradores do I2C para enviar ao Master
mov 0x44, 0x01 ; blablabla: idem

mov 0x50, R3 ; manda o valor R3 para o buffer de saida I2C

mov 0x43, 0xA1 ; travar o buffer de saida para processar o atual e enviar pelo barramento

intIC2:

mov 0x43, 0xA0
mov R5, 0x44 ; le do buffer de entrada do IC2

SUB R5 0x3F ; se for o valor 0x3F:
jz PWM     ; pular paro PWM

ret
