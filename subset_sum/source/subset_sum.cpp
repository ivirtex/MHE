#include "subset_sum.h"

#include <iostream>
#include <random>

#include "helpers.h"

int loss(const std::vector<int>& subset, int target) {
  int sum = std::accumulate(subset.begin(), subset.end(), 0);

  return std::abs(sum - target);
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

std::vector<bool> generate_random_solution_mask(const std::vector<int>& set) {
  std::vector<bool> mask(set.size());

  for (int i = 0; i < set.size(); ++i) {
    mask[i] = rand() % 2;
  }

  return mask;
}
