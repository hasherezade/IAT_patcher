#pragma once
#include <QFile>
#include <bearparser/pefile.h>

class FunctionInfo
{
public:
    FunctionInfo(QString v_name, bool v_isByOrdinal=false)
        : name(v_name), isByOrdinal(v_isByOrdinal) {}
    virtual ~FunctionInfo() {}

protected:
    QString name;
    bool isByOrdinal;
friend class LibraryInfo;
};

class LibraryInfo : public QObject
{
    Q_OBJECT

signals:
    void stateChanged();

public:
    LibraryInfo(QString filename, QObject* parent = NULL) 
        : QObject(parent), fileName(filename)
    {
        QFileInfo inputInfo(filename);
        libName = inputInfo.fileName();
    }

    ~LibraryInfo() {}

    QString getFileName() { return fileName; }
    QString getLibName() { return libName; }
    const QString getFuncNameAt(int i) { if (i > functions.size()) return ""; return functions.at(i).name; }
    bool isFunctionNamed(int i) const { if (i > functions.size()) return false; return !(functions.at(i).isByOrdinal); }
    size_t getFunctionsCount() const { return functions.size(); }

protected:
    QString libName;
    QString fileName;
    QList<FunctionInfo> functions;

friend class LibraryParser;
};

class LibInfos : public QObject
{
Q_OBJECT

signals:
    void listChanged();

public slots:
    void addElement(LibraryInfo *info) { if (_addElement(info)) emit listChanged(); }
    void removeElement(LibraryInfo *info) { if (_removeElement(info)) emit listChanged(); }
    void clear() { _clear(); emit listChanged(); }

public:
    size_t size();
    LibraryInfo* at(size_t index);

    QStringList listLibs();

protected slots:
    void onChildStateChanged() { emit listChanged(); }

private:
    bool _addElement(LibraryInfo *info);
    bool _removeElement(LibraryInfo *info);
    void _clear();

    QList<LibraryInfo *> m_Libs;
    QMap<QString, LibraryInfo*> m_NameToLibInfo;
    QMutex m_listMutex;
};
