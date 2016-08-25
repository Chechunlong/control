#include "controller.h"

controller::controller()
{
    P = 0;
    I = 0;
    D = 0;
}

controller::atualizaController(int tipo,double kp, double ki, double kd,double erro, double erroAnt){

    switch(tipo){

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

        default:
            P = 0;
            I = 0;
            D = 0;
            break;

    }

    return (P+I+D);
}
