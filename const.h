#ifndef CONST_H
#define CONST_H

//static const char* SERVER_IP = "10.13.99.69";
//const int PORT_SERVER = 20081;

const double MAX_VOLTAGE = 4;
const double MIN_VOLTAGE = -4;
const int MAX_LEVEL = 30;
const int MIN_LEVEL = 0;

const double FATOR_CONVERSAO = 6.25;

static const int NUMB_CAN_READ = 8;

static constexpr int DEGRAU = 0;
static constexpr int QUADRADA = 1;
static constexpr int SENOIDAL = 2;
static constexpr int DENTE_DE_SERRA = 3;
static constexpr int ALEATORIO = 4;

const int M_FECHADA = 0;
const int M_ABERTA = 1;

#endif // CONST_H
