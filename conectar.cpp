#include "conectar.h"
#include "ui_conectar.h"

Conectar::Conectar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Conectar)
{
    ui->setupUi(this);

    ui->lineIpAdress->setInputMask(QString("000.000.000.000"));
    ui->lineIpAdress->setValidator(new QRegExpValidator(QRegExp("[0-255].[0-255].[0-255].[0-255]", Qt::CaseInsensitive), this));
    ui->lineIpAdress->setValidator(new QIntValidator(1, 65535, this));

    connect(ui->spinBoxPort,SIGNAL(valueChanged(int)), this, SLOT(setData()));
    connect(ui->lineIpAdress, SIGNAL(textChanged(QString)), this, SLOT(setData()));
    connect(ui->buttonConectar, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->buttonCancelar, SIGNAL(clicked(bool)), this, SLOT(reject()));

    ui->buttonCancelar->setStyleSheet("background-color: red");
    ui->buttonConectar->setStyleSheet("background-color: blue");

    ui->spinBoxPort->setValue(PORT_S);
    ui->lineIpAdress->setText(IP_S);

    ipAdress = IP_S;
    port = PORT_S;
}

Conectar::~Conectar()
{
    delete ui;
}

void Conectar::setData()
{
    this->port = ui->spinBoxPort->value();
    this->ipAdress = ui->lineIpAdress->text();
}

int Conectar::getPort() const
{
    return port;
}

QString Conectar::getIpAdress() const
{
    return ipAdress;
}
