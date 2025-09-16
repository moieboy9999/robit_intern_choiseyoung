#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QDataStream>
#include <QFont>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QRectF>
#include <QTimer>
#include <QVector>
#include <QWidget>
#include <QtMath>

QT_BEGIN_NAMESPACE
namespace Ui { class GameWidget; }
QT_END_NAMESPACE

class QGraphicsEllipseItem;
class QGraphicsRectItem;
class QGraphicsScene;
class QGraphicsTextItem;
class QKeyEvent;
class QTimer;

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    void startGame(bool isHost);

signals:
    void gameOver(const QString& result);
    void sendDatagram(const QByteArray& data);

public slots:
    void onDatagramReceived(const QByteArray& data);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updateAnimation();
    void normalizeBall();
    void relocateItems();

private:
    // UI and Graphics Items
    Ui::GameWidget *ui;
    QGraphicsScene *m_scene;
    QGraphicsEllipseItem *m_paddle1;
    QGraphicsEllipseItem *m_paddle2;
    QGraphicsEllipseItem *m_ellipseItem;
    QGraphicsRectItem *m_goal1;
    QGraphicsRectItem *m_goal2;
    QGraphicsTextItem *m_player1ScoreText;
    QGraphicsTextItem *m_player2ScoreText;
    QGraphicsTextItem *m_gameOverText;
    QGraphicsEllipseItem* m_healthItem = nullptr;
    QGraphicsEllipseItem* m_sawItem = nullptr;

    // Timers
    QTimer *m_animationTimer;
    QTimer *m_itemRelocationTimer;

    // Game State Variables
    qreal m_ellipseSpeedX;
    qreal m_ellipseSpeedY;
    bool m_isMovingUp1, m_isMovingDown1, m_isMovingLeft1 ,m_isMovingRight1 ;
    int m_player1Score;
    int m_player2Score;
    int m_player1Health;
    int m_player2Health;
    int m_lastPlayerToHitBall = 0;
    bool m_isSawBladeBall = false;
    QRectF m_SpawnArea;
    bool m_isHost = false;

    // Health Bars
    QVector<QGraphicsRectItem*> m_player1HealthBars;
    QVector<QGraphicsRectItem*> m_player2HealthBars;

    // Helper methods
    void resetGame();
    void endGame(const QString& winner);
    void sendPaddlePosition();
    void sendGameState();
    void updateScoreDisplay();
    void updateHealthDisplay();
    void spawnItem(QGraphicsEllipseItem** item, const QBrush& brush);
    void createWall(qreal x, qreal y, qreal w, qreal h);
    QPointF getRandomSpawnPoint() const;
    void handlePaddleCollision(QGraphicsEllipseItem* paddle, bool isPlayer1);
};

#endif // GAMEWIDGET_H
