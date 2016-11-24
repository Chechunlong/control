#include "controller.h"

Controller::Controller()
{
    erroAnt = 0;
    integrador = 0;
    tensaoAnt = 0;
    vps = 0;
    taw = 75;
}



double Controller::ganhoKp(double Kp, double erro)
{
    P = erro * Kp;
    return P;
}

double Controller::ganhoKi(double Ki, double Kd, double Kp, double erro)
{
    this->integrador += erro * Ki * TEMPO_AMOSTRAGEM;
    I = this->integrador;

    taw = 75/sqrt(0.005/0.05) * sqrt(Kd/Ki);

    if(isWindUp()){
        I += (Kp/taw)*TEMPO_AMOSTRAGEM*(vps - tensaoAnt);
    }
    this->integrador = I;
    return I;
}

double Controller::ganhoKd(double Kd, double erro)
{
    double derivador = Kd * (erro-this->erroAnt) / TEMPO_AMOSTRAGEM;
    this->erroAnt = erro;

    D = derivador;
    return D;
}

double Controller::controlerP(double Kp, double erro)
{
    return ganhoKp(Kp, erro);
}

double Controller::controlerPI(double Kp, double Ki, double erro)
{
    return ganhoKp(Kp, erro) + ganhoKi(Ki, 0, Kp, erro);
}

double Controller::controlerPD(double Kp, double Kd, double erro)
{
    return ganhoKp(Kp, erro) + ganhoKd(Kd, erro);
}

double Controller::controlerPID(double Kp, double Ki, double Kd, double erro)
{
    return ganhoKp(Kp, erro) + ganhoKi(Ki, Kd, Kp, erro) + ganhoKd(Kd, erro);
}

double Controller::controlerPI_D(double Kp, double Ki, double Kd, double erro, double amplitude)
{
    return ganhoKp(Kp, erro) + ganhoKi(Ki, Kd, Kp, erro) + ganhoKd(Kd, amplitude);
}

double Controller::getP() const {
    return P;
}
double Controller::getI() const {
    return I;
}
double Controller::getD() const {
    return D;
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



