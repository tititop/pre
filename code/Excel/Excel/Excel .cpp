#include "Excel.h"

#include "adapt_glog.h"

#include <Windows.h>

#define DELETE_OBJ(obj) if(nullptr != obj) { MXT_KEY_LOGGER << "delete " #obj; delete obj; obj = nullptr; }

ExcelThread::ExcelThread(QObject* parent)
    : QObject(parent)
    , m_Excel(nullptr)
    , m_SheetIndex(1)
{
    MXT_INFO_LOGGER << VERSION;
    m_WorkBook = nullptr;
    m_WorkBooks = nullptr;
    m_Excel = nullptr;
    m_WorkSheets = nullptr;
    init();
}

ExcelThread::~ExcelThread()
{
    MXT_INFO_LOGGER << "excel quit";

}

void ExcelThread::init()
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    m_Excel = new QAxObject("Excel.Application");
    m_Excel->dynamicCall("SetVisible(bool Visible)", false);
    m_Excel->setProperty("DisplayAlert", false);
}

bool ExcelThread::OpenExcel(const QString filePath)
{
    if (!QFileInfo(filePath).exists()) {
        MXT_INFO_LOGGER << filePath.toStdString() << "file no exists";
        return false;
    }
    m_WorkBooks = m_Excel->querySubObject("WorkBooks");
    m_WorkBook = m_WorkBooks->querySubObject("Open(const QString&)", filePath);
    if (!m_WorkBook)
    {
        MXT_INFO_LOGGER << "m_WorkBook Error";
        return false;
    }
    m_WorkSheets = m_WorkBook->querySubObject("Sheets");

    return true;
}

bool ExcelThread::CloseExcel()
{
    m_Excel->dynamicCall("Quit(void)", false);
    m_WorkBooks->dynamicCall("Close(Boolean)", false);
    // DELETE_OBJ(m_WorkBook)
    // DELETE_OBJ(m_WorkBooks)
    // DELETE_OBJ(m_Excel)
    // DELETE_OBJ(m_WorkSheets)
    return true;
}

void ExcelThread::switchSheet(const int sheetIndex)
{
    m_SheetIndex = sheetIndex;
    if (sheetIndex <= 0) {
        m_SheetIndex = 1;
    }
}

bool ExcelThread::switchSheet(const QString sheetName)
{
    if (sheetName.isEmpty()) return false;

    QStringList sheetNameList = getAllSheetName();
    int index = sheetNameList.indexOf(sheetName);
    if (index != -1) {
        m_SheetIndex = index + 1;
    }
    return false;
}

int ExcelThread::getCurrentSheetIndex()
{
    return m_SheetIndex;
}

QString ExcelThread::getCurrentSheetName()
{
    QAxObject* sheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);
    return sheet->dynamicCall("Name").toString();
}

QStringList ExcelThread::getAllSheetName()
{
    int count = m_WorkSheets->dynamicCall("Count").toInt();

    QStringList sheetNameList;
    for (int i = 1; i <= count; ++i) {
        QAxObject* sheet = m_WorkSheets->querySubObject("Item(int)", i);
        sheetNameList << sheet->dynamicCall("Name").toString();
    }
    return sheetNameList;
}

void ExcelThread::addSheet(const QString sheetName)
{
    m_WorkSheets->querySubObject("Add()");
    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);
    workSheet->setProperty("Name", sheetName);
}

bool ExcelThread::delSheet(const QString sheetName)
{
    if (sheetName.isEmpty()) return false;

    QStringList sheetNameList = getAllSheetName();
    int index = sheetNameList.indexOf(sheetName);
    if (index != -1) {
        QAxObject* sheet = m_WorkSheets->querySubObject("Item(int)", index);
        sheet->dynamicCall("delete");
    }
    return false;
}

void ExcelThread::delSheet(const int sheetindex)
{
    QAxObject* sheet = m_WorkSheets->querySubObject("Item(int)", sheetindex);
    sheet->dynamicCall("delete");
}

bool ExcelThread::getRowAndColumn(int& row, int& column)
{
    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);

    QAxObject* usedrange = workSheet->querySubObject("UsedRange");
    if (!usedrange) {
        MXT_INFO_LOGGER << "usedrange error";
        return false;
    }
    QAxObject* rows, * columns;
    rows = usedrange->querySubObject("Rows");
    columns = usedrange->querySubObject("Columns");

    row = rows->property("Count").toInt();
    column = columns->property("Count").toInt();
    return true;
}

int ExcelThread::getRow()
{
    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);

    QAxObject* usedrange = workSheet->querySubObject("UsedRange");
    if (!usedrange) {
        MXT_INFO_LOGGER << "usedrange error";
        return false;
    }
    QAxObject* rows;
    rows = usedrange->querySubObject("Rows");

    int row = rows->property("Count").toInt();
    return row;
}

int ExcelThread::getColumn()
{
    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);

    QAxObject* usedrange = workSheet->querySubObject("UsedRange");
    if (!usedrange) {
        MXT_INFO_LOGGER << "usedrange error";
        return false;
    }
    QAxObject* columns;
    columns = usedrange->querySubObject("Columns");
    int column = columns->property("Count").toInt();
    return column;
}

bool ExcelThread::getExcelAllContent(QList<QStringList>& allData)
{
    MXT_FUNCTION_TIMER();
    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);
    int row, column;
    if (getRowAndColumn(row, column)) {
        for (int i = 1; i <= row; ++i) {
            QStringList data;
            for (int j = 1; j <= column; ++j) {
                QAxObject* cell = workSheet->querySubObject("Cells(int, int)", i, j);
                data << cell->dynamicCall("Value2()").toString();
            }
            MXT_KEY_LOGGER << "read row: " << i;
            allData << data;
        }
    }
    else {
        return false;
    }
    return true;
}

bool ExcelThread::getExcelAllContent(QList<QStringList>& allData, QList<QList<TableCellAttr> >& allDataStyle)
{
    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);

    int row, column;
    if (getRowAndColumn(row, column)) {
        for (int i = 1; i <= row; ++i) {
            QStringList data;
            QList<TableCellAttr> dataStyle;
            for (int j = 1; j <= column; ++j) {
                QAxObject* cell = workSheet->querySubObject("Cells(int, int)", i, j);
                data << cell->dynamicCall("Value2()").toString();
                dataStyle << readCellStyle(cell);
            }
            allData << data;
            allDataStyle << dataStyle;
        }
    }
    else {
        return false;
    }
    return true;
}

bool ExcelThread::insertFileData(const int row, const int column, const QString data)
{
    int setRow = row;
    int setColumn = column;
    if (setRow == 0) setRow = 1;
    if (setColumn == 0) setColumn = 1;

    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);
    QAxObject* cell = workSheet->querySubObject("Cells(int, int)", setRow, setColumn);

    cell->dynamicCall("SetValue(conts QVariant&)", data);
    return true;
}

bool ExcelThread::insertFileData(const int row, const int column, const QString data, const TableCellAttr tableCellAttr)
{
    int setRow = row;
    int setColumn = column;
    if (setRow == 0) setRow = 1;
    if (setColumn == 0) setColumn = 1;

    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);
    QAxObject* cell = workSheet->querySubObject("Cells(int, int)", setRow, setColumn);

    setCellStyle(tableCellAttr, cell);
    cell->dynamicCall("SetValue(conts QVariant&)", data);
    return true;
}

bool ExcelThread::insertFileData(const int row, const int column, const QList<QVariant> dataList, const TableCellAttr tableCellAttr)
{
    int setRow = row;
    int setColumn = column;
    if (setRow == 0) setRow = 1;
    if (setColumn == 0) setColumn = 1;

    QString cellStr;
    cellStr.append(numberConvertCellStr(setColumn));
    cellStr.append(QString::number(setRow));
    cellStr.append(":");
    cellStr.append(numberConvertCellStr(dataList.size() + setColumn - 1));
    cellStr.append(QString::number(setRow));

    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);
    QAxObject* cell = workSheet->querySubObject("Range(QString)", cellStr);
    setCellStyle(tableCellAttr, cell);
    cell->dynamicCall("SetValue(conts QVariant&)", dataList);
    return true;
}

void ExcelThread::setCellStyle(const TableCellAttr tableCellAttr, QAxObject* cell)
{
    if (tableCellAttr.fillColor != -1) {
        QAxObject* temp = cell->querySubObject("Interior");
        temp->setProperty("ColorIndex", tableCellAttr.fillColor);
    }
    if (tableCellAttr.border != -1) {
        QAxObject* temp = cell->querySubObject("Borders");
        temp->setProperty("Weight", tableCellAttr.border);
    }
    if (tableCellAttr.fontColor != -1) {
        QAxObject* temp = cell->querySubObject("Font");
        temp->setProperty("ColorIndex", tableCellAttr.fontColor);
    }
    if (tableCellAttr.fontSize != -1) {
        QAxObject* temp = cell->querySubObject("Font");
        temp->setProperty("Size", tableCellAttr.fontSize);
    }
    if (tableCellAttr.fontStr != "") {
        QAxObject* temp = cell->querySubObject("Font");
        temp->setProperty("Name", tableCellAttr.fontStr);
    }
    if (tableCellAttr.isItalic) {
        QAxObject* temp = cell->querySubObject("Font");
        temp->setProperty("Italic", tableCellAttr.isItalic);
    }
    if (tableCellAttr.isBoldface) {
        QAxObject* temp = cell->querySubObject("Font");
        temp->setProperty("Bold", tableCellAttr.isBoldface);
    }
    if (tableCellAttr.isUnderline) {
        QAxObject* temp = cell->querySubObject("Font");
        temp->setProperty("UnderLine", tableCellAttr.isUnderline);
    }
    if (tableCellAttr.isWrapText) {
        QAxObject* temp = cell;
        temp->setProperty("WrapText", tableCellAttr.isWrapText);
    }
    if (tableCellAttr.isAutoFitRow) {
        QAxObject* temp = cell;
        temp->dynamicCall("AutoFit()");
    }
    if (tableCellAttr.cellFormat != -1) {
        switch (tableCellAttr.cellFormat) {
        case 1:
            cell->setProperty("NumberFormatLocal", "@");
            break;
        case 2:
            cell->setProperty("NumberFormatLocal", "yyyy/mm/dd");
            break;
        case 3:
            cell->setProperty("NumberFormatLocal", "#,##0.00");
            break;
        case 4:
            cell->setProperty("NumberFormatLocal", "#,##0.00");
            break;
        case 5:
            cell->setProperty("NumberFormatLocal", "#0.00%");
            break;
        default:
            cell->setProperty("NumberFormatLocal", "@");
            break;
        }
    }
}

TableCellAttr ExcelThread::readCellStyle(QAxObject* cell)
{
    if (cell->isNull()) return TableCellAttr();

    TableCellAttr tableCellAttr;

    QAxObject* temp = cell->querySubObject("Interior");
    tableCellAttr.fillColor = temp->dynamicCall("ColorIndex").toInt();

    temp = cell->querySubObject("Borders");;
    tableCellAttr.border = temp->dynamicCall("Weight").toInt();

    temp = cell->querySubObject("Font");;
    tableCellAttr.fontColor = temp->dynamicCall("ColorIndex").toInt();

    temp = cell->querySubObject("Font");;
    tableCellAttr.fontSize = temp->dynamicCall("Size").toInt();

    temp = cell->querySubObject("Font");;
    tableCellAttr.fontStr = temp->dynamicCall("Name").toString();

    temp = cell->querySubObject("Font");;
    tableCellAttr.isItalic = temp->dynamicCall("Italic").toBool();

    temp = cell->querySubObject("Font");;
    tableCellAttr.isBoldface = temp->dynamicCall("Bold").toBool();

    temp = cell->querySubObject("Font");;
    tableCellAttr.isUnderline = temp->dynamicCall("UnderLine").toBool();

    tableCellAttr.isWrapText = cell->dynamicCall("WrapText").toBool();

    tableCellAttr.isAutoFitRow = cell->dynamicCall("AutoFit()").toBool();

    QString cellFormat = cell->dynamicCall("NumberFormatLocal").toString();

    if (cellFormat.contains("yyyy")) {
        tableCellAttr.cellFormat = 2;
    }
    else if (cellFormat.contains("0.00_")) {
        tableCellAttr.cellFormat = 3;
    }
    else if (cellFormat.contains("??#")) {
        tableCellAttr.cellFormat = 4;
    }
    else if (cellFormat.contains("0.00%")) {
        tableCellAttr.cellFormat = 5;
    }
    else {
        tableCellAttr.cellFormat = 1;
    }

    return tableCellAttr;
}

QString ExcelThread::numberConvertCellStr(int size)
{
    QString str;
    if (size > 26 && size <= 255) {
        int number1 = 0;
        while (size % 26 != 0) {
            size--;
            number1++;
        }
        int number2 = size / 26;// 整数求商
        str.append(QChar(number2 - 1 + 'A'));
        str.append(QChar((number1 - 1) + 'A'));
        return str;
    }
    else {
        return QString(QChar(size - 1 + 'A'));
    }
}

bool ExcelThread::clearDataAndStyle(const int row, const int column)
{
    int setRow = row;
    int setColumn = column;
    if (setRow == 0) setRow = 1;
    if (setColumn == 0) setColumn = 1;

    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);
    QAxObject* cell = workSheet->querySubObject("Cells(int, int)", setRow, setColumn);
    cell->querySubObject("ClearContents");
    return true;
}

void ExcelThread::delRow(const int row)
{
    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);
    QAxObject* cell = workSheet->querySubObject("Rows(int)", row);
    cell->querySubObject("Delete");
}

void ExcelThread::delColumn(const int column)
{
    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);
    QAxObject* cell = workSheet->querySubObject("Columns(int)", column);
    cell->querySubObject("Delete");
}

bool ExcelThread::cellMerge(const int beginRow, const int beginColumn, const int endRow, const int endColumn, const bool isMergeCells)
{
    int setBeginRow = beginRow;
    int setBeginColumn = beginColumn;
    if (setBeginRow == 0) setBeginRow = 1;
    if (setBeginColumn == 0) setBeginColumn = 1;

    int setEndRow = endRow;
    int setEndColumn = endColumn;
    if (setEndRow == 0) setEndRow = 1;
    if (setEndColumn == 0) setEndColumn = 1;

    QString cellStr;
    cellStr.append(numberConvertCellStr(setBeginColumn));
    cellStr.append(QString::number(setBeginRow));
    cellStr.append(":");
    cellStr.append(numberConvertCellStr(setEndColumn));
    cellStr.append(QString::number(setEndRow));


    MXT_INFO_LOGGER << cellStr.toStdString();
    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);
    QAxObject* cell = workSheet->querySubObject("Range(QString)", cellStr);
    cell->setProperty("VerticalAlignment", -4108);
    cell->setProperty("MergeCells", isMergeCells);

    return true;
}

bool ExcelThread::setHighAndWidth(const int row, const int column, const int high, const int width, QAxObject* worksheet)
{

    return true;
}

void ExcelThread::setColumnWidth(const int column, int width)
{
    QString cellStr;
    cellStr.append(numberConvertCellStr(column));
    cellStr.append(":");
    cellStr.append(numberConvertCellStr(column));

    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);
    QAxObject* cell = workSheet->querySubObject("Range(QString)", cellStr);
    cell->setProperty("ColumnWidth", width);
}

void ExcelThread::setRowHeight(const int row, int height)
{
    QString cellStr;
    cellStr.append('A');
    cellStr.append(QString::number(row));

    QAxObject* workSheet = m_WorkSheets->querySubObject("Item(int)", m_SheetIndex);
    QAxObject* cell = workSheet->querySubObject("Range(QString)", cellStr);
    QAxObject* temp = cell->querySubObject("Rows");
    temp->setProperty("RowHeight", height);
}

void ExcelThread::writeFinishSave()
{
    // m_Excel->clear();
    // m_WorkBook->clear();
    // m_WorkBooks->clear();
    // 
    // m_WorkBook->deleteLater();
    // m_Excel->deleteLater();
    // m_WorkBooks->deleteLater();
    m_WorkBook->dynamicCall("Save()");
    m_Excel->dynamicCall("Quit(void)", false);

    DELETE_OBJ(m_WorkSheets);
    DELETE_OBJ(m_WorkBook);
    DELETE_OBJ(m_WorkBooks);
}


