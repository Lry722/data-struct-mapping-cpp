#pragma once

#include <algorithm>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <boost/pfr.hpp>

namespace dsmap::utils
{

    template <typename, typename = void>
    struct has_value_type : std::false_type
    {
    };

    template <typename T>
    struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type
    {
    };

    template <typename T>
    using is_not_string = std::negation<std::is_same<T, std::string>>;

    template <typename T>
    using is_sequence = std::conjunction<
        is_not_string<T>,
        has_value_type<T>>;

    template <typename T>
    inline constexpr bool is_sequence_v = is_sequence<T>::value;

    template <size_t I, typename FuncT, typename DataT>
    void applyOnProperties(DataT &data, FuncT &&func)
    {
        if constexpr (I >= boost::pfr::tuple_size<DataT>::value)
        {
            return;
        }
        else
        {
            func(DataT::Properties::get(I), boost::pfr::get<I>(data));
            applyOnProperties<I + 1>(data, std::forward<FuncT>(func));
        }
    }

    template <class FuncT, typename DataT>
    void applyOnProperties(DataT &data, FuncT &&func)
    {
        applyOnProperties<0>(data, std::forward<FuncT>(func));
    }

    std::size_t FindClosingParenthesis(std::string_view str, size_t start_pos);

    std::vector<std::string> parsePropertyNames(const std::string &definition);

    inline void ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                        { return !std::isspace(ch); }));
    }

    inline void rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                             [](unsigned char ch)
                             { return !std::isspace(ch); })
                    .base(),
                s.end());
    }

    inline void trim(std::string &s)
    {
        rtrim(s);
        ltrim(s);
    }

    inline std::string trim_copy(std::string s)
    {
        trim(s);
        return s;
    }

} // namespace dsmap::utils
