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
