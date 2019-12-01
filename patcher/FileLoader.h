#pragma once

#include <QtWidgets>
#include <QFile>
#include <bearparser/core.h>
#include <stdexcept>

class FileLoader : public QThread
{
    Q_OBJECT
public:
    FileLoader(QString fileName) :  m_Buffer(NULL), m_Status(false) { m_FileName = fileName; }
    virtual ~FileLoader() { }

signals:
    void loaded(AbstractByteBuffer *buffer);
    void loadingFailed(QString fileName);

protected:
    void run();
    virtual bool parse(QString &fileName);

    QString m_FileName;
    bool m_Status;


private:
    AbstractByteBuffer *m_Buffer;
    QMutex m_arrMutex;
};
