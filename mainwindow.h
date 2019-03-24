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

signals:
    void NewGamePressed();
    void ScoreDiv60();

public slots:
    void IncScore(int value);
    void ResetScore();

private slots:
    void on_StartPlay_clicked();

    void on_Pause_clicked();

private:
    Ui::MainWindow *ui;

    int timerId;
    bool is_active = 0;
};

#endif // MAINWINDOW_H
