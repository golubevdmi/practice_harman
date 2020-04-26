#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPixmap;
class QActionGroup;
class FilterThread;
class SettingsFilter;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *event = nullptr) override;
    void resizeEvent(QResizeEvent *event = nullptr) override;
private:
    Ui::MainWindow *ui;
    FilterThread *m_pFilterThread;
    QPixmap *m_pPixmap, *m_pSrcPixmap;
    SettingsFilter *m_pSettingsFilter;

    QActionGroup *filtersGroup;
    QAction *medianBlurAct;
    QAction *resizeAct;
    QAction *toGrayAct;
    QAction *gaussianAct;
    QAction *barleyBreakAct;
    QAction *scharrAct;
    QAction *sobelAct;
    QAction *resetAct;
    QAction *openAct;
    QAction *aboutAct;
    QAction *exitAct;

    void createActions();
    void updateMenu();
private slots:
    void applyFilter();
    void openImage();
    void about();
    void filterMedianBlur();
    void filterGray();
    void filterResize();
    void filterGaussian();
    void filterBarleyBreak();
    void filterScharr();
    void filterSobel();
    void updatePixmap(const QPixmap&);
    void resetImage();
    void resetSettings();
};
#endif // MAINWINDOW_H
