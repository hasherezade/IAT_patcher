#include "ImportsLookup.h"

void ImportsLookup::wrap(ImportDirWrapper* imports)
{
    m_Thunks.clear();
    m_Imports = imports;
    if (m_Imports != NULL) {
        m_Thunks = m_Imports->getThunksList();
    }
    wrapFunctions();
}

bool ImportsLookup::hasLib(QString libName)
{
    libName = libName.toUpper();
    QMap<QString, FuncToThunk>::iterator itr = m_libs.find(libName);
    if (itr == m_libs.end()) return false;
    return true;
}

bool ImportsLookup::hasFunc(QString funcName)
{
    funcName = funcName.toUpper();
    FuncToThunk::iterator itr = m_func.find(funcName);
    if (itr == m_func.end()) return false;
    return true;
}

offset_t ImportsLookup::findThunk(QString libName, QString funcName)
{
    libName = libName.toUpper();
    funcName = funcName.toUpper();

    QMap<QString, FuncToThunk>::iterator itr = m_libs.find(libName);
    if (itr == m_libs.end()) return INVALID_ADDR;

    FuncToThunk &func = m_libs[libName];
    FuncToThunk::iterator itr2 = func.find(funcName);
    if (itr2 == func.end()) return INVALID_ADDR;
    return m_func[funcName];
}

/* protected */

void ImportsLookup::wrapFunctions()
{
    for (size_t i = 0; i < m_Thunks.size(); i++) {
        offset_t thunk = m_Thunks[i];
        if (thunk == 0 || thunk == INVALID_ADDR) continue;

        QString lib = thunkToLibName(thunk);
        QString func = thunkToFuncName(thunk);
        addFunc(lib, func, thunk);
    }
}

bool ImportsLookup::addFunc(QString libName, QString funcName, offset_t thunk)
{
    if (libName.size() == 0 || funcName.size() == 0 || thunk == INVALID_ADDR) return false;

    funcName = funcName.toUpper();
    libName = libName.toUpper();

    m_libs[libName].insert(funcName, thunk);
    m_func.insert(funcName, thunk);
    return true;
}
