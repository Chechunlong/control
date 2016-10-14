#include "sistemaO2.h"

double SistemaO2::trunca(double numero) {

    //return numero;

    double fator = 10.0;

    return (int)numero + ( ( (int)((numero - (int)numero) * fator) ) /  fator);
}

bool sinal(double a, double b){ return (a/abs(a))==(b/abs(b));}

double SistemaO2::getMp() const { return mp; }

double SistemaO2::getTr() const { return tr; }

double SistemaO2::getTp() const { return tp; }

double SistemaO2::getTs() const { return ts; }

bool SistemaO2::getStatusTr() const { return statusTr; }

void SistemaO2::setStatusTr(bool value) { statusTr = value; }

bool SistemaO2::getStatusTs() const { return statusTs; }

void SistemaO2::setStatusTs(bool value) { statusTs = value; }

bool SistemaO2::getStatusMP() const { return statusMP; }

void SistemaO2::setStatusMP(bool value) { statusMP = value; }

bool SistemaO2::getStatusTp() const { return statusTp; }

void SistemaO2::setStatusTp(bool value) { statusTp = value; }

void SistemaO2::setTipoAmplitude(bool value) { tipoAmplitude = value; }

void SistemaO2::configTr(int tipoTr, double sinalLeitura, double amplitude) {

    this->tipoTr = tipoTr;

    // resetando
    tr = ts = tp = mp = 0;
    tempTp = tempTr = tempTs = 0;
    statusMP = statusTp = statusTr = statusTs = false;

    switch (tipoTr) {
        case TR100:
            trMin = sinalLeitura;
            trMax = amplitude;
            break;
        case TR95:
            trMin = .05*amplitude;
            trMax = .95*amplitude;
            break;
        case TR90:
            trMin = .1*amplitude;
            trMax = .9*amplitude;
            break;
    }
    trMin += sinalLeitura;
    //qDebug() << "trMin " << trMin << " trMax " << trMax;
}

void SistemaO2::configTs(int tipoTs, double sinalLeitura) {
    this->tipoTs = tipoTs;
    switch (tipoTs) {
        case TS2:
            ts = sinalLeitura*.02;
            break;
        case TS5:
            ts = sinalLeitura*.05;
            break;
        case TS10:
            ts = sinalLeitura*.10;
            break;
    }
}

void SistemaO2::calculaTr(double sinalLeitura, double amplitude) {

    //sinalLeitura = trunca(sinalLeitura);
    //qDebug() << "sl tr" << sinalLeitura;
    if(sinalLeitura<0) sinalLeitura = 0;

    if(!statusTr) {
        if(tipoAmplitude) {

            if(sinalLeitura<trMax) {
                tempTr += TEMPO_AMOSTRAGEM;
                statusTr = false;
            } else {
                statusTr = true;
                tr = tempTr;
                tempTr = 0;

            }
        } else {

            if(sinalLeitura>trMax) {
             tempTr += TEMPO_AMOSTRAGEM;
                statusTr = false;

            } else {
                tr = tempTr;
                tempTr = 0;
                statusTr = true;
            }
        }
    }
}

void SistemaO2::calculaTp(double sinalLeitura, double sinalLeitAnterior, double amplitude, int tipoMp) {

    //sinalLeitura = trunca(sinalLeitura);
    //sinalLeitAnterior = trunca(sinalLeitAnterior);
    //if(sinalLeitura<0) sinalLeitura = 0;
    //if(sinalLeitAnterior<0) sinalLeitAnterior = 0;
    //qDebug() << "sl tp " << sinalLeitura;
    //qDebug() << "sla tp " << sinalLeitAnterior;

    if(!statusTp)
    tempTp += TEMPO_AMOSTRAGEM;

    if(statusTr)
    if(!statusTp) {


        if(tipoAmplitude) {
            //qDebug() << sinalLeitura << sinalLeitAnterior;
            qDebug() << "eh aqui";
            //tr true
            if(sinalLeitura >= sinalLeitAnterior) {

                // cotinua lendo
                statusTp = false;
                statusMP = false;
            } else {


                statusTp = true;
                tp = tempTp;
                tempTp = 0;

                switch(tipoMp){
                    case ABS:
                        mp = sinalLeitura - amplitude;
                    break;
                    case PICOm:
                        mp = sinalLeitura;
                    break;
                }
               if(mp<0) mp=-mp;
                statusMP = true;

                //exit(0);
            }
        } else {
            if(sinalLeitura <= sinalLeitAnterior) {
                //tempTp += TEMPO_AMOSTRAGEM;

                statusTp = false;
                statusMP = false;
            } else {


                statusTp = true;
                tp = tempTp;
                tempTp = 0;

                switch(tipoMp){
                    case ABS:
                        mp = sinalLeitura - amplitude;
                    break;
                    case PICOm:
                        mp = sinalLeitura;
                    break;
                }
               if(mp<0) mp=-mp;
                statusMP = true;

            }
        }
    }
}

void SistemaO2::calculaMp(double sinalLeitura, int tipoMp, double amplitude) {

}

void SistemaO2::calculaTs(double sinalLeitura, double sinalLeitAnterior, double setPoint)
{
    double erro = sinalLeitura - setPoint;
    if (erro <0) erro = -erro;
    double derivada = sinalLeitura - sinalLeitAnterior;
    double tolerancia = 0;

    switch (tipoTs) {
        case TS2:
            tolerancia = setPoint*.02;
            break;
        case TS5:
            tolerancia = setPoint*.05;
            break;
        case TS10:
            tolerancia = setPoint*.10;
            break;
    }

    if(!statusTs && erro >= tolerancia) statusTs = true;
    else if(statusTs) {

    ts_aux += TEMPO_AMOSTRAGEM;

    if(erro <= tolerancia && ts_enable)
    {//Quando o erro for aceitável entrar aqui somente uma vez
        ts = ts_aux;
        ts_enable = false;
        ts_derivada = derivada;
    }
    if (erro > tolerancia)
    {//Mostrar contagem enquanto erro não for aceitável
        ts = ts_aux;
        ts_enable = true;
    }

    if((derivada == 0 || !sinal(ts_derivada,derivada)) && !ts_enable)
    {
        statusTs = false;
        ts_aux = 0;
    }
    }
    qDebug() << "ts " << ts;
}


double SistemaO2::_filtroTS() {
    double tmp = 0;
    for(int i=0;i < FILTRO_TS;i++){
        tmp += filtroTS[i];
    }
    return tmp/FILTRO_TS;
}
