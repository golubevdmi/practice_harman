#include "settingsdialogs.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <QHBoxLayout>
#include <QLabel>


SettingsMedianBlur::SettingsMedianBlur(QWidget *parent)
    : SettingsFilter(parent)
    , pEditKernel(new QLineEdit("3", this))
    , pLabelKernel(new QLabel("kernel", this))
    , pButtonApply(new QPushButton("Apply", this))
    , pHLayout(new QHBoxLayout)
    , pVLayout(new QVBoxLayout(this))
{
    pHLayout->addWidget(pEditKernel);
    pHLayout->addWidget(pLabelKernel);
    pVLayout->addLayout(pHLayout);
    pVLayout->addWidget(pButtonApply);

    QObject::connect(pButtonApply, &QPushButton::clicked, [&](){
        QString sKernel = pEditKernel->text();
        if (!sKernel.isEmpty())
            emit kernelChanged(sKernel.toInt());
    });
    QObject::connect(pButtonApply, &QPushButton::clicked, this, &QDialog::close);
}


SettingsBarleyBreak::SettingsBarleyBreak(QWidget *parent)
    : SettingsFilter(parent)
    , pEditScale(new QLineEdit("4", this))
    , pLabelScale(new QLabel("scale", this))
    , pButtonApply(new QPushButton("Apply", this))
    , pHLayout(new QHBoxLayout)
    , pVLayout(new QVBoxLayout(this))
{
    pHLayout->addWidget(pEditScale);
    pHLayout->addWidget(pLabelScale);
    pVLayout->addLayout(pHLayout);
    pVLayout->addWidget(pButtonApply);

    QObject::connect(pButtonApply, &QPushButton::clicked, [&](){
        QString sScale = pEditScale->text();
        if (!sScale.isEmpty())
            emit scaleChanged(sScale.toInt());
    });
    QObject::connect(pButtonApply, &QPushButton::clicked, this, &QDialog::close);
}


SettingsResize::SettingsResize(QWidget *parent)
    : SettingsFilter(parent)
    , pEditWidth(new QLineEdit("320", this))
    , pEditHeight(new QLineEdit("180", this))
    , pLabelWidth(new QLabel("width", this))
    , pLabelHeight(new QLabel("height", this))
    , pButtonApply(new QPushButton("Apply", this))
    , pHLayout1(new QHBoxLayout)
    , pHLayout2(new QHBoxLayout)
    , pVLayout(new QVBoxLayout(this))
{
    pHLayout1->addWidget(pEditWidth);
    pHLayout1->addWidget(pLabelWidth);
    pHLayout2->addWidget(pEditHeight);
    pHLayout2->addWidget(pLabelHeight);
    pVLayout->addLayout(pHLayout1);
    pVLayout->addLayout(pHLayout2);
    pVLayout->addWidget(pButtonApply);

    QObject::connect(pButtonApply, &QPushButton::clicked, [&](){
        QString sWidth = pEditWidth->text();
        QString sHeight = pEditHeight->text();
        if (!sWidth.isEmpty() && !sHeight.isEmpty())
            emit sizeChanged(sWidth.toInt(), sHeight.toInt());
    });
    QObject::connect(pButtonApply, &QPushButton::clicked, this, &QDialog::close);
}


SettingsGaussian::SettingsGaussian(QWidget *parent)
    : SettingsFilter(parent)
    , pEditKernel(new QLineEdit("3", this))
    , pLabelKernel(new QLabel("kernel", this))
    , pButtonApply(new QPushButton("Apply", this))
    , pHLayout(new QHBoxLayout)
    , pVLayout(new QVBoxLayout(this))
{
    pHLayout->addWidget(pEditKernel);
    pHLayout->addWidget(pLabelKernel);
    pVLayout->addLayout(pHLayout);
    pVLayout->addWidget(pButtonApply);

    QObject::connect(pButtonApply, &QPushButton::clicked, [&](){
        QString sKernel = pEditKernel->text();
        if (!sKernel.isEmpty())
            emit kernelChanged(sKernel.toInt());
    });
    QObject::connect(pButtonApply, &QPushButton::clicked, this, &QDialog::close);
}


SettingsScharr::SettingsScharr(QWidget *parent)
    : SettingsFilter(parent)
    , pEditDDepth(new QLineEdit("3", this))
    , pEditDx(new QLineEdit("1", this))
    , pEditDy(new QLineEdit("0", this))
    , pLabelDDepth(new QLabel("ddepth", this))
    , pLabelDx(new QLabel("dx", this))
    , pLabelDy(new QLabel("dy", this))
    , pButtonApply(new QPushButton("Apply", this))
    , pHLayout1(new QHBoxLayout)
    , pHLayout2(new QHBoxLayout)
    , pHLayout3(new QHBoxLayout)
    , pVLayout(new QVBoxLayout(this))
{
    pHLayout1->addWidget(pEditDDepth);
    pHLayout1->addWidget(pLabelDDepth);
    pHLayout2->addWidget(pEditDx);
    pHLayout2->addWidget(pLabelDx);
    pHLayout3->addWidget(pEditDy);
    pHLayout3->addWidget(pLabelDy);
    pVLayout->addLayout(pHLayout1);
    pVLayout->addLayout(pHLayout2);
    pVLayout->addLayout(pHLayout3);
    pVLayout->addWidget(pButtonApply);

    QObject::connect(pButtonApply, &QPushButton::clicked, [&](){
        QString sDdepth = pEditDDepth->text();
        QString sDx = pEditDx->text();
        QString sDy = pEditDy->text();
        if (!sDdepth.isEmpty() && !sDx.isEmpty() && !sDy.isEmpty())
            emit settingsChanged(sDdepth.toInt(), sDx.toInt(), sDy.toInt());
    });
    QObject::connect(pButtonApply, &QPushButton::clicked, this, &QDialog::close);
}


SettingsSobel::SettingsSobel(QWidget *parent)
    : SettingsFilter(parent)
    , pEditDDepth(new QLineEdit("3", this))
    , pEditDx(new QLineEdit("1", this))
    , pEditDy(new QLineEdit("0", this))
    , pLabelDDepth(new QLabel("ddepth", this))
    , pLabelDx(new QLabel("dx", this))
    , pLabelDy(new QLabel("dy", this))
    , pButtonApply(new QPushButton("Apply", this))
    , pHLayout1(new QHBoxLayout)
    , pHLayout2(new QHBoxLayout)
    , pHLayout3(new QHBoxLayout)
    , pVLayout(new QVBoxLayout(this))
{
    pHLayout1->addWidget(pEditDDepth);
    pHLayout1->addWidget(pLabelDDepth);
    pHLayout2->addWidget(pEditDx);
    pHLayout2->addWidget(pLabelDx);
    pHLayout3->addWidget(pEditDy);
    pHLayout3->addWidget(pLabelDy);
    pVLayout->addLayout(pHLayout1);
    pVLayout->addLayout(pHLayout2);
    pVLayout->addLayout(pHLayout3);
    pVLayout->addWidget(pButtonApply);

    QObject::connect(pButtonApply, &QPushButton::clicked, [&](){
        QString sDdepth = pEditDDepth->text();
        QString sDx = pEditDx->text();
        QString sDy = pEditDy->text();
        if (!sDdepth.isEmpty() && !sDx.isEmpty() && !sDy.isEmpty())
            emit settingsChanged(sDdepth.toInt(), sDx.toInt(), sDy.toInt());
    });
    QObject::connect(pButtonApply, &QPushButton::clicked, this, &QDialog::close);
}
