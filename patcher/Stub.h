#pragma once
#include <bearparser.h>

#include "Executables.h"
#include "FuncReplacements.h"


class StubParam
{
public:
    StubParam(size_t id, offset_t bigOffset, offset_t smallOffset, size_t valueSize, size_t relId) 
        : m_id(id), m_bigOffset(bigOffset), m_smallOffset(smallOffset), m_valueSize(valueSize), m_value(0), m_relativeToId(relId) {}

    StubParam(size_t id, offset_t bigOffset, offset_t smallOffset, size_t valueSize) 
        : m_id(id), m_bigOffset(bigOffset), m_smallOffset(smallOffset), m_valueSize(valueSize), m_value(0), m_relativeToId(id) {}

    virtual ~StubParam() { /*printf("Deleting param id = %d\n", m_id);*/ }

    void setValue(offset_t value) { this->m_value = value; }
    offset_t getValue() { return m_value; }

    bool insertIntoBuffer(ByteBuffer* buf);
    offset_t getTotalOffset() { return m_bigOffset + m_smallOffset; }
    offset_t getBigOffset() { return m_bigOffset; }
    size_t getValueSize() { return m_valueSize; }

protected:
    size_t m_id;
    size_t m_relativeToId;
    offset_t m_bigOffset; // offset of instruction in code
    offset_t m_smallOffset;

    size_t m_valueSize;
    offset_t m_value;

friend class Stub;
};

class Stub {

public:
    enum STUB_PARAMS {
        NEW_EP, //LABEL_newEP
        DATA_RVA, //LABEL_dataRVA
        OEP, //LABEL_oldEP 
        FUNC_GET_MODULE_RVA,
        FUNC_LOAD_LIB_RVA
    };
    
    Stub() { }
    virtual ~Stub() { deleteParams(); }

    virtual bufsize_t getSize() const = 0;
    virtual const unsigned char* getData() const = 0;

    virtual offset_t getSignatureStart() const = 0;
    virtual offset_t getSignatureEnd() const = 0;

    ByteBuffer* createStubBuffer();
    bool containsStub(AbstractByteBuffer *buf) const;

    bool setParam(size_t id, offset_t value) {
        if (this->m_params.find(id) == m_params.end()) {
            printf("No param with ID = %d, cannot set the val = %llx\n", id, value);
            return false;
        }
        this->m_params[id]->setValue(value);
        return true;
    }

protected:

    StubParam* getParam(size_t id) const {
        std::map<size_t, StubParam*>::const_iterator itr = this->m_params.find(id);
        if (itr == m_params.end()) return NULL;
        return itr->second;
    }

    offset_t paramDistance(size_t idFrom, size_t idTo) {
        StubParam *param = this->getParam(idTo);
        StubParam *newEpParam = this->getParam(idFrom);

        if (param == NULL || newEpParam == NULL) return INVALID_ADDR;

        offset_t newEntry = newEpParam->getValue() + newEpParam->getValueSize();
        offset_t oldEntry = param->getValue();

        bufsize_t oldEntryOffset = param->getTotalOffset(); //toOffset

        offset_t dif = ((newEntry + oldEntryOffset) - oldEntry) * (-1);
        return dif;
    }

    virtual bool fillParam(size_t id, ByteBuffer* buf);
    virtual bool fillParams(ByteBuffer* buf);

    ByteBuffer* bufferStubData();

    virtual void createParams() = 0;
    virtual void deleteParams();

    std::map<size_t, StubParam*> m_params;
};