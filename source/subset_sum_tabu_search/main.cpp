#include <chrono>
#include <numeric>
#include <print>
#include <ranges>
#include <string>
#include <vector>

#include "helpers.h"
#include "subset_sum.h"

constexpr int MAX_ITERATIONS = 1000;

int main(int argc, char* argv[]) {
  auto [set_file, target, max_tabu_size] =
      parse_args<std::string, int, std::optional<int>>(
          argc, argv, "<file> <target> <max_tabu_size>");

  solve(
      "Tabu search", set_file, target,
      [&](const std::vector<int>& set, int target) {
        std::vector<double> fitness_history;

        std::vector<std::vector<bool>> tabu_mask_list;
        size_t max_tabu_list_size = 0;
        bool use_max_tabu_size = false;

        if (max_tabu_size.has_value()) {
          max_tabu_list_size = max_tabu_size.value();
          use_max_tabu_size = true;
        }

        auto best_mask = generate_random_solution_mask(set);
        auto current_mask = best_mask;
        auto best_candidate_mask = best_mask;

        tabu_mask_list.push_back(best_mask);

        for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
          auto neighbour_masks = generate_near_neighbour_masks(current_mask);
          int best_neighbour_loss = std::numeric_limits<int>::max();

          for (const auto& neighbour_mask : neighbour_masks) {
            bool is_in_tabu =
                std::ranges::contains(tabu_mask_list, neighbour_mask);

            if (!is_in_tabu && loss(get_subset(set, neighbour_mask), target) <
                                   best_neighbour_loss) {
              best_candidate_mask = neighbour_mask;
              best_neighbour_loss =
                  loss(get_subset(set, neighbour_mask), target);
            }
          }

          if (best_neighbour_loss == std::numeric_limits<int>::max()) {
            break;
          }

          current_mask = best_candidate_mask;
          if (best_neighbour_loss < loss(get_subset(set, best_mask), target)) {
            best_mask = best_candidate_mask;
          }

          tabu_mask_list.push_back(best_candidate_mask);
          if (use_max_tabu_size && tabu_mask_list.size() > max_tabu_list_size) {
            tabu_mask_list.erase(tabu_mask_list.begin());
          }

          fitness_history.push_back(
              fitness(get_subset(set, best_mask), target));
        }

        SubsetSumResult result{
            .best_subset = get_subset(set, best_mask),
            .fitness_history = fitness_history,
            .iterations = MAX_ITERATIONS,
        };

        return result;
      });
}
