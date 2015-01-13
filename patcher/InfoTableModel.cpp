#include "InfoTableModel.h"
#include "StubMaker.h"

#include <QFileDialog>
#include <QMessageBox>
 #include <QIcon>

#define HIGLIHHT_COLOR "yellow"
#define OK_COLOR "green"
#define NOT_OK_COLOR "red"

QVariant InfoTableModel::headerData(int section, Qt::Orientation orientation, int role) const
	{
	if (role != Qt::DisplayRole) return QVariant();
	if (orientation != Qt::Horizontal) return QVariant();
	switch (section) {
        case COL_NAME : return "FileName";
        case COL_HOOKED: return "Hooked?";
        case COL_RAW_SIZE: return "RawSize";
        case COL_VIRTUAL_SIZE: return "VirtualSize";
        case COL_SECNUM : return "Sections";
        case COL_IMPNUM : return "Imports";
	}
	return QVariant();
}

Qt::ItemFlags InfoTableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) return 0;
	Qt::ItemFlags f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if (index.column() == COL_HOOKED) {
        f |= Qt::ItemIsUserCheckable;
        f |= Qt::ItemIsEditable;
	}
	return f;
}

bool InfoTableModel::setData(const QModelIndex &index, const QVariant &, int role)
{
    if (index.column() == COL_HOOKED) {
        if ( role == Qt::CheckStateRole) {
            int id = index.row();
            if (m_Exes == NULL) return false;
            emit hookRequested(this->m_Exes->at(id));
        }
        return true;
    }
    return false;
}


QVariant InfoTableModel::data(const QModelIndex &index, int role) const
{
    int elNum = index.row();
    if (elNum > countElements()) return QVariant();

    int attribute = index.column();
    if (attribute >= COUNT_COL) return QVariant();

    ExeHandler *exeHndl = m_Exes->at(elNum);
	if (exeHndl == NULL) return QVariant();
    Executable *exe = exeHndl->getExe();
    if (exe == NULL) return QVariant();

    if (role == Qt::DisplayRole) {
        return getDisplayData(role, attribute, exeHndl);
    }

    if (role == Qt::BackgroundColorRole) {
        if (exeHndl->getUnappliedState() == true) {
            return QColor(HIGLIHHT_COLOR);
        }
        return QVariant();
    }

    if (role == Qt::DecorationRole && attribute == COL_NAME) {
        if (exe->isBit32()) return QIcon(":/icons/app32.ico");
        if (exe->isBit64()) return QIcon(":/icons/app64.ico");
	}

    if (role == Qt::CheckStateRole && attribute == COL_HOOKED) {
        if (exeHndl->getHookedState()) return Qt::Checked;
        else return Qt::Unchecked;
    }

    PEFile *pe = dynamic_cast<PEFile*>(exeHndl->getExe());
    if (pe == NULL) return QVariant();

    if (role == Qt::TextColorRole ) {
        if (attribute == COL_SECNUM) {
            if (pe->canAddNewSection()) return QColor(OK_COLOR);
                return QColor(NOT_OK_COLOR);
            }
        if (attribute == COL_IMPNUM) {
            if (StubMaker::countMissingImports(exeHndl) > 0) return QColor(NOT_OK_COLOR);
            return QColor(OK_COLOR);
        }
        return QVariant();
    }

    if (role == Qt::ToolTipRole ) {
        if (attribute == COL_SECNUM) {
            if (pe->canAddNewSection()) return "Can add section";
            return "Can NOT add section";
        }
        if (attribute == COL_IMPNUM) {
            if (StubMaker::countMissingImports(exeHndl) > 0) return "Needs to add imports";
            return "Can reuse all imports";
        }
        if (attribute == COL_HOOKED) {
            QString info = exeHndl->getHookedState() ? "Hooked": "Not hooked";
            info += "\nDefined "+ QString::number(exeHndl->m_Repl.size()) + " replacements";
            return info;
        }
        if (attribute == COL_NAME) {
            QString epInfo = "OEP\t= "+ QString::number( exeHndl->getOriginalEP(), 16 );

            if (exeHndl->getHookedState()) {
                epInfo += "\nEP \t= "+ QString::number( exeHndl->getCurrentEP(), 16 );
            }
            return epInfo;
        }
        return "";
    }
    return QVariant();
}

QVariant InfoTableModel::getDisplayData(int role, int attribute, ExeHandler *exeHndl) const
{
    Executable *exe = exeHndl->getExe();
    if (exe == NULL) return QVariant();

    if (role != Qt::DisplayRole) return QVariant();

    switch (attribute) {
        case COL_NAME :
        {
            QFileInfo inputInfo(exeHndl->getFileName());
            QString name = inputInfo.fileName();
            if (exeHndl->getModifiedState() == true) {
                name = "* " + name;
            }
            return name;
        }
        case COL_RAW_SIZE : return  exe->getMappedSize(Executable::RAW);
        case COL_VIRTUAL_SIZE : return  exe->getMappedSize(Executable::RVA);
    }

    PEFile *pe = dynamic_cast<PEFile*>(exeHndl->getExe());
    if (pe == NULL) return QVariant();

    switch (attribute) {
        case COL_SECNUM :
            return qint64(pe->hdrSectionsNum());

        case COL_IMPNUM :
        {
            ExeNodeWrapper *wr = dynamic_cast<ExeNodeWrapper*>(pe->getWrapper(PEFile::WR_DIR_ENTRY + pe::DIR_IMPORT));
            if (!wr) return 0;
            return qint64(wr->getEntriesCount());
        }
        case COL_HOOKED :
        {
            return qint64(exeHndl->m_Repl.size());
        }
    }
    return QVariant();
}

