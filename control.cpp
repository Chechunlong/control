#include "control.h"

Control::Control(int port, QString ip)
{
    quanser = new Quanser(ip.toLatin1().data(),port);
    signal = new Signal();
    controller = new Controller();
    contCascata = new Controller();
    sistemaO2 = new SistemaO2();
    tanq = new Tanque();

/*    timeAux     = 0;
    tipoSinal   = 0;

    tensao      = 0;
    offSet      = 0;
    periodo     = 0;
    amplitude   = 0;
    auxForRand  = 0;

    sinalEscrita    = 0;
    sinalLeitura    = 0;
    sinalCalculado  = 0;
    sinalLeitura_old = 0;

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

    tensaoCas = 0;
    tipoControlerCas = 0;
    KpCas = 0;
    KiCas = 0;
    KdCas = 0;
    erroCas = 0;
    sinalLeituraCas = 0;

    P = I = D = 0;

    statusMp = statusTp = statusTr = statusTs = false;
    mp = tr= ts = tp = 0;

    setPointUP = true;

    windUP = false;

    tanque1 = 0;
    tanque2 = 0;

    simulacao = true;

    ordemSistema = SISTEMA_ORDEM_1;
    tipoMalha = M_ABERTA;
    modeControle = CONTROLE_GANHO;
    modeControleCas = CONTROLE_GANHO;
    modeSegOrdem = C_O2_CONVENCIONAL;
*/
}
/* Recebe um array de tamanho M,
   a saida é a média do ponto[0].*/

double Control::filtroMM(double erro[]){
    double tmp = 0;
    for(int i=0;i < P_MM;i++){
        tmp += erro[i];
    }
    return trunca(tmp/P_MM);
}

double Control::trunca(double numero) {

    return (int)numero + ( ( (int)((numero - (int)numero) * 100) ) / 100. );
}

double Control::getAmplitude() { return amplitude; }

int Control::getCanalEscrita() { return canalEscrita; }

int Control::getCanalLeitura() { return canalLeitura; }

double Control::getCanalValue(int value) { return canaisLeitura_value[value]; }

double Control::getErro() { return erro; }

int Control::getTipoMalha() { return tipoMalha; }

double Control::getSinalCalculado() { return sinalCalculado; }

double Control::getSinalEnviado() { return sinalEscrita; }

double Control::getSinalLeitura() { return sinalLeitura; }

void Control::setAmplitude(double amplitude)
{
    this->amplitude <= amplitude ? setPointUP = true : setPointUP = false;

    sistemaO2->setTipoAmplitude(setPointUP);

    this->amplitude = amplitude;
}

void Control::setAuxForRand(double auxForRand)
{ this->auxForRand = auxForRand; }

void Control::setCanalEscrita(int canalEscrita)
{ this->canalEscrita = canalEscrita; }

void Control::setOffSet(double value) { offSet = value; }

void Control::setPeriodo(double value) { periodo = value; }

void Control::setTensao(double value) { tensao = value; }

void Control::setTipoMalha(int value) { tipoMalha = value; }

void Control::setTipoSinal(int value) { tipoSinal = value; }

double Control::getKp() const { return Kp; }

void Control::setKp(double value) { Kp = value; }

double Control::getKi() const { return Ki; }

void Control::setKi(double value) { Ki = value; }

double Control::getKd() const { return Kd; }

void Control::setKd(double value) { Kd = value; }

double Control::getTi() const { return this->tempoIntegrativo; }

double Control::getTd() const { return this->tempoDerivativo; }

double Control::getP() const { return controller->getP(); }
double Control::getI() const { return controller->getI(); }
double Control::getD() const { return controller->getD(); }

double Control::getTempoIntegrativo() const { return tempoIntegrativo; }

void Control::setTempoIntegrativo(double value) { tempoIntegrativo = value; }

double Control::getTempoDerivativo() const { return tempoDerivativo; }

void Control::setTempoDerivativo(double value) { tempoDerivativo = value; }

int Control::getModeControle() const { return modeControle; }

void Control::setModeControle(int modeControle) { this->modeControle = modeControle; }

int Control::getTipoControler() { return tipoControler; }

void Control::setTipoControler(int tipoControler) { this->tipoControler = tipoControler; }

void Control::setTipoOrdemSistema(int ordemSistema)
{
    this->ordemSistema = ordemSistema;

    if(SISTEMA_ORDEM_1 == ordemSistema) {
        canalLeitura = 0;
    } else if(SISTEMA_ORDEM_2 == ordemSistema) {
        canalLeitura = 1;

        sistemaO2->configTr(tipoTr, sinalLeitura, amplitude);
        sistemaO2->configTs(tipoTs,sinalLeitura);
    }
}

double Control::getTr() const { return tr; }

double Control::getMp() const { return mp; }

double Control::getTp() const { return tp; }

double Control::getTs() const { return ts; }

bool Control::getStatusTr() const{ return statusTr; }

bool Control::getStatusMp() const { return statusMp; }

bool Control::getStatusTp() const { return statusTp; }

bool Control::getStatusTs() const { return statusTs; }

int Control::getOrdemSistema() const { return ordemSistema; }

void Control::setTipoTr(int value) { tipoTr = value; }

void Control::setTipoTs(int value) { tipoTs = value; }

void Control::setTipoMp(int value) { tipoMp = value; }

void Control::setCanalLeitura(int value) { canalLeitura = value; }

void Control::setWindUP(bool value) { windUP = value; }

void Control::setModeSegOrdem(int value) { modeSegOrdem = value; }

void Control::setTipoControlerCas(double value) {
    tipoControlerCas = value;
    {
        qDebug() << "tipoControlerCas = " << tipoControlerCas;
    }
}

void Control::setKpCas(double value) { KpCas = value; }

void Control::setKiCas(double value) { KiCas = value; }

void Control::setKdCas(double value) { KdCas = value; }

void Control::setModeControleCas(double value) { modeControleCas = value; }

void Control::setTempoIntegrativoCas(double value) { tempoIntegrativoCas = value; }

void Control::setTempoDerivativoCas(double value)  { tempoDerivativoCas = value; }

bool Control::getConnectionStatus() { return quanser->getStatus(); }


int Control::levelControl(int value) {
    if(value>MAX_LEVEL) value = MAX_LEVEL;
    else if (value<MIN_LEVEL) value = MIN_LEVEL;

    return value;
}

double Control::voltageControl(double value) {
    if(value>=MAX_VOLTAGE) value = MAX_VOLTAGE;
    else if (value<=MIN_VOLTAGE) value = MIN_VOLTAGE;

    return value;
}

void Control::zerarSinal() {
    delete controller;
    delete contCascata;
    delete signal;

    controller = new Controller();
    contCascata = new Controller();
    signal = new Signal();

    tipoMalha = M_ABERTA;
    tensao = 0;
    sinalCalculado = 0;
    sinalEscrita = 0;
    timeAux = 0;
    erro = 0;
}

double Control::readCanal(int canal) { return quanser->readAD(canal)* FATOR_CONVERSAO; }

void Control::travel() {
    sinalEscrita = voltageControl(sinalEscrita);

    if(tanque1<=3 && sinalEscrita<0) sinalEscrita = 0;

    if(tanque1>=28 && sinalEscrita>0) sinalEscrita = 0;
}

void Control::tempoControle() {
    statusTr = sistemaO2->getStatusTr();
    statusTp = sistemaO2->getStatusTp();
    statusMp = sistemaO2->getStatusMP();
    statusTs = sistemaO2->getStatusTs();

    if(!statusTp) sistemaO2->calculaTp(sinalLeitura, sinalLeitura_old, amplitude, tipoMp);
    else tp = sistemaO2->getTp();

    if(statusMp) mp = sistemaO2->getMp();

    if(!statusMp) sistemaO2->calculaMp(sinalLeitura, tipoMp, amplitude);
    else mp = sistemaO2->getMp();

    if(!statusTr) sistemaO2->calculaTr(sinalLeitura, amplitude);
    else tr = sistemaO2->getTr();

    //if(!statusTs) sistemaO2->calculaTs(sinalLeitura,amplitude);
    //else ts = sistemaO2->getTs();

    sistemaO2->calculaTs(sinalLeitura,sinalLeitura_old,amplitude);
     ts = sistemaO2->getTs();
}

double Control::calculaTensao(double tensao) {
    if(tipoSinal == DEGRAU)
        return signal->degrau(tensao, offSet);
    else if(tipoSinal == SENOIDAL)
        return signal->seno(tensao,timeAux,periodo,offSet);
    else if(tipoSinal == QUADRADA)
        return signal->quadrada(tensao, timeAux, periodo, offSet);
    else if(tipoSinal == DENTE_DE_SERRA)
        return  signal->serra(tensao,timeAux, periodo, offSet);
    else if(tipoSinal == ALEATORIO) {
        double ampMax = amplitude;
        double ampMin = offSet;
        double durMax = periodo;
        double durMin = auxForRand;
        if(timeAux==0) {
            periodo = signal->periodoAleatorio(durMax, durMin);
            return signal->aleatorio(ampMax, ampMin);
        }
    }
}

double Control::calculaTensaoPID(Controller *controller, double tipoControler, double Kp, double Ki, double Kd, double erro, double sinalLeitura) {

    if(tipoControler == CONTROLER_P)
        return calculaTensao(controller->controlerP(Kp, erro));
    else if(tipoControler == CONTROLER_PI)
        return calculaTensao(controller->controlerPI(Kp, Ki, erro));
    else if(tipoControler == CONTROLER_PD)
        return calculaTensao(controller->controlerPD(Kp,Kd, erro));
    else if(tipoControler == CONTROLER_PID)
        return calculaTensao(controller->controlerPID(Kp,Ki,Kd,erro));
     else if(tipoControler == CONTROLER_PI_D)
        return calculaTensao(controller->controlerPI_D(Kp, Ki, Kd, erro, sinalLeitura));
}

void Control::calculaSinal() {
    if(timeAux > periodo) timeAux = 0;

    if(tipoMalha == M_FECHADA) {
        if(modeControle == CONTROLE_CONST_TEMP) {
           Ki = Kp / tempoIntegrativo;
           Kd = Kp * tempoDerivativo;

           if(ordemSistema == SISTEMA_ORDEM_2 && modeSegOrdem == C_O2_CASCATA) {
                KiCas = KpCas / tempoIntegrativoCas;
                KdCas = KpCas * tempoDerivativoCas;
           }
        }
        else {
            tempoDerivativo = Kd/ Kp;
            tempoIntegrativo = Kp / Ki;

            if(ordemSistema == SISTEMA_ORDEM_2 && modeSegOrdem == C_O2_CASCATA) {

                // TODO ...

            }
        }

        controller->setWindUp(windUP);

        /*
            Para Malha fechada e 2a ordem convencional
        */

        sinalCalculado = calculaTensaoPID(controller, tipoControler, Kp, Ki, Kd, erro, sinalLeitura);

        if(debCas)
        qDebug() << "sinalCalculado = " << sinalCalculado;

        if(debCas) {
            qDebug() << "KpCas = " << KpCas << " KiCas = " << KiCas << " KdCas = " << KdCas;
        }

        if(ordemSistema == SISTEMA_ORDEM_2 && modeSegOrdem == C_O2_CASCATA) {
            qDebug() << "controle cascata -: controlador " << tipoControlerCas;

            erroCas = sinalCalculado - tanque1;
            erroCas = trunca(erroCas);
            sinalCalculado = calculaTensaoPID(contCascata, tipoControlerCas, KpCas, KiCas, KdCas, erroCas, sinalCalculado);
        }
    }
    else if(tipoMalha == M_ABERTA) {
        sinalCalculado = calculaTensao(tensao);
    }

    sinalCalculado = trunca(sinalCalculado);

    timeAux += 0.1;
}


void Control::sendSignal() {
    calculaSinal();

    sinalEscrita = sinalCalculado;

    controller->setTensaoAnt(sinalCalculado);

    travel();

    //utilizado no windup
    controller->setVPS(sinalEscrita);

    if(simulacao) tanq->acionaBomba(sinalEscrita);
    else quanser->writeDA(canalEscrita,sinalEscrita);
}

void Control::receiveSigal() {

    for(int canal=0; canal<2; canal++) {

        if(simulacao) {
            tanque1 = tanq->getNivelTq1();
            tanque2 = tanq->getNivelTq2();

            tanque1 = trunca(tanque1);
            tanque2 = trunca(tanque2);

            canaisLeitura_value[0] = tanque1;
            canaisLeitura_value[1] = tanque2;
        } else {
            canaisLeitura_value[canal] = trunca(readCanal(canal));
            tanque1 = canaisLeitura_value[0];
            tanque2 = canaisLeitura_value[1];
        }

        if(debCas) {
            qDebug() << "tanque1 = " << tanque1 << " tanque2 = " << tanque2;
        }

        if(canal==canalLeitura) {
            sinalLeitura_old = sinalLeitura;

            if(ordemSistema == SISTEMA_ORDEM_1) sinalLeitura = tanque1;
            else if(ordemSistema == SISTEMA_ORDEM_2) sinalLeitura = tanque2;

            if(tipoMalha == M_FECHADA) {
                erro = trunca(amplitude - sinalLeitura);

                sinalLeitura = trunca(sinalLeitura);


                if(auxContErro >= 5){
                    auxContErro = 0;
                }

                arrayErro[auxContErro] = erro;
                auxContErro++;

                erro = this->filtroMM(arrayErro);

                if(ordemSistema ==  SISTEMA_ORDEM_2)  {
                    tempoControle();
                }

            }

        }
    }
    if(simulacao) {
        tanq->escoaTanque1(tanque1);
        tanq->escoaTanque2(tanque2);
    }
}




