#include "TableModel.h"


TableModel::TableModel()
{
    m_Items << QString("None")
            << QString("One")
            << QString("Two");
}
TableModel::~TableModel()
{
    m_Data.clear();
}

const QStringList &TableModel::getItemNames() const { return m_Items; }
int TableModel::rowCount(const QModelIndex &parent) const { return parent.isValid() ? 0 : m_Data.size(); }
int TableModel::columnCount(const QModelIndex &parent) const { return parent.isValid() ? 0 : 4; }

QVariant TableModel::data(const QModelIndex &rcIndex, int nRole) const
{
    if (rcIndex.isValid()
            && rcIndex.row() < rowCount()
            && rcIndex.column() < columnCount())
    {
        const Data &rcData = m_Data[rcIndex.row()];
        switch (nRole)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            switch (rcIndex.column())
            {
            case 0:
                return rcData.m_strName;
            case 1:
                return rcData.m_nValue;
            case 2:
                return rcData.m_bEnabled;
            case 3:
                if (nRole == Qt::DisplayRole)
                    return m_Items[rcData.m_nVariant];
                else
                    return rcData.m_nVariant;
            }
        case Qt::CheckStateRole:
            switch (rcIndex.column())
            {
            case 2:
                return rcData.m_bEnabled ? Qt::Checked : Qt::Unchecked;
            }
        } // switch (nRole)
    } // if (rcIndex.isValid() && ...)

    return QString();
}

bool TableModel::setData(const QModelIndex &rcIndex, const QVariant &rcValue, int nRole)
{
    if (rcIndex.isValid()
            && rcIndex.row() < rowCount()
            && rcIndex.column() < columnCount()
            && (nRole == Qt::DisplayRole
                || nRole == Qt::EditRole
                || nRole == Qt::CheckStateRole)
            )
    {
        Data &rData = m_Data[rcIndex.row()];
        switch (nRole)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            switch (rcIndex.column())
            {
            case 0:
                rData.m_strName = rcValue.toString();
                break;
            case 1:
                rData.m_nValue = rcValue.toInt();
                break;
            case 3:
                rData.m_nVariant = rcValue.toInt();
                break;
            default:
                return false;
            } // switch (rcIndex.column())
            break;
        case Qt::CheckStateRole:
            switch (rcIndex.column())
            {
            case 2:
                rData.m_bEnabled = (rcValue.toInt() == Qt::Checked);
                break;
            default:
                return false;
            }
        }
        emit dataChanged(rcIndex, rcIndex);
        return true;
    }
    return false;
}
void TableModel::setData(const QVector<Data>& data)
{
    m_Data = data;
}

QVariant TableModel::headerData(int nSection, Qt::Orientation nOrientation, int nRole) const
{
    if (nRole == Qt::DisplayRole && nOrientation == Qt::Horizontal)
    {
        switch (nSection)
        {
        case 0:
            return QString("Name");
        case 1:
            return QString("Number");
        case 2:
            return QString("Enabled");
        case 3:
            return QString("Enumeration");
        }
    }

    return QAbstractTableModel::headerData(nSection, nOrientation, nRole);
}
Qt::DropActions TableModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}
bool TableModel::insertRows(int nRow, int nCount, const QModelIndex &rcParent)
{
    if (nRow < 0) nRow = rowCount();

    beginInsertRows(QModelIndex(), nRow, nRow + nCount - 1);

    for (int i = 0; i < nCount; ++ i)
        m_Data.insert(nRow, Data());

    endInsertRows();

    return true;
}
bool TableModel::removeRows(int nRow, int nCount, const QModelIndex &rcParent)
{
    beginRemoveRows(QModelIndex(), nRow, nRow + nCount - 1);

    for (int i = 0; i < nCount; ++ i)
        m_Data.removeAt(nRow);

    endRemoveRows();

    return true;
}
bool TableModel::moveRows(const QModelIndex &rcParentSource,
                          int nRowSource, int nCount,
                          const QModelIndex &rcParentDest, int nChildDest)
{
    beginMoveRows(QModelIndex(), nRowSource, nRowSource + nCount - 1,
                  QModelIndex(), nChildDest + nCount - 1);
    for (int i = 0; i < nCount; ++ i)
        m_Data.move(nRowSource, nChildDest + i);
    endMoveRows();
    return true;
}
Qt::ItemFlags TableModel::flags(const QModelIndex &rcIndex) const
{
    Qt::ItemFlags nReturn = QAbstractTableModel::flags(rcIndex);

    nReturn |= Qt::ItemIsDropEnabled;
    if (rcIndex.isValid())
    {
        nReturn |= Qt::ItemIsDragEnabled;
        if (rcIndex.column() == 2)
            nReturn |= Qt::ItemIsUserCheckable;
        else
            nReturn |= Qt::ItemIsEditable;
    }
    //
    return nReturn;
}
void TableModel::sort(int nColumn, Qt::SortOrder nOrder)
{
    emit layoutAboutToBeChanged();
    //
    std::sort(
                m_Data.begin(), m_Data.end(),
                [=] (const Data &rcData1, const Data &rcData2) {
        switch (nColumn)
        {
        case 0:
            switch (nOrder)
            {
            case Qt::AscendingOrder:
                return (rcData1.m_strName < rcData2.m_strName);
            case Qt::DescendingOrder:
                return (rcData1.m_strName > rcData2.m_strName);
            }
        case 1:
            break;
        } // switch (nColumn)
    }); // std::sort()

    emit layoutChanged();
}


QVector<Data> TableModel::getData() const
{
    return m_Data;
}
