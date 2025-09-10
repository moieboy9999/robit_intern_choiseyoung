#include "gamewidget.h"
#include "ui_gamewidget.h"


const int WINNING_SCORE = 10;
const int MAX_HEALTH = 3;
const qreal ITEM_DIAMETER = 60.0;

const qreal PADDLE_SPEED = 7.0;
const qreal BALL_SPEED_INCREMENT = 0.2;
const qreal INITIAL_BALL_SPEED = 4.0;

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameWidget), m_scene(new QGraphicsScene(this))
{
    qDebug() << "GameWidget: Constructor";
    ui->setupUi(this);
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    this->setFocusPolicy(Qt::StrongFocus);

    m_scene->setSceneRect(-800, -450, 1600, 900);

    // 골대, 벽
    qreal goalHeight = 300; qreal wallHeight = (900 - goalHeight) / 2;
    m_goal1 = new QGraphicsRectItem(-800, -goalHeight / 2, 10, goalHeight); m_goal1->setBrush(Qt::red); m_scene->addItem(m_goal1);
    m_goal2 = new QGraphicsRectItem(790, -goalHeight / 2, 10, goalHeight); m_goal2->setBrush(Qt::blue); m_scene->addItem(m_goal2);
    createWall(-800, -450, 1600, 10); // top_border
    createWall(-800, 440, 1600, 10);  // bottom_border
    createWall(-800, -450, 10, wallHeight); // wall1
    createWall(-800, 150, 10, wallHeight);  // wall2
    createWall(790, -450, 10, wallHeight);  // wall3
    createWall(790, 150, 10, wallHeight);  // wall4



    // 패들, 공
    m_paddle1 = new QGraphicsEllipseItem(0, 0, 100, 100); m_paddle1->setBrush(QColor("#00FFFF")); m_scene->addItem(m_paddle1);
    m_paddle2 = new QGraphicsEllipseItem(0, 0, 100, 100); m_paddle2->setBrush(QColor("#CCCCCC")); m_scene->addItem(m_paddle2);
    m_ellipseItem = new QGraphicsEllipseItem(0, 0, 20, 20); m_ellipseItem->setBrush(QColor("#2C3E50")); m_scene->addItem(m_ellipseItem);

    // 점수,게임오버
    QColor scoreTextColor = QColor("#2C3E50"); scoreTextColor.setAlpha(50);
    m_player1ScoreText = new QGraphicsTextItem(""); m_player1ScoreText->setFont(QFont("Arial", 150, QFont::Bold)); m_player1ScoreText->setDefaultTextColor(scoreTextColor); m_player1ScoreText->setZValue(1); m_scene->addItem(m_player1ScoreText);
    m_player2ScoreText = new QGraphicsTextItem(""); m_player2ScoreText->setFont(QFont("Arial", 150, QFont::Bold)); m_player2ScoreText->setDefaultTextColor(scoreTextColor); m_player2ScoreText->setZValue(1); m_scene->addItem(m_player2ScoreText);
    m_gameOverText = new QGraphicsTextItem(""); m_gameOverText->setFont(QFont("Arial", 80, QFont::Bold)); m_gameOverText->setDefaultTextColor(Qt::yellow); m_gameOverText->setZValue(2); m_scene->addItem(m_gameOverText);

    //  Health Bars
    for (int i = 0; i < MAX_HEALTH; ++i) {
        QGraphicsRectItem *p1_hp = new QGraphicsRectItem(0, 0, 50, 15); p1_hp->setBrush(Qt::green); p1_hp->setPos(-780 + (i * 60), 420); m_player1HealthBars.append(p1_hp); m_scene->addItem(p1_hp);
        QGraphicsRectItem *p2_hp = new QGraphicsRectItem(0, 0, 50, 15); p2_hp->setBrush(Qt::green); p2_hp->setPos(770 - (i * 60) - 50, 420); m_player2HealthBars.append(p2_hp); m_scene->addItem(p2_hp);
    }

    //  Item Spawn Areas
    m_SpawnArea = QRectF(-250, -410, 500, 820);

    //  Timers
    m_animationTimer = new QTimer(this); connect(m_animationTimer, &QTimer::timeout, this, &GameWidget::updateAnimation);
    m_itemRelocationTimer = new QTimer(this); connect(m_itemRelocationTimer, &QTimer::timeout, this, &GameWidget::relocateItems);
}

GameWidget::~GameWidget() { delete ui; }

void GameWidget::startGame(bool isHost) {
    m_isMovingUp1 = m_isMovingDown1 = m_isMovingLeft1 = m_isMovingRight1 = false;
    qDebug() << "GameWidget: startGame(isHost=" << isHost << ")";
    m_isHost = isHost;
    resetGame();
    this->setFocus();
}

void GameWidget::resetGame() {
    qDebug() << "GameWidget: resetGame. isHost=" << m_isHost;
    m_player1Score = 0; m_player2Score = 0;
    m_player1Health = MAX_HEALTH; m_player2Health = MAX_HEALTH;
    updateScoreDisplay(); updateHealthDisplay();

    // Set initial paddle positions based on whether this is the host or client
    if (m_isHost) {
        // Host is Player 1 (left side)
        m_paddle1->setPos(-750 - 50, -50);
        m_paddle2->setPos(750 - 50, -50);
    } else {
        // Client is Player 2 (right side)
        m_paddle1->setPos(750 - 50, -50);
        m_paddle2->setPos(-750 - 50, -50);
    }
    m_ellipseItem->setPos(0, 0);

    normalizeBall();
    m_lastPlayerToHitBall = 0;
    m_ellipseSpeedX = 4.0; m_ellipseSpeedY = (QRandomGenerator::global()->bounded(2) == 0 ? 1 : -1) * 4.0;

    if (m_healthItem) { m_scene->removeItem(m_healthItem); delete m_healthItem; m_healthItem = nullptr; }
    if (m_sawItem) { m_scene->removeItem(m_sawItem); delete m_sawItem; m_sawItem = nullptr; }

    if(m_isHost) {
        spawnItem(&m_healthItem, Qt::magenta);
        spawnItem(&m_sawItem, Qt::yellow);
    }

    m_gameOverText->setVisible(false);
    m_animationTimer->start(16); // ~60 FPS
}

void GameWidget::endGame(const QString& winner) {
    qDebug() << "GameWidget: endGame. Winner: " << winner;
    m_animationTimer->stop();
    m_itemRelocationTimer->stop();
    if (m_isHost) {
            QByteArray data;
            QDataStream out(&data, QIODevice::WriteOnly);
            out << QString("GAME_OVER") << winner;
            emit sendDatagram(data); // MainWindow가 peer로 전송
            qDebug() << "GameWidget: Sent GAME_OVER to client.";
        }
    emit gameOver(winner);
}

void GameWidget::sendPaddlePosition() {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << QString("INPUT");
    out << m_paddle1->x() << m_paddle1->y();
    emit sendDatagram(data);
    qDebug() << "GameWidget: Sent INPUT:" << m_paddle1->x() << m_paddle1->y();
}

void GameWidget::onDatagramReceived(const QByteArray& data) {
    qDebug() << "GameWidget: onDatagramReceived. Data size: " << data.size();
    QDataStream in(data);
    QString header;
    in >> header;

    qDebug() << "GameWidget: Received header: " << header;

    if (header == "STATE" && !m_isHost) {
        qreal p1x, p1y, p2x, p2y, ballx, bally;
        int p1score, p2score, p1health, p2health;
        bool isSawBall, healthItemExists, sawItemExists;
        qreal healthItemX, healthItemY, sawItemX, sawItemY;

        in >> p1x >> p1y >> p2x >> p2y >> ballx >> bally
           >> p1score >> p2score >> p1health >> p2health
           >> isSawBall
           >> healthItemExists >> healthItemX >> healthItemY
           >> sawItemExists    >> sawItemX    >> sawItemY;
        m_paddle2->setPos(p1x, p1y);
        m_ellipseItem->setPos(ballx, bally);

        m_player1Score = p1score; m_player2Score = p2score;
        m_player1Health = p1health; m_player2Health = p2health;
        updateScoreDisplay(); updateHealthDisplay();

        // Update ball color if saw ball
        if (isSawBall != m_isSawBladeBall) {
                m_isSawBladeBall = isSawBall;
                m_ellipseItem->setBrush(m_isSawBladeBall ? QColor("#FFFFE0") : QColor("#2C3E50"));
            }

        // Update items
        if (healthItemExists) {
            if (!m_healthItem) {
                m_healthItem = new QGraphicsEllipseItem(0, 0, ITEM_DIAMETER, ITEM_DIAMETER);
                m_healthItem->setBrush(Qt::magenta);
                m_scene->addItem(m_healthItem);
            }
            m_healthItem->setPos(healthItemX, healthItemY);
        } else {
            if (m_healthItem) {
                m_scene->removeItem(m_healthItem);
                delete m_healthItem;
                m_healthItem = nullptr;
            }
        }

        if (sawItemExists) {
            if (!m_sawItem) {
                m_sawItem = new QGraphicsEllipseItem(0, 0, ITEM_DIAMETER, ITEM_DIAMETER);
                m_sawItem->setBrush(Qt::yellow);
                m_scene->addItem(m_sawItem);
            }
            m_sawItem->setPos(sawItemX, sawItemY);
        } else {
            if (m_sawItem) {
                m_scene->removeItem(m_sawItem);
                delete m_sawItem;
                m_sawItem = nullptr;
            }
        }
        qDebug() << "GameWidget: Client updated state.";

    } else if (header == "INPUT" && m_isHost) {
        qreal remoteX, remoteY;   // ← 아래 2)에서 x도 받도록 바꿀 예정
        in >> remoteX >> remoteY;

        m_paddle2->setPos(remoteX, remoteY);
        qDebug() << "GameWidget: Host received INPUT: " << remoteX << remoteY;
    } else if (header == "GAME_OVER" && !m_isHost) {
           // NEW: 클라도 즉시 종료 처리
           QString winner; in >> winner;
           qDebug() << "GameWidget: Client received GAME_OVER:" << winner;
           m_animationTimer->stop();
           m_itemRelocationTimer->stop();
           emit gameOver(winner);
       }
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;
    switch (event->key()) {
        case Qt::Key_W: m_isMovingUp1 = true; break;
        case Qt::Key_S: m_isMovingDown1 = true; break;
        case Qt::Key_A: m_isMovingLeft1 = true; break;
        case Qt::Key_D: m_isMovingRight1 = true; break;
        default: QWidget::keyPressEvent(event);
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;
    switch (event->key()) {
        case Qt::Key_W: m_isMovingUp1 = false; break;
        case Qt::Key_S: m_isMovingDown1 = false; break;
        case Qt::Key_A: m_isMovingLeft1 = false; break;
        case Qt::Key_D: m_isMovingRight1 = false; break;
        default: QWidget::keyReleaseEvent(event);
    }
}

void GameWidget::updateAnimation() {
    // Player 1 (local player) movement
    if (m_isMovingUp1) m_paddle1->moveBy(0, -7.0);
    if (m_isMovingDown1) m_paddle1->moveBy(0, 7.0);
    if (m_isMovingLeft1)  m_paddle1->moveBy(-7.0, 0);
    if (m_isMovingRight1) m_paddle1->moveBy( 7.0, 0);
    // Clamp paddle position
    QRectF r = m_scene->sceneRect();
    qreal p1MinX, p1MaxX;
    qreal oneThirdWidth = r.width() / 3.0; // Calculate 1/3 of the scene width

    if (m_isHost) { // Player 1 (Host) - left side
        p1MinX = r.left();
        p1MaxX = r.left() + oneThirdWidth - m_paddle1->rect().width();
    } else { // Player 2 (Client) - right side
        p1MinX = r.right() - oneThirdWidth;
        p1MaxX = r.right() - m_paddle1->rect().width();
    }
    qreal p1X = qBound(p1MinX, m_paddle1->x(), p1MaxX);
    qreal p1Y = qBound(r.top(),   m_paddle1->y(), r.bottom() - m_paddle1->rect().height());
    m_paddle1->setPos(p1X, p1Y);

    // Send my new position to opponent
    if (!m_isHost) {
        sendPaddlePosition();
    }
    // HOST ONLY: Run the game simulation
    if (m_isHost) {
        QRectF sceneRect = m_scene->sceneRect();


        //  Ball Movement & Wall Collision
        m_ellipseItem->moveBy(m_ellipseSpeedX, m_ellipseSpeedY);
        QRectF ellipseRect = m_ellipseItem->sceneBoundingRect();
        if (ellipseRect.top() <= sceneRect.top()) { m_ellipseSpeedY *= -1; m_ellipseItem->setY(sceneRect.top()); }
        if (ellipseRect.bottom() >= sceneRect.bottom()) { m_ellipseSpeedY *= -1; m_ellipseItem->setY(sceneRect.bottom() - ellipseRect.height()); }
        if (ellipseRect.left() <= sceneRect.left() && !m_ellipseItem->collidesWithItem(m_goal1)) { m_ellipseSpeedX *= -1; m_ellipseItem->setX(sceneRect.left()); }
        if (ellipseRect.right() >= sceneRect.right() && !m_ellipseItem->collidesWithItem(m_goal2)) { m_ellipseSpeedX *= -1; m_ellipseItem->setX(sceneRect.right() - ellipseRect.width()); }

        //  Goal Collision
        if (m_ellipseItem->collidesWithItem(m_goal1) || m_ellipseItem->collidesWithItem(m_goal2)) {
            const bool scoredLeft = m_ellipseItem->collidesWithItem(m_goal1); // <-- 먼저 저장
            if (scoredLeft) m_player2Score++; else m_player1Score++;
            updateScoreDisplay();
            if (m_player1Score >= WINNING_SCORE) { endGame("PLAYER 1 WINS!"); return; }
            if (m_player2Score >= WINNING_SCORE) { endGame("PLAYER 2 WINS!"); return; }

            // 중앙 리스폰 후, "어느 골이었는지" 저장값으로 방향 결정
            m_ellipseItem->setPos(0, 0);
            m_ellipseSpeedX = scoredLeft ?  +INITIAL_BALL_SPEED  // 왼쪽 골에 들어갔으면 오른쪽으로
                                         :  -INITIAL_BALL_SPEED; // 오른쪽 골이면 왼쪽으로
            m_ellipseSpeedY = (QRandomGenerator::global()->bounded(2) == 0 ? 1 : -1) * INITIAL_BALL_SPEED;
            normalizeBall();
        }

        //  Item Collision
        if (m_healthItem && m_ellipseItem->collidesWithItem(m_healthItem)) {
            if (m_lastPlayerToHitBall == 1 && m_player1Health < MAX_HEALTH) m_player1Health++;
            else if (m_lastPlayerToHitBall == 2 && m_player2Health < MAX_HEALTH) m_player2Health++;
            updateHealthDisplay();
            m_scene->removeItem(m_healthItem); delete m_healthItem; m_healthItem = nullptr;
            QTimer::singleShot(5000, [this]() { spawnItem(&m_healthItem, Qt::magenta); });
        }
        if (m_sawItem && m_ellipseItem->collidesWithItem(m_sawItem)) {
            m_isSawBladeBall = true; m_ellipseItem->setBrush(QColor("#FFFFE0"));
            QTimer::singleShot(5000, this, &GameWidget::normalizeBall);
            m_scene->removeItem(m_sawItem); delete m_sawItem; m_sawItem = nullptr;
            QTimer::singleShot(5000, [this]() { spawnItem(&m_sawItem, Qt::yellow); });
        }

        //  Paddle Collision
        handlePaddleCollision(m_paddle1, true);
        handlePaddleCollision(m_paddle2, false);

        // Send the authoritative state to the client
        sendGameState();
    }
}

void GameWidget::handlePaddleCollision(QGraphicsEllipseItem* paddle, bool isPlayer1) {
    if (m_ellipseItem->collidesWithItem(paddle)) {
        m_lastPlayerToHitBall = isPlayer1 ? 1 : 2;
        if (m_isSawBladeBall) {
            if (isPlayer1) {m_player1Health--; normalizeBall();}
            else {m_player2Health--; normalizeBall();}
            updateHealthDisplay();

            if (m_player1Health <= 0) { endGame("PLAYER 2 WINS!"); return; }
            if (m_player2Health <= 0) { endGame("PLAYER 1 WINS!"); return; }
        }
        qreal speed = qSqrt(m_ellipseSpeedX * m_ellipseSpeedX + m_ellipseSpeedY * m_ellipseSpeedY) + BALL_SPEED_INCREMENT;
        qreal intersectY = (m_ellipseItem->pos().y() + 10) - (paddle->pos().y() + 50);
        qreal normY = intersectY / 50.0; qreal angle = normY * qDegreesToRadians(45.0);
        m_ellipseSpeedX = speed * qCos(angle) * (isPlayer1 ? 1 : -1); m_ellipseSpeedY = speed * qSin(angle);
    }
}

void GameWidget::createWall(qreal x, qreal y, qreal w, qreal h) {
    QGraphicsRectItem *wall = new QGraphicsRectItem(x, y, w, h);
    wall->setBrush(QColor("#CCCCCC"));
    wall->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    m_scene->addItem(wall);
}

void GameWidget::spawnItem(QGraphicsEllipseItem** item, const QBrush& brush) {
    if (!m_isHost) return;
    qDebug() << "GameWidget: Spawning Item.";

    if (*item) {
        m_scene->removeItem(*item);
        delete *item;
        *item = nullptr;
    }

    *item = new QGraphicsEllipseItem(0, 0, ITEM_DIAMETER, ITEM_DIAMETER);
    (*item)->setBrush(brush);

    (*item)->setPos(getRandomSpawnPoint());

    m_scene->addItem(*item);

    if (!m_itemRelocationTimer->isActive()) {
        m_itemRelocationTimer->start(10000);
    }
}

void GameWidget::relocateItems() {
    if (!m_isHost) return;
    qDebug() << "GameWidget: Relocating Items.";
    if (m_healthItem) {
        m_healthItem->setPos(getRandomSpawnPoint());
    }
    if (m_sawItem) {
        m_sawItem->setPos(getRandomSpawnPoint());
    }
}

QPointF GameWidget::getRandomSpawnPoint() const {
    qreal x = QRandomGenerator::global()->bounded(m_SpawnArea.width()) + m_SpawnArea.left();
    qreal y = QRandomGenerator::global()->bounded(m_SpawnArea.height()) + m_SpawnArea.top();
    return QPointF(x, y);
}

void GameWidget::normalizeBall() {
    qDebug() << "GameWidget: Normalizing ball.";
    m_isSawBladeBall = false;
    m_ellipseItem->setBrush(QColor("#2C3E50"));
}

void GameWidget::updateScoreDisplay() {
    m_player1ScoreText->setPlainText(QString("%1").arg(m_player1Score)); m_player2ScoreText->setPlainText(QString("%1").arg(m_player2Score));
    m_player1ScoreText->setPos(-400 - m_player1ScoreText->boundingRect().width()/2, -m_player1ScoreText->boundingRect().height()/2);
    m_player2ScoreText->setPos(400 - m_player2ScoreText->boundingRect().width()/2, -m_player2ScoreText->boundingRect().height()/2);
}

void GameWidget::updateHealthDisplay() {
    for (int i = 0; i < MAX_HEALTH; ++i) {
        m_player1HealthBars[i]->setVisible(i < m_player1Health);
        m_player2HealthBars[i]->setVisible(i < m_player2Health);
    }
}

void GameWidget::sendGameState() {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << QString("STATE");

    // 포맷: p1x p1y p2x p2y ballx bally ...
    out << m_paddle1->x() << m_paddle1->y()
        << m_paddle2->x() << m_paddle2->y()
        << m_ellipseItem->x() << m_ellipseItem->y()
        << m_player1Score << m_player2Score << m_player1Health << m_player2Health
        << m_isSawBladeBall
        << (m_healthItem != nullptr) << (m_healthItem ? m_healthItem->x() : 0.0) << (m_healthItem ? m_healthItem->y() : 0.0)
        << (m_sawItem != nullptr)    << (m_sawItem ? m_sawItem->x() : 0.0)    << (m_sawItem ? m_sawItem->y() : 0.0);

    emit sendDatagram(data);
    qDebug() << "GameWidget: Sent STATE.";
}
