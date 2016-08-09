#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timerLeitura = new QTimer(this);
    threadLeitura = new QThread(0);

    timerLeitura->moveToThread(threadEscrita);
    connect(timerLeitura, SIGNAL(timeout()), this, SLOT(receiveData()));

    this->timerLeitura->start(100);
    this->threadLeitura->start();

    timerEscrita= new QTimer(this);
    threadEscrita = new QThread(0);

    timerEscrita->moveToThread(threadEscrita);
    connect(timerEscrita, SIGNAL(timeout()), this, SLOT(sendData()));

    this->timerEscrita->start(100);
    this->threadEscrita->start();

    connect(ui->buttonConectar, SIGNAL(clicked(bool)),this,SLOT(connectServer()));
    connect(ui->buttonAtualizar, SIGNAL(clicked(bool)),this,SLOT(updateData()));

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

    ui->graficoEscrita->addGraph();
    ui->graficoEscrita->graph(0)->setPen(QPen(Qt::blue));
    ui->graficoEscrita->graph(0)->setAntialiasedFill(false);
    ui->graficoEscrita->legend->setVisible(true);
    ui->graficoEscrita->graph(0)->setName("Sinal saturado");
}

MainWindow::~MainWindow()
{
    delete ui;
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
    this->amplitude = ui->dSpinAmp->value();
    this->periodo = ui->dSpinPeriodo->value();
    this->offSet = ui->dSpinOffSet->value();

    int canalEscrita = ui->comboCanalEscrita->currentIndex();
    int sinal = ui->comboTipoSinal->currentIndex();

    {
        qDebug() << "_amplitude: " << this->amplitude;
        qDebug() << "_periodo: " << this->periodo;
        qDebug() << "_offset: " << this->offSet;
        qDebug() << "Canal de escrita: " << canalEscrita;
        qDebug() << "Sinal escolhido: " << sinal << " " << ui->comboTipoSinal->currentText();
    }

    if(ui->radioAberta->isChecked())
    {

        float tensao = this->amplitude;
        tensao = voltageControl(tensao);

        qDebug() << "Tensão escolhida: " << tensao;

    }
    else if(ui->radioFechada->isChecked())
    {
        this->amplitude = levelControl(this->amplitude);
        float tensao = sqrt(2*GRAVITY*this->amplitude);
        tensao = voltageControl(tensao);

    }
}

float MainWindow::voltageControl(float voltage)
{
    if(voltage>MAX_VOLTAGE) voltage = MAX_VOLTAGE;
    else if (voltage<MIN_VOLTAGE) voltage = MIN_VOLTAGE;

    return voltage;
}

int MainWindow::levelControl(int level)
{
    if(level>MAX_LEVEL) level = MAX_LEVEL;
    else if (level<MIN_LEVEL) level = MIN_LEVEL;

    return level;
}

void MainWindow::sendData()
{

   qDebug() << this->amplitude;


}

void MainWindow::receiveData()
{

    float saida = 0;
    if(ui->canal_0->isChecked()) saida = quanser->readAD(0);
    qDebug() << "0 " << saida;
    if(ui->canal_1->isChecked()) saida = quanser->readAD(1);
    qDebug() << "1 " << saida;
    if(ui->canal_2->isChecked()) saida = quanser->readAD(2);
    qDebug() << "2 " << saida;
    if(ui->canal_3->isChecked()) saida = quanser->readAD(3);
    qDebug() << "3 " << saida;
    if(ui->canal_4->isChecked()) saida = quanser->readAD(4);
    qDebug() << "4 " << saida;

}
