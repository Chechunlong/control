#ifndef CONECTAR_H
#define CONECTAR_H

#include <QDialog>

#define PORT_S 20081
#define IP_S "10.13.99.69"

namespace Ui {
class Conectar;
}

class Conectar : public QDialog
{
    Q_OBJECT

    public:
        explicit Conectar(QWidget *parent = 0);
        ~Conectar();

        QString getIpAdress() const;

        int getPort() const;

    private slots:
        void setData();

    private:
        Ui::Conectar *ui;

        QString ipAdress;
        int     port;
};

#endif // CONECTAR_H
