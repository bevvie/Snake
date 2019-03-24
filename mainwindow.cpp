#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap main_bkgnd(":/images/main_bg.jpg");
    main_bkgnd = main_bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, main_bkgnd);
    setPalette(palette);

    QPixmap widget_bkgnd(":/images/bg.jpg");
    //bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    palette.setBrush(QPalette::Background, widget_bkgnd);
    ui->widget->setPalette(palette);

    connect(ui->widget, SIGNAL(FoodEated(int)), this, SLOT(IncScore(int)));
    connect(ui->widget, SIGNAL(NewGame()), this, SLOT(ResetScore()));
    connect(this, SIGNAL(NewGamePressed()), ui->widget, SLOT(GameInit()));
    connect(ui->widget, SIGNAL(Pause()), this, SLOT(on_Pause_clicked()));
    connect(this, SIGNAL(ScoreDiv60()), ui->widget, SLOT(ShowBanana()));

    ui->lcdNumber->display(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::IncScore(int value)
{
    ui->lcdNumber->display(ui->lcdNumber->intValue() + value);

    if (ui->lcdNumber->intValue() % 20 == 0)
        emit ScoreDiv60();
}

void MainWindow::ResetScore()
{
    ui->lcdNumber->display(0);
}

void MainWindow::on_StartPlay_clicked()
{
    if(!is_active)
    {
        timerId = ui->widget->startTimer(140);
        is_active = 1;
    }
    emit NewGamePressed();

}


void MainWindow::on_Pause_clicked()
{
    ui->widget->killTimer(timerId);
    int n = QMessageBox::warning(0, "Snake", "Пауза. Продолжить?",
                                "Продолжить", "Начать заново", QString(), 0, 1);
    if (n==0)
        timerId = ui->widget->startTimer(140);

}
