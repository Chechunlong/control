#include "sistemaO2.h"

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
            trMin = .9*amplitude;
            trMax = .1*amplitude;
            break;
    }
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

    if(!statusTr) {
        if(tipoAmplitude) {
           // qDebug() << "aqui";
            if(sinalLeitura >= trMin && sinalLeitura <= trMax) {
                tempTr += TEMPO_AMOSTRAGEM;
             }
            qDebug() << "tempo de resposta " << tempTr;
            if(sinalLeitura>trMax) {
                statusTr = true;
                tr = tempTr/10;
                tempTr = 0;
            } else {
                statusTr = false;
            }
        } else {
            if(sinalLeitura <= trMin && sinalLeitura >= trMax) {
                tempTr += TEMPO_AMOSTRAGEM;
             }
            if(sinalLeitura<trMax) {
                statusTr = true;
                tr = tempTr/10;
                tempTr = 0;
            } else {
                statusTr = false;
            }
        }
    }
}

void SistemaO2::calculaTp(double sinalLeitura, double sinalLeitAnterior) {

    if(!statusTp) {

        if(tipoAmplitude) {
            qDebug() << sinalLeitura << sinalLeitAnterior;
            if(sinalLeitura >= sinalLeitAnterior) {
                tempTp += TEMPO_AMOSTRAGEM;
            }
            qDebug() << "tempo de pico " << tempTp;

            if(sinalLeitura < sinalLeitAnterior) {
                statusTp = true;
                tp = tempTp/10;
                tempTp = 0;
                statusMP = true;
                //exit(-1);
            } else {

                statusTp = false;
            }
        } else {
            if(sinalLeitura <= sinalLeitAnterior) {
                tempTp += TEMPO_AMOSTRAGEM;
                qDebug() << "biruba";
            }
            if(sinalLeitura > sinalLeitAnterior) {
                statusTp = true;
                tp = tempTp/10;
                tempTp = 0;
                statusMP = true;
            } else {
                statusTp = false;
            }
        }
    }
}

void SistemaO2::calculaMp(double sinalLeitura, double amplitude) {
    if(!statusMP) {
        mp = abs(sinalLeitura-amplitude);
        statusMP = false;
    }
}

void SistemaO2::calculaTs(double sinalLeitura, double setPoint) {
    if(!statusTs) {
        double erro = abs(sinalLeitura - setPoint);
        tempTs += TEMPO_AMOSTRAGEM;
        if(erro <= ts && statusTs) {
            ts = tempTs;
            statusTs = false;
            tempTs = 0;
        } else if (erro > ts) {
            ts = tempTs;
            statusTs = true;
        }
    }

}
