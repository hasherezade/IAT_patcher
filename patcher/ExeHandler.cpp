#include "ExeHandler.h"

bool ExeHandler::defineReplacement(offset_t thunk, FuncDesc newFunc)
{
    bool ret = m_Repl.hook(thunk, newFunc);
    if (ret) hasUnapplied = true;
    emit stateChanged();
    return ret;
}