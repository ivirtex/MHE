#include <numeric>
#include <print>

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

  // Full search metaheuristic
  std::vector<bool> best_mask;
  int best_loss = target;

  // Generate all possible masks
  std::vector<std::vector<bool>> masks;

  for (int i = 0; i < (1 << set.size()); ++i) {
    std::vector<bool> mask;
    for (int j = 0; j < set.size(); ++j) {
      mask.push_back(i & (1 << j));
    }
    masks.push_back(mask);
  }

  // Print all masks
  for (const auto& mask : masks) {
    std::print("{}\n", mask);
  }

  // Evaluate all masks
  for (const auto& mask : masks) {
    auto subset = get_subset(set, mask);
    auto curr_loss = loss(subset, target);

    if (curr_loss < best_loss) {
      best_loss = curr_loss;
      best_mask = mask;
    }
  }

  auto best_subset = get_subset(set, best_mask);
  std::print("Best subset: {}\n", best_subset);
  std::print("Best loss: {}\n", best_loss);
  std::print("Final value: {}\n",
             std::accumulate(best_subset.begin(), best_subset.end(), 0));
  std::print("Target: {}\n", target);

  return 0;
}
