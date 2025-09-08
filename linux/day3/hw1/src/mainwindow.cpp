#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer=new QTimer;
    timer->setInterval(1500);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &MainWindow::update);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update(){timeout=1;}

void MainWindow::updateListWidget()
{
    ui->listWidget->clear();
    QString textToShow;
    for(const QString &s : data) {
        textToShow.append(s);
    }
    ui->listWidget->addItem(textToShow);

    if(shift==1){
        ui->pushButton_11->setStyleSheet("background-color: rgb(153, 193, 241);");
    }
    else if(shift==2){
        ui->pushButton_11->setStyleSheet("background-color: rgb(100, 100, 250);");
    }
    else{
        ui->pushButton_11->setStyleSheet("");
    }

    if(lg==0){
        if(shift==0){
            ui->pushButton->setText(".,?!");
            ui->pushButton_2->setText("abc");
            ui->pushButton_3->setText("def");
            ui->pushButton_5->setText("ghi");
            ui->pushButton_8->setText("jkl");
            ui->pushButton_6->setText("mno");
            ui->pushButton_9->setText("pqrs");
            ui->pushButton_12->setText("tuv");
            ui->pushButton_10->setText("wxyz");
            ui->pushButton_11->setText("↑");
            ui->pushButton_16->setText(",");
            ui->pushButton_15->setText(" ");
        }
        else {
            ui->pushButton->setText(".,?!");
            ui->pushButton_2->setText("ABC");
            ui->pushButton_3->setText("DEF");
            ui->pushButton_5->setText("GHI");
            ui->pushButton_8->setText("JKL");
            ui->pushButton_6->setText("MNO");
            ui->pushButton_9->setText("PQRS");
            ui->pushButton_12->setText("TUV");
            ui->pushButton_10->setText("WXYZ");
            ui->pushButton_11->setText("↑");
            ui->pushButton_16->setText(",");
            ui->pushButton_15->setText(" ");
        }
    }
    else if(lg==1){
        ui->pushButton->setText("ㅣ");
        ui->pushButton_2->setText("ㆍ");
        ui->pushButton_3->setText("ㅡ");
        ui->pushButton_5->setText("ㄱㅋ");
        ui->pushButton_8->setText("ㄴㄹ");
        ui->pushButton_6->setText("ㄷㅌ");
        ui->pushButton_9->setText("ㅂㅍ");
        ui->pushButton_12->setText("ㅅㅎ");
        ui->pushButton_10->setText("ㅈㅊ");
        ui->pushButton_11->setText(".,?!");
        ui->pushButton_16->setText("한자");
        ui->pushButton_15->setText("ㅇㅁ");
    }
    else if(lg == -1) {
        if (sim == 0) {
            ui->pushButton->setText("1");
            ui->pushButton_2->setText("2");
            ui->pushButton_3->setText("3");
            ui->pushButton_5->setText("4");
            ui->pushButton_8->setText("5");
            ui->pushButton_6->setText("6");
            ui->pushButton_9->setText("7");
            ui->pushButton_12->setText("8");
            ui->pushButton_10->setText("9");
            ui->pushButton_15->setText("0");
            ui->pushButton_11->setText(".,-/");
            ui->pushButton_16->setText("%");
        } else if (sim == 1) {
            ui->pushButton->setText("!");
            ui->pushButton_2->setText("?");
            ui->pushButton_3->setText(".");
            ui->pushButton_5->setText(",");
            ui->pushButton_8->setText("(");
            ui->pushButton_6->setText(")");
            ui->pushButton_9->setText("@");
            ui->pushButton_12->setText(":");
            ui->pushButton_10->setText(";");
            ui->pushButton_15->setText("1/3");
            ui->pushButton_11->setText(".,?!");
            ui->pushButton_16->setText("%");
        } else if (sim == 2) {
            ui->pushButton->setText("/");
            ui->pushButton_2->setText("-");
            ui->pushButton_3->setText("*");
            ui->pushButton_5->setText("_");
            ui->pushButton_8->setText("%");
            ui->pushButton_6->setText("~");
            ui->pushButton_9->setText("^");
            ui->pushButton_12->setText("#");
            ui->pushButton_10->setText("+");
            ui->pushButton_15->setText("2/3");
            ui->pushButton_11->setText(".,?!");
            ui->pushButton_16->setText("%");
        } else if (sim == 3) {
            ui->pushButton->setText("=");
            ui->pushButton_2->setText("\"");
            ui->pushButton_3->setText("'");
            ui->pushButton_5->setText("<");
            ui->pushButton_8->setText(">");
            ui->pushButton_6->setText("{");
            ui->pushButton_9->setText("}");
            ui->pushButton_12->setText("[");
            ui->pushButton_10->setText("]");
            ui->pushButton_15->setText("3/3");
            ui->pushButton_11->setText(".,?!");
            ui->pushButton_16->setText("%");
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(lg==0){
        if(data.empty()){ data.push_back("."); }
        else{
            if(timeout==0){
                if(data.back()=="."){data.back()=",";}
                else if(data.back()==","){data.back()="?";}
                else if(data.back()=="?"){data.back()="!";}
                else if(data.back()=="!"){data.back()=".";}
                else{data.push_back(".");}
            } else{data.push_back(".");}
        }
        if(shift==1){shift=0;}
        timeout=0; timer->start();
    }
    else if(lg==1){
        data.push_back("ㅣ");
        timeout=0; timer->start();
    }
    else if(lg == -1){
        if(sim == 0) { data.push_back("1"); }
        else if(sim == 1) { data.push_back("!"); }
        else if(sim == 2) { data.push_back("/"); }
        else if(sim == 3) { data.push_back("="); }
        timeout=1;
    }
    updateListWidget();
}


void MainWindow::on_pushButton_2_clicked()
{
    if(lg==0){
        if(data.empty()){ if(shift==0){data.push_back("a");} else{data.push_back("A");} }
        else{
            if(shift==0){
                if(timeout==0){
                    if(data.back()=="a"){data.back()="b";}
                    else if(data.back()=="b"){data.back()="c";}
                    else if(data.back()=="c"){data.back()="a";}
                    else{data.push_back("a");}
                } else{data.push_back("a");}
            } else{
                if(timeout==0){
                    if(data.back()=="A"){data.back()="B";}
                    else if(data.back()=="B"){data.back()="C";}
                    else if(data.back()=="C"){data.back()="A";}
                    else{data.push_back("A");}
                } else{data.push_back("A");}
            }
        }
        if(shift==1){shift=0;}
        timeout=0; timer->start();
    }
    else if(lg==1){
         if(timeout==0){
            if(data.back()=="ㆍ"){data.back()="ᆢ";}
            else if(data.back()=="ᆢ"){data.back()="ㆍ";}
            else{data.push_back("ㆍ");}
        } else{data.push_back("ㆍ");}
         timeout=0; timer->start();
    }
    else if(lg == -1){
        if(sim == 0) { data.push_back("2"); }
        else if(sim == 1) { data.push_back("?"); }
        else if(sim == 2) { data.push_back("-"); }
        else if(sim == 3) { data.push_back("\""); }
        timeout=1;
    }
    updateListWidget();
}


void MainWindow::on_pushButton_3_clicked()
{
    if(lg==0){
        if(data.empty()){ if(shift==0){data.push_back("d");} else{data.push_back("D");} }
        else{
            if(shift==0){
                if(timeout==0){
                    if(data.back()=="d"){data.back()="e";}
                    else if(data.back()=="e"){data.back()="f";}
                    else if(data.back()=="f"){data.back()="d";}
                    else{data.push_back("d");}
                } else{data.push_back("d");}
            } else{
                if(timeout==0){
                    if(data.back()=="D"){data.back()="E";}
                    else if(data.back()=="E"){data.back()="F";}
                    else if(data.back()=="F"){data.back()="D";}
                    else{data.push_back("D");}
                } else{data.push_back("D");}
            }
        }
        if(shift==1){shift=0;}
        timeout=0; timer->start();
    }
    else if(lg==1){
        data.push_back("ㅡ");
        timeout=0; timer->start();
    }
    else if(lg == -1){
        if(sim == 0) { data.push_back("3"); }
        else if(sim == 1) { data.push_back("."); }
        else if(sim == 2) { data.push_back("*"); }
        else if(sim == 3) { data.push_back("'"); }
        timeout=1;
    }
    updateListWidget();
}


void MainWindow::on_pushButton_4_clicked()
{
    if (!data.empty()) {
        data.pop_back();
    }
    updateListWidget();
    timeout=1;
}


void MainWindow::on_pushButton_5_clicked()
{
    if(lg==0){
        if(data.empty()){ if(shift==0){data.push_back("g");} else{data.push_back("G");} }
        else{
            if(shift==0){
                if(timeout==0){
                    if(data.back()=="g"){data.back()="h";}
                    else if(data.back()=="h"){data.back()="i";}
                    else if(data.back()=="i"){data.back()="g";}
                    else{data.push_back("g");}
                } else{data.push_back("g");}
            } else{
                if(timeout==0){
                    if(data.back()=="G"){data.back()="H";}
                    else if(data.back()=="H"){data.back()="I";}
                    else if(data.back()=="I"){data.back()="G";}
                    else{data.push_back("G");}
                } else{data.push_back("G");}
            }
        }
        if(shift==1){shift=0;}
        timeout=0; timer->start();
    }
    else if(lg==1){
        if(data.empty()){ data.push_back("ㄱ"); }
        else{
            if(timeout==0){
                if(data.back()=="ㄱ"){data.back()="ㅋ";}
                else if(data.back()=="ㅋ"){data.back()="ㄲ";}
                else if(data.back()=="ㄲ"){data.back()="ㄱ";}
                else{data.push_back("ㄱ");}
            } else{data.push_back("ㄱ");}
        }
        timeout=0; timer->start();
    }
    else if(lg == -1){
        if(sim == 0) { data.push_back("4"); }
        else if(sim == 1) { data.push_back(","); }
        else if(sim == 2) { data.push_back("_"); }
        else if(sim == 3) { data.push_back("<"); }
        timeout=1;
    }
    updateListWidget();
}


void MainWindow::on_pushButton_8_clicked()
{
    if(lg==0){
        if(data.empty()){ if(shift==0){data.push_back("j");} else{data.push_back("J");} }
        else{
            if(shift==0){
                if(timeout==0){
                    if(data.back()=="j"){data.back()="k";}
                    else if(data.back()=="k"){data.back()="l";}
                    else if(data.back()=="l"){data.back()="j";}
                    else{data.push_back("j");}
                } else{data.push_back("j");}
            } else{
                if(timeout==0){
                    if(data.back()=="J"){data.back()="K";}
                    else if(data.back()=="K"){data.back()="L";}
                    else if(data.back()=="L"){data.back()="J";}
                    else{data.push_back("J");}
                } else{data.push_back("J");}
            }
        }
        if(shift==1){shift=0;}
        timeout=0; timer->start();
    }
    else if(lg==1){
        if(data.empty()){ data.push_back("ㄴ"); }
        else{
            if(timeout==0){
                if(data.back()=="ㄴ"){data.back()="ㄹ";}
                else if(data.back()=="ㄹ"){data.back()="ㄴ";}
                else{data.push_back("ㄴ");}
            } else{data.push_back("ㄴ");}
        }
        timeout=0; timer->start();
    }
    else if(lg == -1){
        if(sim == 0) { data.push_back("5"); }
        else if(sim == 1) { data.push_back("("); }
        else if(sim == 2) { data.push_back("%"); }
        else if(sim == 3) { data.push_back(">"); }
        timeout=1;
    }
    updateListWidget();
}


void MainWindow::on_pushButton_6_clicked()
{
    if(lg==0){
        if(data.empty()){ if(shift==0){data.push_back("m");} else{data.push_back("M");} }
        else{
            if(shift==0){
                if(timeout==0){
                    if(data.back()=="m"){data.back()="n";}
                    else if(data.back()=="n"){data.back()="o";}
                    else if(data.back()=="o"){data.back()="m";}
                    else{data.push_back("m");}
                } else{data.push_back("m");}
            } else{
                if(timeout==0){
                    if(data.back()=="M"){data.back()="N";}
                    else if(data.back()=="N"){data.back()="O";}
                    else if(data.back()=="O"){data.back()="M";}
                    else{data.push_back("M");}
                } else{data.push_back("M");}
            }
        }
        if(shift==1){shift=0;}
        timeout=0; timer->start();
    }
    else if(lg==1){
        if(data.empty()){ data.push_back("ㄷ"); }
        else{
            if(timeout==0){
                if(data.back()=="ㄷ"){data.back()="ㅌ";}
                else if(data.back()=="ㅌ"){data.back()="ㄸ";}
                else if(data.back()=="ㄸ"){data.back()="ㄷ";}
                else{data.push_back("ㄷ");}
            } else{data.push_back("ㄷ");}
        }
        timeout=0; timer->start();
    }
    else if(lg == -1){
        if(sim == 0) { data.push_back("6"); }
        else if(sim == 1) { data.push_back(")"); }
        else if(sim == 2) { data.push_back("~"); }
        else if(sim == 3) { data.push_back("{"); }
        timeout=1;
    }
    updateListWidget();
}


void MainWindow::on_pushButton_7_clicked()
{
    QString textToSave;
    for(const QString &s : data) {
        textToSave.append(s);
    }

    QFile file("/home/david/intern_ws/qt/day3/hw1/hw1.txt");
    if(!file.open(QFile::Append | QFile::Text)){
        QMessageBox::warning(this,"title","file is not open");
        return;
    }
    QTextStream out(&file);
    out << textToSave << "\n";
    file.close();

    data.clear();
    updateListWidget();
    timeout=1;
}


void MainWindow::on_pushButton_9_clicked()
{
    if(lg==0){
        if(data.empty()){ if(shift==0){data.push_back("p");} else{data.push_back("P");} }
        else{
            if(shift==0){
                if(timeout==0){
                    if(data.back()=="p"){data.back()="q";}
                    else if(data.back()=="q"){data.back()="r";}
                    else if(data.back()=="r"){data.back()="s";}
                    else if(data.back()=="s"){data.back()="p";}
                    else{data.push_back("p");}
                } else{data.push_back("p");}
            } else{
                if(timeout==0){
                    if(data.back()=="P"){data.back()="Q";}
                    else if(data.back()=="Q"){data.back()="R";}
                    else if(data.back()=="R"){data.back()="S";}
                    else if(data.back()=="S"){data.back()="P";}
                    else{data.push_back("P");}
                } else{data.push_back("P");}
            }
        }
        if(shift==1){shift=0;}
        timeout=0; timer->start();
    }
    else if(lg==1){
        if(data.empty()){ data.push_back("ㅂ"); }
        else{
            if(timeout==0){
                if(data.back()=="ㅂ"){data.back()="ㅍ";}
                else if(data.back()=="ㅍ"){data.back()="ㅃ";}
                else if(data.back()=="ㅃ"){data.back()="ㅂ";}
                else{data.push_back("ㅂ");}
            } else{data.push_back("ㅂ");}
        }
        timeout=0; timer->start();
    }
    else if(lg == -1){
        if(sim == 0) { data.push_back("7"); }
        else if(sim == 1) { data.push_back("@"); }
        else if(sim == 2) { data.push_back("^"); }
        else if(sim == 3) { data.push_back("}"); }
        timeout=1;
    }
    updateListWidget();
}


void MainWindow::on_pushButton_12_clicked()
{
    if(lg==0){
        if(data.empty()){ if(shift==0){data.push_back("t");} else{data.push_back("T");} }
        else{
            if(shift==0){
                if(timeout==0){
                    if(data.back()=="t"){data.back()="u";}
                    else if(data.back()=="u"){data.back()="v";}
                    else if(data.back()=="v"){data.back()="t";}
                    else{data.push_back("t");}
                } else{data.push_back("t");}
            } else{
                if(timeout==0){
                    if(data.back()=="T"){data.back()="U";}
                    else if(data.back()=="U"){data.back()="V";}
                    else if(data.back()=="V"){data.back()="T";}
                    else{data.push_back("T");}
                } else{data.push_back("T");}
            }
        }
        if(shift==1){shift=0;}
        timeout=0; timer->start();
    }
    else if(lg==1){
        if(data.empty()){ data.push_back("ㅅ"); }
        else{
            if(timeout==0){
                if(data.back()=="ㅅ"){data.back()="ㅎ";}
                else if(data.back()=="ㅎ"){data.back()="ㅆ";}
                else if(data.back()=="ㅆ"){data.back()="ㅅ";}
                else{data.push_back("ㅅ");}
            } else{data.push_back("ㅅ");}
        }
        timeout=0; timer->start();
    }
    else if(lg == -1){
        if(sim == 0) { data.push_back("8"); }
        else if(sim == 1) { data.push_back(":"); }
        else if(sim == 2) { data.push_back("#"); }
        else if(sim == 3) { data.push_back("["); }
        timeout=1;
    }
    updateListWidget();
}


void MainWindow::on_pushButton_10_clicked()
{
    if(lg==0){
        if(data.empty()){ if(shift==0){data.push_back("w");} else{data.push_back("W");} }
        else{
            if(shift==0){
                if(timeout==0){
                    if(data.back()=="w"){data.back()="x";}
                    else if(data.back()=="x"){data.back()="y";}
                    else if(data.back()=="y"){data.back()="z";}
                    else if(data.back()=="z"){data.back()="w";}
                    else{data.push_back("w");}
                } else{data.push_back("w");}
            } else{
                if(timeout==0){
                    if(data.back()=="W"){data.back()="X";}
                    else if(data.back()=="X"){data.back()="Y";}
                    else if(data.back()=="Y"){data.back()="Z";}
                    else if(data.back()=="Z"){data.back()="W";}
                    else{data.push_back("W");}
                } else{data.push_back("W");}
            }
        }
        if(shift==1){shift=0;}
        timeout=0; timer->start();
    }
    else if(lg==1){
        if(data.empty()){ data.push_back("ㅈ"); }
        else{
            if(timeout==0){
                if(data.back()=="ㅈ"){data.back()="ㅊ";}
                else if(data.back()=="ㅊ"){data.back()="ㅉ";}
                else if(data.back()=="ㅉ"){data.back()="ㅈ";}
                else{data.push_back("ㅈ");}
            } else{data.push_back("ㅈ");}
        }
        timeout=0; timer->start();
    }
    else if(lg == -1){
        if(sim == 0) { data.push_back("9"); }
        else if(sim == 1) { data.push_back(";"); }
        else if(sim == 2) { data.push_back("+"); }
        else if(sim == 3) { data.push_back("]"); }
        timeout=1;
    }
    updateListWidget();
}

void MainWindow::on_pushButton_11_clicked(){
    if(lg==0){
        shift++;
        if(shift>2){shift=0;}
    }
    else if(lg==1 || (lg == -1 && sim > 0)){
        if(data.empty()){data.push_back(".");}
        else{
            if(timeout==0){
                if(data.back()=="."){data.back()=",";}
                else if(data.back()==","){data.back()="?";}
                else if(data.back()=="?"){data.back()="!";}
                else if(data.back()=="!"){data.back()=".";}
                else{data.push_back(".");}
            } else{data.push_back(".");}
        }
        timeout=0;
        timer->start();
    }
    else if(lg == -1 && sim == 0){
        if(data.empty()){ data.push_back("."); }
        else{
            if(timeout==0){
                if(data.back()=="."){data.back()=",";}
                else if(data.back()==","){data.back()="-";}
                else if(data.back()=="-"){data.back()="/";}
                else if(data.back()=="/"){data.back()=".";}
                else{data.push_back(".");}
            } else{data.push_back(".");}
        }
        timeout=0;
        timer->start();
    }
    updateListWidget();
}

void MainWindow::on_pushButton_14_clicked()
{
    if (lg != -1) {lg = -1;sim = 0;}
    else {
        if (sim == 0) {sim = 1;}
        else {sim = 0;}
    }
    timeout = 1;
    shift=0;
    updateListWidget();
}


void MainWindow::on_pushButton_17_clicked()
{
    lg++;
    if(lg>1){lg=0;}
    sim=-1;
    timeout = 1;
    shift=0;
    updateListWidget();
}


void MainWindow::on_pushButton_15_clicked()
{
    if(lg==1){
        if(data.empty()){ data.push_back("ㅇ"); }
        else{
            if(timeout==0){
                if(data.back()=="ㅇ"){data.back()="ㅁ";}
                else if(data.back()=="ㅁ"){data.back()="ㅇ";}
                else{data.push_back("ㅇ");}
            } else{data.push_back("ㅇ");}
        }
        timeout=0;
        timer->start();
    }
    else if(lg == -1){
        if (sim == 0) {data.push_back("0");}
        else if (sim == 1) {sim = 2;}
        else if (sim == 2) {sim = 3;}
        else if (sim == 3) {sim = 1;}
        timeout=1;
    }
    updateListWidget();
}


void MainWindow::on_pushButton_13_clicked()
{
    data.push_back(" ");
    updateListWidget();
    timeout=1;
}


void MainWindow::on_pushButton_16_clicked()
{
    if(lg==0){data.push_back(",");timeout=1;}
    else if(lg==1){timeout=1;}
    else if(lg == -1){data.push_back("%");timeout=1;}
    updateListWidget();
}
