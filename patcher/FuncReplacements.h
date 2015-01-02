#pragma once
#include <QFile>
#include <bearparser.h>

typedef QString FuncDesc;

class FuncUtil {
public:
    static bool validateFuncDesc(FuncDesc library)
    {
        int indx = library.lastIndexOf('.');
        if (indx == -1) {
            return false;
        }

        QString libName = library.left(indx);
        QString funcName = library.mid(indx + 1);
        if (libName.length() == 0 || funcName == 0) return false;
        return true;
    }

    static bool parseFuncDesc(FuncDesc library, QString &libName, QString &funcName)
    {
        int indx = library.lastIndexOf('.');
        if (indx == -1) {
            //printf("Invalid library format!\n");
            return false;
        }

        libName = library.left(indx);
        funcName = library.mid(indx + 1);
        if (libName.length() == 0 || funcName == 0) return false;
        return true;
    }
};

class FuncReplacements : public QObject
{
Q_OBJECT

signals:
    void stateChanged();

public:
    FuncReplacements() { }
    ~FuncReplacements() { }

    bool hook(offset_t thunk, FuncDesc newFunc);
    FuncDesc getAt(offset_t thunk);
    bool hasAt(offset_t thunk);

    size_t size() { return m_replacements.size(); }
    QList<offset_t> getThunks() { return m_replacements.keys(); }

    size_t calcBookedSpace()
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

protected:
    QMap<offset_t, FuncDesc> m_replacements;

};

//--------------
typedef QMap<QString, offset_t> FuncToThunk;

class FunctionsMap : public QObject
{
    Q_OBJECT

public:
    FunctionsMap() { }

    void wrap(ImportDirWrapper* imports)
    {
        m_Thunks.clear();
        m_Imports = imports;
        if (m_Imports != NULL) {
            m_Thunks = m_Imports->getThunksList();
        }
        wrapFunctions();
        /*
        offset_t thunk = findThunk("Kernel32.dll","LoadLibraryA");
        if (thunk != INVALID_ADDR) {
            printf("Got it at %llx\n", thunk);
        }*/
    }

    QString thunkToLibName(offset_t thunk) { return (m_Imports) ? m_Imports->thunkToLibName(thunk) : NULL; }
    QString thunkToFuncName(offset_t thunk) { return (m_Imports) ? m_Imports->thunkToFuncName(thunk) : NULL; }

    bool hasLib(QString libName)
    {
        libName = libName.toUpper();
        QMap<QString, FuncToThunk>::iterator itr = m_libs.find(libName);
        if (itr == m_libs.end()) return false;
        return true;
    }

    bool hasFunc(QString funcName)
    {
        funcName = funcName.toUpper();
        FuncToThunk::iterator itr = m_func.find(funcName);
        if (itr == m_func.end()) return false;
        return true;
    }

    offset_t findThunk(QString libName, QString funcName)
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

    size_t countLibs() { return m_libs.size(); }
    size_t countImps() { return (m_Imports == NULL) ? 0 : m_Thunks.size(); }
    offset_t thunkAt(size_t impNum) { return impNum > m_Thunks.size() ? INVALID_ADDR : m_Thunks.at(impNum); }

protected:
    void wrapFunctions()
    {
        for (size_t i = 0; i < m_Thunks.size(); i++) {
            offset_t thunk = m_Thunks[i];
            if (thunk == 0 || thunk == INVALID_ADDR) continue;

            QString lib = thunkToLibName(thunk);
            QString func = thunkToFuncName(thunk);
            addFunc(lib, func, thunk);
        }
    }
    bool addFunc(QString libName, QString funcName, offset_t thunk)
    {
        if (libName.size() == 0 || funcName.size() == 0 || thunk == INVALID_ADDR) return false;

        funcName = funcName.toUpper();
        libName = libName.toUpper();

        m_libs[libName].insert(funcName, thunk);
        m_func.insert(funcName, thunk);
        return true;
    }


    QMap<QString, FuncToThunk> m_libs;
    FuncToThunk m_func;

    ImportDirWrapper* m_Imports;
    QList<offset_t> m_Thunks;
};
