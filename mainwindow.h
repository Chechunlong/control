#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void configPanel();

    void configSignal();

    void canalReadSelect();

    void closedLoop();

    void openLoop();

    void limitRandInput();

    void travel();

    void configGraphWrite();

    void configGraphRead();

    void connectServer();

    void data();

<<<<<<< HEAD
    int levelControl(int level);

    double voltageControl(double _volts);

    void stop();

=======
>>>>>>> control
    void sendData();

    void receiveData();

private:
    Ui::MainWindow *ui;
<<<<<<< HEAD
    Quanser *quanser;
    Signal *signal;
    QListWidgetItem *item;

    const double MAX_VOLTAGE = 4;
    const double MIN_VOLTAGE = -4;
    const double GRAVITY = 9.806;
    const int MAX_LEVEL = 30;
    const int MIN_LEVEL = 0;

    const double FATOR_CONVERSAO = 6.25;

    const char* SERVER = "10.13.99.69";
    const int PORT = 20081;

    static constexpr int DEGRAU = 0;
    static constexpr int QUADRADA = 1;
    static constexpr int SENOIDAL = 2;
    static constexpr int DENTE_DE_SERRA = 3;
    static constexpr int ALEATORIO = 4;
=======

    Control *control;
>>>>>>> control


    QTimer *timerEscrita;
    QTimer *timerLeitura;

    QThread *threadEscrita;
    QThread *threadLeitura;

<<<<<<< HEAD
    int tipoMalha = 1; /* 0 -> fechada, 1 -> aberto*/

    double time;
    double timeAux;
    int tipo_sinal = 0; /* 0 = Degrau | 1 = Quadrada | 2 = Senoidal | 3 = Dente de Serra | 4 = Aleatório */

    double amplitude;

    double offSet;
    double periodo;
    double sinalEscrita;
    double sinalLeitura;

    double sinalCalculado;

    double tensao;
    double tensaoErro;
    double erro;

    int canalEscrita;
    int canalLeitura;
    bool canalLeituraVec[7];
=======
    bool canalLeituraVec[NUMB_CAN_READ];
>>>>>>> control

};

#endif // MAINWINDOW_H
