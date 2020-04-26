#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>


class MyDialog : public QDialog
{
    Q_OBJECT

public:
    MyDialog(QWidget *parent = nullptr);

signals:
    void VmaxChanged(double);
    void maxTimeChanged(double);
    void dtChanged(double);
};

#endif // MYDIALOG_H
