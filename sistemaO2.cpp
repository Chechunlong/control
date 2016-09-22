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

void SistemaO2::configTr(int tipoTr, bool sinalLeitura, bool amplitude) {

    this->tipoTr = tipoTr;

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

void SistemaO2::calculaTr(bool sinalLeitura, bool amplitude) {

    if(!statusTr) {
        tempTr += TEMPO_AMOSTRAGEM;
        if(tipoAmplitude) {
            if(sinalLeitura >= trMin && sinalLeitura <= trMax) {
                statusTr = true;
                tr = tempTr;
                tempTr = 0;
            } else {
                statusTr = false;
            }
        } else {
            if(sinalLeitura <= trMin && sinalLeitura >= trMax) {
                statusTr = true;
                tr = tempTr;
                tempTr = 0;
            } else {
                statusTr = false;
            }
        }
    }
}

void SistemaO2::calculaTp(bool sinalLeitura, bool sinalLeitAnterior) {

    if(!statusTp) {
        tempTp += TEMPO_AMOSTRAGEM;
        if(tipoAmplitude) {
            if(sinalLeitura <= sinalLeitAnterior) {
                statusTp = true;
                tp = tempTp;
                tempTp = 0;
                statusMP = true;
            } else {
                statusTp = false;
            }
        } else {
            if(sinalLeitura >= sinalLeitAnterior) {
                statusTp = true;
                tp = tempTp;
                tempTp = 0;
                statusMP = true;
            } else {
                statusTp = false;
            }
        }
    }
}

void SistemaO2::calculaMp(bool sinalLeitura, bool amplitude) {
    if(!statusMP) {
        mp = abs(sinalLeitura-amplitude);
        statusMP = false;
    }
}
