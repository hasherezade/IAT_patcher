#pragma once
#include <QFile>
#include <bearparser/core.h>

#include "ExeHandler.h"

class Executables : public QObject
{
Q_OBJECT

signals:
    void exeListChanged();

public slots:
    void addExe(ExeHandler *exe) { if (_addExe(exe)) emit exeListChanged(); }
    void removeExe(ExeHandler *exe) { if (_removeExe(exe)) emit exeListChanged(); }
    void clear() { _clear(); emit exeListChanged(); }

public:
    size_t size();
    ExeHandler* at(size_t index);

    QStringList listFiles();

protected slots:
    void onChildStateChanged() { emit exeListChanged(); }

private:
    bool _addExe(ExeHandler *exe);
    bool _removeExe(ExeHandler *exe);
    void _clear();

    QList<ExeHandler*> m_Exes;
    QMutex m_listMutex;
};
