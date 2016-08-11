#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timerLeitura = new QTimer(0);
    timerEscrita= new QTimer(0);

    threadLeitura = new QThread(this);
    threadEscrita = new QThread(this);

    timerLeitura->start(100);


    timerEscrita->start(100);


    timerLeitura->moveToThread(threadEscrita);
    connect(timerLeitura, SIGNAL(timeout()), this, SLOT(receiveData()));

    timerEscrita->moveToThread(threadEscrita);
    connect(timerEscrita, SIGNAL(timeout()), this, SLOT(sendData()));

    connect(ui->buttonConectar, SIGNAL(clicked(bool)),this,SLOT(connectServer()));
    connect(ui->buttonAtualizar, SIGNAL(clicked(bool)),this,SLOT(data()));
    connect(ui->comboTipoSinal, SIGNAL(currentIndexChanged(int)),this,SLOT(configSignal()));
    connect(ui->radioAberta, SIGNAL(clicked(bool)),this,SLOT(openLoop()));
    connect(ui->radioFechada, SIGNAL(clicked(bool)),this,SLOT(closedLoop()));

    configPanel();
    configGraphWrite();
    configGraphRead();

    signal = new Signal();

    time = 0;
    timeAux = 0;
    amplitude = 0;
    offSet = 0.0;
    tensao = 0;
    sinalEscrita = 0;
}

MainWindow::~MainWindow()
{
    threadEscrita->terminate();
    threadLeitura->terminate();
    delete ui;
}

void MainWindow::configPanel()
{
    // Criando canais de escrita
    for(int i=0; i<4; i++)
    {
        ui->comboCanalEscrita->addItem("Canal " + QString::number(i),QVariant(i));
    }

    // Sinais gerados
    ui->comboTipoSinal->addItem("Degrau",QVariant(0));
    ui->comboTipoSinal->addItem("Quadrada",QVariant(1));
    ui->comboTipoSinal->addItem("Senoidal",QVariant(2));
    ui->comboTipoSinal->addItem("Dente de Serra",QVariant(3));
    ui->comboTipoSinal->addItem("Aleatório",QVariant(4));


    ui->radioAberta->setChecked(true); // Seta Malha aberta no início
    openLoop();
}

void MainWindow::configSignal()
{

    int sinalSelecionado = ui->comboTipoSinal->currentIndex();

    if(ui->radioAberta->isChecked())
    {
        openLoop();
    }
    else if(ui->radioFechada->isChecked())
    {
        closedLoop();
    }

    ui->labelOffSet->setEnabled(true);
    ui->labelPeriodo->setEnabled(true);
    ui->dSpinOffSet->setEnabled(true);
    ui->dSpinPeriodo->setEnabled(true);

    if(sinalSelecionado == 0)
    {
        ui->labelOffSet->setEnabled(false);
        ui->labelPeriodo->setEnabled(false);
        ui->dSpinOffSet->setEnabled(false);
        ui->dSpinPeriodo->setEnabled(false);
    }
}

void MainWindow::closedLoop()
{
    tipo_sinal = ui->comboTipoSinal->currentIndex();
    if(tipo_sinal == 4)
    {
        ui->labelOffSet->setText("Duração MIN:");
        ui->labelPeriodo->setText("Duração MAX:");
    }
    else
    {
        ui->labelAmp->setText("Amplitude (cm) ");
        ui->labelOffSet->setText("Off-set (cm) ");
        ui->labelPeriodo->setText("Período");

        ui->dSpinAmp->setRange(MIN_LEVEL,MAX_LEVEL); // conferir o tamanho em cm
        ui->dSpinOffSet->setRange(MIN_LEVEL,MAX_LEVEL); // conferir o tamanho em cm
    }
}

void MainWindow::openLoop()
{
    int sinalSelecionado = ui->comboTipoSinal->currentIndex();
    if(sinalSelecionado == 4)
    {
        ui->labelOffSet->setText("Duração MIN:");
        ui->labelPeriodo->setText("Duração MAX:");
    }
    else
    {
        ui->labelAmp->setText("Tensão (V) ");
        ui->labelOffSet->setText("Off-set (V) ");
        ui->labelPeriodo->setText("Período");

        ui->dSpinAmp->setRange(MIN_VOLTAGE,MAX_VOLTAGE); // Limita a tensão entre -4V e 4V
        ui->dSpinOffSet->setRange(MIN_VOLTAGE,MAX_VOLTAGE); // Limita a tensão entre -4V e 4V
    }
}

void MainWindow::configGraphWrite()
{
    // sinal enviado
    ui->graficoEscrita->addGraph(); // blue line
    ui->graficoEscrita->graph(0)->setPen(QPen(Qt::red));
    ui->graficoEscrita->graph(0)->setAntialiasedFill(false);
    ui->graficoEscrita->legend->setVisible(true);
    ui->graficoEscrita->graph(0)->setName("Sinal Enviado");

    // sinal calculado
    ui->graficoEscrita->addGraph(); // red line
    ui->graficoEscrita->graph(1)->setPen(QPen(Qt::blue));
    ui->graficoEscrita->graph(1)->setAntialiasedFill(false);
    ui->graficoEscrita->graph(1)->setName("Sinal Calculado");

    ui->graficoEscrita->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->graficoEscrita->xAxis->setDateTimeFormat("hh:mm:ss");

    ui->graficoEscrita->xAxis->setAutoTickStep(false);
    ui->graficoEscrita->xAxis->setTickStep(2);
    ui->graficoEscrita->axisRect()->setupFullAxesBox();

    ui->graficoEscrita->yAxis->setRange(-7,7);
    ui->graficoEscrita->yAxis->setNumberPrecision(1);
    ui->graficoEscrita->yAxis->setLabel("Tensao (v) ");
    ui->graficoEscrita->xAxis->setLabel("Tempo (s)");

    connect(ui->graficoEscrita->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->graficoEscrita->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->graficoEscrita->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graficoEscrita->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::configGraphRead()
{
    // Nivel do tanque1
    ui->graficoLeitura->addGraph();
    ui->graficoLeitura->graph(0)->setPen(QPen(Qt::black));
    ui->graficoLeitura->graph(0)->setAntialiasedFill(false);
    ui->graficoLeitura->graph(0)->setName("Nivel do tanque");

    // Set Point
    ui->graficoLeitura->addGraph();
    ui->graficoLeitura->graph(1)->setPen(QPen(Qt::blue));
    ui->graficoLeitura->graph(1)->setVisible(true);
    ui->graficoLeitura->graph(1)->setName("Set Point");

    // Erro
    ui->graficoLeitura->addGraph(); // red line
    ui->graficoLeitura->graph(2)->setPen(QPen(Qt::red));
    ui->graficoLeitura->graph(2)->setAntialiasedFill(false);
    ui->graficoLeitura->graph(2)->setName("Erro");

    ui->graficoLeitura->legend->setVisible(true);

    ui->graficoLeitura->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->graficoLeitura->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->graficoLeitura->xAxis->setAutoTickStep(false);
    ui->graficoLeitura->xAxis->setTickStep(2);


    ui->graficoLeitura->yAxis->setRange(0,30);
    ui->graficoLeitura->yAxis->setNumberPrecision(2);
    ui->graficoLeitura->yAxis->setLabel("Nivel do tanque (Cm) ");
    ui->graficoLeitura->xAxis->setLabel("Tempo");

}

void MainWindow::connectServer()
{
    quanser = new Quanser("10.13.99.69",this->PORT);

    qDebug() << "Status da conexão: " << quanser->getStatus();

    if(quanser->getStatus())
    {
        ui->labelStatus->setText("Conectado!");
        ui->labelStatus->setStyleSheet("QLabel { color : green; }");
        ui->buttonConectar->setDisabled(true);

        threadEscrita->start();
        threadLeitura->start();
    }
    else
    {
        ui->labelStatus->setText("Conexão Falhou!");
        ui->labelStatus->setStyleSheet("QLabel { color : red; }");
    }
}

void MainWindow::data()
{
    double _amplitude = ui->dSpinAmp->value();
    this->periodo = ui->dSpinPeriodo->value();

    this->offSet = ui->dSpinOffSet->value();

    this->canalEscrita = ui->comboCanalEscrita->currentIndex();
    this->tipo_sinal = ui->comboTipoSinal->currentIndex();

    if(ui->radioAberta->isChecked())
    {
        this->tensao = voltageControl(_amplitude);
    }
    else if(ui->radioFechada->isChecked())
    {
        this->amplitude = levelControl(_amplitude);
        double _tensao = sqrt(2*GRAVITY*this->amplitude);
        this->tensao = voltageControl(_tensao);
    }
}

int MainWindow::levelControl(int level)
{
    if(level>MAX_LEVEL) level = MAX_LEVEL;
    else if (level<MIN_LEVEL) level = MIN_LEVEL;

    return level;
}

double MainWindow::voltageControl(double _volts)
{
    if(_volts>MAX_VOLTAGE) _volts = MAX_VOLTAGE;
    else if (_volts<MIN_VOLTAGE) _volts = MIN_VOLTAGE;

    return _volts;
}

void MainWindow::stop() {
    quanser->writeDA(canalEscrita, 0);
}

void MainWindow::sendData()
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    //static double lastPointKey = 0;

    switch (tipo_sinal) {
    case DEGRAU:
        sinalEscrita = signal->degrau(tensao, offSet);
        break;
    case SENOIDAL:
        sinalEscrita = signal->seno(tensao,timeAux,periodo,offSet);
        break;
    case QUADRADA:
        sinalEscrita = signal->quadrada(tensao, timeAux, periodo, offSet);
        break;
    case DENTE_DE_SERRA:
        sinalEscrita = signal->serra(tensao,timeAux, periodo, offSet);
        break;
    case ALEATORIO:
        if(timeAux==0)
        {
            sinalEscrita = signal->aleatorio();
            periodo = signal->periodoAleatorio();
        }
        break;
    }

    double gerado = sinalEscrita;

    if(sinalEscrita>4) sinalEscrita = 4;
    if(sinalEscrita<-4) sinalEscrita = -4;

    quanser->writeDA(0,sinalEscrita);

    // add data to lines:
    ui->graficoEscrita->graph(0)->addData(key/5, this->sinalEscrita);
    ui->graficoEscrita->graph(1)->addData(key/5, gerado);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graficoEscrita->xAxis->setRange((key + 0.25)/5, 10, Qt::AlignRight);

    ui->graficoEscrita->replot();

   /* qDebug() << "tensao = " << tensao;
    qDebug() << "periodo = " << periodo;
    qDebug() << "offset" << offSet;

    qDebug() << "SINAL DE SAIDA = " << sinalEscrita;
    qDebug() << "tipo sinal = " << tipo_sinal;
    qDebug() << "time" << time;
    qDebug() << "periodo" << this->periodo;
    qDebug() << "timeAUx " << timeAux;
*/
    if(timeAux >= periodo) timeAux = 0;

    timeAux += 0.1;
    //time += 0.1;
}

void MainWindow::receiveData()
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    int canal0 = 0;
    double sinal = 0;
    qDebug() << "canais";
    sinal =  quanser->readAD(0);
    sinal *= 6.25;


    if(sinal<=0) sinal = 0;
    qDebug() << sinal;

    double erro = amplitude - sinal;

    ui->graficoLeitura->graph(0)->addData(key/5,sinal); // nivel tanque 1
    ui->graficoLeitura->graph(2)->addData(key/5,erro); // erro abs
    if(ui->radioFechada->isChecked())
    {
        ui->graficoLeitura->graph(1)->addData(key/5,amplitude); // set point
    }


    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graficoLeitura->xAxis->setRange((key+0.25)/5, 8, Qt::AlignRight);
    ui->graficoLeitura->replot();
}


