#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <QResizeEvent>
#include <QDialog>

#include "mydialog.h"

struct Point2d
{
    double x;
    double y;
    Point2d(double x = 0.0, double y = 0.0) :
        x(x), y(y) {}
};
struct ParticleState
{
    Point2d pos;
    double vx;
    double vy;
    double time;
    ParticleState(Point2d pos = Point2d(0.0, 0.0), double vx = 0.0, double vy = 0.0, double time = 0) :
        pos(pos), vx(vx), vy(vy), time(time) {}
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_button_start_clicked();

protected:
    void paintEvent(QPaintEvent *event = nullptr);
    void resizeEvent(QResizeEvent *event = nullptr);

private:
    Ui::MainWindow *ui;
    MyDialog *m_pDialog;
    QPixmap *m_pPixmap;
    double m_Vmax;
    double m_maxTime;
    double m_dt;
    std::vector<ParticleState> m_particleStates;

    void display(const std::vector<ParticleState> &particleStates);
    void generate(std::vector<ParticleState> &particleStates);

    void closeEvent(QCloseEvent *event);

};
#endif // MAINWINDOW_H
