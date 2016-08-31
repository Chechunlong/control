#ifndef CONTROLLER_H
#define CONTROLLER_H

#define ControllerP 0
#define ControllerPI 1
#define ControllerPD 2
#define ControllerPID 3
#define ControllerPIDer 4


class Controller
{
    public:
        Controller();
        /*
        * @param tipo
        * @param
        */
        double atualizaController(int tipo,double kp, double ki, double kd,double erro, double erroAnt);

    private:
        double P;
        double I;
        double D;
};

#endif // CONTROLLER_H
