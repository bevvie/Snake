#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

signals:
    void NewGamePressed();
    void ScoreDiv60();
    void Saving(int score, int level, int f1, int f2);

public slots:
    void IncScore(int value);
    void ResetScore();
    void StartTimer(int interval);
    void KillTimer();
    void SetLevel(int level) { level_counter = level; }
    void SetF1(int a1, int a2) { f1 = a1; f2 = a2; }

private slots:
    void on_StartPlay_clicked();
    void on_Pause_clicked();
    void on_Saving_clicked();

private:
    Ui::MainWindow *ui;

    int timerId = -1;
    bool is_active = 0;
    int f1 = 10, f2 = 10;
    int level_counter = 0;
    int timer_interval = 140;
};

#endif // MAINWINDOW_H
