#include <chrono>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

#include "helpers.h"
#include "subset_sum.h"

constexpr int MAX_ITERATIONS = 1000;
constexpr double COOLING_RATE = 0.99;
constexpr double MIN_TEMP = 1.0;

int main(int argc, char* argv[]) {
  auto [file, target] =
      parse_args<std::string, int>(argc, argv, "<file> <target>");

  solve("Simulated annealing", file, target,
        [&](const std::vector<int>& set, int target) {
          std::vector<bool> current_mask = generate_random_solution_mask(set);
          int current_loss = loss(get_subset(set, current_mask), target);
          std::vector<bool> best_mask = current_mask;
          int best_loss = current_loss;

          int iterations = 0;
          double temperature = 1000.0;
          double cooling_rate = COOLING_RATE;
          double min_temp = MIN_TEMP;

          for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
            // Decrease temperature
            temperature *= cooling_rate;
            if (temperature < min_temp)
              break;

            // Generate a neighbour
            auto new_mask = generate_near_neighbour_mask(current_mask);
            int new_loss = loss(get_subset(set, new_mask), target);

            // Acceptance probability based on current_loss
            double acceptance_prob =
                std::exp((current_loss - new_loss) / temperature);

            if (new_loss < current_loss ||
                get_random_double(0.0, 1.0) < acceptance_prob) {
              current_mask = new_mask;
              current_loss = new_loss;

              if (new_loss < best_loss) {
                best_mask = new_mask;
                best_loss = new_loss;
              }
            }

            iterations++;
          }

          SubsetSumResult result{
              .best_subset = get_subset(set, best_mask),
              .iterations = iterations,
          };

          return result;
        });
}
