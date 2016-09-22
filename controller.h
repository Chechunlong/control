#include "math.h"
#ifndef CONTROLLER_H
#define CONTROLLER_H

#define TEMPO_AMOSTRAGEM 0.1

#define CONTROLER_P 0
#define CONTROLER_PD 1
#define CONTROLER_PI 2
#define CONTROLER_PID 3
#define CONTROLER_PI_D 4

class Controller
{
    public:
        Controller();

        double ganhoKp(double Kp, double erro);
        double ganhoKi(double Ki, double Kd, double erro);
        double ganhoKd(double Kd, double erro);

        double getP() const;
        double getI() const;
        double getD() const;

        double controlerP(double Kp, double erro);
        double controlerPI(double Kp, double Ki, double erro);
        double controlerPD(double Kp, double Kd, double erro);
        double controlerPID(double Kp, double Ki, double Kd, double erro);
        double controlerPI_D(double Kp, double Ki, double Kd, double erro, double amplitude);
        void setTensaoAnt(double tensao);
        double getTensaoAnt();
        void setWindUp(bool windup);
        bool isWindUp();
        void setVPS(double vps);
        double getVPS();


    private:
        double erroAnt;
        double integrador;
        double tensaoAnt;
        bool windup = false;
        double vps;

        double P;
        double I;
        double D;
};

#endif // CONTROLLER_H
