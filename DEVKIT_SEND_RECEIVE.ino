//----------BIBLIOTECAS----------//
#include <esp_now.h>
#include <WiFi.h>
//--------BIBLIOTECAS/FIM----------//

// Comandos possíveis
#define STANDBY 0        // Aguardando instrução
#define START 1          // Inicia a aquisição de dados no Wemos
#define SENSOR_ALTURA 2  // Pegar o último valor de altura recebido
#define ENVIAR_PWM 3     // Enviar o PWM

// Globais
char comando;  // caractere para escolha da função
uint8_t acao = STANDBY; // variável de ação (default = STANDBY)
bool novo_dado = false; // variável caso receba uma string, usada em conjunto com recebe_valor()
char string_recebida[32]; // variável que armazena a string, usada em conjunto com recebe_valor()

// MAC das placas que receberão a mensagem
uint8_t broadcastAddress[] = { 0xF0, 0x08, 0xD1, 0xD4, 0x05, 0x08 }; // WEMOS

//----------ESP-NOW----------//
// Exemplo de estrutura para receber dados
// Deve combinar com a estrutura de envio
typedef struct tunel {
  int altura = 0;
  double PWM = 0;
  bool start = true;
} tunel;

// Cria uma instancia chamada dados a partir da struct template tunel
tunel dados;

esp_now_peer_info_t peerInfo;

// Função de Callback que será executada quando um dado é enviado
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if(status != ESP_NOW_SEND_SUCCESS) {Serial.println("Envio falhou");}
}

// Função de Callback que será executada quando um dado é recebido
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&dados, incomingData, sizeof(dados));
}
//----------FIM - ESP-NOW----------//

void serialFlush() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void recebe_valor() {
  static byte idx = 0;

  while (!novo_dado) {
    if (Serial.available() > 0) {
      char rc = Serial.read();

      if (rc != '\n') {
        string_recebida[idx] = rc;
        idx++;
      } 
      else {
        string_recebida[idx] = '\0';  // finaliza a string
        idx = 0;
        novo_dado = true;
      }
    }
  }
  novo_dado = false;
}

uint8_t interpretador(char c) {
  switch (c) {
    case 'i': return START;

    case 's': return SENSOR_ALTURA;

    case 'p': return ENVIAR_PWM;

    default:  return STANDBY;
  }
}

void define_acao() {
  switch (acao) {
    case STANDBY:
      break;

    case START:
        esp_now_send(broadcastAddress, (uint8_t *)&dados, sizeof(dados));
        dados.start = !dados.start;
      break;

    case SENSOR_ALTURA:
      {
        Serial.println(dados.altura);
      }
      break;

    case ENVIAR_PWM:
      {
        recebe_valor();
        dados.PWM = atof(string_recebida);
        esp_now_send(broadcastAddress, (uint8_t *)&dados, sizeof(dados));
      }
      break;

    default: break;
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  // Inicia o ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Assim que o ESPNow é iniciado, registra-se o send CB
  // para pegar os status da transmissão dos pacotes
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Assim que o ESPNow é iniciado, registra-se o recv CB
  // para pegar as informações do pacote recv
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

  if (Serial.available() > 0) {
    comando = Serial.read();
    if (comando != '\n') {
      acao = interpretador(comando);
      serialFlush();
      define_acao();
    }
    else {
      serialFlush();
    }
  }
}