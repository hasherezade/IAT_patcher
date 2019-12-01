#pragma once

#include <QtWidgets>
#include <QFile>
#include <bearparser/core.h>
#include <stdexcept>

#include "FileLoader.h"
#include "ExeHandler.h"
 
class ExeHandlerLoader : public FileLoader
{
    Q_OBJECT
signals:
    void loaded(ExeHandler *exeHndl);

public:
    ExeHandlerLoader(QString fileName) : FileLoader(fileName) {}
    virtual bool parse(QString &fileName);
};