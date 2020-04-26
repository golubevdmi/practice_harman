#include "mydialog.h"

MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle("Settings");

    QLabel *pLabelVmax = new QLabel("Vmax", this);
    QLineEdit *pLineEditVmax = new QLineEdit(this);
    QLabel *pLabelMaxTime = new QLabel("Max time", this);
    QLineEdit *pLineEditMaxTime = new QLineEdit(this);
    QLabel *pLabelDt = new QLabel("Dt", this);
    QLineEdit *pLineEditDt = new QLineEdit(this);

    QPushButton *pButtonOK = new QPushButton("&OK", this);
    QPushButton *pButtonClose = new QPushButton("&Close", this);

    //
    // Variables
    //

    // Vmax
    QHBoxLayout *pHEditLayoutVmax = new QHBoxLayout;
    pHEditLayoutVmax->addWidget(pLabelVmax);
    pHEditLayoutVmax->addWidget(pLineEditVmax);
    // Max time
    QHBoxLayout *pHEditLayoutMaxTime = new QHBoxLayout;
    pHEditLayoutMaxTime->addWidget(pLabelMaxTime);
    pHEditLayoutMaxTime->addWidget(pLineEditMaxTime);
    // dt
    QHBoxLayout *pHEditLayoutDt = new QHBoxLayout;
    pHEditLayoutDt->addWidget(pLabelDt);
    pHEditLayoutDt->addWidget(pLineEditDt);
    // Add variables on layout
    QVBoxLayout *pVLayoutVariables = new QVBoxLayout;
    pVLayoutVariables->addLayout(pHEditLayoutVmax);
    pVLayoutVariables->addLayout(pHEditLayoutMaxTime);
    pVLayoutVariables->addLayout(pHEditLayoutDt);

    // Buttons layout
    QVBoxLayout *pVButtonsLayout = new QVBoxLayout;
    pVButtonsLayout->addWidget(pButtonOK);
    pVButtonsLayout->addWidget(pButtonClose);

    // Global Layout
    QHBoxLayout *pHLayout = new QHBoxLayout(this);
    pHLayout->addLayout(pVLayoutVariables);
    pHLayout->addLayout(pVButtonsLayout);

    // Default value
    pLineEditVmax->setText(QString::number(20.0));
    pLineEditMaxTime->setText(QString::number(2.0));
    pLineEditDt->setText(QString::number(0.01));

    QObject::connect(pButtonClose, &QPushButton::clicked, this, &QDialog::close);
    QObject::connect(pButtonOK, &QPushButton::clicked, [pButtonClose, pLineEditVmax, pLineEditMaxTime, pLineEditDt, this]{
        if(!pLineEditVmax->text().isEmpty())
            emit VmaxChanged(pLineEditVmax->text().toDouble());
        if(!pLineEditMaxTime->text().isEmpty())
            emit maxTimeChanged(pLineEditMaxTime->text().toDouble());
        if(!pLineEditDt->text().isEmpty())
            emit dtChanged(pLineEditDt->text().toDouble());
        pButtonClose->click();
    });
}
