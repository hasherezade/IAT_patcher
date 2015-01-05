#include "FuncReplacements.h"

bool FuncReplacements::hook(offset_t thunk, FuncDesc newFunc)
{
    QMap<offset_t, FuncDesc>::Iterator found = m_replacements.find(thunk);
    bool exist = found != m_replacements.end();

    if (newFunc.size() > 0) {
        if (FuncUtil::validateFuncDesc(newFunc) == false) {
            return false;
        }
        m_replacements[thunk] = newFunc;
        emit stateChanged();
        return true;
    }
    if (exist) {
        m_replacements.erase(found);
        emit stateChanged();
        return true;
    }
    return false;
}

FuncDesc FuncReplacements::getAt(offset_t thunk)
{
    if (hasAt(thunk) == false) return "";
    return m_replacements[thunk];
}

bool FuncReplacements::hasAt(offset_t thunk)
{
    return m_replacements.find(thunk) != m_replacements.end();
}
