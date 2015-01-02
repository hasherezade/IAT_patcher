#pragma once

#include <QtGui>
#include <QFile>
#include <bearparser.h>

class FileLoader : public QThread
{
	Q_OBJECT
public:
    FileLoader(QString fileName) :  m_Buffer(NULL), m_Status(false) { m_FileName = fileName; }
    virtual ~FileLoader() { printf("~FileLoader: %s\n", m_FileName.toStdString().c_str());}

signals:
	void loaded(AbstractByteBuffer *buffer);

private:
	void run();
    bool parse(QString &fileName);

    bool m_Status;
    QString m_FileName;
	AbstractByteBuffer *m_Buffer;
	QMutex m_arrMutex;
};
 