#include <chrono>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

#include "helpers.h"
#include "subset_sum.h"

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
  std::vector<std::vector<bool>> tabu_mask_list;
  auto mask = generate_random_solution_mask(set);
  int best_loss = std::numeric_limits<int>::max();
  int max_iterations = 500;

  // Tabu search
  for (int iteration = 0; iteration < max_iterations; ++iteration) {
    auto neighbor_mask = generate_near_neighbour_mask(mask);
    auto curr_loss = loss(get_subset(set, neighbor_mask), target);

    // Check if the neighbor is in the tabu list
    if (std::ranges::find(tabu_mask_list, neighbor_mask) ==
        tabu_mask_list.end()) {
      if (curr_loss < best_loss) {
        best_loss = curr_loss;
        mask = neighbor_mask;
        tabu_mask_list.push_back(neighbor_mask);
      }
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::print("Tabu search results:\n");

  std::print("Time taken: {} ms\n", duration.count());
  std::print("Iterations: {}\n", max_iterations);

  auto best_subset = get_subset(set, mask);
  std::print("Best subset: {} (size: {})\n", best_subset, best_subset.size());
  std::print("Best loss: {}\n", best_loss);
  std::print("Final value: {}\n",
             std::accumulate(best_subset.begin(), best_subset.end(), 0));
  std::print("Target: {}\n", target);
}
