#include "FuncReplacements.h"

bool FuncReplacements::defineReplacement(offset_t thunk, FuncDesc newFunc)
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

size_t FuncReplacements::calcBookedSpace()
{
        const size_t PADDING = 1;
        size_t requiredLen = PADDING;

        QMap<offset_t, FuncDesc>::Iterator itr;
        for (itr = m_replacements.begin(); itr != m_replacements.end(); itr++) {
            FuncDesc &desc = itr.value();
            requiredLen += desc.size();
            requiredLen += PADDING;
        }
        return requiredLen;
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
