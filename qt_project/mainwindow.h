#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDataStream>
#include <QDebug>
#include <QHostAddress>
#include <QMainWindow>
#include <QMessageBox>
#include <QStackedWidget>
#include <QTimer>
#include <QUdpSocket>

class GameWidget;
class HomeScreen;
class QStackedWidget;
class QTimer;
class QUdpSocket;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

private slots:
    void handleGameStart(bool isHost);
    void handleGameOver(const QString& result);
    void handleCountdown();
    void handleDatagramReadyRead();
    void sendDatagram(const QByteArray& data); // For GameWidget
    void sendDatagram(const QByteArray& data, QHostAddress peerAddress, quint16 peerPort); // For HomeScreen
    void handleRematchRequest();
    void handleOpponentQuit();
    void handleBindSocket(quint16 localPort);
    void handleOpponentRematchVote();
    void Reset();

private:
    // UI and Widget Pointers
    QStackedWidget *m_stackedWidget;
    HomeScreen *m_homeScreen;
    GameWidget *m_gameWidget;

    // Networking
    QUdpSocket *m_udpSocket;
    QHostAddress m_peerAddress;
    quint16 m_peerPort;
    QHostAddress m_myLocalAddress;
    bool m_connected;

    // Timers
    QTimer *m_countdownTimer;

    // Game State and Control
    int m_countdownValue;
    bool m_myRematchVote;
    bool m_opponentRematchVote;

    // Helper Methods
    void checkAndStartRematch();
};

#endif // MAINWINDOW_H
