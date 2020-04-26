#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QDataStream>


struct Data
{
    QString m_strName;
    int m_nValue;
    bool m_bEnabled;
    int m_nVariant;
    Data(const QString &rcName = QString(), int nValue = 0, bool bEnabled = false, int nVariant = 0)
        : m_strName(rcName)
        , m_nValue(nValue)
        , m_bEnabled(bEnabled)
        , m_nVariant(nVariant)
    {}
};
inline QDataStream &operator<<(QDataStream &stream, const Data &data)
{
    return stream << data.m_strName << data.m_nValue << data.m_bEnabled << data.m_nVariant;
}

inline QDataStream &operator>>(QDataStream &stream, Data &data)
{
    return stream >> data.m_strName >> data.m_nValue >> data.m_bEnabled >> data.m_nVariant;
}

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TableModel();
    ~TableModel();
    const QStringList &getItemNames() const;
    int rowCount(const QModelIndex &rcParent = QModelIndex()) const override;
    int columnCount(const QModelIndex &rcParent = QModelIndex()) const override;
    QVariant data(const QModelIndex &rcIndex, int nRole) const override;
    bool setData(const QModelIndex &rcIndex, const QVariant &rcValue, int nRole) override;
    void setData(const QVector<Data>& data);
    QVector<Data> getData() const;
    QVariant headerData(int nSection, Qt::Orientation nOrientation, int nRole) const override;
    Qt::DropActions supportedDropActions() const override;
    bool insertRows(int nRow, int nCount, const QModelIndex& = QModelIndex()) override;
    bool removeRows(int nRow, int nCount, const QModelIndex& = QModelIndex()) override;
    bool moveRows(const QModelIndex &rcParentSource,
                  int nRowSource, int nCount,
                  const QModelIndex &rcParentDest,
                  int nChildDest) override;
    Qt::ItemFlags flags(const QModelIndex &rcIndex) const override;
    void sort(int nColumn, Qt::SortOrder nOrder = Qt::AscendingOrder) override;

private:
    QVector <Data> m_Data;
    QStringList m_Items;
};

#endif // TABLEMODEL_H
