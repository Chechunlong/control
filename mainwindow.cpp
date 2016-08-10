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
    threadLeitura->start();

    timerEscrita->start(100);
    threadEscrita->start();

    timerLeitura->moveToThread(threadEscrita);
    connect(timerLeitura, SIGNAL(timeout()), this, SLOT(receiveData()));

    timerEscrita->moveToThread(threadEscrita);
    connect(timerEscrita, SIGNAL(timeout()), this, SLOT(sendData()));

    connect(ui->buttonConectar, SIGNAL(clicked(bool)),this,SLOT(connectServer()));
    connect(ui->buttonAtualizar, SIGNAL(clicked(bool)),this,SLOT(data()));

    configPanel();
    configGraphWrite();
    configGraphRead();

    time = 0;
    timeAux = 0;
    signal = new Signal();

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
    on_radioAberta_clicked();
}

void MainWindow::configGraphWrite()
{
    // sinal enviado
    ui->graficoEscrita->addGraph(); // blue line
    ui->graficoEscrita->graph(0)->setPen(QPen(Qt::blue));
    ui->graficoEscrita->graph(0)->setAntialiasedFill(false);
    ui->graficoEscrita->legend->setVisible(true);
    ui->graficoEscrita->graph(0)->setName("Sinal saturado");

    // sinal calculado
    ui->graficoEscrita->addGraph(); // red line
    ui->graficoEscrita->graph(1)->setPen(QPen(Qt::red));
    ui->graficoEscrita->graph(1)->setAntialiasedFill(false);
    ui->graficoEscrita->graph(1)->setName("Sinal calculado");

    ui->graficoEscrita->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->graficoEscrita->xAxis->setDateTimeFormat("hh:mm:ss");

    ui->graficoEscrita->xAxis->setAutoTickStep(false);
    ui->graficoEscrita->xAxis->setTickStep(2);
    ui->graficoEscrita->axisRect()->setupFullAxesBox();

    ui->graficoEscrita->yAxis->setRange(-12,12);
    ui->graficoEscrita->yAxis->setNumberPrecision(1);
    ui->graficoEscrita->yAxis->setLabel("Tensao (v) ");
    ui->graficoEscrita->xAxis->setLabel("Tempo");

    connect(ui->graficoEscrita->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->graficoEscrita->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->graficoEscrita->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graficoEscrita->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::configGraphRead()
{
    // Nivel do tanque1
    ui->graficoLeitura->addGraph();
    ui->graficoLeitura->graph(0)->setPen(QPen(Qt::black));
    ui->graficoLeitura->graph(0)->setAntialiasedFill(false);
    ui->graficoLeitura->graph(0)->setName("Nivel do tanque1");

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

void MainWindow::on_radioAberta_clicked()
{
    ui->labelAmp->setText("Tensão (V) ");
    ui->labelOffSet->setText("Off-set (V) ");

    ui->dSpinAmp->setRange(MIN_VOLTAGE,MAX_VOLTAGE); // Limita a tensão entre -4V e 4V
    ui->dSpinOffSet->setRange(MIN_VOLTAGE,MAX_VOLTAGE); // Limita a tensão entre -4V e 4V
}

void MainWindow::on_radioFechada_clicked()
{
    ui->labelAmp->setText("Amplitude (cm) ");
    ui->labelOffSet->setText("Off-set (cm) ");

    ui->dSpinAmp->setRange(MIN_LEVEL,MAX_LEVEL); // conferir o tamanho em cm
    ui->dSpinOffSet->setRange(MIN_LEVEL,MAX_LEVEL); // conferir o tamanho em cm
}

void MainWindow::connectServer()
{
    quanser = new Quanser(this->SERVER,this->PORT);

    qDebug() << "Status da conexão: " << quanser->getStatus();

    if(quanser->getStatus())
    {
        ui->labelStatus->setText("Conectado!");
        ui->labelStatus->setStyleSheet("QLabel { color : green; }");
        ui->buttonConectar->setDisabled(true);
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

void MainWindow::sendData()
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    //static double lastPointKey = 0;

    switch (tipo_sinal) {
    case DEGRAU:
        sinalSaida = signal->degrau(tensao, offSet);
        break;
    case SENOIDAL:
        sinalSaida = signal->seno(tensao,timeAux,periodo,offSet);
        break;
    case QUADRADA:
        sinalSaida = signal->quadrada(tensao, timeAux, periodo, offSet);
        break;
    case DENTE_DE_SERRA:
        sinalSaida = signal->serra(tensao,timeAux, periodo, offSet);
        break;
    case ALEATORIO:
        if(timeAux==0)
        {
            sinalSaida = signal->aleatorio();
            periodo = signal->periodoAleatorio();
        }
        break;
    }

    // add data to lines:
    ui->graficoEscrita->graph(0)->addData(key/5, this->sinalSaida);
    //ui->graficoEscrita->graph(1)->addData(key/5, quanser->voltageControl(sinalSaida) );

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graficoEscrita->xAxis->setRange((key + 0.25)/5, 10, Qt::AlignRight);

    ui->graficoEscrita->replot();

    qDebug() << "tensao = " << tensao;
    qDebug() << "periodo = " << periodo;
    qDebug() << "offset" << offSet;

    qDebug() << "SINAL DE SAIDA = " << sinalSaida;
    qDebug() << "tipo sinal = " << tipo_sinal;
    qDebug() << "time" << time;
    qDebug() << "periodo" << this->periodo;
    qDebug() << "timeAUx " << timeAux;

    if(timeAux >= periodo) timeAux = 0;

    timeAux += 0.1;
    time += 0.1;
}

void MainWindow::receiveData()
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    // TODO: ...

    ui->graficoLeitura->graph(0)->addData(key/5,1); // nivel tanque 1
    ui->graficoLeitura->graph(1)->addData(key/5,2); // set point
    ui->graficoLeitura->graph(2)->addData(key/5,3); // erro abs

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graficoLeitura->xAxis->setRange((key+0.25)/5, 8, Qt::AlignRight);
    ui->graficoLeitura->replot();
}
