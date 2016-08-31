#include "configuracao.h"
#include "ui_configuracao.h"

Configuracao::Configuracao(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuracao)
{
    ui->setupUi(this);

    connect(ui->canal_0, SIGNAL(clicked(bool)), this, SLOT(dataSet()));
    connect(ui->canal_1, SIGNAL(clicked(bool)), this, SLOT(dataSet()));
    connect(ui->canal_2, SIGNAL(clicked(bool)), this, SLOT(dataSet()));
    connect(ui->canal_3, SIGNAL(clicked(bool)), this, SLOT(dataSet()));
    connect(ui->canal_4, SIGNAL(clicked(bool)), this, SLOT(dataSet()));
    connect(ui->canal_5, SIGNAL(clicked(bool)), this, SLOT(dataSet()));
    connect(ui->canal_6, SIGNAL(clicked(bool)), this, SLOT(dataSet()));
    connect(ui->canal_7, SIGNAL(clicked(bool)), this, SLOT(dataSet()));
    connect(ui->comboCanalEscrita, SIGNAL(activated(int)), this, SLOT(dataSet()));

    // Criando canais de escrita
    for(int i=0; i<4; i++) ui->comboCanalEscrita->addItem("Canal " + QString::number(i),QVariant(i));
}

Configuracao::~Configuracao()
{
    delete ui;
}

bool Configuracao::getCanalLeituraVec(int i) const
{
    return canalLeituraVec[i];
}

int Configuracao::getCanalEscrita() const
{
    return canalEscrita;
}

void Configuracao::dataSet()
{
    canalLeituraVec[0] = ui->canal_0->isChecked();
    canalLeituraVec[1] = ui->canal_1->isChecked();
    canalLeituraVec[2] = ui->canal_2->isChecked();
    canalLeituraVec[3] = ui->canal_3->isChecked();
    canalLeituraVec[4] = ui->canal_4->isChecked();
    canalLeituraVec[5] = ui->canal_5->isChecked();
    canalLeituraVec[6] = ui->canal_6->isChecked();
    canalLeituraVec[7] = ui->canal_7->isChecked();

    canalEscrita = ui->comboCanalEscrita->currentIndex();
}
