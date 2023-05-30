#pragma once

#include "adapt/adapt.h"

#include "Structs.h"

using MeasurementDataDetail = DataDetail<MXTMeasurementItem>;
using MXTMeasurementItemFuncTraits = FuncTraits<MXTMeasurementItem>;

VISITSTRUCT(MXTMeasurementItem, cell_id, is_pass, group, value_type, measurement, measurement_str)
VISITSTRUCT(MeasurementDataDetail, data_flag, col_names, row_names, datas)

long WriteTxt(_STD ostream& os, const MeasurementDataDetail& detail_data);

