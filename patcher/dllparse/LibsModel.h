#pragma once

#include <QAbstractTableModel>
#include <bearparser/core.h>

#include "LibraryInfo.h"

class LibsModel : public QAbstractTableModel
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

public:
    enum COLS
    {
        COL_NAME = 0,
        COUNT_COL
    };

	LibsModel(QObject *v_parent)
		: QAbstractTableModel(v_parent), m_LibInfos(NULL) {}

	virtual ~LibsModel() { }

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
	LibInfos* m_LibInfos;

    int countElements() const { return (m_LibInfos == NULL)? 0: m_LibInfos->size(); }
};
