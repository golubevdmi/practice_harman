#ifndef SETTINGSDIALOGS_H
#define SETTINGSDIALOGS_H

#include <iostream>

#include <QDialog>

class QLineEdit;
class QPushButton;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;

class SettingsFilter : public QDialog
{
public:
    SettingsFilter(QWidget *parent = nullptr) : QDialog(parent) {}
};

class SettingsMedianBlur : public SettingsFilter
{
    Q_OBJECT

public:
    SettingsMedianBlur(QWidget *parent = nullptr);
signals:
    void kernelChanged(int kernel);
private:
    QLineEdit *pEditKernel;
    QLabel *pLabelKernel;
    QPushButton *pButtonApply;
    QHBoxLayout *pHLayout;
    QVBoxLayout *pVLayout;
};

class SettingsBarleyBreak : public SettingsFilter
{
    Q_OBJECT

public:
    SettingsBarleyBreak(QWidget *parent = nullptr);
signals:
    void scaleChanged(int);
private:
    QLineEdit *pEditScale;
    QLabel *pLabelScale;
    QPushButton *pButtonApply;
    QHBoxLayout *pHLayout;
    QVBoxLayout *pVLayout;
};

class SettingsResize : public SettingsFilter
{
    Q_OBJECT

public:
    SettingsResize(QWidget *parent = nullptr);
signals:
    void sizeChanged(int, int);
private:
    QLineEdit *pEditWidth, *pEditHeight;
    QLabel *pLabelWidth, *pLabelHeight;
    QPushButton *pButtonApply;
    QHBoxLayout *pHLayout1, *pHLayout2;
    QVBoxLayout *pVLayout;
};

class SettingsScharr : public SettingsFilter
{
    Q_OBJECT

public:
    SettingsScharr(QWidget *parent = nullptr);
signals:
    void settingsChanged(int, int, int);
private:
    QLineEdit *pEditDDepth, *pEditDx, *pEditDy;
    QLabel *pLabelDDepth, *pLabelDx, *pLabelDy;
    QPushButton *pButtonApply;
    QHBoxLayout *pHLayout1, *pHLayout2, *pHLayout3;
    QVBoxLayout *pVLayout;
};

class SettingsSobel : public SettingsFilter
{
    Q_OBJECT

public:
    SettingsSobel(QWidget *parent = nullptr);
signals:
    void settingsChanged(int, int, int);
private:
    QLineEdit *pEditDDepth, *pEditDx, *pEditDy;
    QLabel *pLabelDDepth, *pLabelDx, *pLabelDy;
    QPushButton *pButtonApply;
    QHBoxLayout *pHLayout1, *pHLayout2, *pHLayout3;
    QVBoxLayout *pVLayout;
};


class SettingsGaussian : public SettingsFilter
{
    Q_OBJECT

public:
    SettingsGaussian(QWidget *parent = nullptr);
signals:
    void kernelChanged(int);
private:
    QLineEdit *pEditKernel;
    QLabel *pLabelKernel;
    QPushButton *pButtonApply;
    QHBoxLayout *pHLayout;
    QVBoxLayout *pVLayout;
};

#endif // SETTINGSDIALOGS_H
