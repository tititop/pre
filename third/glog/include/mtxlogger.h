/*********************************************************************************
  *Copyright(C),Matrixtime
  *FileName:    mtxlogger.h
  *Author:      xxx
  *Version:     1.0.0
  *Date:        2021/8/16

  *Description: 基于Glog的方便使用的宏。

                1、其中打印函数使用__func__，而不用__FUNCTION__
                   因为__FUNCTION__它会打印类名称，而模板类名称太长

                2、VLOG通过环境变量来控制等级（开发时可以在工程的“属性->调试->环境”中方便设置）
                   例如GLOG_v=1，则输出VLOG(0)、VLOG(1)
                   例如GLOG_vmodule=mapreduce=2,file=1,gfs*=3，GLOG_vmodule会覆盖GLOG_v

  *History:     文件更改历史：Date-修该时间、Author-修改者、Modification-修改内容
**********************************************************************************/

#pragma once
#ifndef __MXT_LOGGER_H__
#define __MXT_LOGGER_H__

#ifndef GLOG_NO_ABBREVIATED_SEVERITIES
#define GLOG_NO_ABBREVIATED_SEVERITIES
#endif // !GLOG_NO_ABBREVIATED_SEVERITIES

#ifndef GOOGLE_GLOG_DLL_DECL
#define GOOGLE_GLOG_DLL_DECL
#endif // !GOOGLE_GLOG_DLL_DECL

#include "logging.h"

//#if defined(_WIN32)
//#include <Windows.h>
//#else
//#endif // DEBUG

const unsigned int N_LOG_BUF_LEN = 8192;

#if defined(_WIN32)
#ifndef PRINT_INFO
#define PRINT_INFO(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};_snprintf_s(szlog, N_LOG_BUF_LEN, format, __VA_ARGS__);LOG(INFO) << "[" << __func__ << "][INFO] " << szlog;}
#endif //PRINT_LOG

#ifndef PRINT_WARN
#define PRINT_WARN(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};_snprintf_s(szlog, N_LOG_BUF_LEN, format, __VA_ARGS__);LOG(WARNING) << "[" << __func__ << "][WARN] " << szlog; }
#endif //PRINT_WARN

#ifndef PRINT_ERROR
#define PRINT_ERROR(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};_snprintf_s(szlog, N_LOG_BUF_LEN, format, __VA_ARGS__);LOG(ERROR) << "[" << __func__ << "][ERROR] " << szlog;}
#endif //PRINT_WARN

#ifndef PRINT_FATAL
#define PRINT_FATAL(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};_snprintf_s(szlog, N_LOG_BUF_LEN, format, __VA_ARGS__);LOG(FATAL) << "[" << __func__ << "][FATAL] " << szlog;}
#endif //PRINT_FATAL

#ifndef PRINT_KEY
#define PRINT_KEY(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};_snprintf_s(szlog, N_LOG_BUF_LEN, format, __VA_ARGS__);LOG(WARNING) << "[" << __func__ << "][KEY] " << szlog; }
#endif //PRINT_KEY

#ifndef PRINT_VLOG1
#define PRINT_VLOG1(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};_snprintf_s(szlog, N_LOG_BUF_LEN, format, __VA_ARGS__);LOG_IF(INFO, FLAGS_v & 1) << "[" << __func__ << "][DEBUG] " << szlog;}
#endif //PRINT_VLOG1

#ifndef PRINT_VLOG2
#define PRINT_VLOG2(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};_snprintf_s(szlog, N_LOG_BUF_LEN, format, __VA_ARGS__);LOG_IF(INFO, FLAGS_v & 2) << "[" << __func__ << "][TRACE] " << szlog;}
#endif //PRINT_VLOG2

#ifndef PRINT_VLOG4
#define PRINT_VLOG4(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};_snprintf_s(szlog, N_LOG_BUF_LEN, format, __VA_ARGS__);LOG_IF(INFO, FLAGS_v & 4) << "[" << __func__ << "][DETAIL] " << szlog;}
#endif //PRINT_VLOG4

#ifndef PRINT_VLOG8
#define PRINT_VLOG8(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};_snprintf_s(szlog, N_LOG_BUF_LEN, format, __VA_ARGS__);LOG_IF(INFO, FLAGS_v & 8) << "[" << __func__ << "][TIMER] " << szlog;}
#endif //PRINT_VLOG8

#ifndef PRINT_VLOGN
#define PRINT_VLOGN(lvl, format, ...) {char szlog[N_LOG_BUF_LEN] = {0};_snprintf_s(szlog, N_LOG_BUF_LEN, format, __VA_ARGS__);LOG_IF(INFO, FLAGS_v & lvl) << "[" << __func__ << "][VLOG" << lvl << "] " << szlog;}
#endif //PRINT_VLOGN

#ifndef PRINT_IorW
#define PRINT_IorW(condition, format, ...) {if (condition) PRINT_INFO(format, __VA_ARGS__) else PRINT_WARN(format, __VA_ARGS__)}
#endif //PRINT_IorW

#ifndef PRINT_IorE
#define PRINT_IorE(condition, format, ...) {if (condition) PRINT_INFO(format, __VA_ARGS__) else PRINT_ERROR(format, __VA_ARGS__)}
#endif //PRINT_IorE
#else
#ifndef PRINT_INFO
#define PRINT_INFO(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};snprintf(szlog, N_LOG_BUF_LEN, format, ##__VA_ARGS__);LOG(INFO)<<szlog;}
#endif //PRINT_INFO

#ifndef PRINT_DEBUG
#define PRINT_DEBUG(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};snprintf(szlog, N_LOG_BUF_LEN, format, ##__VA_ARGS__);LOG(INFO)<<szlog;}
#endif //PRINT_DEBUG

#ifndef PRINT_WARN
#define PRINT_WARN(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};snprintf(szlog, N_LOG_BUF_LEN, format, ##__VA_ARGS__);LOG(WARNING)<<szlog; }
#endif //PRINT_WARN

#ifndef PRINT_ERROR
#define PRINT_ERROR(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};snprintf(szlog, N_LOG_BUF_LEN, format, ##__VA_ARGS__);LOG(ERROR)<<szlog;}
#endif //PRINT_WARN

#ifndef PRINT_FATAL
#define PRINT_FATAL(format, ...) {char szlog[N_LOG_BUF_LEN] = {0};snprintf(szlog, N_LOG_BUF_LEN, format, ##__VA_ARGS__);LOG(FATAL)<<szlog;}
#endif //PRINT_FATAL
#endif

#ifndef MXT_LOGGER_VLOGN
#define MXT_LOGGER_VLOGN    PRINT_VLOGN
#endif //MXT_LOGGER_VLOG

#ifndef MXT_LOGGER_TIMER
#define MXT_LOGGER_TIMER   PRINT_VLOG8
#endif //MXT_LOGGER_TIMER

#ifndef MXT_LOGGER_DETAIL
#define MXT_LOGGER_DETAIL   PRINT_VLOG4
#endif //MXT_LOGGER_DETAIL

#ifndef MXT_LOGGER_TRACE
#define MXT_LOGGER_TRACE    PRINT_VLOG2
#endif //MXT_LOGGER_TRACE

#ifndef MXT_LOGGER_DEBUG
#define MXT_LOGGER_DEBUG    PRINT_VLOG1
#endif //MXT_LOGGER_DEBUG

#ifndef MXT_LOGGER_INFO
#define MXT_LOGGER_INFO     PRINT_INFO
#endif //MXT_LOGGER_INFO

#ifndef MXT_LOGGER_WARN
#define MXT_LOGGER_WARN     PRINT_WARN
#endif //MXT_LOGGER_WARN

#ifndef MXT_LOGGER_ERROR
#define MXT_LOGGER_ERROR    PRINT_ERROR
#endif //MXT_LOGGER_ERROR

#ifndef MXT_LOGGER_FATAL
#define MXT_LOGGER_FATAL    PRINT_FATAL
#endif //MXT_LOGGER_FATAL

#ifndef MXT_LOGGER_KEY
#define MXT_LOGGER_KEY      PRINT_KEY
#endif //MXT_LOGGER_KEY

#ifndef MXT_LOGGER_IorW
#define MXT_LOGGER_IorW     PRINT_IorW
#endif //MXT_LOGGER_IorW

#ifndef MXT_LOGGER_IorE
#define MXT_LOGGER_IorE     PRINT_IorE
#endif //MXT_LOGGER_IorE

#ifndef MXT_FUNCTION_TIMER
#include <chrono>
namespace mxt
{
class MXTFunctionTimer
{
public:
    explicit MXTFunctionTimer(const char* name)
        : _function_name((name == nullptr) ? "" : name),
          _t_start(std::chrono::system_clock::now())
    {
        MXT_LOGGER_KEY("%s start.", _function_name.c_str())
    }

    ~MXTFunctionTimer()
    {
        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - _t_start);
        MXT_LOGGER_KEY("%s end ,cost %lld ms.", _function_name.c_str(), duration.count());
    }

private:
    std::string _function_name;
    std::chrono::system_clock::time_point  _t_start;
};
}
#define MXT_FUNCTION_TIMER() mxt::MXTFunctionTimer t(__func__)
#endif

// 另一套宏
// 用于支持<<操作
// 可以缓解字符串过长问题，即由GLOG内部处理过长问题
// 而且<<会检查类型，如果是QString会编译不过，避免%s配QString的问题

#ifndef MXT_TIMER_LOGGER
#define MXT_TIMER_LOGGER    LOG_IF(INFO, FLAGS_v & 8) << "[" << __func__ << "][TIMER] "
#endif //MXT_TIMER_LOGGER

#ifndef MXT_DETAIL_LOGGER
#define MXT_DETAIL_LOGGER    LOG_IF(INFO, FLAGS_v & 4) << "[" << __func__ << "][DETAIL] "
#endif //MXT_DETAIL_LOGGER

#ifndef MXT_TRACE_LOGGER
#define MXT_TRACE_LOGGER     LOG_IF(INFO, FLAGS_v & 2) << "[" << __func__ << "][TRACE] "
#endif //MXT_TRACE_LOGGER

#ifndef MXT_DEBUG_LOGGER
#define MXT_DEBUG_LOGGER     LOG_IF(INFO, FLAGS_v & 1) << "[" << __func__ << "][DEBUG] "
#endif //MXT_DEBUG_LOGGER

#ifndef MXT_INFO_LOGGER
#define MXT_INFO_LOGGER     LOG(INFO) << "[" << __func__ << "][INFO] "
#endif //MXT_INFO_LOGGER

#ifndef MXT_WARN_LOGGER
#define MXT_WARN_LOGGER     LOG(WARNING) << "[" << __func__ << "][WARN] "
#endif //MXT_WARN_LOGGER

#ifndef MXT_ERROR_LOGGER
#define MXT_ERROR_LOGGER    LOG(ERROR) << "[" << __func__ << "][ERROR] "
#endif //MXT_ERROR_LOGGER

#ifndef MXT_FATAL_LOGGER
#define MXT_FATAL_LOGGER    LOG(FATAL) << "[" << __func__ << "][FATAL] "
#endif //MXT_ERROR_LOGGER

#ifndef MXT_KEY_LOGGER
#define MXT_KEY_LOGGER      LOG(WARNING) << "[" << __func__ << "][KEY] "
#endif //MXT_KEY_LOGGER

#ifndef MXT_WARN_LOGGER_IF
#define MXT_WARN_LOGGER_IF(condition)      LOG_IF(WARNING, condition) << "[" << __func__ << "][WARN] "
#endif //MXT_WARN_LOGGER_IF

#ifndef MXT_ERROR_LOGGER_IF
#define MXT_ERROR_LOGGER_IF(condition)     LOG_IF(ERROR, condition) << "[" << __func__ << "][ERROR] "
#endif //MXT_ERROR_LOGGER_IF

// 通过环境变量来控制等级，环境变量可以在工程的“属性->调试->环境”中方便设置
// 按位判断是否输出，x=1、2、4、8、16...1024
// 例如：FLAGS_v=6=2+4，FLAGS_v=0=关闭，FLAGS_v=-1=全开
#ifndef MXT_VLOG_LOGGER
#define MXT_VLOG_LOGGER(X)  LOG_IF(INFO, (FLAGS_v & X || FLAGS_v == -1)) << "[" << __func__ << "][VLOG" << X << "] "
#endif //MXT_INFO_LOGGER

#endif //_MXT_LOGGER_H_



