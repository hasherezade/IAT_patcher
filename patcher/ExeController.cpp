#include "ExeController.h"

#include <QFileDialog>
#include <QMessageBox>

bool ExeController::saveExecutable(ExeHandler* exeHndl, QString newFilename)
{
    if (exeHndl == NULL) {
        return false;
    }

    Executable *exe = exeHndl->getExe();
    if (exe == NULL) return false;

    if (AbstractFileBuffer::dump(newFilename, *exe, true) > 0) {
        exeHndl->onFileSaved(newFilename);
        emit exeUpdated(exeHndl);
        return true;
    }
    return false;
}

bool ExeController::hookExecutable(ExeHandler* exeHndl, StubSettings &settings)
{
    bool isSuccess = StubMaker::makeStub(exeHndl, settings);
    //update view even if hooking partialy failed...
    StubMaker::fillHookedInfo(exeHndl);
    emit exeUpdated(exeHndl);
    return isSuccess;
}

void ExeController::onExportReplacements(ExeHandler* exeHndl)
{
    if (exeHndl == NULL) return;

    if (exeHndl->hasReplacements() == false) {
        QMessageBox::warning(NULL, "Cannot save!", "The file have NO replacements defined!");
        return;
    }
    QString infoStr ="Save replacements as...";
    QFileInfo inputInfo(exeHndl->getFileName());

    QString fileName = QFileDialog::getSaveFileName(
        NULL,
        infoStr,
        inputInfo.absoluteDir().path(),
        "Config file (*.txt);;Config file (*.cfg);;All files (*.*)"
    );
    if (fileName.length() == 0) return;

    size_t counter = exeHndl->m_Repl.save(fileName);
    if (counter == 0) {
        QMessageBox::warning(NULL, "Error!", "Cannot export!");
    } else {
        QString ending =  (counter > 1) ? "s":" ";
        QMessageBox::information(NULL, "Done!", "Exported: " + QString::number(counter) + " replacement" + ending);
    }
}

void ExeController::onImportReplacements(ExeHandler* exeHndl)
{
    if (exeHndl == NULL) return;

    QString infoStr ="Import replacements";
    QFileInfo inputInfo(exeHndl->getFileName());

    QString fileName = QFileDialog::getOpenFileName(
        NULL,
        infoStr,
        inputInfo.absoluteDir().path(),
        "Config file (*.txt);;Config file (*.cfg);;All files (*.*)"
    );
    if (fileName.length() == 0) return;

    size_t counter = exeHndl->m_Repl.load(fileName);
    if (counter == 0) {
        QMessageBox::warning(NULL, "Error!", "Cannot import!");
        return;
    }
    
    size_t invalidThunks = exeHndl->m_Repl.dropInvalidThunks(exeHndl->m_FuncMap);
    counter -= invalidThunks;
    if (invalidThunks > 0) {
        QMessageBox::warning(NULL, "Warning!", "Found " + QString::number(invalidThunks) + " thunks that are not valid for this executable!");
        return;
    } 
    if (counter > 0) {
        QString ending =  (counter > 1) ? "s":" ";
        QMessageBox::information(NULL, "Done!", "Imported: " + QString::number(counter) + " replacement" + ending);
        if (counter != 0)  exeHndl->setUnappliedState(true);
    }
}