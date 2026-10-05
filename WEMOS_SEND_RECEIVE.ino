//----------BIBLIOTECAS----------//
#include <esp_now.h>
#include <WiFi.h>
#include "Adafruit_VL53L0X.h"
//--------BIBLIOTECAS/FIM----------//

// MAC das placas que receberão a mensagem
uint8_t broadcastAddress1[] = { 0x7C, 0x9E, 0xBD, 0xF5, 0x29, 0xE4 };  // DEVKIT-PC
// uint8_t broadcastAddress2[] = {0x7C, 0x9E, 0xBD, 0xF5, 0x29, 0xE4}; // Íris Futura

//----------DEFINIÇÕES----------//
#define PWM_out 27
#define LED_medicao 26
//--------DEFINIÇÕES/FIM----------//


//----------FUNÇÕES EXTERNAS----------//
void SENSOR_DISTANCIA();
void PWM();
//--------FUNÇÕES EXTERNAS/FIM----------//


//----------VARIÁVEIS GLOBAIS----------//
// Variável que identifica o sensor de distância
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
bool controle = false;
//--------VARIÁVEIS GLOBAIS/FIM----------//

//----------ESP-NOW----------//
// Exemplo de estrutura para receber dados
// Deve combinar com a estrutura de envio
typedef struct tunel {
  int altura = 0;
  double PWM;
  bool start = false;
} tunel;

// Cria uma instancia chamada dados a partir da struct template tunel
tunel dados;

esp_now_peer_info_t peerInfo;

// Função de Callback que será executada quando um dado é enviado
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {Serial.println("Envio falhou"); controle = false;}
}

// Função de Callback que será executada quando um dado é recebido
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&dados, incomingData, sizeof(dados));
  controle = dados.start;
  set_PWM(dados.PWM);
}
//----------FIM - ESP-NOW----------//

void setup() {
  Serial.begin(115200);
  // Prepara o LED indicativo de medição
  pinMode(LED_medicao, OUTPUT);
  digitalWrite(LED_medicao, LOW);

  WiFi.mode(WIFI_STA);

  // Funções de preparação:
  sensor_setup();
  pwm_setup();

  // Inicia o PWM em 0
  set_PWM(0);

  // Inicia o ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Assim que o ESPNow é iniciado, registra-se o send CB
  // para pegar os status da transmissão dos pacotes
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

// Pareamento do segundo endereço 
  // memcpy(peerInfo.peer_addr,broadcastAddress2, 6);
  // if (esp_now_add_peer(&peerInfo) != ESP_OK){
  //   Serial.println("Failed to add peer");
  //   return;
  // }

  // Assim que o ESPNow é iniciado, registra-se o recv CB
  // para pegar as informações do pacote recv
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if(controle) {
    dados.altura = medir_altura();
    esp_now_send(broadcastAddress1, (uint8_t *)&dados, sizeof(dados));
    // Envio para o segundo endereço
    // esp_now_send(broadcastAddress2, (uint8_t *)&dados, sizeof(dados));
  }
}