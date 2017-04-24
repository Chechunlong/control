#ifndef SISTEMAO2_H
#define SISTEMAO2_H

#include <cstdlib>

//#include <qdebug.h>

#define TR100 0
#define TR95 1
#define TR90 2

#define TS2 0
#define TS5 1
#define TS10 2

#define ABS 0
#define PICOm 1

#define TEMPO_AMOSTRAGEM 0.1

#define FILTRO_TS 100

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

    double ts_aux = 0;
    double ts_derivada = 0;
    bool ts_enable = false;

    double trunca(double numero);

    int auxFiltroTs = 0;

    double filtroTS[FILTRO_TS] = {0};

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

    double _filtroTS();

    void configTr(int tipoTr, double sinalLeitura, double amplitude);
    void configTs(int tipoTs, double sinalLeitura);

    void calculaTr(double sinalLeitura, double amplitude);
    void calculaTp(double sinalLeitura, double sinalLeitAnterior, double amplitude, int tipoMp);
    void calculaMp(double sinalLeitura, int tipoMp, double amplitude);
    void calculaTs(double sinalLeitura, double sinalLeitAnterior, double setPoint);
};

#endif // SISTEMAO2_H
