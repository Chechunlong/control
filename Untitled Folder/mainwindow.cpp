#include "mainwindow.h"
#include "ui_mainwindow.h"

#define TAB_SINAL 0
#define TAB_CONTROLE 1

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
    connect(ui->radioFechada, SIGNAL(toggled(bool)), ui->tab_sinal_controle->widget(TAB_CONTROLE), SLOT(setEnabled(bool)));
    connect(ui->radioAberta,  SIGNAL(toggled(bool)), ui->tab_sinal_controle->widget(TAB_CONTROLE), SLOT(setDisabled(bool)));
    connect(ui->rb_constTempo, SIGNAL(clicked(bool)),this,SLOT(UI_configConsControle()));
    connect(ui->rb_constGanho, SIGNAL(clicked(bool)),this,SLOT(UI_configConsControle()));

    // Canais de Leitura e Escrita
    connect(ui->canal_l0, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l1, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l2, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l3, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l4, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l5, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l6, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l7, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    //connect(ui->cb_canalEscrita, SIGNAL(activated(int)), this, SLOT(UI_configCanais()));

    connect(ui->cb_plot_canal0, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal1, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal2, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal3, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal4, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal5, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal6, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_canal7, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));

    // Grafico
    connect(ui->cb_plot_erro, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_setPoint, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_p, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_i, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_plot_d, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosL()));
    connect(ui->cb_graf_sinalCalculado, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosE()));
    connect(ui->cb_graf_sinalEnviado, SIGNAL(clicked(bool)), this, SLOT(UI_configPlotGraficosE()));

    // Botões
    //connect(ui->buttonConectar, SIGNAL(clicked(bool)),this,SLOT(connectServer()));
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

    UI_configPanel(); /* Método principal para setar o INIT da UI */
}

MainWindow::~MainWindow()
{
    threadEscrita->terminate();
    threadLeitura->terminate();
    delete ui;
}



void MainWindow::UI_configGraphWrite()
{
    // Legenda
    ui->graficoEscrita->legend->setVisible(true);
    ui->graficoEscrita->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignLeft|Qt::AlignBottom);
    //ui->graficoEscrita->axisRect()->setupFullAxesBox();

    // sinal enviado
    ui->graficoEscrita->addGraph(); // blue line
    ui->graficoEscrita->graph(0)->setPen(QPen(Qt::blue));
    ui->graficoEscrita->graph(0)->setAntialiasedFill(false);
    ui->graficoEscrita->graph(0)->setName("Sinal Enviado");

    // sinal calculado
    ui->graficoEscrita->addGraph(); // red line
    ui->graficoEscrita->graph(1)->setPen(QPen(Qt::red));
    ui->graficoEscrita->graph(1)->setAntialiasedFill(false);
    ui->graficoEscrita->graph(1)->setName("Sinal Calculado");

    ui->graficoEscrita->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->graficoEscrita->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->graficoEscrita->xAxis->setAutoTickStep(true);
    ui->graficoEscrita->xAxis->setTickStep(1);
    ui->graficoEscrita->xAxis->setLabel("Tempo (hh:mm:ss)");

    ui->graficoEscrita->yAxis->setRange(-4.5,4.5);
    ui->graficoEscrita->yAxis->setNumberPrecision(2);
    ui->graficoEscrita->yAxis->setLabel("Tensão (V) ");

    //connect(ui->graficoEscrita->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->graficoEscrita->xAxis2, SLOT(setRange(QCPRange)));
    //connect(ui->graficoEscrita->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graficoEscrita->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::UI_configGraphRead()
{
    // Erro
    ui->graficoLeitura->addGraph(); // red line
    ui->graficoLeitura->graph(0)->setPen(QPen(Qt::red));
    ui->graficoLeitura->graph(0)->setVisible(false);
    ui->graficoLeitura->graph(0)->setAntialiasedFill(false);
    ui->graficoLeitura->graph(0)->setName("Erro");
    ui->graficoLeitura->graph(0)->removeFromLegend();

    // Set Point
    ui->graficoLeitura->addGraph();
    ui->graficoLeitura->graph(1)->setPen(QPen(Qt::black));
    ui->graficoLeitura->graph(1)->setVisible(false);
    ui->graficoLeitura->graph(1)->setName("Set Point");
    ui->graficoLeitura->graph(1)->removeFromLegend();

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

    ui->graficoLeitura->legend->setVisible(true);
    ui->graficoLeitura->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignLeft|Qt::AlignBottom);

    ui->graficoLeitura->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->graficoLeitura->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->graficoLeitura->xAxis->setAutoTickStep(true);
    ui->graficoLeitura->xAxis->setTickStep(1);
    ui->graficoLeitura->xAxis->setLabel("Tempo (hh:mm:ss)");

    ui->graficoLeitura->yAxis->setRange(0,30);
    ui->graficoLeitura->yAxis->setNumberPrecision(2);
    ui->graficoLeitura->yAxis->setLabel("Nivel do tanque (Cm) ");
}

void MainWindow::UI_configPlotGraficosE()
{

    sinalPlotEscrita[1] = ui->cb_graf_sinalCalculado->isChecked();
    sinalPlotEscrita[0] = ui->cb_graf_sinalEnviado->isChecked();

    for(int i=0; i<PLOT_ESCRITA; i++)
    {
        if(sinalPlotEscrita[i])
        {
            ui->graficoEscrita->graph(i)->addToLegend();
            ui->graficoEscrita->graph(i)->setVisible(true);
        }
        else
        {
            ui->graficoEscrita->graph(i)->removeFromLegend();
            ui->graficoEscrita->graph(i)->setVisible(false);
        }
    }
}

void MainWindow::UI_configPlotGraficosL()
{
    vectorGrafLeitura[0] = ui->cb_plot_erro->isChecked();
    vectorGrafLeitura[1] = ui->cb_plot_setPoint->isChecked();
    vectorGrafLeitura[2] = ui->cb_plot_p->isChecked();
    vectorGrafLeitura[3] = ui->cb_plot_i->isChecked();
    vectorGrafLeitura[4] = ui->cb_plot_d->isChecked();

    for(int i=0; i<2; i++)
    {
        if(vectorGrafLeitura[i])
        {
            ui->graficoLeitura->graph(i)->addToLegend();
            ui->graficoLeitura->graph(i)->setVisible(true);
        }
        else
        {
            ui->graficoLeitura->graph(i)->removeFromLegend();
            ui->graficoLeitura->graph(i)->setVisible(false);
        }
    }


    /*if(!ui->canal_l0->isChecked()) ui->cb_plot_canal0->setChecked(false);
    if(!ui->canal_l1->isChecked()) ui->cb_plot_canal1->setChecked(false);
    if(!ui->canal_l2->isChecked()) ui->cb_plot_canal2->setChecked(false);
    if(!ui->canal_l3->isChecked()) ui->cb_plot_canal3->setChecked(false);
    if(!ui->canal_l4->isChecked()) ui->cb_plot_canal4->setChecked(false);
    if(!ui->canal_l5->isChecked()) ui->cb_plot_canal5->setChecked(false);
    if(!ui->canal_l6->isChecked()) ui->cb_plot_canal6->setChecked(false);
    if(!ui->canal_l7->isChecked()) ui->cb_plot_canal7->setChecked(false);*/

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

    //ui->buttonAtualizar->setStyleSheet("background-color: blue");
    ui->buttonStop->setStyleSheet("background-color: red");
    ui->pb_tanque1->setValue(0);

    // Controladores
    ui->tipoControlador->addItem("P",QVariant(CONTROLER_P));
    ui->tipoControlador->addItem("PD",QVariant(CONTROLER_PD));
    ui->tipoControlador->addItem("PI",QVariant(CONTROLER_PI));
    ui->tipoControlador->addItem("PID",QVariant(CONTROLER_PID));
    ui->tipoControlador->addItem("PI-D",QVariant(CONTROLER_PI_D));

    // Sinais gerados
    ui->comboTipoSinal->addItem("Degrau",QVariant(0));
    ui->comboTipoSinal->addItem("Quadrada",QVariant(1));
    ui->comboTipoSinal->addItem("Senoidal",QVariant(2));
    ui->comboTipoSinal->addItem("Dente de Serra",QVariant(3));
    ui->comboTipoSinal->addItem("Aleatório",QVariant(4));

    ui->dSpinAux->setVisible(false);
    ui->labelAux->setVisible(false);

    UI_malhaAberta();
    UI_configControlador();

    //UI_canaisLeituraPlot();


}

int MainWindow::UI_getTipoMalha()
{
    int tipoMalha;

    if(ui->radioAberta->isChecked()) tipoMalha = M_ABERTA;
    if(ui->radioFechada->isChecked()) tipoMalha = M_FECHADA;

    return tipoMalha;
}

void MainWindow::UI_canaisLeituraPlot()
{


}

void MainWindow::UI_configCanais()
{
    canalLeituraVec[0] = ui->canal_l0->isChecked();
    canalLeituraVec[1] = ui->canal_l1->isChecked();
    canalLeituraVec[2] = ui->canal_l2->isChecked();
    canalLeituraVec[3] = ui->canal_l3->isChecked();
    canalLeituraVec[4] = ui->canal_l4->isChecked();
    canalLeituraVec[5] = ui->canal_l5->isChecked();
    canalLeituraVec[6] = ui->canal_l6->isChecked();
    canalLeituraVec[7] = ui->canal_l7->isChecked();

    ui->cb_plot_canal0->setDisabled((!ui->canal_l0->isChecked()));
    ui->cb_plot_canal1->setDisabled(!ui->canal_l1->isChecked());
    ui->cb_plot_canal2->setDisabled(!ui->canal_l2->isChecked());
    ui->cb_plot_canal3->setDisabled(!ui->canal_l3->isChecked());
    ui->cb_plot_canal4->setDisabled(!ui->canal_l4->isChecked());
    ui->cb_plot_canal5->setDisabled(!ui->canal_l5->isChecked());
    ui->cb_plot_canal6->setDisabled(!ui->canal_l6->isChecked());
    ui->cb_plot_canal7->setDisabled(!ui->canal_l7->isChecked());

    UI_configPlotGraficosL();
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
    }
    else if(controlConstTempo)
    {
        ui->lb_kd_td->setText("Td = ");
        ui->lb_ki_ti->setText("Ti = ");
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


// Control

void MainWindow::connectServer()
{
    int port =  conectar->getPort();
    QString enderecoIP =  conectar->getIpAdress();

    control = new Control(port,enderecoIP);

    if(control->connectionStatus())
    {
        this->show();

        QString mensagem = "Conectado em " + enderecoIP + ":" + QString::number(port);
        ui->labelStatus->setText(mensagem);
        ui->labelStatus->setStyleSheet("QLabel { color : green; }");
        //ui->buttonConectar->setDisabled(true);

        threadEscrita->start();
        threadLeitura->start();

        UI_configGraphWrite();
        UI_configGraphRead();
    }
    else
    {
        //ui->labelStatus->setText("Conexão Falhou!");
        //ui->labelStatus->setStyleSheet("QLabel { color : red; }");

        QMessageBox::critical(this,tr("Control Quanser - Conexão Falhou!"),tr("Verifique o IP e/ou porta do servidor!"));

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
        //control->setTensao(0);
        //control->setTipoMalha(M_ABERTA);
        //QApplication::quit();
        control->zerarSinal();
        QMessageBox::about(this, "Confirmação", "O sinal de envio foi zerado!");
    }
}

void MainWindow::controladorPID()
{
    bool controlerGanho = ui->rb_constGanho->isChecked();
    bool controlerTempo = ui->rb_constTempo->isChecked();

    double Kp    = ui->sp_kp->value();
    double Ki_ti = ui->sp_ki_ti->value();
    double Kd_td = ui->sp_kd_td->value();

    int tipoControler = ui->tipoControlador->currentIndex();

    control->setTipoControler(tipoControler);

    if(controlerGanho) control->setModeControle(CONTROLE_GANHO);
    if(controlerTempo) control->setModeControle(CONTROLE_CONST_TEMP);

    control->setKp(Kp);
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

    control->setTipoControler(tipoControler);
}

void MainWindow::data()
{
    double amplitude = ui->dSpinAmp->value();
    double periodo   = ui->dSpinPeriodo->value();
    double offSet    = ui->dSpinOffSet->value();
    int canalEscrita = ui->cb_canalEscrita->currentIndex();
    int tipoSinal    = ui->comboTipoSinal->currentIndex();

    bool malhaAberta  = ui->radioAberta->isChecked();
    bool malhaFechada = ui->radioFechada->isChecked();

    bool primeiraOrdem = ui->rbSistemaO1->isChecked();
    bool segundaOrdem = ui->rbSistemaO2->isChecked();


    if(malhaAberta)
    {
        control->setTensao(amplitude);
        control->setTipoMalha(M_ABERTA);
    }
    else if(malhaFechada)
    {
        control->setTensao(0);
        control->setAmplitude(amplitude);
        control->setTipoMalha(M_FECHADA);

        controladorPID();

        if(primeiraOrdem) {
            control->setTipoOrdemSistema(SISTEMA_ORDEM_1);
        } else if(segundaOrdem) {
            control->setTipoOrdemSistema(SISTEMA_ORDEM_2);
        }
    }

    if(tipoSinal == ALEATORIO)
    {
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
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    control->sendSignal();

    double sinalEscrita   = control->getSinalEnviado();
    double sinalCalculado = control->getSinalCalculado();
    int canalEscrita = control->getCanalEscrita();

    QString str_canalEscrita;
    if(canalEscrita < 0) str_canalEscrita = "Nenhum canal selecionado";
    else str_canalEscrita = "Escrevendo em canal" + QString::number(canalEscrita);

    QString str_sinalEnviado   = "Sinal enviado = " + QString::number(sinalEscrita) + " V";
    QString str_sinalCalculado = "Sinal calculado = " + QString::number(sinalCalculado) + " V";

    ui->lb_tensaoEscrita->setText(str_sinalEnviado);
    ui->lb_tensaoCalculada->setText(str_sinalCalculado);
    ui->lb_canalEscrita->setText(str_canalEscrita);

    ui->graficoEscrita->graph(0)->addData(key/5, sinalEscrita);
    ui->graficoEscrita->graph(1)->addData(key/5, sinalCalculado);

    ui->graficoEscrita->xAxis->setRange((key + 0.25)/5, 8, Qt::AlignRight);
    ui->graficoEscrita->replot();
}

void MainWindow::receiveData()
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    control->receiveSigal();

    int canalLeitura = control->getCanalEscrita();
    int tipoMalha = control->getTipoMalha();

    double sinalLeitura = 0;

    for(int i=0; i<NUMB_CAN_READ; i++)
    {
        if(canalLeituraVec[i])
        {
            sinalLeitura = control->getCanalValue(i);
            ui->graficoLeitura->graph(i+2)->addData(key/5,sinalLeitura);


            /*
                Se o canal de leitura for igual ao canal de escrita, deve ser
                obtido dados em relaçao ao sinal enviado, como o erro, etc.
            */
            if(i == canalLeitura)
            {
                if(tipoMalha == M_FECHADA) // Malha fechada
                {
                    double erro = control->getErro();
                    double setPoint = control->getAmplitude();

                    ui->lb_erro->setText("Erro = " + QString::number(erro) + " cm");

                    ui->graficoLeitura->graph(0)->addData(key/5,erro);
                    ui->graficoLeitura->graph(1)->addData(key/5,setPoint);

                }
                QString str_sinalLeitura = "Nível do tanque = " + QString::number(sinalLeitura) + " cm";
                ui->lb_sinalLido->setText(str_sinalLeitura);
            }
        }
    }

    ui->graficoLeitura->xAxis->setRange((key+0.25)/5, 8, Qt::AlignRight);
    ui->graficoLeitura->replot();

    ui->pb_tanque1->setValue(MAX_LEVEL/sinalLeitura);
}
