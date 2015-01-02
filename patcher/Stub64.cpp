#include "Stub64.h"
#include "Stub64Data.h"

bufsize_t Stub64::getSize() const
{
    return Stub64Data::size;
}

const unsigned char* Stub64::getData() const
{
    return Stub64Data::data;
}

offset_t Stub64::getSignatureStart() const
{
    return Stub64Data::pointers[2] + 1 + sizeof(DWORD);
}

offset_t Stub64::getSignatureEnd() const
{
    return Stub64Data::pointers[3];
}

void Stub64::createParams()
{
    this->m_params[NEW_EP] = new StubParam(NEW_EP, Stub64Data::pointers[0], 3, sizeof(DWORD));
    this->m_params[DATA_RVA] = new StubParam(DATA_RVA, Stub64Data::pointers[1], 3, sizeof(DWORD));
    this->m_params[OEP] = new StubParam(OEP, Stub64Data::pointers[2], 1, sizeof(DWORD) , NEW_EP);

    this->m_params[FUNC_LOAD_LIB_RVA] = new StubParam(FUNC_LOAD_LIB_RVA, Stub64Data::pointers[3], 2, sizeof(DWORD), NEW_EP);
    this->m_params[FUNC_GET_MODULE_RVA] = new StubParam(FUNC_GET_MODULE_RVA, Stub64Data::pointers[4], 2, sizeof(DWORD), NEW_EP);

}
