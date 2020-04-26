#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>

#include <filterthread.h>
#include <settingsdialogs.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pFilterThread(new FilterThread)
    , m_pPixmap(new QPixmap)
    , m_pSrcPixmap(new QPixmap)
    , m_pSettingsFilter(nullptr)
{
    ui->setupUi(this);

    setWindowTitle("qt_practice3_thread");

    ui->label->setText("");
    ui->label_status->setText("loading image");
    ui->label_thread_status->setText("not work");

    createActions();
    updateMenu();

    //
    // Connects

    // Button
    connect(ui->button_apply, &QPushButton::clicked, this, &MainWindow::applyFilter);
    // Thread
    connect(m_pFilterThread, &FilterThread::send_pixmap, this, &MainWindow::updatePixmap);
    connect(m_pFilterThread, &FilterThread::processing_complete, [&](){
        ui->label_status->setText("filter applied");
    });
    connect(m_pFilterThread, &FilterThread::processing_failed, [&](){
        ui->label_status->setText("filter not applied");
    });
    connect(m_pFilterThread, &FilterThread::started, [&]() {
        ui->label_status->setText("processing...");
        ui->label_thread_status->setText("start");
    });
    connect(m_pFilterThread, &FilterThread::finished, [&]() {
        ui->label_thread_status->setText("finished");
    });
    connect(m_pFilterThread, &FilterThread::started, this, &MainWindow::updateMenu);
    connect(m_pFilterThread, &FilterThread::finished, this, &MainWindow::updateMenu);
    connect(m_pFilterThread, &FilterThread::filter_initialized, this, &MainWindow::updateMenu);
    connect(m_pFilterThread, &FilterThread::filter_not_initialized, this, &MainWindow::updateMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createActions()
{
    //
    // File menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    // Open
    const QIcon openIcon = QIcon::fromTheme("document-new", QIcon(":/resources/open.png"));
    openAct = new QAction(openIcon, tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Create a new file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openImage);
    fileMenu->addAction(openAct);

    // Reset
    const QIcon resetIcon = QIcon::fromTheme("document-new", QIcon(":/resources/reset.png"));
    resetAct = new QAction(resetIcon, tr("&Reset"), this);
    resetAct->setStatusTip(tr("Create a new file"));
    resetAct->setShortcuts(QKeySequence::Close);
    connect(resetAct, &QAction::triggered, this, &MainWindow::resetImage);
    connect(resetAct, &QAction::triggered, this, &MainWindow::updateMenu);
    fileMenu->addAction(resetAct);

    // Exit
    const QIcon exitIcon = QIcon::fromTheme("document-new", QIcon(":/resources/exit.png"));
    exitAct = new QAction(exitIcon, tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Create a new file"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);
    fileMenu->addAction(exitAct);

    //
    // Filter menu
    QMenu *filterMenu = menuBar()->addMenu(tr("&Filter"));
    filtersGroup = new QActionGroup(this);

    // Median blur
    const QIcon medianBlurIcon = QIcon::fromTheme("document-new", QIcon(":/resources/medianblur.png"));
    medianBlurAct = new QAction(medianBlurIcon, tr("&Median blur"), this);
    connect(medianBlurAct, &QAction::triggered, this, &MainWindow::filterMedianBlur);
    filtersGroup->addAction(medianBlurAct);
    filterMenu->addAction(medianBlurAct);
    // Barley break
    const QIcon barleyBreakIcon = QIcon::fromTheme("document-new", QIcon(":/resources/barleybreak.png"));
    barleyBreakAct = new QAction(barleyBreakIcon, tr("&Barley break"), this);
    connect(barleyBreakAct, &QAction::triggered, this, &MainWindow::filterBarleyBreak);
    filtersGroup->addAction(barleyBreakAct);
    filterMenu->addAction(barleyBreakAct);
    // Gaussian
    const QIcon gaussianIcon = QIcon::fromTheme("document-new", QIcon(":/resources/gaussian.png"));
    gaussianAct = new QAction(gaussianIcon, tr("&Gaussian"), this);
    connect(gaussianAct, &QAction::triggered, this, &MainWindow::filterGaussian);
    filtersGroup->addAction(gaussianAct);
    filterMenu->addAction(gaussianAct);
    // Scharr
    const QIcon scharrIcon = QIcon::fromTheme("document-new", QIcon(":/resources/scharr.png"));
    scharrAct = new QAction(scharrIcon, tr("&Scharr"), this);
    connect(scharrAct, &QAction::triggered, this, &MainWindow::filterScharr);
    filtersGroup->addAction(scharrAct);
    filterMenu->addAction(scharrAct);
    // Sobel
    const QIcon sobelIcon = QIcon::fromTheme("document-new", QIcon(":/resources/sobel.png"));
    sobelAct = new QAction(sobelIcon, tr("&Sobel"), this);
    connect(sobelAct, &QAction::triggered, this, &MainWindow::filterSobel);
    filtersGroup->addAction(sobelAct);
    filterMenu->addAction(sobelAct);
    // Resize
    const QIcon resizeIcon = QIcon::fromTheme("document-new", QIcon(":/resources/resize.png"));
    resizeAct = new QAction(resizeIcon, tr("&Resize"), this);
    connect(resizeAct, &QAction::triggered, this, &MainWindow::filterResize);
    filtersGroup->addAction(resizeAct);
    filterMenu->addAction(resizeAct);
    // To gray
    const QIcon toGrayIcon = QIcon::fromTheme("document-new", QIcon(":/resources/togray.png"));
    toGrayAct = new QAction(toGrayIcon, tr("&To gray"), this);
    connect(toGrayAct, &QAction::triggered, this, &MainWindow::filterGray);
    filtersGroup->addAction(toGrayAct);
    filterMenu->addAction(toGrayAct);

    // Checkable
    auto actions = filtersGroup->actions();
    if (!actions.empty())
        for(auto &action : actions)
            action->setCheckable(true);


    //
    // Help menu
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    // About
    const QIcon aboutIcon = QIcon::fromTheme("document-new", QIcon(":/resources/about.png"));
    aboutAct = new QAction(aboutIcon, tr("&About"), this);
    aboutAct->setShortcuts(QKeySequence::HelpContents);
    aboutAct->setStatusTip(tr("Create a new file"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
    helpMenu->addAction(aboutAct);
}


void MainWindow::updateMenu()
{
    assert(m_pFilterThread);
    bool bRunning = m_pFilterThread->isRunning();
    bool bEnable = !m_pPixmap->isNull() && !bRunning ? true : false;

    // Actions file
    openAct->setEnabled(!bRunning);
    resetAct->setEnabled(bEnable);
    // Actions filter
    filtersGroup->setEnabled(bEnable);
    // Buttons
    ui->button_apply->setEnabled(m_pFilterThread->isActive() && bEnable);
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter *pPainter = new QPainter(this);
    if (m_pPixmap)
        pPainter->drawPixmap(ui->label->rect(), *m_pPixmap);
    else
        pPainter->fillRect(ui->label->rect(), Qt::white);
    pPainter->end();
    delete pPainter;
}


void MainWindow::resizeEvent(QResizeEvent *event) {}


void MainWindow::applyFilter()
{
    assert(m_pFilterThread);
    if (m_pPixmap && m_pFilterThread->isActive())
    {
        m_pFilterThread->setData(*m_pPixmap);
        m_pFilterThread->start();
        ui->label_thread_status->setText("run");
    }
    else
    {
        QMessageBox::warning(this, "Error", "Pixmap not found");
        ui->label_status->setText("filter not applied");
    }
}
void MainWindow::filterMedianBlur()
{
    resetSettings();
    m_pSettingsFilter = new SettingsMedianBlur(this);
    QObject::connect(dynamic_cast<SettingsMedianBlur*>(m_pSettingsFilter), &SettingsMedianBlur::kernelChanged,
                     m_pFilterThread, &FilterThread::initMedianBlur);
    m_pSettingsFilter->show();
}
void MainWindow::filterGray()
{
    resetSettings();
    m_pFilterThread->initGray();
}
void MainWindow::filterResize()
{
    resetSettings();
    m_pSettingsFilter = new SettingsResize(this);
    QObject::connect(dynamic_cast<SettingsResize*>(m_pSettingsFilter), &SettingsResize::sizeChanged,
                     m_pFilterThread, &FilterThread::initResize);
    m_pSettingsFilter->show();
}
void MainWindow::filterGaussian()
{
    resetSettings();
    m_pSettingsFilter = new SettingsGaussian(this);
    QObject::connect(dynamic_cast<SettingsGaussian*>(m_pSettingsFilter), &SettingsGaussian::kernelChanged,
                     m_pFilterThread, &FilterThread::initGaussian);
    m_pSettingsFilter->show();
}
void MainWindow::filterBarleyBreak()
{
    resetSettings();
    m_pSettingsFilter = new SettingsBarleyBreak(this);
    QObject::connect(dynamic_cast<SettingsBarleyBreak*>(m_pSettingsFilter), &SettingsBarleyBreak::scaleChanged,
                     m_pFilterThread, &FilterThread::initBarleyBreak);
    m_pSettingsFilter->show();
}
void MainWindow::filterScharr()
{
    resetSettings();
    m_pSettingsFilter = new SettingsScharr(this);
    QObject::connect(dynamic_cast<SettingsScharr*>(m_pSettingsFilter), &SettingsScharr::settingsChanged,
                     m_pFilterThread, &FilterThread::initScharr);
    m_pSettingsFilter->show();
}
void MainWindow::filterSobel()
{
    resetSettings();
    m_pSettingsFilter = new SettingsSobel(this);
    QObject::connect(dynamic_cast<SettingsSobel*>(m_pSettingsFilter), &SettingsSobel::settingsChanged,
                     m_pFilterThread, &FilterThread::initSobel);
    m_pSettingsFilter->show();
}

void MainWindow::resetImage()
{
    *m_pPixmap = m_pSrcPixmap->copy();
    ui->label_status->setText("reset");
    this->update();
}
void MainWindow::resetSettings()
{
    if (m_pSettingsFilter)
    {
        delete m_pSettingsFilter;
        m_pSettingsFilter = nullptr;
    }
}

void MainWindow::updatePixmap(const QPixmap &pixmap)
{
    *m_pPixmap = pixmap.copy();
    this->update();
}

void MainWindow::openImage()
{
    QString fileName
            = QFileDialog::getOpenFileName(this, tr("Open image"),
                                           QDir::currentPath(),
                                           tr("File (*.jpg *png *bmp)"));
    if (!fileName.isEmpty())
    {
        QPixmap *pNewPixmap = new QPixmap(fileName);
        if (pNewPixmap->isNull())
        {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
        *m_pSrcPixmap = pNewPixmap->copy();
        updatePixmap(*m_pSrcPixmap);
        ui->label_status->setText("image loaded");
        updateMenu();
        this->update();
    }
}
void MainWindow::about()
{
    QMessageBox::about(this, tr("About qt_practice3_thread"),
                       tr("The <b>qt_practice3_thread</b> example demonstrates how to use thread "
                          "interface applications using Qt."));
}
