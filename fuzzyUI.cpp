#include "fuzzyUI.h"
#include "ui_fuzzy.h"

FuzzyUI::FuzzyUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FuzzyUI)
{
    ui->setupUi(this);

    //ui->funcionPlot->addGraph();
    ui->funcionPlot->xAxis->setLabel("x");
    ui->funcionPlot->yAxis->setLabel("y");
}

FuzzyUI::~FuzzyUI()
{
    delete ui;
}
