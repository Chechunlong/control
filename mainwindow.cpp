#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->buttonConectar, SIGNAL(clicked(bool)),this,SLOT(connectServer()));
    connect(ui->buttonAtualizar, SIGNAL(clicked(bool)),this,SLOT(updateData()));

    // Criando canais de escrita
    for(int i=0; i<4; i++) {
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

    ui->dSpinAmp->setRange(0,35); // conferir o tamanho em cm
    ui->dSpinOffSet->setRange(0,35); // conferir o tamanho em cm
}

void MainWindow::connectServer()
{
    quanser = new Quanser(this->SERVER,this->PORT);

    qDebug() << "Status da conexão: " << quanser->getStatus();

    if(quanser->getStatus()) {
        ui->labelStatus->setText("Conectado!");
        ui->labelStatus->setStyleSheet("QLabel { color : green; }");
        ui->buttonConectar->setDisabled(true);
    }
    else {
        ui->labelStatus->setText("Conexão Falhou!");
        ui->labelStatus->setStyleSheet("QLabel { color : red; }");
    }
}

void MainWindow::updateData()
{
    QString _amplitude = ui->dSpinAmp->text();
    _amplitude.replace(",",".");

    this->amplitude = _amplitude.toDouble();

    QString _periodo = ui->dSpinPeriodo->text();
    QString _offset = ui->dSpinOffSet->text();

    qDebug() << "_amplitude: " << _amplitude;
    qDebug() << "_periodo: " << _periodo;
    qDebug() << "_offset: " << _offset;

    int canal = ui->comboCanalEscrita->currentIndex();
    int sinal = ui->comboTipoSinal->currentIndex();

    qDebug() << "Canal de escrita: " << canal;
    qDebug() << "Sinal escolhido: " << sinal << " " << ui->comboTipoSinal->currentText();

    if(ui->radioAberta->isChecked()) {


        float tensao = _amplitude.toFloat();

        if(tensao > MAX_VOLTAGE) {
            tensao = 4;
        }
        else if(tensao < MIN_VOLTAGE) {
            tensao = -4;
        }

        qDebug() << "Tensão escolhida: " << tensao;

        //verifica se tem um loop em execução
        if(timerId > 0){
            closeLoop();
        }
        openLoop();

    }
    else if(ui->radioFechada->isChecked()) {

    }
}

void MainWindow::openLoop(){
    timerId = startTimer(10);
}

void MainWindow::closeLoop(){
    killTimer(timerId);
    qDebug() << "parou";
}

void MainWindow::timerEvent(QTimerEvent *event){
    //pega o os parametros lidos em updateData() para gerar o sinal

    //ler o nivel do tanque
    double nivel = quanser->readAD(0);
    //calcula o erro (nivel em cm (?))
    double erro = amplitude - nivel*3.2;//não tenho certeza se o fator de conversão é esse.
    //sinal de saida = erro
    quanser ->writeDA(0,erro);
}
