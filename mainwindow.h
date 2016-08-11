#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <cmath>
#include "qthread.h"

#include "qcustomplot.h"
#include "quanser.h"
#include "signal.h"
//#include "control.h"

#include <iostream>
#include <QDebug>

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
    void configPanel();

    void configSignal();

    void closedLoop();

    void openLoop();

    void configGraphWrite();

    void configGraphRead();

    void connectServer();

    void data();

    int levelControl(int level);

    double voltageControl(double _volts);

    void stop();

    void sendData();

    void receiveData();



private:
    Ui::MainWindow *ui;
    Quanser *quanser;
    Signal *signal;

    const double MAX_VOLTAGE = 4;
    const double MIN_VOLTAGE = -4;
    const double GRAVITY = 9.806;
    const int MAX_LEVEL = 30;
    const int MIN_LEVEL = 0;

    const char* SERVER = "10.13.99.69";
    const int PORT = 20081;

    static constexpr int DEGRAU = 0;
    static constexpr int QUADRADA = 1;
    static constexpr int SENOIDAL = 2;
    static constexpr int DENTE_DE_SERRA = 3;
    static constexpr int ALEATORIO = 4;

    QTimer *timerEscrita;
    QTimer *timerLeitura;

    QThread *threadEscrita;
    QThread *threadLeitura;

    double time;
    double timeAux;
    int tipo_sinal = 0; /* 0 = Degrau | 1 = Quadrada | 2 = Senoidal | 3 = Dente de Serra | 4 = Aleatório */

    double amplitude;
    double tensao;
    double offSet;
    double periodo;
    double sinalEscrita;
    double sinalLeitura;

    int canalEscrita;
    int canalLeitura;

};

#endif // MAINWINDOW_H
