#include <chrono>
#include <cmath>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

#include "helpers.h"
#include "subset_sum.h"

constexpr int MAX_ITERATIONS = 1000;

double T_linear(int i) {
  return 1.0 / (i + 1.0);
}

double T_logarithmic(int i) {
  return 1.0 / std::log(i + 2.0);
}

int main(int argc, char* argv[]) {
  auto [file, target, temp_fn] = parse_args<std::string, int, std::string>(
      argc, argv, "<file> <target> <temp_fn: linear/logarithmic>");

  std::function<double(int)> T;
  if (temp_fn == "linear") {
    T = T_linear;
  } else if (temp_fn == "logarithmic") {
    T = T_logarithmic;
  } else {
    std::print("Invalid temperature function: {}\n", temp_fn);
    return 1;
  }

  solve("Simulated annealing", file, target,
        [&](const std::vector<int>& set, int target) {
          std::vector<double> fitness_history;

          std::vector<bool> current_mask = generate_random_solution_mask(set);
          int current_loss = loss(get_subset(set, current_mask), target);

          int iterations = 0;
          for (; iterations < MAX_ITERATIONS; ++iterations) {
            // Generate a neighbour
            auto new_mask = generate_near_neighbour_mask(current_mask);
            int new_loss = loss(get_subset(set, new_mask), target);

            // Acceptance probability based on current_loss
            double acceptance_prob =
                std::exp((current_loss - new_loss) / T(iterations));

            if (new_loss < current_loss ||
                acceptance_prob > get_random_double(0.0, 1.0)) {
              current_mask = new_mask;
              current_loss = new_loss;
            }

            fitness_history.push_back(
                fitness(get_subset(set, current_mask), target));
          }

          SubsetSumResult result{
              .best_subset = get_subset(set, current_mask),
              .fitness_history = fitness_history,
              .iterations = iterations,
          };

          return result;
        });
}
