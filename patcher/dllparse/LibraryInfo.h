#pragma once
#include <QFile>
#include <bearparser.h>

class LibraryInfo : public QObject
{
    Q_OBJECT

signals:
    void stateChanged();

public:
    LibraryInfo(QString filename, QObject* parent = NULL) 
        : QObject(parent), fileName(filename) {}
    ~LibraryInfo(){}
    QString getFileName() { return fileName; }

protected:
    QString fileName;
    QStringList names;

friend class LibraryParser;
friend class FunctionsModel;
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

    QStringList listFiles();

protected slots:
    void onChildStateChanged() { emit listChanged(); }

private:
    bool _addElement(LibraryInfo *info);
    bool _removeElement(LibraryInfo *info);
    void _clear();

    QList<LibraryInfo *> m_Libs;
    QMutex m_listMutex;
};
