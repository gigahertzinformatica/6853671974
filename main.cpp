#include <Arduino.h>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Dicas inicias                                                            //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

/*

 Dica de Referencias para uso do chip i2c pcf8574p

Pino do Esp32 os pinos i2c são 
D21 SDA
D22 SCL

Pinos no chip i2c pcf8574p

( A0, A1 e A2 ) serve para referenciar no programa segunto tabela abaixo
ex: se ligar A0, A1 e A2 no negativo então a referencia no programa é 32. Tabela abaixo
    
 
 32-> A0=0 A1=0 A2=0 
 33-> A0=1 A1=0 A2=0   
 34-> A0=0 A1=1 A2=0
 35-> A0=1 A1=1 A2=0 
 36-> A0=0 A1=0 A2=1
 37-> A0=1 A1=0 A2=1
 38-> A0=0 A1=1 A2=1
 39-> A0=1 A1=1 A2=1

( A3, A4, A5, A6, A8, A9, A10, A11 ) podem ser usadas com Input ou Output

( A7 ) é o Negativo pra ligar o Chip

( A12 ) Não liga em nada

( A13, A14 ) Atenção: A14 (SDA) liga no D21 que é o SDA do Esp32 e A13 (SCL)liga no D22 que é o SCL do Esp32

( A15 ) é o Positivo para ligar o Chip


//////////////////////////////////////////////////////////////////////////////
// Define a relação entre "D" no NodeMcu e "D" no Esp                       //
//////////////////////////////////////////////////////////////////////////////

#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15
#define D9    3
#define D10   1




*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Fim de Dicas inicias                                                     //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// 1ª etapa - Aqui serão declardos Incluede, Funções ...                    //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Inclusão de Bibliotecas                                                  //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Includes Chip_I2C_PCF8574P inclusão de Biblioteca ---
//

#include <Wire.h>

//
//
//
// --- Fim includes Chip_I2C_PCF8574P inclusão de Biblioteca ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Includes necessário para o wifimanager ---
//

#if defined(ESP8266)
#include <ESP8266WiFi.h>  //ESP8266 Core WiFi Library         
#else
#include <WiFi.h>      //ESP32 Core WiFi Library    
#endif

#if defined(ESP8266)
#include <ESP8266WebServer.h> //Local WebServer used to serve the configuration portal
#else
#include <WebServer.h> //Local WebServer used to serve the configuration portal ( https://github.com/zhouhan0126/WebServer-esp32 )
#endif

#include <DNSServer.h> //Local DNS Server used for redirecting all requests to the configuration portal ( https://github.com/zhouhan0126/DNSServer---esp32 )
#include <WiFiManager.h>   // WiFi Configuration Magic ( https://github.com/zhouhan0126/WIFIMANAGER-ESP32 ) >> https://github.com/tzapu/WiFiManager (ORIGINAL)


//#include "WiFiManager.h"
//#include <WiFi.h> 
//#include <DNSServer.h>
//#include <WebServer.h>


//
//
//
// --- Fim includes necessário para o wifimanager ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Includes necessário para o Mqtt ---
//

#include <PubSubClient.h>

//
//
//
// --- Fim includes necessário para o Mqtt ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Includes necessário para o OTA ---
//

//#include <WiFi.h>  Já existe
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include "cert.h"

//
//
//
// --- Fim includes necessário para o OTA ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Includes necessário para o NTP ---
//

// também faz uso da biblioteca <WiFi.h>
#include "time.h"

//
//
//
// --- Fim includes necessário para o NTP ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Includes necessário para o Gerenciamento de Arquivos e Json ---
//

#include <EEPROM.h>
#include <FS.h>      
#include <ArduinoJson.h> 

//
//
//
// --- Fim includes necessário para o Gerenciamento de Arquivos e Json ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Fim de Inclusão de Bibliotecas                                           //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Mapeamento de Portas nos Chips i2c_PCF8574P                              //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Mapeamento de Portas no chip i2c_PCF8574P_32 ---
//

int D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 = 0;                             // Sensor 5v, 9v ou 12v
int D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 = 1;                             // Sensor 5v, 9v ou 12v
int D2_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne3 = 2;                             // Sensor 5v, 9v ou 12v
int D3_NoChip_I2C_PCF8574P_32_Rele_1 = 3;                                              // Relê1 aciona Carga
int D4_NoChip_I2C_PCF8574P_32_Rele_2 = 4;                                              // Relê2 aciona Carga
int D5_NoChip_I2C_PCF8574P_32_BorneExt = 5;                                            // Borne ext expoem porta D5
int D6_NoChip_I2C_PCF8574P_32_BorneExt = 6;                                            // Borne ext expoem porta D6
int D7_NoChip_I2C_PCF8574P_32_BorneExt = 7;                                            // Borne ext expoem porta D7

//
//
//
// --- Fim Mapeamento de Portas no chip i2c_PCF8574P_32 ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Fim de Mapeamento de Portas nos Chips i2c_PCF8574P                                           //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Mapeamento de Portas no Esp32 usada nas configurações OTA ---
//

#define LED_BUILTIN  2

//
//
//
// --- Fim Mapeamento de Portas no Esp32 usada nas configurações OTA ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Fim de Mapeamento de Portas nos Chips i2c_PCF8574P                                           //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Estado Inicial das variáveis                                             //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//


//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variável armazena com dados da versão do binário usada pelo OTA ---
//

String FirmwareVer = {
  "1.04"
};

//
//
//
// --- Fim de Variável armazena com dados da versão do binário usada pelo OTA ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variável armazena com definições das Urls dos Binarios usada pelo OTA ---
//

#define URL_fw_Version "https://raw.githubusercontent.com/gigahertzinformatica/6853671974/main/esp32_portao/bin_version.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/gigahertzinformatica/6853671974/main/esp32_portao/fw.bin"

/*
#define URL_fw_Version "https://raw.githubusercontent.com/gigahertzinformatica/esp32/master/bin_version.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/gigahertzinformatica/esp32/master/fw.bin"

#define URL_fw_Version "http://cade-make.000webhostapp.com/version.txt"
#define URL_fw_Bin "http://cade-make.000webhostapp.com/firmware.bin"
*/

//
//
//
// --- Fim de Variável armazena com definições das Urls dos Binarios usada pelo OTA ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variável usada para informar quantos chip I2C_PCF8574P tem na placa ---
//

#define quantosChipsI2C_PCF8574P 1                                // Quantidade de Chips I2C_PCF8574P na placa
byte enderecosPCF8574[quantosChipsI2C_PCF8574P] = {32};           // Variável que referencia cada chip existente na placa


//
//
//
// --- Fim de Variável usada para informar quantos chip I2C_PCF8574P tem na placa ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variáveis para "FuncaoPiscaLed" ---
//

unsigned long int FuncaoPiscaLedtempoAnteriorEmMillis = 0;
boolean estadoled = LOW;

//
//
//
// --- Fim de variáveis para "FuncaoPiscaLed" ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variáveis para Contagem em Millis da update OTA ---
//

unsigned long previousMillis = 0; // will store last time LED was updated
unsigned long previousMillis_2 = 0;
const long interval = 30000;
const long mini_interval = 1000;

//
//
//
// --- Fim de Variáveis para Contagem em Millis da update OTA ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variáveis para "funcaoHoraAtual" ---
//

const char* ipDoServerNtp = "br.pool.ntp.org";
const long  gmtOffset_sec = -5400;
const int   daylightOffset_sec = -5400;

//
//
//
// --- Fim de variáveis para "funcaoHoraAtual" ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variáveis para "FuncaoMensagemNaSerial" ---
//

unsigned long int FuncaoMensagemNaSerialtempoAnteriorEmMillis = 0;

//
//
//
// --- Fim de variáveis para "FuncaoMensagemNaSerial" ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variáveis para "FuncaoAgendaTesteDeGeradorSemanalmente" ---
//

unsigned long int FuncaoAgendaTesteDeGeradorSemanalmenteTempoAnteriorEmMillis = 0;

//
//
//
// --- Fim de variáveis para "FuncaoAgendaTesteDeGeradorSemanalmente" ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variáveis usadas para analisar se tem tensão ou não ---
//

int estado_Sensor_Tensao_No_Borne1 = 0;                      // Estado inicial na lógica da programação
int estado_Sensor_Tensao_No_Borne2 = 0;                      // Estado inicial na lógica da programação
int estado_Sensor_Tensao_No_Borne3 = 0;                      // Estado inicial na lógica da programação


//
//
//
// --- Fim de Variáveis usadas para analisar se tem tensão ou não ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variável armazena a solicitação de acionamento do Gerador ---
//

int iniciarAcionamento = 0;              // Variável que guarda a hora em que deve-se acionar Gerador


//
//
//
// --- Fim de Variável armazena a solicitação de acionamento do Gerador ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variável armazena com dados de conexões wifi usada pelo OTA ---
//

//const char * ssid = "Gigahertz.Net_988401511"; // já existe
//const char * password = "fdhf2fox5"; // já existe


//
//
//
// --- Fim de Variável armazena com dados de conexões wifi usada pelo OTA ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variáveis usadas para armazena nome e senha de wifi ---
//

const char* SSID = "Gigahertz.Net_988401511";   
const char* PASSWORD =  "fdhf2fox5";

// criei variáveis abaixo com dados falsos justamente pra o WifiManager não encontrar a rede nem senha e assim entrar em modo de nova
// configuração de rede wifi 
const char* SSID_Automacao = "Automacao";   
const char* PASSWORD_Automacao =  "Automacao";

//
//
//
// --- Fim de Variáveis usadas para armazena nome e senha de wifi ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Variáveis de conexão MQTT ---
//

const char* mqttNome = "Servidor Mosquitto";
const char* mqttServer = "172.31.255.30";                      // Aqui o endereço do seu servidor do Broker 
const int mqttPort =1883;                                     // Aqui mude para sua porta do Broker
const char* mqttUser = "Esp32PortaoGaragem";                       // Aqui o nome de usuario do Broker
const char* mqttPassword = "fdhf2fox";                        // Aqui sua senha fornecida do Broker
const char* mqttTopic1 = "Casa/Esp32Portao1Garagem";  // Aqui sua do toopico que o Esp ira escutar
const char* mqttTopic2 = "Casa/Esp32Portao2Garagem";  // Aqui sua do toopico que o Esp ira escutar
char EstadoSaida = '0';  
WiFiClient espClient;
PubSubClient client(espClient);


//
//
//
// --- Fim de Variáveis de conexão MQTT ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Fim de Estado Inicial das variáveis                                      //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Ao ligar o SETUP irá executar estas funções abaixo apenas uma vez        //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Declarações de Funções existentes no sistema ---
//

/*
void etapa4_Func_que_analiza_instabilidade_da_Concecionaria();
void etapa6_FuncaoQueAcionaDeFatoOGerador();
void etapa8_FuncaoQueDesligaOGerador();
void etapa7_FuncaoQueMonitoraOFuncionamentoDoGeradorEVoltaDaConcecionaria();
void FuncaoQueSolicitaAcionamentoDoGerador();
void etapa9_FuncaoQueTestaOGerador();
void FuncaoAgendaTesteDeGeradorSemanalmente(int intervalo);
void etapa11_FuncaoQueAcionaDeFatoOGeradorSemanalmente();
void FuncaoMensagemNaSerial( String mensagem, int tempo );
*/

void FuncaoPiscaLed(int porta, int tempo);
void InicSaidas(void);
void InicStatusDasSaidas();
void InicSerial();
void InicWiFi();
void InicMQTT();
void ReconectWiFi();
void ReconnectMQTT();
void MudarRedeWiFi();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void etapa3_FuncaoQueSolicitaAcionamentoDoGerador();
void InicHoraAtual();
void funcaoHoraAtual();


//
//
//
// --- Fim de declarações de Funções existentes no sistema ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Funções que controla os pinos do Chip I2C_PCF8574P ---
//

bool i2CciPCF8574PinMode(byte pino, int modo = -1);
void i2CciPCF8574Write(byte pino, bool estado);
bool i2CciPCF8574Read(byte pino);


//
//
//
// --- Fim de funções que controla os pinos do Chip I2C_PCF8574P ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Funções usadas pelo OTA que são executadas antes do SETUP ---
//

//void connect_wifi(); // já existe
void firmwareUpdate();
int FirmwareVersionCheck();

void repeatedCall() {
  static int num=0;
  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis) >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if (FirmwareVersionCheck()) {
      firmwareUpdate();
    }
  }
  if ((currentMillis - previousMillis_2) >= mini_interval) {
    previousMillis_2 = currentMillis;
    Serial.print("Quantidade de Verificações...");
    Serial.print(num++);
    Serial.print(" Versão Atual:");
    Serial.println(FirmwareVer);
   if(WiFi.status() == WL_CONNECTED) 
   {
       Serial.println("wifi connectado");
   }
   else
   {
    InicWiFi();
   }
  }
}

struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button_boot = {
  0,
  0,
  false
};
/*void IRAM_ATTR isr(void* arg) {
    Button* s = static_cast<Button*>(arg);
    s->numberKeyPresses += 1;
    s->pressed = true;
}*/

void IRAM_ATTR isr() {
  button_boot.numberKeyPresses += 1;
  button_boot.pressed = true;
}


//
//
//
// --- Fim de Funções usadas pelo OTA que são executadas antes do SETUP ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Fim de Ao ligar o SETUP irá executar estas funções abaixo apenas uma vez //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// 2ª etapa - Aqui serão Será efetuado funcões e inicializações na          //
// inicialização do Sistema                                                 //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Setup                                                   //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//


void setup() {
  
  Wire.begin();             // Iniciu da Biblioteca do Chip I2C_PCF8574P

  //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Inicialização no Setup direcionadas ao OTA ---
//

  pinMode(button_boot.PIN, INPUT);
  attachInterrupt(button_boot.PIN, isr, RISING);
  Serial.begin(115200);
  Serial.print("Active firmware version:");
  Serial.println(FirmwareVer);
  pinMode(LED_BUILTIN, OUTPUT);
  
//
//
//
// --- Fim de Inicialização no Setup direcionadas ao OTA ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



  InicSaidas();
  InicStatusDasSaidas();
  InicSerial();
  InicWiFi();
  InicMQTT();
  InicHoraAtual();
  

  

/*
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// "Ota" iniciu no setup
//
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("Esp32Principal");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // Dica de senha padrão atualizar programa dentro do Esp32Principal = fdxxxxxx
  ArduinoOTA.setPasswordHash("ee61cd839f077a9d10553edd5ccb4ed1");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

//
// "Ota" final no setup
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
//

*/

}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Fim do Setup                                                             //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// 3ª etapa - Aqui inifinitamente enquanto o esp está ligado é        //
// executado verificações de conexão da wifi e mqtt                         //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
  
void loop() {
      //garante funcionamento das conexões WiFi e ao broker MQTT
      
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

    funcaoHoraAtual();
    Serial.println();
    delay(1000);
    
    }
    {
      client.loop();
    }  

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Execução no loop direcionadas ao OTA ---
//
// Caso botão Boot do Esp32 for apertado o Esp32 irá buscar nova atualização do firmeware

  if (button_boot.pressed) { //to connect wifi via Android esp touch app 
    Serial.println("Firmware update Starting..");
    firmwareUpdate();
    button_boot.pressed = false;
  }
  repeatedCall();
  
//
//
//
// --- Fim de Execução no loop direcionadas ao OTA ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



/*
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// aqui Testa as entradas de sensores D0, D1 e D2 do Chip i2c_I2C_PCF8574P_32 sempre que o loop reinicia

  estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
  delay(30);
  estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );
  delay(30);
  estado_Sensor_Tensao_No_Borne3 = i2CciPCF8574Read( D2_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne3 );
  delay(30);

  // Este if monitora a queda da energia enviada para Carga
  // Vale lembrar que no chips i2cciPCF8574 existe uma caracteristica que não posso esquecer. é que quando suas portas são configuradas
  // no modo "INPUT" elas reagem de forma inversa pois começa com "5v" e pra ficar "0" tem que aterrar em GND então o programa
  // funciona meio invertido. Então quando for lido na porta com "1" então é porque não tem celpe e quando for lido "0" é pq a 
  // celpe está presente. OBS. Nas portas configuradas com "OUTPUT" o comando é enviado corretamente "0" para desligado e "1" para ligado
  
  if (( estado_Sensor_Tensao_No_Borne1 == 0 ) && ( estado_Sensor_Tensao_No_Borne2 == 1 ) && ( iniciarAcionamento == 0 ))
  {

        while ((estado_Sensor_Tensao_No_Borne1 == 0) && (estado_Sensor_Tensao_No_Borne2 == 1) && ( iniciarAcionamento == 0 )){
         

 //             FuncaoMensagemNaSerial( "Sensor borne 1 ligado OK e Sensor borne 2 Desligado Ok", 5000 );
                Serial.println("Sensor borne 1 ligado OK e Sensor borne 2 Desligado Ok");
                delay(3000);

                i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Rele_1, 1 );
                i2CciPCF8574Write( D4_NoChip_I2C_PCF8574P_32_Rele_2, 0 );

                
              
  //            FuncaoAgendaTesteDeGeradorSemanalmente(86400000); // Agendamento para acionar Gerador em 24hrs (86400000).
              
  estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
  estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );
  estado_Sensor_Tensao_No_Borne3 = i2CciPCF8574Read( D2_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne3 );
  delay(3000);
              }
  }
  if ((estado_Sensor_Tensao_No_Borne1 == 0) && (estado_Sensor_Tensao_No_Borne2 == 0))
  {
    Serial.println("Sensor borne 1 LIGADO e Sensor borne 2 LIGADO");

                i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Rele_1, 1 );
                i2CciPCF8574Write( D4_NoChip_I2C_PCF8574P_32_Rele_2, 1 );
    
  estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
  estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );
  estado_Sensor_Tensao_No_Borne3 = i2CciPCF8574Read( D2_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne3 );
  delay(3000);

  }

  else if ((estado_Sensor_Tensao_No_Borne1 == 1) && (estado_Sensor_Tensao_No_Borne2 == 1))
  {
    Serial.println("Sensor borne 1 DESLIGADO e Sensor borne 2 DELIGADO");
    //            i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Rele_1, 0 );
    //            i2CciPCF8574Write( D4_NoChip_I2C_PCF8574P_32_Rele_2, 0 );
  estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
  estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );
  estado_Sensor_Tensao_No_Borne3 = i2CciPCF8574Read( D2_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne3 );
  delay(3000);

  }
  
  else
  {
    Serial.println("Não caiu em nenhuma condição");
    delay(3000);
 
  }

  // Etapa5 Este if monitora a queda da energia da Concecionaria
  if (iniciarAcionamento == 1)
  {
    Serial.println("Esta é Etapa5, if no loop principal que detectou que a variável iniciarAcionamentoDoGerador = 1");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);

    int etapa5estado_Sensor_Tensao_No_Borne1 = 0;
    int etapa5estado_Sensor_Tensao_No_Borne2 = 0;

    etapa5estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
    etapa5estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );

    if ((iniciarAcionamento == 1) && (etapa5estado_Sensor_Tensao_No_Borne1 == 1) && (etapa5estado_Sensor_Tensao_No_Borne2 == 1))
    {

      Serial.println("Procedimentos a ser realizado quando solicitado em alguma parte do sistema atraves de iniciarAcionamento = 1 ");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
    Serial.println("mensagem 1 Nenhuma acao a executar na etapa 5");
    }
    Serial.println("mensagem 2 Nenhuma acao a executar na etapa 5");

  }






*/

// Pisca o led da placa Esp32 apenas pra informar que terminou o loop
  digitalWrite( LED_BUILTIN, 1 ); 
  delay(100);                     
  digitalWrite( LED_BUILTIN, 0 ); 
  delay(100);
  digitalWrite( LED_BUILTIN, 1 ); 
  delay(100);                     
  digitalWrite( LED_BUILTIN, 0 ); 
  delay(100);
}



// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Fim do LOOP
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Iniciu de Funções
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Função MudarRedeWiFi ---
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
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(200);
        Serial.print(". ");
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

void ReconectWiFi(){
{
    
    delay(10);
    Serial.println();
          delay(1000);
          Serial.println();
          Serial.print("%%%%%%%%%%% Tentativa de reconectar na rede: ");
          Serial.print(SSID);
          Serial.print(" %%%%%%%%%%%");
          delay(1000);
          Serial.println();
          Serial.print("<<==>> ");
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD); 
  
    
    while (WiFi.status() != WL_CONNECTED){
      
          delay(1000);
          Serial.println();
          delay(1000);
          Serial.println();
          Serial.print("%%%%%%%%%%% Tentativa de reconectar na rede: ");
          Serial.print(SSID);
          Serial.print(" %%%%%%%%%%%");
          delay(1000);
          Serial.println();
          Serial.print("<<==>> ");
    }
   
    Serial.println();
    Serial.print("Reconectado com sucesso na rede Wifi: ");
    Serial.print(SSID);
    Serial.println();
    Serial.print("IP obtido via dhcp: ");
    Serial.print(WiFi.localIP());
    Serial.println();
    delay(1000);
    ReconnectMQTT();
}
    
}



//
//
//
// --- Fim de Função ReconectWiFi ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// --- Função ReconnectMQTT ---
//

void ReconnectMQTT(){
  
    client.setServer(mqttServer, mqttPort);   //informa qual broker e porta deve ser conectado
    client.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
    while (!client.connected()) {
        Serial.println();
        Serial.print("ReConnectando ao seu MQTT com nome: ");
        Serial.print(mqttNome);
    
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      
        Serial.println();
        Serial.print("ReConnectado ao seu MQTT com Sucesso!");
        Serial.println();
        delay(100);
        
        Serial.print("Se escrevendo no Topico: ");
        Serial.print(mqttTopic1);
        Serial.println();
        delay(100);

        Serial.print("Publicando um teste no Topico: ");
        client.publish("Esp32","Portao1");
        client.subscribe(mqttTopic1);
        Serial.println();
        delay(100);
        
        Serial.println();
        Serial.print("Inscrito no topico: ");
        Serial.print(mqttTopic1);
        Serial.print(" com sucesso!");
        Serial.println();
        delay(100);
      
        Serial.print("Se escrevendo no Topico: ");
        Serial.print(mqttTopic2);
        Serial.println();
        delay(100);
       
        Serial.print("Publicando um teste no Topico: ");
        client.publish("Esp32","Portao2");
        client.subscribe(mqttTopic2);
        Serial.println();
        delay(100);
        
        Serial.println();
        Serial.print("Inscrito no topico: ");
        Serial.print(mqttTopic2);
        Serial.print(" com sucesso!");
        Serial.println();
        delay(100); 
      
    } else {
      Serial.println(); 
      Serial.print("Falhou ao tentar reconectar-se no Broker Mqtt: ");
      Serial.print(mqttNome);
      Serial.print(client.state());
      delay(2000);
 
    }

  }


}

//
//
//
// --- Fim de Função ReconnectMQTT ---
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//////////////////////////////////////////////////////////////////////////////
// Função InicSaidas                                                        //
//////////////////////////////////////////////////////////////////////////////  
void InicSaidas(void){
// NoChip_I2C_PCF8574P_32
delay(50);
i2CciPCF8574PinMode( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1, INPUT );        
i2CciPCF8574PinMode( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2, INPUT );                     
i2CciPCF8574PinMode( D2_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne3, INPUT );
i2CciPCF8574PinMode( D3_NoChip_I2C_PCF8574P_32_Rele_1, OUTPUT );
i2CciPCF8574PinMode( D4_NoChip_I2C_PCF8574P_32_Rele_2, OUTPUT );
i2CciPCF8574PinMode( D5_NoChip_I2C_PCF8574P_32_BorneExt, OUTPUT );
i2CciPCF8574PinMode( D6_NoChip_I2C_PCF8574P_32_BorneExt, OUTPUT );
i2CciPCF8574PinMode( D7_NoChip_I2C_PCF8574P_32_BorneExt, OUTPUT );  
}


//////////////////////////////////////////////////////////////////////////////
// Função Colocar Status iniciais nas porta Output                                                        //
//////////////////////////////////////////////////////////////////////////////  
void InicStatusDasSaidas(void){
// NoChip_I2C_PCF8574P_32
delay(50);
/*
// As portas digitais no modo de Input não pode ser definido aqui seu status pois é feito no programa
i2CciPCF8574Write( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1, INPUT );        
i2CciPCF8574Write( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2, INPUT );                     
i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne3, INPUT );
*/
i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Rele_1, 0 );
i2CciPCF8574Write( D4_NoChip_I2C_PCF8574P_32_Rele_2, 0 );
i2CciPCF8574Write( D5_NoChip_I2C_PCF8574P_32_BorneExt, 0 );
i2CciPCF8574Write( D6_NoChip_I2C_PCF8574P_32_BorneExt, 0 );
i2CciPCF8574Write( D7_NoChip_I2C_PCF8574P_32_BorneExt, 0 );  
}




//////////////////////////////////////////////////////////////////////////////
// Função InicSerial                                                        //
//////////////////////////////////////////////////////////////////////////////

void InicSerial(){
      Serial.begin(9600);
}
    
//////////////////////////////////////////////////////////////////////////////
// Função InicWiFi                                                          //
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Função InicHoraAtual                                                        //
//////////////////////////////////////////////////////////////////////////////

void InicHoraAtual(){
   configTime(gmtOffset_sec, daylightOffset_sec, ipDoServerNtp);
   funcaoHoraAtual();

}
    
//////////////////////////////////////////////////////////////////////////////
// Função InicHoraAtual                                                          //
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Função funcaoHoraAtual                                                   //
//////////////////////////////////////////////////////////////////////////////

void funcaoHoraAtual()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println();
    Serial.println("Falha ao atualizar a Hora");
    return;
  }
  Serial.println();
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
    
//////////////////////////////////////////////////////////////////////////////
// Função funcaoHoraAtual                                                   //
//////////////////////////////////////////////////////////////////////////////

void InicWiFi(){
{
    delay(10);
    Serial.println();
    Serial.println("------Conexao WI-FI ------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Por favor aguarde");
         
 //   WiFi.begin(SSID, PASSWORD); 
  
    WiFiManager wifiManager;
    wifiManager.autoConnect("AP_Ghic_Automacao", "fdhf2fox");
    Serial.println();
    Serial.print("conectado via automacao");
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(200);
        Serial.print(". ");
    }
   
    Serial.println();
    Serial.print("Conectado com sucesso na rede Wifi: ");
    Serial.print(SSID);
    Serial.println();
    Serial.print("IP obtido via dhcp: ");
    Serial.print(WiFi.localIP());
    delay(1000);
    InicMQTT();
}
    
}


//////////////////////////////////////////////////////////////////////////////
// Função InicMQTT                                                          //
//////////////////////////////////////////////////////////////////////////////

void InicMQTT(){
  
    client.setServer(mqttServer, mqttPort);   //informa qual broker e porta deve ser conectado
    client.setCallback(mqtt_callback);        //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
    while (!client.connected()) {
        Serial.println();
        Serial.print("Iniciando conexao ao MQTT com nome: ");
        Serial.print(mqttNome);
    
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      
        Serial.println();
        Serial.print("Conectado ao seu MQTT com Sucesso!");
        Serial.println();
        delay(100);
        
        Serial.print("Se escrevendo no Topico: ");
        Serial.print(mqttTopic1);
        Serial.println();
        delay(100);

        Serial.print("Publicando um teste no Topico: ");
        client.publish("Esp32","Portao1");
        client.subscribe(mqttTopic1);
        Serial.println();
        delay(100);
        
        Serial.println();
        Serial.print("Inscrito no topico: ");
        Serial.print(mqttTopic1);
        Serial.print(" com sucesso!");
        Serial.println();
        delay(100);
      
        Serial.print("Se escrevendo no Topico: ");
        Serial.print(mqttTopic2);
        Serial.println();
        delay(100);
       
        Serial.print("Publicando um teste no Topico: ");
        client.publish("Esp32","Portao2");
        client.subscribe(mqttTopic2);
        Serial.println();
        delay(100);
        
        Serial.println();
        Serial.print("Inscrito no topico: ");
        Serial.print(mqttTopic2);
        Serial.print(" com sucesso!");
        Serial.println();
        delay(100); 
      
    } else {
      Serial.println(); 
      Serial.print("Falhou ao tentar conectar-se no Broker Mqtt: ");
      Serial.print(mqttNome);
      Serial.print(client.state());
      delay(2000);
 
    }

  }


}

//////////////////////////////////////////////////////////////////////////////
// Apartir daqui o esp irá ler menssagens que chega no topico inscrito e    //
// executar tarefas                                                         //
//////////////////////////////////////////////////////////////////////////////

// Aqui é lida as menssagens que estão na variável msg

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.println();
  Serial.print("Menssagem recebida no Topico: ");
  Serial.print(topic);
  Serial.println();
  
  for (int i = 0; i < length; i++) {
  Serial.print((char)payload[i]);

       String msg;
    
 
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) {
       char c = (char)payload[i];
       msg += c;
    }
  
// Aqui é executado as tarefas de acordo com o que foi lido na variável msg

if (msg.equals("L3"))        
    {
    
        i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Rele_1, 1 );
        //EstadoSaida = '1';
        delay(200);
    }
   
if (msg.equals("D3"))
    {
    
        i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Rele_1, 0 );
        //EstadoSaida = '0';
        delay(200);
    }
    if (msg.equals("L4"))
    {
    
        i2CciPCF8574Write( D4_NoChip_I2C_PCF8574P_32_Rele_2, 1 );
        //EstadoSaida = '1';
        delay(200);
    }
if (msg.equals("D4"))
    {
    
        i2CciPCF8574Write( D4_NoChip_I2C_PCF8574P_32_Rele_2, 0 );
        //EstadoSaida = '0';
        delay(200);
    }

if (msg.equals("LT"))
    {
    
                i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Rele_1, 1 );
                i2CciPCF8574Write( D4_NoChip_I2C_PCF8574P_32_Rele_2, 1 );
         delay(200);
    
    }
 if (msg.equals("DT"))
    {
    
                i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Rele_1, 0 );
                i2CciPCF8574Write( D4_NoChip_I2C_PCF8574P_32_Rele_2, 0 );
        delay(200);

   }


if (msg.equals("R3"))
   {
    Serial.println("mqtt foi igual a R3 Pulso de 600ms do D3");
                i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Rele_1, 1 );
        delay(200);
                i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Rele_1, 0 );
    //EstadoSaida = '1';
        delay(200);
   }
   if (msg.equals("MudarRedeWifi"))
   {
    Serial.println("mqtt foi igual a MudarRedeWifi");
        delay(200);
        MudarRedeWiFi();
        delay(200);
   }




  }


  Serial.println();
  Serial.print("--------- Aguardando nova Mensagem nos Topicos ---------");
  Serial.println();
  Serial.print(mqttTopic1);
  Serial.println();
  Serial.print(mqttTopic2);
  Serial.println();
  delay(1000);
}



// --- QTA Desenvolvimento das Funções ---
//
//
//


/*

//---------------------Esta função que analisa instabilidade da concecionária---------------------//

void etapa4_Func_que_analiza_instabilidade_da_Concecionaria()
{
  Serial.println("Etapa4 aqui irá confirmar se energia concecionaria está normal ou Não");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);

  int etapa4estado_Sensor_Tensao_No_Borne1 = 0; // Estado inicial na lógica da programação
  int etapa4estado_Sensor_Tensao_No_Borne2 = 0;       // Estado inicial na lógica da programação
  int contador = 0;

  delay(3000);

  etapa4estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
  etapa4estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );

  if ((etapa4estado_Sensor_Tensao_No_Borne1 == 1) && (etapa4estado_Sensor_Tensao_No_Borne2 == 1))
  {

    while ((etapa4estado_Sensor_Tensao_No_Borne1 == 1) && (etapa4estado_Sensor_Tensao_No_Borne2 == 1) && (contador <= 3))
    {
      delay(3000);

      Serial.println("Etapa4 está testando a Concecionaria");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      etapa4estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
      etapa4estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );
      contador++;
    }

    if ((etapa4estado_Sensor_Tensao_No_Borne1 == 1) && (etapa4estado_Sensor_Tensao_No_Borne2 == 1))
    {
      Serial.println("Concecionaria ainda Ausente");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      i2CciPCF8574Write( D8_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Concecionaria, 0 );
      i2CciPCF8574Write( D1_NoChip_I2C_PCF8574P_32_Led_Monitora_Concecionaria, 0 );
      delay(500);
      i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 0 );
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(500);
      i2CciPCF8574Write( D10_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Carga, 0 );
      i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Led_Monitora_Carga, 0 );
      delay(500);

      iniciarAcionamentoDoGerador = 1;
      delay(500);
    }
    else if (etapa4estado_Sensor_Tensao_No_Borne1 == 0)
    {
      Serial.println("Concecionaria Voltou ");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      delay(500);
    }

    contador = 0;
  }
  else if (etapa4estado_Sensor_Tensao_No_Borne1 == 0)
  {
    Serial.println("Realmente a Concecionaria está OK");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    if ((etapa4estado_Sensor_Tensao_No_Borne1 == 0) && (etapa4estado_Sensor_Tensao_No_Borne2 == 1))
    {

      Serial.println("Realmente a Concecionaria está OK, e Gerador não está ligado Então Ligarei as Cotactoras");
      delay(990);
      Serial.println(">>>>>>>>>>");
      delay(990);
      Serial.println(">>>>>>>>>>");
      delay(990);

      i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 0 );
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(500);
      i2CciPCF8574Write( D8_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Concecionaria, 1 );
      i2CciPCF8574Write( D1_NoChip_I2C_PCF8574P_32_Led_Monitora_Concecionaria, 1 );
      delay(5000);
      i2CciPCF8574Write( D10_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Carga, 1 );
      i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Led_Monitora_Carga, 1 );
      delay(500);
      iniciarAcionamentoDoGerador = 0;
    }
    else
    {
      Serial.println("Realmente a Concecionaria está OK, Porém vou chamar Etapa8 para desligar Gerador");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 0 );
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(500);
      i2CciPCF8574Write( D8_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Concecionaria, 0 );
      i2CciPCF8574Write( D1_NoChip_I2C_PCF8574P_32_Led_Monitora_Concecionaria, 0 );
      delay(500);
      i2CciPCF8574Write( D10_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Carga, 0 );
      i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Led_Monitora_Carga, 0 );
      delay(500);

      etapa8_FuncaoQueDesligaOGerador();
    }
    Serial.println("Etapa4 Fim do Ciclo Energia da Concecionaria Normalizada e Voltará para o 1ª IF ok");
    delay(990);
    Serial.println(">>>>>>>>>>");
    delay(990);
    Serial.println(">>>>>>>>>>");
    delay(990);
    
    }
  }


//
//
//
//---------------------Esta função Aciona o Gerador---------------------//

void etapa6_FuncaoQueAcionaDeFatoOGerador()
{
  Serial.println("Etapa6 Procedimentos para acionar Gerador");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  Serial.println("Desligando as contactoras");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  i2CciPCF8574Write( D10_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Carga, 0 );
  i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Led_Monitora_Carga, 0 );
  delay(500);
  i2CciPCF8574Write( D8_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Concecionaria, 0 );
  i2CciPCF8574Write( D1_NoChip_I2C_PCF8574P_32_Led_Monitora_Concecionaria, 0 );
  delay(500);
  i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 0 );
  i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
  delay(3000);

  int etapa6estado_Sensor_Tensao_No_Borne1 = 0; // Estado inicial na lógica da programação
  int etapa6estado_Sensor_Tensao_No_Borne2 = 0;       // Estado inicial na lógica da programação
  int contador = 1;

  etapa6estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
  etapa6estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );

  if ((etapa6estado_Sensor_Tensao_No_Borne1 == 1) && (etapa6estado_Sensor_Tensao_No_Borne2 == 1))
  {
    delay(500);

    while ((etapa6estado_Sensor_Tensao_No_Borne1 == 1) && (etapa6estado_Sensor_Tensao_No_Borne2 == 1) && (contador <= 3))
    {

      delay(500);
      Serial.println("Garantir que todas as contactoras estão desligadas");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      i2CciPCF8574Write( D8_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Concecionaria, 0 );
      i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 0 );
      i2CciPCF8574Write( D10_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Carga, 0 );
      i2CciPCF8574Write( D11_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Liga_Painel_Do_Gerador, 0 );
      i2CciPCF8574Write( D12_NoChip_I2C_PCF8574P_33_Controla_Contactora_Afogador_Do_Gerador, 0 );
      i2CciPCF8574Write( D13_NoChip_I2C_PCF8574P_33_Controla_Contactora_Start_Do_Gerador, 0 );

      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");

      delay(500);
      Serial.println("Puxando o Afogador");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

     
      i2CciPCF8574Write( D12_NoChip_I2C_PCF8574P_33_Controla_Contactora_Afogador_Do_Gerador, 1 );

                  
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");

      
      delay(500);
      Serial.println("acionar Painel do Gerador");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      i2CciPCF8574Write( D11_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Liga_Painel_Do_Gerador, 1 );

      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");


      delay(500);
      Serial.println("acionar Start do Gerador");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);


      i2CciPCF8574Write( D13_NoChip_I2C_PCF8574P_33_Controla_Contactora_Start_Do_Gerador, 1 );


      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");

      
      Serial.println("soltando o afogador do Gerador");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      i2CciPCF8574Write( D12_NoChip_I2C_PCF8574P_33_Controla_Contactora_Afogador_Do_Gerador, 0 );
      

      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");


      Serial.println("desligando o Start");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      i2CciPCF8574Write( D13_NoChip_I2C_PCF8574P_33_Controla_Contactora_Start_Do_Gerador, 0 );

      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");
      
      
      Serial.println("Verificando se Gerador ligou normal");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(3000);

      etapa6estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
      etapa6estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );
      contador++;
      delay(1000);
    }
  }
  if ((etapa6estado_Sensor_Tensao_No_Borne1 == 1) && (etapa6estado_Sensor_Tensao_No_Borne2 == 1))
  {

    Serial.println("Se O Gerador não ligou e nem a concecionaria voltou então volta para etapa6");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    etapa6_FuncaoQueAcionaDeFatoOGerador();
  }

  if ((etapa6estado_Sensor_Tensao_No_Borne1 == 1) && (etapa6estado_Sensor_Tensao_No_Borne2 == 0))
  {
    Serial.println("Aqui na etapa6 foi concluido com exito a ligação do gerador e a Celpe não voltou");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);

    Serial.println("Agora irei religar as contactoras que libera energia do Gerador para a Carga Mantendo a contactora da Concecionaria Desligada");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);

    i2CciPCF8574Write( D8_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Concecionaria, 0 );
    i2CciPCF8574Write( D1_NoChip_I2C_PCF8574P_32_Led_Monitora_Concecionaria, 0 );
    delay(5000);
    i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 1 );
    i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
    delay(5000);
    i2CciPCF8574Write( D10_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Carga, 1 );
    i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Led_Monitora_Carga, 1 );
    delay(290);

    contador = 0;
    delay(1000);

    Serial.println("Ok Carga recebendo energia do Gerador, Agora vou chamar a Etapa7 pra ficar monitorando Gerador e a volta da Concecionaria");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);

    etapa7_FuncaoQueMonitoraOFuncionamentoDoGeradorEVoltaDaConcecionaria();
  }
}


//
//
//
//
//
//---------------------Esta função monitora o funcionamento do gerador e a volta da concecionaria---------------------//

void etapa7_FuncaoQueMonitoraOFuncionamentoDoGeradorEVoltaDaConcecionaria()
{

  Serial.println("Etapa7 monitoranfo funcionamento do Gerador e observando se a concecionaria volta");

  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  int etapa7estado_Sensor_Tensao_No_Borne1 = 0; // Estado inicial na lógica da programação
  int etapa7estado_Sensor_Tensao_No_Borne2 = 1;       // Estado inicial na lógica da programação

  delay(1000);

  etapa7estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
  etapa7estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );

  delay(1000);
  if ((etapa7estado_Sensor_Tensao_No_Borne1 == 1) && (etapa7estado_Sensor_Tensao_No_Borne2 == 0))
  {

    while ((etapa7estado_Sensor_Tensao_No_Borne1 == 1) && (etapa7estado_Sensor_Tensao_No_Borne2 == 0))
    {
      Serial.println("Aqui na Etapa7 o Gerador está OK e a Concecionaria ainda não Voltou!");
      Serial.println(">>>>>>>>>>");
      delay(3000);

      etapa7estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
      etapa7estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );
    }
    if ((etapa7estado_Sensor_Tensao_No_Borne1 == 1) && (etapa7estado_Sensor_Tensao_No_Borne2 == 1))
    {
      Serial.println("Gerador Falhou, Será preciso Religar Gerador!");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      etapa6_FuncaoQueAcionaDeFatoOGerador();
    }

    if ((etapa7estado_Sensor_Tensao_No_Borne1 == 0) && (etapa7estado_Sensor_Tensao_No_Borne2 == 0))
    {
      Serial.println("Concecionaria voltou desligar Gerador na etapa8 !");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      etapa8_FuncaoQueDesligaOGerador();
    }
  }
}


//
//
//
//
//
//---------------------Esta função desliga o Gerador em caso de Volta da Concecionaria---------------------//

void etapa8_FuncaoQueDesligaOGerador()
{
  Serial.println("Aqui na Etapa8 é a função para Desligar Gerador!");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);

  int etapa8estado_Sensor_Tensao_No_Borne1 = 1;
  int etapa8estado_Sensor_Tensao_No_Borne2 = 1;

  etapa8estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
  etapa8estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );

  if ((etapa8estado_Sensor_Tensao_No_Borne1 == 0) && (etapa8estado_Sensor_Tensao_No_Borne2 == 0))
  {
    Serial.println("Desligar as Contactoras");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    
    i2CciPCF8574Write( D10_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Carga, 0 );
    i2CciPCF8574Write( D3_NoChip_I2C_PCF8574P_32_Led_Monitora_Carga, 0 );
    delay(500);
    i2CciPCF8574Write( D8_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Da_Concecionaria, 0 );
    i2CciPCF8574Write( D1_NoChip_I2C_PCF8574P_32_Led_Monitora_Concecionaria, 0 );
    delay(500);
    i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 0 );
    i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
    delay(5000);
    
    etapa8estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
    etapa8estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );
    
    while ((etapa8estado_Sensor_Tensao_No_Borne1 == 0) && (etapa8estado_Sensor_Tensao_No_Borne2 == 0))
    {

      Serial.println("Desligando o Painel do Gerador");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      i2CciPCF8574Write( D11_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Liga_Painel_Do_Gerador, 0 );

      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");


      etapa8estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
      etapa8estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );
      delay(5000);
    }

    
    if ((etapa8estado_Sensor_Tensao_No_Borne1 == 0) && (etapa8estado_Sensor_Tensao_No_Borne2 == 1))
    {
      Serial.println("Aqui na etapa8 foi concluido com exito o desligamento do gerador");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      iniciarAcionamentoDoGerador = 0;
    }

   }
}

//
//
//
//
//
//---------------------Esta função faz um teste semanal no Gerador, mesmo com a concecionaria em funcionamento--------------------//

void etapa9_FuncaoQueTestaOGerador()
{
  Serial.println("Aqui na Etapa9 é a função testar o Gerador, que foi ligado manualmente ou automaticamente Semanalmente com a Celpe funcionando");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);

  int etapa9estado_Sensor_Tensao_No_Borne1 = 1;
  int etapa9estado_Sensor_Tensao_No_Borne2 = 1;
  int contadorEtapa9 = 0;

  etapa9estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
  etapa9estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );

  if ((etapa9estado_Sensor_Tensao_No_Borne1 == 0) && (etapa9estado_Sensor_Tensao_No_Borne2 == 0))
  {
      Serial.println("Há energia na Concecionária e Gerador, então devo Garantir que a contactora do Gerador está Desligado");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 0 );
      delay(3000);
      
      i2CciPCF8574Write( D11_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Liga_Painel_Do_Gerador, 1 );
      delay(100);

      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");
    
    etapa9estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
    etapa9estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );

    while ((etapa9estado_Sensor_Tensao_No_Borne1 == 0) && (etapa9estado_Sensor_Tensao_No_Borne2 == 0) && (contadorEtapa9 <= 2000))
    {


      FuncaoPiscaLed( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 150 );
      FuncaoMensagemNaSerial( "Teste semanal do Gerador", 3000 );
      
      i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 0 );

      etapa9estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
      etapa9estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );
      
      contadorEtapa9++;
      delay(10);
    }
    if ((etapa9estado_Sensor_Tensao_No_Borne1 == 0) && (etapa9estado_Sensor_Tensao_No_Borne2 == 0))
    {
      Serial.println("Pronto Terminou o periodo de teste do Gerador agora vou desligar o Gerador");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      while ((etapa9estado_Sensor_Tensao_No_Borne1 == 0) && (etapa9estado_Sensor_Tensao_No_Borne2 == 0))
      {

        Serial.println("Desligando Painel do Gerador após teste");
        delay(500);
        Serial.println(">>>>>>>>>>");
        delay(500);
        Serial.println(">>>>>>>>>>");
        delay(500);


        i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 0 );
        

        i2CciPCF8574Write( D11_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Liga_Painel_Do_Gerador, 0 );

        i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
        delay(100);
        i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
        delay(100);
        i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
        delay(100);
        i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
        delay(1000);

        delay(500);
        Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");


        etapa9estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
        etapa9estado_Sensor_Tensao_No_Borne2 = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );
        delay(5000);
      }
      if ((etapa9estado_Sensor_Tensao_No_Borne1 == 0) && (etapa9estado_Sensor_Tensao_No_Borne2 == 1))
      {
        Serial.println("Aqui na etapa9 foi concluido com exito o desligamento do gerador após de Teste Semanal");
        delay(500);
        Serial.println(">>>>>>>>>>");
        delay(500);
        Serial.println(">>>>>>>>>>");
        delay(500);

        i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 0 );
        i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
        delay(10);
        
      }
      
      contadorEtapa9 = 0;
      delay(500);
    }
    
  }
}


//
//
//
//---------------------Esta função Aciona o Gerador Semanalmente---------------------//

void etapa11_FuncaoQueAcionaDeFatoOGeradorSemanalmente()
{
  Serial.println("Etapa11 Procedimentos para acionar Gerador Semanalmente para Teste");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  Serial.println("Desligando as contactoras");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  Serial.println(">>>>>>>>>>");
  delay(500);
  
  i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 0 );
  i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
  delay(3000);

  int etapa11estado_Sensor_Tensao_No_Borne1 = 0; // Estado inicial na lógica da programação
  int etapa11estado_Sensor_Tensao_No_Borne2Semanalmente = 0;       // Estado inicial na lógica da programação
  int contador = 1;

  etapa11estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
  etapa11estado_Sensor_Tensao_No_Borne2Semanalmente = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );

  if ((etapa11estado_Sensor_Tensao_No_Borne1 == 0) && (etapa11estado_Sensor_Tensao_No_Borne2Semanalmente == 1))
  {
    delay(500);
      

    while ((etapa11estado_Sensor_Tensao_No_Borne1 == 0) && (etapa11estado_Sensor_Tensao_No_Borne2Semanalmente == 1) && (contador <= 3))
    {

      delay(500);
      Serial.println("Garantir que a contactora do gerador está desligado pois é um teste semanal");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      i2CciPCF8574Write( D11_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Liga_Painel_Do_Gerador, 0 );
      i2CciPCF8574Write( D12_NoChip_I2C_PCF8574P_33_Controla_Contactora_Afogador_Do_Gerador, 0 );
      i2CciPCF8574Write( D13_NoChip_I2C_PCF8574P_33_Controla_Contactora_Start_Do_Gerador, 0 );

      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");

      delay(500);
      Serial.println("Puxando o Afogador");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

     
      i2CciPCF8574Write( D12_NoChip_I2C_PCF8574P_33_Controla_Contactora_Afogador_Do_Gerador, 1 );

                  
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");

      
      delay(500);
      Serial.println("acionar Painel do Gerador");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      i2CciPCF8574Write( D11_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Liga_Painel_Do_Gerador, 1 );

      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");


      delay(500);
      Serial.println("acionar Start do Gerador");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);


      i2CciPCF8574Write( D13_NoChip_I2C_PCF8574P_33_Controla_Contactora_Start_Do_Gerador, 1 );


      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");

      
      Serial.println("soltando o afogador do Gerador");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      i2CciPCF8574Write( D12_NoChip_I2C_PCF8574P_33_Controla_Contactora_Afogador_Do_Gerador, 0 );
      

      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");


      Serial.println("desligando o Start");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);

      i2CciPCF8574Write( D13_NoChip_I2C_PCF8574P_33_Controla_Contactora_Start_Do_Gerador, 0 );

      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");
      
      
      Serial.println("Verificando se Gerador ligou normal");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(500);
      Serial.println(">>>>>>>>>>");
      delay(3000);

      etapa11estado_Sensor_Tensao_No_Borne1 = i2CciPCF8574Read( D0_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne1 );
      etapa11estado_Sensor_Tensao_No_Borne2Semanalmente = i2CciPCF8574Read( D1_NoChip_I2C_PCF8574P_32_Sensor_Tensao_No_Borne2 );
      contador++;
      delay(1000);
    }
  }
  if ((etapa11estado_Sensor_Tensao_No_Borne1 == 0) && (etapa11estado_Sensor_Tensao_No_Borne2Semanalmente == 1))
  {

    Serial.println("Se O Gerador não ligou e nem a concecionaria voltou então volta para etapa6");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    etapa11_FuncaoQueAcionaDeFatoOGeradorSemanalmente();
  }

  if ((etapa11estado_Sensor_Tensao_No_Borne1 == 0) && (etapa11estado_Sensor_Tensao_No_Borne2Semanalmente == 0))
  {
    Serial.println("Aqui na etapa11 foi concluido com exito e o Gerador irá ficar ligado por ums minutos");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);
    Serial.println(">>>>>>>>>>");
    delay(500);

    i2CciPCF8574Write( D9_NoChip_I2C_PCF8574P_33_Controla_Contactora_Que_Libera_Energia_Do_Gerador, 0 );
    i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
    delay(5000);


    contador = 0;
    delay(1000);

    

    etapa9_FuncaoQueTestaOGerador();
  }
}


//
//
//
//
//
//---------------------Esta função é apenas para agendar teste de gerador semanalmente via millis-------------------//


void FuncaoAgendaTesteDeGeradorSemanalmente(int intervalo){
  
if(millis() - FuncaoAgendaTesteDeGeradorSemanalmenteTempoAnteriorEmMillis >= intervalo ){
  
  Serial.println(millis());
  FuncaoAgendaTesteDeGeradorSemanalmenteTempoAnteriorEmMillis = millis();
  
  Serial.println("Solicitação de Acionamento Semanal do Gerador pra teste");
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 1 );
      delay(100);
      i2CciPCF8574Write( D2_NoChip_I2C_PCF8574P_32_Led_Monitora_Gerador, 0 );
      delay(1000);

      delay(500);
      Serial.println(">>>>>>>>>>>>>>>>>>>> OK >>>>>>>>>>>>>>>>>>>>");
  
  
  etapa11_FuncaoQueAcionaDeFatoOGeradorSemanalmente();

  
}
}



*/

//
//
//
//
//
//--------------------- Função usadas nas configurações OTA -------------------//


void firmwareUpdate(void) {
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  httpUpdate.setLedPin(LED_BUILTIN, LOW);
  t_httpUpdate_return ret = httpUpdate.update(client, URL_fw_Bin);

  switch (ret) {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("HTTP_UPDATE_OK");
    break;
  }
}
int FirmwareVersionCheck(void) {
  String payload;
  int httpCode;
  String fwurl = "";
  fwurl += URL_fw_Version;
  fwurl += "?";
  fwurl += String(rand());
  Serial.println(fwurl);
  WiFiClientSecure * client = new WiFiClientSecure;

  if (client) 
  {
    client -> setCACert(rootCACertificate);

    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is 
    HTTPClient https;

    if (https.begin( * client, fwurl)) 
    { // HTTPS      
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      delay(100);
      httpCode = https.GET();
      delay(100);
      if (httpCode == HTTP_CODE_OK) // if version received
      {
        payload = https.getString(); // save received version
      } else {
        Serial.print("Erro ao baixar arquivo com versão:");
        Serial.println(httpCode);
      }
      https.end();
    }
    delete client;
  }
      
  if (httpCode == HTTP_CODE_OK) // if version received
  {
    payload.trim();
    if (payload.equals(FirmwareVer)) {
      Serial.printf("\nDispositivo já com versão atual", FirmwareVer);
      return 0;
    } 
    else 
    {
      Serial.println(payload);
      Serial.println("===>>> OPA NOVA VERSÃO ENCONTRADA <<<===");
      return 1;
    }
  } 
  return 0;  
}

//--------------------- Fim de função usadas nas configurações OTA -------------------//



//
//
//
//
//
//--------------------- Função determina o modo de operação nos pinos dos Chips i2CciPCF8574Write-------------------//



bool i2CciPCF8574PinMode(byte pino, int modo) {
static byte modoPinos[quantosChipsI2C_PCF8574P];

  if (modo == -1) {
     return bitRead(modoPinos[pino / 8], pino % 8); 
  } else {
     bitWrite(modoPinos[pino / 8], (pino % 8), modo);
     return modo;
  }
}


//--------------------- Fim de Função determina o modo de operação nos pinos dos Chips i2CciPCF8574Write-------------------//


//
//
//
//
//
//--------------------- Função determina o stado nos pinos dos Chips i2CciPCF8574Write-------------------//

void i2CciPCF8574Write(byte pino, bool estado) {
static bool inicio = true;
static byte estadoPin[quantosChipsI2C_PCF8574P];

    if (inicio) {
       byte estadoCI;
       for (int nL = 0; nL < quantosChipsI2C_PCF8574P; nL++) {

           for (int nM = 0; nM < 8; nM++) {
               bitWrite(estadoCI, nM, !i2CciPCF8574PinMode(nM + (nL * 8)) );
           }
           estadoPin[nL] = estadoCI;
       }
       inicio = false;
    }

    bitWrite(estadoPin[pino / 8], pino % 8, estado);
    
    Wire.beginTransmission(enderecosPCF8574[pino / 8]);    
    Wire.write(estadoPin[pino / 8]);                            
    Wire.endTransmission();        
}


//--------------------- Fim de Função determina o stado nos pinos dos Chips i2CciPCF8574Write-------------------//



//
//
//
//
//
//--------------------- Função ler o stado nos pinos dos Chips i2CciPCF8574Write-------------------//


bool i2CciPCF8574Read(byte pino) {
byte lido;
bool estado;

   Wire.requestFrom ( enderecosPCF8574[pino / 8],1 );
   if(Wire.available()) {   
      lido = Wire.read();        
   }
       
   estado = bitRead(lido, pino % 8);
   return estado;  
}



//--------------------- Fim de Função ler o stado nos pinos dos Chips i2CciPCF8574Write-------------------//



//
//
//
//
//
//--------------------- Função é apenas piscar led usando millis-------------------//


void FuncaoPiscaLed(int porta, int tempo){
  
if(millis() - FuncaoPiscaLedtempoAnteriorEmMillis >= tempo ){
  
  //Serial.println(millis());
  FuncaoPiscaLedtempoAnteriorEmMillis = millis();
  
  i2CciPCF8574Write( porta, estadoled );
  estadoled = !estadoled;

  
}
}



//--------------------- Fim de Função é apenas piscar led usando millis-------------------//


//
//
//
//
//
//--------------------- Função é apenas enviar mensagem no serial usando millis-------------------//


void FuncaoMensagemNaSerial( String mensagem, int tempo ){
  
if(millis() - FuncaoMensagemNaSerialtempoAnteriorEmMillis >= tempo ){
  
  Serial.println(millis());
  FuncaoMensagemNaSerialtempoAnteriorEmMillis = millis();
  
  Serial.println(mensagem);

  
}
}


//--------------------- Fim de Função é apenas enviar mensagem no serial usando millis-------------------//



//
//
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Final de Funções
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%