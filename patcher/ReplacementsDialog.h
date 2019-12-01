#pragma once

#include <QtWidgets>
#include <QFile>
#include <bearparser/core.h>

#include "Executables.h"
#include "ui_replacements.h"

#include "dllparse/LibraryParser.h"
#include "dllparse/LibsModel.h"
#include "dllparse/FunctionsModel.h"

class ReplacementsDialog : public QDialog
{
    Q_OBJECT

signals:
    void setReplacement(QString libName, QString funcName);
    void parseLibrary(QString &path);

public:
    enum TABS
    {
        TAB_EDIT = 0,
        TAB_CHOSE = 1,
        COUNT_TABS
    };

    explicit ReplacementsDialog(QWidget *parent = 0);
    ~ReplacementsDialog() { }

    void displayFuncToReplace(offset_t thunk, QString libName, QString funcName);
    void displayReplacement(QString libName, QString funcName);
    void displayReplacement(FuncDesc &desc);

    QString getLibName();
    QString getFuncName();

    /* events */
    void dragEnterEvent(QDragEnterEvent *ev);
    void dropEvent(QDropEvent* ev);

private slots:
    void requestReplacement();
    void openLibrary();

private:
    Ui_Replacements* m_uiReplacements;

    LibsModel *m_libsModel;
    FunctionsModel *m_functModel;
    LibInfos m_libInfos;
    LibraryParser m_libParser;
};
