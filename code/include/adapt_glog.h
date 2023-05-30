#pragma once

#include "mtxlogger.h"
#pragma comment(lib, "glog\\lib\\Release\\glog.lib")

#include <Windows.h>
#include <libloaderapi.h>
#include <fileapi.h>
#include <string>

inline
void CreateDirectoryRecursively(const std::string& directory)
{
#if defined(_WIN32)
    static const std::string separators("\\/");
    DWORD fileAttributes = ::GetFileAttributesA(directory.c_str());
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
        // Recursively do it all again for the parent directory, if any
        std::size_t slashIndex = directory.find_last_of(separators);
        if (slashIndex != std::wstring::npos) {
            CreateDirectoryRecursively(directory.substr(0, slashIndex));
        }
        BOOL result = ::CreateDirectoryA(directory.c_str(), nullptr);
        if (result == FALSE) {
            //throw std::runtime_error("Could not create directory");
        }
    } else {
        bool isDirectoryOrJunction =
            ((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
            ((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

        if (!isDirectoryOrJunction) {
            //throw std::runtime_error(
            //  "Could not create directory because a file with the same name exists"
            //);
        }
    }
#else
    int is_create = mkdir(directory.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
    if (!is_create) {

    } else {
        printf("create path failed! error code\r\n");
    }
#endif
}

struct GlogIniter {
    GlogIniter()
    {
        const char* logPrefix = "";
        const char* logSuffix = "";

        std::string path = "";
        std::string s_time = "";
        char temp[128] = { 0 };
        time_t tt = time(NULL);
        struct tm t;
#if defined(_WIN32)
        localtime_s(&t, &tt);
        sprintf_s(temp, sizeof(temp), "%d-%02d-%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
#else
        localtime_r(&tt, &t);
        snprintf(temp, sizeof(temp), "%d-%02d-%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
#endif
        s_time.append(temp);

#if defined(_WIN32)
        char szFilePath[512] = { 0 };
        GetModuleFileNameA(NULL, szFilePath, 512);
        (strrchr(szFilePath, '\\'))[0] = 0;
        path = szFilePath;
#else
        char* buffer = getcwd(NULL, 0);
        path = std::string(buffer);
#endif
        path.append("/log/");
        path += s_time;
        path.append("/");

        std::string suffix = strlen(logSuffix) ? "_" : "";
        suffix.append(logSuffix);

        google::InitGoogleLogging((const char*)(__FILE__));
        google::SetStderrLogging(google::GLOG_INFO);
        google::SetLogFilenameExtension("log_");

        //google::FlushLogFiles(google::GLOG_INFO);
        //google::InstallFailureSignalHandler();
        //google::InstallFailureWriter(&SignalHandle);

        CreateDirectoryRecursively(path); //创建日志目录
        google::SetLogDestination(google::GLOG_INFO, (path + logPrefix + suffix + "_info_").c_str());
        google::SetLogDestination(google::GLOG_WARNING, (path + logPrefix + suffix + "_warn_").c_str());
        google::SetLogDestination(google::GLOG_ERROR, (path + logPrefix + suffix + "_error_").c_str());
        google::SetLogDestination(google::GLOG_FATAL, (path + logPrefix + suffix + "_fatal_").c_str());

        // 默认设置，可以通过配置文件修改
        FLAGS_max_log_size = 10;        // 设置日志文件大小为10M
        FLAGS_logbufsecs = 0;           // 打开实时写入
        FLAGS_colorlogtostderr = TRUE;  // 打开彩色日志
        FLAGS_minloglevel = 0;          // 设置日志等级
        FLAGS_v = 15;                   // 设置自定义日志等级
    }

    static GlogIniter& Instance() {
        static GlogIniter rt;
        return rt;
    }

};

const static GlogIniter g_golg_initer = GlogIniter::Instance();

template<class InitFunctor>
static void InitOnce()
{
    static  InitFunctor init_functor;
}

