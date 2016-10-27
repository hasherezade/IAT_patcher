#include "ExeHandlerLoader.h"

bool ExeHandlerLoader::parse(QString &fileName)
{
    bool isLoaded = false;
    ExeHandler *exeHndl = NULL;
    try {
        const bufsize_t MINBUF = 0x200;
        AbstractByteBuffer *buf = new FileBuffer(fileName, MINBUF);
        if (buf == NULL) {
            return false;
        }
        ExeFactory::exe_type exeType = ExeFactory::findMatching(buf);
        if (exeType == ExeFactory::NONE) {
            delete buf;
            return false;
        }

        Executable *exe = ExeFactory::build(buf, exeType);
        exeHndl = new ExeHandler(buf, exe);
        if (exeHndl) {
            isLoaded = true;
        }

    } catch (CustomException &e) { }

    emit loaded(exeHndl);
    return isLoaded;
}
//----

