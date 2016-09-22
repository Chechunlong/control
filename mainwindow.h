#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "qthread.h"
#include "qcustomplot.h"
#include "control.h"

#include "conectar.h"

#define PLOT_ESCRITA 2

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

    /*
        Configura o grafico de escrita
    */
    void UI_configGraphWrite();

    /*
        Configura o grafico de leitura
    */
    void UI_configGraphRead();

    /*
        Configura o modo de exibição dos graficos
    */
    void UI_configPlotGraficosE();


    void UI_configPlotGraficosL();

    /*
        Define as configurações iniciais da interface
    */
    void UI_configPanel();


    /*
        Retorna o tipo de malha selecionada na interface

        M_FECHADA ou M_ABERTA
    */
    int UI_getTipoMalha();


    /*
        Permite que o usuario escolha que o grafico
        do canal que ele escolheu para ler seja mostrado
        ou não
    */
   // void UI_canaisLeituraPlot();


    /*
        Configura a exibiçao da
    */
    //void UI_configCanais();

    void UI_configSignal();
    void UI_malhaFechada();
    void UI_malhaAberta();
    void UI_configMalha();
    void UI_updateConsControle(double kd_td, double ki_ti);
    void UI_configConsControle();
    void UI_configControlador();
    void UI_limitRandInput();


    void connectServer();
    void zerarSinal();
    void controladorPID();
    void data();
    void sendData();
    void receiveData();


private:
    Ui::MainWindow *ui;

    Control *control;

    Conectar *conectar;

    QTimer *timerEscrita;
    QTimer *timerLeitura;

    QThread *threadEscrita;
    QThread *threadLeitura;

    bool canalLeituraVec[NUMB_CAN_READ];
    bool canalLeituraPlotVec[NUMB_CAN_READ];
    bool vectorGrafLeitura[5];
    bool sinalPlotEscrita[PLOT_ESCRITA];
    int canalEscrita = 0;

    double tanque1 = 0;
    double tanque2 = 0;

    int canalLeitura = 0;
    int tipoMalha = 0;

    double sinalLeitura = 0;
    int ordemSistema = 0;

    bool start = false;

};

#endif // MAINWINDOW_H
