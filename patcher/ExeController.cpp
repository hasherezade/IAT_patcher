#include "ExeController.h"

#include <QFileDialog>
#include <QMessageBox>


void ExeController::onHookRequested(ExeHandler* exeHndl, StubSettings &settings)
{
    if (exeHndl == NULL) return;

    Executable *exe = exeHndl->getExe();
    if (exe == NULL) return;

    bool isHooked = exeHndl->getHookedState();
    PEFile *pe = dynamic_cast<PEFile*>(exe);
    if (pe == NULL) {
        QMessageBox::warning(NULL, "Cannot hook!", "It is not a PE File!");
        return;
    }
    if (isHooked) {
        if (exeHndl->getUnappliedState() == false) {
            QMessageBox::information(NULL, "No changes!", "No changes to be applied");
            return;
        }
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(NULL, "Already hooked!",
            "This file is already hooked.\nDo you want to modify the existing stub?",
            QMessageBox::Yes|QMessageBox::No
            );

        if (reply == QMessageBox::No) {
            return;
        }
    }

    if (exeHndl->hasReplacements() == false && exeHndl->getHookedState() == false) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(NULL, "No replacements!",
            "You haven't defined any replacement functions.\nDo you really want to add an empty stub?",
            QMessageBox::Yes|QMessageBox::No
            );

        if (reply == QMessageBox::No) {
            return;
        }
    }

    if (!isHooked && pe->canAddNewSection() == false && settings.getAddNewSection()) {
         QMessageBox::information(NULL,"Warning", "Cannot add new section in this file!\nProceed by extending last section...");
    }
    try {
        bool isSuccess = StubMaker::makeStub(exeHndl, settings);
        //update view even if hooking partialy failed...
        StubMaker::fillHookedInfo(exeHndl);
        emit exeUpdated(exeHndl);

        if (isSuccess) {
            QMessageBox::information(NULL, "Done!", "Hooked!\nNow you can save and test the file!");
            return;
        } else {
            QMessageBox::warning(NULL,"Failed", "Cannot hook!");
        }
    } catch (CustomException &e) {
        QMessageBox::warning(NULL, "Error!", e.getInfo());
    }
}

void ExeController::onSaveRequested(ExeHandler* exeHndl)
{
    if (exeHndl == NULL) {
        QMessageBox::warning(NULL, "Cannot save!", "No executable selected!");
        return;
    }

    Executable *exe = exeHndl->getExe();
    if (exe == NULL) return;

    if (exeHndl->getUnappliedState()) {
        QMessageBox::StandardButton reply;
        QMessageBox::warning(NULL, "Unapplied replacements!",
            "The file has unapplied replacements. Hook the file to apply them.");
        return;

    }
    if (exeHndl->getHookedState() == false) {
        //QMessageBox::warning(NULL, "Nothing to save!", "Hook the file first!");

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(NULL, "Unhooked file!",
            "You are trying to save unhooked file. Do you really want?",
            QMessageBox::Yes|QMessageBox::No
            );

        if (reply == QMessageBox::No) {
            return;
        }
    }
    QFileInfo inputInfo(exeHndl->getFileName());

    QString infoStr ="Save executable as:";
    QString fileName = QFileDialog::getSaveFileName(
        NULL,
        infoStr,
        inputInfo.absoluteDir().path(),
        "Exe Files (*.exe);;DLL Files (*.dll);;All files (*.*)"
    );
    if (fileName.length() == 0) return;

    try {
        AbstractFileBuffer::dump(fileName, *exe, true);
        exeHndl->setFileName(fileName);
        exeHndl->isModified = false;
        emit exeUpdated(exeHndl);

    } catch (CustomException &e) {
        QMessageBox::warning(NULL, "Error!", e.getInfo());
    }
    
}

