#include <chrono>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

#include "helpers.h"
#include "subset_sum.h"

constexpr int MAX_ITERATIONS = 100000;

int main(int argc, char* argv[]) {
  auto [file, target, max_tabu_size] =
      parse_args<std::string, int, std::optional<int>>(
          argc, argv, "<file> <target> <max_tabu_size>");

  auto setFile = read_file(file);

  std::vector<int> set;
  for (const auto& line : setFile) {
    set.push_back(std::stoi(line));
  }

  // Measure time
  auto start = std::chrono::high_resolution_clock::now();

  // Tabu search metaheuristic
  std::vector<std::vector<bool>> tabu_mask_list;
  size_t max_tabu_list_size = 0;
  bool use_max_tabu_size = false;

  if (max_tabu_size.has_value()) {
    max_tabu_list_size = max_tabu_size.value();
    use_max_tabu_size = true;
  }

  auto best_mask = generate_random_solution_mask(set);
  auto best_loss = loss(get_subset(set, best_mask), target);

  auto current_mask = best_mask;
  for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
    auto neighbor_mask = generate_near_neighbour_mask(current_mask);
    auto curr_loss = loss(get_subset(set, neighbor_mask), target);

    // Check if the neighbor is in the tabu list
    bool in_tabu_list = std::ranges::find(tabu_mask_list, neighbor_mask) !=
                        tabu_mask_list.end();

    if (!in_tabu_list || curr_loss < best_loss) {
      current_mask = neighbor_mask;

      if (curr_loss < best_loss) {
        best_loss = curr_loss;
        best_mask = neighbor_mask;
      }

      tabu_mask_list.push_back(neighbor_mask);

      if (use_max_tabu_size) {
        if (tabu_mask_list.size() > max_tabu_list_size) {
          tabu_mask_list.erase(tabu_mask_list.begin());
        }
      }
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::print("Tabu search results:\n");

  std::print("Time taken: {} ms\n", duration.count());
  std::print("Iterations: {}\n", MAX_ITERATIONS);

  auto best_subset = get_subset(set, best_mask);
  std::print("Best subset: {} (size: {})\n", best_subset, best_subset.size());
  std::print("Best loss: {}\n", best_loss);
  std::print("Final value: {}\n",
             std::accumulate(best_subset.begin(), best_subset.end(), 0));
  std::print("Target: {}\n", target);
}
