#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "qthread.h"
#include "qcustomplot.h"
#include "control.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void UI_configGraphWrite();

    void UI_configGraphRead();

    void UI_canalReadSelect();

    void UI_configPanel();

    void UI_configSignal();

    void UI_closedLoop();

    void UI_openLoop();

    void UI_limitRandInput();

    void connectServer();

    void data();

    void sendData();

    void receiveData();

private:
    Ui::MainWindow *ui;

    Control *control;

    QTimer *timerEscrita;
    QTimer *timerLeitura;

    QThread *threadEscrita;
    QThread *threadLeitura;

    bool canalLeituraVec[NUMB_CAN_READ];

};

#endif // MAINWINDOW_H
