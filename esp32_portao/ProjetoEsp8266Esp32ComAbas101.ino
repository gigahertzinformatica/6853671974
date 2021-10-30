//////////////////////////////////////////////////////////////////////////////
// ==>> Iniciu do Global                                                    //
//////////////////////////////////////////////////////////////////////////////

//#include <Arduino.h>

#if defined(ESP8266)

  #include "1100_Global_8266.h"
         
#else

  #include "1200_Global_32.h"
   
#endif

//////////////////////////////////////////////////////////////////////////////
// ==>> Fim do Global                                                       //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// ==>> Iniciu do Setup                                                     //
//////////////////////////////////////////////////////////////////////////////

void setup() {

#if defined(ESP8266)

  #include "2100_Setup_8266.h"
         
#else

  #include "2200_Setup_32.h"
   
#endif

}

//////////////////////////////////////////////////////////////////////////////
// ==>> Fim do Setup                                                        //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// ==>> Iniciu do Loop                                                      //
//////////////////////////////////////////////////////////////////////////////

void loop() {

#if defined(ESP8266)

  #include "3100_Loop_8266.h"   
        
#else

  #include "3200_Loop_32.h"
  
#endif

}

//////////////////////////////////////////////////////////////////////////////
// ==>> Fim do Loop                                                         //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// ==>> Iniciu de Funções                                                   //
//////////////////////////////////////////////////////////////////////////////

#if defined(ESP8266)

  #include "4100_Funcoes_8266.h"         
#else

  #include "4200_Funcoes_32.h"
  
#endif

//////////////////////////////////////////////////////////////////////////////
// ==>> Fim de Funções                                                      //
//////////////////////////////////////////////////////////////////////////////

/*
//////////////////////////////////////////////////////////////////////////////
// ==>> INICIU DE DICAS                                                     //
//////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Dica para Regras de Diretivas aplicadas a placas Esp8266 e Esp32 no mesmo sketch

#if defined(ESP8266)

Tarefazs para Esp8266
         
#else

Tarefas para Es32
   
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
// ==>> INICIU Define a relação entre "D" no NodeMcu e "GPIO" no Esp        //
//////////////////////////////////////////////////////////////////////////////
// Abaixo está os indereçamentos das portas informadas no sketch e qual é ela na grafica das placas esp8266

 sketch      PLACA D1        PLACA NODEMCU     PLACA ESP-01
*    0     ->     D8       ->       D3        ->     GP0
*    1     ->   D1 (TX)    ->       TX        ->     TX0
*    2     ->     D9       ->       D4        ->     GP2
*    3     ->   D0 (RX)    ->       RX        ->     RX0
*    4     ->   D4 e D14   ->       D2 
*    5     ->   D3 e D15   ->       D1
*    12    ->   D6 e D12   ->       D6
*    13    ->   D7 e D11   ->       D7
*    14    ->   D5 e D13   ->       D5
*    15    ->     D10      ->       D8
*    16    ->     D2       ->       D0


// Veja abaixo com fica melhor fazer um define pras portas relacionando a porta escrita na grafica da placa com a real porta no 
// esp8266

//PLACA D1

#define D0 3
#define D1 1
#define D2 16
#define D3 5
#define D4 4
#define D5 14
#define D6 12
#define D7 13
#define D8 0
#define D9 2
#define D10 15
#define D11 13
#define D12 12
#define D13 14
#define D14 4
#define D15 5


//PLACA nodeMCU

#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15


//PLACA ESP-01

#define GP0 0
#define TX0 1
#define GP2 2
#define RX0 3


// Obs: Muito importante, vale lembrar que as únicas Gpios que pode ser usado nos
// acionamentos dos portões é 5, 4 e 15 pois são as únicas portas que não tem
// variações em suas saidas na hora que o nodemcu é ligado e assim não irá 
// acionar involuntariamente os portões. Já o botão de configurar nova rede wifi
// pode ser usado em qualquer Gpio, optei pela 16 mas sem motivo importante.


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
// ==>> FIM DE DICAS                                                        //
//////////////////////////////////////////////////////////////////////////////

*/
