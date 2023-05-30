#pragma once

#include "OpenXLSX/OpenXLSX.hpp"
#include "nowide/cstdio.hpp"

#pragma comment(lib, "OpenXLSX/OpenXLSX.lib")
#pragma comment(lib, "OpenXLSX/nowide.lib")

#include <iostream>
#include <cmath>
#include <numeric>
#include <random>
#include <deque>
#include <list>

using namespace std;
using namespace OpenXLSX;

// Demo3
static void printWorkbook(const XLWorkbook& wb) {
    cout << "\nSheets in workbook:\n";
    for (const auto& name : wb.worksheetNames()) cout << wb.indexOfSheet(name) << " : " << name << "\n";
}

// 基础用法
int Demo1_GetSet() {
    cout << "********************************************************************************\n";
    cout << "DEMO PROGRAM #01: Basic Usage\n";
    cout << "********************************************************************************\n";

    // This example program illustrates basic usage of OpenXLSX, for example creation of a new workbook, and read/write
    // of cell values.

    // First, create a new document and access the sheet named 'Sheet1'.
    // New documents contain a single worksheet named 'Sheet1'
    XLDocument doc;
    doc.create("./Demo01.xlsx");
    auto wks = doc.workbook().worksheet("Sheet1");

    // The individual cells can be accessed by using the .cell() method on the worksheet object.
    // The .cell() method can take the cell address as a string, or alternatively take a XLCellReference
    // object. By using an XLCellReference object, the cells can be accessed by row/column coordinates.
    // The .cell() method returns an XLCell object.

    // The .value() method of an XLCell object can be used for both getting and setting the cell value.
    // Setting the value of a cell can be done by using the assignment operator on the .value() method
    // as shown below. Alternatively, a .set() can be used. The cell values can be floating point numbers,
    // integers, strings, and booleans. It can also accept XLDateTime objects, but this requires special
    // handling (see later).
    wks.cell("A1").value() = 3.14159265358979323846;
    wks.cell("B1").value() = 42;
    wks.cell("C1").value() = "  Hello OpenXLSX!  ";
    wks.cell("D1").value() = true;
    wks.cell("E1").value() = std::sqrt(-2); // Result is NAN, resulting in an error value in the Excel spreadsheet.

    // As mentioned, the .value() method can also be used for getting tha value of a cell.
    // The .value() method returns a proxy object that cannot be copied or assigned, but
    // it can be implicitly converted to an XLCellValue object, as shown below.
    // Unfortunately, it is not possible to use the 'auto' keyword, so the XLCellValue
    // type has to be explicitly stated.
    XLCellValue A1 = wks.cell("A1").value();
    XLCellValue B1 = wks.cell("B1").value();
    XLCellValue C1 = wks.cell("C1").value();
    XLCellValue D1 = wks.cell("D1").value();
    XLCellValue E1 = wks.cell("E1").value();

    // The cell value can be implicitly converted to a basic c++ type. However, if the type does not
    // match the type contained in the XLCellValue object (if, for example, floating point value is
    // assigned to a std::string), then an XLValueTypeError exception will be thrown.
    // To check which type is contained, use the .type() method, which will return a XLValueType enum
    // representing the type. As a convenience, the .typeAsString() method returns the type as a string,
    // which can be useful when printing to console.
    double vA1 = wks.cell("A1").value();
    int vB1 = wks.cell("B1").value();
    std::string vC1 = wks.cell("C1").value();
    bool vD1 = wks.cell("D1").value();
    double vE1 = wks.cell("E1").value();

    cout << "Cell A1: (" << A1.typeAsString() << ") " << vA1 << endl;
    cout << "Cell B1: (" << B1.typeAsString() << ") " << vB1 << endl;
    cout << "Cell C1: (" << C1.typeAsString() << ") " << vC1 << endl;
    cout << "Cell D1: (" << D1.typeAsString() << ") " << vD1 << endl;
    cout << "Cell E1: (" << E1.typeAsString() << ") " << vE1 << endl << endl;

    // Instead of using implicit (or explicit) conversion, the underlying value can also be retrieved
    // using the .get() method. This is a templated member function, which takes the desired type
    // as a template argument.
    cout << "Cell A1: (" << A1.typeAsString() << ") " << A1.get<double>() << endl;
    cout << "Cell B1: (" << B1.typeAsString() << ") " << B1.get<int64_t>() << endl;
    cout << "Cell C1: (" << C1.typeAsString() << ") " << C1.get<std::string>() << endl;
    cout << "Cell D1: (" << D1.typeAsString() << ") " << D1.get<bool>() << endl;
    cout << "Cell E1: (" << E1.typeAsString() << ") " << E1.get<double>() << endl << endl;

    // XLCellValue objects can also be copied and assigned to other cells. This following line
    // will copy and assign the value of cell C1 to cell E1. Note tha only the value is copied;
    // other cell properties of the target cell remain unchanged.
    wks.cell("F1").value() = wks.cell(XLCellReference("C1")).value();
    XLCellValue F1 = wks.cell("F1").value();
    cout << "Cell F1: (" << F1.typeAsString() << ") " << F1.get<std::string_view>() << endl << endl;

    // Date/time values is a special case. In Excel, date/time values are essentially just a
    // 64-bit floating point value, that is rendered as a date/time string using special
    // formatting. When retrieving the cell value, it is just a floating point value,
    // and there is no way to identify it as a date/time value.
    // If, however, you know it to be a date time value, or if you want to assign a date/time
    // value to a cell, you can use the XLDateTime class, which falilitates conversion between
    // Excel date/time serial numbers, and the std::tm struct, that is used to store
    // date/time data. See https://en.cppreference.com/w/cpp/chrono/c/tm for more information.

    // An XLDateTime object can be created from a std::tm object:
    std::tm tm;
    tm.tm_year = 121;
    tm.tm_mon = 8;
    tm.tm_mday = 1;
    tm.tm_hour = 12;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    XLDateTime dt(tm);
    //    XLDateTime dt (43791.583333333299);

        // The std::tm object can be assigned to a cell value in the same way as shown previously.
    wks.cell("G1").value() = dt;

    // And as seen previously, an XLCellValue object can be retrieved. However, the object
    // will just contain a floating point value; there is no way to identify it as a date/time value.
    XLCellValue G1 = wks.cell("G1").value();
    cout << "Cell G1: (" << G1.typeAsString() << ") " << G1.get<double>() << endl;

    // If it is known to be a date/time value, the cell value can be converted to an XLDateTime object.
    auto result = G1.get<XLDateTime>();

    // The Excel date/time serial number can be retrieved using the .serial() method.
    cout << "Cell G1: (" << G1.typeAsString() << ") " << result.serial() << endl;

    // Using the .tm() method, the corresponding std::tm object can be retrieved.
    auto tmo = result.tm();
    cout << "Cell G1: (" << G1.typeAsString() << ") " << std::asctime(&tmo);

    doc.save();
    doc.close();

    return 0;
}

// 公式
int Demo2_Formulas() {
    cout << "********************************************************************************\n";
    cout << "DEMO PROGRAM #02: Formulas\n";
    cout << "********************************************************************************\n";

    XLDocument doc;
    doc.create("./Demo02.xlsx");
    auto wks = doc.workbook().worksheet("Sheet1");

    // Similar cell values, which are represented by XLCellValue objects, formulas are represented
    // by XLFormula objects. They can be accessed through the XLCell interface using the .formula()
    // member function. It should be noted, however, that the functionality of XLFormula is somewhat
    // limited. Excel often uses 'shared' formulas, where the same formula is applied to several
    // cells. XLFormula cannot handle shared formulas. Also, it cannot handle array formulas. This,
    // in effect, means that XLFormula is not very useful for reading formulas from existing spread-
    // sheets, but should rather be used to add or overwrite formulas to spreadsheets.

    wks.cell("A1").value() = "Number:";
    wks.cell("B1").value() = 1;
    wks.cell("C1").value() = 2;
    wks.cell("D1").value() = 3;

    // Formulas can be added to a cell using the .formula() method on an XLCell object. The formula can
    // be added by creating a separate XLFormula object, or (as shown below) by assigning a string
    // holding the formula;
    // Nota that OpenXLSX does not calculate the results of a formula. If you add a formula
    // to a spreadsheet using OpenXLSX, you have to open the spreadsheet in the Excel application
    // in order to see the results of the calculation.
    wks.cell("A2").value() = "Calculation:";
    wks.cell("B2").formula() = "SQRT(B1)";
    wks.cell("C2").formula() = "EXP(C1)";
    wks.cell("D2").formula() = "LN(D1)";

    // The XLFormula object can be retrieved using the .formula() member function.
    // The XLFormula .get() method can be used to get the formula string.
    XLFormula B2 = wks.cell("B2").formula();
    XLFormula C2 = wks.cell("C2").formula();
    XLFormula D2 = wks.cell("D2").formula();

    cout << "Formula in B2: " << B2.get() << endl;
    cout << "Formula in C2: " << C2.get() << endl;
    cout << "Formula in D2: " << D2.get() << endl << endl;

    // Alternatively, the XLFormula object can be implicitly converted to a string object.
    std::string B2string = wks.cell("B2").formula();
    std::string C2string = wks.cell("C2").formula();
    std::string D2string = wks.cell("D2").formula();

    cout << "Formula in B2: " << B2string << endl;
    cout << "Formula in C2: " << C2string << endl;
    cout << "Formula in D2: " << D2string << endl << endl;

    doc.save();
    doc.close();

    return 0;
}

// Sheet Handling
int Demo3_SheetHandling() {
    cout << "********************************************************************************\n";
    cout << "DEMO PROGRAM #03: Sheet Handling\n";
    cout << "********************************************************************************\n";

    // OpenXLSX can be used to create and delete sheets in a workbook, as well as re-ordering of sheets.
    // This example illustrates how this can be done. Please note that at the moment, chartsheets can only
    // be renamed and deleted, not created or manipulated.

    // First, create a new document and store the workbook object in a variable. Print the sheet names.
    XLDocument doc;
    doc.create("./Demo03.xlsx");
    auto wbk = doc.workbook();
    printWorkbook(wbk);

    // Add two new worksheets. The 'addWorksheet' method takes the name of the new sheet as an argument,
    // and appends the new workdheet at the end.
    // Only worksheets can be added; there is no 'addChartsheet' method.
    wbk.addWorksheet("Sheet2");
    wbk.addWorksheet("Sheet3");
    printWorkbook(wbk);

    cout << "Sheet1 active: " << (wbk.worksheet("Sheet1").isActive() ? "true" : "false") << endl;
    cout << "Sheet2 active: " << (wbk.worksheet("Sheet2").isActive() ? "true" : "false") << endl;
    cout << "Sheet3 active: " << (wbk.worksheet("Sheet3").isActive() ? "true" : "false") << endl;

    wbk.worksheet("Sheet3").setActive();

    cout << "Sheet1 active: " << (wbk.worksheet("Sheet1").isActive() ? "true" : "false") << endl;
    cout << "Sheet2 active: " << (wbk.worksheet("Sheet2").isActive() ? "true" : "false") << endl;
    cout << "Sheet3 active: " << (wbk.worksheet("Sheet3").isActive() ? "true" : "false") << endl;

    // OpenXLSX provides three different classes to handle workbook sheets: XLSheet, XLWorksheet, and
    // XLChartsheet. As you might have guessed, XLWorksheet and XLChartsheet represents worksheets
    // and chartsheets, respectively. XLChartsheet only has a limited set of functionality.
    // XLSheet, on the other hand, is basically a std::variant that can hold either an XLWorksheet,
    // or an XLChartsheet object. XLSheet has a limited set of functions that are common to
    // both XLWorksheet and XLChartsheet objects, such as 'clone()' or 'setIndex()'. XLSheet is
    // not a parent class to either XLWorksheet or XLChartsheet, and therefore cannot be used
    // polymorphically.

    // From an XLSheet object you can retrieve the contained XLWorksheet or XLChartsheet object by
    // using the 'get<>()' function:
    auto s1 = wbk.sheet("Sheet2").get<XLWorksheet>();

    // Alternatively, you can retrieve the contained object, by using implicit conversion:
    XLWorksheet s2 = wbk.sheet("Sheet2");

    // Existing sheets can be cloned by calling the 'clone' method on the individual sheet,
    // or by calling the 'cloneSheet' method from the XLWorkbook object. If the latter is
    // chosen, both the name of the sheet to be cloned, as well as the name of the new
    // sheet must be provided.
    // In principle, chartsheets can also be cloned, but the results may not be as expected.
    wbk.sheet("Sheet1").clone("Sheet4");
    wbk.cloneSheet("Sheet2", "Sheet5");
    printWorkbook(wbk);

    // The sheets in the workbook can be reordered by calling the 'setIndex' method on the
    // individual sheets (or worksheets/chartsheets).
    wbk.deleteSheet("Sheet1");
    wbk.worksheet("Sheet5").setIndex(1);
    wbk.worksheet("Sheet4").setIndex(2);
    wbk.worksheet("Sheet3").setIndex(3);
    wbk.worksheet("Sheet2").setIndex(4);
    printWorkbook(wbk);

    // The color of each sheet tab can be set using the 'setColor' method for a
    // sheet, and passing an XLColor object as an argument.
    wbk.sheet("Sheet2").setColor(XLColor(0, 0, 0));
    wbk.sheet("Sheet3").setColor(XLColor(255, 0, 0));
    wbk.sheet("Sheet4").setColor(XLColor(0, 255, 0));
    wbk.sheet("Sheet5").setColor(XLColor(0, 0, 255));

    doc.save();

    return 0;
}

// Unicode
int Demo4_Unicode() {
    cout << "********************************************************************************\n";
    cout << "DEMO PROGRAM #04: Unicode\n";
    cout << "********************************************************************************\n";

    // Unicode can be a real pain in the neck. While UTF-8 encoding has become the de facto standard
    // encoding on Linux, MacOS and the internet, some systems use other encodings, most notably
    // Windows which use UTF-16.
    // OpenXLSX is based on UTF-8. That means that all text input and output MUST be in UTF-8 format.
    // On Linux and MacOS, this will work out of the box because UTF-8 is baked into those systems.
    // On Windows, on the other hand, input and output must be converted to UTF-8 first. This includes
    // input strings that are hard-coded into your program. To stay sane, it is recommended that
    // all your source code files are in UTF-8 encoding. All major compilers and IDEs on windows
    // support UTF-8 encoded source files.
    // To convert input/output manually, you can use the Windows API, or 3rd party libraries, such
    // as boost.nowide.
    // In this example boost.nowide is used, as it provides some handy functionality that enables
    // console input/output with implicit conversion to/from UTF-8 on Windows.

    // First, create a new document and access the sheet named 'Sheet1'.
    // Then rename the worksheet to 'Простыня'.
    XLDocument doc1;
    doc1.create("./Demo04.xlsx");
    auto wks1 = doc1.workbook().worksheet("Sheet1");
    wks1.setName("Простыня");

    // Cell values can be set to any Unicode string using the normat value assignment methods.
    wks1.cell(XLCellReference("A1")).value() = "안녕하세요 세계!";
    wks1.cell(XLCellReference("A2")).value() = "你好，世界!";
    wks1.cell(XLCellReference("A3")).value() = "こんにちは 世界";
    wks1.cell(XLCellReference("A4")).value() = "नमस्ते दुनिया!";
    wks1.cell(XLCellReference("A5")).value() = "Привет, мир!";
    wks1.cell(XLCellReference("A6")).value() = "Γειά σου Κόσμε!";

    // Workbooks can also be saved and loaded with Unicode names
    doc1.save();
    doc1.saveAs("./スプレッドシート.xlsx");
    doc1.close();

    doc1.open("./スプレッドシート.xlsx");
    wks1 = doc1.workbook().worksheet("Простыня");

    // The nowide::cout object is a drop-in replacement of the std::cout that enables console output of UTF-8, even on Windows.
    cout << "Cell A1 (Korean)  : " << wks1.cell(XLCellReference("A1")).value().get<std::string>() << std::endl;
    cout << "Cell A2 (Chinese) : " << wks1.cell(XLCellReference("A2")).value().get<std::string>() << std::endl;
    cout << "Cell A3 (Japanese): " << wks1.cell(XLCellReference("A3")).value().get<std::string>() << std::endl;
    cout << "Cell A4 (Hindi)   : " << wks1.cell(XLCellReference("A4")).value().get<std::string>() << std::endl;
    cout << "Cell A5 (Russian) : " << wks1.cell(XLCellReference("A5")).value().get<std::string>() << std::endl;
    cout << "Cell A6 (Greek)   : " << wks1.cell(XLCellReference("A6")).value().get<std::string>() << std::endl;


    cout << "\nNOTE: If you are using a Windows terminal, the above output may look like gibberish,\n"
        "because the Windows terminal does not support UTF-8 at the moment. To view to output,\n"
        "you can use the overloaded 'cout' in the boost::nowide library (as in this sample program).\n"
        "This will require a UTF-8 enabled font in the terminal. Lucinda Console supports some\n"
        "non-ASCII scripts, such as Cyrillic and Greek. NSimSun supports some asian scripts.\n\n";

    doc1.close();

    return 0;
}

// Ranges and Iterators
int Demo5_RangesAndIterators() {
    cout << "********************************************************************************\n";
    cout << "DEMO PROGRAM #05: Ranges and Iterators\n";
    cout << "********************************************************************************\n";

    // With OpenXLSX, a range of cells can be defined, in order to iterate through the
    // cells in the range. A range is a quadratic region of cells in a worksheet.
    // A range can be defined using the 'range()' method on an XLWorksheet object. This function
    // takes two XLCellReferences: the cell in the upper left corner of the range, and the cell
    // in the lower right corner.
    // The resulting XLCellRange objects provides 'begin()' and 'end()' (returning iterators),
    // so that the object can be used in a range-based for-loop.

    // First, create a new document and access the sheet named 'Sheet1'.
    cout << "\nGenerating spreadsheet ..." << endl;
    XLDocument doc;
    doc.create("./Demo05.xlsx");
    auto wks = doc.workbook().worksheet("Sheet1");

    // Create a random-number generator (to be used later)
    std::random_device                 rand_dev;
    std::mt19937                       generator(rand_dev());
    std::uniform_int_distribution<int> distr(0, 99);

    // Create a cell range using the 'range()' method of the XLWorksheet class.
    // The 'range()' method takes two XLCellReference objects; one for the cell
    // in the upper left corner, and one for the cell in the lower right corner.
    // OpenXLSX defines two constants: MAX_ROWS which is the maximum number of
    // rows in a worksheet, and MAX_COLS which is the maximum number of columns.
    auto rng = wks.range(XLCellReference("A1"), XLCellReference(OpenXLSX::MAX_ROWS, 8));

    // An XLCellRange object provides begin and end iterators, and can be used in
    // range-based for-loops. The next line iterates through all the cells in the
    // range and assigns a random number to each cell:
    for (auto& cell : rng) cell.value() = distr(generator);

    // Saving a large spreadsheet can take a while...
    cout << "Saving spreadsheet ..." << endl;
    doc.save();
    doc.close();

    // Reopen the spreadsheet..
    cout << "Re-opening spreadsheet ..." << endl;
    doc.open("./Demo05.xlsx");
    wks = doc.workbook().worksheet("Sheet1");

    // Create the range, count the cells in the range, and sum the numbers assigned
    // to the cells in the range.
    cout << "Reading data from spreadsheet ..." << endl;
    rng = wks.range(XLCellReference("A1"), XLCellReference(OpenXLSX::MAX_ROWS, 8));
    cout << "Cell count: " << std::distance(rng.begin(), rng.end()) << endl;
    cout << "Sum of cell values: "
        << accumulate(rng.begin(), rng.end(), 0, [](uint64_t a, XLCell& b) { return a + b.value().get<int64_t>(); });

    doc.close();

    return 0;
}

// Row Handling (using containers)
int Demo6_RowHandling_UsingContainers() {
    cout << "********************************************************************************\n";
    cout << "DEMO PROGRAM #06: Row Handling (using containers)\n";
    cout << "********************************************************************************\n";

    // As an alternative to using cell ranges, you can use row ranges.
    // This can be significantly faster (up to twice as fast for both reading and
    // writing).

    // First, create a new document and access the sheet named 'Sheet1'.
    cout << "\nGenerating spreadsheet ..." << endl;
    XLDocument doc;
    std::string path = "./Demo06.xlsx";
    doc.create(path);
    auto wks = doc.workbook().worksheet("Sheet1");

    // Create a random-number generator (to be used later)
    std::random_device                 rand_dev;
    std::mt19937                       generator(rand_dev());
    std::uniform_int_distribution<int> distr(0, 99);

    // The XLWorksheet class has a 'rows()' method, that returns a XLRowRange
    // object. Similar to XLCellRange, XLRowRange provides begin and end iterators,
    // enabeling iteration through the individual XLRow objects.
    std::vector<XLCellValue> writeValues;
    for (auto& row : wks.rows(OpenXLSX::MAX_ROWS)) {
        writeValues.clear();
        for (int i = 0; i < 8; ++i) writeValues.emplace_back(distr(generator));

        // The XLRow class provides a 'values()' method. Similar to the 'value()'
        // method of the XLCell class, the 'values()' method returns a reference
        // to a proxy object that facilitates implicit conversions etc.
        // Using the 'values()' method, containers can be assigned to each row.
        // In this case, a std::vector is used, but any container supporting
        // bi-directional iterators (such as std::deque or std::list) can be
        // used.
        // In this example a vector of XLCellValue objects is used. This is
        // useful if you heterogeneous data (such as doubles, ints and strings)
        // in the same dataset. However, if you have homogeneous data, e.g. all
        // doubles, you don't have to use XLCellValue objects. You can, for
        // examlpe, assigne a std::vector of doubles directly to the row.
        row.values() = writeValues;
    }

    // Saving a large spreadsheet can take a while...
    cout << "Saving spreadsheet ..." << endl;
    doc.save();
    doc.close();

    // Reopen the spreadsheet...
    cout << "Re-opening spreadsheet ..." << endl;
    doc.open(path);
    wks = doc.workbook().worksheet("Sheet1");

    // Prepare for reading...
    uint64_t sum = 0;
    uint64_t count = 0;
    std::vector<XLCellValue> readValues;
    cout << "Reading data from spreadsheet ..." << endl;

    // The 'values()' member function can be used for both reading and writing
    // of data. In this case the 'readValues' variable is a std::vector of
    // XLCellValues, and the 'values()' method will perform implicit conversion
    // to the correct type. Again, all containers supporting bi-directional
    // iterators are supported. Also, if you have a vector of doubles, the values
    // will be converted, if possible. Note, however, that if conversion is not
    // possible, an exception will be thrown. For that reason, it is often best
    // to use a container of XLCellValue objects, and then later determine the
    // data type for each object.
    for (auto& row : wks.rows()) {
        readValues = row.values();

        // Count the number of cell values
        count += std::count_if(readValues.begin(), readValues.end(), [](const XLCellValue& v) {
            return v.type() != XLValueType::Empty;
            });

        // Sum the numbers in each cell.
        sum += std::accumulate(readValues.begin(),
            readValues.end(),
            0,
            [](uint64_t a, XLCellValue& b) {return a + b.get<uint64_t>(); });
    }

    cout << "Cell count: " << count << endl;
    cout << "Sum of cell values: " << sum << endl;

    doc.close();

    return 0;
}

// Row Handling (using iterators)
int Demo7_RowHandling_UsingIterators() {
    cout << "********************************************************************************\n";
    cout << "DEMO PROGRAM #07: Row Handling (using iterators)\n";
    cout << "********************************************************************************\n";

    // As an alternative to using cell ranges, you can use row ranges.
    // This can be significantly faster (up to twice as fast for both reading and
    // writing).

    // First, create a new document and access the sheet named 'Sheet1'.
    cout << "\nGenerating spreadsheet ..." << endl;
    XLDocument doc;
    std::string path = "./Demo07.xlsx";
    doc.create(path);
    auto wks = doc.workbook().worksheet("Sheet1");

    // Create a random-number generator (to be used later)
    std::random_device                 rand_dev;
    std::mt19937                       generator(rand_dev());
    std::uniform_int_distribution<int> distr(0, 99);

    // The XLWorksheet class has a 'rows()' method, that returns a XLRowRange
    // object. Similar to XLCellRange, XLRowRange provides begin and end iterators,
    // enabeling iteration through the individual XLRow objects.
    for (auto& row : wks.rows(OpenXLSX::MAX_ROWS)) {
        for (auto cell : row.cells(8)) cell.value() = distr(generator);

        // The XLRow class provides a 'cells()' method. This provides begin and end
        // iterators to the cells, or range of cells, in the row. Iterating through
        // the cells works in the usual manner and values can be read and written.
    }

    // Saving a large spreadsheet can take a while...
    cout << "Saving spreadsheet ..." << endl;
    doc.save();
    doc.close();

    // Reopen the spreadsheet...
    cout << "Re-opening spreadsheet ..." << endl;
    doc.open(path);
    wks = doc.workbook().worksheet("Sheet1");

    // Prepare for reading...
    uint64_t sum = 0;
    uint64_t count = 0;
    cout << "Reading data from spreadsheet ..." << endl;

    // The 'cells()' member function can be used in the usual algorithm in the
    // STL, such as count_if and accumulate.
    for (auto& row : wks.rows()) {
        // Count the number of cell values
        count += std::count_if(row.cells().begin(), row.cells().end(), [](const XLCell& c) {
            return c.value().type() != XLValueType::Empty;
            });

        // Sum the numbers in each cell.
        sum += std::accumulate(row.cells().begin(),
            row.cells().end(),
            0,
            [](uint64_t a, XLCell& b) {return a + b.value().get<uint64_t>(); });
    }

    cout << "Cell count: " << count << endl;
    cout << "Sum of cell values: " << sum << endl;

    doc.close();

    return 0;
}

// Row Data - Implicit Conversion
int Demo8_ImplicitConversion() {
    cout << "********************************************************************************\n";
    cout << "DEMO PROGRAM #08: Row Data - Implicit Conversion\n";
    cout << "********************************************************************************\n";


    // The previous example showed, among other things, how a std::vector of XLCellValue objects
    // could be assigned to to an XLRow object, to populate the data to the cells in that row.
    // In fact, this can be done with any container supporting bi-directional iterators, holding
    // any data type that is convertible to a valid cell value. This is illustrated in this example.

    // First, create a new document and access the sheet named 'Sheet1'.
    cout << "\nGenerating spreadsheet ..." << endl;
    XLDocument doc;
    doc.create("./Demo08.xlsx");
    auto wks = doc.workbook().worksheet("Sheet1");

    // A std::vector holding values that are convertible to a cell value can be assigned to an XLRow
    // object, using the 'values()' method. For example ints, doubles, bools and std::strings.
    wks.row(1).values() = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8 };
    wks.row(2).values() = std::vector<double>{ 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8 };
    wks.row(3).values() = std::vector<bool>{ true, false, true, false, true, false, true, false };
    wks.row(4).values() = std::vector<std::string>{ "A", "B", "C", "D", "E", "F", "G", "H" };

    // Save the sheet...
    cout << "Saving spreadsheet ..." << endl;
    doc.save();
    doc.close();

    // ...and reopen it (just to make sure that it is a valid .xlsx file)
    cout << "Re-opening spreadsheet ..." << endl << endl;
    doc.open("./Demo08.xlsx");
    wks = doc.workbook().worksheet("Sheet1");

    // The '.values()' method returns a proxy object that can be converted to any container supporting
    // bi-directional iterators. The following three blocks shows how the row data can be converted to
    // std::vector, std::deque, and std::list. In principle, this will also work with non-stl containers,
    // e.g. containers in the Qt framework. This has not been tested, though.

    // Conversion to std::vector<XLCellValue>
    cout << "Conversion to std::vector<XLCellValue> ..." << endl;
    for (auto& row : wks.rows()) {
        for (auto& value : std::vector<XLCellValue>(row.values())) {
            cout << value << " ";
        }
        cout << endl;
    }

    // Conversion to std::deque<XLCellValue>
    cout << endl << "Conversion to std::deque<XLCellValue> ..." << endl;
    for (auto& row : wks.rows()) {
        for (auto& value : std::deque<XLCellValue>(row.values())) {
            cout << value << " ";
        }
        cout << endl;
    }

    // Conversion to std::list<XLCellValue>
    cout << endl << "Conversion to std::list<XLCellValue> ..." << endl;
    for (auto& row : wks.rows()) {
        for (auto& value : std::list<XLCellValue>(row.values())) {
            cout << value << " ";
        }
        cout << endl;
    }

    // In addition to supporting any bi-directional container types, the cell values can also be converted to
    // compatible plain data types instead of XLCellValue objects, e.g. ints, doubles, bools and std::string.
    // This is illustrated in the following three blocks:

    // Conversion to std::vector<[int, double, bool, std::string]>
    cout << endl << "Conversion to std::vector<[int, double, bool, std::string]> ..." << endl;
    for (auto& value : std::vector<int>(wks.row(1).values())) cout << value << " ";
    cout << endl;
    for (auto& value : std::vector<double>(wks.row(2).values())) cout << value << " ";
    cout << endl;
    for (bool value : std::vector<bool>(wks.row(3).values())) cout << value << " ";
    cout << endl;
    for (auto& value : std::vector<std::string>(wks.row(4).values())) cout << value << " ";
    cout << endl;

    // Conversion to std::deque<[int, double, bool, std::string]>
    cout << endl << "Conversion to std::deque<[int, double, bool, std::string]> ..." << endl;
    for (auto& value : std::deque<int>(wks.row(1).values())) cout << value << " ";
    cout << endl;
    for (auto& value : std::deque<double>(wks.row(2).values())) cout << value << " ";
    cout << endl;
    for (auto& value : std::deque<bool>(wks.row(3).values())) cout << value << " ";
    cout << endl;
    for (auto& value : std::deque<std::string>(wks.row(4).values())) cout << value << " ";
    cout << endl;

    // Conversion to std::list<[int, double, bool, std::string]>
    cout << endl << "Conversion to std::list<[int, double, bool, std::string]> ..." << endl;
    for (auto& value : std::list<int>(wks.row(1).values())) cout << value << " ";
    cout << endl;
    for (auto& value : std::list<double>(wks.row(2).values())) cout << value << " ";
    cout << endl;
    for (auto& value : std::list<bool>(wks.row(3).values())) cout << value << " ";
    cout << endl;
    for (auto& value : std::list<std::string>(wks.row(4).values())) cout << value << " ";
    cout << endl;

    doc.close();

    return 0;
}
