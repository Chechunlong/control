#include "control.h"

Control::Control()
{
    try {
        quanser = new Quanser("10.13.99.69",20081);

        signal = new Signal();
    } catch(int e) {
        qDebug() << "ops " << e;
    }

    timeAux = 0;
    tipoSinal = 0;

    tensao = 0;
    offSet = 0;
    periodo = 0;
    amplitude = 0;
    auxForRand = 0;

    sinalEscrita = 0;
    sinalLeitura = 0;
    sinalCalculado = 0;

    erro = 0;

    canalEscrita = 0;
    canalLeitura = 0;
}

double Control::getAmplitude()
{
    return amplitude;
}

int Control::getCanalEscrita()
{
    return canalEscrita;
}

int Control::getCanalLeitura()
{
    return canalEscrita;
}

bool Control::getCanalValue(int value)
{
    if(value >= NUMB_CAN_READ) return -1;

    return canaisLeitura_value[value];
}

double Control::getErro()
{
    return erro;
}

int Control::getTipoMalha()
{
    return tipoMalha;
}

double Control::getSinalCalculado()
{
    return sinalCalculado;
}

double Control::getSinalEnviado()
{
    return sinalEscrita;
}

double Control::getSinalLeitura()
{
    return sinalLeitura;
}

void Control::setAmplitude(double value)
{
    amplitude = value;
}

void Control::setAuxForRand(double value)
{
    auxForRand = value;
}

void Control::setCanalEscrita(int value)
{
    canalEscrita = canalLeitura = value;
}

void Control::setOffSet(double value)
{
    offSet = value;
}

void Control::setPeriodo(double value)
{
    periodo = value;
}

void Control::setTensao(double value)
{
    tensao = value;
}

void Control::setTipoMalha(int value)
{
    tipoMalha = value;
}

void Control::setTipoSinal(int value)
{
    tipoSinal = value;

}

bool Control::connectionStatus()
{
    return quanser->getStatus();
}

int Control::levelControl(int value)
{
    if(value>MAX_LEVEL) value = MAX_LEVEL;
    else if (value<MIN_LEVEL) value = MIN_LEVEL;

    return value;
}

double Control::voltageControl(double value)
{
    if(value>=MAX_VOLTAGE) value = MAX_VOLTAGE;
    else if (value<=MIN_VOLTAGE) value = MIN_VOLTAGE;

    return value;
}

void Control::travel()
{
    sinalEscrita = voltageControl(sinalEscrita);

    if(sinalLeitura<=3 && sinalEscrita<0) sinalEscrita = 0;

    if(sinalLeitura>=28 && sinalEscrita>0) sinalEscrita = 0;

}

void Control::sendSignal()
{
    if(timeAux > periodo) timeAux = 0;

    switch (tipoSinal) {
    case DEGRAU:
        sinalCalculado = signal->degrau(tensao, offSet);
        break;
    case SENOIDAL:
        sinalCalculado = signal->seno(tensao,timeAux,periodo,offSet);
        break;
    case QUADRADA:
        sinalCalculado = signal->quadrada(tensao, timeAux, periodo, offSet);
        break;
    case DENTE_DE_SERRA:
        sinalCalculado = signal->serra(tensao,timeAux, periodo, offSet);
        break;
    case ALEATORIO:
        double ampMax = amplitude;
        double ampMin = offSet;
        double durMax = periodo;
        double durMin = auxForRand;
        if(timeAux==0)
        {
            sinalCalculado = signal->aleatorio(ampMax, ampMin);
            periodo = signal->periodoAleatorio(durMax, durMin);
        }
        break;
    }

    sinalEscrita = sinalCalculado;

    if(tipoMalha == M_FECHADA)
    {
        tensao = erro;
    }

    travel();

    qDebug() << "sinalEscrita " << sinalEscrita;
    qDebug() << "sinalCalculado " << sinalCalculado;
    qDebug() << "tensao " << tensao;

    quanser->writeDA(canalEscrita,sinalEscrita);

    timeAux += 0.1;
}

void Control::receiveSigal()
{
    // read all channels (NUMB_CAN_READ)

    double readVoltage;

    //for(int i=0; i<NUMB_CAN_READ; i++)
    //{
    readVoltage = quanser->readAD(canalLeitura);

    qDebug() << "canal de leitura" << canalLeitura;

    sinalLeitura = readVoltage * FATOR_CONVERSAO; // cm
    if(tipoMalha == M_FECHADA)
    {
        erro = amplitude - sinalLeitura; // cm
    }

    qDebug() << "erro " << erro;
    qDebug() << "sinalLeitura " << sinalLeitura;

        //canaisLeitura_value[i] = readVoltage * FATOR_CONVERSAO;
    //}
}
