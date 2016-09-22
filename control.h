#ifndef CONTROL_H
#define CONTROL_H

#include "quanser.h"
#include "signal.h"
#include "controller.h"
#include "sistemaO2.h"

#include "tanque.h"

#define MAX_VOLTAGE 4
#define MIN_VOLTAGE -4

#define MAX_LEVEL 30
#define MIN_LEVEL 0

#define FATOR_CONVERSAO 6.25

#define NUMB_CAN_READ 8

#define DEGRAU 0
#define QUADRADA 1
#define SENOIDAL 2
#define DENTE_DE_SERRA 3
#define ALEATORIO 4

#define M_FECHADA 0
#define M_ABERTA 1

#define CONTROLE_CONST_TEMP 0
#define CONTROLE_GANHO 1

#define SISTEMA_ORDEM_1 1
#define SISTEMA_ORDEM_2 2

//parametro para o filtro média movel
#define M 5

class Control
{
public:
    Control(int port, QString ip);

    double  getAmplitude();

    int     getCanalEscrita();

    int     getCanalLeitura();

    double  getCanalValue(int value);

    double  getErro();

    int     getTipoMalha();

    double  getSinalCalculado();

    double  getSinalEnviado();

    double  getSinalLeitura();

    double filtroMM(double erro[]);


    // Controle
    int getModeControle() const;
    void setModeControle(int modeControle);

    int getTipoControler();
    void setTipoControler(int tipoControler);

    void setTipoOrdemSistema(int ordemSistema);

    double getKp() const;
    void setKp(double value);

    double getKi() const;
    void setKi(double value);

    double getKd() const;
    void setKd(double value);

    double getTi() const;
    double getTd() const;

    double getP() const;
    double getI() const;
    double getD() const;

    double getTempoIntegrativo() const;
    void setTempoIntegrativo(double value);

    double getTempoDerivativo() const;
    void setTempoDerivativo(double value);


    void setAmplitude(double amplitude);

    void setAuxForRand(double auxForRand);

    void setOffSet(double value);

    void setCanalEscrita(int canalEscrita);

    void setPeriodo(double value);

    void setTensao(double value);

    void setTipoMalha(int value);

    void setTipoSinal(int value);


    double readCanal(int canal);

    void calculaTPico();

    void zeraControlOrdem2();

    void calculaSinal();

    bool    getConnectionStatus();

    int     levelControl(int value);

    double  voltageControl(double value);

    void    zerarSinal();

    void    travel();

    void    sendSignal();

    void    receiveSigal();


    double getTr() const;

    double getMp() const;

    double getTp() const;

    double getTs() const;

    bool getStatusTr() const;

    bool getStatusMp() const;

    bool getStatusTp() const;

    bool getStatusTs() const;

    int getOrdemSistema() const;

    void setTipoTr(int value);

    void setTipoTs(int value);

    void setCanalLeitura(int value);

    void setWindUP(bool value);

private:
    Quanser *quanser;
    Signal *signal;
    Controller *controller;
    SistemaO2 *sistemaO2;

    Tanque *tanq;

    int tipoMalha = 1;  /* 0 -> fechada, 1 -> aberto*/

    double timeAux;
    int tipoSinal;      /* 0 = Degrau | 1 = Quadrada | 2 = Senoidal | 3 = Dente de Serra | 4 = Aleatório */

    double sinalEscrita;
    double sinalLeitura;
    double sinalLeitura_old;
    double sinalCalculado;

    // Update data function: _data()
    double tensao;
    double offSet;
    double periodo;
    double amplitude;
    double auxForRand;

    // Malha fechada function: _receiveData()
    double erro, erroAnt;
    double arrayErro[5];
    int auxContErro;

    int canalEscrita;
    int canalLeitura;
    int canalLeituraO2;

    double canaisLeitura_value[NUMB_CAN_READ] = {0};

    // Controlador
    int tipoControler;
    int modeControle;
    double Kp;
    double Ki;
    double Kd;
    double Ti;
    double Td;
    double tempoIntegrativo;
    double tempoDerivativo;

    double P, I , D;
    // Sistema de Ordem 2
    int ordemSistema;

    double tr,
           mp,
           tp,
           ts;

    bool statusTr,
         statusMp,
         statusTp,
         statusTs;

    int tipoTr,
        tipoTs;

    bool windUP;

    bool setPointUP;

    double tanque1;
    double tanque2;
    bool simulacao;
};

#endif // CONTROL_H
