#pragma once
#include <QFile>
#include <QMap>
#include <bearparser/pefile.h>

typedef QMap<QString, offset_t> FuncToThunk;

class ImportsLookup : public QObject
{
    Q_OBJECT

public:
    ImportsLookup() { }

    void wrap(ImportDirWrapper* imports);

    QString thunkToLibName(offset_t thunk) { return (m_Imports) ? m_Imports->thunkToLibName(thunk) : ""; }
    QString thunkToFuncName(offset_t thunk) { return (m_Imports) ? m_Imports->thunkToFuncName(thunk) : ""; }
    
    bool hasThunk(const offset_t &thunk) { return  m_Thunks.contains(thunk); }
    bool hasLib(QString libName);
    bool hasFunc(QString funcName);
    offset_t findThunk(QString libName, QString funcName);

    size_t countLibs() { return m_libs.size(); }
    size_t countImps() { return (m_Imports == NULL) ? 0 : m_Thunks.size(); }
    offset_t thunkAt(size_t impNum) { return impNum > m_Thunks.size() ? INVALID_ADDR : m_Thunks.at(impNum); }

protected:
    void wrapFunctions();
    bool addFunc(QString libName, QString funcName, offset_t thunk);

    QMap<QString, FuncToThunk> m_libs;
    FuncToThunk m_func;
    QList<offset_t> m_Thunks;

    ImportDirWrapper* m_Imports;
};
