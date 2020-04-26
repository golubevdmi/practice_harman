#include "mdichild.h"
#include <TableModel.h>
#include <combodelegate.h>

#include <QMainWindow>
#include <QTextEdit>
#include <QMainWindow>
#include <QWidget>
#include <QTableWidget>
#include <QTableView>
#include <QtWidgets>

MdiChild::MdiChild()
    : m_pTableModel(new TableModel)
    , m_pTableView(new QTableView)
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    connect(m_pTableModel, &TableModel::dataChanged, this, &MdiChild::documentWasModified);
}

void MdiChild::createView()
{
    m_pTableView->setModel(m_pTableModel);
    ComboDelegate *pDelegate = new ComboDelegate(m_pTableModel);

    m_pTableView->setItemDelegateForColumn(4, pDelegate);
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_pTableView->setDragEnabled(true);
    m_pTableView->setAcceptDrops(true);
    m_pTableView->setDropIndicatorShown(true);
    m_pTableView->setSortingEnabled(true);
    m_pTableView->sortByColumn(0, Qt::AscendingOrder);
    setCentralWidget(m_pTableView);
}

void MdiChild::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("document%1.txt").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");
    createView();
    this->documentWasModified();
    //this->resize(m_pTableView->size());
}

bool MdiChild::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("MDI"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QDataStream in(&file);
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QVector<Data> data;
    in >> data;
    m_pTableModel->setData(data);

    QGuiApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
    createView();
    return true;
}

bool MdiChild::save()
{
    if (isUntitled)
        return saveAs();
    else
        return saveFile(curFile);
}
bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), curFile);
    if (fileName.isEmpty())
        return false;
    return saveFile(fileName);
}
bool MdiChild::saveFile(const QString &fileName)
{
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QDataStream out(&file);
        out << m_pTableModel->getData();
//        for(int i = 0; i < m_pTableModel->rowCount(); ++i)
//        {
//            for(int j = 0; j < m_pTableModel->columnCount(); ++j)
//                out << m_pTableModel->data(m_pTableModel->index(i, j), Qt::DisplayRole).toString() << ";";
//            out << "\n";
//        }
        if (!file.commit())
        {
            errorMessage = tr("Cannot write file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    }
    else
    {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                       .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty())
    {
        QMessageBox::warning(this, tr("File "), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    return true;
}

QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}

void MdiChild::documentWasModified()
{
    setWindowModified(true);
}

bool MdiChild::maybeSave()
{
    if (!isWindowModified())
        return true;
    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("MDI"),
                                   tr("'%1' has been modified.\n"
                                      "Do you want to save your changes?")
                                   .arg(userFriendlyCurrentFile()),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);
    switch (ret)
    {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


void MdiChild::addRow()
{
    const int cnRow = m_pTableModel->rowCount();
    m_pTableModel->insertRow(cnRow);
    QModelIndex indexNew = m_pTableModel->index(cnRow, 0);
    m_pTableView->setCurrentIndex(indexNew);
    this->documentWasModified();
}
void MdiChild::deleteSelectedRows()
{
    IntSet rows;
    getSelectedRows(rows);

    auto i = rows.rbegin(), e = rows.rend();
    for (; i != e; ++ i)
        m_pTableModel->removeRow(*i);
    this->documentWasModified();
}
void MdiChild::moveSelectedRows()
{
    IntSet rows;
    getSelectedRows(rows);
    auto first = rows.begin(), last = rows.end();
    for (int n = 0; first != last; ++first, ++ n)
        m_pTableModel->moveRow(QModelIndex(), *first, QModelIndex(), n);
    this->documentWasModified();
}
void MdiChild::getSelectedRows(IntSet &rRows)
{
    QModelIndexList indexes = m_pTableView->selectionModel()->selectedIndexes();
    foreach (QModelIndex index, indexes)
        rRows.insert(index.row());
}
