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
        if(tipoAmplitude) {
            if(sinalLeitura >= trMin && sinalLeitura <= trMax)
                statusTr = true;
            else
                tr += TEMPO_AMOSTRAGEM;
        } else {
            if(sinalLeitura <= trMin && sinalLeitura >= trMax)
                statusTr = true;
            else
                tr += TEMPO_AMOSTRAGEM;
        }
    }
}

void SistemaO2::calculaTp(bool sinalLeitura, bool sinalLeitAnterior) {

    if(!statusTp) {
        tp += TEMPO_AMOSTRAGEM;
        if(tipoAmplitude) {
            if(sinalLeitura <= sinalLeitAnterior) {
                statusTp = true;
                //calculaMp(sinalLeitura, amplitude);
            }
        } else {
            if(sinalLeitura >= sinalLeitAnterior) {
                statusTp = true;
                //calculaMp(sinalLeitura, amplitude);
            }
        }
    }
}

void SistemaO2::calculaMp(bool sinalLeitura, bool amplitude) {
    mp = abs(sinalLeitura-amplitude);
    statusMP = true;
}
