#pragma once

#include "dsmap.h"
#include <yaml-cpp/yaml.h>

namespace dsmap
{
    void yaml2struct(const YAML::Node &source, auto &target);

    static void fetch_single(const YAML::Node &source, auto &property);

    static void fetch_sequence(const YAML::Node &source, auto &property);

    static const auto &fetch = [](const YAML::Node &source, auto &property)
    {
        using PropertyType = std::remove_reference_t<decltype(property)>;

        if constexpr (utils::is_sequence<PropertyType>)
            fetch_sequence(source, property);
        else
            fetch_single(source, property);
    };

    static void fetch_single(const YAML::Node &source, auto &property)
    {
        // Source should not be a sequence here.
        assert(!source.IsSequence());

        using PropertyType = std::remove_reference_t<decltype(property)>;

        if constexpr (std::is_aggregate_v<PropertyType>)
            yaml2struct(source, property);
        else if (!source.IsNull())
        {
            property =
                source.template as<std::remove_reference_t<decltype(property)>>();
        }
    }

    static void fetch_sequence(const YAML::Node &source, auto &property)
    {
        // Source should be a sequence here.
        assert(source.IsSequence());

        property.resize(source.size());
        for (int i = 0; i < source.size(); ++i)
            fetch(source[i], property[i]);
    }

    void yaml2struct(const YAML::Node &source, auto &target)
    {
        toStruct(source, target, fetch);
    }

} // namespace dsmap
