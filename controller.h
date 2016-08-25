#ifndef CONTROLLER_H
#define CONTROLLER_H

#define ControllerP 0
#define ControllerPI 1
#define ControllerPD 2
#define ControllerPID 3
#define ControllerPIDer 4;


class controller
{
public:
    controller();

    double atualizaController(int tipo);

private:
    double P;
    double I;
    double D;
};

#endif // CONTROLLER_H
