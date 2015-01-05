#pragma once
#include <bearparser.h>

#include "Executables.h"
#include "FuncReplacements.h"
#include "Stub.h"

class StubSettings : public QObject
{
    Q_OBJECT
public:
    StubSettings() : addNewSection(true), reuseImports(true) {}
    void setAddNewSection(bool isEnabled) { this->addNewSection = isEnabled; }
    void setReuseImports(bool isEnabled) { this->reuseImports = isEnabled; }

    bool getAddNewSection() const { return this->addNewSection; }
    bool getReuseImports() const { return this->reuseImports; }

protected:
    bool addNewSection;
    bool reuseImports;

friend class StubMaker;
};

class StubMaker {

public:
    static bool makeStub(ExeHandler *exeHndl, const StubSettings &settings)
    {
        if (exeHndl == NULL) return false;
        bool isOk = false;
        PEFile *pe = dynamic_cast<PEFile*>(exeHndl->getExe());
        if (exeHndl->isHooked) {
            printf("File already hooked. Overwriting DataStore...\n");
            isOk = overwriteDataStore(exeHndl);
        } else {
            isOk = makeStub(pe, exeHndl->m_FuncMap, exeHndl->m_Repl, settings);
        }
        exeHndl->rewrapFuncMap();
        return isOk;
    }

    static size_t countMissingImports(ExeHandler *exeHndl)
    {
        return countMissingImports(exeHndl->m_FuncMap);
    }

    static bool fillHookedInfo(ExeHandler *exeHndl);

protected:
    static Stub* makeStub(PEFile *pe);
    static bool setStubParams(Stub* stb, PEFile *pe, const offset_t newEntry, const offset_t dataRva, FunctionsMap &funcMap);

    static size_t calcDataStoreSize(FuncReplacements &funcRepl);
    static size_t calcNewImportsSize(PEFile *pe, size_t addedFuncCount);

    static ByteBuffer* makeDataStore(const offset_t dataRva, FuncReplacements &funcRepl);
    static bool readDataStore(AbstractByteBuffer* storeBuf, const offset_t dataRva, FuncReplacements &out_funcRepl);
    static bool overwriteDataStore(ExeHandler *exeHndl);

    static ByteBuffer* createStub32(PEFile *peFile, offset_t stubRva, offset_t loadLib, offset_t getProcAddr);

    static size_t countMissingImports(FunctionsMap &funcMap);
    static bool makeThunksWriteable(PEFile *pe);

    static bool makeStub(PEFile *pe, FunctionsMap &funcMap, FuncReplacements &funcRepl, const StubSettings &settings);
    static bool addMissingFunctions(PEFile *pe, FunctionsMap &funcMap, bool tryReuse);

    static ImportEntryWrapper* addLibrary(PEFile *pe, QString name, offset_t &storageOffset);
    static bool addFunction(PEFile *pe, ImportEntryWrapper* libWr, ImportedFuncWrapper* func, const QString& name, offset_t &storageOffset);

};
