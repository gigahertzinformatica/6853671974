//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Terceira etapa - Aqui é feito o Loop do sistema:                         //
//                                                                          //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Iniciu do Loop                                                           //
//                                                                          //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//
//
//
//--- Confere se a conexao com wifi e mqtt está ok e caso esteja monitora novas mensagems mqtt---
//
if(((WiFi.status()) != (WL_CONNECTED)) or (!clientMqtt.connected())){
  
  //
//
//
//--- Monitorar Conexoes Fifi e Mqtt ---
//  
        
        Serial.println();
        Serial.print("--------- Falha na conexao -->>   ");
        delay(500);
        VerificaConexoesWiFIeMQTT();
        

} else {

//if (clientMqtt.connected()){
  
      while ((clientMqtt.connected()) && (contadorMqttLoop >= 1)){
        //
        //
        //
        //--- Consulta novas tarefas Mqtt ---
        // 
        {
            clientMqtt.loop();
  
        }
      
        Serial.println();
        Serial.print("--------- Aguardando nova Mensagem no Topico -->>   ");
        contadorMqttLoop--;
        Serial.print(contadorMqttLoop);

        }
        contadorMqttLoop = 200;

        delay(500);
        Serial.println();
        Serial.print("--------- Loop completo -->>   ");

}
//
//
//
//--- Consulta novas Atualizações do Firmware para o Esp ---
//
{
  MonitoraNovaVersaoFirmWare();    
}
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Fim do Loop                                                           //
//                                                                          //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
