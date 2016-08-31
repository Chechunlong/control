#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H

#include <QDialog>

namespace Ui {
    class Configuracao;
}

class Configuracao : public QDialog
{
    Q_OBJECT

    public:
        explicit Configuracao(QWidget *parent = 0);
        ~Configuracao();

        bool getCanalLeituraVec(int i) const;

        int getCanalEscrita() const;

    private slots:
        void dataSet();

    private:
        Ui::Configuracao *ui;

        bool    canalLeituraVec[8];
        int     canalEscrita = 0;

        bool    canalLeituraVecOld[8];
};

#endif // CONFIGURACAO_H
