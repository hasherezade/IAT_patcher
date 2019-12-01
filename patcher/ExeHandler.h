#pragma once
#include <QFile>
#include <bearparser/core.h>

#include "ImportsLookup.h"
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
        m_fileName = (m_Exe) ? m_Exe->getFileName(): "";
        connect(&m_Repl, SIGNAL(stateChanged()), this, SLOT(onChildStateChanged()));
    }

    virtual ~ExeHandler() { delete m_Exe, delete m_Buf;}
    Executable* getExe() { return m_Exe; }

    QString getFileName() { return m_fileName; }

    bool defineReplacement(offset_t thunk, FuncDesc newFunc);
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

    ImportsLookup m_FuncMap;
    FuncReplacements m_Repl;

public slots:
    void onFileSaved(QString newName)
    {
        this->m_fileName = newName;
        this->isModified = false; //modifications are saved
        emit stateChanged();
    }

protected slots:
    void onChildStateChanged() { emit stateChanged(); }

protected:
    void setUnappliedState(bool flag) { hasUnapplied = flag; }

    AbstractByteBuffer *m_Buf;
    Executable* m_Exe;

    QString m_fileName;
    bool isModified, hasUnapplied;
    //TODO: finish and refactor it
    bool isHooked;
    offset_t originalEP, dataStoreRva; // TODO: keep params in separate structure

friend class StubMaker;
friend class ExeController;
};
