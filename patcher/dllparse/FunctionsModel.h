#pragma once

#include <QAbstractTableModel>
#include <bearparser/core.h>

#include "LibraryInfo.h"

class FunctionsModel : public QAbstractTableModel
{
	Q_OBJECT

public slots:
    void modelChanged()
    {
        //>
        this->beginResetModel();
        this->endResetModel();
        //<
    }

    void on_currentndexChanged(int index)
    {
        //>
        this->beginResetModel();
        m_libIndex = index;
        this->endResetModel();
        //<
    }

public:
    enum COLS
    {
        COL_NAME = 0,
        COUNT_COL
    };

	FunctionsModel(QObject *v_parent)
		: QAbstractTableModel(v_parent), m_LibInfos(NULL) {}

	virtual ~FunctionsModel() { }

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

public slots:
    void setLibraries(LibInfos* exes)
    {
        //>
        this->beginResetModel();
        if (this->m_LibInfos != NULL) {
            //disconnect old
            QObject::disconnect(this->m_LibInfos, SIGNAL(listChanged()), this, SLOT( on_listChanged() ) );
        }
        this->m_LibInfos = exes;

        if (this->m_LibInfos != NULL) {
            QObject::connect(this->m_LibInfos, SIGNAL(listChanged()), this, SLOT( on_listChanged() ) );
        }
        this->endResetModel();
        //<
    }

protected slots:
    void on_listChanged()
    {
        //>
        this->beginResetModel();
        this->endResetModel();
        //<
    }

protected:
    int countElements() const;

	LibInfos* m_LibInfos;
    size_t m_libIndex;
};
