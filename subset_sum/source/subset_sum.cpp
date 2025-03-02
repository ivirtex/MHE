#include "subset_sum.h"

#include <iostream>

int loss(const std::vector<int>& subset, int target) {
  int sum = 0;
  for (int i : subset) {
    sum += i;
  }

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
  // Modify the mask to generate a near neighbour.
  std::vector<bool> new_mask = set_mask;

  // Find a random index to flip.
  int flip_index = rand() % set.size();
  new_mask[flip_index] = !new_mask[flip_index];

  return get_subset(set, new_mask);
}

std::vector<bool> generate_random_solution_mask(const std::vector<int>& set) {
  std::vector<bool> mask(set.size());

  for (int i = 0; i < set.size(); ++i) {
    mask[i] = rand() % 2;
  }

  return mask;
}
