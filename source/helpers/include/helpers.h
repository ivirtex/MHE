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

template <typename T>
struct is_optional : std::false_type {};

template <typename T>
struct is_optional<std::optional<T>> : std::true_type {};

template <typename T>
constexpr bool is_optional_v = is_optional<T>::value;

/// \brief Parses the command line arguments, supporting std::optional
/// arguments.
template <typename... Args>
std::tuple<Args...> parse_args(int argc,
                               char* argv[],
                               std::string_view usage_message) {
  constexpr int total_args_num = sizeof...(Args);

  // Count required (non-optional) arguments
  constexpr size_t required_args_num = ([]<typename... Ts>(std::tuple<Ts...>*) {
    return ((is_optional_v<Ts> ? 0 : 1) + ...);
  })(static_cast<std::tuple<Args...>*>(nullptr));

  if (argc - 1 < static_cast<int>(required_args_num) ||
      argc - 1 > static_cast<int>(total_args_num)) {
    std::print("Usage: {} {}", argv[0], usage_message);
    std::exit(1);
  }

  std::tuple<Args...> result;

  auto assign = [&](auto&... args) {
    int i = 1;
    (
        [&] {
          using ArgT = std::decay_t<decltype(args)>;
          if constexpr (is_optional_v<ArgT>) {
            using InnerT = typename ArgT::value_type;
            if (i < argc) {
              args = convert_type<InnerT>(argv[i++]);
            } else {
              args = std::nullopt;
            }
          } else {
            args = convert_type<ArgT>(argv[i++]);
          }
        }(),
        ...);
  };

  std::apply(assign, result);

  return result;
}

std::vector<std::string> read_file(const std::filesystem::path& file_path);

int get_random_int(int min, int max);
double get_random_double(double min, double max);
