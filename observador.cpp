#include "observador.h"

double** Observador::Mat_Aloc(int linhas, int colunas) {
    double **mat = NULL;

    mat = (double**)calloc(linhas, sizeof(double));

    assert(mat != NULL);

    for(int i=0; i<linhas; i++) {
        mat[i] = (double*)calloc(colunas, sizeof(double));
        assert(mat[i] != NULL);
    }

    return mat;
}

double** Observador::Observador::Mat_Free(int linhas, int colunas, double** mat) {
    if(mat == NULL) return NULL;

    for(int i=0; i<linhas; i++) {
        free(mat[i]);
    }
    free(mat);

    return NULL;
}

double** Observador::Mat_Mult(double **matrizA, int linhasMatrizA, int colunasMatrizA, \
    double **matrizB, int linhasMatrizB, int colunasMatrizB) {

    assert(colunasMatrizA == linhasMatrizB);

    double **matrizC = Mat_Aloc(linhasMatrizA, colunasMatrizB);
    double aux;

    for(int i=0; i<linhasMatrizA; i++) {
        for(int j=0; j<colunasMatrizB; j++) {
            matrizC[i][j]=0.0;
            aux=0;
            for(int k=0; k<linhasMatrizB; k++) {
                aux += matrizA[i][k] * matrizB[k][j];
            }
            matrizC[i][j]=aux;
        }
    }

    return matrizC;
}

double** Observador::Mat_MultEscalar(double **mat, int linhas, int colunas, int escalar) {
    double **matResul = Mat_Aloc(linhas, colunas);
    for(int i=0; i<linhas; i++) {
        for(int j=0; j<colunas; j++) {
            matResul[i][j] = mat[i][j] * escalar;
        }
    }
    return matResul;
}

double** Observador::Mat_Sum(double **matA, double **matB, int linhas, int colunas) {
    double **matSum = Mat_Aloc(linhas, colunas);
    for(int i=0; i<linhas; i++) {
        for(int j=0; j<colunas; j++) {
            matSum[i][j] = matA[i][j] + matB[i][j];
        }
    }
    return matSum;
}

double** Observador::Mat_Sum(double **matA, double **matB, double** matC, int linhas, int colunas) {
    double **matSum = Mat_Aloc(linhas, colunas);
    for(int i=0; i<linhas; i++) {
        for(int j=0; j<colunas; j++) {
            matSum[i][j] = matA[i][j] + matB[i][j] + matC[i][j];
        }
    }
    return matSum;
}


void Observador::geraMatQL() {

    matB = Mat_MultEscalar(matG, 2, 2, b);
    matC = Mat_MultEscalar(matI, 2, 2, c);

    matQL = Mat_Sum(matvA, matglB, matglC, 2, 2);;

    //return matQL;
}

void Observador::geraMatL() {
    geraMatQL();

    double **matTemp = Mat_Aloc(2,2);
    matTemp = Mat_Mult(matQL, 2, 2, matWoInv, 2, 2);
    matL = Mat_Mult(matTemp, 2, 2, matColSL, 2, 1);
}

/****************/


Observador::Observador() {
    matColSL = Mat_Aloc(2,1);
    matA = Mat_Aloc(2,2);
    matB = Mat_Aloc(2,2);
    matC = Mat_Aloc(2,2);

    matG = Mat_Aloc(2,2);
    matI = Mat_Aloc(2,2);
    matH = Mat_Aloc(2,1);

    matWoInv = Mat_Aloc(2,2);

    matQL = Mat_Aloc(2,2);
    matL = Mat_Aloc(2,1);

    /*-------------------*/

    matColSL[0][0] = 0.0;
    matColSL[1][0] = 1.0;

    matG[0][0] = 0.993458;
    matG[0][1] = 0.0;
    matG[1][0] = 0.006520;
    matG[1][1] = 0.993458;

    /* Matriz G^2 */
    matA[0][0] = 0.986958798;
    matA[0][1] = 0.0;
    matA[1][0] = 0.012954692;
    matA[1][1] = 0.986958798;

    matI[0][0] = 0.986958798;
    matI[0][1] = 0.0;
    matI[1][0] = 0.012954692;
    matI[1][1] = 0.986958798;

    matWoInv[0][0] = -152.3708589;
    matWoInv[0][1] = 153.3742331;
    matWoInv[1][0] = 1.0;
    matWoInv[1][1] = 0.0;
}

Observador::~Observador() {
    matA = matAloc(2,2,matA);
    matB = matAloc(2,2,matB);
    matC = matAloc(2,2,matC);

    matG = matAloc(2,2,matG);
    matI = matAloc(2,2,matI);
    matH = matAloc(2,1,matH);
    matWoInv = matAloc(2,2,matWoInv);

    matQL = matAloc(2,2,matQL);
    matL = matAloc(2,2,matL);
}

double Observador::getL1Obs() {

}

double Observador::getL2Obs() {

}

void Observador::calculaObservador(double tensao, double y, vector<int> polo1, vector<int> polo2) {
    b = polo1[0]*2;
    c = polo1[0]*polo2[0] + polo1[1]*polo2[1];

    matXObs = Mat_Sum( Mat_Mult(matG,2,2,matXObs,2,1), \
                       Mat_MultEscalar(matL, 2, 1, y-yobs), \
                       Mat_MultEscalar(matH, 2, 1, tensao), \
                        2, 1);
}
