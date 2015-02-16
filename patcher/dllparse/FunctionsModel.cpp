#include "FunctionsModel.h"

#include <QFileDialog>
#include <QMessageBox>

int FunctionsModel::countElements() const 
{
    if (m_LibInfos == NULL) return 0;
    LibraryInfo *info = m_LibInfos->at(m_libIndex);
    if (info == NULL) return 0;
    return info->names.size();
}

QVariant FunctionsModel::headerData(int section, Qt::Orientation orientation, int role) const
	{
	if (role != Qt::DisplayRole) return QVariant();
	if (orientation != Qt::Horizontal) return QVariant();
	switch (section) {
        case COL_NAME : return "Library Name";
	}
	return QVariant();
}

Qt::ItemFlags FunctionsModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) return 0;
	Qt::ItemFlags f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return f;
}

bool FunctionsModel::setData(const QModelIndex &index, const QVariant &, int role)
{
    return false;
}

QVariant FunctionsModel::data(const QModelIndex &index, int role) const
{
    int elNum = index.row();
    if (elNum > countElements()) return QVariant();

    int attribute = index.column();
    if (attribute >= COUNT_COL) return QVariant();

    LibraryInfo *info = m_LibInfos->at(m_libIndex);
	if (info == NULL) return QVariant();

    switch (role) {
        case Qt::DisplayRole: return info->names.at(elNum);
        case Qt::UserRole : return elNum;
    }
    return QVariant();
}
