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

  // Simulated annealing metaheuristic
  std::vector<bool> best_mask;
  int best_loss = target;
  int max_iterations = 100;
  double temperature = 1000.0;
  double cooling_rate = 0.99;
  double min_temperature = 1.0;

  // Generate a random mask
  auto mask = generate_random_solution_mask(set);

  // Simulated annealing
  for (int iteration = 0; iteration < max_iterations; ++iteration) {
    auto neighbor_mask = generate_near_neighbour_mask(mask);
    auto curr_loss = loss(get_subset(set, mask), target);
    auto neighbor_loss = loss(get_subset(set, neighbor_mask), target);

    if (std::exp((curr_loss - neighbor_loss) / temperature) >
        get_random_double(0, 1)) {
      mask = neighbor_mask;
      if (neighbor_loss < best_loss) {
        best_loss = neighbor_loss;
        best_mask = mask;
      }
    }

    // Cool down the temperature
    temperature *= cooling_rate;
    if (temperature < min_temperature) {
      break;
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::print("Time taken: {} ms\n", duration.count());
  std::print("Iterations: {}\n", max_iterations);

  auto best_subset = get_subset(set, best_mask);
  std::print("Best subset: {} (size: {})\n", best_subset, best_subset.size());
  std::print("Best loss: {}\n", best_loss);
  std::print("Final value: {}\n",
             std::accumulate(best_subset.begin(), best_subset.end(), 0));
  std::print("Target: {}\n", target);
}
