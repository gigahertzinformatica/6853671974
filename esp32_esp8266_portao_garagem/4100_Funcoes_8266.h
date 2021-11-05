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
  
  int contadorConWifi = 0;

  if (WiFi.status() != WL_CONNECTED){
    
    while ((WiFi.status() != WL_CONNECTED) && (contadorConWifi <= 3)){

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

      contadorConWifi++;

    }
    contadorConWifi = 0;
    

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

  if (!clientMqtt.connected()){

    clientMqtt.setServer(mqttServer, mqttPort);   //informa qual broker e porta deve ser conectado
    clientMqtt.setCallback(MqttCallback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
   
    int contadorConMqtt = 0;

    while ((!clientMqtt.connected()) && (contadorConMqtt <= 3)){

      Serial.println();
      Serial.print("ReConnectando ao seu MQTT com nome: ");
      Serial.print(mqttNome);
     
      if (clientMqtt.connect( mqttId, mqttUser, mqttPassword )) {
      
        Serial.println();
        Serial.print("ReConnectado ao seu MQTT com Sucesso!");
        Serial.println();
        delay(1000);
        
        Serial.print("Se escrevendo no Topico: ");
        Serial.print(mqttTopic1);
        Serial.println();
        delay(1000);

        Serial.print("Publicando um teste no Topico: ");
        clientMqtt.publish("Esp32","Portao1");
        clientMqtt.subscribe(mqttTopic1);
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
        clientMqtt.publish("Esp32","Portao2");
        clientMqtt.subscribe(mqttTopic2);
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
        Serial.print(clientMqtt.state());
        delay(2000);

        contadorConMqtt++;
        
      
 
      }
      contadorConMqtt = 0;
      

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
        
    if (!clientMqtt.connected()){
          
          delay(1000);
          Serial.println();
          Serial.print("%%%%%%%%%%% Falha na conexao do Cliente Mqtt  %%%%%%%%%%%");
          delay(2000);
          ReconnectMQTT();
          
    } 
    
    if (clientMqtt.connected()){
    Serial.println();
    Serial.print("Conexão com Wifi e Mqtt OK");
    delay(1000);

   
    }

}
//
//
//
//--- Reconectar Wifi ---
//
void ReconectWiFi(){
  
  int contadorConWifi = 0;

  if (WiFi.status() != WL_CONNECTED){

    while ((WiFi.status() != WL_CONNECTED) && (contadorConWifi <= 3)){

      delay(3000);

      Serial.println();
      Serial.print("%%%%%%%%%%% Tentativa de reconectar na rede: ");
      Serial.print(SSID);
      Serial.print(" %%%%%%%%%%%");
      Serial.println();
      Serial.print("<<==>> ");
      WiFi.reconnect();
      delay(1000);

      contadorConWifi++;

    }
    contadorConWifi = 0;

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

  if (!clientMqtt.connected()){

    clientMqtt.setServer(mqttServer, mqttPort);   //informa qual broker e porta deve ser conectado
    clientMqtt.setCallback(MqttCallback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
   
    int contadorConMqtt = 0;

    while ((!clientMqtt.connected()) && (contadorConMqtt <= 3)){

      Serial.println();
      Serial.print("ReConnectando ao seu MQTT com nome: ");
      Serial.print(mqttNome);
     
      if (clientMqtt.connect( mqttId, mqttUser, mqttPassword )) {
      
        Serial.println();
        Serial.print("ReConnectado ao seu MQTT com Sucesso!");
        Serial.println();
        delay(1000);
        
        Serial.print("Se escrevendo no Topico: ");
        Serial.print(mqttTopic1);
        Serial.println();
        delay(1000);

        Serial.print("Publicando um teste no Topico: ");
        clientMqtt.publish("Esp32","Portao1");
        clientMqtt.subscribe(mqttTopic1);
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
        clientMqtt.publish("Esp32","Portao2");
        clientMqtt.subscribe(mqttTopic2);
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
        Serial.print(clientMqtt.state());
        delay(2000);

        contadorConMqtt++;
        
      
 
      }
      contadorConMqtt = 0;

      

    }

    ReconectWiFi();
  
  }

}
//
//
//
//
//--- Função void MudarRedeWiFi() ---
//
void MudarRedeWiFi(){
  {
    delay(10);
    Serial.println();
    Serial.print("------ Foi solicitado via App, a mudança de Rede Wifi  ------");
    Serial.println();
    Serial.print("Tentando conectar na nova rede: ");
    Serial.println();
    Serial.print("Aguarde");
    delay(10);
    // criei variáveis abaixo com dados falsos justamente pra o WifiManager não encontrar a rede nem senha e assim entrar em modo de nova
    // configuração de rede wifi 
    WiFi.begin(SSID_Automacao, PASSWORD_Automacao); 
    WiFiManager wifiManager;
    wifiManager.autoConnect("AP_Ghic_Automacao", "fdhf2fox");
    Serial.println();
    Serial.print("conectado via automacao");
    if (WiFi.status() != WL_CONNECTED){
          delay(1000);
          Serial.println();
          delay(1000);
          Serial.println();
          Serial.print("%%%%%%%%%%% Tentativa de conectar na Nova rede: ");
          Serial.print(" %%%%%%%%%%%");
          delay(1000);
          Serial.println();
          Serial.print("<<==>> ");
          MudarRedeWiFi();
    }
    Serial.println();
    Serial.print("Conectado com sucesso na rede Wifi: ");
    Serial.print(SSID);
    Serial.println();
    Serial.print("IP obtido via dhcp: ");
    Serial.print(WiFi.localIP());
    delay(1000);
    ReconnectMQTT();
 }
}
//
//
//
//
//--- Função que ler e executa tarefas vindas via Mqtt ---
//
void MqttCallback(char* topic, byte* payload, unsigned int length){

    // for (unsigned int i = 0; i < length; i++) {
     //       Serial.print((char)payload[i]);
            String msg;
            //obtem a string do payload recebido
            for(unsigned int i = 0; i < length; i++) 
                {
                char c = (char)payload[i];
                msg += c;
                }

     Serial.println();
     Serial.println();
     Serial.print("Menssagem recebida no Topico: ");
     Serial.print(topic);
     Serial.println();
     Serial.print("Menssagem =>  ");
     Serial.print(msg);
  
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
//
//
//
//--- Função que usa a funçao SetClock e FirmewareUpdate caso já esteja em tempo de verificar se tem nova versão do firmeware ---
//
void MonitoraNovaVersaoFirmWare(){
    unsigned long currentMillis = millis();
    if ((currentMillis - previousMillis) >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      SetClock();
      AtualizarFirmWare();
    }
    if ((currentMillis - previousMillis_2) >= mini_interval) {
      static int idle_counter=0;
      previousMillis_2 = currentMillis;
      Serial.println();
      Serial.println();   
      Serial.print("====>>>> Versão Atual ====>>>> ");
      delay(2000);
      Serial.print(FirmwareVer);
      Serial.print(" <<<<====");
  delay(1000);
  Serial.println();
  Serial.println();
      Serial.print("AINDA NÃO HÁ VERSÃO NOVA DO FIRMWARE, JÁ FOI VERIFICADO....>> ");
      Serial.print(idle_counter++);
      Serial.print(" <<....VEZES");
  delay(1000);
  Serial.println();
  Serial.println();
     if(idle_counter%2==0)
      digitalWrite(LED_BUILTIN, HIGH);
     else 
      digitalWrite(LED_BUILTIN, LOW);
     if(WiFi.status() == !WL_CONNECTED) 
     InicWiFi();
   }
}
//
//
//
//
//--- Função usada na inicialiação pelo OTA ---
//
void SetClock() {
   // Set time via NTP, as required for x.509 validation
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println();
  Serial.println();
  Serial.print("====>>>> ATUALIZANDO HORA DO ESP <<<<====");

  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  delay(1000);
  Serial.println();
  Serial.println();
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("====>>>> HORA ATUALIZADA PARA: ");
  Serial.print(asctime(&timeinfo));
  
} 
//
//
//
//
//--- Função que verifica se há nova versão de firmware para o Esp ---
//
void AtualizarFirmWare(){  
  WiFiClientSecure clientFirmWare;
  clientFirmWare.setTrustAnchors(&cert);
  if (!clientFirmWare.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }
  clientFirmWare.print(String("GET ") + URL_fw_Version + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
  while (clientFirmWare.connected()) {
    String line = clientFirmWare.readStringUntil('\n');
    if (line == "\r") {
      //Serial.println("Headers received");
      break;
    }
  }
  String payload = clientFirmWare.readStringUntil('\n');
  payload.trim();
  if(payload.equals(FirmwareVer) )
  {   
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.print("====>>>> ESP JÁ ESTÁ COM VERSÃO MAIS ATUAL <<<<====");
  }
  else
  {
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.print("====>>>> NOVA VERSÃO DETECTADA, FIRMWARE SERÁ ATUALIZADO AGORA <<<<====");
  delay(2000);
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW); 
    t_httpUpdate_return ret = ESPhttpUpdate.update(clientFirmWare, URL_fw_Bin);
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("FALHA AO TENTAR ACESSAR REPOSITORIO DO FIRMWARE (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;
      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;
      case HTTP_UPDATE_OK:
        Serial.println("UPDATE OK");
        break;
    } 
  }
}
 
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Fim das Funções                                                          //
//                                                                          //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//
