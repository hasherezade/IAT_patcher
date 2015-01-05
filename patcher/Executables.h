#pragma once
#include <QFile>
#include <bearparser.h>

#include "FuncReplacements.h"

class ExeHandler : public QObject
{
Q_OBJECT

signals:
    void stateChanged();

public:
    ExeHandler(AbstractByteBuffer *buf, Executable* exe)
        : m_Buf(buf), m_Exe(exe), isModified(false), hasUnapplied(false),
        dataStoreRva(INVALID_ADDR)
    {
        m_FuncMap.wrap(getImports());
        originalEP = exe->getEntryPoint();
        connect(&m_Repl, SIGNAL(stateChanged()), this, SLOT(onChildStateChanged()));
    }

    virtual ~ExeHandler() { delete m_Exe, delete m_Buf;}
    Executable* getExe() { return m_Exe; }

    bool hook(offset_t thunk, FuncDesc newFunc)
    {
        bool ret = m_Repl.hook(thunk, newFunc);
        if (ret) hasUnapplied = true;
        emit stateChanged();
        return ret;

    }
    FuncDesc getReplAt(offset_t thunk) { return m_Repl.getAt(thunk); }
    bool hasReplacements() { return m_Repl.size() > 0; }

    ImportDirWrapper* getImports()
    {
        MappedExe *mappedExe = dynamic_cast<MappedExe*>(m_Exe);
        if (mappedExe == NULL) return NULL;

        return dynamic_cast<ImportDirWrapper*>(mappedExe->getWrapper(PEFile::WR_DIR_ENTRY + pe::DIR_IMPORT));
    }

    void rewrapFuncMap()
    {
        this->m_FuncMap.wrap(getImports());
    }
    void setHookedState(bool flag) { isHooked = flag; }
    bool getHookedState() { return isHooked; }
    bool getModifiedState() { return isModified; }
    bool getUnappliedState() { return hasUnapplied; }

    offset_t getOriginalEP() { return originalEP; }
    offset_t getCurrentEP() { return m_Exe->getEntryPoint(); }

    FunctionsMap m_FuncMap;
    FuncReplacements m_Repl;

protected slots:
    void onChildStateChanged() { emit stateChanged(); }

protected:
    AbstractByteBuffer *m_Buf;
    Executable* m_Exe;

    bool isModified, hasUnapplied;
    //TODO: finish and refactor it
    bool isHooked;
    offset_t originalEP, dataStoreRva; // TODO: keep params in separate structure

friend class StubMaker;
};

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
