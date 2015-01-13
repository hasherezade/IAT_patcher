#pragma once
#include "Stub.h"

class Stub32 : public Stub {
public:
    Stub32() : Stub() { createParams(); }

    virtual bufsize_t getSize() const;
    virtual const unsigned char* getData() const;

    virtual offset_t getSignatureStart() const;
    virtual offset_t getSignatureEnd() const;
    virtual offset_t getDatastoreOffset() const;

protected:
    virtual void createParams();
};
