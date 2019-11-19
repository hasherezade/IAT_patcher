#pragma once

#include <QtWidgets>
#include "Executables.h"
#include "StubMaker.h"

class ExeController : public QObject
{
    Q_OBJECT
public:
    enum EXE_ACTION
    {
        ACTION_HOOK = 0,
        ACTION_SAVE,
        ACTION_UNLOAD,
        ACTION_RELOAD,
        ACTION_EXPORT_REPL,
        ACTION_IMPORT_REPL,
        COUNT_ACTION
    };

    ExeController(QObject* parent = NULL) {}
    virtual ~ExeController() { }

    bool saveExecutable(ExeHandler* exeHndl, QString newFilename); //throws CustomException
    bool hookExecutable(ExeHandler* exeHndl, StubSettings &settings); //throws CustomException
    size_t loadReplacementsFromFile(ExeHandler* exeHndl, QString filename);
    size_t saveReplacementsToFile(ExeHandler* exeHndl, QString filename);

signals:
    void exeUpdated(ExeHandler* exeHndl);
};

