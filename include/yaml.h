#pragma once

#include "dsmap.h"
#include <yaml-cpp/yaml.h>

namespace dsmap {

template <typename TargetT>
void yaml2struct(const YAML::Node &source, TargetT &target);

template <typename TargetT>
static void fetch_single(const YAML::Node &source, TargetT &property);

template <typename TargetT>
static void fetch_sequence(const YAML::Node &source, TargetT &property);

static const auto &fetch = [](const YAML::Node &source, auto &property) {
  using PropertyType = std::remove_reference_t<decltype(property)>;
  // Property is a container only if it has value_type and isn't a string. Then
  // source node should be a sequence.
  if constexpr (!std::is_same_v<PropertyType, std::string> &&
                utils::has_value_type<PropertyType>::value) {
    fetch_sequence(source, property);
  } else {
    fetch_single(source, property);
  }
};

template <typename TargetT>
static void fetch_single(const YAML::Node &source, TargetT &property) {
  // Source should not be a sequence here.
  assert(!source.IsSequence());

  using PropertyType = std::remove_reference_t<decltype(property)>;

  if constexpr (std::is_aggregate_v<PropertyType>)
    yaml2struct(source, property);
  else if (!source.IsNull()) {
    property =
        source.template as<std::remove_reference_t<decltype(property)>>();
  }
}

template <typename TargetT>
static void fetch_sequence(const YAML::Node &source, TargetT &property) {
  // Source should be a sequence here.
  assert(source.IsSequence());

  property.resize(source.size());
  for (int i = 0; i < source.size(); ++i)
    fetch(source[i], property[i]);
}

template <typename TargetT>
void yaml2struct(const YAML::Node &source, TargetT &target) {
  toStruct(source, target, fetch);
}

} // namespace dsmap

