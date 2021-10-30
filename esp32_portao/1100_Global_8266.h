//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Primeira etapa - Aqui são declaradas:                                    //
// Includes, definições e Funções                                           //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//
//////////////////////////////////////////////////////////////////////////////
// ==>> INICIU dos includes a ser usados no programa                        //
//////////////////////////////////////////////////////////////////////////////
//
//
//
//
//--- Usados no OTA ---
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecure.h>
#include <CertStoreBearSSL.h>
BearSSL::CertStore certStore;
#include <time.h>
//
//
//
//--- Para serviços que usam serviços de rede de internet ---
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//
//
//
//
//--- Bibliotecas comun pra Esp8266 e Esp32 ---
//
#include <WiFiManager.h>    // para uso no WifiManager
#include <PubSubClient.h>   // para uso no Mqtt
#include <Wire.h>           // para uso no serviço I2c
#include <EEPROM.h>         // para uso no Gerenciamento de Arquivos
#include <FS.h>             // para uso no Gerenciamento de Arquivos
#include <ArduinoJson.h>    // para uso no Gerenciamento de Arquivos
//
//////////////////////////////////////////////////////////////////////////////
// ==>> Fim dos includes a ser usados no programa                           //
//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
// ==>> Iniciu de Mapeamentos de portas                                     //
//////////////////////////////////////////////////////////////////////////////
//
//
//
//
//--- Mapeamentos de Portas do Esp01 ---
//
#define D0 0
#define D1 1
#define D2 2
#define D3 3
//
//////////////////////////////////////////////////////////////////////////////
// ==>> Fim de Mapeamentos de portas                                        //
//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
// ==>> Iniciu de Variaves                                                  //
//////////////////////////////////////////////////////////////////////////////
//
//
//
//
//--- variaveis usadas na OTA ---
//
const String FirmwareVer={"1.08"}; 
//
#define URL_fw_Version "https://raw.githubusercontent.com/gigahertzinformatica/6853671974/main/esp32_portao/bin_version.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/gigahertzinformatica/6853671974/main/esp32_portao/fw.bin"
//
const char* host = "raw.githubusercontent.com";
const int httpsPort = 443;
//
// DigiCert High Assurance EV Root CA
const char trustRoot[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j
ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL
MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3
LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug
RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm
+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW
PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM
xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB
Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3
hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg
EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF
MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA
FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec
nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z
eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF
hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2
Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe
vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep
+OkuE6N36B9K
-----END CERTIFICATE-----
)EOF";
X509List cert(trustRoot);
//
extern const unsigned char caCert[] PROGMEM;
extern const unsigned int caCertLen;
//
//
//
//
//--- Variáveis usadas para armazena nome e senha de wifi ---
//
const char* SSID = "Gigahertz.Net_988401511";   
const char* PASSWORD =  "fdhf2fox5";
//
// criei variáveis abaixo com dados falsos justamente pra o WifiManager não encontrar a rede nem senha e assim entrar em modo de nova
// configuração de rede wifi 
const char* SSID_Automacao = "SSIDErradaDeProposito";   
const char* PASSWORD_Automacao =  "SenhaErradaDeProposito";
//
//
//
//
//--- Variáveis de conexão MQTT ---
//
const char* mqttNome =       "Servidor Mosquitto";                  // Aqui dar um nome ao servidor mqtt 
const char* mqttServer =     "172.31.255.30";                        // Aqui o endereço do seu servidor do Broker 
const int mqttPort =          1883;                                 // Aqui mude para sua porta do Broker
const char* mqttUser =       "Esp32PortaoGaragem";                       // Aqui o nome de usuario do Broker
const char* mqttPassword =   "fdhf2fox";                            // Aqui sua senha fornecida do Broker
const char* mqttTopic1 = "Casa/Esp32Portao1Garagem";  // Aqui sua do toopico que o Esp ira escutar
const char* mqttTopic2 = "Casa/Esp32Portao2Garagem";  // Aqui sua do toopico que o Esp ira escutar
char EstadoSaida = '0';  
WiFiClient espClient;
PubSubClient client(espClient);
//
//
//
//
//--- Variaveis usadas na função  void repeatedCall() ---
//
unsigned long previousMillis_2 = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 60000;
const long mini_interval = 1000;
//
//////////////////////////////////////////////////////////////////////////////
// ==>> Fim de Variaves                                                     //
//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
// ==>> Iniciu Declaração (Prototipargem) de Funções                        //
//////////////////////////////////////////////////////////////////////////////
//
//
//
//
//--- Declaração (Prototipargem) ---
//
void InicSaidas(void);
void InicSerial();
void IniciarLimpezaDeConfiguracoesDaEprom();
void InicWiFi();
void InicMQTT();
void ReconectWiFi(); 
void ReconnectMQTT();
void VerificaConexoesWiFIeMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
//
//////////////////////////////////////////////////////////////////////////////
// ==>> Fim Declaração (Prototipargem) de Funções                           //
//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
// ==>> Iniciu de funçoes na Inicialização                                  //
//////////////////////////////////////////////////////////////////////////////
//
//
//
//
//--- Funçoes usadas na inicialiação pelo OTA ---
//
void setClock() {
   // Set time via NTP, as required for x.509 validation
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  /*
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
  */
}
//
void FirmwareUpdate(){  
  WiFiClientSecure client;
  client.setTrustAnchors(&cert);
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }
  client.print(String("GET ") + URL_fw_Version + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      //Serial.println("Headers received");
      break;
    }
  }
  String payload = client.readStringUntil('\n');
  payload.trim();
  if(payload.equals(FirmwareVer) )
  {   
     Serial.println("Device already on latest firmware version"); 
  }
  else
  {
    Serial.println("New firmware detected");
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW); 
    t_httpUpdate_return ret = ESPhttpUpdate.update(client, URL_fw_Bin);
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;
      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;
      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    } 
  }
}  
//
//void connect_wifi();
void repeatedCall(){
    unsigned long currentMillis = millis();
    if ((currentMillis - previousMillis) >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      setClock();
      FirmwareUpdate();
    }
    if ((currentMillis - previousMillis_2) >= mini_interval) {
      static int idle_counter=0;
      previousMillis_2 = currentMillis;    
      Serial.print(" Active fw version:");
      Serial.println(FirmwareVer);
      Serial.print("Idle Loop....");
      Serial.println(idle_counter++);
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
//////////////////////////////////////////////////////////////////////////////
// ==>> Fim de funçoes na Inicialização                                     //
//////////////////////////////////////////////////////////////////////////////
//
