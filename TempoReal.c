/*############################################################################
  #                     Emula��o de PLC em PIC                               #
  #                    Programa��o em Tempo Real                             #
  #                     Alexandre A. S. Lopes                                #
  #                      Alexandre O. Santos                                 #
  #                5� ano engenharia da computa��o - FACIT                   #  
  #                    Prof. Renato Dourado Maia                             #   
  ############################################################################*/

//------------------------------------------------------------------------------
#include <16F628A.h>
#include <stdlibm.h>
//#include <math.h>


#FUSES NOWDT                    //No Watch Dog Timer
#FUSES INTRC_IO                    //Internal RC Osc
#FUSES NOPUT                    //No Power Up Timer
#FUSES NOPROTECT                //Code not protected from reading
#FUSES BROWNOUT                 //Reset when brownout detected
#FUSES NOMCLR                     //Master Clear pin enabled
#FUSES LVP                      //Low Voltage Programming on B3(PIC16) or B5(PIC18)
#FUSES NOCPD                    //No EE protection

#use delay(clock=4000000)

#use rs232(baud=9600,parity=N,xmit=PIN_B2,rcv=PIN_B1,bits=8)

//--Defini��es------------------------------------------------------------------
#define LIGADO 0xFF
#define DESLIGADO 0x00


#define SAIDA_0    PIN_A0
#define SAIDA_1    PIN_A1
#define SAIDA_2    PIN_A2
#define SAIDA_3    PIN_A3

#define ENTRADA_0    PIN_A4
#define ENTRADA_1    PIN_A5
#define ENTRADA_2    PIN_A6
#define ENTRADA_3    PIN_A7

#define NUM_SAIDA_D  4 
#define NUM_ENT_D    4 
#define NUM_ENT_A    4 


//--Declara��o de Tipos e Vari�veis---------------------------------------------

typedef struct _Porta {
     int Valor;
     int Id;
} Porta;


typedef struct _StatusEntradas {
     Porta EntD[NUM_ENT_D];
     Porta EntA[NUM_ENT_A];
} StatusEntradasD;

typedef struct _StatusSaidas {
     Porta SaidaD[NUM_SAIDA_D];
} StatusSaidasD;

typedef struct _StatusPortas {
     StatusSaidasD Saidas;
     StatusEntradasD Entradas;
     
}StatusPortas;

StatusPortas Portas;

//--Declara��o de Fun��es-------------------------------------------------------
void Inicializar();
void Pisca();
void AtualizarSaidasD();
void AtualizarEntradasD();
void AtualizarEntradasA();
void EnviarSaidasD();
void EnviarEntradasD();
void EnviarEntradasA();
void Espera();
void SetarSaidaDigital(char*Str);
char PegarChar();
char * PegarStr(); 
void LimparBuffer();
//------------------------------------------------------------------------------
void main()
{
   Inicializar();


   for(;;)
   {
    AtualizarSaidasD();
    AtualizarEntradasD();
    //AtualizarEntradasA();
    
    EnviarSaidasD();
    EnviarEntradasD();
    //EnviarEntradasA();
    
    Pisca();
    Espera();
    
    enable_interrupts(INT_RDA);
    enable_interrupts(GLOBAL);
   }

}
//------------------------------------------------------------------------------
void Inicializar()
{
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   SET_TRIS_A(0b11110000);
   SET_TRIS_B(0b11111111);
   
   Portas.Saidas.SaidaD[0].Valor = DESLIGADO;
   Portas.Saidas.SaidaD[1].Valor = DESLIGADO;
   Portas.Saidas.SaidaD[2].Valor = DESLIGADO;
   Portas.Saidas.SaidaD[3].Valor = DESLIGADO;
   
   Portas.Saidas.SaidaD[0].Id = SAIDA_0;
   Portas.Saidas.SaidaD[1].Id = SAIDA_1;
   Portas.Saidas.SaidaD[2].Id = SAIDA_2;
   Portas.Saidas.SaidaD[3].Id = SAIDA_3;
   
   
   Portas.Entradas.EntD[0].Valor = DESLIGADO;
   Portas.Entradas.EntD[1].Valor = DESLIGADO;
   Portas.Entradas.EntD[2].Valor = DESLIGADO;
   Portas.Entradas.EntD[3].Valor = DESLIGADO;
   
   Portas.Entradas.EntD[0].Id = ENTRADA_0; 
   Portas.Entradas.EntD[1].Id = ENTRADA_1;
   Portas.Entradas.EntD[2].Id = ENTRADA_2;
   Portas.Entradas.EntD[3].Id = ENTRADA_3;
   
   Portas.Entradas.EntA[0].Valor = 0;
   Portas.Entradas.EntA[1].Valor = 75;
   Portas.Entradas.EntA[2].Valor = 150;
   Portas.Entradas.EntA[3].Valor = 225;
   
   Portas.Entradas.EntA[0].Id = 0; 
   Portas.Entradas.EntA[1].Id = 0;
   Portas.Entradas.EntA[2].Id = 0;
   Portas.Entradas.EntA[3].Id = 0;
}
//------------------------------------------------------------------------------
void Pisca()
{
static int Liga=0;

    if(Liga)
    {
     OUTPUT_HIGH(PIN_B0);
     Liga=0;
    }
    else
    {
     OUTPUT_LOW(PIN_B0);
     Liga=1;
    }

}
//------------------------------------------------------------------------------
void Espera()
{
 int x;
 int y;
 
 for(x=0;x<255;x++)
  for(y=0;y<220;y++)
 {
 

 }
}
//------------------------------------------------------------------------------
void AtualizarSaidasD()
{
int x=0;

 for(x=0;x<NUM_SAIDA_D;x++)
  if(Portas.Saidas.SaidaD[x].Valor == LIGADO)
   OUTPUT_HIGH(Portas.Saidas.SaidaD[x].Id);
  else
   OUTPUT_LOW(Portas.Saidas.SaidaD[x].Id);
   
  
}
//------------------------------------------------------------------------------
void AtualizarEntradasD()
{
int x=0;

 for(x=0;x<NUM_ENT_D;x++)
  if(!INPUT(Portas.Entradas.EntD[x].Id))
   Portas.Entradas.EntD[x].Valor=LIGADO;
  else 
   Portas.Entradas.EntD[x].Valor=DESLIGADO;
}
//------------------------------------------------------------------------------
void AtualizarEntradasA()
{
/*int x;
 
 for(x=0;x<NUM_ENT_A;x++)
  Portas.Entradas.EntA[x].Valor++;   
*/ 
}
//------------------------------------------------------------------------------
void EnviarSaidasD()
{
 int x;
 
 printf("SD");
 
 for(x=0;x<NUM_SAIDA_D;x++)
  if(Portas.Saidas.SaidaD[x].Valor == LIGADO)
   putc('1');
  else 
   putc('0');
  
  
  printf("%c%c",13,10);

}
//------------------------------------------------------------------------------
void EnviarEntradasD()
{
int x; 
 
 printf("ED");
 
 
 for(x=0;x<NUM_ENT_D;x++)
  if(Portas.Entradas.EntD[x].Valor == LIGADO)
   putc('1');
  else 
   putc('0');
 
 printf("%c%c",13,10);
  
  
}
//------------------------------------------------------------------------------
void EnviarEntradasA()
{
int x; 
 /*
 printf("EA");
 
 
 for(x=0;x<NUM_ENT_A;x++)
  putc(sin(Portas.Entradas.EntA[x].Valor));
   
 
 printf("%c%c",13,10);*/
}
//------------------------------------------------------------------------------
#int_RDA
void ReceberComando()
{
 char * Str;
 Str=PegarStr();
 //printf("Str: %s",Str);
 
 //CMDS0T
 
 if(Str[0]=='C' && Str[1]=='M' && Str[2]=='D')
 {
  switch(Str[3])
  {
   case 'S':
    SetarSaidaDigital(Str);
   break;
  
   default:
  
   break;
  }
 }
 
 free(Str); 
 
}
//------------------------------------------------------------------------------
void SetarSaidaDigital(char *Str)
{
int x;
 for(x=0;x<NUM_SAIDA_D;x++)
  if(x==(Str[4]-48)) 
   if(Str[5]=='F')
    Portas.Saidas.SaidaD[x].Valor = DESLIGADO;
   else 
    Portas.Saidas.SaidaD[x].Valor = LIGADO;
  
}
//------------------------------------------------------------------------------
char PegarChar()
{
int32 x;
 for(x=0;x<30000;x++)
 if(KBHIT())
   return getc(); 
 
 return 255;
}
//------------------------------------------------------------------------------
char * PegarStr()
{
char *Str;
char Temp;
int x;

  Str = malloc (15);
  
  if(Str == NULL)
  {
   printf("ERRO Ao alocar memoria");  
   #asm
    GOTO 0x00
   #endasm
  }

 Str[15]='\0';
 for(x=0;x<14;x++)
 {
  Temp=PegarChar();
  
  if(Temp == 255)
  {
   Str[0]='\0';
   break;
  }
  if(x)
   if(Temp == 10 && Str[x-1] == 13)
   {
    Str[x]=Temp;
    Str[x+1]='\0';
    break;
   }
  
  Str[x] = Temp;
 }
 
 LimparBuffer();
 return Str;
}
//------------------------------------------------------------------------------
void LimparBuffer()
{
int x;
char Temp;
 for(x=0;x<150;)
 {
  if(KBHIT())
  {
   x=0;
   Temp=getc();
  }
  else
  {
   x++;
  }
  
 }
}
//------------------------------------------------------------------------------
