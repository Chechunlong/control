#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
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


        int    malha = 0;      /* 0 = Malha Fechada | 1 = Malha Aberta */
        int    tipo_sinal = 0; /* 0 = Degrau | 1 = Quadrada | 2 = Senoidal | 3 = Dente de Serra | 4 = Aleatório */
        double amplitude = 0.0;
        double offSet = 0.0;
        double periodo = 0.0;
        int    canalEscrita = 0;

        QThread *threadEscrita;
        QThread *threadLeitura;


private slots:
        void on_radioAberta_clicked();

        void on_radioFechada_clicked();

        void connectServer();

private:
        Ui::MainWindow *ui;
        Quanser *quanser;
};

#endif // MAINWINDOW_H
