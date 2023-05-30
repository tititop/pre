#include <QtCore/QCoreApplication>
#include <qthread.h>

#include <Windows.h>
#include <ShlObj.h>

#include "Excel/Excel.h"
#include "OpenXLSX/Excel.h"

#include "File.h"
#include "adapt_glog.h"
#include "adapt_struct2x.h"

#include <vector>
#include <string>
#include <set>

struct FileSelectSNContent {
    std::vector<std::string> select_sn;
};
VISITSTRUCT(FileSelectSNContent, select_sn)

const std::string g_from_file_name = "SN_IN_EXCEL.xlsx";
std::string g_to_dir = "D:";
std::string g_to_file_name = "HYC_SELECT_SN.dat";

std::string  getDesktopPath()
{
    LPITEMIDLIST pidl;
    LPMALLOC pShellMalloc;
    char szDir[200];
    if (SUCCEEDED(SHGetMalloc(&pShellMalloc))) {
        if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl))) {
            // 如果成功返回true
            SHGetPathFromIDListA(pidl, szDir);
            pShellMalloc->Free(pidl);
        } else {
            MXT_ERROR_LOGGER << "get desktop dir error";
        }
        pShellMalloc->Release();
    }

    return std::string(szDir);
}

#define STR_PARAM(param)  #param " = " << param

void MicrosoftExcel()
{
    Demo1_GetSet();
    Demo2_Formulas();
    Demo3_SheetHandling();
    try
    {
        Demo4_Unicode();
    } catch (const std::exception& e) {
        MXT_ERROR_LOGGER << "exception detail: " << e.what();
    } catch (...) {
        MXT_ERROR_LOGGER << "exception detail: unknown ";
    }
    Demo5_RangesAndIterators();
    Demo6_RowHandling_UsingContainers();
    Demo7_RowHandling_UsingIterators();
    Demo8_ImplicitConversion();
}

void Process_SpeedReadQRFile_OpenXLSX(std::vector<std::string>& ans, std::string file_path, std::string col_name)
{
    MXT_FUNCTION_TIMER();
    MXT_KEY_LOGGER << "extract data from xlsx file: "
        << STR_PARAM(file_path) << ", " << STR_PARAM(col_name);
    auto& qr_codes = ans;
    XLDocument doc;
    doc.open(file_path);
    auto& book = doc.workbook();
    auto sheet_names = book.sheetNames();

    for (auto sheet_name_it : sheet_names) {
        auto& sheet = book.worksheet(sheet_name_it);
        size_t nRow = sheet.rowCount() + 1;
        size_t nCol = sheet.columnCount();
        // auto rowData = sheet.rows();
        // auto colData = sheet.column(5);

        int qrColIndex = -1;
        for (int c = 0; c < nCol; c++) {
            char cell_row = c + 'A';
            std::string cell_index = std::string(1, cell_row);
            cell_index.push_back('1');
            if (sheet.cell(cell_index).value() == col_name) {
                qrColIndex = c;
                break;
            }
        }
        if (-1 == qrColIndex) {
            MXT_ERROR_LOGGER << "no find qr column, sheet_name = " << sheet_name_it;
        } else {
            MXT_KEY_LOGGER << "extract sheet: " << sheet_name_it;
        }
        
        // 从第二行开始
        for (int r = 2; r < nRow; r++) {
            std::string cell_index = std::string(1, (char)(qrColIndex + 'A')) + std::to_string(r);
            std::string qr_code = sheet.cell(cell_index).value();

            bool sub_str = false;
            if (qr_code.size() >= 3) {
                std::string teer = qr_code.substr(qr_code.size() - 3);
                if (teer == "|NG") {
                    sub_str = true;
                }
                if (sub_str) {
                    qr_code = qr_code.substr(0, qr_code.size() - 3);
                }
            }

            qr_codes.push_back(qr_code);
        }

        // 结尾可能有分隔符
        bool all_is_ = true;
        if (qr_codes.empty()) {
            continue;
        }
        for (auto char_it : qr_codes.back()) {
            if ('-' != char_it) {
                all_is_ = false;
                break;
            }
        }
        if (all_is_) {
            qr_codes.pop_back();
        }
    }
    doc.close();
    int a = 0;
}

// [no touch] create select qr file
void Process_SpeedReadQRFile_QAxObject(std::vector<std::string>& ans, std::string file_path, std::string col_name)
{
    MXT_FUNCTION_TIMER();
    MXT_ERROR_LOGGER << STR_PARAM(file_path);

    auto& qr_codes = ans;

    ExcelThread excel_thrd;
    bool ret = excel_thrd.OpenExcel(file_path.c_str());
    if (!ret) {
        return;
    }

    std::string current_sheet = excel_thrd.getCurrentSheetName().toStdString();
    int nRow = 0, nCol = 0;
    QList<QStringList> all_datas;
    excel_thrd.getRowAndColumn(nRow, nCol);
    MXT_INFO_LOGGER << STR_PARAM(nRow) << ", " << STR_PARAM(nCol);
    excel_thrd.getExcelAllContent(all_datas);

    int qrColIndex = -1;
    for (int c = 0; c < nCol; c++) {
        if (col_name.c_str() == all_datas[0][c]) {
            qrColIndex = c;
        }
    }
    MXT_INFO_LOGGER << STR_PARAM(qrColIndex);
    if (-1 == qrColIndex) {
        MXT_ERROR_LOGGER << "no find QR col";
        return;
    }

    for (int r = 1; r < nRow; r++) {
        std::string strqr = all_datas[r][qrColIndex].toStdString();
        bool sub_str = false;
        if (strqr.size() >= 3) {
            std::string teer = strqr.substr(strqr.size() - 3);
            if (teer == "|NG") {
                sub_str = true;
            }
            if (sub_str) {
                strqr = strqr.substr(0, strqr.size() - 3);
            }
        }
        qr_codes.push_back(strqr);
    }

    bool all_is_ = true;
    for (auto cit : qr_codes.back()) {
        if (cit != '-') {
            all_is_ = false;
        }
    }
    if (all_is_) {
        qr_codes.pop_back();
    }
    MXT_KEY_LOGGER << "read success, nQr = " << qr_codes.size();


    excel_thrd.CloseExcel();
}

void Process_CreateSNSelectFile_OpenXLSX() {
    MXT_FUNCTION_TIMER();

    FileSelectSNContent sn_content;
    FileSelectSNContent sn_content_read;
    std::string  desktop_dir = getDesktopPath();
    const std::string from_file_path = desktop_dir + '\\' + g_from_file_name;

    Process_SpeedReadQRFile_OpenXLSX(sn_content.select_sn, from_file_path, "QR");
    // Process_SpeedReadQRFile_QAxObject(sn_content.select_sn, from_file_path, "QR");

    // log sn
    {
        std::stringstream tmp_ss;
        for (auto qrit : sn_content.select_sn) {
            tmp_ss << "\n" << qrit;
        }
        MXT_INFO_LOGGER << tmp_ss.str();
    }
    SaveFileEncrypt(g_to_dir, g_to_file_name, sn_content);
    LoadFileDecrypt(g_to_dir, g_to_file_name, sn_content_read);
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    // Process();
    // MicrosoftExcel();
    Process_CreateSNSelectFile_OpenXLSX();
    system("pause");
    return 0;
}
