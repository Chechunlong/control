#ifndef FUZZYUI_H
#define FUZZYUI_H

#include <QDialog>

namespace Ui {
class FuzzyUI;
}

class FuzzyUI : public QDialog
{
    Q_OBJECT

public:
    explicit FuzzyUI(QWidget *parent = 0);
    ~FuzzyUI();

private:
    Ui::FuzzyUI *ui;
};

#endif // FUZZYUI_H
