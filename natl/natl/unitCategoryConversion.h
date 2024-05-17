#pragma once

//own
#include "unitsBase.h"
#include "unitsImperial.h"
#include "unitsMetric.h"

//interface
namespace natl {
    //length 
    template<typename FloatDataType, typename FromTag, typename ToTag>
    struct UnitCategoryConversionFactor<
        impl::ImperialLengthTagGroup, impl::MetricLengthTagGroup, FromTag, ToTag, FloatDataType>
        : public impl::UnitCategoryConversionFactorImpl<FloatDataType, 0.0254, impl::InchTag, impl::MeterTag, FromTag, ToTag> {
    };
    template<typename FloatDataType, typename FromTag, typename ToTag>
    struct UnitCategoryConversionFactor<
        impl::MetricLengthTagGroup, impl::ImperialLengthTagGroup, FromTag, ToTag, FloatDataType>
        : public impl::UnitCategoryConversionFactorImpl<FloatDataType, 39.3701, impl::MeterTag, impl::InchTag, FromTag, ToTag> {
    };
}