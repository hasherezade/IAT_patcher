#include "Stub32.h"
#include "Stub32Data.h"

bufsize_t Stub32::getSize() const
{
    return Stub32Data::size;
}

const unsigned char* Stub32::getData() const
{
    return Stub32Data::data;
}

offset_t Stub32::getSignatureStart() const
{
    return Stub32Data::pointers[2] + 1 + sizeof(DWORD);
}

offset_t Stub32::getSignatureEnd() const
{
    return Stub32Data::pointers[3];
}

offset_t Stub32::getDatastoreOffset() const
{
    return Stub32Data::pointers[DATA_RVA];
}

void Stub32::createParams()
{
    this->m_params[NEW_EP] = new StubParam(NEW_EP, Stub32Data::pointers[0], 1, sizeof(DWORD));
    this->m_params[DATA_RVA] = new StubParam(DATA_RVA, Stub32Data::pointers[1], 1, sizeof(DWORD));
    this->m_params[OEP] = new StubParam(OEP, Stub32Data::pointers[2], 1, sizeof(DWORD) , NEW_EP);

    this->m_params[FUNC_LOAD_LIB_RVA] = new StubParam(FUNC_LOAD_LIB_RVA, Stub32Data::pointers[3], 1, sizeof(DWORD));
    this->m_params[FUNC_GET_MODULE_RVA] = new StubParam(FUNC_GET_MODULE_RVA, Stub32Data::pointers[4], 1, sizeof(DWORD));
}
