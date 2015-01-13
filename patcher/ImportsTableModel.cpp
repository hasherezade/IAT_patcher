#include "ImportsTableModel.h"

#include <QFileDialog>
#include <QMessageBox>

QVariant ImportsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole) return QVariant();
	if (orientation != Qt::Horizontal) return QVariant();
	switch (section) {
        case COL_THUNK : return "Thunk";
        case COL_LIB : return "Lib";
        case COL_FUNC : return "Functions";
        case COL_REPLACEMENT : return "ReplacementFunction";
	}
	return QVariant();
}

Qt::ItemFlags ImportsTableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) return 0;
	Qt::ItemFlags f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (index.column() == COL_REPLACEMENT) {
        f |= Qt::ItemIsEditable;
	}
	return f;
}

bool ImportsTableModel::setData(const QModelIndex &index, const QVariant &data, int role)
{
    if (index.column() != COL_REPLACEMENT) {
        return false;
	}
    //TODO: put it in the controller?
	offset_t thunk = indexToFunctionThunk(index);
	QString substName = data.toString();
    if (this->m_ExeHandler->m_Repl.getAt(thunk) == substName) return false;

    if (this->m_ExeHandler->defineReplacement(thunk, substName) == false) {
        QMessageBox::warning(NULL, "Error", "Invalid format supplied!\nValid: <library>.<function>");
        return false;
    }
    return true;
}

QVariant ImportsTableModel::data(const QModelIndex &index, int role) const
{
    if (this->countElements() == 0 || m_FuncMap == NULL) return QVariant();

	if (role != Qt::DisplayRole && role != Qt::EditRole) return QVariant();

    offset_t thunk = indexToFunctionThunk(index);
    if (thunk == INVALID_ADDR) return QVariant();

    int attribute = index.column();
    switch (attribute) {
        case COL_THUNK : return QString::number(thunk, 16);
        case COL_LIB : return m_FuncMap->thunkToLibName(thunk);
        case COL_FUNC : return m_FuncMap->thunkToFuncName(thunk);
        case COL_REPLACEMENT :
        {
            QString replName = this->m_ExeHandler->getReplAt(thunk);
            return replName;
        }
    }
    return QVariant();
}


