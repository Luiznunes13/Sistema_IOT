#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Driver ESP8266 <-> MQTT");

    // 1. Inicializa a biblioteca mosquitto
    mosqpp::lib_init();

    // 2. Conecta ao broker MQTT
    logMessage("Conectando ao broker MQTT em " + m_mqttHost + "...");
    int result = mosqpp::mosquittopp::connect(m_mqttHost.toStdString().c_str(), m_mqttPort, 60);
    if(result != MOSQ_ERR_SUCCESS) {
        logMessage("Erro ao conectar ao MQTT.");
    }

    // Inicia o loop de rede do MQTT em uma thread separada
    loop_start();

    // 3. Configurar o Servidor TCP para ouvir o ESP8266
    m_server = new QTcpServer(this);
    // Especificando o connect do QObject para Sinais e Slots
    QObject::connect(m_server, &QTcpServer::newConnection, this, &MainWindow::handleNewConnection);

    if (!m_server->listen(QHostAddress::Any, 7001)) {
        logMessage("Erro: Servidor TCP nao pode iniciar!");
    } else {
        logMessage("Servidor TCP aguardando conexao na porta 7001...");
    }
}

MainWindow::~MainWindow()
{
    mosqpp::lib_cleanup(); // Limpa a biblioteca mosquitto
    delete ui;
}

// ---- Callbacks do MQTT ----
void MainWindow::on_connect(int rc)
{
    if (rc == 0) {
        logMessage("Conectado ao broker MQTT com sucesso!");

        // PASSO CRUCIAL FALTANTE: Se inscrever no t�pico de comando ap�s conectar.
        int sub_result = subscribe(NULL, m_mqttCommandTopic.toStdString().c_str());

        if(sub_result == MOSQ_ERR_SUCCESS) {
            logMessage("Inscrito no topico de comando: " + m_mqttCommandTopic);
        } else {
            logMessage("ERRO ao se inscrever no topico de comando!");
        }

    } else {
        logMessage("Falha na conexao com o broker MQTT.");
    }
}

void MainWindow::on_disconnect(int rc)
{
    logMessage("Desconectado do broker MQTT.");
}

void MainWindow::on_publish(int mid)
{
    logMessage("MQTT   >> Mensagem publicada com sucesso.");
}

// NOVA FUNO: Chamada quando uma mensagem chega em um tpico assinado
void MainWindow::on_message(const struct mosquitto_message *message)
{
    QString topic = QString(message->topic);
    QString payload = QString::fromLatin1((char*)message->payload, message->payloadlen);

    logMessage("MQTT << Mensagem recebida no topico '" + topic + "': " + payload);

    // Verifica se a mensagem  para o tpico de comando e se o socket est ativo
    if (topic == m_mqttCommandTopic && m_socket && m_socket->isOpen() && m_socket->isWritable())
    {
        // Encaminha o comando para o ESP8266 atravs do socket
        m_socket->write(payload.toUtf8() + '\n' );
        m_socket->flush(); // Garante o envio imediato
        logMessage("Comando '" + payload + "' encaminhado para o ESP8266.");
    }
}

// -------------------------

void MainWindow::handleNewConnection()
{
    m_socket = m_server->nextPendingConnection();
    // Resolvendo a ambiguidade para o connect de Sinais e Slots do Qt
    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::readSocketData);
    QObject::connect(m_socket, &QTcpSocket::disconnected, m_socket, &QTcpSocket::deleteLater);
    logMessage("Nova conexao TCP recebida: " + m_socket->peerAddress().toString());
}

void MainWindow::readSocketData()
{
    QByteArray data = m_socket->readAll();
    QString receivedData = QString::fromUtf8(data).trimmed();
    logMessage("Socket << " + receivedData);

    // Publicar dados no MQTT usando a nova biblioteca
    publish(NULL, m_mqttTopic.toStdString().c_str(), data.length(), data.constData());
}

void MainWindow::logMessage(const QString &msg)
{
    ui->logTextEdit->append(msg);
    qDebug() << msg;
}
