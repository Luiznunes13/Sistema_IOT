#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <mosquittopp.h> // Usamos a nova biblioteca

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// A classe agora herda de mosquittopp para ter a funcionalidade MQTT
class MainWindow : public QMainWindow, public mosqpp::mosquittopp
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleNewConnection();
    void readSocketData();

private:
    // Callbacks da biblioteca Mosquitto
    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_publish(int mid);
    void on_message(const struct mosquitto_message *message);

    void logMessage(const QString &msg);

    Ui::MainWindow *ui;
    QTcpServer *m_server;
    QTcpSocket *m_socket;

    // --- CONFIGURACOES MQTT ---
    const QString m_mqttHost = "127.0.0.1";
    const int m_mqttPort = 1883;
    const QString m_mqttTopic = "esp8266/dados";
    const QString m_mqttCommandTopic = "esp8266/comando";
    // -------------------------
};
#endif // MAINWINDOW_H
