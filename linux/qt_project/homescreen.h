#pragma once

#include <QApplication>
#include <QDataStream>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class HomeScreen; }
QT_END_NAMESPACE

class QUdpSocket;
class QTimer;

class HomeScreen : public QWidget {
    Q_OBJECT

public:
    explicit HomeScreen(QWidget *parent = nullptr);
    ~HomeScreen();

    void setStatus(const QString& status, bool isError = false);
    void resetUI();
    void showCountdown(int count);
    void showResultPage(const QString& result);
    void showConnectionPage();
    void setRematchStatus(const QString& status);
    void enableRematchButtons(bool enable);
    void setMyAddress(const QHostAddress& addr);

    // Public getters for data needed by MainWindow
    QHostAddress getPeerAddress() const { return m_peerAddress; }
    quint16 getPeerPort() const { return m_peerPort; }
    quint16 getLocalPort() const { return m_localPort; }
    bool isHost() const { return m_amIHost; }

signals:
    void startGame(bool isHost);
    void sendDatagram(const QByteArray& data, QHostAddress peerAddress, quint16 peerPort);
    void rematchRequested();
    void opponentVotedForRematch();
    void opponentQuit();
    void Reset();
    void bindSocket(quint16 localPort);

public slots:
    void onDatagramReceived(const QByteArray& data, QHostAddress sender, quint16 senderPort, bool isConnected);

private slots:
    void onConnectClicked();
    void onQuitClicked();
    void onRematchClicked();
    void onExitToHomeClicked();
    void sendReadyStatus();
    void on_reset_botton_clicked();
    void onInputChanged();

private:
    // UI Pointer
    Ui::HomeScreen *ui;

    // Boolean Flags
    bool m_isReady = false;
    bool m_opponentReady = false;
    bool m_amIHost = false;
    bool m_peerConnected = false;

    // Network Addresses and Ports
    QHostAddress m_peerAddress;
    quint16 m_peerPort;
    quint16 m_localPort;
    QHostAddress m_myAddress;

    // Timers
    QTimer *m_readyCheckTimer;

    // Helper Methods
    void checkReadyState();
    void sendSimpleDatagram(const QString& header);
    void sendRichDatagram(const QString& header);
};
