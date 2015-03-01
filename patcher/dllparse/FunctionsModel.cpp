#include "FunctionsModel.h"

#include <QFileDialog>
#include <QMessageBox>

int FunctionsModel::countElements() const 
{
    if (m_LibInfos == NULL) return 0;
    LibraryInfo *info = m_LibInfos->at(m_libIndex);
    if (info == NULL) return 0;
    return info->getFunctionsCount();
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
	if (!index.isValid()) return Qt::NoItemFlags;
    int elNum = index.row();
    LibraryInfo *info = m_LibInfos->at(m_libIndex);
	if (info == NULL) return Qt::NoItemFlags;

    if (info->isFunctionNamed(elNum))
	    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return Qt::NoItemFlags;
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
        case Qt::DisplayRole: return info->getFuncNameAt(elNum);
        case Qt::UserRole : return elNum;
        case Qt::ToolTipRole: 
        {
            if (!info->isFunctionNamed(elNum)) return "Not supported";
            return "";
        }
    }
    return QVariant();
}
