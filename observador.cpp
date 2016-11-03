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

double** Observador::Mat_MultEscalar(double **mat, int linhas, int colunas, double escalar) {

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


void Observador::geraMatQL(double b, double c) {

    matGlB = Mat_MultEscalar(matG, 2, 2, b);
    matGlC = Mat_MultEscalar(matI, 2, 2, c);

    matQL = Mat_Sum(matGlA, matGlB, matGlC, 2, 2);

}

void Observador::geraMatL(double b, double c) {
    geraMatQL( b,  c);

    double **matTemp = Mat_Aloc(2,2);
    matTemp = Mat_Mult(matQL, 2, 2, matWoInv, 2, 2);

    matls = Mat_Mult(matTemp, 2, 2, matColSL, 2, 1);

}

void Observador::geraMatL2(double b, double c) {

    geraMatQL( b,  c);

    double **matTemp = Mat_Aloc(2,2);
    matTemp = Mat_Mult(matQL, 2, 2, matWoInv, 2, 2);

    matL = Mat_Mult(matTemp, 2, 2, matColSL, 2, 1);

}

/****************/


Observador::Observador() {
    b = 0.0;
    c = 0.0;
    bold = 0.0;
    cold = 0.0;
    matColSL = Mat_Aloc(2,1);
    matGlA = Mat_Aloc(2,2);
    matGlB = Mat_Aloc(2,2);
    matGlC = Mat_Aloc(2,2);

    matC = Mat_Aloc(1,2);

    matG = Mat_Aloc(2,2);
    matI = Mat_Aloc(2,2);
    matH = Mat_Aloc(2,1);

    matWoInv = Mat_Aloc(2,2);

    matQL = Mat_Aloc(2,2);
    matL = Mat_Aloc(2,1);

    matXObs = Mat_Aloc(2,1);

    matXObs[0][0] = 0.0;
    matXObs[1][0] = 0.0;

    matpolos = Mat_Aloc(2,2);
    matls = Mat_Aloc(2,1);

    /*-------------------*/

    matC[0][0] = 0;
    matC[0][1] = 1;

    matColSL[0][0] = 0.0;
    matColSL[1][0] = 1.0;

    matG[0][0] = 0.993458;
    matG[0][1] = 0.0;
    matG[1][0] = 0.006520;
    matG[1][1] = 0.993458;

    /* Matriz G^2 */
    matGlA[0][0] = 0.986958798;
    matGlA[0][1] = 0.0;
    matGlA[1][0] = 0.012954692;
    matGlA[1][1] = 0.986958798;

    matI[0][0] = 1.0;
    matI[0][1] = 0.0;
    matI[1][0] = 0.0;
    matI[1][1] = 1.0;

    matH[0][0] = 0.021196;
    matH[1][0] = 0.000069;

    matWoInv[0][0] = -152.3708589;
    matWoInv[0][1] = 153.3742331;
    matWoInv[1][0] = 1.0;
    matWoInv[1][1] = 0.0;
}

Observador::~Observador() {
    matGlA = Mat_Free(2,2,matGlA);
    matGlB = Mat_Free(2,2,matGlB);
    matGlC = Mat_Free(2,2,matGlC);

    matG = Mat_Free(2,2,matG);
    matI = Mat_Free(2,2,matI);
    matH = Mat_Free(2,1,matH);
    matWoInv = Mat_Free(2,2,matWoInv);

    matQL = Mat_Free(2,2,matQL);
    matL = Mat_Free(2,2,matL);

}


double Observador::calculaObservador(double tensao, double y, double polo1[2], double polo2[2]) {

    b = polo1[0]*2;
    c = polo1[0]*polo2[0] + polo1[1]*polo2[1];

    // qDebug() << "polos";
    qDebug() << polo1[0] << polo1[1];
    qDebug() << polo2[0] << polo2[1];
    //qDebug() << "zzzzzzz " << b << c;

    if(b != bold || c != cold) {
       // qDebug() << "#######################################!";
       // qDebug() << "calculaObservador " << b << c;
        geraMatL2(b,c);
        //matL[0][0] = matls[0][0];
        //matL[1][0] = matls[1][0];

        //qDebug() << "matL" << matL[0][0] << matL[1][0];
        //qDebug() << "matls" << matls[0][0] << matls[1][0];

        //qDebug() << "b c";
        //qDebug() << b << c;
        //qDebug() << bold << cold;
        bold = b;
        cold = c;
        bold = b;
        cold = c;
        //qDebug() << bold << cold;
    }

    matXObs = Mat_Sum(Mat_Mult(matG,2,2,matXObs,2,1), \
                      Mat_MultEscalar(matL, 2, 1, y-yObs), \
                      Mat_MultEscalar(matH, 2, 1, tensao), \
                      2, 1);

    double **temp = Mat_Aloc(1,1);
    temp =  Mat_Mult(matC,1,2, matXObs,2,1);

    yObs = temp[0][0];
    erro = y - yObs;
  //  qDebug() << "yObs " << yObs;
    Mat_Free(1,1,temp);
    return yObs;
}

double Observador::getErroObs() {
    return erro;
}

double** Observador::getMatL() {
    return matL;
}

void Observador::setMatL(double** matL) {
    this->matL = matL;
}

double** Observador::getPoloFromL(double** matls)  {

    double l1 =matls[0][0];
    double l2 =matls[1][0];
    double b = (l2/wozz - matGlA[1][0])/ matG[1][0];
    double c = l1/wozz - matGlA[0][0] - b*matG[0][0];


    double complex = pow(b,2)-4*c;
    double real =  b/2;


    if(complex<0) complex *=-1;
    complex = sqrt(complex)/2;

    matpolos[0][0] = real;
    matpolos[0][1] = complex;

    matpolos[1][0] = real;
    matpolos[1][1] = -complex;


    return matpolos;
}

double** Observador::getLFromPolo(double** matpolos) {
   // qDebug() << "matpolos[0][0]" << matpolos[0][0];
   // qDebug() << "matpolos[0][1]" << matpolos[0][1];
   // qDebug() << "matpolos[1][0]" << matpolos[1][0];
   // qDebug() << "matpolos[1][1]" << matpolos[1][1];
  double b = matpolos[0][0]*2;
  double c = matpolos[0][0]*matpolos[1][0] + matpolos[0][1]*matpolos[1][1];
   // return geraMatL(b,c);
 // qDebug() << "b" << b;
 // qDebug() << "c" << c;
    geraMatL(b,c);



    //qDebug() << "matls00" << matls[0][0];
    //qDebug() << "matls10" << matls[1][0];
    return matls;

}
