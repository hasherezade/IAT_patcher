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

size_t ExeController::saveReplacementsToFile(ExeHandler* exeHndl, QString fileName)
{
    if (fileName.length() == 0) {
        return 0;
    }
    size_t counter = exeHndl->m_Repl.save(fileName);
    return counter;
}

size_t ExeController::loadReplacementsFromFile(ExeHandler* exeHndl, QString fileName)
{
    if (fileName.length() == 0) return 0;

    size_t counter = exeHndl->m_Repl.load(fileName);
    if (counter == 0) {
        return 0;
    }

    size_t invalidThunks = exeHndl->m_Repl.dropInvalidThunks(exeHndl->m_FuncMap);
    counter -= invalidThunks;
    if (counter != 0) {
        exeHndl->setUnappliedState(true);
    }
    return counter;
}
