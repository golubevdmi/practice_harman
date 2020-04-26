#ifndef MDICHILD_H
#define MDICHILD_H

#include <set>

#include <QMainWindow>

class TableModel;
class QTableView;

class MdiChild : public QMainWindow
{
    Q_OBJECT

public:
    MdiChild();

    void newFile();
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void addRow();
    void deleteSelectedRows();
    void moveSelectedRows();

private slots:
    void documentWasModified();

private:
    using IntSet = std::set<int>;

    QString curFile;
    bool isUntitled;
    TableModel *m_pTableModel;
    QTableView *m_pTableView;

    void createView();
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void getSelectedRows(IntSet &rRows);
};

#endif
