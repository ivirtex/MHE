#include <chrono>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

#include "helpers.h"
#include "subset_sum.h"

constexpr int MAX_ITERATIONS = 100000;

int main(int argc, char* argv[]) {
  auto [file, target] =
      parse_args<std::string, int>(argc, argv, "<file> <target>");

  auto setFile = read_file(file);

  std::vector<int> set;
  for (const auto& line : setFile) {
    set.push_back(std::stoi(line));
  }

  // Measure time
  auto start = std::chrono::high_resolution_clock::now();

  // Tabu search metaheuristic

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::print("GA search results:\n");

  std::print("Time taken: {} ms\n", duration.count());

  // auto best_subset = get_subset(set, best_mask);
  // std::print("Best subset: {} (size: {})\n", best_subset,
  // best_subset.size()); std::print("Best loss: {}\n", best_loss);
  // std::print("Final value: {}\n",
  //            std::accumulate(best_subset.begin(), best_subset.end(), 0));
  // std::print("Target: {}\n", target);
}
