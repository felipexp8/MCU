;Elaborar um programa que aguarda uma interrupção em um pino IO. Ao disparar a interrupção, 
;aciona o PWM em uma saída. Ao disparar a interrupção novamente, gera o PWM em uma segunda saída.

org 0x03
jmp int1 ; vetor de interrupcao no pino 3.1
jmp 80h ; pule para o Main

org 80h
;setup:
SETB IT2 ; configura interrupcao do pino 3.1
SETB EX0 ; ativa asinterrupcao
MOV R1,0x00 ; registrador para informar quando PMW será usado
MOV R3,0xFF ; contador do PWM para clock de 12 MHZ

;coloque aqui a configuracao dos registradores I/O, no 8051 nao precisa

;main loop
pronto: nop ; fica aguardando eternamente até precionar o pino 3.1
jmp pronto

int1:
CJNE R1, #0, SETPWM1 ; vai para o PWM 2
CJNE R1, #1, SETPWM2 ; vai para o PWM 1

SETPWM1:
MOV R3, 0xFF ; dutycicle de 50%
MOV R1, 0x01
jmp PWM

SETPWM2:
MOV R3, 0x7F ; dutycicle de 
MOV R1, 0x00

PMW:
mov R2, 0xFF

loop: ; subida e descida dos PWMs

	mov P1, 0x01 ; pino subida
	mov R2, R3 ; configura o dutycicle
	loop1:
	INC R2
	CJNE R2, #0, loop1

	mov P1, 0x00 ; pino descida
	mov R2, R3 ; configura o dutycicle
	loop2:
	INC R2
	CJNE R2, #0, loop2:

jmp loop













