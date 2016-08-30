#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    connect(ui->buttonConectar, SIGNAL(clicked(bool)),this,SLOT(connectServer()));
    connect(ui->buttonAtualizar,SIGNAL(clicked(bool)),this,SLOT(data()));
    connect(ui->radioAberta,    SIGNAL(clicked(bool)),this,SLOT(UI_openLoop()));
    connect(ui->radioFechada,   SIGNAL(clicked(bool)),this,SLOT(UI_closedLoop()));
    connect(ui->comboTipoSinal, SIGNAL(currentIndexChanged(int)),this,SLOT(UI_configSignal()));

    connect(ui->dSpinAmp,       SIGNAL(valueChanged(double)), this, SLOT(UI_limitRandInput()));
    connect(ui->dSpinPeriodo,   SIGNAL(valueChanged(double)), this, SLOT(UI_limitRandInput()));
    connect(ui->dSpinOffSet,    SIGNAL(valueChanged(double)), this, SLOT(UI_limitRandInput()));
    connect(ui->dSpinAux,       SIGNAL(valueChanged(double)), this, SLOT(UI_limitRandInput()));

    connect(ui->actionCanal,SIGNAL(triggered(bool)), config, SLOT(show()));
    connect(config,         SIGNAL(accepted()), this, SLOT(JB_dataConfig()));

//    connect(ui->canal_0, SIGNAL(stateChanged(int)),this, SLOT(UI_canalReadSelect()));
//    connect(ui->canal_1, SIGNAL(stateChanged(int)),this, SLOT(UI_canalReadSelect()));
//    connect(ui->canal_2, SIGNAL(stateChanged(int)),this, SLOT(UI_canalReadSelect()));
//    connect(ui->canal_3, SIGNAL(stateChanged(int)),this, SLOT(UI_canalReadSelect()));
//    connect(ui->canal_4, SIGNAL(stateChanged(int)),this, SLOT(UI_canalReadSelect()));
//    connect(ui->canal_5, SIGNAL(stateChanged(int)),this, SLOT(UI_canalReadSelect()));
//    connect(ui->canal_6, SIGNAL(stateChanged(int)),this, SLOT(UI_canalReadSelect()));
//    connect(ui->canal_7, SIGNAL(stateChanged(int)),this, SLOT(UI_canalReadSelect()));

    UI_configPanel();
    UI_configGraphWrite();
    UI_configGraphRead();
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
    ui->graficoEscrita->axisRect()->setupFullAxesBox();

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
    ui->graficoEscrita->xAxis->setDateTimeFormat("s");
    ui->graficoEscrita->xAxis->setAutoTickStep(true);
    ui->graficoEscrita->xAxis->setTickStep(1);
    ui->graficoEscrita->xAxis->setLabel("Tempo (s)");

    ui->graficoEscrita->yAxis->setRange(-7,7);
    ui->graficoEscrita->yAxis->setNumberPrecision(1);
    ui->graficoEscrita->yAxis->setLabel("Tensao (v) ");

    connect(ui->graficoEscrita->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->graficoEscrita->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->graficoEscrita->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graficoEscrita->yAxis2, SLOT(setRange(QCPRange)));
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
    ui->graficoLeitura->xAxis->setDateTimeFormat("s");
    ui->graficoLeitura->xAxis->setAutoTickStep(false);
    ui->graficoLeitura->xAxis->setTickStep(1);
    ui->graficoLeitura->xAxis->setLabel("Tempo");

    ui->graficoLeitura->yAxis->setRange(0,30);
    ui->graficoLeitura->yAxis->setNumberPrecision(2);
    ui->graficoLeitura->yAxis->setLabel("Nivel do tanque (Cm) ");
}

//void MainWindow::UI_canalReadSelect()
//{
//    canalLeituraVec[0] = ui->canal_0->isChecked();
//    canalLeituraVec[1] = ui->canal_1->isChecked();
//    canalLeituraVec[2] = ui->canal_2->isChecked();
//    canalLeituraVec[3] = ui->canal_3->isChecked();
//    canalLeituraVec[4] = ui->canal_4->isChecked();
//    canalLeituraVec[5] = ui->canal_5->isChecked();
//    canalLeituraVec[6] = ui->canal_6->isChecked();
//    canalLeituraVec[7] = ui->canal_7->isChecked();

//    for(int i=0; i<NUMB_CAN_READ; i++)
//    {
//        if(canalLeituraVec[i])
//        {
//            ui->graficoLeitura->graph(i+2)->addToLegend();
//            ui->graficoLeitura->graph(i+2)->setVisible(true);
//        }
//        else
//        {
//            ui->graficoLeitura->graph(i+2)->removeFromLegend();
//            ui->graficoLeitura->graph(i+2)->setVisible(false);
//        }
//    }
//}

void MainWindow::UI_configPanel()
{
    //ui->groupConf->setDisabled(true);

    // Sinais gerados
    ui->comboTipoSinal->addItem("Degrau",QVariant(0));
    ui->comboTipoSinal->addItem("Quadrada",QVariant(1));
    ui->comboTipoSinal->addItem("Senoidal",QVariant(2));
    ui->comboTipoSinal->addItem("Dente de Serra",QVariant(3));
    ui->comboTipoSinal->addItem("Aleatório",QVariant(4));

    ui->dSpinAux->setVisible(false);
    ui->labelAux->setVisible(false);

    ui->radioAberta->setChecked(true); // Seta tipoMalha aberta no início
    UI_openLoop();
}

void MainWindow::UI_configSignal()
{
    int sinalSelecionado = ui->comboTipoSinal->currentIndex();

    if (ui->radioAberta->isChecked()) UI_openLoop();
    else if (ui->radioFechada->isChecked()) UI_closedLoop();

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

void MainWindow::UI_closedLoop()
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

void MainWindow::UI_openLoop()
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

        ui->groupConf->setDisabled(false);
        //ui->groupCanalLeitura->setDisabled(false);

        threadEscrita->start();
        threadLeitura->start();
    }
    else
    {
        ui->labelStatus->setText("Conexão Falhou!");
        ui->labelStatus->setStyleSheet("QLabel { color : red; }");

        QMessageBox::critical(this,tr("Conexão Falhou!"),tr("Verifique o IP e/ou porta do servidor!"));
    }
}

void MainWindow::data()
{
    double amplitude = ui->dSpinAmp->value();
    double periodo = ui->dSpinPeriodo->value();
    double offSet = ui->dSpinOffSet->value();
    int canalEscrita = config->getCanalEscrita();
    int tipoSinal = ui->comboTipoSinal->currentIndex();

    if(ui->radioAberta->isChecked())
    {
        control->setTensao(amplitude);
        control->setTipoMalha(M_ABERTA);
    }
    else if(ui->radioFechada->isChecked())
    {
        control->setTensao(2);
        control->setAmplitude(amplitude);
        control->setTipoMalha(M_FECHADA);
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

    double sinalEscrita = control->getSinalEnviado();
    double sinalCalculado = control->getSinalCalculado();

    QString str_sinalEnviado = "Sinal enviado = " + QString::number(sinalEscrita) + " V";
    QString str_sinalCalculado = "Sinal calculado = " + QString::number(sinalCalculado) + " V";

    //ui->canal_0->isChecked(); ????????????

    ui->lb_tensaoEscrita->setText(str_sinalEnviado);
    ui->lb_tensaoCalculada->setText(str_sinalCalculado);

    ui->graficoEscrita->graph(0)->addData(key/5, sinalEscrita);
    ui->graficoEscrita->graph(1)->addData(key/5, sinalCalculado);
    ui->graficoEscrita->xAxis->setRange((key + 0.25)/5, 10, Qt::AlignRight);
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
}
void MainWindow::JB_dataConfig()
{
    for(int i=0; i<8; i++)
    {
        if(config->getCanalLeituraVec(i))
        {
            ui->graficoLeitura->graph(i+2)->addToLegend();
            ui->graficoLeitura->graph(i+2)->setVisible(true);
        }
        else
        {
            ui->graficoLeitura->graph(i+2)->removeFromLegend();
            ui->graficoLeitura->graph(i+2)->setVisible(false);
        }
        this->canalLeituraVec[i] = config->getCanalLeituraVec(i);
    }
    //this->canalEscrita = config->getCanalEscrita();
}
