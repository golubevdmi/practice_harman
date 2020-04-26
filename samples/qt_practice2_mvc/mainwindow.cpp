#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <TableModel.h>
#include <combodelegate.h>
#include <mdichild.h>

#include <QSplitter>
#include <QFileSystemModel>
#include <QTableView>
#include <QTreeView>
#include <QListView>
#include <QLabel>
#include <QCloseEvent>
#include <QItemSelectionModel>
#include <QCloseEvent>
#include <QSettings>
#include <QScreen>
#include <QMessageBox>
#include <QAction>
#include <QFileDialog>
#include <QMdiArea>
#include <QMdiSubWindow>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pMdiArea(new QMdiArea)
{
    ui->setupUi(this);
    setWindowTitle(tr("qt_practice2_mvc"));

    m_pMdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pMdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(m_pMdiArea);
    connect(m_pMdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::updateMenus);
    createActions();
    updateMenus();
    readSettings();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_pMdiArea->closeAllSubWindows();
    if (m_pMdiArea->currentSubWindow())
        event->ignore();
    else
    {
        writeSettings();
        event->accept();
    }
}


MdiChild *MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild;
    m_pMdiArea->addSubWindow(child);
    return child;
}
MdiChild *MainWindow::activeMdiChild() const
{
    if (QMdiSubWindow *activeSubWindow = m_pMdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return nullptr;
}
QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName) const
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    const QList<QMdiSubWindow *> subWindows = m_pMdiArea->subWindowList();
    for (QMdiSubWindow *window : subWindows)
    {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return nullptr;
}


static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
static inline QString fileKey() { return QStringLiteral("file"); }

//
// Recent files (settings)
static QStringList readRecentFiles(QSettings &settings)
{
    QStringList result;
    const int count = settings.beginReadArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        result.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    return result;
}
static void writeRecentFiles(const QStringList &files, QSettings &settings)
{
    const int count = files.size();
    settings.beginWriteArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), files.at(i));
    }
    settings.endArray();
}

bool MainWindow::hasRecentFiles()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = settings.beginReadArray(recentFilesKey());
    settings.endArray();
    return count > 0;
}
void MainWindow::prependToRecentFiles(const QString &fileName)
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList oldRecentFiles = readRecentFiles(settings);
    QStringList recentFiles = oldRecentFiles;
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    if (oldRecentFiles != recentFiles)
        writeRecentFiles(recentFiles, settings);

    setRecentFilesVisible(!recentFiles.isEmpty());
}
void MainWindow::setRecentFilesVisible(bool visible)
{
    recentFileSubMenuAct->setVisible(visible);
    recentFileSeparator->setVisible(visible);
}
void MainWindow::updateRecentFileActions()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(int(MaxRecentFiles), recentFiles.size());
    int i = 0;
    for ( ; i < count; ++i) {
        const QString fileName = QFileInfo(recentFiles.at(i)).fileName();
        recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
        recentFileActs[i]->setData(recentFiles.at(i));
        recentFileActs[i]->setVisible(true);
    }
    for ( ; i < MaxRecentFiles; ++i)
        recentFileActs[i]->setVisible(false);
}
void MainWindow::openRecentFile()
{
    if (const QAction *action = qobject_cast<const QAction *>(sender()))
        openFile(action->data().toString());
}


//
// Menu bar
//

void MainWindow::createActions()
{
    //
    // File menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    // New
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/resources/new.png"));
    newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);

    // Open
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/resources/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);

    // Save
    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/resources/save.png"));
    saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);

    // Save as...
    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    saveAsAct = new QAction(saveAsIcon, tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
    fileMenu->addAction(saveAsAct);

    fileMenu->addSeparator();

    //
    // Recent menu
    QMenu *recentMenu = fileMenu->addMenu(tr("Recent..."));
    connect(recentMenu, &QMenu::aboutToShow, this, &MainWindow::updateRecentFileActions);
    recentFileSubMenuAct = recentMenu->menuAction();

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = recentMenu->addAction(QString(), this, &MainWindow::openRecentFile);
        recentFileActs[i]->setVisible(false);
    }

    recentFileSeparator = fileMenu->addSeparator();

    setRecentFilesVisible(MainWindow::hasRecentFiles());

    // Switch menu
    fileMenu->addAction(tr("Switch layout direction"), this, &MainWindow::switchLayoutDirection);

    fileMenu->addSeparator();

    // Exit
    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), qApp, &QApplication::closeAllWindows);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    fileMenu->addAction(exitAct);

    //
    // Edit menu
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    // Add row
    const QIcon addRowIcon = QIcon::fromTheme("document-new", QIcon(":/resources/addRow.png"));
    addRowAct = new QAction(addRowIcon, tr("&Add row"), this);
    addRowAct->setShortcuts(QKeySequence::AddTab);
    addRowAct->setStatusTip(tr("Add row"));
    connect(addRowAct, &QAction::triggered, this, &MainWindow::addRow);
    editMenu->addAction(addRowAct);

    // Delete selected rows
    const QIcon deleteSelectedRowsActIcon = QIcon::fromTheme("document-new", QIcon(":/resources/deleterow.png"));
    deleteSelectedRowsAct = new QAction(deleteSelectedRowsActIcon, tr("&Delete rows"), this);
    deleteSelectedRowsAct->setShortcuts(QKeySequence::Delete);
    deleteSelectedRowsAct->setStatusTip(tr("Delete rows"));
    connect(deleteSelectedRowsAct, &QAction::triggered, this, &MainWindow::deleteRows);
    editMenu->addAction(deleteSelectedRowsAct);

    // Move selected rows
    const QIcon moveSelectedRowsIcon = QIcon::fromTheme("document-new", QIcon(":/resources/moverow.png"));
    moveSelectedRowsAct = new QAction(moveSelectedRowsIcon, tr("&Move rows"), this);
    moveSelectedRowsAct->setShortcuts(QKeySequence::MoveToNextLine);
    moveSelectedRowsAct->setStatusTip(tr("Move rows"));
    connect(moveSelectedRowsAct, &QAction::triggered, this, &MainWindow::moveRows);
    editMenu->addAction(moveSelectedRowsAct);

    //
    // Window menu
    windowMenu = menuBar()->addMenu(tr("&Window"));
    connect(windowMenu, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenu);

    // Close
    const QIcon closeIcon = QIcon::fromTheme("document-new", QIcon(":/resources/close.png"));
    closeAct = new QAction(closeIcon, tr("&Close"), this);
    closeAct->setShortcuts(QKeySequence::MoveToNextLine);
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, &QAction::triggered,
            m_pMdiArea, &QMdiArea::closeActiveSubWindow);

    // Close all
    closeAllAct = new QAction(tr("&Close All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, &QAction::triggered, m_pMdiArea, &QMdiArea::closeAllSubWindows);

    // Tile
    const QIcon tileIcon = QIcon::fromTheme("document-new", QIcon(":/resources/tile.png"));
    tileAct = new QAction(tileIcon, tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, &QAction::triggered, m_pMdiArea, &QMdiArea::tileSubWindows);

    // Cascade
    const QIcon cascadeIcon = QIcon::fromTheme("document-new", QIcon(":/resources/cascade.png"));
    cascadeAct = new QAction(cascadeIcon, tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, &QAction::triggered, m_pMdiArea, &QMdiArea::cascadeSubWindows);

    // Next
    const QIcon nextIcon = QIcon::fromTheme("document-new", QIcon(":/resources/next.png"));
    nextAct = new QAction(nextIcon, tr("&Next"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, &QAction::triggered, m_pMdiArea, &QMdiArea::activateNextSubWindow);

    // Previous
    const QIcon previousIcon = QIcon::fromTheme("document-new", QIcon(":/resources/previous.png"));
    previousAct = new QAction(previousIcon, tr("&Previous"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
    connect(previousAct, &QAction::triggered, m_pMdiArea, &QMdiArea::activatePreviousSubWindow);

    // Window menu separator
    windowMenuSeparatorAct = new QAction(this);
    windowMenuSeparatorAct->setSeparator(true);
    updateWindowMenu();

    menuBar()->addSeparator();

    // Help menu
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    // About
    const QIcon aboutIcon = QIcon::fromTheme("document-new", QIcon(":/resources/about.png"));
    QAction *aboutAct = helpMenu->addAction(aboutIcon, tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));
}
//
// File
void MainWindow::save()
{
    if (activeMdiChild() && activeMdiChild()->save())
        statusBar()->showMessage(tr("File saved"), 2000);
}
void MainWindow::saveAs()
{
    MdiChild *child = activeMdiChild();
    if (child && child->saveAs())
    {
        statusBar()->showMessage(tr("File saved"), 2000);
        MainWindow::prependToRecentFiles(child->currentFile());
    }
}
void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
    }
}
void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

void MainWindow::open()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            openFile(fileName);
}
bool MainWindow::openFile(const QString &fileName)
{
    if (QMdiSubWindow *existing = findMdiChild(fileName))
    {
        m_pMdiArea->setActiveSubWindow(existing);
        return true;
    }
    const bool succeeded = loadFile(fileName);
    if (succeeded)
        statusBar()->showMessage(tr("File loaded"), 2000);
    return succeeded;
}
bool MainWindow::loadFile(const QString &fileName)
{
    MdiChild *child = createMdiChild();
    const bool succeeded = child->loadFile(fileName);
    if (succeeded)
        child->show();
    else
        child->close();
    MainWindow::prependToRecentFiles(fileName);
    return succeeded;
}

void MainWindow::newFile()
{
    MdiChild *child = createMdiChild();
    child->newFile();
    child->showMaximized();
}
//
// Edit
void MainWindow::addRow()
{
    if (activeMdiChild())
        activeMdiChild()->addRow();
}
void MainWindow::deleteRows()
{
    if (activeMdiChild())
        activeMdiChild()->deleteSelectedRows();
}
void MainWindow::moveRows()
{
    if (activeMdiChild())
        activeMdiChild()->moveSelectedRows();
}
//
// Help
void MainWindow::about()
{
    QMessageBox::about(this, tr("About qt_practice2_mvc"),
                       tr("The <b>qt_practice2_mvc</b> example demonstrates how to write multiple "
                          "table interface applications using Qt."));
}

void MainWindow::updateMenus()
{
    bool hasMdiChild = (activeMdiChild() != nullptr);

    // File
    saveAct->setEnabled(hasMdiChild);
    saveAsAct->setEnabled(hasMdiChild);

    // Window
    closeAct->setEnabled(hasMdiChild);
    closeAllAct->setEnabled(hasMdiChild);
    tileAct->setEnabled(hasMdiChild);
    cascadeAct->setEnabled(hasMdiChild);
    nextAct->setEnabled(hasMdiChild);
    previousAct->setEnabled(hasMdiChild);
    windowMenuSeparatorAct->setVisible(hasMdiChild);

    // Edit
    addRowAct->setEnabled(hasMdiChild);
    deleteSelectedRowsAct->setEnabled(hasMdiChild);
    moveSelectedRowsAct->setEnabled(hasMdiChild);
}
void MainWindow::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(windowMenuSeparatorAct);

    QList<QMdiSubWindow *> windows = m_pMdiArea->subWindowList();
    windowMenuSeparatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i)
    {
        QMdiSubWindow *mdiSubWindow = windows.at(i);
        MdiChild *child = qobject_cast<MdiChild *>(mdiSubWindow->widget());

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1)
                    .arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i + 1)
                    .arg(child->userFriendlyCurrentFile());
        }
        QAction *action = windowMenu->addAction(text, mdiSubWindow, [this, mdiSubWindow]() {
            m_pMdiArea->setActiveSubWindow(mdiSubWindow);
        });
        action->setCheckable(true);
        action ->setChecked(child == activeMdiChild());
    }
}


void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        QGuiApplication::setLayoutDirection(Qt::RightToLeft);
    else
        QGuiApplication::setLayoutDirection(Qt::LeftToRight);
}
