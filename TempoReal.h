#include <16F628A.h>

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES RC                    	//Resistor/Capacitor Osc with CLKOUT
#FUSES NOPUT                 	//No Power Up Timer
#FUSES NOPROTECT             	//Code not protected from reading
#FUSES BROWNOUT              	//Reset when brownout detected
#FUSES MCLR                  	//Master Clear pin enabled
#FUSES LVP                   	//Low Voltage Programming on B3(PIC16) or B5(PIC18)
#FUSES NOCPD                 	//No EE protection

#use delay(clock=4000000)
#use rs232(baud=9600,parity=N,xmit=PIN_A3,rcv=PIN_A2,bits=8)

