#include <qaction.h>

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

signals:
    void exeUpdated(ExeHandler* exeHndl);

public slots:
    void onHookRequested(ExeHandler* exeHndl, StubSettings &settings);
    void onSaveRequested(ExeHandler* exeHndl);
    //void onUnloadRequested(ExeHandler* exeHndl) {}

    void onExportReplacements(ExeHandler* exeHndl);
    void onImportReplacements(ExeHandler* exeHndl);
};
