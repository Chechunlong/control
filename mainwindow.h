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

    void configGraphWrite();

    void configGraphRead();

    void on_radioAberta_clicked();

    void on_radioFechada_clicked();

    void connectServer();

    void data();

    int levelControl(int level);

    double voltageControl(double _volts);

    void sendData();

    void receiveData();

private:
    Ui::MainWindow *ui;
    Quanser *quanser;
    Signal *signal;

    const float MAX_VOLTAGE = 4;
    const float MIN_VOLTAGE = -4;
    const float GRAVITY = 9.806;
    const int MAX_LEVEL = 30;
    const int MIN_LEVEL = 0;

    const char* SERVER = "10.13.99.6";
    const int PORT = 20081;

    QTimer *timerEscrita;
    QThread *threadEscrita;

    QTimer *timerLeitura;
    QThread *threadLeitura;

    double time;
    double timeAux;
    int malha = 0;      /* 0 = Malha Fechada | 1 = Malha Aberta */
    int tipo_sinal = 0; /* 0 = Degrau | 1 = Quadrada | 2 = Senoidal | 3 = Dente de Serra | 4 = Aleatório */
    double amplitude = 0.0;
    double offSet = 0.0;
    double periodo = 0;
    double tensao;
    int canalEscrita = 0;
    double sinalSaida;

    static constexpr int DEGRAU = 0;
    static constexpr int QUADRADA = 1;
    static constexpr int SENOIDAL = 2;
    static constexpr int DENTE_DE_SERRA = 3;
    static constexpr int ALEATORIO = 4;


protected:

};

#endif // MAINWINDOW_H
