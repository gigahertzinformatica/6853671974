//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Segunda etapa - Aqui é feito o Setup inicial do sistema:                 //
//                                                                          //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//
//////////////////////////////////////////////////////////////////////////////
// ==>> Iniciu de funções a serem executadas no Setup antes do Loop         //
//////////////////////////////////////////////////////////////////////////////
//
//
//
//--- Inicializaçao no Setup Comum ao Esp32 e Esp8266 ---
//
InicSerial();
InicWiFi();
InicSaidas();
InicMQTT();
//
//
//
//--- Função que atualiza a hora ---
//
SetClock();
//
//
//
//--- Função que limpa todos os dados armazenados na eprom ---
//
//IniciarLimpezaDeConfiguracoesDaEprom();
//
//////////////////////////////////////////////////////////////////////////////
// ==>> Fim de funções a serem executadas no Setup antes do Loop            //
//////////////////////////////////////////////////////////////////////////////
//
