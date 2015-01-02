#include "Executables.h"


ExeHandler* Executables::at(size_t index) const
{
    if (index >= size()) return NULL;
    return m_Exes.at(index);
}

void Executables::addExe(ExeHandler *exe)
{
    if (exe == NULL) return;

    m_Exes.push_back(exe);
    connect(exe, SIGNAL(stateChanged()), this, SLOT(onChildStateChanged()));

    emit exeListChanged();
}

 void Executables::removeExe(ExeHandler *exe)
 {
    size_t indx = m_Exes.indexOf(exe);
    if (indx != -1) {
        m_Exes.removeAt(indx);
        emit exeListChanged();
    }
 }

QStringList Executables::listFiles()
{
    QStringList fileNames;
    QList<ExeHandler*>::iterator itr;
    QString fileName = "";

    for (itr = m_Exes.begin(); itr != m_Exes.end(); itr++) {
        ExeHandler *exe = (*itr);
        if (exe->getExe() != NULL) {
            fileName = exe->getExe()->getFileName();
            fileNames << fileName;
        }
    }
    return fileNames;
 }

void Executables::clear()
{
    QList<ExeHandler*>::iterator itr;
    for (itr = m_Exes.begin(); itr != m_Exes.end(); itr++) {
        ExeHandler *exe = (*itr);
        delete exe;
    }
    m_Exes.clear();
    emit exeListChanged();
}
