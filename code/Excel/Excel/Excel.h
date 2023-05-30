#pragma once

#define VERSION "Excel Version V-1.0.1"

#include <QObject>
#include <QAxObject>
#include <QFileDialog>
#include <QDebug>
#include <QThread>
#include <QCloseEvent>

struct TableCellAttr {
    int fillColor;      // 填充色/*无色 = -4142,自动 = -4105,黑色 = 1,褐色 = 53,橄榄 = 52,深绿 = 51,深青 = 49,深蓝 = 11,靛蓝 = 55,灰色80 = 56,深红 = 9,橙色 = 46,深黄 = 12,绿色 = 10,青色 = 14,蓝色 = 5,蓝灰 = 47,灰色50 = 16,红色 = 3,浅橙色 = 45,酸橙色 = 43,海绿 = 50,水绿色 = 42,浅蓝 = 41,    紫罗兰 = 13,灰色40 = 48,粉红 = 7,金色 = 44,黄色 = 6,鲜绿 = 4,青绿 = 8,天蓝 = 33,梅红 = 54,灰色25 = 15,玫瑰红 = 38,茶色 = 40,浅黄 = 36,浅绿 = 35,浅青绿 = 34,淡蓝 = 37,淡紫 = 39,白色 = 2*/
    int border;         // 边框(=1 虚线；=2实线；=-4238 粗实线(一般粗);=4实线(很粗))
    int fontColor;      // 字体颜色 = fillColor注释
    int fontSize;       // 字体大小
    QString fontStr;    // 字体
    bool isItalic;      // 斜体
    bool isBoldface;    // 粗体
    bool isUnderline;   // 下划线
    bool isWrapText;    // 自动换行
    bool isAutoFitRow;  // 自适应行高
    int cellFormat;     // 单元格格式 1文本 2日期 3数字 4货币 5百分比
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
    //打开excel
    bool OpenExcel(const QString filePath);
    bool CloseExcel();

    /*----------工作表----------*/
    //切换工作表
    void switchSheet(const int sheetIndex);
    bool switchSheet(const QString sheetName);
    //获取当前工作表
    int getCurrentSheetIndex();
    QString getCurrentSheetName();
    //获取所有工作表
    QStringList getAllSheetName();
    //增加工作表
    void addSheet(const QString sheetName);
    //删除工作表
    bool delSheet(const QString sheetName);
    void delSheet(const int sheetindex);

    /*--------单元格数据--------*/
    //获取行数列数
    bool getRowAndColumn(int& row, int& column);
    int getRow();
    int getColumn();
    //获取excel所有数据
    bool getExcelAllContent(QList<QStringList>& allData);
    bool getExcelAllContent(QList<QStringList>& allData, QList<QList<TableCellAttr>>& allDataStyle);
    //插入数据
    bool insertFileData(const int row, const int column, const QString data);
    bool insertFileData(const int row, const int column, const QString data,
        const TableCellAttr tableCellAttr); //row and column >=1
    //一行行插入效率快
    bool insertFileData(const int row, const int column, const QList<QVariant> dataList,
        const TableCellAttr tableCellAttr = TableCellAttr());
    //清除内容和样式
    bool clearDataAndStyle(const int row, const int column);
    //删除一行
    void delRow(const int row);
    //删除一列
    void delColumn(const int column);
    //单元格合并
    bool cellMerge(const int beginRow, const int beginColumn, const int endRow,
        const int endColumn, const bool isMergeCells = true);
    //修改单元格行高和行宽
    bool setHighAndWidth(const int row, const int column, const int high,
        const int width, QAxObject* worksheet);
    //修改列宽
    void setColumnWidth(const int column, int width);
    //修改行高
    void setRowHeight(const int row, int height);

    //操作完成保存
    void writeFinishSave();
private:
    //数据样式修改
    void setCellStyle(const TableCellAttr tableCellAttr, QAxObject* cell);
    TableCellAttr readCellStyle(QAxObject* cell);
    //列数转换英文字母超过26累加字母
    QString numberConvertCellStr(int size);

private:
    QList<QStringList> m_DataList;
    QString m_WriteFile;

    QAxObject* m_Excel; //excel程序
    QAxObject* m_WorkBooks; //excel所有工作薄
    QAxObject* m_WorkBook; //excel文件对象
    QAxObject* m_WorkSheets; //所有模板

    int m_SheetIndex;
};
