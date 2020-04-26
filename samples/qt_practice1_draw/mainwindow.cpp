#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pDialog(new MyDialog(this))
    , m_pPixmap(new QPixmap())
    , m_Vmax(100.00)
    , m_maxTime(2.00)
    , m_dt(0.01)
{
    ui->setupUi(this);
    srand(time(NULL));
    this->setWindowTitle("Brownian motion");
    this->resize(600, 600);

    // Connect Actions
    QObject::connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);
    QObject::connect(ui->actionVariables, &QAction::triggered, ui->button_settings, &QPushButton::clicked);
    QObject::connect(ui->actionabout_program, &QAction::triggered, [this]{
        QDialog *pDialog = new QDialog(this);
        pDialog->setFixedSize(300, 40);
        QLabel *pLabel = new QLabel("Qt Practice 1. Brownian motion.", pDialog);
        pDialog->show();
    });

    QObject::connect(ui->button_settings, &QPushButton::clicked, m_pDialog, &QDialog::show);
    QObject::connect(ui->button_reset, &QPushButton::clicked, [this]{
        this->m_particleStates.clear();
        this->update();
    });
    QObject::connect(m_pDialog, &MyDialog::VmaxChanged, [this](double Vmax){
        this->m_Vmax = Vmax;
        std::cout << "Vmax: " << Vmax << std::endl;
    });
    QObject::connect(m_pDialog, &MyDialog::maxTimeChanged, [this](double maxTime){
        this->m_maxTime = maxTime;
        std::cout << "Max time: " << maxTime << std::endl;
    });
    QObject::connect(m_pDialog, &MyDialog::dtChanged, [this](double m_dt){
        this->m_dt = m_dt;
        std::cout << "m_dt: " << m_dt << std::endl;
    });
}


void MainWindow::display(const std::vector<ParticleState> &particleStates)
{
    if (!particleStates.empty())
    {
        unsigned radius = 8;
        QPainter *pPainter = new QPainter(m_pPixmap);
        pPainter->setPen(Qt::black);
        if(particleStates.size() > 2)
            for (auto it1 = particleStates.begin(), it2 = ++particleStates.begin(); it2 != --particleStates.end(); ++it1, ++it2)
            {
                pPainter->drawEllipse(
                            it2->pos.x - radius / 2,
                            it2->pos.y - radius / 2,
                            radius, radius);
                pPainter->drawLine(it1->pos.x, it1->pos.y, it2->pos.x, it2->pos.y);
            }

        // Begin
        int ext = 10;   // shift drawed text
        pPainter->setPen(Qt::blue);
        auto begin = particleStates.begin();
        QString text = "begin(" + QString::number(begin->pos.x) + " x " + QString::number(begin->pos.y) + ")"
                + ", vx:" + QString::number(begin->vx) + ", vy: " + QString::number(begin->vy)
                + ", time:" + QString::number(begin->time);
        pPainter->drawText(begin->pos.x, begin->pos.y + ext, text);
        pPainter->drawEllipse(
                    begin->pos.x - radius / 2,
                    begin->pos.y - radius / 2,
                    radius, radius);

        // End
        auto end = --particleStates.end();
        text = "end(" + QString::number(end->pos.x) + " x " + QString::number(end->pos.y) + ")"
                + ", vx:" + QString::number(end->vx) + ", vy: " + QString::number(end->vy)
                + ", time:" + QString::number(end->time);
        pPainter->drawText(end->pos.x - ext, end->pos.y - ext, text);
        pPainter->drawEllipse(
                    end->pos.x - radius / 2,
                    end->pos.y - radius / 2,
                    radius, radius);

        if(particleStates.size() > 1)
        {
            auto penultimate = end - 1;
            pPainter->drawLine(penultimate->pos.x, penultimate->pos.y, end->pos.x, end->pos.y);
        }
        pPainter->end();
    }
}


void MainWindow::generate(std::vector<ParticleState> &particleStates)
{
    if (!particleStates.empty())
    {
        auto begin = particleStates.begin();
        begin->pos = Point2d(m_pPixmap->size().width() / 2, m_pPixmap->size().height() / 2);
        for(auto it1 = particleStates.begin(), it2 = ++particleStates.begin(); it2 != particleStates.end(); ++it1, ++it2)
        {
            it2->pos.x = it1->pos.x + it2->vx;
            it2->pos.y = it1->pos.y + it2->vy;

            if(it2->pos.x > this->size().width())
                it2->pos.x -= this->size().width();
            if(it2->pos.x < 0.0)
                it2->pos.x += this->size().width();
            if(it2->pos.y > this->size().width())
                it2->pos.y -= this->size().height();
            if(it2->pos.y < 0)
                it2->pos.y += this->size().height();
        }
    }
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    m_pPixmap->fill(Qt::GlobalColor::gray);
    display(m_particleStates);
    QPainter *pPainter = new QPainter(this);
    pPainter->drawPixmap(0, 0, *m_pPixmap);
    pPainter->end();
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    *m_pPixmap = QPixmap(event->size());
    generate(m_particleStates);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    m_particleStates.clear();
    m_pDialog->close();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_button_start_clicked()
{
    if(m_maxTime > 0.00 && m_dt > 0.00)
    {
        ui->button_reset->click();
        ParticleState beginState(Point2d(this->size().width() / 2, this->size().height() / 2));
        m_particleStates.push_back(beginState);
        for (double currentTime = m_dt; currentTime <= m_maxTime; currentTime += m_dt)
        {
            ParticleState state;
            // Calculate speed
            state.vx = (double)(rand() % (int)m_Vmax) - m_Vmax / 2.0;
            state.vy = (double)(rand() % (int)m_Vmax) - m_Vmax / 2.0;
            // Calculate new position
            auto newPos = state.pos;
            newPos.x = m_particleStates.back().pos.x + state.vx;
            newPos.y = m_particleStates.back().pos.y + state.vy;

            if(newPos.x > this->size().width())
                newPos.x -= this->size().width();
            if(newPos.x < 0.0)
                newPos.x += this->size().width();
            if(newPos.y > this->size().width())
                newPos.y -= this->size().height();
            if(newPos.y < 0)
                newPos.y += this->size().height();

            state.pos = newPos;
            state.time = currentTime;
            m_particleStates.push_back(state);
        }
        this->update();
    }
}
