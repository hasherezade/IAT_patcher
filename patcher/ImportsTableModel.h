#pragma once

#include <QAbstractTableModel>
#include <QMenu>
#include "Executables.h"

class ImportsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public slots:
    void modelChanged() { reset(); }

public:
    enum COLS
    {
        COL_THUNK = 0,
        COL_LIB,
        COL_FUNC,
        COL_REPLACEMENT,
        COUNT_COL
    };
    ImportsTableModel(QObject *v_parent)
        : QAbstractTableModel(v_parent), m_ExeHandler(NULL), m_FuncMap(NULL) {}

    virtual ~ImportsTableModel() { }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    int columnCount(const QModelIndex &parent) const { return COUNT_COL; }
    int rowCount(const QModelIndex &parent) const { return countElements(); }

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &, const QVariant &, int);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const
    {
        //no index item pointer
        return createIndex(row, column);
    }

    QModelIndex parent(const QModelIndex &index) const { return QModelIndex(); } // no parent

    offset_t selectedIndexToThunk(const QModelIndex &index) const
    {
        if (index.isValid() == false) return INVALID_ADDR;

        QString thunk0 = index.sibling(index.row(), 0).data().toString();
        bool isOk = false;
        offset_t thunkNum = thunk0.toLongLong(&isOk, 16);
        if (!isOk) return INVALID_ADDR;
        return thunkNum;
    }

public slots:
    void setExecutable(ExeHandler* exeHndl)
    {
        this->m_FuncMap = NULL;
        this->m_ExeHandler = exeHndl;

        if (this->m_ExeHandler) {
            this->m_FuncMap = &(m_ExeHandler->m_FuncMap);

        }
        reset();
    }

protected slots:
    void onExeChanged() { reset(); }

protected:

    offset_t indexToFunctionThunk(const QModelIndex &index) const
    {
        if (index.isValid() == false) {
            return INVALID_ADDR;
        }
        int impNum = index.row();
        size_t entriesCount = countElements();
        if (entriesCount == 0 || impNum >= entriesCount) return INVALID_ADDR;
        
        offset_t thunk = this->m_FuncMap->thunkAt(impNum);
        //printf("Thunk = %llx (%x)\n", thunk, impNum);
        return thunk;
    }

    ExeHandler* m_ExeHandler;
    ImportsLookup *m_FuncMap;
    int countElements() const { return (m_FuncMap == NULL) ? 0 : m_FuncMap->countImps(); }
};
