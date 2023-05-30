#pragma once

#include <string>
#include <errno.h>

#include "adapt_glog.h"

#include "struct2x/include/json/JSONReader.h"
#include "struct2x/include/json/JSONWriter.h"
#include "struct2x/include/struct2x.h"
#pragma comment(lib, "struct2x\\lib\\struct2x.lib")

#include <stdio.h>

template<class _T>
void SaveFileEncrypt(_STD string& file_dir, _STD string& file_name, _T& src_data)
{
    std::string file_path = file_dir + "\\" + file_name;
    CreateDirectoryRecursively(file_dir);
    FILE* pFile = nullptr;
    pFile = fopen(file_path.c_str(), "wb");
    if (nullptr == pFile) {
        MXT_ERROR_LOGGER << "open file error, file = " << file_path;
        return;
    }
    std::string json_str;
    struct2x::JSONReader jr;
    jr << src_data;
    jr.toString(json_str);
    size_t nWrite = fwrite(json_str.c_str(), 1, json_str.size(), pFile);

    MXT_KEY_LOGGER << "write data size = " << nWrite << ", file name = " << file_path;
    fflush(pFile);
    fclose(pFile);
    pFile = nullptr;
}

template<class _T>
void LoadFileDecrypt(_STD string& file_dir, _STD string& file_name, _T& dst_data)
{
    std::string file_path = file_dir + "\\" + file_name;
    FILE* pFile = nullptr;
    pFile = fopen(file_path.c_str(), "rb+");
    if (nullptr == pFile) {
        MXT_ERROR_LOGGER << "open file error, file = " << file_path;
        return;
    }
    fseek(pFile, 0, SEEK_END);
    size_t fileLen = ftell(pFile);
    char* src_data = new char[fileLen + 1];
    memset(src_data, 0, fileLen + 1);
    size_t nRead = fread(src_data, 1, fileLen, pFile);
    struct2x::JSONWriter jw(src_data);
    jw >> dst_data;

    MXT_KEY_LOGGER << "read  data size = " << nRead << ", file name = " << file_path;
    fclose(pFile);
    pFile = nullptr;

}

