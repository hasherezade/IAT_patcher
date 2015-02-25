#include "LibsModel.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QIcon>

QVariant LibsModel::headerData(int section, Qt::Orientation orientation, int role) const
	{
	if (role != Qt::DisplayRole) return QVariant();
	if (orientation != Qt::Horizontal) return QVariant();
	switch (section) {
        case COL_NAME : return "Library Name";
	}
	return QVariant();
}

Qt::ItemFlags LibsModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) return 0;
	Qt::ItemFlags f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return f;
}

bool LibsModel::setData(const QModelIndex &index, const QVariant &, int role)
{
    return false;
}

QVariant LibsModel::data(const QModelIndex &index, int role) const
{
    int elNum = index.row();
    if (elNum > countElements()) return QVariant();

    int attribute = index.column();
    if (attribute >= COUNT_COL) return QVariant();

    LibraryInfo *info = m_LibInfos->at(elNum);
	if (info == NULL) return QVariant();
    switch (role) {
        case Qt::DisplayRole: return info->getLibName();
        case Qt::ToolTipRole: return info->getFileName();
        case Qt::UserRole : return elNum;
    }
    return QVariant();
}
