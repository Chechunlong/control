#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->buttonConectar, SIGNAL(clicked(bool)),this,SLOT(connectServer()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectServer() {
    quanser = new Quanser("10.13.99.69",20081);

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

void MainWindow::on_radioAberta_clicked()
{
    ui->labelAmp->setText("Tensão (V) ");
    ui->labelOffSet->setText("Off-set (V) ");
}

void MainWindow::on_radioFechada_clicked()
{
    ui->labelAmp->setText("Amplitude (cm) ");
    ui->labelOffSet->setText("Off-set (cm) ");
}


