#include <ESP8266WiFi.h>

// --- CONFIGURACOES DE REDE ---
const char* ssid = "xxxxxxxxx";
const char* password = "xxxxxxxxxx";
const char* host_ip = "xxxxxxxxxxx"; // IP do seu Raspberry Pi
const uint16_t host_port = 7001;

// --- NOVA CONFIGURACAO DO PINO DO LED ---
#define EXTERNAL_LED_PIN D0 // D0 na placa NodeMCU e o pino GPIO16

// --- NOVA LOGICA PARA O LED EXTERNO (ATIVO ALTO) ---
// HIGH = LED aceso
// LOW = LED apagado
#define LED_ACESO HIGH
#define LED_APAGADO LOW
// -----------------------------------------

WiFiClient client;

void setup() {
  Serial.begin(115200);
  // Configura o novo pino como saida
  pinMode(EXTERNAL_LED_PIN, OUTPUT);
  // Garante que o LED externo comece apagado
  digitalWrite(EXTERNAL_LED_PIN, LED_APAGADO); 
  delay(10);

  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("Endereco IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // 1. Tenta (Re)conectar ao servidor se não estiver conectado
  if (!client.connected()) {
    if (!client.connect(host_ip, host_port)) {
      Serial.println("Falha na conexao com o servidor Qt.");
      delay(2000);
      return;
    }
    Serial.println("Conectado ao servidor!");
  }

  // 2. Lógica para receber comandos
  if (client.available()) {
    String command = client.readStringUntil('\n');
    command.trim();
    Serial.print("Comando recebido: ");
    Serial.println(command);

    // ALTERADO PARA CONTROLAR O NOVO PINO
    if (command.startsWith("LED_ON")) {
      digitalWrite(EXTERNAL_LED_PIN, LED_ACESO);
    } else if (command.startsWith("LED_OFF")) {
      digitalWrite(EXTERNAL_LED_PIN, LED_APAGADO);
    }
  }

  // 3. Lógica para enviar dados
  static unsigned long lastSendTime = 0;
  if (millis() - lastSendTime > 5000) {
    float temperatura = random(200, 300) / 10.0;
    String data_to_send = "temperatura:" + String(temperatura);
    
    client.println(data_to_send);
    Serial.print("Dado enviado: ");
    Serial.println(data_to_send);
    lastSendTime = millis();
  }
}