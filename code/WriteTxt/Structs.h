#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

enum MXTValueType {
    VALUE_UNDEFINED = 0,
    VALUE_FIX = 0,
    VALUE_BOOL,          // 布尔
    VALUE_INT,           // 整形
    VALUE_DOUBLE,        // 双精度
    VALUE_STRING         // 字符
};

struct MXTMeasurementItem {

    int                     group = 0;
    int                     cell_id = 0;
    bool                    is_pass = false;                    // 此项是否通过
    int                     value_type = VALUE_UNDEFINED;       // 数据类型(默认整型)
    double                  measurement = 0.0;                  // 实际的测量值(双精度)，可以转其它类型，节省序列化空间
    std::string             measurement_str = "";               // OCR QR

    MXTMeasurementItem()
    {
    }

    MXTMeasurementItem(bool _pass, int _value_type, double _measurement, const std::string& _cell_str = "", int _group = 0)
    {
        measurement = _measurement;
        is_pass = _pass;
        group = _group;
        value_type = _value_type;
        measurement_str = _cell_str;
    }

    // 浮点转整型
    static inline int32_t f_toint(double x)
    {
        x += 6755399441055744.0;
        return *(int32_t*)&x;
    }

    std::string ToString() const
    {
        std::stringstream ss;
        std::string ans;
        switch (value_type) {
        case VALUE_INT:
            ans = std::to_string(f_toint(measurement));
            break;
        case VALUE_DOUBLE:
            ss << std::setprecision(7) << measurement;
            ans = ss.str();
            break;
        case VALUE_BOOL:
            ans = ((1 == f_toint(measurement)) ? "TRUE" : "FALSE");
            break;
        case VALUE_STRING:
            ans = measurement_str;
            break;
        case VALUE_FIX:
            ans = "N/A";
            break;
        default:
            ans = "NULL";
            break;
        }
        return ans;
    }
};

template<class _TData>
struct DataDetail {
    using data_t = _TData;

    _STD string data_flag;                      // table xx
    _STD vector<_STD string> col_names;
    _STD vector<_STD string> row_names;

    _STD vector<data_t> datas;

};


template<class _TData>
struct FuncTraits {
    static std::string ToString(_TData data)
    {
        return data.ToString();
    }
};

