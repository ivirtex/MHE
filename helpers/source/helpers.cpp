#include "helpers.h"

#include <fstream>

std::vector<std::string> read_file(const std::filesystem::path& file_path) {
  std::vector<std::string> lines;
  std::ifstream file(file_path);

  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: " + file_path.string());
  }

  std::string line;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }

  return lines;
}
