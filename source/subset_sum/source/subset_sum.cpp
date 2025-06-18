#include "subset_sum.h"

#include <iostream>
#include <numeric>
#include <random>

#include "helpers.h"

int loss(const std::vector<int>& subset, int target) {
  int sum = std::reduce(subset.begin(), subset.end(), 0);

  return std::abs(sum - target);
}

double fitness(const std::vector<int>& subset, int target) {
  int sum = std::reduce(subset.begin(), subset.end(), 0);

  return 1.0 / (1 + std::abs(sum - target));
}

std::vector<int> get_subset(const std::vector<int>& set,
                            const std::vector<bool>& set_mask) {
  std::vector<int> subset;

  for (int i = 0; i < set.size(); ++i) {
    if (set_mask[i]) {
      subset.push_back(set[i]);
    }
  }

  return subset;
}

std::vector<int> generate_near_neighbour(const std::vector<int>& set,
                                         const std::vector<bool>& set_mask) {
  auto new_mask = generate_near_neighbour_mask(set_mask);

  return get_subset(set, new_mask);
}

std::vector<bool> generate_near_neighbour_mask(
    const std::vector<bool>& set_mask) {
  std::vector<bool> new_mask = set_mask;

  // Find a random index to flip.
  int flip_index = get_random_int(0, set_mask.size() - 1);
  new_mask[flip_index] = !new_mask[flip_index];

  return new_mask;
}

std::vector<std::vector<bool>> generate_near_neighbour_masks(
    const std::vector<bool>& set_mask) {
  std::vector<std::vector<bool>> masks;

  for (int i = 0; i < set_mask.size(); ++i) {
    auto new_mask = set_mask;
    new_mask[i] = !new_mask[i];
    masks.push_back(new_mask);
  }

  return masks;
}

std::vector<bool> generate_random_solution_mask(const std::vector<int>& set) {
  std::vector<bool> mask(set.size());

  for (int i = 0; i < set.size(); ++i) {
    mask[i] = get_random_int(0, 1) % 2;
  }

  return mask;
}

void solve(const std::string& algoritm_name,
           const std::string& file,
           int target,
           const std::function<SubsetSumResult(const std::vector<int>& set,
                                               int target)>& algoritm) {
  auto setFile = read_file(file);
  std::vector<int> set;
  for (const auto& line : setFile) {
    set.push_back(std::stoi(line));
  }

  // Measure time
  auto start = std::chrono::high_resolution_clock::now();
  SubsetSumResult result = algoritm(set, target);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed = end - start;
  int final_value =
      std::accumulate(result.best_subset.begin(), result.best_subset.end(), 0);
  int loss_value = loss(result.best_subset, target);

  std::cout << "{\n";
  std::cout << "  \"algorithm\": \"" << algoritm_name << "\",\n";
  std::cout << "  \"time_ms\": " << (elapsed.count() * 1000) << ",\n";
  std::cout << "  \"iterations\": " << result.iterations << ",\n";
  std::cout << "  \"best_subset\": [";
  for (size_t i = 0; i < result.best_subset.size(); ++i) {
    std::cout << result.best_subset[i];
    if (i + 1 < result.best_subset.size())
      std::cout << ", ";
  }
  std::cout << "],\n";
  std::cout << "  \"fitness_history\": [";
  for (size_t i = 0; i < result.fitness_history.size(); ++i) {
    std::cout << result.fitness_history[i];
    if (i + 1 < result.fitness_history.size())
      std::cout << ", ";
  }
  std::cout << "],\n";
  std::cout << "  \"subset_size\": " << result.best_subset.size() << ",\n";
  std::cout << "  \"final_value\": " << final_value << ",\n";
  std::cout << "  \"target\": " << target << ",\n";
  std::cout << "  \"loss\": " << loss_value << "\n";
  std::cout << "}\n";
}
