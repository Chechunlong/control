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

    int levelControl(int level);

    double voltageControl(double volts);

    void stop();

    void travel();

    void sendData();

    void receiveData();



private:
    Ui::MainWindow *ui;
    Quanser *quanser;
    Signal *signal;
    QListWidgetItem *item;


    const char* SERVER = "10.13.99.69";
    const int PORT = 20081;

    const double MAX_VOLTAGE = 4;
    const double MIN_VOLTAGE = -4;
    const double GRAVITY = 9.806;
    const int MAX_LEVEL = 30;
    const int MIN_LEVEL = 0;

    const double FATOR_CONVERSAO = 6.25;

    static const int NUMB_CAN_READ = 8;

    static constexpr int DEGRAU = 0;
    static constexpr int QUADRADA = 1;
    static constexpr int SENOIDAL = 2;
    static constexpr int DENTE_DE_SERRA = 3;
    static constexpr int ALEATORIO = 4;

    QTimer *timerEscrita;
    QTimer *timerLeitura;

    QThread *threadEscrita;
    QThread *threadLeitura;

    int tipoMalha = 1; /* 0 -> fechada, 1 -> aberto*/

    double time;
    double timeAux;
    int tipo_sinal = 0; /* 0 = Degrau | 1 = Quadrada | 2 = Senoidal | 3 = Dente de Serra | 4 = Aleatório */

    double sinalEscrita;
    double sinalLeitura;
    double sinalCalculado;

    // Update data function: _data()
    double tensao;
    double offSet;
    double periodo;
    double amplitude;

    // Malha fechada function: _receiveData()
    double tensaoErro;
    double erro;

    int canalEscrita;
    int canalLeitura;
    bool canalLeituraVec[NUMB_CAN_READ];

};

#endif // MAINWINDOW_H
