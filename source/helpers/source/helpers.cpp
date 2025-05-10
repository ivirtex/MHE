#include "helpers.h"

#include <fstream>
#include <random>

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

int get_random_int(int min, int max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min, max);

  return dis(gen);
}

double get_random_double(double min, double max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(min, max);

  return dis(gen);
}
