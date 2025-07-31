# 🚀 MiniDrive - Sistema IoT Bidirecional

Um sistema completo de IoT que demonstra comunicação bidirecional entre um **ESP8266**, um aplicativo **Qt (Raspberry Pi)** e um **dashboard Node-RED**.
Este projeto tem como foco o **treinamento prático de dois protocolos essenciais em projetos IoT**: **TCP com sockets** e **MQTT**.

- A comunicação **via socket TCP** permite a troca direta de dados entre dispositivos e servidores, com controle total sobre o fluxo.
- Já o **envio de dados via MQTT** fornece uma alternativa leve e escalável para transmissão de informações entre sistemas, podendo ser consumida por ferramentas como **Node-RED, Grafana, Electron**, ou qualquer front-end conectado.---

## 🧩 Arquitetura do Sistema

O projeto é composto por três camadas principais:

- **ESP8266** → Coleta dados de sensores e recebe comandos para controle de LED
- **Aplicativo Qt (Raspberry Pi)** → Ponte entre o ESP8266 (via TCP) e o MQTT
- **Dashboard Node-RED** → Interface para visualização e controle remoto
  
> 💡 *O Node-RED também está instalado no Raspberry Pi neste projeto, mas poderia ser hospedado em qualquer máquina conectada à mesma rede.*---

---

## 1️⃣ Firmware ESP8266

O firmware permite que o NodeMCU se conecte via Wi-Fi ao servidor Qt e envie dados periodicamente.

### 🔧 Características
- Envia leituras simuladas de temperatura a cada 5 segundos
- Recebe comandos remotos para ligar/desligar LED no pino **D0**
- Reconexão automática com servidor TCP

### 💡 Configuração de Hardware
- **LED externo:** conectado ao pino **D0 (GPIO16)**
- **Lógica de controle:** HIGH = aceso, LOW = apagado

### ⚙️ Parâmetros de rede no `miniDrive.ino`

```
// --- CONFIGURAÇÕES DE REDE ---
const char* ssid = "sua_rede_wifi";
const char* password = "sua_senha_wifi";
const char* host_ip = "ip_do_raspberry_pi"; // IP do servidor Qt
const uint16_t host_port = 7001;
```
2️⃣ Aplicativo Qt (Raspberry Pi)
Aplicação escrita em Qt/C++ que atua como servidor TCP para o ESP8266 e cliente MQTT para comunicação com o Node-RED.

🛠️ Características
Servidor TCP que escuta conexões do ESP8266

Cliente MQTT usando libmosquitto

Interface gráfica para logs e controle

🐧 Requisitos
Qt 5.x ou superior

libmosquitto-dev instalado no Raspberry Pi

Sistema Linux (testado no Raspberry Pi OS)

🔨 Compilação
```
cd miniDrive_qt_app
qmake
make
./miniDrive
```
3️⃣ Dashboard Node-RED
Painel de controle interativo acessível via navegador.

📋 Recursos
Gauge para exibir a temperatura em tempo real

Botão para acionar/desligar o LED no ESP8266

Log de mensagens MQTT para depuração

📦 Instalação
Importe Dashboard_ESP8266.json no Node-RED

Configure o broker MQTT (padrão: localhost:1883)

Clique em Deploy

Acesse o dashboard via navegador, se não instale o dashboard 2.0 no node-red

🔁 Fluxo de Comunicação

ESP8266 → Qt (TCP):        Envia temperatura
Qt → MQTT:                 Publica em esp8266/dados
Node-RED (Dashboard):      Lê dados e exibe no UI
Node-RED → MQTT:           Envia comando para esp8266/comando
Qt → ESP8266 (TCP):        Repassa comando para acionar LED


🔐 Segurança
Este projeto é demonstrativo. Para uso real, considere:

Autenticação MQTT

Comunicação via TLS/SSL

Restrições de acesso por IP ou firewall

📄 Licença
Distribuído sob a licença MIT. Consulte o arquivo LICENSE para mais detalhes.

🤝 Contribuições
Contribuições são bem-vindas!
Abra uma issue ou envie um pull request com melhorias, ideias ou correções.

Desenvolvido com 💙 para a comunidade IoT
