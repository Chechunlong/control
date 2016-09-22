#include "control.h"

Control::Control(int port, QString ip)
{
    quanser = new Quanser(ip.toLatin1().data(),port);

    signal = new Signal();
    controller = new Controller();

    sistemaO2 = new SistemaO2();


    timeAux     = 0;
    tipoSinal   = 0;

    tensao      = 0;
    offSet      = 0;
    periodo     = 0;
    amplitude   = 0;
    auxForRand  = 0;

    sinalEscrita    = 0;
    sinalLeitura    = 0;
    sinalCalculado  = 0;

    erro    = 0;
    erroAnt = 0;
    arrayErro[M] = {0};

    //contador auxiliar utilizado para o filtro mm.
    auxContErro = 0;

    canalEscrita = 0;
    canalLeitura = 0;

    // Controlador
    Kp = 0;
    Ki = 0;
    Kd = 0;
    tempoDerivativo = 0;
    tempoIntegrativo = 0;
    tipoControler = 0;
    modeControle = 0;
}
/* Recebe um array de tamanho M,
   a saida é a média do ponto[0].*/
double Control::filtroMM(double erro[]){
    double tmp = 0;
    for(int i=0;i < M;i++){
        tmp += erro[i];
    }
    return tmp/M;
}

double Control::getAmplitude()
{ return amplitude; }

int Control::getCanalEscrita()
{ return canalEscrita; }

int Control::getCanalLeitura()
{ return canalLeitura; }

double Control::getCanalValue(int value)
{
    if(value >= NUMB_CAN_READ) return -1;

    return canaisLeitura_value[value];
}

double Control::getErro()
{ return erro; }

int Control::getTipoMalha()
{ return tipoMalha; }

double Control::getSinalCalculado()
{ return sinalCalculado; }

double Control::getSinalEnviado()
{ return sinalEscrita; }

double Control::getSinalLeitura()
{ return sinalLeitura; }

void Control::setAmplitude(double amplitude)
{
    this->amplitude <= amplitude ? setPointUP = true : setPointUP = false;

    this->amplitude = amplitude;
}

void Control::setAuxForRand(double auxForRand)
{ this->auxForRand = auxForRand; }

void Control::setCanalEscrita(int canalEscrita)
{ this->canalEscrita = canalEscrita; }

void Control::setOffSet(double value)
{ offSet = value; }

void Control::setPeriodo(double value)
{ periodo = value; }

void Control::setTensao(double value)
{ tensao = value; }

void Control::setTipoMalha(int value)
{ tipoMalha = value; }

void Control::setTipoSinal(int value)
{ tipoSinal = value; }

double Control::getKp() const
{ return Kp; }

void Control::setKp(double value)
{ Kp = value; }

double Control::getKi() const
{ return Ki; }

void Control::setKi(double value)
{ Ki = value; }

double Control::getKd() const
{ return Kd; }

void Control::setKd(double value)
{ Kd = value; }

double Control::getTi() const
{ return this->tempoIntegrativo; }
double Control::getTd() const
{ return this->tempoDerivativo; }

double Control::getTempoIntegrativo() const
{ return tempoIntegrativo; }

void Control::setTempoIntegrativo(double value)
{ tempoIntegrativo = value; }

double Control::getTempoDerivativo() const
{ return tempoDerivativo; }

void Control::setTempoDerivativo(double value)
{ tempoDerivativo = value; }

int Control::getModeControle() const
{ return modeControle; }

void Control::setModeControle(int modeControle)
{ this->modeControle = modeControle; }

int Control::getTipoControler()
{ return tipoControler; }

void Control::setTipoControler(int tipoControler)
{ this->tipoControler = tipoControler; }

void Control::setTipoOrdemSistema(int ordemSistema)
{
    this->ordemSistema = ordemSistema;

    if(SISTEMA_ORDEM_1 == ordemSistema) {
        canalLeitura = 0;
    } else if(SISTEMA_ORDEM_2 == ordemSistema) {
        canalLeitura = 1;

        sistemaO2->configTr(tipoTr, sinalLeitura, amplitude);
    }
}

void Control::calculaSinal()
{
    if(timeAux > periodo) timeAux = 0;

    if(tipoMalha == M_FECHADA)
    {
        if(modeControle == CONTROLE_CONST_TEMP)
        {
           Ki = Kp / tempoIntegrativo;
           Kd = Kp * tempoDerivativo;
        }
        else
        {
            tempoDerivativo = Kd/ Kp;
            tempoIntegrativo = Kp / Ki;
        }


        switch (tipoControler) {
        case CONTROLER_P:
            tensao = controller->controlerP(Kp, erro);
            break;
        case CONTROLER_PI:
            tensao = controller->controlerPI(Kp, Ki, erro);
            break;
        case CONTROLER_PD:
            tensao = controller->controlerPD(Kp,Kd, erro);
            break;
        case CONTROLER_PID:
            tensao = controller->controlerPID(Kp,Ki,Kd,erro);
            break;
        case CONTROLER_PI_D:
            tensao = controller->controlerPI_D(Kp, Ki, Kd, erro, sinalLeitura);
            break;
        }
    }

    switch (tipoSinal)
    {
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

    timeAux += 0.1;
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

void Control::zerarSinal()
{
    delete controller;
    delete signal;

    controller = new Controller();
    signal = new Signal();

    tipoMalha = M_ABERTA;
    tensao = 0;
    sinalCalculado = 0;
    sinalEscrita = 0;
    timeAux = 0;
}

void Control::travel()
{
    sinalEscrita = voltageControl(sinalEscrita);

    if(sinalLeitura<=3 && sinalEscrita<0) sinalEscrita = 0;

    if(sinalLeitura>=28 && sinalEscrita>0) sinalEscrita = 0;
}

void Control::sendSignal()
{
    calculaSinal();

    sinalEscrita = sinalCalculado;

    travel();

    quanser->writeDA(canalEscrita,sinalEscrita);
}

void Control::receiveSigal()
{
    double readVoltage;


    for(int canal=0; canal<2; canal++)
    {
        readVoltage = quanser->readAD(canal);
        canaisLeitura_value[canal] = readVoltage * FATOR_CONVERSAO;


        double temp = canaisLeitura_value[0];
        if(temp<=3 && sinalEscrita<0 || temp>=28 && sinalEscrita>0) {
            sinalEscrita = 0;
            quanser->writeDA(0,0);
        }

        if(canal==canalLeitura)
        {
            sinalLeitura = canaisLeitura_value[canal]; // cm
            if(tipoMalha == M_FECHADA)
            {
                erro = amplitude - sinalLeitura; // cm

                if(auxContErro >= M){
                    double saidaErro = filtroMM(arrayErro);
                    auxContErro = 0;
                }
                arrayErro[auxContErro] = erro;
                auxContErro++;
                /*

                    Implementação do filtro

                */

                if(ordemSistema ==  SISTEMA_ORDEM_2)
                {
                    statusTr = sistemaO2->getStatusTr();
                    statusTp = sistemaO2->getStatusTp();
                    statusMp = sistemaO2->getStatusMP();

                    if(!statusTp) {
                        sistemaO2->calculaTp(sinalLeitura, sinalLeitura_old);
                    } else {
                        tp = sistemaO2->getTp();
                    }

                    if(!statusMp) {
                        sistemaO2->calculaMp(sinalLeitura,amplitude);
                    } else {
                        mp = sistemaO2->getMp();
                    }

                    if(!statusTr) {
                        sistemaO2->calculaTr(sinalLeitura, amplitude);
                    } else {
                        tr = sistemaO2->getTr();
                    }
                }

            }

        }
    }
}

double Control::getTr() const
{
    return tr;
}

double Control::getMp() const
{
    return mp;
}

double Control::getTp() const
{
    return tp;
}

double Control::getTs() const
{
    return ts;
}

bool Control::getStatusTr() const
{
    return statusTr;
}

bool Control::getStatusMp() const
{
    return statusMp;
}

bool Control::getStatusTp() const
{
    return statusTp;
}

bool Control::getStatusTs() const
{
    return statusTs;
}

int Control::getOrdemSistema() const
{
    return ordemSistema;
}

void Control::setTipoTr(int value)
{
    tipoTr = value;
}

void Control::setTipoTs(int value)
{
    tipoTs = value;
}

void Control::setCanalLeitura(int value)
{
    canalLeitura = value;
}


