#include "StubMaker.h"

#include "stub/Stub32.h"
#include "stub/Stub64.h"

const size_t SEC_PADDING = 10;

size_t StubMaker::countMissingImports(ImportsLookup &funcMap)
{
    QString libName = "Kernel32.dll";
    QStringList funcNames = (QStringList() << "LoadLibraryA" << "GetProcAddress"); // << "VirtualProtect"

    size_t count = 0;
    for ( size_t i = 0; i < funcNames.size(); i++) {
        offset_t thunk = funcMap.findThunk(libName, funcNames[i]);
        if (thunk == INVALID_ADDR) {
            count++;
        }
    }
    return count;
}

bool StubMaker::fillHookedInfo(ExeHandler *exeHndl)
{
    if (exeHndl == NULL) return false;

    PEFile *pe = static_cast<PEFile*> (exeHndl->getExe());
    if (pe == NULL) return false;

    offset_t ep = pe->getEntryPoint();
    offset_t epRaw = pe->convertAddr(ep, Executable::RVA, Executable::RAW);
    if (epRaw == INVALID_ADDR) return false;

    BufferView epView(pe, epRaw, pe->getContentSize() - epRaw);

    Stub *stub = NULL;
    if (pe->isBit32()) {
        stub = new Stub32();
    } else {
        stub = new Stub64();
    }

    bool isContained = stub->containsStub(&epView);
    exeHndl->setHookedState(isContained);

    if (isContained) {
        if (stub->readParams(&epView)) {
            //TODO: fill all the params in exeHndl
            /*size_t pCnt = stub->getParamsCount();
            for (size_t id = 0; id< pCnt; id++) {
                printf("[%d] = %llx\n", id, stub->getParamValue(id));
            }*/
            //TODO: keep stub params in a separate structure
            exeHndl->originalEP = stub->getParamValue(Stub::OEP);
            exeHndl->dataStoreRva = stub->getParamValue(Stub::DATA_RVA);

            offset_t dataRva =exeHndl->dataStoreRva;
            offset_t dataRaw = pe->toRaw(dataRva, Executable::RVA);
            BufferView dataBuf(pe, dataRaw, pe->getContentSize() - dataRaw);
            //printf("Reading dataStore at = %llx -> %llx\n", dataRva, dataRaw);
            readDataStore(&dataBuf, dataRva, exeHndl->m_Repl);
            //printf("Params OK, OEP = %llx\n", exeHndl->originalEP);
        }
    }
    delete stub;
    return isContained;
}

Stub* StubMaker::makeStub(PEFile *pe)
{
    if (pe == NULL) return NULL;

    Stub *stb = NULL;
    if (pe->isBit32()) {
        stb = new Stub32();
    } else {
        stb  = new Stub64();
    }
    return stb;
}

bool StubMaker::setStubParams(Stub* stb, PEFile *pe, const offset_t newEntry, const offset_t dataRva, ImportsLookup &funcMap)
{
    offset_t loadLib = funcMap.findThunk("Kernel32.dll","LoadLibraryA");
    offset_t getProc = funcMap.findThunk("Kernel32.dll","GetProcAddress");
    offset_t oldEntry = pe->getEntryPoint();

    stb->setParam(Stub::NEW_EP, newEntry);
    stb->setParam(Stub::OEP, oldEntry);
    stb->setParam(Stub::DATA_RVA, dataRva);
    stb->setParam(Stub::FUNC_LOAD_LIB_RVA, loadLib);
    stb->setParam(Stub::FUNC_GET_MODULE_RVA, getProc);

    return true;
}

size_t StubMaker::calcReplNamesSize(FuncReplacements &funcRepl)
{
    const size_t PADDING = 1;
    size_t requiredLen = PADDING;

    QList<offset_t> thunks = funcRepl.getThunks();

    QList<offset_t>::Iterator itr;
    for (itr = thunks.begin(); itr != thunks.end(); itr++) {
        FuncDesc desc = funcRepl.getAt(*itr);
        requiredLen += desc.size();
        requiredLen += PADDING;
    }
    return requiredLen;
}

size_t StubMaker::calcDataStoreSize(FuncReplacements &funcRepl)
{
    const size_t ELEMENTS = 4; //libRVA, funcRVA, thunk, EMPTY

    const size_t OFFSETS_SPACE = (funcRepl.size() * ELEMENTS * sizeof(DWORD)) + sizeof(DWORD) * 2;
    const size_t NAMES_SPACE = calcReplNamesSize(funcRepl);

    const size_t TOTAL_SPACE = OFFSETS_SPACE + NAMES_SPACE;
    return TOTAL_SPACE;
}

ByteBuffer* StubMaker::makeDataStore(const offset_t dataRva, FuncReplacements &funcRepl)
{
    const size_t ELEMENTS = 4; //libRVA, funcRVA, thunk, EMPTY

    const size_t OFFSETS_SPACE = (funcRepl.size() * ELEMENTS * sizeof(DWORD)) + sizeof(DWORD) * 2;
    const size_t NAMES_SPACE = calcReplNamesSize(funcRepl);

    const size_t TOTAL_SPACE = OFFSETS_SPACE + NAMES_SPACE;
    char *buffer = new char[TOTAL_SPACE];

    memset(buffer, 0, TOTAL_SPACE);
    size_t DELTA1 = OFFSETS_SPACE;

    char* namesBuf = &buffer[DELTA1];

    DWORD nameRva = dataRva + DELTA1;
    QList<offset_t> allThunks = funcRepl.getThunks();

    DWORD *fAddress = (DWORD*) buffer;
    size_t addrIndx = 0;

    for (int i = 0; i < allThunks.size(); i++) {

        offset_t thunk = allThunks[i];
        FuncDesc library = funcRepl.getAt(thunk);

        QString libName;
        QString funcName;
        if (FuncUtil::parseFuncDesc(library, libName, funcName) == false) {
            //printf("Invalid library format!\n");
            continue;
        }

        strcpy(namesBuf, libName.toStdString().c_str());
        fAddress[addrIndx++] = nameRva;
        size_t libLen = libName.length() + 1;
        nameRva  += libLen;
        namesBuf += libLen;

        strcpy(namesBuf, funcName.toStdString().c_str());
        fAddress[addrIndx++] = nameRva;
        size_t funcLen = funcName.length() + 1;
        nameRva += funcLen;
        namesBuf += funcLen;

        //save thunk:
        fAddress[addrIndx++] = thunk;

        //add space:
        fAddress[addrIndx++] = 0;
    }
    ByteBuffer *dataBuf = new ByteBuffer((BYTE*) buffer, TOTAL_SPACE, 0);
    delete []buffer;
    return dataBuf;
}

bool StubMaker::readDataStore(AbstractByteBuffer* buf, const offset_t dataRva, FuncReplacements &funcRepl)
{
    const offset_t start = dataRva;
    const size_t unitSize = sizeof(DWORD);

    offset_t valOffset = 0;
    offset_t value = 0;

    bool isOk = true;

    for (valOffset = 0; isOk ;) {
        value = buf->getNumValue(valOffset, unitSize, &isOk);
        valOffset += unitSize;

        if (!isOk || value == 0) break;
        offset_t dllNameOffset = value - start;
        QString dllName = buf->getStringValue(dllNameOffset);

        while (isOk) {
            value = buf->getNumValue(valOffset, unitSize, &isOk);
            valOffset += unitSize;
            offset_t funcNameOffset = value - start;
            if (!isOk || value == 0) break; //end of DLL processing

            value = buf->getNumValue(valOffset, unitSize, &isOk);
            valOffset += unitSize;
            if (!isOk || value == 0) break; //end of DLL processing
            offset_t thunk = value;

            QString funcName = buf->getStringValue(funcNameOffset);
            //TODO: refactor it...
            FuncDesc desc = dllName + "." + funcName;
            funcRepl.defineReplacement(thunk, desc);
        }
    }
    return isOk;
}

bool StubMaker::overwriteDataStore(ExeHandler *exeHndl)
{
    if (exeHndl->isHooked == false) return false;
    PEFile *pe = dynamic_cast<PEFile*>(exeHndl->getExe());
    if (!pe) return false;

    offset_t dataRva = exeHndl->dataStoreRva;
    offset_t dataRaw = pe->toRaw(dataRva, Executable::RVA);
    if (dataRaw == INVALID_ADDR || dataRaw == 0) return false;

    bufsize_t currentSize = pe->getContentSize() - dataRaw;
    bufsize_t requiredSize = calcDataStoreSize(exeHndl->m_Repl);

    if (requiredSize > currentSize) {
        bufsize_t dif = requiredSize - currentSize;
        //perform resising...
        if (!pe->extendLastSection(dif + SEC_PADDING)) return false;
    }

    BufferView dataBuf(pe, dataRaw, pe->getContentSize() - dataRaw);
    dataBuf.fillContent(0);
    ByteBuffer* newStore = makeDataStore(dataRva, exeHndl->m_Repl);
    bool isOk = dataBuf.pasteBuffer(0, newStore, false);
    delete newStore;
    return isOk;
}

bool StubMaker::addFunction(PEFile *pe, ImportEntryWrapper* libWr, ImportedFuncWrapper* func, const QString& name, offset_t &storageOffset)
{
    if (pe == NULL) return false;

    ImportedFuncWrapper* fWr = dynamic_cast<ImportedFuncWrapper*>(libWr->addEntry(func));
    if (fWr == NULL) {
        printf("Cannot add function\n");
        return false;
    }

    offset_t thunkRVA = pe->convertAddr(storageOffset, Executable::RAW, Executable::RVA);

    fWr->setNumValue(ImportedFuncWrapper::THUNK, thunkRVA);
    fWr->setNumValue(ImportedFuncWrapper::ORIG_THUNK, thunkRVA);

    storageOffset += sizeof(WORD); //add sizeof Hint
    if (pe->setStringValue(storageOffset, name) == false) {
        printf("Failed to fill lib  name\n");
        return false;
    }
    storageOffset += name.length() + 1;
    return true;
}

ImportEntryWrapper* StubMaker::addLibrary(PEFile *pe, QString name, offset_t &storageOffset)
{
    //add new library wrapper:
    ImportDirWrapper* imports = dynamic_cast<ImportDirWrapper*> (pe->getWrapper(PEFile::WR_DIR_ENTRY + pe::DIR_IMPORT));
    ImportEntryWrapper* libWr = dynamic_cast<ImportEntryWrapper*> (imports->addEntry(NULL));
    if (libWr == NULL) return NULL;

    const size_t PADDING = libWr->getSize();
    storageOffset = imports->getOffset() +  imports->getContentSize() + PADDING;

    offset_t nameOffset = storageOffset;

    if (pe->setStringValue(storageOffset, name) == false) {
        printf("Failed to fill lib  name\n");
        return NULL;
    }
    storageOffset += name.length() + PADDING;

    offset_t firstThunk = pe->convertAddr(storageOffset, Executable::RAW, Executable::RVA);

    libWr->setNumValue(ImportEntryWrapper::FIRST_THUNK, firstThunk);
    libWr->setNumValue(ImportEntryWrapper::ORIG_FIRST_THUNK, firstThunk);
    libWr->wrap();

    storageOffset += PADDING;
    offset_t nameRva = pe->convertAddr(nameOffset, Executable::RAW, Executable::RVA);
    libWr->setNumValue(ImportEntryWrapper::NAME, nameRva);

    return libWr;
}

bool StubMaker::addMissingFunctions(PEFile *pe, ImportsLookup &funcMap, bool tryReuse)
{
    offset_t storageOffset = 0;
    QString library = "Kernel32.dll";

    ImportEntryWrapper* libWr = addLibrary(pe, library, storageOffset);
    if (libWr == NULL) {
        //printf("Adding library failed!\n");
        return false;
    }
    ImportDirWrapper* imports = dynamic_cast<ImportDirWrapper*> (pe->getWrapper(PEFile::WR_DIR_ENTRY + pe::DIR_IMPORT));
    if (imports == NULL) {
        printf("Cannot fetch imports!\n");
        return false;
    }

    //TODO: add missing without relying on previous...
    size_t prevEntry = imports->getEntriesCount() - 2;
    ImportEntryWrapper* prevWr = dynamic_cast<ImportEntryWrapper*> (imports->getEntryAt(prevEntry));
    if (prevWr == NULL) return false;

    size_t prevWrCount = prevWr->getEntriesCount();
    if (prevWrCount == 0) {
        return false;
    }
    ImportedFuncWrapper* prevLast = dynamic_cast<ImportedFuncWrapper*> (prevWr->getEntryAt(prevWrCount - 1));
    if (prevLast == NULL) return false;
    //----
    const size_t fNum = 2;
    const QString fNames[] = {"LoadLibraryA", "GetProcAddress" };
    size_t entrySize = prevLast->getSize();

    storageOffset += (fNum + 1) * entrySize;

    bool isOk = true;
    for ( size_t i = 0; i < fNum; i++) {

        if (tryReuse && funcMap.findThunk(library, fNames[i]) != INVALID_ADDR) {
            //Function exist, no need to add...
            continue;
        }
        if (addFunction(pe, libWr, prevLast, fNames[i], storageOffset) == false) {
            isOk = false;
            break;
        }
    }
    imports->reloadMapping();
    return isOk;
}

bool StubMaker::makeThunksWriteable(PEFile *pe, FuncReplacements* funcRepl)
{
    ImportDirWrapper* imps = dynamic_cast<ImportDirWrapper* >( pe->getDataDirEntry(pe::DIR_IMPORT));
    if (imps == NULL) return false;

    QList<offset_t> thunks = (funcRepl) ? funcRepl->getThunks() : imps->getThunksList();
    //TODO: optimize it

    for (size_t i = 0; i < thunks.size(); i++) {
        offset_t thunk = thunks[i];
        SectionHdrWrapper *sHdr = pe->getSecHdrAtOffset(thunk, Executable::RVA, true);
        if (sHdr == NULL) continue;
        DWORD oldCharact = sHdr->getCharacteristics();
        sHdr->setCharacteristics(oldCharact | 0xC0000000);
    }
    return true;
}

size_t StubMaker::calcNewImportsSize(PEFile *pe, size_t addedFuncCount)
{
    ImportDirWrapper* imports = dynamic_cast<ImportDirWrapper*> (pe->getWrapper(PEFile::WR_DIR_ENTRY + pe::DIR_IMPORT));
    IMAGE_DATA_DIRECTORY* ddir = pe->getDataDirectory();
    size_t impDirSize = ddir[pe::DIR_IMPORT].Size;
    size_t impSize = imports->getContentSize();
    if (impDirSize > impSize) impSize = impDirSize;

    size_t impNewSize = impSize;
    impNewSize += sizeof(IMAGE_IMPORT_DESCRIPTOR) * (addedFuncCount + 1); //append new functions + one as padding
    return impNewSize;
}

bool StubMaker::makeStub(PEFile *pe, ImportsLookup &funcMap, FuncReplacements &funcRepl, const StubSettings &settings)
{
    try {
        if (pe == NULL) {
            return false;
        }
        ExeNodeWrapper* sec = dynamic_cast<ExeNodeWrapper*>(pe->getWrapper(PEFile::WR_SECTIONS));
        if (sec == NULL) {
            return false;
        }

        bufsize_t startOffset = 0;
        bufsize_t stubOffset = 0;
        bufsize_t TABLE_PADDING = 0x100;

        Stub* stb = StubMaker::makeStub(pe);

        size_t stubSize = stb->getSize();
        bufsize_t stubSectionSize = StubMaker::calcDataStoreSize(funcRepl) + stubSize + TABLE_PADDING;

        bool mustRewriteImports = false;
        size_t missingCount = StubMaker::countMissingImports(funcMap);

        //Check if adding imports is required
        if ( missingCount != 0 || (settings.reuseImports == false)) {
            //TODO: try to add to existing lib...
            //TODO: if failed: try to add to existing Import Table...
            //if failed: include import dir in the new section size...
            mustRewriteImports = true;

            stubOffset = calcNewImportsSize(pe, missingCount) + TABLE_PADDING;
            stubSectionSize += stubOffset;
        }

        SectionHdrWrapper *stubHdr = NULL;
        if (settings.getAddNewSection() && pe->canAddNewSection()) {
            stubHdr = pe->addNewSection("stub", stubSectionSize);
            if (!stubHdr) {
                printf("Failed adding section...\n");
            }
        }

        if (stubHdr == NULL) {
            stubHdr = pe->getLastSection();
            if (stubHdr == NULL) {
                printf("Cannot fetch last section!\n");
                return false;
            }

            // resize section
            offset_t secROffset = stubHdr->getContentOffset(Executable::RAW, true);
            startOffset = pe->getContentSize() - secROffset;
            startOffset += SEC_PADDING;
            stubHdr = pe->extendLastSection(stubSectionSize + SEC_PADDING);
            if (stubHdr == NULL) {
                printf("Cannot fetch last section!\n");
                return false;
            }
        }
        DWORD oldCharact = stubHdr->getCharacteristics();

        stubHdr->setCharacteristics(oldCharact | 0xE0000000);
        stubOffset += startOffset;

        //fetch again after modification...
        sec = dynamic_cast<ExeNodeWrapper*>(pe->getWrapper(PEFile::WR_SECTIONS));

        if (sec == NULL) {
            printf("Cannot fetch sections wrapper\n");
            return false;
        }
        size_t stubSecId = sec->getEntriesCount() - 1;
        //printf("Last section: %d\n", stubSecId);

        const offset_t SEC_RVA = stubHdr->getContentOffset(Executable::RVA);
        //printf("Last sectiont RVA = %llx\n", SEC_RVA );
        const offset_t START_RVA  = SEC_RVA + startOffset;
        //printf("Start writing at : %llx\n", START_RVA );

        if (mustRewriteImports) {
            bool added = false;
            try {
                if (pe->moveDataDirEntry(pe::DIR_IMPORT, START_RVA, Executable::RVA)) {
                    added = addMissingFunctions(pe, funcMap, settings.reuseImports);
                    if (added == false) {
                        printf("Unable to add all the required functions!\n");
                        return false;
                    }
                    makeThunksWriteable(pe);
                    added = true;
                }
            } catch (ExeException &e) {
                printf("Not moved, because: %s\n", e.what());
            }
            if (!added) {
                printf("Unable to add all the required functions#2\n");
                return false;
            }
        }

        pe->unbindImports();
        ImportDirWrapper* imps = dynamic_cast<ImportDirWrapper* >( pe->getDataDirEntry(pe::DIR_IMPORT));
        funcMap.wrap(imps);

        BufferView* stubSecView = pe->createSectionView(stubSecId);
        if (stubSecView == NULL) {
            return false;
        }
        size_t DATA_OFFSET = stubOffset + stubSize;
        offset_t dataRva = SEC_RVA + stubOffset + stubSize;
        offset_t newEntryPoint = SEC_RVA + stubOffset;

        ByteBuffer* dataStore = StubMaker::makeDataStore(dataRva, funcRepl);
        if (StubMaker::setStubParams(stb, pe, newEntryPoint, dataRva, funcMap) == false) {
            printf("Making stub failed: loaderStub == NULL\n");
        }

        ByteBuffer *loaderStub = stb->createStubBuffer();
        delete stb;
        stb = NULL;

        bool hasAdded = false;

        if (stubSecView->pasteBuffer(stubOffset, loaderStub, false)) {
            pe->setEntryPoint(newEntryPoint, Executable::RVA);
            hasAdded = true;
        } else {
            printf("pasting buffer failed!\n");
        }

        if (hasAdded) {
            if (stubSecView->pasteBuffer(DATA_OFFSET, dataStore, false)) {
                printf("pasted data");
                hasAdded = true;
            }
        }

        delete loaderStub;
        delete dataStore;
        delete stubSecView;

        return hasAdded;

    } catch (CustomException &e) {
        printf ("Not added, because: %s\n", e.what());
    }

    return false;
}
