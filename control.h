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

#define C_O2_CONVENCIONAL 1
#define C_O2_CASCATA 2

//parametro para o filtro média movel
#define P_MM 5

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

    double trunca(double numero);


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

    void setTempoIntegrativoCas(double value);
    void setTempoDerivativoCas(double value);


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

    double calculaTensaoPID(Controller *controller, double tipoControler, double Kp, double Ki, double Kd, double erro, double sinalLeitura);

    void tempoControle();

    double calculaTensao(double tensao);

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

    void setTipoMp(int value);

    void setCanalLeitura(int value);

    void setWindUP(bool value);

    void setModeSegOrdem(int value);


    void setTipoControlerCas(double value);

    void setKpCas(double value);

    void setKiCas(double value);

    void setKdCas(double value);

    void setModeControleCas(double value);



private:
    Quanser *quanser;
    Signal *signal;
    Controller *controller;
    Controller *contCascata;
    SistemaO2 *sistemaO2;
    Tanque *tanq;

    int tipoMalha = M_FECHADA;  /* 0 -> fechada, 1 -> aberto*/

    double timeAux = 0.0;
    int tipoSinal = DEGRAU;      /* 0 = Degrau | 1 = Quadrada | 2 = Senoidal | 3 = Dente de Serra | 4 = Aleatório */

    double sinalEscrita = 0;
    double sinalLeitura = 0;
    double sinalLeitura_old = 0;
    double sinalCalculado = 0;

    // Update data function: _data()
    double tensao = 0;
    double offSet = 0;
    double periodo = 0;
    double amplitude = 0;
    double auxForRand = 0;

    // Malha fechada function: _receiveData()
    double erro = 0;
    double erroAnt = 0;
    double arrayErro[5] = {0};
    int auxContErro = 0;

    int canalEscrita = 0;
    int canalLeitura = 0;
    int canalLeituraO2 = 0;

    double canaisLeitura_value[NUMB_CAN_READ] = {0};

    // Controlador
    int tipoControler = CONTROLER_P;
    int modeControle = CONTROLE_GANHO;
    double Kp = 0;
    double Ki = 0;
    double Kd = 0;
    double KpCas = 0;
    double KiCas = 0;
    double KdCas = 0;
    double Ti = 0;
    double Td = 0;
    double tempoIntegrativo = 0;
    double tempoDerivativo = 0;

    double erroCas = 0;
    double tensaoCas = 0;
    double tipoControlerCas = 0;
    double modeControleCas = 0;
    double sinalLeituraCas = 0;

    double tempoIntegrativoCas = 0;
    double tempoDerivativoCas = 0;

    double P = 0;
    double I = 0;
    double D = 0;

    // Sistema de Ordem 2
    int ordemSistema = SISTEMA_ORDEM_1;
    int modeSegOrdem = C_O2_CONVENCIONAL; /* Se eh convencional ou em cascata */

    double tr = 0;
    double mp = 0;
    double tp = 0;
    double ts = 0;

    bool statusTr = false;
    bool statusMp = false;
    bool statusTp = false;
    bool statusTs = false;

    int tipoTr;
    int tipoTs;
    int tipoMp;

    bool windUP = false;

    bool setPointUP;

    double tanque1 = 0;
    double tanque2 = 0;

    bool simulacao = false;

    bool debCas = true;
};

#endif // CONTROL_H
