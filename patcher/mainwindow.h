#pragma once

#include <QtGui>
#include <QFile>
//#include <bearparser/core.h>

#include "Executables.h"

#include "ui_mainwindow.h"
#include "ui_replacements.h"

#include "ImportsTableModel.h"
#include "InfoTableModel.h"
#include "ExeController.h"
#include "ReplacementsDialog.h"

#define SITE_LINK "http://hasherezade.github.io/IAT_patcher/"
#define ISSUES_LINK "https://github.com/hasherezade/IAT_patcher/issues"
#define VERSION "0.3.5.2 Qt5"

class ThreadCounter : public QObject
{
    Q_OBJECT

signals:
    void counterChanged();

public:
    ThreadCounter() { counter = 0; }

    void inc()
    {
        {
        QMutexLocker lock(&m_Mutex);
        counter++;
        }
        emit counterChanged();
    }

    void dec()
    {
        {
        QMutexLocker lock(&m_Mutex);
        counter--;
        }
        emit counterChanged();
    }
    size_t getCount()
    {
        QMutexLocker lock(&m_Mutex);
        return counter;
    }

protected:
    size_t counter;
    QMutex m_Mutex;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void exeSelected(ExeHandler* exe);
    void exeUpdated(ExeHandler* exe);
    void hookRequested(ExeHandler* exe);

    void thunkSelected(offset_t thunk);
    void replacementAccepted();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    /* events */
    void dragEnterEvent(QDragEnterEvent* ev) { ev->accept(); }
    void dropEvent(QDropEvent* ev);
    void closeEvent(QCloseEvent* ev);

private slots:
    void filterLibs(const QString &str);
    void filterFuncs(const QString &str);

    void takeAction();
    void loadingStatusChanged();
    void onLoadingFailed(QString fileName);

    void selectExe(ExeHandler* exe) 
    { 
        m_ExeSelected = exe;
        emit exeSelected(exe);
    }

    void refreshExeView(ExeHandler* exe);

    void customMenuRequested(QPoint pos);
    void functionsMenuRequested(QPoint pos);
    void onHookRequested(ExeHandler* exeHndl);
    void updateReplacement(QString libName, QString funcName);
    void onExportReplacements(ExeHandler* exeHndl);
    void onImportReplacements(ExeHandler* exeHndl);

    void setThunkSelected(offset_t thunk);

    void onLoaderThreadFinished();
    void rowChangedSlot(QModelIndex, QModelIndex);
    void openExe();
    //---
    void on_pushButton_clicked();
    void on_reloadButton_clicked();
    void on_clearAllButton_clicked() { clear(); }
    void on_hookButton_clicked();
    void on_saveButton_clicked();

    void on_actionAbout_triggered() {  info(); }

private:
    void reloadExe(ExeHandler* exe);
    void removeExe(ExeHandler* exe);
    void saveRequested(ExeHandler* exeHndl);
    void hookExecutable(ExeHandler* exeHndl, StubSettings &settings);
    void info();
    void clear();
    bool parse(QString &fileName);

    QAction* addExeAction(QMenu *customMenu, QString text, ExeController::EXE_ACTION a);
    void makeCustomMenu();
    void makeFunctionsMenu();
    void makeFileMenu();
    void initReplacementsDialog();

    ReplacementsDialog *m_replacementsDialog;
    QMenu *customMenu, *functionsMenu;

    QSortFilterProxyModel *m_filteredImpModel;

    Ui::MainWindow m_ui;

    ImportsTableModel *impModel;
    InfoTableModel *infoModel;
    LibsModel *m_libsModel;
    FunctionsModel *m_functModel;

    ThreadCounter m_LoadersCount;

    Executables m_exes;
    ExeHandler* m_ExeSelected;
    offset_t m_ThunkSelected;
    QLabel urlLabel;

    ExeController exeController;
};


