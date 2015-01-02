#include "Executables.h"

class HookManager : public QObject
{
Q_OBJECT

signals:
    void exeUpdated(ExeHandler*);

public:

    HookManager(ExeHandler *exeHander);

    bool canHook();
    bool hook();

protected:
    ExeHandler *m_ExeHndl;
};
