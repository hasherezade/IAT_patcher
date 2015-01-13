#pragma once
#include <QFile>
#include <bearparser.h>

#include "ExeHandler.h"

class Executables : public QObject
{
Q_OBJECT

signals:
    void exeListChanged();

public:
    size_t size() const { return m_Exes.size(); }
    ExeHandler* at(size_t index) const;

    void addExe(ExeHandler *exe);
    void removeExe(ExeHandler *exe);
    void clear();
    QStringList listFiles();

protected slots:
    void onChildStateChanged() { emit exeListChanged(); }

protected:
    QList<ExeHandler*> m_Exes;
};
