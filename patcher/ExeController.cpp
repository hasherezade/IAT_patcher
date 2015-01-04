#include "ExeController.h"

#include <QFileDialog>
#include <QMessageBox>


void ExeController::onHookRequested(ExeHandler* exeHndl, StubSettings &settings)
{
    if (exeHndl == NULL) return;

    Executable *exe = exeHndl->getExe();
    if (exe == NULL) return;

    PEFile *pe = dynamic_cast<PEFile*>(exe);
    if (pe == NULL) {
        QMessageBox::warning(NULL, "Cannot hook!", "It is not a PE File!");
        return;
    }
    if (exeHndl->getHookedState()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(NULL, "Already hooked!",
            "This file is already hooked.\nDo you really want to double hook it?",
            QMessageBox::Yes|QMessageBox::No
            );

        if (reply == QMessageBox::No) {
            return;
        }
    }
    /*
    if (pe->getBitMode() != Executable::BITS_32) {
        QMessageBox::warning(NULL, "Cannot hook!", "PE64 is not supported yet!");
        return;
    }
    */
    if (exeHndl->hasReplacements() == false) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(NULL, "No replacements!",
            "You haven't defined any replacement functions.\nDo you really want to add an empty stub?",
            QMessageBox::Yes|QMessageBox::No
            );

        if (reply == QMessageBox::No) {
            return;
        }
    }

    /*StubSettings settings;
    settings.setAddNewSection(this->m_ui.actionAdd_new_section->isChecked());
    settings.setReuseImports(this->m_ui.actionAdd_imports->isChecked());

    QString settingsStr = "Settings: ";
    if (settings.getAddNewSection()) {
        settingsStr += "add new section ;";
    }
    if (settings.getReuseImports()) {
        settingsStr += "reuse imports";
    }
    this->m_ui.statusBar->showMessage(settingsStr);
    */
    if (pe->canAddNewSection() == false && settings.getAddNewSection()) {
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
    QFileInfo inputInfo(exe->getFileName());

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
    } catch (CustomException &e) {
        QMessageBox::warning(NULL, "Error!", e.getInfo());
    }
}
