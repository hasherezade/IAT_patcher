#include "HookManager.h"


bool HookManager::canHook()
{


}

bool HookManager::hook()
{
    Executable *exe = this->m_ExeSelected->getExe();
    if (exe == NULL) return;

    PEFile *pe = dynamic_cast<PEFile*>(exe);
    if (pe == NULL) {
        QMessageBox::warning(NULL, "Cannot hook!", "It is not a PE File!");
        return;
    }

    if (m_ExeSelected->mustAddImports() == false) {
        QMessageBox::information(NULL, "Cool!", "No need to add imports!");
    } else {
        QMessageBox::information(NULL, "Boo", "Have to add imports!");
    }

    try {
        if (m_ExeSelected->addNewSection(pe)) {
            emit exeUpdated(this->m_ExeSelected);
            QMessageBox::information(NULL, "Done!", "Hooked!");
            return;
        } else {
            QMessageBox::warning(NULL,"Failed", "Cannot add new section");
        }
    } catch (CustomException &e) {
        QMessageBox::warning(NULL, "Error!", e.getInfo());
    }
}
