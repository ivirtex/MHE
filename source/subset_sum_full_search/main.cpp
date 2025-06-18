#include <chrono>
#include <numeric>
#include <print>
#include <vector>

#include "helpers.h"
#include "subset_sum.h"

int main(int argc, char* argv[]) {
  auto [file, target] =
      parse_args<std::string, int>(argc, argv, "<file> <target>");

  solve("Full search", file, target,
        [&](const std::vector<int>& set, int target) {
          std::vector<double> fitness_history;

          std::vector<bool> best_mask;
          int best_loss = target;

          // Generate all possible masks
          std::vector<std::vector<bool>> masks;

          // Generate all masks (2^set_size)
          uint64_t all_combinations = 1ULL << set.size();

          for (uint64_t i = 0; i < all_combinations; ++i) {
            std::vector<bool> mask(set.size());
            for (int j = 0; j < set.size(); ++j) {
              mask[j] = (i & (1ULL << j)) != 0;
            }
            masks.push_back(mask);
          }

          // Evaluate all masks
          for (const auto& mask : masks) {
            auto subset = get_subset(set, mask);
            auto curr_loss = loss(subset, target);

            if (curr_loss < best_loss) {
              best_loss = curr_loss;
              best_mask = mask;

              fitness_history.push_back(fitness(subset, target));
            }
          }

          SubsetSumResult result{
              .best_subset = get_subset(set, best_mask),
              .fitness_history = fitness_history,
              .iterations = static_cast<int>(masks.size()),
          };

          return result;
        });
}
