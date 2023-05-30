#include "WriteTxt.h"

#include <algorithm>
#include <functional>
#include <numeric>
#include <string>


#define NEXT_LINE "\r\n"

template<typename _TData, typename _TFuncTraits = void>
inline void PrintLine(
    _STD ostream& os
    , const _STD vector<_TData>& datas
    , const size_t data_offset, const size_t data_count
    , const _STD vector<size_t> col_buf_sizes
    , char* buffer
    , const size_t buffer_size
)
{
    size_t buf_offset = 0;
    for (size_t i = 0; i < col_buf_sizes.size(); i++) {
        const size_t& section_buf_size = col_buf_sizes[i];
        const size_t data_index = data_offset + i;
        const _TData& data_item = datas[data_index];

        _STD string tmp_str = _TFuncTraits::ToString(data_item);
        memcpy(buffer + buf_offset, tmp_str.c_str(), tmp_str.size());

        buf_offset += section_buf_size;
    }

    buffer[buffer_size] = '\0';
    _STD string tmp_str(buffer, buffer_size);
    os << tmp_str;
}


template<typename _TData, typename _TFuncTraits = MXTMeasurementItemFuncTraits>
long PrintArray(
    _STD ostream& os
    , const _STD vector<_TData>& datas
    , size_t row_size, size_t col_size
)
{
    size_t space_size = 2;

    _STD vector<size_t> adapt_col_size(col_size, 0);
    size_t line_len = 0;
    // adapt col size
    {
        for (size_t col = 0; col < col_size; col++) {
            auto& col_max = adapt_col_size[col];

            size_t data_offset = col - row_size;
            for (size_t row = 0; row < row_size; row++) {
                data_offset += row_size;
                _STD string data_tmpstr = _TFuncTraits::ToString(datas[data_offset]);
                if (col_max < data_tmpstr.size()) {
                    col_max = data_tmpstr.size();
                }
            }
        }

        std::stringstream tmp_logss;
        _STD for_each(adapt_col_size.begin(), adapt_col_size.end(), [&](size_t& val) {
            val += space_size;
            line_len += val;
            tmp_logss << val << ", ";
        });

        MXT_KEY_LOGGER << "line length = " << line_len << ", data col size info: " << tmp_logss.str();
    }

    // memory
    char* buf = nullptr;
    buf = (char*)malloc(line_len + 1);
    if (buf == nullptr) {
        MXT_ERROR_LOGGER << "malloc line memory error";
        return -1;
    }

    // RAII
    _STD string split_line = _STD string(line_len, '-');
    auto raii_start = [&]() {
        os << NEXT_LINE << split_line;
    };
    auto raii_exit = [&]() {
        os << NEXT_LINE << split_line;
        if (buf != nullptr) {
            free(buf);
            buf = nullptr;
        }
    };
    RAIIST raii(raii_start, raii_exit);

    size_t data_offset = -col_size;
    for (size_t col = 0; col < col_size; col++) {
        data_offset += col_size;
        memset(buf, ' ', line_len);
        os << NEXT_LINE;
        PrintLine<_TData, _TFuncTraits>(os, datas, data_offset, col_size, adapt_col_size, buf, line_len);
    }
}

long WriteTxt(_STD ostream& os, const MeasurementDataDetail& data_detail)
{
    const size_t item_space_size = 2;
    const size_t row_size = data_detail.row_names.size();
    const size_t col_size = data_detail.col_names.size();
    const size_t data_size = data_detail.datas.size();

    MXT_INFO_LOGGER << "row  size = " << row_size
                    << ", col  size = " << col_size
                    << ", data size = " << data_size;

    // check
    size_t check_data_size = row_size * col_size;
    if (check_data_size != data_size) {
        MXT_ERROR_LOGGER << "check size error: row * col = " << check_data_size
                         << ", data size = " << data_size;
        return -1;
    }

    // PrintArray
    {
        const size_t arary_row = row_size + 1;
        const size_t arary_col = col_size + 1;
        const size_t array_size = arary_row * arary_col;

        MeasurementDataDetail::data_t default_data(true, VALUE_STRING, 0.0, "");
        _STD vector<MeasurementDataDetail::data_t> print_array(array_size, default_data);

        // 组织数据
        {
            for (size_t col_offset = 0; col_offset < col_size; col_offset++) {
                MeasurementDataDetail::data_t tmp_data(true, VALUE_STRING, 0.0, data_detail.col_names[col_offset]);
                size_t index = col_offset + 1;                                // 第一个buf为空
                print_array[index] = tmp_data;
            }
            size_t data_row_index = 0;
            for (size_t row_offset = 0; row_offset < row_size; row_offset++) {
                data_row_index += arary_col;                                  // 第一行为row
                MeasurementDataDetail::data_t tmp_data(true, VALUE_STRING, 0.0, data_detail.row_names[row_offset]);
                print_array[data_row_index] = tmp_data;
            }
            for (size_t col_offset = 0; col_offset < col_size; col_offset++) {
                for (size_t row_offset = 0; row_offset < row_size; row_offset++) {
                    size_t array_index = ((row_offset + 1) * arary_col) + (col_offset + 1);
                    size_t data_index = (row_offset * col_size) + col_offset;
                    print_array[array_index] = data_detail.datas[data_index];
                }
            }
        }

        PrintArray(os, print_array, arary_row, arary_col);
    }
    return 0;
}
