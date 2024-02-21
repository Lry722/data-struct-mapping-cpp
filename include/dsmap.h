#pragma once

#include "utils.h"
#include <boost/pfr.hpp>

namespace dsmap
{

#define STRUCT_WITH_PROPERTIES(name, definition)                                 \
    struct name                                                                  \
    {                                                                            \
        definition;                                                              \
        template <std::size_t I, typename FuncT, typename DataT>                 \
        friend void dsmap::utils::forEachProperty(DataT &data, FuncT &&func);    \
                                                                                 \
    private:                                                                     \
        struct Properties                                                        \
        {                                                                        \
        public:                                                                  \
            static const std::string_view get(std::size_t i)                     \
            {                                                                    \
                if (!initialized) [[unlikely]]                                   \
                {                                                                \
                    properties_ = dsmap::utils::parsePropertyNames(#definition); \
                    initialized = true;                                          \
                }                                                                \
                return properties_[i];                                           \
            }                                                                    \
                                                                                 \
        private:                                                                 \
            inline static bool initialized{false};                               \
            inline static std::vector<std::string> properties_{};                \
        };                                                                       \
    }

    namespace utils {

        template <std::size_t I, typename FuncT, typename DataT>
        void forEachProperty(DataT &data, FuncT &&func)
        {
            if constexpr (I >= boost::pfr::tuple_size<DataT>::value)
                return;
            else
            {
                func(DataT::Properties::get(I), boost::pfr::get<I>(data));
                forEachProperty<I + 1>(data, std::forward<FuncT>(func));
            }
        }

        template <class FuncT, typename DataT>
        void forEachProperty(DataT &data, FuncT &&func)
        {
            forEachProperty<0>(data, std::forward<FuncT>(func));
        }

    }

    void toStruct(const auto &source, auto &target, auto &&fetch)
    {
        static_assert(std::is_aggregate_v<std::decay_t<decltype(target)>>,
                      "====================> dsmap: Target must be aggregate class!");
        utils::forEachProperty(target,
                               [&](std::string_view property_name, auto &property)
                               {
                                   fetch(source[property_name], property);
                               });
    }

} // namespace dsmap
