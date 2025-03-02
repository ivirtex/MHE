#pragma once

#include <filesystem>
#include <optional>
#include <print>
#include <string_view>
#include <tuple>
#include <vector>

template <typename T>
T convert_type(const std::string& str) {
  if (str.empty()) {
    return T{};
  }

  if constexpr (std::is_same_v<T, int>) {
    return std::stoi(str);
  } else if constexpr (std::is_same_v<T, double>) {
    return std::stod(str);
  } else if constexpr (std::is_same_v<T, std::string>) {
    return str;
  }
}

/// \brief Parses the command line arguments.
template <typename... Args>
std::tuple<Args...> parse_args(int argc,
                               char* argv[],
                               std::string_view usage_message) {
  constexpr size_t expected_args_num = sizeof...(Args);
  if (argc - 1 < expected_args_num) {
    std::print("Usage: {} {}", argv[0], usage_message);
    std::exit(1);
  }

  std::tuple<Args...> result;

  auto assign = [&](auto&... args) {
    int i = 1;

    ((args = convert_type<Args>(argv[i++])), ...);
  };

  std::apply(assign, result);

  return result;
}

std::vector<std::string> read_file(const std::filesystem::path& file_path);
