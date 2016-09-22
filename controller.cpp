#include "controller.h"

Controller::Controller()
{
    erroAnt = 0;
    integrador = 0;
}

double Controller::ganhoKp(double Kp, double erro)
{
    return erro * Kp;
}

double Controller::ganhoKi(double Ki, double Kd, double erro)
{
    this->integrador += erro * Ki * TEMPO_AMOSTRAGEM;

    if(isWindUp()){
        double Tt = sqrt(Kd/Ki);
        this->integrador = this->integrador + Ki * erro + (1/(Tt))*(getVPS() - getTensaoAnt());
    }

    return this->integrador;
}

double Controller::ganhoKd(double Kd, double erro)
{
    double derivador = Kd * (erro-this->erroAnt) / TEMPO_AMOSTRAGEM;
    this->erroAnt = erro;

    return derivador;
}

double Controller::controlerP(double Kp, double erro)
{
    return ganhoKp(Kp, erro);
}

double Controller::controlerPI(double Kp, double Ki, double erro)
{
    return ganhoKp(Kp, erro) + ganhoKi(Ki, 0, erro);
}

double Controller::controlerPD(double Kp, double Kd, double erro)
{
    return ganhoKp(Kp, erro) + ganhoKd(Kd, erro);
}

double Controller::controlerPID(double Kp, double Ki, double Kd, double erro)
{
    return ganhoKp(Kp, erro) + ganhoKi(Ki, Kd, erro) + ganhoKd(Kd, erro);
}

double Controller::controlerPI_D(double Kp, double Ki, double Kd, double erro, double amplitude)
{
    return ganhoKp(Kp, erro) + ganhoKi(Ki, Kd, erro) + ganhoKd(Kd, amplitude);
}

void Controller::setTensaoAnt(double tensao){
    this->tensaoAnt = tensao;
}

double Controller::getTensaoAnt(){
    return this->tensaoAnt;
}

void Controller::setWindUp(bool windup){
    this->windup = windup;
}

bool Controller::isWindUp(){
    return this->windup;
}

void Controller::setVPS(double vps){
    this->vps = vps;
}

double Controller::getVPS(){
    return this->vps;
}
