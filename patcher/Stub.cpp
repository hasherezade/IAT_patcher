#include "Stub.h"

bool StubParam::insertIntoBuffer(AbstractByteBuffer* buf)
{
    if (buf == NULL) return false;

    offset_t fullOffset = this->m_bigOffset + this->m_smallOffset;
    bool isOk = buf->setNumValue(fullOffset, this->m_valueSize, this->m_value);
    //TODO - verify if setting succeseded?
    return true;
}

bool StubParam::readFromBuffer(AbstractByteBuffer* buf)
{
    if (buf == NULL) return false;

    offset_t fullOffset = this->m_bigOffset + this->m_smallOffset;
    bool isOk = false;

    offset_t value = buf->getNumValue(fullOffset, this->m_valueSize, &isOk);
    if (!isOk) return false;

    this->m_value = value;
    return true;
}
//-----

ByteBuffer* Stub::createStubBuffer()
{
    ByteBuffer* buf = bufferStubData();
    if (buf == NULL) return NULL;

    if (this->fillParams(buf) == false) {
        printf("Unable to fill params!\n");
        delete buf;
        return NULL;
    }
    return buf;
}

ByteBuffer* Stub::bufferStubData()
{
    const unsigned char* originalData = this->getData();
    const bufsize_t stubSize = this->getSize();

    unsigned char *stubData = new unsigned char[stubSize];
    memcpy(stubData, originalData, stubSize);
    ByteBuffer *stubBuf = new ByteBuffer(stubData, stubSize, 0);
    delete []stubData;
    return stubBuf;
}

bool Stub::fillParam(size_t id, AbstractByteBuffer* buf)
{
    StubParam *param = this->getParam(id);
    if (param == NULL) {
        printf("Param: %d not found!\n", id);
        return false;
    }
    if (param->m_relativeToId != param->m_id) {
        offset_t dif = this->paramDistance(param->m_relativeToId, id);
        param->setValue(dif);
    }
    return param->insertIntoBuffer(buf);
}


bool Stub::fillParams(AbstractByteBuffer* buf)
{
    std::map<size_t, StubParam*>::iterator itr;
    for (itr = m_params.begin(); itr != m_params.end(); itr++) {
        size_t paramId = itr->first;
        if (fillParam(paramId, buf) == false) {
            return false;
        }
    }
    return true;
}

offset_t Stub::getAbsoluteValue(StubParam *param)
{
    if (param == NULL) {
        return INVALID_ADDR;
    }

    offset_t myValue = param->m_value;

    if (param->m_relativeToId != param->m_id) {
        StubParam *relParam = this->getParam(param->m_relativeToId);
        if (relParam == NULL) return INVALID_ADDR;

        offset_t dif = ((int32_t) param->m_value);
        offset_t relValue = relParam->m_value;

        myValue = relParam->m_value + dif + param->getTotalOffset() + param->getValueSize();
    }
    return myValue;
}

bool Stub::readParam(size_t id, AbstractByteBuffer* buf)
{
    StubParam *param = this->getParam(id);
    if (param == NULL) {
        printf("Param: %d not found!\n", id);
        return false;
    }
    if (param->readFromBuffer(buf) == false) return false;

    offset_t myValue = getAbsoluteValue(param);
    if (myValue == INVALID_ADDR) return false;

    param->setValue(myValue);
    return true;
}


bool Stub::readParams(AbstractByteBuffer* buf)
{
    std::map<size_t, StubParam*>::iterator itr;
    for (itr = m_params.begin(); itr != m_params.end(); itr++) {
        size_t paramId = itr->first;
        if (!readParam(paramId, buf)) {
            return false;
        }
    }
    return true;
}

void Stub::deleteParams()
{
    bool isOk = false;
    std::map<size_t, StubParam*>::iterator itr;
    for (itr = m_params.begin(); itr != m_params.end(); itr++) {
        StubParam* param = itr->second;
        delete param;
    }
    m_params.clear();
}

bool Stub::containsStub(AbstractByteBuffer *buf) const
{
    const unsigned char* stubData = this->getData();
    const bufsize_t stubSize = this->getSize();
    const offset_t signStart = getSignatureStart();
    const offset_t signEnd = getSignatureEnd();

    /* sanity check */
    if (buf == NULL) return false;
    if (buf->getContentSize() < stubSize) return false;

    unsigned char* bufData = (unsigned char*) buf->getContent();
    if (bufData == NULL) return false;

    /* sanity check */
    if (stubData == NULL || signStart > signEnd || signEnd >= stubSize) {
        printf("Malformated stub!");
        return false;
    }

    bufData += signStart;

    const unsigned char* cmpData = stubData + signStart;
    const size_t cmpSize = signEnd - signStart;

    int res = memcmp(cmpData, bufData, cmpSize);
    if (res == 0) {
        return true;
    }
    return false;
}
