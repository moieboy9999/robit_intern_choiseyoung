#include "homescreen.h"
#include "ui_homescreen.h"
#include <QMessageBox>
#include <stdexcept>

HomeScreen::HomeScreen(QWidget *parent)
    : QWidget(parent), ui(new Ui::HomeScreen)
{
    try {
        ui->setupUi(this);
        connect(ui->connect_button, &QPushButton::clicked, this, &HomeScreen::onConnectClicked);
        connect(ui->quit_button, &QPushButton::clicked, this, &HomeScreen::onQuitClicked);
        connect(ui->rematch_button, &QPushButton::clicked, this, &HomeScreen::onRematchClicked);
        connect(ui->exit_to_home_button, &QPushButton::clicked, this, &HomeScreen::onExitToHomeClicked);
        connect(ui->reset_botton, &QPushButton::clicked, this, &HomeScreen::on_reset_botton_clicked);

        // Connect input fields to the validation slot
        connect(ui->local_port_input_line, &QLineEdit::textChanged, this, &HomeScreen::onInputChanged);
        connect(ui->port_input_line, &QLineEdit::textChanged, this, &HomeScreen::onInputChanged);
        connect(ui->ip_input_line, &QLineEdit::textChanged, this, &HomeScreen::onInputChanged);

        ui->countdownLabel->setVisible(false);
        ui->status_label->setText("Enter your local port, opponent IP and port, then click Connect.");

        m_readyCheckTimer = new QTimer(this);
        connect(m_readyCheckTimer, &QTimer::timeout, this, &HomeScreen::sendReadyStatus);

        // Initially disable the connect button
        ui->connect_button->setEnabled(false);

    } catch (const std::exception& e) {
        qCritical() << "Exception in HomeScreen constructor: " << e.what();
        QMessageBox::critical(this, "Fatal Error", "An unexpected error occurred during initialization. The application may be unstable.");
    } catch (...) {
        qCritical() << "Unknown exception in HomeScreen constructor.";
        QMessageBox::critical(this, "Fatal Error", "An unknown, unexpected error occurred during initialization.");
    }
}

HomeScreen::~HomeScreen() { delete ui; }

void HomeScreen::resetUI() {
    try {
        ui->connect_button->setText("Connect");
        ui->connect_button->setEnabled(false); // Disable on reset
        ui->ip_input_line->setEnabled(true);
        ui->port_input_line->setEnabled(true);
        ui->local_port_input_line->setEnabled(true);
        ui->status_label->setText("Enter your local port, opponent IP and port, then click Connect.");
        ui->status_label->setStyleSheet("color: black;"); // Reset color
        ui->countdownLabel->setVisible(false);

        // Reset all state variables to their defaults
        m_isReady = false;
        m_opponentReady = false;
        m_amIHost = false;
        m_peerConnected = false;
        m_peerAddress.clear();
        m_peerPort = 0;
        m_localPort = 0;
        m_myAddress.clear();

        if (m_readyCheckTimer && m_readyCheckTimer->isActive()) {
            m_readyCheckTimer->stop();
        }

        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(ui->Connection_page));
        qDebug() << "UI and state reset successfully.";

    } catch (const std::exception& e) {
        qCritical() << "Exception in resetUI: " << e.what();
        setStatus("Error during reset. Please restart.", true);
    } catch (...) {
        qCritical() << "Unknown exception in resetUI.";
        setStatus("An unknown error occurred during reset.", true);
    }
}

void HomeScreen::setStatus(const QString& status, bool isError) {
    if (!ui) return;
    ui->status_label->setText(status);
    ui->status_label->setStyleSheet(isError ? "color: red;" : "color: black;");
}

void HomeScreen::showCountdown(int count) {
    if (!ui) return;
    if (count > 0) {
        ui->countdownLabel->setVisible(true);
        ui->countdownLabel->setText(QString::number(count));
    } else {
        ui->countdownLabel->setVisible(false);
    }
}

void HomeScreen::showResultPage(const QString& result) {
    if (!ui) return;
    ui->result_text_box->setText(result);
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(ui->Result_page));
}

void HomeScreen::showConnectionPage() {
    if (!ui) return;
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(ui->Connection_page));
}

void HomeScreen::setRematchStatus(const QString& status) {
    if (!ui) return;
    ui->rematch_status_label->setText(status);
}

void HomeScreen::enableRematchButtons(bool enable) {
    if (!ui) return;
    ui->rematch_button->setEnabled(enable);
    ui->exit_to_home_button->setEnabled(enable);
}

void HomeScreen::setMyAddress(const QHostAddress& addr) {
    m_myAddress = addr;
    qDebug() << "HomeScreen: My local address set to " << m_myAddress.toString();
}

void HomeScreen::onConnectClicked() {
    try {
        QString localPortStr = ui->local_port_input_line->text();
        QString ip = ui->ip_input_line->text();
        QString peerPortStr = ui->port_input_line->text();
        bool okLocal, okPeer;

        quint16 localPort = localPortStr.toUShort(&okLocal);
        quint16 peerPort = peerPortStr.toUShort(&okPeer);

        if (!okLocal || localPort == 0) {
            QMessageBox::warning(this, "Invalid Input", "Please enter a valid, non-zero local port number.");
            return;
        }
        if (ip.isEmpty() || !okPeer || peerPort == 0) {
            QMessageBox::warning(this, "Invalid Input", "Please enter a valid opponent IP and non-zero port number.");
            return;
        }
        if (QHostAddress(ip).isNull()) {
            QMessageBox::warning(this, "Invalid Input", "The opponent IP address is not valid.");
            return;
        }


        m_localPort = localPort;
        m_peerAddress = QHostAddress(ip);
        m_peerPort = peerPort;

        qDebug() << "onConnectClicked: m_localPort=" << m_localPort << ", m_peerAddress=" << m_peerAddress << ", m_peerPort=" << m_peerPort;

        if (ui->connect_button->text() == "Connect") {
            setStatus("Ports and IP set. Click Ready to start.");
            ui->connect_button->setText("Ready");
            ui->connect_button->setEnabled(false);
            ui->ip_input_line->setEnabled(false);
            ui->port_input_line->setEnabled(false);
            ui->local_port_input_line->setEnabled(false);
            emit bindSocket(m_localPort);
            sendRichDatagram("CONNECT");
            return;
        }

        if (!m_peerConnected) {
            setStatus("Peer is not connected yet. Please wait...", true);
            return;
        }

        if (m_isReady) return;

        m_isReady = true;
        setStatus("Ready! Waiting for opponent...");
        ui->connect_button->setEnabled(false);

        m_readyCheckTimer->start(2000);

        sendRichDatagram("READY");

        qDebug() << "onConnectClicked: Sent READY. m_isReady=" << m_isReady;
        checkReadyState();

    } catch (const std::exception& e) {
        qCritical() << "Exception in onConnectClicked: " << e.what();
        setStatus("An error occurred during connection.", true);
    } catch (...) {
        qCritical() << "Unknown exception in onConnectClicked.";
        setStatus("An unknown error occurred during connection.", true);
    }
}

void HomeScreen::sendReadyStatus() {
    if (m_isReady && !m_opponentReady) {
        sendRichDatagram("READY");
        qDebug() << "sendReadyStatus: Periodically sending READY.";
    }
}

void HomeScreen::onDatagramReceived(const QByteArray& data, QHostAddress sender, quint16 senderPort, bool isConnected) {
    try {
        if (!isConnected || senderPort != m_peerPort) {
            qDebug() << "Ignoring datagram: Not connected or wrong port. Sender port: " << senderPort << ", Expected port: " << m_peerPort;
            return;
        }

        bool addressMatch = (sender == m_peerAddress || sender.isEqual(m_peerAddress, QHostAddress::TolerantConversion));
        if (!addressMatch) {
             qDebug() << "Ignoring datagram: Address mismatch. Sender: " << sender.toString() << ", Expected: " << m_peerAddress.toString();
             return;
        }

        QDataStream in(data);
        in.setVersion(QDataStream::Qt_5_15);
        QString header;
        in >> header;

        qDebug() << "Header: " << header;

        if (header == "READY") {
            QString opponentAddressStr;
            quint16 opponentPort = 0;
            in >> opponentAddressStr >> opponentPort;
            qDebug() << "Received READY from opponent address: " << opponentAddressStr;

            m_opponentReady = true;
            if (m_localPort > opponentPort) {
                m_amIHost = true;
            } else if (m_localPort < opponentPort) {
                m_amIHost = false;
            } else {
                m_amIHost = (m_myAddress.toString() > opponentAddressStr);
            }

            setStatus("Opponent is ready!");
            qDebug() << "Received READY. m_isReady=" << m_isReady << ", m_opponentReady=" << m_opponentReady << ", m_amIHost=" << m_amIHost;
            checkReadyState();
        } else if (header == "CONNECT") {
            QString peerIp; quint16 peerLocalPort = 0;
            in >> peerIp >> peerLocalPort;
            qDebug() << "Received CONNECT from" << peerIp << ":" << peerLocalPort;

            sendSimpleDatagram("CONNECT_ACK");

            m_peerConnected = true;
            setStatus("Peer connected. You can click Ready.");
            ui->connect_button->setEnabled(true);
        } else if (header == "CONNECT_ACK") {
            qDebug() << "Received CONNECT_ACK.";
            m_peerConnected = true;
            setStatus("Peer connected. You can click Ready.");
            ui->connect_button->setEnabled(true);
        } else if (header == "REMATCH_VOTE" || header == "REMATCH_REQ") {
            qDebug() << "Received REMATCH_VOTE.";
            emit opponentVotedForRematch();
        } else if (header == "RESET_GAME") {
            qDebug() << "Received RESET_GAME from peer.";
            setStatus("Opponent has reset. Please connect again.", false);
            emit Reset(); // Trigger a full local reset
        } else if (header == "QUIT_GAME") {
            qDebug() << "Received QUIT_GAME.";
            ui->reset_botton->setEnabled(true);
            emit opponentQuit();
        }
    } catch (const std::exception& e) {
        qCritical() << "Exception in onDatagramReceived: " << e.what();
        setStatus("Error processing received data.", true);
    } catch (...) {
        qCritical() << "Unknown exception in onDatagramReceived.";
        setStatus("Unknown error processing received data.", true);
    }
}

void HomeScreen::checkReadyState() {
    if (m_isReady && m_opponentReady) {
        ui->reset_botton->setEnabled(false);
        setStatus("Both players ready! Starting game...");
        if (m_readyCheckTimer->isActive()) {
            m_readyCheckTimer->stop();
        }
        qDebug() << "checkReadyState: Emitting startGame(m_amIHost=" << m_amIHost << ")";
        emit startGame(m_amIHost);
    }
}

void HomeScreen::onQuitClicked() {
    qDebug() << "onQuitClicked: Quitting application.";
    sendSimpleDatagram("QUIT_GAME");
    QApplication::quit();
}

void HomeScreen::onRematchClicked() {
    qDebug() << "onRematchClicked: Rematch vote (emit rematchRequested).";
    setRematchStatus("Waiting for opponent's vote...");
    enableRematchButtons(false);
    emit rematchRequested();
}

void HomeScreen::onExitToHomeClicked() {
    qDebug() << "onExitToHomeClicked: Sending QUIT_GAME and resetting UI.";
    ui->reset_botton->setEnabled(true);
    sendSimpleDatagram("QUIT_GAME");
    resetUI();
}

void HomeScreen::on_reset_botton_clicked() {
    try {
        qDebug() << "Reset button clicked.";

        // Inform the peer about the reset before doing it locally
        sendSimpleDatagram("RESET_GAME");

        // Emit signal for local reset (handled by MainWindow)
        emit Reset();

    } catch (const std::exception& e) {
        qCritical() << "Exception in on_reset_botton_clicked: " << e.what();
        QMessageBox::critical(this, "Error", "An unexpected error occurred while trying to reset.");
    } catch (...) {
        qCritical() << "Unknown exception in on_reset_botton_clicked.";
        QMessageBox::critical(this, "Error", "An unknown error occurred while trying to reset.");
    }
}

void HomeScreen::onInputChanged()
{
    const QString localPortStr = ui->local_port_input_line->text();
    const QString peerPortStr = ui->port_input_line->text();
    const QString peerIpStr = ui->ip_input_line->text();

    bool allFieldsFilled = !localPortStr.isEmpty() && !peerPortStr.isEmpty() && !peerIpStr.isEmpty();
    bool portsAreDifferent = localPortStr != peerPortStr;

    ui->connect_button->setEnabled(allFieldsFilled && portsAreDifferent);
}

void HomeScreen::sendSimpleDatagram(const QString& header) {
    if (m_peerAddress.isNull() || m_peerPort == 0) return;
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << header;
    emit sendDatagram(data, m_peerAddress, m_peerPort);
}

void HomeScreen::sendRichDatagram(const QString& header) {
    if (m_peerAddress.isNull() || m_peerPort == 0) return;
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << header << m_myAddress.toString() << m_localPort;
    emit sendDatagram(data, m_peerAddress, m_peerPort);
}

