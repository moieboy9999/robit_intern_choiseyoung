#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <vector>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_17_clicked();
    void on_pushButton_15_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_16_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int timeout=1;
    int lg=0;//0=영어 1= 한국어 -1= 다른 입력상태
    int sim=-1;//0=숫자 1..=문자 -1= 다른 입력상태
    int shift=0;//0=소문자 1=대문자 1번 2=대문자 연속

    std::vector<QString> data;


    void updateListWidget();
    void update();


};


#endif // MAINWINDOW_H
