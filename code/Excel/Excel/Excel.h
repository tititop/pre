#pragma once

#define VERSION "Excel Version V-1.0.1"

#include <QObject>
#include <QAxObject>
#include <QFileDialog>
#include <QDebug>
#include <QThread>
#include <QCloseEvent>

struct TableCellAttr {
    int fillColor;      // ���ɫ/*��ɫ = -4142,�Զ� = -4105,��ɫ = 1,��ɫ = 53,��� = 52,���� = 51,���� = 49,���� = 11,���� = 55,��ɫ80 = 56,��� = 9,��ɫ = 46,��� = 12,��ɫ = 10,��ɫ = 14,��ɫ = 5,���� = 47,��ɫ50 = 16,��ɫ = 3,ǳ��ɫ = 45,���ɫ = 43,���� = 50,ˮ��ɫ = 42,ǳ�� = 41,    ������ = 13,��ɫ40 = 48,�ۺ� = 7,��ɫ = 44,��ɫ = 6,���� = 4,���� = 8,���� = 33,÷�� = 54,��ɫ25 = 15,õ��� = 38,��ɫ = 40,ǳ�� = 36,ǳ�� = 35,ǳ���� = 34,���� = 37,���� = 39,��ɫ = 2*/
    int border;         // �߿�(=1 ���ߣ�=2ʵ�ߣ�=-4238 ��ʵ��(һ���);=4ʵ��(�ܴ�))
    int fontColor;      // ������ɫ = fillColorע��
    int fontSize;       // �����С
    QString fontStr;    // ����
    bool isItalic;      // б��
    bool isBoldface;    // ����
    bool isUnderline;   // �»���
    bool isWrapText;    // �Զ�����
    bool isAutoFitRow;  // ����Ӧ�и�
    int cellFormat;     // ��Ԫ���ʽ 1�ı� 2���� 3���� 4���� 5�ٷֱ�
    TableCellAttr()
        : fillColor(-1)
        , border(-1)
        , fontColor(-1)
        , fontSize(-1)
        , fontStr("")
        , isItalic(false)
        , isBoldface(false)
        , isUnderline(false)
        , isWrapText(false)
        , isAutoFitRow(false)
        , cellFormat(-1)
    {}
};

class ExcelThread : public QObject
{
    Q_OBJECT
public:
    explicit ExcelThread(QObject* parent = nullptr);
    ~ExcelThread();
private slots:
    void init();
public:
    //��excel
    bool OpenExcel(const QString filePath);
    bool CloseExcel();

    /*----------������----------*/
    //�л�������
    void switchSheet(const int sheetIndex);
    bool switchSheet(const QString sheetName);
    //��ȡ��ǰ������
    int getCurrentSheetIndex();
    QString getCurrentSheetName();
    //��ȡ���й�����
    QStringList getAllSheetName();
    //���ӹ�����
    void addSheet(const QString sheetName);
    //ɾ��������
    bool delSheet(const QString sheetName);
    void delSheet(const int sheetindex);

    /*--------��Ԫ������--------*/
    //��ȡ��������
    bool getRowAndColumn(int& row, int& column);
    int getRow();
    int getColumn();
    //��ȡexcel��������
    bool getExcelAllContent(QList<QStringList>& allData);
    bool getExcelAllContent(QList<QStringList>& allData, QList<QList<TableCellAttr>>& allDataStyle);
    //��������
    bool insertFileData(const int row, const int column, const QString data);
    bool insertFileData(const int row, const int column, const QString data,
        const TableCellAttr tableCellAttr); //row and column >=1
    //һ���в���Ч�ʿ�
    bool insertFileData(const int row, const int column, const QList<QVariant> dataList,
        const TableCellAttr tableCellAttr = TableCellAttr());
    //������ݺ���ʽ
    bool clearDataAndStyle(const int row, const int column);
    //ɾ��һ��
    void delRow(const int row);
    //ɾ��һ��
    void delColumn(const int column);
    //��Ԫ��ϲ�
    bool cellMerge(const int beginRow, const int beginColumn, const int endRow,
        const int endColumn, const bool isMergeCells = true);
    //�޸ĵ�Ԫ���иߺ��п�
    bool setHighAndWidth(const int row, const int column, const int high,
        const int width, QAxObject* worksheet);
    //�޸��п�
    void setColumnWidth(const int column, int width);
    //�޸��и�
    void setRowHeight(const int row, int height);

    //������ɱ���
    void writeFinishSave();
private:
    //������ʽ�޸�
    void setCellStyle(const TableCellAttr tableCellAttr, QAxObject* cell);
    TableCellAttr readCellStyle(QAxObject* cell);
    //����ת��Ӣ����ĸ����26�ۼ���ĸ
    QString numberConvertCellStr(int size);

private:
    QList<QStringList> m_DataList;
    QString m_WriteFile;

    QAxObject* m_Excel; //excel����
    QAxObject* m_WorkBooks; //excel���й�����
    QAxObject* m_WorkBook; //excel�ļ�����
    QAxObject* m_WorkSheets; //����ģ��

    int m_SheetIndex;
};
