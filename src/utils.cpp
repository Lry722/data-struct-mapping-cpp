#include "utils.h"

#include <cstddef>
#include <regex>
#include <string_view>

namespace dsmap::utils {

std::size_t FindClosingParenthesis(std::string_view str, size_t start_pos) {
  int open_brackets = 0;
  for (size_t i = start_pos; i < str.size(); ++i) {
    if (str[i] == '(') {
      open_brackets++;
    } else if (str[i] == ')') {
      open_brackets--;
      if (open_brackets == 0) {
        return i;
      }
    }
  }
  // 如果没有找到匹配的右括号，则返回std::string::npos
  return std::string::npos;
}

std::vector<std::string> parsePropertyNames(const std::string &definition) {
  static const std::string_view kMacro{"STRUCT_WITH_PROPERTIES"};
  static const std::regex re(R"(\s+([0-9a-zA-Z_]+)[^;\s]*;)");

  std::vector<std::string> result{};

  size_t pos = 0;
  std::sregex_iterator it(definition.begin(), definition.end(), re);
  std::sregex_iterator end;
  while (true) {
    pos = definition.find(kMacro, pos);
    if (pos == std::string::npos && it == end)
      break;

    if (it == end || pos < it->position()) {
      size_t begin_pos =
          FindClosingParenthesis(definition, pos + kMacro.size()) + 1;
      size_t end_pos = definition.find(';', begin_pos);
      result.push_back(
          trim_copy(definition.substr(begin_pos, end_pos - begin_pos)));
      pos = end_pos + 1;
      while (it != end && it->position() < pos)
        ++it;
    } else {
      result.push_back(it->str(1));
      ++it;
    }
  }

  return std::move(result);
}

} // namespace dsmap::utils
