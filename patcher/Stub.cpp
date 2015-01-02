#include "Stub.h"

bool StubParam::insertIntoBuffer(ByteBuffer* buf)
{
    if (buf == NULL) return false;

    offset_t fullOffset = this->m_bigOffset + this->m_smallOffset;
    BYTE *toFill = buf->getContentAt(fullOffset, this->m_valueSize);
    if (toFill == NULL) return false;

    memcpy(toFill, &m_value, m_valueSize);
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

bool Stub::fillParam(size_t id, ByteBuffer* buf)
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

bool Stub::fillParams(ByteBuffer* buf)
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
