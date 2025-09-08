#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sock(new QUdpSocket(this))//소캣라이브러꺼
{
    ui->setupUi(this);

    // 여기서 내 IP/포트 바인드
    QHostAddress myAddr("172.100.7.254");   // 본인 PC IP
    quint16 myPort = 45455;                // 수신용 포트
    sock->bind(myAddr, myPort,
               QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    // 수신 이벤트 연결
    connect(sock, &QUdpSocket::readyRead, this, &MainWindow::onReadyRead);
}
MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_clicked() {
    QString msg = ui->send->toPlainText();
    QByteArray dat = msg.toUtf8();

    // 상대방 IP와 포트
    QHostAddress peer("172.100.4.199");
    sock->writeDatagram(dat, peer, 45454);
    ui->textEdit->append("나: "+msg);
    ui->send->clear();
}

void MainWindow::onReadyRead() {
    while (sock->hasPendingDatagrams()) {
        QByteArray buf;
        buf.resize(int(sock->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        sock->readDatagram(buf.data(), buf.size(), &sender, &senderPort);
        ui->textEdit->append("상대: " + QString::fromUtf8(buf));
    }
}
