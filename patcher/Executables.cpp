#include "Executables.h"
#include "StubMaker.h"

size_t Executables::size() { 
    QMutexLocker lock(&m_listMutex); //LOCKER 
    return m_Exes.size(); 
}

ExeHandler* Executables::at(size_t index)
{
    QMutexLocker lock(&m_listMutex); //LOCKER
    if (index >= m_Exes.size()) return NULL;
    return m_Exes.at(index);
}

bool Executables::_addExe(ExeHandler *exeHndl)
{
    if (exeHndl == NULL) return false;

    { //start locked scope
        QMutexLocker lock(&m_listMutex); //LOCKER
        m_Exes.push_back(exeHndl);
    } //end locked scope

    StubMaker::fillHookedInfo(exeHndl);
    connect(exeHndl, SIGNAL(stateChanged()), this, SLOT(onChildStateChanged()));
    return true;
}

bool  Executables::_removeExe(ExeHandler *exe)
 {
    QMutexLocker lock(&m_listMutex); //LOCKER
    size_t indx = m_Exes.indexOf(exe);
    if (indx != -1) {
        m_Exes.removeAt(indx);
        return true;
    }
    return false;
 }

QStringList Executables::listFiles()
{
    QStringList fileNames;
    QList<ExeHandler*>::iterator itr;
    QString fileName = "";

    QMutexLocker lock(&m_listMutex); //LOCKER
    for (itr = m_Exes.begin(); itr != m_Exes.end(); itr++) {
        ExeHandler *exeHndl = (*itr);
        if (exeHndl->getExe() != NULL) {
            fileName = exeHndl->getFileName();
            fileNames << fileName;
        }
    }
    return fileNames;
 }

void Executables::_clear()
{
    QMutexLocker lock(&m_listMutex); //LOCKER
    QList<ExeHandler*>::iterator itr;
    for (itr = m_Exes.begin(); itr != m_Exes.end(); itr++) {
        ExeHandler *exe = (*itr);
        delete exe;
    }
    m_Exes.clear();
}
