#include "FileLoader.h"

#include <stdexcept>

void FileLoader::run()
{
    QMutexLocker lock(&m_arrMutex);
    try {
        if (m_FileName == NULL) return;
        parse(m_FileName);
    } catch (...) { }

    emit loaded(m_Buffer);
}

bool FileLoader::parse(QString &fileName)
{
    bufsize_t maxMapSize = FILE_MAXSIZE;
    bool isLoaded = false;
    try {
        const bufsize_t MINBUF = 0x200;
        m_Buffer = new FileBuffer(fileName, MINBUF);
        isLoaded = true;
    } catch (CustomException &e) { }

    return isLoaded;
}

