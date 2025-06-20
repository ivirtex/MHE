#include <chrono>
#include <numeric>
#include <print>
#include <vector>

#include "helpers.h"
#include "subset_sum.h"

int main(int argc, char* argv[]) {
  auto [file, target] =
      parse_args<std::string, int>(argc, argv, "<file> <target>");

  solve("Hill climbing", file, target,
        [&](const std::vector<int>& set, int target) {
          std::vector<double> fitness_history;

          int best_loss = std::numeric_limits<int>::max();
          auto mask = generate_random_solution_mask(set);

          bool improved = true;

          while (improved) {
            improved = false;

            auto neighbor_masks = generate_near_neighbour_masks(mask);
            std::vector<bool> best_neighbor_mask;
            int best_neighbor_loss = std::numeric_limits<int>::max();

            for (const auto& neighbor_mask : neighbor_masks) {
              auto curr_loss = loss(get_subset(set, neighbor_mask), target);

              if (curr_loss < best_neighbor_loss) {
                best_neighbor_loss = curr_loss;
                best_neighbor_mask = neighbor_mask;
              }
            }

            if (best_neighbor_loss < best_loss) {
              best_loss = best_neighbor_loss;
              mask = best_neighbor_mask;
              improved = true;
            }

            fitness_history.push_back(fitness(get_subset(set, mask), target));
          }

          SubsetSumResult result{
              .best_subset = get_subset(set, mask),
              .fitness_history = fitness_history,
              .iterations = 1,  // Hill climbing is a single iteration process
          };

          return result;
        });
}
