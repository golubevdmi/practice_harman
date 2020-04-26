#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MdiChild;
class QAction;
class QMdiSubWindow;
class QMdiArea;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event = nullptr);

private slots:
    void updateRecentFileActions();
    void openRecentFile();
    void open();
    void save();
    void addRow();
    void deleteRows();
    void moveRows();
    void saveAs();
    void newFile();
    void about();
    void switchLayoutDirection();

private:
    enum { MaxRecentFiles = 5 };
    Ui::MainWindow *ui;
    QMdiArea *m_pMdiArea;
    // File actions
    QAction *newAct;
    QAction *saveAct;
    QAction *saveAsAct;
    // Table active actions
    QAction *addRowAct;
    QAction *deleteSelectedRowsAct;
    QAction *moveSelectedRowsAct;
    // Recent actions
    QAction *recentFileActs[MaxRecentFiles];
    QAction *recentFileSeparator;
    QAction *recentFileSubMenuAct;
    // Window menu and actions
    QMenu *windowMenu;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *windowMenuSeparatorAct;

    static bool hasRecentFiles();
    void prependToRecentFiles(const QString &fileName);
    void setRecentFilesVisible(bool visible);
    void writeSettings();
    void readSettings();

    void createActions();
    MdiChild* createMdiChild();
    MdiChild* activeMdiChild() const;
    QMdiSubWindow* findMdiChild(const QString &fileName) const;
    void updateMenus();
    void updateWindowMenu();
    bool openFile(const QString &fileName);
    bool loadFile(const QString &fileName);
};

#endif
