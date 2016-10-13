#ifndef FILTROMMV_H
#define FILTROMMV_H

#define N_AMOSTRA 4

class FiltroMMV {
private:
    double amostras[N_AMOSTRA] = {0};
    int ind = 0, indAux;
    bool status;
public:
    FiltroMMV() {
        ind = 0;
        indAux = 0;
        status = true;
    }

    void add(double value) {
        if(status) {
            amostras[indAux] = value;
            indAux++;
            if(indAux == N_AMOSTRA)
                status = false;
        } else {
            amostras[ind] = value;
            ind==N_AMOSTRA-1 ? ind = 0 : ind ++;
        }
    }

    double media() {
        double temp = 0;
        int fator;

        if(status) fator = indAux;
        else fator = N_AMOSTRA;

        for(int i=0; i<fator; i++) {
            temp += amostras[i];
        }
        return temp/fator;
    }


};

#endif // FILTROMMV_H
