#ifndef CONTROL_H
#define CONTROL_H

#include "quanser.h"
#include "signal.h"
#include "const.h"

class Control
{
public:
    Control();

    double getAmplitude();

    int getCanalEscrita();

    int getCanalLeitura();

    bool getCanalValue(int value);

    double getErro();

    int getTipoMalha();

    double getSinalCalculado();

    double getSinalEnviado();

    double getSinalLeitura();

    void setAmplitude(double value);

    void setAuxForRand(double value);

    void setOffSet(double value);

    void setCanalEscrita(int value);

    void setPeriodo(double value);

    void setTensao(double value);

    void setTipoMalha(int value);

    void setTipoSinal(int value);


    bool connectionStatus();

    int levelControl(int value);

    double voltageControl(double value);

    void travel();

    void sendSignal();

    void receiveSigal();



private:
    Quanser *quanser;
    Signal *signal;

    int tipoMalha = 1; /* 0 -> fechada, 1 -> aberto*/

    double timeAux;
    int tipoSinal = 0; /* 0 = Degrau | 1 = Quadrada | 2 = Senoidal | 3 = Dente de Serra | 4 = Aleatório */

    double sinalEscrita;
    double sinalLeitura;
    double sinalCalculado;

    // Update data function: _data()
    double tensao;
    double offSet;
    double periodo;
    double amplitude;
    double auxForRand;

    // Malha fechada function: _receiveData()
    double erro;

    int canalEscrita;
    int canalLeitura;

    double canaisLeitura_value[NUMB_CAN_READ];
};

#endif // CONTROL_H
