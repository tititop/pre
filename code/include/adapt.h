#pragma once

#include "adapt_glog.h"
#include "adapt/adapt_struct2x.h"


#include <functional>

struct RAIIST {
    using func_t = _STD function<void()>;
    RAIIST(func_t start, func_t exit) : start_(start), exit_(exit)
    {
        start_();
    }
    ~RAIIST()
    {
        exit_();
    }

    func_t start_;
    func_t exit_;
};
