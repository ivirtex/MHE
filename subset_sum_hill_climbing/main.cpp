#include <chrono>
#include <numeric>
#include <print>
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

  // Hill climbing
  int best_loss = std::numeric_limits<int>::max();
  auto mask = generate_random_solution_mask(set);

  bool improved = true;

  while (improved) {
    improved = false;

    auto neighbor_mask = generate_near_neighbour_mask(mask);
    auto curr_loss = loss(get_subset(set, neighbor_mask), target);

    if (curr_loss < best_loss) {
      best_loss = curr_loss;
      mask = neighbor_mask;
      improved = true;
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::print("Hill climbing results:\n");

  std::print("Time taken: {} ms\n", duration.count());

  auto best_subset = get_subset(set, mask);
  std::print("Best subset: {} (size: {})\n", best_subset, best_subset.size());
  std::print("Best loss: {}\n", best_loss);
  std::print("Final value: {}\n",
             std::accumulate(best_subset.begin(), best_subset.end(), 0));
  std::print("Target: {}\n", target);
}
