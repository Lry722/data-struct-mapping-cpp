#pragma once

#include <algorithm>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <boost/pfr.hpp>

namespace dsmap::utils
{

    template <typename T>
    concept is_sequence =
        requires {
            typename T::value_type;
        } &&
        !std::is_same_v<T, std::string>;

    template <size_t I, typename FuncT, typename DataT>
    void forEachProperty(DataT &data, FuncT &&func)
    {
        if constexpr (I >= boost::pfr::tuple_size<DataT>::value)
        {
            return;
        }
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

    std::size_t FindMatchingParenthesis(std::string_view str, size_t start_pos);

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
