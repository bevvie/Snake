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

    connect(ui->widget, SIGNAL(IncScore(int)), this, SLOT(IncScore(int)));
    connect(ui->widget, SIGNAL(NewGame()), this, SLOT(ResetScore()));
    connect(this, SIGNAL(NewGamePressed()), ui->widget, SLOT(GameInit()));
    connect(ui->widget, SIGNAL(Pause()), this, SLOT(on_Pause_clicked()));
    connect(this, SIGNAL(ScoreDiv60()), ui->widget, SLOT(ShowBanana()));
    connect(this, SIGNAL(Saving(int, int, int, int)), ui->widget, SLOT(SaveGame(int, int, int, int)));
    connect(ui->widget, SIGNAL(StartTimer()), this, SLOT(StartTimer()));
    connect(ui->widget, SIGNAL(KillTimer()), this, SLOT(KillTimer()));
    connect(ui->widget, SIGNAL(SetLevel(int)), this, SLOT(SetLevel(int)));
    connect(ui->widget, SIGNAL(SetF1(int, int)), this, SLOT(Setf1(int, int)));

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

    if (ui->lcdNumber->intValue() >= f1)
    {
        level_counter++;
        int tmp = f1;
        f1 = f1 + f2;
        f2 = tmp;

        KillTimer();
        if (level_counter > 4)
            timer_interval -= 7;
        else
            timer_interval -= 3;
        StartTimer(timer_interval);
    }

    QString str;
    if (level_counter == 0)
        str = "Следующая цель: " + QString::number(f1);
    else
        str = "Уровень " + QString::number(level_counter) + " Пройден! Следующая цель: " + QString::number(f1);
    ui->level_label->setText(str);
}

void MainWindow::ResetScore()
{
    ui->lcdNumber->display(0);
    timer_interval = 140;
    level_counter = 0;
    f1 = 10; f2 = 10;
    QString str = " Следующая цель: " + QString::number(f1);
    ui->level_label->setText(str);
}

void MainWindow::on_StartPlay_clicked()
{
    ResetScore();
    emit NewGamePressed();
    if(!is_active)
    {
        StartTimer(timer_interval);
        is_active = 1;
    }

}


void MainWindow::on_Pause_clicked()
{
    KillTimer();
    int n = QMessageBox::warning(nullptr, "Snake", "Пауза",
                                "Продолжить", "Начать заново", "Сохранить и выйти", 0, 1);
    if (n == 0)
        StartTimer(timer_interval);
    else if (n == 1){
        emit NewGamePressed();
        timer_interval = 140;
        level_counter = 0;
        f1 = 10; f2 = 10;
        QString str = " Следующая цель: " + QString::number(f1);
        ui->level_label->setText(str);
        StartTimer(timer_interval);
    }
    else
    {
        emit Saving(ui->lcdNumber->intValue(), level_counter, f1, f2);
        this->close();
    }
}

void MainWindow::on_Saving_clicked()
{
    emit Saving(ui->lcdNumber->intValue(), level_counter, f1, f2);
    this->close();
}

void MainWindow::StartTimer(int interval)
{
    if(!is_active)
    {
        timerId = ui->widget->startTimer(interval);
        is_active = 1;
    }
}

void MainWindow::KillTimer()
{
    ui->widget->killTimer(timerId);
    is_active = 0;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    KillTimer();
    event->ignore();
    int n = QMessageBox::question(nullptr, "Snake", "Вы действительно хотите выйти?",
                                "Нет", "Да", QString(), 0, 1);
    if (n == 1)
        event->accept();
    if (timerId != -1)
        StartTimer(timer_interval);
}
