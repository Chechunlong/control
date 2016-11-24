#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPrinter>

#define TAB_SINAL 0
#define TAB_CONTROLE 1

#define TAB_MESTRE 0
#define TAB_ESCRAVO 1

#define ABS 0
#define PICOm 1

#define Tr100 0
#define Tr95 1
#define Tr90 2

#define GRAPH_SINAL_ENVIADO 0
#define GRAPH_SINAL_CALCULADO 1

#define GRAPH_ERRO 0
#define GRAPH_SET_POINT 1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    conectar = new Conectar();
    conectar->show();

    connect(conectar, SIGNAL(accepted()), this, SLOT(connectServer()));

    ui->setupUi(this);

    timerLeitura = new QTimer(0);
    timerEscrita = new QTimer(0);

    threadLeitura = new QThread(this);
    threadEscrita = new QThread(this);

    timerLeitura->start(100);
    timerEscrita->start(100);

    timerLeitura->moveToThread(threadEscrita);
    connect(timerLeitura, SIGNAL(timeout()), this, SLOT(receiveData()));

    timerEscrita->moveToThread(threadEscrita);
    connect(timerEscrita, SIGNAL(timeout()), this, SLOT(sendData()));

    // Controlador P-I-D
    connect(ui->rb_constGanho,   SIGNAL(toggled(bool)),this,SLOT(UI_configControlador()));
    connect(ui->rb_constTempo,   SIGNAL(toggled(bool)),this,SLOT(UI_configControlador()));
    connect(ui->tipoControlador, SIGNAL(currentIndexChanged(int)),this,SLOT(UI_configControlador()));
    connect(ui->radioFechada,    SIGNAL(toggled(bool)), ui->tab_sinal_controle->widget(TAB_CONTROLE), SLOT(setEnabled(bool)));
    connect(ui->radioAberta,     SIGNAL(toggled(bool)), ui->tab_sinal_controle->widget(TAB_CONTROLE), SLOT(setDisabled(bool)));
    connect(ui->rb_constTempo,   SIGNAL(toggled(bool)),this,SLOT(UI_configConsControle()));
    connect(ui->rb_constGanho,   SIGNAL(toggled(bool)),this,SLOT(UI_configConsControle()));

    /// Habilitar Canais e Mostrar Canais no Gráfico
    connect(ui->canal_l0, SIGNAL(toggled(bool)), ui->cb_plot_canal0, SLOT(setEnabled(bool)));
    connect(ui->canal_l1, SIGNAL(toggled(bool)), ui->cb_plot_canal1, SLOT(setEnabled(bool)));
    connect(ui->canal_l2, SIGNAL(toggled(bool)), ui->cb_plot_canal2, SLOT(setEnabled(bool)));
    connect(ui->canal_l3, SIGNAL(toggled(bool)), ui->cb_plot_canal3, SLOT(setEnabled(bool)));
    connect(ui->canal_l4, SIGNAL(toggled(bool)), ui->cb_plot_canal4, SLOT(setEnabled(bool)));
    connect(ui->canal_l5, SIGNAL(toggled(bool)), ui->cb_plot_canal5, SLOT(setEnabled(bool)));
    connect(ui->canal_l6, SIGNAL(toggled(bool)), ui->cb_plot_canal6, SLOT(setEnabled(bool)));
    connect(ui->canal_l7, SIGNAL(toggled(bool)), ui->cb_plot_canal7, SLOT(setEnabled(bool)));
    connect(ui->canal_l0, SIGNAL(toggled(bool)), ui->cb_plot_canal0, SLOT(setChecked(bool)));
    connect(ui->canal_l1, SIGNAL(toggled(bool)), ui->cb_plot_canal1, SLOT(setChecked(bool)));
    connect(ui->canal_l2, SIGNAL(toggled(bool)), ui->cb_plot_canal2, SLOT(setChecked(bool)));
    connect(ui->canal_l3, SIGNAL(toggled(bool)), ui->cb_plot_canal3, SLOT(setChecked(bool)));
    connect(ui->canal_l4, SIGNAL(toggled(bool)), ui->cb_plot_canal4, SLOT(setChecked(bool)));
    connect(ui->canal_l5, SIGNAL(toggled(bool)), ui->cb_plot_canal5, SLOT(setChecked(bool)));
    connect(ui->canal_l6, SIGNAL(toggled(bool)), ui->cb_plot_canal6, SLOT(setChecked(bool)));
    connect(ui->canal_l7, SIGNAL(toggled(bool)), ui->cb_plot_canal7, SLOT(setChecked(bool)));

    //Grafico e Canais
    connect(ui->cb_plot_canal0, SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal1, SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal2, SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal3, SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal4, SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal5, SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal6, SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal7, SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_canalEscrita, SIGNAL(activated(int)), this, SLOT(UI_configPlotGraficosL()));

    // Grafico e Variavéis
    connect(ui->cb_plot_erro,           SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_setPoint,       SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_p,              SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_i,              SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_d,              SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_graf_sinalCalculado, SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosE()));
    connect(ui->cb_graf_sinalEnviado,   SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosE()));
    connect(ui->cb_SinalParcial,   SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosE()));
    //connect(ui->cb_plot_erro_2,           SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    //connect(ui->cb_plot_setPoint_2,       SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_p_2,        SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_i_2,        SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_d_2,        SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->checkErroObsTq01,   SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->checkErroObsTq02,   SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->checkObsTq01,       SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->checkObsTq02,       SIGNAL(toggled(bool)), this, SLOT(UI_configPlotGraficosL()));


    // Botões
    connect(ui->buttonAtualizar,SIGNAL(clicked(bool)),this,SLOT(data()));
    connect(ui->buttonStop,     SIGNAL(clicked(bool)),this,SLOT(zerarSinal()));

    // Tipo de Malha
    connect(ui->radioAberta,    SIGNAL(clicked(bool)),this,SLOT(UI_configMalha()));
    connect(ui->radioFechada,   SIGNAL(clicked(bool)),this,SLOT(UI_configMalha()));

    connect(ui->comboTipoSinal, SIGNAL(currentIndexChanged(int)),this,SLOT(UI_configSignal()));

    // Controlar o limite das entradas
    connect(ui->dSpinAmp,       SIGNAL(valueChanged(double)), this, SLOT(UI_limitRandInput()));
    connect(ui->dSpinPeriodo,   SIGNAL(valueChanged(double)), this, SLOT(UI_limitRandInput()));
    connect(ui->dSpinOffSet,    SIGNAL(valueChanged(double)), this, SLOT(UI_limitRandInput()));
    connect(ui->dSpinAux,       SIGNAL(valueChanged(double)), this, SLOT(UI_limitRandInput()));

    //Ordem do Sistema e comportamento do sistema
    connect(ui->radioFechada, SIGNAL(toggled(bool)), ui->frameOrdem, SLOT(setEnabled(bool)));
    connect(ui->rbSistemaO2, SIGNAL(toggled(bool)), ui->combo_Mp, SLOT(setEnabled(bool)));
    connect(ui->rbSistemaO2, SIGNAL(toggled(bool)), ui->combo_Tr, SLOT(setEnabled(bool)));
    connect(ui->rbSistemaO2, SIGNAL(toggled(bool)), ui->combo_Ts, SLOT(setEnabled(bool)));
    connect(ui->rbSistemaO2, SIGNAL(toggled(bool)), ui->canal_l1, SLOT(setChecked(bool)));
    connect(ui->rbSistemaO1, SIGNAL(toggled(bool)), ui->canal_l0, SLOT(setChecked(bool)));

    //Sistema de Segunda Ordem e Controle Simples e Cascata
    ui->gp_graf_pid_2->setVisible(false);
    ui->tabControlador->widget(TAB_ESCRAVO)->setEnabled(false);
    connect(ui->radioCascata, SIGNAL(toggled(bool)), this, SLOT(UI_tipo2Ordem_setEnable()));
    connect(ui->rbSistemaO2,  SIGNAL(toggled(bool)), this, SLOT(UI_tipo2Ordem_setEnable()));
    connect(ui->radioFechada, SIGNAL(toggled(bool)), this, SLOT(UI_tipo2Ordem_setEnable()));

    //Observador de Estado
    connect(ui->radioObservador,SIGNAL(toggled(bool)), this, SLOT(UI_connect_tipoDeSistema()));
    connect(ui->radioFechada,   SIGNAL(toggled(bool)), this, SLOT(UI_connect_tipoDeSistema()));
    connect(ui->rbSistemaO2,    SIGNAL(toggled(bool)), this, SLOT(UI_connect_tipoDeSistema()));
    connect(ui->dSpinL1, SIGNAL(valueChanged(double)), this, SLOT(UI_connect_observador_LtoP()));
    connect(ui->dSpinL2, SIGNAL(valueChanged(double)), this, SLOT(UI_connect_observador_LtoP()));
    connect(ui->dSpinP1Real, SIGNAL(valueChanged(double)), this, SLOT(UI_connect_observador_PtoL()));
    connect(ui->dSpinP2Real, SIGNAL(valueChanged(double)), this, SLOT(UI_connect_observador_PtoL()));
    connect(ui->dSpinComplexo, SIGNAL(valueChanged(double)), this, SLOT(UI_connect_observador_PtoL()));

    UI_configPanel(); /* Método principal para setar o INIT da UI */

    matls = (double**)calloc(2, sizeof(double));

    matpolos = (double**)calloc(2, sizeof(double));

    for(int i=0; i<2; i++) {
        matpolos[i] = (double*)calloc(2, sizeof(double));
    }
    for(int i=0; i<2; i++) {
        matls[i] = (double*)calloc(1, sizeof(double));
    }
}

MainWindow::~MainWindow()
{
    threadEscrita->terminate();
    threadLeitura->terminate();
    delete ui;
}
void MainWindow:: UI_connect_observador_LtoP()
{


    matls[0][0] = ui->dSpinL1->value();
    matls[1][0] = ui->dSpinL2->value();


    matpolos = control->getPoloFromL(matls);


    ui->dSpinP1Real->blockSignals(true);
    ui->dSpinP2Real->blockSignals(true);
    ui->dSpinComplexo->blockSignals(true);

    ui->dSpinP1Real->setValue(matpolos[0][0]);
    ui->dSpinComplexo->setValue(matpolos[0][1]);

    ui->dSpinP2Real->setValue(matpolos[1][0]);
    ui->labelComplexo->setText(QString::number(matpolos[1][1],'g',3));

    ui->dSpinP1Real->blockSignals(false);
    ui->dSpinP2Real->blockSignals(false);
    ui->dSpinComplexo->blockSignals(false);

}
void MainWindow:: UI_connect_observador_PtoL()
{

    double c = ui->dSpinComplexo->value();
    double p1 = ui->dSpinP1Real->value();
    if(c > -1e-5 && c < 1e-5)
    {
        ui->dSpinP2Real->setEnabled(true);
        ui->labelComplexo->setText("0.0");
    }
    else
    {
        ui->dSpinP2Real->setEnabled(false);
        ui->dSpinP2Real->setValue(p1);
        ui->labelComplexo->setText(QString::number(-c,'g',3));
    }
    double p2 = ui->dSpinP2Real->value();


    matpolos[0][0] = p1;
    matpolos[0][1] = c;
    matpolos[1][0] = p2;
    matpolos[1][1] = (-1)*c;

    //qDebug() << "p1" << p1;
   // qDebug() << "p2" << p2;
   // qDebug() << "c" << c;




    matls = control->getLFromPolo(matpolos);


    ui->dSpinL1->blockSignals(true);
    ui->dSpinL2->blockSignals(true);

    ui->dSpinL1->setValue(matls[0][0]);
    ui->dSpinL2->setValue(matls[1][0]);

    ui->dSpinL1->blockSignals(false);
    ui->dSpinL2->blockSignals(false);
}

void MainWindow:: UI_connect_tipoDeSistema()
{
    bool obs =  ui->radioFechada->isChecked() &&
                ui->rbSistemaO2->isChecked() &&
                ui->radioObservador->isChecked();
    ui->frameKP->setVisible(obs);
    ui->groupObservador->setVisible(obs);
}

void MainWindow::UI_configGraphWrite()
{
    // Legenda
    ui->graficoEscrita->legend->setVisible(true);
    ui->graficoEscrita->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignLeft|Qt::AlignBottom);

    // sinal enviado
    ui->graficoEscrita->addGraph(); // blue line
    ui->graficoEscrita->graph(GRAPH_SINAL_ENVIADO)->setPen(QPen(Qt::blue));
    ui->graficoEscrita->graph(GRAPH_SINAL_ENVIADO)->setAntialiasedFill(false);
    ui->graficoEscrita->graph(GRAPH_SINAL_ENVIADO)->setName("Sinal Enviado");

    // sinal calculado
    ui->graficoEscrita->addGraph(); // red line
    ui->graficoEscrita->graph(GRAPH_SINAL_CALCULADO)->setPen(QPen(Qt::red));
    ui->graficoEscrita->graph(GRAPH_SINAL_CALCULADO)->setAntialiasedFill(false);
    ui->graficoEscrita->graph(GRAPH_SINAL_CALCULADO)->setName("Sinal Calculado");

    //Sinal Parcial calculado
    ui->graficoEscrita->addGraph(); // red line
    ui->graficoEscrita->graph(2)->setPen(QPen(Qt::yellow));
    ui->graficoEscrita->graph(2)->setAntialiasedFill(false);
    ui->graficoEscrita->graph(2)->setName("Sinal Paricial Calculado");
    ui->graficoEscrita->graph(2)->removeFromLegend();
    ui->graficoEscrita->graph(2)->setVisible(false);

    ui->graficoEscrita->xAxis->setLabel("Tempo (s)");
    ui->graficoEscrita->yAxis->setRange(-4.5,4.5);
    ui->graficoEscrita->yAxis->setNumberPrecision(2);
    ui->graficoEscrita->yAxis->setLabel("Tensão (V) ");
    ui->graficoEscrita->xAxis->setSubTickCount(10);

    //Usuário arraste eixo varia com o mouse, zoom com a roda do mouse e selecione gráficos clicando:
    ui->graficoEscrita->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables );
    ui->graficoEscrita->axisRect(0)->setRangeDrag(Qt::Vertical);
    ui->graficoEscrita->axisRect(0)->setRangeZoom(Qt::Vertical);
}

void MainWindow::UI_configGraphRead()
{
    // Erro
    ui->graficoLeitura->addGraph(); // red line
    ui->graficoLeitura->graph(GRAPH_ERRO)->setPen(QPen(Qt::red));
    ui->graficoLeitura->graph(GRAPH_ERRO)->setVisible(false);
    ui->graficoLeitura->graph(GRAPH_ERRO)->setAntialiasedFill(false);
    ui->graficoLeitura->graph(GRAPH_ERRO)->setName("Erro");
    ui->graficoLeitura->graph(GRAPH_ERRO)->removeFromLegend();

    // Set Point
    ui->graficoLeitura->addGraph();
    ui->graficoLeitura->graph(GRAPH_SET_POINT)->setPen(QPen(Qt::black));
    ui->graficoLeitura->graph(GRAPH_SET_POINT)->setVisible(false);
    ui->graficoLeitura->graph(GRAPH_SET_POINT)->setName("Set Point");
    ui->graficoLeitura->graph(GRAPH_SET_POINT)->removeFromLegend();

    QString nomeCanal;
    for(int i=0; i<8; i++)
    {
        if(i < 2) nomeCanal = "Tanque " + QString::number(i+1);
        else nomeCanal = "Canal " + QString::number(i);

        ui->graficoLeitura->addGraph();
        ui->graficoLeitura->graph(i+2)->setPen(QPen(QColor(rand()%200+10,rand()%200+10,rand()%200+10,255)));
        ui->graficoLeitura->graph(i+2)->setAntialiasedFill(false);

        ui->graficoLeitura->graph(i+2)->setName(nomeCanal);
        ui->graficoLeitura->graph(i+2)->setVisible(false);
        ui->graficoLeitura->graph(i+2)->removeFromLegend();
    }

    for(int i=8; i<18; i++) {
        if(i == 8 )nomeCanal = "P ";
        else if(i == 9 )nomeCanal = "I ";
        else if(i == 10 )nomeCanal = "D ";
        else if(i == 11 )nomeCanal = "P Cascata ";
        else if(i == 12 )nomeCanal = "I Cascata";
        else if(i == 13 )nomeCanal = "D Cascata";
        else if(i == 14 )nomeCanal = "Obs. Tanque 01";
        else if(i == 15 )nomeCanal = "Obs. Tanque 02";
        else if(i == 16 )nomeCanal = "Erro Obs. Tq.1";
        else if(i == 17 )nomeCanal = "Erro Obs. Tq.2";

        ui->graficoLeitura->addGraph();
        ui->graficoLeitura->graph(i+2)->setPen(QPen(QColor(rand()%200+10,rand()%200+10,rand()%200+10,255)));
        ui->graficoLeitura->graph(i+2)->setAntialiasedFill(false);

        ui->graficoLeitura->graph(i+2)->setName(nomeCanal);
        ui->graficoLeitura->graph(i+2)->setVisible(false);
        ui->graficoLeitura->graph(i+2)->removeFromLegend();
    }

    ui->graficoLeitura->legend->setVisible(true);
    ui->graficoLeitura->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignLeft|Qt::AlignBottom);

    ui->graficoLeitura->xAxis->setLabel("Tempo (s)");

    ui->graficoLeitura->yAxis->setRange(0,30);
    ui->graficoLeitura->yAxis->setNumberPrecision(2);
    ui->graficoLeitura->yAxis->setLabel("Nivel do tanque (Cm) ");

    //Usuário dê zoom com a roda do mouse na vertical
    ui->graficoLeitura->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->graficoLeitura->axisRect(0)->setRangeZoom(Qt::Vertical);
    ui->graficoLeitura->axisRect(0)->setRangeDrag(Qt::Vertical);
}

void MainWindow::UI_configPlotGraficosE()
{
    sinalPlotEscrita[1] = ui->cb_graf_sinalCalculado->isChecked();
    sinalPlotEscrita[0] = ui->cb_graf_sinalEnviado->isChecked();
    sinalPlotEscrita[2] = ui->cb_SinalParcial->isChecked();

    for(int i=0; i<PLOT_ESCRITA+1; i++)
    {
        ui->graficoEscrita->graph(i)->setVisible(sinalPlotEscrita[i]);
        if(sinalPlotEscrita[i]) ui->graficoEscrita->graph(i)->addToLegend();
        else ui->graficoEscrita->graph(i)->removeFromLegend();
    }
}

void MainWindow::UI_configPlotGraficosL()
{    
    vectorGrafLeitura[0] = ui->cb_plot_erro->isChecked();
    vectorGrafLeitura[1] = ui->cb_plot_setPoint->isChecked();
    vectorGrafLeitura[2] = ui->cb_plot_p->isChecked();
    vectorGrafLeitura[3] = ui->cb_plot_i->isChecked();
    vectorGrafLeitura[4] = ui->cb_plot_d->isChecked();
    vectorGrafLeitura[5] = ui->cb_plot_p_2->isChecked();
    vectorGrafLeitura[6] = ui->cb_plot_i_2->isChecked();
    vectorGrafLeitura[7] = ui->cb_plot_d_2->isChecked();

    vectorGrafLeitura[8] = ui->checkObsTq01->isChecked();
    vectorGrafLeitura[9] = ui->checkObsTq02->isChecked();
    vectorGrafLeitura[10] = ui->checkErroObsTq01->isChecked();
    vectorGrafLeitura[11] = ui->checkErroObsTq02->isChecked();

    for(int i=0; i<2; i++)
    {
        ui->graficoLeitura->graph(i)->setVisible(vectorGrafLeitura[i]);
        if(vectorGrafLeitura[i]) ui->graficoLeitura->graph(i)->addToLegend();
        else                     ui->graficoLeitura->graph(i)->removeFromLegend();
    }
    for(int i=8;i<18; i++)
    {
        ui->graficoLeitura->graph(i+2)->setVisible(vectorGrafLeitura[i-6]);
        if(vectorGrafLeitura[i-6]) ui->graficoLeitura->graph(i+2)->addToLegend();
        else                       ui->graficoLeitura->graph(i+2)->removeFromLegend();
    }
    canalLeituraPlotVec[0] = ui->cb_plot_canal0->isChecked();
    canalLeituraPlotVec[1] = ui->cb_plot_canal1->isChecked();
    canalLeituraPlotVec[2] = ui->cb_plot_canal2->isChecked();
    canalLeituraPlotVec[3] = ui->cb_plot_canal3->isChecked();
    canalLeituraPlotVec[4] = ui->cb_plot_canal4->isChecked();
    canalLeituraPlotVec[5] = ui->cb_plot_canal5->isChecked();
    canalLeituraPlotVec[6] = ui->cb_plot_canal6->isChecked();
    canalLeituraPlotVec[7] = ui->cb_plot_canal7->isChecked();


    for(int i=0; i<NUMB_CAN_READ; i++)
    {
        if(canalLeituraPlotVec[i] && canalLeituraVec[i])
        {
            ui->graficoLeitura->graph(i+2)->addToLegend();
            ui->graficoLeitura->graph(i+2)->setVisible(true);
        }
        else
        {
            ui->graficoLeitura->graph(i+2)->removeFromLegend();
            ui->graficoLeitura->graph(i+2)->setVisible(false);
        }
    }

}

void MainWindow::UI_configPanel()
{

    // Criando canais de escrita
    for(int i=0; i<4; i++) ui->cb_canalEscrita->addItem("Canal " + QString::number(i),QVariant(i));

    ui->buttonStop->setStyleSheet("background-color: red");
    ui->pb_tanque1->setValue(0);

    // Controladores
    ///Mestre
    ui->tipoControlador->addItem("P",QVariant(CONTROLER_P));
    ui->tipoControlador->addItem("PD",QVariant(CONTROLER_PD));
    ui->tipoControlador->addItem("PI",QVariant(CONTROLER_PI));
    ui->tipoControlador->addItem("PID",QVariant(CONTROLER_PID));
    ui->tipoControlador->addItem("PI-D",QVariant(CONTROLER_PI_D));
    ///Escravo
    ui->tipoControlador_2->addItem("P",QVariant(CONTROLER_P));
    ui->tipoControlador_2->addItem("PD",QVariant(CONTROLER_PD));
    ui->tipoControlador_2->addItem("PI",QVariant(CONTROLER_PI));
    ui->tipoControlador_2->addItem("PID",QVariant(CONTROLER_PID));
    ui->tipoControlador_2->addItem("PI-D",QVariant(CONTROLER_PI_D));

    // Sinais gerados
    ui->comboTipoSinal->addItem("Degrau",QVariant(DEGRAU));
    ui->comboTipoSinal->addItem("Quadrada",QVariant(QUADRADA));
    ui->comboTipoSinal->addItem("Senoidal",QVariant(SENOIDAL));
    ui->comboTipoSinal->addItem("Dente de Serra",QVariant(DENTE_DE_SERRA));
    ui->comboTipoSinal->addItem("Aleatório",QVariant(ALEATORIO));

    //Comportamento do Sistema
    // Mestre
    ui->combo_Mp->addItem("ABS",QVariant(ABS));
    ui->combo_Mp->addItem("PICOm",QVariant(PICOm));
    ui->combo_Tr->addItem("0-100%",QVariant(Tr100));
    ui->combo_Tr->addItem("5-95%",QVariant(Tr95));
    ui->combo_Tr->addItem("10-90%",QVariant(Tr90));
    ui->combo_Ts->addItem("2%",QVariant(0));
    ui->combo_Ts->addItem("5%",QVariant(1));
    ui->combo_Ts->addItem("10%",QVariant(2));
    //Escravo
    ui->combo_Mp_2->addItem("ABS",QVariant(ABS));
    ui->combo_Mp_2->addItem("PICOm",QVariant(PICOm));
    ui->combo_Tr_2->addItem("0-100%",QVariant(Tr100));
    ui->combo_Tr_2->addItem("5-95%",QVariant(Tr95));
    ui->combo_Tr_2->addItem("10-90%",QVariant(Tr90));
    ui->combo_Ts_2->addItem("2%",QVariant(0));
    ui->combo_Ts_2->addItem("5%",QVariant(1));
    ui->combo_Ts_2->addItem("10%",QVariant(2));

    ui->dSpinAux->setVisible(false);
    ui->labelAux->setVisible(false);
    ui->frameKP->setVisible(false);

    UI_malhaAberta();
    UI_configControlador();
}

int MainWindow::UI_getTipoMalha()
{
    int tipoMalha;

    if(ui->radioAberta->isChecked()) tipoMalha = M_ABERTA;
    if(ui->radioFechada->isChecked()) tipoMalha = M_FECHADA;

    return tipoMalha;
}

void MainWindow::UI_updateConsControle(double kd_td, double ki_ti)
{
    int tipoControlador = ui->tipoControlador->currentIndex();

    if(tipoControlador == CONTROLER_PD)
    {
        ui->sp_kd_td->setValue(kd_td);
    }
    else if(tipoControlador == CONTROLER_PID || tipoControlador == CONTROLER_PI_D)
    {
        ui->sp_kd_td->setValue(kd_td);
        ui->sp_ki_ti->setValue(ki_ti);
    }
    else if(tipoControlador == CONTROLER_PI)
    {
        ui->sp_ki_ti->setValue(ki_ti);
    }
}

void MainWindow::UI_configConsControle()
{
    bool constTempo = ui->rb_constTempo->isChecked();
    bool constGanho = ui->rb_constGanho->isChecked();

    double kp = ui->sp_kp->value();
    double kd_td = ui->sp_kd_td->value();
    double ki_ti = ui->sp_ki_ti->value();

    if(constTempo)
    {
        double td = kd_td / kp;
        double ti = kp / ki_ti;

        UI_updateConsControle(td, ti);
    }
    else if(constGanho)
    {
        double kd = kp * kd_td;
        double ki = kp / ki_ti;
        UI_updateConsControle(kd, ki);
    }
}

void MainWindow::UI_configControlador()
{
    bool controlConstTempo = ui->rb_constTempo->isChecked();
    bool controlConstGanho = ui->rb_constGanho->isChecked();
    int tipoControler = ui->tipoControlador->currentIndex();

    if(controlConstGanho)
    {
        ui->lb_kd_td->setText("Kd = ");
        ui->lb_ki_ti->setText("Ki = ");
        ui->lb_kd_td_2->setText("Kd = ");
        ui->lb_ki_ti_2->setText("Ki = ");
    }
    else if(controlConstTempo)
    {
        ui->lb_kd_td->setText("Td = ");
        ui->lb_ki_ti->setText("Ti = ");
        ui->lb_kd_td_2->setText("Td = ");
        ui->lb_ki_ti_2->setText("Ti = ");
    }

    switch (tipoControler)
    {
    case CONTROLER_P:
        ui->sp_kd_td->setDisabled(true);
        ui->sp_ki_ti->setDisabled(true);

        ui->cb_plot_i->setDisabled(true);
        ui->cb_plot_d->setDisabled(true);
        break;
    case CONTROLER_PD:
        ui->sp_kd_td->setDisabled(false);
        ui->sp_ki_ti->setDisabled(true);

        ui->cb_plot_i->setDisabled(true);
        ui->cb_plot_d->setDisabled(false);
        break;
    case CONTROLER_PI:
        ui->sp_kd_td->setDisabled(true);
        ui->sp_ki_ti->setDisabled(false);

        ui->cb_plot_i->setDisabled(false);
        ui->cb_plot_d->setDisabled(true);
        break;
    case CONTROLER_PID:
        ui->sp_kd_td->setDisabled(false);
        ui->sp_ki_ti->setDisabled(false);

        ui->cb_plot_i->setDisabled(false);
        ui->cb_plot_d->setDisabled(false);
        break;
    case CONTROLER_PI_D:
        ui->sp_kd_td->setDisabled(false);
        ui->sp_ki_ti->setDisabled(false);

        ui->cb_plot_i->setDisabled(false);
        ui->cb_plot_d->setDisabled(false);
        break;
    }
}

void MainWindow::UI_configSignal()
{
    int sinalSelecionado = ui->comboTipoSinal->currentIndex();

    int tipoMalha = UI_getTipoMalha();
    if (tipoMalha == M_ABERTA) UI_malhaAberta();
    else if (tipoMalha == M_FECHADA) UI_malhaFechada();

    ui->labelOffSet->setEnabled(true);
    ui->labelPeriodo->setEnabled(true);
    ui->dSpinOffSet->setEnabled(true);
    ui->dSpinPeriodo->setEnabled(true);
    ui->dSpinAux->setVisible(false);
    ui->labelAux->setVisible(false);

    if(sinalSelecionado == DEGRAU)
    {
        ui->labelOffSet->setEnabled(false);
        ui->labelPeriodo->setEnabled(false);
        ui->dSpinOffSet->setEnabled(false);
        ui->dSpinPeriodo->setEnabled(false);
    }
    else if(sinalSelecionado == ALEATORIO)
    {
        ui->dSpinAux->setVisible(true);
        ui->labelAux->setVisible(true);

        ui->dSpinOffSet->setMaximum(ui->dSpinAmp->value());
        ui->dSpinAux->setMaximum(ui->dSpinPeriodo->value());
    }
}

void MainWindow::UI_malhaFechada()
{
    int sinalSelecionado = ui->comboTipoSinal->currentIndex();

    ui->dSpinAmp->setRange(MIN_LEVEL,MAX_LEVEL); // Limita nivel
    ui->dSpinOffSet->setRange(MIN_LEVEL,MAX_LEVEL); // Limita nivel

    ui->cb_plot_p->setDisabled(false);
    ui->cb_plot_setPoint->setDisabled(false);
    ui->cb_plot_erro->setDisabled(false);

    ui->tipoControlador->setDisabled(false);

    ui->gp_modoControle->setDisabled(false);
    ui->gp_parametrosControle->setDisabled(false);
    ui->gp_graf_pid->setDisabled(false);

    ui->tipoControlador->setCurrentIndex(0);

    if(sinalSelecionado == ALEATORIO)
    {
        ui->labelAmp->setText("Amplitude MAX (cm):");
        ui->labelOffSet->setText("Amplitude MIN (cm):");
        ui->labelPeriodo->setText("Duração MAX (s):");
        ui->labelAux->setText("Duração MIN (s):");
    }
    else
    {
        ui->labelAmp->setText("Amplitude (cm) ");
        ui->labelOffSet->setText("Off-set (cm) ");
        ui->labelPeriodo->setText("Período");
    }
}

void MainWindow::UI_malhaAberta()
{
    int sinalSelecionado = ui->comboTipoSinal->currentIndex();
    ui->dSpinAmp->setRange(MIN_VOLTAGE,MAX_VOLTAGE); // Limita a tensão entre -4V e 4V
    ui->dSpinOffSet->setRange(MIN_VOLTAGE,MAX_VOLTAGE); // Limita a tensão entre -4V e 4V

    ui->cb_plot_p->setDisabled(true);
    ui->cb_plot_setPoint->setDisabled(true);
    ui->cb_plot_erro->setDisabled(true);

    ui->tipoControlador->setDisabled(true);
    ui->gp_modoControle->setDisabled(true);
    ui->gp_parametrosControle->setDisabled(true);

    ui->gp_graf_pid->setDisabled(true);

    if(sinalSelecionado == ALEATORIO)
    {
        ui->labelAmp->setText("Amplitude MAX (V):");
        ui->labelOffSet->setText("Amplitude MIN (V):");
        ui->labelPeriodo->setText("Duração MAX (s):");
        ui->labelAux->setText("Duração MIN (s):");
    }
    else
    {
        ui->labelAmp->setText("Tensão (V) ");
        ui->labelOffSet->setText("Off-set (V) ");
        ui->labelPeriodo->setText("Período");
    }
}

void MainWindow::UI_configMalha()
{
    int tipoMalha = UI_getTipoMalha();

    if(tipoMalha == M_ABERTA) UI_malhaAberta();
    else if(tipoMalha == M_FECHADA) UI_malhaFechada();
}

void MainWindow::UI_limitRandInput()
{
    int tipoMalha = UI_getTipoMalha();

    if(tipoMalha == M_ABERTA)
    {
        ui->dSpinOffSet->setMaximum(ui->dSpinAmp->value());
        ui->dSpinAux->setMaximum(ui->dSpinPeriodo->value());
        ui->dSpinAmp->setMinimum(ui->dSpinOffSet->value());
        ui->dSpinPeriodo->setMinimum(ui->dSpinAux->value());
    }
}

void MainWindow::UI_tipo2Ordem_setEnable()
{
    ui->frameTipo2Ordem->setEnabled(ui->rbSistemaO2->isChecked() && ui->radioFechada->isChecked());
    ui->gp_graf_pid_2->setVisible(ui->rbSistemaO2->isChecked() && ui->radioFechada->isChecked());
    ui->tabControlador->widget(TAB_ESCRAVO)->setEnabled(ui->radioFechada->isChecked() &&
                                                        ui->rbSistemaO2->isChecked() &&
                                                        ui->radioCascata->isChecked());
}

// Control
void MainWindow::connectServer()
{
    int port =  conectar->getPort();
    QString enderecoIP =  conectar->getIpAdress();

    control = new Control(port,enderecoIP);

    if(control->getConnectionStatus())
    {
        this->show();
        QString mensagem = "Conectado em " + enderecoIP + ":" + QString::number(port);
        ui->labelStatus->setText(mensagem);
        ui->labelStatus->setStyleSheet("QLabel { color : green; }");

        threadEscrita->start();
        threadLeitura->start();

        UI_configGraphWrite();
        UI_configGraphRead();
    }
    else
    {
        QMessageBox::critical(this,tr("Control Quanser - Conexão Falhou!"),
                              tr("Verifique o IP e/ou porta do servidor!"));
        conectar->show();

    }
}

void MainWindow::zerarSinal()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::critical(this, "Confirmação", "Você realmente deseja zerar o sinal da planta?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        control->zerarSinal();
        QMessageBox::about(this, "Confirmação", "O sinal de envio foi zerado!");
    }
}


void MainWindow::controladorPID()
{
    if(ui->radioSimples->isChecked()) {
        control->setModeSegOrdem(C_O2_CONVENCIONAL);
    }
    else if(ui->radioCascata->isChecked()) {
        control->setModeSegOrdem(C_O2_CASCATA);
    }
    /*C_O2_SEGUIDOR seguidor */
    else {
        control->setModeSegOrdem(C_O2_CONVENCIONAL);
    }
    control->setModeSegOrdem(C_O2_SEGUIDOR);

    bool controlerGanho = ui->rb_constGanho->isChecked();
    bool controlerTempo = ui->rb_constTempo->isChecked();

    double Kp    = ui->sp_kp->value();
    double Ki_ti = ui->sp_ki_ti->value();
    double Kd_td = ui->sp_kd_td->value();
    int tipoControler = ui->tipoControlador->currentIndex();

    if(ui->rbSistemaO2->isChecked())  {
        control->setTipoOrdemSistema(SISTEMA_ORDEM_2);
    } else {
        control->setTipoOrdemSistema(SISTEMA_ORDEM_1);
    }

    if(controlerGanho) {
        control->setModeControle(CONTROLE_GANHO);
        control->setModeControleCas(CONTROLE_GANHO);
    }
    if(controlerTempo) {
        control->setModeControle(CONTROLE_CONST_TEMP);
        control->setModeControleCas(CONTROLE_CONST_TEMP);
    }

    control->setKp(Kp);
    control->setTipoControler(tipoControler);
    if(controlerGanho)
    {
        control->setKi(Ki_ti);
        control->setKd(Kd_td);
    }
    else if(controlerTempo)
    {
        control->setTempoDerivativo(Kd_td);
        control->setTempoIntegrativo(Ki_ti);
    }


    /* Controlador em cascata */
    if(ui->rbSistemaO2->isChecked() && ui->radioCascata->isChecked()) {
        double kd_tdCas = ui->sp_kd_td_2->value();
        double ki_tiCas = ui->sp_ki_ti_2->value();
        double kpCas = ui->sp_kp_2->value();
        int tipoControlerCas = ui->tipoControlador_2->currentIndex();

        if(controlerGanho) {
            control->setKdCas(kd_tdCas);
            control->setKiCas(ki_tiCas);
        }
        else {
            control->setTempoDerivativoCas(kd_tdCas);
            control->setTempoIntegrativoCas(ki_tiCas);
        }

        control->setKpCas(kpCas);
        control->setTipoControlerCas(tipoControlerCas);
    }
}


void MainWindow::data()
{
    double  amplitude = ui->dSpinAmp->value();
    double  periodo   = ui->dSpinPeriodo->value();
    double  offSet    = ui->dSpinOffSet->value();
    int     canalEscrita = ui->cb_canalEscrita->currentIndex();
    int     tipoSinal    = ui->comboTipoSinal->currentIndex();
    bool windUP = false;

    bool malhaAberta  = ui->radioAberta->isChecked();
    bool malhaFechada = ui->radioFechada->isChecked();

    bool primeiraOrdem = ui->rbSistemaO1->isChecked();
    bool segundaOrdem = ui->rbSistemaO2->isChecked();
    bool observador = ui->radioObservador->isChecked();
    //bool controladorCascataO2 = ui->radioCascata->isChecked();
    //bool controladorSimplesO2 = ui->radioSimples->isChecked();

    double polo1[2] = {ui->dSpinP1Real->value(),ui->dSpinComplexo->value()};
    double polo2[2] = {ui->dSpinP2Real->value(),-(ui->dSpinComplexo->value())};

    qDebug() << "polos main windows";
    qDebug() << polo1[0] << polo1[1];
    qDebug() << polo2[0] << polo2[1];
    if(malhaAberta) {
        control->setTensao(amplitude);
        control->setTipoMalha(M_ABERTA);
        control->setWindUP(false);
    } else if(malhaFechada) {
        windUP = ui->windup->isChecked();
        control->setWindUP(windUP);
        control->setTensao(0);
        control->setAmplitude(amplitude);
        control->setTipoMalha(M_FECHADA);

        controladorPID();

        if(primeiraOrdem) {
            control->setTipoOrdemSistema(SISTEMA_ORDEM_1);
            control->setCanalLeitura(0);
        } else if(segundaOrdem) {
            int tipoTr = ui->combo_Tr->currentIndex();
            int tipoTs = ui->combo_Ts->currentIndex();
            int tipoMp = ui->combo_Mp->currentIndex();

            control->setTipoTr(tipoTr);
            control->setTipoTs(tipoTs);
            control->setTipoMp(tipoMp);

            control->setTipoOrdemSistema(SISTEMA_ORDEM_2);
            control->setCanalLeitura(1);


        }
        if(observador) {
            control->setPolos(polo1,polo2);
            control->setObservador(true);
        } else {
            control->setObservador(false);
        }
    }

    if(tipoSinal == ALEATORIO) {
        double auxForRand = ui->dSpinAux->value();
        control->setAuxForRand(auxForRand);
    }

    control->setCanalEscrita(canalEscrita);
    control->setPeriodo(periodo);
    control->setOffSet(offSet);
    control->setTipoSinal(tipoSinal);
}

void MainWindow::sendData()
{
    control->sendSignal();

    double sinalEscrita   = control->getSinalEnviado();
    double sinalCalculado = control->getSinalCalculado();
    int canalEscrita = control->getCanalEscrita();

    QString str_canalEscrita;
    if(canalEscrita < 0) str_canalEscrita = "Nenhum canal selecionado";
    else str_canalEscrita = "Escrevendo em canal " + QString::number(canalEscrita);

    QString str_sinalEnviado   = "Sinal enviado = " + QString::number(sinalEscrita) + " V";
    QString str_sinalCalculado = "Sinal calculado = " + QString::number(sinalCalculado) + " V";

    ui->lb_tensaoEscrita->setText(str_sinalEnviado);
    ui->lb_tensaoCalculada->setText(str_sinalCalculado);
    ui->lb_canalEscrita->setText(str_canalEscrita);

    ui->graficoEscrita->graph(0)->addData(tempoEscrita, sinalEscrita);
    ui->graficoEscrita->graph(1)->addData(tempoEscrita, sinalCalculado);
    ui->graficoEscrita->graph(0)->removeData(tempoEscrita-120);
    ui->graficoEscrita->graph(1)->removeData(tempoEscrita-120);

    ui->graficoEscrita->xAxis->setRange(tempoEscrita, 60, Qt::AlignRight);
    tempoEscrita += 0.1;
    ui->graficoEscrita->replot();
}

void MainWindow::receiveData()
{
    control->receiveSigal();

    canalLeitura = control->getCanalLeitura();
    tipoMalha = control->getTipoMalha();
    ordemSistema = control->getOrdemSistema();


    for(int i=0; i<2; i++) {

        tanque1 = control->getCanalValue(0);
        tanque2 = control->getCanalValue(1);

        if(ordemSistema == SISTEMA_ORDEM_1) sinalLeitura = tanque1;
        else if(ordemSistema == SISTEMA_ORDEM_2) sinalLeitura = tanque2;

        ui->pb_tanque1->setValue(tanque1);
        ui->pb_tanque2->setValue(tanque2);
        ui->graficoLeitura->graph(0+2)->addData(tempoLeitura,tanque1);
        ui->graficoLeitura->graph(1+2)->addData(tempoLeitura,tanque2);

        if(i == canalLeitura) {
            if(tipoMalha == M_FECHADA) {
                double erro = control->getErro();
                double setPoint = control->getAmplitude();

                double ii = control->getI();
                double pp = control->getP();
                double dd = control->getD();

                Controller *ccas = new Controller();
                ccas = control->getControlerEsc();
                double ii2 = ccas->getI();
                double pp2 = ccas->getP();
                double dd2 = ccas->getD();

                double obsTanque1 = control->getObsTan1();
                double obsTanque2 = control->getObsTan2();
                double obsErTanque1 = control->getObsErTan1();
                double obsErTanque2 = control->getObsErTan2();


                ui->lb_erro->setText("Erro = " + QString::number(erro) + " cm");

                ui->graficoLeitura->graph(0)->addData(tempoLeitura,erro);
                ui->graficoLeitura->graph(1)->addData(tempoLeitura,setPoint);
                ui->graficoLeitura->graph(0)->removeData(tempoLeitura-120);
                ui->graficoLeitura->graph(1)->removeData(tempoLeitura-120);

                ui->graficoLeitura->graph(10)->addData(tempoLeitura,pp);
                ui->graficoLeitura->graph(11)->addData(tempoLeitura,ii);
                ui->graficoLeitura->graph(12)->addData(tempoLeitura,dd);
                ui->graficoLeitura->graph(10)->removeData(tempoLeitura-120);
                ui->graficoLeitura->graph(11)->removeData(tempoLeitura-120);
                ui->graficoLeitura->graph(12)->removeData(tempoLeitura-120);

                ui->graficoLeitura->graph(13)->addData(tempoLeitura,pp2);
                ui->graficoLeitura->graph(14)->addData(tempoLeitura,ii2);
                ui->graficoLeitura->graph(15)->addData(tempoLeitura,dd2);
                ui->graficoLeitura->graph(13)->removeData(tempoLeitura-120);
                ui->graficoLeitura->graph(14)->removeData(tempoLeitura-120);
                ui->graficoLeitura->graph(15)->removeData(tempoLeitura-120);

                ui->graficoLeitura->graph(16)->addData(tempoLeitura,obsTanque1);
                ui->graficoLeitura->graph(17)->addData(tempoLeitura,obsTanque2);
                ui->graficoLeitura->graph(18)->addData(tempoLeitura,obsErTanque1);
                ui->graficoLeitura->graph(19)->addData(tempoLeitura,obsErTanque2);

                ui->graficoLeitura->graph(16)->removeData(obsTanque1-120);
                ui->graficoLeitura->graph(17)->removeData(obsTanque2-120);
                ui->graficoLeitura->graph(18)->removeData(obsErTanque1-120);
                ui->graficoLeitura->graph(19)->removeData(obsErTanque2-120);

                if(ordemSistema == SISTEMA_ORDEM_2) {
                    bool statusTr = control->getStatusTr();
                    bool statusTp = control->getStatusTp();
                    bool statusMp = control->getStatusMp();
                    bool statusTs = control->getStatusTs();

                    double tr = control->getTr();
                    double tp = control->getTp();
                    double ts = control->getTs();
                    double mp = control->getMp();

                    double pmp;
                    pmp = mp/MAX_LEVEL;
                    pmp *= 100;

                    if(statusTr) ui->lb_tr->setText("Tr = " + QString::number(tr) + " ms");
                    else ui->lb_tr->clear();
                    if(statusTp) ui->lb_tp->setText("Tp = " + QString::number(tp) + " ms");
                    else ui->lb_tp->clear();
                    if(statusTs) ui->lb_ts->setText("Ts = " + QString::number(ts) + " s");
                    else ui->lb_ts->clear();
                    if(statusMp) ui->lb_mp->setText("Mp = " + QString::number(mp) + " cm (" + QString::number(pmp) +"%)");
                    else ui->lb_mp->clear();


                } else {
                    ui->lb_tr->clear();
                    ui->lb_tp->clear();
                    ui->lb_mp->clear();
                    ui->lb_ts->clear();
                }
            }
            ui->lb_sinalLido->setText("Nível do tanque 1 = " + QString::number(tanque1) + " cm");
            ui->lb_sinalLidoT2->setText("Nível do tanque 2 = " + QString::number(tanque2) + " cm");
        }
    }

    ui->graficoLeitura->xAxis->setRange(tempoLeitura, 60, Qt::AlignRight);
    tempoLeitura += 0.1;
    ui->graficoLeitura->replot();
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{

    if(event->key() == Qt::Key_F1)
    {
        ui->graficoEscrita->saveJpg("Grafico Escrita.jpg",0,0,9,10);
    }
    else if (event->key() == Qt::Key_F2)
    {
        ui->graficoLeitura->saveJpg("Grafico Leitura.jpg");
    }
}
