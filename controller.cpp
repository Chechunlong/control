#include "controller.h"

Controller::Controller()
{
    P = 0;
    I = 0;
    D = 0;
}

Controller::atualizaController(int tipo,double kp, double ki, double kd,double erro, double erroAnt)
{

    switch(tipo)
    {

        case ControllerP:
            P = erro * kp;
            I = 0;
            D = 0;
            break;

        case ControllerPI:
            P = erro * kp;
            I += erro * ki * 0.1;
            D = 0;
            break;

        case ControllerPD:
            P = erro * kp;
            I = 0;
            D = (erro - erroAnt) * kd/0.1;
            erroAnt = erro;
            break;

        case ControllerPID:
            P = erro * kp;
            I += erro * ki * 0.1;
            D = (erro - erroAnt) * kd/0.1;
            erroAnt = erro;
            break;

        case ControllerPIDer:
            P = erro * kp;
            I += erro * ki * 0.1;
            D = 0; //TODO
            break;

        default:
            P = 0;
            I = 0;
            D = 0;
            break;

    }

    return (P+I+D);
}
