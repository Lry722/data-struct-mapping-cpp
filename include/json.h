#pragma once

#include "dsmap.h"
#include <nlohmann/json.hpp>

namespace dsmap
{

    template <typename TargetT>
    void yaml2struct(const nlohmann::json &source, TargetT &target)
    {
        toStruct(source, target, [](const nlohmann::json &source, auto &property) {

        });
    }

} // namespace dsmap