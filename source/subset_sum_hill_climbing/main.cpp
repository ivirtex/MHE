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

          SubsetSumResult result{
              .best_subset = get_subset(set, mask),
              .iterations = 1,  // Hill climbing is a single iteration process
          };

          return result;
        });
}
