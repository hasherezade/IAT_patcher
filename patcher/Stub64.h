#pragma once
#include "Stub.h"

class Stub64 : public Stub {
public:
    Stub64() : Stub() { createParams(); }
    virtual bufsize_t getSize() const;
    virtual const unsigned char* getData() const;

    virtual offset_t getSignatureStart() const;
    virtual offset_t getSignatureEnd() const;

protected:
    virtual void createParams();
};
