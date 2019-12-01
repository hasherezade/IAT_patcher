#pragma once
#include <QFile>
#include <bearparser/core.h>

#include "LibraryInfo.h"

class LibraryParser: public QObject
{
    Q_OBJECT
signals:
    void infoCreated(LibraryInfo*);
public:
    LibraryParser(QObject* parent = NULL) 
        : QObject(parent) {}
    ~LibraryParser(){}

    void makeLibraryInfo(Executable* exe, QString fileName);
    ExportDirWrapper* getExports(Executable* exe);

public slots:
    void on_parseLibrary(QString&);
};
