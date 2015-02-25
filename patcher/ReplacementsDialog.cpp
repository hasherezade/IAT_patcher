#include "ReplacementsDialog.h"

ReplacementsDialog::ReplacementsDialog(QWidget *parent)
     : QDialog(parent)
{
    this->m_uiReplacements = new Ui_Replacements();
    this->m_uiReplacements->setupUi(this);
    setAcceptDrops(true);

    m_libsModel = new LibsModel(this->m_uiReplacements->libraryCombo);
    m_functModel = new FunctionsModel(this->m_uiReplacements->functionCombo);

    this->m_uiReplacements->libraryCombo->setModel(m_libsModel);
    this->m_uiReplacements->functionCombo->setModel(m_functModel);

    m_libsModel->setLibraries(&m_libInfos);
    m_functModel->setLibraries(&m_libInfos);

    connect(this->m_uiReplacements->okCancel_buttons, SIGNAL(rejected()), this, SLOT(hide()));
    connect(this->m_uiReplacements->okCancel_buttons, SIGNAL(accepted()), this, SLOT(requestReplacement()));
    connect(m_uiReplacements->addLibraryButton, SIGNAL(clicked()), this, SLOT(openLibrary()));

    connect(m_uiReplacements->libraryCombo, SIGNAL(currentIndexChanged(int)), m_functModel, SLOT(on_currentndexChanged(int)));
    connect(this, SIGNAL(parseLibrary(QString&)), &m_libParser, SLOT(on_parseLibrary(QString&)) );
    connect(&m_libParser, SIGNAL(infoCreated(LibraryInfo*)), &m_libInfos, SLOT(addElement(LibraryInfo *)) );
}

void ReplacementsDialog::displayFuncToReplace(offset_t thunk, QString libName, QString funcName)
{
    this->m_uiReplacements->thunkLabel->setText(QString::number(thunk, 16));
    this->m_uiReplacements->libToReplaceLabel->setText(libName+"."+funcName);
}

void ReplacementsDialog::displayReplacement(QString libName, QString funcName)
{
    this->m_uiReplacements->libraryEdit->setText(libName);
    this->m_uiReplacements->functionEdit->setText(funcName);
}

void ReplacementsDialog::displayReplacement(FuncDesc &desc)
{
    QString libName;
    QString funcName;
    FuncUtil::parseFuncDesc(desc, libName, funcName);
    this->displayReplacement(libName, funcName);
}

QString ReplacementsDialog::getLibName()
{
    int tabNum = this->m_uiReplacements->tabWidget->currentIndex();

    switch (tabNum) {
        case TAB_EDIT :
            return this->m_uiReplacements->libraryEdit->text();
        case TAB_CHOSE :
            return this->m_uiReplacements->libraryCombo->currentText();
    }
    return "";
}

QString ReplacementsDialog::getFuncName()
{
    int tabNum = this->m_uiReplacements->tabWidget->currentIndex();

    switch (tabNum) {
        case TAB_EDIT :
            return this->m_uiReplacements->functionEdit->text();
        case TAB_CHOSE :
            return this->m_uiReplacements->functionCombo->currentText();
    }
    return "";
}

void ReplacementsDialog::openLibrary()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open executable"),
        QDir::homePath(),
        "DLL Files (*.dll);;All files (*.*)"
        );

    if (fileName != "") {
        emit parseLibrary(fileName);
    }
}

void ReplacementsDialog::requestReplacement()
{
    emit setReplacement(getLibName(), getFuncName());
}

void ReplacementsDialog::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void ReplacementsDialog::dropEvent(QDropEvent* ev)
{
    this->m_uiReplacements->tabWidget->setCurrentIndex(TAB_CHOSE);
    QList<QUrl> urls = ev->mimeData()->urls();
    QList<QUrl>::Iterator urlItr;
    QCursor cur = this->cursor();
    this->setCursor(Qt::BusyCursor);

    for (urlItr = urls.begin() ; urlItr != urls.end(); urlItr++) {
        QString fileName = urlItr->toLocalFile();
        if (fileName == "") continue;
        emit parseLibrary(fileName);
    }
    this->setCursor(cur);
}
