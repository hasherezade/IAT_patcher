#pragma once
#include <QFile>
#include <bearparser/core.h>

#include "ImportsLookup.h"

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

    size_t load(QString &fileName);
    size_t save(QString &fileName);
    size_t dropInvalidThunks(ImportsLookup &lookup);

    bool defineReplacement(offset_t thunk, FuncDesc newFunc);
    bool undefReplacement(offset_t thunk);

    size_t size() { return m_replacements.size(); }
    QList<offset_t> getThunks() { return m_replacements.keys(); }

    FuncDesc getAt(offset_t thunk);
    bool hasAt(offset_t thunk);

protected:
    bool _undefReplacement(offset_t thunk);
    bool _defineReplacement(offset_t thunk, FuncDesc newFunc);

    QMap<offset_t, FuncDesc> m_replacements;
};

