#include "FuncReplacements.h"

bool FuncReplacements::defineReplacement(offset_t thunk, FuncDesc newFunc)
{
    if (_defineReplacement(thunk, newFunc)) {
        emit stateChanged();
        return true;
    }
    return false;
}

bool FuncReplacements::undefReplacement(offset_t thunk)
{
    if (_undefReplacement(thunk)) {
        emit stateChanged();
        return true;
    }
    return false;
}

bool FuncReplacements::_undefReplacement(offset_t thunk)
{
    QMap<offset_t, FuncDesc>::Iterator found = m_replacements.find(thunk);
    bool exist = found != m_replacements.end();
    if (exist) {
        m_replacements.erase(found);
        return true;
    }
    return false;
}

bool FuncReplacements::_defineReplacement(offset_t thunk, FuncDesc newFunc)
{
    QMap<offset_t, FuncDesc>::Iterator found = m_replacements.find(thunk);
    if (newFunc.size() == 0) {
        return _undefReplacement(thunk);
    }
    if (FuncUtil::validateFuncDesc(newFunc) == false) {
        return false;
    }
    m_replacements[thunk] = newFunc;
    return true;
}

FuncDesc FuncReplacements::getAt(offset_t thunk)
{
    if (hasAt(thunk) == false) return "";
    return m_replacements[thunk];
}

bool FuncReplacements::hasAt(offset_t thunk)
{
    return m_replacements.find(thunk) != m_replacements.end();
}

size_t  FuncReplacements::load(QString &fileName)
{
    size_t loaded = 0;
    QFile inputFile(fileName);

    if ( !inputFile.open(QIODevice::ReadOnly) ) {
        printf("Cannot open file!");
        return 0;
    }
    offset_t thunk = 0;
    QString thunkStr = "";
    QString funcDesc = "";

    QTextStream in(&inputFile);
    while ( !in.atEnd() ) {
        QString line = in.readLine();
        QTextStream(&line) >> thunkStr >> funcDesc;
        bool isOk = false;
        thunk = thunkStr.toLongLong(&isOk, 16);

        if (!isOk || !FuncUtil::validateFuncDesc(funcDesc) ) {
            continue;
        }
        if (_defineReplacement(thunk, funcDesc)) loaded++;
    }
    inputFile.close();

    if (loaded > 0) {
        emit stateChanged();
    }
    return loaded;
}

size_t FuncReplacements::save(QString &fileName)
{
    QFile outputFile(fileName);
    if ( !outputFile.open(QIODevice::WriteOnly | QIODevice::Text) ) return 0;

    QTextStream out(&outputFile);
    QMap<offset_t, FuncDesc>::iterator itr;
    size_t counter = 0;
    for (itr = m_replacements.begin(); itr != m_replacements.end(); itr++) {
        out << hex << itr.key();
        out << " ";
        out << itr.value();
        out << '\n';
        counter++;
        
    }
    outputFile.close(); 
    return counter;
}

size_t FuncReplacements::dropInvalidThunks(ImportsLookup &lookup)
{
    size_t invalidThunks = 0;
    QList<offset_t>::iterator itr;
    QList<offset_t> thunks = getThunks();
    for (itr = thunks.begin(); itr != thunks.end(); itr++) {
        if (lookup.hasThunk(*itr) == false) {
            invalidThunks++;
            this->_undefReplacement(*itr);
        }
    }
    if (invalidThunks > 0) {
        emit stateChanged();
    }
    return invalidThunks;
}