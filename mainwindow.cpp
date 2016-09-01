#include "mainwindow.h"
#include "ui_mainwindow.h"

#define TAB_SINAL 0
#define TAB_CONTROLE 1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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

    // Canais de Leitura e Escrita
    connect(ui->canal_l0, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l1, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l2, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l3, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l4, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l5, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l6, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->canal_l7, SIGNAL(clicked(bool)), this, SLOT(UI_configCanais()));
    connect(ui->cb_canalEscrita, SIGNAL(activated(int)), this, SLOT(UI_configCanais()));


    // Botões
    connect(ui->buttonConectar, SIGNAL(clicked(bool)),this,SLOT(connectServer()));
    connect(ui->buttonAtualizar,SIGNAL(clicked(bool)),this,SLOT(data()));
    connect(ui->buttonStop,     SIGNAL(clicked(bool)),this,SLOT(stopAll()));

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
    ui->graficoLeitura->graph(0)->setAntialiasedFill(false);
    ui->graficoLeitura->graph(0)->setName("Erro");

    // Set Point
    ui->graficoLeitura->addGraph();
    ui->graficoLeitura->graph(1)->setPen(QPen(Qt::black));
    ui->graficoLeitura->graph(1)->setVisible(true);
    ui->graficoLeitura->graph(1)->setName("Set Point");


    for(int i=0; i<8; i++)
    {
        ui->graficoLeitura->addGraph();
        ui->graficoLeitura->graph(i+2)->setPen(QPen(QColor(rand()%200+10,rand()%200+10,rand()%200+10,255)));
        ui->graficoLeitura->graph(i+2)->setAntialiasedFill(false);
        ui->graficoLeitura->graph(i+2)->setName("Canal " + QString::number(i));
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


void MainWindow::UI_configPanel()
{

    // Criando canais de escrita
    for(int i=0; i<4; i++) ui->cb_canalEscrita->addItem("Canal " + QString::number(i),QVariant(i));



    ui->buttonAtualizar->setStyleSheet("background-color: blue");
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
}

int MainWindow::UI_getTipoMalha()
{
    int tipoMalha;

    if(ui->radioAberta->isChecked()) tipoMalha = M_ABERTA;
    if(ui->radioFechada->isChecked()) tipoMalha = M_FECHADA;

    return tipoMalha;
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

    canalEscrita = ui->cb_canalEscrita->currentIndex();

    for(int i=0; i<NUMB_CAN_READ; i++)
    {
        if(canalLeituraVec[i])
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
        break;
    case CONTROLER_PD:
        ui->sp_kd_td->setDisabled(false);
        ui->sp_ki_ti->setDisabled(true);
        break;
    case CONTROLER_PI:
        ui->sp_kd_td->setDisabled(true);
        ui->sp_ki_ti->setDisabled(false);
        break;
    case CONTROLER_PID:
        ui->sp_kd_td->setDisabled(false);
        ui->sp_ki_ti->setDisabled(false);
        break;
    case CONTROLER_PI_D:
        ui->sp_kd_td->setDisabled(false);
        ui->sp_ki_ti->setDisabled(false);
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
    ui->dSpinOffSet->setMaximum(ui->dSpinAmp->value());
    ui->dSpinAux->setMaximum(ui->dSpinPeriodo->value());
    ui->dSpinAmp->setMinimum(ui->dSpinOffSet->value());
    ui->dSpinPeriodo->setMinimum(ui->dSpinAux->value());
}


// Control

void MainWindow::connectServer()
{
    control = new Control();

    if(control->connectionStatus())
    {
        ui->labelStatus->setText("Conectado!");
        ui->labelStatus->setStyleSheet("QLabel { color : green; }");
        ui->buttonConectar->setDisabled(true);

        threadEscrita->start();
        threadLeitura->start();

        UI_configGraphWrite();
        UI_configGraphRead();
    }
    else
    {
        ui->labelStatus->setText("Conexão Falhou!");
        ui->labelStatus->setStyleSheet("QLabel { color : red; }");

        QMessageBox::critical(this,tr("Conexão Falhou!"),tr("Verifique o IP e/ou porta do servidor!"));
    }
}

void MainWindow::stopAll()
{

    QMessageBox::StandardButton reply;

    reply = QMessageBox::critical(this, "Confirmação", "Você realmente deseja parar a planta?",
                                    QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        control->setTensao(0);
        control->setTipoMalha(M_ABERTA);
        //QApplication::quit();
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

    QString str_sinalEnviado   = "Sinal enviado = " + QString::number(sinalEscrita) + " V";
    QString str_sinalCalculado = "Sinal calculado = " + QString::number(sinalCalculado) + " V";

    ui->lb_tensaoEscrita->setText(str_sinalEnviado);
    ui->lb_tensaoCalculada->setText(str_sinalCalculado);

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
            double value = control->getCanalValue(i);
            ui->graficoLeitura->graph(i+2)->addData(key/5,value);

            if(i == canalLeitura)
            {
                sinalLeitura = value;
                if(tipoMalha == M_FECHADA) // Malha fechada
                {
                    double erro = control->getErro();
                    double setPoint = control->getAmplitude();

                    ui->lb_erro->setText("Erro = " + QString::number(erro) + " cm");

                    ui->graficoLeitura->graph(0)->addData(key/5,erro);
                    ui->graficoLeitura->graph(1)->addData(key/5,setPoint);
                }
            }
        }
    }

    QString str_sinalLeitura = "Nível do tanque = " + QString::number(sinalLeitura) + " cm";
    ui->lb_sinalLido->setText(str_sinalLeitura);

    ui->graficoLeitura->xAxis->setRange((key+0.25)/5, 8, Qt::AlignRight);
    ui->graficoLeitura->replot();

    ui->pb_tanque1->setValue(MAX_LEVEL/sinalLeitura);
}
