#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <cmath>

#include "qcustomplot.h"
#include "quanser.h"
#include "qthread.h"

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
    void on_radioAberta_clicked();

    void on_radioFechada_clicked();

    void connectServer();

    void data();

    float voltageControl(float voltage);

    int levelControl(int level);

    void sendData();

    void receiveData();

private:
    Ui::MainWindow *ui;
    Quanser *quanser;

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


    int malha = 0;      /* 0 = Malha Fechada | 1 = Malha Aberta */
    int tipo_sinal = 0; /* 0 = Degrau | 1 = Quadrada | 2 = Senoidal | 3 = Dente de Serra | 4 = Aleatório */
    float amplitude = 0.0;
    float offSet = 0.0;
    float periodo = 0.0;
    int canalEscrita = 0;


protected:

};

#endif // MAINWINDOW_H
