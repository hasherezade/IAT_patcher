#include "LibraryInfo.h"

size_t LibInfos::size() { 
    QMutexLocker lock(&m_listMutex); //LOCKER 
    return m_Libs.size(); 
}

LibraryInfo* LibInfos::at(size_t index)
{
    QMutexLocker lock(&m_listMutex); //LOCKER
    if (index >= m_Libs.size()) return NULL;
    return m_Libs.at(index);
}

bool LibInfos::_addElement(LibraryInfo *hndl)
{
    if (hndl == NULL) return false;

    { //start locked scope
        QMutexLocker lock(&m_listMutex); //LOCKER
        m_Libs.push_back(hndl);
    } //end locked scope
    connect(hndl, SIGNAL(stateChanged()), this, SLOT(onChildStateChanged()));
    printf("added element\n");
    return true;
}

bool  LibInfos::_removeElement(LibraryInfo *exe)
 {
    QMutexLocker lock(&m_listMutex); //LOCKER
    size_t indx = m_Libs.indexOf(exe);
    if (indx != -1) {
        m_Libs.removeAt(indx);
        return true;
    }
    return false;
 }

QStringList LibInfos::listFiles()
{
    QStringList fileNames;
    QList<LibraryInfo*>::iterator itr;
    QString fileName = "";

    QMutexLocker lock(&m_listMutex); //LOCKER
    for (itr = m_Libs.begin(); itr != m_Libs.end(); itr++) {
        LibraryInfo *info = (*itr);
        fileNames << info->getFileName();
    }
    return fileNames;
 }

void LibInfos::_clear()
{
    QMutexLocker lock(&m_listMutex); //LOCKER
    QList<LibraryInfo*>::iterator itr;
    for (itr = m_Libs.begin(); itr != m_Libs.end(); itr++) {
        LibraryInfo *exe = (*itr);
        delete exe;
    }
    m_Libs.clear();
}
