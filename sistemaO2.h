#ifndef SISTEMAO2_H
#define SISTEMAO2_H

#include <cstdlib>

#define TR100 0
#define TR95 1
#define TR90 2

#define TS2 0
#define TS5 1
#define TS10 2

#define TEMPO_AMOSTRAGEM 0.1

class SistemaO2 {

    bool tipoAmplitude;
    /* Subida eh true, descida eh false */

    double mp;
    double tr, tempTr;
    double tp, tempTp;
    double ts, tempTs;

    int tipoTr;
    int tipoTs;

    double trMin;
    double trMax;

    bool statusTp;
    bool statusTr;
    bool statusTs;
    bool statusMP;
public:
    SistemaO2() {
        tipoAmplitude = true;

        mp = 0;
        tr = 0;
        tp = 0;
        ts = 0;

        tempTr = 0;
        tempTp = 0;
        tempTs = 0;

        statusTp = false;
        statusTr = false;
        statusTs = false;
        statusMP = false;
    }

    double getMp() const;
    double getTr() const;
    double getTp() const;
    double getTs() const;

    bool getStatusTp() const;
    void setStatusTp(bool value);
    bool getStatusTr() const;
    void setStatusTr(bool value);
    bool getStatusTs() const;
    void setStatusTs(bool value);
    bool getStatusMP() const;
    void setStatusMP(bool value);
    void setTipoAmplitude(bool value);

    void configTr(int tipoTr, bool sinalLeitura, bool amplitude);

    void calculaTr(bool sinalLeitura, bool amplitude);
    void calculaTp(bool sinalLeitura, bool sinalLeitAnterior);
    void calculaMp(bool sinalLeitura, bool amplitude);
};

#endif // SISTEMAO2_H
