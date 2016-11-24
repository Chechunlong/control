#ifndef TANQUE_H
#define TANQUE_H

#include <QDebug>
#include <cmath>
class Tanque
{
public:


    Tanque() {
	    this->nivelTq1 = 0;
    	this->nivelTq2 = 0;
	}

    double getRuido() {
        srand(time(NULL));
        double ampMax = 10;
        double ampMin = -10;
         double gg = (((double) rand () / ((double) RAND_MAX + 1)) * (ampMax - ampMin + 1))+ ampMin;
         gg /= 100;

         qDebug() << gg;
         return gg;

    }

    double getNivelTq1() {

        return this->nivelTq1;
	}    
    double getNivelTq2() {
		return this->nivelTq2;
	}    
    double acionaBomba(double sinal) {

		if(sinal > 4.0) sinal = 4.0;
		if(sinal < -4.0) sinal = -4.0;


		if(nivelTq1 <= nivelMinimo && sinal < 0){
		    sinal = 0;
		}

		if(this->nivelTq1 > nivelMaximo && sinal > 0){
		    sinal = 0;
		}

		if(this->nivelTq1 > nivelCritico){
		    sinal = 0;
		}

		this->nivelTq1 += 0.1*(sinal * this->K) / this->AreaTanque;

		return sinal;
	}
    void escoaTanque1(double nivelTanque) {

		double deltaNivel = 0.1*this->AreaOrificioSaida * sqrt(2*980*nivelTanque)/this->AreaTanque;

		this->nivelTq1 -= deltaNivel;

		if(this->nivelTq1 < 0) this->nivelTq1 = 0;

		this->nivelTq2 += deltaNivel;

	}    
    void escoaTanque2(double niveltanque) {

		double deltaNivel = 0.1*this->AreaOrificioSaida * sqrt(2*980*niveltanque)/this->AreaTanque;
		this->nivelTq2 -= deltaNivel;
		if(this->nivelTq2 < 0) this->nivelTq2 = 0;
	}



private:
    double const K = 10;//cm3/v;
    double const AreaOrificioSaida = 0.17813919765; //cm2
    double const AreaTanque = 15.95179 ;//cm2

    double nivelTq1 = 0;
    double nivelTq2 = 0;
    double nivelMinimo = 1.5;
    double nivelMaximo = 28.0;
    double nivelCritico = 29.5;
};

#endif // TANQUE_H
