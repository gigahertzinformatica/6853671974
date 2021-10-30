//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Quarta etapa - Aqui são criadas as funções para usar do Esp8266          //
//                                                                          //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Iniciu das Funções                                                       //
//                                                                          //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//
//
//
//
//--- Iniciar Serial ---
//
void InicSerial(){
      Serial.begin(9600);
}
//
//
//
//--- Iniciar Saidas e já com seus Modos de Trabalho ---
//
void InicSaidas(void){
    pinMode(D0, OUTPUT);
    digitalWrite(D0, LOW);
    pinMode(D2, OUTPUT);
    digitalWrite(D2, LOW);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}
//
//
//
//--- Conecta com wifi ---
//
void InicWiFi(){
  
  int contador = 0;

  if (WiFi.status() != WL_CONNECTED){
    
    while ((WiFi.status() != WL_CONNECTED) && (contador <= 3)){

      delay(3000);

      Serial.println();
      Serial.print("%%%%%%%%%%% Tentativa de reconectar na rede: ");
      Serial.print(SSID);
      Serial.print(" %%%%%%%%%%%");
      Serial.println();
      Serial.print("<<==>> ");
      WiFiManager wifiManager;
      //wifiManager.resetSettings();
      //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

      wifiManager.autoConnect("AP_Ghic_Automacao", "fdhf2fox");
      Serial.println();
      Serial.print("conectado via automacao");
      delay(1000);

      contador++;

    }

  } else {

    if (WiFi.status() == WL_CONNECTED){

      Serial.println();
      Serial.print("%%%%%%%%%%% Reconectado na rede: ");
      Serial.print(SSID);
      Serial.print(" Com Sucesso %%%%%%%%%%%");
      Serial.println();
      Serial.print("IP obtido via dhcp: ");
      Serial.print(WiFi.localIP());
      Serial.println();
      delay(1000);
                    
      InicMQTT();

      return;

    }

  }
           

    
}
//
//
//
//--- Conecta com Mqtt ---
//
void InicMQTT(){

  if (!client.connected()){

    client.setServer(mqttServer, mqttPort);   //informa qual broker e porta deve ser conectado
    client.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
   
    int contador = 0;

    while ((!client.connected()) && (contador <= 3)){

      Serial.println();
      Serial.print("ReConnectando ao seu MQTT com nome: ");
      Serial.print(mqttNome);
     
      if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      
        Serial.println();
        Serial.print("ReConnectado ao seu MQTT com Sucesso!");
        Serial.println();
        delay(1000);
        
        Serial.print("Se escrevendo no Topico: ");
        Serial.print(mqttTopic1);
        Serial.println();
        delay(1000);

        Serial.print("Publicando um teste no Topico: ");
        client.publish("Esp32","Portao1");
        client.subscribe(mqttTopic1);
        Serial.println();
        delay(1000);
        
        Serial.println();
        Serial.print("Inscrito no topico: ");
        Serial.print(mqttTopic1);
        Serial.print(" com sucesso!");
        Serial.println();
        delay(1000);
      
        Serial.print("Se escrevendo no Topico: ");
        Serial.print(mqttTopic2);
        Serial.println();
        delay(1000);
       
        Serial.print("Publicando um teste no Topico: ");
        client.publish("Esp32","Portao2");
        client.subscribe(mqttTopic2);
        Serial.println();
        delay(1000);
        
        Serial.println();
        Serial.print("Inscrito no topico: ");
        Serial.print(mqttTopic2);
        Serial.print(" com sucesso!");
        Serial.println();
        delay(1000); 
      
      } else {
        Serial.println(); 
        Serial.print("Falhou ao tentar reconectar-se no Broker Mqtt: ");
        Serial.print(mqttNome);
        Serial.print(client.state());
        delay(2000);

        contador++;
        
      
 
      }

      

    }

    ReconectWiFi();
  
  }


}
//
//
//
//--- Monitorar Conexao Wifi e Mqtt ---
//
void VerificaConexoesWiFIeMQTT(void){

if (WiFi.status() != WL_CONNECTED){
      
          delay(1000);
          Serial.println();
          Serial.print("%%%%%%%%%%% Falha na conexao com Wifi na rede: ");
          Serial.print(SSID);
          Serial.print(" %%%%%%%%%%%");
          delay(2000);
          Serial.println();
          Serial.print("<<==>> ");
          ReconectWiFi();
    }
        
    if (!client.connected()){
          
          delay(1000);
          Serial.println();
          Serial.print("%%%%%%%%%%% Falha na conexao do Cliente Mqtt  %%%%%%%%%%%");
          delay(2000);
          ReconnectMQTT();
          
    } 
    
    if (client.connected()){
    Serial.println();
    Serial.print("Conexão com Wifi e Mqtt OK");
    delay(1000);

    //funcaoHoraAtual();
    //Serial.println();
    //delay(1000);
    
    }

}
//
//
//
//--- Reconectar Wifi ---
//
void ReconectWiFi(){
  
  int contador = 0;

  if (WiFi.status() != WL_CONNECTED){

    while ((WiFi.status() != WL_CONNECTED) && (contador <= 3)){

      delay(3000);

      Serial.println();
      Serial.print("%%%%%%%%%%% Tentativa de reconectar na rede: ");
      Serial.print(SSID);
      Serial.print(" %%%%%%%%%%%");
      Serial.println();
      Serial.print("<<==>> ");
      WiFi.reconnect();
      delay(1000);

      contador++;

    }

  } else {

    if (WiFi.status() == WL_CONNECTED){

      Serial.println();
      Serial.print("%%%%%%%%%%% Reconectado na rede: ");
      Serial.print(SSID);
      Serial.print(" Com Sucesso %%%%%%%%%%%");
      Serial.println();
      Serial.print("IP obtido via dhcp: ");
      Serial.print(WiFi.localIP());
      Serial.println();
      delay(1000);
                    
      ReconnectMQTT();

      return;

    }

  }
           

}
//
//
//
//--- Reconectar Mqtt ---
//
void ReconnectMQTT(){    

  if (!client.connected()){

    client.setServer(mqttServer, mqttPort);   //informa qual broker e porta deve ser conectado
    client.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
   
    int contador = 0;

    while ((!client.connected()) && (contador <= 3)){

      Serial.println();
      Serial.print("ReConnectando ao seu MQTT com nome: ");
      Serial.print(mqttNome);
     
      if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      
        Serial.println();
        Serial.print("ReConnectado ao seu MQTT com Sucesso!");
        Serial.println();
        delay(1000);
        
        Serial.print("Se escrevendo no Topico: ");
        Serial.print(mqttTopic1);
        Serial.println();
        delay(1000);

        Serial.print("Publicando um teste no Topico: ");
        client.publish("Esp32","Portao1");
        client.subscribe(mqttTopic1);
        Serial.println();
        delay(1000);
        
        Serial.println();
        Serial.print("Inscrito no topico: ");
        Serial.print(mqttTopic1);
        Serial.print(" com sucesso!");
        Serial.println();
        delay(1000);
      
        Serial.print("Se escrevendo no Topico: ");
        Serial.print(mqttTopic2);
        Serial.println();
        delay(1000);
       
        Serial.print("Publicando um teste no Topico: ");
        client.publish("Esp32","Portao2");
        client.subscribe(mqttTopic2);
        Serial.println();
        delay(1000);
        
        Serial.println();
        Serial.print("Inscrito no topico: ");
        Serial.print(mqttTopic2);
        Serial.print(" com sucesso!");
        Serial.println();
        delay(1000); 
      
      } else {
        Serial.println(); 
        Serial.print("Falhou ao tentar reconectar-se no Broker Mqtt: ");
        Serial.print(mqttNome);
        Serial.print(client.state());
        delay(2000);

        contador++;
        
      
 
      }

      

    }

    ReconectWiFi();
  
  }

}
//
//
//
//
//--- Função que ler e executa tarefas vindas via Mqtt ---
//
void mqtt_callback(char* topic, byte* payload, unsigned int length){
     Serial.println();
     Serial.println();
     Serial.print("Menssagem recebida no Topico: ");
     Serial.print(topic);
     Serial.println();
     Serial.print("Menssagem =>  ");
     for (unsigned int i = 0; i < length; i++) {
            Serial.print((char)payload[i]);
            String msg;
            //obtem a string do payload recebido
            for(unsigned int i = 0; i < length; i++) 
                {
                char c = (char)payload[i];
                msg += c;
                }
  
                                    // Aqui é executado as tarefas de acordo com o que foi lido na variável msg


                    // D0 é usado com butão para configurar nova rede wifi
                    if (msg.equals("R1"))
                         {
                         digitalWrite(D0, HIGH);
                         delay(300);
                         digitalWrite(D0, LOW);
                         EstadoSaida = '0';
                         }
                   if (msg.equals("R2"))
                         {
                         digitalWrite(D2, HIGH);
                         delay(300);
                         digitalWrite(D2, LOW);
                         EstadoSaida = '0';
                         }

                   if (msg.equals("L0"))
                         {
                         digitalWrite(D0, HIGH);
                         EstadoSaida = '0';
                         }
                   if (msg.equals("D0"))
                         {
                         digitalWrite(D0, LOW);
                         EstadoSaida = '1';
                         }
                   if (msg.equals("L2"))
                         {
                         digitalWrite(D2, HIGH);
                         EstadoSaida = '0';
                         }
                   if (msg.equals("D2"))
                         {
                         digitalWrite(D2, LOW);
                         EstadoSaida = '1';
                         }

         }
         
              Serial.println();
              Serial.print("%%%%%%%%%%%%%%%%%%%%% MENSAGEM RECEBIDA E EXECUTADA COM SUCESSO %%%%%%%%%%%%%%%%%%%%%%");
}
//
//
//
//
//--- Iniciar limpeza de configurações armazenadas da Eprom ---
//
void IniciarLimpezaDeConfiguracoesDaEprom(){
  Serial.println();
  Serial.print("%%%%%%%%%%% Neste momnento está cendo apagados as configuraçoes salvas na EPRON: ");
  ESP.eraseConfig();
  delay(2000);
  ESP.reset();
}
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Fim das Funções                                                          //
//                                                                          //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//
