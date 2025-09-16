#include "mainwindow.h"
#include "homescreen.h"
#include "gamewidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "MainWindow: Constructor";
    m_stackedWidget = new QStackedWidget(this);
    m_homeScreen = new HomeScreen(this);
    m_gameWidget = new GameWidget(this);
    m_udpSocket = new QUdpSocket(this);
    m_countdownTimer = new QTimer(this);

    m_stackedWidget->addWidget(m_homeScreen);
    m_stackedWidget->addWidget(m_gameWidget);

    setCentralWidget(m_stackedWidget);
    m_stackedWidget->setCurrentWidget(m_homeScreen);

    // Connect UDP socket readyRead signal
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &MainWindow::handleDatagramReadyRead);
    qDebug() << "MainWindow: readyRead signal connected.";

    // Connect HomeScreen signals to MainWindow slots
    connect(m_homeScreen, &HomeScreen::startGame, this, &MainWindow::handleGameStart);
    connect(m_homeScreen, QOverload<const QByteArray&, QHostAddress, quint16>::of(&HomeScreen::sendDatagram), this, static_cast<void(MainWindow::*)(const QByteArray&, QHostAddress, quint16)>(&MainWindow::sendDatagram));
    connect(m_homeScreen, &HomeScreen::rematchRequested, this, &MainWindow::handleRematchRequest);
    connect(m_homeScreen, &HomeScreen::Reset, this, &MainWindow::Reset);

    connect(m_homeScreen, &HomeScreen::opponentQuit, this, &MainWindow::handleOpponentQuit);
    connect(m_homeScreen, &HomeScreen::bindSocket, this, &MainWindow::handleBindSocket);
    connect(m_homeScreen, &HomeScreen::opponentVotedForRematch, this, &MainWindow::handleOpponentRematchVote);

    // Connect GameWidget signals to MainWindow slots
    connect(m_gameWidget, &GameWidget::gameOver, this, &MainWindow::handleGameOver);
    connect(m_gameWidget, &GameWidget::sendDatagram, this, static_cast<void(MainWindow::*)(const QByteArray&)>(&MainWindow::sendDatagram));

    // Connect countdown timer
    connect(m_countdownTimer, &QTimer::timeout, this, &MainWindow::handleCountdown);
}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow";
}



void MainWindow::handleGameStart(bool isHost) {
    qDebug() << "MainWindow: handleGameStart(isHost=" << isHost << ")";
    m_peerAddress = m_homeScreen->getPeerAddress();
    m_peerPort = m_homeScreen->getPeerPort();

    m_countdownValue = 3;
    m_homeScreen->showCountdown(m_countdownValue);
    m_countdownTimer->start(1000); // 1 second interval
}

void MainWindow::handleCountdown() {
    qDebug() << "MainWindow: handleCountdown. Value: " << m_countdownValue;
    m_countdownValue--;
    if (m_countdownValue > 0) {
        m_homeScreen->showCountdown(m_countdownValue);
    } else {
        m_countdownTimer->stop();
        m_homeScreen->showCountdown(0); // Hide countdown
        m_stackedWidget->setCurrentWidget(m_gameWidget);
        m_gameWidget->startGame(m_homeScreen->isHost());
        m_gameWidget->setFocus();
        qDebug() << "MainWindow: Countdown finished. Starting game.";
    }
}

void MainWindow::handleGameOver(const QString& result) {
    qDebug() << "MainWindow: handleGameOver. Result: " << result;
    m_stackedWidget->setCurrentWidget(m_homeScreen);
    m_homeScreen->showResultPage(result);
    m_homeScreen->enableRematchButtons(true);
    m_myRematchVote = false;
    m_opponentRematchVote = false;


}

void MainWindow::handleDatagramReadyRead() {
    qDebug() << "MainWindow: handleDatagramReadyRead() called. Bytes available: " << m_udpSocket->bytesAvailable();
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        qDebug() << "MainWindow: Received datagram from " << sender << ":" << senderPort << ", size=" << datagram.size();

          if (m_stackedWidget->currentWidget() == m_homeScreen) {
            m_homeScreen->onDatagramReceived(datagram, sender, senderPort, m_connected);
        } else if (m_stackedWidget->currentWidget() == m_gameWidget) {
            m_gameWidget->onDatagramReceived(datagram);
        }
    }
}

// This slot is for GameWidget to send data, uses stored peer address/port
void MainWindow::sendDatagram(const QByteArray& data) {
    qDebug() << "MainWindow: sendDatagram (from GameWidget). Socket state: " << m_udpSocket->state();
    if (m_peerAddress.isNull() || m_peerPort == 0) {
        qDebug() << "MainWindow: Peer address or port not set. Cannot send datagram (from GameWidget).";
        return;
    }
    qDebug() << "MainWindow: Sending datagram (from GameWidget) to " << m_peerAddress << ":" << m_peerPort << ", size=" << data.size();
    m_udpSocket->writeDatagram(data, m_peerAddress, m_peerPort);
}

// This slot is for HomeScreen to send data, uses provided peer address/port
void MainWindow::sendDatagram(const QByteArray& data, QHostAddress peerAddress, quint16 peerPort) {
    qDebug() << "MainWindow: sendDatagram (from HomeScreen). Socket state: " << m_udpSocket->state();
    if (peerAddress.isNull() || peerPort == 0) {
        qDebug() << "MainWindow: Peer address or port not set. Cannot send datagram (from HomeScreen).";
        return;
    }
    qDebug() << "MainWindow: Sending datagram (from HomeScreen) to " << peerAddress << ":" << peerPort << ", size=" << data.size();
    m_udpSocket->writeDatagram(data, peerAddress, peerPort);
}

void MainWindow::handleRematchRequest() {
    qDebug() << "MainWindow: handleRematchRequest (My click).";
    m_myRematchVote = true;

    // Send REMATCH_VOTE to opponent
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << QString("REMATCH_VOTE"); // New message type
    sendDatagram(dat, m_homeScreen->getPeerAddress(), m_homeScreen->getPeerPort());

    m_homeScreen->setRematchStatus("Waiting for opponent's vote...");
    checkAndStartRematch();
}

void MainWindow::handleOpponentRematchVote() {
    qDebug() << "MainWindow: handleOpponentRematchVote (Received vote).";
    m_opponentRematchVote = true;
    m_homeScreen->enableRematchButtons(true); // Enable my buttons to accept
    checkAndStartRematch();
}

void MainWindow::checkAndStartRematch() {
    if (m_myRematchVote && m_opponentRematchVote) {
        qDebug() << "MainWindow: Both voted for rematch. Starting game.";
        m_homeScreen->setStatus("Starting new game...");
        m_gameWidget->startGame(m_homeScreen->isHost());
        m_stackedWidget->setCurrentWidget(m_gameWidget);
        m_gameWidget->setFocus();
        m_myRematchVote = false; // Reset flags for next game
        m_opponentRematchVote = false;
    }
}



void MainWindow::handleOpponentQuit() {
    qDebug() << "MainWindow: handleOpponentQuit.";
    m_homeScreen->setStatus("Opponent left the game.", true);
    m_homeScreen->resetUI();
    m_stackedWidget->setCurrentWidget(m_homeScreen);
    // Unbind socket after opponent quits
    m_udpSocket->close();
    qDebug() << "MainWindow: UDP socket closed after opponent quit.";
}

void MainWindow::Reset(){
    qDebug() << " reset.";
    m_udpSocket->close();
    m_homeScreen->resetUI();
}
void MainWindow::handleBindSocket(quint16 localPort) {
    if (m_udpSocket->state() != QAbstractSocket::UnconnectedState) {
        m_udpSocket->close();
    }
    if (!m_udpSocket->bind(QHostAddress::Any, localPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        qDebug() << "MainWindow: UDP bind failed: " << m_udpSocket->errorString();
        QMessageBox::critical(this, "UDP Error", "Could not bind UDP socket to port " + QString::number(localPort) + ": " + m_udpSocket->errorString());
        m_homeScreen->resetUI();
        return;
    }
    m_connected = true;
    m_myLocalAddress = m_udpSocket->localAddress();
    if (m_myLocalAddress == QHostAddress::Any || m_myLocalAddress == QHostAddress::AnyIPv6) {
        m_myLocalAddress = QHostAddress::LocalHost; // Use 127.0.0.1 or ::1
    }
    m_homeScreen->setMyAddress(m_myLocalAddress); // Pass my local IP to HomeScreen
    qDebug() << "MainWindow: UDP socket bound to port " << localPort << " in handleBindSocket.";
}
