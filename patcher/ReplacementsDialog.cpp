#include "ReplacementsDialog.h"

ReplacementsDialog::ReplacementsDialog(QWidget *parent)
     : QDialog(parent)
{
    this->m_uiReplacements = new Ui_Replacements();
    this->m_uiReplacements->setupUi(this);

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
    connect(&m_libParser, SIGNAL(infoCreated(LibraryInfo*)),  &m_libInfos, SLOT(addElement(LibraryInfo *)) );
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
    QString libName;

    int tabNum = this->m_uiReplacements->tabWidget->currentIndex();
    if (tabNum == 0) {
        libName = this->m_uiReplacements->libraryEdit->text();
    } else if (tabNum == 1) {
        libName = this->m_uiReplacements->libraryCombo->currentText();
    }
    return libName;
}

QString ReplacementsDialog::getFuncName()
{
    QString funcName;

    int tabNum = this->m_uiReplacements->tabWidget->currentIndex();
    if (tabNum == 0) {
        funcName = this->m_uiReplacements->functionEdit->text();
    } else if (tabNum == 1) {
        funcName = this->m_uiReplacements->functionCombo->currentText();
    }
    return funcName;
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
